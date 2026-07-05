#ifndef PE_PARSER_H
#define PE_PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// DOS Header
typedef struct {
    uint16_t e_magic;      // MZ signature
    uint16_t e_cblp;       // Bytes on last page
    uint16_t e_cp;         // Pages in file
    uint16_t e_crlc;       // Relocations
    uint16_t e_cparhdr;    // Size of header in paragraphs
    uint16_t e_minalloc;   // Minimum extra paragraphs
    uint16_t e_maxalloc;   // Maximum extra paragraphs
    uint16_t e_ss;         // Initial relative SS value
    uint16_t e_sp;         // Initial SP value
    uint16_t e_csum;       // Checksum
    uint16_t e_ip;         // Initial IP value
    uint16_t e_cs;         // Initial relative CS value
    uint16_t e_lfarlc;     // Address of relocation table
    uint16_t e_ovno;       // Overlay number
    uint16_t e_res[4];     // Reserved words
    uint16_t e_oemid;      // OEM identifier
    uint16_t e_oeminfo;    // OEM information
    uint16_t e_res2[10];   // Reserved words
    uint32_t e_lfanew;     // File address of new exe header
} DOS_HEADER;

// PE Header
typedef struct {
    uint32_t signature;
    uint16_t machine;
    uint16_t numberOfSections;
    uint32_t timeDateStamp;
    uint32_t pointerToSymbolTable;
    uint32_t numberOfSymbols;
    uint16_t sizeOfOptionalHeader;
    uint16_t characteristics;
} PE_HEADER;

// Import Directory Entry
typedef struct {
    uint32_t importLookupTable;
    uint32_t timeDateStamp;
    uint32_t forwarderChain;
    uint32_t name;
    uint32_t importAddressTable;
} IMPORT_DIRECTORY_ENTRY;

// Function prototypes
int parse_pe_file(const char* filename);
void print_dos_header(DOS_HEADER* dos_header);
void print_pe_header(PE_HEADER* pe_header);
int resolve_imports(FILE* file, uint32_t import_table_rva, uint32_t import_table_size);

#endif
