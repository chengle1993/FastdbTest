
#ifndef __SDBUS_SESSION_H
#define __SDBUS_SESSION_H 1

#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/string.h"
#include "sdbus/message.h"
#include "sdbus/safe_object.h"
#include "sdbus/callback.h"

namespace sdbus{

class SessObjImpl ;
class SDBUSAPI SessObj {
public:
    SessObj() ;
    virtual ~SessObj() ;

    virtual uint32_t ObjID() const ;

    inline uint32_t ClientID() const {return client_id_ ;}
    inline void ClientID(uint32_t cid) {client_id_ = cid;}

    inline uint32_t ClientSequence() const {return client_sequence_ ;}
    inline void ClientSequence(uint32_t cseq) {client_sequence_ = cseq ;}

    inline uint32_t ServiceID() const {return service_id_ ;}
    inline void ServiceID(uint32_t sid) {service_id_ = sid;}

    inline uint32_t ServiceSequence() const {return service_sequence_ ;}
    inline void ServiceSequence(uint32_t sseq) {service_sequence_ = sseq ;}

    inline uint32_t SourceAddress() const {return source_address_ ;}
    inline void SourceAddress(uint32_t addr) {source_address_ = addr ;}

    inline uint32_t DestAddress() const {return dest_address_ ;}
    inline void DestAddress(uint32_t addr ) {dest_address_ = addr;}

    inline const sdbus::string& SessionID() const {return session_id_ ;}
    inline void SessionID(const sdbus::string& sid) {session_id_ = sid ;}

    inline bool AutoDelete() const {return autodelete_ ;}
    inline void AutoDelete(bool ad) {autodelete_ = ad ;}

    inline bool AutoCreate() const {return autocreate_ ;}
    inline void AutoCreate(bool ac) {autocreate_ = ac ;}

    long increment() const;
    long decrement() const;

    bool GenerateSessionStatus(SessionStatus& status) const;

private:

    uint32_t client_id_ ;
    uint32_t client_sequence_ ;
    uint32_t service_id_ ;
    uint32_t service_sequence_ ;

    uint32_t source_address_ ;
    uint32_t dest_address_ ;             //服务端所在的地址

    sdbus::string session_id_ ;

    //当会话断开时，自动删除。默认为假。如果需要创建临时会话，需要手工设置。
    bool autodelete_ ;

    //当客户端发起创建指令时，自动创建，默认为真。
    bool autocreate_ ;

    SessObjImpl * impl_ ;
} ;

typedef SafeRef<SessObj> SessObjRef ;


/*
    客户端和服务端之间的预定义FID，主要用于会话指令。
    由于前面有消息类型的限定，所以不需要当心和其他重复。
*/
static const uint32_t TAG_RETURN_CODE       =   2075   ;
static const uint32_t TAG_ERROR_MESSAGE     =   2076   ;

static const uint32_t TAG_OPTION_AUTODELETE =   999991 ;
static const uint32_t TAG_OPTION_AUTOCREATE =   999992 ;

/*
    客户端
*/
class ClientManager ;
class SDBUSAPI Client : public SessObj{
public:
    Client() ;
    virtual ~Client() ;

    virtual bool Init() ;
    virtual bool Final() ;

    virtual void OnMessage(const Message& msg) ;

    inline uint32_t ObjID() const {return ClientID() ;}

    bool Send(const Message& msg) ;

    /*
        阻塞式请求，timeout单位为秒。如果小于0，那么为无限超时。
    */
    bool Send(const Message& req , Message& rsp , int timeout = -1) ;

    /*
        绑定到服务端一个会话，以sess为关键字。
        根据服务端的会话管理器返回的参数，来初始化本地参数。

        如果addr为0，那么就让服务端自动选择。
        如果sess为空，那么将强制被设置为临时会话。
    */
    bool BindSession(uint32_t addr , const sdbus::string& sess) ;
    bool UnbindSession() ;

