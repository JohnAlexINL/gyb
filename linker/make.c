void bytecode_symboloffset(symbol_t *symbols, int count, unsigned int ro, unsigned int rw, unsigned int x) {
    int i; for (i=0;i<count;i++) {
        unsigned char section = symbols[i].section;
        switch(section) {
            case section_readonly: symbols[i].offset += ro;
            case section_readwrite: symbols[i].offset += rw;
            case section_executable: symbols[i].offset += x;
            default: printf("Invalid section code %c for symbol \"%s\"\n", section, symbols[i].name); exit(2);
        }
    }
}

void bytecode_make(char *output, int srcn, char **srcs){
    symboltable_t symtable;
    char *buffer;
    int i; for(i=0;i<srcn;i++) {
        // Load the object file into memory
        if ( file_exists(srcs[i]) == false ) {
            printf("file does not exist \"%s\"\n", srcs[i]); exit(2); }
        int buffersize = file_size(srcs[i]);
        buffer = malloc(sizeof(char)*buffersize);
        file_read(srcs[i], buffer, buffersize);
        // Check that the object is a relocatable type
        unsigned char type = bytecode_magic(buffer);
        if (type != 'O') {
            printf("file \"%s\" is not reloctable garter bytecode\n", srcs[i]); exit(2); }
    }
}