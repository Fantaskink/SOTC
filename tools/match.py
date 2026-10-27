#!/usr/bin/env python3
"""Helpers for the single-unit matching loop.

Usage:
  python3 tools/match.py check build/SCPS_150.97/src/SCPS_150.97/padSys.o
  python3 tools/match.py context LoaderSysResetSystem
  python3 tools/match.py next
"""

from __future__ import annotations

import argparse
import os
import subprocess
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent


def run(cmd: list[str], **kw) -> int:
    print("+", " ".join(cmd), flush=True)
    return subprocess.call(cmd, cwd=ROOT, **kw)


def cmd_check(obj: str) -> int:
    """Rebuild one object, then objdiff it against the expected object if possible."""
    obj_path = Path(obj)
    if not obj_path.is_absolute():
        obj_path = ROOT / obj_path

    # Infer the ninja file from the path: build/<basename>/...
    try:
        basename = obj_path.relative_to(ROOT / "build").parts[0]
    except ValueError:
        print(f"Object path must be under build/<module>/: {obj}", file=sys.stderr)
        return 1

    ninja_file = ROOT / f"{basename}.ninja"
    if not ninja_file.is_file():
        print(f"Missing {ninja_file.name}; run: ./configure.py -t <name>", file=sys.stderr)
        return 1

    # Rebuild just this object
    rc = run(["ninja", "-f", str(ninja_file), str(obj_path.relative_to(ROOT))])
    if rc != 0:
        return rc

    expected = ROOT / "expected" / obj_path.relative_to(ROOT / "build")
    if expected.is_file() and shutil_which("objdiff-cli"):
        return run(["objdiff-cli", "diff", str(expected), str(obj_path)])
    if expected.is_file():
        print(f"Expected object exists at {expected}")
        print("Install objdiff-cli for a side-by-side diff: https://github.com/encounter/objdiff")
        return 0

    print("Rebuilt OK. No expected/ object found for comparison.")
    print("Tip: copy matching objects into expected/build-style paths, or use decomp.me.")
    return 0


def cmd_context(name: str) -> int:
    """Locate a nonmatching .s file and print a decomp.me-ready snippet path."""
    hits = list((ROOT / "asm").rglob(f"{name}.s"))
    if not hits:
        # try address-style names
        hits = list((ROOT / "asm").rglob(f"*{name}*.s"))
    if not hits:
        print(f"No assembly found for '{name}' under asm/", file=sys.stderr)
        return 1
    for h in hits:
        print(h)
        print("---")
        print(h.read_text()[:4000])
        if len(h.read_text()) > 4000:
            print("... (truncated)")
        print()
    print("Paste the assembly into https://decomp.me (Shadow of the Colossus preset).")
    return 0


def cmd_next() -> int:
    """Suggest a few nonmatching functions to work on (smallest first)."""
    nm_root = ROOT / "asm"
    candidates: list[tuple[int, Path]] = []
    for p in nm_root.rglob("nonmatchings/**/*.s"):
        try:
            size = p.stat().st_size
        except OSError:
            continue
        candidates.append((size, p))
    candidates.sort(key=lambda x: x[0])
    if not candidates:
        print("No nonmatchings found. Run configure + build first, or everything is matched.")
        return 0
    print("Smallest nonmatching functions (good starting points):\n")
    for size, p in candidates[:15]:
        rel = p.relative_to(ROOT)
        print(f"  {size:6d} B  {rel}")
    print("\nUse:  python3 tools/match.py context <function_name>")
    return 0


def shutil_which(cmd: str) -> str | None:
    from shutil import which
    return which(cmd)


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="cmd", required=True)

    p_check = sub.add_parser("check", help="Rebuild one object and optionally objdiff it")
    p_check.add_argument("obj", help="Path to the .o under build/")

    p_ctx = sub.add_parser("context", help="Print nonmatching assembly for decomp.me")
    p_ctx.add_argument("name", help="Function name or unique substring")

    sub.add_parser("next", help="List small nonmatching functions to work on")

    args = parser.parse_args()
    if args.cmd == "check":
        return cmd_check(args.obj)
    if args.cmd == "context":
        return cmd_context(args.name)
    if args.cmd == "next":
        return cmd_next()
    return 1


if __name__ == "__main__":
    sys.exit(main())
