#ifndef _3DS_CONFIG_H_
#define _3DS_CONFIG_H_

// Available build modes:
#define _3DS_MODE_REPL    (0)
#define _3DS_MODE_NETLOAD (1)
#define _3DS_MODE_ROMFS   (2)
#define _3DS_MODE_ARGS    (3)

#ifndef _3DS_MODE
#define _3DS_MODE (_3DS_MODE_NETLOAD)
#endif

#endif // _3DS_CONFIG_H_