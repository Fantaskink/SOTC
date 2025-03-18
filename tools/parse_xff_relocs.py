# Thank you to joshlory for this file

import struct
import sys
from enum import IntEnum
from pathlib import Path

VRAM = 0x40010000

class RelocType(IntEnum):
    R_MIPS_NONE  = 0
    R_MIPS_16    = 1
    R_MIPS_32    = 2
    R_MIPS_REL32 = 3
    R_MIPS_26    = 4
    R_MIPS_HI16  = 5
    R_MIPS_LO16  = 6

def sign_extend16(value):
    sign_bit = 1 << (16 - 1)
    return (value & (sign_bit - 1)) - (value & sign_bit)

def read_uint32(file):
    return struct.unpack('<I', file.read(4))[0]

def read_uint8(file):
    return struct.unpack('B', file.read(1))[0]

def read_xffEntPntHdr(file):
    file.seek(0)
    hdr = {}
    hdr['ident'] = file.read(4)
    file.seek(0x1C)
    hdr['impSymIxsNrE'] = read_uint32(file)
    hdr['impSymIxs'] = read_uint32(file)
    hdr['symTabNrE'] = read_uint32(file)
    hdr['symTab'] = read_uint32(file)
    hdr['symTabStr'] = read_uint32(file)
    hdr['sectTab'] = read_uint32(file)
    file.seek(0x38)
    hdr['relocTabNrE'] = read_uint32(file)
    hdr['relocTab'] = read_uint32(file)
    hdr['sectNrE'] = read_uint32(file)
    file.seek(0x50)
    hdr['impSymIxs_Rel'] = read_uint32(file)
    hdr['symTab_Rel'] = read_uint32(file)
    hdr['symTabStr_Rel'] = read_uint32(file)
    hdr['sectTab_Rel'] = read_uint32(file)
    file.seek(0x64)
    hdr['relocTab_Rel'] = read_uint32(file)
    hdr['ssNamesOffs_Rel'] = read_uint32(file)
    hdr['ssNamesBase_Rel'] = read_uint32(file)
    return hdr

def read_xffRelocEnt(file, offset):
    file.seek(offset)
    entry = {}
    entry['type'] = read_uint32(file)
    entry['nrEnt'] = read_uint32(file)
    entry['sect'] = read_uint32(file)
    entry['addr'] = read_uint32(file)
    entry['inst'] = read_uint32(file)
    entry['addr_Rel'] = read_uint32(file)
    entry['inst_Rel'] = read_uint32(file)
    return entry

def read_xffRelocAddrEnt(file, offset):
    file.seek(offset)
    addr = read_uint32(file)
    tyIx = read_uint32(file)
    return addr, tyIx & 0xFF, tyIx >> 8

def read_xffRelocInstEnt(file, offset):
    file.seek(offset)
    instr = read_uint32(file)
    unk = read_uint32(file)
    return instr, unk

def read_xffSymEnt(file, offset):
    file.seek(offset)
    entry = {}
    entry['nameOffs'] = read_uint32(file)
    entry['addr'] = read_uint32(file)
    entry['size'] = read_uint32(file)
    info = read_uint8(file)
    entry['type'] = info & 0xF
    entry['bindAttr'] = info >> 4
    entry['unk0D'] = read_uint8(file)
    entry['sect'] = struct.unpack('<H', file.read(2))[0]
    return entry

def read_xffSectEnt(file, offset):
    file.seek(offset)
    entry = {}
    entry['memPt'] = read_uint32(file)
    entry['filePt'] = read_uint32(file)
    entry['size'] = read_uint32(file)
    entry['align'] = read_uint32(file)
    entry['type'] = read_uint32(file)
    entry['flags'] = read_uint32(file)
    entry['moved'] = read_uint32(file)
    entry['offs_Rel'] = read_uint32(file)
    return entry

def read_string(file, offset):
    file.seek(offset)
    result = bytearray()
    while True:
        char = file.read(1)
        if char == b'\0':
            return result.decode('utf-8')
        result.extend(char)

def get_symbol_info(file, sym_table, sym_str_table, sym_idx, section_names):
    sym_offset = sym_table + sym_idx * 0x10
    sym = read_xffSymEnt(file, sym_offset)
    name = read_string(file, sym_str_table + sym['nameOffs'])
    return name if name else section_names[sym['sect']], sym['sect'], sym['addr']

def format_reloc_output(addr, relType, sym_name, text_offs_rel):
    rom_addr = text_offs_rel + addr
    reloc_type = {
        2: "MIPS_32",
        4: "MIPS_26",
        5: "MIPS_HI16",
        6: "MIPS_LO16"
    }.get(relType)

    if relType in [2, 4, 5, 6]:
        return f"rom:0x{rom_addr:X} symbol:{sym_name} reloc:{reloc_type}"
    else:
        return None

