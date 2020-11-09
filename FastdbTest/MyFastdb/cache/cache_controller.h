#ifndef CACHECONTROLLER_H_
#define CACHECONTROLLER_H_

#include "cache_center.h"
#include "cache_monitor.h"
#include "sdbus/connection.h"
#include "include/pugixml.hpp"
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <unordered_map>
#include <boost/format.hpp> 
#include <boost/date_time.hpp>  
using namespace boost::gregorian;
using namespace boost::posix_time;

typedef boost::mutex                   Uni_Mutex;
typedef boost::shared_mutex            WR_Mutex;
typedef boost::unique_lock<WR_Mutex>   writeLock;
typedef boost::shared_lock<WR_Mutex>   readLock;

template <class T>
class CacheController {
public:
	typedef std::shared_ptr<T> T_Ptr;
	typedef std::shared_ptr<std::vector<T_Ptr> > T_Vec_Ptr;

	CacheController() {};
	virtual ~CacheController() {};

	std::string sqlFetchCache() { return _sqlFetchCache(T::SQL_TAG); }
	static std::string getFastdbTableName() { return T::dbDescriptor.getName(); }


	// 自动提交 默认打开， dbSync默认打开
	void commit(bool autoCommit = true, bool dbSync = true) {
		if (autoCommit) {
			CacheCenter::sharedInstance()->CommitCache();
		}
	};
	void rollback(bool autoCommit) {
		if (autoCommit) {
			CacheCenter::sharedInstance()->RollbackCache();
		}
	};

	virtual void cacheTable() { _cacheTable(sqlFetchCache()); }
	void clearCache() { _clearCache(); }

	static void cacheInMem(const std::string& key);
	static T_Ptr getCacheInMem(const std::string &key, const std::string &value);
	static void clearMemCache();

	int getSizeByQuery(dbQuery& q) { return _getSizeByQuery(q); }
	int getSizeByQueryInThreadSafty(dbQuery& q) { return _getSizeByQueryInThreadSafty(q); }
    int getSizeByQueryInThreadSafty(const std::string& cond) {  
        dbQuery query(cond.c_str());
        return _getSizeByQueryInThreadSafty(query); 
    }

    std::string getLastModifyTime()
    {
        dbQuery q;
        q = "order by modifytime desc limit 1";
        T_Ptr p = getCacheByQueryInThreadSafty(q);
        if (p) {
            return p->modifytime;
        }
        return "";
    }

	virtual void insertCache(sql::ResultSet* resSet) {
		if (!CacheMonitor::getInstance()->isFastDBEnable()) {
			LOGGER_ERROR(getFastdbTableName() << " - fastdb is not open!!!");
			return;
		}
		T cache;
		if (!insertCacheG(&cache, resSet))
			LOGGER_ERROR(getFastdbTableName() << " - Get fields from sql failed!");
		try {
			CacheCenter::sharedInstance()->AttachCurrentThread();
			CacheCenter::sharedInstance()->getFastDB()->insert(cache);
			CacheCenter::sharedInstance()->DetachCurrentThread();
		} catch (dbException e) {
			LOGGER_ERROR(getFastdbTableName() << " - " << e.getMsg());
		}
	}
	bool insertCache(T* cache, bool autoCommit = true) { return _insertCache(cache, true, autoCommit); }
	bool insertCacheInThreadSafty(T* cache, bool autoCommit = true) { return _insertCacheInThreadSafty(cache, true, autoCommit); }
	bool insertCacheWithoutDBSync(T* cache, bool autoCommit = true) { return _insertCache(cache, false, autoCommit); }
	bool insertCacheInThreadSaftyWithoutDBSync(T* cache, bool autoCommit = true) { return _insertCacheInThreadSafty(cache, false, autoCommit); }

	bool deleteCacheByQuery(dbQuery& q, bool autoCommit = true) { return _deleteCacheByQuery(q, true, autoCommit); }
	bool deleteCacheByQueryWithoutDBSync(dbQuery& q, bool autoCommit = true) { return _deleteCacheByQuery(q, false, autoCommit); }
	bool deleteCacheByQueryInThreadSafty(dbQuery& q, bool autoCommit = true) { return _deleteCacheByQueryInthreadSafty(q, true, autoCommit); }
	bool deleteCacheByQueryInThreadSaftyWithoutDBSync(dbQuery& q, bool autoCommit = true) { return _deleteCacheByQueryInthreadSafty(q, false, autoCommit); }

