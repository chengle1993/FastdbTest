#ifndef _COMMON_H_
#define _COMMON_H_

#include <string>
#include <map>
#include <vector>
#include <list>
#include <set>
#include "sdbus/string.h"
#include "sdbus/message.h"
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/noncopyable.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#define INVALIDVALUE -999
#define STR_INVALIDVALUE "-999"
#define FLOATING_PRECISION 1.0e-7

class BondFilter;

std::string GetNewGuid();
std::string& Trim(std::string &s);
void split(const std::string& s, const std::string& delim, std::vector< std::string >* ret);
std::string ParseCondition(const sdbus::string& cond, int cond_fid);
std::string ParseCondition(const sdbus::string& cond, const std::map<int, std::string>& fid_mp);
std::string ParseSortBy(const sdbus::string& sortby, const std::map<int, std::string>& fid_mp, const std::string& sortby_default);
std::string ParseRange(const sdbus::string& range);
void ParseRange(const sdbus::string& range, int& start, int& cnt);
std::string ParseParam(const sdbus::string& cond, const sdbus::string& sortby, const sdbus::string& range,
	const std::map<int, std::string>& fid_mp, const std::string& sortby_default = "");

int ParseRatingStr(const std::string& rating_str);

std::string	ParseRateType(std::string &FRN_Index_ID, int Next_Coupon_Date, int MaturityDate);

std::string GetTimeToMaturity(int maturity_date, int interest_start_date, const std::string& option_type_client);
double GetTimeToMaturityReal(int maturity_date, int interest_start_date, const std::string& option_type_client);
double GetTimeToMaturity(int maturity_date, int interest_start_date);
std::string CheckOnTheRun(int listed_date);
int GetSortIndexByBondType(const std::string &bondtype);
std::string GBKToUTF8(const std::string& strGBK);
std::string Utf8ToAnsi(const sdbus::string &strUTF8);
std::string GetCompletedBondType(const std::string &bondtype, const std::string &frnindexid, const std::string &optiontype);
bool hasOptionType(const std::string &optiontype);

std::string BuildTitle(const std::string&bond_type);

time_t ParseTimeString(const std::string &time_str);
time_t ParseMaturityDateString(const std::string &date_str, const char* format);
time_t ParseModifyTime(const std::string &modifytime); // 毫秒
struct tm* LocalTimeSafe(time_t t, struct tm &local);
std::string GetTimeString(time_t t);
time_t GetCurrentTimeMilliSec();     // 获取毫秒级时间
time_t GetCurrentTimeMicroSec();     // 获取微秒级时间
std::string GetCurrentTimeString();
std::string GetCurrentDateString();
std::string GetCurrentTimeString_x_minutes_ago(int min);
std::string GetTime_x_seconds_latter(const std::string& strTime, int sec);
std::string GetTodayDate();
std::string GetCurrentMinuteString();
std::string GetCurrentHMSString();
std::string GetTDateString(const char* format);
std::string GetTDateString(time_t t, const char* format);
std::string GetT1DateString(const char* format);
std::string GetBeforeMonthsString(const char* format, int months);
std::string GetBeforeDaysString(const char* format, int days);
std::string GetBeforeMinuteString(int minute); // yyyy-MM-dd HH:mm:ss
std::string GetFullDateTimeString(); // yyyy-MM-dd HH:mm:ss.zzz
std::string GetDateTimeForMillisecond(time_t millisecond);
std::string GetDateTimeForMillisecond(); // yyyy-MM-dd HH:mm:ss.zzz
time_t yyyymmdd2time_t(const std::string yyyymmdd); // yyyy-MM-dd

void GetPriceStrings(double price, const std::string& fan_dian_flag, double fan_dian, int symbol, int price_type, std::string& price_string, std::string& fan_dian_strin);
void GetBestPriceStrings(double price, const std::string &fan_dian_flag, double fan_dian, const std::string &quote_id, int quote_side, std::string &price_string, std::string &fan_dian_string);

int CountChineseWords(const std::string& str);

std::string GetDealStatusString(const std::string& deal_status, const bool bid_checked, const bool ofr_checked);

std::string GetDcsDealStatusString(const std::string& deal_status, bool in_hand);

double Round(double t, int n);
std::string Format(double t, int min, int max = -1);
std::string truncateDouble(double value, const int& percision);

