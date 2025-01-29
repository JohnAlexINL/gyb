uint32_t symbolhash(char *name);
gyb_symboltable_t *gyb_symtable_load(gyb_symboltable_t *table, char *source, int size);
gyb_symbol_t *gyb_symtable_flatten(gyb_symboltable_t *table);
uint32_t symboltable_count(gyb_symboltable_t *table);
bool gyb_symbol_new(gyb_symboltable_t *table, char *name, unsigned char section, uint32_t offset);
bool gyb_symbol_static(gyb_symbol_t *symbols, int count);
void gyb_symtable_import(gyb_symboltable_t *parent, gyb_symboltable_t *source);
void gyb_symtable_print(gyb_symboltable_t *table);
