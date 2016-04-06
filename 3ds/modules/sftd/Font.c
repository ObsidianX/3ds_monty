#include <string.h>

#include <sftd.h>

#include "py/runtime.h"

#define SELF(src) mod_sftd_Font_t *self = src

#define ATTR_METHOD(__n) \
    if(!strcmp(name, qstr_str(MP_QSTR_##__n))) { \
        dest[0] = (mp_obj_t) &mod_sftd_Font_##__n##_obj; \
    }

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_sftd_Font_##__n##_obj, mod_sftd_Font_##__n)
#define METHOD_OBJ_VAR(__min, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_VAR(mod_sftd_Font_##__n##_obj, __min, mod_sftd_Font_##__n)
#define METHOD_OBJ_VAR_N(__num, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_sftd_Font_##__n##_obj, __num, __num, mod_sftd_Font_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_sftd_Font_##__n##_obj}

const mp_obj_type_t mod_sftd_Font_type;

typedef struct {
    mp_obj_base_t base;

    sftd_font *font;

    unsigned int color;
    unsigned int size;
    unsigned int line_width;
} mod_sftd_Font_t;

enum {
    NEW_ARG_FILE = 0,
    NEW_ARG_SIZE,
    NEW_ARG_COLOR,
    NEW_ARG_LINE_WIDTH,
    NEW_ARG_COUNT
};

STATIC mp_obj_t mod_sftd_Font_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in) {
    mp_arg_check_num(n_args, n_kw, 2, NEW_ARG_COUNT, true);

    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_filename,   MP_ARG_OBJ | MP_ARG_REQUIRED, {}},
            {MP_QSTR_size,       MP_ARG_INT | MP_ARG_REQUIRED, {}},
            {MP_QSTR_color,      MP_ARG_INT,                   {.u_int = 0}},
            {MP_QSTR_line_width, MP_ARG_INT,                   {.u_int = -1}}
    };

    mp_arg_val_t args[NEW_ARG_COUNT];
    mp_arg_parse_all_kw_array(n_args, n_kw, args_in, NEW_ARG_COUNT, allowed_args, args);

    mod_sftd_Font_t *obj = m_new_obj(mod_sftd_Font_t);
    obj->base.type = (mp_obj_t) &mod_sftd_Font_type;

    const char *filename = mp_obj_str_get_str(args[NEW_ARG_FILE].u_obj);
    int size = args[NEW_ARG_SIZE].u_int;
    int color = args[NEW_ARG_COLOR].u_int;
    int line_width = args[NEW_ARG_LINE_WIDTH].u_int;

    obj->font = sftd_load_font_file(filename);
    obj->size = size;
    obj->color = color;
    obj->line_width = line_width;

    return obj;
}

STATIC void mod_sftd_Font_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    SELF(self_in);

    mp_printf(print, "<Font sftd_font=%p>", self->font);
}

STATIC mp_obj_t mod_sftd_Font___del__(mp_obj_t self_in) {
    SELF(self_in);

    sftd_free_font(self->font);

    return mp_const_none;
}

enum {
    DRAW_ARG_SELF = 0,
    DRAW_ARG_X,
    DRAW_ARG_Y,
    DRAW_ARG_TEXT
};

STATIC mp_obj_t mod_sftd_Font_draw_text(size_t n_args, const mp_obj_t *args) {
    SELF(args[DRAW_ARG_SELF]);

    int x = mp_obj_get_int(args[DRAW_ARG_X]);
    int y = mp_obj_get_int(args[DRAW_ARG_Y]);
    const char *text = mp_obj_str_get_str(args[DRAW_ARG_TEXT]);

    if (self->line_width > 0) {
        sftd_draw_text_wrap(self->font, x, y, self->color, self->size, self->line_width, text);
    } else {
        sftd_draw_text(self->font, x, y, self->color, self->size, text);
    }

    return mp_const_none;
}

STATIC mp_obj_t mod_sftd_Font_get_text_width(mp_obj_t self_in, mp_obj_t text) {
    SELF(self_in);

    const char *txt = mp_obj_str_get_str(text);

    return mp_obj_new_int(sftd_get_text_width(self->font, self->size, txt));
}

STATIC mp_obj_t mod_sftd_Font_calc_bounding_box(mp_obj_t self_in, mp_obj_t text) {
    SELF(self_in);

    if (self->line_width <= 0) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_RuntimeError, "Font.line_width must be set before using calc_bounding_box"));
    }

    int width = 0, height = 0;
    const char *txt = mp_obj_str_get_str(text);
    sftd_calc_bounding_box(&width, &height, self->font, self->size, self->line_width, txt);

    mp_obj_tuple_t *tuple = mp_obj_new_tuple(2, NULL);
    tuple->items[0] = mp_obj_new_int(width);
    tuple->items[1] = mp_obj_new_int(height);

    return tuple;
}

METHOD_OBJ_N(1, __del__);
METHOD_OBJ_VAR_N(4, draw_text);
METHOD_OBJ_N(2, get_text_width);
METHOD_OBJ_N(2, calc_bounding_box);

STATIC void mod_sftd_Font_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    SELF(self_in);
    const char *name = qstr_str(attr);
    bool load = dest[0] == MP_OBJ_NULL;

    // attributes are read-only
    if (load) {
        // Methods
        ATTR_METHOD(__del__)
        else ATTR_METHOD(draw_text)
        else ATTR_METHOD(get_text_width)
        else ATTR_METHOD(calc_bounding_box)

        if (dest[0] != MP_OBJ_NULL) {
            dest[1] = self_in;
            return;
        }
    }

    if (!strcmp(name, qstr_str(MP_QSTR_size))) {
        if (load) {
            dest[0] = mp_obj_new_int(self->size);
        } else {
            self->size = mp_obj_get_int(dest[1]);
            dest[0] = MP_OBJ_NULL;
        }
    } else if (!strcmp(name, qstr_str(MP_QSTR_color))) {
        if (load) {
            dest[0] = mp_obj_new_int(self->color);
        } else {
            self->color = mp_obj_get_int(dest[1]);
            dest[0] = MP_OBJ_NULL;
        }
    } else if (!strcmp(name, qstr_str(MP_QSTR_line_width))) {
        if (load) {
            dest[0] = mp_obj_new_int(self->line_width);
        } else {
            self->line_width = mp_obj_get_int(dest[1]);
            dest[0] = MP_OBJ_NULL;
        }
    }
}

STATIC const mp_map_elem_t mod_sftd_Font_locals_dict_table[] = {
        // Methods
        LOCAL_METHOD(__del__),
        LOCAL_METHOD(draw_text),
        LOCAL_METHOD(get_text_width),
        LOCAL_METHOD(calc_bounding_box),

        // Attributes
        // color
        // size
        // line_width
};

STATIC MP_DEFINE_CONST_DICT(mod_sftd_Font_locals_dict, mod_sftd_Font_locals_dict_table);

const mp_obj_type_t mod_sftd_Font_type = {
        {&mp_type_type},
        .name = MP_QSTR_Font,
        .print = mod_sftd_Font_print,
        .make_new = mod_sftd_Font_make_new,
        .locals_dict = (mp_obj_t) &mod_sftd_Font_locals_dict,
        .attr = mod_sftd_Font_attr,
};