#include <stdio.h>
#include <windows.h>

int main() {
    printf("Hello, World!\n");
    MessageBox(NULL, "Hello from PE!", "Test", MB_OK);
    return 0;
}
