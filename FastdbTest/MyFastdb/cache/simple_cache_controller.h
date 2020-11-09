#ifndef _SIMPLE_CACHECONTROLLER_H_
#define _SIMPLE_CACHECONTROLLER_H_

#include <string>
#include "include/logger.h"
#include "db/dbconnector.h"
#include "include/pugixml.hpp"
#include <boost/thread/mutex.hpp>
#include <boost/thread/shared_mutex.hpp>
#include "include/constants.h"
#include "include/common.h"

typedef boost::mutex                   Uni_Mutex;
typedef boost::shared_mutex            WR_Mutex;
typedef boost::unique_lock<WR_Mutex>   writeLock;
typedef boost::shared_lock<WR_Mutex>   readLock;

namespace sql {
	class ResultSet;
}

template<class T, class T_Ptr, class T_Vec_Ptr>
class SimpleCacheController {
public:
	// default constructor, for cache without db sync
	SimpleCacheController();

	// constructor, for cache with db sync
	SimpleCacheController(const std::string &sql_tag);

	void init();

	void sync(std::vector<T_Ptr>& update_vec, std::vector<T_Ptr>& add_vec, std::vector<T_Ptr>& delete_vec);

	// get one row data by primary key
	T_Ptr get(const std::string &id) const;
	// get all data
	T_Vec_Ptr getVec() const;

	int size() const;

	// update one row
	bool updateCache(const std::string &id, const T_Ptr &cache);
	// insert one row
	bool insertCache(const std::string &id, const T_Ptr &cache);
	// delete one row
	bool deleteCache(const std::vector<std::string> &id_list);

	// update one row
	bool updateCacheWithoutDBSync(const std::string &id, const T_Ptr &cache);
	// insert one row
	bool insertCacheWithoutDBSync(const std::string &id, const T_Ptr &cache);
	// delete one row
	bool deleteCacheWithoutDBSync(const std::vector<std::string> &id_list);

protected:
	// load db data
	virtual void loadCache(std::map<std::string, T_Ptr> &data_mp, sql::ResultSet *resSet) = 0;

	// save data to db
	virtual int updateDiskDB(const T_Ptr& cache_new, const T_Ptr& cache_old) = 0;
	virtual int insertDiskDB(const T_Ptr& cache) = 0;
	virtual int deleteDiskDB(const std::vector<std::string> &id_list) = 0;
	sql::ResultSet* query();

	// get one row data from data mp by primary key 
	T_Ptr get(const std::map<std::string, T_Ptr> &data_mp, const std::string &id) const;

	void insert(const std::string &id, const T_Ptr &cache);
	void update(const std::string &id, const T_Ptr &cache);
	void remove(const std::vector<std::string> &id_list);

	// load data from db, save to data mp
	void load(std::map<std::string, T_Ptr> &data_mp, sql::ResultSet *resSet);
	// compare new data map with old data map, and update data map
	void calcDataMp(std::map<std::string, T_Ptr> &data_mp_new, std::vector<T_Ptr>& update_vec, std::vector<T_Ptr>& add_vec, std::vector<T_Ptr>& delete_vec);

	// get fetch sql str
	std::string fetchSQL(const std::string &name);

	std::map<std::string, T_Ptr> data_mp_;  // data mp

	std::string sql_tag_;                   // sql tag
	std::string schema_name_;               // database schema

	bool db_sync_;                          // db sync flag

private:
	mutable WR_Mutex wr_mutex;              // mutex, protect the access for data_mp_

	//static log4cxx::LoggerPtr logger_;
};

//template<class T, class T_Ptr, class T_Vec_Ptr>
//log4cxx::LoggerPtr SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::logger_ = log4cxx::Logger::getLogger(logger_name_ + "SimpleCacheController");

template<class T, class T_Ptr, class T_Vec_Ptr>
SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::SimpleCacheController()
	: db_sync_(false), schema_name_(T::SCHEMA_NAME) {

}

