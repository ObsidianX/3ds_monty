#include <string.h>

#include <3ds.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>

#include "py/runtime.h"

#define LOCAL_EXCEPTION(__n) \
    {MP_ROM_QSTR(MP_QSTR_##__n), MP_ROM_PTR(&mod_socket_exception_##__n##_type)}
#define LOCAL_FIELD(__n) \
    {MP_ROM_QSTR(MP_QSTR_##__n), MP_ROM_INT(__n)}

const mp_obj_type_t mod_socket_exception_error_type;

const mp_obj_type_t mod_socket_Socket_type;

STATIC mp_obj_t mod_socket_gethostid(void) {
    struct in_addr addr = {(in_addr_t) gethostid()};
    char *host = inet_ntoa(addr);

    return mp_obj_new_str(host, strlen(host), false);
}

MP_DEFINE_CONST_FUN_OBJ_0(mod_socket_gethostid_obj, mod_socket_gethostid);

STATIC const mp_rom_map_elem_t mp_module_socket_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_socket)},

        // Exceptions
        LOCAL_EXCEPTION(error),

        // Classes
        {MP_ROM_QSTR(MP_QSTR_socket), MP_ROM_PTR(&mod_socket_Socket_type)},

        // Functions
        {MP_ROM_QSTR(MP_QSTR_gethostid), MP_ROM_PTR(&mod_socket_gethostid_obj)},

        // Constants
        LOCAL_FIELD(SOL_SOCKET),
        LOCAL_FIELD(SOL_TCP),

        LOCAL_FIELD(AF_INET),

        LOCAL_FIELD(SOCK_STREAM),
        LOCAL_FIELD(SOCK_DGRAM),

        LOCAL_FIELD(IPPROTO_UDP),
        LOCAL_FIELD(IPPROTO_TCP),
        LOCAL_FIELD(IPPROTO_IP),

        LOCAL_FIELD(MSG_OOB),
        LOCAL_FIELD(MSG_PEEK),
        LOCAL_FIELD(MSG_DONTWAIT),

        LOCAL_FIELD(SHUT_RD),
        LOCAL_FIELD(SHUT_WR),
        LOCAL_FIELD(SHUT_RDWR),

        LOCAL_FIELD(SO_REUSEADDR),
        LOCAL_FIELD(SO_LINGER),
        LOCAL_FIELD(SO_OOBINLINE),
        LOCAL_FIELD(SO_SNDBUF),
        LOCAL_FIELD(SO_RCVBUF),
        LOCAL_FIELD(SO_SNDLOWAT),
        LOCAL_FIELD(SO_RCVLOWAT),
        LOCAL_FIELD(SO_TYPE),
        LOCAL_FIELD(SO_ERROR),


};

STATIC MP_DEFINE_CONST_DICT(mp_module_socket_globals, mp_module_socket_globals_table);

const mp_obj_module_t mp_module_socket = {
        .base = {&mp_type_module},
        .name = MP_QSTR_socket,
        .globals = (mp_obj_dict_t *) &mp_module_socket_globals,
};
