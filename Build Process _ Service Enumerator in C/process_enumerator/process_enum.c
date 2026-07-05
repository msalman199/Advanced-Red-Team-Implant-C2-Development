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
        // Remove parentheses from process name
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
            }
        }
    }
    
    closedir(proc_dir);
}

int main() {
    printf("Process & Service Enumerator\n");
    printf("============================\n\n");
    
    enumerate_processes();
    
    return 0;
}
