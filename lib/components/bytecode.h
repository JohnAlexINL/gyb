unsigned char gyb_bytecode_magic(char *buffer);
gybfile_t gyb_bytecode_new();
gybfile_t gyb_bytecode_load(char *filename);
int gyb_bytecode_save(char *filename, gybfile_t source);
