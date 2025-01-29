void gyb_internal_pchar(gybfile_t *object, char *data, int size) {
    if (object->ebuffersize <= object->header.executable[1] + size) {
        object->executable = realloc(object->executable, object->header.executable[1] + size);
        object->ebuffersize = object->header.executable[1] + size;
    }   memcpy(object->executable + object->header.executable[1], data, size);
    object->header.executable[1] += size;
}

bool gyb_internal_pureg(gybfile_t *object, unsigned char op, register_t base) {
    if ( base > gybh_register_bp ) return false;
    char data[2] = {op, base};
    gyb_internal_pchar(object, data, 2); return true;
}

bool gyb_internal_pbreg(gybfile_t *object, unsigned char op, register_t base, register_t source){
    if ( base > gybh_register_bp || source > gybh_register_bp ) return false;
    char data[3] = {op, base, source};
    gyb_internal_pchar(object, data, 3); return true;
}

bool gyb_internal_pbsym(gybfile_t *object, unsigned char op, register_t base, gyb_symbol_t source){
    if ( base > gybh_register_bp ) { return false; } char data[6] = {op, base};
    int hash = symbolhash(source.name); memcpy(data + 2, &hash, 4);
    gyb_internal_pchar(object, data, 3); return true;
}

bool gyb_internal_pusym(gybfile_t *object, unsigned char op, gyb_symbol_t source) {
    char data[5] = {op};
    int hash = symbolhash(source.name); memcpy(data + 1, &hash, 4);
    gyb_internal_pchar(object, data, 6); return true;
}

bool gyb_bp_move  (gybfile_t *object, register_t base, register_t source)   { return gyb_internal_pbreg(object, gybh_move, base, source); }
bool gyb_bp_load  (gybfile_t *object, register_t base, gyb_symbol_t source) { return gyb_internal_pbsym(object, gybh_load, base, source); }
bool gyb_bp_store (gybfile_t *object, register_t base, gyb_symbol_t source) { return gyb_internal_pbsym(object, gybh_store, base, source); }
bool gyb_bp_imm   (gybfile_t *object, register_t base, gyb_symbol_t source) { return gyb_internal_pbsym(object, gybh_imm, base, source); }
bool gyb_bp_cmp   (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_cmp, base, adder); }
bool gyb_bp_add   (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_add, base, adder); }
bool gyb_bp_sub   (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_sub, base, adder); }
bool gyb_bp_mul   (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_mul, base, adder); }
bool gyb_bp_div   (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_div, base, adder); }
bool gyb_bp_left  (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_left, base, adder); }
bool gyb_bp_right (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_right, base, adder); }
bool gyb_bp_and   (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_and, base, adder); }
bool gyb_bp_or    (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_or, base, adder); }
bool gyb_bp_xor   (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_xor, base, adder); }
bool gyb_bp_flip  (gybfile_t *object, register_t base)                      { return gyb_internal_pureg(object, gybh_flip, base); }
bool gyb_bp_fadd  (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_fadd, base, adder); }
bool gyb_bp_fsub  (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_fsub, base, adder); }
bool gyb_bp_fmul  (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_fmul, base, adder); }
bool gyb_bp_fdiv  (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_fdiv, base, adder); }
bool gyb_bp_goto  (gybfile_t *object, gyb_symbol_t symbol)                  { return gyb_internal_pusym(object, gybh_goto, symbol); }
bool gyb_bp_if    (gybfile_t *object, gyb_symbol_t symbol)                  { return gyb_internal_pusym(object, gybh_if, symbol); }
bool gyb_bp_else  (gybfile_t *object, gyb_symbol_t symbol)                  { return gyb_internal_pusym(object, gybh_else, symbol); }
bool gyb_bp_lt    (gybfile_t *object, gyb_symbol_t symbol)                  { return gyb_internal_pusym(object, gybh_lt, symbol); }
bool gyb_bp_gt    (gybfile_t *object, gyb_symbol_t symbol)                  { return gyb_internal_pusym(object, gybh_gt, symbol); }
bool gyb_bp_le    (gybfile_t *object, gyb_symbol_t symbol)                  { return gyb_internal_pusym(object, gybh_le, symbol); }
bool gyb_bp_ge    (gybfile_t *object, gyb_symbol_t symbol)                  { return gyb_internal_pusym(object, gybh_ge, symbol); }
bool gyb_bp_fmov  (gybfile_t *object, register_t base)                      { return gyb_internal_pureg(object, gybh_fmov, base); }
bool gyb_bp_fcast (gybfile_t *object, register_t base)                      { return gyb_internal_pureg(object, gybh_fcast, base); }
bool gyb_bp_imov  (gybfile_t *object, register_t base)                      { return gyb_internal_pureg(object, gybh_imov, base); }
bool gyb_bp_icast (gybfile_t *object, register_t base)                      { return gyb_internal_pureg(object, gybh_icast, base); }
bool gyb_bp_push  (gybfile_t *object, register_t base)                      { return gyb_internal_pureg(object, gybh_push, base); }
bool gyb_bp_pop   (gybfile_t *object, register_t base)                      { return gyb_internal_pureg(object, gybh_pop, base); }
bool gyb_bp_sys1  (gybfile_t *object, register_t arg0)                      { return gyb_internal_pureg(object, gybh_sys1, arg0); }
bool gyb_bp_sys2  (gybfile_t *object, register_t arg0, register_t arg1)     { return gyb_internal_pbreg(object, gybh_sys2, arg0, arg1); }

