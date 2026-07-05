#!/bin/bash

echo "Building Windows API project..."

# Create build directory if it doesn't exist
mkdir -p build

# Compile GUI application
echo "Compiling GUI application..."
x86_64-w64-mingw32-gcc -o build/hello_gui.exe src/main.c -luser32 -lgdi32 -lkernel32

# Compile console application
echo "Compiling console application..."
x86_64-w64-mingw32-gcc -o build/hello_console.exe src/console.c -lkernel32

# Check if builds were successful
if [ -f "build/hello_gui.exe" ] && [ -f "build/hello_console.exe" ]; then
    echo "Build successful!"
    echo "Files created:"
    ls -la build/*.exe
else
    echo "Build failed!"
    exit 1
fi
