#include <errno.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>

#include <3ds.h>
#include <netdb.h>
#include <arpa/inet.h>

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

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_socket_Socket_##__n##_obj}

#define RAISE_ERRNO \
    nlr_raise(mp_obj_new_exception_arg1(&mod_socket_exception_error_type, mp_obj_new_int(errno)))

#define HOST_BROADCAST_IP "255.255.255.255"
#define HOST_BROADCAST_NAME "<broadcast>"

const mp_obj_type_t mod_socket_Socket_type;
const mp_obj_type_t mod_socket_exception_error_type;

typedef struct {
    mp_obj_base_t base;

    int fd;
    int type;
    int proto;
} mod_socket_Socket_t;

STATIC void _mod_socket_sockaddr_from_tuple(mp_obj_t self_in, mp_obj_t tuple_in, struct sockaddr_in *addr) {
    SELF(self_in);

    mp_obj_tuple_t *addr_tuple = MP_OBJ_TO_PTR(tuple_in);
    if (!MP_OBJ_IS_TYPE(tuple_in, &mp_type_tuple) || addr_tuple->len != 2) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_TypeError, "address must be a 2-item tuple"));
    }

    if (!MP_OBJ_IS_STR(addr_tuple->items[0]) || !MP_OBJ_IS_INT(addr_tuple->items[1])) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_TypeError, "address tuple must be (str, int)"));
    }

    memset(addr, 0, sizeof(*addr));

    const char *host = mp_obj_str_get_str(addr_tuple->items[0]);
    short port = mp_obj_get_int(addr_tuple->items[1]);
    port = htons(port);

    struct addrinfo hints;
    struct addrinfo *info;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;

    if (!strcmp(host, HOST_BROADCAST_IP) || !strcmp(host, HOST_BROADCAST_NAME)) {
        // broadcast bind

        addr->sin_addr.s_addr = INADDR_BROADCAST;
    } else if (inet_pton(AF_INET, host, &addr->sin_addr) > 0) {
        // IP address bind, nothing to do
    } else {
        int error;

        if (host[0] == '\0') {
            // blank, use wildcard bind
            hints.ai_socktype = self->type;
            hints.ai_flags = AI_PASSIVE;

            error = getaddrinfo(NULL, "0", &hints, &info);
        } else {
            // hostname bind
            error = getaddrinfo(host, NULL, &hints, &info);
        }

        if (error) {
            nlr_raise(mp_obj_new_exception_msg_varg(&mod_socket_exception_error_type, "getaddrinfo: error %d", error));
        }

        if (info->ai_next) {
            freeaddrinfo(info);
            nlr_raise(mp_obj_new_exception_msg(&mod_socket_exception_error_type, "getaddrinfo: wildcard resolved multiple addresses"));
        }

        memcpy(addr, info->ai_addr, sizeof(struct sockaddr));

        freeaddrinfo(info);
    }

    addr->sin_family = AF_INET;
    addr->sin_port = port;
}

STATIC mp_obj_t _mod_socket_tuple_from_sockaddr(struct sockaddr_in *addr, socklen_t len) {
    char host[NI_MAXHOST];

    int error = getnameinfo((struct sockaddr *) addr, len, host, sizeof(host), NULL, 0, NI_NUMERICHOST);
    if (error) {
        RAISE_ERRNO;
    }

    int port = ntohs(((struct sockaddr_in *) addr)->sin_port);

    mp_obj_tuple_t *tuple = mp_obj_new_tuple(2, NULL);
    tuple->items[0] = mp_obj_new_str(host, strlen(host), false);
    tuple->items[1] = mp_obj_new_int(port);

    return tuple;
}

enum {
    NEW_ARG_FAMILY = 0,
    NEW_ARG_TYPE,
    NEW_ARG_PROTO,
    NEW_ARG_COUNT
};

