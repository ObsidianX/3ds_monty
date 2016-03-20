#include <string.h>

#include <3ds.h>

#include "py/runtime.h"

#include "../helpers.h"

#define IGNORE_DEFAULT(dest, src, def) (dest = src == def ? dest : src)
#define SELF(src) mod_citrus_console_PrintConsole_t *self = src;

#define ATTR_METHOD(__n) \
    if(!strcmp(name, qstr_str(MP_QSTR_##__n))) { \
        dest[0] = (mp_obj_t) &mod_citrus_console_PrintConsole_##__n##_obj; \
    }
#define ATTR_FIELD(__n, __f) \
    if (!strcmp(name, qstr_str(MP_QSTR_##__n))) { \
        dest[0] = mp_obj_new_int(self->console.__f); \
    }

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_citrus_console_PrintConsole_##__n##_obj, mod_citrus_console_PrintConsole_##__n)
#define METHOD_OBJ_KW(__min, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_KW(mod_citrus_console_PrintConsole_##__n##_obj, __min, mod_citrus_console_PrintConsole_##__n)
#define METHOD_OBJ_VAR_N(__num, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_citrus_console_PrintConsole_##__n##_obj, __num, __num, mod_citrus_console_PrintConsole_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_console_PrintConsole_##__n##_obj}

const mp_obj_type_t mod_citrus_console_PrintConsole_type;

typedef struct {
    mp_obj_base_t base;

    PrintConsole console;
} mod_citrus_console_PrintConsole_t;

mp_obj_t _mod_citrus_console_PrintConsole_new_default(void) {
    PrintConsole *con = consoleGetDefault();
    return _mod_citrus_console_PrintConsole_new_from_c(con);
}

mp_obj_t _mod_citrus_console_PrintConsole_new_from_c(PrintConsole *con) {
    mod_citrus_console_PrintConsole_t *obj = m_new_obj(mod_citrus_console_PrintConsole_t);
    obj->base.type = (mp_obj_t) &mod_citrus_console_PrintConsole_type;

    obj->console = *con;

    return obj;
}

enum {
    NEW_ARG_SCREEN = 0,
    NEW_ARG_WINDOW_X,
    NEW_ARG_WINDOW_Y,
    NEW_ARG_WINDOW_WIDTH,
    NEW_ARG_WINDOW_HEIGHT,
    NEW_ARG_TAB_SIZE,
    NEW_ARG_COUNT
};

STATIC mp_obj_t mod_citrus_console_PrintConsole_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in) {
    mp_arg_check_num(n_args, n_kw, 1, NEW_ARG_COUNT, true);

    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_screen,        MP_ARG_INT | MP_ARG_REQUIRED, {.u_int = GFX_TOP}},
            {MP_QSTR_window_x,      MP_ARG_KW_ONLY | MP_ARG_INT,  {.u_int = -1}},
            {MP_QSTR_window_y,      MP_ARG_KW_ONLY | MP_ARG_INT,  {.u_int = -1}},
            {MP_QSTR_window_width,  MP_ARG_KW_ONLY | MP_ARG_INT,  {.u_int = -1}},
            {MP_QSTR_window_height, MP_ARG_KW_ONLY | MP_ARG_INT,  {.u_int = -1}},
            {MP_QSTR_tab_size,      MP_ARG_KW_ONLY | MP_ARG_INT,  {.u_int = -1}},
    };

    mp_arg_val_t args[NEW_ARG_COUNT];
    mp_arg_parse_all_kw_array(n_args, n_kw, args_in, NEW_ARG_COUNT, allowed_args, args);

    mod_citrus_console_PrintConsole_t *obj = m_new_obj(mod_citrus_console_PrintConsole_t);
    obj->base.type = (mp_obj_t) &mod_citrus_console_PrintConsole_type;

    int _screen = _mod_citrus_gfx_is_gfx_screen(args[NEW_ARG_SCREEN].u_int);
    consoleInit(_screen, &obj->console);

    IGNORE_DEFAULT(obj->console.windowX, args[NEW_ARG_WINDOW_X].u_int, -1);
    IGNORE_DEFAULT(obj->console.windowY, args[NEW_ARG_WINDOW_Y].u_int, -1);
    IGNORE_DEFAULT(obj->console.windowWidth, args[NEW_ARG_WINDOW_WIDTH].u_int, -1);
    IGNORE_DEFAULT(obj->console.windowHeight, args[NEW_ARG_WINDOW_WIDTH].u_int, -1);
    IGNORE_DEFAULT(obj->console.tabSize, args[NEW_ARG_TAB_SIZE].u_int, -1);

    return obj;
}

STATIC void mod_citrus_console_PrintConsole_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    SELF(self_in);

    mp_printf(print, "<PrintConsole cursor=(%d,%d) console=(%d,%d), window=(%d,%d)@(%d,%d)>",
              self->console.cursorX, self->console.cursorY,
              self->console.consoleWidth, self->console.consoleHeight,
              self->console.windowWidth, self->console.windowHeight,
              self->console.windowX, self->console.windowY);
}

STATIC mp_obj_t mod_citrus_console_PrintConsole_select(mp_obj_t self_in) {
    SELF(self_in);

    consoleSelect(&self->console);

    return mp_const_none;
}

