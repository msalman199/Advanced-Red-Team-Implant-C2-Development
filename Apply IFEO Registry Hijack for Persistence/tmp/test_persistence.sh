#!/bin/bash
echo "=== Persistence Test ==="
echo "Testing target_app hijack:"
/tmp/target_app

echo "Testing ls hijack:"
ls /home/ > /dev/null

echo "Checking logs:"
echo "Hijack log entries:"
wc -l /tmp/hijack_log.txt
echo "System hijack log entries:"
wc -l /tmp/system_hijack_log.txt
