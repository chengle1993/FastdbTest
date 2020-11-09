#ifndef _CACHE_MONITOR_H_
#define _CACHE_MONITOR_H_

#include <fstream> 
#include "fastdb/fastdb.h"
class CacheMonitor {
public:
	static CacheMonitor* getInstance();
	void setDataBase(dbDatabase* db);
	void setSaveFileName(const std::string& filename);
	void start();
	std::string getFastDBInfo();
	void saveFastDBInfo();
	void checkFastDB();
	bool isFastDBEnable();
private:
	CacheMonitor();
	~CacheMonitor();
	void print();
	bool isMonitorEnable();

	dbDatabase* pDB;
	std::string saveFileName;
	std::ofstream saveFile;
};
#endif // _CACHE_MONITOR_H_
