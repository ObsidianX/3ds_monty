#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "py/runtime.h"

extern const mp_obj_module_t mp_module_os_path;

mp_obj_t mod_os_mkdir(size_t n_args, const mp_obj_t *args) {
    const char *path = mp_obj_str_get_str(args[0]);
    mode_t mode = 0777;
    if (n_args > 1) {
        mode = mp_obj_get_int(args[1]);
    }

    return mp_obj_new_int(mkdir(path, mode));
}

mp_obj_t mod_os_makedirs(size_t n_args, const mp_obj_t *args) {
    const char *path = mp_obj_str_get_str(args[0]);
    mode_t mode = 0777;
    if (n_args > 1) {
        mode = mp_obj_get_int(args[1]);
    }

    char tmp[PATH_MAX];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp), "%s", path);
    len = strlen(tmp);
    if (tmp[len - 1] == '/') {
        tmp[len - 1] = '\0';
    }

    for (p = tmp + 1; *p; p++) {
        if (*p == '/') {
            *p = '\0';
            mkdir(tmp, mode);
            *p = '/';
        }
    }
    mkdir(tmp, mode);

    return mp_const_none;
}

mp_obj_t mod_os_remove(mp_obj_t path) {
    const char *_path = mp_obj_str_get_str(path);

    struct stat st;
    stat(_path, &st);

    int res = -1;
    if (S_ISDIR(st.st_mode)) {
        res = rmdir(_path);
    } else if (S_ISREG(st.st_mode)) {
        res = unlink(_path);
    }

    return mp_obj_new_int(res);
}

MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_os_mkdir_obj, 1, 2, mod_os_mkdir);
MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_os_makedirs_obj, 1, 2, mod_os_makedirs);
MP_DEFINE_CONST_FUN_OBJ_1(mod_os_remove_obj, mod_os_remove);

STATIC const mp_rom_map_elem_t mp_module_os_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_os)},

        // Submodules
        {MP_ROM_QSTR(MP_QSTR_path),     MP_ROM_PTR(&mp_module_os_path)},

        // Functions
        {MP_ROM_QSTR(MP_QSTR_mkdir),    MP_ROM_PTR(&mod_os_mkdir_obj)},
        {MP_ROM_QSTR(MP_QSTR_makedirs), MP_ROM_PTR(&mod_os_makedirs_obj)},
        {MP_ROM_QSTR(MP_QSTR_remove),   MP_ROM_PTR(&mod_os_remove_obj)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_os_globals, mp_module_os_globals_table);

const mp_obj_module_t mp_module_os = {
        .base = {&mp_type_module},
        .name = MP_QSTR_os,
        .globals = (mp_obj_dict_t *) &mp_module_os_globals,
};