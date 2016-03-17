#ifndef _CITRUS_HELPERS_H_
#define _CITRUS_HELPERS_H_

#include <3ds.h>

#include "py/runtime.h"

#include "../helpers.h"

// Console
mp_obj_t _mod_citrus_console_PrintConsole_new_default(void);

mp_obj_t _mod_citrus_console_PrintConsole_new_from_c(PrintConsole *con);

// GFX
gfxScreen_t _mod_citrus_gfx_get_gfx_screen(mp_obj_t screen);

gfxScreen_t _mod_citrus_gfx_is_gfx_screen(mp_int_t screen);

gfx3dSide_t _mod_citrus_gfx_get_gfx_3d_side(mp_obj_t side);

// GSP
GSPGPU_FramebufferFormats _mod_citrus_gsp_get_framebuffer_format(mp_obj_t format);

// GPU
GPU_TEXUNIT _mod_citrus_gpu_get_texunit(mp_obj_t unit);

// HTTPC
HTTPC_RequestMethod _mod_citrus_httpc_get_request_method(mp_int_t method);

// FS
FS_MediaType _mod_citrus_fs_get_mediatype(mp_obj_t type);

// SOC
bool _mod_citrus_soc_is_init(void);

#endif // _CITRUS_HELPERS_H_