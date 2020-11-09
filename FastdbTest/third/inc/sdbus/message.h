
#ifndef __SDBUS_MESSAGE_H
#define __SDBUS_MESSAGE_H 1

#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/variant.h"
#include "sdbus/string.h"

namespace sdbus{

class SDBUSAPI Message {
public:
    typedef Variant::Map    Map ;
    typedef Variant::List   List ;

    static const int FIELD_VOID = Variant::VAR_VOID ;

    static const int FIELD_BOOL = Variant::VAR_BOOL ;

    static const int FIELD_UINT8 = Variant::VAR_UINT8 ;
    static const int FIELD_UINT16 = Variant::VAR_UINT16 ;
    static const int FIELD_UINT32 = Variant::VAR_UINT32 ;
    static const int FIELD_UINT64 = Variant::VAR_UINT64 ;

    static const int FIELD_INT8 = Variant::VAR_INT8 ;
    static const int FIELD_INT16 = Variant::VAR_INT16 ;
    static const int FIELD_INT32 = Variant::VAR_INT32 ;
    static const int FIELD_INT64 = Variant::VAR_INT64 ;

    static const int FIELD_FLOAT = Variant::VAR_FLOAT ;
    static const int FIELD_DOUBLE = Variant::VAR_DOUBLE ;

    static const int FIELD_STRING = Variant::VAR_STRING ;
    static const int FIELD_RAW = Variant::VAR_RAW ;
    static const int FIELD_MESSAGE = Variant::VAR_MAP ;
    static const int FIELD_LIST = Variant::VAR_LIST ;

	static const uint64_t DEFAULT_TTL = 3600 * 1000; //qpid消息默认超时时间

    Message() ;
    virtual ~Message() ;

    Message(const Message& msg) ;
    Message& operator=(const Message& msg) ;

    inline int FieldCount() const {return map_values_.Size() ;}
    bool GetFieldInfo(int index , uint32_t& tag , int& type) const ;
    bool GetFieldInfo(int index , sdbus::string& name , int& type) const ;
    bool GetFieldValue(int index , const Variant *& var) const ;

    inline const sdbus::string& GetType() const {return type_ ;}
    inline void SetType(const char * type) { type_ = type ;}
    inline void SetType(const std::string& type) { type_ = type ;}
    inline void SetType(const sdbus::string& type) { type_ = type ;}

    inline const sdbus::string& GetSubject() const {return subject_ ;}
    inline void SetSubject(const char * subject) {subject_ = subject;}
    inline void SetSubject(const std::string& subject) {subject_ = subject;}
    inline void SetSubject(const sdbus::string& subject) {subject_ = subject;}

    inline const sdbus::string& GetMessageID() const {return msg_id_ ;}
    inline void SetMessageID(const char * id) {msg_id_ = id;}
    inline void SetMessageID(const std::string& id) {msg_id_ = id;}
    inline void SetMessageID(const sdbus::string& id) {msg_id_ = id;}

    inline const sdbus::string& GetReplyTo() const {return reply_to_ ;}
	inline void SetReplyTo(const char * reply, bool durable = true) { sdbus::string qname = reply; SetReplyTo(qname, durable); }
	inline void SetReplyTo(const std::string& reply, bool durable = true) { sdbus::string qname = reply; SetReplyTo(qname, durable); }
	inline void SetReplyTo(const sdbus::string& reply, bool durable = true) { if (durable) reply_to_ = reply; else reply_to_ = reply + "; {create:always ,  delete:always , node:{type:queue , x-declare : {auto-delete: true} }}"; }

	/* 设置qpid消息超时时间， 毫秒 */
	inline uint64_t GetTtl() const { return ttl_; }
	inline void SetTtl(uint64_t ttl) { ttl_ = ttl; }

    /*
        TotalMessages和MessageNumber是配合使用的。主要用在一个应答包中，包含多个
        应答消息。在使用之前，请注意该两个函数的使用场景。

        total messages表示这个应答中，总共有多少个应答包。message_number_表示，这个
        消息在该应答包中处于哪个位置，从0开始。

        当message_number_ + 1 >= total messsages时，表示该应答包结束。

        如果没有设置TotalMessages，那么默认该应答包只包含一个消息。当send之后，不能再使用同样
        的replyto回复消息了。
    */
    inline uint32_t GetTotalMessages() const {return total_messages_ ;}
    inline void SetTotalMessages(uint32_t val) {total_messages_ = val;}

    inline uint32_t GetMessageNumber() const {return message_number_ ;}
    inline void SetMessageNumber(uint32_t val) {message_number_ = val ;}


    bool SetBool(uint32_t tag, bool value);

    bool SetUInt8(uint32_t tag, uint8_t value);
    bool SetUInt16(uint32_t tag, uint16_t value);
    bool SetUInt32(uint32_t tag, uint32_t value);
    bool SetUInt64(uint32_t tag, uint64_t value);
    bool SetInt8(uint32_t tag, int8_t value);
    bool SetInt16(uint32_t tag, int16_t value);
    bool SetInt32(uint32_t tag, int32_t value);
    bool SetInt64(uint32_t tag, int64_t value);

