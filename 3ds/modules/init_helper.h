#ifndef _INIT_HELPER_H_
#define _INIT_HELPER_H_

#define INIT_ONCE(var) \
    if(var) { \
        return mp_const_none; \
    } \
    var = 1;

#define EXIT_ONCE(var) \
    if(!var) {\
        return mp_const_none; \
    } \
    var = 0;

#endif // _INIT_HELPER_H_