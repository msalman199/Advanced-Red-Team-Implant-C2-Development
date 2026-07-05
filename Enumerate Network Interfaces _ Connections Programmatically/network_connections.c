#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

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
            
            // Convert addresses from hex to dotted decimal
            printf("%d.%d.%d.%d:%d -> %d.%d.%d.%d:%d",
                   (int)(local_addr & 0xFF),
                   (int)((local_addr >> 8) & 0xFF),
                   (int)((local_addr >> 16) & 0xFF),
                   (int)((local_addr >> 24) & 0xFF),
                   local_port,
                   (int)(remote_addr & 0xFF),
                   (int)((remote_addr >> 8) & 0xFF),
                   (int)((remote_addr >> 16) & 0xFF),
                   (int)((remote_addr >> 24) & 0xFF),
                   remote_port);
            
            // Print connection state
            switch(state) {
                case 1: printf(" [ESTABLISHED]"); break;
                case 2: printf(" [SYN_SENT]"); break;
                case 3: printf(" [SYN_RECV]"); break;
                case 10: printf(" [LISTEN]"); break;
                default: printf(" [STATE_%d]", state); break;
            }
            printf("\n");
        }
    }
    
    fclose(file);
}

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
    
    // Parse TCP connections
    parse_tcp_connections("/proc/net/tcp", "TCP");
    
    // Parse UDP connections  
    parse_tcp_connections("/proc/net/udp", "UDP");
    
    // Show socket statistics
    show_socket_statistics();
    
    return 0;
}
