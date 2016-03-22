#include <malloc.h>

#include <3ds.h>

#include "py/runtime.h"

#include "../init_helper.h"

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_citrus_soc_##__n##_obj, mod_citrus_soc_##__n)
#define METHOD_OBJ_KW(__min, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_KW(mod_citrus_soc_##__n##_obj, __min, mod_citrus_soc_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_soc_##__n##_obj}

#define SOC_BUFFER_ALIGNMENT 0x1000
#define SOC_BUFFER_SIZE 0x100000

static void *_mod_citrus_soc_buffer = NULL;
static int _mod_citrus_soc_is_setup = 0;

bool _mod_citrus_soc_is_init(void) {
    return _mod_citrus_soc_buffer != NULL;
}

STATIC mp_obj_t mod_citrus_soc_init(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    INIT_ONCE(_mod_citrus_soc_is_setup);

    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_buffer_size, MP_ARG_INT, {.u_int = SOC_BUFFER_SIZE}},
    };

    mp_arg_val_t args[1];
    mp_arg_parse_all((mp_uint_t) n_args, pos_args, kw_args, 1, allowed_args, args);

    u32 size = args[0].u_int;

    _mod_citrus_soc_buffer = memalign(SOC_BUFFER_ALIGNMENT, size);
    Result res = socInit(_mod_citrus_soc_buffer, size);

    return mp_obj_new_int(res);
}

mp_obj_t mod_citrus_soc_exit(void) {
    EXIT_ONCE(_mod_citrus_soc_is_setup);

    Result res = socExit();
    free(_mod_citrus_soc_buffer);
    _mod_citrus_soc_buffer = NULL;

    return mp_obj_new_int(res);
}

STATIC mp_obj_t mod_citrus_soc_get_host_id(void) {
    return mp_obj_new_int_from_ll(gethostid());
}

METHOD_OBJ_KW(0, init);
METHOD_OBJ_N(0, exit);
METHOD_OBJ_N(0, get_host_id);

STATIC const mp_rom_map_elem_t mp_module_citrus_soc_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__),    MP_ROM_QSTR(MP_QSTR_soc)},

        // Functions
        LOCAL_METHOD(init),
        LOCAL_METHOD(exit),
        LOCAL_METHOD(get_host_id),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_soc_globals, mp_module_citrus_soc_globals_table);

const mp_obj_module_t mp_module_citrus_soc = {
        .base = {&mp_type_module},
        .name = MP_QSTR_soc,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_soc_globals,
};