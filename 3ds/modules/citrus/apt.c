#include <3ds.h>

#include "py/runtime.h"

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_citrus_apt_##__n##_obj, mod_citrus_apt_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_apt_##__n##_obj}
#define LOCAL_INT(__n, __v) \
    {MP_ROM_QSTR(MP_QSTR_##__n), MP_ROM_INT(__v)}

extern const mp_obj_type_t mod_citrus_apt_Hook_type;

STATIC APT_AppStatus _mod_citrus_apt_get_app_status(mp_obj_t status) {
    if (mp_obj_is_integer(status)) {
        int _status = mp_obj_get_int(status);
        if (_status >= APP_NOTINITIALIZED && _status <= APP_APPLETCLOSED) {
            return _status;
        }
    }
    nlr_raise(mp_obj_new_exception(&mp_type_TypeError));
}

STATIC mp_obj_t mod_citrus_apt_init(void) {
    return mp_obj_new_int(aptInit());
}

STATIC mp_obj_t mod_citrus_apt_exit(void) {
    aptExit();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_apt_open_session(void) {
    aptOpenSession();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_apt_close_session(void) {
    aptCloseSession();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_apt_set_status(mp_obj_t status) {
    APT_AppStatus _status = _mod_citrus_apt_get_app_status(status);
    aptSetStatus(_status);

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_apt_get_status(void) {
    return mp_obj_new_int(aptGetStatus());
}

STATIC mp_obj_t mod_citrus_apt_get_status_power(void) {
    return mp_obj_new_int(aptGetStatusPower());
}

STATIC mp_obj_t mod_citrus_apt_set_status_power(mp_obj_t status) {
    u32 _status = mp_obj_get_int(status);
    aptSetStatusPower(_status);

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_apt_return_to_menu(void) {
    aptReturnToMenu();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_apt_wait_status_event(void) {
    aptWaitStatusEvent();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_apt_signal_ready_for_sleep(void) {
    aptSignalReadyForSleep();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_apt_get_menu_app_id(void) {
    return mp_obj_new_int(aptGetMenuAppID());
}

STATIC mp_obj_t mod_citrus_apt_main_loop(void) {
    return mp_obj_new_bool(aptMainLoop());
}

METHOD_OBJ_N(0, init);
METHOD_OBJ_N(0, exit);
METHOD_OBJ_N(0, open_session);
METHOD_OBJ_N(0, close_session);
METHOD_OBJ_N(1, set_status);
METHOD_OBJ_N(0, get_status);
METHOD_OBJ_N(0, get_status_power);
METHOD_OBJ_N(1, set_status_power);
METHOD_OBJ_N(0, return_to_menu);
METHOD_OBJ_N(0, wait_status_event);
METHOD_OBJ_N(0, signal_ready_for_sleep);
METHOD_OBJ_N(0, get_menu_app_id);
METHOD_OBJ_N(0, main_loop);

STATIC const mp_rom_map_elem_t mp_module_citrus_apt_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_apt)},

        // Classes
        {MP_ROM_QSTR(MP_QSTR_Hook), MP_ROM_PTR(&mod_citrus_apt_Hook_type)},

        // Functions
        LOCAL_METHOD(init),
        LOCAL_METHOD(exit),
        LOCAL_METHOD(open_session),
        LOCAL_METHOD(close_session),
        LOCAL_METHOD(set_status),
        LOCAL_METHOD(get_status),
        LOCAL_METHOD(get_status_power),
        LOCAL_METHOD(set_status_power),
        LOCAL_METHOD(return_to_menu),
        LOCAL_METHOD(wait_status_event),
        LOCAL_METHOD(signal_ready_for_sleep),
        LOCAL_METHOD(get_menu_app_id),
        LOCAL_METHOD(main_loop),

        // NS_APPID
        LOCAL_INT(APPID_HOMEMENU, APPID_HOMEMENU),
        LOCAL_INT(APPID_CAMERA, APPID_CAMERA),
        LOCAL_INT(APPID_FRIENDS_LIST, APPID_FRIENDS_LIST),
        LOCAL_INT(APPID_GAME_NOTES, APPID_GAME_NOTES),
        LOCAL_INT(APPID_WEB, APPID_WEB),
        LOCAL_INT(APPID_INSTRUCTION_MANUAL, APPID_INSTRUCTION_MANUAL),
        LOCAL_INT(APPID_NOTIFICATIONS, APPID_NOTIFICATIONS),
        LOCAL_INT(APPID_MIIVERSE, APPID_MIIVERSE),
        LOCAL_INT(APPID_MIIVERSE_POSTING, APPID_MIIVERSE_POSTING),
        LOCAL_INT(APPID_AMIIBO_SETTINGS, APPID_AMIIBO_SETTINGS),
        LOCAL_INT(APPID_APPLICATION, APPID_APPLICATION),
        LOCAL_INT(APPID_ESHOP, APPID_ESHOP),
        LOCAL_INT(APPID_SOFTWARE_KEYBOARD, APPID_SOFTWARE_KEYBOARD),
        LOCAL_INT(APPID_APPLETED, APPID_APPLETED),
        LOCAL_INT(APPID_PNOTE_AP, APPID_PNOTE_AP),
        LOCAL_INT(APPID_SNOTE_AP, APPID_SNOTE_AP),
        LOCAL_INT(APPID_ERROR, APPID_ERROR),
        LOCAL_INT(APPID_MINT, APPID_MINT),
        LOCAL_INT(APPID_EXTRAPAD, APPID_EXTRAPAD),
        LOCAL_INT(APPID_MEMOLIB, APPID_MEMOLIB),

        // APT_AppStatus
        LOCAL_INT(STATUS_NOTINITIALIZED, APP_NOTINITIALIZED),
        LOCAL_INT(STATUS_RUNNING, APP_RUNNING),
        LOCAL_INT(STATUS_SUSPENDED, APP_SUSPENDED),
        LOCAL_INT(STATUS_EXITING, APP_EXITING),
        LOCAL_INT(STATUS_SUSPENDING, APP_SUSPENDING),
        LOCAL_INT(STATUS_SLEEPMODE, APP_SLEEPMODE),
        LOCAL_INT(STATUS_PREPARE_SLEEPMODE, APP_PREPARE_SLEEPMODE),
        LOCAL_INT(STATUS_APPLETSTARTED, APP_APPLETSTARTED),
        LOCAL_INT(STATUS_APPLETCLOSED, APP_APPLETCLOSED),

        // APT_Signal
        LOCAL_INT(SIGNAL_HOMEBUTTON, APTSIGNAL_HOMEBUTTON),
        LOCAL_INT(SIGNAL_PREPARESLEEP, APTSIGNAL_PREPARESLEEP),
        LOCAL_INT(SIGNAL_ENTERSLEEP, APTSIGNAL_ENTERSLEEP),
        LOCAL_INT(SIGNAL_WAKEUP, APTSIGNAL_WAKEUP),
        LOCAL_INT(SIGNAL_ENABLE, APTSIGNAL_ENABLE),
        LOCAL_INT(SIGNAL_POWERBUTTON, APTSIGNAL_POWERBUTTON),
        LOCAL_INT(SIGNAL_UTILITY, APTSIGNAL_UTILITY),
        LOCAL_INT(SIGNAL_SLEEPSYSTEM, APTSIGNAL_SLEEPSYSTEM),
        LOCAL_INT(SIGNAL_ERROR, APTSIGNAL_ERROR),

        // APT_HookType
        LOCAL_INT(HOOK_ONSUSPEND, APTHOOK_ONSUSPEND),
        LOCAL_INT(HOOK_ONRESTORE, APTHOOK_ONRESTORE),
        LOCAL_INT(HOOK_ONSLEEP, APTHOOK_ONSLEEP),
        LOCAL_INT(HOOK_ONWAKEUP, APTHOOK_ONWAKEUP),
        LOCAL_INT(HOOK_ONEXIT, APTHOOK_ONEXIT),
        LOCAL_INT(HOOK_COUNT, APTHOOK_COUNT),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_apt_globals, mp_module_citrus_apt_globals_table);

const mp_obj_module_t mp_module_citrus_apt = {
        .base = {&mp_type_module},
        .name = MP_QSTR_apt,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_apt_globals,
};
