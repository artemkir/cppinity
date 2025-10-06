#!/bin/bash

# Define variables for your project paths and Emscripten compilation
SOURCE_DIRS="src"
OUTPUT_DIR="emcc_build"
OUTPUT_FILE="$OUTPUT_DIR/snake.html"

# Collect all .cpp files from the specified source directories
SOURCE_FILES=""
for dir in $SOURCE_DIRS; do
    SOURCE_FILES+="$(find $dir -type f -name '*.c*') "
done

INCLUDE_DIRS="-I./include -I./xcode_build/_deps/sokol-src"

# Define Emscripten flags
EM_FLAGS="-DSOKOL_GLES3 -s USE_WEBGL2=1 -s -O3"

# Create the output directory if it does not exist
mkdir -p $OUTPUT_DIR

echo "Source files: $SOURCE_FILES"
echo "Include files: $INCLUDE_DIRS"

# Command to compile the source files using Emscripten
emcc $SOURCE_FILES -o $OUTPUT_FILE $EM_FLAGS $INCLUDE_DIRS

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Build successful: $OUTPUT_FILE"
else
    echo "Build failed"
fi