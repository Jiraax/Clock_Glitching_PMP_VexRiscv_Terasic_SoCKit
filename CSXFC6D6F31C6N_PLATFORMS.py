#
# This file is part of LiteX-Boards.
#
# Copyright (c) 2020-2021 Xuanyu Hu <xuanyu.hu@whu.edu.cn>
# SPDX-License-Identifier: BSD-2-Clause

from litex.build.generic_platform import *
from litex.build.altera import AlteraPlatform
from litex.build.altera.programmer import USBBlaster

# IOs ----------------------------------------------------------------------------------------------

_io = [
    # Clk / Rst
    #("clk50", 0, Pins("AF14"), IOStandard("3.3-V LVTTL")),
    
    ("clk_chipwhisperer", 0, Pins("J3:1"), IOStandard("3.3-V LVTTL")),
    ("trigger_chipwhisperer", 0, Pins("J3:36"), IOStandard("3.3-V LVTTL")),
    ("nrst_chipwhisperer", 0, Pins("J3:40"), IOStandard("3.3-V LVTTL")),
    
    # Leds
    ("user_led",  0, Pins("AF10"), IOStandard("3.3-V LVTTL")),
    ("user_led",  1, Pins("AD10"), IOStandard("3.3-V LVTTL")),
    ("user_led",  2, Pins("AE11"), IOStandard("3.3-V LVTTL")),
    ("user_led",  3, Pins("AD7"), IOStandard("3.3-V LVTTL")),


    # Serial
    ("serial", 0,
        Subsignal("tx", Pins("J3:9")),
        Subsignal("rx", Pins("J3:10")),
        IOStandard("3.3-V LVTTL"),
    ),

]

# Connectors ---------------------------------------------------------------------------------------

_connectors_hsmc_gpio_daughterboard = [
    # PIN    0  1   2   3   4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19  20  21  22  23 24  25 26  27 28 29 30 31 32 33 34 35 36 37 38 39 40
    ("J2",  "-  G15 F14 H15 F15 A13 G13 B13 H14 B11 E13 -   -   C12 F13 B8  B12 C8  C13 A10 D10 A11 D11 B7 D12 C7 E12 A5 D9 -  -  A6 E9 A3 B5 A4 B6 B1 C2 B2 D2"),
    
    ("J2p", "- D1 E1 E11 F11"), # Top to bottom, starting with 57.

    ("J3", "- AB27 F8 AA26 F9 B3 G8 C3 H8 D4 H7 - - E4 J7 E2 K8 E3 K7 E6 J9 E7 J10 C4 J12 D5 G10 C5 J12 - - D6 K12 F6 G11 G7 G12 D7 A8 E8 A9"),
    ("J3p", "- C9 C10 H12 H13"), # Top to bottom, starting with 117.

    ("J4", "- - - AD3 AE1 AD4 AE2 - - AB3 AC1 - - AB4 AC2 - - Y3 AA1 Y4 AA2 - - V3 W1 V4 W2 - - - - T3 U1 T4 R1 - R2 P3 U2 P4 -"),
    ("J4p", "- M3 M4 - H3 H4 J14 AD29 - N1 N2 - J1 J2") # Top to bottom, starting with 169.
]


# PMODS --------------------------------------------------------------------------------------------
    
    
class Platform(AlteraPlatform):
    #default_clk_name   = "clk50"
    default_clk_name   = "clk_chipwhisperer"
    default_clk_period = 1e9/50e6
    
    def __init__(self, toolchain="quartus"):
        AlteraPlatform.__init__(self, "5CSXFC6D6F31C6", _io, connectors=_connectors_hsmc_gpio_daughterboard, toolchain=toolchain)

    def create_programmer(self):
    	return USBBlaster(cable_name="CV SoCKit")
    
    def do_finalize(self, fragment):
        AlteraPlatform.do_finalize(self, fragment)
        #self.add_period_constraint(self.lookup_request("clk50", loose=True), 1e9/50e6)
        self.add_period_constraint(self.lookup_request("clk_chipwhisperer", loose=True), 1e9/50e6)

