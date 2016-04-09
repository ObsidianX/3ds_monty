#include <string.h>

#include <3ds.h>

#include "py/runtime.h"

STATIC mp_obj_t mod_time_time(void) {
    double time = osGetTime() / 1000;

    return mp_obj_new_float(time);
}

MP_DEFINE_CONST_FUN_OBJ_0(mod_time_time_obj, mod_time_time);

STATIC const mp_rom_map_elem_t mp_module_time_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_time)},

        // Functions
        {MP_ROM_QSTR(MP_QSTR_time), MP_ROM_PTR(&mod_time_time_obj)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_time_globals, mp_module_time_globals_table);

const mp_obj_module_t mp_module_time = {
        .base = {&mp_type_module},
        .name = MP_QSTR_time,
        .globals = (mp_obj_dict_t *) &mp_module_time_globals,
};