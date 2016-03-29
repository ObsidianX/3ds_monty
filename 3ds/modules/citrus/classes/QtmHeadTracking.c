#include <string.h>

#include <3ds.h>

#include "py/runtime.h"

#define SELF(src) mod_citrus_qtm_HeadTracking_t *self = src

#define ATTR_METHOD(__n) \
    if(!strcmp(name, qstr_str(MP_QSTR_##__n))) { \
        dest[0] = (mp_obj_t) &mod_citrus_qtm_HeadTracking_##__n##_obj; \
    }

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_citrus_qtm_HeadTracking_##__n##_obj, mod_citrus_qtm_HeadTracking_##__n)
#define METHOD_OBJ_VAR(__min, __max, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_citrus_qtm_HeadTracking_##__n##_obj, __min, __max, mod_citrus_qtm_HeadTracking_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_qtm_HeadTracking_##__n##_obj}

const mp_obj_type_t mod_citrus_qtm_HeadTracking_type;

typedef struct {
    mp_obj_base_t base;

    int val;
    QTM_HeadTrackingInfo info;
} mod_citrus_qtm_HeadTracking_t;

STATIC void _mod_citrus_qtm_update_info(mp_obj_t self_in) {
    SELF(self_in);

    Result res = QTM_GetHeadTrackingInfo(self->val, &self->info);

    if (R_FAILED(res)) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_RuntimeError,
                                                "couldn't read HeadTracking\nModule: %d\nLevel: %d\nSummary: %d\nDescription: %d",
                                                R_MODULE(res), R_LEVEL(res), R_SUMMARY(res), R_DESCRIPTION(res)));
    }
}

enum {
    NEW_ARG_VAL = 0,
    NEW_ARG_COUNT
};

STATIC mp_obj_t mod_citrus_qtm_HeadTracking_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in) {
    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_val, MP_ARG_INT, {.u_int = 0}},
    };

    mp_arg_val_t args[NEW_ARG_COUNT];
    mp_arg_parse_all_kw_array(n_args, n_kw, args_in, NEW_ARG_COUNT, allowed_args, args);

    mod_citrus_qtm_HeadTracking_t *obj = m_new_obj(mod_citrus_qtm_HeadTracking_t);
    obj->base.type = (mp_obj_t) &mod_citrus_qtm_HeadTracking_type;

    obj->val = args[NEW_ARG_VAL].u_int;

    _mod_citrus_qtm_update_info(obj);

    return obj;
}

STATIC void mod_citrus_qtm_HeadTracking_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    SELF(self_in);

    mp_printf(print, "<HeadTracking val=%d>", self->val);
}

STATIC mp_obj_t mod_citrus_qtm_HeadTracking_is_head_fully_detected(mp_obj_t self_in) {
    SELF(self_in);

    _mod_citrus_qtm_update_info(self_in);

    return mp_obj_new_bool(qtmCheckHeadFullyDetected(&self->info));
}

enum {
    CONVERT_ARG_SELF = 0,
    CONVERT_ARG_COORD_INDEX,
    CONVERT_ARG_SCREEN,
};

STATIC mp_obj_t mod_citrus_qtm_HeadTracking_convert_coord_to_screen(size_t n_args, const mp_obj_t *args) {
    mp_obj_t self_in = args[CONVERT_ARG_SELF];
    mp_obj_t coord_index = args[CONVERT_ARG_COORD_INDEX];
    mp_obj_t screen_size = NULL;
    if (n_args > 1) {
        screen_size = mp_const_none;
    }

    SELF(self_in);

    int index = mp_obj_get_int(coord_index);

    u32 x;
    u32 y;

    Result res;

    _mod_citrus_qtm_update_info(self_in);

    if (screen_size != NULL && MP_OBJ_IS_TYPE(screen_size, &mp_type_tuple)) {
        mp_obj_tuple_t *screen = MP_OBJ_TO_PTR(screen_size);
        if (screen->len != 2) {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "screen tuple must be (width,height)"));
        }

        float width = mp_obj_get_float(screen->items[0]);
        float height = mp_obj_get_float(screen->items[1]);

        res = qtmConvertCoordToScreen(&self->info.coords0[index], &width, &height, &x, &y);
    } else {
        res = qtmConvertCoordToScreen(&self->info.coords0[index], NULL, NULL, &x, &y);
    }

    if (R_FAILED(res)) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_RuntimeError,
                                                "couldn't convert coordinates\nModule: %d\nLevel: %d\nSummary: %d\nDescription: %d",
                                                R_MODULE(res), R_LEVEL(res), R_SUMMARY(res), R_DESCRIPTION(res)));
    }

    mp_obj_tuple_t *ret = mp_obj_new_tuple(2, NULL);
    ret->items[0] = mp_obj_new_int(x);
    ret->items[1] = mp_obj_new_int(y);

    return ret;
}

