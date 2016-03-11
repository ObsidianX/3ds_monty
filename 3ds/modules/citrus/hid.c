#include <3ds.h>

#include "py/runtime.h"

STATIC mp_obj_t mod_citrus_hid_init(void) {
    return mp_obj_new_int(hidInit());
}

STATIC mp_obj_t mod_citrus_hid_exit(void) {
    hidExit();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_hid_scan_input(void) {
    hidScanInput();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_hid_keys_held(void) {
    return mp_obj_new_int(hidKeysHeld());
}

STATIC mp_obj_t mod_citrus_hid_keys_down(void) {
    return mp_obj_new_int(hidKeysDown());
}

STATIC mp_obj_t mod_citrus_hid_keys_up(void) {
    return mp_obj_new_int(hidKeysUp());
}

STATIC mp_obj_t mod_citrus_hid_touch_read(void) {
    touchPosition tp;
    hidTouchRead(&tp);

    mp_obj_tuple_t *ret = mp_obj_new_tuple(2, NULL);
    ret->items[0] = mp_obj_new_int(tp.px);
    ret->items[1] = mp_obj_new_int(tp.py);

    return ret;
}

STATIC mp_obj_t mod_citrus_hid_circle_read(void) {
    circlePosition cp;
    hidCircleRead(&cp);

    mp_obj_tuple_t *ret = mp_obj_new_tuple(2, NULL);
    ret->items[0] = mp_obj_new_int(cp.dx);
    ret->items[1] = mp_obj_new_int(cp.dy);

    return ret;
}

STATIC mp_obj_t mod_citrus_hid_accel_read(void) {
    accelVector av;
    hidAccelRead(&av);

    mp_obj_tuple_t *ret = mp_obj_new_tuple(3, NULL);
    ret->items[0] = mp_obj_new_int(av.x);
    ret->items[1] = mp_obj_new_int(av.y);
    ret->items[2] = mp_obj_new_int(av.z);

    return ret;
}

STATIC mp_obj_t mod_citrus_hid_gyro_read(void) {
    angularRate ar;
    hidGyroRead(&ar);

    mp_obj_tuple_t *ret = mp_obj_new_tuple(3, NULL);
    ret->items[0] = mp_obj_new_int(ar.x);
    ret->items[1] = mp_obj_new_int(ar.y);
    ret->items[2] = mp_obj_new_int(ar.z);

    return ret;
}

STATIC mp_obj_t mod_citrus_hid_wait_for_event(mp_obj_t id, mp_obj_t next_event) {
    int _id = mp_obj_get_int(id);
    hidWaitForEvent(_id, mp_obj_is_true(next_event));

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_hid_user_get_handles(void) {
    Handle out_mem, pad0, pad1, accel, gyro, debug_pad;
    Result re = HIDUSER_GetHandles(&out_mem, &pad0, &pad1, &accel, &gyro, &debug_pad);

    mp_obj_tuple_t *ret = mp_obj_new_tuple(7, NULL);
    ret->items[0] = mp_obj_new_int(re);
    ret->items[1] = mp_obj_new_int(out_mem);
    ret->items[2] = mp_obj_new_int(pad0);
    ret->items[3] = mp_obj_new_int(pad1);
    ret->items[4] = mp_obj_new_int(accel);
    ret->items[5] = mp_obj_new_int(gyro);
    ret->items[6] = mp_obj_new_int(debug_pad);

    return ret;
}

STATIC mp_obj_t mod_citrus_hid_user_enable_accelerometer(void) {
    return mp_obj_new_int(HIDUSER_EnableAccelerometer());
}

STATIC mp_obj_t mod_citrus_hid_user_disable_accelerometer(void) {
    return mp_obj_new_int(HIDUSER_DisableAccelerometer());
}

STATIC mp_obj_t mod_citrus_hid_user_enable_gyroscope(void) {
    return mp_obj_new_int(HIDUSER_EnableGyroscope());
}

STATIC mp_obj_t mod_citrus_hid_user_disable_gyroscope(void) {
    return mp_obj_new_int(HIDUSER_DisableGyroscope());
}

STATIC mp_obj_t mod_citrus_hid_user_get_gyroscope_raw_to_dps_coefficient(void) {
    float coeff = 0;
    Result re = HIDUSER_GetGyroscopeRawToDpsCoefficient(&coeff);

    mp_obj_tuple_t *ret = mp_obj_new_tuple(2, NULL);
    ret->items[0] = mp_obj_new_int(re);
    ret->items[1] = mp_obj_new_float(coeff);

    return ret;
}

STATIC mp_obj_t mod_citrus_hid_user_get_sound_volume(void) {
    u8 vol = 0;
    Result re = HIDUSER_GetSoundVolume(&vol);

    mp_obj_tuple_t *ret = mp_obj_new_tuple(2, NULL);
    ret->items[0] = mp_obj_new_int(re);
    ret->items[1] = mp_obj_new_int(vol);

    return ret;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_hid_init_obj, mod_citrus_hid_init);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_hid_exit_obj, mod_citrus_hid_exit);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_hid_scan_input_obj, mod_citrus_hid_scan_input);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_hid_keys_held_obj, mod_citrus_hid_keys_held);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_hid_keys_down_obj, mod_citrus_hid_keys_down);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_hid_keys_up_obj, mod_citrus_hid_keys_up);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_hid_touch_read_obj, mod_citrus_hid_touch_read);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_hid_circle_read_obj, mod_citrus_hid_circle_read);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_hid_accel_read_obj, mod_citrus_hid_accel_read);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_hid_gyro_read_obj, mod_citrus_hid_gyro_read);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_citrus_hid_wait_for_event_obj, mod_citrus_hid_wait_for_event);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_hid_user_get_handles_obj, mod_citrus_hid_user_get_handles);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_hid_user_enable_accelerometer_obj, mod_citrus_hid_user_enable_accelerometer);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_hid_user_disable_accelerometer_obj, mod_citrus_hid_user_disable_accelerometer);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_hid_user_enable_gyroscope_obj, mod_citrus_hid_user_enable_gyroscope);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_hid_user_disable_gyroscope_obj, mod_citrus_hid_user_disable_gyroscope);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_hid_user_get_gyroscope_raw_to_dps_coefficient_obj, mod_citrus_hid_user_get_gyroscope_raw_to_dps_coefficient);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_hid_user_get_sound_volume_obj, mod_citrus_hid_user_get_sound_volume);

