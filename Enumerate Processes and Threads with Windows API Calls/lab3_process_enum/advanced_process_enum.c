#include <windows.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <stdio.h>

void GetProcessMemoryInfo(DWORD processID) {
    HANDLE hProcess;
    PROCESS_MEMORY_COUNTERS pmc;
    
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (hProcess == NULL) {
        printf("Could not open process %lu for memory information\n", processID);
        return;
    }
    
    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
        printf("Memory Usage for Process ID %lu:\n", processID);
        printf("  Working Set Size: %lu KB\n", pmc.WorkingSetSize / 1024);
        printf("  Peak Working Set: %lu KB\n", pmc.PeakWorkingSetSize / 1024);
        printf("  Page File Usage: %lu KB\n", pmc.PagefileUsage / 1024);
    }
    
    CloseHandle(hProcess);
}

void DetailedProcessEnum() {
    HANDLE hProcessSnap;
    PROCESSENTRY32 pe32;
    int processCount = 0;
    
    printf("=== DETAILED PROCESS INFORMATION ===\n");
    
    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        printf("CreateToolhelp32Snapshot failed\n");
        return;
    }
    
    pe32.dwSize = sizeof(PROCESSENTRY32);
    
    if (Process32First(hProcessSnap, &pe32)) {
        do {
            processCount++;
            printf("\n--- Process #%d ---\n", processCount);
            printf("Process ID: %lu\n", pe32.th32ProcessID);
            printf("Process Name: %s\n", pe32.szExeFile);
            printf("Thread Count: %lu\n", pe32.cntThreads);
            printf("Parent Process ID: %lu\n", pe32.th32ParentProcessID);
            
            GetProcessMemoryInfo(pe32.th32ProcessID);
            
        } while (Process32Next(hProcessSnap, &pe32));
    }
    
    printf("\nTotal Processes Found: %d\n", processCount);
    CloseHandle(hProcessSnap);
}

int main() {
    printf("Advanced Windows API Process Monitor\n");
    printf("===================================\n");
    
    DetailedProcessEnum();
    
    return 0;
}