def extract_reloc_and_symbols(filename: Path):
    xff_name = filename.stem.lower()
    relocs_output_file = Path("config") / f"{xff_name}.relocs.txt"
    symbols_output_file = Path("config") / f"{xff_name}.symbols.txt"

    with open(filename, 'rb') as file, open(relocs_output_file, 'w') as relocs_out, open(symbols_output_file, 'w') as symbols_out:
        hdr = read_xffEntPntHdr(file)

        if hdr['ident'] != b'xff2':
            print("Not a valid XFF2 file")
            return

#        print(f"Number of relocation tables: {hdr['relocTabNrE']}")
#        print(f"Number of symbols: {hdr['symTabNrE']}")
#        print(f"Number of sections: {hdr['sectNrE']}")

        # Extract section headers
#        print("\nSection Headers:")
#        print("  memAbs   fileAbs  size     align    type     flags    moved    addrR")
        section_names = {}
        section_offsets = {}
        text_offs_rel = None
        for i in range(hdr['sectNrE']):
            sect_offset = hdr['sectTab_Rel'] + i * 0x20
            sect = read_xffSectEnt(file, sect_offset)
            file.seek(hdr['ssNamesOffs_Rel'] + i * 4)
            name_offset = read_uint32(file)
            section_names[i] = read_string(file, hdr['ssNamesBase_Rel'] + name_offset)
            section_offsets[i] = sect['offs_Rel']
#            print(f"{i:2X} {section_names[i]}")
#            print(f"{sect['memPt']:8X} {sect['filePt']:8X} {sect['size']:8X} {sect['align']:8X} {sect['type']:8X} {sect['flags']:8X} {sect['moved']:8X} {sect['offs_Rel']:8X}")

            if section_names[i] == '.text':
                text_offs_rel = sect['offs_Rel']

        # Extract symbols and build symbol table
#        print("\nSymbols:")
        symbol_table = {}
        for i in range(1, hdr['symTabNrE']):
            sym_offset = hdr['symTab_Rel'] + i * 0x10
            sym = read_xffSymEnt(file, sym_offset)
            name = read_string(file, hdr['symTabStr_Rel'] + sym['nameOffs'])
            name = name if name else f"UNKNOWN_{i}"

            # Calculate the actual address by adding the section offset
            actual_address = sym['addr'] + section_offsets.get(sym['sect'], 0)

#            print(f"  Name: {name}")
#            print(f"    Address: 0x{sym['addr']:X} Size: {sym['size']} Type: {sym['type']} Binding: {sym['bindAttr']} Section: {sym['sect']:X}")

            # Skip absolute section symbols as those belong to the main elf
            if sym['sect'] == 0xfff1 and sym['bindAttr'] != 0:
                pass
            
            elif sym['sect'] != 0 and sym['bindAttr'] != 0:
                # Write symbol information to the symbols output file
                symbols_out.write(f"{name} = 0x{(VRAM + actual_address):X};\n")# // size:{sym['size']}\n")

                # Add symbol to the symbol table
                symbol_table[VRAM + actual_address] = name

        # Not all relocs resolve to a symbol, so we make a generic symbol in those instances
        # these generic symbols need to also be in symbol_addrs so we collect them here
        noname_syms = dict()

        # Extract relocation tables
        for i in range(hdr['relocTabNrE']):
            reloc_entry_offset = hdr['relocTab_Rel'] + i * 0x1C
            reloc_entry = read_xffRelocEnt(file, reloc_entry_offset)

