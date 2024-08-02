#include "common.h"
// XXX: WARNING: This is (very) old and shouldn't be used for new code. Use fl_xffType.h instead!

// Relocated by RelocateElfInfoHeader_sub_1004b8

struct xffHdr_t
{
    unsigned char ident[4]; // 00  "xff2" / "xff\0"
    unsigned int unk04;     // 04   0   type, machine ? - never read
    // File can contain multiple XFF headers for Tab7 (relocation stuff)
    unsigned int nextHdr; // 08   0   Pointer to another XFF header with Tab7 within file XXXNOT:version?
    // Number of special sections = type+1; if type=0 => numOfSpecialSect=2
    // used by strHash sub_1B4498
    // if !=0 => typeProc = type+1 ; else typeProc = type(=0)+2
    // special entries in the beginning of symTab??
    //  if ((SymTabNrEnt - (typeProc + tab20NrEnt)) == 0) some entry of communication struct is written
    //  else SymTabNrEnt = (SymTabNrEnt - (typeProc + tab20NrEnt))
    //  those are the first special entries in symTab
    //  This is "numberOfSpecialSect - 1" - this number does not include the zero at the start of SymTab!
    unsigned int SpecSectNr; // 0C   6=exe; 9=script/text? 0xF (iosKernel = ?) //type;
                             //  =0 in file - yes =0 in some files evenwith Zero-section and .rodata.
                             // The SpecSectNr in isysCreateModuleObjExisted_sub_1b4f28 does the folowing:
    /*
    if (SpecSectNr == 0) {
        SpecSectNr = 2; //files with this =0, have two sections: Zero and .rodata/.data (NMO, CLB, ..etc.)
    } else {
        SpecSectNr += 1; //Compensate for the Zero-section
    }


    */

    // 10 writen by DecodeSection_sub_100278
    unsigned int entry; // 10   entry point function addr. Written on load, before that is =0
    //(about the following:) +0x00010000 ??
    unsigned int stackRel; // 14  MAY ALSO BE "NEXT FILE START" Stack relative to start of file addr. (points right after the end of file) XXXX NOT: section header table offset from beginning of file
    // =0 in file
    // 18 written by RelocateElfInfoHeader_sub_1004b8
    unsigned int stackAbs; // 18   MAY ALSO BE "NEXT FILE START" Stack absolute addr. (=0 before load) XXXX NOT: absolute section header offset in memory (before loading = 0)

    unsigned int expSymTabNrE; // 1C   size of the following table [words] = [entries]
    // vvvv +0x40010000
    // 20 The unk20Tab should be multiplied by 16 and added to symTab
    //  (symTab + unk20EntryValue*16) are local entries for which to compute string hash value??
    // This table might specify whether to hash a name so that it can be linked - otherwise it will remain local.
    // in short - it specifies which funcs are exported (unk20EntValue *16 + symTab).
    // Number of each entry (1 word) in symTab, for the name string of which,
    // hash will be computed for it to be externally exported.
    // Holds Exported and Imported symbols!
    // Only simbols that are exclusively for internal use are not in this table!

    // WARNING: ABOVE IS INCORRECT: THIS IS TABLE OF UNEXPORTED SYMBOLS! Symbols with "sect"field (wrong name?) = 1 are exported, while those with 0 are not!
    // this is more-of-a table of "internally-used symbols"
    // Table (only) of imported symbols.  Symbols with indexes in this table are not visible to external modules (if they originate from this module), and are visible to this module (only).
    // The remaining symbols (from symTab) are visible to external modules (if created by this module), but if among them there is an external function, it will not be visible for this module.
    // In the current implementation, the symbols of this table are those with "sect" == 0.
    // THIS SEEMS TO INCLUDE IMPORTED SYMBOLS (untested, but mostly certain from NMO imported textures).
    // Actually - it is probably only a table of imported symbols...

    unsigned int expSymTab; // 20   growing numbers - 7, 8, 9, 0xC, 0xF, .. some are skipped

