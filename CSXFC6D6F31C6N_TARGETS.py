#!/usr/bin/env python3

#
# This file is part of LiteX-Boards.
#
# Copyright (c) 2020-2021 Xuanyu Hu <xuanyu.hu@whu.edu.cn>
# SPDX-License-Identifier: BSD-2-Clause

from migen import *

from litex.gen import *
from litex_boards.platforms import CSXFC6D6F31C6N

from litex.soc.cores.clock import CycloneVPLL
from litex.soc.integration.soc import SoCRegion
from litex.soc.integration.soc_core import *
from litex.soc.integration.builder import *

from litex.soc.cores.led import LedChaser

from litex.build.io import CRG
from litex.soc.cores.gpio import GPIOIn, GPIOTristate, GPIOOut

# CRG ----------------------------------------------------------------------------------------------

class _CRG(LiteXModule):
    def __init__(self, platform, sys_clk_freq):
        self.rst    = Signal()
        self.cd_sys = ClockDomain()

        #Clk / Rst
        clk50 = platform.request("clk50")
        
        self.pll = pll = CycloneVPLL(speedgrade="-C6")
        self.comb += pll.reset.eq(self.rst)
        pll.register_clkin(clk50, 50e6)
        pll.create_clkout(self.cd_sys, sys_clk_freq)
        
# BaseSoC ------------------------------------------------------------------------------------------
class BaseSoC(SoCCore):
    def __init__(self, sys_clk_freq=50e6, toolchain="quartus", with_led_chaser=True, **kwargs):
        platform = CSXFC6D6F31C6N.Platform()
        
        # CRG --------------------------------------------------------------------------------------
        #self.crg = _CRG(platform, sys_clk_freq)
        self.crg = CRG(platform.request(platform.default_clk_name), ~platform.request("nrst_chipwhisperer"))
	         
        # SoCCore ----------------------------------_-----------------------------------------------
        SoCCore.__init__(self, platform, sys_clk_freq, ident="LiteX SoC on Cyclone V SX", **kwargs)

        # Leds -------------------------------------------------------------------------------------
        if with_led_chaser:
            self.leds = LedChaser(
                pads         = platform.request_all("user_led"),
                sys_clk_freq = sys_clk_freq)
                
        
        # GPIOs Glitch Trigger ----------------------------------------------------------------------------
        self.TRIGGER = GPIOOut(platform.request("trigger_chipwhisperer"), 0)
        self.add_csr("TRIGGER")
        
        
# Build --------------------------------------------------------------------------------------------  
def main():
    from litex.build.parser import LiteXArgumentParser
    parser = LiteXArgumentParser(platform=CSXFC6D6F31C6N.Platform, description="LiteX SoC on Cyclone V SX.")
    parser.add_target_argument("--sys-clk-freq", default=50e6, type=float, help="System clock frequency.")
    args = parser.parse_args()

    soc = BaseSoC(
        sys_clk_freq        = args.sys_clk_freq,
        toolchain = args.toolchain,
        **parser.soc_argdict
    )

    builder = Builder(soc, **parser.builder_argdict)
    if args.build:
        builder.build(**parser.toolchain_argdict)

    if args.load:
        prog = soc.platform.create_programmer()
        prog.load_bitstream(builder.get_bitstream_filename(mode="sram"))

if __name__ == "__main__":
    main()
