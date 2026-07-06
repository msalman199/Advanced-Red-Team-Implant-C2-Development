#!/usr/bin/env python3
import subprocess

def analyze_registry():
    """Analyze registry persistence entries"""
    print("=== Registry Persistence Analysis ===")
    
    # Check Run keys
    print("\n[*] Analyzing Run keys:")
    run_keys = [
        "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run",
        "HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
    ]
    
    for key in run_keys:
        result = subprocess.run([
            'wine', 'reg', 'query', key
        ], capture_output=True, text=True)
        
        if result.returncode == 0 and result.stdout.strip():
            print(f"[+] Found entries in {key}:")
            print(result.stdout)
        else:
            print(f"[-] No entries found in {key}")
    
    # Check RunOnce keys
    print("\n[*] Analyzing RunOnce keys:")
    runonce_keys = [
        "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce",
        "HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce"
    ]
    
    for key in runonce_keys:
        result = subprocess.run([
            'wine', 'reg', 'query', key
        ], capture_output=True, text=True)
        
        if result.returncode == 0 and result.stdout.strip():
            print(f"[+] Found entries in {key}:")
            print(result.stdout)
        else:
            print(f"[-] No entries found in {key}")

if __name__ == "__main__":
    analyze_registry()
