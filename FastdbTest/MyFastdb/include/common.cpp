#include "fid.h"
#include "common.h"
#include "constants.h"
#include "sdbus/expr.h"
#include "sdbus/codec.h"
#include "sdbus/zip.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <stack>
#include <vector>
#include <time.h>
#include <ctime>
#include <chrono>
#include <algorithm>
#include <malloc.h>
#include <mutex>
#include <thread>
#ifdef WIN32
#include <boost/asio.hpp>
#include <boost/asio/io_service.hpp>
#else
#include <stdio.h>
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#endif

/*

#include "boost/locale/encoding.hpp"*/
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/locale/encoding.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>  
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp> 
#include <boost/algorithm/string/replace.hpp>
#include <boost/thread/mutex.hpp>
#include "logger.h"
#include "process_memory.h"

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif
#include "config.h"
#include "IDBDefine.h"

static const boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));
static const std::string _DATE_FORMAT = "%Y-%m-%d";
static boost::uuids::random_generator rg;
static boost::mutex rg_mutex;

std::string GetNewGuid() {
	boost::mutex::scoped_lock lock(rg_mutex);
	boost::uuids::uuid u = rg();
	std::string result = boost::lexical_cast<std::string>(u);
	result = boost::replace_all_copy(result, "-", "");
	/*std::string result;
	result.reserve(36);

	std::size_t i=0;
	for (boost::uuids::uuid::const_iterator it_data = u.begin(); it_data != u.end(); ++it_data) {
	const size_t hi = ((*it_data) >> 4) & 0x0F;
	result += boost::uuids::detail::to_char(hi);

	const size_t lo = (*it_data) & 0x0F;
	result += boost::uuids::detail::to_char(lo);
	}
	*/
	return result;
}

std::string& Trim(std::string &s) {
	if (s.empty()) {
		return s;
	}

	s.erase(0, s.find_first_not_of(" "));
	s.erase(s.find_last_not_of(" ") + 1);
	return s;
}

void split(const std::string& s, const std::string& delim, std::vector< std::string >* ret) {
	size_t last = 0;
	size_t index = s.find_first_of(delim, last);
	while (index != std::string::npos) {
		ret->push_back(s.substr(last, index - last));
		last = index + 1;
		index = s.find_first_of(delim, last);
	}
	if (index - last > 0) {
		ret->push_back(s.substr(last, index - last));
	}
}

std::vector<int> _PARSE_FID_LIKE = { // for contains
	FID_BOND_SHORT_NAME, FID_BOND_CODE, FID_ACROSS_MARKET_BOND_CODES, FID_PIN_YIN, FID_PIN_YIN_FULL, FID_DEAL_NO, FID_ISSUE_INSTITUTION,
	FID_ISSUE_INSTITUTION_SHORT, FID_ISSUE_INSTITUTION_PINYIN, FID_ISSUE_INSTITUTION_PINYINFULL,
	FID_INSTITUTION_NAME, FID_BID_INSTITUTION_NAME, FID_BID_INSTITUTION_CODE, FID_OFR_INSTITUTION_NAME, FID_OFR_INSTITUTION_CODE,
	FID_TRADER_NAME, FID_TRADER_FCODE, FID_BID_TRADER_NAME, FID_BID_TRADER_CODE, FID_OFR_TRADER_NAME, FID_BID_TRADER_CODE,
	FID_BROKER_NAME, FID_BID_BROKER_NAME, FID_OFR_BROKER_NAME, 
	FID_BID_BROKER_NAME_A, FID_BID_BROKER_NAME_B, FID_BID_BROKER_NAME_C, FID_BID_BROKER_NAME_D,
	FID_OFR_BROKER_NAME_A, FID_OFR_BROKER_NAME_B, FID_OFR_BROKER_NAME_C, FID_OFR_BROKER_NAME_D
};

std::vector<int> _PARSE_FID_NOQUOTE = { // for int bool double
	FID_QUOTE_SIDE,
	FID_TIME_TO_MATURITY_REAL,
	FID_MATURITY,
	FID_MATURITY_HOLIDAYS,
	FID_MATURITY_HOLIDAYS_SSE,
	FID_MATURITY_HOLIDAYS_SZE,
	FID_FILTER_MATURITY,
	FID_BID_OFR_SPREAD,
	FID_CDC_OFFSET,
	FID_LISTED_DATE,
	FID_BOND_IS_EXPIRED,
	FID_ISSUE_YEAR,
	FID_DURATION_FILTER,
	FID_PRICE,
	FID_BID_PRICE,
	FID_OFR_PRICE,
	FID_BID_QUOTE_COUNT,
	FID_OFR_QUOTE_COUNT,
	FID_BID_FLAG_URGENT,
	FID_OFR_FLAG_URGENT,
	FID_BEST_UNION_BID_URGENT,
	FID_BEST_UNION_OFR_URGENT
};

std::vector<int> _PARSE_FID_ARRAY = { // for dbarray
	FID_TRADER_ID_LIST, FID_BEST_BID_TRADER_ID_LIST, FID_BEST_OFR_TRADER_ID_LIST, FID_BROKER_ID_LIST, FID_BID_BROKER_ID_LIST, FID_BID_EXTERNAL_BROKER_ID_LIST, FID_OFR_BROKER_ID_LIST, FID_GUARANTEE_INSTITUTE_CODE
};

namespace {
	std::string ParseConditionNew(const sdbus::string& cond, const std::map<int, std::string>& fid_mp) {
		// 功能:将IDBBond传递过来的过滤条件转化为SQL能接受的where条件表达式
		// 说明:过滤条件中,名称都是数字id,声明在fid.h中,通过名称表fid_mp,将其转化为名称
		//      过滤条件是由原子表达式通过and/or连接起来构成一个括号表达式()
		//		原子表达式格式: 名称 操作符 取值
		//		IDBBond支持的操作符只有6种:=,!=,<,>,<=,>=
		typedef std::map<int, std::string> FidNameMap;
		typedef sdbus::AtomExpression AtomExpression;
		typedef sdbus::GroupExpression GroupExpression;
		typedef sdbus::Expression Expression;
		class SqlTransformer {
		public:
			static std::string ParseAtom(const AtomExpression* atom, int fid, const std::string& name, const sdbus::StringType& value) {
				// value 是解析出来的原始字符串值,原先的首尾引号基于部分原因没有移除,在这里要进行简单检测
				//		 如果有双引号,要改成单引号
				// 功能:将解析好的atom原子表达式转化成SQL所需要的where表达式之中的子表达式
				// 疑问:contains转化可能存在问题
				std::string str;
				std::string val;
				int nvalue = (int)value.length();
				if (nvalue >= 2 && (value[0] == '\'' || value[0] == '\"') && (value[nvalue - 1] == '\'' || value[nvalue - 1] == '\"'))
					val = std::string(value.data() + 1, value.length() - 2);
				else
					val = value;

				if (std::find(_PARSE_FID_LIKE.cbegin(), _PARSE_FID_LIKE.cend(), fid) != _PARSE_FID_LIKE.cend()) {
					// 处理模糊匹配
					//st.push(std::string("") + it->second + " like '%" + value + "%'");
					str += name + " contains '" + val + "'";
				} else if (std::find(_PARSE_FID_NOQUOTE.cbegin(), _PARSE_FID_NOQUOTE.cend(), fid) != _PARSE_FID_NOQUOTE.cend()) {
					// 处理int bool double
					str += name + " " + atom->GetComp() + val;
				} else if (std::find(_PARSE_FID_ARRAY.cbegin(), _PARSE_FID_ARRAY.cend(), fid) != _PARSE_FID_ARRAY.cend()) {
					str += std::string("\'") + val + "' in " + name;//以数组类型来存储的字段
				} else if (fid == FID_RELATED_MSG) {
					if (val == "Y") {
						str += "length(" + name + ")>0";
					} else if (val == "N") {
						str += "length(" + name + ")=0";
					} else {
						str += name + " contains " + "'" + val + "'";
					}
				}
				else if (fid == FID_BID_DESCRIPTION || fid == FID_OFR_DESCRIPTION)
				{
					str += name + " contains " + "'" + val + "'";
				}
				//else if (fid == FID_DCS_SEQ){
				//	str += name + " = " + val;
				//}
				else if (fid == FID_URGENT_STATUS)
				{
					str += name + " = " + val;
				}
				else {
					// 处理字符串
					str += name + " " + atom->GetComp() + " '" + val + "'";
				}
				return str;
			}
			static bool ParseGroup(std::string& str, const GroupExpression* parent, const FidNameMap& fid_mp) {
				int nadded = 0;//>0才能表明是解析成功,str才是有效的
				int ncount = parent->size();
				for (int i = 0; i < ncount; i++) {
					const Expression* expr = parent->at(i);
					if (expr->GetType() == sdbus::kExprTypeGroup) {
						std::string grpstr;
						const GroupExpression* group = (const GroupExpression*)expr;
						if (ParseGroup(grpstr, group, fid_mp)) {
							if (nadded != 0)
								str += expr->GetLink() == sdbus::kExprLinkOr ? " or " : " and ";
							str += grpstr;
							nadded++;
						}
					} else if (expr->GetType() == sdbus::kExprTypeAtom) {
						const AtomExpression* atom = (const AtomExpression*)expr;
						int id = atoi(atom->GetName().c_str());
						FidNameMap::const_iterator it = fid_mp.find(id);
						if (it == fid_mp.end()) {
							continue;
						}
						if (nadded != 0)//为0表示()之内的第一个,此时不需要连接符:and,or
							str += expr->GetLink() == sdbus::kExprLinkOr ? " or " : " and ";
						const sdbus::StringType& value = atom->GetValue();
						str += ParseAtom(atom, id, it->second, value);
						nadded++;
					}
				}
				if (parent->GetParent() != 0)
					str = "(" + str + ")";
				return nadded > 0;
			}
			static int GetAtomCount(const GroupExpression* parent) {
				// 递归获取总原子表达式的个数,原子表达式即 name = value 的简单表达式
				// 递归只是简单计数,且层次不会很深,通常都只有1-4层
				int ncount = 0;
				if (parent == 0)
					return ncount;
				for (int i = 0; i < parent->size(); i++) {
					const Expression* expr = parent->at(i);
					if (expr->GetType() == sdbus::kExprTypeGroup) {
						const GroupExpression* group = (const GroupExpression*)expr;
						ncount += GetAtomCount(group);
					} else if (expr->GetType() == sdbus::kExprTypeAtom) {
						ncount++;
					}
				}
				return ncount;
			}
		};
		if (cond.length() <= 0)
			return "";
		std::string str = "";
		sdbus::SqlExpression parser;
		const GroupExpression* root = parser.Parse(cond.c_str(), (int)cond.length());
		if (root) {
			if (SqlTransformer::GetAtomCount(root) > 1500) {
				LOGGER_WARN("ParseCondition failed, condition num exceeds limit. cond: " << cond);
				return "";//当客户端查询条件太多时，服务端拒绝服务
			}
			if (SqlTransformer::ParseGroup(str, root, fid_mp))
				return str;
		}
		LOGGER_ERROR("ParseCondition failed, cond: " << cond);
		return "";
	}
}

