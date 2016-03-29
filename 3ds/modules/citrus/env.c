#include <string.h>

#include <3ds.h>

#include "py/runtime.h"

#define METHOD_OBJ(__n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_env_##__n##_obj, mod_citrus_env_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_env_##__n##_obj}

STATIC mp_obj_t mod_citrus_env_is_homebrew(void) {
    return mp_obj_new_bool(envIsHomebrew());
}

STATIC mp_obj_t mod_citrus_env_get_app_id(void) {
    return mp_obj_new_int(envGetAptAppId());
}

STATIC mp_obj_t mod_citrus_env_get_heap_size(void) {
    return mp_obj_new_int(envGetHeapSize());
}

STATIC mp_obj_t mod_citrus_env_get_linear_heap_size(void) {
    return mp_obj_new_int(envGetLinearHeapSize());
}

STATIC mp_obj_t mod_citrus_env_get_system_arg_list(void) {
    const char *args = envGetSystemArgList();

    return mp_obj_new_str(args, strlen(args), true);
}

STATIC mp_obj_t mod_citrus_env_get_system_run_flags(void) {
    return mp_obj_new_int(envGetSystemRunFlags());
}

METHOD_OBJ(is_homebrew);
METHOD_OBJ(get_app_id);
METHOD_OBJ(get_heap_size);
METHOD_OBJ(get_linear_heap_size);
METHOD_OBJ(get_system_arg_list);
METHOD_OBJ(get_system_run_flags);

STATIC const mp_rom_map_elem_t mp_module_citrus_env_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_env)},

        // Functions
        LOCAL_METHOD(is_homebrew),
        LOCAL_METHOD(get_app_id),
        LOCAL_METHOD(get_heap_size),
        LOCAL_METHOD(get_linear_heap_size),
        LOCAL_METHOD(get_system_arg_list),
        LOCAL_METHOD(get_system_run_flags),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_env_globals, mp_module_citrus_env_globals_table);

const mp_obj_module_t mp_module_citrus_env = {
        .base = {&mp_type_module},
        .name = MP_QSTR_env,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_env_globals,
};
