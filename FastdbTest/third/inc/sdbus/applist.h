

#ifndef __SDBUS_APPLIST_H
#define __SDBUS_APPLIST_H 1


#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/string.h"

#include <vector>

namespace sdbus{

class AppInfoImpl ;

class SDBUSAPI AppInfo{
public:
    AppInfo() ;
    ~AppInfo() ;

    AppInfo(const AppInfo& info) ;
    AppInfo& operator=(const AppInfo& info) ;

    int Addr() const ;
    void Addr(int val) ;

    const sdbus::string& Name() const ;
    void Name(const sdbus::string& name) ;

    int Type() const ;
    void Type(int val) ;

    int Group() const ;
    void Group(int val) ;

    const sdbus::string& IP() const ;
    void IP(const sdbus::string& val) ;

    int64_t STime() const ;
    void STime(int64_t val) ;

    int ServiceCount() const ;
    int Services(int index) const ;

    void AddService(int val) ;

    static const AppInfo EMPTY ;
private:
    void Assign(const AppInfo& info) ;

    AppInfoImpl * impl_ ;
} ;


class AppList : public std::vector<AppInfo> {
public:
    static const AppList EMPTY ;
} ;

class AppManImpl ;

class SDBUSAPI AppMan{
public:
    AppMan() ;
    ~AppMan() ;


    void Add(int svc , const AppList& apps) ;

    void Add(int svc , const AppInfo& app) ;

    const AppList& Find(int svc) const ;

    const AppList& Find(const sdbus::string& name) const ;

    void Clear() ;

private:
    AppManImpl *impl_ ;
} ;

}
#endif  /** __SDBUS_APPLIST_H */
