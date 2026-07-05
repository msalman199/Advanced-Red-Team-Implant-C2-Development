#include "pe_parser.h"

int parse_pe_file(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return -1;
    }

    // Read DOS header
    DOS_HEADER dos_header;
    fread(&dos_header, sizeof(DOS_HEADER), 1, file);
    
    if (dos_header.e_magic != 0x5A4D) {  // "MZ"
        printf("Error: Invalid DOS signature\n");
        fclose(file);
        return -1;
    }

    print_dos_header(&dos_header);

    // Seek to PE header
    fseek(file, dos_header.e_lfanew, SEEK_SET);
    
    // Read PE header
    PE_HEADER pe_header;
    fread(&pe_header, sizeof(PE_HEADER), 1, file);
    
    if (pe_header.signature != 0x00004550) {  // "PE\0\0"
        printf("Error: Invalid PE signature\n");
        fclose(file);
        return -1;
    }

    print_pe_header(&pe_header);

    // Skip optional header for now
    fseek(file, pe_header.sizeOfOptionalHeader, SEEK_CUR);

    fclose(file);
    return 0;
}

void print_dos_header(DOS_HEADER* dos_header) {
    printf("\n=== DOS HEADER ===\n");
    printf("Magic: 0x%04X\n", dos_header->e_magic);
    printf("PE Header Offset: 0x%08X\n", dos_header->e_lfanew);
}

void print_pe_header(PE_HEADER* pe_header) {
    printf("\n=== PE HEADER ===\n");
    printf("Signature: 0x%08X\n", pe_header->signature);
    printf("Machine: 0x%04X\n", pe_header->machine);
    printf("Number of Sections: %d\n", pe_header->numberOfSections);
    printf("Size of Optional Header: %d\n", pe_header->sizeOfOptionalHeader);
    printf("Characteristics: 0x%04X\n", pe_header->characteristics);
}
