#include <string.h>

#include <sf2d.h>

#include "py/runtime.h"

#include "helpers.h"
#include "../citrus/helpers.h"

#define SELF(src) mod_sf2d_Texture_t *self = src

const mp_obj_type_t mod_sf2d_Texture_type;
STATIC const mp_obj_fun_builtin_t mod_sf2d_Texture___del___obj;

typedef struct {
    mp_obj_base_t base;

    sf2d_texture *tex;
} mod_sf2d_Texture_t;

enum {
    NEW_ARG_WIDTH = 0,
    NEW_ARG_HEIGHT,
    NEW_ARG_FORMAT,
    NEW_ARG_PLACE,
    NEW_ARG_COUNT
};

STATIC mp_obj_t mod_sf2d_Texture_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in) {
    mp_arg_check_num(n_args, n_kw, 4, NEW_ARG_COUNT, true);

    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_width,  MP_ARG_INT | MP_ARG_REQUIRED, {}},
            {MP_QSTR_height, MP_ARG_INT | MP_ARG_REQUIRED, {}},
            {MP_QSTR_format, MP_ARG_INT | MP_ARG_REQUIRED, {}},
            {MP_QSTR_place,  MP_ARG_INT | MP_ARG_REQUIRED, {}},
    };

    mp_arg_val_t args[NEW_ARG_COUNT];
    mp_arg_parse_all_kw_array(n_args, n_kw, args_in, NEW_ARG_COUNT, allowed_args, args);

    mod_sf2d_Texture_t *obj = m_new_obj(mod_sf2d_Texture_t);
    obj->base.type = (mp_obj_t) &mod_sf2d_Texture_type;

    int width = args[NEW_ARG_WIDTH].u_int;
    int height = args[NEW_ARG_HEIGHT].u_int;
    sf2d_texfmt format = _mod_sf2d_get_texfmt(args[NEW_ARG_FORMAT].u_int);
    sf2d_place place = _mod_sf2d_get_place(args[NEW_ARG_PLACE].u_int);

    obj->tex = sf2d_create_texture(width, height, format, place);

    return obj;
}

STATIC void mod_sf2d_Texture_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    const char *name = qstr_str(attr);
    bool load = dest[0] == MP_OBJ_NULL;

    // attributes are read-only
    if (!load) {
        return;
    }

    // Methods
    if (!strcmp(name, qstr_str(MP_QSTR___del__))) {
        dest[0] = (mp_obj_t) &mod_sf2d_Texture___del___obj;
    }
    if (dest[0] != MP_OBJ_NULL) {
        dest[1] = self_in;
        return;
    }
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
    DRAW_ARG_CENTER_X,
    DRAW_ARG_CENTER_Y,
    DRAW_ARG_ANGLE,
    DRAW_ARG_SCALE_X,
    DRAW_ARG_SCALE_Y,
    DRAW_ARG_CLIP_X,
    DRAW_ARG_CLIP_Y,
    DRAW_ARG_CLIP_W,
    DRAW_ARG_CLIP_H,
    DRAW_ARG_CLIP_DEPTH,
    DRAW_ARG_COUNT
};

