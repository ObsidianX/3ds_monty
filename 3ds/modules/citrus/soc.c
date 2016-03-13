#include <3ds.h>

#include "py/runtime.h"

STATIC mp_obj_t mod_citrus_soc_init(mp_obj_t context_addr, mp_obj_t context_size) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_citrus_soc_exit(void) {
    socExit();

    return mp_const_none;
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