STATIC mp_obj_t mod_socket_Socket_make_new(const mp_obj_type_t *type_in, size_t n_args, size_t n_kw, const mp_obj_t *args_in) {
    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_family, MP_ARG_INT, {.u_int = AF_INET}},
            {MP_QSTR_type,   MP_ARG_INT, {.u_int = SOCK_STREAM}},
            {MP_QSTR_proto,  MP_ARG_INT, {.u_int = IPPROTO_IP}},
    };

    mp_arg_val_t args[NEW_ARG_COUNT];
    mp_arg_parse_all_kw_array(n_args, n_kw, args_in, NEW_ARG_COUNT, allowed_args, args);

    int family = args[NEW_ARG_FAMILY].u_int;
    int type = args[NEW_ARG_TYPE].u_int;
    int proto = args[NEW_ARG_PROTO].u_int;

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
    obj->type = type;
    obj->proto = proto;

    if (obj->fd == -1) {
        RAISE_ERRNO;
    }

    return obj;
}

STATIC void mod_socket_Socket_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    SELF(self_in);

    mp_printf(print, "<socket fd=%d>", self->fd);
}

STATIC mp_obj_t mod_socket_Socket_accept(mp_obj_t self_in) {
    SELF(self_in);

    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    int client_fd = accept(self->fd, (struct sockaddr *) &client_addr, &addr_len);
    if (client_fd < 0) {
        RAISE_ERRNO;
    }

    mod_socket_Socket_t *client_obj = m_new_obj(mod_socket_Socket_t);
    client_obj->base.type = (mp_obj_t) &mod_socket_Socket_type;
    client_obj->fd = client_fd;
    client_obj->type = self->type;
    client_obj->proto = self->proto;

    mp_obj_tuple_t *ret = mp_obj_new_tuple(2, NULL);
    ret->items[0] = client_obj;
    ret->items[1] = _mod_socket_tuple_from_sockaddr(&client_addr, addr_len);

    return ret;
}

STATIC mp_obj_t mod_socket_Socket_bind(mp_obj_t self_in, mp_obj_t address) {
    SELF(self_in);

    struct sockaddr_in bind_addr;
    _mod_socket_sockaddr_from_tuple(self_in, address, &bind_addr);

    int error = bind(self->fd, (struct sockaddr *) &bind_addr, sizeof(struct sockaddr_in));
    if (error < 0) {
        RAISE_ERRNO;
    }

    return mp_const_none;
}

STATIC mp_obj_t mod_socket_Socket_close(mp_obj_t self_in) {
    SELF(self_in);

    close(self->fd);

    self->fd = -1;

    return mp_const_none;
}

STATIC mp_obj_t mod_socket_Socket_connect(mp_obj_t self_in, mp_obj_t address) {
    SELF(self_in);

    struct sockaddr_in addr;
    _mod_socket_sockaddr_from_tuple(self_in, address, &addr);

    int error = connect(self->fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
    if (error < 0) {
        RAISE_ERRNO;
    }

    return mp_const_none;
}

STATIC mp_obj_t mod_socket_Socket_connect_ex(mp_obj_t self_in, mp_obj_t address) {
    SELF(self_in);

    struct sockaddr_in addr;
    _mod_socket_sockaddr_from_tuple(self_in, address, &addr);

    int error = connect(self->fd, (struct sockaddr *) &addr, sizeof(struct sockaddr_in));
    if (error < 0) {
        return mp_obj_new_int(errno);
    }
    return mp_obj_new_int(0);
}

STATIC mp_obj_t mod_socket_Socket_fileno(mp_obj_t self_in) {
    SELF(self_in);

    return mp_obj_new_int(self->fd);
}

STATIC mp_obj_t mod_socket_Socket_getpeername(mp_obj_t self_in) {
    SELF(self_in);

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int error = getpeername(self->fd, (struct sockaddr *) &addr, &len);
    if (error < 0) {
        RAISE_ERRNO;
    }

    return _mod_socket_tuple_from_sockaddr(&addr, len);
}

STATIC mp_obj_t mod_socket_Socket_getsockname(mp_obj_t self_in) {
    SELF(self_in);

    struct sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int error = getsockname(self->fd, (struct sockaddr *) &addr, &len);
    if (error < 0) {
        RAISE_ERRNO;
    }

    return _mod_socket_tuple_from_sockaddr(&addr, len);
}

enum {
    GET_OPT_SELF = 0,
    GET_OPT_LEVEL,
    GET_OPT_NAME,
    GET_OPT_LEN
};

STATIC mp_obj_t mod_socket_Socket_getsockopt(size_t n_args, const mp_obj_t *args) {
    SELF(args[GET_OPT_SELF]);

    int level = mp_obj_get_int(args[GET_OPT_LEVEL]);
    int name = mp_obj_get_int(args[GET_OPT_NAME]);
    if (n_args == 3) {
        int flag = 0;
        socklen_t size = sizeof(flag);

        int error = getsockopt(self->fd, level, name, (void *) &flag, &size);
        if (error < 0) {
            RAISE_ERRNO;
        }

        return mp_obj_new_int(flag);
    } else {
        socklen_t len = mp_obj_get_int(args[GET_OPT_LEN]);
        if (len <= 0 || len > 1024) {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "buflen out of range. 0 < buflen <= 1024"));
        }

        const byte *buf = malloc(len);
        int error = getsockopt(self->fd, level, name, (void *) buf, &len);
        if (error < 0) {
            free((void *) buf);
            RAISE_ERRNO;
        }

        mp_obj_t ret = mp_obj_new_bytes(buf, len);
        free((void *) buf);
        return ret;
    }
}

