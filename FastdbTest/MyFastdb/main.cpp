
#include <iostream>
#include "cache/controller/account_cachecontroller.h"
#include "include/config.h"
#include "include/logger.h"
#include <log4cxx/logger.h>
#include <log4cxx/propertyconfigurator.h>
#include "db/connection_pool_manager.h"
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

int main(int argc, char** argv)
{
	/* 日志                                                                     */
	log4cxx::PropertyConfigurator::configure("log4cxx.properties");
	setlocale(LC_ALL, "");
	LOGGER_INITIALIZATION("MyFastDBTest");

	/* 配置文件                                                                 */
	Config::singleton()->config("Cfg.ini");
	if (!Config::singleton()->load()) {
		LOGGER_ERROR("read config file failed!");
		return -1;
	}

	/* 数据库                                                                     */
	ConnectionPoolManager::getInstance();

	/* fastdb                                                                     */
	CacheCenter::sharedInstance()->Start();
	/*CacheMonitor::getInstance()->setDataBase(CacheCenter::sharedInstance()->getFastDB());
	CacheMonitor::getInstance()->setSaveFileName(CacheCenter::sharedInstance()->getLogFile());
	CacheMonitor::getInstance()->start();*/

	AccountCacheController accountCC;
	accountCC.cacheTable();
	accountCC.commit();

	AccountCacheController::cacheInMem("id");

	std::cout << "size:" << accountCC.getSizeByQueryInThreadSafty("") << std::endl;

	while (1)
	{
		boost::this_thread::sleep(boost::posix_time::seconds(5));
	}
	return 0;
}
