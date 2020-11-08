#include "connection_pool.h"
//#include "../util/constants.h"
//#include "../util/idbutil.h"
#include "../util/config.h"
//#include "../util/cache_def.h"

ConnPool* ConnPool::singleton_ = NULL;

ConnPool::ConnPool()
{
	try {
		driver_ = sql::mysql::get_mysql_driver_instance();
	}
	catch (sql::SQLException &e) {
		LOGGER_ERROR("failed to get mysql driver instance!");
		LOGGER_DEBUG("# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__);
		LOGGER_ERROR("# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )");
	}
	catch (std::exception &e) {
		LOGGER_ERROR(e.what());
	}
	catch (...) {
		LOGGER_ERROR("unkonwn error!!! ");
	}
}

ConnPool::~ConnPool() {
	destConnPool();
}

ConnPool* ConnPool::getInstance() {
	if (singleton_ == NULL) {
		singleton_ = new ConnPool();
	}
	return singleton_;
}

void ConnPool::init()
{
}

void ConnPool::setSchemaHost(const std::string &schema, const std::string &host)
{
    schema_host_[schema] = host;
}

// 初始化连接池
void ConnPool::initConnPool(int size, const std::string& host, const std::string& username, const std::string& passwd) {
	boost::mutex::scoped_lock lock(mutex_);
    std::map<std::string, std::list<sql::Connection*>>::const_iterator citer = conn_map_.find(host);
    if (citer != conn_map_.cend()) {
        LOGGER_DEBUG("initConnPool exist, host:" << host);
        return;
    }

    sql::ConnectOptionsMap connection_properties;
    connection_properties["hostName"] = host;
    connection_properties["userName"] = username;
    connection_properties["password"] = passwd;
    connection_properties["OPT_RECONNECT"] = true;
    connection_properties["CLIENT_MULTI_STATEMENTS"] = true;
    cfg_map_[host] = connection_properties;

	std::list<sql::Connection*> conn_list;
	for (int i = 0; i < size; ++i) {
        sql::Connection *conn = createConnection(host);
		if (conn) {
			conn_list.push_back(conn);
		}
	}
    conn_map_[host] = conn_list;
}

// 销毁连接池, 首先要先销毁连接池的中连接
void ConnPool::destConnPool() {
	boost::mutex::scoped_lock lock(mutex_);
	std::map<std::string, std::list<sql::Connection*>>::iterator it = conn_map_.begin();
	for (; it != conn_map_.end(); ++it)
	{
		std::list<sql::Connection*> &conn_list = it->second;
		std::list<sql::Connection*>::iterator ite = conn_list.begin();
		for (; ite != conn_list.end(); ++ite) {
			destConnection(*ite);
		}
		conn_list.clear();
	}
	conn_map_.clear();
}

static int s_count = 0;
// 创建一个连接
sql::Connection* ConnPool::createConnection(const std::string &host) {
	sql::Connection *conn = NULL;
	try {
        std::map<std::string, sql::ConnectOptionsMap>::iterator iter = cfg_map_.find(host);
        if (iter != cfg_map_.end()) {
            conn = driver_->connect(iter->second);
        } else {
            LOGGER_ERROR("createConnection ERROR, host:" << host);
        }
	}
	catch (sql::SQLException &e) {
		LOGGER_ERROR("failed to create sql connection!");
		LOGGER_DEBUG("# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__);
		LOGGER_ERROR("# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )");
	}
	catch (std::exception &e) {
		LOGGER_ERROR(e.what());
	}
	catch (...) {
		LOGGER_ERROR("unkonwn error!!! ");
	}

	if (conn) {
		++s_count;
        LOGGER_DEBUG("ConnPool::createConnection count: " << s_count << ", host: " << host);
	}
	return conn;
}

// 销毁一个连接
void ConnPool::destConnection(sql::Connection* conn) {
	if (conn) {
		try {
			conn->close();
			delete conn;
			--s_count;
			LOGGER_DEBUG("ConnPool::destConnection count: " << s_count);
		}
		catch (sql::SQLException &e) {
			LOGGER_ERROR("failed to close sql connection!");
			LOGGER_DEBUG("# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__);
			LOGGER_ERROR("# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )");
		}
		catch (std::exception &e) {
			LOGGER_ERROR(e.what());
		}
		catch (...) {
			LOGGER_ERROR("unkonwn error!!! ");
		}
	}
}

// 在连接池中获取一个连接
sql::Connection* ConnPool::getConnection(const std::string& schema, bool autocommit) {
	boost::mutex::scoped_lock lock(mutex_);

	sql::Connection *conn = NULL;
	try {
        std::string host = schema_host_[schema];
        if (host.empty()) {
            LOGGER_ERROR("host is empty, host: " << host << ", schema: " << schema);
        }
        std::list<sql::Connection*> &conn_list = conn_map_[host];
		if (conn_list.size() > 0) {    // 连接池容器中还有连接  
			conn = conn_list.front();  // 得到第一个连接
			conn_list.pop_front();     // 移除第一个连接 
			//LOGGER_DEBUG("Remaining connection count of current connection pool: " << conn_list_.size())

			if (conn->isClosed()) {     // 连接被关闭, 删除后重新建立一个
				delete conn;
				conn = createConnection(host);
			}
		}
		else {
			// 连接池不够, 创建新的连接
			conn = createConnection(host);
		}
		if (conn) {
			conn->setSchema(schema);
			conn->setAutoCommit(autocommit);
		}
	}
	catch (sql::SQLException &e) {
		LOGGER_ERROR("failed to set sql connection!");
		LOGGER_DEBUG("# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__);
		LOGGER_ERROR("# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )");
	}
	catch (std::exception &e) {
		LOGGER_ERROR(e.what());
	}
	catch (...) {
		LOGGER_ERROR("unkonwn error!!! ");
	}

	return conn;
}

// 回收数据库连接
void ConnPool::releaseConnection(sql::Connection *conn) {
	boost::mutex::scoped_lock lock(mutex_);

	if (conn) {
		try {
			if (!conn->getAutoCommit()) {
				conn->setAutoCommit(true);
			}
            
            std::string schema(conn->getSchema().c_str());
            std::string host(schema_host_[schema]);
            if (!host.empty()) {
                conn_map_[host].push_back(conn);
            } else {
                LOGGER_ERROR("host is empty, schema: " << schema);
            }
		}
		catch (sql::SQLException &e) {
			LOGGER_ERROR("failed to set sql connection!");
			LOGGER_DEBUG("# ERR: SQLException in " << __FILE__ << "(" << __FUNCTION__ << ") on line " << __LINE__);
			LOGGER_ERROR("# ERR: " << e.what() << " (MySQL error code: " << e.getErrorCode() << ", SQLState: " << e.getSQLState() << " )");
		}
		catch (std::exception &e) {
			LOGGER_ERROR(e.what());
		}
		catch (...) {
			LOGGER_ERROR("unkonwn error!!! ");
		}
	}
}