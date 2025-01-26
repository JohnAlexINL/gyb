unsigned char bytecode_magic(char *buffer) {
    if ( buffer[0] != 'G' )     { return -1; }
    if ( buffer[1] != 'Y' )     { return -1; }
    if ( buffer[3] != '\0' )    { return -1; }
    if ( buffer[2] == 'O' ) { return 'O'; }
    if ( buffer[2] == 'B' ) { return 'B'; }
}