enum {
    SET_COLOR_ARG_SELF = 0,
    SET_COLOR_ARG_FG,
    SET_COLOR_ARG_BG,
    SET_COLOR_ARG_COUNT
};

STATIC mp_obj_t mod_citrus_console_PrintConsole_set_color(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_self, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_fg,   MP_ARG_KW_ONLY | MP_ARG_INT,  {.u_int = 7 /*white*/}},
            {MP_QSTR_bg,   MP_ARG_KW_ONLY | MP_ARG_INT,  {.u_int = 0 /*black*/}},
    };

    mp_arg_val_t args[SET_COLOR_ARG_COUNT];
    mp_arg_parse_all((mp_uint_t) n_args, pos_args, kw_args, SET_COLOR_ARG_COUNT, allowed_args, args);

    SELF(args[SET_COLOR_ARG_SELF].u_obj);

    self->console.fg = args[SET_COLOR_ARG_FG].u_int;
    self->console.bg = args[SET_COLOR_ARG_BG].u_int;

    return mp_const_none;
}

enum {
    SET_POS_ARG_SELF = 0,
    SET_POS_ARG_X,
    SET_POS_ARG_Y,
    SET_POS_ARG_COUNT
};

STATIC mp_obj_t mod_citrus_console_PrintConsole_set_position(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_self, MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_x,    MP_ARG_INT,                   {.u_int = 0}},
            {MP_QSTR_y,    MP_ARG_INT,                   {.u_int = 0}},
    };

    mp_arg_val_t args[SET_POS_ARG_COUNT];
    mp_arg_parse_all((mp_uint_t) n_args, pos_args, kw_args, SET_POS_ARG_COUNT, allowed_args, args);

    SELF(args[SET_POS_ARG_SELF].u_obj);

    self->console.cursorX = args[SET_POS_ARG_X].u_int;
    self->console.cursorY = args[SET_POS_ARG_Y].u_int;

    return mp_const_none;
}

enum {
    SET_WIN_ARG_SELF = 0,
    SET_WIN_ARG_X,
    SET_WIN_ARG_Y,
    SET_WIN_ARG_WIDTH,
    SET_WIN_ARG_HEIGHT
};

STATIC mp_obj_t mod_citrus_console_PrintConsole_set_window(size_t n_args, const mp_obj_t *args) {
    SELF(args[SET_WIN_ARG_SELF]);

    int x = mp_obj_get_int(args[SET_WIN_ARG_X]);
    int y = mp_obj_get_int(args[SET_WIN_ARG_Y]);
    int width = mp_obj_get_int(args[SET_WIN_ARG_WIDTH]);
    int height = mp_obj_get_int(args[SET_WIN_ARG_HEIGHT]);

    consoleSetWindow(&self->console, x, y, width, height);

    return mp_const_none;
}

METHOD_OBJ_N(1, select);
METHOD_OBJ_KW(0, set_color);
METHOD_OBJ_KW(0, set_position);
METHOD_OBJ_VAR_N(5, set_window);

STATIC void mod_citrus_console_PrintConsole_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    SELF(self_in);
    const char *name = qstr_str(attr);
    bool load = dest[0] == MP_OBJ_NULL;

    // attributes are read-only
    if (!load) {
        return;
    }

    // Methods
    ATTR_METHOD(select)
    else ATTR_METHOD(set_color)
    else ATTR_METHOD(set_position)
    else ATTR_METHOD(set_window)

    if (dest[0] != MP_OBJ_NULL) {
        dest[1] = self_in;
        return;
    }

    // Variables
    ATTR_FIELD(cursor_x, cursorX)
    else ATTR_FIELD(cursor_y, cursorY)
    else ATTR_FIELD(width, consoleWidth)
    else ATTR_FIELD(height, consoleHeight)
    else ATTR_FIELD(window_x, windowX)
    else ATTR_FIELD(window_y, windowY)
    else ATTR_FIELD(window_width, windowWidth)
    else ATTR_FIELD(window_height, windowHeight)
    else ATTR_FIELD(fg, fg)
    else ATTR_FIELD(bg, bg)
    else ATTR_FIELD(flags, flags)
    else ATTR_FIELD(tab_size, tabSize)
}

STATIC const mp_map_elem_t mod_citrus_console_PrintConsole_locals_dict_table[] = {
        // Methods
        LOCAL_METHOD(select),
        LOCAL_METHOD(set_color),
        LOCAL_METHOD(set_position),
        LOCAL_METHOD(set_window),

        // Attributes (all int's)
        // cursor_x
        // cursor_y
        // width
        // height
        // window_x
        // window_y
        // window_width
        // window_height
        // fg
        // bg
        // flags
        // tab_size
};
STATIC MP_DEFINE_CONST_DICT(mod_citrus_console_PrintConsole_locals_dict, mod_citrus_console_PrintConsole_locals_dict_table);

const mp_obj_type_t mod_citrus_console_PrintConsole_type = {
        {&mp_type_type},
        .name = MP_QSTR_PrintConsole,
        .print = mod_citrus_console_PrintConsole_print,
        .make_new = mod_citrus_console_PrintConsole_make_new,
        .locals_dict = (mp_obj_t) &mod_citrus_console_PrintConsole_locals_dict,
        .attr = (mp_obj_t) &mod_citrus_console_PrintConsole_attr,
};