std::string ParseCondition(const sdbus::string& cond, int cond_fid) {
	if (cond.length() == 0) {
		return "";
	}

	char name[128] = { 0 };
	sprintf(name, "%d", cond_fid);
	sdbus::SqlExpression parser;
	const sdbus::AtomExpression* atom = parser.ParseFirstName(cond.c_str(), (int)cond.length(), name);
	if (atom)
		return atom->GetValue().c_str();
	LOGGER_ERROR("ParseCondition failed, id: " << cond_fid << ", cond: " << cond);
	return "";
}

std::string ParseCondition(const sdbus::string& cond, const std::map<int, std::string>& fid_mp) {
	if (cond.length() == 0) {
		return "";
	}
	return  ParseConditionNew(cond, fid_mp);
}

std::string ParseSortBy(const sdbus::string& sortby, const std::map<int, std::string>& fid_mp, const std::string& sortby_default) {
	if (sortby.length() == 0) {
		if (sortby_default.length() > 0) {
			return "order by " + sortby_default;
		} else {
			return "";
		}
	}

	std::string sort_str;
	std::vector<std::string> columns = split(sortby.c_str(), ',');
	int cnt = 0;
	if (columns.size() > 0) {
		for (size_t i = 0; i < columns.size(); ++i) {
			std::string column = columns[i];
			Trim(column);
			std::vector<std::string> tokens = split(column, ' ');
			if (tokens.size() == 2) {
				int fid = atoi(tokens[0].c_str());
				std::map<int, std::string>::const_iterator it = fid_mp.find(fid);
				if (it != fid_mp.end()) {
					if (cnt > 0) {
						sort_str += ", ";
					} else {
						sort_str += "order by ";
					}

					std::vector<std::string> fieldnames = split(it->second, '|');
					for (size_t j = 0; j < fieldnames.size(); j++) {
						if (j > 0)
							sort_str += ", ";
						sort_str += fieldnames[j] + " " + tokens[1];
					}

					cnt++;
				}
			}
		}
	}

	return sort_str;
}

std::string ParseRange(const sdbus::string& range) {
	if (range.length() == 0) {
		return "";
	}

	std::string range_str = std::string("limit ") + range.c_str();

	return range_str;
}

void ParseRange(const sdbus::string& range, int& start, int& cnt) {
	std::vector<std::string> vec = split(range.c_str(), ',');

	if (vec.size() >= 2) {
		start = atoi(Trim(vec[0]).c_str());
		cnt = atoi(Trim(vec[1]).c_str());
	}
}

std::string ParseParam(const sdbus::string& cond, 
	const sdbus::string& sortby, 
	const sdbus::string& range,
	const std::map<int, std::string>& fid_mp, 
	const std::string& sortby_default) 
{
	std::string cond_str = ParseCondition(cond, fid_mp);
	
	if (cond_str.empty() && !cond.empty()) 
	{
		// 在正常解析失败后，取出or括号中的字符串单独解析，然后拼接起来
		std::string str = std::string(cond.c_str());
		std::string lowStr = str;
		std::transform(lowStr.begin(), lowStr.end(), lowStr.begin(), ::tolower);
		std::string searchKey = "or (";
		int start = lowStr.find(searchKey);
		if (start == -1) 
		{
			searchKey = "or(";
			start = lowStr.find(searchKey);
		}
		
		int end = str.find_last_of(')');
		if (end > start && start >= 0) {
			std::string str1 = str.substr(0, start);
			std::string str2 = str.substr(start + searchKey.length(), end - start - searchKey.size());
			std::string str3 = str.substr(end + 1);
			str1 = ParseCondition(str1, fid_mp);
			str2 = ParseCondition(str2, fid_mp);
			str3 = ParseCondition(str3, fid_mp);
			cond_str = str1 + searchKey + str2 + ")" + str3;
		}
	}

	std::string sort_str = ParseSortBy(sortby, fid_mp, sortby_default);
	std::string range_str = ParseRange(range);

	std::string str;
	if (cond_str.length() > 0) {
		str += cond_str;
	}
	if (sort_str.length() > 0) {
		if (str.length() > 0) {
			str += " ";
		}
		str += sort_str;
	}
	if (range_str.length() > 0) {
		if (str.length() > 0) {
			str += " ";
		}
		str += range_str;
	}

	if (str.length() > 512) {
		LOGGER_DEBUG("query: " << str.substr(0, 512));
	} else {
		LOGGER_DEBUG("query: " << str);
	}

	return str;
}

int ParseRatingStr(const std::string& rating_str) {
	if (rating_str.length() == 0) {
		return 0;
	}

	int rating = 1;

	const char *p = rating_str.c_str();

	while (*p) {
		char c = *p++;
		switch (c) {
		case 'A':
		case 'B':
		case 'C':
			rating *= 3;
			break;
		case '+':
			rating += 1;
			break;
		case '-':
		case '1':
			rating -= 1;
			break;
		default:
			break;
		}
	}

	switch (rating_str.at(0)) {
	case 'A':
		rating *= 100;
		break;
	case 'B':
		rating *= 10;
		break;
	case 'C':
		rating *= 1;
		break;
	default:
		break;
	}

	return rating;
}

//add by Samuel For Parse rate type
std::string ParseRateType(std::string &FRN_Index_ID, int Next_Coupon_Date, int MaturityDate) 
{
	if (FRN_Index_ID.empty()) 
	{ //  || 0 == Next_Coupon_Date || Next_Coupon_Date == MaturityDate
		return "FIXED";
	} 
	else 
	{
		std::vector<std::string> strs = split(FRN_Index_ID, '_');
		std::string rate_type_first, rate_type_second;
		if (strs.size() > 1) 
		{
			rate_type_first = strs.at(0);
			rate_type_second = strs.at(1);
		} 
		else if (strs.size() == 1) 
		{
			rate_type_first = strs.at(0);
		}

		if (rate_type_first == "B2W" || rate_type_first == "B") 
		{
			return "REPO";
		} 
		else if (rate_type_first == "DEPO") 
		{
			return "DEPO";
		} 
		else if (rate_type_first == "SHIBOR") 
		{
			return "SHIBOR";
		} 
		else if (rate_type_first == "LIBOR") 
		{
			return "LIBOR";
		} 
		else if (rate_type_first == "LRB") 
		{
			return "LRB";
		} 
		else if (rate_type_first == "USD" && rate_type_second == "LIBOR") 
		{
			return "USD_LIBOR";
		}
		else if (rate_type_first == "LPR")
		{
			return "LPR";
		}
	}

	return "";
}

int GetBondSmallTermDays(int maturity_date, int interest_start_date) {
	int days_now_to_maturity;

	struct tm tm_maturity, tm_interest_start, tm_today;
	time_t maturity, interest_start, today;
	time_t lt = time(NULL);
    struct tm tmTemp;
    struct tm *local = LocalTimeSafe(lt, tmTemp);

	memset(&tm_maturity, 0, sizeof(struct tm));
	memset(&tm_interest_start, 0, sizeof(struct tm));
	memset(&tm_today, 0, sizeof(struct tm));

	tm_maturity.tm_year = maturity_date / 10000 - 1900;
	tm_maturity.tm_mon = (maturity_date % 10000) / 100 - 1;
	tm_maturity.tm_mday = (maturity_date % 10000) % 100;

	tm_interest_start.tm_year = interest_start_date / 10000 - 1900;
	tm_interest_start.tm_mon = (interest_start_date % 10000) / 100 - 1;
	tm_interest_start.tm_mday = (interest_start_date % 10000) % 100;

	tm_today.tm_year = local->tm_year;
	tm_today.tm_mon = local->tm_mon;
	tm_today.tm_mday = local->tm_mday;

	maturity = mktime(&tm_maturity);
	interest_start = mktime(&tm_interest_start);
	today = mktime(&tm_today);

	if (difftime(today, interest_start) < 0) {
		days_now_to_maturity = (int)difftime(maturity, interest_start) / (24 * 60 * 60) + 1;
	} else {
		days_now_to_maturity = (int)difftime(maturity, today) / (24 * 60 * 60);
	}

	return days_now_to_maturity;
}

