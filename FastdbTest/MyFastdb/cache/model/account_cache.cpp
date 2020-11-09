#include "include/cache_def.h"
#include "account_cache.h"
#include "cache/cache_controller_declare.h"
#include <set>

std::string AccountCache::SCHEMA_NAME = "idb_account";
std::string AccountCache::TABLE_NAME = "idb_account";
std::string AccountCache::PKEY_NAME = "id";
std::string AccountCache::SQL_TAG = "FetchAccountSQL";

AccountCache::AccountCache() {
	info_status = 0;
}

std::string AccountCache::getPrimaryKey() {
	return id;
}

AccountCache::~AccountCache() {
}


bool AccountCache::operator==(const AccountCache &rhs) {
	return isCacheEqual(*this, rhs);
}

bool AccountCache::operator!=(const AccountCache &rhs) {
	return !operator==(rhs);
}

void AccountCache::loadCombinationFields() {
	//loadAccountGroupFields();
	//loadAccountPermissionFields();
	//loadAccountPinyinFields();
}
//
//void AccountCache::loadAccountGroupFields() {
//	AccountGroupCacheController account_group_cc;
//	department_codes = "";
//	role_codes = "";
//
//	dbQuery q;
//	q = "account_id = ", id, " order by department_code";
//	std::set<std::string> dept_codes_set, role_codes_set;
//	AccountGroupCacheVecPtr account_group_cache_list = account_group_cc.getCacheListByQueryInThreadSafty(q);
//	for (size_t i = 0; i < account_group_cache_list->size(); ++i) {
//		const std::string& department_code = account_group_cache_list->at(i)->department_code;
//		const std::string& role_code = department_code + "-" + account_group_cache_list->at(i)->group_name;
//		dept_codes_set.insert(department_code);
//		role_codes_set.insert(role_code);
//	}
//	department_codes = join(dept_codes_set, ", ");
//	role_codes = join(role_codes_set, ", ");
//}
//
//void AccountCache::loadAccountPermissionFields() {
//	AccountPermissionCacheController account_permission_cc;
//	permission_codes = "";
//
//	dbQuery q;
//	q = "account_id = ", id, " order by permission_name";
//	AccountPermissionCacheVecPtr account_permission_cache_list = account_permission_cc.getCacheListByQueryInThreadSafty(q);
//
//	std::set<std::string> permission_set;
//	for (size_t i = 0; i < account_permission_cache_list->size(); ++i) {
//		AccountPermissionCachePtr account_permission = account_permission_cache_list->at(i);
//
//		std::string account_group_id = account_permission->account_group_id;
//
//		AccountGroupCacheController account_group_cc;
//		dbQuery query;
//		query = "account_id = ", id, " and account_group_id = ", account_group_id;
//		AccountGroupCachePtr account_group = account_group_cc.getCacheByQueryInThreadSafty(query);
//		if (account_group) {
//			permission_set.insert(account_group->department_code + "-" + account_permission->permission_name);
//		}
//	}
//	permission_codes = join(permission_set, ", ");
//}
//
//void AccountCache::loadAccountPinyinFields() {
//    if (display_name.empty()) {
//        pinyin = "";
//    } else {
//        pinyin = getFullAndInitialWithSeperator(display_name, "|");
//    }
//}

REGISTER(AccountCache);
