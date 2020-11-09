

#ifndef __SDBUS_TRANSCODER_H
#define __SDBUS_TRANSCODER_H 1

#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/message.h"
#include "sdbus/string.h"


namespace google{
namespace protobuf{
class Message ;
}
}

namespace sdbus{

/*
    定义一个protobuf的消息和sdbus消息之间的转换器。
    避免增加或者变更protobuf的消息，都要重新编译sdbus。

    这个类，只是在google::protobuf::Message和sdbus::Message之间值的拷贝。
*/
class SDBUSAPI MsgTranscoder{
public:
    MsgTranscoder() ;
    virtual ~MsgTranscoder() {}

    virtual bool FromProtobuf(Message& msg , const google::protobuf::Message& protobuf) const {return false ;}

    virtual bool ToProtobuf(const Message& msg , const sdbus::string& class_name , google::protobuf::Message*& protobuf) const {return false ;}

    int AppID ;

    static const sdbus::string kFIXCMsgName ;
} ;

/*
    默认的，转码和解码FIXC.Message，不需要判断AppID。
*/
class SDBUSAPI FIXCTranscoder : public MsgTranscoder{
public:
    bool FromProtobuf(Message& msg , const google::protobuf::Message& protobuf) const ;

    bool ToProtobuf(const Message& msg , const sdbus::string& class_name , google::protobuf::Message*& protobuf) const ;

    static const FIXCTranscoder& Singleton() ;
} ;


SDBUSAPI bool RegisterMsgTranscoder(int appid , MsgTranscoder * transcoder) ;

SDBUSAPI const MsgTranscoder * FindMsgTranscoder(int appid) ;

} 

#endif  /** __SDBUS_TRANSCODER_H */


