#!./.python3/bin/python3

import argparse
import os
import shutil
import subprocess
import sys
import re

from pathlib import Path
from typing import Dict, List, Set, Union

import ninja_syntax

from tools import parse_xff_relocs

import splat
import splat.scripts.split as split
from splat.segtypes.linker_entry import LinkerEntry

ROOT = Path(__file__).parent
TOOLS_DIR = ROOT / "tools"

COMMON_INCLUDES = "-Iinclude -I include/sdk/common -I include/sdk/ee -I include/sdk -I include/gcc"

COMPILER = "ee-gcc2.96"
GAME_CC_DIR = f"{TOOLS_DIR}/cc/{COMPILER}/bin"
CUSTOM_SPECS_FILE = f"{TOOLS_DIR}/cc/{COMPILER}/lib/regnames.specs"

GAME_COMPILE_CMD = f"{GAME_CC_DIR}/ee-gcc -c {COMMON_INCLUDES} $optlevel -g2 $regnames"

XFF_COMPILE_CMD = f"{GAME_CC_DIR}/ee-gcc -c {COMMON_INCLUDES} $optlevel -G0 -g2 $regnames"

# Custom spec rule that invokes the preprocessor before assembling
# avoids us having to manually pipe each step and just use GCC
# it defines PREPROCESS_ASM so it can be checked in source if need be
CUSTOM_SPECS = """@c:
cc1 -lang-c %{ansi:-std=c89} %(cpp_options) -DPREPROCESS_ASM %(cc1_options) -o %{|!pipe:%g.si} |
 %(trad_capable_cpp) -lang-asm %{v} %I -I./ %{|!pipe:%g.si} -o %{|!pipe:%g.s} |
 as %(asm_options) %{!pipe:%g.s} %A
"""


def create_custom_specs():
    specs_file = Path(CUSTOM_SPECS_FILE)
    if not specs_file.exists():
        with specs_file.open("w") as f:
            f.write(CUSTOM_SPECS)


def exec_shell(command: List[str], stdout=subprocess.PIPE) -> str:
    ret = subprocess.run(command, stdout=stdout, stderr=subprocess.PIPE, text=True)
    return ret.stdout


def clean(target):
    if os.path.exists(".splache"):
        os.remove(".splache")
    if os.path.exists(CUSTOM_SPECS_FILE):
        os.remove(CUSTOM_SPECS_FILE)
    shutil.rmtree(f"asm/{target}", ignore_errors=True)
    shutil.rmtree(f"assets/{target}", ignore_errors=True)
    shutil.rmtree(f"build/{target}", ignore_errors=True)


def write_permuter_settings():
    rel_cc_dir = Path(GAME_CC_DIR).relative_to(ROOT)
    with open("permuter_settings.toml", "w") as f:
        f.write(
            f"""compiler_command = "{rel_cc_dir}/ee-gcc -c {COMMON_INCLUDES} -O2 -g2"
assembler_command = "mips-linux-gnu-as -march=r5900 -Iinclude"
compiler_type = "gcc"

[preserve_macros]

[decompme.compilers]
"{rel_cc_dir}/ee-gcc" = "{COMPILER}"
"""
        )


def cmake_quote(value) -> str:
    return '"' + str(value).replace("\\", "\\\\").replace('"', '\\"').replace("$", "\\$") + '"'


def write_cmake_manifest(path, is_xff, rows):
    """One decomp_object() line per object, same data the ninja rules get."""
    with open(path, "w") as f:
        f.write(f"set(DECOMP_XFF {'ON' if is_xff else 'OFF'})\n")
        f.write(f"set(DECOMP_INCLUDES {cmake_quote(COMMON_INCLUDES)})\n")
        for kind, obj, srcs, variables in rows:
            quoted_srcs = " ".join(cmake_quote(src) for src in srcs)
            line = f"decomp_object({kind} {cmake_quote(obj)} SRC {quoted_srcs}"
            for key in ("optlevel", "regnames", "override"):
                if variables.get(key):
                    line += f" {key.upper()} {cmake_quote(variables[key])}"
            f.write(line + ")\n")


