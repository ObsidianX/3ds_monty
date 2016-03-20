#include <3ds.h>

#include "py/runtime.h"

#include "helpers.h"

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_citrus_console_##__n##_obj, mod_citrus_console_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_console_##__n##_obj}
#define LOCAL_INT(__n, __v) \
    {MP_ROM_QSTR(MP_QSTR_##__n), MP_ROM_INT(__v)}

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

METHOD_OBJ_N(0, get_default);
METHOD_OBJ_N(1, init);
METHOD_OBJ_N(1, debug_init);
METHOD_OBJ_N(0, clear);

STATIC const mp_rom_map_elem_t mp_module_citrus_console_globals_table[] = {
        // Module info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_console)},

        // Classes
        {MP_ROM_QSTR(MP_QSTR_PrintConsole), MP_ROM_PTR(&mod_citrus_console_PrintConsole_type)},

        // Functions
        LOCAL_METHOD(get_default),
        LOCAL_METHOD(init),
        LOCAL_METHOD(debug_init),
        LOCAL_METHOD(clear),

        // formatting
        LOCAL_INT(FLAG_COLOR_BOLD, CONSOLE_COLOR_BOLD),
        LOCAL_INT(FLAG_COLOR_FAINT, CONSOLE_COLOR_FAINT),
        LOCAL_INT(FLAG_ITALIC, CONSOLE_ITALIC),
        LOCAL_INT(FLAG_UNDERLINE, CONSOLE_UNDERLINE),
        LOCAL_INT(FLAG_BLINK_SLOW, CONSOLE_BLINK_SLOW),
        LOCAL_INT(FLAG_BLINK_FAST, CONSOLE_BLINK_FAST),
        LOCAL_INT(FLAG_COLOR_REVERSE, CONSOLE_COLOR_REVERSE),
        LOCAL_INT(FLAG_CONCEAL, CONSOLE_CONCEAL),
        LOCAL_INT(FLAG_CROSSED_OUT, CONSOLE_CROSSED_OUT),

        // debugDevice
        LOCAL_INT(DEBUG_NULL, debugDevice_NULL),
        LOCAL_INT(DEBUG_3DMOO, debugDevice_3DMOO),
        LOCAL_INT(DEBUG_CONSOLE, debugDevice_CONSOLE),

        // colors
        LOCAL_INT(COLOR_BLACK, COLOR_BLACK),
        LOCAL_INT(COLOR_RED, COLOR_RED),
        LOCAL_INT(COLOR_GREEN, COLOR_GREEN),
        LOCAL_INT(COLOR_YELLOW, COLOR_YELLOW),
        LOCAL_INT(COLOR_BLUE, COLOR_BLUE),
        LOCAL_INT(COLOR_MAGENTA, COLOR_MAGENTA),
        LOCAL_INT(COLOR_CYAN, COLOR_CYAN),
        LOCAL_INT(COLOR_WHITE, COLOR_WHITE),
        LOCAL_INT(COLOR_BRIGHT_BLACK, COLOR_BRIGHT_BLACK),
        LOCAL_INT(COLOR_BRIGHT_RED, COLOR_BRIGHT_RED),
        LOCAL_INT(COLOR_BRIGHT_GREEN, COLOR_BRIGHT_GREEN),
        LOCAL_INT(COLOR_BRIGHT_YELLOW, COLOR_BRIGHT_YELLOW),
        LOCAL_INT(COLOR_BRIGHT_BLUE, COLOR_BRIGHT_BLUE),
        LOCAL_INT(COLOR_BRIGHT_MAGENTA, COLOR_BRIGHT_MAGENTA),
        LOCAL_INT(COLOR_BRIGHT_CYAN, COLOR_BRIGHT_CYAN),
        LOCAL_INT(COLOR_BRIGHT_WHITE, COLOR_BRIGHT_WHITE),
        LOCAL_INT(COLOR_FAINT_BLACK, COLOR_FAINT_BLACK),
        LOCAL_INT(COLOR_FAINT_RED, COLOR_FAINT_RED),
        LOCAL_INT(COLOR_FAINT_GREEN, COLOR_FAINT_GREEN),
        LOCAL_INT(COLOR_FAINT_YELLOW, COLOR_FAINT_YELLOW),
        LOCAL_INT(COLOR_FAINT_BLUE, COLOR_FAINT_BLUE),
        LOCAL_INT(COLOR_FAINT_MAGENTA, COLOR_FAINT_MAGENTA),
        LOCAL_INT(COLOR_FAINT_CYAN, COLOR_FAINT_CYAN),
        LOCAL_INT(COLOR_FAINT_WHITE, COLOR_FAINT_WHITE),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_console_globals, mp_module_citrus_console_globals_table);

const mp_obj_module_t mp_module_citrus_console = {
        .base = {&mp_type_module},
        .name = MP_QSTR_console,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_console_globals,
};
