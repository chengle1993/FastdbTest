#ifndef ACCOUNT_CACHECONTROLLER_H_
#define ACCOUNT_CACHECONTROLLER_H_

#include "cache/cache_controller.h"
#include "cache/model/account_cache.h"

class AccountCacheController : public CacheController<AccountCache> {
public:
	AccountCacheController();
	virtual ~AccountCacheController();

private:
	void insertCache(sql::ResultSet* resSet);

};

#endif /* ACCOUNT_CACHECONTROLLER_H_ */