template<class T, class T_Ptr, class T_Vec_Ptr>
SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::SimpleCacheController(const std::string &sql_tag)
	: db_sync_(true), sql_tag_(sql_tag), schema_name_(T::SCHEMA_NAME) {

}

template<class T, class T_Ptr, class T_Vec_Ptr>
void SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::init() {
	if (!db_sync_) {
		return;
	}

	sql::ResultSet* resSet = query();

	// load data from db
	if (resSet != NULL) {
		writeLock lock(wr_mutex);
		load(data_mp_, resSet);
	}
}

template<class T, class T_Ptr, class T_Vec_Ptr>
void SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::sync(std::vector<T_Ptr>& update_vec, std::vector<T_Ptr>& add_vec, std::vector<T_Ptr>& delete_vec) {
	if (!db_sync_) {
		return;
	}

	sql::ResultSet* resSet = query();

	// load latest data from db
	if (resSet != NULL) {
		std::map<std::string, T_Ptr> data_mp_new;
		load(data_mp_new, resSet);

		calcDataMp(data_mp_new, update_vec, add_vec, delete_vec);
	}
}

template<class T, class T_Ptr, class T_Vec_Ptr>
std::string SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::fetchSQL(const std::string &name) {
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file("sqlquery.xml");

	if (result.status != pugi::status_ok) {
		return "";
	}
	std::string sql_str = doc.child("SQL").child(name.c_str()).child_value();
	trimSql(sql_str);
	//if(name == "FetchAccountSQL")
	//    sql_str += " AND idb_account.company_id = '" + kCurrentCompanyId + "';"; //Del by Young 05-Apr-2016

	return sql_str;
}

template<class T, class T_Ptr, class T_Vec_Ptr>
sql::ResultSet* SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::query() {
	if (sql_tag_.length() == 0 || schema_name_.length() == 0) {
		return NULL;
	}

	std::string sql_str = fetchSQL(sql_tag_);

	if (sql_str.length() == 0) {
		return NULL;
	}

	return DBConnector::getInstance()->executeQuery(sql_str, schema_name_);
}

template<class T, class T_Ptr, class T_Vec_Ptr>
T_Ptr SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::get(const std::string &id) const {
	readLock lock(wr_mutex);
	return get(data_mp_, id);
}

template<class T, class T_Ptr, class T_Vec_Ptr>
T_Vec_Ptr SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::getVec() const {
	readLock lock(wr_mutex);
	T_Vec_Ptr vec(new std::vector<T_Ptr>());

	typename std::map<std::string, T_Ptr>::const_iterator it = data_mp_.begin();

	for (; it != data_mp_.end(); ++it) {
		vec->push_back((*it).second);
	}

	return vec;
}

template<class T, class T_Ptr, class T_Vec_Ptr>
int SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::size() const {
	readLock lock(wr_mutex);
	return data_mp_.size();
}

template<class T, class T_Ptr, class T_Vec_Ptr>
bool SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::updateCache(const std::string &id, const T_Ptr &cache) {
	T_Ptr cache_old = this->get(id);

	if (cache_old == NULL) {
		return false;
	}

	if (db_sync_) {
		if (updateDiskDB(cache, cache_old) > 0) {
			this->update(id, cache);
			return true;
		} else {
			return false;
		}
	} else {
		this->update(id, cache);
		return true;
	}
}

template<class T, class T_Ptr, class T_Vec_Ptr>
bool SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::insertCache(const std::string &id, const T_Ptr &cache) {
	if (db_sync_) {
		if (insertDiskDB(cache) > 0) {
			this->update(id, cache);
			return true;
		} else {
			return false;
		}
	} else {
		this->insert(id, cache);
		return true;
	}
}

template<class T, class T_Ptr, class T_Vec_Ptr>
bool SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::deleteCache(const std::vector<std::string> &id_list) {
	if (id_list.size() == 0) {
		return true;
	}
	if (db_sync_) {
		if (deleteDiskDB(id_list) > 0) {
			this->remove(id_list);
			return true;
		} else {
			return false;
		}
	} else {
		this->remove(id_list);
		return true;
	}
}

