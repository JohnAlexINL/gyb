#define symboltype_undefined    '\0'    // self-explanatory
#define symboltype_constant     '\1'    // replaced where is, not a ptr
#define symboltype_external     '@'     // expect definition elsewhere
#define symboltype_readable     'R'     // ptr to somewhere in readable section
#define symboltype_writeable    'W'     // ptr to somewhere in writable section
#define symboltype_executable   'E'     // ptr to somewhere in executable section
#define bytecode_buffer_default 1024
#define section_readonly 'R'
#define section_readwrite 'W'
#define section_executable 'E'
#define symbol_depth 1024
#define symbol_namelen 64 - (sizeof(unsigned char)*2) - sizeof(uint32_t) - 4
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
unsigned char bytecode_magic(char *buffer);
gybfile_t bytecode_new();
gybfile_t bytecode_load(char *filename);
int bytecode_save(char *filename, gybfile_t source);
uint32_t symbolhash(char *name);
symboltable_t *symtable_load(symboltable_t *table, char *source, int size);
symbol_t *symtable_flatten(symboltable_t *table);
uint32_t symboltable_count(symboltable_t *table);
bool symbol_new(symboltable_t *table, char *name, unsigned char section, uint32_t offset);
bool symbol_static(symbol_t *symbols, int count);
void symtable_import(symboltable_t *parent, symboltable_t *source);
void symtable_print(symboltable_t *table);
int file_read(char *filename, char *buffer, int max);
int file_write (char *filename, char *buffer, int max);
bool file_exists (char *filename);
int file_size(char *filename);
void file_route(char *path);
void file_delete(char *path);
const char badmissing[] = "could not find file \"%s\"\n";
const char badtype[] = "file \"%s\" is wrong type\n";
const char badentrysection[] = "error: entry symbol points to invalid section\n";
const char badentryoob[] = "error: entry symbol points outside of executable\n";
const char entrywarning[] = "warning: unable to find entry symbol,\ndefaulting to beginning of executable section\n";