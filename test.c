#include "lib/libgyb.h"
#include "lib/libgyb.c"

int main (int argc, char **argv) {
    gybfile_t reference = gyb_bytecode_new();
    gyb_bp_cmp(&reference, gybh_register_ar, gybh_register_br);
    gyb_debug_printheader(&reference.header);
    gyb_bytecode_save("test.gyb", reference);
}