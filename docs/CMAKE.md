# Building with CMake

The CMake build lives next to the existing `make` build and produces the same files in the same
places (`build/`, `asm/`, `assets/`), so tools that read `build/<module>/*.elf` or `*.map` keep working.
Use one build system at a time; they share those directories.

Linux or WSL2 only: the game's compiler (`ee-gcc` 2.96) is a 32-bit Linux program.

## Quick start

```sh
scripts/bootstrap.sh                 # system packages (Debian/Ubuntu); --dry-run shows what it would do
# put the game files in iso/ as described in the README
cmake --preset default               # sets up what is missing, runs splat, writes the build files
cmake --build out                    # builds every enabled module
ctest --test-dir out                 # checks each result against checksum-<module>.sha1
```

The first `cmake --preset default` takes a while: it creates `.python3`, installs `requirements.txt`,
downloads and verifies the compiler into `tools/cc/ee-gcc2.96`, and runs splat for each enabled module.
Later runs skip whatever is already done.

What it needs: CMake 3.20+, Ninja, the MIPS binutils, `cpp`, `iconv`, `sh`, Python 3 with `venv`, and the
32-bit libc (`libc6-i386`). `perl` is needed only when register rewriting is active (see
[Assembler register names](#assembler-register-names)). `scripts/bootstrap.sh` installs all of these on
Debian/Ubuntu.

## Everyday use

| Task | Command |
|---|---|
| Build one module | `cmake --build out --target loader` (or `kernel`) |
| Extract and build only some modules | `cmake --preset default -DSOTC_MODULES=loader` (a list works: `"loader;kernel"`) |
| Back to all modules | `cmake --preset default -DSOTC_MODULES=all` |
| After editing a module's YAML | just build; CMake notices and re-runs splat for that module |
| Start from scratch | `rm -rf out build asm assets` |
| Skip downloads and installs | `-DDECOMP_AUTO_SETUP=OFF` (the checks still run) |

Splat only re-runs when `config/<module>/<module>.yaml`, `configure.py`, or the module's `asm/` directory
changed or is missing, not on every CMake edit.

## Options

Pass them as `-D<NAME>=<value>` when configuring.

| Option                | Default                                                                                                 | Meaning                                                                                 |
|-----------------------|---------------------------------------------------------------------------------------------------------|-----------------------------------------------------------------------------------------|
| `SOTC_MODULES`        | `all`                                                                                                   | Which modules to extract and build: `all` or a list of names from `config/modules.txt`  |
| `DECOMP_AUTO_SETUP`   | `ON`                                                                                                    | Create the venv and download the compiler when missing                                  |
| `DECOMP_CROSS`        | `mips-linux-gnu-`                                                                                       | Prefix of the binutils (`as`, `ld`, `objcopy`, `strip`); may be an absolute path prefix |
| `DECOMP_CC`           | `tools/cc/ee-gcc2.96/bin/ee-gcc`                                                                        | The game's compiler                                                                     |
| `DECOMP_PYTHON`       | `.python3/bin/python3`                                                                                  | Python with splat installed                                                             |
| `DECOMP_NUMERIC_REGS` | `AUTO`                                                                                                  | `AUTO`, `ON` or `OFF`; see below                                                        |
| `DECOMP_EEGCC_URL`    | [DecompMe Release](https://github.com/decompme/compilers/releases/download/compilers/ee-gcc2.96.tar.xz) | Where the compiler archive is downloaded from                                           |
| `DECOMP_EEGCC_SHA256` | <abbr title="0590d2ca9da8f5903889d66761220d14b47a8d14ba987ca53db84a1650a1fd0a">`0590d2ca...`</abbr>     | Expected hash of that archive; empty skips the check                                    |

## Files

| File | Role |
|---|---|
| `config/modules.txt` | The module registry (name, file, format, options), read by `configure.py`, the makefile and `CMakeLists.txt` |
| `CMakeLists.txt` | Reads the registry, handles `SOTC_MODULES`, includes the files below |
| `cmake/Dependencies.cmake` | Venv, compiler download, tool and game-file checks |
| `cmake/Toolchain.cmake` | Assembler checks, python and patcher settings |
| `cmake/Decomp.cmake` | The rules: objects, link, objcopy, sha1 test |
| `cmake/CheckSha1.cmake` | The comparison `ctest` runs |
| `cmake/probe/eabi_registers.s` | Tiny file used to probe the assembler |
| `tools/elf_patcher_lite.py` | Same patch as `tools/elf_patcher.py`, without the `spimdisasm` import |
| `tools/regs_numeric.pl` | Rewrites `$t0`-style register names to numbers (see below) |
| `configure.py` | Reads `config/modules.txt` instead of hardcoding the two binaries, and gains `--emit-cmake`; its output for `make` is unchanged |
| `makefile` | Generates the per-module rules from `config/modules.txt`; same targets as before |

The legacy `*.ninja` files are only used by `make`. The `*.ld` files are written by splat and are needed by
both builds. `configure-kernel.py` and `configure-SCPS_150.97.py` are not used by either.

## Troubleshooting

| Symptom | Cause and fix                                                                                                                                                                                                                                    |
|---|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| `ee-gcc does not run` | 32-bit libc missing: `sudo apt install libc6-i386`                                                                                                                                                                                               |
| `could not create .python3` | `sudo apt install python3-venv python3-pip`                                                                                                                                                                                                      |
| `downloading the compiler failed` | The release asset changed or the network failed. Check `DECOMP_EEGCC_URL`                                                                                                                                                                        |
| `missing game files: iso/...` | Extract the files from your own ISO into `iso/`                                                                                                                                                                                                  |
| `'...as' was not found or cannot run` | Install `binutils-mips-linux-gnu` or set `DECOMP_CROSS`                                                                                                                                                                                          |
| `Unknown module 'x'` | The name is not in `config/modules.txt`                                                                                                                                                                                                          |
| `missing and no known rule` for an `asm/...` file | `asm/` was deleted; run `cmake --preset default` again to re-extract                                                                                                                                                                             |
| `No such file` for `asm/<NAME>/nonmatchings/...` after moving a function between C and `INCLUDE_ASM` | Splat decides which per-function `.s` files to write by looking at the C file, and CMake only re-runs it when the YAML or `configure.py` changed. Force it: `rm -rf asm/<NAME> && cmake --preset default`                                        |
| `rule 'x' is not defined` | `configure.py` emitted an object kind that `cmake/Decomp.cmake` does not implement (only `cc` and `as` exist). Add the rule in `decomp_object`, and if it needs more variables, extend `write_cmake_manifest` in `configure.py`                  |
| `loader_sha1` or `kernel_sha1` fails | Compare `sha1sum iso/<file>` with `checksum-<file>.sha1`; they must be equal. If they differ you have a different game version or a bad dump. If they match, suspect the toolchain (assembler, compiler flags) also try -DDECOMP_NUMERIC_REGS=ON |
