#include <3ds.h>

#include "py/runtime.h"

#define RES(r) int res = mp_obj_get_int(r)

STATIC mp_obj_t mod_citrus_result_succeeded(mp_obj_t result) {
    RES(result);
    return mp_obj_new_bool(R_SUCCEEDED(res));
}

STATIC mp_obj_t mod_citrus_result_failed(mp_obj_t result) {
    RES(result);
    return mp_obj_new_bool(R_FAILED(res));
}

STATIC mp_obj_t mod_citrus_result_level(mp_obj_t result) {
    RES(result);
    return mp_obj_new_int(R_LEVEL(res));
}

STATIC mp_obj_t mod_citrus_result_summary(mp_obj_t result) {
    RES(result);
    return mp_obj_new_int(R_SUMMARY(res));
}

STATIC mp_obj_t mod_citrus_result_module(mp_obj_t result) {
    RES(result);
    return mp_obj_new_int(R_MODULE(res));
}

STATIC mp_obj_t mod_citrus_result_description(mp_obj_t result) {
    RES(result);
    return mp_obj_new_int(R_DESCRIPTION(res));
}

STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_result_succeeded_obj, mod_citrus_result_succeeded);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_result_failed_obj, mod_citrus_result_failed);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_result_level_obj, mod_citrus_result_level);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_result_summary_obj, mod_citrus_result_summary);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_result_module_obj, mod_citrus_result_module);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_result_description_obj, mod_citrus_result_description);

