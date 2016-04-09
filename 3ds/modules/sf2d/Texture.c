#include <string.h>

#include <sf2d.h>

#include "py/runtime.h"

#include "helpers.h"
#include "../citrus/helpers.h"

#define SELF(src) mod_sf2d_Texture_t *self = src

#define HASARG(n) ((used_args & BIT(n)) != 0)

#define ATTR_METHOD(__n) \
    if(!strcmp(name, qstr_str(MP_QSTR_##__n))) { \
        dest[0] = (mp_obj_t) &mod_sf2d_Texture_##__n##_obj; \
    }

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_sf2d_Texture_##__n##_obj, mod_sf2d_Texture_##__n)
#define METHOD_OBJ_KW(__min, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_KW(mod_sf2d_Texture_##__n##_obj, __min, mod_sf2d_Texture_##__n)
#define METHOD_OBJ_VAR(__min, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_VAR(mod_sf2d_Texture_##__n##_obj, __min, mod_sf2d_Texture_##__n)
#define METHOD_OBJ_VAR_N(__num, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_sf2d_Texture_##__n##_obj, __num, __num, mod_sf2d_Texture_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_sf2d_Texture_##__n##_obj}

const mp_obj_type_t mod_sf2d_Texture_type;

typedef struct {
    mp_obj_base_t base;

    sf2d_texture *tex;
} mod_sf2d_Texture_t;

mp_obj_t _mod_sf2d_Texture_from_ptr(sf2d_texture *ptr) {
    mod_sf2d_Texture_t *obj = m_new_obj(mod_sf2d_Texture_t);
    obj->base.type = (mp_obj_t) &mod_sf2d_Texture_type;
    obj->tex = ptr;
    return obj;
}

enum {
    NEW_ARG_WIDTH = 0,
    NEW_ARG_HEIGHT,
    NEW_ARG_FORMAT,
    NEW_ARG_PLACE,
    NEW_ARG_DATA,
    NEW_ARG_COUNT
};

STATIC mp_obj_t mod_sf2d_Texture_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in) {
    mp_arg_check_num(n_args, n_kw, 2, NEW_ARG_COUNT, true);

    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_width,  MP_ARG_INT | MP_ARG_REQUIRED, {}},
            {MP_QSTR_height, MP_ARG_INT | MP_ARG_REQUIRED, {}},
            {MP_QSTR_format, MP_ARG_INT,                   {.u_int = TEXFMT_RGBA8}},
            {MP_QSTR_place,  MP_ARG_INT,                   {.u_int = SF2D_PLACE_RAM}},
            {MP_QSTR_data,   MP_ARG_OBJ,                   {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
    };

    mp_arg_val_t args[NEW_ARG_COUNT];
    mp_arg_parse_all_kw_array(n_args, n_kw, args_in, NEW_ARG_COUNT, allowed_args, args);

    mod_sf2d_Texture_t *obj = m_new_obj(mod_sf2d_Texture_t);
    obj->base.type = (mp_obj_t) &mod_sf2d_Texture_type;

    int width = args[NEW_ARG_WIDTH].u_int;
    int height = args[NEW_ARG_HEIGHT].u_int;
    sf2d_texfmt format = _mod_sf2d_get_texfmt(args[NEW_ARG_FORMAT].u_int);
    sf2d_place place = _mod_sf2d_get_place(args[NEW_ARG_PLACE].u_int);

    if (args[NEW_ARG_DATA].u_rom_obj != &mp_const_none_obj) {
        mp_buffer_info_t buffer_info;
        mp_get_buffer(args[NEW_ARG_DATA].u_obj, &buffer_info, MP_BUFFER_READ);

        obj->tex = sf2d_create_texture_mem_RGBA8(buffer_info.buf, width, height, format, place);
    } else {
        obj->tex = sf2d_create_texture(width, height, format, place);
    }

    if (obj->tex == NULL) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_MemoryError, "not enough memory for texture"));
    }

    return obj;
}

STATIC void mod_sf2d_Texture_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    SELF(self_in);

    mp_printf(print, "<Texture sf2d_texture=%p>", self->tex);
}

STATIC mp_obj_t mod_sf2d_Texture___del__(mp_obj_t self_in) {
    SELF(self_in);

    sf2d_free_texture(self->tex);

    return mp_const_none;
}

enum {
    FILL_ARG_SELF = 0,
    FILL_ARG_WIDTH,
    FILL_ARG_HEIGHT,
    FILL_ARG_DATA
};

