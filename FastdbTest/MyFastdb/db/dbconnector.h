#ifndef _DBCONNECTOR_H_
#define _DBCONNECTOR_H_

#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>
#include <cppconn/prepared_statement.h>
#include "../include/logger.h"
#include "boost/thread/mutex.hpp"
#include "boost/thread/condition_variable.hpp"
#include <queue>

class ConnPool;

typedef struct {
	std::vector<std::string> sql_vec;
	std::string sql_str;
	std::string schema;
} UpdateMsg;

class DBConnector {
public:
	static DBConnector* getInstance();

	sql::ResultSet* executeQuery(const std::string& sql_str, const std::string& schema);

	int executeUpdate(const std::string& sql_str, const std::string& schema);
	bool executeUpdate(const UpdateMsg& update_msg);
	void executeUpdate(const std::vector<std::string>& sql_vec, const std::string& schma);

	// 事务处理
	sql::Connection* beginTransaction(const std::string& schema);
	int executeUpdate(sql::Connection* conn, const std::string& sql_str);
	void commit(sql::Connection* conn);
	void rollback(sql::Connection* conn);

	// 更新线程
	void threadExeUpdate();

    // 禁止更新，slave不允许入库
    void setEnableUpdate(bool enable);

    int getCacheSize();

private:
	DBConnector();
	~DBConnector();

	bool executeBatchUpdate(sql::Connection* conn, const std::vector<std::string>& sql_vec, const std::string& schema);

	std::string subSqlLength(const std::string& sql);

	std::queue<UpdateMsg> msg_queue_;
	boost::mutex queue_mutex_;
	boost::condition_variable_any queue_cond_var_;
    bool enable_update_;

	ConnPool *conn_pool_;
	static boost::mutex singleton_mutex_;
	static DBConnector* singleton_;

};

class DBWriteUtil : public boost::noncopyable {
public:
	DBWriteUtil(const std::string &schema, const std::string &sql);
	void setInt(sql::PreparedStatement *prep, const std::string &field, int32_t value);
	void setUInt(sql::PreparedStatement *prep, const std::string &field, uint32_t value);
	void setString(sql::PreparedStatement *prep, const std::string &field, const std::string &value);
	void setDouble(sql::PreparedStatement *prep, const std::string &field, double value);
	void executeUpdate(sql::PreparedStatement *prep);

private:
	int getParameterIndex(const std::string &field);

private:
	std::string sql_;
	std::map<std::string, int> fieldIndex_;
};

#endif