    unsigned int symTabNrE; // 24   size of the Symbol table [entries] (1 entry = 4 words = 16 bytes)
                            // The symTabNrE contains both expSymTabNrE and SpecSectNr(with processing: if ( == 0) =2; else +=1) and "normal" symbols, so to get the normal symbols, the above two have to be subtracted from symTabNrE.

    // vvvv +0x40010000
    // handled (also) by RelocateSelfSymbol_sub_1018b8
    unsigned int SymTab; // 28   Symbol table offset (before loading = from start of file), after load = absolute
    // vvvv +0x40010000
    unsigned int SymTabStr; // 2C   Strings used by the entries in the Symbol Table
    // vvvv +0x40010000
    unsigned int SectTab; // 30   Section header table? - "xff2-table" - see below
    // vvvv +0x40010000
    unsigned int symRelTab; // 34   Relative addr. of each entry in symTab based on (offset from the start) its section. some table of strange values - mostly middle numbers
    unsigned int RelTabNrE; // 38   Size in entries of the Relocation Table (contains 7-word structs, each of which points to two tables - foer addr and instr.
    // vvvv +0x40010000
    // 3C Tab7 seems to hold structs which specify how to relocated code
    // Handles (also) by RelocateCode_sub_1016e8
    //
    unsigned int relocTab; // 3C   some table headers ... see "7-entry table"   "Tab7"
    unsigned int SectNrE;  // 40   Number of Sections (entries)  //ssNameSz;  Special Sections Names Table Size [entries] - The zero section included.
    // vvvv +0x40010000
    // AKA xSectStrPnt_t
    unsigned int ssNamePt; // 44   Special Sections Names Table offsets (from the following word)
    // vvvv +0x40010000
    unsigned int ssNameBs; // 48   Base addr. for the special sections' strings (names)

    // from here relative to the start of file offseys start:
    unsigned int entry_Rel;     // 4C  relative offset in code section of extry point function
    unsigned int expSymTab_Rel; // 50
    unsigned int SymTab_Rel;    // 54
    unsigned int SymTabStr_Rel; // 58
    unsigned int SectTab_Rel;   // 5C
    unsigned int symRelTab_Rel; // 60
    unsigned int relocTab_Rel;  // 64
    unsigned int ssNamePt_Rel;  // 68
    unsigned int ssNameBs_Rel;  // 6C
};

// The names folloed by "_Rel" hold offsets of the givven structures & tables, relative to the start of the file.
// The variables' names without "_Rel" (besides variables containing sizes), hold asbsolute addresses in RAM and are written by the ElfInfoRelocator.

/*
in DBGMGR there where absolute addresses apper on loading,
the values from the start of the file are +0x40010000 and some are +0x00010000
*/

// MAY ALSO BE "NEXT FILE START"
// THIS IS THE STACK - Its relative offset
//  points to *after* the loaded file (which is correct)
// section header or ... who knows what...
// Words 0x14 (relative to start of file), 0x18 (absolute - in RAM)
// A linked-list, highly confusing something...
#if 0
struct sh_01 {
   unsigned int u01;   //00   
   unsigned int u01;   //04  
   unsigned int u01;   //08  
   unsigned int u01;   //0C  
   unsigned int u01;   //14  =0
   unsigned int u01;   //18  =0
   unsigned int u01;   //1C  
   unsigned int u01;   //20  =0
   
   //unsigned int thisStructAbsOffs;   //24      ??
};

struct sh_02 {
   unsigned int u01;   //00   
   unsigned int u01;   //04  =3
   unsigned int u01;   //08  
   unsigned int u01;   //0C  =addr  
   unsigned int u01;   //14  =addr as above
   unsigned int u01;   //18  =0
   unsigned int u01;   //1C  =0
   unsigned int u01;   //20  =0
   unsigned int u01;  //24  =0

};
struct sh_03 {
   unsigned int u01;   //00   
   unsigned int u01;   //04  
   unsigned int u01;   //08  
   unsigned int u01;   //0C   
   unsigned int u01;   //14  =0
   unsigned int u01;   //18  =0
   unsigned int u01;   //1C  
   unsigned int u01;   //20  =0
   unsigned int thisStructAbsOffs;  //24  
   
   unsigned int prev_02_StructAbsOffs;  //28  
     
