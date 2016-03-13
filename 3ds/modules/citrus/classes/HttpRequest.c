#include <3ds.h>

#include "py/runtime.h"

#include "../helpers.h"

#define SELF(src) mod_citrus_httpc_Request_t *self = src;

const mp_obj_type_t mod_citrus_httpc_Request_type;

typedef struct {
    mp_obj_base_t base;

    httpcContext context;
} mod_citrus_httpc_Request_t;

enum {
    NEW_ARG_URL = 0,
    NEW_ARG_METHOD,
    NEW_ARG_USE_PROXY,
    NEW_ARG_COUNT
};

STATIC mp_obj_t mod_citrus_httpc_Request_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in) {
    mp_arg_check_num(n_args, n_kw, 1, NEW_ARG_COUNT, true);

    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_url,       MP_ARG_OBJ | MP_ARG_REQUIRED, {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_method,    MP_ARG_INT,                   {.u_int = HTTPC_METHOD_GET}},
            {MP_QSTR_use_proxy, MP_ARG_BOOL,                  {.u_bool = false}},
    };

    mp_arg_val_t args[NEW_ARG_COUNT];
    mp_arg_parse_all_kw_array(n_args, n_kw, args_in, NEW_ARG_COUNT, allowed_args, args);

    mod_citrus_httpc_Request_t *obj = m_new_obj(mod_citrus_httpc_Request_t);
    obj->base.type = (mp_obj_t) &mod_citrus_httpc_Request_type;

    if (!MP_OBJ_IS_STR(args[NEW_ARG_URL].u_obj)) {
        nlr_raise(mp_obj_new_exception(&mp_type_TypeError));
    }

    const char *url = mp_obj_str_get_str(args[NEW_ARG_URL]);
    int method = _mod_citrus_httpc_get_request_method(args[NEW_ARG_METHOD].u_int);
    bool proxy = args[NEW_ARG_USE_PROXY].u_bool;

    httpcOpenContext(&obj->context, method, url, proxy ? 1 : 0);

    return obj;
}

STATIC void mod_citrus_httpc_Request_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    SELF(self_in);
    const char *name = qstr_str(attr);
    bool load = dest[0] == MP_OBJ_NULL;

    // attributes are read-only
    if (!load) {
        return;
    }

    // Methods
    /*if (!strcmp(name, qstr_str(MP_QSTR_))) {
        dest[0] = (mp_obj_t) &mod_citrus_httpc_Request__obj;
    }*/
}

STATIC void mod_citrus_httpc_Request_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    SELF(self_in);

    mp_printf(print, "<Request context=%p>", &self->context);
}

STATIC const mp_map_elem_t mod_citrus_httpc_Request_locals_dict_table[] = {
        // Methods
        //{MP_OBJ_NEW_QSTR(MP_QSTR_), (mp_obj_t) &mod_citrus_httpc_Request_},
};
STATIC MP_DEFINE_CONST_DICT(mod_citrus_httpc_Request_locals_dict, mod_citrus_httpc_Request_locals_dict_table);

const mp_obj_type_t mod_citrus_httpc_Request_type = {
        {&mp_type_type},
        .name = MP_QSTR_PrintConsole,
        .print = mod_citrus_httpc_Request_print,
        .make_new = mod_citrus_httpc_Request_make_new,
        .locals_dict = (mp_obj_t) &mod_citrus_httpc_Request_locals_dict,
        .attr = (mp_obj_t) &mod_citrus_httpc_Request_attr,
};