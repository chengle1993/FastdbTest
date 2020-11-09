
#ifndef __SDBUS_QUEUE_H
#define __SDBUS_QUEUE_H 1

#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/string.h"

namespace sdbus{

class QueueImpl ;
class SDBUSAPI Queue {
public:
    Queue() ;
    ~Queue() ;
    
    bool Init(size_t cap) ;
    void Final() ;
    void Clear() ;

    bool Push(void * data , int timeout = 0) ;
    bool TryPush(void * data) ;

    void * Pop(int timeout = 0) ;
    void * TryPop() ;

    int Pop(void **datas , int count , int timeout = 0) ;

    size_t Size() const ;
    bool Full() const ;
    bool Empty() const ;

    const void * Get(int idx) const ;    
    bool Extend(size_t cap) ;

    size_t Head() const ;
    size_t Tail() const ;

    inline const sdbus::string& Name() const {return name_ ;}
    inline void Name(const sdbus::string& name) {name_ = name ;}

private:
    sdbus::string name_ ;
    QueueImpl * impl_ ;
} ;

}

#endif  /** __SDBUS_QUEUE_H */
