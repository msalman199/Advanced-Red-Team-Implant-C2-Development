#!/bin/bash
echo "Compiling Advanced Process Monitor..."
x86_64-w64-mingw32-gcc -o advanced_process_enum.exe advanced_process_enum.c -lkernel32 -lpsapi -static
echo "Advanced compilation completed!"
