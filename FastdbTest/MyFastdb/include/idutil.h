#ifndef _ID_UTILS_H_
#define _ID_UTILS_H_
#include <map>
#include <string>
#include <boost/thread/thread.hpp>
#include <boost/thread/shared_mutex.hpp>

using namespace std;
class IDUtil {
public:
	enum IDType {
		QUOTE = 1,
		BEST_BID,
		BEST_OFR,
		DEAL
	};
	static IDUtil* getInstance();

	//bool init();
	//string generateId(const std::string& bond_category, const string &companyId, const IDUtil::IDType& type = IDUtil::IDType::QUOTE);

private:
	IDUtil();
	virtual ~IDUtil();

	void initVersion(int& iQuote, int& iBestBid, int& iBestOfr, int& iDeal, const string& companyId);
	void splitVersion(const std::string& str, int& version);

	static IDUtil* instance_;

	bool useIntId;
	string serviceId, defaultCompanyId;
	map<string, string> type_cache;
	boost::shared_mutex quote_mutex_, best_bid_mutex_, best_ofr_mutex_, deal_mutex_;
};

#endif // _IDB_UTILS_H_