METHOD_OBJ_N(1, is_head_fully_detected);
METHOD_OBJ_VAR(1, 2, convert_coord_to_screen);

STATIC void mod_citrus_qtm_HeadTracking_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    SELF(self_in);
    const char *name = qstr_str(attr);
    bool load = dest[0] == MP_OBJ_NULL;

    // attributes are read-only
    if (!load) {
        return;
    }

    // Methods
    ATTR_METHOD(is_head_fully_detected)
    else ATTR_METHOD(convert_coord_to_screen)

    if (dest[0] != MP_OBJ_NULL) {
        dest[1] = self_in;
        return;
    }

    _mod_citrus_qtm_update_info(self_in);

    mp_obj_tuple_t *tuple = NULL;

    if (!strcmp(name, qstr_str(MP_QSTR_flags))) {
        mp_obj_tuple_t *tuple = mp_obj_new_tuple(5, NULL);
        for (int i = 0; i < 5; i++) {
            tuple->items[i] = mp_obj_new_int(self->info.flags[i]);
        }
    } else if (!strcmp(name, qstr_str(MP_QSTR_padding))) {
        mp_obj_tuple_t *tuple = mp_obj_new_tuple(3, NULL);
        for (int i = 0; i < 3; i++) {
            tuple->items[i] = mp_obj_new_int(self->info.padding[i]);
        }
    } else if (!strcmp(name, qstr_str(MP_QSTR_coordinates))) {
        mp_obj_tuple_t *tuple = mp_obj_new_tuple(4, NULL);
        for (int i = 0; i < 4; i++) {
            tuple->items[i] = mp_obj_new_tuple(2, NULL);
            ((mp_obj_tuple_t *) tuple->items[i])->items[0] = mp_obj_new_float(self->info.coords0[i].x);
            ((mp_obj_tuple_t *) tuple->items[i])->items[1] = mp_obj_new_float(self->info.coords0[i].y);
        }
    } else {
        dest[0] = MP_OBJ_NULL;
        return;
    }

    dest[0] = tuple;
}

STATIC const mp_map_elem_t mod_citrus_qtm_HeadTracking_locals_dict_table[] = {
        // Methods
        LOCAL_METHOD(is_head_fully_detected),
        LOCAL_METHOD(convert_coord_to_screen),

        // Attributes
        // flags (int, int, int, int, int)
        // padding (int, int, int)
        // coordinates ((float, float), (float, float), (float, float), (float, float))
};
STATIC MP_DEFINE_CONST_DICT(mod_citrus_qtm_HeadTracking_locals_dict, mod_citrus_qtm_HeadTracking_locals_dict_table);

const mp_obj_type_t mod_citrus_qtm_HeadTracking_type = {
        {&mp_type_type},
        .name = MP_QSTR_HeadTracking,
        .print = mod_citrus_qtm_HeadTracking_print,
        .make_new = mod_citrus_qtm_HeadTracking_make_new,
        .locals_dict = (mp_obj_t) &mod_citrus_qtm_HeadTracking_locals_dict,
        .attr = (mp_obj_t) &mod_citrus_qtm_HeadTracking_attr,
};