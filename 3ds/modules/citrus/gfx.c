#include <3ds.h>

#include "py/runtime.h"

#include "helpers.h"

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
    gfxInitDefault();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gfx_init(mp_obj_t top_format, mp_obj_t bottom_format, mp_obj_t vram_buffers) {
    int top = _mod_citrus_gsp_get_framebuffer_format(top_format);
    int bot = _mod_citrus_gsp_get_framebuffer_format(bottom_format);

    gfxInit(top, bot, mp_obj_is_true(vram_buffers));

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gfx_exit(void) {
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

STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gfx_init_default_obj, mod_citrus_gfx_init_default);
STATIC MP_DEFINE_CONST_FUN_OBJ_3(mod_citrus_gfx_init_obj, mod_citrus_gfx_init);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gfx_exit_obj, mod_citrus_gfx_exit);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_gfx_set_3d_obj, mod_citrus_gfx_set_3d);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gfx_is_3d_obj, mod_citrus_gfx_is_3d);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_citrus_gfx_set_screen_format_obj, mod_citrus_gfx_set_screen_format);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_gfx_get_screen_format_obj, mod_citrus_gfx_get_screen_format);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_citrus_gfx_set_double_buffering_obj, mod_citrus_gfx_set_double_buffering);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gfx_flush_buffers_obj, mod_citrus_gfx_flush_buffers);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_citrus_gfx_config_screen_obj, mod_citrus_gfx_config_screen);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gfx_swap_buffers_obj, mod_citrus_gfx_swap_buffers);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gfx_swap_buffers_gpu_obj, mod_citrus_gfx_swap_buffers_gpu);

STATIC const mp_rom_map_elem_t mp_module_citrus_gfx_globals_table[] = {
        // Module info
        {MP_ROM_QSTR(MP_QSTR___name__),             MP_ROM_QSTR(MP_QSTR_gfx)},

        // Functions
        {MP_ROM_QSTR(MP_QSTR_init_default),         MP_ROM_PTR(&mod_citrus_gfx_init_default_obj)},
        {MP_ROM_QSTR(MP_QSTR_init),                 MP_ROM_PTR(&mod_citrus_gfx_init_obj)},
        {MP_ROM_QSTR(MP_QSTR_exit),                 MP_ROM_PTR(&mod_citrus_gfx_exit_obj)},
        {MP_ROM_QSTR(MP_QSTR_set_3d),               MP_ROM_PTR(&mod_citrus_gfx_set_3d_obj)},
        {MP_ROM_QSTR(MP_QSTR_is_3d),                MP_ROM_PTR(&mod_citrus_gfx_is_3d_obj)},
        {MP_ROM_QSTR(MP_QSTR_set_screen_format),    MP_ROM_PTR(&mod_citrus_gfx_set_screen_format_obj)},
        {MP_ROM_QSTR(MP_QSTR_get_screen_format),    MP_ROM_PTR(&mod_citrus_gfx_get_screen_format_obj)},
        {MP_ROM_QSTR(MP_QSTR_set_double_buffering), MP_ROM_PTR(&mod_citrus_gfx_set_double_buffering_obj)},
        {MP_ROM_QSTR(MP_QSTR_flush_buffers),        MP_ROM_PTR(&mod_citrus_gfx_flush_buffers_obj)},
        {MP_ROM_QSTR(MP_QSTR_config_screen),        MP_ROM_PTR(&mod_citrus_gfx_config_screen_obj)},
        {MP_ROM_QSTR(MP_QSTR_swap_buffers),         MP_ROM_PTR(&mod_citrus_gfx_swap_buffers_obj)},
        {MP_ROM_QSTR(MP_QSTR_swap_buffers_gpu),     MP_ROM_PTR(&mod_citrus_gfx_swap_buffers_gpu_obj)},

        // gfxScreen_t
        {MP_ROM_QSTR(MP_QSTR_SCREEN_TOP),           MP_ROM_INT(GFX_TOP)},
        {MP_ROM_QSTR(MP_QSTR_SCREEN_BOTTOM),        MP_ROM_INT(GFX_BOTTOM)},

        // gfx3dSide_t
        {MP_ROM_QSTR(MP_QSTR_SIDE_LEFT),            MP_ROM_INT(GFX_LEFT)},
        {MP_ROM_QSTR(MP_QSTR_SIDE_RIGHT),           MP_ROM_INT(GFX_RIGHT)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_gfx_globals, mp_module_citrus_gfx_globals_table);

const mp_obj_module_t mp_module_citrus_gfx = {
        .base = {&mp_type_module},
        .name = MP_QSTR_gfx,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_gfx_globals,
};
