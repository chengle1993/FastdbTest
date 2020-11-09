#include "idutil.h"
#include <vector>

#include "config.h"
//#include "../business/VersionBusiness.h"
#include "cache/cache_controller_declare.h"
#include "db/dbconnector.h"
#include "common.h"
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

IDUtil* IDUtil::instance_ = NULL;

#define WRITELOCK(a) boost::unique_lock<boost::shared_mutex> lock(a)

vector<string> _ID_KEYS = {
	"ID_QUOTE", "ID_BESTBID", "ID_BESTOFR", "ID_DEAL"
};

string _SQL_QUOTE = "select id from bond_offer where company_id='%s' and createtime>'%s' and id like '%s' order by createtime desc, id desc limit 1",
_SQL_BEST = "select push_bid_id,push_ofr_id from bond_best_offer where company_id='%s' and createtime>'%s' order by createtime desc, push_bid_id desc, push_ofr_id desc limit 1",
_SQL_DEAL = "select id from bond_deal where company_id='%s' and createtime>'%s' and id like '%s' order by createtime desc, id desc limit 1";

IDUtil* IDUtil::getInstance() {
	if (NULL == instance_) {
		instance_ = new IDUtil;
	}
	return instance_;
}

IDUtil::IDUtil() {
	useIntId = Config::singleton()->getBoolValue("Service.UseIntId", false);
	serviceId = Config::singleton()->getValue("Service.ServiceId", "1");
	defaultCompanyId = Config::singleton()->getValue("Service.CompanyId");
	type_cache["BCO"] = "01";
	type_cache["BNC"] = "02";
	type_cache["ABS"] = "03";
	type_cache["NCD"] = "04";
}

IDUtil::~IDUtil() {

}
//
//bool IDUtil::init() {
//	if (!useIntId) {
//		LOGGER_DEBUG("use old id-generator. useIntId: " << (useIntId ? "true" : "false"));
//		return true;
//	}
//	string date = GetTDateString("%Y%m%d");
//	SystemCompanyCacheController company_cc;
//	dbQuery q;
//	q = "status='1'";
//	SystemCompanyCacheVecPtr company_vec = company_cc.getCacheListByQueryInThreadSafty(q);
//	if (!company_vec || company_vec->empty()) {
//		LOGGER_ERROR("cannot find valid company!!! use default id-generator.");
//		useIntId = false;
//		return false;
//	}
//	for (size_t i = 0; i < company_vec->size(); ++i) {
//		int iQuoteVersion = 0, iBestBidVersion = 0, iBestOfrVersion = 0, iDealVersion = 0;
//		std::string tCompanyId = company_vec->at(i)->id, companyId;
//		if (!tCompanyId.empty() && isDigits(tCompanyId)) {
//			if (tCompanyId.length() > 2) {
//				if (!defaultCompanyId.empty() && company_vec->size() == 1) {
//					companyId = defaultCompanyId;
//				} else {
//					LOGGER_WARN("config error, use default id-generator!!! size: " << company_vec->size() << ", companyId: " << tCompanyId << ", config companyId: " << defaultCompanyId);
//					useIntId = false;
//					return true;
//				}
//			} else {
//				companyId = tCompanyId;
//			}
//		} else {
//			if (company_vec->size() == 1 && !defaultCompanyId.empty()) {
//				companyId = defaultCompanyId;
//			} else {
//				LOGGER_WARN("config error, use default id-generator!!! size: " << company_vec->size() << ", companyId: " << tCompanyId << ", config companyId: " << defaultCompanyId);
//				useIntId = false;
//				return true;
//			}
//		}
//		initVersion(iQuoteVersion, iBestBidVersion, iBestOfrVersion, iDealVersion, companyId);
//		VersionBusiness::InitVersion("ID_QUOTE", companyId, iQuoteVersion);
//		VersionBusiness::InitVersion("ID_BESTBID", companyId, iBestBidVersion);
//		VersionBusiness::InitVersion("ID_BESTOFR", companyId, iBestOfrVersion);
//		VersionBusiness::InitVersion("ID_DEAL", companyId, iDealVersion);
//	}
//	
//	return true;
//}
//string IDUtil::generateId(const std::string& bond_category, const std::string &tCompanyId, const IDUtil::IDType& type) {
//	if (!useIntId || bond_category.empty() || tCompanyId.empty())
//		return GetNewGuid();
//	if (type_cache.find(bond_category) == type_cache.end()) {
//		return GetNewGuid();
//	}
//	string companyId = tCompanyId, date = GetTDateString("%Y%m%d").substr(2, 6), id;
//	if (isDigits(tCompanyId)) {
//		if (tCompanyId.length() > 2) {
//			if (defaultCompanyId.empty()) {
//				return GetNewGuid();
//			} else {
//				companyId = defaultCompanyId;
//			}
//		}
//	} else {
//		if (defaultCompanyId.empty()) {
//			return GetNewGuid();
//		} else {
//			companyId = tCompanyId;
//		}
//	}
//	VersionBusiness busi;
//	int version = 0;
//	switch (type) {
//	case IDType::QUOTE:{
//		WRITELOCK(quote_mutex_);
//		busi.IncOneAndGetVersion("ID_QUOTE", companyId, version);
//		break;
//	}
//	case IDType::BEST_BID:{
//		WRITELOCK(best_bid_mutex_);
//		busi.IncOneAndGetVersion("ID_BESTBID", companyId, version);
//		break;
//	}
//	case IDType::BEST_OFR:{
//		WRITELOCK(best_ofr_mutex_);
//		busi.IncOneAndGetVersion("ID_BESTOFR", companyId, version);
//		break;
//	}
//	case IDType::DEAL:{
//		WRITELOCK(deal_mutex_);
//		busi.IncOneAndGetVersion("ID_DEAL", companyId, version);
//		break;
//	}
//	default:
//		return GetNewGuid();
//		//break;
//	}
//	std::stringstream ss;
//	ss << date << boost::format("%02s") % companyId << serviceId << (type_cache[bond_category]) << type << boost::format("%012d") % version;
//	id = ss.str();
//	return id;
//}

