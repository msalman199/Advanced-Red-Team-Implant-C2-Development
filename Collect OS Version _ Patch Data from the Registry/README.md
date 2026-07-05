<div align="center">

# 🗂️ Collect OS Version & Patch Data from the Registry

### Windows Registry Forensics & Cross-Platform System Inventory 

![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Wine](https://img.shields.io/badge/Wine-722F37?style=for-the-badge&logo=wine&logoColor=white)
![MinGW](https://img.shields.io/badge/MinGW--w64-8A2BE2?style=for-the-badge&logo=gnu&logoColor=white)
![GCC](https://img.shields.io/badge/GCC-A42E2B?style=for-the-badge&logo=gnu&logoColor=white)
![Bash](https://img.shields.io/badge/Bash-4EAA25?style=for-the-badge&logo=gnubash&logoColor=white)
![Difficulty](https://img.shields.io/badge/Difficulty-Intermediate-yellow?style=for-the-badge)

*A hands-on Al Nafi lab for programmatic OS version, patch level, and hardware inventory collection using the Windows Registry API — simulated on Linux with Wine and MinGW*

</div>

---

## 📑 Table of Contents

- [Learning Objectives](#-learning-objectives)
- [Prerequisites](#-prerequisites)
- [Lab Environment](#-lab-environment)
- [Task 1: Environment Setup and Tool Installation](#-task-1-environment-setup-and-tool-installation)
- [Task 2: Create Registry Access Program](#-task-2-create-registry-access-program)
- [Task 3: Compile and Test Programs](#-task-3-compile-and-test-programs)
- [Task 4: Create Enhanced Registry Scanner](#-task-4-create-enhanced-registry-scanner)
- [Task 5: Create Output Report Generator](#-task-5-create-output-report-generator)
- [Verification and Testing](#-verification-and-testing)
- [MITRE ATT&CK Mapping](#-mitre-attck-mapping)
- [Troubleshooting](#-troubleshooting)
- [Key Takeaways](#-key-takeaways)

---

## 🎯 Learning Objectives

| # | Objective |
|---|-----------|
| 1 | Understand Windows Registry structure and access methods |
| 2 | Write C programs using Windows Registry APIs |
| 3 | Extract OS version and patch information programmatically |
| 4 | Simulate Windows Registry operations on Linux using cross-platform tools |

## ✅ Prerequisites

| Requirement | Details |
|---|---|
| 💻 Programming | Basic C programming knowledge |
| 🖥️ OS Concepts | Understanding of operating system fundamentals |
| ⌨️ CLI Skills | Familiarity with the Linux command line |
| 🔧 Build Tools | General knowledge of compilation processes |

## 🌐 Lab Environment

> Al Nafi provides a dedicated **bare-metal Linux cloud machine** for this lab — no pre-installed tools. Click **Start Lab** to begin; every component is installed from scratch during the walkthrough.

---

## 🧩 Task 1: Environment Setup and Tool Installation

![Step](https://img.shields.io/badge/Step-1-blue?style=flat-square) ![Setup](https://img.shields.io/badge/Type-Environment_Setup-informational?style=flat-square)

### Subtask 1.1: Install Development Tools

```bash
# 📦 Update package lists and install core build tools
sudo apt update
sudo apt install -y gcc make git wget curl

# 🪟 Install cross-compiler and Windows runtime layer
sudo apt install -y mingw-w64 wine64
```

### Subtask 1.2: Install Wine for Windows API Simulation

```bash
# ⚙️ Launch Wine configuration utility
winecfg
```
> 🖱️ Select **Windows 10** as the Windows version, then click **OK**.

### Subtask 1.3: Create Project Directory

```bash
# 📁 TODO: Create and enter your working directory
mkdir ~/registry_lab
cd ~/registry_lab
```

---

## 🧩 Task 2: Create Registry Access Program

![Step](https://img.shields.io/badge/Step-2-blue?style=flat-square) ![Coding](https://img.shields.io/badge/Type-C_Programming-orange?style=flat-square)

### Subtask 2.1: Write the Main Registry Reader

```bash
# 📝 Create the source file
nano registry_reader.c
```

```c
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

// 🔍 Reads ProductName, CurrentVersion, CurrentBuild, and ReleaseId
void printOSVersion() {
    HKEY hKey;
    DWORD dwType = REG_SZ;
    DWORD dwSize = 256;
    char szValue[256];

    printf("=== Operating System Information ===\n");

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                     "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
                     0, KEY_READ, &hKey) == ERROR_SUCCESS) {

        dwSize = 256;
        if (RegQueryValueEx(hKey, "ProductName", NULL, &dwType,
                           (LPBYTE)szValue, &dwSize) == ERROR_SUCCESS) {
            printf("Product Name: %s\n", szValue);
        }

        // TODO: Add RegQueryValueEx calls for "CurrentVersion",
        //       "CurrentBuild", and "ReleaseId" following the pattern above

        RegCloseKey(hKey);
    } else {
        printf("Failed to open OS version registry key\n");
    }
}

// 📋 Enumerates installed KB updates under the Uninstall key
void printInstalledUpdates() {
    HKEY hKey;
    DWORD dwIndex = 0;
    char szKeyName[256];
    DWORD dwKeyNameSize = 256;
    FILETIME ftLastWriteTime;

    printf("\n=== Installed Updates ===\n");

    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                     "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
                     0, KEY_READ, &hKey) == ERROR_SUCCESS) {

        printf("Enumerating installed updates...\n");

        // TODO: Implement the RegEnumKeyEx loop to walk every subkey,
        //       filter entries whose name starts with "KB", and print them

        RegCloseKey(hKey);
    } else {
        printf("Failed to open updates registry key\n");
    }
}

// 🖥️ Reads processor name and computer name
void printSystemInfo() {
    HKEY hKey;
    DWORD dwType = REG_SZ;
    DWORD dwSize = 256;
    char szValue[256];

    printf("\n=== System Information ===\n");

    // TODO: Open HARDWARE\DESCRIPTION\System\CentralProcessor\0
    //       and query "ProcessorNameString"

    // TODO: Open SYSTEM\CurrentControlSet\Control\ComputerName\ComputerName
    //       and query "ComputerName"
}

int main() {
    printf("Windows Registry OS Version and Patch Data Collector\n");
    printf("====================================================\n\n");

    printOSVersion();
    printInstalledUpdates();
    printSystemInfo();

    printf("\n=== Registry Data Collection Complete ===\n");
    return 0;
}
```

### Subtask 2.2: Create Linux Alternative Version

```bash
# 📝 Create the Linux-native comparison program
nano linux_system_info.c
```

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// 🐧 Parses /etc/os-release and uname for OS + kernel info
void printLinuxOSInfo() {
    FILE *fp;
    char buffer[256];

    printf("=== Linux System Information ===\n");

    fp = fopen("/etc/os-release", "r");
    if (fp != NULL) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strncmp(buffer, "PRETTY_NAME=", 12) == 0) {
                printf("OS: %s", buffer + 13);
            } else if (strncmp(buffer, "VERSION=", 8) == 0) {
                printf("Version: %s", buffer + 9);
            }
        }
        fclose(fp);
    }

    // TODO: Use popen("uname -r", "r") to capture and print the kernel version
}

// 📦 Lists a sample of installed dpkg packages
void printInstalledPackages() {
    FILE *fp;
    char buffer[256];
    int count = 0;

    printf("\n=== Installed Packages (Sample) ===\n");

    // TODO: popen("dpkg -l | grep '^ii' | head -10", "r") and print each line
}

// 🔧 Prints CPU, memory, and hostname details
void printSystemSpecs() {
    FILE *fp;
    char buffer[256];

    printf("\n=== Hardware Information ===\n");

    // TODO: Query /proc/cpuinfo for "model name" and /proc/meminfo for "MemTotal"

    if (gethostname(buffer, sizeof(buffer)) == 0) {
        printf("Hostname: %s\n", buffer);
    }
}

int main() {
    printf("Linux System Information Collector\n");
    printf("==================================\n\n");

    printLinuxOSInfo();
    printInstalledPackages();
    printSystemSpecs();

    printf("\n=== System Data Collection Complete ===\n");
    return 0;
}
```

---

## 🧩 Task 3: Compile and Test Programs

![Step](https://img.shields.io/badge/Step-3-blue?style=flat-square) ![Build](https://img.shields.io/badge/Type-Compilation-success?style=flat-square)

### Subtask 3.1: Compile Windows Version

```bash
# 🪟 Cross-compile the registry reader for Windows using MinGW
x86_64-w64-mingw32-gcc -o registry_reader.exe registry_reader.c -ladvapi32
```

### Subtask 3.2: Compile Linux Version

```bash
# 🐧 Compile the native Linux system information program
gcc -o linux_system_info linux_system_info.c
```

### Subtask 3.3: Test Linux Version

```bash
# ▶️ Run the Linux collector
./linux_system_info
```

### Subtask 3.4: Test Windows Version (Simulation)

```bash
# 🍷 Run the Windows binary through Wine
wine registry_reader.exe
```
> ℹ️ The Windows version may show limited data in Wine simulation, but it demonstrates the registry access methodology.

---

## 🧩 Task 4: Create Enhanced Registry Scanner

![Step](https://img.shields.io/badge/Step-4-blue?style=flat-square) ![Advanced](https://img.shields.io/badge/Type-Error_Handling-red?style=flat-square)

### Subtask 4.1: Create Advanced Scanner

```bash
nano advanced_registry.c
```

```c
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char name[256];
    char value[512];
} RegistryEntry;

// 🔎 Opens a key and either reads a single named value or enumerates all values
int scanRegistryKey(HKEY hRootKey, const char* subKey, const char* valueName) {
    HKEY hKey;
    DWORD dwType;
    DWORD dwSize = 512;
    char szValue[512];
    LONG result;

    result = RegOpenKeyEx(hRootKey, subKey, 0, KEY_READ, &hKey);

    if (result == ERROR_SUCCESS) {
        printf("Successfully opened key: %s\n", subKey);

        if (valueName) {
            result = RegQueryValueEx(hKey, valueName, NULL, &dwType,
                                   (LPBYTE)szValue, &dwSize);
            if (result == ERROR_SUCCESS) {
                printf("  %s: %s\n", valueName, szValue);
            } else {
                printf("  Failed to read value: %s (Error: %ld)\n", valueName, result);
            }
        } else {
            // TODO: Implement the RegEnumValue loop to print every value
            //       under this key (see dwIndex pattern from earlier tasks)
        }

        RegCloseKey(hKey);
        return 1;
    } else {
        printf("Failed to open key: %s (Error: %ld)\n", subKey, result);
        return 0;
    }
}

int main() {
    printf("Advanced Windows Registry Scanner\n");
    printf("=================================\n\n");

    printf("--- OS Version Information ---\n");
    scanRegistryKey(HKEY_LOCAL_MACHINE,
                   "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion",
                   "ProductName");

    // TODO: Call scanRegistryKey again for "CurrentVersion" and "CurrentBuild"

    printf("\n--- Windows Update Information ---\n");
    // TODO: Call scanRegistryKey with NULL valueName against
    //       SOFTWARE\Microsoft\Windows\CurrentVersion\WindowsUpdate\Auto Update

    printf("\n=== Registry Scan Complete ===\n");
    return 0;
}
```

### Subtask 4.2: Compile Advanced Version

```bash
x86_64-w64-mingw32-gcc -o advanced_registry.exe advanced_registry.c -ladvapi32
```

### Subtask 4.3: Test Advanced Version

```bash
wine advanced_registry.exe
```

---

## 🧩 Task 5: Create Output Report Generator

![Step](https://img.shields.io/badge/Step-5-blue?style=flat-square) ![Reporting](https://img.shields.io/badge/Type-Reporting-purple?style=flat-square)

### Subtask 5.1: Create Report Generator

```bash
nano generate_report.sh
```

```bash
#!/bin/bash

echo "System Information Report" > system_report.txt
echo "=========================" >> system_report.txt
echo "Generated on: $(date)" >> system_report.txt
echo "" >> system_report.txt

echo "--- Linux System Information ---" >> system_report.txt
./linux_system_info >> system_report.txt

echo "" >> system_report.txt
echo "--- Windows Registry Simulation ---" >> system_report.txt
wine registry_reader.exe >> system_report.txt 2>/dev/null

echo "" >> system_report.txt
echo "--- Advanced Registry Scan ---" >> system_report.txt
wine advanced_registry.exe >> system_report.txt 2>/dev/null

echo "Report generated: system_report.txt"
```

### Subtask 5.2: Make Script Executable and Run

```bash
# 🔐 Grant execute permission and run the generator
chmod +x generate_report.sh
./generate_report.sh
```

### Subtask 5.3: View Generated Report

```bash
cat system_report.txt
```

---

## 🔬 Verification and Testing

![Step](https://img.shields.io/badge/Step-Verify-brightgreen?style=flat-square)

### Test Registry Access Methods

```bash
# 📄 Confirm both binaries were produced correctly
ls -la *.exe
ls -la linux_system_info
file registry_reader.exe
file linux_system_info
```

### Validate Output

```bash
echo "Testing Linux version:"
./linux_system_info | head -10

echo "Testing Windows simulation:"
wine registry_reader.exe 2>/dev/null | head -10
```

---

## 🛡️ MITRE ATT&CK Mapping

| Tactic | Technique | ID | Relevance to This Lab |
|---|---|---|---|
| Discovery | System Information Discovery | T1082 | Collecting OS version, build, and hardware details |
| Discovery | Query Registry | T1012 | Reading OS/patch data directly from registry hives |
| Discovery | Software Discovery | T1518 | Enumerating installed updates and packages |

> 📌 This mapping is provided for **defensive awareness and detection-engineering context**. The techniques covered here reflect legitimate system inventory and asset-management activity; the same discovery primitives are also used by defenders to baseline systems and by adversaries during post-compromise reconnaissance, which is why detection teams monitor registry-enumeration activity.

---

## 🧯 Troubleshooting

<details>
<summary><strong>Click to expand common issues and fixes</strong></summary>

| Issue | Cause | Fix |
|---|---|---|
| Wine not configured | `winecfg` never run or misconfigured | Run `winecfg` and select **Windows 10** |
| Compilation errors | MinGW toolchain missing or incomplete | Reinstall with `sudo apt install -y mingw-w64` |
| Registry access denied | Expected behavior under Wine simulation | No action needed — Wine's registry is a simulated hive, not a real Windows system |
| Missing libraries | `mingw-w64` package not installed | Run `sudo apt install -y mingw-w64` and recompile |

</details>

---

## 🔑 Key Takeaways

| Concept | Summary |
|---|---|
| Registry APIs | `RegOpenKeyEx`, `RegQueryValueEx`, and `RegEnumKeyEx`/`RegEnumValue` are the core Win32 calls for reading registry data |
| Cross-compilation | `mingw-w64` lets Linux build native Windows `.exe` binaries without a Windows host |
| Simulation vs. reality | Wine approximates the Windows registry for testing but is not a substitute for validation on real Windows systems |
| Practical use case | Automated OS/patch inventory supports compliance auditing, vulnerability management, and IT asset tracking |

<div align="center">

---

### 🎓 Al Nafi Cybersecurity Training Platform
*Empowering the next generation of cybersecurity professionals*

![Al Nafi](https://img.shields.io/badge/Al%20Nafi-Cybersecurity%20Labs-blueviolet?style=for-the-badge)

</div>
