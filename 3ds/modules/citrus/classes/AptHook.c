#include <string.h>

#include <3ds.h>

#include "py/runtime.h"

#define SELF(src) mod_citrus_apt_Hook_t *self = src

#define ATTR_METHOD(__n) \
    if(!strcmp(name, qstr_str(MP_QSTR_##__n))) { \
        dest[0] = (mp_obj_t) &mod_citrus_apt_Hook_##__n##_obj; \
    }

const mp_obj_type_t mod_citrus_apt_Hook_type;

typedef struct {
    mp_obj_base_t base;

    aptHookCookie cookie;
    mp_obj_t callback;
} mod_citrus_apt_Hook_t;

STATIC void _mod_citrus_apt_Hook_callback(APT_HookType hook, void *param) {
    SELF(param);

    mp_call_function_1(self->callback, mp_obj_new_int(hook));
}

enum {
    NEW_ARG_CALLBACK = 0,
    NEW_ARG_COUNT
};

STATIC mp_obj_t mod_citrus_apt_Hook_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in) {
    mp_arg_check_num(n_args, n_kw, 1, NEW_ARG_COUNT, true);

    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_callback, MP_ARG_OBJ | MP_ARG_REQUIRED, {}},
    };

    mp_arg_val_t args[NEW_ARG_COUNT];
    mp_arg_parse_all_kw_array(n_args, n_kw, args_in, NEW_ARG_COUNT, allowed_args, args);

    mod_citrus_apt_Hook_t *obj = m_new_obj(mod_citrus_apt_Hook_t);
    obj->base.type = (mp_obj_t) &mod_citrus_apt_Hook_type;

    obj->callback = args[NEW_ARG_CALLBACK].u_obj;
    if (!MP_OBJ_IS_FUN(obj->callback)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "callback must be a function"));
    }

    aptHook(&obj->cookie, _mod_citrus_apt_Hook_callback, obj);

    return obj;
}

STATIC void mod_citrus_apt_Hook_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    SELF(self_in);

    mp_printf(print, "<Hook cookie=%p>", self->cookie);
}

STATIC mp_obj_t mod_citrus_apt_Hook___del__(mp_obj_t self_in) {
    SELF(self_in);

    aptUnhook(&self->cookie);

    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_apt_Hook___del___obj, mod_citrus_apt_Hook___del__);

STATIC void mod_citrus_apt_Hook_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    const char *name = qstr_str(attr);
    bool load = dest[0] == MP_OBJ_NULL;

    // attributes are read-only
    if (!load) {
        return;
    }

    // Methods
    ATTR_METHOD(__del__)

    if (dest[0] != MP_OBJ_NULL) {
        dest[1] = self_in;
        return;
    }
}

STATIC const mp_map_elem_t mod_citrus_apt_Hook_locals_dict_table[] = {
        // Methods
        {MP_OBJ_NEW_QSTR(MP_QSTR___del__), (mp_obj_t) &mod_citrus_apt_Hook___del___obj},
};
STATIC MP_DEFINE_CONST_DICT(mod_citrus_apt_Hook_locals_dict, mod_citrus_apt_Hook_locals_dict_table);

const mp_obj_type_t mod_citrus_apt_Hook_type = {
        {&mp_type_type},
        .name = MP_QSTR_Hook,
        .print = mod_citrus_apt_Hook_print,
        .make_new = mod_citrus_apt_Hook_make_new,
        .locals_dict = (mp_obj_t) &mod_citrus_apt_Hook_locals_dict,
        .attr = (mp_obj_t) &mod_citrus_apt_Hook_attr,
};