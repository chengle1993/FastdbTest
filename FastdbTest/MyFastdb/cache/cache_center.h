#ifndef CACHECENTER_H_
#define CACHECENTER_H_

#include "include/cache_def.h"
#include "db/dbconnector.h"
#include "fastdb/fastdb.h"

class CacheCenter {

private:
	CacheCenter();
	CacheCenter(CacheCenter const& cacheCenter); // never implement
	virtual ~CacheCenter();

	void operator=(CacheCenter const& cacheCenter); // never implement

	static void FastDbErrHandler(int error, char const* msg, int msgarg, void* context);

public:
	static CacheCenter* sharedInstance() {
		if (_singleton == NULL) {
			_singleton = new CacheCenter();

		}
		return _singleton;
	}

	bool Init();
	void Start();
	void Open();
	void Close();
	dbDatabase* getFastDB();

	void CommitCache();
	void RollbackCache();
	void AttachCurrentThread();

	/*
	flags对应database的DetachFlags
	0-precommit 只释放锁，而不把数据更新推送至fastDB
	1-commit 释放锁，并将数据推送至fastDB，在批量更新结束时候使用
	2-destroycontext 销毁线程上下文对象，一般线程结束使用
	3-1|2
	*/
	void DetachCurrentThread(int flags = 0);

	std::string getLogFile();

	void clearCache();

	bool isInitData(int argc, char * argv[]);
	bool isFastDBOpened();

private:
	static CacheCenter* _singleton;
	bool openFastDB();
	void checkFastDBOpen();
	std::string cvtType(dbDatabase::dbAccessType accessType);

	dbDatabase _fstDb;
	dbLocalEvent timer;
	dbDatabase *pDB;
	dbDatabase::OpenParameters params;
	std::string local_host, cache_name, cache_file;
	std::string _log_file;

};

#endif /* CACHECENTER_H_ */
