import os
import re

rodata_syms = set()

for root, dirs, files in os.walk("asm/data"):
    for file in files:
        if file.endswith(".rodata.s"):
            with open(os.path.join(root, file)) as f:
                data = f.read()
            for match in re.finditer("(glabel )(D_([0-9A-F]+))", data):
                rodata_syms.add(match.group(2))

for root, dirs, files in os.walk("asm"):
    for file in files:
        if "blob" in file:
            print("Scanning " + file)  
            with open(os.path.join(root, file)) as f:
                data = f.read()

            for sym in rodata_syms:
                num = data.count(sym)
                if num > 2:
                    pass