	std::map<std::string, T*> getCacheHashByQuery(dbQuery& q);
	T_Ptr getCacheByQuery(dbQuery& q);
	T_Ptr getCacheByQueryInThreadSafty(dbQuery& q);
	T_Ptr getCacheByQueryInThreadSafty(const std::string& sql) {
		dbQuery q(sql.c_str());
		return getCacheByQueryInThreadSafty(q);
	};
	T_Vec_Ptr getCacheListByQuery(dbQuery& q);
	T_Vec_Ptr getCacheListByQueryInThreadSafty(const std::list<std::string>& key_list, const std::string& key, const std::string& sql_prefix = "");
	T_Vec_Ptr getCacheListByQueryInThreadSafty(dbQuery& q);
	T_Vec_Ptr getCacheListByQueryInThreadSafty(const std::string& sql) {
		dbQuery q(sql.c_str());
		return getCacheListByQueryInThreadSafty(q);
	}
	void getCacheMapByQueryInThreadSafty(const std::list<std::string>& key_list, const std::string& key, std::unordered_map<std::string, T_Ptr>& result, const std::string& sql_prefix = "");

	void traverseCacheByQueryInThreadSafty(dbQuery& q, void(*callBack)(T*, void* param) = NULL, void* param = NULL) {
		_traverseCacheByQueryInThreadSafty(q, callBack, param);
	}
	void traverseCacheByQuery(dbQuery& q, void(*callBack)(T*, void* param) = NULL, void* param = NULL) {
		\
			_traverseCacheByQuery(q, callBack, param);
	}

	bool updateCacheInThreadSafty(dbQuery& q, T* new_cache, bool autoCommit = true) { return _updateCacheInThreadSafty(q, new_cache, true, autoCommit); }
	bool updateCacheInThreadSaftyWithoutDBSync(dbQuery& q, T* new_cache, bool autoCommit = true) { return _updateCacheInThreadSafty(q, new_cache, false, autoCommit); }

	void updateCacheByQuery(dbQuery& q,
		void(*callBack)(T*, void* param),
		void* param = NULL,
		void(*updateSuccessed)(T*, void* param) = NULL,
		void(*updateFailed)(T*, void* param) = NULL,
		void* oparam = NULL,
		bool autoCommit = true
		) {
		_updateCacheByQuery(q, callBack, param, updateSuccessed, updateFailed, oparam, true, autoCommit);
	}
	void updateCacheByQueryWithoutDBSync(dbQuery& q,
		void(*callBack)(T*, void* param),
		void* param = NULL,
		void(*updateSuccessed)(T*, void* param) = NULL,
		void(*updateFailed)(T*, void* param) = NULL,
		void* oparam = NULL,
		bool autoCommit = true
		) {
		_updateCacheByQuery(q, callBack, param, updateSuccessed, updateFailed, oparam, false, autoCommit);
	}
	void updateCacheByQueryInThreadSafty(dbQuery& q,
		void(*callBack)(T*, void* param),
		void* param = NULL,
		void(*updateSuccessed)(T*, void* param) = NULL,
		void(*updateFailed)(T*, void* param) = NULL,
		void* oparam = NULL,
		bool autoCommit = true
		) {
		_updateCacheByQueryInThreadSafty(q, callBack, param, updateSuccessed, updateFailed, oparam, true, autoCommit);
	}
	void updateCacheByQueryInThreadSaftyWithoutDBSync(dbQuery& q,
		void(*callBack)(T*, void* param),
		void* param = NULL,
		void(*updateSuccessed)(T*, void* param) = NULL,
		void(*updateFailed)(T*, void* param) = NULL,
		void* oparam = NULL,
		bool autoCommit = true
		) {
		_updateCacheByQueryInThreadSafty(q, callBack, param, updateSuccessed, updateFailed, oparam, false, autoCommit);
	}

protected:
	void _cacheTable(const std::string& fetchSQL);
	void _clearCache();

	int _getSizeByQuery(dbQuery& q);
	int _getSizeByQueryInThreadSafty(dbQuery& q);

