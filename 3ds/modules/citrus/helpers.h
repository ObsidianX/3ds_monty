#ifndef _HELPERS_H_
#define _HELPERS_H_

#include "py/runtime.h"

// GFX
int _mod_citrus_gfx_get_gfx_screen(mp_obj_t screen);

// GSP
int _mod_citrus_gsp_get_framebuffer_format(mp_obj_t format);

#endif // _HELPERS_H_