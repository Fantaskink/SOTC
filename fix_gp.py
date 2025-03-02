import re
import glob
import tqdm


def main():
    inst_pattern = r"(\/\* .* \*\/  .*\([^) ]+\) \/\* gp_rel: .*\*\/)"
    asm_files = glob.glob("asm/**/*.s", recursive=True)
    for asm_file in tqdm.tqdm(asm_files, desc="Fixing gp_rel"):
        lines: list[str] = []
        syms = set()
        with open(asm_file, mode="r") as fh:
            lines.append(fh.read())

        syms.update(re.findall(r"\(([^) ]+).*\) \/\* gp_rel: .*\*\/", lines[0]))
        lines[0] = re.sub(inst_pattern, ".set at\n    \\1\n    .set noat", lines[0])

        if syms:
            with open(asm_file, mode="w") as wh:
                wh.writelines(lines)


if __name__ == "__main__":
    main()