STATIC const mp_rom_map_elem_t mp_module_citrus_result_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__),                  MP_ROM_QSTR(MP_QSTR_result)},

        // Functions
        {MP_ROM_QSTR(MP_QSTR_succeeded),                 MP_ROM_PTR(&mod_citrus_result_succeeded_obj)},
        {MP_ROM_QSTR(MP_QSTR_failed),                    MP_ROM_PTR(&mod_citrus_result_failed_obj)},
        {MP_ROM_QSTR(MP_QSTR_level),                     MP_ROM_PTR(&mod_citrus_result_level_obj)},
        {MP_ROM_QSTR(MP_QSTR_summary),                   MP_ROM_PTR(&mod_citrus_result_summary_obj)},
        {MP_ROM_QSTR(MP_QSTR_module),                    MP_ROM_PTR(&mod_citrus_result_module_obj)},
        {MP_ROM_QSTR(MP_QSTR_description),               MP_ROM_PTR(&mod_citrus_result_description_obj)},

        // levels
        {MP_ROM_QSTR(MP_QSTR_LEVEL_SUCCESS),             MP_ROM_INT(RL_SUCCESS)},
        {MP_ROM_QSTR(MP_QSTR_LEVEL_INFO),                MP_ROM_INT(RL_INFO)},
        {MP_ROM_QSTR(MP_QSTR_LEVEL_FATAL),               MP_ROM_INT(RL_FATAL)},
        {MP_ROM_QSTR(MP_QSTR_LEVEL_RESET),               MP_ROM_INT(RL_RESET)},
        {MP_ROM_QSTR(MP_QSTR_LEVEL_REINITIALIZE),        MP_ROM_INT(RL_REINITIALIZE)},
        {MP_ROM_QSTR(MP_QSTR_LEVEL_USAGE),               MP_ROM_INT(RL_USAGE)},
        {MP_ROM_QSTR(MP_QSTR_LEVEL_PERMANENT),           MP_ROM_INT(RL_PERMANENT)},
        {MP_ROM_QSTR(MP_QSTR_LEVEL_TEMPORARY),           MP_ROM_INT(RL_TEMPORARY)},
        {MP_ROM_QSTR(MP_QSTR_LEVEL_STATUS),              MP_ROM_INT(RL_STATUS)},

        // summaries
        {MP_ROM_QSTR(MP_QSTR_SUMMARY_SUCCESS),           MP_ROM_INT(RS_SUCCESS)},
        {MP_ROM_QSTR(MP_QSTR_SUMMARY_NOP),               MP_ROM_INT(RS_NOP)},
        {MP_ROM_QSTR(MP_QSTR_SUMMARY_WOULDBLOCK),        MP_ROM_INT(RS_WOULDBLOCK)},
        {MP_ROM_QSTR(MP_QSTR_SUMMARY_OUTOFRESOURCE),     MP_ROM_INT(RS_OUTOFRESOURCE)},
        {MP_ROM_QSTR(MP_QSTR_SUMMARY_NOTFOUND),          MP_ROM_INT(RS_NOTFOUND)},
        {MP_ROM_QSTR(MP_QSTR_SUMMARY_INVALIDSTATE),      MP_ROM_INT(RS_INVALIDSTATE)},
        {MP_ROM_QSTR(MP_QSTR_SUMMARY_NOTSUPPORTED),      MP_ROM_INT(RS_NOTSUPPORTED)},
        {MP_ROM_QSTR(MP_QSTR_SUMMARY_INVALIDARG),        MP_ROM_INT(RS_INVALIDARG)},
        {MP_ROM_QSTR(MP_QSTR_SUMMARY_WRONGARG),          MP_ROM_INT(RS_WRONGARG)},
        {MP_ROM_QSTR(MP_QSTR_SUMMARY_CANCELED),          MP_ROM_INT(RS_CANCELED)},
        {MP_ROM_QSTR(MP_QSTR_SUMMARY_STATUSCHANGED),     MP_ROM_INT(RS_STATUSCHANGED)},
        {MP_ROM_QSTR(MP_QSTR_SUMMARY_INTERNAL),          MP_ROM_INT(RS_INTERNAL)},
        {MP_ROM_QSTR(MP_QSTR_SUMMARY_INVALIDRESVAL),     MP_ROM_INT(RS_INVALIDRESVAL)},

        // descriptions
        {MP_ROM_QSTR(MP_QSTR_DESC_SUCCESS),              MP_ROM_INT(RD_SUCCESS)},
        {MP_ROM_QSTR(MP_QSTR_DESC_INVALID_RESULT_VALUE), MP_ROM_INT(RD_INVALID_RESULT_VALUE)},
        {MP_ROM_QSTR(MP_QSTR_DESC_TIMEOUT),              MP_ROM_INT(RD_TIMEOUT)},
        {MP_ROM_QSTR(MP_QSTR_DESC_OUT_OF_RANGE),         MP_ROM_INT(RD_OUT_OF_RANGE)},
        {MP_ROM_QSTR(MP_QSTR_DESC_ALREADY_EXISTS),       MP_ROM_INT(RD_ALREADY_EXISTS)},
        {MP_ROM_QSTR(MP_QSTR_DESC_CANCEL_REQUESTED),     MP_ROM_INT(RD_CANCEL_REQUESTED)},
        {MP_ROM_QSTR(MP_QSTR_DESC_NOT_FOUND),            MP_ROM_INT(RD_NOT_FOUND)},
        {MP_ROM_QSTR(MP_QSTR_DESC_ALREADY_INITIALIZED),  MP_ROM_INT(RD_ALREADY_INITIALIZED)},
        {MP_ROM_QSTR(MP_QSTR_DESC_NOT_INITIALIZED_),     MP_ROM_INT(RD_NOT_INITIALIZED)},
        {MP_ROM_QSTR(MP_QSTR_DESC_INVALID_HANDLE),       MP_ROM_INT(RD_INVALID_HANDLE)},
        {MP_ROM_QSTR(MP_QSTR_DESC_INVALID_POINTER),      MP_ROM_INT(RD_INVALID_POINTER)},
        {MP_ROM_QSTR(MP_QSTR_DESC_INVALID_ADDRESS),      MP_ROM_INT(RD_INVALID_ADDRESS)},
        {MP_ROM_QSTR(MP_QSTR_DESC_NOT_IMPLEMENTED),      MP_ROM_INT(RD_NOT_IMPLEMENTED)},
        {MP_ROM_QSTR(MP_QSTR_DESC_OUT_OF_MEMORY),        MP_ROM_INT(RD_OUT_OF_MEMORY)},
        {MP_ROM_QSTR(MP_QSTR_DESC_MISALIGNED_SIZE),      MP_ROM_INT(RD_MISALIGNED_SIZE)},
        {MP_ROM_QSTR(MP_QSTR_DESC_MISALIGNED_ADDREss),   MP_ROM_INT(RD_MISALIGNED_ADDRESS)},
        {MP_ROM_QSTR(MP_QSTR_DESC_BUSY),                 MP_ROM_INT(RD_BUSY)},
        {MP_ROM_QSTR(MP_QSTR_DESC_NO_DATA),              MP_ROM_INT(RD_NO_DATA)},
        {MP_ROM_QSTR(MP_QSTR_DESC_INVALID_COMBINATION),  MP_ROM_INT(RD_INVALID_COMBINATION)},
        {MP_ROM_QSTR(MP_QSTR_DESC_INVALID_ENUM_VALUE),   MP_ROM_INT(RD_INVALID_ENUM_VALUE)},
        {MP_ROM_QSTR(MP_QSTR_DESC_INVALID_SIZE),         MP_ROM_INT(RD_INVALID_SIZE)},
        {MP_ROM_QSTR(MP_QSTR_DESC_ALREADY_DONE),         MP_ROM_INT(RD_ALREADY_DONE)},
        {MP_ROM_QSTR(MP_QSTR_DESC_NOT_AUTHORIZED),       MP_ROM_INT(RD_NOT_AUTHORIZED)},
        {MP_ROM_QSTR(MP_QSTR_DESC_TOO_LARGE),            MP_ROM_INT(RD_TOO_LARGE)},
        {MP_ROM_QSTR(MP_QSTR_DESC_INVALID_SELECTION),    MP_ROM_INT(RD_INVALID_SELECTION)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_result_globals, mp_module_citrus_result_globals_table);

const mp_obj_module_t mp_module_citrus_result = {
        .base = {&mp_type_module},
        .name = MP_QSTR_result,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_result_globals,
};