inline bool doubleEqual(const double& d1, const double& d2) //相等返回true，否则返回false
{
	return fabs(d1 - d2) < FLOATING_PRECISION;
}

std::string IntToString(int value);
std::string FloatToString(float value);
template<typename T>
std::string T2Str(const T &t)
{
    std::stringstream ss;
    ss << t;
    return ss.str();
}

std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);
std::string join(const std::vector<std::string>& str_arr, const std::string& delim);
std::string joinSql(const std::vector<std::string>& str_arr);
std::string join(const std::set<std::string>& str_arr, const std::string& delim);
template<typename T>
std::string join2Str(const std::set<T>& sets, const std::string& delim) {
	if (sets.empty())
		return "";
	std::ostringstream ss;
	typename std::set<T>::const_iterator it = sets.cbegin();
	size_t count = 0;
	for (; it != sets.cend(); ++it) {
		ss << *it;
		count++;
		if (count < sets.size())
			ss << delim;
	}
	return ss.str();
}
char* allocString(const std::string& value);
void stringSetter(const char** property, const std::string& value);
char const* stringGetter(char const* property);
void clearStringProperty(const char** property);

// add by lsq 判断某一天是星期几
size_t getWeekDay(size_t year, size_t month, size_t day);
int GetBondSmallTermDays(int maturity_date, int interest_start_date);
double GetBondSmallTermYears(int maturity_date);
int GetDayDifference(int from_date, int to_date);
int GetMsTimeDiff(const std::string& fromMsTime, const std::string& toMsTime);
long long GetMsTimeFromStr(const std::string& msTime);

int GetMaturityTermInDays(int end_date, std::pair<std::string, std::string>& maturity_term_pair); 
std::pair<std::string, std::string> CalculateMaturityTerm(int maturity_date, int option_date);
std::string GetMaturityTermInDays(int start_date, int end_date);
std::string GetIpAddressFromString(const std::string &msg_string);

// zip sdbus msg
bool zipMessage(const sdbus::Message &msg, sdbus::Message &zip_msg);
bool unzipMessage(const sdbus::Message &zip_msg, sdbus::Message &msg);
bool encodeMessage(const sdbus::Message &msg, sdbus::Message &zip_msg, size_t &srcLen, size_t &dstLen);
void decodeMessage(const sdbus::Message &zip_msg, sdbus::Message &msg);

bool isBCOCategoryType(const std::string& bond_category);
bool isLocalGovType(const std::string& bond_category, const std::string& bond_type);

enum QQ2IDB_ERROR_CODE {
	QQ2IDB_ERROR_BOND_CODE_IS_EMPTY = 1,
	QQ2IDB_ERROR_BOND_CODE_NOT_FOUND_IN_IDB,
	QQ2IDB_ERROR_BOND_INSTITUTION_IS_EMPTY,
	QQ2IDB_ERROR_BOND_INSTITUTION_NOT_FOUND_IN_IDB,
	QQ2IDB_ERROR_BOND_LISTMARKET_NOT_CORRECT,
	QQ2IDB_ERROR_BOND_KEY_LISTED_MARKET_IS_EMPTY,
	QQ2IDB_ERROR_BOND_TRADER_IS_EMPTY,
	QQ2IDB_ERROR_BOND_TRADER_NOT_FOUND_IN_IDB,
	QQ2IDB_ERROR_BOND_PRICE_IS_NEGATIVE,
	QQ2IDB_ERROR_BOND_VOLUME_IS_NEGATIVE,
	QQ2IDB_ERROR_BOND_CODE_WITH_INVALID_FORMAT,
	QQ2IDB_ERROR_BOND_NOT_FOUND_WITH_MARKET_AND_SHORTNAME_IN_IDB,
	QQ2IDB_ERROR_FOUND_MULTI_BOND_IN_IDB,
	QQ2IDB_ERROR_FOUND_EMPTY_BOND_CODE_WITH_MARKET_AND_SHORTNAME_IN_IDB,
	QQ2IDB_ERROR_COULD_NOT_FOUND_CREATED_INSTITUTION,
	QQ2IDB_ERROR_COULD_NOT_FOUND_CREATED_TRADER,
	QQ2IDB_ERROR_COULD_NOT_GET_BOND_OR_INSTITUTION_OR_TRADER
};
#define QQ2IDB_ERROR_NAME(CODE) #CODE
std::string getIntToStringWithBracket(int i);