    inline const ClientManager * Owner() const {return owner_ ;}
    inline ClientManager * Owner() {return owner_ ;}
    inline void Owner(ClientManager * owner) {owner_ = owner ;}

private:
    ClientManager * owner_ ;
} ;

typedef SessObjRef ClientRef ;
class SessObjManagerImpl ;
class SDBUSAPI ClientManager : public Callback{
public:
    ClientManager() ;
    virtual ~ClientManager() ;

    virtual bool Init(int appid , const sdbus::string& name) ;
    virtual void Final() ;

    virtual void OnMsgs(const Message ** msgs , int count) ;

    bool Send(const Message& msg) ;

    bool Send(const Message& req , Message& rsp , int timeout = -1) ;

    int Count() const ;
    size_t List(ClientRef * refs , size_t count) const ;

    ClientRef FindByCliID(uint32_t cid) const ;
    ClientRef FindBySessID(const sdbus::string& sid) const ;

    /*
        如果Client没有设置本地的ClientID和ClientSequence，
        那么在AddClient时，会自动赋值。
    */
    bool AddClient(Client * cli) ;
    void DelClient(const ClientRef& ref) ;
    void DelClient(uint32_t cid) ;
    void DelClient(const sdbus::string& sid) ;

    void Clear() ;

    /*
        生成一个不在本地重复的ClientID
    */
    uint32_t GenerateClientID() const ;

private:
    SessObjManagerImpl * objs_ ;
} ;


/*
    生成一个非0的随机序列号
*/
SDBUSAPI uint32_t GenerateRandSequence() ;

/*
    服务端
*/
class SessionManager ;
class SDBUSAPI Session : public SessObj{
public:
    Session() ;
    virtual ~Session() ;

    virtual bool Init() ;
    virtual bool Final() ;

    virtual uint32_t ObjID() const {return ServiceID() ;}

    virtual void OnMessage(const Message& msg) ;

    bool Send(const sdbus::string& dest , const Message& msg) ;

    /*
        由于会话只有应答，不应该存在同步模式，所以不支持。
    */

    inline const SessionManager * Owner() const {return owner_ ;}
    inline SessionManager * Owner() {return owner_ ;}
    inline void Owner(SessionManager * owner) {owner_ = owner ;}

private:
    SessionManager * owner_ ;
} ;

typedef SessObjRef SessionRef ;

class SDBUSAPI SessionManager : public Callback{
public:
    SessionManager() ;
    virtual ~SessionManager() ;

    typedef Session * (*SessionCreator)(SessionManager *  owner) ;

    virtual bool Init(int appid , const sdbus::string& name , SessionCreator creator) ;
    virtual void Final() ;

    virtual void OnMsgs(const Message ** msgs , int count) ;

    bool Send(const sdbus::string& dest , const Message& msg) ;

    int Count() const ;
    size_t List(SessionRef * refs , size_t count) const ;

    SessionRef FindBySvcID(uint32_t sid) const ;
    SessionRef FindBySessID(const sdbus::string& sid) const ;

    /*
        如果Client没有设置本地的ClientID和ClientSequence，
        那么在AddClient时，会自动赋值。
    */
    bool AddSession(Session * sess) ;
    void DelSession(const SessionRef& ref) ;
    void DelSession(uint32_t sid) ;
    void DelSession(const sdbus::string& sid) ;

    void Clear() ;

    /*
        生成一个不在本地重复的ServiceID
    */
    uint32_t GenerateServiceID() const ;

private:
    SessObjManagerImpl * objs_ ;
    SessionCreator creator_ ;

    void OnSessionBind(const Message& msg) ;
    void OnSessionUnbind(const Message& msg) ;
    void OnSessionProcess(const Message& msg) ;

    void ReportError(const Message& msg , int retcode , const sdbus::string& errmsg) ;
} ;

}


#endif  /** __SDBUS_SESSION_H */

