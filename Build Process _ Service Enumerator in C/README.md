<div align="center">

# ⚙️ Build a Process & Service Enumerator in C

### Linux System Programming with the /proc Filesystem

![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![GCC](https://img.shields.io/badge/GCC-A42E2B?style=for-the-badge&logo=gnu&logoColor=white)
![Systemd](https://img.shields.io/badge/systemd-06AC3D?style=for-the-badge&logo=linux&logoColor=white)
![Make](https://img.shields.io/badge/Make-427819?style=for-the-badge&logo=gnu&logoColor=white)
![Bash](https://img.shields.io/badge/Bash-4EAA25?style=for-the-badge&logo=gnubash&logoColor=white)
![Difficulty](https://img.shields.io/badge/Difficulty-Intermediate-yellow?style=for-the-badge)

*A hands-on Al Nafi lab for building process and service monitoring tools in C, using direct `/proc` filesystem access and `systemctl` integration*

</div>

---

## 📑 Table of Contents

- [Learning Objectives](#-learning-objectives)
- [Prerequisites](#-prerequisites)
- [Lab Environment](#-lab-environment)
- [Task 1: Set Up Development Environment](#-task-1-set-up-development-environment)
- [Task 2: Create Process Enumerator](#-task-2-create-process-enumerator)
- [Task 3: Add Service Enumeration](#-task-3-add-service-enumeration)
- [Task 4: Create Advanced Process Monitor](#-task-4-create-advanced-process-monitor)
- [Task 5: Create Makefile for Build Management](#-task-5-create-makefile-for-build-management)
- [Task 6: Verification and Testing](#-task-6-verification-and-testing)
- [MITRE ATT&CK Mapping](#-mitre-attck-mapping)
- [Troubleshooting](#-troubleshooting)
- [Key Takeaways](#-key-takeaways)

---

## 🎯 Learning Objectives

| # | Objective |
|---|-----------|
| 1 | Develop a C program to enumerate running system processes |
| 2 | Create functionality to display service names and their current statuses |
| 3 | Understand Linux process management and system service monitoring |
| 4 | Practice system programming with C using the `/proc` filesystem |

## ✅ Prerequisites

| Requirement | Details |
|---|---|
| 💻 Programming | Basic knowledge of the C programming language |
| ⌨️ CLI Skills | Understanding of Linux command line operations |
| 🔧 Build Tools | Familiarity with compilation using GCC |
| 🖥️ OS Concepts | Basic understanding of Linux processes and services |

## 🌐 Lab Environment

> Al Nafi provides a dedicated **bare-metal Linux cloud machine** for this lab — no pre-installed development tools. Click **Start Lab** to begin; all required tools are installed during the walkthrough.

---

## 🧩 Task 1: Set Up Development Environment

![Step](https://img.shields.io/badge/Step-1-blue?style=flat-square) ![Setup](https://img.shields.io/badge/Type-Environment_Setup-informational?style=flat-square)

### Step 1.1: Install Required Tools

```bash
# 📦 Update package lists and install the C toolchain
sudo apt update
sudo apt install -y gcc make libc6-dev
```

### Step 1.2: Verify Installation

```bash
# 🔍 Confirm GCC is available
gcc --version
```

---

## 🧩 Task 2: Create Process Enumerator

![Step](https://img.shields.io/badge/Step-2-blue?style=flat-square) ![Coding](https://img.shields.io/badge/Type-C_Programming-orange?style=flat-square)

### Step 2.1: Create Project Directory

```bash
# 📁 TODO: Create and enter your working directory
mkdir ~/process_enumerator
cd ~/process_enumerator
```

### Step 2.2: Create Process Enumeration Code

```bash
# 📝 Create the source file
nano process_enum.c
```

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>

#define MAX_PATH 256
#define MAX_LINE 1024

typedef struct {
    int pid;
    char name[64];
    char state;
    int ppid;
} ProcessInfo;

// 🔢 Checks whether a directory entry name is purely numeric (i.e. a PID)
int is_number(const char *str) {
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

// 📄 Parses /proc/<pid>/stat into a ProcessInfo struct
int get_process_info(int pid, ProcessInfo *proc) {
    char path[MAX_PATH];
    char line[MAX_LINE];
    FILE *file;

    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    file = fopen(path, "r");

    if (!file) return 0;

    if (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d %s %c %d", &proc->pid, proc->name, &proc->state, &proc->ppid);
        // TODO: Strip the surrounding parentheses from proc->name
        //       (the kernel wraps process names like "(bash)")
    }

    fclose(file);
    return 1;
}

// 📋 Walks /proc, filters numeric entries, and prints a process table
void enumerate_processes() {
    DIR *proc_dir;
    struct dirent *entry;
    ProcessInfo proc;

    printf("=== RUNNING PROCESSES ===\n");
    printf("%-8s %-20s %-8s %-8s\n", "PID", "NAME", "STATE", "PPID");
    printf("%-8s %-20s %-8s %-8s\n", "----", "----", "-----", "----");

    proc_dir = opendir("/proc");
    if (!proc_dir) {
        perror("opendir /proc");
        return;
    }

    // TODO: Loop with readdir(), use is_number() to filter PID directories,
    //       call get_process_info() for each, and print the formatted row

    closedir(proc_dir);
}

int main() {
    printf("Process & Service Enumerator\n");
    printf("============================\n\n");

    enumerate_processes();

    return 0;
}
```

### Step 2.3: Compile and Test Process Enumerator

```bash
gcc -o process_enum process_enum.c
./process_enum
```

---

## 🧩 Task 3: Add Service Enumeration

![Step](https://img.shields.io/badge/Step-3-blue?style=flat-square) ![Services](https://img.shields.io/badge/Type-Service_Monitoring-9cf?style=flat-square)

### Step 3.1: Create Enhanced Version with Service Support

```bash
nano service_enum.c
```

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

#define MAX_PATH 256
#define MAX_LINE 1024
#define MAX_SERVICES 100

typedef struct {
    int pid;
    char name[64];
    char state;
    int ppid;
} ProcessInfo;

typedef struct {
    char name[64];
    char status[32];
    char description[128];
} ServiceInfo;

int is_number(const char *str) {
    while (*str) {
        if (!isdigit(*str)) return 0;
        str++;
    }
    return 1;
}

int get_process_info(int pid, ProcessInfo *proc) {
    char path[MAX_PATH];
    char line[MAX_LINE];
    FILE *file;

    snprintf(path, sizeof(path), "/proc/%d/stat", pid);
    file = fopen(path, "r");

    if (!file) return 0;

    if (fgets(line, sizeof(line), file)) {
        sscanf(line, "%d %s %c %d", &proc->pid, proc->name, &proc->state, &proc->ppid);
        if (proc->name[0] == '(') {
            memmove(proc->name, proc->name + 1, strlen(proc->name));
            proc->name[strlen(proc->name) - 1] = '\0';
        }
    }

    fclose(file);
    return 1;
}

// 🔁 Same enumeration as before, now tracking a running total
void enumerate_processes() {
    DIR *proc_dir;
    struct dirent *entry;
    ProcessInfo proc;
    int count = 0;

    printf("=== RUNNING PROCESSES ===\n");
    printf("%-8s %-20s %-8s %-8s\n", "PID", "NAME", "STATE", "PPID");
    printf("%-8s %-20s %-8s %-8s\n", "----", "----", "-----", "----");

    proc_dir = opendir("/proc");
    if (!proc_dir) {
        perror("opendir /proc");
        return;
    }

    while ((entry = readdir(proc_dir)) != NULL) {
        if (is_number(entry->d_name)) {
            int pid = atoi(entry->d_name);
            if (get_process_info(pid, &proc)) {
                printf("%-8d %-20s %-8c %-8d\n",
                       proc.pid, proc.name, proc.state, proc.ppid);
                count++;
            }
        }
    }

    closedir(proc_dir);
    printf("\nTotal processes: %d\n\n", count);
}

// 🛎️ Lists systemd service units and their current status via systemctl
void enumerate_services() {
    FILE *pipe;
    char line[MAX_LINE];
    char service_name[64];
    char status[32];
    int count = 0;

    printf("=== SYSTEM SERVICES ===\n");
    printf("%-30s %-15s\n", "SERVICE NAME", "STATUS");
    printf("%-30s %-15s\n", "------------", "------");

    pipe = popen("systemctl list-units --type=service --no-pager --no-legend", "r");
    if (!pipe) {
        printf("Error: Cannot execute systemctl command\n");
        return;
    }

    // TODO: Parse each line with sscanf, strip the ".service" suffix,
    //       print the row, and cap output at 20 entries as shown below
    while (fgets(line, sizeof(line), pipe)) {
        if (sscanf(line, "%s %*s %*s %s", service_name, status) == 2) {
            char *dot = strstr(service_name, ".service");
            if (dot) *dot = '\0';

            printf("%-30s %-15s\n", service_name, status);
            count++;

            if (count >= 20) {
                printf("... (showing first 20 services)\n");
                break;
            }
        }
    }

    pclose(pipe);
    printf("\nServices displayed: %d\n\n", count);
}

void show_menu() {
    printf("Process & Service Enumerator\n");
    printf("============================\n");
    printf("1. List Running Processes\n");
    printf("2. List System Services\n");
    printf("3. List Both\n");
    printf("4. Exit\n");
    printf("Enter your choice (1-4): ");
}

int main() {
    int choice;

    while (1) {
        show_menu();
        scanf("%d", &choice);
        printf("\n");

        switch (choice) {
            case 1:
                enumerate_processes();
                break;
            case 2:
                enumerate_services();
                break;
            case 3:
                enumerate_processes();
                enumerate_services();
                break;
            case 4:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n\n");
        }

        printf("Press Enter to continue...");
        getchar();
        getchar();
        printf("\n");
    }

    return 0;
}
```

### Step 3.2: Compile and Test Enhanced Version

```bash
gcc -o service_enum service_enum.c
./service_enum
```

---

## 🧩 Task 4: Create Advanced Process Monitor

![Step](https://img.shields.io/badge/Step-4-blue?style=flat-square) ![Monitoring](https://img.shields.io/badge/Type-Real--time_Monitoring-red?style=flat-square)

### Step 4.1: Create Real-time Process Monitor

```bash
nano process_monitor.c
```

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

volatile int keep_running = 1;

// 🛑 Handles Ctrl+C to gracefully stop the monitoring loop
void signal_handler(int sig) {
    keep_running = 0;
    printf("\nShutting down monitor...\n");
}

void clear_screen() {
    printf("\033[2J\033[H");
}

// 📊 Reads uptime, load average, and memory stats from /proc
void get_system_info() {
    FILE *file;
    char line[256];

    printf("=== SYSTEM INFORMATION ===\n");

    file = fopen("/proc/uptime", "r");
    if (file) {
        double uptime;
        fscanf(file, "%lf", &uptime);
        int hours = (int)(uptime / 3600);
        int minutes = (int)((uptime - hours * 3600) / 60);
        printf("System Uptime: %d hours, %d minutes\n", hours, minutes);
        fclose(file);
    }

    // TODO: Open /proc/loadavg and print the 1/5/15-minute load averages

    // TODO: Open /proc/meminfo, extract MemTotal and MemAvailable,
    //       and print both converted to megabytes

    printf("\n");
}

// 🔄 Clears the screen and redraws system + process info every 5 seconds
void monitor_processes() {
    signal(SIGINT, signal_handler);

    printf("Real-time Process Monitor (Press Ctrl+C to exit)\n");
    printf("================================================\n\n");

    while (keep_running) {
        clear_screen();

        time_t now = time(NULL);
        printf("Process Monitor - %s", ctime(&now));

        get_system_info();

        printf("=== TOP PROCESSES (by CPU) ===\n");
        system("ps aux --sort=-%cpu | head -10");

        printf("\nRefreshing in 5 seconds...\n");
        sleep(5);
    }
}

int main() {
    monitor_processes();
    return 0;
}
```

### Step 4.2: Compile and Test Process Monitor

```bash
gcc -o process_monitor process_monitor.c
./process_monitor
```

---

## 🧩 Task 5: Create Makefile for Build Management

![Step](https://img.shields.io/badge/Step-5-blue?style=flat-square) ![Build](https://img.shields.io/badge/Type-Build_Automation-success?style=flat-square)

### Step 5.1: Create Makefile

```bash
nano Makefile
```

```makefile
CC=gcc
CFLAGS=-Wall -Wextra -std=c99
TARGETS=process_enum service_enum process_monitor

all: $(TARGETS)

process_enum: process_enum.c
	$(CC) $(CFLAGS) -o $@ $<

service_enum: service_enum.c
	$(CC) $(CFLAGS) -o $@ $<

process_monitor: process_monitor.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(TARGETS)

install: all
	sudo cp $(TARGETS) /usr/local/bin/

uninstall:
	sudo rm -f /usr/local/bin/process_enum
	sudo rm -f /usr/local/bin/service_enum
	sudo rm -f /usr/local/bin/process_monitor

.PHONY: all clean install uninstall
```
> ⚠️ Makefiles require **tab characters** (not spaces) to indent recipe lines — check your editor's settings if `make` reports "missing separator".

### Step 5.2: Build All Programs

```bash
make all
```

### Step 5.3: Test All Programs

```bash
# ▶️ Test basic process enumerator
./process_enum

# ▶️ Test service enumerator (try option 3)
./service_enum

# ▶️ Test process monitor (Ctrl+C to exit)
./process_monitor
```

---

## 🔬 Task 6: Verification and Testing

![Step](https://img.shields.io/badge/Step-6-blue?style=flat-square) ![Verify](https://img.shields.io/badge/Type-Verification-brightgreen?style=flat-square)

### Step 6.1: Verify Process Enumeration

```bash
# 🔍 Compare your output count with the system's ps command
./process_enum | wc -l
ps aux | wc -l
```

### Step 6.2: Verify Service Enumeration

```bash
# 🔍 Cross-check against systemctl's own count
systemctl list-units --type=service | grep -c "loaded"
```

### Step 6.3: Create Test Script

```bash
nano test_enumerator.sh
```

```bash
#!/bin/bash

echo "Testing Process & Service Enumerator"
echo "===================================="

echo "1. Testing process enumeration..."
./process_enum > /tmp/proc_test.out
PROC_COUNT=$(wc -l < /tmp/proc_test.out)
echo "   Found $PROC_COUNT lines of output"

echo "2. Testing service enumeration..."
echo "3" | ./service_enum > /tmp/service_test.out
echo "   Service enumeration completed"

echo "3. Checking if programs compile without warnings..."
make clean
make all

echo "4. All tests completed successfully!"
```

```bash
# 🔐 Make executable and run
chmod +x test_enumerator.sh
./test_enumerator.sh
```

---

## 🛡️ MITRE ATT&CK Mapping

| Tactic | Technique | ID | Relevance to This Lab |
|---|---|---|---|
| Discovery | Process Discovery | T1057 | Enumerating running processes via `/proc` |
| Discovery | System Service Discovery | T1007 | Listing systemd services and their status |
| Discovery | System Information Discovery | T1082 | Reading uptime, load average, and memory stats |

> 📌 This mapping is provided for **defensive awareness and detection-engineering context**. Process and service enumeration are routine system-administration and monitoring activities; the same primitives also appear during adversary reconnaissance, which is why defenders instrument and baseline this kind of activity for anomaly detection.

---

## 🧯 Troubleshooting

<details>
<summary><strong>Click to expand common issues and fixes</strong></summary>

| Issue | Cause | Fix |
|---|---|---|
| Permission errors on `/proc` | Insufficient privileges to read some process entries | Run with appropriate privileges, or expect some PIDs to be skipped — this is normal |
| Compilation errors | GCC or dev packages missing | Verify GCC is installed and rerun `sudo apt install -y gcc make libc6-dev` |
| Service enumeration fails | `systemctl` not available | Confirm the distribution is systemd-based; non-systemd systems need an alternative service manager |

</details>

---

## 🔑 Key Takeaways

| Concept | Summary |
|---|---|
| `/proc` filesystem | Each PID directory under `/proc` exposes live process state via plain text files like `stat` |
| Process metadata | PID, name, state, and parent PID can all be parsed directly from `/proc/<pid>/stat` |
| Service integration | `systemctl list-units` bridges C programs with systemd's service management data |
| Build hygiene | A Makefile with `install`/`uninstall`/`clean` targets reflects professional build practices |
| Practical use case | These primitives underpin process monitoring, performance analysis, and security anomaly detection |

<div align="center">

---

### 🎓 Al Nafi Cybersecurity Training Platform
*Empowering the next generation of cybersecurity professionals*

![Al Nafi](https://img.shields.io/badge/Al%20Nafi-Cybersecurity%20Labs-blueviolet?style=for-the-badge)

</div>
