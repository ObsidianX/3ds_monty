#include <3ds.h>

#include "py/runtime.h"

extern const mp_obj_type_t mod_citrus_csnd_Sound_type;
extern const mp_obj_type_t mod_citrus_csnd_Channel_type;

STATIC mp_obj_t mod_citrus_csnd_init(void) {
    return mp_obj_new_int(csndInit());
}

STATIC mp_obj_t mod_citrus_csnd_exit(void) {
    csndExit();

    return mp_const_none;
}

enum {
    PLAY_ARG_CHANNEL = 0,
    PLAY_ARG_FLAGS,
    PLAY_ARG_SAMPLE_RATE,
    PLAY_ARG_VOLUME,
    PLAY_ARG_PAN,
    PLAY_ARG_SOUND
};

STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_csnd_init_obj, mod_citrus_csnd_init);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_csnd_exit_obj, mod_citrus_csnd_exit);

STATIC const mp_rom_map_elem_t mp_module_citrus_csnd_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_csnd)},

        // Classes
        {MP_ROM_QSTR(MP_QSTR_Sound),    MP_ROM_PTR(&mod_citrus_csnd_Sound_type)},
        {MP_ROM_QSTR(MP_QSTR_Channel),  MP_ROM_PTR(&mod_citrus_csnd_Channel_type)},

        // Functions
        {MP_ROM_QSTR(MP_QSTR_init),     MP_ROM_PTR(&mod_citrus_csnd_init_obj)},
        {MP_ROM_QSTR(MP_QSTR_exit),     MP_ROM_PTR(&mod_citrus_csnd_exit_obj)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_csnd_globals, mp_module_citrus_csnd_globals_table);

const mp_obj_module_t mp_module_citrus_csnd = {
        .base = {&mp_type_module},
        .name = MP_QSTR_csnd,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_csnd_globals,
};