void gyb_debug_printheader(gyb_header_t *source) {
    printf("Header:\n");
    printf("   bytemagic: %c%c%c%c\n", source->bytemagic[0], source->bytemagic[1], source->bytemagic[2], source->bytemagic[3]);
    printf("   entry:      %08x\n", source->entry);
    printf("   readonly:   %08x, size %d\n", source->readonly[0], source->readonly[1]);
    printf("   writeable:  %08x, size %d\n", source->writeable[0], source->writeable[1]);
    printf("   executable: %08x, size %d\n", source->executable[0], source->executable[1]);
    printf("   symbols:    %08x, size %d\n", source->symbols[0], source->symbols[1]);
}