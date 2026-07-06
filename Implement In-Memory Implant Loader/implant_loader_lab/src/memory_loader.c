#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

// Test shellcode - prints message and exits
unsigned char test_payload[] = 
    "\x48\x31\xc0"                // xor rax, rax
    "\x48\x31\xff"                // xor rdi, rdi
    "\x48\x31\xf6"                // xor rsi, rsi
    "\x48\x31\xd2"                // xor rdx, rdx
    "\x48\xc7\xc0\x01\x00\x00\x00" // mov rax, 1 (sys_write)
    "\x48\xc7\xc7\x01\x00\x00\x00" // mov rdi, 1 (stdout)
    "\x48\x8d\x35\x0a\x00\x00\x00" // lea rsi, [rip+10]
    "\x48\xc7\xc2\x0d\x00\x00\x00" // mov rdx, 13
    "\x0f\x05"                    // syscall
    "\x48\x31\xc0"                // xor rax, rax
    "\x48\xc7\xc0\x3c\x00\x00\x00" // mov rax, 60 (sys_exit)
    "\x48\x31\xff"                // xor rdi, rdi
    "\x0f\x05"                    // syscall
    "Hello World!\n";

typedef void (*shellcode_func)();

void* allocate_executable_memory(size_t size) {
    void* mem = mmap(NULL, size, 
                     PROT_READ | PROT_WRITE | PROT_EXEC,
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (mem == MAP_FAILED) {
        perror("mmap failed");
        return NULL;
    }
    
    printf("[+] Allocated executable memory at: %p\n", mem);
    return mem;
}

int load_and_execute_shellcode(unsigned char* payload, size_t payload_size) {
    printf("[+] Loading shellcode of size: %zu bytes\n", payload_size);
    
    // Allocate executable memory
    void* exec_mem = allocate_executable_memory(payload_size);
    if (!exec_mem) {
        return -1;
    }
    
    // Copy shellcode to executable memory
    memcpy(exec_mem, payload, payload_size);
    printf("[+] Shellcode copied to memory\n");
    
    // Cast memory to function pointer
    shellcode_func execute = (shellcode_func)exec_mem;
    
    printf("[+] Executing shellcode...\n");
    
    // Execute the shellcode
    execute();
    
    // Clean up (this may not be reached if shellcode exits)
    munmap(exec_mem, payload_size);
    printf("[+] Memory cleaned up\n");
    
    return 0;
}

int main(int argc, char* argv[]) {
    printf("=== In-Memory Implant Loader ===\n");
    printf("[+] Starting loader...\n");
    
    size_t payload_size = sizeof(test_payload) - 1; // Exclude null terminator
    
    if (load_and_execute_shellcode(test_payload, payload_size) != 0) {
        fprintf(stderr, "[-] Failed to load and execute shellcode\n");
        return 1;
    }
    
    return 0;
}
