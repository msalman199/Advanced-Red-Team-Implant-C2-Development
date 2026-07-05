#include "pe_parser.h"

// External function declaration
int parse_pe_with_imports(const char* filename);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <pe_file>\n", argv[0]);
        return 1;
    }

    printf("Enhanced PE Parser with Import Resolution\n");
    printf("========================================\n");
    printf("Analyzing file: %s\n", argv[1]);
    
    if (parse_pe_with_imports(argv[1]) == 0) {
        printf("\n[SUCCESS] PE file analysis completed!\n");
    } else {
        printf("\n[ERROR] Failed to analyze PE file!\n");
        return 1;
    }

    return 0;
}
