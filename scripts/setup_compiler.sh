#!/bin/bash

# Define variables
BASE_DIR="tools"
TARGET_DIR="$BASE_DIR/cc/ee-gcc2.96"
FILE_URL="https://github.com/decompme/compilers/releases/download/compilers/ee-gcc2.96.tar.xz"
TAR_FILE="ee-gcc2.96.tar.xz"

# Create destination directory if it does not exist
mkdir -p "$TARGET_DIR"

echo "Downloading $FILE_URL..."
curl -L -o "$TAR_FILE" "$FILE_URL"

# Extract the file
tar -xf "$TAR_FILE" -C "$TARGET_DIR"

# Set executable permissions for binaries in the cc directory
find "$TARGET_DIR" -type f -exec chmod +x {} \;

# Clean up by removing the downloaded file
rm "$TAR_FILE"

echo "Download and extraction completed."
