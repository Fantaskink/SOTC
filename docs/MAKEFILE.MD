# Building with MakeFile
The Make build lives next to the existing `cmake` build and produces the same files in the same
places (`build/`, `asm/`, `assets/`), so tools that read `build/<module>/*.elf` or `*.map` keep working.
Use one build system at a time; they share those directories.

Linux or WSL2 only: the game's compiler (`ee-gcc` 2.96) is a 32-bit Linux program.

## Quick start
look at [MakeFile Requirements](../README.md#requirements) before running

```sh
scripts/bootstrap.sh # system packages (Debian/Ubuntu); --dry-run shows what it would do

# before building put the game files in iso/ as described in the README
make             

```

## Everyday use

| Task                          | Command                               |
|-------------------------------|---------------------------------------|
| Build one module              | `make loader` (or `kernel`)           |
| Clean build files             | `make clean`                          |
| List make commands            | `make help`                           |
| SHA-1 & Non-Matching count    | `make status`                         |
| Write objdiff.json            | `make objdif`                         |
| objdif report + percentages   | `make progress`                       |
| Refresh compile_commands.json | `make compdb`                         |
| Check individual objects      | `make check OBJ=build/<Module>/foo.o` |