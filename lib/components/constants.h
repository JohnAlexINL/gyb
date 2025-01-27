#define symboltype_undefined    '\0'    // self-explanatory
#define symboltype_constant     '\1'    // replaced where is, not a ptr
#define symboltype_external     '@'     // expect definition elsewhere
#define symboltype_readable     'R'     // ptr to somewhere in readable section
#define symboltype_writeable    'W'     // ptr to somewhere in writable section
#define symboltype_executable   'E'     // ptr to somewhere in executable section
#define bytecode_buffer_default 1024
#define section_readonly 'R'
#define section_readwrite 'W'
#define section_executable 'E'
#define symbol_depth 1024
#define symbol_namelen 64 - (sizeof(unsigned char)*2) - sizeof(uint32_t) - 4
