#include <windows.h>
#include <stdio.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, int nCmdShow) {
    
    // Get system information
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    
    char buffer[256];
    sprintf(buffer, "System Information:\n\nProcessor Architecture: %d\nNumber of Processors: %d\nPage Size: %d bytes", 
            sysInfo.wProcessorArchitecture, 
            sysInfo.dwNumberOfProcessors, 
            sysInfo.dwPageSize);
    
    MessageBox(NULL, buffer, "System Info via Windows API", MB_OK | MB_ICONINFORMATION);
    
    // Get Windows directory
    char winDir[MAX_PATH];
    GetWindowsDirectory(winDir, MAX_PATH);
    
    sprintf(buffer, "Windows Directory: %s", winDir);
    MessageBox(NULL, buffer, "Windows Directory", MB_OK | MB_ICONINFORMATION);
    
    return 0;
}
