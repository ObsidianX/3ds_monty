#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include <3ds.h>
#include <zipfs.h>

#include "py/nlr.h"
#include "py/compile.h"
#include "py/runtime.h"
#include "py/repl.h"

#include "config.h"
#include "basic_console.h"
#include "netrepl.h"

#define ERR_FILE_NOT_FOUND  1
#define ERR_MEMORY_ALLOC    2
#define ERR_PARSE           3
#define ERR_SYS_EXIT        4
#define ERR_NETLOAD         5

#define TYPE_NONE   0
#define TYPE_PY     1
#define TYPE_ZIP    2

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
    if (restart) {
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
    } else {
        if (mp_obj_exception_match(nlr.ret_val, &mp_type_SystemExit)) {
            return ERR_SYS_EXIT;
        }

        if (mp_obj_is_exception_type(nlr.ret_val) && ((mp_obj_exception_t *) nlr.ret_val)->args != NULL && ((mp_obj_exception_t *) nlr.ret_val)->args->len == 1) {
            int code = mp_obj_get_int(((mp_obj_exception_t *) nlr.ret_val)->args->items[0]);
            if (code == 0xDEAD0000) {
                return ERR_NETLOAD;
            }
        }

        init_console();
        mp_obj_print_exception(&mp_plat_print, MP_OBJ_FROM_PTR(nlr.ret_val));
        return ERR_PARSE;
    }
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

STATIC int do_file(const char *file) {
    struct stat st;
    stat(file, &st);
    if (!S_ISREG(st.st_mode)) {
        init_console();
        printf("Could not locate \"%s\"\n", file);
        return ERR_FILE_NOT_FOUND;
    }

    mp_lexer_t *lex = mp_lexer_new_from_file(file);
    return execute_from_lexer(lex, MP_PARSE_FILE_INPUT, false);
}

STATIC int run_file(const char *filename, int argc, char **argv, bool return_result) {
    mp_init();
    setup_sys(argc, argv);

    int ret = do_file(filename);
    if (!return_result && ret && ret != ERR_SYS_EXIT) {
        fatal_error(false);
    }

    mp_deinit();

    mod_citrus_exit_all();

    return ret;
}

STATIC int boot_file(const char *device, const char *boot_from, int argc, char **argv, bool return_result) {
    char boot_from_path[PATH_MAX];
    char filename[PATH_MAX];
    char boot_filename[PATH_MAX];

    sprintf(boot_from_path, "%s%s", device, boot_from);
    int init_fd = open(boot_from, O_RDONLY);

    int count = read(init_fd, boot_filename, sizeof(filename));

    if (count <= 0) {
        init_console();
        printf("Couldn't read '%s'\nerrno: %d", boot_from_path, errno);
        close(init_fd);
        fatal_error(false);
        return ERR_FILE_NOT_FOUND;
    }

    close(init_fd);

    char *newline = strchr(boot_filename, '\n');
    if (newline) {
        *newline = '\0';
    } else {
        newline = strchr(boot_filename, '\r');
        if (newline) {
            *newline = '\0';
        }
    }

    snprintf(filename, PATH_MAX, "%s%s", device, boot_filename);

    return run_file(filename, argc, argv, return_result);
}

STATIC void main_netload(int argc, char **argv) {
    bool restart = true;

    while (restart) {
        int ret = run_file("romfs:/netload.py", argc, argv, true);

        if (ret == ERR_PARSE) {
            restart = fatal_error(false);
        } else if (ret == ERR_NETLOAD) {
            restart = false;
        } else {
            mp_obj_dict_t *globals = mp_globals_get();

            qstr filename_qstr = qstr_from_str("monty_filename");
            qstr filetype_qstr = qstr_from_str("monty_filetype");
            qstr cancel_qstr = qstr_from_str("monty_cancel");

            mp_obj_t monty_filename = MP_OBJ_NEW_QSTR(filename_qstr);
            mp_obj_t monty_filetype = MP_OBJ_NEW_QSTR(filetype_qstr);
            mp_obj_t monty_cancel = MP_OBJ_NEW_QSTR(cancel_qstr);

            mp_map_elem_t *filename_item = mp_map_lookup(&globals->map, monty_filename, MP_MAP_LOOKUP);
            mp_map_elem_t *filetype_item = mp_map_lookup(&globals->map, monty_filetype, MP_MAP_LOOKUP);
            mp_map_elem_t *cancel_item = mp_map_lookup(&globals->map, monty_cancel, MP_MAP_LOOKUP);

            if (cancel_item != NULL && mp_obj_is_true(cancel_item->value)) {
                restart = false;
                break;
            }

            const char *filename = "";
            if (filename_item != NULL) {
                filename = mp_obj_str_get_str(filename_item->value);
            }
            char file_path[PATH_MAX];
            sprintf(file_path, "sdmc:/monty3ds/%s", filename);

            int type = TYPE_NONE;
            if (filetype_item != NULL) {
                type = mp_obj_get_int(filetype_item->value);
            }

            switch (type) {
                case TYPE_PY: {
                    ret = run_file(file_path, argc, argv, true);
                    break;
                }
                case TYPE_ZIP: {
                    zipfs_handle_t handle = zipfs_mount(file_path, "zip");
                    zipfs_set_default(handle);

                    ret = boot_file("zip:/", "boot", argc, argv, true);

                    zipfs_unmount(handle);

                    break;
                }
                default:
                    continue;
            }

            if (ret) {
                restart = fatal_error(true);
            }
        }
    }

    mod_citrus_exit_all();
}

void main_repl(int argc, char **argv) {
    if (!net_repl_connect()) {

    }

    net_repl_packet_t packet;
    char *line;

    mp_init();
    setup_sys(argc, argv);

    for (; ;) {
        // non-blocking
        net_repl_recv(&packet);
        switch (packet.type) {
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
            } else if (packet.line_len == 0) {
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

void main_romfs(int argc, char **argv) {
    boot_file("romfs:/", "boot", argc, argv, false);
}

void main_args(int argc, char **argv) {
    if (argc > 1) {
        run_file(argv[1], argc, argv, false);
    }
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