#            print(f"\nRelocation Table {i}:")
#            print(f"  Type: {reloc_entry['type']}")
#            print(f"  Number of entries: {reloc_entry['nrEnt']}")
#            print(f"  Section: {reloc_entry['sect']}")

            addr_table_offset = reloc_entry['addr_Rel']
            inst_table_offset = reloc_entry['inst_Rel']
            
            # Some bss relocs have dangling LO16s, so we keep track of the last bss HI16
            # in order to be able to resolve them to one of the generic symbols we made earlier
            # as bss seems to have no symbols at all
            last_bss_hi = 0

            # We also save the last HI16 to pair with LO16s like the MIPS doc says
            last_normal_hi = 0

            for j in range(reloc_entry['nrEnt']):
                addr, relType, tgSymIx = read_xffRelocAddrEnt(file, addr_table_offset + j * 8)
                instr, unk = read_xffRelocInstEnt(file, inst_table_offset + j * 8)
                
                # Offset of this reloc section (mainly for R_MIPS_32 relocs, which aren't in .text)
                sect_offs = section_offsets[reloc_entry["sect"]]

                if relType == RelocType.R_MIPS_NONE:
                    # Do nothing
                    pass
                elif relType == RelocType.R_MIPS_32:
                    sym_name, sym_sect, sym_addr = get_symbol_info(file, hdr['symTab_Rel'], hdr['symTabStr_Rel'], tgSymIx, section_names)
                    _addr = VRAM + section_offsets[sym_sect] + sym_addr + instr
                    sym_name = symbol_table.get(_addr, f"D_{_addr:08X}")
                elif relType == RelocType.R_MIPS_26:
                    sym_name, _, sym_addr = get_symbol_info(file, hdr['symTab_Rel'], hdr['symTabStr_Rel'], tgSymIx, section_names)
                    if tgSymIx == 1:
                        func_offset = (instr & 0x3FFFFFF) * 4
                        func_addr = VRAM + text_offs_rel + func_offset
                        sym_name = symbol_table.get(func_addr, f"func_{func_addr:08X}")
                        _addr = func_addr
                        
                        if func_addr not in symbol_table:
                            noname_syms[_addr] = sym_name
                    if sym_name.startswith("."):
                        print("WRONG MIPS26!")

                elif relType == RelocType.R_MIPS_HI16:
                    _sym_name, sym_sect, sym_addr = get_symbol_info(file, hdr['symTab_Rel'], hdr['symTabStr_Rel'], tgSymIx, section_names)
                    loInstr = search_reloc_lo(file, inst_table_offset, addr_table_offset, j)
                    
                    # hi/lo pairs
                    hi = (instr << 0x10) & 0xFFFF0000
                    lo = sign_extend16(loInstr)

                    _addr = VRAM + sym_addr + hi + lo

                    if _sym_name.startswith("."):
                        last_normal_hi = hi # save hi16 value
                        if _sym_name == ".bss":
                            last_bss_hi = hi # save bss hi16
                            s_off = section_offsets[14] # rodata end, cuz bss has no offset
                        else:
                            s_off = section_offsets[sym_sect]
                        
                        _addr += s_off
                        _addr &= 0xFFFFFFFF
                        sym_name = symbol_table.get(_addr, f"D_{_addr:08X}")
                    else:
                        sym_name = _sym_name
                        # Here I save the symbol instead, taking advantage of Python's dynamic typing
                        # just to save up a variable if we have an actual symbol to attatch to
                        last_normal_hi = sym_name

                    # Save in noname_syms only if the address isn't there already, just to have `func_`
                    # symbols take higher priority
                    if sym_name.startswith("D_") and sym_name not in noname_syms:
                        noname_syms[_addr] = sym_name
                elif relType == RelocType.R_MIPS_LO16:
                    _sym_name, sym_sect, sym_addr = get_symbol_info(file, hdr['symTab_Rel'], hdr['symTabStr_Rel'], tgSymIx, section_names)
                    
                    # if the last hi16 was a string then we are done here
                    if type(last_normal_hi) is str:
                        sym_name = last_normal_hi
                    else:
                        _addr  = (VRAM + sign_extend16(instr) + sym_addr)
                        if _sym_name == ".bss":
                            _addr += last_bss_hi
                            s_off = section_offsets[14] # rodata end, cuz bss has no offset
                        else:
                            _addr += last_normal_hi
                            s_off = section_offsets[sym_sect]
                        
                        _addr += s_off
                        _addr &= 0xFFFFFFFF
                        sym_name = symbol_table.get(_addr, f"D_{_addr:08X}")

                        # Save in noname_syms only if the address isn't there already, just to have `func_`
                        # symbols take higher priority
                        if sym_name.startswith("D_") and sym_name not in noname_syms:
                            noname_syms[_addr] = sym_name
                else:
                    print(f"xff_parser: Unknown reloc type ({relType}) found at 0x{addr:04X}")
                    sys.exit(-1)
                
                formatted_output = format_reloc_output(addr, relType, sym_name, sect_offs)
                if formatted_output:
                    relocs_out.write(f"{formatted_output}\n")
            
        # Save all found noname_syms
        for k,v in noname_syms.items():
            symbols_out.write(f"{v} = 0x{k:X};\n")

#        print(f"\nRelocation information has been written to {relocs_output_file}")
#        print(f"Symbol information has been written to {symbols_output_file}")

def search_reloc_lo(file, inst_table_offset: int, addr_table_offset: int, index: int):
    for i in range(index, 100_000):
        _, relType, _ = read_xffRelocAddrEnt(file, addr_table_offset + i * 8)
        if relType == RelocType.R_MIPS_HI16:
            # Ignore consecutive HI16s
            pass
        elif relType == RelocType.R_MIPS_LO16:
            instr, unk = read_xffRelocInstEnt(file, inst_table_offset + i * 8)
            return instr
        else:
            print(f"xff_parser: Can't find LO16 pair for HI16 ({index})")
            return 0

def main(filename: Path):
    if not filename.exists() and filename.is_file():
        print(f"File not found: {filename}")
        sys.exit(-1)

    if not filename.is_file():
        print(f"Path supplied is a folder: {filename}")
        sys.exit(-1)
    
    extract_reloc_and_symbols(filename)

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python script.py <filename>")
        sys.exit(1)

    main(Path(sys.argv[1]))
