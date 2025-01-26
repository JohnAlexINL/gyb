void object_make(char *output, int srcn, char **srcs){
    // make sure that we only have one source for ELF creation
    if (srcn != 1) { printf("INVALID OBJECT MAKE GIVEN %d SRC FILES\n", srcn); exit(1); }
    printf("OBJECT MAKE: %s\nINPUTS:\n", output);
    int i; for (i=0;i<srcn;i++) {
        printf("    %s\n", srcs[i]);
    }
}