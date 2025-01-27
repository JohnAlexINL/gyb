uint32_t symbolhash(char *name);
symboltable_t *symtable_load(symboltable_t *table, char *source, int size);
symbol_t *symtable_flatten(symboltable_t *table);
uint32_t symboltable_count(symboltable_t *table);
bool symbol_new(symboltable_t *table, char *name, unsigned char section, uint32_t offset);
bool symbol_static(symbol_t *symbols, int count);
void symtable_import(symboltable_t *parent, symboltable_t *source);
void symtable_print(symboltable_t *table);
