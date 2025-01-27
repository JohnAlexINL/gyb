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
    uint32_t offset;
    unsigned char section;
    unsigned char defined;
} symbol_t;

typedef struct {
    symbol_t entries[symbol_depth];
} symboltable_t;

typedef struct {
    header_t header;
    char *readonly; int rbuffersize;
    char *writeable; int wbuffersize;
    char *executable; int ebuffersize;
    symboltable_t symtable;
} gybfile_t;
