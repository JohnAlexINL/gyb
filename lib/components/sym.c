uint32_t symbolhash(char *name){
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