STATIC mp_obj_t mod_sf2d_Texture_draw(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    // args:
    // positional: self, x, y
    // keyword:
    // - color
    // - center_x
    // - center_y
    // - angle
    // - scale_x
    // - scale_y
    // - clip_x
    // - clip_y
    // - clip_w
    // - clip_h
    // - depth
    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_self,     MP_ARG_OBJ | MP_ARG_REQUIRED},
            {MP_QSTR_x,        MP_ARG_INT | MP_ARG_REQUIRED},
            {MP_QSTR_y,        MP_ARG_INT | MP_ARG_REQUIRED},
            {MP_QSTR_color,    MP_ARG_INT | MP_ARG_KW_ONLY,  {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_center_x, MP_ARG_INT | MP_ARG_KW_ONLY,  {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_center_y, MP_ARG_INT | MP_ARG_KW_ONLY,  {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_angle,    MP_ARG_OBJ | MP_ARG_KW_ONLY,  {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_scale_x,  MP_ARG_OBJ | MP_ARG_KW_ONLY,  {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_scale_y,  MP_ARG_OBJ | MP_ARG_KW_ONLY,  {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_clip_x,   MP_ARG_INT | MP_ARG_KW_ONLY,  {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_clip_y,   MP_ARG_INT | MP_ARG_KW_ONLY,  {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_clip_w,   MP_ARG_INT | MP_ARG_KW_ONLY,  {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_clip_h,   MP_ARG_INT | MP_ARG_KW_ONLY,  {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_depth,    MP_ARG_INT | MP_ARG_KW_ONLY,  {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
    };

    mp_arg_val_t args[DRAW_ARG_COUNT];
    mp_arg_parse_all((mp_uint_t) n_args, pos_args, kw_args, DRAW_ARG_COUNT, allowed_args, args);

    SELF(args[DRAW_ARG_SELF].u_obj);

    int x = args[DRAW_ARG_X].u_int;
    int y = args[DRAW_ARG_Y].u_int;

    sf2d_draw_texture(self->tex, x, y);

    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_Texture_draw_quad_uv(size_t n_args, const mp_obj_t *args) {
    // self, l, t, r, b, u0, v0, u1, v1, params
    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_Texture_set_pixel(size_t n_args, const mp_obj_t *args) {
    // self, x, y, color
    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_Texture_get_pixel(mp_obj_t self_in, mp_obj_t x, mp_obj_t y) {
    // returns color
    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_Texture_tile32(mp_obj_t self_in) {
    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_sf2d_Texture___del___obj, mod_sf2d_Texture___del__);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_sf2d_Texture_bind_obj, mod_sf2d_Texture_bind);
STATIC MP_DEFINE_CONST_FUN_OBJ_3(mod_sf2d_Texture_bind_color_obj, mod_sf2d_Texture_bind_color);
STATIC MP_DEFINE_CONST_FUN_OBJ_3(mod_sf2d_Texture_bind_parameters_obj, mod_sf2d_Texture_bind_parameters);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_sf2d_Texture_set_params_obj, mod_sf2d_Texture_set_params);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_sf2d_Texture_get_params_obj, mod_sf2d_Texture_get_params);
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(mod_sf2d_Texture_draw_obj, 3, mod_sf2d_Texture_draw);
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_sf2d_Texture_draw_quad_uv_obj, 10, 10, mod_sf2d_Texture_draw_quad_uv);
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_sf2d_Texture_set_pixel_obj, 4, 4, mod_sf2d_Texture_set_pixel);
STATIC MP_DEFINE_CONST_FUN_OBJ_3(mod_sf2d_Texture_get_pixel_obj, mod_sf2d_Texture_get_pixel);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_sf2d_Texture_tile32_obj, mod_sf2d_Texture_tile32);

STATIC const mp_map_elem_t mod_sf2d_Texture_locals_dict_table[] = {
        // Methods
        {MP_OBJ_NEW_QSTR(MP_QSTR___del__),         (mp_obj_t) &mod_sf2d_Texture___del___obj},
        {MP_OBJ_NEW_QSTR(MP_QSTR_bind),            (mp_obj_t) &mod_sf2d_Texture_bind_obj},
        {MP_OBJ_NEW_QSTR(MP_QSTR_bind_color),      (mp_obj_t) &mod_sf2d_Texture_bind_color_obj},
        {MP_OBJ_NEW_QSTR(MP_QSTR_bind_parameters), (mp_obj_t) &mod_sf2d_Texture_bind_parameters_obj},
        {MP_OBJ_NEW_QSTR(MP_QSTR_set_params),      (mp_obj_t) &mod_sf2d_Texture_set_params_obj},
        {MP_OBJ_NEW_QSTR(MP_QSTR_get_params),      (mp_obj_t) &mod_sf2d_Texture_get_params_obj},
        {MP_OBJ_NEW_QSTR(MP_QSTR_draw),            (mp_obj_t) &mod_sf2d_Texture_draw_obj},
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