#!/usr/bin/env python3
import subprocess
import os

def remove_persistence():
    """Remove all persistence entries"""
    print("=== Cleaning Up Persistence ===")
    
    # Remove Run key entry
    result = subprocess.run([
        'wine', 'reg', 'delete',
        'HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run',
        '/v', 'SecurityUpdate', '/f'
    ], capture_output=True, text=True)
    
    if result.returncode == 0:
        print("[+] Removed Run key persistence")
    else:
        print("[-] Run key persistence not found or already removed")
    
    # Remove payload file
    payload_path = os.path.expanduser("~/.wine/drive_c/persistence_payload.bat")
    if os.path.exists(payload_path):
        os.remove(payload_path)
        print("[+] Removed payload file")
    
    # Remove log file
    log_path = os.path.expanduser("~/.wine/drive_c/persistence_log.txt")
    if os.path.exists(log_path):
        os.remove(log_path)
        print("[+] Removed log file")
    
    print("[*] Cleanup complete")

if __name__ == "__main__":
    remove_persistence()
