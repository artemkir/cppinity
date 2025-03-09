#!/bin/bash

# Define variables for your project paths and Emscripten compilation
SOURCE_FILE="src/game.cpp"
OUTPUT_DIR="emcc_build"
OUTPUT_FILE="$OUTPUT_DIR/snake.html"
INCLUDE_DIRS="-I./src"

# Add other include directories as needed
# INCLUDE_DIRS+=" -I./OtherIncludeDir"

# Define Emscripten flags
EM_FLAGS="USE_SDL=2 -s -O3"

# Create the output directory if it does not exist
mkdir -p $OUTPUT_DIR

# Command to compile the source file using Emscripten
emcc $SOURCE_FILE -o $OUTPUT_FILE $EM_FLAGS $INCLUDE_DIRS

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    echo "Build successful: $OUTPUT_FILE"
else
    echo "Build failed"
fi