	std::string _sqlFetchCache(const std::string& sqlTag) {
		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_file("sqlquery.xml");

		if (result.status != pugi::status_ok) {
			LOGGER_DEBUG(getFastdbTableName() << " - load result: " << result.description());
			return "";
		}
		std::string fetchSQL = doc.child("SQL").child(sqlTag.c_str()).child_value();
		trimSql(fetchSQL);
		return fetchSQL;
	}

	virtual int insertDiskDB(T* cache, sql::Savepoint* &savepoint) { return insertDatabase(T::TABLE_NAME, T::SCHEMA_NAME, cache); }
	virtual int updateDiskDB(T* cacheAfterUpdate, T* cacheBeforeUpdate) { return updateDatabase(T::TABLE_NAME, T::SCHEMA_NAME, cacheAfterUpdate, cacheBeforeUpdate, T::PKEY_NAME); }
	virtual std::string getUpdateDiskDBSQL(T* cacheAfterUpdate, T* cacheBeforeUpdate) { return getUpdateDBSQL(T::TABLE_NAME, T::SCHEMA_NAME, cacheAfterUpdate, cacheBeforeUpdate, T::PKEY_NAME); };
	virtual int deleteDiskDB(const std::vector<std::string>& primaryKeyList, sql::Savepoint* &savepoint) {
		std::vector<std::string> fieldList;
		std::vector<std::string> conditionList;
		std::string sqlStr = "delete from " + T::TABLE_NAME + " ";
		sqlStr += " Where " + T::PKEY_NAME + " in ";
		std::string conditionString = join(primaryKeyList, ",");
		sqlStr += "(" + conditionString + ")";
		return DBConnector::getInstance()->executeUpdate(sqlStr, T::SCHEMA_NAME);
	}

	bool _insertCacheInThreadSafty(T* cache, bool dbSync = true, bool autoCommit = true);
	bool _insertCache(T* cache, bool dbSync = true, bool autoCommit = true);

	bool _updateCacheInThreadSafty(dbQuery& q, T* update_cache, bool dbSync = true, bool autoCommit = true);
	bool _updateCache(dbQuery& q, T* update_cache, bool dbSync = true, bool autoCommit = true);

	void _updateCacheByQueryInThreadSafty(dbQuery& q,
		void(*callBack)(T*, void* param) = NULL,
		void* param = NULL,
		void(*updateSuccessed)(T*, void* param) = NULL,
		void(*updateFailed)(T*, void* param) = NULL,
		void* oparam = NULL,
		bool dbSync = true,
		bool autoCommit = true);
	void _updateCacheByQuery(dbQuery& q,
		void(*callBack)(T*, void* param) = NULL,
		void* param = NULL,
		void(*updateSuccessed)(T*, void* param) = NULL,
		void(*updateFailed)(T*, void* param) = NULL,
		void* oparam = NULL,
		bool dbSync = true,
		bool autoCommit = true);

	bool _deleteCacheByQueryInthreadSafty(dbQuery& q, bool dbSync = true, bool autoCommit = true);

	bool _deleteCacheByQuery(dbQuery& q, bool dbSync = true, bool autoCommit = true);

	void _traverseCacheByQueryInThreadSafty(dbQuery& q, void(*callBack)(T*, void* param) = NULL, void* param = NULL, bool mutexOn = true);
	void _traverseCacheByQuery(dbQuery& q, void(*callBack)(T*, void* param) = NULL, void* param = NULL);

private:
	static WR_Mutex wr_mutex;
	static std::unordered_map<std::string, T_Ptr> mem_cache;
};

template <class T>
WR_Mutex CacheController<T>::wr_mutex;

template <class T>
std::unordered_map<std::string, typename CacheController<T>::T_Ptr> CacheController<T>::mem_cache;

template <class T>
void CacheController<T>::clearMemCache() {
	if (!useCacheInMem && mem_cache.size() > 0) {
		mem_cache.clear();
	}
}