int GetDayDifference(int from_date, int to_date) {
	struct tm tm_from, tm_to;
	time_t date_from, date_to;
	time_t lt = time(NULL);

	memset(&tm_from, 0, sizeof(struct tm));
	memset(&tm_to, 0, sizeof(struct tm));

	tm_from.tm_year = from_date / 10000 - 1900;
	tm_from.tm_mon = (from_date % 10000) / 100 - 1;
	tm_from.tm_mday = (from_date % 10000) % 100;

	tm_to.tm_year = to_date / 10000 - 1900;
	tm_to.tm_mon = (to_date % 10000) / 100 - 1;
	tm_to.tm_mday = (to_date % 10000) % 100;

	date_from = mktime(&tm_from);
	date_to = mktime(&tm_to);

	int days_now_to_maturity = (int)difftime(date_to, date_from) / (24 * 60 * 60) + 1;

	return days_now_to_maturity;
}

bool isLeapYear(int year) {
	return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

double GetBondSmallTermYears(int maturity_date) {
	struct tm tm_maturity, tm_today, tm_next;
	time_t maturity, today, next;
	time_t lt = time(NULL);
    struct tm tmTemp;
    struct tm *local = LocalTimeSafe(lt, tmTemp);

	memset(&tm_maturity, 0, sizeof(struct tm));
	memset(&tm_today, 0, sizeof(struct tm));
	memset(&tm_next, 0, sizeof(struct tm));

	tm_maturity.tm_year = maturity_date / 10000 - 1900;
	tm_maturity.tm_mon = (maturity_date % 10000) / 100 - 1;
	tm_maturity.tm_mday = (maturity_date % 10000) % 100;

	tm_today.tm_year = local->tm_year;
	tm_today.tm_mon = local->tm_mon;
	tm_today.tm_mday = local->tm_mday;

	// 从到期日依次向前推，直到下一年度
	tm_next.tm_year = (tm_maturity.tm_mon < tm_today.tm_mon || (tm_maturity.tm_mon == tm_today.tm_mon && tm_maturity.tm_mday < tm_today.tm_mday)) ? tm_today.tm_year + 1 : tm_today.tm_year;
	tm_next.tm_mon = tm_maturity.tm_mon;
	tm_next.tm_mday = tm_maturity.tm_mday;

	maturity = mktime(&tm_maturity);
	today = mktime(&tm_today);
	next = mktime(&tm_next);

	//if (tm_last.tm_mday == 29 && tm_last.tm_mday == 1 && !isLeapYear(tm_last.tm_year + 1900)) {
	//    tm_last.tm_mday = 28;
	//}
	int n = tm_maturity.tm_year - tm_next.tm_year;
	if (n < 0) n = 0;

	int check_year = tm_next.tm_mon < 2 ? tm_next.tm_year - 1 : tm_next.tm_year;
	if (isLeapYear(check_year + 1900)) {
		return n + difftime(next, today) / (366 * 24 * 60 * 60);
	} else {
		return n + difftime(next, today) / (365 * 24 * 60 * 60);
	}
}

double n_hundred(int n) {
	double sum = 1.0;
	for (int i = 1; i <= n; ++i)
		sum = sum * 10;
	return sum;
}

double Round(double t, int n) {
	/* //如果要求保留t小数点后n位数字的话
	int ival = (int)(t * n_hundred(n));
	//看小数点后第n+1位数字是否大于5来进行四舍五入
	int temp = (int)(t * n_hundred(n + 1)) % 10;
	if (temp >= 5)
		ival = ival + 1;
	double dval = ival / n_hundred(n);
	return dval; */
	double off = std::pow(10.0, n);
	double ret = std::round(t*off) / off;
	return ret;
}

// 小数点后至少显示min位, 至多max位
std::string Format(double t, int min, int max) {
	if (doubleEqual(t, DOUBLE_NULL)) {
		return "";
	}
	int precision = min;
	std::ostringstream ostr;
	if (max != -1) {
		ostr << std::setiosflags(std::ios::fixed) << std::setprecision(max) << t;
		std::string str = ostr.str();
		str = str.substr(0, str.find_last_not_of('0') + 1);
		std::vector<std::string> vec = split(str, '.');

		if (vec.size() >= 2) {
			std::string float_str = vec[1];
			if (float_str.length() > min) {
				precision = float_str.length();
			}
		}
	}

	std::ostringstream oret;
	oret << std::setiosflags(std::ios::fixed) << std::setprecision(precision) << t;
	return oret.str();
}

std::string truncateDouble(double value, const int& percision) {
	int default_precision = 4;
	int int_precision = boost::lexical_cast<int>(percision);
	int delta_precision = default_precision;
	if (int_precision < default_precision) delta_precision = int_precision;

	std::string double_str = Format(value, 0, percision);
	if (double_str.empty() || ("" == double_str)) return double_str;

	std::string ret = double_str;
	if (boost::algorithm::contains(double_str, ".")) {
		int length = double_str.length();
		int valid_len = -1;
		int start = length - 1, end = length - delta_precision;
		for (int i = start; i >= end; --i) {
			if ('0' != double_str.at(i)) {
				valid_len = i + 1;
				break;
			}
		}
		if (-1 == valid_len) valid_len = length - delta_precision;
		ret = double_str.length() > 1 && valid_len < (double_str.length() - 1) ? double_str.substr(0, valid_len) : double_str;
	}
	if (boost::algorithm::ends_with(ret, ".")) {
		ret = ret.substr(0, ret.length() - 1);
	}
	return ret;
}

std::string GetTimeToMaturity(int maturity_date, int interest_start_date, const std::string& option_type_client) {
	std::vector<std::string> option_array = split(option_type_client.c_str(), '+');
	std::ostringstream ostr;

	if (maturity_date == 0) {
		return "";
	}

	int days_now_to_maturity = GetBondSmallTermDays(maturity_date, interest_start_date);
	double years_to_maturity = GetBondSmallTermYears(maturity_date);
	double year, option_year;

	if (option_array.size() < 2) {
		if (days_now_to_maturity < 365) {
			ostr << days_now_to_maturity << "D";
		} else {
			ostr << Round(years_to_maturity, 2) << "Y";
		}
	} else {
		year = days_now_to_maturity / 365.0;
		option_year = atof(option_array[1].c_str());
		if (year > option_year) {
			year = years_to_maturity - option_year;
			ostr << Round(year, 2) << "Y+" << Round(atof(option_array[1].c_str()), 2) << "Y";
		} else {
			if (days_now_to_maturity < 365) {
				ostr << days_now_to_maturity << "D";
			} else {
				ostr << Round(years_to_maturity, 2) << "Y";
			}
		}
	}

	return ostr.str();
}

double GetTimeToMaturityReal(int maturity_date, int interest_start_date, const std::string& option_type_client) {
	std::vector<std::string> option_array = split(option_type_client.c_str(), '+');

	int days_now_to_maturity = GetBondSmallTermDays(maturity_date, interest_start_date);
	double years_to_maturity = GetBondSmallTermYears(maturity_date);
	double year, option_year;

	if (option_array.size() < 2) {
		if (days_now_to_maturity < 365) {
			return days_now_to_maturity / 365.0;
		} else {
			return Round(years_to_maturity, 2);
		}
	} else {
		year = days_now_to_maturity / 365.0;
		option_year = atof(option_array[1].c_str());
		if (year > option_year) {
			year = years_to_maturity - option_year;
			return Round(year, 2);
		} else {
			if (days_now_to_maturity < 365) {
				return days_now_to_maturity / 365.0;
			} else {
				return Round(years_to_maturity, 2);
			}
		}
	}
}

double GetTimeToMaturity(int maturity_date, int interest_start_date) {
	if (maturity_date == 0) {
		return 0;
	}

	int days_now_to_maturity = GetBondSmallTermDays(maturity_date, interest_start_date);

	return days_now_to_maturity / 365.0;
}

int GetMaturityTermInDays(int end_date, std::pair<std::string, std::string>& maturity_term_pair) {
	std::string today = GetTDateString("%Y%m%d");
	int days_to_maturity = GetDayDifference(std::atoi(today.c_str()), end_date);

	std::ostringstream remaining_maturity_term;
	std::ostringstream term_unit;

	if (days_to_maturity > 365) {
		char buf[10];
		sprintf(buf, "%2f", days_to_maturity / 365.0);
		remaining_maturity_term << std::fixed << std::setprecision(2) << days_to_maturity / 365.0;
		term_unit << "Y";
	} else {
		remaining_maturity_term << days_to_maturity;
		term_unit << "D";
	}

	maturity_term_pair.first = remaining_maturity_term.str();
	maturity_term_pair.second = term_unit.str();

	return days_to_maturity;
}

std::pair<std::string, std::string> CalculateMaturityTerm(int maturity_date, int option_date) {

	std::pair<std::string, std::string> maturity_term_pair;
	if (option_date == 0) {
		int days_to_maturity = GetMaturityTermInDays(maturity_date, maturity_term_pair);
	} else {
		int days_to_maturity = GetMaturityTermInDays(option_date, maturity_term_pair);
		if (days_to_maturity < 0) {
			GetMaturityTermInDays(maturity_date, maturity_term_pair);
		}
	}

	return maturity_term_pair;
}

std::string GetMaturityTermInDays(int start_date, int end_date) {
	int days_to_maturity = GetDayDifference(start_date, end_date);
	std::stringstream ss;
	if (days_to_maturity > 365) {
		char buf[10];
		sprintf(buf, "%2f", days_to_maturity / 365.0);
		ss << std::fixed << std::setprecision(2) << days_to_maturity / 365.0;
		ss << "Y";
	} else {
		ss << days_to_maturity;
		ss << "D";
	}
	return ss.str();
}

std::string CheckOnTheRun(int listed_date) {
	if (listed_date == 0) {
		return "N";
	}
	
	struct tm tm_listed, tm_today;
	time_t listed, today;
	time_t lt = time(NULL);
    struct tm tmTemp;
    struct tm *local = LocalTimeSafe(lt, tmTemp);

	memset(&tm_listed, 0, sizeof(struct tm));
	memset(&tm_today, 0, sizeof(struct tm));

	tm_listed.tm_year = listed_date / 10000 - 1900;
	tm_listed.tm_mon = (listed_date % 10000) / 100 - 1;
	tm_listed.tm_mday = (listed_date % 10000) % 100;

	tm_today.tm_year = local->tm_year;
	tm_today.tm_mon = local->tm_mon;
	tm_today.tm_mday = local->tm_mday;

	listed = mktime(&tm_listed);
	today = mktime(&tm_today);

	//2020/07/10 by david.zhang:time_t本质是int64,
	//所以在超出int范围的时候，转换时会出现溢出,故将类型改为double
	double diff = difftime(today, listed);
	if (diff >= idb::precision.PRECISION4 && diff < 5.0000 * 24 * 60 * 60)
		return "Y";

	return "N";
}

time_t ParseTimeString(const std::string &time_str) {
	struct tm local;
	memset(&local, 0, sizeof(struct tm));

	if (sscanf(time_str.c_str(), "%4d-%2d-%2d %2d:%2d:%2d", &local.tm_year, &local.tm_mon, &local.tm_mday, &local.tm_hour, &local.tm_min, &local.tm_sec) == 6) {
		local.tm_year -= 1900;
		local.tm_mon -= 1;
		return mktime(&local);
	}

	return -1;
}

time_t ParseMaturityDateString(const std::string &date_str, const char* format) {
	struct tm local;
	memset(&local, 0, sizeof(struct tm));

	if (sscanf(date_str.c_str(), format, &local.tm_year, &local.tm_mon, &local.tm_mday) == 3) {
		local.tm_year -= 1900;
		local.tm_mon -= 1;
		local.tm_hour = 0;
		local.tm_min = 0;
		local.tm_sec = 0;
		return mktime(&local);
	}

	return -1;
}

time_t ParseModifyTime(const std::string &modifytime)
{
    int pos = modifytime.find_last_of('.');
    if (pos > 0) {
        time_t t = ParseTimeString(modifytime.substr(0, pos));
        if (t > 0) {
            return t * 1000 + ss_lexical_cast(modifytime.substr(pos + 1), 0);
        }
    }
    return -1;
}

struct tm* LocalTimeSafe(time_t t, struct tm &local)
{
#ifdef WIN32
    // 目前的版本windows是线程安全的
    return localtime(&t);
#else
    // linux下localtime_r是线程安全的, localtime线程不安全
    return localtime_r(&t, &local);
#endif
}

std::string GetTimeString(time_t t) {
    if (t == -1) {
        return "";
    }

    struct tm tmTemp;
    struct tm *local = LocalTimeSafe(t, tmTemp);
    char timeChars[50];
    strftime(timeChars, 50, "%Y-%m-%d %H:%M:%S", local);
    return std::string(timeChars);
}

time_t GetCurrentTimeMilliSec() {
	boost::int64_t time64 = (boost::posix_time::microsec_clock::universal_time() - epoch).total_milliseconds();

	return time64;
}

time_t GetCurrentTimeMicroSec() {
	boost::int64_t time64 = (boost::posix_time::microsec_clock::universal_time() - epoch).total_microseconds();

	return time64;
}

std::string GetCurrentTimeString() {
	char   timeChars[50];
	time_t lt = time(NULL);
    struct tm tmTemp;
    struct tm *local = LocalTimeSafe(lt, tmTemp);

	strftime(timeChars, 50, "%Y-%m-%d %H:%M:%S", local);

	return std::string(timeChars);
}

std::string GetCurrentDateString() {
    char   timeChars[50];
    time_t lt = time(NULL);
    struct tm tmTemp;
    struct tm *local = LocalTimeSafe(lt, tmTemp);

    strftime(timeChars, 50, "%Y-%m-%d", local);

    return std::string(timeChars);
}

std::string GetCurrentTimeString_x_minutes_ago(int min) {
	using std::chrono::system_clock;
	std::time_t half_an_hour_ago = system_clock::to_time_t(system_clock::now() - std::chrono::minutes(min));
	char timeChars[50] = { 0 };
    struct tm tmTemp;
    strftime(timeChars, 50, "%Y-%m-%d %H:%M:%S", LocalTimeSafe(half_an_hour_ago, tmTemp));

	return std::string(timeChars);
}

std::string GetTime_x_seconds_latter(const std::string& strTime, int sec) {
	using std::chrono::system_clock;

	int year(0);
	int month(0);
	int day(0);
	int hour(0);
	int minute(0);
	int second(0);
	sscanf(strTime.c_str(), "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);

	std::tm tm_;
	tm_.tm_year = year - 1900;
	tm_.tm_mon = month - 1;
	tm_.tm_mday = day;
	tm_.tm_hour = hour;
	tm_.tm_min = minute;
	tm_.tm_sec = second;

	char timeChars[50] = { 0 };
	std::time_t x_sec_latter = system_clock::to_time_t(std::chrono::system_clock::from_time_t(mktime(&tm_)) + std::chrono::seconds(sec));
    struct tm tmTemp;
    strftime(timeChars, 50, "%Y-%m-%d %H:%M:%S", LocalTimeSafe(x_sec_latter, tmTemp));
	return std::string(timeChars);
}

std::string GetTodayDate() {
	using std::chrono::system_clock;
	std::time_t half_an_hour_ago = system_clock::to_time_t(system_clock::now() - std::chrono::minutes(30));
	char timeChars[50] = { 0 };
    struct tm tmTemp;
    strftime(timeChars, 50, "%Y-%m-%d 00:00:00", LocalTimeSafe(half_an_hour_ago, tmTemp));

	return std::string(timeChars);
}

std::string GetFullDateTimeString() {
	boost::posix_time::ptime time_(boost::posix_time::microsec_clock::local_time());
	const boost::posix_time::time_duration td = time_.time_of_day();
	const long hours = td.hours();
	const long minutes = td.minutes();
	const long seconds = td.seconds();
	const long milliseconds = td.total_milliseconds() - ((hours * 3600 + minutes * 60 + seconds) * 1000);
	char buf[10];
	sprintf(buf, ".%03ld", milliseconds);
	std::string s = GetCurrentTimeString() + std::string(buf);
	return s;
}

std::string GetDateTimeForMillisecond(time_t millisecond)
{
    char buf[32] = { 0 };
    sprintf(buf, "%03d", millisecond % 1000);
    return GetTimeString(millisecond / 1000) + "." + std::string(buf);
}

time_t yyyymmdd2time_t(const std::string yyyymmdd)
{
    struct tm t;
    memset(&t, 0, sizeof(t));
    sscanf(yyyymmdd.c_str(), "%04d-%02d-%02d", &t.tm_year, &t.tm_mon, &t.tm_mday);
    t.tm_year -= 1900;
    t.tm_mon -= 1;
    return mktime(&t);
}

std::string GetDateTimeForMillisecond()
{
    return GetDateTimeForMillisecond(GetCurrentTimeMilliSec());
}

std::string GetCurrentMinuteString() {
	char   timeChars[50];
	time_t lt = time(NULL);
    struct tm tmTemp;
    struct tm *local = LocalTimeSafe(lt, tmTemp);

	strftime(timeChars, 50, "%H:%M", local);

	return std::string(timeChars);
}

std::string GetCurrentHMSString() {
	char   timeChars[50];
	time_t lt = time(NULL);
    struct tm tmTemp;
    struct tm *local = LocalTimeSafe(lt, tmTemp);

	strftime(timeChars, 50, "%H:%M:%S", local);

	return std::string(timeChars);
}

std::string GetTDateString(time_t t, const char* format) {
	if (t <= 0) {
		return "";
	}

	char   dayChars[50];
    struct tm tmTemp;
    struct tm *local = LocalTimeSafe(t, tmTemp);
	strftime(dayChars, 50, format, local);

	return std::string(dayChars);
}

std::string GetTDateString(const char* format) {
	time_t lt = time(NULL);
	return GetTDateString(lt, format);
}

std::string GetT1DateString(const char* format) {
	time_t lt = time(NULL);
	lt += 24 * 3600;
	return GetTDateString(lt, format);
}

// month 0-11
static int GetDaysInMonth(int year, int month) {
	static int daysInMonths[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int days = daysInMonths[month];

	if (month == 1 && isLeapYear(year)) // February of a leap year
	{
		days += 1;
	}

	return days;
}

static tm SubtractMonths(const tm &d, int months) {
	int year = d.tm_year - months / 12;
	int month = d.tm_mon - months % 12;

	if (month < 0) {
		year -= 1;
		month += 12;
	}

	int daysInMonth = GetDaysInMonth(year, month);
	int day = d.tm_mday < daysInMonth ? d.tm_mday : daysInMonth;
	tm result = tm();
	memset(&result, 0, sizeof(result));
	result.tm_year = year;
	result.tm_mon = month;
	result.tm_mday = day;

	result.tm_hour = d.tm_hour;
	result.tm_min = d.tm_min;
	result.tm_sec = d.tm_sec;

	return result;
}

static tm  intToTm(int yyyymmdd) {
	struct tm t;
	memset(&t, 0, sizeof(t));
	t.tm_year = int(yyyymmdd / 10000) - 1900;
	t.tm_mon = (yyyymmdd / 100) % 100 - 1;
	t.tm_mday = yyyymmdd % 100;
	return t;
}

std::string GetBeforeMonthsString(const tm &d, const char* format, int months) {
	struct tm ret = SubtractMonths(d, months);
	char dayChars[50];
	strftime(dayChars, sizeof(dayChars), format, &ret);
	return std::string(dayChars);
}

std::string GetBeforeMonthsString(const char* format, int months) {
	time_t lt = time(NULL);
    struct tm tmTemp;
    struct tm *pNow = LocalTimeSafe(lt, tmTemp);
	return GetBeforeMonthsString(*pNow, format, months);
}

std::string GetBeforeMonthsString(int yyyymmdd, const char* format, int months) {
	struct tm t = intToTm(yyyymmdd);
	return GetBeforeMonthsString(t, format, months);
}


std::string GetBeforeDaysString(const char* format, int days) {
	time_t lt = time(NULL);
	lt = lt - days * 24 * 3600;
    struct tm tmTemp;
    struct tm *local = LocalTimeSafe(lt, tmTemp);
	char buf[128] = { 0 };
	strftime(buf, sizeof buf, format, local);
	return buf;
}

std::string GetBeforeMinuteString(int minute)
{
    time_t lt = time(NULL);
    lt = lt - minute * 60;
    struct tm tmTemp;
    struct tm *local = LocalTimeSafe(lt, tmTemp);
    char buf[128] = { 0 };
    strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", local);
    return buf;
}

int GetSortIndexByBondType(const std::string &bondtype) 
{
	static std::map<string, int> typeMapping = { { GBKToUTF8("国债"), 1 },
	{ GBKToUTF8("金融债"), 2 },
	{ GBKToUTF8("地方债"), 3 },
	{ GBKToUTF8("金融债(Shibor)"), 4 },
	{ GBKToUTF8("金融债(LPR)"), 5 },
	{ GBKToUTF8("金融债(Depo)"), 6 },
	{ GBKToUTF8("金融债(含权)"), 7 },	
	{ GBKToUTF8("金融债(Shibor含权)"), 8 },
	{ GBKToUTF8("金融债(LPR含权)"), 9 },
	{ GBKToUTF8("金融债(Depo含权)"), 10 },
	{ GBKToUTF8("央票"), 11 },
	{ GBKToUTF8("其他"), 12 } };

	int type = 12;

	std::map<string, int>::iterator it = typeMapping.find(bondtype);

	if (it != typeMapping.end()) 
		type = it->second;

	return type;
}

std::string GBKToUTF8(const std::string& strGBK) {
	return boost::locale::conv::between(strGBK, "UTF-8", "GBK");
}

std::string Utf8ToAnsi(const sdbus::string &strUTF8) {
	return boost::locale::conv::between(strUTF8, "GBK", "UTF-8");
}

std::string GetCompletedBondType(const std::string &bondtype, 
	const std::string &frnindexid, 
	const std::string &optiontype) 
{
	static std::string fb = GBKToUTF8("金融债");
	static std::string fbOption = GBKToUTF8("金融债(含权)");
	static std::string fbDepo = GBKToUTF8("金融债(Depo)");
	static std::string fbDepoOption = GBKToUTF8("金融债(Depo含权)");
	static std::string fbShibor = GBKToUTF8("金融债(Shibor)");
	static std::string fbShiborOption = GBKToUTF8("金融债(Shibor含权)");
	static std::string fbLpr = GBKToUTF8("金融债(LPR)");
	static std::string fbLprOption = GBKToUTF8("金融债(LPR含权)");

	if (bondtype != fb)
		return bondtype;

	if (-1 != frnindexid.find("DEPO")) 
	{
		if (!hasOptionType(optiontype))
			return fbDepo;
		else
			return fbDepoOption;
	} 
	else if (-1 != frnindexid.find("SHIBOR")) 
	{
		if (!hasOptionType(optiontype))
			return fbShibor;
		else
			return fbShiborOption;
	} 
	else if (-1 != frnindexid.find("LPR"))
	{
		if (!hasOptionType(optiontype))
			return fbLpr;
		else
			return fbLprOption;
	}
	else 
	{
		if (!hasOptionType(optiontype))
			return fb;
		else
			return fbOption;
	}
	return "";
}

bool hasOptionType(const std::string &optiontype) {
	if (optiontype.empty() || optiontype == "NON")
		return false;
	else
		return true;
}

std::string BuildTitle(const std::string&bond_type)
{
	if (bond_type == GBKToUTF8("金融债"))
	{
		return GBKToUTF8("金融债 固息");
	}
	else if (bond_type == GBKToUTF8("金融债(含权)"))
	{
		return GBKToUTF8("金融债 固息 含权");
	}
	else if (bond_type == GBKToUTF8("金融债(Shibor)"))
	{
		return GBKToUTF8("金融债 Shibor");
	}
	else if (bond_type == GBKToUTF8("金融债(Shibor含权)"))
	{
		return GBKToUTF8("金融债 Shibor 含权");
	}
	else if (bond_type == GBKToUTF8("金融债(Depo)"))
	{
		return GBKToUTF8("金融债 Depo");
	}
	else if (bond_type == GBKToUTF8("金融债(Depo含权)"))
	{
		return GBKToUTF8("金融债 Depo 含权");
	}
	else if (bond_type == GBKToUTF8("金融债(LPR)"))
	{
		return GBKToUTF8("金融债 LPR");
	}
	else if (bond_type == GBKToUTF8("金融债(LPR含权)"))
	{
		return GBKToUTF8("金融债 LPR 含权");
	}
	else
	{
		return bond_type;
	}
}

int CountChineseWords(const std::string& str) {
	int count = 0;
	std::string::const_iterator iter = str.begin();

	for (; iter < str.end(); ++iter) {
		char ch = *iter;
		bool isAlphabet = ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) ? true : false;
		bool isNumber = (ch >= '0' && ch <= '9') ? true : false;

		if (!isAlphabet && !isNumber) {
			count++;
		}
	}

	return count / 3;// 中文占用3个字符
}

void GetPriceStrings(double price, const std::string& fan_dian_flag, double fan_dian, int symbol, int price_type, std::string& price_string, std::string& fan_dian_string) {
	if (!doubleEqual(price, INVALIDVALUE)) // 价格不为空
	{
		price_string = Format(price, 2, 4);

		if (fan_dian_flag == "1") // 有返点
		{
			if (doubleEqual(fan_dian, INVALIDVALUE)) {
				fan_dian_string = fan_dian_string + "F--";
			} else {
				fan_dian_string = fan_dian_string + "F" + Format(fan_dian, 2, 4);
			}
		}
	}

	else // 价格为空
	{
		if (fan_dian_flag == "1") // 有返点
		{
			if (doubleEqual(fan_dian, INVALIDVALUE)) {
				price_string = price_string + GBKToUTF8("平价返");
			} else {
				price_string = price_string + "--";
				fan_dian_string = fan_dian_string + "F" + Format(fan_dian, 2, 4);
			}
		} else {
			if (price_type == 0) {
				price_string = price_string + (symbol == 1 ? "Bid" : "Ofr");
			} else {
				price_string = "--";
			}
		}
	}
}

// For the best quote
void GetBestPriceStrings(double price, const std::string &fan_dian_flag, double fan_dian, const std::string &quote_id, int quote_side, std::string &price_string, std::string &fan_dian_string) {
	if (!doubleEqual(price, INVALIDVALUE)) // 价格不为空
	{
		price_string = Format(price, 2, 4);

		if (fan_dian_flag == "1") // 有返点
		{
			if (doubleEqual(fan_dian, INVALIDVALUE)) {
				fan_dian_string = "F--";
			} else {
				fan_dian_string = "F" + Format(fan_dian, 2, 4);
			}
		}
	} else // 价格为空
	{
		if (fan_dian_flag == "1") // 有返点
		{
			if (doubleEqual(fan_dian, INVALIDVALUE)) {
				price_string = GBKToUTF8("平价返");
			} else {
				price_string = "--";
				fan_dian_string = "F" + Format(fan_dian, 2, 4);
			}
		} else {
			//quote_side 1 bid; -1 ofr
			if (quote_id.length() > 0) {
				price_string = quote_side == 1 ? "Bid" : "Ofr";
			}
		}
	}
}

std::string GetDcsDealStatusString(const std::string& deal_status, bool in_hand) {
	std::string status = kDcsDealtoBeConfirmString;

	if (deal_status == kDcsDealtoBeConfirm) {
		status = kDcsDealtoBeConfirmString;
	} else if (deal_status == kDcsDealtoBeSubmit) {
		status = kDcsDealtoBeSubmitString;
	} else if (deal_status == kDcsDealSubmitted) {
		status = kDcsDealSubmittedString;
	} else if (deal_status == kDcsDealPassed) {
		status = kDcsDealPassedString;
	} else if (deal_status == kDcsDealNoPass) {
		status = kDcsDealNoPassString;
	} else if (deal_status == kDcsDealDestroyed) {
		status = kDcsDealDestroyedString;
	} else if (deal_status == kDcsDealIntheReview) {
		status = kDcsDealIntheReviewString;
	} else if (deal_status == kDcsDealDeleted) {
		status = kDcsDealDeletedString;
	}

	if (in_hand) {
		status = kDcsDealInHandString;
	}

	return status;
}

std::string IntToString(int value) {
	std::stringstream ssValue;
	ssValue << value;

	return ssValue.str();
}

// TODO: use template
std::string FloatToString(float value) {
	std::stringstream ssValue;
	ssValue << value;

	return ssValue.str();
}


std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems) {
	std::stringstream ss(s);
	std::string item;
	while (getline(ss, item, delim)) {
		boost::algorithm::trim(item);
		elems.push_back(item);
	}
	return elems;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::vector<std::string> elems;
	split(s, delim, elems);
	return elems;
}

std::string join(const std::vector<std::string>& str_arr, const std::string& delim) {
	if (str_arr.size() == 0)
		return "";

	std::ostringstream ss;
	ss << str_arr[0];
	for (size_t i = 1; i < str_arr.size(); ++i)
		ss << delim << str_arr[i];

	return ss.str();
}

std::string joinSql(const std::vector<std::string>& str_arr) {
	if (str_arr.empty())
		return "";
	std::ostringstream ss;
	std::string delim = "'";
	ss << delim << str_arr[0] << delim;
	for (size_t i = 1; i < str_arr.size(); ++i)
		ss << "," << delim << str_arr[i] << delim;

	return ss.str();
}

std::string join(const std::set<std::string>& str_arr, const std::string& delim) {
	if (str_arr.size() == 0)
		return "";

	std::ostringstream ss;
	std::set<std::string>::const_iterator it = str_arr.cbegin();
	size_t count = 0;
	for (; it != str_arr.cend(); ++it) {
		ss << *it;
		count++;
		if (count < str_arr.size())
			ss << delim;
	}
	return ss.str();
}

char* allocString(const std::string& value) {
	char* str = new char[value.length() + 1];
	strcpy(str, value.c_str());
	return str;
}

void stringSetter(const char** property, const std::string& value) {
	clearStringProperty(property);
	*property = const_cast<const char*>(allocString(value));
}

char const* stringGetter(char const* property) {
	return property == NULL ? "" : property;
}

void clearStringProperty(const char** property) {
	if (*property != NULL) {
		delete *property;
	}
}

// 0-星期天 1-6-星期一~星期六 8-无效值
size_t getWeekDay(size_t year, size_t month, size_t day) {
	if ((year < 1) || (month < 1) || (day < 1)) {
		return 8;
	}
	if ((month > 12) || (day > 31)) {
		return 8;
	}

	if (month < 3) {
		month += 12;
		year--;
	}

	return (day + 1 + 2 * month + 3 * (month + 1) / 5 + year + year / 4 - year / 100 + year / 400) % 7;
}

std::string GetIpAddressFromString(const std::string &msg_string) {
	std::vector<std::string> ipaddress;
	boost::regex ip_pattern("(\\d{1, 3}.){3}\\d{1, 3}$");
	find_all_regex(ipaddress, msg_string, ip_pattern);
	if (ipaddress.size() == 0) {
		LOGGER_ERROR("BondLogin: Ip Address Error: " << msg_string)
			return "";
	}
	return ipaddress[0];
}

// zip sdbus msg
bool zipMessage(const sdbus::Message &msg, sdbus::Message &zip_msg) {
	bool ret = false;
	size_t len;
	if (!VariantMapCalcSize(msg.MapValue(), len)) {
		return false;
	}

	char* buffer = new char[len];
	if (buffer == NULL) {
		return false;
	}
	memset(buffer, 0, sizeof(char) * len);

	// serialize
	if (!VariantMapEncode(msg.MapValue(), buffer, sizeof(char) * len, len)) {
		delete[] buffer;
		return false;
	}

	int zip_len = len > 8096 ? len : 8096;
	char* zip_buffer = new char[zip_len];
	if (zip_buffer == NULL) {
		delete[] buffer;
		return false;
	}

	if (!sdbus::zip(buffer, len, zip_buffer, zip_len)) {
		delete[] buffer;
		delete[] zip_buffer;
		return false;
	}

	delete[] buffer;

	zip_msg.SetRaw(FID_ZIP_DATA, zip_buffer, zip_len);
	zip_msg.SetInt32(FID_UNZIP_LEN, len);
	zip_msg.SetInt32(FID_ZIP_LEN, zip_len);

	delete[] zip_buffer;
	return true;
}

bool unzipMessage(const sdbus::Message &zip_msg, sdbus::Message &msg) {
	bool ret = false;

	int len = 0;
	zip_msg.GetInt32(FID_UNZIP_LEN, len);

	if (len <= 0) {
		return false;
	}

	size_t zip_len = 0;
	const void * zip_buffer;
	zip_msg.RefRaw(FID_ZIP_DATA, zip_buffer, zip_len);

	if (zip_len == 0) {
		return false;
	}

	char* buffer = new char[len];
	if (buffer == NULL) {
		return false;
	}
	memset(buffer, 0, sizeof(char) * len);
	if (!sdbus::unzip((const char*)zip_buffer, zip_len, buffer, len)) {
		delete[] buffer;
		return false;
	}

	sdbus::VariantMap &mp = msg.MapValue();
	size_t used = 0;
	if (VariantMapDecode(mp, buffer, sizeof(buffer), used)) {
		msg.SetType(zip_msg.GetType());
		msg.SetSubject(zip_msg.GetSubject());
		msg.SetMessageID(zip_msg.GetMessageID());
        msg.SetMessageNumber(zip_msg.GetMessageNumber());
        msg.SetTotalMessages(zip_msg.GetTotalMessages());
		ret = true;
	}

	delete[] buffer;

	return ret;

}

bool encodeMessage(const sdbus::Message &msg, sdbus::Message &zip_msg, size_t &srcLen, size_t &dstLen)
{
    srcLen = 0;
    dstLen = 0;
    bool result = false;
    VariantMapCalcSize(msg.MapValue(), srcLen);
    if (srcLen > 1024) 
	{
        if (zipMessage(msg, zip_msg)) 
		{
            zip_msg.SetType(msg.GetType());
            zip_msg.SetSubject(msg.GetSubject());
            zip_msg.SetMessageID(msg.GetMessageID());
            zip_msg.SetMessageNumber(msg.GetMessageNumber());
            zip_msg.SetTotalMessages(msg.GetTotalMessages());
            if (VariantMapCalcSize(zip_msg.MapValue(), dstLen)) 
			{
                result = true;
            }
        }
    }
    return result;
}

void decodeMessage(const sdbus::Message &zip_msg, sdbus::Message &msg)
{
    if (!unzipMessage(zip_msg, msg)) {
        msg = zip_msg;
    }
}

bool isBCOCategoryType(const std::string& bond_category) {
	static std::string _BCO_CATEGORY_TYPES[] = { "BCO", "ABS", "NCD" };
	static const std::set<std::string> mySet(_BCO_CATEGORY_TYPES,
		_BCO_CATEGORY_TYPES + sizeof(_BCO_CATEGORY_TYPES) / sizeof(_BCO_CATEGORY_TYPES[0]));
	return mySet.count(bond_category) == 1 ? true : false;
}

bool isLocalGovType(const std::string& bond_category, const std::string& bond_type) {
	if (bond_category == "BNC" && bond_type == GBKToUTF8("地方债"))
		return true;
	return false;
}

std::string dateShortToLong(const std::string& shortDate) {
	if (shortDate.empty() || shortDate.length() < 8)
		return shortDate;

	char buf[20];
	sprintf(buf, "%c%c%c%c-%c%c-%c%c", shortDate[0], shortDate[1], shortDate[2], shortDate[3],
		shortDate[4], shortDate[5],
		shortDate[6], shortDate[7]);
	return std::string(buf);
}
const std::locale fmt(std::locale::classic(), new boost::gregorian::date_facet(_DATE_FORMAT.c_str()));
const std::locale fmt_input(std::locale::classic(), new boost::gregorian::date_facet(_DATE_FORMAT.c_str()));
std::string dateToStr(const boost::gregorian::date& date) {
	if (date.is_not_a_date()) {
		return "";
	}

	return GetTDateString(to_time_t(date), "%Y-%m-%d");
}
boost::gregorian::date strToDate(const std::string& dateStr) {
	if (dateStr.empty()) {
		return currentDate();
	}
	//std::stringstream ss(dateStr);
	//ss.imbue(fmt_input);
	boost::gregorian::date d = boost::gregorian::from_string(dateStr);
	//ss >> d;
	return d;
}

time_t to_time_t(const boost::gregorian::date& date) {
	using namespace boost::posix_time;
	static ptime epoch(boost::gregorian::date(1970, 1, 1));
	time_duration::sec_type secs = (ptime(date, seconds(0)) - epoch).total_seconds();
	return time_t(secs);
}

boost::gregorian::date currentDate() {
	boost::gregorian::date current_date(boost::gregorian::day_clock::local_day());
	return current_date;
}

boost::gregorian::date_duration dateDuration(const boost::gregorian::date& start, const boost::gregorian::date& end) {
	boost::gregorian::date_period dd(start, end);
	return dd.length();
}

boost::gregorian::date getNextWorkDate(const std::set<std::string>& holiday_date_sets, const boost::gregorian::date& date) {
	if (holiday_date_sets.empty() || date.is_not_a_date())
		return date;
	std::string str = dateToStr(date);
	if (holiday_date_sets.find(str) != holiday_date_sets.end()) {
		boost::gregorian::days d(1);
		return getNextWorkDate(holiday_date_sets, date + d);
	} else {
		return date;
	}
}

boost::gregorian::date getLastWorkDate(const std::set<std::string>& holiday_date_sets, const boost::gregorian::date& date) {
	if (holiday_date_sets.empty() || date.is_not_a_date())
		return date;
	std::string str = dateToStr(date);
	if (holiday_date_sets.find(str) != holiday_date_sets.end()) {
		boost::gregorian::days d(1);
		return getLastWorkDate(holiday_date_sets, date - d);
	} else {
		return date;
	}
}

boost::gregorian::date getLastWorkDate(const std::set<std::string>& holiday_date_sets, const boost::gregorian::date& date, int days) { // 取date日之前days天的工作日
	if (holiday_date_sets.empty() || date.is_not_a_date())
		return date;
	boost::gregorian::date tDate = date;
	int i = 0;
	while (i < days) {
		boost::gregorian::days d(1);
		tDate = getLastWorkDate(holiday_date_sets, tDate - d);
		if (holiday_date_sets.find(dateToStr(tDate)) == holiday_date_sets.end()) {
			i++;
		}
	} ;
	return tDate;
}

bool lessThan1Year(const std::string& endDate) {
	using namespace boost::gregorian;
	date d = strToDate(endDate);
	if (d.is_not_a_date())
		return false;
	years y(1);
	date tD = currentDate();
	return d >= tD && d <= tD + y;
}

#include "cache/cache_controller_declare.h"
#include "fastdb/query.h"
static std::map<std::string, std::set<std::string>> _HOLIDAY_CACHES;
//static std::vector<std::string> _HOLIDAY_CACHES;
void initHolidayCache() {
	//if (!_HOLIDAY_CACHES.empty())
	//	_HOLIDAY_CACHES.clear();
	//HolidayInfoCacheController hicc;
	//using namespace boost::gregorian;
	//date sD = currentDate() - years(1);
	//years y(2);
	//date eD = sD + y;
	//std::string start = dateToStr(sD);
	//std::string end = dateToStr(eD);
	//std::string sql = "holiday_date >= '" + start + "' and holiday_date <= '" + end + "'";
	//dbQuery q(sql.c_str());
	//HolidayInfoCacheVecPtr vecs = hicc.getCacheListByQueryInThreadSafty(q);
	//if (!vecs->empty()) {
	//	for (std::vector<HolidayInfoCachePtr>::iterator it = vecs->begin(); it != vecs->end(); ++it) {
	//		if (*it) {
	//			const std::string& market_type = (*it)->market_type;
	//			std::map<std::string, std::set<std::string>>::iterator iter = _HOLIDAY_CACHES.find(market_type);
	//			if (iter == _HOLIDAY_CACHES.end()) {
	//				std::set<std::string> sets;
	//				sets.insert((*it)->holiday_date);
	//				_HOLIDAY_CACHES[market_type] = sets;
	//			} else {
	//				_HOLIDAY_CACHES[market_type].insert((*it)->holiday_date);
	//			}
	//			//_HOLIDAY_CACHES.push_back((*it)->holiday_date);
	//		}
	//	}
	//}
	//LOGGER_DEBUG("holiday cache size: " << _HOLIDAY_CACHES.size() << ", cache sql: " << sql);
}

const std::set<std::string>& getHolidayCache(const std::string& market_type) {
	return _HOLIDAY_CACHES[market_type];
}

void clearHolidayCache() {
	_HOLIDAY_CACHES.clear();
}

bool strContains(const std::string& source, const std::string& str) {
	if (source.empty()) {
		return false;
	}
	if (str.empty()) {
		return false;
	}
	if (source.find(str) != std::string::npos) {
		return true;
	}
	return false;
}

std::string getIntToStringWithBracket(int i) {
	return "[" + IntToString(i) + "]";
}

std::string GetFastdbQuery(const std::string &key, const std::string &val) {
	return key + " = '" + val + "'";
}

std::string GetFastdbQueryByBondKeyListedMarket(const std::string &val, const std::string &companyId) {
	std::string ret = GetFastdbQuery(std::string("bond_key_listed_market"), val);
	if (!companyId.empty())
		ret += " and company_id = '" + companyId + "'";
	return ret;
}

std::string GetFastdbQueryById(const std::string &val, const std::string &companyId) {
	std::string ret = GetFastdbQuery(std::string("id"), val);
	if (!companyId.empty())
		ret += " and company_id = '" + companyId + "'";
	return ret;
}

void trimSql(std::string& sql) {
	if (!sql.empty()) {
		boost::replace_all(sql, "\r\n", "");
		boost::replace_all(sql, "\n", "");
		boost::algorithm::trim(sql);
	}
}

double getBestQuoteDeviate(const std::string& quote_type, const std::string& cdcYield, double price, int quoteSide) {
	double deviate = DOUBLE_NULL;
	if (kQuoteTypeYield != quote_type || cdcYield.empty() || doubleEqual(price, DOUBLE_NULL))
		return deviate;
	bool ok = false;
	double _yield = 0.0;
	if (strContains(cdcYield, "|")) {
		std::vector<std::string> vec = split(cdcYield, '|');
		if (!vec.empty()) {
			_yield = ss_lexical_cast<double>(vec.at(0), 0.0);
		}
	} else {
		_yield = ss_lexical_cast<double>(cdcYield, 0.0);
	}
	if (_yield <= 0.0) {
		return deviate;
	}
	if (kBidQuote == quoteSide) {
		deviate = price - _yield;
	} else if (kOfrQuote == quoteSide) {
		deviate = _yield - price;
	}
	return deviate;
}

int GetPid() {
	return getpid();
}

std::string bytesFormat(size_t size)
{
    std::stringstream ss;
    if (size < 1024) {
        ss << size << " B";
    } else if (size < 1024 * 1024) {
        ss << std::setiosflags(std::ios::fixed) << std::setprecision(2) << (size * 1.0 / 1024) << " KB";
    } else if (size < 1024 * 1024 * 1024) {
        ss << std::setiosflags(std::ios::fixed) << std::setprecision(2) << (size * 1.0 / 1024 / 1024) << " MB";
    } else {
        ss << std::setiosflags(std::ios::fixed) << std::setprecision(2) << (size * 1.0 / 1024 / 1024 / 1024) << " GB";
    }
    return ss.str();
}

void sleepSecond(int second)
{
    boost::this_thread::sleep(boost::posix_time::seconds(second));
}

std::string getFormatRss()
{
    return bytesFormat(getCurrentRSS());
}

// goodsCode, remove ".SH" && ".SZ", jialu, 2016/9/22
std::string getHeadOfGoodsCode(std::string gc) {
	int len = gc.length();
	if (len >= 3)
		if (gc[len - 3] == '.' &&
			(gc[len - 2] == 's' || gc[len - 2] == 'S') &&
			(gc[len - 1] == 'h' || gc[len - 1] == 'H' || gc[len - 1] == 'z' || gc[len - 1] == 'Z'))
			return gc.substr(0, len - 3);
	return gc;
}

bool isDigits(const std::string &str) {
	return str.find_first_not_of("0123456789") == std::string::npos;
}

size_t wordOccurrenceCount(std::string const & str, std::string const & word){
	size_t count(0);
	std::string::size_type word_pos(0);
	while (word_pos != std::string::npos)
	{
		word_pos = str.find(word, word_pos);
		if (word_pos != std::string::npos)
		{
			++count;
			// start next search after this word 
			word_pos += word.length();
		}
	}

	return count;
}

int GetMsTimeDiff(const std::string& fromMsTime, const std::string& toMsTime) {
	if (fromMsTime.empty() || toMsTime.empty())
		return -1;
	long long from = GetMsTimeFromStr(fromMsTime),
		to = GetMsTimeFromStr(toMsTime);
	if (from < 0 || to < 0)
		return -1;
	return to - from;
}

long long GetMsTimeFromStr(const std::string& msTime) { // 2019-12-06 09:45:12.333
	int pos = msTime.find_last_of('.');
	if (pos > 0) {
		time_t t = ParseTimeString(msTime.substr(0, pos));
		if (t > 0) {
			return t * 1000 + ss_lexical_cast(msTime.substr(pos + 1), 0);
		}
	}
	return -1;
}

std::string getLocalIpAddress(const std::string &slash) {
	static bool s_once = true;
	static std::string s_result;

	if (s_once) {
		s_once = false;
#ifdef WIN32
		using boost::asio::ip::tcp;
		try {
			boost::asio::io_service io_service;
			tcp::resolver resolver(io_service);
			tcp::resolver::query query(tcp::v4(), boost::asio::ip::host_name(), "");
			tcp::resolver::iterator iter = resolver.resolve(query);
			tcp::resolver::iterator end; // End marker.
			while (iter != end) {
				tcp::endpoint ep = *iter++;
				if (!s_result.empty()) {
					s_result += slash;
				}
				s_result += ep.address().to_string();
			}
		} catch (std::exception &e) {
			LOGGER_ERROR("get local ip address failed, " << e.what());
		}
#else
		struct ifaddrs * ifAddrStruct = NULL;
		struct ifaddrs * ifa = NULL;
		void * tmpAddrPtr = NULL;

		getifaddrs(&ifAddrStruct);
		for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
			if (!ifa->ifa_addr) {
				continue;
			}
			if (ifa->ifa_addr->sa_family == AF_INET) { // check it is IP4
				// is a valid IP4 Address
				tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
				char addressBuffer[INET_ADDRSTRLEN];
				inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
				std::string address(addressBuffer);
				if (address != "127.0.0.1") {
					if (!s_result.empty()) {
						s_result += slash;
					}
					s_result += address;
				}
			}
		}
		if (ifAddrStruct != NULL) freeifaddrs(ifAddrStruct);
#endif
	}
	return s_result;
}

