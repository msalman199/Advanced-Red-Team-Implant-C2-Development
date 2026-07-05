#include <stdio.h>
#include <windows.h>

int main() {
    printf("Hello World from Windows API Console!\n");
    printf("This program was cross-compiled on Linux for Windows.\n");
    
    // Test Windows API function
    DWORD version = GetVersion();
    printf("Windows version info available.\n");
    
    return 0;
}
