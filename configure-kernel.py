#!/usr/bin/python3

import argparse
import os
import shutil
import subprocess
import sys
import re

from pathlib import Path
from typing import Dict, List, Set, Union

#from fix_gp import main as fix_gp_main

import ninja_syntax

from tools import parse_xff_relocs

import splat
import splat.scripts.split as split
from splat.segtypes.linker_entry import LinkerEntry

ROOT = Path(__file__).parent
TOOLS_DIR = ROOT / "tools"

XFF_FILE = "iso/KERNEL.XFF"

YAML_FILE = "config/kernel.yaml"
BASENAME = "KERNEL.XFF"
LD_PATH = f"{BASENAME}.ld"
ELF_PATH = f"build/{BASENAME}"
MAP_PATH = f"build/{BASENAME}.map"
PRE_ELF_PATH = f"build/{BASENAME}.elf"

COMMON_INCLUDES = "-Iinclude -I include/sdk/ee -I include/sdk -I include/gcc"

COMPILER = "ee-gcc2.96"
GAME_CC_DIR = f"{TOOLS_DIR}/cc/{COMPILER}/bin"
CUSTOM_SPECS_FILE = f"{TOOLS_DIR}/cc/{COMPILER}/lib/regnames.specs"

GAME_COMPILE_CMD = f"{GAME_CC_DIR}/ee-gcc -c {COMMON_INCLUDES} -O2 -g2 $regnames"

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


def clean():
    if os.path.exists(".splache"):
        os.remove(".splache")
    if os.path.exists(CUSTOM_SPECS_FILE):
        os.remove(CUSTOM_SPECS_FILE)
    shutil.rmtree("asm", ignore_errors=True)
    shutil.rmtree("assets", ignore_errors=True)
    shutil.rmtree("build", ignore_errors=True)


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


def build_stuff(linker_entries: List[LinkerEntry]):
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

    ninja = ninja_syntax.Writer(open(str(ROOT / "build.ninja"), "w"), width=9999)

    # Rules
    cross = "mips-linux-gnu-"
    ld_args = "-EL -T config/kernel_undefined_syms_auto.txt -T config/kernel_undefined_funcs_auto.txt -Map $mapfile -r -T $in -o $out"

    ninja.rule(
        "as",
        description="as $in",
        command=f"cpp {COMMON_INCLUDES} $in | {cross}as -no-pad-sections -EL -march=5900 -mabi=eabi -Iinclude -o $out",
    )

    ninja.rule(
        "cc",
        description="cc $in",
        command=f"{GAME_COMPILE_CMD} $in -o $out && {cross}strip $out -N dummy-symbol-name",
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

        if isinstance(seg, splat.segtypes.common.asm.CommonSegAsm) or isinstance(
            seg, splat.segtypes.common.data.CommonSegData
        ):
            build(entry.object_path, entry.src_paths, "as")
        elif isinstance(seg, splat.segtypes.common.cpp.CommonSegCpp):
            build(entry.object_path, entry.src_paths, "cpp")
        elif isinstance(seg, splat.segtypes.common.c.CommonSegC):
            if split.config["options"]["named_regs_for_c_funcs"]:
                regnames = "--specs=regnames.specs"
            else:
                regnames = ""
            build(entry.object_path, entry.src_paths, "cc", variables={"regnames": regnames})
        elif isinstance(seg, splat.segtypes.common.databin.CommonSegDatabin
            ) or isinstance(seg, splat.segtypes.common.rodatabin.CommonSegRodatabin
            ) or isinstance(seg, splat.segtypes.common.textbin.CommonSegTextbin):
            build(entry.object_path, entry.src_paths, "as")
        else:
            print(f"ERROR: Unsupported build segment type {seg.type}")
            sys.exit(1)

    ninja.build(
        PRE_ELF_PATH,
        "ld",
        LD_PATH,
        implicit=[str(obj) for obj in built_objects],
        variables={"mapfile": MAP_PATH},
    )

    ninja.build(
        ELF_PATH,
        "elf",
        PRE_ELF_PATH,
    )

    ninja.build(
        ELF_PATH + ".ok",
        "sha1sum",
        "checksum-kernel.sha1",
        implicit=[ELF_PATH],
    )


# Pattern to workaround unintended nops around loops
COMMENT_PART = r"\/\* (.+) ([0-9A-Z]{2})([0-9A-Z]{2})([0-9A-Z]{2})([0-9A-Z]{2}) \*\/"
INSTRUCTION_PART = r"(\b(bne|bnel|beq|beql|bnez|bnezl|beqzl|bgez|bgezl|bgtz|bgtzl|blez|blezl|bltz|bltzl|b)\b.*)"
#OPCODE_PATTERN = re.compile(f"{COMMENT_PART}  {INSTRUCTION_PART}")

PROBLEMATIC_FUNCS = set(
    [
        
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
    args = parser.parse_args()

    if args.clean:
        clean()

    if args.cleansrc:
        shutil.rmtree("src", ignore_errors=True)

#    parse_xff_relocs.main(XFF_FILE)

    split.main([YAML_FILE], modes="all", verbose=False)

    linker_entries = split.linker_writer.entries

    create_custom_specs()

    build_stuff(linker_entries)

    write_permuter_settings()

#    if not split.config["options"]["use_gp_rel_macro_nonmatching"]:
#        fix_gp_main()

#    if not args.no_short_loop_workaround:
#        replace_instructions_with_opcodes(split.config["options"]["asm_path"])

    if not os.path.isfile("compile_commands.json"):
        exec_shell(["ninja", "-t", "compdb"], open("compile_commands.json", "w"))
