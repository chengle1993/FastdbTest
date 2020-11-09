#ifndef _CONNECTION_POOL_H_
#define _CONNECTION_POOL_H_

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <boost/thread/mutex.hpp>
#include <list>
#include <string>
#include <map>
#include "../include/logger.h"

class ConnPool {
public:
	~ConnPool();

	static ConnPool* getInstance();

    void init();
    void setSchemaHost(const std::string &schema, const std::string &host);
    void initConnPool(int size, const std::string& host, const std::string& username, const std::string& passwd);
	void destConnPool();

	sql::Connection* getConnection(const std::string& schema, bool autocommit = true);
	void releaseConnection(sql::Connection *conn);

private:
	ConnPool();
    sql::Connection* createConnection(const std::string &host);
    void destConnection(sql::Connection* conn);

	sql::mysql::MySQL_Driver* driver_;

	std::map<std::string, std::list<sql::Connection*>> conn_map_;
    std::map<std::string, sql::ConnectOptionsMap> cfg_map_;
    std::map<std::string, std::string> schema_host_;
	boost::mutex mutex_;
	static ConnPool* singleton_;

};


#endif // _CONNECTION_POOL_H_