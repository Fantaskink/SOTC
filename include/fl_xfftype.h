#include "common.h"

/*
###############
BY WISI
###############
*/

/*
New XFF file type definitions.
The old one in xffType.h is mostly deprecated, but should be kept for reference, but best not be included in new projects.

*/

// Single entries are also called "Tab" so that the naming is more consistent, otherwise things like struct t_xffSymTab would be called struct t_xffSymEnt.

// For resource files
#define XFF_RES_MAGIC_XFF (0x00666678)
#define XFF_SHTEXE_MAGIC_XFF2 (0x32666678)

enum
{
    XFF_FL_TYPE_RESOURCE = 1,
    XFF_FL_TYPE_SHEET,
    XFF_FL_TYPE_EXECUTABLE,
    XFF_FL_TYPE_MAX
};

// Symbol Table
// XFF header/ 0x28 Abs, 0x54 Rel
//  used also by strHash  sub_1B4498
// Now, after much consideration, it appears to be identical with the one of the ELF.
// The only reason to change values in converter, would be to 'hack' config. such as: global/local, visblity, ...

struct t_xffSymEnt
{
    s32 nameOffs;    // 00  Name string offset in Symbol Strings Table - symTabStr.
    void *addr;      // 04  Symbol address - initially relative(never read by parser) and overwritten to absolute
    u32 size;        // 08  Symbol size in bytes
    u8 type : 4;     // 0C 3:0  info = Type & Binding attributes
    u8 bindAttr : 4; // 0C 7:4	 Binding attributes
    u8 unk0D;        // 0D	used at 001B4418 it seems. Seems to be set for imported symbols that (after they) have been found, so that ResolveRelocation_sub_1000d0 is not called for relocs referencing them. It is done for imported relocs only, as the local ones are in a separate tables that get discarded.  Actually it seems to be cleared for all imported symbols for a given file, perhaps to save time by not calling ResolveRelocation() on them. When a new gile is loaded, perhaps it sets this field for all symbols it adds, so that they get relocated again in all XFF that reference those symbols.
    u16 sect;        // 0E	section, to which this symbol belongs
}; // = 0x10

// Function address: beforeLoad=RelativeCodeSection
// The relative (initial) value address is never read by the parser
// afterLoad=Absolute_inRAM

// Type value:
// Type (= info & 0xF):
// 00
// 01
// 02
// 03
// Type >=4 && <0 are not handled by the local file symTab handler

// Section value:
// 0E Section (0-3) to which it belongs:
// 00 undefined by this file (resolve by name) - global - offset 0 in RAM
// 01 local code section
//...
//  < 0xFEFF
// 0xFFF1 copy address from symRelTab directky to absAddr in symTab
// Abs_Section_offs. = sectTab + (sect * 0x20(=one_sectHdrSize))

struct t_xffSymRelEnt
{             // Table with Relative addreses of entries in symTab
    u32 offs; // 00 relative offset to the start of the section the symbol is in.
};

// Array of the indices of imported symbols (from symTab)
struct t_xffImpSymIxs
{
    s32 stIx; // 00	SymTab Index
};
// Array of the indices in SymTab of the symbols of it that should be imported. All other symbols are either internal (created and used only in this file) or exported (also available to external files). Growing numbers - 7, 8, 9, 0xC, 0xF, .. some are skipped

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

/*
//XXX: this seems to have been for internal use by my elf2xff converte and perhas isn't an offical struct.
//Symbol Table Names' Strings

struct t_xffSymStrEnt {
    int	size;	//size of the table in bytes
    char	*str;	//pointer to the table of strings
};
*/

// Section header table
// 0x30 abs / 0x5C rel

// Relocated by RelocateElfInfoHeader_sub_1004b8
// In the table, the first section is all zero - the (zero) undefined symbols section
// Section Header
struct t_xffSectEnt
{
    // 00 set by SectionDecoder
    void *memPt; // 00 This is the actual pointer to the section that should be used when accessing it in RAM. If the section was set to be moved, or it is a "nobits" (.bss) section and does not exist in the XFF file, then this is set to its allocated location in RAM, while filPt below is NULL (AFAIK). memAddr_Abs;       //00 points to start of section  or =0 if sect. has no data - NOT ENTIRELY CORRECT - CHECK CONDITION
    // 04 absolute Start = startOfFile + (1C)RelativeSectStart
    // Set by relocator
    void *filePt; // 04	This is an absolute pointer to the section *in the file* as it is in RAM.  This seems to be the actual absolute section start addr, while the above is unknown.  u32 fileAddr_Abs;    //04 poins to start of section if real or start of file if sect. has no data - absolute in RAM
    int size;     // 08 size in bytes
    int align;    // 0C alignment in bytes  This alignment is always fulfilled on loading (by allocating new memory for the section if the dile alignment is i,nsufficient) but if flags(u14) is set, then the maximum alignment of 0x100 is used.
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
    s32 flags; // 14 flags: 0=UseAlignmentSetting(align) / other=UseMaxAlignment=0x100   - this also seems to be a force-allocate (alloc flag) flag. See DecodeSection() in xffFuncs.c. - of =0 the section is used as is in the file, else it is moved. Nobits section is always allocated, as it is not present in the file.
    // Flags:
    //	0: Alignment setting(align) is used and the loader writes moved = 1;
    //	!=0: Max alignment setting = 0x100bytes is used and the loader writes moved = 2;

