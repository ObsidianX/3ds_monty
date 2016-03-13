#include <3ds.h>

#include "py/runtime.h"

#include "helpers.h"

int _mod_citrus_gsp_get_framebuffer_format(mp_obj_t format) {
    if (mp_obj_is_integer(format)) {
        int fmt = mp_obj_get_int(format);
        if (fmt >= GSP_RGBA8_OES && fmt <= GSP_RGBA4_OES) {
            return fmt;
        }
    }

    nlr_raise(mp_obj_new_exception(&mp_type_TypeError));
}

int _mod_citrus_gsp_get_gpu_event(mp_obj_t event) {
    if (mp_obj_is_integer(event)) {
        int evt = mp_obj_get_int(event);
        if (evt >= GSPGPU_EVENT_PSC0 && evt < GSPGPU_EVENT_MAX) {
            return evt;
        }
    }

    nlr_raise(mp_obj_new_exception(&mp_type_TypeError));
}

STATIC mp_obj_t mod_citrus_gsp_init(void) {
    gspInit();

    return mp_const_none;
}

STATIC mp_obj_t mod_citrus_gsp_exit(void) {
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

STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gsp_init_obj, mod_citrus_gsp_init);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gsp_exit_obj, mod_citrus_gsp_exit);
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(mod_citrus_gsp_set_event_callback_obj, 4, mod_citrus_gsp_set_event_callback);
STATIC MP_DEFINE_CONST_FUN_OBJ_3(mod_citrus_gsp_init_event_handler_obj, mod_citrus_gsp_init_event_handler);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gsp_exit_event_handler_obj, mod_citrus_gsp_exit_event_handler);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_citrus_gsp_wait_for_event_obj, mod_citrus_gsp_wait_for_event);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gsp_wait_for_any_event_obj, mod_citrus_gsp_wait_for_any_event);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gsp_wait_for_psc0_obj, mod_citrus_gsp_wait_for_psc0);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gsp_wait_for_psc1_obj, mod_citrus_gsp_wait_for_psc1);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gsp_wait_for_vblank_obj, mod_citrus_gsp_wait_for_vblank);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gsp_wait_for_vblank0_obj, mod_citrus_gsp_wait_for_vblank0);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gsp_wait_for_vblank1_obj, mod_citrus_gsp_wait_for_vblank1);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gsp_wait_for_ppf_obj, mod_citrus_gsp_wait_for_ppf);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gsp_wait_for_p3d_obj, mod_citrus_gsp_wait_for_p3d);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gsp_wait_for_dma_obj, mod_citrus_gsp_wait_for_dma);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_citrus_gsp_submit_gx_command_obj, mod_citrus_gsp_submit_gx_command);

STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_gsp_gpu_acquire_right_obj, mod_citrus_gsp_gpu_acquire_right);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gsp_gpu_release_right_obj, mod_citrus_gsp_gpu_release_right);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_gsp_gpu_import_display_capture_info_obj, mod_citrus_gsp_gpu_import_display_capture_info);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gsp_gpu_save_vram_sys_area_obj, mod_citrus_gsp_gpu_save_vram_sys_area);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gsp_gpu_restore_vram_sys_area_obj, mod_citrus_gsp_gpu_restore_vram_sys_area);
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mod_citrus_gsp_gpu_set_lcd_force_black_obj, mod_citrus_gsp_gpu_set_lcd_force_black);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_citrus_gsp_gpu_set_buffer_swap_obj, mod_citrus_gsp_gpu_set_buffer_swap);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_citrus_gsp_gpu_flush_data_cache_obj, mod_citrus_gsp_gpu_flush_data_cache);
STATIC MP_DEFINE_CONST_FUN_OBJ_2(mod_citrus_gsp_gpu_invalidate_data_cache_obj, mod_citrus_gsp_gpu_invalidate_data_cache);
STATIC MP_DEFINE_CONST_FUN_OBJ_3(mod_citrus_gsp_gpu_write_hw_regs_obj, mod_citrus_gsp_gpu_write_hw_regs);
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(mod_citrus_gsp_gpu_write_hw_regs_with_mask_obj, 5, mod_citrus_gsp_gpu_write_hw_regs_with_mask);
STATIC MP_DEFINE_CONST_FUN_OBJ_3(mod_citrus_gsp_gpu_read_hw_regs_obj, mod_citrus_gsp_gpu_read_hw_regs);
STATIC MP_DEFINE_CONST_FUN_OBJ_KW(mod_citrus_gsp_gpu_register_interrupt_relay_queue_obj, 4, mod_citrus_gsp_gpu_register_interrupt_relay_queue);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gsp_gpu_unregister_interrupt_relay_queue_obj, mod_citrus_gsp_gpu_unregister_interrupt_relay_queue);
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mod_citrus_gsp_gpu_trigger_cmd_req_queue_obj, mod_citrus_gsp_gpu_trigger_cmd_req_queue);

