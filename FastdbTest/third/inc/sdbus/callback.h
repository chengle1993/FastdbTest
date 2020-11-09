
#ifndef __SDBUS_CALLBACK_H
#define __SDBUS_CALLBACK_H 1

#include "sdbus/message.h"
#include "sdbus/dict.h" 
#include "sdbus/options.h"

namespace sdbus{

class Connection ;
class CallbackImpl ;
class SDBUSAPI Callback{
public:
    Callback() ;
    virtual ~Callback() ;

    /*
        2015-01-09
        为了解决临时连接到qpid，如果是互联网连接，会因为TCP拆连接的速度比较慢，导致qpid
        将应答发往已经失效的连接。

        需要注意的是：
        1、当type == DeliverPublish，那么durable设置无效，总是为false，非持久化的。
        2、当type == DeliverDirect时，durable默认为true，这时name必须非空。
        3、当type == DeliverDirect，同时durable == false ，那么 name无效。
           这时，系统自动为其生成UUID作为队列名。

        需要注意的是，如果队列名称为系统自动生成时，callback的名称会在断开重连时，
        发生变化。因此，在每次发消息时，必须重新取得该callback的name，作为ReplyTo。
    */
    bool Init(const sdbus::string& name , int type , bool durable = true) ;

    const sdbus::string& Name() const ;
    void Name(const char * name) ;

    int Type() const  ;
    void Type(int type) ;

    bool Durable() const ;
    void Durable(bool durable) ;

    virtual void OnMsgs(const Message ** msgs , int count) ;

    inline void SetOptions(const Options& options) {options_ = options ;}
    inline const Options& GetOptions() const {return options_ ;}

    inline const Connection * Owner() const  {return connection_ ;}
    inline Connection * Owner() {return connection_ ;}

private:

    CallbackImpl * impl_ ;

    Options options_ ;
    Connection * connection_ ;
} ;

}

#endif  /**__SDBUS_CALLBACK_H */
