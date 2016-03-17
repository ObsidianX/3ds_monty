#include <stdint.h>

// options to control how Micro Python is built

#define MICROPY_QSTR_BYTES_IN_HASH  (1)
#define MICROPY_ALLOC_PATH_MAX      (512)
#define MICROPY_EMIT_X64            (0)
#define MICROPY_EMIT_THUMB          (0)
#define MICROPY_EMIT_INLINE_THUMB   (0)
#define MICROPY_COMP_MODULE_CONST   (0)
#define MICROPY_COMP_CONST          (0)
#define MICROPY_COMP_DOUBLE_TUPLE_ASSIGN (0)
#define MICROPY_COMP_TRIPLE_TUPLE_ASSIGN (0)
#define MICROPY_MEM_STATS           (0)
#define MICROPY_DEBUG_PRINTERS      (0)
#define MICROPY_ENABLE_GC           (0)
#define MICROPY_HELPER_REPL         (0)
#define MICROPY_HELPER_LEXER_UNIX   (0)
#define MICROPY_ENABLE_SOURCE_LINE  (1)
#define MICROPY_ENABLE_DOC_STRING   (1)
#define MICROPY_ERROR_REPORTING     (MICROPY_ERROR_REPORTING_DETAILED)
#define MICROPY_BUILTIN_METHOD_CHECK_SELF_ARG (0)
#define MICROPY_PY_BUILTINS_BYTEARRAY (1)
#define MICROPY_PY_BUILTINS_MEMORYVIEW (0)
#define MICROPY_PY_BUILTINS_ENUMERATE (0)
#define MICROPY_PY_BUILTINS_FROZENSET (0)
#define MICROPY_PY_BUILTINS_REVERSED (0)
#define MICROPY_PY_BUILTINS_SET     (0)
#define MICROPY_PY_BUILTINS_SLICE   (0)
#define MICROPY_PY_BUILTINS_PROPERTY (0)
#define MICROPY_PY___FILE__         (1)
#define MICROPY_PY_GC               (0)
#define MICROPY_PY_ARRAY            (1)
#define MICROPY_PY_ATTRTUPLE        (1)
#define MICROPY_PY_COLLECTIONS      (1)
#define MICROPY_PY_MATH             (1)
#define MICROPY_PY_CMATH            (0)
#define MICROPY_PY_IO               (0)
#define MICROPY_PY_STRUCT           (1)
#define MICROPY_PY_SYS              (1)
#define MICROPY_CPYTHON_COMPAT      (0)
#define MICROPY_LONGINT_IMPL        (MICROPY_LONGINT_IMPL_LONGLONG)
#define MICROPY_FLOAT_IMPL          (MICROPY_FLOAT_IMPL_FLOAT)

// type definitions for the specific machine

#define BYTES_PER_WORD (4)

#define MICROPY_MAKE_POINTER_CALLABLE(p) ((void*)((mp_uint_t)(p) | 1))

#define UINT_FMT "%lu"
#define INT_FMT "%ld"

typedef int32_t mp_int_t; // must be pointer size
typedef uint32_t mp_uint_t; // must be pointer size
typedef void *machine_ptr_t; // must be of pointer size
typedef const void *machine_const_ptr_t; // must be of pointer size
typedef long mp_off_t;

// dummy print
#include <stdio.h>

#define MP_PLAT_PRINT_STRN(str, len) printf(str)

// extra built in names to add to the global namespace
extern const struct _mp_obj_fun_builtin_t mp_builtin_open_obj;
#define MICROPY_PORT_BUILTINS \
    { MP_OBJ_NEW_QSTR(MP_QSTR_open), (mp_obj_t)&mp_builtin_open_obj },

// We need to provide a declaration/definition of alloca()
#include <alloca.h>

extern const struct _mp_obj_module_t mp_module_citrus;
extern const struct _mp_obj_module_t mp_module_sf2d;
extern const struct _mp_obj_module_t mp_module_socket;

#define MICROPY_PORT_BUILTIN_MODULES \
    { MP_OBJ_NEW_QSTR(MP_QSTR_citrus), (mp_obj_t)&mp_module_citrus }, \
    { MP_OBJ_NEW_QSTR(MP_QSTR_sf2d),   (mp_obj_t)&mp_module_sf2d   }, \
    { MP_OBJ_NEW_QSTR(MP_QSTR_socket), (mp_obj_t)&mp_module_socket },
