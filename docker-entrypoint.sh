#!/bin/bash
set -euo pipefail

arch=$(uname -m)

case "$arch" in
  x86_64)
  ;;
  aarch64|arm64)
  if ! qemu-i386 -version >/dev/null 2>&1 && ! qemu-x86_64 -version >/dev/null 2>&1; then
        echo "arm64 host: install qemu-user-static or run:" >&2
        echo "  docker run --platform linux/amd64 ..." >&2
      fi
  ;;
esac

WORK=/tmp/sotc-build
rm -rf "$WORK"
mkdir -p "$WORK"

# Copy everything the build needs (exclude heavy/generated dirs)
rsync -a --exclude='out' --exclude='.python3' --exclude='build' \
         --exclude='.git' /workspace/ "$WORK/"

cd "$WORK"

mkdir -p tools/cc
if [ ! -e tools/cc/ee-gcc2.96 ]; then
  ln -sfn /opt/sotc-cc/ee-gcc2.96 tools/cc/ee-gcc2.96
fi

find . -type f \( -name "*.py" -o -name "*.sh" \) -exec chmod +x {} +

if [ -d "tools" ]; then
    chmod -R +x tools/
fi

if [ -d "scripts" ]; then
    chmod -R +x scripts/
fi

# Clean + configure + build + test
rm -rf out .python3 build

if [ $# -gt 0 ]; then
    echo "Running custom commands..."

    FULL_CMD="$*"
    IFS=',' read -ra COMMANDS <<< "$FULL_CMD"

    for cmd in "${COMMANDS[@]}"; do
        cmd="$(echo "$cmd" | xargs)"

        [ -z "$cmd" ] && continue

        if [[ "$cmd" =~ ^cmake[[:space:]]+.*--preset ]]; then
            [[ "$cmd" =~ DECOMP_PYTHON ]] || cmd="$cmd -DDECOMP_PYTHON=/opt/sotc-venv/bin/python3"
            [[ "$cmd" =~ DECOMP_CC ]] || cmd="$cmd -DDECOMP_CC=/opt/sotc-cc/ee-gcc2.96/bin/ee-gcc"
            [[ "$cmd" =~ DECOMP_AUTO_SETUP ]] || cmd="$cmd -DDECOMP_AUTO_SETUP=OFF"
        fi

        echo "----------------------------------------"
        echo "Executing: $cmd"
        echo "----------------------------------------"

        bash -c "$cmd" || exit 1
    done

    # Sync artifacts back to Windows after all commands finish
    mkdir -p /workspace/out /workspace/build
    cp -a out/. /workspace/out/ 2>/dev/null || true
    cp -a build/. /workspace/build/ 2>/dev/null || true
    echo "Custom commands finished and artifacts synced."
    exit 0
fi

echo "Starting clean configuration and build..."
cmake --preset default \
  -DDECOMP_AUTO_SETUP=OFF \
  -DDECOMP_PYTHON=/opt/sotc-venv/bin/python3 \
  -DDECOMP_CC=/opt/sotc-cc/ee-gcc2.96/bin/ee-gcc
cmake --build out -j$(nproc)
ctest --test-dir out --output-on-failure

# Copy useful artifacts back to the Windows-visible folder
mkdir -p /workspace/out /workspace/build
cp -a out/. /workspace/out/ 2>/dev/null || true
cp -a build/. /workspace/build/ 2>/dev/null || true

echo "Build finished successfully"