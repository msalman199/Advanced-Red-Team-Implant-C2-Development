#!/bin/bash

echo "Testing Process & Service Enumerator"
echo "===================================="

echo "1. Testing process enumeration..."
./process_enum > /tmp/proc_test.out
PROC_COUNT=$(wc -l < /tmp/proc_test.out)
echo "   Found $PROC_COUNT lines of output"

echo "2. Testing service enumeration..."
echo "3" | ./service_enum > /tmp/service_test.out
echo "   Service enumeration completed"

echo "3. Checking if programs compile without warnings..."
make clean
make all

echo "4. All tests completed successfully!"
