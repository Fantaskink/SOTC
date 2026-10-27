from __future__ import annotations

import json
from pathlib import Path
from typing import Iterable, List


def units_for_basename(basename: str) -> List[dict]:
    units = []
    src_root = Path("src") / basename
    if not src_root.is_dir():
        return units
    for c_path in sorted(src_root.rglob("*.c")):
        # build/<basename>/src/<rel>.c.o  — matches this project's object paths
        rel = c_path.relative_to("src")  # e.g. SCPS_150.97/foo/bar.c
        obj = Path("build") / basename / "src" / rel
        obj = obj.with_suffix(obj.suffix + ".o")  # .c -> .c.o
        obj_s = obj.as_posix()
        units.append({
            "name": f"{basename}/{c_path.name}",
            "target_path": obj_s,
            "base_path": obj_s,
            "metadata": {"source_path": c_path.as_posix()},
        })
    return units


def write_objdiff_config(path: str | Path, basenames: Iterable[str]) -> None:
    units: List[dict] = []
    for b in basenames:
        units.extend(units_for_basename(b))

    # Prefer make so multi-module objects resolve via the makefile
    config = {
        "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
        "custom_make": "make",
        "custom_args": [],
        "build_target": False,
        "build_base": True,
        "watch_patterns": ["*.c", "*.h", "*.s", "*.inc", "*.py", "*.yaml"],
        "ignore_patterns": ["build/**/*", "asm/**/*"],
        "units": units,
    }
    path = Path(path)
    path.write_text(json.dumps(config, indent=2) + "\n", encoding="utf-8")


if __name__ == "__main__":
    import argparse
    p = argparse.ArgumentParser(description="Write objdiff.json")
    p.add_argument("output", nargs="?", default="objdiff.json")
    p.add_argument(
        "-b", "--basename", action="append", dest="basenames",
        help="Module basename (repeatable). Default: every dir under src/",
    )
    args = p.parse_args()
    bases = args.basenames
    if not bases:
        bases = [d.name for d in Path("src").iterdir() if d.is_dir()]
    write_objdiff_config(args.output, bases)