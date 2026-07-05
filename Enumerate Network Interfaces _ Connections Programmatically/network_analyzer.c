#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

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
                printf("Port %d: %d.%d.%d.%d\n", 
                       local_port,
                       (int)(local_addr & 0xFF),
                       (int)((local_addr >> 8) & 0xFF),
                       (int)((local_addr >> 16) & 0xFF),
                       (int)((local_addr >> 24) & 0xFF));
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
