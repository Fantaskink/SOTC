# Build System Internals

This document explains how the SOTC decompilation turns the original binaries into a matching rebuild. It covers configuration, splitting, rule generation, compilation, linking, and verification.

---

## 1. High-level pipeline

```
iso/<file>                     original binary (ELF or XFF)
        │
        ▼
config/<file>/<file>.yaml      splat split description
        │
        ▼
configure.py -t <name>
        │
        ├── splat.split()      → asm/, assets/, <file>.ld, symbol tables
        ├── short-loop fix     → rewrite a few branch sites to raw opcodes
        └── build_stuff()      → <file>.ninja
                │
                ▼
            ninja -f <file>.ninja
                │
                ├── as / cc    → build/<file>/**/*.o
                ├── ld         → build/<file>/<file>.elf
                ├── objcopy    → build/<file>/<file>   (+ fix_xff.py if XFF)
                └── sha1sum -c checksum-<file>.sha1
```

`make` is only a thin wrapper. It reads `config/modules.txt`, maps each module name to its basename, and invokes `configure.py` + `ninja` per module. All real work lives in `configure.py` and the generated Ninja files.

---

## 2. Module list (`config/modules.txt`)

```
# name      file          format  options
loader      SCPS_150.97   elf     align=sdk:.text:0x4 align=data/section:.data:0x4
kernel      KERNEL.XFF    xff
```

| Field | Used by | Meaning |
|-------|---------|---------|
| `name` | `-t`, `make <name>`, CMake target | Short handle for the module |
| `file` | paths under `iso/`, `config/`, `build/`, checksums | On-disk basename |
| `format` | `configure.py` | `elf` → final flat binary; `xff` → relocatable link + `tools/fix_xff.py` |
| `align=<dir>:<section>:<alignment>` | `build_stuff()` | Objects under `asm/<file>/<dir>/` get `--section-align` when passed through `elf_patcher.py` |

`load_modules()` parses this file into a list of `Module` named tuples. Everything else (makefile targets, Ninja filenames, output paths) is derived from those fields. Adding a binary is “new line here + YAML + checksum”, not scattered hardcoding.

### 2.1 Adding a module

Source subfolders under a module need nothing: the manifest lists every object.

A new module (a new binary from the ISO) needs:

1. `config/<NAME>/<NAME>.yaml` and its symbol files, as for the existing modules.
2. One line in `config/modules.txt`: `<target name> <NAME> <elf|xff>`, plus `align=<dir>:<section>:<alignment>`
   entries if some of its asm needs a section-alignment override (see the comments in that file).
3. `checksum-<NAME>.sha1` containing the original file's sha1 and `build/<NAME>/<NAME>`.
4. The game file in `iso/<NAME>`.

`configure.py -t <target name>`, `make <target name>` and CMake pick the module up from that one line. Anything
more specific than `elf`/`xff` and `align=` still has to be written in `configure.py`.


---

## 3. Configure step (`configure.py`)

Entry point:

```bash
./configure.py [-c] [-t name ...] [-r] [--emit-cmake PATH] [--emit-objdiff PATH]
```

### 3.1 Clean (`-c`)

For each selected module, `clean(basename)` removes:

- `.splache`
- `asm/<basename>/`, `assets/<basename>/`, `build/<basename>/`
- the custom `regnames.specs` file if present

### 3.2 Split (splat)

For each module:

1. Resolve `config/<basename>/<basename>.yaml`.
2. Optionally (`-r` and XFF only) run `parse_xff_relocs` to refresh relocation data.
3. Call `splat.scripts.split.split(...)`, which writes:
   - `asm/<basename>/` — disassembled functions and data
   - `assets/<basename>/` — extracted binary blobs
   - `<basename>.ld` — linker script
   - `config/<basename>/*_undefined_*_auto.txt` — auto-generated symbol lists

splat’s `LinkerEntry` list is the contract between the split and the Ninja generator: each entry is one object that must be built, with its segment type, source paths, and output object path.

### 3.3 Short-loop NOP workaround

ee-gcc 2.96 can emit an extra NOP around certain short loops. For a fixed set of functions (`PROBLEMATIC_FUNCS`), `replace_instructions_with_opcodes()` rewrites the disassembled branch lines so the raw original opcode bytes are assembled instead of a re-encoded instruction. That keeps those functions matching without changing the compiler.

