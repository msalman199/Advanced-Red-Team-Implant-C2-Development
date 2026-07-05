#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

// Constructor function - executes when library is loaded
__attribute__((constructor))
void lib_init() {
    printf("[INJECTED] Library loaded into process PID: %d\n", getpid());
    printf("[INJECTED] DLL injection successful!\n");
}

// Destructor function - executes when library is unloaded
__attribute__((destructor))
void lib_cleanup() {
    printf("[INJECTED] Library unloaded from process PID: %d\n", getpid());
}

// Example function that can be called
void injected_function() {
    printf("[INJECTED] Function called from injected library\n");
}
