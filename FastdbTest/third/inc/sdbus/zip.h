
#ifndef __SDBUS_ZIP_H
#define __SDBUS_ZIP_H 1

#include "sdbus/compile.h"

namespace sdbus{

SDBUSAPI bool zip(const char * ibuf , int ilen , char * obuf , int& olen) ;

SDBUSAPI bool unzip(const char * ibuf , int ilen , char * obuf , int& olen) ;

}


#endif  /** __SDBUS_ZIP_H */
