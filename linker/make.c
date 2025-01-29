void bytecode_symboloffset(gyb_symboltable_t *table, unsigned int ro, unsigned int rw, unsigned int x) {
    int i; for (i=0;i<gyb_symbol_depth;i++) {
        unsigned char section = table->entries[i].section;
        switch(section) {
            case section_readonly: table->entries[i].offset += ro; break;
            case section_readwrite: table->entries[i].offset += rw; break;
            case section_executable: table->entries[i].offset += x; break;
            default: break; // external, constant, or undefined; do nothing
        }
    }
}

void bytecode_buffsizeup(gybfile_t parent, unsigned char type, int size, char *buffer) {
    int dif; int newsize; int oldsize;
    switch(type) {
        case symboltype_readable: oldsize = parent.header.readonly[1]; break;
        case symboltype_writeable: oldsize = parent.header.writeable[1]; break;
        case symboltype_executable: oldsize = parent.header.executable[1]; break;
    }
    if ( size <= oldsize ) {
        // Round up to the nearest kilobyte
        dif = size - oldsize;
        newsize = oldsize + (dif / 1024 + 1) * 1024;
    }
    switch(type) {
        case symboltype_readable:
            parent.header.readonly[1] = size;
            parent.readonly = realloc(buffer, newsize);
            break;
        case symboltype_writeable:
            parent.header.writeable[1] = size;
            parent.writeable = realloc(buffer, newsize);
            break;
        case symboltype_executable:
            parent.header.executable[1] = size;
            parent.executable = realloc(buffer, newsize);
            break;
        default:
            printf("invalid section type %c\n", type); exit(2);
    }
}

bool bytecode_make(char *output, int srcn, char **srcs){
    gybfile_t make = gyb_bytecode_new();
    // these get updated as we read in each symbol table
    int roffset = 0; int woffset = 0; int xoffset = 0;
    // Load in all of the bytecode files
    int i; for(i=0;i<srcn;i++) {
        // Load in the bytecode
        gybfile_t local = gyb_bytecode_load(srcs[i]);
        // Offset its symbol table
        bytecode_symboloffset(&local.symtable, roffset, woffset, xoffset);
        // Then actually import its symbol table
        gyb_symtable_import(&make.symtable, &local.symtable);
        // Make sure the sections have enough space for the new sections
        bytecode_buffsizeup(make, section_readonly, roffset+local.header.readonly[1], make.readonly);
        bytecode_buffsizeup(make, section_readwrite, woffset+local.header.writeable[1], make.writeable);
        bytecode_buffsizeup(make, section_executable, xoffset+local.header.executable[1], make.executable);
        // Then clone in the section data
        memcpy(make.readonly+roffset, local.readonly, local.header.readonly[1]);
        memcpy(make.writeable+woffset, local.writeable, local.header.writeable[1]);
        memcpy(make.executable+xoffset, local.executable, local.header.executable[1]);
        // Update the offsets
        roffset += local.header.readonly[1];
        woffset += local.header.writeable[1];
        xoffset += local.header.executable[1];
    }
    // Flatten the symbol table
    int symbolcount = symboltable_count(&make.symtable);
    gyb_symbol_t *local_symbols = gyb_symtable_flatten(&make.symtable);
    // Update the header to include our offsets and symboltable size
    make.header.symbols[1] = symbolcount * sizeof(gyb_symbol_t);
    make.header.readonly[1] = roffset;
    make.header.writeable[1] = woffset;
    make.header.executable[1] = xoffset;
    // Set the typemagic
    if ( gyb_symbol_static(local_symbols, symbolcount) == true )
    { make.header.bytemagic[2] = 'B'; } else
    { make.header.bytemagic[2] = 'O'; }
    // Once the object is ready, have it commit to file
    return gyb_bytecode_save(output, make);
}