#include <malloc.h>

#include <3ds.h>

#include "py/runtime.h"

#include "../init_helper.h"

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_citrus_ir_##__n##_obj, mod_citrus_ir_##__n)
#define METHOD_OBJ_VAR(__min, __max, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_citrus_ir_##__n##_obj, __min, __max, mod_citrus_ir_##__n)
#define METHOD_OBJ_KW(__min, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_KW(mod_citrus_ir_##__n##_obj, __min, mod_citrus_ir_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_ir_##__n##_obj}

#define DEFAULT_MEM_SIZE 0x2000

static int _mod_citrus_ir_is_init = 0;

static void *_mod_citrus_ir_memory;

STATIC mp_obj_t mod_citrus_ir_init(size_t n_args, const mp_obj_t *args) {
    INIT_ONCE(_mod_citrus_ir_is_init);

    size_t size = DEFAULT_MEM_SIZE;
    if (n_args == 1) {
        size = mp_obj_get_int(args[0]);
    }

    _mod_citrus_ir_memory = malloc(size);
    return mp_obj_new_int(iruInit(_mod_citrus_ir_memory, size));
}

mp_obj_t mod_citrus_ir_exit(void) {
    EXIT_ONCE(_mod_citrus_ir_is_init);

    iruExit();
    free(_mod_citrus_ir_memory);

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ir_send(mp_obj_t data) {
    mp_buffer_info_t buf_info;
    mp_get_buffer(data, &buf_info, MP_BUFFER_READ);

    return mp_obj_new_int(iruSendData(buf_info.buf, buf_info.len, true));
}

enum {
    RECV_ARG_SIZE = 0,
    RECV_ARG_FLAGS,
    RECV_ARG_COUNT
};

STATIC mp_obj_t mod_citrus_ir_recv(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_count, MP_ARG_INT | MP_ARG_REQUIRED, {}},
            {MP_QSTR_flags, MP_ARG_INT,                   {.u_int = 0}},
    };

    mp_arg_val_t args[RECV_ARG_COUNT];
    mp_arg_parse_all((mp_uint_t) n_args, pos_args, kw_args, RECV_ARG_COUNT, allowed_args, args);

    u32 size = args[RECV_ARG_SIZE].u_int;
    u8 flags = args[RECV_ARG_FLAGS].u_int;

    u8 *buffer = malloc(size);
    u32 received;
    Result res = iruRecvData(buffer, size, flags, &received, true);

    if (R_FAILED(res)) {
        free(buffer);
        return mp_obj_new_int(-1);
    }

    mp_obj_t bytes = mp_obj_new_bytes(buffer, received);
    free(buffer);

    return bytes;
}

// TODO: ir_send_background
// TODO: ir_recv_background

STATIC mp_obj_t mod_citrus_ir_set_bit_rate(mp_obj_t rate) {
    u8 value = mp_obj_get_int(rate);

    return mp_obj_new_int(IRU_SetBitRate(value));
}

STATIC mp_obj_t mod_citrus_ir_get_bit_rate(void) {
    u8 rate;
    IRU_GetBitRate(&rate);

    return mp_obj_new_int(rate);
}

STATIC mp_obj_t mod_citrus_ir_set_led_state(mp_obj_t state) {
    u32 value = mp_obj_get_int(state);

    return mp_obj_new_int(IRU_SetIRLEDState(value));
}

STATIC mp_obj_t mod_citrus_ir_get_led_state(void) {
    u32 state;
    IRU_GetIRLEDRecvState(&state);

    return mp_obj_new_int(state);
}

METHOD_OBJ_VAR(0, 1, init);
METHOD_OBJ_N(0, exit);
METHOD_OBJ_N(1, send);
METHOD_OBJ_KW(1, recv);
METHOD_OBJ_N(1, set_bit_rate);
METHOD_OBJ_N(0, get_bit_rate);
METHOD_OBJ_N(1, set_led_state);
METHOD_OBJ_N(0, get_led_state);

STATIC const mp_rom_map_elem_t mp_module_citrus_ir_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ir)},

        // Methods
        LOCAL_METHOD(init),
        LOCAL_METHOD(exit),
        LOCAL_METHOD(send),
        LOCAL_METHOD(recv),
        LOCAL_METHOD(set_bit_rate),
        LOCAL_METHOD(get_bit_rate),
        LOCAL_METHOD(set_led_state),
        LOCAL_METHOD(get_led_state),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_ir_globals, mp_module_citrus_ir_globals_table);

const mp_obj_module_t mp_module_citrus_ir = {
        .base = {&mp_type_module},
        .name = MP_QSTR_ir,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_ir_globals,
};