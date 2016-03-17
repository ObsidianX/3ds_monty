#include <malloc.h>

#include <3ds.h>

#include "py/runtime.h"

#define SOC_BUFFER_ALIGNMENT 0x1000
#define SOC_BUFFER_SIZE 0x100000

static void *_mod_citrus_soc_buffer = NULL;

STATIC mp_obj_t mod_citrus_soc_init(size_t n_args, const mp_obj_t *args_in) {
    // don't init twice...
    if (_mod_citrus_soc_buffer != NULL) {
        return mp_const_none;
    }

    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_buffer_size, MP_ARG_INT, {.u_int = SOC_BUFFER_SIZE}},
    };

    mp_arg_val_t args[1];
    mp_arg_parse_all_kw_array(n_args, n_kw, args_in, 1, allowed_args, args);

    u32 size = args[0].u_int;

    _mod_citrus_soc_buffer = memalign(SOC_BUFFER_ALIGNMENT, size);
    Result res = socInit(_mod_citrus_soc_buffer, size);

    return mp_obj_new_int(res);
}

STATIC mp_obj_t mod_citrus_soc_exit(void) {
    Result res = socExit();
    free(_mod_citrus_soc_buffer);

    return mp_obj_new_int(res);
}

STATIC mp_obj_t mod_citrus_soc_get_host_id(void) {
    return mp_obj_new_int_from_ll(gethostid());
}

STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_citrus_soc_init_obj, mod_citrus_soc_init);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_soc_exit_obj, mod_citrus_soc_exit);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_soc_get_host_id_obj, mod_citrus_soc_get_host_id);

STATIC const mp_rom_map_elem_t mp_module_citrus_soc_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__),    MP_ROM_QSTR(MP_QSTR_soc)},

        // Functions
        {MP_ROM_QSTR(MP_QSTR_init),        MP_ROM_PTR(&mod_citrus_soc_init_obj)},
        {MP_ROM_QSTR(MP_QSTR_exit),        MP_ROM_PTR(&mod_citrus_soc_exit_obj)},
        {MP_ROM_QSTR(MP_QSTR_get_host_id), MP_ROM_PTR(&mod_citrus_soc_get_host_id_obj)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_soc_globals, mp_module_citrus_soc_globals_table);

const mp_obj_module_t mp_module_citrus_soc = {
        .base = {&mp_type_module},
        .name = MP_QSTR_soc,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_soc_globals,
};