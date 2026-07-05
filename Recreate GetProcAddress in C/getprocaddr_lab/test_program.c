#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "custom_getprocaddr.h"

// Function pointer types for testing
typedef double (*sqrt_func_t)(double);
typedef int (*printf_func_t)(const char*, ...);
typedef void* (*malloc_func_t)(size_t);

void test_math_functions() {
    printf("\n=== Testing Math Library Functions ===\n");
    
    // Load math library
    void* math_handle = custom_load_library("libm.so.6");
    if (!math_handle) {
        printf("[-] Failed to load math library\n");
        return;
    }
    
    // Get sqrt function
    sqrt_func_t sqrt_ptr = (sqrt_func_t)custom_get_proc_address(math_handle, "sqrt");
    if (sqrt_ptr) {
        double result = sqrt_ptr(16.0);
        printf("[+] sqrt(16.0) = %.2f\n", result);
    }
    
    // Get sin function
    double (*sin_ptr)(double) = (double(*)(double))custom_get_proc_address(math_handle, "sin");
    if (sin_ptr) {
        double result = sin_ptr(3.14159 / 2);
        printf("[+] sin(π/2) = %.6f\n", result);
    }
    
    custom_free_library(math_handle);
}

void test_libc_functions() {
    printf("\n=== Testing LibC Functions ===\n");
    
    // Load C library
    void* libc_handle = custom_load_library("libc.so.6");
    if (!libc_handle) {
        printf("[-] Failed to load libc\n");
        return;
    }
    
    // Get malloc function
    malloc_func_t malloc_ptr = (malloc_func_t)custom_get_proc_address(libc_handle, "malloc");
    if (malloc_ptr) {
        void* ptr = malloc_ptr(100);
        printf("[+] malloc(100) returned: %p\n", ptr);
        
        // Get free function and clean up
        void (*free_ptr)(void*) = (void(*)(void*))custom_get_proc_address(libc_handle, "free");
        if (free_ptr && ptr) {
            free_ptr(ptr);
            printf("[+] Memory freed successfully\n");
        }
    }
    
    // Test strlen function
    size_t (*strlen_ptr)(const char*) = (size_t(*)(const char*))custom_get_proc_address(libc_handle, "strlen");
    if (strlen_ptr) {
        const char* test_str = "Hello, World!";
        size_t len = strlen_ptr(test_str);
        printf("[+] strlen('%s') = %zu\n", test_str, len);
    }
    
    custom_free_library(libc_handle);
}

void test_symbol_enumeration() {
    printf("\n=== Testing Symbol Enumeration ===\n");
    
    void* handle = custom_load_library("libm.so.6");
    if (handle) {
        enumerate_symbols(handle, NULL);
        custom_free_library(handle);
    }
}

void test_direct_resolution() {
    printf("\n=== Testing Direct Function Resolution ===\n");
    
    // Resolve function directly
    void* cos_func = resolve_function_address("libm.so.6", "cos");
    if (cos_func) {
        double (*cos_ptr)(double) = (double(*)(double))cos_func;
        double result = cos_ptr(0.0);
        printf("[+] cos(0.0) = %.6f\n", result);
    }
}

int main() {
    printf("Custom GetProcAddress Implementation Test\n");
    printf("=========================================\n");
    
    test_math_functions();
    test_libc_functions();
    test_symbol_enumeration();
    test_direct_resolution();
    
    printf("\n[+] All tests completed!\n");
    return 0;
}