def build_stuff(basename, linker_entries: List[LinkerEntry], is_xff, split, manifest_rows=None):
    built_objects: Set[Path] = set()

    def build(
        object_paths: Union[Path, List[Path]],
        src_paths: List[Path],
        task: str,
        variables: Dict[str, str] = {},
        implicit_outputs: List[str] = [],
    ):
        if not isinstance(object_paths, list):
            object_paths = [object_paths]

        object_strs = [str(obj) for obj in object_paths]

        if manifest_rows is not None:
            for object_str in object_strs:
                manifest_rows.append((task, object_str, [str(p) for p in src_paths], variables))

        for object_path in object_paths:
            if object_path.suffix == ".o":
                built_objects.add(object_path)
            ninja.build(
                outputs=object_strs,
                rule=task,
                inputs=[str(s) for s in src_paths],
                variables=variables,
                implicit_outputs=implicit_outputs,
            )

    ninja = ninja_syntax.Writer(open(str(ROOT / f"{basename}.ninja"), "w"), width=9999)

    # Rules
    cross = "mips-linux-gnu-"
    if not is_xff:
        ld_args = f"-EL -T config/{basename}/{basename}_undefined_syms_auto.txt -T config/{basename}/{basename}_undefined_funcs_auto.txt -T config/{basename}/undefined_syms.txt -Map $mapfile -T $in -o $out"
    else:
        ld_args = f"-EL -T config/{basename}/{basename}_undefined_syms_auto.txt -T config/{basename}/{basename}_undefined_funcs_auto.txt -T config/{basename}/undefined_syms.txt -Map $mapfile -r -T $in -o $out"

    if not is_xff:
        ninja.rule(
            "as",
            description="as $in",
            command=f"cpp {COMMON_INCLUDES} $in | {cross}as -no-pad-sections -EL -march=5900 -mabi=eabi -Iinclude -o $out && python3 tools/elf_patcher.py $out gas $override",
        )
        
        ninja.rule(
            "cc",
            description="cc $in",
            command=f"{GAME_COMPILE_CMD} $in -o $out && {cross}strip $out -N dummy-symbol-name",
        )

    else:
        ninja.rule(
            "as",
            description="as $in",
            command=f"cpp {COMMON_INCLUDES} $in | iconv -f=UTF-8 -t=EUC-JP $in | {cross}as -no-pad-sections -EL -march=5900 -mabi=eabi -Iinclude -o $out",
        )
        
        ninja.rule(
            "cc",
            description="cc $in",
            command=f"{XFF_COMPILE_CMD} $in -o $out && {cross}strip $out -N dummy-symbol-name",
        )

    ninja.rule(
        "ld",
        description="link $out",
        command=f"{cross}ld {ld_args}",
    )

    ninja.rule(
        "sha1sum",
        description="sha1sum $in",
        command="sha1sum -c $in && touch $out",
    )

    if not is_xff:
        ninja.rule(
            "elf",
            description="elf $out",
            command=f"{cross}objcopy $in $out -O binary",
        )
    else:
        ninja.rule(
            "elf",
            description="elf $out",
            command=f"{cross}objcopy $in $out -O binary && python3 tools/fix_xff.py $out",
        )

    for entry in linker_entries:
        seg = entry.segment

        if seg.type[0] == ".":
            continue

        if entry.object_path is None:
            continue

        if entry.object_path.is_relative_to(Path("build/SCPS_150.97/asm/SCPS_150.97/sdk")):
            override = "--section-align .text:0x4"
        elif entry.object_path.is_relative_to(Path("build/SCPS_150.97/asm/SCPS_150.97/data/section")):
            override = "--section-align .data:0x4"
        else:
            override = ""

        if isinstance(seg, splat.segtypes.common.asm.CommonSegAsm) or isinstance(
            seg, splat.segtypes.common.data.CommonSegData
        ):
            build(entry.object_path, entry.src_paths, "as", variables={"override": override})
        elif isinstance(seg, splat.segtypes.common.cpp.CommonSegCpp):
            build(entry.object_path, entry.src_paths, "cpp")
        elif isinstance(seg, splat.segtypes.common.c.CommonSegC):
            if split.config["options"]["named_regs_for_c_funcs"]:
                regnames = "--specs=regnames.specs"
            else:
                regnames = ""

            if len(seg.yaml) < 4:
                opt = "-O2"
            else:
                opt = seg.yaml[3]
            build(entry.object_path, entry.src_paths, "cc", variables={"regnames": regnames, "optlevel": opt})
        elif isinstance(
            seg, splat.segtypes.common.databin.CommonSegDatabin
        ) or isinstance(seg, splat.segtypes.common.rodatabin.CommonSegRodatabin
        ) or isinstance(seg, splat.segtypes.common.textbin.CommonSegTextbin):
            build(entry.object_path, entry.src_paths, "as", variables={"override": override})
        else:
            print(f"ERROR: Unsupported build segment type {seg.type}")
            sys.exit(1)


    elf_path = f"build/{basename}/{basename}"
    pre_elf_path = f"build/{basename}/{basename}.elf"
    ld_path = f"{basename}.ld"
    map_path = f"build/{basename}/{basename}.map"

    ninja.build(
        pre_elf_path,
        "ld",
        ld_path,
        implicit=[str(obj) for obj in built_objects],
        variables={"mapfile": map_path},
    )

    ninja.build(
        elf_path,
        "elf",
        pre_elf_path,
    )

    ninja.build(
        elf_path + ".ok",
        "sha1sum",
        f"checksum-{basename}.sha1",
        implicit=[elf_path],
    )


