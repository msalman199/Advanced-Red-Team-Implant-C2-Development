#!/usr/bin/env python3
import subprocess
import sys
import argparse

class SMBEnumerator:
    def __init__(self, target, username="", password=""):
        self.target = target
        self.username = username
        self.password = password
    
    def enum_users_smbclient(self):
        """Enumerate users using smbclient"""
        print("\n[+] Enumerating users with smbclient...")
        try:
            if self.username and self.password:
                cmd = f"smbclient -L {self.target} -U {self.username}%{self.password}"
            else:
                cmd = f"smbclient -L {self.target} -N"
            
            result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
            print("SMB Shares:")
            print(result.stdout)
            
        except Exception as e:
            print(f"Error: {e}")
    
    def enum_with_rpcclient(self):
        """Enumerate using rpcclient"""
        print("\n[+] Enumerating with rpcclient...")
        commands = [
            "srvinfo",
            "enumdomusers",
            "enumdomgroups",
            "querydominfo"
        ]
        
        for cmd in commands:
            print(f"\n--- Executing: {cmd} ---")
            try:
                if self.username and self.password:
                    rpc_cmd = f"rpcclient -U '{self.username}%{self.password}' {self.target} -c '{cmd}'"
                else:
                    rpc_cmd = f"rpcclient -U '' {self.target} -c '{cmd}'"
                
                result = subprocess.run(rpc_cmd, shell=True, capture_output=True, text=True)
                if result.stdout:
                    print(result.stdout)
                if result.stderr:
                    print(f"Error: {result.stderr}")
                    
            except Exception as e:
                print(f"Error executing {cmd}: {e}")
    
    def enum_with_enum4linux(self):
        """Enumerate using enum4linux-ng"""
        print("\n[+] Enumerating with enum4linux-ng...")
        try:
            if self.username and self.password:
                cmd = f"enum4linux-ng -u {self.username} -p {self.password} {self.target}"
            else:
                cmd = f"enum4linux-ng {self.target}"
            
            result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
            print(result.stdout)
            
        except Exception as e:
            print(f"Error: {e}")

def main():
    parser = argparse.ArgumentParser(description="SMB Enumeration Tool")
    parser.add_argument("target", help="Target IP address")
    parser.add_argument("-u", "--username", default="", help="Username")
    parser.add_argument("-p", "--password", default="", help="Password")
    
    args = parser.parse_args()
    
    enumerator = SMBEnumerator(args.target, args.username, args.password)
    
    print(f"Starting enumeration of {args.target}")
    print("="*50)
    
    enumerator.enum_users_smbclient()
    enumerator.enum_with_rpcclient()
    enumerator.enum_with_enum4linux()

if __name__ == "__main__":
    main()
