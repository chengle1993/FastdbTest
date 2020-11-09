
#ifndef __SDBUS_OPTIONS_H
#define __SDBUS_OPTIONS_H 1

#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/string.h"
#include <string>

namespace sdbus{

class OptionsImpl ;
class SDBUSAPI Options{
public:
    Options() ;
    Options(const Options& options) ;
    Options(const sdbus::string& options) ;
    ~Options() ;


    Options& operator=(const Options& options) ;
    Options& operator=(const sdbus::string& options) ;


    /*
        以key":"value";"格式，输入多个option
    */
    bool SetOptions(const sdbus::string& options) ;

    bool SetOption(const sdbus::string& name , bool val) ;
    bool SetOption(const sdbus::string& name , int32_t val) ;
    bool SetOption(const sdbus::string& name , uint32_t val) ;
    bool SetOption(const sdbus::string& name , int64_t val) ;
    bool SetOption(const sdbus::string& name , uint64_t val) ;

    bool SetOption(const sdbus::string& name , float val) ;
    bool SetOption(const sdbus::string& name , double val) ;

    bool SetOption(const sdbus::string& name , const sdbus::string& val) ;
    inline bool SetOption(const sdbus::string& name , const std::string& val) 
    {
        sdbus::string str = val ;
        return SetOption(name , str) ;
    }

    bool DelOption(const sdbus::string& name);

    bool GetOption(const sdbus::string& name , bool& val) const ;
    bool GetOption(const sdbus::string& name , int32_t& val) const ;
    bool GetOption(const sdbus::string& name , uint32_t& val) const ;
    bool GetOption(const sdbus::string& name , int64_t& val) const ;
    bool GetOption(const sdbus::string& name , uint64_t& val) const ;

    bool GetOption(const sdbus::string& name , float& val) const ;
    bool GetOption(const sdbus::string& name , double& val) const ;

    bool GetOption(const sdbus::string& name , sdbus::string& val) const ;

    inline bool GetOption(const sdbus::string& name , std::string& val) const 
    {
        sdbus::string option ;
        if(GetOption(name , option) == false)
            return false ;
        val = option.c_str() ;
        return true ;
    }

    static const Options EMPTY ;

private:
    OptionsImpl * impl_ ;

    bool Parse(const sdbus::string& options) ;
} ;

}

#endif  /** __SDBUS_OPTIONS_H */