   unsigned int u01;   //2C  
   unsigned int u01;   //30  =6
   unsigned int u01;   //34  
   unsigned int u01;   //38  =0
   
   unsigned int u01;   //3C  func addr ... maybe by chance.. 
   unsigned int u01;   //40  also near to the above func addr .. most likely not
   unsigned int u01;   //44
   unsigned int u01;   //48 func addr ??
   unsigned int u01;   //4C near the above but not func (within)
   unsigned int u01;   //50 =0
   unsigned int u01;   //54    
   unsigned int u01;   //58 some array/struct addr
   unsigned int u01;   //5C    
  // then continue 0 / 1 / 2...
};
#endif
/*
It is some format...
The only certain thing is that the starting addr of a struct
is written at its last element, and that structs can be nested -
- the starting addr of one struct can appear after the end
(starting addr word) of another (nested) struct..
This looks a lot like a stack...
*/

// IMPORTED!!!!
// ExpSymTab  //unk20 - some table with small growing numbers (some are skipped)
// its size is the word before its addr in the XFF header
struct xExpSymEnt_t
{
    unsigned int remapNr;
};

// struct ExpSymEnt_t xExpSymTab[EXPSYMTAB_SZ];

// Symbol Table
// XFF header/ 0x28 Abs, 0x54 Rel
//  used also by strHash  sub_1B4498
// Now, after much consideration, it appears to beidentical with the one of the ELF.
// The only reason to change values in converter, would be to 'hack' config. such as: global/local, visblity, ...

struct xSymEnt_t
{
    u32 nameOffs; // 00  Name string offset in Symbol Strings Table
    // Function address: beforeLoad=RelativeCodeSection
    // The relative (initial) value address is never read by the parser
    // afterLoad=Absolute_inRAM
    u32 addr; // 04  Symbol address
    u32 size; // 08  Symbol size in bytes
    // Type (= info & 0xF):
    // 00
    // 01
    // 02
    // 03
    // Type >=4 && <0 are not handled by the local file symTab handler
    u8 info;  // 0C  Type & Binding attributes
    u8 unk0D; // 0D
    // 0E Section (0-3) to which it belongs:
    // 00 undefined by this file (resolve by name) - global - offset 0 in RAM
    // 01 local code section
    //...
    //  < 0xFEFF
    // 0xFFF1 copy address from symRelTab directky to absAddr in symTab
    // Abs_Section_offs. = sectTab + (sect * 0x20(=one_sectHdrSize))
    u16 sect; // 0E  Visibility-IS NOT - it is section, to which it belongs
};

// struct xSymEnt_t xSymTab[SYM_TAB_MAX_SZ];

struct xSymRelEnt_t
{ // Table with Relative addreses of entries in symTab
    u32 addr;
};

// struct xSymRelEnt_t xSymRelTab[];

