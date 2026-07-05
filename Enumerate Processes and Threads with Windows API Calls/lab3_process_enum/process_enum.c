#include <windows.h>
#include <tlhelp32.h>
#include <stdio.h>
#include <tchar.h>

void EnumerateProcesses() {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    
    printf("=== ACTIVE PROCESSES ===\n");
    printf("%-8s %-30s %-8s %-8s\n", "PID", "Process Name", "Threads", "Parent PID");
    printf("================================================================\n");
    
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        printf("CreateToolhelp32Snapshot failed\n");
        return;
    }
    
    pe32.dwSize = sizeof(PROCESSENTRY32);
    
    if (!Process32First(hProcessSnap, &pe32)) {
        printf("Process32First failed\n");
        CloseHandle(hProcessSnap);
        return;
    }
    
    do {
        printf("%-8lu %-30s %-8lu %-8lu\n", 
               pe32.th32ProcessID, 
               pe32.szExeFile, 
               pe32.cntThreads, 
               pe32.th32ParentProcessID);
    } while (Process32Next(hProcessSnap, &pe32));
    
    CloseHandle(hProcessSnap);
}

void EnumerateThreads(DWORD processID) {
    HANDLE hThreadSnap;
    THREADENTRY32 te32;
    
    printf("\n=== THREADS FOR PROCESS ID: %lu ===\n", processID);
    printf("%-8s %-8s %-12s %-8s\n", "Thread ID", "Process ID", "Base Priority", "Delta Priority");
    printf("================================================\n");
    
    hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
    if (hThreadSnap == INVALID_HANDLE_VALUE) {
        printf("CreateToolhelp32Snapshot failed for threads\n");
        return;
    }
    
    te32.dwSize = sizeof(THREADENTRY32);
    
    if (!Thread32First(hThreadSnap, &te32)) {
        printf("Thread32First failed\n");
        CloseHandle(hThreadSnap);
        return;
    }
    
    do {
        if (te32.th32OwnerProcessID == processID) {
            printf("%-8lu %-8lu %-12ld %-8ld\n", 
                   te32.th32ThreadID, 
                   te32.th32OwnerProcessID, 
                   te32.tpBasePri, 
                   te32.tpDeltaPri);
        }
    } while (Thread32Next(hThreadSnap, &te32));
    
    CloseHandle(hThreadSnap);
}

int main() {
    printf("Windows API Process and Thread Enumeration\n");
    printf("==========================================\n\n");
    
    EnumerateProcesses();
    
    DWORD targetPID;
    printf("\nEnter a Process ID to view its threads (0 to skip): ");
    scanf("%lu", &targetPID);
    
    if (targetPID > 0) {
        EnumerateThreads(targetPID);
    }
    
    printf("\nEnumeration completed successfully!\n");
    return 0;
}
