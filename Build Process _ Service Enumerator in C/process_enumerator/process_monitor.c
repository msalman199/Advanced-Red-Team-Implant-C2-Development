#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>

volatile int keep_running = 1;

void signal_handler(int sig) {
    keep_running = 0;
    printf("\nShutting down monitor...\n");
}

void clear_screen() {
    printf("\033[2J\033[H");
}

void get_system_info() {
    FILE *file;
    char line[256];
    
    printf("=== SYSTEM INFORMATION ===\n");
    
    // Get uptime
    file = fopen("/proc/uptime", "r");
    if (file) {
        double uptime;
        fscanf(file, "%lf", &uptime);
        int hours = (int)(uptime / 3600);
        int minutes = (int)((uptime - hours * 3600) / 60);
        printf("System Uptime: %d hours, %d minutes\n", hours, minutes);
        fclose(file);
    }
    
    // Get load average
    file = fopen("/proc/loadavg", "r");
    if (file) {
        float load1, load5, load15;
        fscanf(file, "%f %f %f", &load1, &load5, &load15);
        printf("Load Average: %.2f, %.2f, %.2f\n", load1, load5, load15);
        fclose(file);
    }
    
    // Get memory info
    file = fopen("/proc/meminfo", "r");
    if (file) {
        long total_mem = 0, free_mem = 0, available_mem = 0;
        while (fgets(line, sizeof(line), file)) {
            if (strncmp(line, "MemTotal:", 9) == 0) {
                sscanf(line, "MemTotal: %ld kB", &total_mem);
            } else if (strncmp(line, "MemFree:", 8) == 0) {
                sscanf(line, "MemFree: %ld kB", &free_mem);
            } else if (strncmp(line, "MemAvailable:", 13) == 0) {
                sscanf(line, "MemAvailable: %ld kB", &available_mem);
            }
        }
        printf("Memory: %ld MB total, %ld MB available\n", 
               total_mem/1024, available_mem/1024);
        fclose(file);
    }
    
    printf("\n");
}

void monitor_processes() {
    signal(SIGINT, signal_handler);
    
    printf("Real-time Process Monitor (Press Ctrl+C to exit)\n");
    printf("================================================\n\n");
    
    while (keep_running) {
        clear_screen();
        
        time_t now = time(NULL);
        printf("Process Monitor - %s", ctime(&now));
        
        get_system_info();
        
        // Show top processes by CPU usage
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
