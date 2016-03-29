#include <3ds.h>

#include "py/runtime.h"

#include "../init_helper.h"

#define METHOD_OBJ(__n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_cfg_##__n##_obj, mod_citrus_cfg_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_cfg_##__n##_obj}
#define LOCAL_INT(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), MP_ROM_INT(CFG_##__n)}

enum {
    CFG_MODEL_O3DS = 0,
    CFG_MODEL_O3DSXL,
    CFG_MODEL_N3DS,
    CFG_MODEL_2DS,
    CFG_MODEL_N3DSXL
};

static int _mod_citrus_cfg_is_init = 0;

STATIC mp_obj_t mod_citrus_cfg_init(void) {
    INIT_ONCE(_mod_citrus_cfg_is_init);

    return mp_obj_new_int(cfguInit());
}

mp_obj_t mod_citrus_cfg_exit(void) {
    EXIT_ONCE(_mod_citrus_cfg_is_init);

    cfguExit();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_cfg_get_region(void) {
    u8 region;
    CFGU_SecureInfoGetRegion(&region);

    return mp_obj_new_int(region);
}

STATIC mp_obj_t mod_citrus_cfg_is_canada_or_usa(void) {
    u8 na;
    CFGU_GetRegionCanadaUSA(&na);

    return mp_obj_new_bool(na);
}

STATIC mp_obj_t mod_citrus_cfg_get_model(void) {
    u8 model;
    CFGU_GetSystemModel(&model);

    return mp_obj_new_int(model);
}

STATIC mp_obj_t mod_citrus_cfg_is_2ds(void) {
    u8 value;
    CFGU_GetModelNintendo2DS(&value);

    return mp_obj_new_bool(value == 0);
}

STATIC mp_obj_t mod_citrus_cfg_get_language(void) {
    u8 lang;
    CFGU_GetSystemLanguage(&lang);

    return mp_obj_new_int(lang);
}

METHOD_OBJ(init);
METHOD_OBJ(exit);
METHOD_OBJ(get_region);
METHOD_OBJ(is_canada_or_usa);
METHOD_OBJ(get_model);
METHOD_OBJ(is_2ds);
METHOD_OBJ(get_language);

STATIC const mp_rom_map_elem_t mp_module_citrus_cfg_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_cfg)},

        // Functions
        LOCAL_METHOD(init),
        LOCAL_METHOD(exit),
        LOCAL_METHOD(get_region),
        LOCAL_METHOD(is_canada_or_usa),
        LOCAL_METHOD(get_model),
        LOCAL_METHOD(is_2ds),
        LOCAL_METHOD(get_language),

        // Regions
        LOCAL_INT(REGION_JPN),
        LOCAL_INT(REGION_USA),
        LOCAL_INT(REGION_EUR),
        LOCAL_INT(REGION_AUS),
        LOCAL_INT(REGION_CHN),
        LOCAL_INT(REGION_KOR),
        LOCAL_INT(REGION_TWN),

        // Languages
        LOCAL_INT(LANGUAGE_JP),
        LOCAL_INT(LANGUAGE_EN),
        LOCAL_INT(LANGUAGE_FR),
        LOCAL_INT(LANGUAGE_DE),
        LOCAL_INT(LANGUAGE_IT),
        LOCAL_INT(LANGUAGE_ES),
        LOCAL_INT(LANGUAGE_ZH),
        LOCAL_INT(LANGUAGE_KO),
        LOCAL_INT(LANGUAGE_NL),
        LOCAL_INT(LANGUAGE_PT),
        LOCAL_INT(LANGUAGE_RU),
        LOCAL_INT(LANGUAGE_TW),

        // Models
        LOCAL_INT(MODEL_O3DS),
        LOCAL_INT(MODEL_O3DSXL),
        LOCAL_INT(MODEL_N3DS),
        LOCAL_INT(MODEL_2DS),
        LOCAL_INT(MODEL_N3DSXL),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_cfg_globals, mp_module_citrus_cfg_globals_table);

const mp_obj_module_t mp_module_citrus_cfg = {
        .base = {&mp_type_module},
        .name = MP_QSTR_cfg,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_cfg_globals,
};