bool gyb_bp_sys3  (gybfile_t *object, register_t arg0, register_t arg1, register_t arg2) {
    if ( arg0 > gybh_register_bp || arg1 > gybh_register_bp || arg2 > gybh_register_bp ) return false;
    char data[5] = {gybh_sys3, arg0, arg1, arg2};
    gyb_internal_pchar(object, data, 5); return true;
}
bool gyb_bp_sys4  (gybfile_t *object, register_t arg0, register_t arg1, register_t arg2, register_t arg3) {
    if ( arg0 > gybh_register_bp || arg1 > gybh_register_bp || arg2 > gybh_register_bp || arg3 > gybh_register_bp ) return false;
    char data[6] = {gybh_sys4, arg0, arg1, arg2, arg3};
    gyb_internal_pchar(object, data, 6); return true;
}
unsigned char gyb_bytecode_magic(char *buffer) {
    if ( buffer[0] != 'G' )     { return -1; }
    if ( buffer[1] != 'Y' )     { return -1; }
    if ( buffer[3] != '\0' )    { return -1; }
    if ( buffer[2] == 'O' ) { return 'O'; }
    if ( buffer[2] == 'B' ) { return 'B'; }
}

gybfile_t gyb_bytecode_new() {
    gybfile_t result;
    strncpy(result.header.bytemagic, "GYO\0", 4);
    result.rbuffersize = sizeof(gyb_bytecode_buffer_default);
    result.wbuffersize = sizeof(gyb_bytecode_buffer_default);
    result.ebuffersize = sizeof(gyb_bytecode_buffer_default);
    result.readonly = malloc(sizeof(gyb_bytecode_buffer_default));
    result.writeable = malloc(sizeof(gyb_bytecode_buffer_default));
    result.executable = malloc(sizeof(gyb_bytecode_buffer_default));
    result.header.readonly[1] = 0;
    result.header.writeable[1] = 0;
    result.header.executable[1] = 0;
    return result;
}

gybfile_t gyb_bytecode_load(char *filename) {
    gybfile_t gybfile;
    // Load the object file into memory
    if ( file_exists(filename) == false ) {
        printf(badmissing, filename); exit(2); }
    int buffersize = file_size(filename);
    char buffer[buffersize];
    file_read(filename, buffer, buffersize);
    // Check that the object is a relocatable type
    unsigned char type = gyb_bytecode_magic(buffer);
    if (type != 'B' || type != 'O') {
        printf(badtype, filename); exit(2); }
    // Load the header
    memcpy(&gybfile.header, buffer, sizeof(gyb_header_t));
    // Load the readonly section
    gybfile.readonly = malloc(gybfile.header.readonly[1]);
    memcpy(gybfile.readonly, buffer+gybfile.header.readonly[0], gybfile.header.readonly[1]);
    // Load the writeable section
    gybfile.writeable = malloc(gybfile.header.writeable[1]);
    memcpy(gybfile.writeable, buffer+gybfile.header.writeable[0], gybfile.header.writeable[1]);
    // Load the executable section
    gybfile.executable = malloc(gybfile.header.executable[1]);
    memcpy(gybfile.executable, buffer+gybfile.header.executable[0], gybfile.header.executable[1]);
    // Load the symbol table
    gyb_symtable_load(&gybfile.symtable, buffer+gybfile.header.symbols[0], gybfile.header.symbols[1]);
    return gybfile;
}

