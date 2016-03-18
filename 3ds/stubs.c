#include <3ds.h>

#include "py/runtime.h"
#include "py/lexer.h"

void gc_collect(void) {
}

mp_import_stat_t mp_import_stat(const char *path) {
    return MP_IMPORT_STAT_NO_EXIST;
}

void nlr_jump_fail(void *val) {
}

