<div align="center">

# 🔐 Privilege Escalation Module
## SeDebugPrivilege & Token Impersonation

![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![Privilege Escalation](https://img.shields.io/badge/Privilege%20Escalation-Advanced-red?style=for-the-badge)
![Windows Concepts](https://img.shields.io/badge/Windows%20Concepts-Simulation-orange?style=for-the-badge)
![Token Impersonation](https://img.shields.io/badge/Token%20Impersonation-Technique-purple?style=for-the-badge)
![Red Team](https://img.shields.io/badge/Red%20Team-Ops-darkred?style=for-the-badge)
![Al Nafi](https://img.shields.io/badge/Al%20Nafi-Training-blue?style=for-the-badge)

**Master privilege escalation techniques through SeDebugPrivilege simulation and token impersonation on Linux**

[Objectives](#-objectives) • [Prerequisites](#-prerequisites) • [Tasks](#-tasks) • [Verification](#-verification) • [Key Takeaways](#-key-takeaways)

</div>

---

## 📋 Table of Contents

1. [Objectives](#-objectives)
2. [Prerequisites](#-prerequisites)
3. [Lab Environment](#-lab-environment)
4. [Task 1: SeDebugPrivilege Escalation](#-task-1-sedebugprivilege-escalation)
5. [Task 2: Token Impersonation](#-task-2-token-impersonation)
6. [Verification & Testing](#-verification--testing)
7. [Troubleshooting](#-troubleshooting)
8. [Key Takeaways](#-key-takeaways)
9. [MITRE ATT&CK Mapping](#-mitre-attck-mapping)

---

## 🎯 Objectives

| # | Objective | Status |
|---|-----------|--------|
| 1️⃣ | Understand Windows privilege escalation concepts on Linux | ☐ |
| 2️⃣ | Implement SeDebugPrivilege escalation techniques | ☐ |
| 3️⃣ | Perform token impersonation to elevate privileges | ☐ |
| 4️⃣ | Create comprehensive privilege escalation module | ☐ |
| 5️⃣ | Analyze privilege context and security implications | ☐ |

---

## ✅ Prerequisites

| Prerequisite | Level | Details |
|--------------|-------|---------|
| 🐧 Linux System Administration | Intermediate | User/group management, permissions, UIDs/GIDs |
| 🖥️ C Programming | Intermediate | Functions, structs, system calls, error handling |
| 👤 Process Management | Intermediate | UID/GID concepts, privilege separation, process context |
| 📈 Privilege Escalation Concepts | Beginner | Understanding vertical privilege escalation |
| 🛠️ GCC & Make | Beginner | Compilation, Makefiles, linking |

---

## 🏗️ Lab Environment

<div align="center">

**Al Nafi provides Linux-based cloud machines for this lab**

</div>

- ✨ Dedicated bare-metal Linux environment
- 🔑 Root access for testing elevated operations
- 🚀 Click **Start Lab** to begin your session
- ⚙️ All tools installed during lab walkthrough

---

## 🔐 Task 1: SeDebugPrivilege Escalation

### 🔹 Subtask 1.1: Install Required Tools

<span style="background-color: #4CAF50; color: white; padding: 3px 8px; border-radius: 3px;">**ESSENTIAL**</span>

Set up your development environment with necessary tools:

```bash
# 📦 Update system packages
sudo apt update && sudo apt upgrade -y

# 💻 Install development tools
sudo apt install -y gcc make libc6-dev build-essential

# 📊 Install process monitoring tools
sudo apt install -y htop procps util-linux

# 📂 Create working directory
mkdir -p ~/privilege-escalation-lab
cd ~/privilege-escalation-lab

# ✅ Verify installations
gcc --version
make --version
```

**Expected Output:**
```
gcc (Ubuntu 11.x.x-...) 11.x.x
GNU Make x.x.x
```

---

### 🔹 Subtask 1.2: Create SeDebugPrivilege Simulation Module

<span style="background-color: #FF9800; color: white; padding: 3px 8px; border-radius: 3px;">**PRIVILEGE CHECK**</span>

Implement SeDebugPrivilege simulation that mimics Windows debugging capabilities:

```bash
cat > sedebug_privilege.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>
#include <errno.h>

// 🔐 Simulate SeDebugPrivilege check
int check_debug_privilege() {
    uid_t uid = getuid();
    uid_t euid = geteuid();
    
    printf("[+] Current UID: %d\n", uid);
    printf("[+] Effective UID: %d\n", euid);
    
    // 🔑 Check if running as root (simulates SeDebugPrivilege)
    if (euid == 0) {
        printf("[+] SeDebugPrivilege equivalent: ENABLED\n");
        return 1;
    } else {
        printf("[-] SeDebugPrivilege equivalent: DISABLED\n");
        return 0;
    }
}

// 🎯 Simulate process debugging capability
int debug_process(pid_t target_pid) {
    printf("[*] Attempting to debug process %d\n", target_pid);
    
    // 🔍 Check if process exists
    if (kill(target_pid, 0) == -1) {
        printf("[-] Process %d not found or access denied\n", target_pid);
        return 0;
    }
    
    printf("[+] Successfully accessed process %d\n", target_pid);
    return 1;
}

// ⬆️ Simulate privilege escalation attempt
void escalate_privileges() {
    printf("\n[*] Attempting privilege escalation...\n");
    
    if (check_debug_privilege()) {
        printf("[+] Debug privileges available\n");
        printf("[+] Can access system processes\n");
        printf("[+] Privilege escalation simulation successful\n");
    } else {
        printf("[-] Insufficient privileges for escalation\n");
    }
}

int main(int argc, char *argv[]) {
    printf("=== SeDebugPrivilege Escalation Module ===\n\n");
    
    // 📍 Display current privileges
    check_debug_privilege();
    
    // ⬆️ Attempt privilege escalation
    escalate_privileges();
    
    // 🧪 Test process access
    printf("\n[*] Testing process access capabilities...\n");
    debug_process(1); // Try to access init process
    
    return 0;
}
EOF
```

**🔑 Key Components:**
- 🆔 `getuid()` - Get real user ID
- ⚡ `geteuid()` - Get effective user ID (reflects actual privileges)
- 💀 `kill(pid, 0)` - Test process access without sending signal
- 🔐 Root equivalence simulates SeDebugPrivilege possession

---

### 🔹 Subtask 1.3: Compile and Test SeDebugPrivilege

<span style="background-color: #F44336; color: white; padding: 3px 8px; border-radius: 3px;">**BUILD & TEST**</span>

Compile the module and compare behavior at different privilege levels:

```bash
# 🔨 Compile the program
gcc -o sedebug_privilege sedebug_privilege.c

# ✅ Verify compilation
file sedebug_privilege
ls -lah sedebug_privilege

# 👤 Test as regular user
echo "=== Testing as regular user (unprivileged) ==="
./sedebug_privilege

# 🔓 Test with elevated privileges
echo -e "\n=== Testing with elevated privileges (root) ==="
sudo ./sedebug_privilege

# 📊 Compare outputs
echo -e "\n=== Privilege Comparison ==="
echo "Regular user EUID:"
./sedebug_privilege | grep "Effective UID"
echo "Root EUID:"
sudo ./sedebug_privilege | grep "Effective UID"
```

**Expected Output - Regular User:**
```
[+] Current UID: 1000
[+] Effective UID: 1000
[-] SeDebugPrivilege equivalent: DISABLED
```

**Expected Output - Root:**
```
[+] Current UID: 0
[+] Effective UID: 0
[+] SeDebugPrivilege equivalent: ENABLED
[+] Successfully accessed process 1
```

**✍️ TODO Items:**
- Document the privilege difference
- Explain why root can access init process
- Research Windows SeDebugPrivilege permissions

---

## 👤 Task 2: Token Impersonation

### 🔹 Subtask 2.1: Create Token Impersonation Module

<span style="background-color: #9C27B0; color: white; padding: 3px 8px; border-radius: 3px;">**TOKEN MANIPULATION**</span>

Implement token impersonation to demonstrate privilege context switching:

```bash
cat > token_impersonation.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

// 📋 Display current token information
void display_token_info() {
    uid_t uid = getuid();
    uid_t euid = geteuid();
    gid_t gid = getgid();
    gid_t egid = getegid();
    
    struct passwd *pw = getpwuid(uid);
    struct group *gr = getgrgid(gid);
    
    printf("[+] Token Information:\n");
    printf("    Real UID: %d (%s)\n", uid, pw ? pw->pw_name : "unknown");
    printf("    Effective UID: %d\n", euid);
    printf("    Real GID: %d (%s)\n", gid, gr ? gr->gr_name : "unknown");
    printf("    Effective GID: %d\n", egid);
}

// 🎭 Simulate token impersonation
int impersonate_token(uid_t target_uid) {
    printf("\n[*] Attempting token impersonation...\n");
    printf("[*] Target UID: %d\n", target_uid);
    
    // 🔐 Check current privileges
    if (geteuid() != 0) {
        printf("[-] Insufficient privileges for token impersonation\n");
        printf("    Current EUID: %d (need 0 for root)\n", geteuid());
        return 0;
    }
    
    // 🎭 Simulate token impersonation by changing effective UID
    if (seteuid(target_uid) == 0) {
        printf("[+] Token impersonation successful\n");
        display_token_info();
        
        // 🔄 Restore original privileges
        seteuid(0);
        printf("[+] Restored original token\n");
        return 1;
    } else {
        printf("[-] Token impersonation failed\n");
        return 0;
    }
}

// ⬆️ Simulate privilege escalation through token manipulation
void escalate_via_token() {
    printf("\n=== Token-Based Privilege Escalation ===\n");
    
    display_token_info();
    
    // 🧪 Attempt to impersonate different users
    printf("\n[*] Testing token impersonation capabilities...\n");
    
    // 👤 Try to impersonate user with UID 1000 (typical first user)
    impersonate_token(1000);
    
    // 🖥️ Try to impersonate system user
    impersonate_token(1);
}

int main() {
    printf("=== Token Impersonation Module ===\n\n");
    
    escalate_via_token();
    
    return 0;
}
EOF
```

**🔑 Key Functions:**
- 👤 `getuid()` / `geteuid()` - Get real and effective UIDs
- 👥 `getgid()` / `getegid()` - Get real and effective GIDs
- 👀 `getpwuid()` - Retrieve user information
- 🎭 `seteuid()` - Change effective UID (requires root)
- ℹ️ `struct passwd` - User account information structure

---

### 🔹 Subtask 2.2: Create Comprehensive Privilege Escalation Module

<span style="background-color: #673AB7; color: white; padding: 3px 8px; border-radius: 3px;">**ADVANCED ESCALATION**</span>

Combine multiple escalation techniques into a unified module:

```bash
cat > privilege_escalation_module.c << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <signal.h>
#include <string.h>

// 🔧 Structure to hold privilege context
typedef struct {
    uid_t uid;
    uid_t euid;
    gid_t gid;
    gid_t egid;
    int has_debug_privilege;
} privilege_context_t;

// 📊 Get current privilege context
privilege_context_t get_privilege_context() {
    privilege_context_t ctx;
    ctx.uid = getuid();
    ctx.euid = geteuid();
    ctx.gid = getgid();
    ctx.egid = getegid();
    ctx.has_debug_privilege = (ctx.euid == 0) ? 1 : 0;
    
    return ctx;
}

// 📋 Display privilege context
void display_context(privilege_context_t ctx) {
    struct passwd *pw = getpwuid(ctx.uid);
    
    printf("[+] Privilege Context:\n");
    printf("    User: %s (UID: %d)\n", pw ? pw->pw_name : "unknown", ctx.uid);
    printf("    Effective UID: %d\n", ctx.euid);
    printf("    Debug Privilege: %s\n", ctx.has_debug_privilege ? "ENABLED" : "DISABLED");
}

// ⬆️ Simulate comprehensive privilege escalation
int perform_escalation() {
    printf("\n[*] Performing comprehensive privilege escalation...\n");
    
    privilege_context_t ctx = get_privilege_context();
    
    if (!ctx.has_debug_privilege) {
        printf("[-] SeDebugPrivilege not available\n");
        printf("[-] Cannot perform token impersonation\n");
        return 0;
    }
    
    printf("[+] SeDebugPrivilege available\n");
    printf("[+] Token impersonation possible\n");
    
    // 🧠 Simulate accessing system processes
    printf("[+] Can access system processes\n");
    printf("[+] Can read process memory\n");
    printf("[+] Can inject code into processes\n");
    
    return 1;
}

// 🚀 Main escalation routine
void escalation_module() {
    printf("=== Comprehensive Privilege Escalation Module ===\n\n");
    
    privilege_context_t initial_ctx = get_privilege_context();
    printf("[*] Initial State:\n");
    display_context(initial_ctx);
    
    if (perform_escalation()) {
        printf("\n[+] Privilege escalation successful!\n");
        printf("[+] System-level access achieved\n");
    } else {
        printf("\n[-] Privilege escalation failed\n");
        printf("[-] Insufficient initial privileges\n");
    }
}

int main() {
    escalation_module();
    return 0;
}
EOF
```

**🏗️ Module Structure:**
- `privilege_context_t` - Data structure for privilege state
- `get_privilege_context()` - Snapshot current permissions
- `perform_escalation()` - Execute escalation logic
- Combined SeDebugPrivilege + Token Impersonation workflow

---

### 🔹 Subtask 2.3: Compile and Test All Modules

<span style="background-color: #2196F3; color: white; padding: 3px 8px; border-radius: 3px;">**COMPILE & VALIDATE**</span>

Build all three modules and test escalation behavior:

```bash
# 🔨 Compile token impersonation module
gcc -o token_impersonation token_impersonation.c

# 🔨 Compile comprehensive module
gcc -o privilege_escalation_module privilege_escalation_module.c

# ✅ Verify all binaries created
ls -lah *.c | grep -E "(sedebug|token|privilege)"
file sedebug_privilege token_impersonation privilege_escalation_module

# 👤 Test token impersonation as regular user
echo "=== Token Impersonation (Regular User) ==="
./token_impersonation

# 🔓 Test token impersonation with elevated privileges
echo -e "\n=== Token Impersonation (Root) ==="
sudo ./token_impersonation

# 👤 Test comprehensive module (regular user)
echo -e "\n=== Comprehensive Module (Regular User) ==="
./privilege_escalation_module

# 🔓 Test comprehensive module (root)
echo -e "\n=== Comprehensive Module (Root) ==="
sudo ./privilege_escalation_module
```

**Expected Behavior:**
- ✅ Regular user: "Insufficient privileges" messages
- ✅ Root: "SeDebugPrivilege available", full access
- ✅ Clear privilege differential between runs

---

### 🔹 Subtask 2.4: Create Demonstration Script

<span style="background-color: #E91E63; color: white; padding: 3px 8px; border-radius: 3px;">**ORCHESTRATION**</span>

Build an automated demonstration script:

```bash
cat > demo_privilege_escalation.sh << 'EOF'
#!/bin/bash

echo "╔════════════════════════════════════════════════════════════╗"
echo "║   Privilege Escalation Module Demonstration                ║"
echo "║   SeDebugPrivilege & Token Impersonation                  ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo

# 📍 Check current user
echo "[📍] Current user context:"
id
echo

# 🔐 Test SeDebugPrivilege simulation
echo "[🔐] Testing SeDebugPrivilege simulation:"
./sedebug_privilege
echo

# 👤 Test token impersonation
echo "[👤] Testing token impersonation:"
./token_impersonation
echo

# 🚀 Test comprehensive module
echo "[🚀] Testing comprehensive privilege escalation:"
./privilege_escalation_module
echo

echo "[✅] Demonstration complete!"
echo "[💡] Note: Run with 'sudo ./demo_privilege_escalation.sh' to see elevated privilege behavior"
echo
EOF

# 🔧 Make script executable
chmod +x demo_privilege_escalation.sh

# 🧪 Verify script
file demo_privilege_escalation.sh
ls -lah demo_privilege_escalation.sh
```

**Script Features:**
- 🎨 Formatted header with ASCII border
- 📊 Displays current user context
- 🧪 Runs all three modules sequentially
- 📝 Informative output with section markers
- ✍️ **TODO:** Add timing measurements between tests

---

### 🔹 Subtask 2.5: Verify Module Functionality

<span style="background-color: #00BCD4; color: white; padding: 3px 8px; border-radius: 3px;">**VERIFICATION**</span>

Validate that all modules compile and function correctly:

```bash
# 👤 Run demonstration as regular user
echo "=== Running demonstration as regular user ==="
./demo_privilege_escalation.sh

# 🔓 Run demonstration with elevated privileges
echo -e "\n=== Running demonstration with elevated privileges ==="
sudo ./demo_privilege_escalation.sh

# 📋 List created files and binaries
echo -e "\n=== Created Privilege Escalation Modules ==="
echo "[Source Files]"
ls -lah *.c

echo -e "\n[Compiled Binaries]"
ls -lah sedebug_privilege token_impersonation privilege_escalation_module

echo -e "\n[Scripts]"
ls -lah *.sh

# 📊 Show total project size
echo -e "\n=== Project Statistics ==="
du -sh .
wc -l *.c *.sh
```

**Verification Checklist:**
- ✅ All source files compile without errors
- ✅ All binaries are executable
- ✅ Regular user can run (limited output)
- ✅ Root can run (full capabilities shown)
- ✅ Script executes successfully

---

## 🧪 Verification & Testing

### 🔹 Test Current Implementation

<span style="background-color: #1976D2; color: white; padding: 3px 8px; border-radius: 3px;">**COMPREHENSIVE TESTING**</span>

Validate module functionality across different scenarios:

```bash
# ✅ Verify all modules compile successfully
echo "[✅] Verifying module compilation:"
file sedebug_privilege token_impersonation privilege_escalation_module
echo

# 🧪 Test process access simulation
echo "[🧪] Testing process access capabilities:"
ps aux | head -5
echo "  [Regular user attempt:]"
./sedebug_privilege 2>&1 | tail -2
echo "  [Root attempt:]"
sudo ./sedebug_privilege 2>&1 | tail -2
echo

# 📊 Check privilege differences
echo "[📊] Comparing privilege levels:"
echo "  Regular user output:"
./privilege_escalation_module 2>&1 | grep -E "(Privilege|Debug)"
echo "  Elevated user output:"
sudo ./privilege_escalation_module 2>&1 | grep -E "(Privilege|Debug)"
```

**Test Expectations:**
- 🔴 Regular user: All escalation attempts fail
- 🟢 Root user: All escalation attempts succeed
- 📈 Clear permission boundary demonstrated

---

## 🛠️ Troubleshooting

<details>
<summary><b>❌ Permission denied when using seteuid()</b></summary>

**Problem:** `seteuid()` returns -1 and fails without elevated privileges

**Solutions:**

```bash
# 🔍 Check current effective UID
id -u
id -u -r

# 🔓 Run as root to test
sudo ./token_impersonation

# 💡 Understand the constraint
echo "seteuid() requires root (EUID=0) to work"

# 🔧 Alternative: Use setuid() for permanent change
# Warning: This permanently changes the UID for the process!
# Only use for testing in isolated environments

# ✍️ TODO: Create a wrapper script that uses sudo
cat > run_with_sudo.sh << 'SCRIPT'
#!/bin/bash
sudo ./token_impersonation
SCRIPT
chmod +x run_with_sudo.sh
```

</details>

<details>
<summary><b>❌ Compilation fails with "undefined reference"</b></summary>

**Problem:** Linker error for pwd/grp functions

**Solutions:**

```bash
# 🔗 Link with required libraries
gcc -o token_impersonation token_impersonation.c -lc

# 📋 Check which libraries are needed
nm token_impersonation.c | grep getpwuid

# 🔨 Explicit compilation with flags
gcc -std=c99 -Wall -Wextra -o token_impersonation token_impersonation.c

# ✅ Verify successful compilation
ldd ./token_impersonation | grep libc
```

</details>

<details>
<summary><b>⚠️ Process access denied for init (PID 1)</b></summary>

**Problem:** `kill(1, 0)` fails even with root privileges

**Solutions:**

```bash
# 🔍 Check process ownership
ls -la /proc/1/
ps aux | grep -E "^\s*root.*init"

# 🧪 Test with accessible process instead
ps aux | head -2 | tail -1 | awk '{print $2}'  # Get your own PID

# 🔧 Modify code to test against current process
./sedebug_privilege | grep "Attempting to debug process 1"
echo "Try this instead:"
PID=$$
kill -0 $PID && echo "✅ Current process accessible"

# ✍️ TODO: Update sedebug_privilege.c to use getpid()
```

</details>

<details>
<summary><b>❌ Script fails with "command not found"</b></summary>

**Problem:** Bash script cannot find binaries

**Solutions:**

```bash
# 🔍 Check current directory in PATH
echo $PATH

# 🔧 Use absolute paths in script
sed -i 's/\.\//\.\//' demo_privilege_escalation.sh

# ✅ Verify binaries exist
test -f ./sedebug_privilege && echo "✅ sedebug_privilege found"
test -f ./token_impersonation && echo "✅ token_impersonation found"

# 🔄 Re-run script with explicit paths
bash -x ./demo_privilege_escalation.sh
```

</details>

<details>
<summary><b>⚠️ Inconsistent results between user and root</b></summary>

**Problem:** Same code produces different output when run as user vs root

**Solutions:**

```bash
# 📊 This is expected behavior - verify:
echo "Regular user EUID:"
./privilege_escalation_module | grep "Effective UID"

echo "Root EUID:"
sudo ./privilege_escalation_module | grep "Effective UID"

# 💡 Create comparison script
cat > compare_privilege.sh << 'CMP'
#!/bin/bash
echo "=== Privilege Context Comparison ==="
echo "Regular User:"
id
./privilege_escalation_module | grep -A 3 "Privilege Context"

echo ""
echo "Root User:"
sudo id
sudo ./privilege_escalation_module | grep -A 3 "Privilege Context"
CMP
chmod +x compare_privilege.sh
./compare_privilege.sh
```

**Expected Differences:**
- 👤 User: EUID matches UID (unprivileged)
- 🔓 Root: EUID = 0 (privileged)

</details>

---

## 🎓 Key Takeaways

| Concept | Learning Point | Windows Equivalent | Real-World Application |
|---------|-----------------|-------------------|----------------------|
| 🔐 **SeDebugPrivilege** | Grants system process access | Windows SE_DEBUG_NAME | Malware debugging target processes |
| 👤 **Token Impersonation** | Assume different user context | ImpersonateLoggedOnUser() | Lateral movement, privilege delegation |
| 🆔 **UID/GID System** | Linux privilege separation | Windows SID/Token | Access control foundation |
| ⬆️ **Privilege Escalation** | Moving from low to high privilege | UAC bypass techniques | Common attack vector |
| 🧠 **Process Memory Access** | Read/write arbitrary process memory | OpenProcess()/ReadProcessMemory() | Malware injection, rootkits |
| 🎭 **Context Switching** | Change effective permissions | Token context switching | Privilege delegation attacks |
| 📊 **Privilege Context** | Complete privilege state snapshot | Token structure analysis | Security audit, detection |

---

## 🎯 MITRE ATT&CK Mapping

| Tactic | Technique | Procedure | Detection |
|--------|-----------|-----------|-----------|
| **Privilege Escalation** | [T1134](https://attack.mitre.org/techniques/T1134/) | Access Token Manipulation | Monitor seteuid/setegid syscalls |
| **Privilege Escalation** | [T1548.001](https://attack.mitre.org/techniques/T1548/001/) | Abuse Elevation Control | Track privilege changes via auditd |
| **Execution** | [T1059.004](https://attack.mitre.org/techniques/T1059/004/) | Unix Shell | Monitor shell access to system calls |
| **Persistence** | [T1037](https://attack.mitre.org/techniques/T1037/) | Boot or Logon Initialization | Audit elevated process creation |
| **Credential Access** | [T1110](https://attack.mitre.org/techniques/T1110/) | Brute Force | Monitor failed privilege operations |

**Detection Methods:**
- 📍 Monitor `/var/log/auth.log` for privilege changes
- 🔍 Track uid/gid modifications via `auditd`
- 📊 Alert on root process creation from unprivileged users
- 🚨 Monitor seteuid/setegid syscall patterns
- 🔔 Baseline normal escalation patterns

---

## 📚 Advanced Topics

### Building on This Lab

```bash
# 🔧 Create Makefile for easy compilation
cat > Makefile << 'MAKE'
CC = gcc
CFLAGS = -Wall -Wextra -std=c99
TARGETS = sedebug_privilege token_impersonation privilege_escalation_module

all: $(TARGETS)

sedebug_privilege: sedebug_privilege.c
	$(CC) $(CFLAGS) -o $@ $<

token_impersonation: token_impersonation.c
	$(CC) $(CFLAGS) -o $@ $<

privilege_escalation_module: privilege_escalation_module.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS) *.o

run: all
	./demo_privilege_escalation.sh

sudo-run: all
	sudo ./demo_privilege_escalation.sh

.PHONY: all clean run sudo-run
MAKE

# 🔨 Build with Makefile
make all
make clean
make run
```

**✍️ TODO Exercises:**

1. 🔧 Add capability-based privilege checking (using `getcap` command)
2. 🔐 Implement sudo privilege simulation with authentication
3. 👤 Create group-based privilege escalation module
4. 📊 Add detailed logging to all escalation attempts
5. 🎯 Implement privilege holder detection

---

## 🔗 References & Further Reading

### Privilege Escalation Resources

- **Linux Privilege Escalation**: Study uid/gid separation and capability system
- **Windows Token Concepts**: Learn Windows access token structure and impersonation
- **POSIX Capabilities**: Modern privilege model alternative to setuid
- **SELinux/AppArmor**: Mandatory Access Control systems for granular permissions

### Code Examples to Explore

```bash
# 📚 System audit framework
sudo apt install auditd
sudo auditctl -w /etc/passwd -p wa

# 🔍 Monitor privilege operations
sudo ausearch -m capset,setuid,setgid

# 💻 Explore process capabilities
cat /proc/$$/status | grep Cap
getcap /usr/bin/sudo
```

---

<div align="center">

## 📚 Al Nafi Cybersecurity Training Platform

**Master privilege escalation and access control through hands-on labs**

![Progress](https://img.shields.io/badge/Progress-Lab%20Complete-brightgreen?style=flat-square)
![Difficulty](https://img.shields.io/badge/Difficulty-Advanced-red?style=flat-square)
![Time](https://img.shields.io/badge/Estimated%20Time-120%20minutes-blue?style=flat-square)

**Next Steps:** Explore capability-based privilege escalation, POSIX ACLs, and SELinux/AppArmor mandatory access control systems

---

*Last Updated: July 2024 | Version: 1.0 | Certification: Al Nafi Red Team Ops*

</div>
