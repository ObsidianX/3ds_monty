#include <3ds.h>

#include "py/runtime.h"

#include "helpers.h"
#include "../init_helper.h"

#define METHOD_OBJ_N(__args, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_##__args(mod_citrus_gsp_##__n##_obj, mod_citrus_gsp_##__n)
#define METHOD_OBJ_KW(__min, __n) \
    STATIC MP_DEFINE_CONST_FUN_OBJ_KW(mod_citrus_gsp_##__n##_obj, __min, mod_citrus_gsp_##__n)

#define LOCAL_METHOD(__n) \
    {MP_OBJ_NEW_QSTR(MP_QSTR_##__n), (mp_obj_t) &mod_citrus_gsp_##__n##_obj}
#define LOCAL_INT(__n, __v) \
    {MP_ROM_QSTR(MP_QSTR_##__n), MP_ROM_INT(__v)}

static int _mod_citrus_gsp_is_init = 0;

GSPGPU_FramebufferFormats _mod_citrus_gsp_get_framebuffer_format(mp_obj_t format) {
    if (mp_obj_is_integer(format)) {
        int fmt = mp_obj_get_int(format);
        if (fmt >= GSP_RGBA8_OES && fmt <= GSP_RGBA4_OES) {
            return fmt;
        }
    }

    nlr_raise(mp_obj_new_exception(&mp_type_TypeError));
}

GSPGPU_Event _mod_citrus_gsp_get_gpu_event(mp_obj_t event) {
    if (mp_obj_is_integer(event)) {
        int evt = mp_obj_get_int(event);
        if (evt >= GSPGPU_EVENT_PSC0 && evt < GSPGPU_EVENT_MAX) {
            return evt;
        }
    }

    nlr_raise(mp_obj_new_exception(&mp_type_TypeError));
}

STATIC mp_obj_t mod_citrus_gsp_init(void) {
    INIT_ONCE(_mod_citrus_gsp_is_init);

    gspInit();

    return mp_const_none;
}

mp_obj_t mod_citrus_gsp_exit(void) {
    EXIT_ONCE(_mod_citrus_gsp_is_init);

    gspExit();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gsp_set_event_callback(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_citrus_gsp_init_event_handler(mp_obj_t event, mp_obj_t gsp_shared_mem, mp_obj_t gsp_thread_id) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_citrus_gsp_exit_event_handler(void) {
    gspExitEventHandler();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gsp_wait_for_event(mp_obj_t id, mp_obj_t next_event) {
    int event = _mod_citrus_gsp_get_gpu_event(id);
    gspWaitForEvent(event, mp_obj_is_true(next_event));

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gsp_wait_for_any_event(void) {
    gspWaitForAnyEvent();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gsp_wait_for_psc0(void) {
    gspWaitForPSC0();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gsp_wait_for_psc1(void) {
    gspWaitForPSC1();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gsp_wait_for_vblank(void) {
    gspWaitForVBlank();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gsp_wait_for_vblank0(void) {
    gspWaitForVBlank0();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gsp_wait_for_vblank1(void) {
    gspWaitForVBlank1();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gsp_wait_for_ppf(void) {
    gspWaitForPPF();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gsp_wait_for_p3d(void) {
    gspWaitForP3D();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gsp_wait_for_dma(void) {
    gspWaitForDMA();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gsp_submit_gx_command(mp_obj_t cmd_buf, mp_obj_t cmd) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_citrus_gsp_gpu_acquire_right(mp_obj_t flags) {
    u8 _flags = mp_obj_get_int(flags);

    return mp_obj_new_int(GSPGPU_AcquireRight(_flags));
}

STATIC mp_obj_t mod_citrus_gsp_gpu_release_right(void) {
    return mp_obj_new_int(GSPGPU_ReleaseRight());
}

STATIC mp_obj_t mod_citrus_gsp_gpu_import_display_capture_info(mp_obj_t info) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_citrus_gsp_gpu_save_vram_sys_area(void) {
    return mp_obj_new_int(GSPGPU_SaveVramSysArea());
}

STATIC mp_obj_t mod_citrus_gsp_gpu_restore_vram_sys_area(void) {
    return mp_obj_new_int(GSPGPU_RestoreVramSysArea());
}

STATIC mp_obj_t mod_citrus_gsp_gpu_set_lcd_force_black(mp_obj_t flags) {
    u8 _flags = mp_obj_get_int(flags);
    return mp_obj_new_int(GSPGPU_SetLcdForceBlack(_flags));
}

STATIC mp_obj_t mod_citrus_gsp_gpu_set_buffer_swap(mp_obj_t screen, mp_obj_t framebuffer) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_citrus_gsp_gpu_flush_data_cache(mp_obj_t addr, mp_obj_t size) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_citrus_gsp_gpu_invalidate_data_cache(mp_obj_t addr, mp_obj_t size) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_citrus_gsp_gpu_write_hw_regs(mp_obj_t reg_addr, mp_obj_t data, mp_obj_t size) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_citrus_gsp_gpu_write_hw_regs_with_mask(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_citrus_gsp_gpu_read_hw_regs(mp_obj_t reg_addr, mp_obj_t data, mp_obj_t size) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_citrus_gsp_gpu_register_interrupt_relay_queue(size_t n_args, const mp_obj_t *args, mp_map_t *kw_args) {
    nlr_raise(mp_obj_new_exception(&mp_type_NotImplementedError));
}

STATIC mp_obj_t mod_citrus_gsp_gpu_unregister_interrupt_relay_queue(void) {
    return mp_obj_new_int(GSPGPU_UnregisterInterruptRelayQueue());
}

STATIC mp_obj_t mod_citrus_gsp_gpu_trigger_cmd_req_queue(void) {
    return mp_obj_new_int(GSPGPU_TriggerCmdReqQueue());
}

METHOD_OBJ_N(0, init);
METHOD_OBJ_N(0, exit);
METHOD_OBJ_KW(4, set_event_callback);
METHOD_OBJ_N(3, init_event_handler);
METHOD_OBJ_N(0, exit_event_handler);
METHOD_OBJ_N(2, wait_for_event);
METHOD_OBJ_N(0, wait_for_any_event);
METHOD_OBJ_N(0, wait_for_psc0);
METHOD_OBJ_N(0, wait_for_psc1);
METHOD_OBJ_N(0, wait_for_vblank);
METHOD_OBJ_N(0, wait_for_vblank0);
METHOD_OBJ_N(0, wait_for_vblank1);
METHOD_OBJ_N(0, wait_for_ppf);
METHOD_OBJ_N(0, wait_for_p3d);
METHOD_OBJ_N(0, wait_for_dma);
METHOD_OBJ_N(2, submit_gx_command);

METHOD_OBJ_N(1, gpu_acquire_right);
METHOD_OBJ_N(0, gpu_release_right);
METHOD_OBJ_N(1, gpu_import_display_capture_info);
METHOD_OBJ_N(0, gpu_save_vram_sys_area);
METHOD_OBJ_N(0, gpu_restore_vram_sys_area);
METHOD_OBJ_N(1, gpu_set_lcd_force_black);
METHOD_OBJ_N(2, gpu_set_buffer_swap);
METHOD_OBJ_N(2, gpu_flush_data_cache);
METHOD_OBJ_N(2, gpu_invalidate_data_cache);
METHOD_OBJ_N(3, gpu_write_hw_regs);
METHOD_OBJ_KW(5, gpu_write_hw_regs_with_mask);
METHOD_OBJ_N(3, gpu_read_hw_regs);
METHOD_OBJ_KW(4, gpu_register_interrupt_relay_queue);
METHOD_OBJ_N(0, gpu_unregister_interrupt_relay_queue);
METHOD_OBJ_N(0, gpu_trigger_cmd_req_queue);

STATIC const mp_rom_map_elem_t mp_module_citrus_gsp_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__), MP_ROM_QSTR(MP_QSTR_gsp)},

        // Functions
        LOCAL_METHOD(init),
        LOCAL_METHOD(exit),
        LOCAL_METHOD(set_event_callback),
        LOCAL_METHOD(init_event_handler),
        LOCAL_METHOD(exit_event_handler),
        LOCAL_METHOD(wait_for_event),
        LOCAL_METHOD(wait_for_any_event),
        LOCAL_METHOD(wait_for_psc0),
        LOCAL_METHOD(wait_for_psc1),
        LOCAL_METHOD(wait_for_vblank),
        LOCAL_METHOD(wait_for_vblank0),
        LOCAL_METHOD(wait_for_vblank1),
        LOCAL_METHOD(wait_for_ppf),
        LOCAL_METHOD(wait_for_p3d),
        LOCAL_METHOD(wait_for_dma),
        LOCAL_METHOD(submit_gx_command),

        LOCAL_METHOD(gpu_acquire_right),
        LOCAL_METHOD(gpu_release_right),
        LOCAL_METHOD(gpu_import_display_capture_info),
        LOCAL_METHOD(gpu_save_vram_sys_area),
        LOCAL_METHOD(gpu_restore_vram_sys_area),
        LOCAL_METHOD(gpu_set_lcd_force_black),
        LOCAL_METHOD(gpu_set_buffer_swap),
        LOCAL_METHOD(gpu_flush_data_cache),
        LOCAL_METHOD(gpu_invalidate_data_cache),
        LOCAL_METHOD(gpu_write_hw_regs),
        LOCAL_METHOD(gpu_write_hw_regs_with_mask),
        LOCAL_METHOD(gpu_read_hw_regs),
        LOCAL_METHOD(gpu_register_interrupt_relay_queue),
        LOCAL_METHOD(gpu_unregister_interrupt_relay_queue),
        LOCAL_METHOD(gpu_trigger_cmd_req_queue),

        // GSPGPU_FramebufferFormats
        LOCAL_INT(GSP_FB_FORMAT_RGBA8, GSP_RGBA8_OES),
        LOCAL_INT(GSP_FB_FORMAT_BGR8, GSP_BGR8_OES),
        LOCAL_INT(GSP_FB_FORMAT_RGB565, GSP_RGB565_OES),
        LOCAL_INT(GSP_FB_FORMAT_RGB5_A1, GSP_RGB5_A1_OES),
        LOCAL_INT(GSP_FB_FORMAT_RGBA4, GSP_RGBA4_OES),

        // GSPGPU_Event
        LOCAL_INT(GSP_EVENT_PSC0, GSPGPU_EVENT_PSC0),
        LOCAL_INT(GSP_EVENT_PSC1, GSPGPU_EVENT_PSC1),
        LOCAL_INT(GSP_EVENT_VBLANK0, GSPGPU_EVENT_VBlank0),
        LOCAL_INT(GSP_EVENT_VBLANK1, GSPGPU_EVENT_VBlank1),
        LOCAL_INT(GSP_EVENT_PPF, GSPGPU_EVENT_PPF),
        LOCAL_INT(GSP_EVENT_P3D, GSPGPU_EVENT_P3D),
        LOCAL_INT(GSP_EVENT_DMA, GSPGPU_EVENT_DMA),
        LOCAL_INT(GSP_EVENT_MAX, GSPGPU_EVENT_MAX),
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_gsp_globals, mp_module_citrus_gsp_globals_table);

const mp_obj_module_t mp_module_citrus_gsp = {
        .base = {&mp_type_module},
        .name = MP_QSTR_gsp,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_gsp_globals,
};
