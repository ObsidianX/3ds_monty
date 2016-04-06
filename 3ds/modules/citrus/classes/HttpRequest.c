#include <string.h>

#include <3ds.h>

#include "py/runtime.h"
#include "py/objstr.h"
#include "py/misc.h"

#include "../helpers.h"

#define SELF(src) mod_citrus_httpc_Request_t *self = src;

#define ATTR_METHOD(__n) \
    if(!strcmp(name, qstr_str(MP_QSTR_##__n))) { \
        dest[0] = (mp_obj_t) &mod_citrus_httpc_Request_##__n##_obj; \
    }

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_citrus_httpc_Request_##__n##_obj, mod_citrus_httpc_Request_##__n)
#define METHOD_OBJ_VAR_N(__num, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_citrus_httpc_Request_##__n##_obj, __num, __num, mod_citrus_httpc_Request_##__n)
#define METHOD_OBJ_VAR(__min, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_VAR(mod_citrus_httpc_Request_##__n##_obj, __min, mod_citrus_httpc_Request_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_httpc_Request_##__n##_obj}

#define DEFAULT_HEADER_BUFFER_SIZE 1024

const mp_obj_type_t mod_citrus_httpc_Request_type;

typedef struct {
    mp_obj_base_t base;

    httpcContext context;
    Result last_result;
} mod_citrus_httpc_Request_t;

enum {
    NEW_ARG_URL = 0,
    NEW_ARG_METHOD,
    NEW_ARG_USE_PROXY,
    NEW_ARG_COUNT
};

STATIC mp_obj_t mod_citrus_httpc_Request_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in) {
    mp_arg_check_num(n_args, n_kw, 1, NEW_ARG_COUNT, true);

    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_url,       MP_ARG_OBJ | MP_ARG_REQUIRED, {}},
            {MP_QSTR_method,    MP_ARG_INT,                   {.u_int = HTTPC_METHOD_GET}},
            {MP_QSTR_use_proxy, MP_ARG_BOOL,                  {.u_bool = false}},
    };

    mp_arg_val_t args[NEW_ARG_COUNT];
    mp_arg_parse_all_kw_array(n_args, n_kw, args_in, NEW_ARG_COUNT, allowed_args, args);

    mod_citrus_httpc_Request_t *obj = m_new_obj_with_finaliser(mod_citrus_httpc_Request_t);
    obj->base.type = (mp_obj_t) &mod_citrus_httpc_Request_type;

    if (!MP_OBJ_IS_STR(args[NEW_ARG_URL].u_obj)) {
        nlr_raise(mp_obj_new_exception(&mp_type_TypeError));
    }

    const char *url = mp_obj_str_get_str(args[NEW_ARG_URL].u_obj);
    int method = _mod_citrus_httpc_get_request_method(args[NEW_ARG_METHOD].u_int);
    bool proxy = args[NEW_ARG_USE_PROXY].u_bool;

    httpcOpenContext(&obj->context, method, (char *) url, proxy ? 1 : 0);

    return obj;
}

STATIC void mod_citrus_httpc_Request_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    SELF(self_in);

    mp_printf(print, "<Request context=%p>", &self->context);
}

STATIC mp_obj_t mod_citrus_httpc_Request_close(mp_obj_t self_in) {
    SELF(self_in);

    self->last_result = httpcCloseContext(&self->context);

    return mp_obj_new_int(self->last_result);
}

STATIC mp_obj_t mod_citrus_httpc_Request_add_header(mp_obj_t self_in, mp_obj_t name, mp_obj_t value) {
    SELF(self_in);

    const char *_name = mp_obj_str_get_str(name);
    const char *_value = mp_obj_str_get_str(value);

    self->last_result = httpcAddRequestHeaderField(&self->context, (char *) _name, (char *) _value);

    return mp_obj_new_int(self->last_result);
}

STATIC mp_obj_t mod_citrus_httpc_Request_add_post_string(mp_obj_t self_in, mp_obj_t name, mp_obj_t value) {
    SELF(self_in);

    const char *_name = mp_obj_str_get_str(name);
    const char *_value = mp_obj_str_get_str(value);

    self->last_result = httpcAddPostDataAscii(&self->context, (char *) _name, (char *) _value);

    return mp_obj_new_int(self->last_result);
}

STATIC mp_obj_t mod_citrus_httpc_Request_add_post_bytes(mp_obj_t self_in, mp_obj_t value) {
    SELF(self_in);

    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(value, &bufinfo, MP_BUFFER_READ);

    self->last_result = httpcAddPostDataRaw(&self->context, bufinfo.buf, bufinfo.len);

    return mp_obj_new_int(self->last_result);
}

STATIC mp_obj_t mod_citrus_httpc_Request_begin_request(mp_obj_t self_in) {
    SELF(self_in);

    self->last_result = httpcBeginRequest(&self->context);

    return mp_obj_new_int(self->last_result);
}

STATIC mp_obj_t mod_citrus_httpc_Request_receive_data(mp_obj_t self_in, mp_obj_t size) {
    SELF(self_in);

    if (!mp_obj_is_integer(size)) {
        nlr_raise(mp_obj_new_exception(&mp_type_TypeError));
    }
    int _size = mp_obj_get_int(size);
    byte *buf = m_new(byte, _size);

    self->last_result = httpcReceiveData(&self->context, buf, _size);

    if (!R_SUCCEEDED(self->last_result)) {
        m_del(byte, buf, _size);
        return mp_const_none;
    }

    mp_obj_t ret = mp_obj_new_str_of_type(&mp_type_bytes, buf, _size);
    m_del(byte, buf, _size);

    return ret;
}

