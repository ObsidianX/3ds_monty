#include <3ds.h>

#include "py/runtime.h"

#include "../init_helper.h"

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_citrus_ptmsysm_##__n##_obj, mod_citrus_ptmsysm_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_ptmsysm_##__n##_obj}

static int _mod_citrus_ptmsysm_is_init = 0;

STATIC mp_obj_t mod_citrus_ptmsysm_init(void) {
    INIT_ONCE(_mod_citrus_ptmsysm_is_init);

    return mp_obj_new_int(ptmSysmInit());
}

mp_obj_t mod_citrus_ptmsysm_exit(void) {
    EXIT_ONCE(_mod_citrus_ptmsysm_is_init);

    ptmSysmExit();

    return mp_const_none;
}

mp_obj_t mod_citrus_ptmsysm_configure_new3ds_cpu(mp_obj_t enable_highspeed, mp_obj_t enable_cache) {
    int cpu = mp_obj_is_true(enable_highspeed) ? 1 : 0;
    int l2 = mp_obj_is_true(enable_cache) ? (1 << 1) : 0;

    u8 val = cpu | l2;

    return mp_obj_new_int(PTMSYSM_ConfigureNew3DSCPU(val));
}

METHOD_OBJ_N(0, init);
METHOD_OBJ_N(0, exit);
METHOD_OBJ_N(2, configure_new3ds_cpu);

STATIC const mp_rom_map_elem_t mp_module_citrus_ptmsysm_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ptmsysm)},

        // Functions
        LOCAL_METHOD(init),
        LOCAL_METHOD(exit),
        LOCAL_METHOD(configure_new3ds_cpu),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_ptmsysm_globals, mp_module_citrus_ptmsysm_globals_table);

const mp_obj_module_t mp_module_citrus_ptmsysm = {
        .base = {&mp_type_module},
        .name = MP_QSTR_ptmsysm,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_ptmsysm_globals,
};