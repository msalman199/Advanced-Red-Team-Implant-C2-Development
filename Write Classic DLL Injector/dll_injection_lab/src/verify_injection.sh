#!/bin/bash

if [ $# -ne 1 ]; then
    echo "Usage: $0 <pid>"
    exit 1
fi

PID=$1

echo "Checking process $PID for loaded libraries..."
echo "=== Memory Maps ==="
cat /proc/$PID/maps | grep -E "(lib|so)"

echo ""
echo "=== Open Files ==="
lsof -p $PID | grep -E "\.so"
