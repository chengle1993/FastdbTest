#include "include/cache_def.h"
#include "cache/cache_center.h"
#include "account_cachecontroller.h"

AccountCacheController::AccountCacheController() {
	// TODO Auto-generated constructor stub
}

AccountCacheController::~AccountCacheController() {
	// TODO Auto-generated destructor stub
}

void AccountCacheController::insertCache(sql::ResultSet* resSet) {
	SET_PROPERTIES_BEGIN(AccountCache);

	// Set customerized properties
	//CacheCenter::sharedInstance()->AttachCurrentThread();
	cache.loadCombinationFields();
	//CacheCenter::sharedInstance()->DetachCurrentThread();

	SET_PROPERTIES_END;
}
