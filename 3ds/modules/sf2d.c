#include <sf2d.h>

#include "py/runtime.h"

#include "citrus/helpers.h"

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
    return mp_obj_new_int(sf2d_init());
}

STATIC mp_obj_t mod_sf2d_init_advanced(mp_obj_t gpucmd_size, mp_obj_t temppool_size) {
    int gpu = mp_obj_get_int(gpucmd_size);
    int pool = mp_obj_get_int(temppool_size);

    return mp_obj_new_int(sf2d_init_advanced(gpu, pool));
}

STATIC mp_obj_t mod_sf2d_fini(void) {
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

STATIC mp_obj_t mod_sf2d_swapbuffers(void) {
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

STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_sf2d_init_obj, mod_sf2d_init);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_sf2d_init_advanced_obj, mod_sf2d_init_advanced);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_sf2d_fini_obj, mod_sf2d_fini);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_sf2d_set_3d_obj, mod_sf2d_set_3d);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_sf2d_start_frame_obj, mod_sf2d_start_frame);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_sf2d_end_frame_obj, mod_sf2d_end_frame);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_sf2d_swapbuffers_obj, mod_sf2d_swapbuffers);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_sf2d_set_vblank_wait_obj, mod_sf2d_set_vblank_wait);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_sf2d_get_fps_obj, mod_sf2d_get_fps);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_sf2d_set_clear_color_obj, mod_sf2d_set_clear_color);
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_sf2d_draw_line_obj, 6, 6, mod_sf2d_draw_line);
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_sf2d_draw_rectangle_obj, 5, 5, mod_sf2d_draw_rectangle);
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_sf2d_draw_rectangle_rotate_obj, 6, 6, mod_sf2d_draw_rectangle_rotate);
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_sf2d_draw_fill_circle_obj, 4, 4, mod_sf2d_draw_fill_circle);
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_sf2d_rgba8_obj, 4, 4, mod_sf2d_rgba8);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_sf2d_rgba8_get_r_obj, mod_sf2d_rgba8_get_r);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_sf2d_rgba8_get_g_obj, mod_sf2d_rgba8_get_g);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_sf2d_rgba8_get_b_obj, mod_sf2d_rgba8_get_b);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_sf2d_rgba8_get_a_obj, mod_sf2d_rgba8_get_a);
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(mod_sf2d_set_scissor_test_obj, 5, 5, mod_sf2d_set_scissor_test);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_sf2d_get_current_screen_obj, mod_sf2d_get_current_screen);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_sf2d_get_current_side_obj, mod_sf2d_get_current_side);

