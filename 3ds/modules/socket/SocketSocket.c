#include <errno.h>

#include <3ds.h>

#include "py/runtime.h"

#define SELF(src) mod_socket_Socket_t *self = src

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_socket_Socket_##__n##_obj, mod_socket_Socket_##__n)
#define METHOD_OBJ_KW(__min, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_KW(mod_socket_Socket_##__n##_obj, __min, mod_socket_Socket_##__n)
#define METHOD_OBJ_VAR(__min, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_VAR(mod_socket_Socket_##__n##_obj, __min, mod_socket_Socket_##__n)
#define METHOD_OBJ_VAR_MAX(__min, __max, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_socket_Socket_##__n##_obj, __min, __max, mod_socket_Socket_##__n)
#define METHOD_OBJ_VAR_N(__num, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_socket_Socket_##__n##_obj, __num, __num, mod_socket_Socket_##__n)

const mp_obj_type_t mod_socket_Socket_type;
const mp_obj_type_t mod_socket_exception_error_type;

typedef struct {
    mp_obj_base_t base;

    int fd;
} mod_socket_Socket_t;

enum {
    NEW_ARG_FAMILY = 0,
    NEW_ARG_TYPE,
    NEW_ARG_PROTO,
    NEW_ARG_COUNT
};

STATIC mp_obj_t mod_socket_Socket_make_new(const mp_obj_type_t *type_in, size_t n_args, size_t n_kw, const mp_obj_t *args_in) {
    mp_arg_check_num(n_args, n_kw, 2, NEW_ARG_COUNT, true);

    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_family, MP_ARG_INT, {.u_int = AF_INET}},
            {MP_QSTR_type,   MP_ARG_INT, {.u_int = SOCK_STREAM}},
            {MP_QSTR_proto,  MP_ARG_INT, {.u_int = IPPROTO_IP}},
    };

    mp_arg_val_t args[NEW_ARG_COUNT];
    mp_arg_parse_all_kw_array(n_args, n_kw, args_in, NEW_ARG_COUNT, allowed_args, args);

    int family = args[NEW_ARG_FAMILY].u_int;
    int type = args[NEW_ARG_TYPE].u_int;
    int proto = args[NEW_ARG_TYPE].u_int;

    if (family != AF_INET) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "family must be socket.AF_INET"));
    }
    if (type != SOCK_STREAM && type != SOCK_DGRAM) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "type must be one of socket.SOCK_STREAM or socket.SOCK_DGRAM"));
    }
    if (proto && proto != IPPROTO_TCP && proto != IPPROTO_UDP) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "proto must be one of socket.IPPROTO_IP, socket.IPPROTO_TCP, socket.IPPROTO_UDP"));
    }

    mod_socket_Socket_t *obj = m_new_obj(mod_socket_Socket_t);
    obj->base.type = (mp_obj_t) &mod_socket_Socket_type;
    obj->fd = socket(family, type, proto);

    if (obj->fd == -1) {
        nlr_raise(mp_obj_new_exception_arg1(&mod_socket_exception_error_type, mp_obj_new_int(errno)));
    }

    return obj;
}

STATIC void mod_socket_Socket_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    SELF(self_in);

    mp_printf(print, "<socket fd=%d>", self->fd);
}

STATIC mp_obj_t mod_socket_Socket_accept(mp_obj_t self_in) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_bind(mp_obj_t self_in, mp_obj_t address) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_close(mp_obj_t self_in) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_connect(mp_obj_t self_in, mp_obj_t address) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_connect_ex(mp_obj_t self_in, mp_obj_t address) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_fileno(mp_obj_t self_in) {
    SELF(self_in);

    return mp_obj_new_int(self->fd);
}

STATIC mp_obj_t mod_socket_Socket_getpeername(mp_obj_t self_in) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_getsockname(mp_obj_t self_in) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_getsockopt(size_t n_args, const mp_obj_t *args) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_ioctl(mp_obj_t self_in, mp_obj_t control, mp_obj_t option) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_listen(mp_obj_t self_in, mp_obj_t backlog) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_recv(size_t n_args, const mp_obj_t *args) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_recvfrom(size_t n_args, const mp_obj_t *args) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_recvfrom_into(size_t n_args, const mp_obj_t *args) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_recv_into(size_t n_args, const mp_obj_t *args) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_send(size_t n_args, const mp_obj_t *args) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_sendall(size_t n_args, const mp_obj_t *args) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_sendto(size_t n_args, const mp_obj_t *args) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_setblocking(mp_obj_t self_in, mp_obj_t flag) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_settimeout(mp_obj_t self_in, mp_obj_t value) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_gettimeout(mp_obj_t self_in) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_setsockopt(size_t n_args, const mp_obj_t *args) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_shutdown(mp_obj_t self_in, mp_obj_t how) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

METHOD_OBJ_N(1, accept);
METHOD_OBJ_N(2, bind);
METHOD_OBJ_N(1, close);
METHOD_OBJ_N(2, connect);
METHOD_OBJ_N(2, connect_ex);
METHOD_OBJ_N(1, fileno);
METHOD_OBJ_N(1, getpeername);
METHOD_OBJ_N(1, getsockname);
METHOD_OBJ_VAR_MAX(3, 4, getsockopt);
METHOD_OBJ_N(3, ioctl);
METHOD_OBJ_N(2, listen);
METHOD_OBJ_VAR_MAX(2, 3, recv);
METHOD_OBJ_VAR_MAX(2, 3, recvfrom);
METHOD_OBJ_VAR_MAX(2, 4, recvfrom_into);
METHOD_OBJ_VAR_MAX(2, 4, recv_into);
METHOD_OBJ_VAR_MAX(2, 3, send);
METHOD_OBJ_VAR_MAX(2, 3, sendall);
METHOD_OBJ_VAR_MAX(3, 4, sendto);
METHOD_OBJ_N(2, setblocking);
METHOD_OBJ_N(2, settimeout);
METHOD_OBJ_N(1, gettimeout);
METHOD_OBJ_VAR_N(4, setsockopt);
METHOD_OBJ_N(2, shutdown);

STATIC const mp_map_elem_t mod_socket_Socket_locals_dict_table[] = {
        // Methods
};
STATIC MP_DEFINE_CONST_DICT(mod_socket_Socket_locals_dict, mod_socket_Socket_locals_dict_table);

const mp_obj_type_t mod_socket_Socket_type = {
        {&mp_type_type},
        .name = MP_QSTR_socket,
        .print = mod_socket_Socket_print,
        .make_new = mod_socket_Socket_make_new,
        .locals_dict = (mp_obj_t) &mod_socket_Socket_locals_dict,
};