template <class T>
void CacheController<T>::cacheInMem(const std::string& key) {
	if (!CacheMonitor::getInstance()->isFastDBEnable()) {
		LOGGER_ERROR(getFastdbTableName() << " - fastdb is not open!!!");
		return;
	}
	ptime bt1(microsec_clock::local_time());

	dbFieldDescriptor* fd = T::dbDescriptor.getFirstField();
	if (!fd)
		return;
	LOGGER_INFO(T::dbDescriptor.getName() << " - init mem cache<" << key << "> ...");
	dbFieldDescriptor* fdFirst = fd;
	if (fd->name != key) {
		fd = fd->next;
		while (fd->name != key && fd != fdFirst)
			fd = fd->next;

		if (fd == fdFirst)
			return;
	}
	long long offs = fd->appOffs;

	mem_cache.clear();

	readLock lock(wr_mutex);
	CacheCenter::sharedInstance()->AttachCurrentThread();

	dbQuery q("");
	dbCursor<T> cursor(CacheCenter::sharedInstance()->getFastDB(), dbCursorViewOnly);
	int resultSize = cursor.select(q);
	if (resultSize > 0) {
		do {
			T* cache = new T();
			*cache = *cursor.get();
			T_Ptr cachePtr(cache);
			long long dstOffs = (long long)cache + offs;
			mem_cache[*(std::string*)dstOffs] = cachePtr;
		} while (cursor.next());
	}

	CacheCenter::sharedInstance()->DetachCurrentThread();

	ptime bt2(microsec_clock::local_time());
	LOGGER_INFO(T::dbDescriptor.getName() << " - init mem cache<" << key << "> success. size: " << mem_cache.size() << ", cost: " << (bt2.time_of_day() - bt1.time_of_day()).total_milliseconds() << "ms");
}

template <class T>
typename CacheController<T>::T_Ptr
CacheController<T>::getCacheInMem(const std::string &key, const std::string &value) {
	T_Ptr tp;
	if (useCacheInMem) {
		if (!value.empty() && !mem_cache.empty()) {
			typename std::unordered_map<std::string, typename CacheController<T>::T_Ptr>::iterator iter = mem_cache.find(value);
			if (iter != mem_cache.end()) {
                tp = iter->second;
			}
			//tp = mem_cache[value];
		}
	} else {
		CacheController<T> cc;
		std::string q_str(key + std::string("='") + value + "'");
		dbQuery q(q_str.c_str());
		tp = cc.getCacheByQueryInThreadSafty(q);
	}
	//if (!tp)
	//LOGGER_INFO("No data in fastdb with key:[" << key << "]");

	return tp;
}

template <class T>
void CacheController<T>::_cacheTable(const std::string& fetchSQL) {
	if (fetchSQL.length() == 0) {
		return;
	}
	if (!CacheMonitor::getInstance()->isFastDBEnable()) {
		LOGGER_ERROR(getFastdbTableName() << " - fastdb is not open!!!");
		return;
	}
	LOGGER_INFO(getFastdbTableName() << " - cache ...");
	sql::ResultSet* resSet = DBConnector::getInstance()->executeQuery(fetchSQL, T::SCHEMA_NAME);
	if (resSet == NULL) {
		return;
	}
	LOGGER_INFO(getFastdbTableName() << " - cache resultSet: " << resSet->rowsCount());
	try {
		_clearCache();
		LOGGER_INFO(getFastdbTableName() << " - cache clear success.");
		while (resSet->next()) {
			//CacheCenter::sharedInstance()->AttachCurrentThread();
			insertCache(resSet);
			//CacheCenter::sharedInstance()->DetachCurrentThread();
		}
		LOGGER_INFO(getFastdbTableName() << " - cache insert success.");
		delete resSet;
		LOGGER_INFO(getFastdbTableName() << " - cache success.");

	} catch (dbException e) {
		LOGGER_ERROR(e.getMsg());
	}
}

template <class T>
void CacheController<T>::_clearCache() {
	if (!CacheMonitor::getInstance()->isFastDBEnable()) {
		LOGGER_ERROR(getFastdbTableName() << " - fastdb is not open!!!");
		return;
	}
	try {
		writeLock lock(wr_mutex);
		CacheCenter::sharedInstance()->AttachCurrentThread();

		dbCursor<T> cursor(CacheCenter::sharedInstance()->getFastDB(), dbCursorForUpdate);
		cursor.removeAll();

		CacheCenter::sharedInstance()->DetachCurrentThread();
		commit();
	} catch (dbException e) {
		LOGGER_ERROR(getFastdbTableName << " - " << e.getMsg());
	}
}

