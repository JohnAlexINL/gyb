uint32_t symbolhash(char *name){
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
}