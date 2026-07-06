#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

// Payload function that will be injected
void injected_payload() {
    // Write message using direct syscall
    const char msg[] = "[ADVANCED] APC payload executed successfully!\n";
    syscall(SYS_write, 1, msg, sizeof(msg) - 1);
    
    // Create a file to prove execution
    int fd = syscall(SYS_openat, AT_FDCWD, "/tmp/apc_proof.txt", 
                     O_CREAT | O_WRONLY | O_TRUNC, 0644);
    if (fd >= 0) {
        const char proof[] = "APC injection successful\n";
        syscall(SYS_write, fd, proof, sizeof(proof) - 1);
        syscall(SYS_close, fd);
    }
}

int main() {
    injected_payload();
    return 0;
}