STATIC const mp_rom_map_elem_t mp_module_citrus_gsp_globals_table[] = {
        // Package Info
        {MP_ROM_QSTR(MP_QSTR___name__),                             MP_ROM_QSTR(MP_QSTR_gsp)},

        // Functions
        {MP_ROM_QSTR(MP_QSTR_init),                                 MP_ROM_PTR(&mod_citrus_gsp_init_obj)},
        {MP_ROM_QSTR(MP_QSTR_exit),                                 MP_ROM_PTR(&mod_citrus_gsp_exit_obj)},
        {MP_ROM_QSTR(MP_QSTR_set_event_callback),                   MP_ROM_PTR(&mod_citrus_gsp_set_event_callback_obj)},
        {MP_ROM_QSTR(MP_QSTR_init_event_handler),                   MP_ROM_PTR(&mod_citrus_gsp_init_event_handler_obj)},
        {MP_ROM_QSTR(MP_QSTR_exit_event_handler),                   MP_ROM_PTR(&mod_citrus_gsp_exit_event_handler_obj)},
        {MP_ROM_QSTR(MP_QSTR_wait_for_event),                       MP_ROM_PTR(&mod_citrus_gsp_wait_for_event_obj)},
        {MP_ROM_QSTR(MP_QSTR_wait_for_any_event),                   MP_ROM_PTR(&mod_citrus_gsp_wait_for_any_event_obj)},
        {MP_ROM_QSTR(MP_QSTR_wait_for_psc0),                        MP_ROM_PTR(&mod_citrus_gsp_wait_for_psc0_obj)},
        {MP_ROM_QSTR(MP_QSTR_wait_for_psc1),                        MP_ROM_PTR(&mod_citrus_gsp_wait_for_psc1_obj)},
        {MP_ROM_QSTR(MP_QSTR_wait_for_vblank),                      MP_ROM_PTR(&mod_citrus_gsp_wait_for_vblank_obj)},
        {MP_ROM_QSTR(MP_QSTR_wait_for_vblank0),                     MP_ROM_PTR(&mod_citrus_gsp_wait_for_vblank0_obj)},
        {MP_ROM_QSTR(MP_QSTR_wait_for_vblank1),                     MP_ROM_PTR(&mod_citrus_gsp_wait_for_vblank1_obj)},
        {MP_ROM_QSTR(MP_QSTR_wait_for_ppf),                         MP_ROM_PTR(&mod_citrus_gsp_wait_for_ppf_obj)},
        {MP_ROM_QSTR(MP_QSTR_wait_for_p3d),                         MP_ROM_PTR(&mod_citrus_gsp_wait_for_p3d_obj)},
        {MP_ROM_QSTR(MP_QSTR_wait_for_dma),                         MP_ROM_PTR(&mod_citrus_gsp_wait_for_dma_obj)},
        {MP_ROM_QSTR(MP_QSTR_submit_gx_command),                    MP_ROM_PTR(&mod_citrus_gsp_submit_gx_command_obj)},

        {MP_ROM_QSTR(MP_QSTR_gpu_acquire_right),                    MP_ROM_PTR(&mod_citrus_gsp_gpu_acquire_right_obj)},
        {MP_ROM_QSTR(MP_QSTR_gpu_release_right),                    MP_ROM_PTR(&mod_citrus_gsp_gpu_release_right_obj)},
        {MP_ROM_QSTR(MP_QSTR_gpu_import_display_capture_info),      MP_ROM_PTR(&mod_citrus_gsp_gpu_import_display_capture_info_obj)},
        {MP_ROM_QSTR(MP_QSTR_gpu_save_vram_sys_area),               MP_ROM_PTR(&mod_citrus_gsp_gpu_save_vram_sys_area_obj)},
        {MP_ROM_QSTR(MP_QSTR_gpu_restore_vram_sys_area),            MP_ROM_PTR(&mod_citrus_gsp_gpu_restore_vram_sys_area_obj)},
        {MP_ROM_QSTR(MP_QSTR_gpu_set_lcd_force_black),              MP_ROM_PTR(&mod_citrus_gsp_gpu_set_lcd_force_black_obj)},
        {MP_ROM_QSTR(MP_QSTR_gpu_set_buffer_swap),                  MP_ROM_PTR(&mod_citrus_gsp_gpu_set_buffer_swap_obj)},
        {MP_ROM_QSTR(MP_QSTR_gpu_flush_data_cache),                 MP_ROM_PTR(&mod_citrus_gsp_gpu_flush_data_cache_obj)},
        {MP_ROM_QSTR(MP_QSTR_gpu_invalidate_data_cache),            MP_ROM_PTR(&mod_citrus_gsp_gpu_invalidate_data_cache_obj)},
        {MP_ROM_QSTR(MP_QSTR_gpu_write_hw_regs),                    MP_ROM_PTR(&mod_citrus_gsp_gpu_write_hw_regs_obj)},
        {MP_ROM_QSTR(MP_QSTR_gpu_write_hw_regs_with_mask),          MP_ROM_PTR(&mod_citrus_gsp_gpu_write_hw_regs_with_mask_obj)},
        {MP_ROM_QSTR(MP_QSTR_gpu_read_hw_regs),                     MP_ROM_PTR(&mod_citrus_gsp_gpu_read_hw_regs_obj)},
        {MP_ROM_QSTR(MP_QSTR_gpu_register_interrupt_relay_queue),   MP_ROM_PTR(&mod_citrus_gsp_gpu_register_interrupt_relay_queue_obj)},
        {MP_ROM_QSTR(MP_QSTR_gpu_unregister_interrupt_relay_queue), MP_ROM_PTR(&mod_citrus_gsp_gpu_unregister_interrupt_relay_queue_obj)},
        {MP_ROM_QSTR(MP_QSTR_gpu_trigger_cmd_req_queue),            MP_ROM_PTR(&mod_citrus_gsp_gpu_trigger_cmd_req_queue_obj)},

        // GSPGPU_FramebufferFormats
        {MP_ROM_QSTR(MP_QSTR_GSP_FB_FORMAT_RGBA8),                  MP_ROM_INT(GSP_RGBA8_OES)},
        {MP_ROM_QSTR(MP_QSTR_GSP_FB_FORMAT_BGR8),                   MP_ROM_INT(GSP_BGR8_OES)},
        {MP_ROM_QSTR(MP_QSTR_GSP_FB_FORMAT_RGB565),                 MP_ROM_INT(GSP_RGB565_OES)},
        {MP_ROM_QSTR(MP_QSTR_GSP_FB_FORMAT_RGB5_A1),                MP_ROM_INT(GSP_RGB5_A1_OES)},
        {MP_ROM_QSTR(MP_QSTR_GSP_FB_FORMAT_RGBA4),                  MP_ROM_INT(GSP_RGBA4_OES)},

        // GSPGPU_Event
        {MP_ROM_QSTR(MP_QSTR_GSP_EVENT_PSC0),                       MP_ROM_INT(GSPGPU_EVENT_PSC0)},
        {MP_ROM_QSTR(MP_QSTR_GSP_EVENT_PSC1),                       MP_ROM_INT(GSPGPU_EVENT_PSC1)},
        {MP_ROM_QSTR(MP_QSTR_GSP_EVENT_VBLANK0),                    MP_ROM_INT(GSPGPU_EVENT_VBlank0)},
        {MP_ROM_QSTR(MP_QSTR_GSP_EVENT_VBLANK1),                    MP_ROM_INT(GSPGPU_EVENT_VBlank1)},
        {MP_ROM_QSTR(MP_QSTR_GSP_EVENT_PPF),                        MP_ROM_INT(GSPGPU_EVENT_PPF)},
        {MP_ROM_QSTR(MP_QSTR_GSP_EVENT_P3D),                        MP_ROM_INT(GSPGPU_EVENT_P3D)},
        {MP_ROM_QSTR(MP_QSTR_GSP_EVENT_DMA),                        MP_ROM_INT(GSPGPU_EVENT_DMA)},
        {MP_ROM_QSTR(MP_QSTR_GSP_EVENT_MAX),                        MP_ROM_INT(GSPGPU_EVENT_MAX)},
};

STATIC MP_DEFINE_CONST_DICT(mp_module_citrus_gsp_globals, mp_module_citrus_gsp_globals_table);

const mp_obj_module_t mp_module_citrus_gsp = {
        .base = {&mp_type_module},
        .name = MP_QSTR_gsp,
        .globals = (mp_obj_dict_t *) &mp_module_citrus_gsp_globals,
};
