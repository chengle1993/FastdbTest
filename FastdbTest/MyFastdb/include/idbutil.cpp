#include "idbutil.h"

#include "common.h"
#include "config.h"
#include "constants.h"
#include "cache/cache_controller_declare.h"
#include "cache/model/account_cache.h"
//#include "../business/SystemCompanyBusiness.h"
#include <boost/foreach.hpp>
//#include "../cache/controller/bondsmallview_cachecontroller.h"

IDBUtil* IDBUtil::instance_ = NULL;
#define WRITELOCK(a) boost::unique_lock<boost::shared_mutex> lock(a)
IDBUtil* IDBUtil::getInstance() {
	if (NULL == instance_) {
		instance_ = new IDBUtil;
	}
	return instance_;
}

IDBUtil::IDBUtil() {
	rng.seed(GetCurrentTimeMilliSec());
}

IDBUtil::~IDBUtil() {

}

int IDBUtil::getRandomInt(int min, int max) {
	boost::random::uniform_int_distribution<> random(min, max);
	int x = random(rng);
	return x;
}

std::string lastSyncTime;
bool IDBUtil::isSyncSchedule() {
	if (!kCacheLoaded) {
		LOGGER_WARN("cache data is not loaded or service is not ready, ignore!!! kCacheLoaded: " << (kCacheLoaded ? "true" : "false") << ", kRunningSync: " << (kRunningSync ? "true" : "false"));
		return false;
	}
	if (kRunningSync) {
		LOGGER_WARN("sync base data is running, ignore!!! kRunningSync: " << (kRunningSync ? "true" : "false") << ", lastSyncTime: " << lastSyncTime);
		return false;
	}
	string schedule = Config::singleton()->getValue("Service.DataSchedule", "07:00,12:30"),
		datetime = GetCurrentTimeString(),
		date = GetCurrentDateString();
	int diff = -1;
	if (!lastSyncTime.empty()) {
		diff = GetMsTimeDiff(lastSyncTime + ".000", datetime + ".000");
	}
	vector<string> vec = split(schedule, ',');
	BOOST_FOREACH(const string& s, vec) {
		if (s.length() != 5) {
			continue;
		}
		string str = date + " " + s;
		if (strContains(datetime, str)) {
			if (diff < 1 || diff > 1000 * 60) {
				LOGGER_INFO("sync base data ready to schedule ... conf: " << s << ", now time: " << datetime);
				lastSyncTime = datetime;
				return true;
			} else {
				LOGGER_DEBUG("sync base data is already scheduled. conf: " << s << ", kRunningSync: " << (kRunningSync ? "true" : "false")
					<< ", now time: " << datetime << ", last schedule time: " << lastSyncTime);
			}
		}
	}
	return false;
}

void IDBUtil::updateCacheSchema() {
	AccountCache::SCHEMA_NAME = kSchemaAccount;
}