template <class T>
bool CacheController<T>::_insertCache(T* cache, bool dbSync, bool autoCommit) {
	bool ret = false;
	if (!CacheMonitor::getInstance()->isFastDBEnable()) {
		LOGGER_ERROR(getFastdbTableName() << " - fastdb is not open!!!");
		return ret;
	}
	try {
		CacheCenter::sharedInstance()->getFastDB()->insert(*cache);
		// The insertion need to sync with DB
		if (dbSync) {
			sql::Savepoint* savepoint = NULL;
			insertDiskDB(cache, savepoint);
		}
		commit(autoCommit);
		ret = true;
	} catch (dbException e) {
		LOGGER_ERROR(getFastdbTableName() << " - " << e.getMsg());
	}

	return ret;
}

template <class T>
bool CacheController<T>::_insertCacheInThreadSafty(T* cache, bool dbSync, bool autoCommit) {
	writeLock lock(wr_mutex);
	bool ret = false;
	try {
		CacheCenter::sharedInstance()->AttachCurrentThread();
		ret = _insertCache(cache, dbSync, autoCommit);
		CacheCenter::sharedInstance()->DetachCurrentThread();
	} catch (dbException e) {
		LOGGER_ERROR(getFastdbTableName() << " - " << e.getMsg());
	}
	return ret;
}

template <class T>
bool CacheController<T>::_updateCache(dbQuery& q, T* update_cache, bool dbSync /* = true */, bool autoCommit /* = true */) {
	if (!CacheMonitor::getInstance()->isFastDBEnable()) {
		LOGGER_ERROR(getFastdbTableName() << " - fastdb is not open!!!");
		return false;
	}
	try {
		dbCursor<T> cursor(CacheCenter::sharedInstance()->getFastDB(), dbCursorForUpdate);

		int result_num = cursor.select(q);

		if (result_num < 0) {
			return false;
		}

		T* cache = cursor.get();
		if (cache) {
			T old_cache = *cache;
			*cache = *update_cache;

			cursor.update();

			// We need to sync the change to mysql database
			if (dbSync) {
				updateDiskDB(cache, &old_cache);
			}
			commit(autoCommit);
		}
	} catch (dbException e) {
		LOGGER_ERROR(getFastdbTableName() << " - " << e.getMsg());
		return false;
	}
	return true;
}

template <class T>
bool CacheController<T>::_updateCacheInThreadSafty(dbQuery& q, T* update_cache, bool dbSync /* = true */, bool autoCommit /* = true */) {

	writeLock lock(wr_mutex);
	bool bSuccess = false;
	try {
		CacheCenter::sharedInstance()->AttachCurrentThread();
		bSuccess = _updateCache(q, update_cache, dbSync, autoCommit);
		CacheCenter::sharedInstance()->DetachCurrentThread();
	} catch (dbException e) {
		LOGGER_ERROR(getFastdbTableName() << " - " << e.getMsg());
		return false;
	}
	return bSuccess;
}

template <class T>
void CacheController<T>::_updateCacheByQueryInThreadSafty(dbQuery& q,
	void(*callBack)(T*, void* param),
	void* param,
	void(*updateSuccessed)(T*, void* param),
	void(*updateFailed)(T*, void* param),
	void* oparam,
	bool dbSync,
	bool autoCommit) {

	writeLock lock(wr_mutex);
	try {
		CacheCenter::sharedInstance()->AttachCurrentThread();
		_updateCacheByQuery(q, callBack, param, updateSuccessed, updateFailed, oparam, dbSync, autoCommit);
		CacheCenter::sharedInstance()->DetachCurrentThread();
	} catch (dbException e) {
		LOGGER_ERROR(getFastdbTableName() << " - " << e.getMsg());
	}
}