bool releaseFreeMemory() {
#ifndef WIN32
	return 1 == malloc_trim(0);
#else
	return false;
#endif
}

MemoryGuard::MemoryGuard() {
	releaseFreeMemory();
}

MemoryGuard::~MemoryGuard() {
	releaseFreeMemory();
}

/*
根据起始、结束日期，格式化为剩余期限(88D,1Y,1.5Y...)
iStart-起始日期，如20200101
iEnd-结束日期，如20220501
cvt2Days-日期间隔小于1年时，是否转换为天数
*/
std::string GetMaturityTerms(int iStart, int iEnd, bool cvt2Days) {
	boost::gregorian::years s_year_single(1);
	boost::gregorian::date s_start = boost::gregorian::date_from_iso_string(IntToString(iStart)),
		s_end = boost::gregorian::date_from_iso_string(IntToString(iEnd)),
		s_year = s_end - s_year_single;
	boost::gregorian::date_period dp(s_year, s_end);
	std::stringstream ss;
	if (cvt2Days && dp.contains(s_start)) {		//	转换成天数，且小于1年(含)
		boost::gregorian::date_period dp1(s_start, s_end);
		if (dp1.length().days() >= 365) {	//	365 or 366,可能为闰年
			ss << "1Y";
		} else {
			ss << dp1.length().days() << "D";
		}
	} else {
		int iY = 0;
		std::string s;
		boost::gregorian::year_iterator it_year(s_end);
		--it_year;
		while (it_year <= s_end) {	//	从结束日期起，往前推，每次减一年
			iY++;
			if (it_year <= s_start) {
				iY--;
				++it_year;
				boost::gregorian::date_period tdp(s_start, boost::gregorian::date(it_year->year(), it_year->month(), it_year->day()));
				if (tdp.length().days() >= 365) { // 可能为闰年
					ss << (iY + 1) << "Y";
				} else {
					double iCnt = 365.00;	// 声明为double类型，默认一年365天
					if (boost::gregorian::gregorian_calendar::is_leap_year(s_start.year())) {	//	起始年份为闰年
						boost::gregorian::date td(s_start.year(), 2, 29);
						if (tdp.contains(td))	//	包含0229
							iCnt = 366.00;
					} else if (boost::gregorian::gregorian_calendar::is_leap_year(it_year->year())) {	//	结束日期依次往前减，最近一次区间为闰年
						boost::gregorian::date td(it_year->year(), 2, 29);
						if (tdp.contains(td))	//	包含0229
							iCnt = 366.00;
					}
					double d = Round(tdp.length().days() / iCnt, 2);	// 格式化为最多两位小数的0.xx，不包含末尾的0
					ss << (iY + d) << "Y";	// 格式化为m+n.xxY
				}
				break;
			}
			--it_year;
		}
	}
	return ss.str();
}

