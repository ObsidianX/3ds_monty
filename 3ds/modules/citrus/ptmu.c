#include <3ds.h>

#include "py/runtime.h"

#include "../init_helper.h"

#define METHOD_OBJ(__n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_ptmu_##__n##_obj, mod_citrus_ptmu_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_ptmu_##__n##_obj}

static int _mod_citrus_ptmu_is_init = 0;

STATIC mp_obj_t mod_citrus_ptmu_init(void) {
    INIT_ONCE(_mod_citrus_ptmu_is_init);

    return mp_obj_new_int(ptmuInit());
}

mp_obj_t mod_citrus_ptmu_exit(void) {
    EXIT_ONCE(_mod_citrus_ptmu_is_init);

    ptmuExit();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ptmu_is_shell_closed(void) {
    u8 ret;
    PTMU_GetShellState(&ret);

    return mp_obj_new_bool(ret);
}

STATIC mp_obj_t mod_citrus_ptmu_get_battery_level(void) {
    u8 ret;
    PTMU_GetBatteryLevel(&ret);

    return mp_obj_new_int(ret);
}

STATIC mp_obj_t mod_citrus_ptmu_is_charging(void) {
    u8 ret;
    PTMU_GetBatteryChargeState(&ret);

    return mp_obj_new_bool(ret);
}

STATIC mp_obj_t mod_citrus_ptmu_is_pedometer_counting(void) {
    u8 ret;
    PTMU_GetPedometerState(&ret);

    return mp_obj_new_bool(ret);
}

STATIC mp_obj_t mod_citrus_ptmu_get_step_count(void) {
    u32 ret;
    PTMU_GetTotalStepCount(&ret);

    return mp_obj_new_int(ret);
}

METHOD_OBJ(init);
METHOD_OBJ(exit);
METHOD_OBJ(is_shell_closed);
METHOD_OBJ(get_battery_level);
METHOD_OBJ(is_charging);
METHOD_OBJ(is_pedometer_counting);
METHOD_OBJ(get_step_count);

STATIC const mp_rom_map_elem_t mp_module_citrus_ptmu_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ptmu)},

        // Functions
        LOCAL_METHOD(init),
        LOCAL_METHOD(exit),
        LOCAL_METHOD(is_shell_closed),
        LOCAL_METHOD(get_battery_level),
        LOCAL_METHOD(is_charging),
        LOCAL_METHOD(is_pedometer_counting),
        LOCAL_METHOD(get_step_count),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_ptmu_globals, mp_module_citrus_ptmu_globals_table);

const mp_obj_module_t mp_module_citrus_ptmu = {
        .base = {&mp_type_module},
        .name = MP_QSTR_ptmu,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_ptmu_globals,
};