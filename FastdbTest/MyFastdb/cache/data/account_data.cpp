#include "account_data.h"
#include "include/fid.h"
#include "cache/model/account_cache.h"

void AccountData::SetMap(const AccountCache* cache, sdbus::VariantMap *mp, bool isFull) {
    if (isFull) {
        SetMapMore(cache, mp);
    }
    mp->SetString(FID_ID, cache->id);
    mp->SetString(FID_LOGIN_NAME, cache->username);
    mp->SetString(FID_USER_NAME, cache->display_name);
    mp->SetString(FID_ACCOUNT_PASSWORD, cache->password);
    mp->SetString(FID_ACCOUNT_CODE, cache->account_code);
    mp->SetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
    mp->SetString(FID_ACCOUNT_COMPANY_NAME, cache->company_name);
    mp->SetString(FID_ACCOUNT_DEPARTMENT, cache->department_codes);
    mp->SetString(FID_ACCOUNT_ROLE, cache->role_codes);
    mp->SetString(FID_ACCOUNT_PERMISSION, cache->permission_codes);
    mp->SetString(FID_PIN_YIN, cache->pinyin);
    mp->SetString(FID_MODIFY_TIME, cache->modify_time == "" ? cache->create_time : cache->modify_time);

    mp->SetString(FID_ACCOUNT_TELEPHONE, cache->telephone);
    mp->SetString(FID_ACCOUNT_PHONE, cache->mobile);
    mp->SetString(FID_ACCOUNT_EMAIL, cache->email);
    //mp->SetString(FID_ACCOUNT_RM, cache->rm); // DELETE
    mp->SetString(FID_ACCOUNT_MSN, cache->msn);
    mp->SetString(FID_ACCOUNT_QQ, cache->qq);
    mp->SetString(FID_ACCOUNT_ADDRESS, cache->address);
    mp->SetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
    mp->SetString(FID_ACCOUNT_COMPANY_NAME, cache->company_name);
    mp->SetString(FID_ACCOUNT_DEPT, cache->dept);
    mp->SetString(FID_ACCOUNT_PASSWORD_VALID_TIME, cache->valid_time);
}

void AccountData::SetMapMore(const AccountCache* cache, sdbus::VariantMap *mp)
{

}


void AccountData::SetCache(const sdbus::VariantMap *mp, AccountCache *cache) {
    mp->GetString(FID_ID, cache->id);
    mp->GetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
    mp->GetString(FID_ACCOUNT_COMPANY_NAME, cache->company_name);
    mp->GetString(FID_LOGIN_NAME, cache->username);
    mp->GetString(FID_USER_NAME, cache->display_name);
    mp->GetString(FID_ACCOUNT_PASSWORD, cache->password);
    mp->GetString(FID_ACCOUNT_CODE, cache->account_code);
    mp->GetString(FID_ACCOUNT_COMPANY_ID, cache->company_id);
    mp->GetString(FID_ACCOUNT_DEPARTMENT, cache->department_codes);
    mp->GetString(FID_ACCOUNT_ROLE, cache->role_codes);
    mp->GetString(FID_ACCOUNT_PERMISSION, cache->permission_codes);
    mp->GetString(FID_ACCOUNT_CODE, cache->account_code);
    mp->GetString(FID_PIN_YIN, cache->pinyin);
    mp->GetString(FID_MODIFY_TIME, cache->modify_time);
    mp->GetString(FID_ACCOUNT_DEPT, cache->dept);
    mp->GetString(FID_ACCOUNT_PASSWORD_VALID_TIME, cache->valid_time);
}
