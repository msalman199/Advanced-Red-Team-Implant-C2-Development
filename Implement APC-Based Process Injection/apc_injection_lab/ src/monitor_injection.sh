#!/bin/bash
echo "Starting injection monitoring..."

# Start target process
./target_processes/target_process &
TARGET_PID=$!
echo "Target PID: $TARGET_PID"

# Monitor with strace in background
strace -p $TARGET_PID -o injection_trace.log &
STRACE_PID=$!

# Wait for strace to attach
sleep 2

# Perform injection
sudo ./src/apc_injector $TARGET_PID

# Wait for completion
sleep 3

# Stop monitoring
kill $STRACE_PID 2>/dev/null
kill $TARGET_PID 2>/dev/null

# Analyze results
echo "Injection trace analysis:"
grep -E "(mmap|ptrace|write)" injection_trace.log | head -10
