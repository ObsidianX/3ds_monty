#include <3ds.h>

#include "py/runtime.h"

#include "helpers.h"

extern const mp_obj_type_t mod_citrus_console_PrintConsole_type;

enum {
    COLOR_BLACK = 0,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_WHITE,

    COLOR_BRIGHT_BLACK,
    COLOR_BRIGHT_RED,
    COLOR_BRIGHT_GREEN,
    COLOR_BRIGHT_YELLOW,
    COLOR_BRIGHT_BLUE,
    COLOR_BRIGHT_MAGENTA,
    COLOR_BRIGHT_CYAN,
    COLOR_BRIGHT_WHITE,

    COLOR_FAINT_BLACK,
    COLOR_FAINT_RED,
    COLOR_FAINT_GREEN,
    COLOR_FAINT_YELLOW,
    COLOR_FAINT_BLUE,
    COLOR_FAINT_MAGENTA,
    COLOR_FAINT_CYAN,
    COLOR_FAINT_WHITE,
};

STATIC mp_obj_t mod_citrus_console_get_default(void) {
    return _mod_citrus_console_PrintConsole_new_default();
}

STATIC mp_obj_t mod_citrus_console_init(mp_obj_t screen) {
    int scr = _mod_citrus_gfx_get_gfx_screen(screen);

    PrintConsole *con = consoleInit(scr, NULL);
    return _mod_citrus_console_PrintConsole_new_from_c(con);
}

STATIC mp_obj_t mod_citrus_console_debug_init(mp_obj_t device) {
    consoleDebugInit(mp_obj_get_int(device));

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_console_clear(void) {
    consoleClear();

    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_console_get_default_obj, mod_citrus_console_get_default);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_console_init_obj, mod_citrus_console_init);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_console_debug_init_obj, mod_citrus_console_debug_init);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_console_clear_obj, mod_citrus_console_clear);

STATIC const mp_rom_map_elem_t mp_module_citrus_console_globals_table[] = {
        // Module info
        {MP_ROM_QSTR(MP_QSTR___name__),             MP_ROM_QSTR(MP_QSTR_console)},

        // Classes
        {MP_ROM_QSTR(MP_QSTR_PrintConsole),         MP_ROM_PTR(&mod_citrus_console_PrintConsole_type)},

        // Functions
        {MP_ROM_QSTR(MP_QSTR_get_default),          MP_ROM_PTR(&mod_citrus_console_get_default_obj)},
        {MP_ROM_QSTR(MP_QSTR_init),                 MP_ROM_PTR(&mod_citrus_console_init_obj)},
        {MP_ROM_QSTR(MP_QSTR_debug_init),           MP_ROM_PTR(&mod_citrus_console_debug_init_obj)},
        {MP_ROM_QSTR(MP_QSTR_clear),                MP_ROM_PTR(&mod_citrus_console_clear_obj)},

        // formatting
        {MP_ROM_QSTR(MP_QSTR_FLAG_COLOR_BOLD),      MP_ROM_INT(CONSOLE_COLOR_BOLD)},
        {MP_ROM_QSTR(MP_QSTR_FLAG_COLOR_FAINT),     MP_ROM_INT(CONSOLE_COLOR_FAINT)},
        {MP_ROM_QSTR(MP_QSTR_FLAG_ITALIC),          MP_ROM_INT(CONSOLE_ITALIC)},
        {MP_ROM_QSTR(MP_QSTR_FLAG_UNDERLINE),       MP_ROM_INT(CONSOLE_UNDERLINE)},
        {MP_ROM_QSTR(MP_QSTR_FLAG_BLINK_SLOW),      MP_ROM_INT(CONSOLE_BLINK_SLOW)},
        {MP_ROM_QSTR(MP_QSTR_FLAG_BLINK_FAST),      MP_ROM_INT(CONSOLE_BLINK_FAST)},
        {MP_ROM_QSTR(MP_QSTR_FLAG_COLOR_REVERSE),   MP_ROM_INT(CONSOLE_COLOR_REVERSE)},
        {MP_ROM_QSTR(MP_QSTR_FLAG_CONCEAL),         MP_ROM_INT(CONSOLE_CONCEAL)},
        {MP_ROM_QSTR(MP_QSTR_FLAG_CROSSED_OUT),     MP_ROM_INT(CONSOLE_CROSSED_OUT)},

        // debugDevice
        {MP_ROM_QSTR(MP_QSTR_DEBUG_NULL),           MP_ROM_INT(debugDevice_NULL)},
        {MP_ROM_QSTR(MP_QSTR_DEBUG_3DMOO),          MP_ROM_INT(debugDevice_3DMOO)},
        {MP_ROM_QSTR(MP_QSTR_DEBUG_CONSOLE),        MP_ROM_INT(debugDevice_CONSOLE)},

        // colors
        {MP_ROM_QSTR(MP_QSTR_COLOR_BLACK),          MP_ROM_INT(COLOR_BLACK)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_RED),            MP_ROM_INT(COLOR_RED)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_GREEN),          MP_ROM_INT(COLOR_GREEN)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_YELLOW),         MP_ROM_INT(COLOR_YELLOW)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_BLUE),           MP_ROM_INT(COLOR_BLUE)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_MAGENTA),        MP_ROM_INT(COLOR_MAGENTA)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_CYAN),           MP_ROM_INT(COLOR_CYAN)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_WHITE),          MP_ROM_INT(COLOR_WHITE)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_BRIGHT_BLACK),   MP_ROM_INT(COLOR_BRIGHT_BLACK)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_BRIGHT_RED),     MP_ROM_INT(COLOR_BRIGHT_RED)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_BRIGHT_GREEN),   MP_ROM_INT(COLOR_BRIGHT_GREEN)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_BRIGHT_YELLOW),  MP_ROM_INT(COLOR_BRIGHT_YELLOW)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_BRIGHT_BLUE),    MP_ROM_INT(COLOR_BRIGHT_BLUE)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_BRIGHT_MAGENTA), MP_ROM_INT(COLOR_BRIGHT_MAGENTA)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_BRIGHT_CYAN),    MP_ROM_INT(COLOR_BRIGHT_CYAN)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_BRIGHT_WHITE),   MP_ROM_INT(COLOR_BRIGHT_WHITE)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_FAINT_BLACK),    MP_ROM_INT(COLOR_FAINT_BLACK)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_FAINT_RED),      MP_ROM_INT(COLOR_FAINT_RED)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_FAINT_GREEN),    MP_ROM_INT(COLOR_FAINT_GREEN)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_FAINT_YELLOW),   MP_ROM_INT(COLOR_FAINT_YELLOW)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_FAINT_BLUE),     MP_ROM_INT(COLOR_FAINT_BLUE)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_FAINT_MAGENTA),  MP_ROM_INT(COLOR_FAINT_MAGENTA)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_FAINT_CYAN),     MP_ROM_INT(COLOR_FAINT_CYAN)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_FAINT_WHITE),    MP_ROM_INT(COLOR_FAINT_WHITE)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_console_globals, mp_module_citrus_console_globals_table);

const mp_obj_module_t mp_module_citrus_console = {
        .base = {&mp_type_module},
        .name = MP_QSTR_console,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_console_globals,
};