template<class T, class T_Ptr, class T_Vec_Ptr>
bool SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::updateCacheWithoutDBSync(const std::string &id, const T_Ptr &cache) {
	T_Ptr cache_old = this->get(id);

	if (cache_old == NULL) {
		return false;
	}

	this->update(id, cache);
	return true;
}

template<class T, class T_Ptr, class T_Vec_Ptr>
bool SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::insertCacheWithoutDBSync(const std::string &id, const T_Ptr &cache) {
	this->insert(id, cache);
	return true;
}

template<class T, class T_Ptr, class T_Vec_Ptr>
bool SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::deleteCacheWithoutDBSync(const std::vector<std::string> &id_list) {
	this->remove(id_list);
	return true;
}

template<class T, class T_Ptr, class T_Vec_Ptr>
T_Ptr SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::get(const std::map<std::string, T_Ptr> &data_mp, const std::string &id) const {
	typename std::map<std::string, T_Ptr>::const_iterator it = data_mp.find(id);

	if (it != data_mp.end()) {
		return (*it).second;
	}

	return T_Ptr();
}

template<class T, class T_Ptr, class T_Vec_Ptr>
void SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::insert(const std::string &id, const T_Ptr &cache) {
	writeLock lock(wr_mutex);
	data_mp_[id] = cache;
}

template<class T, class T_Ptr, class T_Vec_Ptr>
void SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::update(const std::string &id, const T_Ptr &cache) {
	writeLock lock(wr_mutex);
	data_mp_[id] = cache;
}

template<class T, class T_Ptr, class T_Vec_Ptr>
void SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::remove(const std::vector<std::string> &id_list) {
	writeLock lock(wr_mutex);

	for (size_t i = 0; i < id_list.size(); ++i) {
		data_mp_.erase(id_list[i]);
	}
}

template<class T, class T_Ptr, class T_Vec_Ptr>
void SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::load(std::map<std::string, T_Ptr> &data_mp, sql::ResultSet* resSet) {
	if (resSet != NULL) {
		LOGGER_INFO(sql_tag_ << " - cache resultSet: " << resSet->rowsCount());
		while (resSet->next()) {
			loadCache(data_mp, resSet);
		}
		delete resSet;
		LOGGER_INFO(sql_tag_ << " - load success. ");
	}
}

template<class T, class T_Ptr, class T_Vec_Ptr>
void SimpleCacheController<T, T_Ptr, T_Vec_Ptr>::calcDataMp(std::map<std::string, T_Ptr> &data_mp_new, std::vector<T_Ptr>& update_vec, std::vector<T_Ptr>& add_vec, std::vector<T_Ptr>& delete_vec) {
	//
	// compare new cache with old cache
	typename std::map<std::string, T_Ptr>::const_iterator iter_new = data_mp_new.begin();
	for (; iter_new != data_mp_new.end(); ++iter_new) {
		std::string id = (*iter_new).first;
		T_Ptr cache_new = (*iter_new).second;

		if (cache_new == NULL) {
			continue;
		}

		T_Ptr cache = this->get(id);

		if (cache) {
			if (*cache_new != *cache) {
				// Update
				update_vec.push_back(cache_new);
				this->update(id, cache_new);
			}
		} else {
			// Add
			add_vec.push_back(cache_new);
			this->insert(id, cache_new);
		}
	}

	{
		writeLock lock(wr_mutex);

		typename std::map<std::string, T_Ptr>::const_iterator iter = data_mp_.begin();
		for (; iter != data_mp_.end();) {
			std::string id = (*iter).first;
			T_Ptr cache = (*iter).second;

			T_Ptr cache_new = this->get(data_mp_new, id);

			if (cache_new == NULL) {
				// Delete
				delete_vec.push_back(cache);
				data_mp_.erase(iter++);
			} else {
				iter++;
			}
		}
	}
}

#endif // _SIMPLE_CACHECONTROLLER_H_