// EXTERNAL DEPENDENCIES
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/stat.h>
// INTERNAL
#include "license.h"
#include "strings.h"
#include "lib/libgyb.h"
#include "lib/libgyb.c"
#include "linker/make.c"
#include "object/make.c"
#include "backends/backend.h"
#include "backends/backend.c"

int main ( int argc , char ** argv ) {
    // catch helpstates
    if ( argc == 2 ) {
        if ( argv[1][0] == '-' ) {
            if (argv[1][1] == 'v') { printf(version); exit(0); }
            if (argv[1][1] == 'h') { printf(help); exit(0); }
            if (argv[1][1] == 'l') { printLicense(); exit(0); }
        }
        else if ( strncmp("license", argv[1], 7)==0) { printLicense(); exit(0); }
        else if ( strncmp("help", argv[1], 4)==0) { printf(help); exit(0); }
        else if ( strncmp("version", argv[1], 7)==0) { printf(version); exit(0); }
        else { printf(badhelp); exit(128); }
    }
    // check no args here before we check argv out of bounds
    if ( argc == 1 ) { printf(badhelp); exit(128); }
    // check commands
    if ( strncmp(cmd_link, argv[1], sizeof(cmd_link))==0) {
        if ( argc < 4) { printf(badargs, cmd_link); exit(2); }
        bytecode_make(
            argv[2],
            argc - 3,
            (char **)(argv + 3)
        ); return 0;
    }
    if ( strncmp(cmd_elf, argv[1], sizeof(cmd_elf))==0){
        if ( argc != 4 ) { printf(badargs, cmd_elf); exit(2); }
        object_make(
            argv[2],
            argc - 3,
            (char **)(argv + 3)
        ); return 0;
    }
    if ( strncmp(cmd_make, argv[1], sizeof(cmd_make))==0 ){
        if ( argc != 5 ) { printf(badargs, cmd_make); exit(2); }
        unsigned char platform = getPlatform(argv[2]); if ( platform == 0 ) { printf(badplatform, cmd_make); exit(3); }
        backend(platform, argv[3], argv[4]);
        return 0;
    }
    // if an elsewise invalid command
    printf(badhelp); exit(128);
}