### 3.4 Ninja generation (`build_stuff`)

`build_stuff(basename, linker_entries, is_xff, split, section_aligns)` opens `<basename>.ninja` and:

1. Declares rules (`as`, `cc`, `ld`, `elf`, `sha1sum`) — see §4.
2. Walks every `LinkerEntry` and emits a build edge for its object.
3. Emits the link → objcopy → sha1sum chain for the final binary.

Optional side outputs:

- `--emit-cmake PATH` — one `decomp_object(...)` line per object (same metadata the Ninja rules use).
- `--emit-objdiff PATH` — project file listing `build/**/*.o` vs `expected/**/*.o` for objdiff.

---

## 4. Ninja rules

Cross prefix: `mips-linux-gnu-` (system binutils).  
C compiler: `tools/cc/ee-gcc2.96/bin/ee-gcc` (period compiler).

### 4.1 Assemble (`as`)

**ELF modules (loader):**

```text
cpp <includes> $in | mips-linux-gnu-as -no-pad-sections -EL -march=5900 -mabi=eabi -Iinclude -o $out
&& python3 tools/elf_patcher.py $out gas $override
```

`$override` is empty, or `--section-align <section>:<alignment>` when the object path matches an `align=` entry from `modules.txt`. The first matching directory wins.

**XFF modules (kernel):**

Same assembler flags, but the source is also run through `iconv -f=UTF-8 -t=EUC-JP` (no `elf_patcher` step in the current XFF rule).

### 4.2 Compile C (`cc`)

```text
ee-gcc -c <includes> $optlevel -g2 $regnames $in -o $out
&& mips-linux-gnu-strip $out -N dummy-symbol-name
```

- `$optlevel` comes from the splat YAML (4th field on the segment); default `-O2`.
- `$regnames` is `--specs=regnames.specs` when `named_regs_for_c_funcs` is enabled in the YAML.
- XFF units additionally pass `-G0`.
- `strip -N dummy-symbol-name` drops a placeholder symbol so the object layout matches the original.

A custom `regnames.specs` (written by `create_custom_specs()`) can preprocess assembly that is fed through the C compiler path.

### 4.3 Link (`ld`)

```text
mips-linux-gnu-ld -EL \
  -T config/<file>/<file>_undefined_syms_auto.txt \
  -T config/<file>/<file>_undefined_funcs_auto.txt \
  -T config/<file>/undefined_syms.txt \
  -Map $mapfile \
  [-r] \                    # only for XFF (relocatable)
  -T $in -o $out            # $in is the .ld script
```

- ELF: fully linked → `build/<file>/<file>.elf`
- XFF: relocatable (`-r`) → same path; relocations are fixed up later

Implicit inputs: every `.o` produced by the earlier rules.

### 4.4 Final binary (`elf` rule)

| Format | Command |
|--------|---------|
| ELF | `objcopy $in $out -O binary` |
| XFF | `objcopy $in $out -O binary && python3 tools/fix_xff.py $out` |

Output: `build/<file>/<file>` (no extension).

### 4.5 Verify (`sha1sum`)

```text
sha1sum -c checksum-<file>.sha1 && touch $out
```

`$out` is `build/<file>/<file>.ok`. The checksum file points at the final binary path. A matching build leaves a `.ok` stamp; a mismatch fails the Ninja graph.

---

## 5. Segment type → rule mapping

Inside `build_stuff`, each splat segment type selects a rule:

| Segment type | Rule | Notes |
|--------------|------|--------|
| `asm`, `data` | `as` | May get section-align override |
| `c` | `cc` | optlevel + optional regnames |
| `cpp` | `cpp` | (rule name only; same idea as C) |
| `databin`, `rodatabin`, `textbin` | `as` | Binary blobs included as assembly |
| linker-only (type starts with `.`) | skipped | Handled by the `.ld` script |

Unsupported types abort configure with an error.

---

## 6. Assembler register names

The project's assembly uses names like `$t4`. GNU as development snapshots after the 2.45 release apply
n32-style register names to `-mabi=eabi`, where `$t4`-`$t7` do not exist and `$t0`-`$t3` silently mean
different registers, so such an assembler either fails or assembles the wrong code.

