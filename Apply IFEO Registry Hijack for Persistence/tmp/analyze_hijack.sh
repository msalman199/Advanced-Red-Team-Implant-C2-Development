#!/bin/bash
echo "=== IFEO Hijack Analysis ==="

echo "1. File modifications:"
ls -la /tmp/target_app* /bin/ls*

echo "2. Log analysis:"
echo "Total hijack attempts: $(wc -l < /tmp/hijack_log.txt 2>/dev/null || echo 0)"
echo "System command hijacks: $(wc -l < /tmp/system_hijack_log.txt 2>/dev/null || echo 0)"

echo "3. Process verification:"
pgrep -f hijack || echo "No hijack processes currently running"

echo "4. Persistence verification:"
if [ -f /tmp/target_app ] && [ -f /bin/ls ]; then
    echo "Hijacks are persistent and in place"
else
    echo "Some hijacks may have been removed"
fi