# Pattern to workaround unintended nops around loops
COMMENT_PART = r"\/\* (.+) ([0-9A-Z]{2})([0-9A-Z]{2})([0-9A-Z]{2})([0-9A-Z]{2}) \*\/"
INSTRUCTION_PART = r"(\b(bne|bnel|beq|beql|bnez|bnezl|beqzl|bgez|bgezl|bgtz|bgtzl|blez|blezl|bltz|bltzl|b)\b.*)"
OPCODE_PATTERN = re.compile(f"{COMMENT_PART}  {INSTRUCTION_PART}")

PROBLEMATIC_FUNCS = set(
    [
        "OutputLinkerScriptFile",
        "LoaderSysResetSystem",
        "func_00105A60",
        "_putString",
        "InitDisp",
    ]
)


def replace_instructions_with_opcodes(asm_folder: Path) -> None:
    nm_folder = ROOT / asm_folder / "nonmatchings"

    for p in nm_folder.rglob("*.s"):
        if p.stem not in PROBLEMATIC_FUNCS:
            continue

        with p.open("r") as file:
            content = file.read()

        if re.search(OPCODE_PATTERN, content):
            # Reference found
            # Embed the opcode, we have to swap byte order for correct endianness
            content = re.sub(
                OPCODE_PATTERN,
                r"/* \1 \2\3\4\5 */  .word      0x\5\4\3\2 /* \6 */",
                content,
            )

            # Write the updated content back to the file
            with p.open("w") as file:
                file.write(content)


def split_binaries(args):
    target_aliases = {
        "loader" : "SCPS_150.97", 
        "kernel" : "KERNEL.XFF",
        }
    
    target_files = []
    
    if not args.target:
        target_files = list(target_aliases.values())
    else:
        for target in args.target:
            if target not in target_aliases.keys():
                print(f"Error: Unrecognized target. Valid targets: {list(target_aliases.keys())}." )
                return
            target_files.append(target_aliases.get(target))

#    ninja = ninja_syntax.Writer(open(str(ROOT / "build.ninja"), "w"), width=9999)
#    ninja.rule(
#        "ninja",
#        description="ninja $in",
#        command=f"ninja -f $in",
#    )

    for target_file in target_files:

        if args.clean:
            clean(target_file)
        
        if args.rescan:
            parse_xff_relocs.main(f"iso/{target_file}")

        yaml_file = f"config/{target_file}/{target_file}.yaml"

        split.main([Path(yaml_file)], modes="all", verbose=False)

        linker_entries = split.linker_writer.entries

        create_custom_specs()

        manifest_rows = [] if args.emit_cmake else None
        is_xff = target_file != "SCPS_150.97"

        build_stuff(target_file, linker_entries, is_xff, split, manifest_rows)

        if args.emit_cmake:
            write_cmake_manifest(args.emit_cmake, is_xff, manifest_rows)
        
#        ninja.build(
#            f"build/{target_file}/{target_file}",
#            "ninja",
#            f"{target_file}.ninja",
#        )
        
        if not args.no_short_loop_workaround:
            replace_instructions_with_opcodes(split.config["options"]["asm_path"])

        


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description="Configure the project")
    parser.add_argument(
        "-c",
        "--clean",
        help="Clean extraction and build artifacts",
        action="store_true",
    )
    parser.add_argument(
        "-csrc",
        "--cleansrc",
        help="Clean the 'src' folder",
        action="store_true",
    )
    parser.add_argument(
        "-noloop",
        "--no-short-loop-workaround",
        help="Do not replace branch instructions with raw opcodes for functions that trigger the short loop bug",
        action="store_true",
    )
    parser.add_argument(
        "-t",
        "--target",
        help="Choose one or more target binaries",
        action="extend",
        nargs="+",
        type=str
    )
    parser.add_argument(
        "-r",
        "--rescan",
        help="Rescans the target XFF file for reallocations",
        action="store_true",
    )
    parser.add_argument(
        "--emit-cmake",
        help="Also write a CMake object manifest to this path (use with a single -t target)",
        metavar="PATH",
        default=None,
    )
    args = parser.parse_args()

    if args.emit_cmake and (not args.target or len(args.target) != 1):
        parser.error("--emit-cmake needs exactly one -t target")

    if args.cleansrc:
        shutil.rmtree("src", ignore_errors=True)

    split_binaries(args)

    write_permuter_settings()

    if not os.path.isfile("compile_commands.json"):
        exec_shell(["ninja", "-t", "compdb"], open("compile_commands.json", "w"))
