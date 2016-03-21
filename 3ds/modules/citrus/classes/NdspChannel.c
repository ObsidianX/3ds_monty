#include <string.h>

#include <3ds.h>

#include "py/runtime.h"

#include "../helpers.h"

#define SELF(src) mod_citrus_ndsp_Channel_t *self = src;

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_citrus_ndsp_Channel_##__n##_obj, mod_citrus_ndsp_Channel_##__n)
#define METHOD_OBJ_KW(__min, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_KW(mod_citrus_ndsp_Channel_##__n##_obj, __min, mod_citrus_ndsp_Channel_##__n)

#define ATTR_METHOD(__n) \
    if(!strcmp(name, qstr_str(MP_QSTR_##__n))) { \
        dest[0] = (mp_obj_t) &mod_citrus_ndsp_Channel_##__n##_obj; \
    }

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_ndsp_Channel_##__n##_obj}

const mp_obj_type_t mod_citrus_ndsp_Channel_type;

typedef struct {
    mp_obj_base_t base;

    int id;

    ndspInterpType interp;
    int sample_rate;
    int format;
} mod_citrus_ndsp_Channel_t;

enum {
    NEW_ARG_ID = 0,
    NEW_ARG_COUNT
};

STATIC mp_obj_t mod_citrus_ndsp_Channel_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args_in) {
    mp_arg_check_num(n_args, n_kw, 1, NEW_ARG_COUNT, true);

    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_id, MP_ARG_INT | MP_ARG_REQUIRED},
    };

    mp_arg_val_t args[NEW_ARG_COUNT];
    mp_arg_parse_all_kw_array(n_args, n_kw, args_in, NEW_ARG_COUNT, allowed_args, args);

    mod_citrus_ndsp_Channel_t *obj = m_new_obj(mod_citrus_ndsp_Channel_t);
    obj->base.type = (mp_obj_t) &mod_citrus_ndsp_Channel_type;

    obj->id = args[NEW_ARG_ID].u_int;
    obj->interp = NDSP_INTERP_NONE;
    obj->sample_rate = -1;
    obj->format = -1;

    ndspChnInitParams(obj->id);

    return obj;
}

STATIC void mod_citrus_ndsp_Channel_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    SELF(self_in);

    mp_printf(print, "<Channel id=%d>", self->id);
}

