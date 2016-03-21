#include <3ds.h>

#include "py/runtime.h"

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_citrus_ndsp_##__n##_obj, mod_citrus_ndsp_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_ndsp_##__n##_obj}
#define LOCAL_INT(__n, __v) \
    {MP_ROM_QSTR(MP_QSTR_##__n), MP_ROM_INT(__v)}

extern const mp_obj_type_t mod_citrus_ndsp_Channel_type;
extern const mp_obj_type_t mod_citrus_ndsp_Sound_type;

STATIC ndspOutputMode _mod_citrus_ndsp_get_output_mode(mp_obj_t mode) {
    if (mp_obj_is_integer(mode)) {
        int _mode = mp_obj_get_int(mode);
        if (_mode >= NDSP_OUTPUT_MONO && _mode <= NDSP_OUTPUT_SURROUND) {
            return _mode;
        }
    }

    nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "invalid ndspOutputMode"));
}

STATIC ndspClippingMode _mod_citrus_ndsp_get_clipping_mode(mp_obj_t mode) {
    if (mp_obj_is_integer(mode)) {
        int _mode = mp_obj_get_int(mode);
        if (_mode == NDSP_CLIP_NORMAL || _mode == NDSP_CLIP_SOFT) {
            return _mode;
        }
    }

    nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "invalid ndspClippingMode"));
}

STATIC ndspClippingMode _mod_citrus_ndsp_get_speaker_pos(mp_obj_t pos) {
    if (mp_obj_is_integer(pos)) {
        int _pos = mp_obj_get_int(pos);
        if (_pos >= NDSP_SPKPOS_SQUARE || _pos <= NDSP_SPKPOS_NUM) {
            return _pos;
        }
    }

    nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "invalid ndspSpeakerPos"));
}

ndspInterpType _mod_citrus_ndsp_get_interp_type(mp_obj_t type) {
    if (mp_obj_is_integer(type)) {
        int _type = mp_obj_get_int(type);
        if (_type >= NDSP_INTERP_POLYPHASE && _type <= NDSP_INTERP_NONE) {
            return _type;
        }
    }

    nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "invalid ndspInterpType"));
}

STATIC mp_obj_t mod_citrus_ndsp_init(void) {
    return mp_obj_new_int(ndspInit());
}