STATIC mp_obj_t mod_citrus_httpc_Request_get_response_header(size_t n_args, const mp_obj_t *args) {
    SELF(args[0]);

    const char *_name = mp_obj_get_type_str(args[1]);

    int buffer_size = DEFAULT_HEADER_BUFFER_SIZE;
    if (n_args == 3) {
        buffer_size = mp_obj_get_int(args[2]);
    }

    char *value = m_new(char, buffer_size);
    self->last_result = httpcGetResponseHeader(&self->context, (char *) _name, value, sizeof(value));

    if (!R_SUCCEEDED(self->last_result)) {
        return mp_const_none;
    }

    return mp_obj_new_str((const char *) value, strlen(value), false);
}

STATIC mp_obj_t mod_citrus_httpc_Request_download_data(mp_obj_t self_in, mp_obj_t size) {
    SELF(self_in);

    u32 _size = mp_obj_get_int(size);
    u32 downloaded = 0;
    byte *buf = m_new(byte, _size);

    self->last_result = httpcDownloadData(&self->context, buf, _size, &downloaded);

    if (!R_SUCCEEDED(self->last_result)) {
        m_del(byte, buf, _size);
        return mp_const_none;
    }

    mp_obj_t ret = mp_obj_new_str_of_type(&mp_type_bytes, buf, _size);
    m_del(byte, buf, _size);

    return ret;
}

METHOD_OBJ_N(1, close);
METHOD_OBJ_N(3, add_header);
METHOD_OBJ_N(3, add_post_string);
METHOD_OBJ_N(2, add_post_bytes);
METHOD_OBJ_N(1, begin_request);
METHOD_OBJ_N(2, receive_data);
METHOD_OBJ_VAR(2, get_response_header);
METHOD_OBJ_N(2, download_data);

STATIC void mod_citrus_httpc_Request_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    SELF(self_in);
    const char *name = qstr_str(attr);
    bool load = dest[0] == MP_OBJ_NULL;

    // attributes are read-only
    if (!load) {
        return;
    }

    // Methods
    if (!strcmp(name, qstr_str(MP_QSTR___del__))) {
        dest[0] = (mp_obj_t) &mod_citrus_httpc_Request_close_obj;
    }
    else ATTR_METHOD(close)
    else ATTR_METHOD(add_header)
    else ATTR_METHOD(add_post_string)
    else ATTR_METHOD(add_post_bytes)
    else ATTR_METHOD(begin_request)
    else ATTR_METHOD(receive_data)
    else ATTR_METHOD(get_response_header)
    else ATTR_METHOD(download_data)

    if (dest[0] != MP_OBJ_NULL) {
        dest[1] = self_in;
        return;
    }

    if (!strcmp(name, qstr_str(MP_QSTR_request_state))) {
        HTTPC_RequestStatus status;
        self->last_result = httpcGetRequestState(&self->context, &status);

        if (!R_SUCCEEDED(self->last_result)) {
            return;
        }

        dest[0] = mp_obj_new_int(status);
    } else if (!strcmp(name, qstr_str(MP_QSTR_response_status_code))) {
        u32 status = 0;
        self->last_result = httpcGetResponseStatusCode(&self->context, &status, 0 /* delay: unused 1.1.0 */);

        if (!R_SUCCEEDED(self->last_result)) {
            return;
        }

        dest[0] = mp_obj_new_int(status);
    } else if (!strcmp(name, qstr_str(MP_QSTR_bytes_downloaded)) || !strcmp(name, qstr_str(MP_QSTR_download_size))) {
        u32 downloaded_size = 0;
        u32 content_size = 0;
        self->last_result = httpcGetDownloadSizeState(&self->context, &downloaded_size, &content_size);

        if (!R_SUCCEEDED(self->last_result)) {
            return;
        }

        if (!strcmp(name, qstr_str(MP_QSTR_bytes_downloaded))) {
            dest[0] = mp_obj_new_int(downloaded_size);
        } else if (!strcmp(name, qstr_str(MP_QSTR_download_size))) {
            dest[0] = mp_obj_new_int(content_size);
        }
    } else if (!strcmp(name, qstr_str(MP_QSTR_last_result))) {
        dest[0] = mp_obj_new_int(self->last_result);
    }
}

STATIC const mp_map_elem_t mod_citrus_httpc_Request_locals_dict_table[] = {
        // Methods
        {MP_OBJ_NEW_QSTR(MP_QSTR___del__), (mp_obj_t) &mod_citrus_httpc_Request_close_obj},
        LOCAL_METHOD(close),
        LOCAL_METHOD(add_header),
        LOCAL_METHOD(add_post_string),
        LOCAL_METHOD(add_post_bytes),
        LOCAL_METHOD(begin_request),
        LOCAL_METHOD(receive_data),
        LOCAL_METHOD(get_response_header),
        LOCAL_METHOD(download_data),

        // Attributes (all int's)
        // request_state
        // response_status_code
        // last_result
        // bytes_downloaded
        // download_size
};
STATIC MP_DEFINE_CONST_DICT(mod_citrus_httpc_Request_locals_dict, mod_citrus_httpc_Request_locals_dict_table);

const mp_obj_type_t mod_citrus_httpc_Request_type = {
        {&mp_type_type},
        .name = MP_QSTR_PrintConsole,
        .print = mod_citrus_httpc_Request_print,
        .make_new = mod_citrus_httpc_Request_make_new,
        .locals_dict = (mp_obj_t) &mod_citrus_httpc_Request_locals_dict,
        .attr = (mp_obj_t) &mod_citrus_httpc_Request_attr,
};