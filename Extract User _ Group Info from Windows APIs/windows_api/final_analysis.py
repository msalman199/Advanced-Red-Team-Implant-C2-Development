#!/usr/bin/env python3
import json
import subprocess

def analyze_enumeration_results():
    """Perform final analysis of enumeration results"""
    
    print("="*60)
    print("FINAL WINDOWS API ENUMERATION ANALYSIS")
    print("="*60)
    
    # Load and display data
    try:
        with open('windows_enumeration.json', 'r') as f:
            data = json.load(f)
        
        print(f"\n📊 ENUMERATION SUMMARY")
        print(f"Target System: {data.get('target', 'N/A')}")
        print(f"Scan Time: {data.get('timestamp', 'N/A')}")
        print(f"Users Discovered: {len(data['users'])}")
        print(f"Groups Discovered: {len(data['groups'])}")
        
        print(f"\n🔍 KEY FINDINGS")
        
        # Analyze user accounts
        admin_users = [u for u in data['users'] if 'admin' in u['username'].lower()]
        service_users = [u for u in data['users'] if 'service' in u['username'].lower()]
        default_users = [u for u in data['users'] if u['username'] in ['Administrator', 'Guest']]
        
        if admin_users:
            print(f"Administrative Users: {len(admin_users)}")
            for user in admin_users:
                print(f"  - {user['username']} (RID: {user['rid']})")
        
        if service_users:
            print(f"Service Accounts: {len(service_users)}")
            for user in service_users:
                print(f"  - {user['username']} (RID: {user['rid']})")
        
        if default_users:
            print(f"Default Accounts: {len(default_users)}")
            for user in default_users:
                print(f"  - {user['username']} (Status: {user.get('status', 'Unknown')})")
        
        # Analyze groups
        admin_groups = [g for g in data['groups'] if 'admin' in g['groupname'].lower()]
        domain_groups = [g for g in data['groups'] if g.get('type') == 'Domain']
        
        if admin_groups:
            print(f"Administrative Groups: {len(admin_groups)}")
            for group in admin_groups:
                print(f"  - {group['groupname']} (Type: {group.get('type', 'Unknown')})")
        
        if domain_groups:
            print(f"Domain Groups: {len(domain_groups)}")
        
        print(f"\n📋 RECOMMENDATIONS")
        print("1. Review administrative account usage")
        print("2. Audit service account permissions")
        print("3. Disable unnecessary default accounts")
        print("4. Monitor privileged group memberships")
        print("5. Implement least privilege principles")
        
        print(f"\n📁 Generated Files:")
        files = ['windows_enumeration.json', 'users_export.csv', 'groups_export.csv', 'enumeration_report.html']
        for file in files:
            try:
                size = subprocess.check_output(['stat', '-c', '%s', file]).decode().strip()
                print(f"  - {file} ({size} bytes)")
            except:
                print(f"  - {file} (not found)")
        
    except FileNotFoundError:
        print("Error: Enumeration data not found. Run the main extractor first.")
    except Exception as e:
        print(f"Error analyzing results: {e}")

if __name__ == "__main__":
    analyze_enumeration_results()
