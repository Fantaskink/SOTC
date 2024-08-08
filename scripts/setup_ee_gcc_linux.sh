#!/bin/bash

set -e

TOP=$(cd "$(dirname "$0")"; pwd)/..


SDK_VER=242

die() { # perl-style `die` expressions.
    echo "Error: $@"
    exit 1
}

# Downloads files without checking integrity
download() {
    echo "Downloading $1..."
    wget -qP /tmp $1

    BASENAME=$(basename $1)

    #echo "moving files out of /tmp"
    #mv /tmp/$BASENAME $TOP
}

echo "Starting ee-gcc setup script..."

# Download required files (registry + SDK package)
download "https://github.com/decompme/compilers/releases/download/compilers/ee-gcc2.96.tar.xz"

# Extract the compiler into the tools dir
echo "Extracting compiler to $TOP/tools..."
tar -xJf /tmp/ee-gcc2.96.tar.xz -C $TOP/tools

echo "Removing temporary files..."
rm /tmp/ee-gcc2.96.tar.xz

echo "Setup complete!"
