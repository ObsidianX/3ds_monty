#include <3ds.h>

#include "py/runtime.h"

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

STATIC mp_obj_t mod_citrus_apt_hook(mp_obj_t cookie, mp_obj_t callback, mp_obj_t param) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_citrus_apt_unhook(mp_obj_t cookie) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_apt_init_obj, mod_citrus_apt_init);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_apt_exit_obj, mod_citrus_apt_exit);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_apt_open_session_obj, mod_citrus_apt_open_session);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_apt_close_session_obj, mod_citrus_apt_close_session);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_apt_set_status_obj, mod_citrus_apt_set_status);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_apt_get_status_obj, mod_citrus_apt_get_status);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_apt_get_status_power_obj, mod_citrus_apt_get_status_power);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_apt_set_status_power_obj, mod_citrus_apt_set_status_power);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_apt_return_to_menu_obj, mod_citrus_apt_return_to_menu);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_apt_wait_status_event_obj, mod_citrus_apt_wait_status_event);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_apt_signal_ready_for_sleep_obj, mod_citrus_apt_signal_ready_for_sleep);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_apt_get_menu_app_id_obj, mod_citrus_apt_get_menu_app_id);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_apt_main_loop_obj, mod_citrus_apt_main_loop);
STATIC MP_DEFINE_CONST_FUN_OBJ_3(mod_citrus_apt_hook_obj, mod_citrus_apt_hook);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_apt_unhook_obj, mod_citrus_apt_unhook);

