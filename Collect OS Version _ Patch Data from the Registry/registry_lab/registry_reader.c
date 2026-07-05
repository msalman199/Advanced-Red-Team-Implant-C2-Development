#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

void printOSVersion() {
    HKEY hKey;
    DWORD dwType = REG_SZ;
    DWORD dwSize = 256;
    char szValue[256];
    
    printf("=== Operating System Information ===\n");
    
    // Open registry key for OS version
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
                     "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 
                     0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        
        // Get Product Name
        dwSize = 256;
        if (RegQueryValueEx(hKey, "ProductName", NULL, &dwType, 
                           (LPBYTE)szValue, &dwSize) == ERROR_SUCCESS) {
            printf("Product Name: %s\n", szValue);
        }
        
        // Get Version
        dwSize = 256;
        if (RegQueryValueEx(hKey, "CurrentVersion", NULL, &dwType, 
                           (LPBYTE)szValue, &dwSize) == ERROR_SUCCESS) {
            printf("Version: %s\n", szValue);
        }
        
        // Get Build Number
        dwSize = 256;
        if (RegQueryValueEx(hKey, "CurrentBuild", NULL, &dwType, 
                           (LPBYTE)szValue, &dwSize) == ERROR_SUCCESS) {
            printf("Build: %s\n", szValue);
        }
        
        // Get Release ID
        dwSize = 256;
        if (RegQueryValueEx(hKey, "ReleaseId", NULL, &dwType, 
                           (LPBYTE)szValue, &dwSize) == ERROR_SUCCESS) {
            printf("Release ID: %s\n", szValue);
        }
        
        RegCloseKey(hKey);
    } else {
        printf("Failed to open OS version registry key\n");
    }
}

void printInstalledUpdates() {
    HKEY hKey;
    DWORD dwIndex = 0;
    char szKeyName[256];
    DWORD dwKeyNameSize = 256;
    FILETIME ftLastWriteTime;
    
    printf("\n=== Installed Updates ===\n");
    
    // Open registry key for installed updates
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
                     "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall", 
                     0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        
        printf("Enumerating installed updates...\n");
        
        // Enumerate subkeys (installed programs/updates)
        while (RegEnumKeyEx(hKey, dwIndex, szKeyName, &dwKeyNameSize, 
                           NULL, NULL, NULL, &ftLastWriteTime) == ERROR_SUCCESS) {
            
            // Check if it's a Windows Update (KB prefix)
            if (strncmp(szKeyName, "KB", 2) == 0) {
                printf("Update: %s\n", szKeyName);
            }
            
            dwIndex++;
            dwKeyNameSize = 256;
        }
        
        RegCloseKey(hKey);
    } else {
        printf("Failed to open updates registry key\n");
    }
}

void printSystemInfo() {
    HKEY hKey;
    DWORD dwType = REG_SZ;
    DWORD dwSize = 256;
    char szValue[256];
    
    printf("\n=== System Information ===\n");
    
    // Open registry key for system info
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
                     "HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0", 
                     0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        
        // Get Processor Name
        dwSize = 256;
        if (RegQueryValueEx(hKey, "ProcessorNameString", NULL, &dwType, 
                           (LPBYTE)szValue, &dwSize) == ERROR_SUCCESS) {
            printf("Processor: %s\n", szValue);
        }
        
        RegCloseKey(hKey);
    }
    
    // Get Computer Name
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, 
                     "SYSTEM\\CurrentControlSet\\Control\\ComputerName\\ComputerName", 
                     0, KEY_READ, &hKey) == ERROR_SUCCESS) {
        
        dwSize = 256;
        if (RegQueryValueEx(hKey, "ComputerName", NULL, &dwType, 
                           (LPBYTE)szValue, &dwSize) == ERROR_SUCCESS) {
            printf("Computer Name: %s\n", szValue);
        }
        
        RegCloseKey(hKey);
    }
}

int main() {
    printf("Windows Registry OS Version and Patch Data Collector\n");
    printf("====================================================\n\n");
    
    printOSVersion();
    printInstalledUpdates();
    printSystemInfo();
    
    printf("\n=== Registry Data Collection Complete ===\n");
    return 0;
}
