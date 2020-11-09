
#ifndef __SDBUS_STRING_H
#define __SDBUS_STRING_H 1

#include "sdbus/compile.h"
#include "sdbus/type.h"

#include <string>

namespace sdbus{

class SDBUSAPI string{
public:
    string() ;
    ~string() ;

    string(const string& str) ;    
    string(const char * str) ;
    inline string(const std::string& str) {init() ; assign(str.c_str() , str.length()) ;  }
    string(const char * str , size_t size);

    string& operator= (const string& str) ;
    inline string& operator= (const std::string& str) {assign(str.c_str() , str.length()) ; return(*this) ;}
    string& operator= (const char * str) ;

    inline string& operator+=(const char * str) {return append(str) ;}
    inline string& operator+=(const string& str) {return append(str.c_str()) ;}

    inline bool operator==(const string& str) const {return cmp(str.c_str() , str.length()) == 0 ;}
    inline bool operator!=(const string& str) const {return cmp(str.c_str() , str.length()) != 0 ;}
    inline bool operator<(const string& str) const {return cmp(str.c_str() , str.length()) < 0 ;}
    inline bool operator<=(const string& str) const {return cmp(str.c_str() , str.length()) > 0 ;}
    inline bool operator>(const string& str) const {return cmp(str.c_str() , str.length()) > 0 ;}
    inline bool operator>=(const string& str) const {return cmp(str.c_str() , str.length()) >= 0 ;}

    inline bool operator==(const char * str) const {return cmp(str) == 0 ;}
    inline bool operator!=(const char * str) const {return cmp(str) != 0 ;}
    inline bool operator<(const char * str) const {return cmp(str) < 0 ;}
    inline bool operator<=(const char * str) const {return cmp(str) > 0 ;}
    inline bool operator>(const char * str) const {return cmp(str) > 0 ;}
    inline bool operator>=(const char * str) const {return cmp(str) >= 0 ;}

    inline bool operator==(const std::string& str) const {return cmp(str.c_str() , str.length()) == 0 ;}
    inline bool operator!=(const std::string& str) const {return cmp(str.c_str() , str.length()) != 0 ;}
    inline bool operator<(const std::string& str) const {return cmp(str.c_str() , str.length()) < 0 ;}
    inline bool operator<=(const std::string& str) const {return cmp(str.c_str() , str.length()) > 0 ;}
    inline bool operator>(const std::string& str) const {return cmp(str.c_str() , str.length()) > 0 ;}
    inline bool operator>=(const std::string& str) const {return cmp(str.c_str() , str.length()) >= 0 ;}

    inline operator const char * () const {return (buffer_ == NULL) ? kEmptyString:buffer_ ;}


    inline size_t length() const {return length_ ;}
    inline size_t capacity() const {return capacity_ ;}
    inline const char * c_str() const {return (buffer_ == NULL) ? kEmptyString : buffer_ ;}
    inline const char * data() const {return (buffer_ == NULL) ? kEmptyString : buffer_ ;}
    inline bool empty() const {return length_ == 0 ;}


    void clear() ;

    void assign(const char * str , size_t size) ;
    int cmp(const char * str , size_t size = 0) const ;
    string& append(const char * str) ;

    void to_upper() ;
    void to_lower() ;

    /*
        将字符串的左右的空格删除。
    */
    int trim() ;

    const string upper() const ;
    const string lower() const ;

    /*
        这个功能比较超常规，将源字符串的内存转移到自己，然后重置源，请注意小心使用。
    */
    void attach(string& str) ;

    static const string EMPTY ;
    static const char * kEmptyString ;

private:
    char * buffer_ ;
    size_t length_ ;
    size_t capacity_ ;

    void init() ;
} ;

}

inline bool operator==(const std::string& src , const sdbus::string& dst)  {return dst.cmp(src.c_str() , src.length()) == 0 ;}
inline bool operator!=(const std::string& src , const sdbus::string& dst)  {return dst.cmp(src.c_str() , src.length()) != 0 ;}
inline bool operator<(const std::string& src , const sdbus::string& dst)  {return dst.cmp(src.c_str() , src.length()) > 0 ;}
inline bool operator<=(const std::string& src , const sdbus::string& dst)  {return dst.cmp(src.c_str() , src.length()) >= 0 ;}
inline bool operator>(const std::string& src , const sdbus::string& dst)  {return dst.cmp(src.c_str() , src.length()) < 0 ;}
inline bool operator>=(const std::string& src , const sdbus::string& dst)  {return dst.cmp(src.c_str() , src.length()) <= 0 ;}


inline bool operator==(const char * src , const sdbus::string& dst)  {return dst.cmp(src) == 0 ;}
inline bool operator!=(const char * src , const sdbus::string& dst)  {return dst.cmp(src ) != 0 ;}
inline bool operator<(const char * src , const sdbus::string& dst)  {return dst.cmp(src ) > 0 ;}
inline bool operator<=(const char * src , const sdbus::string& dst)  {return dst.cmp(src) >= 0 ;}
inline bool operator>(const char * src , const sdbus::string& dst)  {return dst.cmp(src)  < 0 ;}
inline bool operator>=(const char * src , const sdbus::string& dst)  {return dst.cmp(src ) <= 0 ;}

inline sdbus::string SDBUSAPI operator+(const sdbus::string& left , const char * right) {   return sdbus::string(left) += right ; }
inline sdbus::string SDBUSAPI operator+(const sdbus::string& left , const sdbus::string& right) {   return sdbus::string(left) += right ; }


#endif  /** __SDBUS_STRING_H */