    bool SetFloat(uint32_t tag, float value);
    bool SetDouble(uint32_t tag, double value);

    bool SetString(uint32_t tag, const char * value);
    bool SetString(uint32_t tag, const sdbus::string& value);
    inline bool SetString(uint32_t tag, const std::string& value) {return SetString(tag , value.c_str()) ;}

    bool SetRaw(uint32_t tag, const char *value, size_t size);
    bool SetRaw(uint32_t tag, const sdbus::string& value);

    bool SetMap(uint32_t tag , const Map& map) ;
    bool SetList(uint32_t tag , const List& list) ;

    inline bool SetBool(const char * name, bool value){ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return SetBool(tag , value) ;}

    inline bool SetUInt8(const char * name, uint8_t value){ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return SetUInt8(tag , value) ;}
    inline bool SetUInt16(const char * name, uint16_t value){ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return SetUInt16(tag , value) ;}
    inline bool SetUInt32(const char * name, uint32_t value){ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return SetUInt32(tag , value) ;}
    inline bool SetUInt64(const char * name, uint64_t value){ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return SetUInt64(tag , value) ;}

    inline bool SetInt8(const char * name, int8_t value){ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return SetInt8(tag , value) ;}
    inline bool SetInt16(const char * name, int16_t value){ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return SetInt16(tag , value) ;}
    inline bool SetInt32(const char * name, int32_t value){ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return SetInt32(tag , value) ;}
    inline bool SetInt64(const char * name, int64_t value){ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return SetInt64(tag , value) ;}

    inline bool SetFloat(const char * name, float value){ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return SetFloat(tag , value) ;}
    inline bool SetDouble(const char * name, double value){ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return SetDouble(tag , value) ;}

    inline bool SetString(const char * name, const char * value){ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return SetString(tag , value) ;}
    inline bool SetString(const char * name, const sdbus::string& value){ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return SetString(tag , value) ;}

    inline bool SetRaw(const char * name, const char * value , size_t size){ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return SetRaw(tag , value , size) ;}
    inline bool SetRaw(const char * name, const sdbus::string& value){ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return SetRaw(tag , value) ;}

    inline bool SetMap(const char * name, const Map& map){ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return SetMap(tag , map) ;}
    inline bool SetList(const char * name, const List& list){ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return SetList(tag , list) ;}

    /*
        这三个函数专门是为了提高性能，推荐使用。Add之后，会返回内部生成的类，然后直接在这个类上面操作
    */
    VariantMap* AddMap(uint32_t tag) ;
    VariantList* AddList(uint32_t tag) ;
    Variant* AddVariant(uint32_t tag) ;

    inline VariantMap* AddMap(const char * name) {uint32_t tag = Name2Tag(name) ; if(tag == 0) return NULL ; else return AddMap(tag) ;}
    inline VariantList* AddList(const char * name) {uint32_t tag = Name2Tag(name) ; if(tag == 0) return NULL ; else return AddList(tag) ;}
    inline Variant* AddVariant(const char * name) {uint32_t tag = Name2Tag(name) ; if(tag == 0) return NULL ; else return AddVariant(tag) ;}


    bool GetBool(uint32_t tag, bool &value) const;
    bool GetUInt8(uint32_t tag, uint8_t &value) const;
    bool GetUInt16(uint32_t tag, uint16_t &value) const;
    bool GetUInt32(uint32_t tag, uint32_t &value) const;
    bool GetUInt64(uint32_t tag, uint64_t &value) const;
    bool GetInt8(uint32_t tag, int8_t &value) const;
    bool GetInt16(uint32_t tag, int16_t &value) const;
    bool GetInt32(uint32_t tag, int32_t &value) const;
    bool GetInt64(uint32_t tag, int64_t &value) const;
    bool GetFloat(uint32_t tag, float &value) const;
    bool GetDouble(uint32_t tag, double &value) const;
    bool GetString(uint32_t tag, sdbus::string& value) const;
    inline bool GetString(uint32_t tag, std::string& value) const 
    {
        const sdbus::string * val = NULL ;
        if(RefString(tag , val) == false || val == NULL)
            return false ;
        
        value = val->c_str() ;
        return true ;
    }
    bool GetRaw(uint32_t tag, sdbus::string& value) const;
    bool GetMap(uint32_t tag, Map&value) const;
    bool GetList(uint32_t tag , List& value) const;

