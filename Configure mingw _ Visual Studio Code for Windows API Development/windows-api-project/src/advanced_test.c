#include <windows.h>
#include <stdio.h>

int main() {
    // Test various Windows API functions
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    
    printf("Cross-compilation test successful!\n");
    printf("Target architecture: %s\n", 
           (sysInfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) ? 
           "x64" : "x86");
    
    return 0;
}
