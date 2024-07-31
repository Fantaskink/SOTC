import re
import glob
import tqdm
import functools


@functools.lru_cache(maxsize=1024)
def get_symbol_address(symbol: str):
    with open("symbol_addrs.txt", mode="r") as fh:
        for line in fh:
            if line.startswith(f"{symbol} "):
                addr = int(line.split("=")[1].split(";")[0].strip(), 16)
                # print(f"found symbol {symbol}: {hex(addr)}")
                return addr

    assert False, f"{symbol} not found"


def main(gp_value: int):
    if gp_value <= 0:
        return

    asm_files = glob.glob("asm/**/*.s", recursive=True)
    for asm_file in tqdm.tqdm(asm_files, desc="Fixing gp_rel"):
        lines: list[str] = []
        with open(asm_file, mode="r") as fh:
            for line in fh:
                if match := re.match(r"^(.*)%gp_rel\(([^)]+)\)(.*)$", line):
                    # ol = line
                    instr_pre = match.group(1)
                    instr_post = match.group(3)
                    address_str = match.group(2)
                    if address_str.startswith("D_"):
                        address_str = address_str.replace("D_", "0x")
                        res = eval(address_str)
                        address = res
                    else:
                        address = get_symbol_address(address_str)
                    gp_rel = address - gp_value
                    line = f"{instr_pre}{hex(gp_rel)}{instr_post}\n"
                    # print(
                    #     f"FIXING: {ol.strip().split('*/', maxsplit=1)[1].strip():64s} -> {line.strip().split('*/', maxsplit=1)[1].strip()}"
                    # )
                lines.append(line)
        with open(asm_file, mode="w") as wh:
            wh.writelines(lines)


if __name__ == "__main__":
    import sys

    main(int(sys.argv[1], 16))