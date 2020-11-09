
#ifndef __SDBUS_CONNECTION_H
#define __SDBUS_CONNECTION_H 1

#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/message.h"
#include "sdbus/callback.h"
#include "sdbus/engine.h"

namespace sdbus{

static const int DeliverDirect  = 0 ;       //等价于 Request   / Response
static const int DeliverPublish = 1 ;       //等价于 Subscribe / Publish

class SDBUSAPI Connection{
public:
    Connection() ;
    ~Connection() ;

    /*
        而url的含义是由engine指定的传输引擎来解析的，目前支持qpid引擎。
    */
    bool Connect(const sdbus::string& url , const sdbus::string& engine = "qpid") ;
    void Close() ;

    bool AddHandler(Callback * cb) ;
    bool AddHandler(const sdbus::string& name , int type , Callback * cb) ;
    void DelHandler(const Callback * cb) ;

    bool Send(const sdbus::string& name , const Message& msg) ;
    
    /*
        2014-11-05
        从性能上看，我并不想支持同步模式，但是从业务角度来看，同步模式有其适用的场景。
        由于请求通道和应答通道不是同一条通道，因此在发送同步请求之前，必须保证已经注册了
        一个Callback，而且要保证这个Callback的队列名没有被其他程序复用，具有全局唯一性。
        在request的ReplyTo属性设置这个Callback队列名，那么服务端就可以通过这个队列返回
        应答的结果。
        request的MessagID在底层引擎中，会被替换为其他唯一的ID，在response回来时，应用层的
        MessageID被回填。
        如果超时过后，即使应答返回，这个应答消息也会被抛弃。如果应答消息超过多个，那么也只会
        被返回一个，其他被抛弃。
    */
    bool Send(const sdbus::string& name , const Message& request , Message& response , int timeout = -1) ;

    bool Publish(const sdbus::string& topic , const Message& msg) ;

    bool SetOptions(const Options& options) ;
    const Options& GetOptions() const;

    static void LogLevel(int level) ;
    static int LogLevel() ;
    
private:
	Options options_;
    Engine * engine_ ;
} ;

}

#endif  /** __SDBUS_CONNECTION_H */
