#!/bin/bash

echo "=== Execution Verification Script ==="

# Test 1: Basic loader functionality
echo "[+] Test 1: Basic loader"
timeout 5s ./bin/memory_loader
RESULT1=$?
echo "Basic loader exit code: $RESULT1"

# Test 2: Advanced loader functionality  
echo "[+] Test 2: Advanced loader"
timeout 5s ./bin/advanced_loader
RESULT2=$?
echo "Advanced loader exit code: $RESULT2"

# Test 3: Memory allocation verification
echo "[+] Test 3: Memory verification"
strace -e mmap,munmap ./bin/advanced_loader 2>&1 | grep -E "(mmap|munmap)"

echo "[+] All tests completed"
