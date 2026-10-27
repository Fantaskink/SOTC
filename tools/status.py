#!/usr/bin/env python3
"""Print a concise build / matching status for every module.

Usage:
  python3 tools/status.py              # SHA-1 + nonmatching counts
  python3 tools/status.py --report build/report.json   # also show objdiff %
"""

from __future__ import annotations

import argparse
import hashlib
import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
MODULES_FILE = ROOT / "config" / "modules.txt"


def load_modules():
    modules = []
    for line in MODULES_FILE.read_text().splitlines():
        fields = line.split("#")[0].split()
        if len(fields) >= 2:
            modules.append((fields[0], fields[1]))
    return modules


def sha1_file(path: Path) -> str | None:
    if not path.is_file():
        return None
    h = hashlib.sha1()
    with path.open("rb") as f:
        for chunk in iter(lambda: f.read(1 << 20), b""):
            h.update(chunk)
    return h.hexdigest()


def expected_sha1(basename: str) -> str | None:
    checksum = ROOT / f"checksum-{basename}.sha1"
    if not checksum.is_file():
        return None
    # format: <hash>  build/<basename>/<basename>
    first = checksum.read_text().split()
    return first[0] if first else None


def count_nonmatchings(basename: str) -> tuple[int, int]:
    """Return (nonmatching_funcs, total_asm_funcs) under asm/<basename>/nonmatchings."""
    nm = ROOT / "asm" / basename / "nonmatchings"
    if not nm.is_dir():
        return 0, 0
    total = 0
    for p in nm.rglob("*.s"):
        total += 1
    return total, total  # all files under nonmatchings are currently unmatched


def load_objdiff_report(path: Path) -> dict | None:
    if not path.is_file():
        return None
    try:
        return json.loads(path.read_text())
    except json.JSONDecodeError:
        return None


def main() -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--report", type=Path, help="objdiff report JSON from 'make progress'")
    args = parser.parse_args()

    modules = load_modules()
    if not modules:
        print("No modules found in config/modules.txt", file=sys.stderr)
        return 1

    report = load_objdiff_report(args.report) if args.report else None
    measures = (report or {}).get("measures") or {}

    print(f"{'module':<10} {'file':<14} {'SHA-1':<10} {'nonmatch':>10}  notes")
    print("-" * 70)

    any_fail = False
    for name, basename in modules:
        built = ROOT / "build" / basename / basename
        got = sha1_file(built)
        exp = expected_sha1(basename)

        if got is None:
            sha_status = "missing"
            any_fail = True
        elif exp is None:
            sha_status = "no-checksum"
        elif got == exp:
            sha_status = "OK"
        else:
            sha_status = "MISMATCH"
            any_fail = True

        nm_count, _ = count_nonmatchings(basename)
        nm_str = str(nm_count) if nm_count else "-"

        notes = []
        if report and measures:
            # Global measures; per-unit breakdown would need richer report parsing
            code_pct = measures.get("matched_code_percent")
            if code_pct is not None:
                notes.append(f"objdiff code {code_pct:.1f}%")
            funcs = measures.get("matched_functions")
            total_funcs = measures.get("total_functions")
            if funcs is not None and total_funcs is not None:
                notes.append(f"{funcs}/{total_funcs} funcs")

        print(f"{name:<10} {basename:<14} {sha_status:<10} {nm_str:>10}  {'  '.join(notes)}")

    if report and measures:
        print()
        print("objdiff summary:")
        for key in (
            "matched_code_percent",
            "matched_data_percent",
            "matched_functions",
            "total_functions",
            "matched_bytes",
            "total_bytes",
        ):
            if key in measures:
                print(f"  {key}: {measures[key]}")

    print()
    if any_fail:
        print("One or more modules are missing or do not match. Run 'make' to rebuild.")
        return 1
    print("All present modules match their checksums.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