/*
The first several entries are 'specific...'

0  00 00  The very first one is 'undefined symbol entry'
   - where undefined (missing) symbols get mapped to (the "err screen").
1  01 03  The second one is the Code Section (start address).
2  05 03  The third, is the Data Section start address.
3  08 03  String table for use in code.
4  0B 03  Common (shared) data (ex: used by printf in snapShotHigh in DBGMGR)
5  04 03  usually 0
6  0C 03  -||-

Section code (type):
00  NULL
01  code - progbits
02  symTab
03  strTab (for symTab)
04  relA       Pre-set data
05  hash       Data section "normal" (pre-set values ?)
06  dynamic    External data, debug?
07  note
08  nobits     String table for use in code
09  rel
0A  shLib      String table, for use in debug
0B
0C
0D  Also data - floats and pointers to strings
0E  Init array
0F  fIni array
10  pre-init array some table with who knows what - 3words- two at ends are offs and size (?) the middle - addr absolute

For MANAGER.XFF this initial part of the symbol table is very long.


in symtab
00		string offset
04		func addr (absolute)(?)
08		func size
Bytes:
0C 12=external or internal, not exported function
    11=external (or internal, not exported (?)) variable
    10 = External 	not linked at loading - the file that exports it is not yet loaded
   (but can very well exist - examplt - TOOLS loaded by DBGCORE)
0D	? always 0
0E	00=Localy visible (external or internal) symbol
    01=Exported internal(only?) symbol
0F	? always 0
Example values: 00000010 00000011 00000012 00010012.


From http://sco.com/developers/gabi/latest/ch4.symtab.html :
Index 0 both designates the first entry in the table
and serves as the undefined symbol index.
Name 	      Value
STN_UNDEF 	0

Info - Type & Binding attributes:
   #define ELF32_ST_BIND(i)   ((i)>>4)
   #define ELF32_ST_TYPE(i)   ((i)&0xf)
   #define ELF32_ST_INFO(b,t) (((b)<<4)+((t)&0xf))

Symbol Binding:
Name 	      Value
STB_LOCAL 	0
STB_GLOBAL 	1
STB_WEAK 	2
STB_LOOS 	10
STB_HIOS 	12
STB_LOPROC 	13
STB_HIPROC 	15

Symbol Types:
Name 	      Value
STT_NOTYPE 	0    Unspecified
STT_OBJECT 	1    Data object, such as a variable, array, ...
STT_FUNC 	2    Function or other executable code.
STT_SECTION 3    Symbol table entries of this type exist primarily for relocation
and normally have STB_LOCAL binding
STT_FILE 	4    Conventionally, the symbol's name gives the name of the source file
associated with the object file. A file symbol has STB_LOCAL binding, its section index is SHN_ABS, and it precedes the other STB_LOCAL symbols for the file, if it is present.
STT_COMMON 	5    The symbol labels an uninitialized common block:
Symbols with type STT_COMMON label uninitialized common blocks.
In relocatable objects, these symbols are not allocated
and must have the special section index SHN_COMMON (see below).
In shared objects and executables these symbols must be allocated
to some section in the defining object.
In relocatable objects, symbols with type STT_COMMON
are treated just as other symbols with index SHN_COMMON.
If the link-editor allocates space for the SHN_COMMON symbol in an output section
of the object it is producing, it must preserve the type of the output symbol
as STT_COMMON.
When the dynamic linker encounters a reference to a symbol
that resolves to a definition of type STT_COMMON,
it may (but is not required to) change its symbol resolution rules as follows:
instead of binding the reference to the first symbol found with the given name,
the dynamic linker searches for the first symbol with that name with type
other than STT_COMMON. If no such symbol is found, it looks for the STT_COMMON
definition of that name that has the largest size.
STT_TLS 	   6
STT_LOOS 	10
STT_HIOS 	12
STT_LOPROC 	13
STT_HIPROC 	15


#define ELF32_ST_VISIBILITY(o) ((o)&0x3)
Symbol Visibility
Name 	         Value
STV_DEFAULT 	0
STV_INTERNAL 	1
STV_HIDDEN 	   2
STV_PROTECTED 	3
*/

// Symbol Table Names' Strings

// char xSymTabStr[SYM_TAB_STR_MAX];
struct xSymStr_t
{
    int size;  // size of the table in bytes
    char *str; // pointer to the table of strings
};

// Section header table
// 0x30 abs / 0x5C rel

// Relocated by RelocateElfInfoHeader_sub_1004b8
// In the table, the first section is all zero - the undefined symbols section

struct xSectHdr_t
{
    // 00 set by SectionDecoder
    u32 memAddr_Abs; // 00 points to start of section  or =0 if  sect. has no data - NOT ENTIRELY CORRECT - CHECK CONDITION
    // 04 absolute Start = startOfFile + (1C)RelativeSectStart
    // Set by relocator
    u32 fileAddr_Abs; // 04 poins to start of section if real or start of file if sect. has no data - absolute in RAM
    u32 size;         // 08 size in bytes
    u32 align;        // 0C alignment in bytes
    // 10 Type:
    // 0 - 8
    //  1 OR 0x7FFFF420 - magic - same effect
    // 1=progbit
    // 8=nobit
    // else - overlayData
    // 0=null
    // 2=symtab
    u32 type; // 10 type: checked for =8

