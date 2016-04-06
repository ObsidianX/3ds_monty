#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <3ds.h>

#include "py/nlr.h"
#include "py/compile.h"
#include "py/runtime.h"
#include "py/repl.h"

#include "config.h"
#include "basic_console.h"
#include "netload.h"
#include "netrepl.h"

#define ERR_FILE_NOT_FOUND 1
#define ERR_MEMORY_ALLOC 2
#define ERR_PARSE 3
#define ERR_SYS_EXIT 4

static const char *sys_paths[] = {
        "romfs:",
        "romfs:/lib",
        "/",
        "/python",
};

static const int sys_path_count = sizeof(sys_paths) / sizeof(sys_paths[0]);

extern void mod_citrus_exit_all(void);

STATIC bool fatal_error(bool restart) {
    bool should_restart = false;
    if(restart) {
        printf("\x1b[28;12HPress Select to restart.");
    }
    printf("\x1b[29;12H  Press Start to exit.  ");

    while (aptMainLoop()) {
        hidScanInput();

        int down = hidKeysDown();
        if (down & KEY_START) {
            break;
        } else if (restart && (down & KEY_SELECT)) {
            should_restart = true;
            break;
        }

        gfxFlushBuffers();
        gfxSwapBuffers();

        gspWaitForVBlank();
    }

    gfxExit();

    return should_restart;
}

STATIC int execute_from_lexer(mp_lexer_t *lex, mp_parse_input_kind_t kind, bool repl) {
    if (lex == NULL) {
        init_console();
        printf("MemoryError: lexer could not allocate memory\n");
        return ERR_MEMORY_ALLOC;
    }

    nlr_buf_t nlr;
    if (nlr_push(&nlr) == 0) {
        qstr source_name = lex->source_name;

        if (kind == MP_PARSE_FILE_INPUT) {
            mp_store_global(MP_QSTR___file__, MP_OBJ_NEW_QSTR(source_name));
        }

        mp_parse_tree_t parse_tree = mp_parse(lex, kind);
        mp_obj_t module_fun = mp_compile(&parse_tree, source_name, MP_EMIT_OPT_NONE, repl);
        mp_call_function_0(module_fun);
        if (MP_STATE_VM(mp_pending_exception) != MP_OBJ_NULL) {
            mp_obj_t obj = MP_STATE_VM(mp_pending_exception);
            MP_STATE_VM(mp_pending_exception) = MP_OBJ_NULL;
            nlr_raise(obj);
        }

        nlr_pop();
        return 0;
    } else if(MP_OBJ_IS_TYPE(nlr.ret_val, &mp_type_SystemExit)) {
        return ERR_SYS_EXIT;
    } else {
        init_console();
        mp_obj_print_exception(&mp_plat_print, MP_OBJ_FROM_PTR(nlr.ret_val));
        return ERR_PARSE;
    }
}

STATIC int do_file(const char *file) {
    FILE *exists = fopen(file, "r");
    if (!exists) {
        init_console();
        printf("Could not locate \"%s\"\n", file);
        return ERR_FILE_NOT_FOUND;
    }
    fclose(exists);

    mp_lexer_t *lex = mp_lexer_new_from_file(file);
    return execute_from_lexer(lex, MP_PARSE_FILE_INPUT, false);
}

STATIC void setup_sys(int argc, char **argv) {
    mp_obj_list_init(mp_sys_path, 0);
    for (int i = 0; i < sys_path_count; i++) {
        mp_obj_list_append(mp_sys_path, mp_obj_new_str(sys_paths[i], (mp_uint_t) strlen(sys_paths[i]), true));
    }

    mp_obj_list_init(mp_sys_argv, 0);
    for (int i = 0; i < argc; i++) {
        mp_obj_list_append(mp_sys_argv, mp_obj_new_str(argv[i], strlen(argv[i]), true));
    }
}

void main_netload(int argc, char **argv) {
    mp_init();
    setup_sys(argc, argv);
    int ret = do_file("romfs:/netload.py");

    if(ret) {
        fatal_error(false);
    }

    mp_deinit();

    mod_citrus_exit_all();
    return;

    bool retry = true;
    while (retry) {
        mod_citrus_exit_all();
        int res = net_load();
        if (!res) {
            return;
        } else if (res == 2) {
            continue;
        }

        mp_init();
        setup_sys(argc, argv);

        res = do_file("/monty_netload.py");
        unlink("/monty_netload.py");

        mp_deinit();

        if (res) {
            retry = fatal_error(true);
        }

        // ensure all services are exited to be a good citizen
        mod_citrus_exit_all();
    }
}

void main_repl(int argc, char **argv) {
    if(!net_repl_connect()) {

    }

    net_repl_packet_t packet;
    char *line;

    mp_init();
    setup_sys(argc, argv);

    for (;;) {
        // non-blocking
        net_repl_recv(&packet);
        switch(packet.type) {
            case REPL_QUIT:
                return;
            case REPL_WAIT:
                continue;
            default:
                break;
        }
        if (packet.type == REPL_QUIT) {
            return;
        }

        line = strdup(packet.line);

        while (mp_repl_continue_with_input(line)) {
            net_repl_send(REPL_CONTINUE, NULL, false);
            net_repl_recv(&packet);

            if (packet.type == REPL_QUIT) {
                free(line);
                return;
            } else if(packet.line_len == 0) {
                break;
            }

            char *joined = strjoin(line, '\n', packet.line);
            free(line);

            line = joined;
        }

        mp_lexer_t *lex = mp_lexer_new_from_str_len(MP_QSTR__lt_stdin_gt_, line, strlen(line), false);
        int ret = execute_from_lexer(lex, MP_PARSE_SINGLE_INPUT, true);

        free(line);
        if (ret == ERR_SYS_EXIT) {
            return;
        }
    }
}

void run_file(const char *filename, int argc, char **argv) {
    mp_init();
    setup_sys(argc, argv);

    int ret = do_file(filename);
    if(ret) {
        fatal_error(false);
    }

    mp_deinit();
}

void main_romfs(int argc, char **argv) {
    char filename[FILENAME_MAX];
    char boot_filename[FILENAME_MAX];

    FILE *init = fopen("romfs:/boot", "r");

    fread(boot_filename, sizeof(filename), 1, init);

    char *newline = strchr(boot_filename, '\n');
    if(newline) {
        *newline = '\0';
    } else {
        newline = strchr(boot_filename, '\r');
        if(newline) {
            *newline = '\0';
        }
    }

    snprintf(filename, FILENAME_MAX, "romfs:/%s", boot_filename);

    if(ferror(init)) {
        init_console();
        printf("Couldn't read 'romfs:/boot'\nerrno: %d", errno);
        fatal_error(false);
        return;
    }
    fclose(init);

    run_file(filename, argc, argv);
}

void main_args(int argc, char **argv) {
    run_file(argv[1], argc, argv);
}

int main(int argc, char **argv) {
    romfsInit();

#if _3DS_MODE == _3DS_MODE_REPL
    main_repl(argc, argv);
#elif _3DS_MODE == _3DS_MODE_NETLOAD
    main_netload(argc, argv);
#elif _3DS_MODE == _3DS_MODE_ROMFS
    main_romfs(argc, argv);
#elif _3DS_MODE == _3DS_MODE_ARGS
    main_args(argc, argv);
#endif

    romfsExit();

    // ensure all services are exited to be a good citizen
    mod_citrus_exit_all();

    return 0;
}

