<div align="center">

# 🌐 Enumerate Network Interfaces & Connections Programmatically

### Linux System Programming with getifaddrs() and /proc/net

![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![C](https://img.shields.io/badge/C-00599C?style=for-the-badge&logo=c&logoColor=white)
![GCC](https://img.shields.io/badge/GCC-A42E2B?style=for-the-badge&logo=gnu&logoColor=white)
![TCP/IP](https://img.shields.io/badge/TCP%2FIP-005571?style=for-the-badge&logo=cisco&logoColor=white)
![Bash](https://img.shields.io/badge/Bash-4EAA25?style=for-the-badge&logo=gnubash&logoColor=white)
![Difficulty](https://img.shields.io/badge/Difficulty-Intermediate-yellow?style=for-the-badge)

*A hands-on Al Nafi lab for building C tools that enumerate network interfaces and active connections using Linux system calls and the `/proc/net` filesystem*

</div>

---

## 📑 Table of Contents

- [Objectives](#-objectives)
- [Prerequisites](#-prerequisites)
- [Lab Environment](#-lab-environment)
- [Task 1: Use Linux APIs to List Network Interfaces](#-task-1-use-linux-apis-to-list-network-interfaces)
- [Task 2: Enumerate Active Network Connections](#-task-2-enumerate-active-network-connections)
- [Verification and Testing](#-verification-and-testing)
- [MITRE ATT&CK Mapping](#-mitre-attck-mapping)
- [Troubleshooting](#-troubleshooting)
- [Key Takeaways](#-key-takeaways)

---

## 🎯 Objectives

| # | Objective |
|---|-----------|
| 1 | Write C programs to enumerate network interfaces on Linux systems |
| 2 | Programmatically list active network connections |
| 3 | Use Linux system calls and libraries for network interface discovery |
| 4 | Parse network information from system files and commands |

## ✅ Prerequisites

| Requirement | Details |
|---|---|
| 💻 Programming | Basic C programming knowledge |
| ⌨️ CLI Skills | Understanding of the Linux command line |
| 🌐 Networking | Familiarity with network concepts (interfaces, connections, sockets) |
| 🔧 System Programming | Basic knowledge of system programming |

## 🌐 Lab Environment

> Al Nafi provides a dedicated **bare-metal Linux cloud machine** for this lab — no pre-installed development tools. Click **Start Lab** to begin; all required packages are installed during the walkthrough. All enumeration in this lab targets the local machine's own interfaces and connections.

---

## 🧩 Task 1: Use Linux APIs to List Network Interfaces

![Step](https://img.shields.io/badge/Step-1-blue?style=flat-square) ![Setup](https://img.shields.io/badge/Type-Environment_Setup-informational?style=flat-square)

### Subtask 1.1: Install Development Tools

```bash
# 📦 Install the C toolchain and headers
sudo apt update
sudo apt install -y gcc libc6-dev build-essential
```

### Subtask 1.2: Create Network Interface Enumeration Program

```bash
# 📝 Create the source file
nano network_interfaces.c
```

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

// 🖧 Prints the name, address, and flags of a single interface entry
void print_interface_info(struct ifaddrs *ifa) {
    char host[NI_MAXHOST];
    int family = ifa->ifa_addr->sa_family;

    printf("Interface: %s\n", ifa->ifa_name);

    if (family == AF_INET || family == AF_INET6) {
        int s = getnameinfo(ifa->ifa_addr,
                           (family == AF_INET) ? sizeof(struct sockaddr_in) :
                                               sizeof(struct sockaddr_in6),
                           host, NI_MAXHOST,
                           NULL, 0, NI_NUMERICHOST);

        if (s == 0) {
            printf("  Address: %s\n", host);
            printf("  Family: %s\n", (family == AF_INET) ? "IPv4" : "IPv6");
        }
    }

    // TODO: Print the interface flags (UP, RUNNING, LOOPBACK, BROADCAST)
    //       by checking ifa->ifa_flags against the IFF_* constants
    printf("\n");
}

int main() {
    struct ifaddrs *ifaddr, *ifa;

    printf("=== Network Interface Enumeration ===\n\n");

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;
        print_interface_info(ifa);
    }

    freeifaddrs(ifaddr);
    return 0;
}
```

### Subtask 1.3: Compile and Test Interface Program

```bash
# ⚙️ Compile the program
gcc -o network_interfaces network_interfaces.c
```

```bash
# ▶️ Run the program
./network_interfaces
```
> ✅ Verify output shows network interfaces with their IP addresses and status flags.

---

## 🧩 Task 2: Enumerate Active Network Connections

![Step](https://img.shields.io/badge/Step-2-blue?style=flat-square) ![Coding](https://img.shields.io/badge/Type-C_Programming-orange?style=flat-square)

### Subtask 2.1: Create Connection Enumeration Program

```bash
nano network_connections.c
```

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// 📄 Parses a /proc/net/{tcp,udp} file and prints each connection's
//     local/remote endpoint and state
void parse_tcp_connections(const char *filename, const char *protocol) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("fopen");
        return;
    }

    char line[256];
    int line_count = 0;

    printf("\n=== %s Connections ===\n", protocol);
    printf("Local Address:Port -> Remote Address:Port [State]\n");
    printf("------------------------------------------------\n");

    while (fgets(line, sizeof(line), file)) {
        line_count++;
        if (line_count == 1) continue; // Skip header

        unsigned long local_addr, remote_addr;
        unsigned int local_port, remote_port, state;

        if (sscanf(line, "%*d: %lX:%X %lX:%X %X",
                   &local_addr, &local_port,
                   &remote_addr, &remote_port, &state) == 5) {

            // TODO: Convert local_addr and remote_addr from packed hex
            //       to dotted-decimal notation and print "A.B.C.D:port ->
            //       A.B.C.D:port" as shown in the header above

            // TODO: Map the numeric `state` value to a human-readable
            //       label (1=ESTABLISHED, 2=SYN_SENT, 3=SYN_RECV,
            //       10=LISTEN, default=STATE_<n>) and print it
            printf("\n");
        }
    }

    fclose(file);
}

// 📊 Prints summary lines from /proc/net/sockstat
void show_socket_statistics() {
    FILE *file = fopen("/proc/net/sockstat", "r");
    if (!file) {
        perror("fopen sockstat");
        return;
    }

    char line[256];
    printf("\n=== Socket Statistics ===\n");

    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, "TCP:") || strstr(line, "UDP:") ||
            strstr(line, "sockets:")) {
            printf("%s", line);
        }
    }

    fclose(file);
}

int main() {
    printf("=== Active Network Connections ===\n");

    parse_tcp_connections("/proc/net/tcp", "TCP");
    parse_tcp_connections("/proc/net/udp", "UDP");
    show_socket_statistics();

    return 0;
}
```

### Subtask 2.2: Compile and Test Connection Program

```bash
gcc -o network_connections network_connections.c
./network_connections
```

### Subtask 2.3: Create Combined Network Analysis Tool

```bash
nano network_analyzer.c
```

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

// 🖧 Prints IPv4 interfaces with UP/RUNNING flags
void show_interfaces() {
    struct ifaddrs *ifaddr, *ifa;
    char host[NI_MAXHOST];

    printf("=== NETWORK INTERFACES ===\n");

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return;
    }

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL) continue;

        int family = ifa->ifa_addr->sa_family;
        if (family == AF_INET) {
            int s = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in),
                               host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
            if (s == 0) {
                printf("%-10s: %s", ifa->ifa_name, host);
                if (ifa->ifa_flags & IFF_UP) printf(" [UP]");
                if (ifa->ifa_flags & IFF_RUNNING) printf(" [RUNNING]");
                printf("\n");
            }
        }
    }

    freeifaddrs(ifaddr);
    printf("\n");
}

// 👂 Prints only sockets in the LISTEN state from /proc/net/tcp
void show_listening_ports() {
    FILE *file = fopen("/proc/net/tcp", "r");
    if (!file) return;

    char line[256];
    int line_count = 0;

    printf("=== LISTENING PORTS ===\n");

    while (fgets(line, sizeof(line), file)) {
        line_count++;
        if (line_count == 1) continue;

        unsigned long local_addr;
        unsigned int local_port, state;

        if (sscanf(line, "%*d: %lX:%X %*lX:%*X %X",
                   &local_addr, &local_port, &state) == 3) {

            if (state == 10) { // LISTEN state
                // TODO: Convert local_addr to dotted-decimal and print
                //       "Port <port>: A.B.C.D" as done in Task 2.1
            }
        }
    }

    fclose(file);
    printf("\n");
}

int main(int argc, char *argv[]) {
    printf("=== NETWORK ANALYZER ===\n\n");

    if (argc > 1 && strcmp(argv[1], "--interfaces") == 0) {
        show_interfaces();
    } else if (argc > 1 && strcmp(argv[1], "--ports") == 0) {
        show_listening_ports();
    } else {
        show_interfaces();
        show_listening_ports();
    }

    return 0;
}
```

### Subtask 2.4: Compile and Test Combined Tool

```bash
# ⚙️ Compile the analyzer
gcc -o network_analyzer network_analyzer.c
```

```bash
# ▶️ Test different options
./network_analyzer
./network_analyzer --interfaces
./network_analyzer --ports
```

### Subtask 2.5: Verify Results with System Commands

```bash
# 🔍 Compare interfaces
ip addr show
ifconfig -a

# 🔍 Compare connections
ss -tuln
netstat -tuln
```

---

## 🔬 Verification and Testing

![Step](https://img.shields.io/badge/Step-Verify-brightgreen?style=flat-square)

```bash
# ✅ Test interface enumeration
echo "Testing interface enumeration..."
./network_interfaces | grep -E "(Interface|Address)"

# ✅ Test connection enumeration
echo "Testing connection enumeration..."
./network_connections | grep -E "(TCP|UDP|LISTEN)"

# ✅ Test combined analyzer
echo "Testing network analyzer..."
./network_analyzer --interfaces
```

---

## 🛡️ MITRE ATT&CK Mapping

| Tactic | Technique | ID | Relevance to This Lab |
|---|---|---|---|
| Discovery | System Network Configuration Discovery | T1016 | Enumerating interfaces, IP addresses, and flags via `getifaddrs()` |
| Discovery | System Network Connections Discovery | T1049 | Parsing `/proc/net/tcp` and `/proc/net/udp` for active connections and listening ports |

> 📌 This mapping is provided for **defensive awareness and detection-engineering context**. Network configuration and connection discovery are routine system-administration and troubleshooting activities; the same primitives also appear during adversary reconnaissance on a compromised host, which is why defenders monitor for unusual enumeration patterns.

---

## 🧯 Troubleshooting

<details>
<summary><strong>Click to expand common issues and fixes</strong></summary>

| Issue | Cause | Fix |
|---|---|---|
| `getifaddrs` compile error | Missing headers | Confirm `libc6-dev` and `build-essential` are installed |
| No output from `network_connections` | `/proc/net/tcp` or `/proc/net/udp` unreadable | Confirm the lab machine uses a standard Linux kernel with `/proc` mounted |
| Garbled IP addresses | Incorrect byte-order handling when converting hex to dotted-decimal | Recheck the bit-shifting logic — Linux stores addresses in `/proc/net/tcp` in little-endian hex |
| `network_analyzer` shows no listening ports | No services currently listening on the lab machine | Start a test service (e.g. `python3 -m http.server`) and rerun `--ports` |

</details>

---

## 🔑 Key Takeaways

| Concept | Summary |
|---|---|
| `getifaddrs()` | The standard POSIX API for enumerating network interfaces and their addresses/flags |
| `/proc/net/tcp` and `/proc/net/udp` | Kernel-exposed text files listing every active socket, its state, and its endpoints |
| Hex-encoded addresses | Addresses in `/proc/net/*` are stored as packed little-endian hex and must be converted for display |
| Tool composition | Combining interface and connection enumeration into one analyzer mirrors how tools like `ss` and `netstat` are built internally |
| Practical use case | These techniques support network troubleshooting, system administration, and security monitoring |

<div align="center">

---

### 🎓 Al Nafi Cybersecurity Training Platform
*Empowering the next generation of cybersecurity professionals*

![Al Nafi](https://img.shields.io/badge/Al%20Nafi-Cybersecurity%20Labs-blueviolet?style=for-the-badge)

</div>
