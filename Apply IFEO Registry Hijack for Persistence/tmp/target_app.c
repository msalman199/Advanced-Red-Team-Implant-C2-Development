#include <stdio.h>
#include <unistd.h>

int main() {
    printf("Original target application running...\n");
    printf("PID: %d\n", getpid());
    sleep(5);
    printf("Target application finished.\n");
    return 0;
}
