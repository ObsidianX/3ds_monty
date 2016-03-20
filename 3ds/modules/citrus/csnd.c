#include <3ds.h>

#include "py/runtime.h"

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_citrus_csnd_##__n##_obj, mod_citrus_csnd_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_csnd_##__n##_obj}
#define LOCAL_INT(__n, __v) \
    {MP_ROM_QSTR(MP_QSTR_##__n), MP_ROM_INT(__v)}

extern const mp_obj_type_t mod_citrus_csnd_Sound_type;
extern const mp_obj_type_t mod_citrus_csnd_Channel_type;

STATIC u32 _mod_citrus_csnd_get_channel(mp_obj_t channel) {
    if (mp_obj_is_integer(channel)) {
        u32 chan = mp_obj_get_int(channel);
        if (chan >= 0 && chan < CSND_NUM_CHANNELS) {
            return chan;
        }
    }

    nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "invalid channel. 0 <= channel < 32"));
}

STATIC u32 _mod_citrus_csnd_get_encoding(mp_obj_t encoding) {
    if (mp_obj_is_integer(encoding)) {
        u32 enc = mp_obj_get_int(encoding);
        if (enc >= CSND_ENCODING_PCM8 && enc <= CSND_ENCODING_PSG) {
            return enc;
        }
    }

    nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "invalid encoding"));
}

STATIC mp_obj_t mod_citrus_csnd_init(void) {
    return mp_obj_new_int(csndInit());
}

STATIC mp_obj_t mod_citrus_csnd_exit(void) {
    csndExit();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_csnd_set_channel_volume(mp_obj_t channel, mp_obj_t chan_volume, mp_obj_t cap_volume) {
    u32 chan = _mod_citrus_csnd_get_channel(channel);
    u32 chan_vol = mp_obj_get_int(chan_volume);
    u32 cap_vol = mp_obj_get_int(cap_volume);

    CSND_SetVol(chan, chan_vol, cap_vol);

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_csnd_set_channel_looping(mp_obj_t channel, mp_obj_t loop) {
    u32 chan = _mod_citrus_csnd_get_channel(channel);

    CSND_SetLooping(chan, mp_obj_is_true(loop) ? 1 : 0);

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_csnd_set_channel_encoding(mp_obj_t channel, mp_obj_t encoding) {
    u32 chan = _mod_citrus_csnd_get_channel(channel);
    u32 enc = _mod_citrus_csnd_get_encoding(encoding);

    CSND_SetEncoding(chan, enc);

    return mp_const_none;
}

METHOD_OBJ_N(0, init);
METHOD_OBJ_N(0, exit);
METHOD_OBJ_N(3, set_channel_volume);
METHOD_OBJ_N(2, set_channel_looping);
METHOD_OBJ_N(2, set_channel_encoding);

STATIC const mp_rom_map_elem_t mp_module_citrus_csnd_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_csnd)},

        // Classes
        {MP_ROM_QSTR(MP_QSTR_Sound), MP_ROM_PTR(&mod_citrus_csnd_Sound_type)},

        // Functions
        LOCAL_METHOD(init),
        LOCAL_METHOD(exit),
        LOCAL_METHOD(set_channel_volume),
        LOCAL_METHOD(set_channel_looping),
        LOCAL_METHOD(set_channel_encoding),

        // Encodings
        LOCAL_INT(ENCODING_PCM8, CSND_ENCODING_PCM8),
        LOCAL_INT(ENCODING_PCM16, CSND_ENCODING_PCM16),
        LOCAL_INT(ENCODING_ADPCM, CSND_ENCODING_ADPCM),
        LOCAL_INT(ENCODING_PSG, CSND_ENCODING_PSG),

        // Loop Modes
        LOCAL_INT(LOOP_MANUAL, CSND_LOOPMODE_MANUAL),
        LOCAL_INT(LOOP_NORMAL, CSND_LOOPMODE_NORMAL),
        LOCAL_INT(LOOP_ONESHOT, CSND_LOOPMODE_ONESHOT),
        LOCAL_INT(LOOP_NORELOAD, CSND_LOOPMODE_NORELOAD),

        // Flags
        LOCAL_INT(FLAG_LINEAR_INTERP, SOUND_LINEAR_INTERP),
        LOCAL_INT(FLAG_REPEAT, SOUND_REPEAT),
        LOCAL_INT(FLAG_ONE_SHOT, SOUND_ONE_SHOT),
        LOCAL_INT(FLAG_FORMAT_8BIT, SOUND_FORMAT_8BIT),
        LOCAL_INT(FLAG_FORMAT_16BIT, SOUND_FORMAT_16BIT),
        LOCAL_INT(FLAG_FORMAT_ADPCM, SOUND_FORMAT_ADPCM),
        LOCAL_INT(FLAG_FORMAT_PSG, SOUND_FORMAT_PSG),
        LOCAL_INT(FLAG_ENABLE, SOUND_ENABLE),

        // Capture
        LOCAL_INT(CAPTURE_REPEAT, CAPTURE_REPEAT),
        LOCAL_INT(CAPTURE_ONE_SHOT, CAPTURE_ONE_SHOT),
        LOCAL_INT(CAPTURE_FORMAT_16BIT, CAPTURE_FORMAT_16BIT),
        LOCAL_INT(CAPTURE_FORMAT_8BIT, CAPTURE_FORMAT_8BIT),
        LOCAL_INT(CAPTURE_ENABLE, CAPTURE_ENABLE),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_csnd_globals, mp_module_citrus_csnd_globals_table);

const mp_obj_module_t mp_module_citrus_csnd = {
        .base = {&mp_type_module},
        .name = MP_QSTR_csnd,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_csnd_globals,
};