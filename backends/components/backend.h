// The specific values here do no matter as long as they are discrete
#define platform_pe_x8664       1
#define platform_pe_aarch64     2
#define platform_macho_x8664    3
#define platform_macho_aarch64  4
#define platform_elf_x8664      5
#define platform_elf_x8632      6
#define platform_elf_aarch64    7
#define platform_elf_riscv64    8
const char backend_platforms[][16] = {
    "win-x86",
    "win-aarch",
    "mac-x86",
    "mac-aarch",
    "elf-x86",
    "elf-x86_32",
    "elf-aarch",
    "elf-riscv"
};

bool backend_pe_x8664       (char *filename, gybfile_t *source);
bool backend_elf_x8664      (char *filename, gybfile_t *source);
bool backend_macho_x8664    (char *filename, gybfile_t *source);