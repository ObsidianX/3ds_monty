#include <3ds.h>

#include "py/runtime.h"

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_citrus_result_##__n##_obj, mod_citrus_result_##__n)
#define METHOD_OBJ_KW(__min, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_KW(mod_citrus_result_##__n##_obj, __min, mod_citrus_result_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_result_##__n##_obj}
#define LOCAL_INT(__n, __v) \
    {MP_ROM_QSTR(MP_QSTR_##__n), MP_ROM_INT(__v)}

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

METHOD_OBJ_N(1, succeeded);
METHOD_OBJ_N(1, failed);
METHOD_OBJ_N(1, level);
METHOD_OBJ_N(1, summary);
METHOD_OBJ_N(1, module);
METHOD_OBJ_N(1, description);

STATIC const mp_rom_map_elem_t mp_module_citrus_result_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_result)},

        // Functions
        LOCAL_METHOD(succeeded),
        LOCAL_METHOD(failed),
        LOCAL_METHOD(level),
        LOCAL_METHOD(summary),
        LOCAL_METHOD(module),
        LOCAL_METHOD(description),

        // levels
        LOCAL_INT(LEVEL_SUCCESS, RL_SUCCESS),
        LOCAL_INT(LEVEL_INFO, RL_INFO),
        LOCAL_INT(LEVEL_FATAL, RL_FATAL),
        LOCAL_INT(LEVEL_RESET, RL_RESET),
        LOCAL_INT(LEVEL_REINITIALIZE, RL_REINITIALIZE),
        LOCAL_INT(LEVEL_USAGE, RL_USAGE),
        LOCAL_INT(LEVEL_PERMANENT, RL_PERMANENT),
        LOCAL_INT(LEVEL_TEMPORARY, RL_TEMPORARY),
        LOCAL_INT(LEVEL_STATUS, RL_STATUS),

        // summaries
        LOCAL_INT(SUMMARY_SUCCESS, RS_SUCCESS),
        LOCAL_INT(SUMMARY_NOP, RS_NOP),
        LOCAL_INT(SUMMARY_WOULDBLOCK, RS_WOULDBLOCK),
        LOCAL_INT(SUMMARY_OUTOFRESOURCE, RS_OUTOFRESOURCE),
        LOCAL_INT(SUMMARY_NOTFOUND, RS_NOTFOUND),
        LOCAL_INT(SUMMARY_INVALIDSTATE, RS_INVALIDSTATE),
        LOCAL_INT(SUMMARY_NOTSUPPORTED, RS_NOTSUPPORTED),
        LOCAL_INT(SUMMARY_INVALIDARG, RS_INVALIDARG),
        LOCAL_INT(SUMMARY_WRONGARG, RS_WRONGARG),
        LOCAL_INT(SUMMARY_CANCELED, RS_CANCELED),
        LOCAL_INT(SUMMARY_STATUSCHANGED, RS_STATUSCHANGED),
        LOCAL_INT(SUMMARY_INTERNAL, RS_INTERNAL),
        LOCAL_INT(SUMMARY_INVALIDRESVAL, RS_INVALIDRESVAL),

        // descriptions
        LOCAL_INT(DESC_SUCCESS, RD_SUCCESS),
        LOCAL_INT(DESC_INVALID_RESULT_VALUE, RD_INVALID_RESULT_VALUE),
        LOCAL_INT(DESC_TIMEOUT, RD_TIMEOUT),
        LOCAL_INT(DESC_OUT_OF_RANGE, RD_OUT_OF_RANGE),
        LOCAL_INT(DESC_ALREADY_EXISTS, RD_ALREADY_EXISTS),
        LOCAL_INT(DESC_CANCEL_REQUESTED, RD_CANCEL_REQUESTED),
        LOCAL_INT(DESC_NOT_FOUND, RD_NOT_FOUND),
        LOCAL_INT(DESC_ALREADY_INITIALIZED, RD_ALREADY_INITIALIZED),
        LOCAL_INT(DESC_NOT_INITIALIZED, RD_NOT_INITIALIZED),
        LOCAL_INT(DESC_INVALID_HANDLE, RD_INVALID_HANDLE),
        LOCAL_INT(DESC_INVALID_POINTER, RD_INVALID_POINTER),
        LOCAL_INT(DESC_INVALID_ADDRESS, RD_INVALID_ADDRESS),
        LOCAL_INT(DESC_NOT_IMPLEMENTED, RD_NOT_IMPLEMENTED),
        LOCAL_INT(DESC_OUT_OF_MEMORY, RD_OUT_OF_MEMORY),
        LOCAL_INT(DESC_MISALIGNED_SIZE, RD_MISALIGNED_SIZE),
        LOCAL_INT(DESC_MISALIGNED_ADDREss, RD_MISALIGNED_ADDRESS),
        LOCAL_INT(DESC_BUSY, RD_BUSY),
        LOCAL_INT(DESC_NO_DATA, RD_NO_DATA),
        LOCAL_INT(DESC_INVALID_COMBINATION, RD_INVALID_COMBINATION),
        LOCAL_INT(DESC_INVALID_ENUM_VALUE, RD_INVALID_ENUM_VALUE),
        LOCAL_INT(DESC_INVALID_SIZE, RD_INVALID_SIZE),
        LOCAL_INT(DESC_ALREADY_DONE, RD_ALREADY_DONE),
        LOCAL_INT(DESC_NOT_AUTHORIZED, RD_NOT_AUTHORIZED),
        LOCAL_INT(DESC_TOO_LARGE, RD_TOO_LARGE),
        LOCAL_INT(DESC_INVALID_SELECTION, RD_INVALID_SELECTION),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_result_globals, mp_module_citrus_result_globals_table);

const mp_obj_module_t mp_module_citrus_result = {
        .base = {&mp_type_module},
        .name = MP_QSTR_result,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_result_globals,
};