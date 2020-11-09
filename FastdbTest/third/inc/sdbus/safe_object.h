
#ifndef __SDBUS_SAFE_OBJECT_H
#define __SDBUS_SAFE_OBJECT_H 1

#include "sdbus/compile.h"
#include "sdbus/type.h"

namespace sdbus{

/*
    我们需要支持引用，为了简化接口，我们假设对象支持increment和decrement。
*/
template <typename T >
class SafeRef{
public:
    SafeRef() {object_ = NULL ;}
    virtual ~SafeRef() {Detach() ;}

    SafeRef(const SafeRef& ref)
    {
        object_ = NULL ;
        Attach(ref.Get()) ;
    }

    SafeRef(const T * obj)
    {
        object_ = NULL ;
        Attach(obj) ;
    }

    SafeRef& operator=(const SafeRef& ref)
    {
        Attach(ref.Get()) ;    
        return (*this) ;
    }

    SafeRef& operator=(const T * obj)
    {
        Attach(obj) ;
        return (*this) ;
    }

    inline bool operator==(const SafeRef& ref) const {return (object_ == ref.Get()) ;}
    inline bool operator!=(const SafeRef& ref) const {return (object_ != ref.Get()) ; }

    inline bool operator==(const T * obj) const{ return (object_ == obj) ;}
    inline bool operator!=(const T * obj) const{ return (object_ != obj) ;}

    inline T * Get() const {return object_ ;}

    void Attach(const T * obj)
    {
        T * nobj = (T *)obj ;
        if(nobj != NULL)
            nobj->increment() ;

        Detach() ;
    
        object_ = nobj ;        
    }

    void Detach()
    {
        T * obj = object_ ;
        object_ = NULL ;

        if(obj != NULL)
        {
            long v = obj->decrement() ;
            if(v == 0)
                delete obj ;
        }        
    }

    inline const T * operator->() const {return object_ ;}
    inline T * operator->() {return object_ ;}
    inline bool IsValid() const {return (object_ != NULL) ;}
    inline operator bool () const {return (object_ != NULL) ;}

private:
    mutable T * object_ ;
} ;

}

#endif  /** __SDBUS_SAFE_OBJECT_H */
