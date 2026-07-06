#!/bin/bash

echo "=== Memory Analysis Script ==="

# Function to analyze process memory
analyze_process() {
    local pid=$1
    echo "[+] Analyzing process $pid"
    
    if [ -d "/proc/$pid" ]; then
        echo "Memory maps:"
        cat /proc/$pid/maps | grep -E "(rwx|r-x)"
        echo ""
        echo "Memory status:"
        cat /proc/$pid/status | grep -E "(VmSize|VmRSS|VmExe)"
    else
        echo "[-] Process $pid not found"
    fi
}

# Run loader in background and analyze
echo "[+] Starting loader analysis..."
./bin/advanced_loader &
LOADER_PID=$!

sleep 1
analyze_process $LOADER_PID

wait $LOADER_PID
echo "[+] Analysis complete"
