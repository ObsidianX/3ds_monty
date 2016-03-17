#include <3ds.h>

#include "py/runtime.h"

GPU_TEXUNIT _mod_citrus_gpu_get_texunit(mp_obj_t unit) {
    if (mp_obj_is_integer(unit)) {
        int _unit = mp_obj_get_int(unit);
        if (_unit >= GPU_TEXUNIT0 && _unit <= GPU_TEXUNIT2) {
            return _unit;
        }
    }

    nlr_raise(mp_obj_new_exception(&mp_type_ValueError));
}

STATIC const mp_rom_map_elem_t mp_module_citrus_gpu_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__),  MP_ROM_QSTR(MP_QSTR_gpu)},

        // Functions

        // GPU_TEXUNIT
        {MP_ROM_QSTR(MP_QSTR_TEXUNIT_0), MP_ROM_INT(GPU_TEXUNIT0)},
        {MP_ROM_QSTR(MP_QSTR_TEXUNIT_1), MP_ROM_INT(GPU_TEXUNIT1)},
        {MP_ROM_QSTR(MP_QSTR_TEXUNIT_2), MP_ROM_INT(GPU_TEXUNIT2)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_gpu_globals, mp_module_citrus_gpu_globals_table);

const mp_obj_module_t mp_module_citrus_gpu = {
        .base = {&mp_type_module},
        .name = MP_QSTR_gpu,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_gpu_globals,
};