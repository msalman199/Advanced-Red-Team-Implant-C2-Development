#!/usr/bin/env python3
import subprocess
import time
import os

def simulate_startup():
    """Simulate Windows startup process"""
    print("[*] Simulating Windows startup...")
    
    # Clear previous log
    log_path = os.path.expanduser("~/.wine/drive_c/persistence_log.txt")
    if os.path.exists(log_path):
        os.remove(log_path)
    
    # Execute Run key entries
    print("[*] Processing Run key entries...")
    run_result = subprocess.run([
        'wine', 'reg', 'query', 
        'HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run'
    ], capture_output=True, text=True)
    
    if "SecurityUpdate" in run_result.stdout:
        print("[+] Run key persistence found - executing payload")
        subprocess.run(['wine', 'cmd', '/c', 'C:\\persistence_payload.bat'])
    
    # Execute RunOnce key entries
    print("[*] Processing RunOnce key entries...")
    runonce_result = subprocess.run([
        'wine', 'reg', 'query', 
        'HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce'
    ], capture_output=True, text=True)
    
    if "SystemCheck" in runonce_result.stdout:
        print("[+] RunOnce key persistence found - executing payload")
        subprocess.run(['wine', 'cmd', '/c', 'C:\\persistence_payload.bat'])
        
        # Remove RunOnce entry (simulating Windows behavior)
        subprocess.run([
            'wine', 'reg', 'delete',
            'HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce',
            '/v', 'SystemCheck', '/f'
        ])
        print("[*] RunOnce entry removed after execution")

def check_persistence_log():
    """Check if persistence payload executed"""
    log_path = os.path.expanduser("~/.wine/drive_c/persistence_log.txt")
    
    if os.path.exists(log_path):
        print("\n[+] Persistence log found:")
        with open(log_path, 'r') as f:
            print(f.read())
        return True
    else:
        print("\n[-] No persistence log found")
        return False

if __name__ == "__main__":
    print("=== Persistence Verification ===")
    simulate_startup()
    time.sleep(2)
    check_persistence_log()