STATIC const mp_rom_map_elem_t mp_module_citrus_apt_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__),                 MP_ROM_QSTR(MP_QSTR_apt)},

        // Functions
        {MP_ROM_QSTR(MP_QSTR_init),                     MP_ROM_PTR(&mod_citrus_apt_init_obj)},
        {MP_ROM_QSTR(MP_QSTR_exit),                     MP_ROM_PTR(&mod_citrus_apt_exit_obj)},
        {MP_ROM_QSTR(MP_QSTR_open_session),             MP_ROM_PTR(&mod_citrus_apt_open_session_obj)},
        {MP_ROM_QSTR(MP_QSTR_close_session),            MP_ROM_PTR(&mod_citrus_apt_close_session_obj)},
        {MP_ROM_QSTR(MP_QSTR_set_status),               MP_ROM_PTR(&mod_citrus_apt_set_status_obj)},
        {MP_ROM_QSTR(MP_QSTR_get_status),               MP_ROM_PTR(&mod_citrus_apt_get_status_obj)},
        {MP_ROM_QSTR(MP_QSTR_get_status_power),         MP_ROM_PTR(&mod_citrus_apt_get_status_power_obj)},
        {MP_ROM_QSTR(MP_QSTR_set_status_power),         MP_ROM_PTR(&mod_citrus_apt_set_status_power_obj)},
        {MP_ROM_QSTR(MP_QSTR_return_to_menu),           MP_ROM_PTR(&mod_citrus_apt_return_to_menu_obj)},
        {MP_ROM_QSTR(MP_QSTR_wait_status_event),        MP_ROM_PTR(&mod_citrus_apt_wait_status_event_obj)},
        {MP_ROM_QSTR(MP_QSTR_signal_ready_for_sleep),   MP_ROM_PTR(&mod_citrus_apt_signal_ready_for_sleep_obj)},
        {MP_ROM_QSTR(MP_QSTR_get_menu_app_id),          MP_ROM_PTR(&mod_citrus_apt_get_menu_app_id_obj)},
        {MP_ROM_QSTR(MP_QSTR_main_loop),                MP_ROM_PTR(&mod_citrus_apt_main_loop_obj)},
        {MP_ROM_QSTR(MP_QSTR_hook),                     MP_ROM_PTR(&mod_citrus_apt_hook_obj)},
        {MP_ROM_QSTR(MP_QSTR_unhook),                   MP_ROM_PTR(&mod_citrus_apt_unhook_obj)},

        // NS_APPID
        {MP_ROM_QSTR(MP_QSTR_APPID_HOMEMENU),           MP_ROM_INT(APPID_HOMEMENU)},
        {MP_ROM_QSTR(MP_QSTR_APPID_CAMERA),             MP_ROM_INT(APPID_CAMERA)},
        {MP_ROM_QSTR(MP_QSTR_APPID_FRIENDS_LIST),       MP_ROM_INT(APPID_FRIENDS_LIST)},
        {MP_ROM_QSTR(MP_QSTR_APPID_GAME_NOTES),         MP_ROM_INT(APPID_GAME_NOTES)},
        {MP_ROM_QSTR(MP_QSTR_APPID_WEB),                MP_ROM_INT(APPID_WEB)},
        {MP_ROM_QSTR(MP_QSTR_APPID_INSTRUCTION_MANUAL), MP_ROM_INT(APPID_INSTRUCTION_MANUAL)},
        {MP_ROM_QSTR(MP_QSTR_APPID_NOTIFICATIONS),      MP_ROM_INT(APPID_NOTIFICATIONS)},
        {MP_ROM_QSTR(MP_QSTR_APPID_MIIVERSE),           MP_ROM_INT(APPID_MIIVERSE)},
        {MP_ROM_QSTR(MP_QSTR_APPID_MIIVERSE_POSTING),   MP_ROM_INT(APPID_MIIVERSE_POSTING)},
        {MP_ROM_QSTR(MP_QSTR_APPID_AMIIBO_SETTINGS),    MP_ROM_INT(APPID_AMIIBO_SETTINGS)},
        {MP_ROM_QSTR(MP_QSTR_APPID_APPLICATION),        MP_ROM_INT(APPID_APPLICATION)},
        {MP_ROM_QSTR(MP_QSTR_APPID_ESHOP),              MP_ROM_INT(APPID_ESHOP)},
        {MP_ROM_QSTR(MP_QSTR_APPID_SOFTWARE_KEYBOARD),  MP_ROM_INT(APPID_SOFTWARE_KEYBOARD)},
        {MP_ROM_QSTR(MP_QSTR_APPID_APPLETED),           MP_ROM_INT(APPID_APPLETED)},
        {MP_ROM_QSTR(MP_QSTR_APPID_PNOTE_AP),           MP_ROM_INT(APPID_PNOTE_AP)},
        {MP_ROM_QSTR(MP_QSTR_APPID_SNOTE_AP),           MP_ROM_INT(APPID_SNOTE_AP)},
        {MP_ROM_QSTR(MP_QSTR_APPID_ERROR),              MP_ROM_INT(APPID_ERROR)},
        {MP_ROM_QSTR(MP_QSTR_APPID_MINT),               MP_ROM_INT(APPID_MINT)},
        {MP_ROM_QSTR(MP_QSTR_APPID_EXTRAPAD),           MP_ROM_INT(APPID_EXTRAPAD)},
        {MP_ROM_QSTR(MP_QSTR_APPID_MEMOLIB),            MP_ROM_INT(APPID_MEMOLIB)},

        // APT_AppStatus
        {MP_ROM_QSTR(MP_QSTR_STATUS_NOTINITIALIZED),    MP_ROM_INT(APP_NOTINITIALIZED)},
        {MP_ROM_QSTR(MP_QSTR_STATUS_RUNNING),           MP_ROM_INT(APP_RUNNING)},
        {MP_ROM_QSTR(MP_QSTR_STATUS_SUSPENDED),         MP_ROM_INT(APP_SUSPENDED)},
        {MP_ROM_QSTR(MP_QSTR_STATUS_EXITING),           MP_ROM_INT(APP_EXITING)},
        {MP_ROM_QSTR(MP_QSTR_STATUS_SUSPENDING),        MP_ROM_INT(APP_SUSPENDING)},
        {MP_ROM_QSTR(MP_QSTR_STATUS_SLEEPMODE),         MP_ROM_INT(APP_SLEEPMODE)},
        {MP_ROM_QSTR(MP_QSTR_STATUS_PREPARE_SLEEPMODE), MP_ROM_INT(APP_PREPARE_SLEEPMODE)},
        {MP_ROM_QSTR(MP_QSTR_STATUS_APPLETSTARTED),     MP_ROM_INT(APP_APPLETSTARTED)},
        {MP_ROM_QSTR(MP_QSTR_STATUS_APPLETCLOSED),      MP_ROM_INT(APP_APPLETCLOSED)},

        // APT_Signal
        {MP_ROM_QSTR(MP_QSTR_SIGNAL_HOMEBUTTON),        MP_ROM_INT(APTSIGNAL_HOMEBUTTON)},
        {MP_ROM_QSTR(MP_QSTR_SIGNAL_PREPARESLEEP),      MP_ROM_INT(APTSIGNAL_PREPARESLEEP)},
        {MP_ROM_QSTR(MP_QSTR_SIGNAL_ENTERSLEEP),        MP_ROM_INT(APTSIGNAL_ENTERSLEEP)},
        {MP_ROM_QSTR(MP_QSTR_SIGNAL_WAKEUP),            MP_ROM_INT(APTSIGNAL_WAKEUP)},
        {MP_ROM_QSTR(MP_QSTR_SIGNAL_ENABLE),            MP_ROM_INT(APTSIGNAL_ENABLE)},
        {MP_ROM_QSTR(MP_QSTR_SIGNAL_POWERBUTTON),       MP_ROM_INT(APTSIGNAL_POWERBUTTON)},
        {MP_ROM_QSTR(MP_QSTR_SIGNAL_UTILITY),           MP_ROM_INT(APTSIGNAL_UTILITY)},
        {MP_ROM_QSTR(MP_QSTR_SIGNAL_SLEEPSYSTEM),       MP_ROM_INT(APTSIGNAL_SLEEPSYSTEM)},
        {MP_ROM_QSTR(MP_QSTR_SIGNAL_ERROR),             MP_ROM_INT(APTSIGNAL_ERROR)},

        // APT_HookType
        {MP_ROM_QSTR(MP_QSTR_HOOK_ONSUSPEND),           MP_ROM_INT(APTHOOK_ONSUSPEND)},
        {MP_ROM_QSTR(MP_QSTR_HOOK_ONRESTORE),           MP_ROM_INT(APTHOOK_ONRESTORE)},
        {MP_ROM_QSTR(MP_QSTR_HOOK_ONSLEEP),             MP_ROM_INT(APTHOOK_ONSLEEP)},
        {MP_ROM_QSTR(MP_QSTR_HOOK_ONWAKEUP),            MP_ROM_INT(APTHOOK_ONWAKEUP)},
        {MP_ROM_QSTR(MP_QSTR_HOOK_ONEXIT),              MP_ROM_INT(APTHOOK_ONEXIT)},
        {MP_ROM_QSTR(MP_QSTR_HOOK_COUNT),               MP_ROM_INT(APTHOOK_COUNT)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_apt_globals, mp_module_citrus_apt_globals_table);

const mp_obj_module_t mp_module_citrus_apt = {
        .base = {&mp_type_module},
        .name = MP_QSTR_apt,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_apt_globals,
};
