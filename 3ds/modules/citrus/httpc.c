#include <3ds.h>

#include "py/runtime.h"

#include "../init_helper.h"

// 8kb
#define DEFAULT_SHAREDMEM_SIZE 8192

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_citrus_httpc_##__n##_obj, mod_citrus_httpc_##__n)
#define METHOD_OBJ_KW(__min, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_KW(mod_citrus_httpc_##__n##_obj, __min, mod_citrus_httpc_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_httpc_##__n##_obj}
#define LOCAL_INT(__n, __v) \
    {MP_ROM_QSTR(MP_QSTR_##__n), MP_ROM_INT(__v)}

extern const mp_obj_type_t mod_citrus_httpc_Request_type;

static int _mod_citrus_httpc_is_init = 0;

int _mod_citrus_httpc_get_request_method(int method) {
    if (method >= HTTPC_METHOD_GET && method <= HTTPC_METHOD_DELETE) {
        return method;
    }

    nlr_raise(mp_obj_new_exception(&mp_type_TypeError));
}

enum {
    INIT_ARG_SHAREDMEM_SIZE,
    INIT_ARG_COUNT
};

STATIC mp_obj_t mod_citrus_httpc_init(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    INIT_ONCE(_mod_citrus_httpc_is_init);

    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_sharedmem_size, MP_ARG_KW_ONLY | MP_ARG_INT, {.u_int = DEFAULT_SHAREDMEM_SIZE}},
    };

    mp_arg_val_t args[INIT_ARG_COUNT];
    mp_arg_parse_all((mp_uint_t) n_args, pos_args, kw_args, INIT_ARG_COUNT, allowed_args, args);

    return mp_obj_new_int(httpcInit(args[INIT_ARG_SHAREDMEM_SIZE].u_int));
}

mp_obj_t mod_citrus_httpc_exit(void) {
    EXIT_ONCE(_mod_citrus_httpc_is_init);

    httpcExit();

    return mp_const_none;
}

METHOD_OBJ_KW(0, init);
METHOD_OBJ_N(0, exit);

STATIC const mp_rom_map_elem_t mp_module_citrus_httpc_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_httpc)},

        // Classes
        {MP_ROM_QSTR(MP_QSTR_Request), MP_ROM_PTR(&mod_citrus_httpc_Request_type)},

        // Functions
        LOCAL_METHOD(init),
        LOCAL_METHOD(exit),

        // HTTPC_RequestMethod
        LOCAL_INT(METHOD_GET, HTTPC_METHOD_GET),
        LOCAL_INT(METHOD_POST, HTTPC_METHOD_POST),
        LOCAL_INT(METHOD_HEAD, HTTPC_METHOD_HEAD),
        LOCAL_INT(METHOD_PUT, HTTPC_METHOD_PUT),
        LOCAL_INT(METHOD_DELETE, HTTPC_METHOD_DELETE),

        // HTTPC_RequestStatus
        LOCAL_INT(STATUS_IN_PROGRESS, HTTPC_STATUS_REQUEST_IN_PROGRESS),
        LOCAL_INT(STATUS_DOWNLOAD_READY, HTTPC_STATUS_DOWNLOAD_READY),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_httpc_globals, mp_module_citrus_httpc_globals_table);

const mp_obj_module_t mp_module_citrus_httpc = {
        .base = {&mp_type_module},
        .name = MP_QSTR_httpc,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_httpc_globals,
};