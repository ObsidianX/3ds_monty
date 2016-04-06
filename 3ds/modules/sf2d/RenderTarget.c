#include <string.h>

#include <3ds.h>
#include <sf2d.h>

#include "py/runtime.h"

#include "helpers.h"

#define SELF(src) mod_sf2d_RenderTarget_t *self = src

#define ATTR_METHOD(__n) \
    if(!strcmp(name, qstr_str(MP_QSTR_##__n))) { \
        dest[0] = (mp_obj_t) &mod_sf2d_RenderTarget_##__n##_obj; \
    }

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_sf2d_RenderTarget_##__n##_obj, mod_sf2d_RenderTarget_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_sf2d_RenderTarget_##__n##_obj}

const mp_obj_type_t mod_sf2d_RenderTarget_type;

typedef struct {
    mp_obj_base_t base;

    sf2d_rendertarget *target;
    u32 clear_color;
    mp_obj_t texture;
} mod_sf2d_RenderTarget_t;

enum {
    NEW_ARG_WIDTH = 0,
    NEW_ARG_HEIGHT,
    NEW_ARG_COUNT
};

STATIC mp_obj_t mod_sf2d_RenderTarget_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in) {
    mp_arg_check_num(n_args, n_kw, 2, NEW_ARG_COUNT, true);

    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_width,  MP_ARG_INT | MP_ARG_REQUIRED},
            {MP_QSTR_height, MP_ARG_INT | MP_ARG_REQUIRED},
    };

    mp_arg_val_t args[NEW_ARG_COUNT];
    mp_arg_parse_all_kw_array(n_args, n_kw, args_in, NEW_ARG_COUNT, allowed_args, args);

    mod_sf2d_RenderTarget_t *obj = m_new_obj(mod_sf2d_RenderTarget_t);
    obj->base.type = (mp_obj_t) &mod_sf2d_RenderTarget_type;

    int width = args[NEW_ARG_WIDTH].u_int;
    int height = args[NEW_ARG_HEIGHT].u_int;

    obj->target = sf2d_create_rendertarget(width, height);
    //obj->texture = _mod_sf2d_Texture_from_ptr(&obj->target->texture);

    return obj;
}

STATIC void mod_sf2d_RenderTarget_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    SELF(self_in);

    mp_printf(print, "<RenderTarget sf2d_rendertarget=%p>", self->target);
}

STATIC mp_obj_t mod_sf2d_RenderTarget___del__(mp_obj_t self_in) {
    SELF(self_in);

    sf2d_free_target(self->target);

    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_RenderTarget_clear(mp_obj_t self_in) {
    SELF(self_in);

    sf2d_clear_target(self->target, self->clear_color);

    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_RenderTarget_start_frame(mp_obj_t self_in) {
    SELF(self_in);

    sf2d_start_frame_target(self->target);

    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_RenderTarget_end_frame(mp_obj_t self_in) {
    sf2d_end_frame();

    return mp_const_none;
}

METHOD_OBJ_N(1, __del__);
METHOD_OBJ_N(1, clear);
METHOD_OBJ_N(1, start_frame);
METHOD_OBJ_N(1, end_frame);

STATIC void mod_sf2d_RenderTarget_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    SELF(self_in);
    const char *name = qstr_str(attr);
    bool load = dest[0] == MP_OBJ_NULL;

    if (load) {
        // Methods
        ATTR_METHOD(__del__)
        else ATTR_METHOD(clear)
        else ATTR_METHOD(start_frame)
        else ATTR_METHOD(end_frame)

        if (dest[0] != MP_OBJ_NULL) {
            dest[1] = self_in;
            return;
        }
    }

    // Variables
    if (!strcmp(name, qstr_str(MP_QSTR_clear_color))) {
        if (load) {
            dest[0] = mp_obj_new_int(self->clear_color);
        } else {
            self->clear_color = mp_obj_get_int(dest[1]);
            dest[0] = MP_OBJ_NULL;
        }
    } else if (!strcmp(name, qstr_str(MP_QSTR_texture))) {
        if (load) {
            dest[0] = self->texture;
        }
    } else if (!strcmp(name, qstr_str(MP_QSTR_width))) {
        if (load) {
            dest[0] = mp_obj_new_int(self->target->texture.width);
        }
    } else if (!strcmp(name, qstr_str(MP_QSTR_height))) {
        if (load) {
            dest[0] = mp_obj_new_int(self->target->texture.height);
        }
    }
}

STATIC const mp_map_elem_t mod_sf2d_RenderTarget_locals_dict_table[] = {
        // Methods
        LOCAL_METHOD(__del__),
        LOCAL_METHOD(clear),
        LOCAL_METHOD(start_frame),
        LOCAL_METHOD(end_frame),

        // Attributes
        // clear_color
};
STATIC MP_DEFINE_CONST_DICT(mod_sf2d_RenderTarget_locals_dict, mod_sf2d_RenderTarget_locals_dict_table);

const mp_obj_type_t mod_sf2d_RenderTarget_type = {
        {&mp_type_type},
        .name = MP_QSTR_RenderTarget,
        .print = mod_sf2d_RenderTarget_print,
        .make_new = mod_sf2d_RenderTarget_make_new,
        .locals_dict = (mp_obj_t) &mod_sf2d_RenderTarget_locals_dict,
        .attr = (mp_obj_t) &mod_sf2d_RenderTarget_attr,
};