int gyb_bytecode_save(char *filename, gybfile_t source) {
    // Prep the symboltable
    int symbols = symboltable_count(&source.symtable);
    int symtablesize = symbols * sizeof(gyb_symbol_t);
    gyb_symbol_t *symtable = gyb_symtable_flatten(&source.symtable);
    bool symstatic = gyb_symbol_static(symtable, symtablesize);
    int filesize = (
        sizeof(gyb_header_t) +
        source.header.readonly[1] +
        source.header.writeable[1] +
        source.header.executable[1] +
        symtablesize
    );
    // If static, try to link entry
    if ( symstatic == true ) {
        source.header.bytemagic[2] = 'B';
        int entryhash = symbolhash("entry");
        if ( source.symtable.entries[entryhash].defined == symboltype_undefined ) {
            source.header.entry = source.header.executable[0];
            printf(entrywarning);
        } else {
            if ( source.symtable.entries[entryhash].section != section_executable ) { printf(badentrysection); exit(1); }
            if ( source.symtable.entries[entryhash].offset >= source.header.executable[1] ) { printf(badentryoob); exit(1); }
            source.header.entry = source.header.executable[0] + source.symtable.entries[entryhash].offset;
        }
    } else {
        source.header.bytemagic[2] = 'O';
    }
    // Update the section offsets
    source.header.readonly[0] =     sizeof(gyb_header_t);
    source.header.writeable[0] =    source.header.readonly[0] + source.header.readonly[1];
    source.header.executable[0] =   source.header.writeable[0] + source.header.writeable[1];
    source.header.symbols[0] =      source.header.executable[0] + source.header.executable[1];
    // Prep the actual file content
    char *buffer = malloc(filesize);
    int cursor = 0;
    memcpy(buffer+cursor, &source.header, sizeof(gyb_header_t)); cursor += sizeof(gyb_header_t);
    memcpy(buffer+cursor, source.readonly, source.header.readonly[1]); cursor += source.header.readonly[1];
    memcpy(buffer+cursor, source.writeable, source.header.writeable[1]); cursor += source.header.writeable[1];
    memcpy(buffer+cursor, source.executable, source.header.executable[1]); cursor += source.header.executable[1];
    memcpy(buffer+cursor, symtable, symtablesize);
    return file_write(filename, buffer, filesize);
}/* REQUIRES: <sys/stat.h> */
int file_read(char *filename, char *buffer, int max) {
    FILE *fp = fopen(filename, "rb");
    if ( fp == NULL ) { return false; }
    int cursor = 0; char c; while(cursor < max -1) {
      c = fgetc(fp);
      if( feof(fp) ) { break; }
      buffer[cursor] = c; cursor++;
   } fclose(fp); return cursor;
}

// we need file_write to still write out NULL characters for this application
int file_write (char *filename, char *buffer, int max) {
    FILE *fp = fopen(filename, "w+");
    if ( fp == NULL ) { return false; }
    int cursor; for (cursor = 0; cursor < max /*&& buffer[cursor] != '\0'*/; cursor++) {
        if (fputc(buffer[cursor], fp) == EOF) { fclose(fp); return false; }
    } fclose(fp); return cursor;
}

bool file_exists (char *filename) { struct stat buffer; return (stat(filename, &buffer) == 0); }
int file_size(char *filename) { struct stat st; stat(filename, &st); return st.st_size; }

void file_route(char *path) {
    #ifdef _WIN_32
        int len = strlen(path); int i;
        for(i=0;i<len;i++) { if (path[i]=='/') { path[i]='\\'; } }
    #endif
}

void file_delete(char *path) {
    char command_path[1024]; file_route(path);
    #ifdef _WIN_32
        sprintf(command_path, "del %s", path);
    #else
        sprintf(command_path, "rm %s", path);
    #endif
    system(command_path);
}uint32_t symbolhash(char *name){
    int i; uint32_t a=0; uint32_t b=0; uint32_t c=0;
    uint32_t s=0;
    for(i=0;i<gyb_symbol_namelen;i++) {
        if ( name[i]==0 ) { break; }
        a += name[i] % 3;
        b += name[i] % 5;
        c += name[i] % 7;
        s += name[i];
    }
    return (
        (strnlen(name, gyb_symbol_namelen) << 24) +
        (s << 16) +
        (c << 8) +
        (b << 4) +
        a
    ) % gyb_symbol_depth;
}

