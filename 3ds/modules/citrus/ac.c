#include <3ds.h>

#include "py/runtime.h"

#include "../init_helper.h"

#define METHOD_OBJ(__n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_ac_##__n##_obj, mod_citrus_ac_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_ac_##__n##_obj}

static int _mod_citrus_ac_is_init = 0;

STATIC mp_obj_t mod_citrus_ac_init(void) {
    INIT_ONCE(_mod_citrus_ac_is_init);

    return mp_obj_new_int(acInit());
}

mp_obj_t mod_citrus_ac_exit(void) {
    EXIT_ONCE(_mod_citrus_ac_is_init);

    acExit();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ac_wait_for_internet(void) {
    return mp_obj_new_int(acWaitInternetConnection());
}

STATIC mp_obj_t mod_citrus_ac_get_wifi_status(void) {
    u32 status;
    ACU_GetWifiStatus(&status);

    return mp_obj_new_int(status);
}

METHOD_OBJ(init);
METHOD_OBJ(exit);
METHOD_OBJ(wait_for_internet);
METHOD_OBJ(get_wifi_status);

STATIC const mp_rom_map_elem_t mp_module_citrus_ac_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ac)},

        // Methods
        LOCAL_METHOD(init),
        LOCAL_METHOD(exit),
        LOCAL_METHOD(wait_for_internet),
        LOCAL_METHOD(get_wifi_status),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_ac_globals, mp_module_citrus_ac_globals_table);

const mp_obj_module_t mp_module_citrus_ac = {
        .base = {&mp_type_module},
        .name = MP_QSTR_ac,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_ac_globals,
};