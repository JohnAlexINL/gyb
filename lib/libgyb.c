unsigned char bytecode_magic(char *buffer) {
    if ( buffer[0] != 'G' )     { return -1; }
    if ( buffer[1] != 'Y' )     { return -1; }
    if ( buffer[3] != '\0' )    { return -1; }
    if ( buffer[2] == 'O' ) { return 'O'; }
    if ( buffer[2] == 'B' ) { return 'B'; }
}

gybfile_t bytecode_new() {
    gybfile_t result;
    strncpy(result.header.bytemagic, "GYO\0", 4);
    result.rbuffersize = sizeof(bytecode_buffer_default);
    result.wbuffersize = sizeof(bytecode_buffer_default);
    result.ebuffersize = sizeof(bytecode_buffer_default);
    result.readonly = malloc(sizeof(bytecode_buffer_default));
    result.writeable = malloc(sizeof(bytecode_buffer_default));
    result.executable = malloc(sizeof(bytecode_buffer_default));
    result.header.readonly[1] = 0;
    result.header.writeable[1] = 0;
    result.header.executable[1] = 0;
    return result;
}

gybfile_t bytecode_load(char *filename) {
    gybfile_t gybfile;
    // Load the object file into memory
    if ( file_exists(filename) == false ) {
        printf(badmissing, filename); exit(2); }
    int buffersize = file_size(filename);
    char buffer[buffersize];
    file_read(filename, buffer, buffersize);
    // Check that the object is a relocatable type
    unsigned char type = bytecode_magic(buffer);
    if (type != 'B' || type != 'O') {
        printf(badtype, filename); exit(2); }
    // Load the header
    memcpy(&gybfile.header, buffer, sizeof(header_t));
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
    symtable_load(&gybfile.symtable, buffer+gybfile.header.symbols[0], gybfile.header.symbols[1]);
    return gybfile;
}

int bytecode_save(char *filename, gybfile_t source) {
    // Prep the symboltable
    int symbols = symboltable_count(&source.symtable);
    int symtablesize = symbols * sizeof(symbol_t);
    symbol_t *symtable = symtable_flatten(&source.symtable);
    bool symstatic = symbol_static(symtable, symtablesize);
    int filesize = (
        sizeof(header_t) +
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
    source.header.readonly[0] =     sizeof(header_t);
    source.header.writeable[0] =    source.header.readonly[0] + source.header.readonly[1];
    source.header.executable[0] =   source.header.writeable[0] + source.header.writeable[1];
    source.header.symbols[0] =      source.header.executable[0] + source.header.executable[1];
    // Prep the actual file content
    char *buffer = malloc(filesize);
    int cursor = 0;
    memcpy(buffer+cursor, &source.header, sizeof(header_t)); cursor += sizeof(header_t);
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
    for(i=0;i<symbol_namelen;i++) {
        if ( name[i]==0 ) { break; }
        a += name[i] % 3;
        b += name[i] % 5;
        c += name[i] % 7;
        s += name[i];
    }
    return (
        (strnlen(name, symbol_namelen) << 24) +
        (s << 16) +
        (c << 8) +
        (b << 4) +
        a
    ) % symbol_depth;
}

uint32_t symboltable_count(symboltable_t *table) {
    int count = 0;
    int i; for(i=0;i<symbol_depth;i++) {
        if ( table->entries[i].defined != symboltype_undefined ) { count++; }
    }   return count;
}

bool symbol_static(symbol_t *symbols, int count) {
    int i; for(i=0;i<count;i++) { if ( symbols[i].defined == symboltype_external ) { return false; } }
    return true;
}

bool symbol_new(symboltable_t *table, char *name, unsigned char section, uint32_t offset) {
    int hash = symbolhash(name);
    // Regular Symbol
    if ( section != symboltype_external ) {
    if (table->entries[hash].defined == symboltype_undefined ||
        table->entries[hash].defined == symboltype_external ) {
        table->entries[hash].defined = 1;
        table->entries[hash].section = section;
        table->entries[hash].offset  = offset;
        strncpy(table->entries[hash].name, name, symbol_namelen);
        return true;
    }   return false; }
    // Undefined External Symbol
    else if (table->entries[hash].defined == symboltype_undefined) {
        table->entries[hash].defined = 1;
        table->entries[hash].section = section;
        table->entries[hash].offset  = offset;
        strncpy(table->entries[hash].name, name, symbol_namelen);
        return true;
    }
}

symboltable_t *symtable_load(symboltable_t *table, char *source, int size) {
    if ( sizeof(symbol_t) % size != 0 ) {
        printf("ERROR: symboltable of incorrect size %d, %d hanging bytes\n",
            size, size % sizeof(symbol_t)
        );  exit(1);
    }   int symbolcount = size / sizeof(symbol_t);
    symbol_t sourcetable[symbolcount];
    memcpy(sourcetable, source, sizeof(sourcetable));
    int i; for(i=0;i<symbolcount;i++) {
        bool check = symbol_new(
            table,
            sourcetable[i].name,
            sourcetable[i].section,
            sourcetable[i].offset
        );  if ( check == false ) {
            printf("symtable_load, redefinition of symbol %s\n",
            sourcetable[i].name); exit(2);
        }
    }
    return table;
}

void symtable_import(symboltable_t *parent, symboltable_t *source) {
    int i; for(i=0;i<symbol_depth;i++) {
        if ( source->entries[i].defined == symboltype_undefined ) { continue; }
        symbol_new(
            parent,
            source->entries[i].name,
            source->entries[i].section,
            source->entries[i].offset
        );
    }
}

void symbol_print(symbol_t *symbol) { printf("    %-32s %c %08x\n", symbol->name, symbol->section, symbol->offset); }
void symtable_print(symboltable_t *table){
    printf("SYMBOL                         SECTION OFFSET\n");
    int i; for(i=0;i<symbol_depth;i++) {
        if ( table->entries[i].defined == 0 ) { continue; }
        symbol_print(&table->entries[i]);
    }
}

symbol_t *symtable_flatten(symboltable_t *table){
    int count = symboltable_count(table);
    symbol_t *result = malloc(sizeof(symbol_t) * count);
    int i; for(i=0;i<symbol_depth;i++) {
        if ( table->entries[i].defined == 0 ) { continue; }
        memcpy(&result[i], &table->entries[i], sizeof(symbol_t));
    }
    return result;
}