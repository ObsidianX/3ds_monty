#include <sftd.h>

#include "py/runtime.h"

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_sftd_##__n##_obj, mod_sftd_##__n)

#define LOCAL_CLASS(__n) \
    {MP_ROM_QSTR(MP_QSTR_##__n), MP_ROM_PTR(&mod_sftd_##__n##_type)}
#define LOCAL_METHOD(__n) \
    {MP_ROM_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_sftd_##__n##_obj}

extern const mp_obj_type_t mod_sftd_Font_type;

STATIC mp_obj_t mod_sftd_init(void) {
    sftd_init();

    return mp_const_none;
}

STATIC mp_obj_t mod_sftd_fini(void) {
    sftd_fini();

    return mp_const_none;
}

METHOD_OBJ_N(0, init);
METHOD_OBJ_N(0, fini);

STATIC const mp_rom_map_elem_t mp_module_sftd_globals_table[] = {
        // Methods
        LOCAL_METHOD(init),
        LOCAL_METHOD(fini),

        // Classes
        LOCAL_CLASS(Font),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_sftd_globals, mp_module_sftd_globals_table);

const mp_obj_module_t mp_module_sftd = {
        .base = {&mp_type_module},
        .name = MP_QSTR_sftd,
        .globals = (mp_obj_dict_t *) &mp_module_sftd_globals,
};