#include <sf2d.h>

#include "py/runtime.h"

#include "citrus/helpers.h"
#include "init_helper.h"

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_sf2d_##__n##_obj, mod_sf2d_##__n)
#define METHOD_OBJ_VAR_N(__num, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_sf2d_##__n##_obj, __num, __num, mod_sf2d_##__n)

#define LOCAL_CLASS(__n) \
    {MP_ROM_QSTR(MP_QSTR_##__n), MP_ROM_PTR(&mod_sf2d_##__n##_type)}
#define LOCAL_METHOD(__n) \
    {MP_ROM_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_sf2d_##__n##_obj}
#define LOCAL_INT(__n, __v) \
    {MP_ROM_QSTR(MP_QSTR_##__n), MP_ROM_INT(__v)}

extern const mp_obj_type_t mod_sf2d_Texture_type;
extern const mp_obj_type_t mod_sf2d_RenderTarget_type;

static int _mod_sf2d_is_init = 0;

sf2d_texfmt _mod_sf2d_get_texfmt(mp_int_t format) {
    if (format >= TEXFMT_RGBA8 && format <= TEXFMT_ETC1A4) {
        return format;
    }

    nlr_raise(mp_obj_new_exception(&mp_type_ValueError));
}

sf2d_place _mod_sf2d_get_place(mp_int_t place) {
    if (place >= SF2D_PLACE_RAM && place <= SF2D_PLACE_TEMP) {
        return place;
    }

    nlr_raise(mp_obj_new_exception(&mp_type_ValueError));
}

STATIC mp_obj_t mod_sf2d_init(void) {
    INIT_ONCE(_mod_sf2d_is_init);

    return mp_obj_new_int(sf2d_init());
}

STATIC mp_obj_t mod_sf2d_init_advanced(mp_obj_t gpucmd_size, mp_obj_t temppool_size) {
    INIT_ONCE(_mod_sf2d_is_init);

    int gpu = mp_obj_get_int(gpucmd_size);
    int pool = mp_obj_get_int(temppool_size);

    return mp_obj_new_int(sf2d_init_advanced(gpu, pool));
}

STATIC mp_obj_t mod_sf2d_fini(void) {
    EXIT_ONCE(_mod_sf2d_is_init);

    return mp_obj_new_int(sf2d_fini());
}

STATIC mp_obj_t mod_sf2d_set_3d(mp_obj_t enable) {
    sf2d_set_3D(mp_obj_is_true(enable));

    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_start_frame(mp_obj_t screen, mp_obj_t side) {
    gfxScreen_t _screen = _mod_citrus_gfx_get_gfx_screen(screen);
    gfx3dSide_t _side = _mod_citrus_gfx_get_gfx_3d_side(side);

    sf2d_start_frame(_screen, _side);

    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_end_frame(void) {
    sf2d_end_frame();

    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_swap_buffers(void) {
    sf2d_swapbuffers();

    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_set_vblank_wait(mp_obj_t enable) {
    sf2d_set_vblank_wait(mp_obj_is_true(enable));

    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_get_fps(void) {
    return mp_obj_new_float(sf2d_get_fps());
}

STATIC mp_obj_t mod_sf2d_set_clear_color(mp_obj_t color) {
    u32 clr = mp_obj_get_int(color);

    sf2d_set_clear_color(clr);

    return mp_const_none;
}

enum {
    DRAW_LINE_ARG_X0 = 0,
    DRAW_LINE_ARG_Y0,
    DRAW_LINE_ARG_X1,
    DRAW_LINE_ARG_Y1,
    DRAW_LINE_ARG_WIDTH,
    DRAW_LINE_ARG_COLOR,
};

STATIC mp_obj_t mod_sf2d_draw_line(size_t n_args, const mp_obj_t *args) {
    int x0 = mp_obj_get_int(args[DRAW_LINE_ARG_X0]);
    int y0 = mp_obj_get_int(args[DRAW_LINE_ARG_Y0]);
    int x1 = mp_obj_get_int(args[DRAW_LINE_ARG_X1]);
    int y1 = mp_obj_get_int(args[DRAW_LINE_ARG_Y1]);
    int width = mp_obj_get_int(args[DRAW_LINE_ARG_WIDTH]);
    int color = mp_obj_get_int(args[DRAW_LINE_ARG_COLOR]);

    sf2d_draw_line(x0, y0, x1, y1, width, color);

    return mp_const_none;
}

enum {
    DRAW_RECT_ARG_X = 0,
    DRAW_RECT_ARG_Y,
    DRAW_RECT_ARG_W,
    DRAW_RECT_ARG_H,
    DRAW_RECT_ARG_COLOR,
    DRAW_RECT_ROT_ARG_RAD
};

STATIC mp_obj_t mod_sf2d_draw_rectangle(size_t n_args, const mp_obj_t *args) {
    int x = mp_obj_get_int(args[DRAW_RECT_ARG_X]);
    int y = mp_obj_get_int(args[DRAW_RECT_ARG_Y]);
    int w = mp_obj_get_int(args[DRAW_RECT_ARG_W]);
    int h = mp_obj_get_int(args[DRAW_RECT_ARG_H]);
    u32 color = mp_obj_get_int(args[DRAW_RECT_ARG_COLOR]);

    sf2d_draw_rectangle(x, y, w, h, color);

    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_draw_rectangle_rotate(size_t n_args, const mp_obj_t *args) {
    int x = mp_obj_get_int(args[DRAW_RECT_ARG_X]);
    int y = mp_obj_get_int(args[DRAW_RECT_ARG_Y]);
    int w = mp_obj_get_int(args[DRAW_RECT_ARG_W]);
    int h = mp_obj_get_int(args[DRAW_RECT_ARG_H]);
    u32 color = mp_obj_get_int(args[DRAW_RECT_ARG_COLOR]);
    float rad = mp_obj_get_float(args[DRAW_RECT_ROT_ARG_RAD]);

    sf2d_draw_rectangle_rotate(x, y, w, h, color, rad);

    return mp_const_none;
}

enum {
    DRAW_CIRCLE_X = 0,
    DRAW_CIRCLE_Y,
    DRAW_CIRCLE_R,
    DRAW_CIRCLE_COLOR
};

STATIC mp_obj_t mod_sf2d_draw_fill_circle(size_t n_args, const mp_obj_t *args) {
    int x = mp_obj_get_int(args[DRAW_CIRCLE_X]);
    int y = mp_obj_get_int(args[DRAW_CIRCLE_Y]);
    int r = mp_obj_get_int(args[DRAW_CIRCLE_R]);
    u32 color = mp_obj_get_int(args[DRAW_CIRCLE_COLOR]);

    sf2d_draw_fill_circle(x, y, r, color);

    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_rgba8(size_t n_args, const mp_obj_t *args) {
    int r = mp_obj_get_int(args[0]);
    int g = mp_obj_get_int(args[1]);
    int b = mp_obj_get_int(args[2]);
    int a = mp_obj_get_int(args[3]);

    return mp_obj_new_int(RGBA8(r, g, b, a));
}

STATIC mp_obj_t mod_sf2d_rgba8_get_r(mp_obj_t rgba8) {
    int color = mp_obj_get_int(rgba8);

    return mp_obj_new_int(RGBA8_GET_R(color));
}

STATIC mp_obj_t mod_sf2d_rgba8_get_g(mp_obj_t rgba8) {
    int color = mp_obj_get_int(rgba8);

    return mp_obj_new_int(RGBA8_GET_G(color));
}

STATIC mp_obj_t mod_sf2d_rgba8_get_b(mp_obj_t rgba8) {
    int color = mp_obj_get_int(rgba8);

    return mp_obj_new_int(RGBA8_GET_B(color));
}

STATIC mp_obj_t mod_sf2d_rgba8_get_a(mp_obj_t rgba8) {
    int color = mp_obj_get_int(rgba8);

    return mp_obj_new_int(RGBA8_GET_A(color));
}

enum {
    SCISSOR_ARG_MODE = 0,
    SCISSOR_ARG_X,
    SCISSOR_ARG_Y,
    SCISSOR_ARG_W,
    SCISSOR_ARG_H
};

STATIC mp_obj_t mod_sf2d_set_scissor_test(size_t n_args, const mp_obj_t *args) {
    // mode, x, y, w, h
    int mode = mp_obj_get_int(args[SCISSOR_ARG_MODE]);
    int x = mp_obj_get_int(args[SCISSOR_ARG_X]);
    int y = mp_obj_get_int(args[SCISSOR_ARG_Y]);
    int w = mp_obj_get_int(args[SCISSOR_ARG_W]);
    int h = mp_obj_get_int(args[SCISSOR_ARG_H]);

    sf2d_set_scissor_test(mode, x, y, w, h);

    return mp_const_none;
}

STATIC mp_obj_t mod_sf2d_get_current_screen(void) {
    return mp_obj_new_int(sf2d_get_current_screen());
}

STATIC mp_obj_t mod_sf2d_get_current_side(void) {
    return mp_obj_new_int(sf2d_get_current_side());
}

METHOD_OBJ_N(0, init);
METHOD_OBJ_N(2, init_advanced);
METHOD_OBJ_N(0, fini);
METHOD_OBJ_N(1, set_3d);
METHOD_OBJ_N(2, start_frame);
METHOD_OBJ_N(0, end_frame);
METHOD_OBJ_N(0, swap_buffers);
METHOD_OBJ_N(1, set_vblank_wait);
METHOD_OBJ_N(0, get_fps);
METHOD_OBJ_N(1, set_clear_color);
METHOD_OBJ_VAR_N(6, draw_line);
METHOD_OBJ_VAR_N(5, draw_rectangle);
METHOD_OBJ_VAR_N(6, draw_rectangle_rotate);
METHOD_OBJ_VAR_N(4, draw_fill_circle);
METHOD_OBJ_VAR_N(4, rgba8);
METHOD_OBJ_N(1, rgba8_get_r);
METHOD_OBJ_N(1, rgba8_get_g);
METHOD_OBJ_N(1, rgba8_get_b);
METHOD_OBJ_N(1, rgba8_get_a);
METHOD_OBJ_VAR_N(5, set_scissor_test);
METHOD_OBJ_N(0, get_current_screen);
METHOD_OBJ_N(0, get_current_side);

STATIC const mp_rom_map_elem_t mp_module_sf2d_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_sf2d)},

        // Classes
        LOCAL_CLASS(Texture),
        LOCAL_CLASS(RenderTarget),

        // Functions
        LOCAL_METHOD(init),
        LOCAL_METHOD(init_advanced),
        LOCAL_METHOD(fini),
        LOCAL_METHOD(set_3d),
        LOCAL_METHOD(start_frame),
        LOCAL_METHOD(end_frame),
        LOCAL_METHOD(swap_buffers),
        LOCAL_METHOD(set_vblank_wait),
        LOCAL_METHOD(get_fps),
        LOCAL_METHOD(set_clear_color),
        LOCAL_METHOD(draw_line),
        LOCAL_METHOD(draw_rectangle),
        LOCAL_METHOD(draw_rectangle_rotate),
        LOCAL_METHOD(draw_fill_circle),
        LOCAL_METHOD(rgba8),
        LOCAL_METHOD(rgba8_get_r),
        LOCAL_METHOD(rgba8_get_g),
        LOCAL_METHOD(rgba8_get_b),
        LOCAL_METHOD(rgba8_get_a),
        LOCAL_METHOD(set_scissor_test),
        LOCAL_METHOD(get_current_screen),
        LOCAL_METHOD(get_current_side),

        // sf2d_texfmt
        LOCAL_INT(TEXFMT_RGBA8, TEXFMT_RGBA8),
        LOCAL_INT(TEXFMT_RGB8, TEXFMT_RGB8),
        LOCAL_INT(TEXFMT_RGB5A1, TEXFMT_RGB5A1),
        LOCAL_INT(TEXFMT_RGB565, TEXFMT_RGB565),
        LOCAL_INT(TEXFMT_RGBA4, TEXFMT_RGBA4),
        LOCAL_INT(TEXFMT_IA8, TEXFMT_IA8),
        LOCAL_INT(TEXFMT_I8, TEXFMT_I8),
        LOCAL_INT(TEXFMT_A8, TEXFMT_A8),
        LOCAL_INT(TEXFMT_IA4, TEXFMT_IA4),
        LOCAL_INT(TEXFMT_I4, TEXFMT_I4),
        LOCAL_INT(TEXFMT_A4, TEXFMT_A4),
        LOCAL_INT(TEXFMT_ETC1, TEXFMT_ETC1),
        LOCAL_INT(TEXFMT_ETC1A4, TEXFMT_ETC1A4),

        // sf2d_place
        LOCAL_INT(PLACE_RAM, SF2D_PLACE_RAM),
        LOCAL_INT(PLACE_VRAM, SF2D_PLACE_VRAM),
        LOCAL_INT(PLACE_TEMP, SF2D_PLACE_TEMP),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_sf2d_globals, mp_module_sf2d_globals_table);

const mp_obj_module_t mp_module_sf2d = {
        .base = {&mp_type_module},
        .name = MP_QSTR_sf2d,
        .globals = (mp_obj_dict_t *) &mp_module_sf2d_globals,
};