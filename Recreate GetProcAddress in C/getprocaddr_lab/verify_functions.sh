#!/bin/bash

echo "=== Verifying Dynamic Library Functions ==="

# Check available symbols in math library
echo "Math library symbols:"
nm -D /lib/x86_64-linux-gnu/libm.so.6 | grep -E "(sqrt|sin|cos)" | head -5

echo -e "\nLibC symbols:"
nm -D /lib/x86_64-linux-gnu/libc.so.6 | grep -E "(malloc|strlen|printf)" | head -5

echo -e "\nRunning our custom implementation:"
./test_getprocaddr
