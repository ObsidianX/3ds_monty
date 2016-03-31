#ifndef _BASIC_CONSOLE_H_
#define _BASIC_CONSOLE_H_

#include <3ds.h>

inline void init_console(void) {
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);
}

#endif // _BASIC_CONSOLE_H_