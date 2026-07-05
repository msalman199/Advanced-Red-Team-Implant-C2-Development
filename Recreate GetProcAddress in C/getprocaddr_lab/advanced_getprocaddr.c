#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <errno.h>
#include <time.h>

typedef enum {
    LOG_INFO,
    LOG_WARNING,
    LOG_ERROR
} log_level_t;

void log_message(log_level_t level, const char* format, ...) {
    time_t now;
    time(&now);
    char* time_str = ctime(&now);
    time_str[strlen(time_str) - 1] = '\0'; // Remove newline
    
    const char* level_str[] = {"INFO", "WARN", "ERROR"};
    printf("[%s] %s: ", time_str, level_str[level]);
    
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);
    printf("\n");
}

void* safe_get_proc_address(void* handle, const char* proc_name) {
    if (!handle) {
        log_message(LOG_ERROR, "Invalid module handle");
        return NULL;
    }
    
    if (!proc_name || strlen(proc_name) == 0) {
        log_message(LOG_ERROR, "Invalid procedure name");
        return NULL;
    }
    
    // Clear any previous errors
    dlerror();
    
    void* symbol = dlsym(handle, proc_name);
    char* error = dlerror();
    
    if (error) {
        log_message(LOG_ERROR, "Symbol resolution failed: %s", error);
        return NULL;
    }
    
    if (!symbol) {
        log_message(LOG_WARNING, "Symbol '%s' resolved to NULL", proc_name);
        return NULL;
    }
    
    log_message(LOG_INFO, "Successfully resolved '%s' at %p", proc_name, symbol);
    return symbol;
}

int main() {
    printf("Advanced GetProcAddress with Error Handling\n");
    printf("===========================================\n");
    
    void* handle = dlopen("libm.so.6", RTLD_LAZY);
    if (!handle) {
        log_message(LOG_ERROR, "Failed to load library: %s", dlerror());
        return 1;
    }
    
    // Test valid function
    void* sqrt_func = safe_get_proc_address(handle, "sqrt");
    
    // Test invalid function
    void* invalid_func = safe_get_proc_address(handle, "nonexistent_function");
    
    // Test NULL parameters
    void* null_test = safe_get_proc_address(NULL, "sqrt");
    void* empty_test = safe_get_proc_address(handle, "");
    
    dlclose(handle);
    return 0;
}
