#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "py/runtime.h"

mp_obj_t mod_os_path_exists(mp_obj_t path) {
    const char *_path = mp_obj_str_get_str(path);

    struct stat st;
    int res = stat(_path, &st);

    return res < 0 ? mp_const_false : mp_const_true;
}

mp_obj_t mod_os_path_isfile(mp_obj_t path) {
    const char *_path = mp_obj_str_get_str(path);

    struct stat st;
    int res = stat(_path, &st);

    return (res == 0 && S_ISREG(st.st_mode) != 0) ? mp_const_true : mp_const_false;
}

mp_obj_t mod_os_path_isdir(mp_obj_t path) {
    const char *_path = mp_obj_str_get_str(path);

    struct stat st;
    int res = stat(_path, &st);

    return (res == 0 && S_ISDIR(st.st_mode) != 0) ? mp_const_true : mp_const_false;
}

MP_DEFINE_CONST_FUN_OBJ_1(mod_os_path_exists_obj, mod_os_path_exists);
MP_DEFINE_CONST_FUN_OBJ_1(mod_os_path_isfile_obj, mod_os_path_isfile);
MP_DEFINE_CONST_FUN_OBJ_1(mod_os_path_isdir_obj, mod_os_path_isdir);

STATIC const mp_rom_map_elem_t mp_module_os_path_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_path)},

        // Functions
        {MP_ROM_QSTR(MP_QSTR_exists),   MP_ROM_PTR(&mod_os_path_exists_obj)},
        {MP_ROM_QSTR(MP_QSTR_isfile),   MP_ROM_PTR(&mod_os_path_isfile_obj)},
        {MP_ROM_QSTR(MP_QSTR_isdir),    MP_ROM_PTR(&mod_os_path_isdir_obj)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_os_path_globals, mp_module_os_path_globals_table);

const mp_obj_module_t mp_module_os_path = {
        .base = {&mp_type_module},
        .name = MP_QSTR_path,
        .globals = (mp_obj_dict_t *) &mp_module_os_path_globals,
};