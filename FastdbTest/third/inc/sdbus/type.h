
#ifndef __SDBUS_TYPE_H
#define __SDBUS_TYPE_H 1

#include "sdbus/compile.h"

namespace sdbus{

#if OSTYPE==OSTYPE_WINDOWS

    typedef __int8                  int8_t    ;
    typedef unsigned __int8         uint8_t   ;
    typedef __int16                 int16_t   ;
    typedef unsigned __int16        uint16_t  ;
    typedef __int32                 int32_t   ;
    typedef unsigned __int32        uint32_t  ;
    typedef __int64                 int64_t   ;
    typedef unsigned __int64        uint64_t  ;

#else

    #include <stdint.h>

    typedef ::int8_t                int8_t    ;
    typedef ::uint8_t               uint8_t   ;
    typedef ::int16_t               int16_t   ;
    typedef ::uint16_t              uint16_t  ;
    typedef ::int32_t               int32_t   ;
    typedef ::uint32_t              uint32_t  ;
    typedef ::int64_t               int64_t   ;
    typedef ::uint64_t              uint64_t  ;

#endif

typedef ::uintptr_t                 uintptr_t ;
typedef ::intptr_t                  intprt_t  ;

SDBUSAPI int64_t atoi64(const char * str) ;

#if (WORDSIZE == 32)
static const char * U64FMT = "%llu" ;
static const char * I64FMT = "%lld" ;
#else
static const char * U64FMT = "%lu" ;
static const char * I64FMT = "%ld" ;
#endif
}


#endif  /** __SDBUS_TYPE_H */
