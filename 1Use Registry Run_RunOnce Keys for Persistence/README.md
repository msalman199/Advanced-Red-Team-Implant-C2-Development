<div align="center">

# 🔑 Registry Run/RunOnce Keys Persistence
## Windows Registry-Based Persistence on Linux Simulation

![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![Wine](https://img.shields.io/badge/Wine-Emulation-darkred?style=for-the-badge)
![Python3](https://img.shields.io/badge/Python3-3776AB?style=for-the-badge&logo=python&logoColor=white)
![Registry Persistence](https://img.shields.io/badge/Registry%20Persistence-Technique-orange?style=for-the-badge)
![Windows Simulation](https://img.shields.io/badge/Windows%20Simulation-Advanced-red?style=for-the-badge)
![Malware Analysis](https://img.shields.io/badge/Malware%20Analysis-Forensics-purple?style=for-the-badge)
![Al Nafi](https://img.shields.io/badge/Al%20Nafi-Training-blue?style=for-the-badge)

**Master Windows registry persistence techniques using Run/RunOnce keys in a controlled Linux environment**

[Objectives](#-objectives) • [Prerequisites](#-prerequisites) • [Tasks](#-tasks) • [Verification](#-verification) • [Key Takeaways](#-key-takeaways)

</div>

---

## 📋 Table of Contents

1. [Objectives](#-objectives)
2. [Prerequisites](#-prerequisites)
3. [Lab Environment](#-lab-environment)
4. [Task 1: Windows Simulation Setup](#-task-1-windows-simulation-setup)
5. [Task 2: Registry Persistence Scripts](#-task-2-registry-persistence-scripts)
6. [Task 3: Verification & Testing](#-task-3-verification--testing)
7. [Task 4: Analysis & Cleanup](#-task-4-analysis--cleanup)
8. [Troubleshooting](#-troubleshooting)
9. [Key Takeaways](#-key-takeaways)
10. [MITRE ATT&CK Mapping](#-mitre-attck-mapping)

---

## 🎯 Objectives

| # | Objective | Status |
|---|-----------|--------|
| 1️⃣ | Understand Windows registry persistence mechanisms (Run/RunOnce) | ☐ |
| 2️⃣ | Create registry-based persistence in simulated environment | ☐ |
| 3️⃣ | Verify persistence across simulated system reboots | ☐ |
| 4️⃣ | Analyze registry modifications for security assessment | ☐ |
| 5️⃣ | Master defensive detection of registry persistence | ☐ |

---

## ✅ Prerequisites

| Prerequisite | Level | Details |
|--------------|-------|---------|
| 🪟 Windows Registry Knowledge | Intermediate | Registry structure, keys, values, HKCU/HKLM |
| 🐧 Linux Command-Line | Intermediate | File operations, bash, subprocess execution |
| 🍷 Virtualization Concepts | Beginner | Wine emulation, Windows environment simulation |
| 🔒 Persistence Techniques | Intermediate | Understanding of malware persistence mechanisms |
| 🐍 Python Programming | Beginner | Subprocess, file I/O, basic scripting |

---

## 🏗️ Lab Environment

<div align="center">

**Al Nafi provides Linux-based cloud machines for this lab**

</div>

- ✨ Dedicated bare-metal Linux environment (Ubuntu/Debian)
- 🍷 Wine emulation for Windows registry simulation
- 🔑 Full registry access via Wine's registry tools
- 🚀 Click **Start Lab** to begin
- ⚙️ All tools installed during lab walkthrough

---

## 🪟 Task 1: Windows Simulation Setup

### 🔹 Subtask 1.1: Install Required Tools

<span style="background-color: #4CAF50; color: white; padding: 3px 8px; border-radius: 3px;">**ESSENTIAL**</span>

Install Wine and registry tools to simulate Windows environment:

```bash
# 📦 Update system packages
sudo apt update
sudo apt install -y wine winetricks

# ✅ Verify installations
wine --version
winetricks --version

# 📍 Expected output:
# wine-x.x (Staging) x.x
# winetricks x.x
```

**🔧 Tool Purposes:**
- 🍷 **Wine** - Windows API emulation layer
- 🛠️ **winetricks** - Install Windows components and libraries
- 📋 **Registry Tools** - Windows reg.exe emulation

---

### 🔹 Subtask 1.2: Initialize Wine Environment

<span style="background-color: #2196F3; color: white; padding: 3px 8px; border-radius: 3px;">**CONFIGURATION**</span>

Set up Windows environment configuration:

```bash
# 🪟 Initialize Wine with default configuration
winecfg

# 📋 Configuration window will open - follow these steps:
# 1. In "Staging" tab: Leave defaults
# 2. In "Windows Version" tab:
#    - Select: Windows 10
#    - Click OK

# ✅ Verify Wine prefix created
ls -la ~/.wine/
# Expected directories:
# - drive_c (Windows C: drive)
# - system32 (System libraries)
# - userdef (User configuration)
```

**Configuration Details:**
- 🪟 **Windows Version**: Set to Windows 10 for compatibility
- 💾 **Wine Prefix**: Located at `~/.wine/`
- 🚗 **C: Drive**: Mapped to `~/.wine/drive_c/`

---

### 🔹 Subtask 1.3: Install Registry Editor

<span style="background-color: #FF9800; color: white; padding: 3px 8px; border-radius: 3px;">**SETUP REGISTRY TOOLS**</span>

Install and verify registry editor access:

```bash
# 📦 Install Windows regedit utility
winetricks regedit

# ✅ Verify registry editor works
wine regedit

# 📋 Registry editor window will open
# - You can browse HKEY_CURRENT_USER and HKEY_LOCAL_MACHINE
# - This verifies registry access is working
# - Close the window (no edits needed yet)

# 🔍 Check registry database
ls -la ~/.wine/system.reg
ls -la ~/.wine/user.reg
```

**Registry Structure:**
- 📋 `system.reg` - HKEY_LOCAL_MACHINE entries
- 👤 `user.reg` - HKEY_CURRENT_USER entries
- 🔑 Wine implements full registry API

---

## 📝 Task 2: Registry Persistence Scripts

### 🔹 Subtask 2.1: Create Persistence Payload

<span style="background-color: #9C27B0; color: white; padding: 3px 8px; border-radius: 3px;">**PAYLOAD CREATION**</span>

Create the payload script that will be executed via registry persistence:

```bash
# 📄 Create batch file payload
cat > persistence_payload.bat << 'EOF'
@echo off
REM 📝 Persistence Payload - Logs execution timestamp
echo Persistence payload executed at %date% %time% >> C:\persistence_log.txt
timeout /t 5 /nobreak > nul
EOF

# ✅ Convert to Windows line endings
unix2dos persistence_payload.bat
# or: dos2unix --keep-bom persistence_payload.bat

# 📂 Copy payload to Wine C: drive
cp persistence_payload.bat ~/.wine/drive_c/

# 🔍 Verify copy
ls -la ~/.wine/drive_c/persistence_payload.bat
file ~/.wine/drive_c/persistence_payload.bat
```

**Payload Details:**
- 📝 **Function**: Logs execution timestamp to file
- ⏱️ **Timeout**: 5-second delay for visibility
- 📋 **Output**: Creates `persistence_log.txt` on C: drive
- 🎯 **Purpose**: Proves persistence mechanism executed

---

### 🔹 Subtask 2.2: Create Registry Modification Script

<span style="background-color: #673AB7; color: white; padding: 3px 8px; border-radius: 3px;">**REGISTRY INSTALLER**</span>

Build the script to add registry persistence entries:

```bash
# 🔧 Create Python registry modification tool
cat > add_persistence.py << 'EOF'
#!/usr/bin/env python3
"""
🔑 Registry Persistence Installer
Adds Run and RunOnce keys to Windows registry via Wine
"""
import subprocess
import sys
import os

def add_run_key():
    """➰ Add persistent Run key (executes every boot)"""
    try:
        # 🔑 Registry path for HKEY_CURRENT_USER
        reg_path = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run"
        
        # 🔧 Command to add registry entry
        cmd = [
            'wine', 'reg', 'add', reg_path,
            '/v', 'SecurityUpdate',          # 📝 Value name (appears benign)
            '/t', 'REG_SZ',                  # 📋 Type: String
            '/d', 'C:\\persistence_payload.bat',  # 🎯 Data: path to payload
            '/f'                             # 🔄 Force (overwrite if exists)
        ]
        
        result = subprocess.run(cmd, capture_output=True, text=True)
        
        if result.returncode == 0:
            print("[✅] Successfully added Run key persistence")
            print(f"    Registry: {reg_path}\\SecurityUpdate")
            print(f"    Value: C:\\persistence_payload.bat")
        else:
            print(f"[❌] Failed to add registry entry: {result.stderr}")
            return False
        
        return True
        
    except Exception as e:
        print(f"[❌] Error: {e}")
        return False

def add_runonce_key():
    """🔐 Add one-time RunOnce key (executes once, then deletes)"""
    try:
        # 🔑 Registry path for HKEY_CURRENT_USER
        reg_path = "HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce"
        
        # 🔧 Command to add registry entry
        cmd = [
            'wine', 'reg', 'add', reg_path,
            '/v', 'SystemCheck',             # 📝 Value name (appears legitimate)
            '/t', 'REG_SZ',                  # 📋 Type: String
            '/d', 'C:\\persistence_payload.bat',  # 🎯 Data: path to payload
            '/f'                             # 🔄 Force (overwrite if exists)
        ]
        
        result = subprocess.run(cmd, capture_output=True, text=True)
        
        if result.returncode == 0:
            print("[✅] Successfully added RunOnce key persistence")
            print(f"    Registry: {reg_path}\\SystemCheck")
            print(f"    Value: C:\\persistence_payload.bat")
        else:
            print(f"[❌] Failed to add registry entry: {result.stderr}")
            return False
        
        return True
        
    except Exception as e:
        print(f"[❌] Error: {e}")
        return False

if __name__ == "__main__":
    print("╔════════════════════════════════════════════════════════╗")
    print("║        Registry Persistence Installation Tool           ║")
    print("╚════════════════════════════════════════════════════════╝")
    print()
    
    print("[🔧] Installing registry persistence mechanisms...")
    print()
    
    # ➰ Add Run key
    print("[1️⃣] Processing Run key (persistent across reboots):")
    run_success = add_run_key()
    
    print()
    
    # 🔐 Add RunOnce key
    print("[2️⃣] Processing RunOnce key (executes once only):")
    runonce_success = add_runonce_key()
    
    print()
    
    if run_success and runonce_success:
        print("[✅] Persistence installation complete!")
        print("[📍] Ready for verification testing")
    else:
        print("[⚠️] Some persistence entries failed to install")
        sys.exit(1)
EOF

# 🔧 Make script executable
chmod +x add_persistence.py

# 🔍 Verify script syntax
python3 -m py_compile add_persistence.py
echo "✅ Script syntax validated"
```

**🔑 Key Registry Concepts:**
- ➰ **Run** - Executes every time user logs in, persists across reboots
- 🔐 **RunOnce** - Executes once at next login, then auto-deletes
- 📝 **Value Name** - Disguised as legitimate Windows component
- 🎯 **Data** - Path to malicious payload executable
- 🔄 **Force Flag** - Overwrites existing values without prompting

---

### 🔹 Subtask 2.3: Execute Persistence Installation

<span style="background-color: #F44336; color: white; padding: 3px 8px; border-radius: 3px;">**DEPLOY**</span>

Install registry persistence entries:

```bash
# 🚀 Run persistence installation
python3 add_persistence.py

# 📊 Expected output:
# ✅ Successfully added Run key persistence
# ✅ Successfully added RunOnce key persistence
# ✅ Persistence installation complete!

# ✅ Verify Run key was added
echo "[🔍] Verifying Run key entry:"
wine reg query "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run"

# Expected to show:
# HKEY_CURRENT_USER\...\Run
#     SecurityUpdate    REG_SZ    C:\persistence_payload.bat

# ✅ Verify RunOnce key was added
echo "[🔍] Verifying RunOnce key entry:"
wine reg query "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\RunOnce"

# Expected to show:
# HKEY_CURRENT_USER\...\RunOnce
#     SystemCheck    REG_SZ    C:\persistence_payload.bat
```

**Installation Verification:**
- ✅ Both registry keys created successfully
- ✅ Payload path correctly configured
- ✅ Registry values are readable
- 📊 Ready for persistence testing

---

## 🧪 Task 3: Verification & Testing

### 🔹 Subtask 3.1: Create Verification Script

<span style="background-color: #00BCD4; color: white; padding: 3px 8px; border-radius: 3px;">**TESTING FRAMEWORK**</span>

Build script to simulate system startup and test persistence:

```bash
# 📋 Create verification and persistence tester
cat > verify_persistence.py << 'EOF'
#!/usr/bin/env python3
"""
🔄 Persistence Verification Tool
Simulates Windows startup and verifies persistence execution
"""
import subprocess
import time
import os

def simulate_startup():
    """🔄 Simulate Windows startup process"""
    print("╔════════════════════════════════════════════════════════╗")
    print("║        Simulating Windows Startup Sequence              ║")
    print("╚════════════════════════════════════════════════════════╝")
    print()
    
    # 🧹 Clear previous execution log
    log_path = os.path.expanduser("~/.wine/drive_c/persistence_log.txt")
    if os.path.exists(log_path):
        print("[🧹] Clearing previous log file...")
        os.remove(log_path)
        print("    ✅ Log cleared")
    
    print()
    
    # ➰ Process Run key entries
    print("[1️⃣] Processing Run key entries (persistent):")
    run_result = subprocess.run([
        'wine', 'reg', 'query', 
        'HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run'
    ], capture_output=True, text=True)
    
    if "SecurityUpdate" in run_result.stdout:
        print("    ✅ Run key persistence found")
        print("    ⚙️  Executing payload...")
        
        # 🎯 Execute the payload
        exec_result = subprocess.run(
            ['wine', 'cmd', '/c', 'C:\\persistence_payload.bat'],
            capture_output=True,
            text=True
        )
        
        if exec_result.returncode == 0:
            print("    ✅ Payload executed successfully")
        else:
            print(f"    ⚠️  Payload execution returned code {exec_result.returncode}")
    else:
        print("    ❌ Run key persistence NOT found")
    
    print()
    
    # 🔐 Process RunOnce key entries
    print("[2️⃣] Processing RunOnce key entries (one-time only):")
    runonce_result = subprocess.run([
        'wine', 'reg', 'query', 
        'HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce'
    ], capture_output=True, text=True)
    
    if "SystemCheck" in runonce_result.stdout:
        print("    ✅ RunOnce key persistence found")
        print("    ⚙️  Executing payload...")
        
        # 🎯 Execute the payload
        exec_result = subprocess.run(
            ['wine', 'cmd', '/c', 'C:\\persistence_payload.bat'],
            capture_output=True,
            text=True
        )
        
        if exec_result.returncode == 0:
            print("    ✅ Payload executed successfully")
        else:
            print(f"    ⚠️  Payload execution returned code {exec_result.returncode}")
        
        # 🗑️ Remove RunOnce entry (Windows behavior)
        print("    🗑️  Deleting RunOnce entry after execution...")
        subprocess.run([
            'wine', 'reg', 'delete',
            'HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce',
            '/v', 'SystemCheck', '/f'
        ], capture_output=True)
        print("    ✅ RunOnce entry removed")
    else:
        print("    ❌ RunOnce key persistence NOT found")

def check_persistence_log():
    """📖 Check if persistence payload executed"""
    log_path = os.path.expanduser("~/.wine/drive_c/persistence_log.txt")
    
    print()
    print("[📊] Checking execution results:")
    print()
    
    if os.path.exists(log_path):
        print("✅ Persistence log found!")
        print("━" * 50)
        with open(log_path, 'r') as f:
            content = f.read()
            print(content)
        print("━" * 50)
        return True
    else:
        print("❌ No persistence log found")
        print("   Persistence may not have executed")
        return False

if __name__ == "__main__":
    simulate_startup()
    time.sleep(2)
    success = check_persistence_log()
    
    if success:
        print("[✅] Persistence verification successful!")
    else:
        print("[⚠️] Persistence verification failed")
EOF

# 🔧 Make executable
chmod +x verify_persistence.py

# ✅ Verify syntax
python3 -m py_compile verify_persistence.py
```

**🧪 Verification Process:**
1. 🧹 Clear previous logs
2. ➰ Query Run key and execute payload if found
3. 🔐 Query RunOnce key and execute payload if found
4. 🗑️ Delete RunOnce entry (simulating Windows behavior)
5. 📖 Verify execution via log file creation

---

### 🔹 Subtask 3.2: Test First Boot

<span style="background-color: #1976D2; color: white; padding: 3px 8px; border-radius: 3px;">**FIRST EXECUTION**</span>

Simulate first system boot and verify persistence execution:

```bash
# 🎯 Run first boot simulation
echo "[🚀] Simulating first Windows boot..."
python3 verify_persistence.py

# 📊 Expected output should show:
# [1️⃣] Processing Run key entries (persistent)
#     ✅ Run key persistence found
#     ⚙️  Executing payload...
#     ✅ Payload executed successfully
#
# [2️⃣] Processing RunOnce key entries (one-time only)
#     ✅ RunOnce key persistence found
#     ⚙️  Executing payload...
#     ✅ Payload executed successfully
#     🗑️  Deleting RunOnce entry...
#
# [✅] Persistence log found!
# Persistence payload executed at [DATE] [TIME]
# Persistence payload executed at [DATE] [TIME]
```

**Test Results:**
- ✅ Both Run and RunOnce entries executed
- ✅ Log file created with two entries
- ✅ RunOnce entry removed automatically
- 📋 Persistence mechanisms working correctly

---

### 🔹 Subtask 3.3: Test Subsequent Boot

<span style="background-color: #E91E63; color: white; padding: 3px 8px; border-radius: 3px;">**SECOND EXECUTION**</span>

Simulate second boot and verify persistence behavior differences:

```bash
# 🔄 Run second boot simulation
echo "[🚀] Simulating second Windows boot..."
python3 verify_persistence.py

# 📊 Expected output should show:
# [1️⃣] Processing Run key entries (persistent)
#     ✅ Run key persistence found
#     ⚙️  Executing payload...
#     ✅ Payload executed successfully
#
# [2️⃣] Processing RunOnce key entries (one-time only)
#     ❌ RunOnce key persistence NOT found
#     (Because it was deleted after first execution)
#
# [✅] Persistence log found!
# Persistence payload executed at [DATE] [TIME]  (First boot)
# Persistence payload executed at [DATE] [TIME]  (Second boot)

# ✅ Verify RunOnce key is gone
echo "[🔍] Verifying RunOnce key status:"
wine reg query "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\RunOnce"
# Should show: The system was unable to find the specified registry key or value.

# ✅ Verify Run key still exists
echo "[🔍] Verifying Run key still exists:"
wine reg query "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run"
# Should show: SecurityUpdate REG_SZ C:\persistence_payload.bat
```

**Behavior Comparison:**
- ➰ **Run Key**: Executed on both boots (persistent)
- 🔐 **RunOnce Key**: Only executed on first boot, then deleted
- 📊 **Log File**: Contains two execution entries
- 🎯 **Key Difference**: Run for long-term persistence, RunOnce for temporary

---

## 🔍 Task 4: Analysis & Cleanup

### 🔹 Subtask 4.1: Create Analysis Script

<span style="background-color: #9C27B0; color: white; padding: 3px 8px; border-radius: 3px;">**FORENSIC ANALYSIS**</span>

Build tool to analyze registry persistence for security assessment:

```bash
# 📊 Create registry analysis tool
cat > analyze_persistence.py << 'EOF'
#!/usr/bin/env python3
"""
🔍 Registry Persistence Analysis Tool
Forensic analysis of registry-based persistence mechanisms
"""
import subprocess
import json

def analyze_registry():
    """🔎 Analyze all persistence registry keys"""
    print("╔════════════════════════════════════════════════════════╗")
    print("║       Registry Persistence Forensic Analysis            ║")
    print("╚════════════════════════════════════════════════════════╝")
    print()
    
    # ➰ Check Run keys
    print("[🔑] Analyzing Run Keys (Persistent):")
    print("━" * 50)
    
    run_keys = [
        ("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", 
         "Current User"),
        ("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\Run", 
         "Local Machine")
    ]
    
    for key_path, key_location in run_keys:
        print(f"\n📍 Location: {key_location}")
        print(f"   Path: {key_path}")
        
        result = subprocess.run([
            'wine', 'reg', 'query', key_path
        ], capture_output=True, text=True)
        
        if result.returncode == 0 and result.stdout.strip():
            print("   ✅ ENTRIES FOUND:")
            # Parse output
            for line in result.stdout.strip().split('\n')[1:]:
                if line.strip():
                    parts = line.split(None, 2)
                    if len(parts) >= 3:
                        value_name = parts[0]
                        value_type = parts[1]
                        value_data = parts[2] if len(parts) > 2 else ""
                        
                        print(f"      • {value_name}")
                        print(f"        Type: {value_type}")
                        print(f"        Data: {value_data}")
        else:
            print("   ❌ No entries found")
    
    print()
    print()
    
    # 🔐 Check RunOnce keys
    print("[🔑] Analyzing RunOnce Keys (One-Time Only):")
    print("━" * 50)
    
    runonce_keys = [
        ("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce", 
         "Current User"),
        ("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\RunOnce", 
         "Local Machine")
    ]
    
    for key_path, key_location in runonce_keys:
        print(f"\n📍 Location: {key_location}")
        print(f"   Path: {key_path}")
        
        result = subprocess.run([
            'wine', 'reg', 'query', key_path
        ], capture_output=True, text=True)
        
        if result.returncode == 0 and result.stdout.strip():
            print("   ✅ ENTRIES FOUND:")
            for line in result.stdout.strip().split('\n')[1:]:
                if line.strip():
                    parts = line.split(None, 2)
                    if len(parts) >= 3:
                        value_name = parts[0]
                        value_type = parts[1]
                        value_data = parts[2] if len(parts) > 2 else ""
                        
                        print(f"      • {value_name}")
                        print(f"        Type: {value_type}")
                        print(f"        Data: {value_data}")
        else:
            print("   ❌ No entries found")
    
    print()
    print()
    
    # 📊 Summary
    print("[📊] Persistence Assessment Summary:")
    print("━" * 50)
    print("🟢 Run keys: Used for long-term persistence")
    print("   - Executed every boot/login")
    print("   - Typical malware mechanism")
    print()
    print("🟡 RunOnce keys: Used for temporary persistence")
    print("   - Executed once, then auto-removed")
    print("   - Common in trojans and worms")
    print()
    print("[💡] Detection Recommendations:")
    print("   • Monitor Run and RunOnce keys regularly")
    print("   • Alert on suspicious value names")
    print("   • Verify all executable paths")
    print("   • Baseline legitimate Windows entries")

if __name__ == "__main__":
    analyze_registry()
EOF

# 🔧 Make executable
chmod +x analyze_persistence.py

# ✅ Verify syntax
python3 -m py_compile analyze_persistence.py
```

**🔍 Analysis Focus:**
- 🔑 Both HKCU and HKLM locations
- ✅ Value names and types
- 🎯 Executable paths
- 📊 Persistence duration assessment
- 💡 Detection recommendations

---

### 🔹 Subtask 4.2: Run Forensic Analysis

<span style="background-color: #FF6B6B; color: white; padding: 3px 8px; border-radius: 3px;">**ANALYSIS**</span>

Execute registry analysis and document findings:

```bash
# 🔍 Run forensic analysis
python3 analyze_persistence.py

# 📊 Expected output shows:
# [🔑] Analyzing Run Keys (Persistent):
# 📍 Location: Current User
#    ✅ ENTRIES FOUND:
#       • SecurityUpdate
#         Type: REG_SZ
#         Data: C:\persistence_payload.bat
#
# [🔑] Analyzing RunOnce Keys (One-Time Only):
# 📍 Location: Current User
#    ❌ No entries found  (Already deleted after first execution)

# 📋 Document findings
echo "=== Forensic Analysis Results ===" | tee analysis_report.txt
python3 analyze_persistence.py >> analysis_report.txt

# 🔍 View complete report
cat analysis_report.txt
```

**Analysis Findings:**
- ✅ Run key entry persists across reboots
- 🗑️ RunOnce entry removed after first execution
- 📊 Demonstrates difference in persistence mechanisms
- 💡 Shows how to detect each type

---

### 🔹 Subtask 4.3: Cleanup Persistence

<span style="background-color: #4CAF50; color: white; padding: 3px 8px; border-radius: 3px;">**REMEDIATION**</span>

Remove all persistence entries and artifacts:

```bash
# 🧹 Create cleanup tool
cat > cleanup_persistence.py << 'EOF'
#!/usr/bin/env python3
"""
🧹 Persistence Cleanup Tool
Remove all registry persistence entries and artifacts
"""
import subprocess
import os

def remove_persistence():
    """🗑️ Remove all persistence entries"""
    print("╔════════════════════════════════════════════════════════╗")
    print("║          Cleaning Up Persistence Artifacts              ║")
    print("╚════════════════════════════════════════════════════════╝")
    print()
    
    # 🗑️ Remove Run key entry
    print("[1️⃣] Removing Run key entry...")
    result = subprocess.run([
        'wine', 'reg', 'delete',
        'HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Run',
        '/v', 'SecurityUpdate', '/f'
    ], capture_output=True, text=True)
    
    if result.returncode == 0:
        print("    ✅ Run key entry removed")
    else:
        print("    ⚠️  Run key entry not found or already removed")
    
    print()
    
    # 🗑️ Remove payload file
    print("[2️⃣] Removing payload executable...")
    payload_path = os.path.expanduser("~/.wine/drive_c/persistence_payload.bat")
    if os.path.exists(payload_path):
        os.remove(payload_path)
        print("    ✅ Payload file removed")
    else:
        print("    ⚠️  Payload file not found")
    
    print()
    
    # 🗑️ Remove log file
    print("[3️⃣] Removing execution log...")
    log_path = os.path.expanduser("~/.wine/drive_c/persistence_log.txt")
    if os.path.exists(log_path):
        os.remove(log_path)
        print("    ✅ Log file removed")
    else:
        print("    ⚠️  Log file not found")
    
    print()
    print("[✅] Cleanup complete!")
    print("[📍] All persistence artifacts removed")

if __name__ == "__main__":
    remove_persistence()
EOF

# 🔧 Make executable
chmod +x cleanup_persistence.py

# 🧹 Execute cleanup
python3 cleanup_persistence.py

# ✅ Verify cleanup
echo "[🔍] Verifying cleanup..."
wine reg query "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run" | grep SecurityUpdate
# Should return: No results

ls -la ~/.wine/drive_c/persistence_payload.bat 2>&1
# Should return: No such file

ls -la ~/.wine/drive_c/persistence_log.txt 2>&1
# Should return: No such file

echo "[✅] Cleanup verification complete"
```

**Cleanup Steps:**
1. 🗑️ Delete Run key registry entry
2. 🗑️ Remove payload executable file
3. 🗑️ Remove execution log file
4. ✅ Verify complete removal

---

## 🛠️ Troubleshooting

<details>
<summary><b>❌ Wine not installed or not working</b></summary>

**Problem:** Wine command not found or returns errors

**Solutions:**

```bash
# 🍷 Check Wine installation
which wine
wine --version

# 📦 Reinstall if needed
sudo apt remove wine winetricks
sudo apt install -y wine winetricks

# 🔧 Verify Wine directory
ls -la ~/.wine/

# 💾 Reinitialize Wine prefix if corrupted
rm -rf ~/.wine
winecfg

# ✅ Test Wine functionality
wine cmd /c echo "Wine works"
```

</details>

<details>
<summary><b>❌ Registry editor fails to open</b></summary>

**Problem:** regedit installation or execution fails

**Solutions:**

```bash
# 🔧 Reinstall registry editor
winetricks regedit

# 🍷 Check Wine version
wine --version
# Ensure version is recent (8.0+)

# 💾 Verify registry files exist
ls -la ~/.wine/system.reg
ls -la ~/.wine/user.reg

# 🧪 Test with reg.exe instead
wine reg query HKEY_CURRENT_USER

# 🔄 Try reimporting registry
wine reg import ~/.wine/user.reg

# ⚙️ Check Wine configuration
winecfg
# Ensure Windows version is set correctly
```

</details>

<details>
<summary><b>⚠️ Registry entries created but not executing</b></summary>

**Problem:** Registry keys exist but payloads don't run

**Solutions:**

```bash
# 🔍 Verify registry entries
wine reg query "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\Run"

# 🧪 Test payload manually
wine cmd /c C:\persistence_payload.bat

# 🔍 Check payload location
ls -la ~/.wine/drive_c/persistence_payload.bat

# 📋 Verify line endings (should be Windows CRLF)
file ~/.wine/drive_c/persistence_payload.bat
# Should show: CRLF line terminators

# 🔧 Convert line endings if needed
unix2dos ~/.wine/drive_c/persistence_payload.bat

# 💾 Check if Wine can read/execute
wine notepad C:\persistence_payload.bat

# 🎯 Test direct execution
wine cmd /c "type C:\persistence_payload.bat"
```

</details>

<details>
<summary><b>❌ Persistence script errors</b></summary>

**Problem:** Python scripts fail with syntax or import errors

**Solutions:**

```bash
# ✅ Check Python version
python3 --version
# Should be Python 3.6+

# 🔍 Verify script syntax
python3 -m py_compile add_persistence.py
python3 -m py_compile verify_persistence.py

# 🧪 Run with verbose output
python3 -u add_persistence.py

# 📍 Check subprocess import
python3 -c "import subprocess; print('✅ subprocess available')"

# 🔧 Run with debugging
python3 -X dev add_persistence.py

# 💡 Check Wine command availability
which wine
wine reg /?
```

</details>

<details>
<summary><b>⚠️ Payload not creating log file</b></summary>

**Problem:** Persistence executes but no log file created

**Solutions:**

```bash
# 🔍 Check payload file content
cat ~/.wine/drive_c/persistence_payload.bat

# 🧪 Test payload execution directly
wine cmd /c C:\persistence_payload.bat

# 📋 Check C: drive location
ls -la ~/.wine/drive_c/

# 💾 Check permissions
chmod 777 ~/.wine/drive_c/

# 🔧 Create test log file manually
wine cmd /c "echo test > C:\test.txt"

# 📊 View Wine temp directory
ls -la ~/.wine/drive_c/users/$USER/AppData/Local/Temp/

# 🔄 Recreate payload with full path
cat > ~/.wine/drive_c/persistence_payload.bat << 'EOF'
@echo off
echo Persistence payload executed at %date% %time% >> C:\persistence_log.txt
type C:\persistence_log.txt
EOF
```

</details>

<details>
<summary><b>❌ RunOnce key not auto-deleting</b></summary>

**Problem:** RunOnce entry remains after execution

**Solutions:**

```bash
# 🔍 Verify RunOnce entry exists
wine reg query "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\RunOnce"

# 🧪 Test manual deletion
wine reg delete "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\RunOnce" /v SystemCheck /f

# 💡 Note: In real Windows, RunOnce auto-deletes
# Wine may not fully simulate this behavior

# 🔧 Manual workaround in script
# Add explicit deletion after RunOnce execution:
# wine reg delete ... /v SystemCheck /f

# ✅ Verify deletion
wine reg query "HKEY_CURRENT_USER\Software\Microsoft\Windows\CurrentVersion\RunOnce"
# Should show: No results
```

</details>

---

## 🎓 Key Takeaways

| Concept | Learning Point | Threat Actor Use | Detection Method |
|---------|-----------------|------------------|------------------|
| ➰ **Run Key** | Persistent auto-execution | Long-term malware persistence | Registry monitoring tools |
| 🔐 **RunOnce Key** | One-time auto-execution | Initial compromise foothold | Event log analysis |
| 📝 **Registry Values** | Obfuscated malware names | APT masquerading as Windows | Suspicious value names |
| 🎯 **Payload Path** | Execution location | Remote code execution | Process monitoring |
| 🔄 **Boot Persistence** | Survives reboots | Advanced threats (APT) | Startup behavior analysis |
| 🧹 **Cleanup** | Remove forensic evidence | Anti-forensics | Timeline reconstruction |
| 📊 **Registry Queries** | Discovery technique | Reconnaissance phase | Audit log review |

---

## 🎯 MITRE ATT&CK Mapping

| Tactic | Technique | Procedure | Detection |
|--------|-----------|-----------|-----------|
| **Persistence** | [T1547.001](https://attack.mitre.org/techniques/T1547/001/) | Registry Run Keys | Monitor registry changes |
| **Persistence** | [T1547.001](https://attack.mitre.org/techniques/T1547/001/) | Registry RunOnce Keys | Alert on new Run/RunOnce entries |
| **Execution** | [T1059.003](https://attack.mitre.org/techniques/T1059/003/) | Windows Command Shell | Track cmd.exe execution |
| **Privilege Escalation** | [T1547.001](https://attack.mitre.org/techniques/T1547/001/) | Boot Persistence | Monitor system startup |
| **Defense Evasion** | [T1112](https://attack.mitre.org/techniques/T1112/) | Modify Registry | Registry access auditing |

**Detection Strategies:**

```bash
# 🔍 Windows Event Log - Registry modifications
# Event ID 13: Registry value set
# Event ID 14: Registry object renamed
# Source: Microsoft-Windows-Sysmon/Operational

# 📊 Registry Monitoring
# Monitor: HKCU\Software\Microsoft\Windows\CurrentVersion\Run
# Monitor: HKCU\Software\Microsoft\Windows\CurrentVersion\RunOnce
# Monitor: HKLM\Software\Microsoft\Windows\CurrentVersion\Run
# Monitor: HKLM\Software\Microsoft\Windows\CurrentVersion\RunOnce

# 🚨 Detection Rules
# Alert on: New values in Run/RunOnce keys
# Alert on: Unsigned executables referenced
# Alert on: Non-standard Windows paths
# Alert on: Scheduled tasks modifications

# 📖 Log Sources
# Windows Event Log (Security)
# Sysmon (Event ID 13)
# Registry audit logs
# PowerShell logging
```

---

## 📚 Registry Keys Reference

| Registry Location | Scope | Persistence Type | Auto-Delete | User Level |
|-------------------|-------|------------------|-------------|------------|
| `HKCU\...\Run` | Current User | Persistent | No | User |
| `HKCU\...\RunOnce` | Current User | One-time | Yes | User |
| `HKLM\...\Run` | All Users | Persistent | No | System |
| `HKLM\...\RunOnce` | All Users | One-time | Yes | System |
| `HKCU\...\RunServices` | Legacy | Persistent | No | Deprecated |
| `HKLM\...\Notify` | System Events | Persistent | No | System |

---

## 🔐 Defensive Countermeasures

### Enable Registry Auditing

```bash
# 🪟 Windows Command (for reference)
auditpol /set /subcategory:"Registry" /success:enable /failure:enable

# 📊 Monitor registry changes with Sysmon
# Download Sysmon configuration with registry monitoring

# 🔍 PowerShell monitoring
Get-ItemProperty HKCU:\Software\Microsoft\Windows\CurrentVersion\Run
Get-ItemProperty HKCU:\Software\Microsoft\Windows\CurrentVersion\RunOnce
```

### Hardening Measures

1. 🔐 **Restrict Registry Access** - Use Group Policy to limit who can modify Run keys
2. 📋 **Whitelist Applications** - Only allow known Windows utilities in Run keys
3. 📊 **Regular Audits** - Scan Run/RunOnce keys for anomalies
4. 🚨 **Monitor Creation** - Alert on new registry value additions
5. 🛡️ **Application Whitelisting** - Prevent execution of unauthorized programs
6. 🔄 **Configuration Management** - Track baseline registry state

---

## 🔗 Advanced Topics

### Alternative Persistence Mechanisms

```bash
# 📋 Other Windows persistence methods to explore:

# 1️⃣ Scheduled Tasks
HKCU\Software\Microsoft\Windows NT\CurrentVersion\Schedule\TaskCache

# 2️⃣ WMI Event Subscriptions
HKCU\Software\Classes\__FilterToConsumerBinding

# 3️⃣ Browser Extensions
HKCU\Software\[Chrome|Firefox]\[Extensions]

# 4️⃣ COM Registry Hijacking
HKCU\Software\Classes\CLSID\...

# 5️⃣ Image File Execution Options
HKCU\Software\Microsoft\Windows NT\CurrentVersion\Image File Execution Options

# 6️⃣ Startup Folder
C:\Users\[User]\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\Startup
```

---

<div align="center">

## 📚 Al Nafi Cybersecurity Training Platform

**Master Windows persistence mechanisms for offensive and defensive security**

![Progress](https://img.shields.io/badge/Progress-Lab%20Complete-brightgreen?style=flat-square)
![Difficulty](https://img.shields.io/badge/Difficulty-Advanced-red?style=flat-square)
![Time](https://img.shields.io/badge/Estimated%20Time-120%20minutes-blue?style=flat-square)

**Next Steps:** Explore WMI event subscriptions, COM hijacking, and scheduled task persistence mechanisms

---

*Last Updated: July 2024 | Version: 1.0 | Certification: Al Nafi Red Team Ops - Persistence Techniques*

</div>
