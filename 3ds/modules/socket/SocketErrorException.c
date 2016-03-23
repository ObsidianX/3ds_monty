#include "py/runtime.h"
#include "py/objexcept.h"

STATIC void mod_socket_exception_error_print(const mp_print_t *print, mp_obj_t o_in, mp_print_kind_t kind) {
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

STATIC void mod_socket_exception_error_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
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

const mp_obj_type_t mod_socket_exception_error_type = {
        {&mp_type_type},
        .name = MP_QSTR_error,
        .print = mod_socket_exception_error_print,
        .make_new = mp_obj_exception_make_new,
        .attr = mod_socket_exception_error_attr,
        .bases_tuple = (mp_obj_tuple_t *) (mp_rom_obj_tuple_t *) &mp_type_BaseException_base_tuple,
};