    // 14 flags:
    // The section will still be copied(moved) (even if not configured to be) if its initial allignemt(0C) is wrong (bad).
    s32 flags; // 14 flags - of =0 the section is used as is in the file, else it is moved.
    // 18 moved flag
    // on Sects with sz=0, this is written 0 by the parser
    // 0 not copied/written
    // 1 copied only to fix allignment (?)
    // 2 moved to a new area (according to set type, flags, addr)
    s32 moved; // 18  1=not copied(or copied only to fix allignment)
    // 1C This offset (usually =0) is addred to the start of file in RAM
    // to get the value (start of section in RAM) to write to field 04
    s32 addr_Rel; // 1C Relative to start of file offset
};

struct xSectStrPnt_t
{
    s32 offs;
};

struct xSectStr_t
{
    int size;  // size of the table in bytes
    char *str; // pointer to the table of strings
};

// relocTab //Tab7 0x3C - Relocation tables
// Relocated by RelocateElfInfoHeader_sub_1004b8
// Contents handled by ResolveRelocation_sub_1000d0
// Specifies the types of reloaction for different elements of the file
// The absolute addresses are set by the header relocator
// according to the relative values.
struct xRelocEnt_t
{ // Relocation Entry - holds addresses of the two (addr & instr) tables for relocating contents of a given section.
    // 00 type
    // 4
    // 9
    u32 type;          // 00 usualy 9  or 4 (only 9, 4 are supported) - 9=rel<section>  4=rela<section> - contains addEnd - the instr/data table for them is different
    u32 nrEnt;         // 04 Number of entries in this table.
    u32 sect;          // 08 section to which this table of reloc. belongs to
    u32 addrTyIx;      // 0C
    u32 instrTyIx;     // 10
    u32 addrTyIx_Rel;  // 14 relative of 0c
    u32 instrTyIx_Rel; // 18 relative of 10
};

// two RelocTabs usualy exist for each of sections: 1=text(code), 5=data(vars), 8=rodata(const)
// In practice my code shouldn't need any relocation in the data and rodata sections.
// It is unknown why there are two different tables for each..

// addrTyIx:
// pointer to reloc. table with:
// 00 address where to do reloc. (where to change instr/data) (relative to code start)
// 04 Relocation type (1byte)
// 05 Index of target in symTab (3bytes)
struct xRelocAddrEnt_t
{
    u32 addr;
    u32 tyIx; // type, index
};

struct xRelocInstrEnt_t
{
    u32 instr;
    u32 tyIx;
};

// There are usually two of each 'type' relocHeadeTabs:
//'Types' are:
//  for text (code) section
//  for data section
//  for rodata section
// the first table has all relocations pointing to the 'normal' symbols in symtab
// while the second - to the special symbols- example:
/*
0	text, normalSymbols
1	data, normalSymbols
2	rodata, normalSymbols
3	text, SpecialSymbols
4	data, SpecialSymbols
5	rodata, SpecialSymbols

Entries of tables pointing to special symbols only use the base address of that section - the entry itself contains both the data to place at the relocPoint and the address offset into the section.


*/

// The index of the RelocInstrTab is not used ...(always?) - the offset used in the addrTab is used in the instrTab too.

// Relocation type 0-6:
// 0 no reloc.
// 1 unsupported
// 2 = (instr + addr)
// 3 unsupported
// 4 =  ((addr / 4) & 0x03FFFFFF) + instr
// 5 used for multiple-instruction relocation (lui <addrHigh>  addiu <addLow>)
// Checks all consecutive relocations with type=5
// and the last one should be type=6 (for the low16)
//  lastInstr = ((lastType=6_entry *8) + instrTyIxBase)
// TODO....
//  low16 = (((....  )+1) >>1(arithm)) & 0xFFFF
//  high16 = 0xFFFF0000 & firstInstr
//  = (high16 | low16)
// 6 = ((instr + addr) & 0xFFFF) | (instr & 0xFFFF0000)
//>=7 unsupported
/*
----------------------
Reallocation table:
00		Reallocation address - always relative to start of code (referenced directly - no multiplication necessary in most cases. (MNAGER has a wierd table.. or is it a different table..)
04		The top 3(?) bytes specify the index of the SymTab this points to. The lower byte specifies the reallocation tyre (for jal = 4).
------------------------
Table with instr. for realloc:
00		Instruction, which will be ORed with other data at reallocation
04		The top 3(?) bytes specify the index of the SymTab this points to. The lower byte specifies the reallocation type (for jal = 4).
For jal, the instr is 0x0C000000.
--------------------------
*/

