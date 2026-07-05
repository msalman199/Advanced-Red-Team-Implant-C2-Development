#!/bin/bash
echo "Compiling Windows API Process Enumeration Program..."
x86_64-w64-mingw32-gcc -o process_enum.exe process_enum.c -lkernel32 -static
echo "Compilation completed!"
