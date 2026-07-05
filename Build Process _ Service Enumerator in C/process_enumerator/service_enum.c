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

void enumerate_services() {
    FILE *pipe;
    char line[MAX_LINE];
    char service_name[64];
    char status[32];
    int count = 0;
    
    printf("=== SYSTEM SERVICES ===\n");
    printf("%-30s %-15s\n", "SERVICE NAME", "STATUS");
    printf("%-30s %-15s\n", "------------", "------");
    
    // Use systemctl to list services
    pipe = popen("systemctl list-units --type=service --no-pager --no-legend", "r");
    if (!pipe) {
        printf("Error: Cannot execute systemctl command\n");
        return;
    }
    
    while (fgets(line, sizeof(line), pipe)) {
        if (sscanf(line, "%s %*s %*s %s", service_name, status) == 2) {
            // Remove .service extension if present
            char *dot = strstr(service_name, ".service");
            if (dot) *dot = '\0';
            
            printf("%-30s %-15s\n", service_name, status);
            count++;
            
            // Limit output for readability
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
        getchar(); // consume newline
        getchar(); // wait for Enter
        printf("\n");
    }
    
    return 0;
}
