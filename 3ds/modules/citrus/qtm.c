#include <3ds.h>

#include "py/runtime.h"

#include "../init_helper.h"

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_citrus_qtm_##__n##_obj, mod_citrus_qtm_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_qtm_##__n##_obj}

extern const mp_obj_type_t mod_citrus_qtm_HeadTracking_type;

static int _mod_citrus_qtm_is_init = 0;

STATIC mp_obj_t mod_citrus_qtm_init(void) {
    INIT_ONCE(_mod_citrus_qtm_is_init);

    return mp_obj_new_int(qtmInit());
}

mp_obj_t mod_citrus_qtm_exit(void) {
    EXIT_ONCE(_mod_citrus_qtm_is_init);

    qtmExit();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_qtm_check_initialized(void) {
    return mp_obj_new_bool(qtmCheckInitialized());
}

METHOD_OBJ_N(0, init);
METHOD_OBJ_N(0, exit);
METHOD_OBJ_N(0, check_initialized);

STATIC const mp_rom_map_elem_t mp_module_citrus_qtm_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_qtm)},

        // Classes
        {MP_ROM_QSTR(MP_QSTR_HeadTracking), MP_ROM_PTR(&mod_citrus_qtm_HeadTracking_type)},

        // Functions
        LOCAL_METHOD(init),
        LOCAL_METHOD(exit),
        LOCAL_METHOD(check_initialized),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_qtm_globals, mp_module_citrus_qtm_globals_table);

const mp_obj_module_t mp_module_citrus_qtm = {
        .base = {&mp_type_module},
        .name = MP_QSTR_qtm,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_qtm_globals,
};