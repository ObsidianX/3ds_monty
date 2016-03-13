#include <3ds.h>

#include "py/runtime.h"

// 8kb
#define DEFAULT_SHAREDMEM_SIZE 8192

int _mod_citrus_httpc_get_request_method(mp_obj_t method) {
    if (mp_obj_is_integer(method)) {
        int m = mp_obj_get_int(method);
        if (m >= HTTPC_METHOD_GET && m <= HTTPC_METHOD_DELETE) {
            return m;
        }
    }

    nlr_raise(mp_obj_new_exception(&mp_type_TypeError));
}

enum {
    INIT_ARG_SHAREDMEM_SIZE,
    INIT_ARG_COUNT
};

STATIC mp_obj_t mod_citrus_httpc_init(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_sharedmem_size, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = DEFAULT_SHAREDMEM_SIZE}},
    };

    mp_arg_val_t args[INIT_ARG_COUNT];
    mp_arg_parse_all((mp_uint_t) n_args, pos_args, kw_args, INIT_ARG_COUNT, allowed_args, args);

    return mp_obj_new_int(httpcInit(args[INIT_ARG_SHAREDMEM_SIZE].u_int));
}

STATIC mp_obj_t mod_citrus_httpc_exit(void) {
    httpcExit();

    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_KW(mod_citrus_httpc_init_obj, 0, mod_citrus_init);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_httpc_exit_obj, mod_citrus_httpc_exit);

STATIC const mp_rom_map_elem_t mp_module_citrus_httpc_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__),              MP_ROM_QSTR(MP_QSTR_httpc)},

        // Functions
        {MP_ROM_QSTR(MP_QSTR_init),                  MP_ROM_PTR(&mod_citrus_httpc_init_obj)},
        {MP_ROM_QSTR(MP_QSTR_exit),                  MP_ROM_PTR(&mod_citrus_httpc_exit_obj)},

        // HTTPC_RequestMethod
        {MP_ROM_QSTR(MP_QSTR_METHOD_GET),            MP_ROM_INT(HTTPC_METHOD_GET)},
        {MP_ROM_QSTR(MP_QSTR_METHOD_POST),           MP_ROM_INT(HTTPC_METHOD_POST)},
        {MP_ROM_QSTR(MP_QSTR_METHOD_HEAD),           MP_ROM_INT(HTTPC_METHOD_HEAD)},
        {MP_ROM_QSTR(MP_QSTR_METHOD_PUT),            MP_ROM_INT(HTTPC_METHOD_PUT)},
        {MP_ROM_QSTR(MP_QSTR_METHOD_DELETE),         MP_ROM_INT(HTTPC_METHOD_DELETE)},

        // HTTPC_RequestStatus
        {MP_ROM_QSTR(MP_QSTR_STATUS_IN_PROGRESS),    MP_ROM_INT(HTTPC_STATUS_REQUEST_IN_PROGRESS)},
        {MP_ROM_QSTR(MP_QSTR_STATUS_DOWNLOAD_READY), MP_ROM_INT(HTTPC_STATUS_DOWNLOAD_READY)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_httpc_globals, mp_module_citrus_httpc_globals_table);

const mp_obj_module_t mp_module_citrus_httpc = {
        .base = {&mp_type_module},
        .name = MP_QSTR_httpc,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_httpc_globals,
};