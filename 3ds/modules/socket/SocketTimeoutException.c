#include "py/runtime.h"
#include "py/objexcept.h"

STATIC mp_obj_t mod_socket_exception_timeout_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 0, MP_OBJ_FUN_ARGS_MAX, false);
    mp_obj_exception_t *o = m_new_obj_var_maybe(mp_obj_exception_t, mp_obj_t, 0);
    if (o == NULL) {
        // Couldn't allocate heap memory; use local data instead.
        o = &MP_STATE_VM(mp_emergency_exception_obj);
        // We can't store any args.
        o->args = (mp_obj_tuple_t *) &mp_const_empty_tuple_obj;
    } else {
        o->args = MP_OBJ_TO_PTR(mp_obj_new_tuple(n_args, args));
    }
    o->base.type = type;
    o->traceback_data = NULL;
    return MP_OBJ_FROM_PTR(o);
}

STATIC void mod_socket_exception_timeout_print(const mp_print_t *print, mp_obj_t o_in, mp_print_kind_t kind) {
    mp_obj_exception_t *o = MP_OBJ_TO_PTR(o_in);
    mp_print_kind_t k = kind & ~PRINT_EXC_SUBCLASS;
    bool is_subclass = kind & PRINT_EXC_SUBCLASS;
    if (!is_subclass && (k == PRINT_REPR || k == PRINT_EXC)) {
        mp_print_str(print, qstr_str(o->base.type->name));
    }

    if (k == PRINT_EXC) {
        mp_print_str(print, ": ");
    }

    if (k == PRINT_STR || k == PRINT_EXC) {
        if (o->args == NULL || o->args->len == 0) {
            mp_print_str(print, "");
            return;
        } else if (o->args->len == 1) {
            mp_obj_print_helper(print, o->args->items[0], PRINT_STR);
            return;
        }
    }
    mp_obj_tuple_print(print, MP_OBJ_FROM_PTR(o->args), kind);
}

STATIC void mod_socket_exception_timeout_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    if (dest[0] != MP_OBJ_NULL) {
        // load only
        return;
    }

    mp_obj_exception_t *self = MP_OBJ_TO_PTR(self_in);
    if (attr == MP_QSTR_args) {
        dest[0] = MP_OBJ_FROM_PTR(self->args);
    } else if (self->base.type == &mp_type_StopIteration && attr == MP_QSTR_value) {
        dest[0] = mp_obj_exception_get_value(self_in);
    }
}

STATIC const mp_rom_obj_tuple_t mp_type_BaseException_base_tuple = {{&mp_type_tuple}, 1, {MP_ROM_PTR(&mp_type_BaseException)}};

const mp_obj_type_t mod_socket_exception_timeout_type = {
        {&mp_type_type},
        .name = MP_QSTR_timeout,
        .print = mod_socket_exception_timeout_print,
        .make_new = mod_socket_exception_timeout_make_new,
        .attr = mod_socket_exception_timeout_attr,
        .bases_tuple = (mp_obj_tuple_t *) (mp_rom_obj_tuple_t *) &mp_type_BaseException_base_tuple,
};