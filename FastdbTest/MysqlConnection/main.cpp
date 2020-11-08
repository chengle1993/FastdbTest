// ReadConfigFile.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include "util/logger.h"
#include "util/config.h"
#include "db/connection_pool_manager.h"
#include "db/dbconnector.h"
#include "util/common.h"

int main()
{
	//log initialization
	log4cxx::PropertyConfigurator::configure("log4cxx.properties");
	LOGGER_INITIALIZATION("ReadConfigFileTest");
	LOGGER_DEBUG("now, can use logger.");

	//load configure file
	Config::singleton()->config("Cfg.ini");
	if (!Config::singleton()->load())
	{
		LOGGER_ERROR("load config file error.");
		return -1;
	}
	LOGGER_INFO(Config::singleton()->getValue("ReloadTime"));
	std::cout << Config::singleton()->getValue("ReloadTime") << std::endl;

	//数据库初始化
	ConnectionPoolManager::getInstance();
	sql::ResultSet* resultSet = DBConnector::getInstance()->executeQuery("select * from bond limit 100;", "idb_bond");
	if (!resultSet)
	{
		std::cout << "read DB error." << std::endl;
		return -1;
	}
	while (resultSet->next())
	{
		std::string id = resultSet->getString("id");
		std::string bond_key = resultSet->getString("Bond_Key");
		std::string short_name = resultSet->getString("Short_Name");
		std::cout << "id:" << id << "    bond_key:" <<bond_key << "    short_name:" << Utf8ToAnsi(short_name) << std::endl;
	}

	system("pause");
	return 0;

}
