#include <3ds.h>

#include "py/runtime.h"

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

STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_csnd_init_obj, mod_citrus_csnd_init);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_csnd_exit_obj, mod_citrus_csnd_exit);
STATIC MP_DEFINE_CONST_FUN_OBJ_3(mod_citrus_csnd_set_channel_volume_obj, mod_citrus_csnd_set_channel_volume);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_citrus_csnd_set_channel_looping_obj, mod_citrus_csnd_set_channel_looping);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_citrus_csnd_set_channel_encoding_obj, mod_citrus_csnd_set_channel_encoding);

STATIC const mp_rom_map_elem_t mp_module_citrus_csnd_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__),             MP_ROM_QSTR(MP_QSTR_csnd)},

        // Classes
        {MP_ROM_QSTR(MP_QSTR_Sound),                MP_ROM_PTR(&mod_citrus_csnd_Sound_type)},

        // Functions
        {MP_ROM_QSTR(MP_QSTR_init),                 MP_ROM_PTR(&mod_citrus_csnd_init_obj)},
        {MP_ROM_QSTR(MP_QSTR_exit),                 MP_ROM_PTR(&mod_citrus_csnd_exit_obj)},
        {MP_ROM_QSTR(MP_QSTR_set_channel_volume),   MP_ROM_PTR(&mod_citrus_csnd_set_channel_volume_obj)},
        {MP_ROM_QSTR(MP_QSTR_set_channel_looping),  MP_ROM_PTR(&mod_citrus_csnd_set_channel_looping_obj)},
        {MP_ROM_QSTR(MP_QSTR_set_channel_encoding), MP_ROM_PTR(&mod_citrus_csnd_set_channel_encoding_obj)},

        // Encodings
        {MP_ROM_QSTR(MP_QSTR_ENCODING_PCM8),        MP_ROM_INT(CSND_ENCODING_PCM8)},
        {MP_ROM_QSTR(MP_QSTR_ENCODING_PCM16),       MP_ROM_INT(CSND_ENCODING_PCM16)},
        {MP_ROM_QSTR(MP_QSTR_ENCODING_ADPCM),       MP_ROM_INT(CSND_ENCODING_ADPCM)},
        {MP_ROM_QSTR(MP_QSTR_ENCODING_PSG),         MP_ROM_INT(CSND_ENCODING_PSG)},

        // Loop Modes
        {MP_ROM_QSTR(MP_QSTR_LOOP_MANUAL),          MP_ROM_INT(CSND_LOOPMODE_MANUAL)},
        {MP_ROM_QSTR(MP_QSTR_LOOP_NORMAL),          MP_ROM_INT(CSND_LOOPMODE_NORMAL)},
        {MP_ROM_QSTR(MP_QSTR_LOOP_ONESHOT),         MP_ROM_INT(CSND_LOOPMODE_ONESHOT)},
        {MP_ROM_QSTR(MP_QSTR_LOOP_NORELOAD),        MP_ROM_INT(CSND_LOOPMODE_NORELOAD)},

        // Flags
        {MP_ROM_QSTR(MP_QSTR_FLAG_LINEAR_INTERP),   MP_ROM_INT(SOUND_LINEAR_INTERP)},
        {MP_ROM_QSTR(MP_QSTR_FLAG_REPEAT),          MP_ROM_INT(SOUND_REPEAT)},
        {MP_ROM_QSTR(MP_QSTR_FLAG_ONE_SHOT),        MP_ROM_INT(SOUND_ONE_SHOT)},
        {MP_ROM_QSTR(MP_QSTR_FLAG_FORMAT_8BIT),     MP_ROM_INT(SOUND_FORMAT_8BIT)},
        {MP_ROM_QSTR(MP_QSTR_FLAG_FORMAT_16BIT),    MP_ROM_INT(SOUND_FORMAT_16BIT)},
        {MP_ROM_QSTR(MP_QSTR_FLAG_FORMAT_ADPCM),    MP_ROM_INT(SOUND_FORMAT_ADPCM)},
        {MP_ROM_QSTR(MP_QSTR_FLAG_FORMAT_PSG),      MP_ROM_INT(SOUND_FORMAT_PSG)},
        {MP_ROM_QSTR(MP_QSTR_FLAG_ENABLE),          MP_ROM_INT(SOUND_ENABLE)},

        // Capture
        {MP_ROM_QSTR(MP_QSTR_CAPTURE_REPEAT),       MP_ROM_INT(CAPTURE_REPEAT)},
        {MP_ROM_QSTR(MP_QSTR_CAPTURE_ONE_SHOT),     MP_ROM_INT(CAPTURE_ONE_SHOT)},
        {MP_ROM_QSTR(MP_QSTR_CAPTURE_FORMAT_16BIT), MP_ROM_INT(CAPTURE_FORMAT_16BIT)},
        {MP_ROM_QSTR(MP_QSTR_CAPTURE_FORMAT_8BIT),  MP_ROM_INT(CAPTURE_FORMAT_8BIT)},
        {MP_ROM_QSTR(MP_QSTR_CAPTURE_ENABLE),       MP_ROM_INT(CAPTURE_ENABLE)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_csnd_globals, mp_module_citrus_csnd_globals_table);

const mp_obj_module_t mp_module_citrus_csnd = {
        .base = {&mp_type_module},
        .name = MP_QSTR_csnd,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_csnd_globals,
};