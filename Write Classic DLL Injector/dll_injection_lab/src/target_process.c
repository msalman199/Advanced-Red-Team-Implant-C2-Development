#include <stdio.h>
#include <unistd.h>
#include <signal.h>

volatile int running = 1;

void signal_handler(int sig) {
    printf("\nTarget process received signal %d, exiting...\n", sig);
    running = 0;
}

int main() {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
    
    printf("Target process started with PID: %d\n", getpid());
    printf("Press Ctrl+C to stop...\n");
    
    while(running) {
        printf("Target process running... PID: %d\n", getpid());
        sleep(3);
    }
    
    printf("Target process exiting...\n");
    return 0;
}
