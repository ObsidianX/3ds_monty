#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdarg.h>

#include <3ds.h>

#include "py/runtime.h"
#include "py/lexer.h"

void gc_collect(void) {
}

mp_import_stat_t mp_import_stat(const char *path) {
    FILE *f = fopen(path, "r");
    if (f != NULL) {
        fclose(f);
        return MP_IMPORT_STAT_FILE;
    }
    return MP_IMPORT_STAT_NO_EXIST;
}

void nlr_jump_fail(void *val) {
}

void plat_print_3ds(const char *msg, size_t len) {
    char *out = malloc(len + 1);
    snprintf(out, len + 1, msg);
    printf(out);
    free(out);
}

int DEBUG_printf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    printf(fmt, ap);
    va_end(ap);
    return 0;
}