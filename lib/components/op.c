void gyb_internal_pchar(gybfile_t *object, char *data, int size) {
    if (object->ebuffersize <= object->header.executable[1] + size) {
        object->executable = realloc(object->executable, object->header.executable[1] + size);
        object->ebuffersize = object->header.executable[1] + size;
    }   memcpy(object->executable + object->header.executable[1], data, size);
    object->header.executable[1] += size;
}

bool gyb_internal_pureg(gybfile_t *object, unsigned char op, register_t base) {
    if ( base > gybh_register_bp ) return false;
    char data[2] = {op, base};
    gyb_internal_pchar(object, data, 2); return true;
}

bool gyb_internal_pbreg(gybfile_t *object, unsigned char op, register_t base, register_t source){
    if ( base > gybh_register_bp || source > gybh_register_bp ) return false;
    char data[3] = {op, base, source};
    gyb_internal_pchar(object, data, 3); return true;
}

bool gyb_internal_pbsym(gybfile_t *object, unsigned char op, register_t base, gyb_symbol_t source){
    if ( base > gybh_register_bp ) { return false; } char data[6] = {op, base};
    int hash = symbolhash(source.name); memcpy(data + 2, &hash, 4);
    gyb_internal_pchar(object, data, 3); return true;
}

bool gyb_internal_pusym(gybfile_t *object, unsigned char op, gyb_symbol_t source) {
    char data[5] = {op};
    int hash = symbolhash(source.name); memcpy(data + 1, &hash, 4);
    gyb_internal_pchar(object, data, 6); return true;
}

bool gyb_bp_move  (gybfile_t *object, register_t base, register_t source)   { return gyb_internal_pbreg(object, gybh_move, base, source); }
bool gyb_bp_load  (gybfile_t *object, register_t base, gyb_symbol_t source) { return gyb_internal_pbsym(object, gybh_load, base, source); }
bool gyb_bp_store (gybfile_t *object, register_t base, gyb_symbol_t source) { return gyb_internal_pbsym(object, gybh_store, base, source); }
bool gyb_bp_imm   (gybfile_t *object, register_t base, gyb_symbol_t source) { return gyb_internal_pbsym(object, gybh_imm, base, source); }
bool gyb_bp_cmp   (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_cmp, base, adder); }
bool gyb_bp_add   (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_add, base, adder); }
bool gyb_bp_sub   (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_sub, base, adder); }
bool gyb_bp_mul   (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_mul, base, adder); }
bool gyb_bp_div   (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_div, base, adder); }
bool gyb_bp_left  (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_left, base, adder); }
bool gyb_bp_right (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_right, base, adder); }
bool gyb_bp_and   (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_and, base, adder); }
bool gyb_bp_or    (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_or, base, adder); }
bool gyb_bp_xor   (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_xor, base, adder); }
bool gyb_bp_flip  (gybfile_t *object, register_t base)                      { return gyb_internal_pureg(object, gybh_flip, base); }
bool gyb_bp_fadd  (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_fadd, base, adder); }
bool gyb_bp_fsub  (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_fsub, base, adder); }
bool gyb_bp_fmul  (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_fmul, base, adder); }
bool gyb_bp_fdiv  (gybfile_t *object, register_t base, register_t adder)    { return gyb_internal_pbreg(object, gybh_fdiv, base, adder); }
bool gyb_bp_goto  (gybfile_t *object, gyb_symbol_t symbol)                  { return gyb_internal_pusym(object, gybh_goto, symbol); }
bool gyb_bp_if    (gybfile_t *object, gyb_symbol_t symbol)                  { return gyb_internal_pusym(object, gybh_if, symbol); }
bool gyb_bp_else  (gybfile_t *object, gyb_symbol_t symbol)                  { return gyb_internal_pusym(object, gybh_else, symbol); }
bool gyb_bp_lt    (gybfile_t *object, gyb_symbol_t symbol)                  { return gyb_internal_pusym(object, gybh_lt, symbol); }
bool gyb_bp_gt    (gybfile_t *object, gyb_symbol_t symbol)                  { return gyb_internal_pusym(object, gybh_gt, symbol); }
bool gyb_bp_le    (gybfile_t *object, gyb_symbol_t symbol)                  { return gyb_internal_pusym(object, gybh_le, symbol); }
bool gyb_bp_ge    (gybfile_t *object, gyb_symbol_t symbol)                  { return gyb_internal_pusym(object, gybh_ge, symbol); }
bool gyb_bp_fmov  (gybfile_t *object, register_t base)                      { return gyb_internal_pureg(object, gybh_fmov, base); }
bool gyb_bp_fcast (gybfile_t *object, register_t base)                      { return gyb_internal_pureg(object, gybh_fcast, base); }
bool gyb_bp_imov  (gybfile_t *object, register_t base)                      { return gyb_internal_pureg(object, gybh_imov, base); }
bool gyb_bp_icast (gybfile_t *object, register_t base)                      { return gyb_internal_pureg(object, gybh_icast, base); }
bool gyb_bp_push  (gybfile_t *object, register_t base)                      { return gyb_internal_pureg(object, gybh_push, base); }
bool gyb_bp_pop   (gybfile_t *object, register_t base)                      { return gyb_internal_pureg(object, gybh_pop, base); }
bool gyb_bp_sys1  (gybfile_t *object, register_t arg0)                      { return gyb_internal_pureg(object, gybh_sys1, arg0); }
bool gyb_bp_sys2  (gybfile_t *object, register_t arg0, register_t arg1)     { return gyb_internal_pbreg(object, gybh_sys2, arg0, arg1); }

bool gyb_bp_sys3  (gybfile_t *object, register_t arg0, register_t arg1, register_t arg2) {
    if ( arg0 > gybh_register_bp || arg1 > gybh_register_bp || arg2 > gybh_register_bp ) return false;
    char data[5] = {gybh_sys3, arg0, arg1, arg2};
    gyb_internal_pchar(object, data, 5); return true;
}
bool gyb_bp_sys4  (gybfile_t *object, register_t arg0, register_t arg1, register_t arg2, register_t arg3) {
    if ( arg0 > gybh_register_bp || arg1 > gybh_register_bp || arg2 > gybh_register_bp || arg3 > gybh_register_bp ) return false;
    char data[6] = {gybh_sys4, arg0, arg1, arg2, arg3};
    gyb_internal_pchar(object, data, 6); return true;
}
