#include <3ds.h>
#include <3ds/services/nfc.h>

#include "py/runtime.h"

#include "../init_helper.h"

#define METHOD_OBJ(__n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_nfc_##__n##_obj, mod_citrus_nfc_##__n)
#define METHOD_OBJ_VAR(__min, __max, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_citrus_nfc_##__n##_obj, __min, __max, mod_citrus_nfc_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_nfc_##__n##_obj}
#define LOCAL_INT(__name, __val) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__name), MP_ROM_INT(__val)}

static int _mod_citrus_nfc_is_init = 0;

STATIC mp_obj_t mod_citrus_nfc_init(void) {
    INIT_ONCE(_mod_citrus_nfc_is_init);

    return mp_obj_new_int(nfcInit());
}

mp_obj_t mod_citrus_nfc_exit(void) {
    EXIT_ONCE(_mod_citrus_nfc_is_init);

    nfcExit();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_nfc_start_communication(void) {
    return mp_obj_new_int(NFC_StartCommunication());
}

STATIC mp_obj_t mod_citrus_nfc_stop_communication(void) {
    return mp_obj_new_int(NFC_StopCommunication());
}

STATIC mp_obj_t mod_citrus_nfc_start_tag_scan(size_t n_args, const mp_obj_t *args) {
    u16 unknown = 0;
    if (n_args == 1) {
        unknown = mp_obj_get_int(args[0]);
    }
    return mp_obj_new_int(NFC_StartTagScanning(unknown));
}

STATIC mp_obj_t mod_citrus_nfc_stop_tag_scan(void) {
    return mp_obj_new_int(NFC_StopTagScanning());
}

STATIC mp_obj_t mod_citrus_nfc_load_amiibo_data(void) {
    return mp_obj_new_int(NFC_LoadAmiiboData());
}

STATIC mp_obj_t mod_citrus_nfc_reset_tag_scan(void) {
    return mp_obj_new_int(NFC_ResetTagScanState());
}

STATIC mp_obj_t mod_citrus_nfc_get_tag_state(void) {
    u8 out;
    NFC_GetTagState(&out);

    return mp_obj_new_int(out);
}

METHOD_OBJ(init);
METHOD_OBJ(exit);
METHOD_OBJ(start_communication);
METHOD_OBJ(stop_communication);
METHOD_OBJ_VAR(0, 1, start_tag_scan);
METHOD_OBJ(stop_tag_scan);
METHOD_OBJ(load_amiibo_data);
METHOD_OBJ(reset_tag_scan);
METHOD_OBJ(get_tag_state);

STATIC const mp_rom_map_elem_t mp_module_citrus_nfc_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_nfc)},

        // Functions
        LOCAL_METHOD(init),
        LOCAL_METHOD(exit),
        LOCAL_METHOD(start_communication),
        LOCAL_METHOD(stop_communication),
        LOCAL_METHOD(start_tag_scan),
        LOCAL_METHOD(stop_tag_scan),
        LOCAL_METHOD(load_amiibo_data),
        LOCAL_METHOD(reset_tag_scan),
        LOCAL_METHOD(get_tag_state),

        // NFC States
        LOCAL_INT(TAG_UNINITIALIZED, 0),
        LOCAL_INT(TAG_INITIALIZED, 1),
        LOCAL_INT(TAG_SCANNING, 2),
        LOCAL_INT(TAG_IN_RANGE, 3),
        LOCAL_INT(TAG_OUT_OF_RANGE, 4),
        LOCAL_INT(TAG_DATA_LOADED, 5),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_nfc_globals, mp_module_citrus_nfc_globals_table);

const mp_obj_module_t mp_module_citrus_nfc = {
        .base = {&mp_type_module},
        .name = MP_QSTR_nfc,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_nfc_globals,
};