template <class T>
void CacheController<T>::_updateCacheByQuery(dbQuery& q,
	void(*callBack)(T*, void* param),
	void* param,
	void(*updateSuccessed)(T*, void* param),
	void(*updateFailed)(T*, void* param),
	void* oparam,
	bool dbSync,
	bool autoCommit
	) {
	if (oparam == NULL) {
		oparam = param;
	}
	if (!CacheMonitor::getInstance()->isFastDBEnable()) {
		LOGGER_ERROR(getFastdbTableName() << " - fastdb is not open!!!");
		return;
	}
	std::vector<std::string> sql_vec;
	std::string schema_name;
	T* tempCache = NULL;
	try {
		dbCursor<T> cursor(CacheCenter::sharedInstance()->getFastDB(), dbCursorForUpdate);

		int resultLine = cursor.select(q);
		//cout << "Need to update " << resultLine << " lines in Cache - " << T::TABLE_NAME<< endl;
		if (resultLine < 1) {
			//LOGGER_WARN("no record to update!!! cache name - " << T::TABLE_NAME);
			return;
		}
		do {
			T* cache = cursor.get();
			tempCache = cache;
			if (cache && callBack) {
				T oldCache = *cache;
				callBack(cache, param);

				cursor.update();
				// We need to sync the change to mysql database
				if (dbSync) {
					//updateDiskDB(cache, &oldCache);	
					schema_name = T::SCHEMA_NAME;
					const std::string& sql = getUpdateDiskDBSQL(cache, &oldCache);
					if (!sql.empty()) {
						sql_vec.push_back(sql);
					}
				}

				if (updateSuccessed) {
					updateSuccessed(cache, oparam);
				}
			}
		} while (cursor.next());

		commit(autoCommit);
		if (dbSync && sql_vec.size() > 0) {
			DBConnector::getInstance()->executeUpdate(sql_vec, schema_name);
		}
	} catch (dbException e) {
		LOGGER_ERROR(getFastdbTableName() << " - " << e.getMsg());
		if (updateFailed && tempCache) {
			updateFailed(tempCache, oparam);
		}
		rollback(autoCommit);
	}
}

template <class T>
bool CacheController<T>::_deleteCacheByQueryInthreadSafty(dbQuery& q, bool dbSync, bool autoCommit) {
	writeLock lock(wr_mutex);
	bool retResult = false;
	try {
		CacheCenter::sharedInstance()->AttachCurrentThread();
		retResult = _deleteCacheByQuery(q, dbSync, autoCommit);
		CacheCenter::sharedInstance()->DetachCurrentThread();
	} catch (dbException e) {
		LOGGER_ERROR(getFastdbTableName() << " - " << e.getMsg());
	}
	return retResult;
}


template <class T>
bool CacheController<T>::_deleteCacheByQuery(dbQuery& q, bool dbSync, bool autoCommit) {
	bool retResult = false;
	if (!CacheMonitor::getInstance()->isFastDBEnable()) {
		LOGGER_ERROR(getFastdbTableName() << " - fastdb is not open!!!");
		return retResult;
	}
	T* tempCache = NULL;
	try {
		dbCursor<T> cursor(CacheCenter::sharedInstance()->getFastDB(), dbCursorForUpdate);

		int resultLine = cursor.select(q);

		std::vector<std::string> primaryKeyList;
		for (int i = 0; i < resultLine; ++i) {
			T* cache = cursor.get();

			if (cache) {
				primaryKeyList.push_back("'" + cache->getPrimaryKey() + "'");
				cursor.remove();
			}
		}
		//cursor.removeAllSelected();
		if (primaryKeyList.size() > 0) {
			if (dbSync) {
				sql::Savepoint* savepoint;

				int effectedLines = deleteDiskDB(primaryKeyList, savepoint);
				if (effectedLines > 0) {
					//assert(effectedLines == primaryKeyList.size()); //此处effectedLines的值永远为1，和primaryKeyList.size()不一定相等
					commit(autoCommit);
					retResult = true;
				} else {
					rollback(autoCommit);
				}

			} else {
				commit(autoCommit, false);
				retResult = true;
			}

		} else {
			LOGGER_DEBUG(getFastdbTableName() << " - " << "delete result: " << resultLine << " lines in Cache - " << T::TABLE_NAME);
			retResult = true;
		}
	} catch (dbException e) {
		LOGGER_ERROR(getFastdbTableName() << " - " << e.getMsg());
	}
	return retResult;
}

template <class T>
void CacheController<T>::_traverseCacheByQueryInThreadSafty(dbQuery& q, void(*callBack)(T*, void* param), void* param, bool mutexOn) {

	readLock lock(wr_mutex);
	try {
		CacheCenter::sharedInstance()->AttachCurrentThread();
		_traverseCacheByQuery(q, callBack, param);
		CacheCenter::sharedInstance()->DetachCurrentThread();
	} catch (dbException e) {
		LOGGER_ERROR(getFastdbTableName() << " - " << e.getMsg());
	}

}