double GetMaturityTermsReal(int iStart, int iEnd) {
	boost::gregorian::date s_start = boost::gregorian::date_from_iso_string(IntToString(iStart)),
		s_end = boost::gregorian::date_from_iso_string(IntToString(iEnd));
	double ret = 0.0;
	int iY = 0;
	boost::gregorian::year_iterator it_year(s_end);
	--it_year;
	while (it_year <= s_end) {	//	从结束日期起，往前推，每次减一年
		iY++;
		if (it_year < s_start) {
			iY--;
			++it_year;			
			boost::gregorian::date_period tdp(s_start, boost::gregorian::date(it_year->year(), it_year->month(), it_year->day()));
			if (tdp.length().days() >= 365) { // 可能为闰年
				ret += iY + 1.0;
			} else {
				double iCnt = 365.00;	// 声明为double类型，默认一年365天
				if (boost::gregorian::gregorian_calendar::is_leap_year(s_start.year())) {	//	起始年份为闰年
					boost::gregorian::date td(s_start.year(), 2, 29);
					if (tdp.contains(td))	//	包含0229
						iCnt = 366.00;
				} else if (boost::gregorian::gregorian_calendar::is_leap_year(it_year->year())) {	//	结束日期依次往前减，最近一次区间为闰年
					boost::gregorian::date td(it_year->year(), 2, 29);
					if (tdp.contains(td))	//	包含0229
						iCnt = 366.00;
				}
				double d = tdp.length().days() / iCnt;
				ret += (iY + d);	// 格式化为m+n.xxxx
			}			
			break;
		}
		--it_year;
	}
	return ret;
}

