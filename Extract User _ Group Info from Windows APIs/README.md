<div align="center">

# 🔑 Extract User & Group Info from Windows APIs

### SMB/CIFS-Based Enumeration & Reporting with Python

![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![Python](https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white)
![Samba](https://img.shields.io/badge/Samba-0086CB?style=for-the-badge&logo=samba&logoColor=white)
![Impacket](https://img.shields.io/badge/Impacket-4B0082?style=for-the-badge&logo=python&logoColor=white)
![HTML5](https://img.shields.io/badge/HTML5-E34F26?style=for-the-badge&logo=html5&logoColor=white)
![Difficulty](https://img.shields.io/badge/Difficulty-Intermediate-yellow?style=for-the-badge)

*A hands-on Al Nafi lab for enumerating Windows users and groups over SMB/CIFS from a Linux host, and generating structured JSON, CSV, and HTML reports*

</div>

---

## 📑 Table of Contents

- [Learning Objectives](#-learning-objectives)
- [Prerequisites](#-prerequisites)
- [Lab Environment](#-lab-environment)
- [Task 1: Install Required Tools and Dependencies](#-task-1-install-required-tools-and-dependencies)
- [Task 2: Set Up Target Windows Environment Simulation](#-task-2-set-up-target-windows-environment-simulation)
- [Task 3: Create Advanced Enumeration Tools](#-task-3-create-advanced-enumeration-tools)
- [Task 4: Execute User and Group Enumeration](#-task-4-execute-user-and-group-enumeration)
- [Task 5: Display and Analyze Collected Data](#-task-5-display-and-analyze-collected-data)
- [Verification and Testing](#-verification-and-testing)
- [MITRE ATT&CK Mapping](#-mitre-attck-mapping)
- [Troubleshooting](#-troubleshooting)
- [Key Takeaways](#-key-takeaways)

---

## 🎯 Learning Objectives

| # | Objective |
|---|-----------|
| 1 | Extract user and group information from Windows systems using open-source tools on Linux |
| 2 | Enumerate Windows users and groups through SMB/CIFS protocols |
| 3 | Display and analyze collected Windows authentication data |
| 4 | Understand Windows security identifier (SID) structures |

## ✅ Prerequisites

| Requirement | Details |
|---|---|
| ⌨️ CLI Skills | Basic Linux command-line knowledge |
| 🗂️ Windows Concepts | Understanding of Windows user/group concepts |
| 🌐 Protocols | Familiarity with network protocols (SMB/CIFS) |
| 🐍 Scripting | Knowledge of Python scripting basics |

## 🌐 Lab Environment

> Al Nafi provides a dedicated **bare-metal Linux cloud machine** for this lab — no pre-installed tools. Click **Start Lab** to begin; every component is installed from scratch during the walkthrough. All enumeration in this lab targets `127.0.0.1` (localhost) using simulated data — no external or third-party systems are contacted.

---

## 🧩 Task 1: Install Required Tools and Dependencies

![Step](https://img.shields.io/badge/Step-1-blue?style=flat-square) ![Setup](https://img.shields.io/badge/Type-Environment_Setup-informational?style=flat-square)

### Subtask 1.1: Update System and Install Base Tools

```bash
# 📦 Update system and install SMB/RPC enumeration utilities
sudo apt update && sudo apt upgrade -y
sudo apt install -y python3 python3-pip smbclient cifs-utils rpcclient enum4linux-ng
```

### Subtask 1.2: Install Python Libraries

```bash
# 🐍 Install SMB-related Python libraries
pip3 install impacket-scripts smbprotocol pysmb
```

### Subtask 1.3: Verify Installation

```bash
# 🔍 Confirm tools and libraries are available
which rpcclient smbclient enum4linux-ng
python3 -c "import smbprotocol; print('SMB libraries installed successfully')"
```

---

## 🧩 Task 2: Set Up Target Windows Environment Simulation

![Step](https://img.shields.io/badge/Step-2-blue?style=flat-square) ![Coding](https://img.shields.io/badge/Type-Python_Scripting-orange?style=flat-square)

### Subtask 2.1: Create Mock Windows Data Structure

```bash
# 📁 TODO: Create your lab directory
mkdir -p ~/lab8_windows_api
cd ~/lab8_windows_api
```

### Subtask 2.2: Create Python Script for Windows API Simulation

```bash
cat > windows_api_extractor.py << 'EOF'
```

```python
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
            # TODO: Parse result.stdout line by line, use a regex to pull
            #       out username and rid from lines containing "user:",
            #       and build a users list with derived SID strings
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
            # TODO: Parse result.stdout the same way as extract_users_rpcclient,
            #       matching lines containing "group:" instead of "user:"
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

        # TODO: Filter self.users for default accounts (Administrator, Guest),
        #       service-style accounts, and filter self.groups for
        #       privileged group names — print each category found

if __name__ == "__main__":
    extractor = WindowsAPIExtractor()

    print("Windows API User & Group Extractor")
    print("==================================")

    extractor.simulate_windows_data()

    extractor.display_users()
    extractor.display_groups()

    extractor.analyze_security_risks()

    extractor.export_to_json()
```

```bash
EOF
```

### Subtask 2.3: Make Script Executable

```bash
chmod +x windows_api_extractor.py
```

---

## 🧩 Task 3: Create Advanced Enumeration Tools

![Step](https://img.shields.io/badge/Step-3-blue?style=flat-square) ![Enumeration](https://img.shields.io/badge/Type-SMB_Enumeration-9cf?style=flat-square)

### Subtask 3.1: Create SMB Enumeration Script

```bash
cat > smb_enum_tool.py << 'EOF'
```

```python
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
            # TODO: Build the enum4linux-ng command (with or without
            #       credentials, following the pattern used above) and
            #       run it with subprocess.run, printing result.stdout
            pass
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
```

```bash
EOF
```

> ⚠️ **Authorized scope only.** `rpcclient`, `smbclient`, and `enum4linux-ng` should only ever be pointed at systems you own or are explicitly authorized to test — in this lab, that's `127.0.0.1` on your own lab machine.

### Subtask 3.2: Make SMB Tool Executable

```bash
chmod +x smb_enum_tool.py
```

---

## 🧩 Task 4: Execute User and Group Enumeration

![Step](https://img.shields.io/badge/Step-4-blue?style=flat-square) ![Execution](https://img.shields.io/badge/Type-Script_Execution-success?style=flat-square)

### Subtask 4.1: Run Main Extraction Script

```bash
python3 windows_api_extractor.py
```

### Subtask 4.2: Test SMB Enumeration Against Localhost

```bash
python3 smb_enum_tool.py 127.0.0.1
```

### Subtask 4.3: Create Comprehensive Report Generator

```bash
cat > report_generator.py << 'EOF'
```

```python
#!/usr/bin/env python3
import json
import os
from datetime import datetime

class ReportGenerator:
    def __init__(self, json_file="windows_enumeration.json"):
        self.json_file = json_file
        self.data = self.load_data()

    def load_data(self):
        """Load enumeration data from JSON file"""
        try:
            with open(self.json_file, 'r') as f:
                return json.load(f)
        except FileNotFoundError:
            print(f"Error: {self.json_file} not found. Run the main extractor first.")
            return None

    def generate_html_report(self):
        """Generate HTML report"""
        if not self.data:
            return

        html_content = f"""
<!DOCTYPE html>
<html>
<head>
    <title>Windows API Enumeration Report</title>
    <style>
        body {{ font-family: Arial, sans-serif; margin: 20px; }}
        .header {{ background-color: #2c3e50; color: white; padding: 20px; }}
        .section {{ margin: 20px 0; }}
        table {{ border-collapse: collapse; width: 100%; }}
        th, td {{ border: 1px solid #ddd; padding: 8px; text-align: left; }}
        th {{ background-color: #f2f2f2; }}
    </style>
</head>
<body>
    <div class="header">
        <h1>Windows API Enumeration Report</h1>
        <p>Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}</p>
        <p>Target: {self.data.get('target', 'N/A')}</p>
    </div>

    <div class="section">
        <h2>Summary</h2>
        <p>Total Users: {self.data['summary']['total_users']}</p>
        <p>Total Groups: {self.data['summary']['total_groups']}</p>
    </div>

    <div class="section">
        <h2>Users</h2>
        <table>
            <tr><th>Username</th><th>RID</th><th>SID</th><th>Status</th></tr>
"""
        # TODO: Loop over self.data['users'] and append a <tr> row for
        #       each user, following the header columns above

        html_content += """
        </table>
    </div>

    <div class="section">
        <h2>Groups</h2>
        <table>
            <tr><th>Group Name</th><th>RID</th><th>SID</th><th>Type</th></tr>
"""
        # TODO: Loop over self.data['groups'] and append a <tr> row for
        #       each group, following the header columns above

        html_content += """
        </table>
    </div>
</body>
</html>
"""

        with open('enumeration_report.html', 'w') as f:
            f.write(html_content)

        print("HTML report generated: enumeration_report.html")

    def generate_csv_export(self):
        """Generate CSV exports"""
        if not self.data:
            return

        with open('users_export.csv', 'w') as f:
            f.write("Username,RID,SID,Status\n")
            for user in self.data['users']:
                f.write(f"{user['username']},{user['rid']},{user['sid']},{user.get('status', 'Unknown')}\n")

        with open('groups_export.csv', 'w') as f:
            f.write("Group Name,RID,SID,Type\n")
            for group in self.data['groups']:
                f.write(f"{group['groupname']},{group['rid']},{group['sid']},{group.get('type', 'Unknown')}\n")

        print("CSV exports generated: users_export.csv, groups_export.csv")

if __name__ == "__main__":
    generator = ReportGenerator()
    generator.generate_html_report()
    generator.generate_csv_export()
```

```bash
EOF
```

### Subtask 4.4: Generate Reports

```bash
chmod +x report_generator.py
python3 report_generator.py
```

---

## 🧩 Task 5: Display and Analyze Collected Data

![Step](https://img.shields.io/badge/Step-5-blue?style=flat-square) ![Analysis](https://img.shields.io/badge/Type-Data_Analysis-purple?style=flat-square)

### Subtask 5.1: View Generated Files

```bash
ls -la *.json *.csv *.html
```

### Subtask 5.2: Display JSON Data

```bash
cat windows_enumeration.json | python3 -m json.tool
```

### Subtask 5.3: View CSV Data

```bash
echo "=== USERS ==="
cat users_export.csv
echo -e "\n=== GROUPS ==="
cat groups_export.csv
```

### Subtask 5.4: Create Final Analysis Script

```bash
cat > final_analysis.py << 'EOF'
```

```python
#!/usr/bin/env python3
import json
import subprocess

def analyze_enumeration_results():
    """Perform final analysis of enumeration results"""

    print("="*60)
    print("FINAL WINDOWS API ENUMERATION ANALYSIS")
    print("="*60)

    try:
        with open('windows_enumeration.json', 'r') as f:
            data = json.load(f)

        print(f"\n📊 ENUMERATION SUMMARY")
        print(f"Target System: {data.get('target', 'N/A')}")
        print(f"Scan Time: {data.get('timestamp', 'N/A')}")
        print(f"Users Discovered: {len(data['users'])}")
        print(f"Groups Discovered: {len(data['groups'])}")

        print(f"\n🔍 KEY FINDINGS")

        # TODO: Build admin_users, service_users, and default_users lists
        #       by filtering data['users'] on username patterns, then
        #       print each category with counts (mirror the groups logic below)

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
```

```bash
EOF
```

### Subtask 5.5: Run Final Analysis

```bash
chmod +x final_analysis.py
python3 final_analysis.py
```

---

## 🔬 Verification and Testing

![Step](https://img.shields.io/badge/Step-Verify-brightgreen?style=flat-square)

### Verify All Components

```bash
echo "=== Lab 8 Verification ==="
echo "Files created:"
ls -la *.py *.json *.csv *.html 2>/dev/null || echo "Some files missing"

echo -e "\nTesting main extractor:"
python3 -c "import windows_api_extractor; print('✓ Main extractor working')"

echo -e "\nTesting report generator:"
python3 -c "import report_generator; print('✓ Report generator working')"

echo -e "\nVerifying JSON data:"
python3 -c "import json; data=json.load(open('windows_enumeration.json')); print(f'✓ JSON valid: {len(data[\"users\"])} users, {len(data[\"groups\"])} groups')"
```

---

## 🛡️ MITRE ATT&CK Mapping

| Tactic | Technique | ID | Relevance to This Lab |
|---|---|---|---|
| Discovery | Account Discovery | T1087 | Enumerating domain/local users via `rpcclient` and `enum4linux-ng` |
| Discovery | Permission Groups Discovery | T1069 | Enumerating groups and analyzing privileged group membership |
| Discovery | Network Share Discovery | T1135 | Listing SMB shares via `smbclient` |
| Collection | Data from Local System | T1005 | Aggregating enumeration results into JSON/CSV/HTML reports |

> 📌 This mapping is provided for **defensive awareness and detection-engineering context**. SMB/RPC user and group enumeration is a standard authorized-assessment technique for auditing account hygiene and privileged group membership; the same techniques are monitored by defenders because they also appear during adversary reconnaissance. All enumeration in this lab is scoped to localhost with simulated data.

---

## 🧯 Troubleshooting

<details>
<summary><strong>Click to expand common issues and fixes</strong></summary>

| Issue | Cause | Fix |
|---|---|---|
| `rpcclient`/`smbclient` not found | Samba packages not installed | Rerun `sudo apt install -y smbclient cifs-utils rpcclient enum4linux-ng` |
| `import smbprotocol` fails | pip packages missing or wrong Python environment | Rerun `pip3 install impacket-scripts smbprotocol pysmb` |
| Empty `enumdomusers`/`enumdomgroups` output | No SMB service listening on the target, or anonymous access disabled | Confirm a service is running on `127.0.0.1`; this lab primarily uses simulated data via `simulate_windows_data()` |
| `windows_enumeration.json` not found | Main extractor script not run yet | Run `python3 windows_api_extractor.py` before the report generator or final analysis |

</details>

---

## 🔑 Key Takeaways

| Concept | Summary |
|---|---|
| SMB/RPC tooling | `rpcclient`, `smbclient`, and `enum4linux-ng` are standard open-source tools for authorized Windows enumeration from Linux |
| SIDs and RIDs | Security Identifiers combine a domain/local prefix with a Relative Identifier that names a specific account or group |
| Multi-format reporting | The same enumeration data can be rendered as JSON (machine-readable), CSV (spreadsheet-friendly), and HTML (human-readable) |
| Security review | Flagging default accounts, service accounts, and privileged groups supports account-hygiene audits |
| Practical use case | These techniques underpin authorized penetration testing, security auditing, and incident-response investigations |

<div align="center">

---

### 🎓 Al Nafi Cybersecurity Training Platform
*Empowering the next generation of cybersecurity professionals*

![Al Nafi](https://img.shields.io/badge/Al%20Nafi-Cybersecurity%20Labs-blueviolet?style=for-the-badge)

</div>