template <class T>
void CacheController<T>::_traverseCacheByQuery(dbQuery& q, void(*callBack)(T*, void* param), void* param) {
	if (!CacheMonitor::getInstance()->isFastDBEnable()) {
		LOGGER_ERROR(getFastdbTableName() << " - fastdb is not open!!!");
		return;
	}

	try {
		dbCursor<T> cursor(CacheCenter::sharedInstance()->getFastDB(), dbCursorViewOnly);

		int resultLine = cursor.select(q);
		LOGGER_DEBUG(getFastdbTableName() << " - Get Cache ... - Results Lines: " << resultLine);

		do {
			T* cache = cursor.get();
			if (cache && callBack) {
				callBack(cache, param);
			}
		} while (cursor.next());
	} catch (dbException e) {
		LOGGER_ERROR(getFastdbTableName() << " - " << e.getMsg());
	}
}

template<class T>
typename CacheController<T>::T_Ptr CacheController<T>::getCacheByQuery(dbQuery& q) {
	if (!CacheMonitor::getInstance()->isFastDBEnable()) {
		LOGGER_ERROR(getFastdbTableName() << " - fastdb is not open!!!");
		return T_Ptr(new T());
	}
	T* cache = NULL;
	try {
		dbCursor<T> cursor(CacheCenter::sharedInstance()->getFastDB(), dbCursorViewOnly);
		int resultSize = cursor.select(q);
		if (resultSize > 0) {
			cache = new T();
			*cache = *cursor.get();
		}
	} catch (dbException e) {
		LOGGER_ERROR(getFastdbTableName() << " - " << e.getMsg());
	}
	T_Ptr cachePtr(cache);
	return cachePtr;
}

template<class T>
int CacheController<T>::_getSizeByQuery(dbQuery& q) {
	if (!CacheMonitor::getInstance()->isFastDBEnable()) {
		LOGGER_ERROR(getFastdbTableName() << " - fastdb is not open!!!");
		return 0;
	}
	try {
		dbCursor<T> cursor(CacheCenter::sharedInstance()->getFastDB(), dbCursorViewOnly);
		return cursor.select(q);

	} catch (dbException e) {
		LOGGER_ERROR(getFastdbTableName() << " - " << e.getMsg());
	}

	return 0;
}

template<class T>
int CacheController<T>::_getSizeByQueryInThreadSafty(dbQuery& q) {
	readLock lock(wr_mutex);
	int size = 0;
	try {
		CacheCenter::sharedInstance()->AttachCurrentThread();
		size = _getSizeByQuery(q);
		CacheCenter::sharedInstance()->DetachCurrentThread();
	} catch (dbException e) {
		LOGGER_ERROR(getFastdbTableName() << " - " << e.getMsg());
	}
	return size;
}

template<class T>
typename CacheController<T>::T_Ptr CacheController<T>::getCacheByQueryInThreadSafty(dbQuery& q) {
	readLock lock(wr_mutex);
	T_Ptr cachePtr;
	try {
		CacheCenter::sharedInstance()->AttachCurrentThread();
		cachePtr = getCacheByQuery(q);
		CacheCenter::sharedInstance()->DetachCurrentThread();
	} catch (dbException e) {
		LOGGER_ERROR(getFastdbTableName() << " - " << e.getMsg());
	}

	return cachePtr;
}

template<class T>
std::map<std::string, T*> CacheController<T>::getCacheHashByQuery(dbQuery& q) {
	try {
		std::map<std::string, T*> cacheList;
		if (!CacheMonitor::getInstance()->isFastDBEnable())
			return cacheList;
		dbCursor<T> cursor(CacheCenter::sharedInstance()->getFastDB(), dbCursorViewOnly);

		T* cache = NULL;
		int resultSize = cursor.select(q);
		if (resultSize > 0) {
			do {
				cache = new T();
				*cache = *cursor.get();
				cacheList.insert(std::pair<std::string, T*>(cache->getPrimaryKey(), cache));
			} while (cursor.next());
		}

		return cacheList;
	} catch (dbException e) {
		LOGGER_ERROR(getFastdbTableName() << " - " << e.getMsg());
	}

	return std::map<std::string, T*>();
}

