#include "dbconnector.h"
#include "connection_pool.h"
#include <boost/locale/encoding.hpp>	
#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>
#include <boost/format.hpp> 
#include "../include/config.h"
#include "../include/constants.h"
#include "../include/common.h"
#include <cppconn/parameter_metadata.h>

DBConnector* DBConnector::singleton_ = 0;
boost::mutex DBConnector::singleton_mutex_;
static size_t _SQL_MAX_LENGTH = 2000;

DBConnector* DBConnector::getInstance() {
	boost::mutex::scoped_lock lock(singleton_mutex_);
	if (singleton_ == NULL) {
		singleton_ = new DBConnector();
	}

	return singleton_;
}

DBConnector::DBConnector() : enable_update_(true) {
	conn_pool_ = ConnPool::getInstance();
	int work_threads = Config::singleton()->getIntValue("Connection.WorkThreads", 1);
	LOGGER_INFO("init db work thread: " << work_threads);
	for (int i = 0; i < work_threads; i++) {
		boost::function0<void> f = boost::bind(&DBConnector::threadExeUpdate, this);
		boost::thread thrd(f);
	}
}

DBConnector::~DBConnector() {

}

sql::ResultSet* DBConnector::executeQuery(const std::string& sql_str, const std::string& schema) {
	sql::Connection *conn = conn_pool_->getConnection(schema);
	sql::Statement *stmt = NULL;
	sql::ResultSet *resSet = NULL;

	if (conn == NULL) {
		return NULL;
	}

	try {
		conn->setSchema(schema);
		stmt = conn->createStatement();
		resSet = stmt->executeQuery(sql_str);
	} catch (sql::SQLException &e) {
		LOGGER_ERROR("failed to execute query! schema: " << schema << ", sql: " << subSqlLength(sql_str));
		LOGGER_DEBUG("# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__);
		LOGGER_ERROR("# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )");
	} catch (std::exception &e) {
		LOGGER_ERROR(e.what());
	} catch (...) {
		LOGGER_ERROR("unkonwn error!!! ");
	}

	if (stmt != NULL) {
		delete stmt;
	}

	conn_pool_->releaseConnection(conn);
	return resSet;
}

int DBConnector::executeUpdate(const std::string& sql_str, const std::string& schema) {
	UpdateMsg update_msg;
	update_msg.sql_str = sql_str;
	update_msg.schema = schema;

	boost::mutex::scoped_lock lock(queue_mutex_);
	msg_queue_.push(update_msg);
	queue_cond_var_.notify_one();

	return 1;
}

void DBConnector::executeUpdate(const std::vector<std::string>& sql_vec, const std::string& schma) {
	if (schma.empty() || sql_vec.empty()) {
		return;
	}
	UpdateMsg update_msg;
	update_msg.schema = schma;
	if (sql_vec.size() == 1) {
		update_msg.sql_str = sql_vec.at(0);
	} else {
		update_msg.sql_vec = sql_vec;
	}

	boost::mutex::scoped_lock lock(queue_mutex_);
	msg_queue_.push(update_msg);
	queue_cond_var_.notify_one();
}

bool DBConnector::executeUpdate(const UpdateMsg& update_msg) {
	std::string sql_str = update_msg.sql_str;
	std::string schema = update_msg.schema;

	sql::Connection *conn = conn_pool_->getConnection(schema);
	if (conn == NULL) {
		return false;
	}

	bool result = false;
	if (!sql_str.empty()) {
		sql::Statement *stmt = NULL;
		sql::ResultSet *resSet = NULL;
		int effectedLines = -1;

		//if (sql_str.size() > _SQL_MAX_LENGTH)
		//	LOGGER_INFO("Too Long sql string, ignore for LOGGER_INFO");

		//if(sql_str.size() <= _SQL_MAX_LENGTH)
		//	LOGGER_INFO(sql_str);

		try {
			conn->setSchema(schema);
			stmt = conn->createStatement();
			effectedLines = stmt->executeUpdate(sql_str);
			result = true;
		} catch (sql::SQLException &e) {
			LOGGER_ERROR("failed to execute update! schema: " << schema << ", sql: " << subSqlLength(sql_str));
			LOGGER_DEBUG("# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__);
			LOGGER_ERROR("# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )");
		} catch (std::exception &e) {
			LOGGER_ERROR(e.what());
		} catch (...) {
			LOGGER_ERROR("unkonwn error!!! ");
		}

		if (stmt != NULL) {
			delete stmt;
		}
	}

	if (!update_msg.sql_vec.empty()) {
		conn->commit();
		result = executeBatchUpdate(conn, update_msg.sql_vec, update_msg.schema);
	}
	conn_pool_->releaseConnection(conn);
	return result;
}

bool DBConnector::executeBatchUpdate(sql::Connection* conn, const std::vector<std::string>& sql_vec, const std::string& schema) {
	if (!conn || !conn->isValid() || sql_vec.empty()) {
		return true;
	}
	sql::Statement *stmt = NULL;
	bool result = false;
	try {
		LOGGER_DEBUG("execute sql, schema[" << schema << "], sql_vecs[" << sql_vec.size() << "] start ...");
		conn->setAutoCommit(false);
		conn->setSchema(schema);
		stmt = conn->createStatement();
		int ret = 0;
		int iCnt = 0;
		for (size_t i = 0; i < sql_vec.size(); ++i) {
			const std::string& sql = sql_vec.at(i);
			ret += stmt->executeUpdate(sql);
			iCnt++;
			if (iCnt >= kDBBatchCommit || i == sql_vec.size() - 1) {
				conn->commit();
				LOGGER_DEBUG("execute [" << iCnt << "] sql success, schema[" << schema << "], effective lines[" << ret << "]");
				iCnt = 0;
				ret = 0;
			}
		}
		if (iCnt > 0) {
			conn->commit();
			LOGGER_DEBUG("execute [" << iCnt << "] sql success, schema[" << schema << "], effective lines[" << ret << "]");
		}
		stmt->close();
		result = true;
		LOGGER_DEBUG("execute sql, schema[" << schema << "], sql_vecs[" << sql_vec.size() << "] success");
	} catch (sql::SQLException &e) {
		if (conn != NULL) {
			conn->rollback();
		}
		LOGGER_ERROR("failed to execute update! schema: " << schema << ", sql_vecs count: " << sql_vec.size() << ", some sql: " << subSqlLength(sql_vec.at(0)));
		LOGGER_DEBUG("# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__);
		LOGGER_ERROR("# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )");
		result = false;
	} catch (std::exception &e) {
		if (conn != NULL) {
			conn->rollback();
		}
		LOGGER_ERROR(e.what());
		result = false;
	} catch (...) {
		LOGGER_ERROR("unkonwn error!!! ");
	}
	if (stmt != NULL) {
		delete stmt;
	}
	conn->setAutoCommit(true);
	return result;
}

sql::Connection* DBConnector::beginTransaction(const std::string& schema) {
	sql::Connection *conn = conn_pool_->getConnection(schema, false);

	return conn;
}

int DBConnector::executeUpdate(sql::Connection* conn, const std::string& sql_str) {
	sql::Statement *stmt = NULL;
	sql::ResultSet *resSet = NULL;
	int effectedLines = -1;

	if (conn == NULL) {
		return -1;
	}

	//LOGGER_INFO(sql_str);
	try {
		stmt = conn->createStatement();
		effectedLines = stmt->executeUpdate(sql_str);
	} catch (sql::SQLException &e) {
		LOGGER_ERROR("failed to execute update! sql" << subSqlLength(sql_str));
		LOGGER_DEBUG("# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__);
		LOGGER_ERROR("# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )");
	} catch (std::exception &e) {
		LOGGER_ERROR(e.what());
	} catch (...) {
		LOGGER_ERROR("unkonwn error!!! ");
	}

	if (stmt != NULL) {
		delete stmt;
	}

	return effectedLines;
}

void DBConnector::commit(sql::Connection* conn) {
	if (conn == NULL) {
		return;
	}

	conn->commit();
	conn_pool_->releaseConnection(conn);
}

void DBConnector::rollback(sql::Connection* conn) {
	if (conn == NULL) {
		return;
	}

	conn->rollback();
	conn_pool_->releaseConnection(conn);
}

void DBConnector::threadExeUpdate() {
    LOGGER_DEBUG("start thread to update db");
    int debugCount = 0;
    time_t debugStartTime = 0;

	while (true) {
		std::map<std::string, UpdateMsg> tmpMsgLst;
		{
			boost::mutex::scoped_lock lock(queue_mutex_);

			while (msg_queue_.empty()) {
                if (debugCount > 0) {
                    time_t cost = GetCurrentTimeMilliSec() - debugStartTime;
                    LOGGER_DEBUG("[active_active_debug] write database end, count: " << debugCount << ", cost: " << cost << "ms");
                    debugCount = 0;
                }
				queue_cond_var_.wait(lock);
			}

			while (!msg_queue_.empty())
			{
                if (debugCount == 0) {
                    debugStartTime = GetCurrentTimeMilliSec();
                    LOGGER_DEBUG("[active_active_debug] write database start");
                }

				UpdateMsg update_msg = msg_queue_.front();
				msg_queue_.pop();
                if (!enable_update_) {
                    continue;
                }

				std::map<std::string, UpdateMsg>::iterator itr = tmpMsgLst.find(update_msg.schema);
				if (itr == tmpMsgLst.end()) {
					tmpMsgLst[update_msg.schema] = UpdateMsg();
				}
				tmpMsgLst[update_msg.schema].schema = update_msg.schema;
				if (!update_msg.sql_str.empty()) {
					tmpMsgLst[update_msg.schema].sql_vec.push_back(update_msg.sql_str);
				}
                if (!update_msg.sql_vec.empty()) {
                    tmpMsgLst[update_msg.schema].sql_vec.insert(tmpMsgLst[update_msg.schema].sql_vec.end(),
                        update_msg.sql_vec.begin(), update_msg.sql_vec.end());
                }

                ++debugCount;
			}
		}
			
        if (enable_update_) {
			for (std::map<std::string, UpdateMsg>::const_iterator itorUpdate = tmpMsgLst.begin();
				itorUpdate != tmpMsgLst.end(); itorUpdate++) {
				executeUpdate(itorUpdate->second);
			}
        } else {
            LOGGER_WARN("DBConnector::threadExeUpdate disbale write database!!!");
        }
	}
}

void DBConnector::setEnableUpdate(bool enable)
{
    enable_update_ = enable;
}

int DBConnector::getCacheSize()
{
    boost::mutex::scoped_lock lock(queue_mutex_);
    return msg_queue_.size();
}

std::string DBConnector::subSqlLength(const std::string& sql) {
	if (sql.empty())
		return sql;
	if (sql.length() > _SQL_MAX_LENGTH) {
		return sql.substr(0, _SQL_MAX_LENGTH) + " ...";
	}
	return sql;
}

//////////////////////////////////////////////////////////////////////////
DBWriteUtil::DBWriteUtil(const std::string &schema, const std::string &sql)
{
	int first = sql.find_first_of('(');
	int second = sql.find(')', first);
	if (first > 0 && second > first) {
		std::string str = sql.substr(first + 1, second - first - 1);
		std::vector<std::string> fields = split(str, ',');
		for (std::size_t i = 0; i < fields.size(); i++) {
			fieldIndex_[fields[i]] = i + 1;
		}
	}
}

void DBWriteUtil::setInt(sql::PreparedStatement *prep, const std::string &field, int32_t value)
{
	if (prep) {
		int index = getParameterIndex(field);
		if (index > 0) {
			prep->setInt(index, value);
		}
	}
}

void DBWriteUtil::setUInt(sql::PreparedStatement *prep, const std::string &field, uint32_t value)
{
	if (prep) {
		int index = getParameterIndex(field);
		if (index > 0) {
			prep->setUInt(index, value);
		}
	}
}

void DBWriteUtil::setString(sql::PreparedStatement *prep, const std::string &field, const std::string &value)
{
	if (prep) {
		int index = getParameterIndex(field);
		if (index > 0) {
			prep->setString(index, value);
		}
	}
}

void DBWriteUtil::setDouble(sql::PreparedStatement *prep, const std::string &field, double value)
{
	if (prep) {
		int index = getParameterIndex(field);
		if (index > 0) {
			prep->setDouble(index, value);
		}
	}
}

void DBWriteUtil::executeUpdate(sql::PreparedStatement *prep)
{
	if (prep) {
		prep->executeUpdate();
		prep->clearParameters();
	}
}

int DBWriteUtil::getParameterIndex(const std::string &field)
{
	std::map<std::string, int>::iterator iter = fieldIndex_.find(field);
	if (iter != fieldIndex_.end()) {
		return iter->second;
	}
	LOGGER_ERROR(SBF("getParameterIndex error, field:%1%", field));
	return 0;
}