    // 18 moved flag
    // on Sects with sz=0, this is written 0 by the parser
    // 0 not copied/written
    // 1 copied only to fix allignment (?)
    // 2 moved to a new area (according to set type, flags, addr)
    int moved; // 18  1=not copied(or copied only to fix allignment)
    // 1C This offset (usually =0) is addred to the start of file in RAM
    // to get the value (start of section in RAM) to write to field 04
    int offs_Rel; // 1C Relative to start of file offset
}; // = 0x20

struct t_xffSectStrOffs
{
    s32 offs;
};

/*XXX: maybe custom local use...  don't remember why I added this
struct xSectStr_t {
    int size;	//size of the table in bytes
    char *str;	//pointer to the table of strings
};
*/

// Relocation Tables:

// two RelocTabs usualy exist for each of sections: 1=text(code), 5=data(vars), 8=rodata(const)
// In practice my code shouldn't need any relocation in the data and rodata sections.
// It is unknown why there are two different tables for each..

// addrTyIx:
// pointer to reloc. table with:
// 00 address where to do reloc. (where to change instr/data) (relative to code start)
// 04 Relocation type (1byte)
// 05 Index of target in symTab (3bytes)
struct t_xffRelocAddrEnt
{
    u32 addr; // Address where to apply the changes
    // u32	tyIx;			//type, index
    u32 relType : 8;  // The type of modification to do.
    u32 tgSymIx : 24; // Index of the target symbol in SymTab (the symbol to which that thing at addr should point to.
};

// intrsuction or actually target relocated data, and not necessarily instruction
struct t_xffRelocInstEnt
{
    u32 inst;
    u32 tyIx;
};

// relocTab //Tab7 0x3C - Relocation tables
// Relocated by RelocateElfInfoHeader_sub_1004b8
// Contents handled by ResolveRelocation_sub_1000d0
// Specifies the types of reloaction for different elements of the file
// The absolute addresses are set by the header relocator
// according to the relative values.
struct t_xffRelocEnt
{ // Relocation Entry - holds addresses of the two (addr & instr) tables for relocating contents of a given section.
    // 00 type
    // 4
    // 9
    u32 type;                       // 00 usualy 9  or 4 (only 9, 4 are supported) - 9=rel<section>  4=rela<section> - contains addEnd - the instr/data table for them is different
    u32 nrEnt;                      // 04 Number of entries in this table.
    u32 sect;                       // 08 section to which this table of reloc. belongs to
    struct t_xffRelocAddrEnt *addr; // 0C
    struct t_xffRelocInstEnt *inst; // 10
    u32 addr_Rel;                   // 14 relative of 0C
    u32 inst_Rel;                   // 18 relative of 10
}; // = 0x1C

