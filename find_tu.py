import re
import glob
import tqdm

def main():
    addresses = {}

    asm_files = glob.glob("asm/**/*.s", recursive=True)
    for asm_file in tqdm.tqdm(asm_files, desc="Finding TUs"):
        if asm_file.endswith("crt0.s"):
            continue

        with open(asm_file, mode="r") as fh:
            for index, line in enumerate(fh):
                # Check for %gp_rel
                if match := re.match(r"^(.*)%gp_rel\(([^)]+)\)(.*)$", line):
                    instr_pre = match.group(1)
                    instr_post = match.group(3)
                    address_str = match.group(2)

                    if addresses.get(address_str) == "hi/lo":
                        print(f"Inconsistent data access detected at line {index} in {asm_file}")
                    else:
                        addresses[address_str] = "gp"
                    
                # Check for %hi or %lo
                elif match := re.match(r"^(.*)%(hi|lo)\(([^)]+)\)(.*)$", line):
                    instr_pre = match.group(1)
                    instr_post = match.group(4)
                    address_str = match.group(3)

                    if addresses.get(address_str) == "gp":
                        print(f"Inconsistent data access detected at line {index} in {asm_file}")
                    else:
                        addresses[address_str] = "hi/lo"

if __name__ == "__main__":
    main()
