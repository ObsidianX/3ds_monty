#include <3ds.h>

#include "py/runtime.h"

#define LOCAL_EXCEPTION(__n) \
    {MP_ROM_QSTR(MP_QSTR_##__n), MP_ROM_PTR(&mod_socket_exception_##__n##_type)}

const mp_obj_type_t mod_socket_exception_timeout_type;
const mp_obj_type_t mod_socket_exception_error_type;

const mp_obj_type_t mod_socket_Socket_type;

STATIC const mp_rom_map_elem_t mp_module_socket_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_socket)},

        // Exceptions
        LOCAL_EXCEPTION(error),
        LOCAL_EXCEPTION(timeout),

        // Classes
        {MP_ROM_QSTR(MP_QSTR_socket), MP_ROM_PTR(&mod_socket_Socket_type)}
};

STATIC MP_DEFINE_CONST_DICT(mp_module_socket_globals, mp_module_socket_globals_table);

const mp_obj_module_t mp_module_socket = {
        .base = {&mp_type_module},
        .name = MP_QSTR_socket,
        .globals = (mp_obj_dict_t *) &mp_module_socket_globals,
};
