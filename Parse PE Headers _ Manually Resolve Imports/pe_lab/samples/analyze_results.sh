#!/bin/bash

echo "=== PE Analysis Results ==="
echo

echo "File information:"
file samples/hello.exe
echo

echo "Hexdump of first 64 bytes:"
hexdump -C samples/hello.exe | head -4
echo

echo "Basic parser output:"
./output/pe_parser samples/hello.exe
echo

echo "Enhanced parser output:"
./output/pe_parser_enhanced samples/hello.exe