STATIC const mp_rom_map_elem_t mp_module_sf2d_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__),              MP_ROM_QSTR(MP_QSTR_sf2d)},

        // Functions
        {MP_ROM_QSTR(MP_QSTR_init),                  MP_ROM_PTR(&mod_sf2d_init_obj)},
        {MP_ROM_QSTR(MP_QSTR_init_advanced),         MP_ROM_PTR(&mod_sf2d_init_advanced_obj)},
        {MP_ROM_QSTR(MP_QSTR_fini),                  MP_ROM_PTR(&mod_sf2d_fini_obj)},
        {MP_ROM_QSTR(MP_QSTR_set_3d),                MP_ROM_PTR(&mod_sf2d_set_3d_obj)},
        {MP_ROM_QSTR(MP_QSTR_start_frame),           MP_ROM_PTR(&mod_sf2d_start_frame_obj)},
        {MP_ROM_QSTR(MP_QSTR_end_frame),             MP_ROM_PTR(&mod_sf2d_end_frame_obj)},
        {MP_ROM_QSTR(MP_QSTR_swapbuffers),           MP_ROM_PTR(&mod_sf2d_swapbuffers_obj)},
        {MP_ROM_QSTR(MP_QSTR_set_vblank_wait),       MP_ROM_PTR(&mod_sf2d_set_vblank_wait_obj)},
        {MP_ROM_QSTR(MP_QSTR_get_fps),               MP_ROM_PTR(&mod_sf2d_get_fps_obj)},
        {MP_ROM_QSTR(MP_QSTR_set_clear_color),       MP_ROM_PTR(&mod_sf2d_set_clear_color_obj)},
        {MP_ROM_QSTR(MP_QSTR_draw_line),             MP_ROM_PTR(&mod_sf2d_draw_line_obj)},
        {MP_ROM_QSTR(MP_QSTR_draw_rectangle),        MP_ROM_PTR(&mod_sf2d_draw_rectangle_obj)},
        {MP_ROM_QSTR(MP_QSTR_draw_rectangle_rotate), MP_ROM_PTR(&mod_sf2d_draw_rectangle_rotate_obj)},
        {MP_ROM_QSTR(MP_QSTR_draw_fill_circle),      MP_ROM_PTR(&mod_sf2d_draw_fill_circle_obj)},
        {MP_ROM_QSTR(MP_QSTR_rgba8),                 MP_ROM_PTR(&mod_sf2d_rgba8_obj)},
        {MP_ROM_QSTR(MP_QSTR_rgba8_get_r),           MP_ROM_PTR(&mod_sf2d_rgba8_get_r_obj)},
        {MP_ROM_QSTR(MP_QSTR_rgba8_get_g),           MP_ROM_PTR(&mod_sf2d_rgba8_get_g_obj)},
        {MP_ROM_QSTR(MP_QSTR_rgba8_get_b),           MP_ROM_PTR(&mod_sf2d_rgba8_get_b_obj)},
        {MP_ROM_QSTR(MP_QSTR_rgba8_get_a),           MP_ROM_PTR(&mod_sf2d_rgba8_get_a_obj)},
        {MP_ROM_QSTR(MP_QSTR_set_scissor_test),      MP_ROM_PTR(&mod_sf2d_set_scissor_test_obj)},
        {MP_ROM_QSTR(MP_QSTR_get_current_screen),    MP_ROM_PTR(&mod_sf2d_get_current_screen_obj)},
        {MP_ROM_QSTR(MP_QSTR_get_current_side),      MP_ROM_PTR(&mod_sf2d_get_current_side_obj)},

        // sf2d_texfmt
        {MP_ROM_QSTR(MP_QSTR_TEXFMT_RGBA8),          MP_ROM_INT(TEXFMT_RGBA8)},
        {MP_ROM_QSTR(MP_QSTR_TEXFMT_RGB8),           MP_ROM_INT(TEXFMT_RGB8)},
        {MP_ROM_QSTR(MP_QSTR_TEXFMT_RGB5A1),         MP_ROM_INT(TEXFMT_RGB5A1)},
        {MP_ROM_QSTR(MP_QSTR_TEXFMT_RGB565),         MP_ROM_INT(TEXFMT_RGB565)},
        {MP_ROM_QSTR(MP_QSTR_TEXFMT_RGBA4),          MP_ROM_INT(TEXFMT_RGBA4)},
        {MP_ROM_QSTR(MP_QSTR_TEXFMT_IA8),            MP_ROM_INT(TEXFMT_IA8)},
        {MP_ROM_QSTR(MP_QSTR_TEXFMT_I8),             MP_ROM_INT(TEXFMT_I8)},
        {MP_ROM_QSTR(MP_QSTR_TEXFMT_A8),             MP_ROM_INT(TEXFMT_A8)},
        {MP_ROM_QSTR(MP_QSTR_TEXFMT_IA4),            MP_ROM_INT(TEXFMT_IA4)},
        {MP_ROM_QSTR(MP_QSTR_TEXFMT_I4),             MP_ROM_INT(TEXFMT_I4)},
        {MP_ROM_QSTR(MP_QSTR_TEXFMT_A4),             MP_ROM_INT(TEXFMT_A4)},
        {MP_ROM_QSTR(MP_QSTR_TEXFMT_ETC1),           MP_ROM_INT(TEXFMT_ETC1)},
        {MP_ROM_QSTR(MP_QSTR_TEXFMT_ETC1A4),         MP_ROM_INT(TEXFMT_ETC1A4)},

        // sf2d_place
        {MP_ROM_QSTR(MP_QSTR_PLACE_RAM),             MP_ROM_INT(SF2D_PLACE_RAM)},
        {MP_ROM_QSTR(MP_QSTR_PLACE_VRAM),            MP_ROM_INT(SF2D_PLACE_VRAM)},
        {MP_ROM_QSTR(MP_QSTR_PLACE_TEMP),            MP_ROM_INT(SF2D_PLACE_TEMP)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_sf2d_globals, mp_module_sf2d_globals_table);

const mp_obj_module_t mp_module_sf2d = {
        .base = {&mp_type_module},
        .name = MP_QSTR_sf2d,
        .globals = (mp_obj_dict_t *) &mp_module_sf2d_globals,
};