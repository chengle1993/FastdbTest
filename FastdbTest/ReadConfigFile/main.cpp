// ReadConfigFile.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include "logger.h"
#include "config.h"

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
	system("pause");
	return 0;

}