// There are usually two of each 'type' relocHeadeTabs:
//'Types' are:
//  for text (code) section
//  for data section
//  for rodata section
// the first table has all relocations pointing to the 'normal' symbols in symtab
// while the second - to the special symbols- example:
/*
0	text, normalSymbols		E#xternal references - that are needed even if this XFF does not get relocated afterwards, but modules it references do get relocated.
1	data, normalSymbols
2	rodata, normalSymbols

3	text, SpecialSymbols		Local references - if this XFF is not relocated after loading, these are discarded to save space.
4	data, SpecialSymbols
5	rodata, SpecialSymbols

Entries of tables pointing to special symbols only use the base address of that section - the entry itself contains both the data to place at the relocPoint and the address offset into the section.



The local references are placed at the very end of the file, so they can be easily discarded.
The address from which to start discarding is calculated by adding the size of a single addr-entry (without the instr-entry) size (0x8) multiplied by the nrE of the first set of tables. The first set of tables are always half the tables, so there must always be two tables for each section (even if only one is needed - the other can simply have 0 entries).




   //This should be the correct way:
(DF = datafields)

    DF_relocTI,
    DF_relocDI,
    DF_relocRI,
    DF_relocTA,  //<- offset base	 - this is added to the size of the following three to get the addr from which to start discarding bu iosFreeParts().
    DF_relocDA,
    DF_relocRA,     //up to here (including this) the data is saved.

    DF_relocTAS, //this and below is discarded after loading.
    DF_relocDAS,
    DF_relocRAS,

   DF_relocTIS,
    DF_relocDIS,
    DF_relocRIS,


//relocation entries in main reloc tab - the extern tabs must be listed first
enum {
   XRELOC_TN = 0,  //the first set of tables is saved, as it references external symbols, which when relocated, the reloc tabls are needed again
   XRELOC_DN,
   XRELOC_RN,

   XRELOC_TS, //this second set is discared, because it contains only internal relocations, not needed after loading this module, because it is not moved after loading.
   XRELOC_DS,
   XRELOC_RS,
};


  //the order is even more messed-up:
  //Each section gets two tables - the table with the -S and the one without it.
  //All the tables with the -S are listed first, BUT in them the Instr table is first, and the addr tab is after it!
  //For the tables without -S, the address table is first and then the instr:
  /*
  Preview GAMECORE.XFF:

  TI,  DI,  RI,   TA,  DA,  RA,
  TAS, DAS, RAS,  TIS, DIS, RIS   Why the complicated order - I have no idea.
  Also notice the order in the Main RelocTab!

 RelocTab NREnt = 6
 RelTabNr type     NrEnt    Sect     addrAbs  InstrAbs AddrRel  InstrRel
        0        9     48E1        1 40188EC8 40164710   178EC8   154710
        1        9        0        5 401AD5D0 40188E18   19D5D0   178E18
        2        9       16        8 401AD5D0 40188E18   19D5D0   178E18
        3        9     49F1        1 401AD680 401D4D90   19D680   1C4D90
        4        9       EE        5 401D2608 401F9D18   1C2608   1E9D18
        5        9      403        8 401D2D78 401FA488   1C2D78   1EA488

  Maybe the relocation info of one of the tabs sets is not used and due to that it is freed after loading...
  - because some sections are never moved.

  maybe one of the sets is only for internal relocations-references,
  so they don't need to be re-calculated after the module is loaded, as it is no-longer moved.


  So only the first set of tables are left by isysLaunchModule_sub_1b5980
  DisposeRelocationElement_sub_101950 does the same (but only the freeing).
  It uses the free func passed to it by a1.

iosFreePartsFunc() here is actually a caller to it with a2 = 0:
void *iosFreePartsFunc(void *mallocPt, void *freeStartAddr) {
    return iosFreePartsFunc(mallocPt, freeStartAddr, 0);
}

void DisposeRelocationElement(struct t_xffEntPntHdr *xffEp, void (*iosFreePartsFunc)(void *mallocPt, void *freeStartAddr)) { //_sub_101950:
    if (xffEp->ident != XFF_SHTEXE_MAGIC_XFF2) return;

    int halfTabsNrE = xffEp->relocTabNrE; //half are for referencing external symbols and the other half for local
    struct t_xffRelocEnt *rt = xffEp->relocTab;
    int externRefNrE = 0;
    for (i=0; i<halfTabsNrE; i++) { //go through the extern tabs
        externRefNrE += rt[i]->nrE;
    }
    struct t_xffRelocAddrEnt *firstExtrRelAddr = rt[0].addr; //DF_relocTA
    void *startOfLocalRelocTabs = (u32)firstExtrRelAddr + (externRefNrE * sizeof(struct t_xffRelocAddrEnt));
    iosFreeParts(xffEp, startOfLocalRelocTabs); //frees everything after the addr startOfLocalRelocTabs

    for (i=halfTabsNrE; i<(halfTabsNrE*2); i++) { //clear the local tabs nrE to 0 as they are missing now.
        rt[i]->nrE = 0;
    }
    return;
}

void *isysDisposeModuleObjRelocationElement(struct t_xffEntPntHdr *xff) {	//_sub_1b6a48
...
is the same as the above, but returns the start address of the freed space after the XFF, and internally calls iosFreePartsFunc().

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

// Number of special sections = type+1; if type=0 => numOfSpecialSect=2
// used by strHash sub_1B4498
// if !=0 => typeProc = type+1 ; else typeProc = type(=0)+2
// special entries in the beginning of symTab??
//  if ((SymTabNrEnt - (typeProc + tab20NrEnt)) == 0) some entry of communication struct is written
//  else SymTabNrEnt = (SymTabNrEnt - (typeProc + tab20NrEnt))
//  those are the first special entries in symTab
//  This is "numberOfSpecialSect - 1" - this number does not include the zero at the start of SymTab!

// specSectNrE:
//  =0 in file - yes =0 in some files evenwith Zero-section and .rodata.
// The SpecSectNr in isysCreateModuleObjExisted_sub_1b4f28 does the folowing:
/*
if (SpecSectNr == 0) {
    SpecSectNr = 2; //files with this =0, have two sections: Zero and .rodata/.data (NMO, CLB, ..etc.)
} else {
    SpecSectNr += 1; //Compensate for the Zero-section
}
*/

