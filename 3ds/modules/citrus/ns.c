#include <3ds.h>

#include "py/runtime.h"

#include "helpers.h"

static Result _ns_last_result;

STATIC mp_obj_t mod_citrus_ns_init(void) {
    _ns_last_result = nsInit();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ns_exit(void) {
    nsExit();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ns_launch_title(mp_obj_t title_id, mp_obj_t launch_flags) {
    u64 title = mp_obj_get_int(title_id);
    u32 flags = mp_obj_get_int(launch_flags);
    u32 proc;

    _ns_last_result = NS_LaunchTitle(title, flags, &proc);

    return mp_obj_new_int(proc);
}

STATIC mp_obj_t mod_citrus_ns_reboot_to_title(mp_obj_t media_type, mp_obj_t title_id) {
    u8 media = _mod_citrus_fs_get_mediatype(media_type);
    u64 title = mp_obj_get_int(title_id);

    _ns_last_result = NS_RebootToTitle(media, title);

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ns_terminate_process_tid(mp_obj_t title_id) {
    u64 title = mp_obj_get_int(title_id);

    _ns_last_result = NS_TerminateProcessTID(title);

    return mp_const_none;
}

STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_ns_init_obj, mod_citrus_ns_init);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_ns_exit_obj, mod_citrus_ns_exit);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_citrus_ns_launch_title_obj, mod_citrus_ns_launch_title);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_citrus_ns_reboot_to_title_obj, mod_citrus_ns_reboot_to_title);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_ns_terminate_process_tid_obj, mod_citrus_ns_terminate_process_tid);

STATIC const mp_rom_map_elem_t mp_module_citrus_ns_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__),              MP_ROM_QSTR(MP_QSTR_ns)},

        // Functions
        {MP_ROM_QSTR(MP_QSTR_init),                  MP_ROM_PTR(&mod_citrus_ns_init_obj)},
        {MP_ROM_QSTR(MP_QSTR_exit),                  MP_ROM_PTR(&mod_citrus_ns_exit_obj)},
        {MP_ROM_QSTR(MP_QSTR_launch_title),          MP_ROM_PTR(&mod_citrus_ns_launch_title_obj)},
        {MP_ROM_QSTR(MP_QSTR_reboot_to_title),       MP_ROM_PTR(&mod_citrus_ns_reboot_to_title_obj)},
        {MP_ROM_QSTR(MP_QSTR_terminate_process_tid), MP_ROM_PTR(&mod_citrus_ns_terminate_process_tid_obj)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_ns_globals, mp_module_citrus_ns_globals_table);

const mp_obj_module_t mp_module_citrus_ns = {
        .base = {&mp_type_module},
        .name = MP_QSTR_ns,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_ns_globals,
};