#include <3ds.h>

#include "py/runtime.h"

#define LOCAL_INT(__n, __v) \
    {MP_ROM_QSTR(MP_QSTR_##__n), MP_ROM_INT(__v)}

FS_MediaType _mod_citrus_fs_get_mediatype(mp_obj_t type) {
    if (mp_obj_is_integer(type)) {
        int _type = mp_obj_get_int(type);
        if (_type >= MEDIATYPE_NAND && _type <= MEDIATYPE_GAME_CARD) {
            return _type;
        }
    }

    nlr_raise(mp_obj_new_exception(&mp_type_ValueError));
}

STATIC const mp_rom_map_elem_t mp_module_citrus_fs_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_fs)},

        // Functions

        // FS_MediaType
        LOCAL_INT(MEDIATYPE_NAND, MEDIATYPE_NAND),
        LOCAL_INT(MEDIATYPE_SD, MEDIATYPE_SD),
        LOCAL_INT(MEDIATYPE_GAME_CARD, MEDIATYPE_GAME_CARD),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_fs_globals, mp_module_citrus_fs_globals_table);

const mp_obj_module_t mp_module_citrus_fs = {
        .base = {&mp_type_module},
        .name = MP_QSTR_fs,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_fs_globals,
};