/*
The folowing example of data file (albeit probably with parts missing - one of the crashing quadrants)
shows how two symbols are imported:

For non-imported symbols, the relocTab index simply stays at 1 :
(reloc is done in section #1 = .rodata, as the only reloc tab is for .rodata:
                               v
 RelTabNr type     NrEnt    Sect     addrAbs  InstrAbs AddrRel  InstrRel
        0        9        6  -->> 1  A9B2260  A9B2290     2260     2290
 )
                 here=1 => number 1 of symTab is the base of the .rodata section.
                  v
  2 00001DDC 00000102  00000000 00000000
  3 00001E80 00000102  00001710 00000000
  4 00001E88 00000102  000019B0 00000000
  5 00001E90 00000102  00001DD0 00000000


While for imported symbols, (to .rodata), the index equals the number of the imported symbol in symTab:
                  v
  0 00001850 00000202  00000000 00000000
  1 000019A0 00000302  00000000 00000000
Symtab:
 nameOff  addr     size     info     unk0D    sect       Imp
 Ix=2 addRel=00000000  >lb_dry_ground2<
        2        0 61124A6C       10       4A        0   1<-both are imported
 Ix=3 addRel=00000000  >mk_pose_wall_mip<
       11        0 C4E20000       10       80        0   1
                                                                      ^-they "exist" in the zero-section, so take them from external file
The address where to do the relocation sets where to place their data(/pointers to data) in the .rodata secton.


 ### explore/i1_sk/nmo/i1_sk_p03_m03_lyr1.nmo :

unk04=00000000 NextHdr=00000000 Entry_Rel=00001E70 Entry=00001E70  ExportendSyms
=2  symRel= B8

    Stack    ExpSym   SymTab   StrTab   SectTab  ReloCTab ShStrPt  ShStrBs
rel     22C0       E8       F0      14C       78       CC       70      140
abs 436851ED  A9B00E8  A9B00F0  A9B014C  A9B0078  A9B00CC  A9B0070  A9B0140
Sections= 2

  memAbs   fileAbs  size     align    type     flags    moved    addrR
 0
        0        0        0        1        8        0        0 696D5F6C
 1 .rodata
  A99ED30  A9B0190     20D0       10        1        0        0      190

 RelocTab NREnt = 1
 RelTabNr type     NrEnt    Sect     addrAbs  InstrAbs AddrRel  InstrRel
        0        9        6        1  A9B2260  A9B2290     2260     2290
 RELOC TAB 0 : ##################
Nr    addr     tyIx      instr    tyIx
  0 00001850 00000202  00000000 00000000
  1 000019A0 00000302  00000000 00000000
  2 00001DDC 00000102  00000000 00000000
  3 00001E80 00000102  00001710 00000000
  4 00001E88 00000102  000019B0 00000000
  5 00001E90 00000102  00001DD0 00000000

 SymTab NrEnt= 5  SpecialSect = 0
 nameOff  addr     size     info     unk0D    sect       Imp
 Ix=0 addRel=00000000  ><
        0        0 C5010000       10        0        0   0
 Ix=1 addRel=00000000  ><
        1        0 61124A64       10       4A        1   0
 Ix=2 addRel=00000000  >lb_dry_ground2<
        2        0 61124A6C       10       4A        0   1
 Ix=3 addRel=00000000  >mk_pose_wall_mip<
       11        0 C4E20000       10       80        0   1
 Ix=4 addRel=00001E70  >i1_sk_p03_m03_lyr1.nmo<
       22     1E70 C5150000       10       28        1   0Press any key to conti
nue . . .




Note: TODO: 2017.06.18: COFF (XCOFF and so-on) formats - the "XCOFF", if the "CO" is removed becomes "XFF", but otherwise the COFF format looks very different from XFF. Still - check taht someday.


*/
