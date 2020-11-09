
#ifndef __SDBUS_CODEC_H
#define __SDBUS_CODEC_H 1


#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/message.h"
#include "sdbus/variant.h"


namespace sdbus{


/*
    内存格式为Tag + Type + Value

    对于List和Map来说， Value = Size + Bytes ;

    在List内部，每个元素为Type + Value。如果Type为基础类型，那么Value是确定的。
    如果Type为复合类型，那么Value又是一个Size + Bytes ；

    对于Map内部，每个成员都包含Tag + Type + Value
*/

SDBUSAPI bool VariantMapCalcSize(const VariantMap& map , size_t& size) ;

SDBUSAPI bool VariantMapEncode(const VariantMap& map , char * buffer , size_t buflen , size_t& used) ;

SDBUSAPI bool VariantMapDecode(VariantMap& map , const char * buffer , size_t buflen , size_t& used) ;


SDBUSAPI bool VariantListCalcSize(const VariantList& list , size_t& size) ;

SDBUSAPI bool VariantListEncode(const VariantList& list , char * buffer , size_t buflen , size_t& used) ;

SDBUSAPI bool VariantListDecode(VariantList& list , const char * buffer , size_t buflen , size_t& used) ;


SDBUSAPI bool VariantRawCalcSize(const sdbus::string& raw , size_t& size) ;

SDBUSAPI bool VariantRawEncode(const sdbus::string& raw , char * buffer , size_t buflen , size_t& used) ;

SDBUSAPI bool VariantRawDecode(sdbus::string& raw , const char * buffer , size_t buflen , size_t& used) ;

}

#endif  /** __SDBUS_CODEC_H */
