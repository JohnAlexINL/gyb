#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "gyb.h"
#include "symbol.c"
#include "share/file.c"
#include "object/make.c"
#include "linker/test.c"
#include "linker/make.c"

#define cmd_elf "elf"
#define cmd_link "link"
#define version "gyb linker, (c) 2025 Modula.dev\n"
const char help[] = version "\
    gyb <command> <dest> [sources]\n\
    converts garter relocatable objects into bytecode\n\
    or ELF objects for cross-compilation\n\
    commands:\n\
        " cmd_link "\tcombine *.gyo into one *.gyb bytecode\n\
        " cmd_elf "\tconvert a *.gyb bytecode to an *.o ELF object\n\0";

int main ( int argc , char ** argv ) {
    // catch helpstates
    if ( argc == 2 ) {
        if ( argv[1][0] == '-' ) {
            if (argv[1][1] == 'v') { printf(version); exit(0); }
            if (argv[1][1] == 'h') { printf(help); exit(0); }
        }
    }   else if ( argc < 4) { printf(help); exit(1); }

    // check commands
    if ( strncmp(cmd_link, argv[1], sizeof(cmd_link))==0) {
        bytecode_make(
            argv[2],
            argc - 3,
            (char **)(argv + 3)
        ); return 0;
    }
    if ( strncmp(cmd_elf, argv[1], sizeof(cmd_elf))==0){
        object_make(
            argv[2],
            argc - 3,
            (char **)(argv + 3)
        ); return 0;
    }

    // if nonvalid command, print help
    printf(help); exit(128);
}