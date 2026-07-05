#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, 
                   LPSTR lpCmdLine, int nCmdShow) {
    
    MessageBox(NULL, 
               "Hello from Windows API!\nThis program runs on Linux using Wine.", 
               "My First Windows API Program", 
               MB_OK | MB_ICONINFORMATION);
    
    MessageBox(NULL, 
               "Do you want to see another message?", 
               "Question", 
               MB_YESNO | MB_ICONQUESTION);
    
    return 0;
}