STATIC mp_obj_t mod_citrus_ndsp_exit(void) {
    ndspExit();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ndsp_get_dropped_frames(void) {
    return mp_obj_new_int(ndspGetDroppedFrames());
}

STATIC mp_obj_t mod_citrus_ndsp_get_frame_count(void) {
    return mp_obj_new_int(ndspGetFrameCount());
}

STATIC mp_obj_t mod_citrus_ndsp_set_master_volume(mp_obj_t volume) {
    float vol = mp_obj_get_float(volume);
    ndspSetMasterVol(vol);

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ndsp_set_output_mode(mp_obj_t mode) {
    ndspOutputMode output = _mod_citrus_ndsp_get_output_mode(mode);
    ndspSetOutputMode(output);

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ndsp_set_clipping_mode(mp_obj_t mode) {
    ndspClippingMode clip = _mod_citrus_ndsp_get_clipping_mode(mode);
    ndspSetClippingMode(clip);

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ndsp_set_output_count(mp_obj_t count) {
    int num = mp_obj_get_int(count);
    ndspSetOutputCount(num);

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ndsp_set_surround_depth(mp_obj_t depth) {
    u16 _depth = mp_obj_get_int(depth);
    ndspSurroundSetDepth(_depth);

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ndsp_set_surround_position(mp_obj_t position) {
    ndspSpeakerPos pos = _mod_citrus_ndsp_get_speaker_pos(position);
    ndspSurroundSetPos(pos);

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ndsp_set_surround_rear_ratio(mp_obj_t ratio) {
    u16 _ratio = mp_obj_get_int(ratio);
    ndspSurroundSetRearRatio(_ratio);

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ndsp_enable_aux(mp_obj_t id, mp_obj_t enable) {
    int _id = mp_obj_get_int(id);

    ndspAuxSetEnable(_id, mp_obj_is_true(enable));

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ndsp_set_aux_front_bypass(mp_obj_t id, mp_obj_t bypass) {
    int _id = mp_obj_get_int(id);
    ndspAuxSetFrontBypass(_id, mp_obj_is_true(bypass));

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ndsp_set_aux_volume(mp_obj_t id, mp_obj_t volume) {
    int _id = mp_obj_get_int(id);
    float vol = mp_obj_get_float(volume);
    ndspAuxSetVolume(_id, vol);

    return mp_const_none;
}

METHOD_OBJ_N(0, init);
METHOD_OBJ_N(0, exit);
METHOD_OBJ_N(0, get_dropped_frames);
METHOD_OBJ_N(0, get_frame_count);
METHOD_OBJ_N(1, set_master_volume);
METHOD_OBJ_N(1, set_output_mode);
METHOD_OBJ_N(1, set_clipping_mode);
METHOD_OBJ_N(1, set_output_count);
METHOD_OBJ_N(1, set_surround_depth);
METHOD_OBJ_N(1, set_surround_position);
METHOD_OBJ_N(1, set_surround_rear_ratio);
METHOD_OBJ_N(2, enable_aux);
METHOD_OBJ_N(2, set_aux_front_bypass);
METHOD_OBJ_N(2, set_aux_volume);

STATIC const mp_rom_map_elem_t mp_module_citrus_ndsp_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_ndsp)},

        // Classes
        {MP_ROM_QSTR(MP_QSTR_Channel), MP_ROM_PTR(&mod_citrus_ndsp_Channel_type)},
        {MP_ROM_QSTR(MP_QSTR_Sound), MP_ROM_PTR(&mod_citrus_ndsp_Sound_type)},

        // Methods
        LOCAL_METHOD(init),
        LOCAL_METHOD(exit),
        LOCAL_METHOD(get_dropped_frames),
        LOCAL_METHOD(get_frame_count),
        LOCAL_METHOD(set_master_volume),
        LOCAL_METHOD(set_output_mode),
        LOCAL_METHOD(set_clipping_mode),
        LOCAL_METHOD(set_output_count),
        LOCAL_METHOD(set_surround_depth),
        LOCAL_METHOD(set_surround_position),
        LOCAL_METHOD(set_surround_position),
        LOCAL_METHOD(set_surround_rear_ratio),
        LOCAL_METHOD(enable_aux),
        LOCAL_METHOD(set_aux_front_bypass),
        LOCAL_METHOD(set_aux_volume),

        // ndspOutputMode
        LOCAL_INT(OUTPUT_MONO, NDSP_OUTPUT_MONO),
        LOCAL_INT(OUTPUT_STEREO, NDSP_OUTPUT_STEREO),
        LOCAL_INT(OUTPUT_SURROUND, NDSP_OUTPUT_SURROUND),

        // ndspClippingMode
        LOCAL_INT(CLIP_NORMAL, NDSP_CLIP_NORMAL),
        LOCAL_INT(CLIP_SOFT, NDSP_CLIP_SOFT),

        // ndspSpeakerPos
        LOCAL_INT(POS_SQUARE, NDSP_SPKPOS_SQUARE),
        LOCAL_INT(POS_WIDE, NDSP_SPKPOS_WIDE),
        LOCAL_INT(POS_NUM, NDSP_SPKPOS_NUM),

        // Wave Status
        LOCAL_INT(STATUS_FREE, NDSP_WBUF_FREE),
        LOCAL_INT(STATUS_QUEUED, NDSP_WBUF_QUEUED),
        LOCAL_INT(STATUS_PLAYING, NDSP_WBUF_PLAYING),
        LOCAL_INT(STATUS_DONE, NDSP_WBUF_DONE),

        // Channel Encoding
        LOCAL_INT(ENCODING_PCM8, NDSP_ENCODING_PCM8),
        LOCAL_INT(ENCODING_PCM16, NDSP_ENCODING_PCM16),
        LOCAL_INT(ENCODING_ADPCM, NDSP_ENCODING_ADPCM),

        // Channel Format
        LOCAL_INT(FORMAT_MONO_PCM8, NDSP_FORMAT_MONO_PCM8),
        LOCAL_INT(FORMAT_MONO_PCM16, NDSP_FORMAT_MONO_PCM16),
        LOCAL_INT(FORMAT_MONO_ADPCM, NDSP_FORMAT_MONO_ADPCM),
        LOCAL_INT(FORMAT_STEREO_PCM8, NDSP_FORMAT_STEREO_PCM8),
        LOCAL_INT(FORMAT_STEREO_PCM16, NDSP_FORMAT_STEREO_PCM16),
        LOCAL_INT(FORMAT_PCM8, NDSP_FORMAT_PCM8),
        LOCAL_INT(FORMAT_PCM16, NDSP_FORMAT_PCM16),
        LOCAL_INT(FORMAT_ADPCM, NDSP_FORMAT_ADPCM),

        // ndspInterpType
        LOCAL_INT(INTERP_POLYPHASE, NDSP_INTERP_POLYPHASE),
        LOCAL_INT(INTERP_LINEAR, NDSP_INTERP_LINEAR),
        LOCAL_INT(INTERP_NONE, NDSP_INTERP_NONE),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_ndsp_globals, mp_module_citrus_ndsp_globals_table);

const mp_obj_module_t mp_module_citrus_ndsp = {
        .base = {&mp_type_module},
        .name = MP_QSTR_ndsp,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_ndsp_globals,
};