`DECOMP_NUMERIC_REGS=AUTO` probes the assembler once at configure time. If it behaves that way, the build pipes
each assembly file through `tools/regs_numeric.pl`, which rewrites register names to numbers (string literals
are left alone) before assembling. Numeric names mean the same on every version. With an assembler that accepts
the names (2.42 and the released 2.45 do), nothing is rewritten. `ON` always rewrites; `OFF` never does
and stops with an error if the assembler needs it. The legacy `make` build does not have this mode.


## 7. Makefile role

```make
MODULES := $(shell awk '...config/modules.txt')
# for each name:file pair:
#   name: file.ninja
#           ninja -f file.ninja
#   file.ninja:
#           ./configure.py -c -t name
```

- `make` / `make all` builds every module name.
- `make loader` / `make kernel` builds one module.
- `make clean` deletes `asm/`, `assets/`, `build/`, and `*.ninja`.
- QoL targets (`status`, `progress`, `objdiff`, `compdb`, `check`) call the helpers under `tools/`; they do not change the core graph.

The makefile never hardcodes binary names. If `modules.txt` gains a row, `make` and `make help` pick it up automatically.

---

## 8. Directory layout (build-related)

```
config/
  modules.txt                 module table
  <file>/
    <file>.yaml               splat config
    undefined_syms.txt        manual undefined symbols
    <file>_undefined_*_auto.txt   splat-generated

iso/<file>                    input binary (not in git)

build/<file>/
  **/*.o                      objects
  <file>.elf                  linked ELF (or relocatable ELF for XFF)
  <file>                      final binary
  <file>.map                  link map

<file>.ld                     linker script (splat)
<file>.ninja                  generated build file
checksum-<file>.sha1          golden hash
```

---

## 9. End-to-end example (loader)

```bash
./configure.py -t loader
# 1. load_modules() → Module(name=loader, basename=SCPS_150.97, is_xff=False, section_aligns=...)
# 2. splat splits iso/SCPS_150.97 using config/SCPS_150.97/SCPS_150.97.yaml
# 3. short-loop fix on PROBLEMATIC_FUNCS under asm/SCPS_150.97/nonmatchings/
# 4. build_stuff writes SCPS_150.97.ninja
#      - as/cc rules for every LinkerEntry
#      - section-align overrides for asm/.../sdk and asm/.../data/section
#      - ld → objcopy → sha1sum chain

ninja -f SCPS_150.97.ninja
# builds objects, links build/SCPS_150.97/SCPS_150.97.elf,
# objcopies to build/SCPS_150.97/SCPS_150.97,
# checks checksum-SCPS_150.97.sha1 → touches .ok
```

Kernel is the same path with `is_xff=True`: link with `-r`, then `tools/fix_xff.py` after objcopy.

---

## 10. Extension points

| Goal | Where to change |
|------|------------------|
| New binary | Add a row to `config/modules.txt` + YAML + checksum |
| Per-directory section alignment | `align=` options on that row |
| New segment type | `build_stuff` type dispatch + possibly a new Ninja rule |
| Compiler flags for one C unit | 4th field on the splat YAML segment (`-O0`, `-O2`, …) |
| Named registers in C | `named_regs_for_c_funcs` in the YAML options |
| Short-loop exceptions | `PROBLEMATIC_FUNCS` set in `configure.py` |
| CMake / objdiff export | `--emit-cmake` / `--emit-objdiff` |

---

## 11. Related tools (invoked by the graph)

| Tool | When | Role |
|------|------|------|
| `tools/elf_patcher.py` | After `as` (ELF modules) | Apply `--section-align` so section headers match the original |
| `tools/fix_xff.py` | After `objcopy` (XFF) | Rewrite XFF headers / relocations for a matching file |
| `tools/parse_xff_relocs` | configure `-r` | Rescan XFF relocations into splat inputs |
| `scripts/setup_compiler.sh` | once, setup | Install ee-gcc 2.96 under `tools/cc/` |

Helper scripts that sit *outside* the Ninja graph (`tools/status.py`, `tools/match.py`) read build outputs for status and single-unit work; they do not define how the binary is produced.