STATIC mp_obj_t mod_socket_Socket_ioctl(mp_obj_t self_in, mp_obj_t control, mp_obj_t option) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_listen(mp_obj_t self_in, mp_obj_t backlog) {
    SELF(self_in);

    int bl = mp_obj_get_int(backlog);
    if (bl < 0) {
        bl = 0;
    }

    if (listen(self->fd, bl) < 0) {
        RAISE_ERRNO;
    }

    return mp_const_none;
}

enum {
    RECV_ARG_SELF = 0,
    RECV_ARG_LEN,
    RECV_ARG_FLAGS
};

STATIC mp_obj_t mod_socket_Socket_recv(size_t n_args, const mp_obj_t *args) {
    SELF(args[RECV_ARG_SELF]);

    size_t len = mp_obj_get_int(args[RECV_ARG_LEN]);
    void *buf = malloc(len);

    int flags = 0;
    if (n_args == 3) {
        flags = mp_obj_get_int(args[RECV_ARG_FLAGS]);
    }

    int read = recv(self->fd, buf, len, flags);
    if (read < 0) {
        RAISE_ERRNO;
    }

    return mp_obj_new_bytes(buf, read);
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

enum {
    SEND_ARG_SELF = 0,
    SEND_ARG_STRING,
    SEND_ARG_FLAGS
};

STATIC mp_obj_t mod_socket_Socket_send(size_t n_args, const mp_obj_t *args) {
    SELF(args[SEND_ARG_SELF]);

    mp_buffer_info_t buf_info;
    mp_get_buffer(args[SEND_ARG_STRING], &buf_info, MP_BUFFER_READ);

    int flags = 0;
    if (n_args == 3) {
        flags = mp_obj_get_int(args[SEND_ARG_FLAGS]);
    }

    int sent = send(self->fd, buf_info.buf, buf_info.len, flags);
    if (sent < 0) {
        RAISE_ERRNO;
    }

    return mp_obj_new_int(sent);
}

enum {
    SENDALL_ARG_SELF = 0,
    SENDALL_ARG_STRING,
    SENDALL_ARG_FLAGS
};

STATIC mp_obj_t mod_socket_Socket_sendall(size_t n_args, const mp_obj_t *args) {
    SELF(args[SENDALL_ARG_SELF]);

    mp_buffer_info_t buf_info;
    mp_get_buffer(args[SEND_ARG_STRING], &buf_info, MP_BUFFER_READ);

    int flags = 0;
    if (n_args == 3) {
        flags = mp_obj_get_int(args[SEND_ARG_FLAGS]);
    }

    int sent = 0;

    while (sent < buf_info.len) {
        int res = send(self->fd, buf_info.buf + sent, buf_info.len - sent, flags);

        if (res < 0) {
            RAISE_ERRNO;
        }

        sent += res;
    }

    return mp_const_none;
}

STATIC mp_obj_t mod_socket_Socket_sendto(size_t n_args, const mp_obj_t *args) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_socket_Socket_setblocking(mp_obj_t self_in, mp_obj_t flag) {
    SELF(self_in);
    bool enabled = mp_obj_is_true(flag);

    int flags = fcntl(self->fd, F_GETFL, 0);
    int new_flags;

    if (enabled) {
        new_flags = flags & ~O_NONBLOCK;
    } else {
        new_flags = flags | O_NONBLOCK;
    }

    if (flags != new_flags) {
        fcntl(self->fd, F_SETFL, new_flags);
    }

    return mp_const_none;
}

enum {
    SET_OPT_ARG_SELF = 0,
    SET_OPT_ARG_LEVEL,
    SET_OPT_ARG_NAME,
    SET_OPT_ARG_VALUE
};

STATIC mp_obj_t mod_socket_Socket_setsockopt(size_t n_args, const mp_obj_t *args) {
    SELF(args[SET_OPT_ARG_SELF]);

    int level = mp_obj_get_int(args[SET_OPT_ARG_LEVEL]);
    int name = mp_obj_get_int(args[SET_OPT_ARG_NAME]);
    mp_obj_t value = args[SET_OPT_ARG_VALUE];

    int error;
    if (MP_OBJ_IS_INT(value)) {
        int val = mp_obj_get_int(value);
        error = setsockopt(self->fd, level, name, &val, sizeof(val));
    } else if (MP_OBJ_IS_STR(value)) {
        mp_buffer_info_t buf_info;
        mp_get_buffer(value, &buf_info, MP_BUFFER_READ);

        error = setsockopt(self->fd, level, name, buf_info.buf, buf_info.len);
    } else {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_TypeError, "value must be int or string"));
    }

    if (error < 0) {
        RAISE_ERRNO;
    }

    return mp_const_none;
}

