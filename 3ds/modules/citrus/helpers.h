#ifndef _HELPERS_H_
#define _HELPERS_H_

#include "py/runtime.h"

#include "../helpers.h"

// Console
mp_obj_t _mod_citrus_console_PrintConsole_new_default(void);

mp_obj_t _mod_citrus_console_PrintConsole_new_from_c(PrintConsole *con);

// GFX
int _mod_citrus_gfx_get_gfx_screen(mp_obj_t screen);

int _mod_citrus_gfx_is_gfx_screen(mp_int_t screen);

// GSP
int _mod_citrus_gsp_get_framebuffer_format(mp_obj_t format);

// HTTPC
int _mod_citrus_httpc_get_request_method(int method);

#endif // _HELPERS_H_