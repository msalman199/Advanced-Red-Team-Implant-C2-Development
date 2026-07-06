#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char *argv[]) {
    printf("[HIJACK] Persistence payload executed!\n");
    printf("[HIJACK] Original command: %s\n", argv[0]);
    printf("[HIJACK] PID: %d\n", getpid());
    
    // Log the hijack attempt
    system("echo \"$(date): IFEO hijack executed\" >> /tmp/hijack_log.txt");
    
    // Execute original program if it exists
    if (access("/tmp/target_app_original", F_OK) == 0) {
        printf("[HIJACK] Executing original program...\n");
        execv("/tmp/target_app_original", argv);
    }
    
    return 0;
}
