#include "connection_pool_manager.h"
#include "../include/config.h"
#include "../include/constants.h"
#include "connection_pool.h"

ConnectionPoolManager* ConnectionPoolManager::singleton = NULL;

ConnectionPoolManager::ConnectionPoolManager()
{
	std::string host = Config::singleton()->getValue("Connection.Host");
	std::string user = Config::singleton()->getValue("Connection.User", "artogrid");
	std::string passwd = Config::singleton()->getValue("Connection.Password", "artogrid");
	int pool_size = Config::singleton()->getIntValue("Connection.PoolSize", 2);
	kDBBatchCommit = Config::singleton()->getIntValue("Connection.BatchCommit", 500);
	kSchemaAccount = Config::singleton()->getValue("Connection.AccountDB", "idb_account");
	kSchemaBond = Config::singleton()->getValue("Connection.BondDB", "idb_bond");
    if (!host.empty()) {
        ConnPool::getInstance()->setSchemaHost(kSchemaAccount, host);
        ConnPool::getInstance()->setSchemaHost(kSchemaBond, host);
        ConnPool::getInstance()->initConnPool(pool_size, host, user, passwd);
		LOGGER_DEBUG(SBF("ConnectionPoolManager host:%s, user:%s, passwd:%s, pooSize:%d, batchCommit:%d, accountSchema:%s, bondSchema:%s", 
			host % user % passwd % pool_size % kDBBatchCommit % kSchemaAccount % kSchemaBond));
    }

	std::string base_host = Config::singleton()->getValue("BaseConnection.Host");
	std::string base_user = Config::singleton()->getValue("BaseConnection.User", "artogrid");
	std::string base_passwd = Config::singleton()->getValue("BaseConnection.Password", "artogrid");
	int base_pool_size = Config::singleton()->getIntValue("BaseConnection.PoolSize", 2);
	kSchemaBase = Config::singleton()->getValue("BaseConnection.BaseDB", "base_data");
    if (!base_host.empty()) {
        ConnPool::getInstance()->setSchemaHost(kSchemaBase, base_host);
        ConnPool::getInstance()->initConnPool(base_pool_size, base_host, base_user, base_passwd);
		LOGGER_DEBUG(SBF("ConnectionPoolManager host:%s, user:%s, passwd:%s, pooSize:%d, baseSchema:%s", 
			base_host % base_user % base_passwd % base_pool_size % kSchemaBase));
    }
}


ConnectionPoolManager::~ConnectionPoolManager()
{
}

ConnectionPoolManager* ConnectionPoolManager::getInstance()
{
	if (singleton == NULL)
	{
		singleton = new ConnectionPoolManager();
	}
	return singleton;
}