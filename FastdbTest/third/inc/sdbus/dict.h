
#ifndef __SDBUS_DICTIONARY_H
#define __SDBUS_DICTIONARY_H 1

#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/string.h"

/*
    在最初的设计里面，只需要字段的定义，如FIX协议。但在DataBus的实现过程中，又加入了
    Command和Topic的配置。实在不知道该怎么管理他们，就统一放入这里，期望有一天最终去掉
    这两个玩意儿。
*/

namespace sdbus{

/*
    字段定义
*/
class SDBUSAPI FieldDef{
public:
    FieldDef() ;    
    virtual ~FieldDef() ;

    FieldDef(const FieldDef& def) ;
    FieldDef(int type , uint32_t tag , const sdbus::string& name) ;
    FieldDef& operator=(const FieldDef& def) ;

    bool operator==(const FieldDef& def) const;
    bool operator!=(const FieldDef& def) const;

    operator uint32_t() const ;


    int                 Type    ;
    uint32_t            Tag     ;
    sdbus::string       Name ;

    bool Validate() const ;

    static const FieldDef EMPTY ;
private:
    void Assign(const FieldDef& def) ;
    bool Equal(const FieldDef& def) const;
} ;

class FieldDictImpl ;
class SDBUSAPI FieldDict{
public:
    FieldDict() ;
    virtual ~FieldDict() ;

    bool Add(const FieldDef& def) ;
    bool Add(int type , uint32_t tag , const sdbus::string& name) ;

    const FieldDef& Find(uint32_t tag) const ;
    const FieldDef& Find(const sdbus::string& name) const ;
    const FieldDef& Find(const char * name) const  ;

    int Size() const ;
    const FieldDef& GetByIndex(int index) const ;

    bool Empty() const ;

    static int Desc2Type(const char * name) ;
    static const char * Type2Desc(int idx) ;

    static FieldDict& Singleton() ;
private:
    FieldDictImpl * impl_ ;
} ;

/*
    类型直接输入字符串，自动转化为数字。以下是类型列表：

    void 

    bool

    int8
    uint8
    int16
    uint16
    int32
    uint32
    int64
    uint64

    float
    double

    string 
    raw

    map
    list
*/

#define DEFINE_FIELD_CLASS(NAME , TYPE)											                        \
class fielddef_##NAME##_register{												                        \
public :																		                        \
	fielddef_##NAME##_register()												                        \
	{ 																			                        \
        sdbus::FieldDict::Singleton().Add(sdbus::FieldDict::Desc2Type(#TYPE) , NAME , #NAME);	        \
	}																			                        \
};																				                        \
static fielddef_##NAME##_register fielddef_##NAME##_register_class

#define DEFINE_FIELD_CLASS_NAME(TAG , TYPE , NAME)                                                      \
class fielddef_##NAME##_##TAG##_register{												                \
public :																		                        \
	fielddef_##NAME##_##TAG##_register()												                \
	{ 																			                        \
        sdbus::FieldDict::Singleton().Add(sdbus::FieldDict::Desc2Type(#TYPE) , TAG , #NAME);	        \
	}																			                        \
};																				                        \
static fielddef_##NAME##_##TAG##_register __fielddef_##NAME##_##TAG##_register_class__


} 

    



#endif  /** __SDBUS_DICTIONARY_H */

