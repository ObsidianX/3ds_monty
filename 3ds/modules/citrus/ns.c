#include <3ds.h>

#include "py/runtime.h"

#include "helpers.h"
#include "../init_helper.h"

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_citrus_ns_##__n##_obj, mod_citrus_ns_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_ns_##__n##_obj}

static Result _ns_last_result;
static int _mod_citrus_ns_is_init = 0;

STATIC mp_obj_t mod_citrus_ns_init(void) {
    INIT_ONCE(_mod_citrus_ns_is_init);

    _ns_last_result = nsInit();

    return mp_const_none;
}

mp_obj_t mod_citrus_ns_exit(void) {
    EXIT_ONCE(_mod_citrus_ns_is_init);

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

METHOD_OBJ_N(0, init);
METHOD_OBJ_N(0, exit);
METHOD_OBJ_N(2, launch_title);
METHOD_OBJ_N(2, reboot_to_title);
METHOD_OBJ_N(1, terminate_process_tid);

STATIC const mp_rom_map_elem_t mp_module_citrus_ns_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ns)},

        // Functions
        LOCAL_METHOD(init),
        LOCAL_METHOD(exit),
        LOCAL_METHOD(launch_title),
        LOCAL_METHOD(reboot_to_title),
        LOCAL_METHOD(terminate_process_tid),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_ns_globals, mp_module_citrus_ns_globals_table);

const mp_obj_module_t mp_module_citrus_ns = {
        .base = {&mp_type_module},
        .name = MP_QSTR_ns,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_ns_globals,
};