STATIC mp_obj_t mod_socket_Socket_shutdown(mp_obj_t self_in, mp_obj_t how) {
    SELF(self_in);

    int _how = mp_obj_get_int(how);

    int error = shutdown(self->fd, _how);
    if (error < 0) {
        RAISE_ERRNO;
    }

    return mp_const_none;
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
METHOD_OBJ_VAR_N(4, setsockopt);
METHOD_OBJ_N(2, shutdown);

STATIC const mp_map_elem_t mod_socket_Socket_locals_dict_table[] = {
        // Methods
        LOCAL_METHOD(accept),
        LOCAL_METHOD(bind),
        LOCAL_METHOD(close),
        LOCAL_METHOD(connect),
        LOCAL_METHOD(connect_ex),
        LOCAL_METHOD(fileno),
        LOCAL_METHOD(getpeername),
        LOCAL_METHOD(getsockname),
        LOCAL_METHOD(getsockopt),
        LOCAL_METHOD(ioctl),
        LOCAL_METHOD(listen),
        LOCAL_METHOD(recv),
        LOCAL_METHOD(recvfrom),
        LOCAL_METHOD(recvfrom_into),
        LOCAL_METHOD(recv_into),
        LOCAL_METHOD(send),
        LOCAL_METHOD(sendall),
        LOCAL_METHOD(sendto),
        LOCAL_METHOD(setblocking),
        LOCAL_METHOD(setsockopt),
        LOCAL_METHOD(shutdown),
};
STATIC MP_DEFINE_CONST_DICT(mod_socket_Socket_locals_dict, mod_socket_Socket_locals_dict_table);

const mp_obj_type_t mod_socket_Socket_type = {
        {&mp_type_type},
        .name = MP_QSTR_socket,
        .print = mod_socket_Socket_print,
        .make_new = mod_socket_Socket_make_new,
        .locals_dict = (mp_obj_t) &mod_socket_Socket_locals_dict,
};