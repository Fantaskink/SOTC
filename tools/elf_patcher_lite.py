#!/usr/bin/env python3
# Dependency-free equivalent of elf_patcher.py (same CLI, same byte patch).
# Original: SPDX-FileCopyrightText: (c) 2024 AngheloAlf, SPDX-License-Identifier: MIT
# Only touches sh_addralign, so it needs nothing but struct (no spimdisasm import).

import struct
import sys

REALIGN = {
    ".text": 0x8, ".data": 0x8, ".rodata": 0x8,
    ".sdata": 0x8, ".sbss": 0x4, ".bss": 0x10,
}


def main(argv):
    if len(argv) < 3 or argv[2] != "gas":
        sys.exit("usage: elf_patcher_lite.py <elf> gas [--section-align .sec:0xN ...]")
    path = argv[1]

    override = {}
    if len(argv) > 3:
        if argv[3] != "--section-align":
            sys.exit(f"unexpected argument {argv[3]!r}")
        for entry in argv[4:]:
            name, value = entry.split(":")
            override[name] = int(value, 0)

    data = bytearray(open(path, "rb").read())
    if data[:4] != b"\x7fELF" or data[4] != 1:
        sys.exit(f"{path}: not an ELF32 file")
    e = "<" if data[5] == 1 else ">"

    shoff, = struct.unpack_from(e + "I", data, 0x20)
    shnum, shstrndx = struct.unpack_from(e + "HH", data, 0x30)
    strtab_off, = struct.unpack_from(e + "I", data, shoff + shstrndx * 0x28 + 0x10)

    for i in range(shnum):
        sec = shoff + i * 0x28
        name_off, = struct.unpack_from(e + "I", data, sec)
        start = strtab_off + name_off
        name = bytes(data[start:data.index(b"\0", start)]).decode()
        align = override.get(name, REALIGN.get(name))
        if align is not None:
            struct.pack_into(e + "I", data, sec + 0x20, align)

    open(path, "wb").write(data)


main(sys.argv)
