#define section_readonly 'R'
#define section_readwrite 'W'
#define section_executable 'E'

#define symbol_depth 1024
#define symbol_namelen 64 - (sizeof(unsigned char)*2) - sizeof(unsigned int) - 4

typedef struct {
    unsigned char bytemagic[4]; // GYB0 or GYO0
    uint32_t entry;
    uint32_t readonly[2];   // offset, size
    uint32_t writeable[2];  // offset, size
    uint32_t executable[2]; // offset, size
    uint32_t symbols[2];    // offset, size
} header_t;

typedef struct {
    char name[symbol_namelen];
    unsigned int offset;
    unsigned char section;
    unsigned char defined;
} symbol_t;

typedef struct {
    symbol_t entries[symbol_depth];
} symboltable_t;