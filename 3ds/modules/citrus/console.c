#include <3ds.h>

#include "py/runtime.h"

#include "helpers.h"

// TODO: PrintConsole class

STATIC mp_obj_t mod_citrus_console_set_font(mp_obj_t console, mp_obj_t font) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_console_set_window(size_t n_args, const mp_obj_t *args, mp_map_t *kw) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_console_get_default(void) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_citrus_console_select(mp_obj_t console) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_citrus_console_init(mp_obj_t screen, mp_obj_t console) {
    int scr = _mod_citrus_gfx_get_gfx_screen(screen);
    // TODO:
    PrintConsole *con = NULL;

    con = consoleInit(scr, con);
    // TODO: return con

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_console_debug_init(mp_obj_t device) {
    consoleDebugInit(mp_obj_get_int(device));

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_console_clear(void) {
    consoleClear();

    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_citrus_console_set_font_obj, mod_citrus_console_set_font);
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(mod_citrus_console_set_window_obj, 5, mod_citrus_console_set_window);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_console_get_default_obj, mod_citrus_console_get_default);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_console_select_obj, mod_citrus_console_select);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_citrus_console_init_obj, mod_citrus_console_init);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_console_debug_init_obj, mod_citrus_console_debug_init);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_console_clear_obj, mod_citrus_console_clear);

STATIC const mp_rom_map_elem_t mp_module_citrus_console_globals_table[] = {
        // Module info
        {MP_ROM_QSTR(MP_QSTR___name__),      MP_ROM_QSTR(MP_QSTR_console)},

        // Functions
        {MP_ROM_QSTR(MP_QSTR_set_font),      MP_ROM_PTR(&mod_citrus_console_set_font_obj)},
        {MP_ROM_QSTR(MP_QSTR_set_window),    MP_ROM_PTR(&mod_citrus_console_set_window_obj)},
        {MP_ROM_QSTR(MP_QSTR_get_default),   MP_ROM_PTR(&mod_citrus_console_get_default_obj)},
        {MP_ROM_QSTR(MP_QSTR_select),        MP_ROM_PTR(&mod_citrus_console_select_obj)},
        {MP_ROM_QSTR(MP_QSTR_init),          MP_ROM_PTR(&mod_citrus_console_init_obj)},
        {MP_ROM_QSTR(MP_QSTR_debug_init),    MP_ROM_PTR(&mod_citrus_console_debug_init_obj)},
        {MP_ROM_QSTR(MP_QSTR_clear),         MP_ROM_PTR(&mod_citrus_console_clear_obj)},

        // formatting
        {MP_ROM_QSTR(MP_QSTR_COLOR_BOLD),    MP_ROM_INT(CONSOLE_COLOR_BOLD)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_FAINT),   MP_ROM_INT(CONSOLE_COLOR_FAINT)},
        {MP_ROM_QSTR(MP_QSTR_ITALIC),        MP_ROM_INT(CONSOLE_ITALIC)},
        {MP_ROM_QSTR(MP_QSTR_UNDERLINE),     MP_ROM_INT(CONSOLE_UNDERLINE)},
        {MP_ROM_QSTR(MP_QSTR_BLINK_SLOW),    MP_ROM_INT(CONSOLE_BLINK_SLOW)},
        {MP_ROM_QSTR(MP_QSTR_BLINK_FAST),    MP_ROM_INT(CONSOLE_BLINK_FAST)},
        {MP_ROM_QSTR(MP_QSTR_COLOR_REVERSE), MP_ROM_INT(CONSOLE_COLOR_REVERSE)},
        {MP_ROM_QSTR(MP_QSTR_CONCEAL),       MP_ROM_INT(CONSOLE_CONCEAL)},
        {MP_ROM_QSTR(MP_QSTR_CROSSED_OUT),   MP_ROM_INT(CONSOLE_CROSSED_OUT)},

        // debugDevice
        {MP_ROM_QSTR(MP_QSTR_DEBUG_NULL),    MP_ROM_INT(debugDevice_NULL)},
        {MP_ROM_QSTR(MP_QSTR_DEBUG_3DMOO),   MP_ROM_INT(debugDevice_3DMOO)},
        {MP_ROM_QSTR(MP_QSTR_DEBUG_CONSOLE), MP_ROM_INT(debugDevice_CONSOLE)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_console_globals, mp_module_citrus_console_globals_table);

const mp_obj_module_t mp_module_citrus_console = {
        .base = {&mp_type_module},
        .name = MP_QSTR_console,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_console_globals,
};
