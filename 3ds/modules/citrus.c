#include "py/runtime.h"

// submodules
extern const mp_obj_module_t mp_module_citrus_apt;
extern const mp_obj_module_t mp_module_citrus_console;
extern const mp_obj_module_t mp_module_citrus_fs;
extern const mp_obj_module_t mp_module_citrus_gfx;
extern const mp_obj_module_t mp_module_citrus_gpu;
extern const mp_obj_module_t mp_module_citrus_gsp;
extern const mp_obj_module_t mp_module_citrus_hid;
extern const mp_obj_module_t mp_module_citrus_httpc;
extern const mp_obj_module_t mp_module_citrus_news;
extern const mp_obj_module_t mp_module_citrus_ns;
extern const mp_obj_module_t mp_module_citrus_os;
extern const mp_obj_module_t mp_module_citrus_romfs;
extern const mp_obj_module_t mp_module_citrus_soc;

STATIC const mp_rom_map_elem_t mp_module_citrus_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_citrus)},

        // Sub-modules
        {MP_ROM_QSTR(MP_QSTR_apt),      MP_ROM_PTR(&mp_module_citrus_apt)},
        {MP_ROM_QSTR(MP_QSTR_console),  MP_ROM_PTR(&mp_module_citrus_console)},
        {MP_ROM_QSTR(MP_QSTR_fs),       MP_ROM_PTR(&mp_module_citrus_fs)},
        {MP_ROM_QSTR(MP_QSTR_gfx),      MP_ROM_PTR(&mp_module_citrus_gfx)},
        {MP_ROM_QSTR(MP_QSTR_gpu),      MP_ROM_PTR(&mp_module_citrus_gpu)},
        {MP_ROM_QSTR(MP_QSTR_gsp),      MP_ROM_PTR(&mp_module_citrus_gsp)},
        {MP_ROM_QSTR(MP_QSTR_hid),      MP_ROM_PTR(&mp_module_citrus_hid)},
        {MP_ROM_QSTR(MP_QSTR_httpc),    MP_ROM_PTR(&mp_module_citrus_httpc)},
        {MP_ROM_QSTR(MP_QSTR_news),     MP_ROM_PTR(&mp_module_citrus_news)},
        {MP_ROM_QSTR(MP_QSTR_news),     MP_ROM_PTR(&mp_module_citrus_ns)},
        {MP_ROM_QSTR(MP_QSTR_os),       MP_ROM_PTR(&mp_module_citrus_os)},
        {MP_ROM_QSTR(MP_QSTR_romfs),    MP_ROM_PTR(&mp_module_citrus_romfs)},
        {MP_ROM_QSTR(MP_QSTR_soc),      MP_ROM_PTR(&mp_module_citrus_soc)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_globals, mp_module_citrus_globals_table);

const mp_obj_module_t mp_module_citrus = {
        .base = {&mp_type_module},
        .name = MP_QSTR_citrus,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_globals,
};
