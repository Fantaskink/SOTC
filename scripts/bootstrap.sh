#!/bin/sh
# Installs the system packages the SOTC build needs (Debian/Ubuntu, Linux or WSL2).
# Everything else (python venv, requirements, the game's compiler) is set up by CMake itself.
#
#   scripts/bootstrap.sh              install
#   scripts/bootstrap.sh --dry-run    only show what apt would do
set -eu

# The first line mirrors the README; the rest are what the CMake build and the 32-bit compiler need.
PACKAGES="gcc-mipsel-linux-gnu binutils-mips-linux-gnu ninja-build
          python3 python3-pip python3-venv
          cmake cpp perl libc6-i386"

if ! command -v apt-get >/dev/null 2>&1; then
  echo "apt-get was not found. Install the equivalents of:" >&2
  echo "  $(echo $PACKAGES)" >&2
  exit 1
fi

if [ "${1:-}" = "--dry-run" ]; then
  # shellcheck disable=SC2086
  exec apt-get -s install $PACKAGES
fi

SUDO=""
if [ "$(id -u)" -ne 0 ]; then
  SUDO="sudo"
fi

# shellcheck disable=SC2086
$SUDO apt-get update
# shellcheck disable=SC2086
$SUDO apt-get install -y $PACKAGES

# The CMake files need 3.20 or newer.
version=$(cmake --version | head -n 1 | sed 's/[^0-9.]//g')
if [ "$(printf '%s\n3.20\n' "$version" | sort -V | head -n 1)" != "3.20" ]; then
  echo "WARNING: cmake $version is older than 3.20; get a newer one (https://cmake.org/download/ or 'pip install cmake')." >&2
fi

echo "mips assembler: $(mips-linux-gnu-as --version | head -n 1)"
echo "Next: put the game files in iso/ (see the README), then run  cmake --preset default"
