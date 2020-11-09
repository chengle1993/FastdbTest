
#ifndef __SDBUS_ENGINE_H
#define __SDBUS_ENGINE_H 1


#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/string.h"
#include "sdbus/options.h"

#include "sdbus/callback.h"
#include "sdbus/message.h"

namespace sdbus{

/*
    在总线层面，可能会有不同的选型。但是对于应用开发者来说，需要一个稳定的接口，
    同时屏蔽后面的具体实现，对于应用开发的影响。
    对于sdbus来说，应用层API和engine是桥接模式，因为应用层不希望了解总线的具体实现。
    而对于engine本身来说是适配模式，需要完成不同总线的封装。
*/
class SDBUSAPI Engine{
public:
    Engine() ;
    Engine(const Engine& engine) {Assign(engine) ;}
    virtual ~Engine() ;
    Engine& operator=(const Engine& engine) {Assign(engine) ;return (*this) ;}

    Engine * (*Creator) ();
    void (*Destroyer)(Engine * engine) ;

    bool (*Initializer)(Engine * engine , const sdbus::string& url) ;
    void (*Finalizer)(Engine * engine) ;

    bool (*AddHandler)(Engine * engine , const sdbus::string& name , int type , Callback * cb) ;
    void (*DelHandler)(Engine * engine , const Callback * cb) ;

    bool (*AsyncSend)(Engine * engine , const sdbus::string& name , const Message& msg) ;
    bool (*SyncSend)(Engine * engine , const sdbus::string& name , const Message& request , Message& response , int timeout) ;

    bool (*Publish)(Engine * engine , const sdbus::string& topic , const Message& msg) ;    

    bool (*SetOptions)(Engine * engine , const Options& options) ;
    const Options& (*GetOptions)(Engine * engine ) ;

    void * PrivData ;
	Options PrivOptions;
private:
    void Assign(const Engine& engine) ;
} ;


SDBUSAPI bool RegisterEngine(const sdbus::string& name , const Engine& engine) ;

SDBUSAPI bool UnregisterEngine(const sdbus::string& name) ;

SDBUSAPI Engine * CreateEngine(const sdbus::string& name) ;

SDBUSAPI void DestroyEngine(Engine * engine) ;


}

#endif /** __SDBUS_ENGINE_H */
