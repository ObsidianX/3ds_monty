#include <3ds.h>

#include "py/runtime.h"

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_citrus_hid_##__n##_obj, mod_citrus_hid_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_hid_##__n##_obj}
#define LOCAL_INT(__n, __v) \
    {MP_ROM_QSTR(MP_QSTR_##__n), MP_ROM_INT(__v)}

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

METHOD_OBJ_N(0, init);
METHOD_OBJ_N(0, exit);
METHOD_OBJ_N(0, scan_input);
METHOD_OBJ_N(0, keys_held);
METHOD_OBJ_N(0, keys_down);
METHOD_OBJ_N(0, keys_up);
METHOD_OBJ_N(0, touch_read);
METHOD_OBJ_N(0, circle_read);
METHOD_OBJ_N(0, accel_read);
METHOD_OBJ_N(0, gyro_read);
METHOD_OBJ_N(2, wait_for_event);
METHOD_OBJ_N(0, user_get_handles);
METHOD_OBJ_N(0, user_enable_accelerometer);
METHOD_OBJ_N(0, user_disable_accelerometer);
METHOD_OBJ_N(0, user_enable_gyroscope);
METHOD_OBJ_N(0, user_disable_gyroscope);
METHOD_OBJ_N(0, user_get_gyroscope_raw_to_dps_coefficient);
METHOD_OBJ_N(0, user_get_sound_volume);

STATIC const mp_rom_map_elem_t mp_module_citrus_hid_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_hid)},

        // Functions
        LOCAL_METHOD(init),
        LOCAL_METHOD(exit),
        LOCAL_METHOD(scan_input),
        LOCAL_METHOD(keys_held),
        LOCAL_METHOD(keys_down),
        LOCAL_METHOD(keys_up),
        LOCAL_METHOD(touch_read),
        LOCAL_METHOD(circle_read),
        LOCAL_METHOD(accel_read),
        LOCAL_METHOD(gyro_read),
        LOCAL_METHOD(wait_for_event),
        LOCAL_METHOD(user_enable_accelerometer),
        LOCAL_METHOD(user_disable_accelerometer),
        LOCAL_METHOD(user_enable_gyroscope),
        LOCAL_METHOD(user_disable_gyroscope),
        LOCAL_METHOD(user_get_gyroscope_raw_to_dps_coefficient),
        LOCAL_METHOD(user_get_sound_volume),

        // Keys
        LOCAL_INT(KEY_A, KEY_A),
        LOCAL_INT(KEY_B, KEY_B),
        LOCAL_INT(KEY_SELECT, KEY_SELECT),
        LOCAL_INT(KEY_START, KEY_START),
        LOCAL_INT(KEY_DPAD_RIGHT, KEY_DRIGHT),
        LOCAL_INT(KEY_DPAD_LEFT, KEY_DLEFT),
        LOCAL_INT(KEY_DPAD_UP, KEY_DUP),
        LOCAL_INT(KEY_DPAD_DOWN, KEY_DDOWN),
        LOCAL_INT(KEY_R, KEY_R),
        LOCAL_INT(KEY_L, KEY_L),
        LOCAL_INT(KEY_X, KEY_X),
        LOCAL_INT(KEY_Y, KEY_Y),
        LOCAL_INT(KEY_ZL, KEY_ZL),
        LOCAL_INT(KEY_ZR, KEY_ZR),
        LOCAL_INT(KEY_TOUCH, KEY_TOUCH),
        LOCAL_INT(KEY_CSTICK_RIGHT, KEY_CSTICK_RIGHT),
        LOCAL_INT(KEY_CSTICK_LEFT, KEY_CSTICK_LEFT),
        LOCAL_INT(KEY_CSTICK_UP, KEY_CSTICK_UP),
        LOCAL_INT(KEY_CSTICK_DOWN, KEY_CSTICK_DOWN),
        LOCAL_INT(KEY_CPAD_RIGHT, KEY_CPAD_RIGHT),
        LOCAL_INT(KEY_CPAD_LEFT, KEY_CPAD_LEFT),
        LOCAL_INT(KEY_CPAD_UP, KEY_CPAD_UP),
        LOCAL_INT(KEY_CPAD_DOWN, KEY_CPAD_DOWN),
        LOCAL_INT(KEY_UP, KEY_UP),
        LOCAL_INT(KEY_DOWN, KEY_DOWN),
        LOCAL_INT(KEY_LEFT, KEY_LEFT),
        LOCAL_INT(KEY_RIGHT, KEY_RIGHT),

        // HID_Event
        LOCAL_INT(EVENT_PAD0, HIDEVENT_PAD0),
        LOCAL_INT(EVENT_PAD1, HIDEVENT_PAD1),
        LOCAL_INT(EVENT_ACCEL, HIDEVENT_Accel),
        LOCAL_INT(EVENT_GYRO, HIDEVENT_Gyro),
        LOCAL_INT(EVENT_DEBUG_PAD, HIDEVENT_DebugPad),
        LOCAL_INT(EVENT_MAX, HIDEVENT_MAX),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_hid_globals, mp_module_citrus_hid_globals_table);

const mp_obj_module_t mp_module_citrus_hid = {
        .base = {&mp_type_module},
        .name = MP_QSTR_hid,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_hid_globals,
};
