#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void generate_nop_sled(unsigned char* buffer, size_t size) {
    for (size_t i = 0; i < size; i++) {
        buffer[i] = 0x90; // NOP instruction
    }
}

void print_payload_hex(unsigned char* payload, size_t size) {
    printf("Payload hex dump:\n");
    for (size_t i = 0; i < size; i++) {
        printf("\\x%02x", payload[i]);
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

int main() {
    printf("=== Payload Generator ===\n");
    
    // Generate a simple payload
    unsigned char payload[64];
    generate_nop_sled(payload, 60);
    
    // Add exit syscall at the end
    payload[60] = 0x48; // mov rax, 60
    payload[61] = 0xc7;
    payload[62] = 0xc0;
    payload[63] = 0x3c;
    
    print_payload_hex(payload, 64);
    
    return 0;
}
