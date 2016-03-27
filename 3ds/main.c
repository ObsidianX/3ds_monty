#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <3ds.h>

#include "py/nlr.h"
#include "py/compile.h"
#include "py/runtime.h"

#define ERR_FILE_NOT_FOUND 1
#define ERR_MEMORY_ALLOC 2
#define ERR_PARSE 3

extern bool net_load();

extern void mod_citrus_exit_all(void);

static void initConsole() {
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

        // ensure all modules are unloaded to be a good citizen
        mod_citrus_exit_all();

        return 0;
    } else {
        // ensure all modules are unloaded to be a good citizen
        mod_citrus_exit_all();

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
    bool retry = true;

    romfsInit();

    while (retry) {
        int res = net_load();
        if (!res) {
            return 0;
        } else if (res == 2) {
            continue;
        }

        mp_init();

        mp_obj_list_init(mp_sys_path, 0);
        // ROMFS:
        mp_obj_list_append(mp_sys_path, mp_obj_new_str("romfs:", 6, true));
        mp_obj_list_append(mp_sys_path, mp_obj_new_str("romfs:/lib", 10, true));
        // SDMC:
        mp_obj_list_append(mp_sys_path, mp_obj_new_str("/", 1, true));
        mp_obj_list_append(mp_sys_path, mp_obj_new_str("/python", 7, true));

        mp_obj_list_init(mp_sys_argv, 0);
        for (int i = 0; i < argc; i++) {
            mp_obj_list_append(mp_sys_argv, mp_obj_new_str(argv[i], strlen(argv[i]), true));
        }

        res = do_file("/tmp.py");
        mp_deinit();

        if (res) {
            printf("\x1b[27;12H    Fatal Error: %d     ", res);
            printf("\x1b[28;12HPress Select to restart.");
            printf("\x1b[29;12H  Press Start to exit.  ");
            while (aptMainLoop()) {
                hidScanInput();

                int down = hidKeysDown();
                if (down & KEY_START) {
                    retry = false;
                    break;
                } else if (down & KEY_SELECT) {
                    break;
                }

                gfxFlushBuffers();
                gfxSwapBuffers();

                gspWaitForVBlank();
            }

            gfxExit();
        }
    }

    romfsExit();

    return 0;
}

