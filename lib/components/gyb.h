typedef struct {
    unsigned char bytemagic[4]; // GYB0 or GYO0
    uint32_t entry;
    uint32_t readonly[2];   // offset, size
    uint32_t writeable[2];  // offset, size
    uint32_t executable[2]; // offset, size
    uint32_t symbols[2];    // offset, size
} gyb_header_t;

typedef struct {
    char name[gyb_symbol_namelen];
    uint32_t offset;
    unsigned char section;
    unsigned char defined;
} gyb_symbol_t;

typedef struct {
    gyb_symbol_t entries[gyb_symbol_depth];
} gyb_symboltable_t;

typedef struct {
    gyb_header_t header;
    char *readonly; int rbuffersize;
    char *writeable; int wbuffersize;
    char *executable; int ebuffersize;
    gyb_symboltable_t symtable;
} gybfile_t;
