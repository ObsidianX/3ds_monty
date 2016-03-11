#include <stdio.h>
#include <stdlib.h>

#include <3ds.h>

#include "py/nlr.h"
#include "py/compile.h"
#include "py/runtime.h"

#define ERR_FILE_NOT_FOUND 1
#define ERR_MEMORY_ALLOC 2
#define ERR_PARSE 3

void initConsole() {
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);
}

STATIC int execute_from_lexer(mp_lexer_t *lex, mp_parse_input_kind_t kind) {
    if (lex == NULL) {
        initConsole();
        printf("MemoryError: lexer could not allocate memory\n");
        return ERR_MEMORY_ALLOC;
    }

    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        qstr source_name = lex->source_name;

        mp_parse_tree_t parse_tree = mp_parse(lex, kind);
        mp_obj_t module_fun = mp_compile(&parse_tree, source_name, MP_EMIT_OPT_NONE, false);
        mp_call_function_0(module_fun);
        if (MP_STATE_VM(mp_pending_exception) != MP_OBJ_NULL) {
            mp_obj_t obj = MP_STATE_VM(mp_pending_exception);
            MP_STATE_VM(mp_pending_exception) = MP_OBJ_NULL;
            nlr_raise(obj);
        }

        nlr_pop();
        return 0;
    } else {
        initConsole();
        mp_obj_print_exception(&mp_plat_print, MP_OBJ_FROM_PTR(nlr.ret_val));
        return ERR_PARSE;
    }
}

STATIC int do_file(const char *file) {
    FILE *exists = fopen(file, "r");
    if (!exists) {
        initConsole();
        printf("Could not locate %s on SD\n", file);
        return ERR_FILE_NOT_FOUND;
    }
    fclose(exists);

    mp_lexer_t *lex = mp_lexer_new_from_file(file);
    return execute_from_lexer(lex, MP_PARSE_FILE_INPUT);
}

int main(int argc, char **argv) {
    mp_init();
    int res = do_file("/test.py");
    mp_deinit();

    if (res) {
        printf("\x1b[28;12H    Fatal Error: %d     ", res);
        printf("\x1b[29;12H  Press Start to exit.  ");
        while (aptMainLoop()) {
            hidScanInput();

            if (hidKeysDown() & KEY_START) {
                break;
            }

            gfxFlushBuffers();
            gfxSwapBuffers();

            gspWaitForVBlank();
        }

        gfxExit();
    }

    return 0;
}

