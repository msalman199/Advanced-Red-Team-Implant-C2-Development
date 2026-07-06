#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

// Simple calculator shellcode (adds two numbers)
unsigned char calc_payload[] = 
    "\x48\x31\xc0"                // xor rax, rax
    "\x48\xc7\xc0\x05\x00\x00\x00" // mov rax, 5
    "\x48\xc7\xc3\x03\x00\x00\x00" // mov rbx, 3
    "\x48\x01\xd8"                // add rax, rbx
    "\x48\x89\xc7"                // mov rdi, rax (result = 8)
    "\x48\xc7\xc0\x3c\x00\x00\x00" // mov rax, 60 (sys_exit)
    "\x0f\x05";                   // syscall

typedef int (*payload_func)();

struct loader_context {
    void* memory;
    size_t size;
    int is_allocated;
};

int initialize_loader_context(struct loader_context* ctx, size_t size) {
    ctx->size = size;
    ctx->is_allocated = 0;
    
    // Allocate memory with RWX permissions
    ctx->memory = mmap(NULL, size,
                       PROT_READ | PROT_WRITE | PROT_EXEC,
                       MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (ctx->memory == MAP_FAILED) {
        fprintf(stderr, "[-] Memory allocation failed: %s\n", strerror(errno));
        return -1;
    }
    
    ctx->is_allocated = 1;
    printf("[+] Allocated %zu bytes at %p\n", size, ctx->memory);
    return 0;
}

void cleanup_loader_context(struct loader_context* ctx) {
    if (ctx->is_allocated && ctx->memory) {
        munmap(ctx->memory, ctx->size);
        printf("[+] Memory freed\n");
        ctx->is_allocated = 0;
        ctx->memory = NULL;
    }
}

int load_payload(struct loader_context* ctx, unsigned char* payload, size_t payload_size) {
    if (!ctx->is_allocated) {
        fprintf(stderr, "[-] Context not initialized\n");
        return -1;
    }
    
    if (payload_size > ctx->size) {
        fprintf(stderr, "[-] Payload too large for allocated memory\n");
        return -1;
    }
    
    // Copy payload to executable memory
    memcpy(ctx->memory, payload, payload_size);
    printf("[+] Payload loaded (%zu bytes)\n", payload_size);
    
    return 0;
}

int execute_payload(struct loader_context* ctx) {
    if (!ctx->is_allocated) {
        fprintf(stderr, "[-] Context not initialized\n");
        return -1;
    }
    
    printf("[+] Executing payload...\n");
    
    // Cast to function pointer and execute
    payload_func func = (payload_func)ctx->memory;
    int result = func();
    
    printf("[+] Payload executed, result: %d\n", result);
    return result;
}

int main() {
    printf("=== Advanced In-Memory Loader ===\n");
    
    struct loader_context ctx;
    size_t payload_size = sizeof(calc_payload) - 1;
    
    // Initialize loader context
    if (initialize_loader_context(&ctx, 4096) != 0) {
        return 1;
    }
    
    // Load payload
    if (load_payload(&ctx, calc_payload, payload_size) != 0) {
        cleanup_loader_context(&ctx);
        return 1;
    }
    
    // Execute payload
    execute_payload(&ctx);
    
    // Cleanup
    cleanup_loader_context(&ctx);
    
    printf("[+] Loader completed successfully\n");
    return 0;
}
