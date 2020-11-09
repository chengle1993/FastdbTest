#include "constants.h"
#include <boost/atomic.hpp>
#include "cache/controller/account_cachecontroller.h"
//#include "cache/model/system_company_cache.h"
#include "cache/cache_controller_declare.h"
#include "fid.h"

// qpid for QB sync
std::string kQBSyncTopic, kIDCSyncTopic;
std::string kQBSyncServiceQueue, kIDCSyncServiceQueue;

std::string kSchemaAccount, kSchemaBond, kSchemaBase;

// best quote make and insert into to mysql database
std::string kCalcAndInsertIntoMysqlDatabase;
bool kFirstLaunch;
bool useCacheInMem = false;

bool kCacheLoaded = false;
time_t kServiceReadyTime = -1;
int kCompanySize = 0;
bool kRunningSync = false;

//DCS开关
bool kDcsEnable = false;

//DCS收单状态
bool kDcsSwitchStatus;

//每次请求DCS成交单数据的条数
int kDcsPageSize;

bool kDcsAsyncInitEnable = true;

int kServiceSyncLimit; // datamanager sync pagesize
int kServiceSyncTimeout;
int kDBBatchCommit; // db commit batch size

enum ServiceMode {
    SM_SINGLE = 1,  // 独立模式
    SM_MASTER = 2,  // master模式
    SM_SLAVE = 3,   // slave模式
};
boost::atomic_int kServiceMode(SM_SINGLE);

std::string kBestQuoteSetting = kBestQuoteSettingExternal;

std::string GetCurrentCompanyId(const std::string& account_id) {
	std::string companyId;
	if (account_id.empty())
		return std::string();

	AccountCacheController account_cc;
	std::string str = "id = '" + account_id + "'";
	dbQuery q(str.c_str());
	AccountCachePtr cache = account_cc.getCacheByQueryInThreadSafty(q);
	if (cache != NULL)
		companyId = cache->company_id;

	LOGGER_INFO("Find company id[" + companyId + "] with account id[" + account_id + "]");
	return companyId;
}

std::string GetCurrentCompanyId(const sdbus::Message* msg) {
	std::string company_id;
	msg->GetString(FID_ACCOUNT_COMPANY_ID, company_id);
	if (company_id.empty()) {
        std::string account_id;
        msg->GetString(FID_ACCOUNT_ID, account_id);
		company_id = GetCurrentCompanyId(account_id);
	}

	return company_id;
}
//
//std::string GetCurrentCompanyName(const std::string& company_id) {
//	std::string companyName;
//	if (company_id.empty()) {
//		LOGGER_WARN("Find company name[" + companyName + "] with company id[" + company_id + "]");
//		return companyName;
//	}
//
//	/*AccountCacheController account_cc;
//	std::string str = "company_id = '" + company_id + "' limit 1";
//	dbQuery q(str.c_str());
//	AccountCachePtr cache = account_cc.getCacheByQueryInThreadSafty(q);
//	if (cache != NULL)
//	companyName = cache->company_name;
//	else
//	LOGGER_INFO("Find company name[" + companyName + "] with company id[" + company_id + "]");
//	return companyName;*/
//	std::string str = "id='" + company_id + "'";
//	dbQuery q(str.c_str());
//	SystemCompanyCacheController cc;
//	SystemCompanyCachePtr cache = cc.getCacheByQueryInThreadSafty(q);
//	if (cache.get() != NULL) {
//		companyName = cache->name;
//	} else {
//		LOGGER_INFO("Find company name[" + companyName + "] with company id[" + company_id + "]");
//	}
//	return companyName;
//}

std::string GetCurrentCompanyIdByAccountName(const std::string& account_name) {
	std::string companyId;
	if (account_name.empty())
		return std::string();

	AccountCacheController account_cc;
	std::string str = "username = '" + account_name + "'";
	dbQuery q(str.c_str());
	AccountCachePtr cache = account_cc.getCacheByQueryInThreadSafty(q);
	if (cache != NULL)
		companyId = cache->company_id;

	LOGGER_INFO("Find company id [" + companyId + "] with account name [" + account_name + "]");
	return companyId;
}

std::string GetCurrentAccountName(const std::string& account_id) {
	std::string user_name;
	if (account_id.empty())
		return std::string();

	AccountCacheController account_cc;
	std::string str = "id = '" + account_id + "'";
	dbQuery q(str.c_str());
	AccountCachePtr cache = account_cc.getCacheByQueryInThreadSafty(q);
	if (cache != NULL)
		user_name = cache->username;

	LOGGER_INFO("Find user name [" + user_name + "] with account id [" + account_id + "]");
	return user_name;
}

void SetMasterMode() { kServiceMode = SM_MASTER; }
void SetSlaveMode() { kServiceMode = SM_SLAVE; }
bool SingleMode() { return kServiceMode == SM_SINGLE; }
bool MasterMode() { return kServiceMode == SM_MASTER; }
bool SlaveMode() { return kServiceMode == SM_SLAVE; }
