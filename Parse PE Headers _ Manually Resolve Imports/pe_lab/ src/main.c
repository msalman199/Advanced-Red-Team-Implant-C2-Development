#include "pe_parser.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <pe_file>\n", argv[0]);
        return 1;
    }

    printf("Parsing PE file: %s\n", argv[1]);
    
    if (parse_pe_file(argv[1]) == 0) {
        printf("\nPE file parsed successfully!\n");
    } else {
        printf("\nFailed to parse PE file!\n");
        return 1;
    }

    return 0;
}
