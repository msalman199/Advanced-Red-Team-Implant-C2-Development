#ifndef CUSTOM_GETPROCADDR_H
#define CUSTOM_GETPROCADDR_H

// Function prototypes
void* custom_get_proc_address(void* module_handle, const char* proc_name);
void* custom_load_library(const char* library_name);
int enumerate_symbols(void* handle, const char* filter);
void* resolve_function_address(const char* library_path, const char* function_name);
void custom_free_library(void* handle);

#endif
