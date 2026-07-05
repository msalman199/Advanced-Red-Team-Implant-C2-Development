#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void printLinuxOSInfo() {
    FILE *fp;
    char buffer[256];
    
    printf("=== Linux System Information ===\n");
    
    // Get OS Release Info
    fp = fopen("/etc/os-release", "r");
    if (fp != NULL) {
        while (fgets(buffer, sizeof(buffer), fp)) {
            if (strncmp(buffer, "PRETTY_NAME=", 12) == 0) {
                printf("OS: %s", buffer + 13);
            } else if (strncmp(buffer, "VERSION=", 8) == 0) {
                printf("Version: %s", buffer + 9);
            }
        }
        fclose(fp);
    }
    
    // Get Kernel Version
    fp = popen("uname -r", "r");
    if (fp != NULL) {
        if (fgets(buffer, sizeof(buffer), fp)) {
            printf("Kernel: %s", buffer);
        }
        pclose(fp);
    }
}

void printInstalledPackages() {
    FILE *fp;
    char buffer[256];
    int count = 0;
    
    printf("\n=== Installed Packages (Sample) ===\n");
    
    // Get installed packages using dpkg
    fp = popen("dpkg -l | grep '^ii' | head -10", "r");
    if (fp != NULL) {
        while (fgets(buffer, sizeof(buffer), fp) && count < 10) {
            printf("Package: %s", buffer);
            count++;
        }
        pclose(fp);
    }
}

void printSystemSpecs() {
    FILE *fp;
    char buffer[256];
    
    printf("\n=== Hardware Information ===\n");
    
    // Get CPU info
    fp = popen("grep 'model name' /proc/cpuinfo | head -1 | cut -d':' -f2", "r");
    if (fp != NULL) {
        if (fgets(buffer, sizeof(buffer), fp)) {
            printf("CPU:%s", buffer);
        }
        pclose(fp);
    }
    
    // Get Memory info
    fp = popen("grep 'MemTotal' /proc/meminfo | awk '{print $2 \" \" $3}'", "r");
    if (fp != NULL) {
        if (fgets(buffer, sizeof(buffer), fp)) {
            printf("Memory: %s", buffer);
        }
        pclose(fp);
    }
    
    // Get hostname
    if (gethostname(buffer, sizeof(buffer)) == 0) {
        printf("Hostname: %s\n", buffer);
    }
}

int main() {
    printf("Linux System Information Collector\n");
    printf("==================================\n\n");
    
    printLinuxOSInfo();
    printInstalledPackages();
    printSystemSpecs();
    
    printf("\n=== System Data Collection Complete ===\n");
    return 0;
}
