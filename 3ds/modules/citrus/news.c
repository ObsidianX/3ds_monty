#include <string.h>
#include <stdint.h>

#include <3ds.h>

#include "py/runtime.h"

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_citrus_news_##__n##_obj, mod_citrus_news_##__n)
#define METHOD_OBJ_VAR_N(__num, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_citrus_news_##__n##_obj, __num, __num, mod_citrus_news_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_news_##__n##_obj}

static Result _news_last_result;

STATIC mp_obj_t mod_citrus_news_init(void) {
    newsInit();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_news_exit(void) {
    newsExit();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_news_last_result(void) {
    return mp_obj_new_int(_news_last_result);
}

enum {
    ADD_ARG_TITLE = 0,
    ADD_ARG_MESSAGE,
    ADD_ARG_IMAGE,
    ADD_ARG_IS_JPEG
};

STATIC mp_obj_t mod_citrus_news_add_notification(size_t n_args, const mp_obj_t *args) {
    const char *title = mp_obj_str_get_str(args[ADD_ARG_TITLE]);
    int title_len = strlen(title);
    int title16_len = title_len * 2;
    uint16_t *title16 = m_new(uint16_t, title16_len);
    utf8_to_utf16(title16, (uint8_t *) title, title16_len);

    const char *message = mp_obj_str_get_str(args[ADD_ARG_MESSAGE]);
    int message_len = strlen(message);
    int message16_len = message_len * 2;
    uint16_t *message16 = m_new(uint16_t, message16_len);
    utf8_to_utf16(message16, (uint8_t *) message, message16_len);

    void *image = NULL;
    size_t image_len = 0;
    bool is_jpeg = mp_obj_is_true(args[ADD_ARG_IS_JPEG]);

    mp_obj_type_t *type = mp_obj_get_type(args[3]);
    if (type != &mp_type_NoneType) {
        // load...
        // image_len from byte array
    }

    _news_last_result = NEWS_AddNotification(title16, title_len, message16, message_len, image, image_len, is_jpeg);

    m_del(uint16_t, title16, title16_len);
    m_del(uint16_t, message16, message16_len);

    return mod_citrus_news_last_result();
}

METHOD_OBJ_N(0, init);
METHOD_OBJ_N(0, exit);
METHOD_OBJ_N(0, last_result);
METHOD_OBJ_VAR_N(4, add_notification);

STATIC const mp_rom_map_elem_t mp_module_citrus_news_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_news)},

        // Functions
        LOCAL_METHOD(init),
        LOCAL_METHOD(exit),
        LOCAL_METHOD(last_result),
        LOCAL_METHOD(add_notification),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_news_globals, mp_module_citrus_news_globals_table);

const mp_obj_module_t mp_module_citrus_news = {
        .base = {&mp_type_module},
        .name = MP_QSTR_news,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_news_globals,
};