template<class T>
typename CacheController<T>::T_Vec_Ptr CacheController<T>::getCacheListByQuery(dbQuery& q) {
	T_Vec_Ptr cachePtrList = T_Vec_Ptr(new std::vector<T_Ptr>);
	if (!CacheMonitor::getInstance()->isFastDBEnable()) {
		LOGGER_ERROR(getFastdbTableName() << " - fastdb is not open!!!");
		return cachePtrList;
	}
	dbCursor<T> cursor(CacheCenter::sharedInstance()->getFastDB(), dbCursorViewOnly);
	try {
		int resultSize = cursor.select(q);
		if (resultSize > 0) {
            cachePtrList->reserve(resultSize);
			do {
				T* cache = new T();
				*cache = *cursor.get();
				T_Ptr cachePtr(cache);
				cachePtrList->push_back(cachePtr);
			} while (cursor.next());
		}
	} catch (dbException e) {
		LOGGER_ERROR(getFastdbTableName() << " - " << e.getMsg());
	}
	return cachePtrList;
}

template<class T>
typename CacheController<T>::T_Vec_Ptr CacheController<T>::getCacheListByQueryInThreadSafty(const std::list<std::string>& key_list, const std::string& key, const std::string& sql_prefix) {
	readLock lock(wr_mutex);
	T_Vec_Ptr result(new std::vector<std::shared_ptr<T>>);
	if (key_list.empty() || key.empty()) {
		return result;
	}
	CacheCenter::sharedInstance()->AttachCurrentThread();
	for (std::list<std::string>::const_iterator itorKey = key_list.begin(); itorKey != key_list.end(); itorKey++) {
		if (itorKey->empty())
			continue;
		std::string strSql;
		if (sql_prefix.empty()) {
			strSql = (boost::format("%s='%s'") % key % *itorKey).str();
		} else {
			strSql = (boost::format("%s='%s' and %s") % key % *itorKey % sql_prefix).str();
		}
		dbQuery q(strSql.c_str());
		T_Vec_Ptr list = getCacheListByQuery(q);
        if (list) {
            result->insert(result->end(), list->begin(), list->end());
        }
	}
	CacheCenter::sharedInstance()->DetachCurrentThread();
	return result;
}

template<class T>
void CacheController<T>::getCacheMapByQueryInThreadSafty(const std::list<std::string>& key_list, const std::string& key, std::unordered_map<std::string, T_Ptr>& result, const std::string& sql_prefix /* other conditions */) {
	if (key_list.empty() || key.empty())
		return;
	readLock lock(wr_mutex);
	dbFieldDescriptor* fd = T::dbDescriptor.getFirstField();
	if (!fd)
		return;
	dbFieldDescriptor* fdFirst = fd;
	if (fd->name != key) {
		fd = fd->next;
		while (fd->name != key && fd != fdFirst)
			fd = fd->next;
		if (fd == fdFirst)
			return;
	}
	long long offs = fd->appOffs;

	CacheCenter::sharedInstance()->AttachCurrentThread();
	for (std::list<std::string>::const_iterator itorKey = key_list.begin(); itorKey != key_list.end(); itorKey++) {
		if (itorKey->empty())
			continue;
		std::string strSql;
		if (sql_prefix.empty()) {
			strSql = (boost::format("%s='%s'") % key % *itorKey).str();
		} else {
			strSql = (boost::format("%s='%s' and %s") % key % *itorKey % sql_prefix).str();
		}
		dbQuery q(strSql.c_str());
		T_Vec_Ptr list = getCacheListByQuery(q);
		for (typename std::vector<T_Ptr>::iterator iter = list->begin(); iter != list->end(); ++iter) {
			T* cache = iter->get();
			long long dstOffs = (long long)cache + offs;
			result[*(std::string*)dstOffs] = *iter;
		}
	}
	CacheCenter::sharedInstance()->DetachCurrentThread();
}

template<class T>
typename CacheController<T>::T_Vec_Ptr CacheController<T>::getCacheListByQueryInThreadSafty(dbQuery& q) {
	readLock lock(wr_mutex);
	T_Vec_Ptr cacheList = T_Vec_Ptr(new std::vector<T_Ptr>);
	try {
		CacheCenter::sharedInstance()->AttachCurrentThread();
		cacheList = getCacheListByQuery(q);
		CacheCenter::sharedInstance()->DetachCurrentThread();
	} catch (dbException e) {
		LOGGER_ERROR(getFastdbTableName() << " - " << e.getMsg());
	}
	return cacheList;
}

#endif /* CACHECONTROLLER_H_ */