std::string cvtDateToCalcDate(const std::string& date, bool useDefaultCurrentDate) {
	std::string default_ret = "", ret = "";
	if (useDefaultCurrentDate)
		default_ret = GetTDateString("%Y%m%d");
	if (date.empty()) {
		return default_ret; 
	}
	if (date.length() >= 19) { // yyyy-MM-dd HH:mm:ss
		ret = date.substr(0, 10);
		boost::replace_all(ret, "-", "");
	} else if (date.length() >= 10) {
		ret = date;
		boost::replace_all(ret, "-", "");
	} else if (date.length() == 8) {
		ret = date;
	} else {
		ret = default_ret;
	}
	return ret;
}

bool isVectorStringEqual(std::vector<std::string> &left, std::vector<std::string> &right)
{
    if (left.size() != right.size()) {
        return false;
    }
    std::sort(left.begin(), left.end());
    std::sort(right.begin(), right.end());
    return left == right;
}

namespace idbjson {
	using namespace boost::property_tree;
	std::string GenerateJson(const ptree& pt, bool isSubList) {
		if (pt.empty()) return "";

		bool success = true;
		std::string json_str = "";
		try {
			std::ostringstream ostream;
			write_json(ostream, pt, false);
			json_str = ostream.str();
			boost::erase_all(json_str, "    ");
			if (isSubList && (json_str.find("[") != std::string::npos) && (json_str.find("]") != std::string::npos)) {
				json_str = json_str.substr(json_str.find_first_of("["), json_str.find_last_of("]") - json_str.find_first_of("[") + 1);
			}
		} catch (ptree_error& e) {
			LOGGER_ERROR("exception raise , when generate json, errmsg[ " << e.what() << " ]");
		}

		return json_str;
	}

