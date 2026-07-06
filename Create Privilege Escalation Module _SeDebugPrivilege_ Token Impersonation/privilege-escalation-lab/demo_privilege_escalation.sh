#!/bin/bash

echo "=== Privilege Escalation Module Demonstration ==="
echo

# Check current user
echo "[*] Current user context:"
id
echo

# Test SeDebugPrivilege simulation
echo "[*] Testing SeDebugPrivilege simulation:"
./sedebug_privilege
echo

# Test token impersonation
echo "[*] Testing token impersonation:"
./token_impersonation
echo

# Test comprehensive module
echo "[*] Testing comprehensive privilege escalation:"
./privilege_escalation_module
echo

echo "[*] Demonstration complete!"
echo "[*] Note: Run with 'sudo ./demo_privilege_escalation.sh' to see elevated privilege behavior"