STATIC mp_obj_t mod_citrus_ndsp_Channel_reset(mp_obj_t self_in) {
    SELF(self_in);
    ndspChnReset(self->id);

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ndsp_Channel_is_playing(mp_obj_t self_in) {
    SELF(self_in);

    return mp_obj_new_bool(ndspChnIsPlaying(self->id));
}

STATIC mp_obj_t mod_citrus_ndsp_Channel_get_sample_position(mp_obj_t self_in) {
    SELF(self_in);

    return mp_obj_new_int(ndspChnGetSamplePos(self->id));
}

STATIC mp_obj_t mod_citrus_ndsp_Channel_get_wave_sequence_id(mp_obj_t self_in) {
    SELF(self_in);

    return mp_obj_new_int(ndspChnGetWaveBufSeq(self->id));
}

STATIC mp_obj_t mod_citrus_ndsp_Channel_set_interpolation(mp_obj_t self_in, mp_obj_t interp) {
    SELF(self_in);
    ndspInterpType type = _mod_citrus_ndsp_get_interp_type(interp);

    ndspChnSetInterp(self->id, type);
    self->interp = type;

    return mp_const_none;
}

enum {
    MIX_ARG_SELF = 0,
    MIX_ARG_FRONT_TUPLE,
    MIX_ARG_BACK_TUPLE,
    MIX_ARG_AUX1_FRONT_TUPLE,
    MIX_ARG_AUX1_BACK_TUPLE,
    MIX_ARG_AUX2_FRONT_TUPLE,
    MIX_ARG_AUX2_BACK_TUPLE,
    MIX_ARG_COUNT
};

STATIC mp_obj_t mod_citrus_ndsp_Channel_set_mix(size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    static const mp_arg_t allowed_args[] = {
            {MP_QSTR_self,       MP_ARG_OBJ | MP_ARG_REQUIRED, {}},
            {MP_QSTR_front,      MP_ARG_OBJ,                   {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_back,       MP_ARG_OBJ,                   {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_aux1_front, MP_ARG_OBJ,                   {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_aux1_back,  MP_ARG_OBJ,                   {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_aux2_front, MP_ARG_OBJ,                   {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
            {MP_QSTR_aux2_back,  MP_ARG_OBJ,                   {.u_rom_obj = MP_ROM_PTR(&mp_const_none_obj)}},
    };

    mp_arg_val_t args[MIX_ARG_COUNT];
    mp_arg_parse_all((mp_uint_t) n_args, pos_args, kw_args, MIX_ARG_COUNT, allowed_args, args);

    SELF(args[MIX_ARG_SELF].u_obj);

    float mix[12];
    int speaker = 0;

    for (int i = 1; i < MIX_ARG_COUNT; i++) {
        if (args[i].u_rom_obj == &mp_const_none_obj) {
            mix[speaker++] = 1.0f;
            mix[speaker++] = 1.0f;
            continue;
        }

        if (!MP_OBJ_IS_TYPE(args[i].u_obj, &mp_type_tuple)) {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "values must be (left, right) tuples"));
        }

        mp_obj_tuple_t *tuple = MP_OBJ_TO_PTR(args[i].u_obj);
        if (tuple->len != 2) {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "set_mix requires left and right values per tuple"));
        }

        if (!mp_obj_is_float(tuple->items[0]) || !mp_obj_is_float(tuple->items[1])) {
            nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "must provide floats for speaker volumes"));
        }

        mix[speaker++] = mp_obj_get_float(tuple->items[0]);
        mix[speaker++] = mp_obj_get_float(tuple->items[1]);
    }

    ndspChnSetMix(self->id, mix);

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ndsp_Channel_set_adpcm_coefs(mp_obj_t self_in, mp_obj_t coef_tuple) {
    SELF(self_in);

    mp_obj_tuple_t *coef_items = MP_OBJ_TO_PTR(coef_tuple);
    if (!MP_OBJ_IS_TYPE(coef_tuple, &mp_type_tuple) || coef_items->len != 16) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_ValueError, "coef must be 16 element tuple"));
    }

    u16 coefs[16];
    for (int i = 0; i < 16; i++) {
        coefs[i] = mp_obj_get_int(coef_items->items[i]);
    }

    ndspChnSetAdpcmCoefs(self->id, coefs);

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ndsp_Channel_clear_wave(mp_obj_t self_in) {
    SELF(self_in);
    ndspChnWaveBufClear(self->id);

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ndsp_Channel_add_wave(mp_obj_t self_in, mp_obj_t wave) {
    SELF(self_in);

    if (!MP_OBJ_IS_TYPE(wave, &mod_citrus_ndsp_Sound_type)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_TypeError, "invalid wave type: expecting ndsp.Sound"));
    }
    mod_citrus_ndsp_Sound_t *sound = MP_OBJ_TO_PTR(wave);

    if (sound->sample_rate != self->sample_rate || sound->format != self->format) {
        ndspChnReset(self->id);
        ndspChnSetInterp(self->id, self->interp);
        ndspChnSetRate(self->id, sound->sample_rate);
        ndspChnSetFormat(self->id, sound->format);

        self->sample_rate = sound->sample_rate;
        self->format = sound->format;
    }

    ndspChnWaveBufAdd(self->id, &sound->wave);

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ndsp_Channel_play(mp_obj_t self_in, mp_obj_t wave) {
    SELF(self_in);

    if (!MP_OBJ_IS_TYPE(wave, &mod_citrus_ndsp_Sound_type)) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_TypeError, "invalid wave type: expecting ndsp.Sound"));
    }
    mod_citrus_ndsp_Sound_t *sound = MP_OBJ_TO_PTR(wave);

    ndspChnWaveBufClear(self->id);
    ndspChnReset(self->id);
    ndspChnSetInterp(self->id, self->interp);
    ndspChnSetRate(self->id, sound->sample_rate);
    ndspChnSetFormat(self->id, sound->format);
    ndspChnWaveBufAdd(self->id, &sound->wave);

    self->sample_rate = sound->sample_rate;
    self->format = sound->format;

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ndsp_Channel_iir_mono_set_enable(mp_obj_t self_in, mp_obj_t enable) {
    SELF(self_in);
    ndspChnIirMonoSetEnable(self->id, mp_obj_is_true(enable));

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_ndsp_Channel_iir_biquad_set_enable(mp_obj_t self_in, mp_obj_t enable) {
    SELF(self_in);
    ndspChnIirBiquadSetEnable(self->id, mp_obj_is_true(enable));

    return mp_const_none;
}

METHOD_OBJ_N(1, reset);
METHOD_OBJ_N(1, is_playing);
METHOD_OBJ_N(1, get_sample_position);
METHOD_OBJ_N(1, get_wave_sequence_id);
METHOD_OBJ_N(2, set_interpolation);
METHOD_OBJ_KW(0, set_mix);
METHOD_OBJ_N(2, set_adpcm_coefs);
METHOD_OBJ_N(1, clear_wave);
METHOD_OBJ_N(2, add_wave);
METHOD_OBJ_N(2, play);
METHOD_OBJ_N(2, iir_mono_set_enable);
METHOD_OBJ_N(2, iir_biquad_set_enable);

STATIC void mod_citrus_ndsp_Channel_attr(mp_obj_t self_in, qstr attr, mp_obj_t *dest) {
    SELF(self_in);
    const char *name = qstr_str(attr);
    bool load = dest[0] == MP_OBJ_NULL;

    // Methods
    if (load) {
        ATTR_METHOD(reset)
        else ATTR_METHOD(is_playing)
        else ATTR_METHOD(get_sample_position)
        else ATTR_METHOD(get_wave_sequence_id)
        else ATTR_METHOD(set_interpolation)
        else ATTR_METHOD(set_mix)
        else ATTR_METHOD(set_adpcm_coefs)
        else ATTR_METHOD(clear_wave)
        else ATTR_METHOD(add_wave)
        else ATTR_METHOD(play)
        else ATTR_METHOD(iir_mono_set_enable)
        else ATTR_METHOD(iir_biquad_set_enable)

        if (dest[0] != MP_OBJ_NULL) {
            dest[1] = self_in;
            return;
        }
    }

    if (!strcmp(name, qstr_str(MP_QSTR_paused))) {
        if (load) {
            dest[0] = mp_obj_new_bool(ndspChnIsPaused(self->id));
        } else {
            ndspChnSetPaused(self->id, mp_obj_is_true(dest[0]));
            dest[0] = NULL;
        }
    }
}

STATIC const mp_map_elem_t mod_citrus_ndsp_Channel_locals_dict_table[] = {
        // Methods
        LOCAL_METHOD(reset),
        LOCAL_METHOD(is_playing),
        LOCAL_METHOD(get_sample_position),
        LOCAL_METHOD(get_wave_sequence_id),
        LOCAL_METHOD(set_interpolation),
        LOCAL_METHOD(set_mix),
        LOCAL_METHOD(set_adpcm_coefs),
        LOCAL_METHOD(clear_wave),
        LOCAL_METHOD(add_wave),
        LOCAL_METHOD(play),
        LOCAL_METHOD(iir_mono_set_enable),
        LOCAL_METHOD(iir_biquad_set_enable),

        // Fields
        // paused
};
STATIC MP_DEFINE_CONST_DICT(mod_citrus_ndsp_Channel_locals_dict, mod_citrus_ndsp_Channel_locals_dict_table);

const mp_obj_type_t mod_citrus_ndsp_Channel_type = {
        {&mp_type_type},
        .name = MP_QSTR_Channel,
        .print = mod_citrus_ndsp_Channel_print,
        .make_new = mod_citrus_ndsp_Channel_make_new,
        .locals_dict = (mp_obj_t) &mod_citrus_ndsp_Channel_locals_dict,
        .attr = (mp_obj_t) &mod_citrus_ndsp_Channel_attr,
};