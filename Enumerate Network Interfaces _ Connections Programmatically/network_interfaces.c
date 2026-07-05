#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>

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
    
    printf("  Flags: ");
    if (ifa->ifa_flags & IFF_UP) printf("UP ");
    if (ifa->ifa_flags & IFF_RUNNING) printf("RUNNING ");
    if (ifa->ifa_flags & IFF_LOOPBACK) printf("LOOPBACK ");
    if (ifa->ifa_flags & IFF_BROADCAST) printf("BROADCAST ");
    printf("\n\n");
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
