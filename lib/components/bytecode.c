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
}