uint32_t symboltable_count(gyb_symboltable_t *table) {
    int count = 0;
    int i; for(i=0;i<gyb_symbol_depth;i++) {
        if ( table->entries[i].defined != symboltype_undefined ) { count++; }
    }   return count;
}

bool gyb_symbol_static(gyb_symbol_t *symbols, int count) {
    int i; for(i=0;i<count;i++) { if ( symbols[i].defined == symboltype_external ) { return false; } }
    return true;
}

bool gyb_symbol_new(gyb_symboltable_t *table, char *name, unsigned char section, uint32_t offset) {
    int hash = symbolhash(name);
    // Regular Symbol
    if ( section != symboltype_external ) {
    if (table->entries[hash].defined == symboltype_undefined ||
        table->entries[hash].defined == symboltype_external ) {
        table->entries[hash].defined = 1;
        table->entries[hash].section = section;
        table->entries[hash].offset  = offset;
        strncpy(table->entries[hash].name, name, gyb_symbol_namelen);
        return true;
    }   return false; }
    // Undefined External Symbol
    else if (table->entries[hash].defined == symboltype_undefined) {
        table->entries[hash].defined = 1;
        table->entries[hash].section = section;
        table->entries[hash].offset  = offset;
        strncpy(table->entries[hash].name, name, gyb_symbol_namelen);
        return true;
    }
}

gyb_symboltable_t *gyb_symtable_load(gyb_symboltable_t *table, char *source, int size) {
    if ( sizeof(gyb_symbol_t) % size != 0 ) {
        printf("ERROR: symboltable of incorrect size %d, %d hanging bytes\n",
            size, size % sizeof(gyb_symbol_t)
        );  exit(1);
    }   int symbolcount = size / sizeof(gyb_symbol_t);
    gyb_symbol_t sourcetable[symbolcount];
    memcpy(sourcetable, source, sizeof(sourcetable));
    int i; for(i=0;i<symbolcount;i++) {
        bool check = gyb_symbol_new(
            table,
            sourcetable[i].name,
            sourcetable[i].section,
            sourcetable[i].offset
        );  if ( check == false ) {
            printf("gyb_symtable_load, redefinition of symbol %s\n",
            sourcetable[i].name); exit(2);
        }
    }
    return table;
}

void gyb_symtable_import(gyb_symboltable_t *parent, gyb_symboltable_t *source) {
    int i; for(i=0;i<gyb_symbol_depth;i++) {
        if ( source->entries[i].defined == symboltype_undefined ) { continue; }
        gyb_symbol_new(
            parent,
            source->entries[i].name,
            source->entries[i].section,
            source->entries[i].offset
        );
    }
}

void gyb_symbol_print(gyb_symbol_t *symbol) { printf("    %-32s %c %08x\n", symbol->name, symbol->section, symbol->offset); }
void gyb_symtable_print(gyb_symboltable_t *table){
    printf("SYMBOL                         SECTION OFFSET\n");
    int i; for(i=0;i<gyb_symbol_depth;i++) {
        if ( table->entries[i].defined == 0 ) { continue; }
        gyb_symbol_print(&table->entries[i]);
    }
}

gyb_symbol_t *gyb_symtable_flatten(gyb_symboltable_t *table){
    int count = symboltable_count(table);
    gyb_symbol_t *result = malloc(sizeof(gyb_symbol_t) * count);
    int i; for(i=0;i<gyb_symbol_depth;i++) {
        if ( table->entries[i].defined == 0 ) { continue; }
        memcpy(&result[i], &table->entries[i], sizeof(gyb_symbol_t));
    }
    return result;
}void gyb_debug_printheader(gyb_header_t *source) {
    printf("Header:\n");
    printf("   bytemagic: %c%c%c%c\n", source->bytemagic[0], source->bytemagic[1], source->bytemagic[2], source->bytemagic[3]);
    printf("   entry:      %08x\n", source->entry);
    printf("   readonly:   %08x, size %d\n", source->readonly[0], source->readonly[1]);
    printf("   writeable:  %08x, size %d\n", source->writeable[0], source->writeable[1]);
    printf("   executable: %08x, size %d\n", source->executable[0], source->executable[1]);
    printf("   symbols:    %08x, size %d\n", source->symbols[0], source->symbols[1]);
}