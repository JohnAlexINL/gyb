unsigned char bytecode_magic(char *buffer);
gybfile_t bytecode_new();
gybfile_t bytecode_load(char *filename);
int bytecode_save(char *filename, gybfile_t source);
