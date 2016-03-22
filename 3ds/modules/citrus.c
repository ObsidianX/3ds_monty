#include "py/runtime.h"

#include "citrus/exits.h"

#define IMPORT_SUBMODULE(__n) \
    extern const mp_obj_module_t mp_module_citrus_##__n;

#define SUBMODULE(__n) \
    {MP_ROM_QSTR(MP_QSTR_##__n), MP_ROM_PTR(&mp_module_citrus_##__n)}

// submodules
IMPORT_SUBMODULE(apt);
IMPORT_SUBMODULE(console);
IMPORT_SUBMODULE(fs);
IMPORT_SUBMODULE(gfx);
IMPORT_SUBMODULE(gpu);
IMPORT_SUBMODULE(gsp);
IMPORT_SUBMODULE(hid);
IMPORT_SUBMODULE(httpc);
IMPORT_SUBMODULE(ndsp);
IMPORT_SUBMODULE(news);
IMPORT_SUBMODULE(ns);
IMPORT_SUBMODULE(os);
IMPORT_SUBMODULE(romfs);
IMPORT_SUBMODULE(soc);

void mod_citrus_exit_all(void) {
    mod_citrus_apt_exit();
    mod_citrus_gfx_exit();
    mod_citrus_gsp_exit();
    mod_citrus_hid_exit();
    mod_citrus_httpc_exit();
    mod_citrus_ndsp_exit();
    mod_citrus_news_exit();
    mod_citrus_ns_exit();
    mod_citrus_romfs_exit();
    mod_citrus_soc_exit();
}

STATIC const mp_rom_map_elem_t mp_module_citrus_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_citrus)},

        // Sub-modules
        SUBMODULE(apt),
        SUBMODULE(console),
        SUBMODULE(fs),
        SUBMODULE(gfx),
        SUBMODULE(gpu),
        SUBMODULE(gsp),
        SUBMODULE(hid),
        SUBMODULE(httpc),
        SUBMODULE(ndsp),
        SUBMODULE(news),
        SUBMODULE(ns),
        SUBMODULE(os),
        SUBMODULE(romfs),
        SUBMODULE(soc),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_globals, mp_module_citrus_globals_table);

const mp_obj_module_t mp_module_citrus = {
        .base = {&mp_type_module},
        .name = MP_QSTR_citrus,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_globals,
};