void IDUtil::initVersion(int& iQuote, int& iBestBid, int& iBestOfr, int& iDeal, const string& companyId) {
	sql::ResultSet* res = NULL;
	string date = GetTDateString("%Y-%m-%d"), id_ = GetTDateString("%Y%m%d").substr(2, 6) + "%";
	string sql = boost::str(boost::format(_SQL_QUOTE) % companyId % date % id_);
	LOGGER_DEBUG("init version ... quote sql: " << sql);
	res = DBConnector::getInstance()->executeQuery(sql, kSchemaBond);
	if (res != NULL) {
		if (res->next()) {
			splitVersion(res->getString("id"), iQuote);
		}
	}
	delete res;
	sql = boost::str(boost::format(_SQL_BEST) % companyId % date);
	LOGGER_DEBUG("init version ... best quote sql: " << sql);
	res = DBConnector::getInstance()->executeQuery(sql, kSchemaBond);
	if (res != NULL) {
		if (res->next()) {
			splitVersion(res->getString("push_bid_id"), iBestBid);
			splitVersion(res->getString("push_ofr_id"), iBestOfr);
		}
	}
	delete res;
	sql = boost::str(boost::format(_SQL_DEAL) % companyId % date % id_);
	LOGGER_DEBUG("init version ... deal sql: " << sql);
	res = DBConnector::getInstance()->executeQuery(sql, kSchemaBond);
	if (res != NULL) {
		if (res->next()) {
			splitVersion(res->getString("id"), iDeal);
		}
	}
	delete res;
}

void IDUtil::splitVersion(const std::string& str, int& version) {
	if (str.empty())
		return;
	if (!isDigits(str))
		return;
	string date = GetTDateString("%Y%m%d").substr(2, 6);
	if (date != str.substr(0, 6))
		return;
	std::string t = str.substr(12, 12);
	version = ss_lexical_cast(t, 0);
}