	//--------------------------------------------------------------------------------------------------------------------------------------------
	//重写boost write_json接口，修复UTF-8中文处理问题
	void write_json(std::basic_ostream<ptree::key_type::value_type> &stream, const ptree &pt, bool pretty) {
		write_json_helper(stream, pt, 0, pretty);
	}

	//from boost source code
	void write_json_helper(std::basic_ostream<ptree::key_type::value_type> &stream, const ptree &pt, int indent, bool pretty) {

		typedef ptree::key_type::value_type Ch;
		typedef std::basic_string<Ch> Str;

		// Value or object or array
		if (indent > 0 && pt.empty()) {
			// Write value
			Str data = create_escapes(pt.get_value<Str>());

			stream << Ch('"') << data << Ch('"');

		} else if (indent > 0 && pt.count(Str()) == pt.size()) {
			// Write array
			stream << Ch('[');
			if (pretty) stream << Ch('\n');
			ptree::const_iterator it = pt.begin();
			for (; it != pt.end(); ++it) {
				if (pretty) stream << Str(4 * (indent + 1), Ch(' '));
				write_json_helper(stream, it->second, indent + 1, pretty);
				if (boost::next(it) != pt.end())
					stream << Ch(',');
				if (pretty) stream << Ch('\n');
			}
			stream << Str(4 * indent, Ch(' ')) << Ch(']');

		} else {
			// Write object
			stream << Ch('{');
			if (pretty) stream << Ch('\n');
			ptree::const_iterator it = pt.begin();
			for (; it != pt.end(); ++it) {
				if (pretty) stream << Str(4 * (indent + 1), Ch(' '));
				stream << Ch('"') << create_escapes(it->first) << Ch('"') << Ch(':');
				if (pretty) {
					if (it->second.empty())
						stream << Ch(' ');
					else
						stream << Ch('\n') << Str(4 * (indent + 1), Ch(' '));
				}
				write_json_helper(stream, it->second, indent + 1, pretty);
				if (boost::next(it) != pt.end())
					stream << Ch(',');
				if (pretty) stream << Ch('\n');
			}
			if (pretty) stream << Str(4 * indent, Ch(' '));
			stream << Ch('}');
		}

	}

