#include "pe_parser.h"

typedef struct {
    uint32_t virtualAddress;
    uint32_t size;
} DATA_DIRECTORY;

typedef struct {
    uint16_t magic;
    uint8_t majorLinkerVersion;
    uint8_t minorLinkerVersion;
    uint32_t sizeOfCode;
    uint32_t sizeOfInitializedData;
    uint32_t sizeOfUninitializedData;
    uint32_t addressOfEntryPoint;
    uint32_t baseOfCode;
    uint64_t imageBase;
    uint32_t sectionAlignment;
    uint32_t fileAlignment;
    uint16_t majorOperatingSystemVersion;
    uint16_t minorOperatingSystemVersion;
    uint16_t majorImageVersion;
    uint16_t minorImageVersion;
    uint16_t majorSubsystemVersion;
    uint16_t minorSubsystemVersion;
    uint32_t win32VersionValue;
    uint32_t sizeOfImage;
    uint32_t sizeOfHeaders;
    uint32_t checkSum;
    uint16_t subsystem;
    uint16_t dllCharacteristics;
    uint64_t sizeOfStackReserve;
    uint64_t sizeOfStackCommit;
    uint64_t sizeOfHeapReserve;
    uint64_t sizeOfHeapCommit;
    uint32_t loaderFlags;
    uint32_t numberOfRvaAndSizes;
    DATA_DIRECTORY dataDirectory[16];
} OPTIONAL_HEADER64;

int resolve_imports(FILE* file, uint32_t import_table_rva, uint32_t import_table_size) {
    if (import_table_rva == 0 || import_table_size == 0) {
        printf("No import table found\n");
        return 0;
    }

    printf("\n=== IMPORT TABLE ===\n");
    printf("RVA: 0x%08X, Size: %d bytes\n", import_table_rva, import_table_size);

    // Seek to import table (simplified - assumes RVA = file offset)
    fseek(file, import_table_rva, SEEK_SET);

    IMPORT_DIRECTORY_ENTRY import_entry;
    int dll_count = 0;

    while (1) {
        fread(&import_entry, sizeof(IMPORT_DIRECTORY_ENTRY), 1, file);
        
        // Check for end of import table
        if (import_entry.name == 0) {
            break;
        }

        dll_count++;
        printf("\nDLL #%d:\n", dll_count);
        printf("  Import Lookup Table: 0x%08X\n", import_entry.importLookupTable);
        printf("  Name RVA: 0x%08X\n", import_entry.name);
        printf("  Import Address Table: 0x%08X\n", import_entry.importAddressTable);

        // Read DLL name (simplified)
        long current_pos = ftell(file);
        fseek(file, import_entry.name, SEEK_SET);
        
        char dll_name[256];
        fgets(dll_name, sizeof(dll_name), file);
        printf("  DLL Name: %s", dll_name);
        
        fseek(file, current_pos, SEEK_SET);
    }

    printf("\nTotal DLLs imported: %d\n", dll_count);
    return dll_count;
}

int parse_pe_with_imports(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Cannot open file %s\n", filename);
        return -1;
    }

    // Read DOS header
    DOS_HEADER dos_header;
    fread(&dos_header, sizeof(DOS_HEADER), 1, file);
    
    if (dos_header.e_magic != 0x5A4D) {
        printf("Error: Invalid DOS signature\n");
        fclose(file);
        return -1;
    }

    // Seek to PE header
    fseek(file, dos_header.e_lfanew, SEEK_SET);
    
    // Read PE header
    PE_HEADER pe_header;
    fread(&pe_header, sizeof(PE_HEADER), 1, file);
    
    if (pe_header.signature != 0x00004550) {
        printf("Error: Invalid PE signature\n");
        fclose(file);
        return -1;
    }

    print_pe_header(&pe_header);

    // Read optional header (simplified for 64-bit)
    if (pe_header.sizeOfOptionalHeader > 0) {
        OPTIONAL_HEADER64 opt_header;
        fread(&opt_header, sizeof(OPTIONAL_HEADER64), 1, file);
        
        printf("\n=== OPTIONAL HEADER ===\n");
        printf("Magic: 0x%04X\n", opt_header.magic);
        printf("Entry Point: 0x%08X\n", opt_header.addressOfEntryPoint);
        printf("Image Base: 0x%016lX\n", opt_header.imageBase);
        
        // Import table is at index 1 in data directory
        if (opt_header.numberOfRvaAndSizes > 1) {
            resolve_imports(file, 
                          opt_header.dataDirectory[1].virtualAddress,
                          opt_header.dataDirectory[1].size);
        }
    }

    fclose(file);
    return 0;
}
