#include <3ds.h>

#include "py/runtime.h"

STATIC mp_obj_t mod_citrus_romfs_init(void) {
    romfsInit();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_romfs_exit(void) {
    romfsExit();

    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_romfs_init_obj, mod_citrus_romfs_init);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_romfs_exit_obj, mod_citrus_romfs_exit);

STATIC const mp_rom_map_elem_t mp_module_citrus_romfs_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_romfs)},

        // Functions
        {MP_ROM_QSTR(MP_QSTR_init),     MP_ROM_PTR(&mod_citrus_romfs_init_obj)},
        {MP_ROM_QSTR(MP_QSTR_exit),     MP_ROM_PTR(&mod_citrus_romfs_exit_obj)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_romfs_globals, mp_module_citrus_romfs_globals_table);

const mp_obj_module_t mp_module_citrus_romfs = {
        .base = {&mp_type_module},
        .name = MP_QSTR_romfs,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_romfs_globals,
};