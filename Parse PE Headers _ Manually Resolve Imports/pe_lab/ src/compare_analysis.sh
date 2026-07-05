#!/bin/bash

PE_FILE="samples/hello.exe"

echo "=== COMPARISON: Manual Parser vs System Tools ==="
echo

echo "--- Our Enhanced Parser ---"
./output/pe_parser_enhanced $PE_FILE
echo

echo "--- System Verification ---"
./output/verify_imports $PE_FILE
echo

echo "--- Raw PE Header (first 200 bytes) ---"
hexdump -C $PE_FILE | head -12
