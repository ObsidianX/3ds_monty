#include <3ds.h>

#include "py/runtime.h"

#include "helpers.h"
#include "../init_helper.h"

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_citrus_gfx_##__n##_obj, mod_citrus_gfx_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_gfx_##__n##_obj}
#define LOCAL_INT(__n, __v) \
    {MP_ROM_QSTR(MP_QSTR_##__n), MP_ROM_INT(__v)}

static int _mod_citrus_gfx_is_init = 0;

gfxScreen_t _mod_citrus_gfx_get_gfx_screen(mp_obj_t screen) {
    if (mp_obj_is_integer(screen)) {
        int scr = mp_obj_get_int(screen);
        if (scr == GFX_TOP || scr == GFX_BOTTOM) {
            return scr;
        }
    }

    nlr_raise(mp_obj_new_exception(&mp_type_TypeError));
}

gfxScreen_t _mod_citrus_gfx_is_gfx_screen(mp_int_t scr) {
    if (scr == GFX_TOP || scr == GFX_BOTTOM) {
        return scr;
    }

    nlr_raise(mp_obj_new_exception(&mp_type_TypeError));
}

gfx3dSide_t _mod_citrus_gfx_get_gfx_3d_side(mp_obj_t side) {
    if (mp_obj_is_integer(side)) {
        int _side = mp_obj_get_int(side);
        if (_side == GFX_LEFT || _side == GFX_RIGHT) {
            return _side;
        }
    }

    nlr_raise(mp_obj_new_exception(&mp_type_TypeError));
}

STATIC mp_obj_t mod_citrus_gfx_init_default(void) {
    INIT_ONCE(_mod_citrus_gfx_is_init);

    gfxInitDefault();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gfx_init(mp_obj_t top_format, mp_obj_t bottom_format, mp_obj_t vram_buffers) {
    INIT_ONCE(_mod_citrus_gfx_is_init);

    int top = _mod_citrus_gsp_get_framebuffer_format(top_format);
    int bot = _mod_citrus_gsp_get_framebuffer_format(bottom_format);

    gfxInit(top, bot, mp_obj_is_true(vram_buffers));

    return mp_const_none;
}

mp_obj_t mod_citrus_gfx_exit(void) {
    EXIT_ONCE(_mod_citrus_gfx_is_init);

    gfxExit();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gfx_set_3d(mp_obj_t enabled) {
    gfxSet3D(mp_obj_is_true(enabled));

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gfx_is_3d(void) {
    return gfxIs3D() ? mp_const_true : mp_const_false;
}

STATIC mp_obj_t mod_citrus_gfx_set_screen_format(mp_obj_t which, mp_obj_t format) {
    int screen = _mod_citrus_gfx_get_gfx_screen(which);
    int fmt = _mod_citrus_gsp_get_framebuffer_format(format);

    gfxSetScreenFormat(screen, fmt);

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gfx_get_screen_format(mp_obj_t which) {
    int screen = _mod_citrus_gfx_get_gfx_screen(which);

    int ret = gfxGetScreenFormat(screen);
    return mp_obj_new_int(ret);
}

STATIC mp_obj_t mod_citrus_gfx_set_double_buffering(mp_obj_t which, mp_obj_t enabled) {
    int screen = _mod_citrus_gfx_get_gfx_screen(which);

    gfxSetDoubleBuffering(screen, mp_obj_is_true(enabled));

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gfx_flush_buffers(void) {
    gfxFlushBuffers();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gfx_config_screen(mp_obj_t which, mp_obj_t immediate) {
    int screen = _mod_citrus_gfx_get_gfx_screen(which);

    gfxConfigScreen(screen, mp_obj_is_true(immediate));

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gfx_swap_buffers(void) {
    gfxSwapBuffers();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gfx_swap_buffers_gpu(void) {
    gfxSwapBuffersGpu();

    return mp_const_none;
}

METHOD_OBJ_N(0, init_default);
METHOD_OBJ_N(3, init);
METHOD_OBJ_N(0, exit);
METHOD_OBJ_N(1, set_3d);
METHOD_OBJ_N(0, is_3d);
METHOD_OBJ_N(2, set_screen_format);
METHOD_OBJ_N(1, get_screen_format);
METHOD_OBJ_N(2, set_double_buffering);
METHOD_OBJ_N(0, flush_buffers);
METHOD_OBJ_N(2, config_screen);
METHOD_OBJ_N(0, swap_buffers);
METHOD_OBJ_N(0, swap_buffers_gpu);

STATIC const mp_rom_map_elem_t mp_module_citrus_gfx_globals_table[] = {
        // Module info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_gfx)},

        // Functions
        LOCAL_METHOD(init_default),
        LOCAL_METHOD(init),
        LOCAL_METHOD(exit),
        LOCAL_METHOD(set_3d),
        LOCAL_METHOD(is_3d),
        LOCAL_METHOD(set_screen_format),
        LOCAL_METHOD(get_screen_format),
        LOCAL_METHOD(set_double_buffering),
        LOCAL_METHOD(flush_buffers),
        LOCAL_METHOD(config_screen),
        LOCAL_METHOD(swap_buffers),
        LOCAL_METHOD(swap_buffers_gpu),

        // gfxScreen_t
        LOCAL_INT(SCREEN_TOP, GFX_TOP),
        LOCAL_INT(SCREEN_BOTTOM, GFX_BOTTOM),

        // gfx3dSide_t
        LOCAL_INT(SIDE_LEFT, GFX_LEFT),
        LOCAL_INT(SIDE_RIGHT, GFX_RIGHT),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_gfx_globals, mp_module_citrus_gfx_globals_table);

const mp_obj_module_t mp_module_citrus_gfx = {
        .base = {&mp_type_module},
        .name = MP_QSTR_gfx,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_gfx_globals,
};
