unsigned char getPlatform(char *name) {
    int i; for(i=0;i<sizeof(backend_platforms);i++) {
        if ( strncmp(name, backend_platforms[i], 16)==0 ) { return i+1; }
    }   return 0;
}

void backend(unsigned char platform, char *output, char *source) {
    gybfile_t gybfile = gyb_bytecode_load(source);
    switch(platform) {
        case platform_elf_x8664:    backend_elf_x8664(output, &gybfile); break;
        case platform_pe_x8664:     backend_pe_x8664(output, &gybfile); break;
        case platform_macho_x8664:  backend_macho_x8664(output, &gybfile); break;
        // Pending
        case platform_macho_aarch64:
        case platform_elf_x8632:
        case platform_elf_aarch64:
        case platform_elf_riscv64:
            break;
        // Unsupported
        case platform_pe_aarch64:
            break;
    }
}