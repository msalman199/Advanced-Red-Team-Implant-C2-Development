#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

volatile int running = 1;

void signal_handler(int sig) {
    printf("[TARGET] Received signal %d\n", sig);
    running = 0;
}

int main() {
    signal(SIGTERM, signal_handler);
    signal(SIGINT, signal_handler);
    
    printf("[TARGET] Process started with PID: %d\n", getpid());
    printf("[TARGET] Waiting for APC injection...\n");
    
    while(running) {
        sleep(1);
        printf("[TARGET] Still running...\n");
    }
    
    printf("[TARGET] Process terminating\n");
    return 0;
}
