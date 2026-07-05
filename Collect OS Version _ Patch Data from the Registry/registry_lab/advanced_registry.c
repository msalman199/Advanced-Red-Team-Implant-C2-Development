#include <windows.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    char name[256];
    char value[512];
} RegistryEntry;

int scanRegistryKey(HKEY hRootKey, const char* subKey, const char* valueName) {
    HKEY hKey;
    DWORD dwType;
    DWORD dwSize = 512;
    char szValue[512];
    LONG result;
    
    result = RegOpenKeyEx(hRootKey, subKey, 0, KEY_READ, &hKey);
    
    if (result == ERROR_SUCCESS) {
        printf("Successfully opened key: %s\n", subKey);
        
        if (valueName) {
            result = RegQueryValueEx(hKey, valueName, NULL, &dwType, 
                                   (LPBYTE)szValue, &dwSize);
            if (result == ERROR_SUCCESS) {
                printf("  %s: %s\n", valueName, szValue);
            } else {
                printf("  Failed to read value: %s (Error: %ld)\n", valueName, result);
            }
        } else {
            // Enumerate all values in the key
            DWORD dwIndex = 0;
            char szValueName[256];
            DWORD dwValueNameSize = 256;
            
            while (RegEnumValue(hKey, dwIndex, szValueName, &dwValueNameSize,
                               NULL, &dwType, (LPBYTE)szValue, &dwSize) == ERROR_SUCCESS) {
                printf("  %s: %s\n", szValueName, szValue);
                dwIndex++;
                dwValueNameSize = 256;
                dwSize = 512;
            }
        }
        
        RegCloseKey(hKey);
        return 1;
    } else {
        printf("Failed to open key: %s (Error: %ld)\n", subKey, result);
        return 0;
    }
}

int main() {
    printf("Advanced Windows Registry Scanner\n");
    printf("=================================\n\n");
    
    // Scan OS Version Information
    printf("--- OS Version Information ---\n");
    scanRegistryKey(HKEY_LOCAL_MACHINE, 
                   "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 
                   "ProductName");
    scanRegistryKey(HKEY_LOCAL_MACHINE, 
                   "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 
                   "CurrentVersion");
    scanRegistryKey(HKEY_LOCAL_MACHINE, 
                   "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 
                   "CurrentBuild");
    
    // Scan Windows Update Information
    printf("\n--- Windows Update Information ---\n");
    scanRegistryKey(HKEY_LOCAL_MACHINE, 
                   "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\WindowsUpdate\\Auto Update", 
                   NULL);
    
    printf("\n=== Registry Scan Complete ===\n");
    return 0;
}