    inline bool GetBool(const char * name, bool &value) const{ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return GetBool(tag , value) ; }
    inline bool GetUInt8(const char * name, uint8_t &value) const{ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return GetUInt8(tag , value) ; }
    inline bool GetUInt16(const char * name, uint16_t &value) const{ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return GetUInt16(tag , value) ; }
    inline bool GetUInt32(const char * name, uint32_t &value) const{ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return GetUInt32(tag , value) ; }
    inline bool GetUInt64(const char * name, uint64_t &value) const{ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return GetUInt64(tag , value) ; }
    inline bool GetInt8(const char * name, int8_t &value) const{ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return GetInt8(tag , value) ; }
    inline bool GetInt16(const char * name, int16_t &value) const{ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return GetInt16(tag , value) ; }
    inline bool GetInt32(const char * name, int32_t &value) const{ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return GetInt32(tag , value) ; }
    inline bool GetInt64(const char * name, int64_t &value) const{ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return GetInt64(tag , value) ; }
    inline bool GetFloat(const char * name, float &value) const{ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return GetFloat(tag , value) ; }
    inline bool GetDouble(const char * name, double &value) const{ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return GetDouble(tag , value) ; }
    inline bool GetString(const char * name, sdbus::string& value) const{ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return GetString(tag , value) ; }
    inline bool GetString(const char * name, std::string& value) const{ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return GetString(tag , value) ; }
    inline bool GetRaw(const char * name, sdbus::string& value) const{ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return GetRaw(tag , value) ; }
    inline bool GetMap(const char * name, Map&value) const{ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return GetMap(tag , value) ; }
    inline bool GetList(const char * name , List& value) const{ uint32_t tag = Name2Tag(name) ; if(tag == 0) return false ; else return GetList(tag , value) ; }

    /*
        内存引用模式，必须在Message析构之前，释放引用。
        推荐使用
    */
    bool RefString(uint32_t tag, const char *& value, size_t &size) const;
    bool RefString(uint32_t tag, const sdbus::string*& value) const;

    bool RefRaw(uint32_t tag, const void *& value, size_t &size) const;
    bool RefRaw(uint32_t tag, const sdbus::string* value) const;

    bool RefVar(uint32_t tag, const Variant* &value) const;
    bool RefMap(uint32_t tag, const Map* &value) const;
    bool RefList(uint32_t tag , const List*& value) const;    

    bool RefVar(uint32_t tag, Variant* &value) ;
    bool RefMap(uint32_t tag, Map* &value) ;
    bool RefList(uint32_t tag , List*& value) ;    

    inline bool RefString(const char * name, const char *& value, size_t &size) const{ uint32_t tag = Name2Tag(name); if(tag == 0) return false ;else return RefString(tag , value , size);}
    inline bool RefString(const char * name, const sdbus::string*& value) const{ uint32_t tag = Name2Tag(name); if(tag == 0) return false ;else return RefString(tag , value);}

    inline bool RefRaw(const char * name, const void *& value, size_t &size) const{ uint32_t tag = Name2Tag(name); if(tag == 0) return false ;else return RefRaw(tag , value , size);}
    inline bool RefRaw(const char * name, const sdbus::string* value) const{ uint32_t tag = Name2Tag(name); if(tag == 0) return false ;else return RefRaw(tag , value);}

    inline bool RefVar(const char * name, const Variant* &value) const{ uint32_t tag = Name2Tag(name); if(tag == 0) return false ;else return RefVar(tag , value);}
    inline bool RefMap(const char * name, const Map* &value) const{ uint32_t tag = Name2Tag(name); if(tag == 0) return false ;else return RefMap(tag , value);}
    inline bool RefList(const char * name , const List*& value) const{ uint32_t tag = Name2Tag(name); if(tag == 0) return false ;else return RefList(tag , value);}

    inline bool RefVar(const char * name, Variant* &value){ uint32_t tag = Name2Tag(name); if(tag == 0) return false ;else return RefVar(tag , value);}
    inline bool RefMap(const char * name, Map* &value){ uint32_t tag = Name2Tag(name); if(tag == 0) return false ;else return RefMap(tag , value);}
    inline bool RefList(const char * name , List*& value) { uint32_t tag = Name2Tag(name); if(tag == 0) return false ;else return RefList(tag , value);} 

    inline Map& MapValue() {return map_values_ ;}
    inline const Map& MapValue() const {return map_values_ ;}

    void Clear() ;

    void Attach(Message& msg) ;

    inline const FieldDict * Dict() const {return map_values_.Dict() ;}
    inline void Dict(const FieldDict * dict) {map_values_.Dict(dict) ;}

    inline uint32_t Name2Tag(const char * name) const {return map_values_.Name2Tag(name) ;}
    inline const char * Tag2Name(uint32_t tag) const {return map_values_.Tag2Name(tag) ;}
protected:
    sdbus::string type_ ;
    sdbus::string subject_ ;
    sdbus::string msg_id_ ;
    sdbus::string reply_to_ ;
	uint64_t ttl_; //qpid消息超时时间，毫秒

    /*
        为了解决request package缓冲，无法确定释放时机，加入了这两个成员。
        当message_number_ >= total message时，表示应答结束。
    */
    uint32_t total_messages_ ;
    uint32_t message_number_ ;

    Map map_values_ ;

    void Assign(const Message& msg) ;
} ;

}

#endif  /** __SDBUS_MESSAGE_H */
