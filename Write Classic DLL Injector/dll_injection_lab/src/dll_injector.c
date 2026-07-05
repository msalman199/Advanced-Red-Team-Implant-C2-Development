#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <dlfcn.h>
#include <link.h>
#include <errno.h>

// Function to find dlopen address in target process
unsigned long find_dlopen_addr(pid_t pid) {
    char maps_path[256];
    char line[1024];
    FILE *maps_file;
    unsigned long libc_base = 0;
    
    snprintf(maps_path, sizeof(maps_path), "/proc/%d/maps", pid);
    maps_file = fopen(maps_path, "r");
    
    if (!maps_file) {
        perror("fopen maps");
        return 0;
    }
    
    // Find libc base address
    while (fgets(line, sizeof(line), maps_file)) {
        if (strstr(line, "libc.so") || strstr(line, "libc-")) {
            sscanf(line, "%lx", &libc_base);
            break;
        }
    }
    fclose(maps_file);
    
    if (libc_base == 0) {
        printf("Could not find libc base address\n");
        return 0;
    }
    
    // Get dlopen offset from current process
    void *local_dlopen = dlsym(RTLD_DEFAULT, "dlopen");
    void *local_libc = dlopen("libc.so.6", RTLD_LAZY);
    
    if (!local_dlopen || !local_libc) {
        printf("Could not get local dlopen address\n");
        return 0;
    }
    
    // Calculate offset and target address
    unsigned long offset = (unsigned long)local_dlopen - (unsigned long)local_libc;
    dlclose(local_libc);
    
    return libc_base + offset;
}

// Inject DLL into target process
int inject_dll(pid_t target_pid, const char *dll_path) {
    struct user_regs_struct old_regs, regs;
    unsigned long dlopen_addr;
    unsigned long remote_string;
    int status;
    long ret;
    
    printf("Attempting to inject %s into process %d\n", dll_path, target_pid);
    
    // Attach to target process
    if (ptrace(PTRACE_ATTACH, target_pid, NULL, NULL) == -1) {
        perror("ptrace attach");
        return -1;
    }
    
    // Wait for process to stop
    waitpid(target_pid, &status, 0);
    
    // Get current registers
    if (ptrace(PTRACE_GETREGS, target_pid, NULL, &old_regs) == -1) {
        perror("ptrace getregs");
        ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
        return -1;
    }
    
    regs = old_regs;
    
    // Find dlopen address
    dlopen_addr = find_dlopen_addr(target_pid);
    if (dlopen_addr == 0) {
        printf("Could not find dlopen address\n");
        ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
        return -1;
    }
    
    printf("Found dlopen at: 0x%lx\n", dlopen_addr);
    
    // Allocate memory for DLL path string
    regs.rip = dlopen_addr;
    regs.rdi = (unsigned long)dll_path;  // First argument (filename)
    regs.rsi = RTLD_LAZY;                // Second argument (flags)
    
    // Set registers and execute dlopen
    if (ptrace(PTRACE_SETREGS, target_pid, NULL, &regs) == -1) {
        perror("ptrace setregs");
        ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
        return -1;
    }
    
    // Continue execution
    if (ptrace(PTRACE_CONT, target_pid, NULL, NULL) == -1) {
        perror("ptrace cont");
        ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
        return -1;
    }
    
    // Wait for syscall to complete
    waitpid(target_pid, &status, 0);
    
    // Get return value
    if (ptrace(PTRACE_GETREGS, target_pid, NULL, &regs) == -1) {
        perror("ptrace getregs");
        ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
        return -1;
    }
    
    printf("dlopen returned: 0x%llx\n", regs.rax);
    
    // Restore original registers
    if (ptrace(PTRACE_SETREGS, target_pid, NULL, &old_regs) == -1) {
        perror("ptrace setregs restore");
    }
    
    // Detach from process
    ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
    
    if (regs.rax == 0) {
        printf("DLL injection failed\n");
        return -1;
    }
    
    printf("DLL injection successful!\n");
    return 0;
}

int main(int argc, char *argv[]) {
    pid_t target_pid;
    char *dll_path;
    char full_dll_path[1024];
    
    if (argc != 3) {
        printf("Usage: %s <target_pid> <dll_path>\n", argv[0]);
        return 1;
    }
    
    target_pid = atoi(argv[1]);
    dll_path = argv[2];
    
    // Convert to absolute path
    if (realpath(dll_path, full_dll_path) == NULL) {
        perror("realpath");
        return 1;
    }
    
    printf("DLL Injector starting...\n");
    printf("Target PID: %d\n", target_pid);
    printf("DLL Path: %s\n", full_dll_path);
    
    return inject_dll(target_pid, full_dll_path);
}
