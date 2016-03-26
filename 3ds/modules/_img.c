#include "py/runtime.h"

extern const mp_obj_type_t mod__img__PngLoader_type;
extern const mp_obj_type_t mod__img__JpegLoader_type;

STATIC const mp_rom_map_elem_t mp_module_img_globals_table[] = {
        {MP_ROM_QSTR(MP_QSTR__PngLoader),  MP_ROM_PTR(&mod__img__PngLoader_type)},
        {MP_ROM_QSTR(MP_QSTR__JpegLoader), MP_ROM_PTR(&mod__img__JpegLoader_type)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_img_globals, mp_module_img_globals_table);

const mp_obj_module_t mp_module_img = {
        .base = {&mp_type_module},
        .name = MP_QSTR__img,
        .globals = (mp_obj_dict_t *) &mp_module_img_globals,
};