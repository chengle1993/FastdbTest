#ifndef ACCOUNT_CACHE_H_
#define ACCOUNT_CACHE_H_

#include "include/cache_def.h"

class AccountCache;
typedef std::shared_ptr<AccountCache> AccountCachePtr;
typedef std::shared_ptr<std::vector<AccountCachePtr> > AccountCacheVecPtr;

class AccountCache {
public:
	static std::string SCHEMA_NAME;
	static std::string TABLE_NAME;
	static std::string PKEY_NAME;
	static std::string SQL_TAG;
public:
	AccountCache();
	virtual ~AccountCache();
	std::string getPrimaryKey();

	bool operator ==(const AccountCache &rhs);
	bool operator !=(const AccountCache &rhs);

	void loadCombinationFields();
	void loadAccountGroupFields();
	void loadAccountPermissionFields();
	void loadAccountPinyinFields();

	std::string id;
	std::string company_id;
	std::string company_name;
	std::string account_code;
	std::string username;
	std::string password;
	std::string display_name;
	std::string mobile;
	std::string telephone;
	std::string email;
	std::string rm;
	std::string msn;
	std::string qq;
	std::string address;
	std::string account_type;
	std::string is_forbidden;
	std::string create_time;
	std::string modify_time;
	std::string create_by;
	std::string modify_by;
	std::string status;
	std::string dept;
	std::string title;
	std::string valid_time;
	int info_status;
	std::string role_id;

	// ----------------- Combination Fields ----------------------
	//
	std::string department_codes;
	std::string role_codes;

	std::string permission_codes;

	//Non-Dabase Fields
	std::string pinyin;

	static std::map<std::string, std::string> pinyin_mp;

public:
	TYPE_DESCRIPTOR(
		(KEY(id, HASHED/* | INDEXED*/),
		KEY(company_id, 0/*HASHED | INDEXED*/),
		FIELD(company_name),
		KEY(account_code, HASHED/* | INDEXED*/),
		KEY(username, HASHED/* | INDEXED*/),
		FIELD(password),
		FIELD(display_name),
		FIELD(mobile),
		FIELD(telephone),
		FIELD(email),
		FIELD(rm),
		FIELD(msn),
		FIELD(qq),
		FIELD(address),
		FIELD(account_type),
		FIELD(is_forbidden),
		FIELD(create_time),
		FIELD(modify_time),
		FIELD(create_by),
		FIELD(modify_by),
		FIELD(status),
		FIELD(dept),
		FIELD(title),
		FIELD(valid_time),
		FIELD(info_status),
		NOUPDATE(role_id),
		COMBINED(department_codes),
		COMBINED(role_codes),
		COMBINED(permission_codes),
		COMBINED(pinyin)
		)
		);
};

#endif /* ACCOUNT_CACHE_H_ */
