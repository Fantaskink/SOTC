import re
import glob
import tqdm


def main():
    asm_files = glob.glob("asm/**/*.s", recursive=True)
    for asm_file in tqdm.tqdm(asm_files, desc="Fixing gp_rel"):
        lines: list[str] = []
        syms = set()
        with open(asm_file, mode="r") as fh:
            lines.append(fh.read())

        syms.update(re.findall(r"\/\* gp_rel: \(([^)]+)\).*", lines[0]))
        
        if syms:
            lines.insert(0, "/* Symbols accessed via $gp register */\n")
            lines.insert(1, "\n")
            for s in syms:
                lines.insert(1, f".extern {s}, 1\n")

        with open(asm_file, mode="w") as wh:
            wh.writelines(lines)


if __name__ == "__main__":
    main()