STATIC const mp_rom_map_elem_t mp_module_citrus_hid_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__),                                  MP_ROM_QSTR(MP_QSTR_hid)},

        // Functions
        {MP_ROM_QSTR(MP_QSTR_init),                                      MP_ROM_PTR(&mod_citrus_hid_init_obj)},
        {MP_ROM_QSTR(MP_QSTR_exit),                                      MP_ROM_PTR(&mod_citrus_hid_exit_obj)},
        {MP_ROM_QSTR(MP_QSTR_scan_input),                                MP_ROM_PTR(&mod_citrus_hid_scan_input_obj)},
        {MP_ROM_QSTR(MP_QSTR_keys_held),                                 MP_ROM_PTR(&mod_citrus_hid_keys_held_obj)},
        {MP_ROM_QSTR(MP_QSTR_keys_down),                                 MP_ROM_PTR(&mod_citrus_hid_keys_down_obj)},
        {MP_ROM_QSTR(MP_QSTR_keys_up),                                   MP_ROM_PTR(&mod_citrus_hid_keys_up_obj)},
        {MP_ROM_QSTR(MP_QSTR_touch_read),                                MP_ROM_PTR(&mod_citrus_hid_touch_read_obj)},
        {MP_ROM_QSTR(MP_QSTR_circle_read),                               MP_ROM_PTR(&mod_citrus_hid_circle_read_obj)},
        {MP_ROM_QSTR(MP_QSTR_accel_read),                                MP_ROM_PTR(&mod_citrus_hid_accel_read_obj)},
        {MP_ROM_QSTR(MP_QSTR_gyro_read),                                 MP_ROM_PTR(&mod_citrus_hid_gyro_read_obj)},
        {MP_ROM_QSTR(MP_QSTR_wait_for_event),                            MP_ROM_PTR(&mod_citrus_hid_wait_for_event_obj)},
        {MP_ROM_QSTR(MP_QSTR_user_get_handles),                          MP_ROM_PTR(&mod_citrus_hid_user_get_handles_obj)},
        {MP_ROM_QSTR(MP_QSTR_user_enable_accelerometer),                 MP_ROM_PTR(&mod_citrus_hid_user_enable_accelerometer_obj)},
        {MP_ROM_QSTR(MP_QSTR_user_disable_accelerometer),                MP_ROM_PTR(&mod_citrus_hid_user_disable_accelerometer_obj)},
        {MP_ROM_QSTR(MP_QSTR_user_enable_gyroscope),                     MP_ROM_PTR(&mod_citrus_hid_user_enable_gyroscope_obj)},
        {MP_ROM_QSTR(MP_QSTR_user_disable_gyroscope),                    MP_ROM_PTR(&mod_citrus_hid_user_disable_gyroscope_obj)},
        {MP_ROM_QSTR(MP_QSTR_user_get_gyroscope_raw_to_dps_coefficient), MP_ROM_PTR(&mod_citrus_hid_user_get_gyroscope_raw_to_dps_coefficient_obj)},
        {MP_ROM_QSTR(MP_QSTR_user_get_sound_volume),                     MP_ROM_PTR(&mod_citrus_hid_user_get_sound_volume_obj)},

        // Keys
        {MP_ROM_QSTR(MP_QSTR_KEY_A),                                     MP_ROM_INT(KEY_A)},
        {MP_ROM_QSTR(MP_QSTR_KEY_B),                                     MP_ROM_INT(KEY_B)},
        {MP_ROM_QSTR(MP_QSTR_KEY_SELECT),                                MP_ROM_INT(KEY_SELECT)},
        {MP_ROM_QSTR(MP_QSTR_KEY_START),                                 MP_ROM_INT(KEY_START)},
        {MP_ROM_QSTR(MP_QSTR_KEY_DPAD_RIGHT),                            MP_ROM_INT(KEY_DRIGHT)},
        {MP_ROM_QSTR(MP_QSTR_KEY_DPAD_LEFT),                             MP_ROM_INT(KEY_DLEFT)},
        {MP_ROM_QSTR(MP_QSTR_KEY_DPAD_UP),                               MP_ROM_INT(KEY_DUP)},
        {MP_ROM_QSTR(MP_QSTR_KEY_DPAD_DOWN),                             MP_ROM_INT(KEY_DDOWN)},
        {MP_ROM_QSTR(MP_QSTR_KEY_R),                                     MP_ROM_INT(KEY_R)},
        {MP_ROM_QSTR(MP_QSTR_KEY_L),                                     MP_ROM_INT(KEY_L)},
        {MP_ROM_QSTR(MP_QSTR_KEY_X),                                     MP_ROM_INT(KEY_X)},
        {MP_ROM_QSTR(MP_QSTR_KEY_Y),                                     MP_ROM_INT(KEY_Y)},
        {MP_ROM_QSTR(MP_QSTR_KEY_ZL),                                    MP_ROM_INT(KEY_ZL)},
        {MP_ROM_QSTR(MP_QSTR_KEY_ZR),                                    MP_ROM_INT(KEY_ZR)},
        {MP_ROM_QSTR(MP_QSTR_KEY_TOUCH),                                 MP_ROM_INT(KEY_TOUCH)},
        {MP_ROM_QSTR(MP_QSTR_KEY_CSTICK_RIGHT),                          MP_ROM_INT(KEY_CSTICK_RIGHT)},
        {MP_ROM_QSTR(MP_QSTR_KEY_CSTICK_LEFT),                           MP_ROM_INT(KEY_CSTICK_LEFT)},
        {MP_ROM_QSTR(MP_QSTR_KEY_CSTICK_UP),                             MP_ROM_INT(KEY_CSTICK_UP)},
        {MP_ROM_QSTR(MP_QSTR_KEY_CSTICK_DOWN),                           MP_ROM_INT(KEY_CSTICK_DOWN)},
        {MP_ROM_QSTR(MP_QSTR_KEY_CPAD_RIGHT),                            MP_ROM_INT(KEY_CPAD_RIGHT)},
        {MP_ROM_QSTR(MP_QSTR_KEY_CPAD_LEFT),                             MP_ROM_INT(KEY_CPAD_LEFT)},
        {MP_ROM_QSTR(MP_QSTR_KEY_CPAD_UP),                               MP_ROM_INT(KEY_CPAD_UP)},
        {MP_ROM_QSTR(MP_QSTR_KEY_CPAD_DOWN),                             MP_ROM_INT(KEY_CPAD_DOWN)},
        {MP_ROM_QSTR(MP_QSTR_KEY_UP),                                    MP_ROM_INT(KEY_UP)},
        {MP_ROM_QSTR(MP_QSTR_KEY_DOWN),                                  MP_ROM_INT(KEY_DOWN)},
        {MP_ROM_QSTR(MP_QSTR_KEY_LEFT),                                  MP_ROM_INT(KEY_LEFT)},
        {MP_ROM_QSTR(MP_QSTR_KEY_RIGHT),                                 MP_ROM_INT(KEY_RIGHT)},

        // HID_Event
        {MP_ROM_QSTR(MP_QSTR_EVENT_PAD0),                                MP_ROM_INT(HIDEVENT_PAD0)},
        {MP_ROM_QSTR(MP_QSTR_EVENT_PAD1),                                MP_ROM_INT(HIDEVENT_PAD1)},
        {MP_ROM_QSTR(MP_QSTR_EVENT_ACCEL),                               MP_ROM_INT(HIDEVENT_Accel)},
        {MP_ROM_QSTR(MP_QSTR_EVENT_GYRO),                                MP_ROM_INT(HIDEVENT_Gyro)},
        {MP_ROM_QSTR(MP_QSTR_EVENT_DEBUG_PAD),                           MP_ROM_INT(HIDEVENT_DebugPad)},
        {MP_ROM_QSTR(MP_QSTR_EVENT_MAX),                                 MP_ROM_INT(HIDEVENT_MAX)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_hid_globals, mp_module_citrus_hid_globals_table);

const mp_obj_module_t mp_module_citrus_hid = {
        .base = {&mp_type_module},
        .name = MP_QSTR_hid,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_hid_globals,
};