/* 20170101 -> 2017-01-01*/
std::string dateShortToLong(const std::string& shortDate);
/*return yyyy-MM-dd*/
std::string dateToStr(const boost::gregorian::date& date);
/* yyyy-MM-dd*/
boost::gregorian::date strToDate(const std::string& dateStr);
boost::gregorian::date currentDate();
time_t to_time_t(const boost::gregorian::date& date);
boost::gregorian::date_duration dateDuration(const boost::gregorian::date& start, const boost::gregorian::date& end);
boost::gregorian::date getNextWorkDate(const std::set<std::string>& holiday_date_set, const boost::gregorian::date& date);
boost::gregorian::date getLastWorkDate(const std::set<std::string>& holiday_date_set, const boost::gregorian::date& date);
boost::gregorian::date getLastWorkDate(const std::set<std::string>& holiday_date_set, const boost::gregorian::date& date, int days); // 取date日之前days天的工作日
bool lessThan1Year(const std::string& endDate);

void initHolidayCache();
const std::set<std::string>& getHolidayCache(const std::string& market_type = "CIB");
void clearHolidayCache();

bool strContains(const std::string& source, const std::string& str);

std::string GetFastdbQuery(const std::string &key, const std::string &val);
std::string GetFastdbQueryByBondKeyListedMarket(const std::string &val, const std::string &companyId);
std::string GetFastdbQueryById(const std::string &val, const std::string &companyId);

template<typename T>
std::list<T> vector2list(const std::vector<T> &vec) {
	std::list<T> result;
	for (auto iter = vec.begin(); iter != vec.end(); ++iter) {
		result.push_back(*iter);
	}
	return std::move(result);
}

template<typename T>
std::vector<T> list2vector(const std::list<T> &list) {
	std::vector<T> result;
	for (auto iter = list.begin(); iter != list.end(); ++iter) {
		result.push_back(*iter);
	}
	return std::move(result);
}
void trimSql(std::string& sql);

double getBestQuoteDeviate(const std::string& quote_type, const std::string& cdcYield, double price, int quoteSide);

// 获取进程PID
int GetPid();
#ifndef PROTO_HAS_SET
#define PROTO_HAS_SET(srcObj, srcValName, dstVal) if((srcObj).has_##srcValName()) { dstVal = (srcObj).srcValName(); }
#endif

template<typename T>
T ss_lexical_cast(const std::string& orgval, const T& defaultVal) {
	try {
		return boost::lexical_cast<T>(orgval);
	} catch (...) {
		return defaultVal;
	}
}

// 字节大小格式化可读性
std::string bytesFormat(size_t size);
// 线程睡眠秒数
void sleepSecond(int second);
// 得到当前进程内存，仅作为调试使用
std::string getFormatRss();

std::string getHeadOfGoodsCode(std::string gc);
bool isDigits(const std::string &str);
size_t wordOccurrenceCount(std::string const & str, std::string const & word);

std::string getLocalIpAddress(const std::string &slash = ";");
bool releaseFreeMemory();
class MemoryGuard
{
public:
	MemoryGuard();
	~MemoryGuard();
};

std::string GetMaturityTerms(int iStart, int iEnd, bool cvt2Days);
double GetMaturityTermsReal(int iStart, int iEnd);

std::string cvtDateToCalcDate(const std::string& date, bool useDefaultCurrentDate = false);
bool isVectorStringEqual(std::vector<std::string> &left, std::vector<std::string> &right);

namespace idbjson {
	std::string GenerateJson(const boost::property_tree::ptree& pt, bool isSubList = false);
	void write_json(std::basic_ostream<boost::property_tree::ptree::key_type::value_type> &stream, const boost::property_tree::ptree &pt, bool pretty);
	void write_json_helper(std::basic_ostream<boost::property_tree::ptree::key_type::value_type> &stream, const boost::property_tree::ptree &pt, int indent, bool pretty);
	std::basic_string<char> create_escapes(const std::basic_string<char> &s);
}

// 延迟执行
template<class T>
class AutoDelete : public boost::noncopyable {
public:
	AutoDelete(T *t) : t_(t) {}
	~AutoDelete() { if (t_) delete t_; }
	T* operator->() const { return t_; }
	operator bool() const { return !!t_; }
	bool operator!() const { return !t_; }
	T* get() const { return t_; }

private:
	T *t_;
};

#endif