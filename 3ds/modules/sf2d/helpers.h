#ifndef _SF2D_HELPERS_H_
#define _SF2D_HELPERS_H_

#include "py/runtime.h"

#include <sf2d.h>

sf2d_texfmt _mod_sf2d_get_texfmt(mp_int_t format);

sf2d_place _mod_sf2d_get_place(mp_int_t place);

#endif // _SF2D_HELPERS_H_