STATIC mp_obj_t mod_sf2d_Texture_fill_from_rgba8(size_t n_args, const mp_obj_t *args) {
    SELF(args[FILL_ARG_SELF]);

    int width = mp_obj_get_int(args[FILL_ARG_WIDTH]);
    int height = mp_obj_get_int(args[FILL_ARG_HEIGHT]);

    mp_buffer_info_t buffer_info;
    mp_get_buffer(args[FILL_ARG_DATA], &buffer_info, MP_BUFFER_READ);

    sf2d_fill_texture_from_RGBA8(self->tex, buffer_info.buf, width, height);

    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_Texture_bind(mp_obj_t self_in, mp_obj_t unit) {
    SELF(self_in);

    GPU_TEXUNIT _unit = _mod_citrus_gpu_get_texunit(unit);

    sf2d_bind_texture(self->tex, _unit);

    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_Texture_bind_color(mp_obj_t self_in, mp_obj_t unit, mp_obj_t color) {
    SELF(self_in);

    GPU_TEXUNIT _unit = _mod_citrus_gpu_get_texunit(unit);
    u32 _color = mp_obj_get_int(color);

    sf2d_bind_texture_color(self->tex, _unit, _color);

    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_Texture_bind_parameters(mp_obj_t self_in, mp_obj_t unit, mp_obj_t params) {
    SELF(self_in);

    GPU_TEXUNIT _unit = _mod_citrus_gpu_get_texunit(unit);
    u32 _params = mp_obj_get_int(params);

    sf2d_bind_texture_parameters(self->tex, _unit, _params);

    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_Texture_set_params(mp_obj_t self_in, mp_obj_t params) {
    SELF(self_in);

    u32 _params = mp_obj_get_int(params);

    sf2d_texture_set_params(self->tex, _params);

    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_Texture_get_params(mp_obj_t self_in) {
    SELF(self_in);

    return mp_obj_new_int(sf2d_texture_get_params(self->tex));
}

enum {
    DRAW_ARG_SELF = 0,
    DRAW_ARG_X,
    DRAW_ARG_Y,
    DRAW_ARG_COLOR,
    DRAW_ARG_CENTER_TUPLE,
    DRAW_ARG_ANGLE,
    DRAW_ARG_SCALE_TUPLE,
    DRAW_ARG_CLIP_TUPLE,
    DRAW_ARG_DEPTH,
    DRAW_ARG_COUNT
};

STATIC mp_obj_t mod_sf2d_Texture_draw(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // args:
    // positional: self, x, y
    // keyword:
    // - color
    // - center
    // - angle
    // - scale
    // - clip
    // - depth
    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_self,   MP_ARG_OBJ | MP_ARG_REQUIRED},
            {MP_QSTR_x,      MP_ARG_INT | MP_ARG_REQUIRED},
            {MP_QSTR_y,      MP_ARG_INT | MP_ARG_REQUIRED},
            {MP_QSTR_color,  MP_ARG_INT | MP_ARG_KW_ONLY, {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_center, MP_ARG_OBJ | MP_ARG_KW_ONLY, {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_angle,  MP_ARG_OBJ | MP_ARG_KW_ONLY, {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_scale,  MP_ARG_OBJ | MP_ARG_KW_ONLY, {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_clip,   MP_ARG_OBJ | MP_ARG_KW_ONLY, {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_depth,  MP_ARG_INT | MP_ARG_KW_ONLY, {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
    };

    mp_arg_val_t args[DRAW_ARG_COUNT];
    mp_arg_parse_all((mp_uint_t) n_args, pos_args, kw_args, DRAW_ARG_COUNT, allowed_args, args);

    SELF(args[DRAW_ARG_SELF].u_obj);

    int x = args[DRAW_ARG_X].u_int;
    int y = args[DRAW_ARG_Y].u_int;
    int z = args[DRAW_ARG_DEPTH].u_int;

    int color = args[DRAW_ARG_COLOR].u_int;

    mp_obj_tuple_t *center_tuple = MP_OBJ_TO_PTR(args[DRAW_ARG_CENTER_TUPLE].u_obj);
    mp_obj_tuple_t *scale_tuple = MP_OBJ_TO_PTR(args[DRAW_ARG_SCALE_TUPLE].u_obj);
    mp_obj_tuple_t *clip_tuple = MP_OBJ_TO_PTR(args[DRAW_ARG_CLIP_TUPLE].u_obj);

    float scale_x, scale_y, center_x, center_y, angle;

    u16 used_args = 0;
    for (int i = DRAW_ARG_COLOR; i < DRAW_ARG_COUNT; i++) {
        if (args[i].u_rom_obj != &mp_const_none_obj) {
            used_args |= BIT(i);
        }
    }

    if (HASARG(DRAW_ARG_SCALE_TUPLE)) {
        scale_x = mp_obj_get_float(scale_tuple->items[0]);
        scale_y = mp_obj_get_float(scale_tuple->items[1]);
    }

    if (HASARG(DRAW_ARG_ANGLE)) {
        angle = mp_obj_get_float(args[DRAW_ARG_ANGLE].u_obj);
    }

    if (HASARG(DRAW_ARG_CENTER_TUPLE)) {
        center_x = mp_obj_get_float(center_tuple->items[0]);
        center_y = mp_obj_get_float(center_tuple->items[1]);
    }

    // !! OVERLOAD !!
    if (HASARG(DRAW_ARG_DEPTH)) {
        if (HASARG(DRAW_ARG_COLOR)) {
            sf2d_draw_texture_depth_blend(self->tex, x, y, z, color);
        } else {
            sf2d_draw_texture_depth(self->tex, x, y, z);
        }
    } else if (HASARG(DRAW_ARG_CLIP_TUPLE)) {
        int tex_x = mp_obj_get_int(clip_tuple->items[0]);
        int tex_y = mp_obj_get_int(clip_tuple->items[1]);
        int tex_w = mp_obj_get_int(clip_tuple->items[2]);
        int tex_h = mp_obj_get_int(clip_tuple->items[3]);

        if (HASARG(DRAW_ARG_SCALE_TUPLE)) {
            if (HASARG(DRAW_ARG_ANGLE)) {
                if (HASARG(DRAW_ARG_COLOR)) {
                    sf2d_draw_texture_part_rotate_scale_blend(self->tex, x, y, angle, tex_x, tex_y, tex_w, tex_h, scale_x, scale_y, color);
                } else {
                    sf2d_draw_texture_part_rotate_scale(self->tex, x, y, angle, tex_x, tex_y, tex_w, tex_h, scale_x, scale_y);
                }
            } else if (HASARG(DRAW_ARG_COLOR)) {
                sf2d_draw_texture_part_scale_blend(self->tex, x, y, tex_x, tex_y, tex_w, tex_h, scale_x, scale_y, color);
            } else {
                sf2d_draw_texture_part_scale(self->tex, x, y, tex_x, tex_y, tex_w, tex_h, scale_x, scale_y);
            }
        } else if (HASARG(DRAW_ARG_COLOR)) {
            sf2d_draw_texture_part_blend(self->tex, x, y, tex_x, tex_y, tex_w, tex_h, color);
        } else {
            sf2d_draw_texture_part(self->tex, x, y, tex_x, tex_y, tex_w, tex_h);
        }
    } else if (HASARG(DRAW_ARG_SCALE_TUPLE)) {
        if (HASARG(DRAW_ARG_COLOR)) {
            sf2d_draw_texture_scale_blend(self->tex, x, y, scale_x, scale_y, color);
        } else {
            sf2d_draw_texture_scale(self->tex, x, y, scale_x, scale_y);
        }
    } else if (HASARG(DRAW_ARG_ANGLE)) {
        if (HASARG(DRAW_ARG_CENTER_TUPLE)) {
            if (HASARG(DRAW_ARG_SCALE_TUPLE)) {
                if (HASARG(DRAW_ARG_COLOR)) {
                    sf2d_draw_texture_rotate_scale_hotspot_blend(self->tex, x, y, angle, scale_x, scale_y, center_x, center_y, color);
                } else {
                    sf2d_draw_texture_rotate_scale_hotspot(self->tex, x, y, angle, scale_x, scale_y, center_x, center_y);
                }
            } else if (HASARG(DRAW_ARG_COLOR)) {
                sf2d_draw_texture_rotate_hotspot_blend(self->tex, x, y, angle, center_x, center_y, color);
            } else {
                sf2d_draw_texture_rotate_hotspot(self->tex, x, y, angle, center_x, center_y);
            }
        } else {
            sf2d_draw_texture_rotate(self->tex, x, y, angle);
        }
    } else if (HASARG(DRAW_ARG_CENTER_TUPLE)) {
        sf2d_draw_texture_rotate_hotspot(self->tex, x, y, 0, center_x, center_y);
    } else if (HASARG(DRAW_ARG_COLOR)) {
        sf2d_draw_texture_blend(self->tex, x, y, color);
    } else {
        sf2d_draw_texture(self->tex, x, y);
    }

    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_Texture_draw_quad_uv(size_t n_args, const mp_obj_t *args) {
    // self, l, t, r, b, u0, v0, u1, v1, params
    SELF(args[0]);

    float left = mp_obj_get_float(args[1]);
    float top = mp_obj_get_float(args[2]);
    float right = mp_obj_get_float(args[3]);
    float bottom = mp_obj_get_float(args[4]);

    float u0 = mp_obj_get_float(args[5]);
    float v0 = mp_obj_get_float(args[6]);
    float u1 = mp_obj_get_float(args[7]);
    float v1 = mp_obj_get_float(args[8]);

    unsigned int params = mp_obj_get_int(args[9]);

    sf2d_draw_quad_uv(self->tex, left, top, right, bottom, u0, v0, u1, v1, params);
    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_Texture_set_pixel(size_t n_args, const mp_obj_t *args) {
    // self, x, y, color
    SELF(args[0]);

    int x = mp_obj_get_int(args[1]);
    int y = mp_obj_get_int(args[2]);

    u32 color = mp_obj_get_int(args[3]);

    sf2d_set_pixel(self->tex, x, y, color);
    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_Texture_get_pixel(mp_obj_t self_in, mp_obj_t x, mp_obj_t y) {
    // returns color
    SELF(self_in);

    int _x = mp_obj_get_int(x);
    int _y = mp_obj_get_int(y);

    return mp_obj_new_int(sf2d_get_pixel(self->tex, _x, _y));
}

STATIC mp_obj_t mod_sf2d_Texture_tile32(mp_obj_t self_in) {
    SELF(self_in);

    sf2d_texture_tile32(self->tex);
    return mp_const_none;
}

METHOD_OBJ_N(1, __del__);
METHOD_OBJ_VAR_N(4, fill_from_rgba8);
METHOD_OBJ_N(2, bind);
METHOD_OBJ_N(3, bind_color);
METHOD_OBJ_N(3, bind_parameters);
METHOD_OBJ_N(2, set_params);
METHOD_OBJ_N(1, get_params);
METHOD_OBJ_KW(3, draw);
METHOD_OBJ_VAR_N(10, draw_quad_uv);
METHOD_OBJ_VAR_N(4, set_pixel);
METHOD_OBJ_N(3, get_pixel);
METHOD_OBJ_N(1, tile32);

STATIC void mod_sf2d_Texture_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    SELF(self_in);
    const char *name = qstr_str(attr);
    bool load = dest[0] == MP_OBJ_NULL;

    // attributes are read-only
    if (!load) {
        return;
    }

    // Methods
    ATTR_METHOD(__del__)
    else ATTR_METHOD(fill_from_rgba8)
    else ATTR_METHOD(bind)
    else ATTR_METHOD(bind_color)
    else ATTR_METHOD(bind_parameters)
    else ATTR_METHOD(set_params)
    else ATTR_METHOD(get_params)
    else ATTR_METHOD(draw)
    else ATTR_METHOD(draw_quad_uv)
    else ATTR_METHOD(set_pixel)
    else ATTR_METHOD(get_pixel)
    else ATTR_METHOD(tile32)

    if (dest[0] != MP_OBJ_NULL) {
        dest[1] = self_in;
        return;
    }

    if (!strcmp(name, qstr_str(MP_QSTR_width))) {
        dest[0] = mp_obj_new_int(self->tex->width);
    } else if (!strcmp(name, qstr_str(MP_QSTR_height))) {
        dest[0] = mp_obj_new_int(self->tex->height);
    }
}

STATIC const mp_map_elem_t mod_sf2d_Texture_locals_dict_table[] = {
        // Methods
        LOCAL_METHOD(__del__),
        LOCAL_METHOD(fill_from_rgba8),
        LOCAL_METHOD(bind),
        LOCAL_METHOD(bind_color),
        LOCAL_METHOD(bind_parameters),
        LOCAL_METHOD(set_params),
        LOCAL_METHOD(get_params),
        LOCAL_METHOD(draw),
        LOCAL_METHOD(draw_quad_uv),
        LOCAL_METHOD(set_pixel),
        LOCAL_METHOD(get_pixel),
        LOCAL_METHOD(tile32),
};
STATIC MP_DEFINE_CONST_DICT(mod_sf2d_Texture_locals_dict, mod_sf2d_Texture_locals_dict_table);

const mp_obj_type_t mod_sf2d_Texture_type = {
        {&mp_type_type},
        .name = MP_QSTR_Texture,
        .print = mod_sf2d_Texture_print,
        .make_new = mod_sf2d_Texture_make_new,
        .locals_dict = (mp_obj_t) &mod_sf2d_Texture_locals_dict,
        .attr = (mp_obj_t) &mod_sf2d_Texture_attr,
};