// impSymTab:
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

struct t_xffSsNmOffs
{
    int nmOffs; // 00	offset of the name of the indexed Special Section, starting from ssNamesBaseA
}; // = 0x4

// fields succeeded by: 'A' = Absolute = pointer / 'R' = relative = offset (either from the start of the file or start of section.
// No- changing the above to: absolute are clean names, while relative has _Rel added.

// Relocated by RelocateElfInfoHeader_sub_1004b8
// Main XFF header at the start of an XFF file.
struct t_xffEntPntHdr
{
    u32 ident; // 00  "xff2" / "xff\0"
    u32 u04;   // 04   0   type, machine ? - never read
    // File can contain multiple XFF headers for Tab7 (relocation stuff)
    u32 nextXffHdr;  // 08   0   offset? ->? Pointer to another XFF header with Tab7 within file XXXNOT:version?
    s32 specSectNrE; // 0C   6=exe; 9=script/text? 0xF (iosKernel = ?) //type;  read as u8 - 001B44D0
    // 10 writen by DecodeSection_sub_100278
    void *entryPnt; // 10   entry point function addr. Written on load, before that is =0
    //(about the following:) +0x00010000 ??
    u32 stack_Rel;                    // 14 Stack relative to start of file addr. (points right after the end of file) - it shows its size, but maybe if there are multiple XFF headers, it will only show the offset to the next header.
    void *stack;                      // 18   MAY ALSO BE "NEXT FILE START" Stack absolute addr. (=0 before load)
    int impSymIxsNrE;                 // 1C   size of the following table [words] = [entries]
    struct t_xffImpSymIxs *impSymIxs; // 20	Indices of imported symbols.

    int symTabNrE; // 24   size of the Symbol table [entries] (1 entry = 4 words = 16 bytes)
    // The symTabNrE contains both expSymTabNrE and SpecSectNr(with processing: if ( == 0) =2; else +=1) and "normal" symbols, so to get the normal symbols, the above two have to be subtracted from symTabNrE.
    // handled (also) by RelocateSelfSymbol_sub_1018b8
    struct t_xffSymEnt *symTab;        // 28  NOT?: Symbol table offset (before loading = from start of file), after load = absolute
    char *symTabStr;                   // 2C   Strings used by the entries in the Symbol Table
    struct t_xffSectEnt *sectTab;      // 30   Section header table? - "xff2-table" - see below
    struct t_xffSymRelEnt *symRelTab;  // 34   Relative addr. of each entry in symTab based on (offset from the start) its section. Same indexing as SymTab. some table of strange values - mostly middle numbers
    int relocTabNrE;                   // 38   Size in entries of the Relocation Table (contains 7-word structs, each of which points to two tables - for addr and instr.
    struct t_xffRelocEnt *relocTab;    // 3C   Array of per-section Relocation tables.  some table headers ... see "7-entry table"   "Tab7"
    int sectNrE;                       // 40   Number of Sections (entries)  //ssNameSz;  Special Sections Names Table Size [entries] - The zero section included.
    struct t_xffSsNmOffs *ssNamesOffs; // 44   Special Sections Names offsets table (from the following word)   //AKA xSectStrPnt_t
    char *ssNamesBase;                 // 48   Base addr. for the special sections' strings (names). The offset from this of each one is set by the values in array ssNamesOffs.
    // From here the relative, to the start of file, offsets start:
    u32 entryPnt_Rel;    // 4C  Relative offset from the start of a section. In Executable files, the entry point is in the .text section, while in data files, it depends on the fileType (usually .rodada or .data).
    u32 impSymIxs_Rel;   // 50
    u32 symTab_Rel;      // 54
    u32 symTabStr_Rel;   // 58
    u32 sectTab_Rel;     // 5C
    u32 symRelTab_Rel;   // 60
    u32 relocTab_Rel;    // 64
    u32 ssNamesOffs_Rel; // 68
    u32 ssNamesBase_Rel; // 6C
}; // = 0x70

// The names folloed by "_Rel" hold offsets of the givven structures & tables, relative to the start of the file.
// The variables' names without "_Rel" (besides variables containing sizes), hold asbsolute addresses in RAM and are written by the ElfInfoRelocator RelocateElfInfoHeader_sub_1004b8.

/*
in DBGMGR there where absolute addresses apper on loading,
the values from the start of the file are +0x40010000 and some are +0x00010000
*/
