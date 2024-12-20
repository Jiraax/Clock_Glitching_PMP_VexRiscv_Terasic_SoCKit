#!/usr/bin/env python3

#
# This file is part of LiteX.
#
# Copyright (c) 2020-2022 Florent Kermarrec <florent@enjoy-digital.fr>
# SPDX-License-Identifier: BSD-2-Clause

import os
import sys
import argparse

from litex.build.tools import replace_in_file

def main():
    parser = argparse.ArgumentParser(description="LiteX Bare Metal glitch-pmp App.")
    parser.add_argument("--build-path",                      help="Target's build path (ex build/board_name).", required=True)
    parser.add_argument("--with-cxx",   action="store_true", help="Enable CXX support.")
    parser.add_argument("--mem",        default="rom",  help="Memory Region where code will be loaded/executed.")
    args = parser.parse_args()

    os.system("rm -rf glitch-pmp/")
    os.system("rm glitch-pmp.bin")
    os.system("rm glitch-pmp.elf.map")
    os.system("rm glitch-pmp.fbi")

    # Create glitch-pmp directory
    os.makedirs("glitch-pmp", exist_ok=True)

    # Copy contents to glitch-pmp directory
    os.system(f"cp {os.path.abspath(os.path.dirname(__file__))}/* glitch-pmp")
    os.system("chmod -R u+w glitch-pmp") # Nix specific: Allow linker script to be modified.

    # Update memory region.
    replace_in_file("glitch-pmp/linker_rom.ld", "rom", args.mem)

    # Compile glitch-pmp
    build_path = args.build_path if os.path.isabs(args.build_path) else os.path.join("..", args.build_path)
    os.system(f"export BUILD_DIR={build_path} && {'export WITH_CXX=1 &&' if args.with_cxx else ''} cd glitch-pmp && make")

    # Copy glitch-pmp.bin
    os.system("cp glitch-pmp/glitch-pmp.bin ./")

    # Prepare flash boot image.
    python3 = sys.executable or "python3" # Nix specific: Reuse current Python executable if available.
    os.system(f"{python3} -m litex.soc.software.crcfbigen glitch-pmp.bin -o glitch-pmp.fbi --fbi --little") # FIXME: Endianness.

if __name__ == "__main__":
    main()