	//from boost source code
	std::basic_string<char> create_escapes(const std::basic_string<char> &s) {
		std::basic_string<char> result;
		std::basic_string<char>::const_iterator b = s.begin();
		std::basic_string<char>::const_iterator e = s.end();
		while (b != e) {
			// This assumes an ASCII superset. But so does everything in PTree.
			// We escape everything outside ASCII, because this code can't
			// handle high unicode characters.
			if (*b == 0x20 || *b == 0x21 || (*b >= 0x23 && *b <= 0x2E) ||
				(*b >= 0x30 && *b <= 0x5B) || (*b >= 0x5D && *b <= 0xFF)  //it fails here because char are signed
				|| (*b >= -0x80 && *b < 0)) // 增加此条件用于处理UTF8中文
				result += *b;
			else if (*b == char('\b')) result += char('\\'), result += char('b');
			else if (*b == char('\f')) result += char('\\'), result += char('f');
			else if (*b == char('\n')) result += char('\\'), result += char('n');
			else if (*b == char('\r')) result += char('\\'), result += char('r');
			else if (*b == char('/')) result += char('\\'), result += char('/');
			else if (*b == char('"'))  result += char('\\'), result += char('"');
			else if (*b == char('\\')) result += char('\\'), result += char('\\');
			else {
				const char *hexdigits = "0123456789ABCDEF";
				unsigned long u = (std::min)(static_cast<unsigned long>(
					static_cast<unsigned char>(*b)),
					0xFFFFul);
				int d1 = u / 4096; u -= d1 * 4096;
				int d2 = u / 256; u -= d2 * 256;
				int d3 = u / 16; u -= d3 * 16;
				int d4 = u;
				result += char('\\'); result += char('u');
				result += char(hexdigits[d1]); result += char(hexdigits[d2]);
				result += char(hexdigits[d3]); result += char(hexdigits[d4]);
			}
			++b;
		}
		return result;
	}
}
