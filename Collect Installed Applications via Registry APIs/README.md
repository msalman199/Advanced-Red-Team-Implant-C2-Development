<div align="center">

# 🗂️ Collect Installed Applications via Registry APIs

### Windows Software Inventory & Forensic Analysis with Python

![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![Python](https://img.shields.io/badge/Python-3776AB?style=for-the-badge&logo=python&logoColor=white)
![JSON](https://img.shields.io/badge/JSON-000000?style=for-the-badge&logo=json&logoColor=white)
![Impacket](https://img.shields.io/badge/Impacket-4B0082?style=for-the-badge&logo=python&logoColor=white)
![Samba](https://img.shields.io/badge/Samba-0086CB?style=for-the-badge&logo=samba&logoColor=white)
![Difficulty](https://img.shields.io/badge/Difficulty-Intermediate-yellow?style=for-the-badge)

*A hands-on Al Nafi lab for parsing Windows registry uninstall keys, extracting software inventory data, and building forensic analysis and reporting tooling in Python*

</div>

---

## 📑 Table of Contents

- [Learning Objectives](#-learning-objectives)
- [Prerequisites](#-prerequisites)
- [Lab Environment](#-lab-environment)
- [Task 1: Set Up Registry Query Environment](#-task-1-set-up-registry-query-environment)
- [Task 2: Query Registry for Installed Software Keys](#-task-2-query-registry-for-installed-software-keys)
- [Task 3: Output Software Details](#-task-3-output-software-details)
- [Verification and Testing](#-verification-and-testing)
- [MITRE ATT&CK Mapping](#-mitre-attck-mapping)
- [Troubleshooting](#-troubleshooting)
- [Key Takeaways](#-key-takeaways)

---

## 🎯 Learning Objectives

| # | Objective |
|---|-----------|
| 1 | Understand Windows registry structure for installed applications |
| 2 | Use open-source tools to query Windows registry remotely from Linux |
| 3 | Extract and format software installation data |
| 4 | Implement registry API calls for forensic analysis |

## ✅ Prerequisites

| Requirement | Details |
|---|---|
| ⌨️ CLI Skills | Basic Linux command-line knowledge |
| 🗂️ Registry Concepts | Understanding of Windows registry concepts |
| 🐍 Scripting | Familiarity with Python scripting |
| 🌐 Networking | Network connectivity concepts |

## 🌐 Lab Environment

> Al Nafi provides a dedicated **bare-metal Linux cloud machine** for this lab — no pre-installed tools. Click **Start Lab** to begin; every component is installed from scratch during the walkthrough.

---

## 🧩 Task 1: Set Up Registry Query Environment

![Step](https://img.shields.io/badge/Step-1-blue?style=flat-square) ![Setup](https://img.shields.io/badge/Type-Environment_Setup-informational?style=flat-square)

### Subtask 1.1: Install Required Tools

```bash
# 📦 Update system packages
sudo apt update

# 🐍 Install Python and pip
sudo apt install -y python3 python3-pip

# 📚 Install registry manipulation libraries
pip3 install python-registry winreg-kb impacket-scripts

# 🔧 Install additional utilities
sudo apt install -y samba-common-bin
```

### Subtask 1.2: Create Working Directory

```bash
# 📁 TODO: Create your lab directory
mkdir ~/registry-lab
cd ~/registry-lab

# 📁 TODO: Create subdirectories for organization
mkdir scripts output samples
```

---

## 🧩 Task 2: Query Registry for Installed Software Keys

![Step](https://img.shields.io/badge/Step-2-blue?style=flat-square) ![Coding](https://img.shields.io/badge/Type-Python_Scripting-orange?style=flat-square)

### Subtask 2.1: Create Registry Query Script

```bash
# 📝 Create the source file
nano scripts/registry_query.py
```

```python
#!/usr/bin/env python3
import os
import json
from datetime import datetime

class RegistryParser:
    def __init__(self):
        self.software_keys = [
            "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
            "SOFTWARE\\WOW6432Node\\Microsoft\\Windows\\CurrentVersion\\Uninstall"
        ]
        self.installed_apps = []

    def parse_registry_file(self, registry_file):
        """Parse Windows registry file for installed applications"""
        try:
            from Registry import Registry

            reg = Registry.Registry(registry_file)

            for key_path in self.software_keys:
                try:
                    uninstall_key = reg.open(key_path)
                    self.extract_software_info(uninstall_key)
                except Registry.RegistryKeyNotFoundException:
                    print(f"Key not found: {key_path}")
                    continue

        except Exception as e:
            print(f"Error parsing registry: {e}")

    def extract_software_info(self, uninstall_key):
        """Extract software information from uninstall keys"""
        for subkey in uninstall_key.subkeys():
            app_info = {
                'name': 'Unknown',
                'version': 'Unknown',
                'publisher': 'Unknown',
                'install_date': 'Unknown',
                'install_location': 'Unknown',
                'registry_key': subkey.name()
            }

            try:
                # TODO: Loop over subkey.values() and populate app_info
                #       fields for DisplayName, DisplayVersion, Publisher,
                #       InstallDate, and InstallLocation

                if app_info['name'] != 'Unknown':
                    self.installed_apps.append(app_info)

            except Exception as e:
                print(f"Error processing subkey {subkey.name()}: {e}")

    def save_results(self, output_file):
        """Save extracted software information to JSON file"""
        with open(output_file, 'w') as f:
            json.dump(self.installed_apps, f, indent=2)

        print(f"Found {len(self.installed_apps)} installed applications")
        print(f"Results saved to: {output_file}")

def main():
    parser = RegistryParser()

    print("Registry Query Tool for Installed Applications")
    print("=" * 50)

    registry_file = input("Enter path to Windows registry file (or press Enter for demo): ").strip()

    if not registry_file:
        # 🧪 Demo dataset used when no live registry hive is supplied
        demo_apps = [
            {
                'name': 'Microsoft Office 365',
                'version': '16.0.14326.20404',
                'publisher': 'Microsoft Corporation',
                'install_date': '20231015',
                'install_location': 'C:\\Program Files\\Microsoft Office',
                'registry_key': '{90160000-0011-0000-1000-0000000FF1CE}'
            },
            {
                'name': 'Google Chrome',
                'version': '118.0.5993.88',
                'publisher': 'Google LLC',
                'install_date': '20231020',
                'install_location': 'C:\\Program Files\\Google\\Chrome',
                'registry_key': '{8A69D345-D564-463C-AFF1-A69D9E530F96}'
            },
            {
                'name': 'Adobe Acrobat Reader DC',
                'version': '23.006.20380',
                'publisher': 'Adobe Inc.',
                'install_date': '20231018',
                'install_location': 'C:\\Program Files\\Adobe\\Acrobat DC',
                'registry_key': '{AC76BA86-7AD7-1033-7B44-AC0F074E4100}'
            }
        ]

        parser.installed_apps = demo_apps
        print("Using demo data for illustration...")
    else:
        if os.path.exists(registry_file):
            parser.parse_registry_file(registry_file)
        else:
            print(f"Registry file not found: {registry_file}")
            return

    output_file = f"output/installed_apps_{datetime.now().strftime('%Y%m%d_%H%M%S')}.json"
    parser.save_results(output_file)

if __name__ == "__main__":
    main()
```

### Subtask 2.2: Make Script Executable

```bash
chmod +x scripts/registry_query.py
```

---

## 🧩 Task 3: Output Software Details

![Step](https://img.shields.io/badge/Step-3-blue?style=flat-square) ![Analysis](https://img.shields.io/badge/Type-Data_Analysis-purple?style=flat-square)

### Subtask 3.1: Create Output Formatter Script

```bash
nano scripts/format_output.py
```

```python
#!/usr/bin/env python3
import json
import sys
from datetime import datetime

class SoftwareReportGenerator:
    def __init__(self, json_file):
        self.json_file = json_file
        self.software_data = []
        self.load_data()

    def load_data(self):
        """Load software data from JSON file"""
        try:
            with open(self.json_file, 'r') as f:
                self.software_data = json.load(f)
        except Exception as e:
            print(f"Error loading data: {e}")
            sys.exit(1)

    def generate_summary_report(self):
        """Generate summary statistics"""
        print("\n" + "="*60)
        print("INSTALLED SOFTWARE SUMMARY REPORT")
        print("="*60)
        print(f"Total Applications Found: {len(self.software_data)}")
        print(f"Report Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
        print("="*60)

        # TODO: Build a `publishers` dict counting apps per publisher,
        #       then print the top 5 sorted by count

    def generate_detailed_report(self):
        """Generate detailed application listing"""
        print(f"\n{'='*60}")
        print("DETAILED APPLICATION LISTING")
        print("="*60)

        for i, app in enumerate(self.software_data, 1):
            print(f"\n[{i}] {app['name']}")
            print(f"    Version: {app['version']}")
            print(f"    Publisher: {app['publisher']}")
            print(f"    Install Date: {app['install_date']}")
            print(f"    Install Location: {app['install_location']}")
            print(f"    Registry Key: {app['registry_key']}")

    def export_csv(self, csv_file):
        """Export data to CSV format"""
        import csv

        fieldnames = ['name', 'version', 'publisher', 'install_date', 'install_location', 'registry_key']

        with open(csv_file, 'w', newline='') as f:
            writer = csv.DictWriter(f, fieldnames=fieldnames)
            writer.writeheader()
            writer.writerows(self.software_data)

        print(f"\nData exported to CSV: {csv_file}")

    def search_software(self, search_term):
        """Search for specific software"""
        results = []
        search_term = search_term.lower()

        # TODO: Iterate self.software_data and collect apps whose name
        #       or publisher contains search_term (case-insensitive)

        if results:
            print(f"\nSearch Results for '{search_term}':")
            print("-" * 40)
            for app in results:
                print(f"• {app['name']} v{app['version']} by {app['publisher']}")
        else:
            print(f"No applications found matching '{search_term}'")

def main():
    if len(sys.argv) < 2:
        print("Usage: python3 format_output.py <json_file>")
        sys.exit(1)

    json_file = sys.argv[1]
    generator = SoftwareReportGenerator(json_file)

    while True:
        print(f"\n{'='*40}")
        print("SOFTWARE ANALYSIS MENU")
        print("="*40)
        print("1. Summary Report")
        print("2. Detailed Report")
        print("3. Export to CSV")
        print("4. Search Software")
        print("5. Exit")

        choice = input("\nSelect option (1-5): ").strip()

        if choice == '1':
            generator.generate_summary_report()
        elif choice == '2':
            generator.generate_detailed_report()
        elif choice == '3':
            csv_file = f"output/software_export_{datetime.now().strftime('%Y%m%d_%H%M%S')}.csv"
            generator.export_csv(csv_file)
        elif choice == '4':
            search_term = input("Enter search term: ").strip()
            generator.search_software(search_term)
        elif choice == '5':
            print("Exiting...")
            break
        else:
            print("Invalid option. Please try again.")

if __name__ == "__main__":
    main()
```

### Subtask 3.2: Run the Registry Query Tool

```bash
cd ~/registry-lab
python3 scripts/registry_query.py
```
> ⏎ When prompted, press **Enter** to use demo data for this lab.

### Subtask 3.3: Format and Analyze Results

```bash
# 📄 Find the generated JSON file
ls -la output/

# ▶️ Run the formatter (replace with actual filename)
python3 scripts/format_output.py output/installed_apps_*.json
```

### Subtask 3.4: Create Registry Analysis Script

```bash
nano scripts/registry_analysis.py
```

```python
#!/usr/bin/env python3
import json
import re
from collections import Counter
from datetime import datetime

class RegistryAnalyzer:
    def __init__(self, json_file):
        with open(json_file, 'r') as f:
            self.apps = json.load(f)

    def analyze_installation_patterns(self):
        """Analyze installation date patterns"""
        print("\nINSTALLATION PATTERN ANALYSIS")
        print("-" * 40)

        dates = [app['install_date'] for app in self.apps if app['install_date'] != 'Unknown']

        years = []
        months = []

        # TODO: For each YYYYMMDD date string, split into year (first 4
        #       chars) and month (chars 4:6) and append to years/months

        if years:
            year_counts = Counter(years)
            month_counts = Counter(months)

            print("Installation by Year:")
            for year, count in sorted(year_counts.items()):
                print(f"  {year}: {count} installations")

            print("\nInstallation by Month:")
            month_names = {
                '01': 'Jan', '02': 'Feb', '03': 'Mar', '04': 'Apr',
                '05': 'May', '06': 'Jun', '07': 'Jul', '08': 'Aug',
                '09': 'Sep', '10': 'Oct', '11': 'Nov', '12': 'Dec'
            }

            for month, count in sorted(month_counts.items()):
                month_name = month_names.get(month, month)
                print(f"  {month_name}: {count} installations")

    def analyze_publishers(self):
        """Analyze software publishers"""
        print("\nPUBLISHER ANALYSIS")
        print("-" * 40)

        publishers = [app['publisher'] for app in self.apps if app['publisher'] != 'Unknown']
        publisher_counts = Counter(publishers)

        print("Top Software Publishers:")
        for publisher, count in publisher_counts.most_common(10):
            print(f"  {publisher}: {count} applications")

    def analyze_install_locations(self):
        """Analyze installation locations"""
        print("\nINSTALLATION LOCATION ANALYSIS")
        print("-" * 40)

        locations = [app['install_location'] for app in self.apps if app['install_location'] != 'Unknown']

        drives = []
        program_files = 0

        for location in locations:
            if location:
                if len(location) > 0 and location[1] == ':':
                    drives.append(location[0].upper())

                if 'Program Files' in location:
                    program_files += 1

        if drives:
            drive_counts = Counter(drives)
            print("Installation by Drive:")
            for drive, count in sorted(drive_counts.items()):
                print(f"  {drive}:\\ - {count} applications")

        print(f"\nProgram Files installations: {program_files}")

    def detect_suspicious_patterns(self):
        """Flag installation records worth a closer manual look"""
        print("\nITEMS WORTH REVIEWING")
        print("-" * 40)

        flagged = []

        for app in self.apps:
            # TODO: Flag entries installed under temp/appdata/user paths,
            #       entries missing a publisher, and other anomalies
            #       worth a human analyst's attention
            pass

        if flagged:
            print("Potential items of interest:")
            for item in flagged[:10]:
                print(f"  • {item}")
        else:
            print("No notable patterns detected.")

def main():
    import sys

    if len(sys.argv) < 2:
        print("Usage: python3 registry_analysis.py <json_file>")
        sys.exit(1)

    analyzer = RegistryAnalyzer(sys.argv[1])

    print("REGISTRY ANALYSIS REPORT")
    print("=" * 50)
    print(f"Generated: {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")

    analyzer.analyze_installation_patterns()
    analyzer.analyze_publishers()
    analyzer.analyze_install_locations()
    analyzer.detect_suspicious_patterns()

if __name__ == "__main__":
    main()
```

> 📌 **Note on naming:** the review step above is framed as *flagging entries for analyst review*, not as automated "malware detection" — registry-based indicators alone (missing publisher, install path) are weak signals and should always be corroborated with other evidence before action is taken.

### Subtask 3.5: Run Advanced Analysis

```bash
chmod +x scripts/registry_analysis.py
python3 scripts/registry_analysis.py output/installed_apps_*.json
```

---

## 🔬 Verification and Testing

![Step](https://img.shields.io/badge/Step-Verify-brightgreen?style=flat-square)

### Test Registry Query Functionality

```bash
# 📄 Verify all scripts are executable
ls -la scripts/

# ✅ Test the complete workflow
echo "Testing complete registry analysis workflow..."

# ▶️ Run query tool
python3 scripts/registry_query.py

# ▶️ Run formatter
python3 scripts/format_output.py output/installed_apps_*.json

# ▶️ Run analysis
python3 scripts/registry_analysis.py output/installed_apps_*.json
```

### Create Summary Report

```bash
# 📝 Generate final summary
echo "REGISTRY LAB COMPLETION SUMMARY" > output/lab_summary.txt
echo "================================" >> output/lab_summary.txt
echo "Date: $(date)" >> output/lab_summary.txt
echo "Files created:" >> output/lab_summary.txt
ls -la output/ >> output/lab_summary.txt
echo "" >> output/lab_summary.txt
echo "Scripts created:" >> output/lab_summary.txt
ls -la scripts/ >> output/lab_summary.txt

cat output/lab_summary.txt
```

---

## 🛡️ MITRE ATT&CK Mapping

| Tactic | Technique | ID | Relevance to This Lab |
|---|---|---|---|
| Discovery | Software Discovery | T1518 | Enumerating installed applications from registry uninstall keys |
| Discovery | Query Registry | T1012 | Reading software metadata directly from registry hives |
| Collection | Data from Local System | T1005 | Aggregating installation data into structured JSON/CSV reports |

> 📌 This mapping is provided for **defensive awareness and detection-engineering context**. Registry-based software inventory is a standard forensic and asset-management technique; the same registry keys are also reviewed by incident responders when reconstructing what was installed on a system and when.

---

## 🧯 Troubleshooting

<details>
<summary><strong>Click to expand common issues and fixes</strong></summary>

| Issue | Cause | Fix |
|---|---|---|
| `python-registry` import fails | Package not installed or wrong Python environment | Rerun `pip3 install python-registry winreg-kb impacket-scripts` |
| No output files in `output/` | Script run from wrong directory or crashed early | Confirm you're in `~/registry-lab` and check console output for exceptions |
| CSV export missing fields | JSON records have unexpected keys | Verify the source JSON matches the expected `RegistryParser` schema |
| Wildcard `installed_apps_*.json` not matching | Multiple runs created several timestamped files | List `output/` and pass the exact filename to the script |

</details>

---

## 🔑 Key Takeaways

| Concept | Summary |
|---|---|
| Registry structure | Installed software lives under `...\CurrentVersion\Uninstall`, with one subkey per application |
| Cross-platform parsing | `python-registry` lets Linux hosts parse offline Windows registry hives without booting Windows |
| Structured output | JSON and CSV exports make inventory data easy to feed into other tools or spreadsheets |
| Analytical layering | Separate query, formatting, and analysis scripts keep each stage of the pipeline testable |
| Practical use case | This workflow supports digital forensics, compliance auditing, and enterprise software inventory management |

<div align="center">

---

### 🎓 Al Nafi Cybersecurity Training Platform
*Empowering the next generation of cybersecurity professionals*

![Al Nafi](https://img.shields.io/badge/Al%20Nafi-Cybersecurity%20Labs-blueviolet?style=for-the-badge)

</div>
