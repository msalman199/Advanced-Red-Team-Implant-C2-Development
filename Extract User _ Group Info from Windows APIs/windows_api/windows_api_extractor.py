#!/usr/bin/env python3
import subprocess
import json
import re
from datetime import datetime

class WindowsAPIExtractor:
    def __init__(self, target_ip="127.0.0.1"):
        self.target_ip = target_ip
        self.users = []
        self.groups = []
        
    def extract_users_rpcclient(self, target_ip, username="", password=""):
        """Extract users using rpcclient"""
        try:
            cmd = f"rpcclient -U '{username}%{password}' {target_ip} -c 'enumdomusers'"
            result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
            
            users = []
            for line in result.stdout.split('\n'):
                if 'user:' in line:
                    match = re.search(r'user:\[(.*?)\].*rid:\[(.*?)\]', line)
                    if match:
                        users.append({
                            'username': match.group(1),
                            'rid': match.group(2),
                            'sid': f"S-1-5-21-{match.group(2)}"
                        })
            return users
        except Exception as e:
            print(f"Error extracting users: {e}")
            return []
    
    def extract_groups_rpcclient(self, target_ip, username="", password=""):
        """Extract groups using rpcclient"""
        try:
            cmd = f"rpcclient -U '{username}%{password}' {target_ip} -c 'enumdomgroups'"
            result = subprocess.run(cmd, shell=True, capture_output=True, text=True)
            
            groups = []
            for line in result.stdout.split('\n'):
                if 'group:' in line:
                    match = re.search(r'group:\[(.*?)\].*rid:\[(.*?)\]', line)
                    if match:
                        groups.append({
                            'groupname': match.group(1),
                            'rid': match.group(2),
                            'sid': f"S-1-5-21-{match.group(2)}"
                        })
            return groups
        except Exception as e:
            print(f"Error extracting groups: {e}")
            return []
    
    def simulate_windows_data(self):
        """Simulate Windows user and group data for demonstration"""
        self.users = [
            {'username': 'Administrator', 'rid': '500', 'sid': 'S-1-5-21-500', 'status': 'Active'},
            {'username': 'Guest', 'rid': '501', 'sid': 'S-1-5-21-501', 'status': 'Disabled'},
            {'username': 'john.doe', 'rid': '1001', 'sid': 'S-1-5-21-1001', 'status': 'Active'},
            {'username': 'jane.smith', 'rid': '1002', 'sid': 'S-1-5-21-1002', 'status': 'Active'},
            {'username': 'service_account', 'rid': '1003', 'sid': 'S-1-5-21-1003', 'status': 'Active'}
        ]
        
        self.groups = [
            {'groupname': 'Administrators', 'rid': '544', 'sid': 'S-1-5-32-544', 'type': 'Local'},
            {'groupname': 'Users', 'rid': '545', 'sid': 'S-1-5-32-545', 'type': 'Local'},
            {'groupname': 'Domain Admins', 'rid': '512', 'sid': 'S-1-5-21-512', 'type': 'Domain'},
            {'groupname': 'Domain Users', 'rid': '513', 'sid': 'S-1-5-21-513', 'type': 'Domain'},
            {'groupname': 'IT Support', 'rid': '1100', 'sid': 'S-1-5-21-1100', 'type': 'Custom'}
        ]
    
    def display_users(self):
        """Display extracted user information"""
        print("\n" + "="*60)
        print("WINDOWS USER ENUMERATION RESULTS")
        print("="*60)
        print(f"{'Username':<20} {'RID':<8} {'SID':<20} {'Status':<10}")
        print("-"*60)
        
        for user in self.users:
            print(f"{user['username']:<20} {user['rid']:<8} {user['sid']:<20} {user.get('status', 'Unknown'):<10}")
        
        print(f"\nTotal Users Found: {len(self.users)}")
    
    def display_groups(self):
        """Display extracted group information"""
        print("\n" + "="*60)
        print("WINDOWS GROUP ENUMERATION RESULTS")
        print("="*60)
        print(f"{'Group Name':<20} {'RID':<8} {'SID':<20} {'Type':<10}")
        print("-"*60)
        
        for group in self.groups:
            print(f"{group['groupname']:<20} {group['rid']:<8} {group['sid']:<20} {group.get('type', 'Unknown'):<10}")
        
        print(f"\nTotal Groups Found: {len(self.groups)}")
    
    def export_to_json(self, filename="windows_enumeration.json"):
        """Export results to JSON file"""
        data = {
            'timestamp': datetime.now().isoformat(),
            'target': self.target_ip,
            'users': self.users,
            'groups': self.groups,
            'summary': {
                'total_users': len(self.users),
                'total_groups': len(self.groups)
            }
        }
        
        with open(filename, 'w') as f:
            json.dump(data, f, indent=2)
        
        print(f"\nResults exported to: {filename}")
    
    def analyze_security_risks(self):
        """Analyze potential security risks"""
        print("\n" + "="*60)
        print("SECURITY ANALYSIS")
        print("="*60)
        
        # Check for default accounts
        default_accounts = [user for user in self.users if user['username'] in ['Administrator', 'Guest']]
        if default_accounts:
            print("⚠️  Default accounts found:")
            for account in default_accounts:
                print(f"   - {account['username']} (Status: {account.get('status', 'Unknown')})")
        
        # Check for service accounts
        service_accounts = [user for user in self.users if 'service' in user['username'].lower()]
        if service_accounts:
            print("\n🔧 Service accounts identified:")
            for account in service_accounts:
                print(f"   - {account['username']}")
        
        # Check for privileged groups
        privileged_groups = [group for group in self.groups if 'admin' in group['groupname'].lower()]
        if privileged_groups:
            print("\n🔐 Privileged groups found:")
            for group in privileged_groups:
                print(f"   - {group['groupname']} ({group.get('type', 'Unknown')})")

if __name__ == "__main__":
    extractor = WindowsAPIExtractor()
    
    print("Windows API User & Group Extractor")
    print("==================================")
    
    # Simulate Windows data extraction
    extractor.simulate_windows_data()
    
    # Display results
    extractor.display_users()
    extractor.display_groups()
    
    # Security analysis
    extractor.analyze_security_risks()
    
    # Export results
    extractor.export_to_json()
