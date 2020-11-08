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

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/locale/encoding.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>  
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/regex.hpp> 
#include <boost/algorithm/string/replace.hpp>
#include <boost/thread/mutex.hpp>

#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
#endif

static const boost::posix_time::ptime epoch(boost::gregorian::date(1970, boost::gregorian::Jan, 1));

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

time_t GetCurrentTimeMilliSec() {
	boost::int64_t time64 = (boost::posix_time::microsec_clock::universal_time() - epoch).total_milliseconds();

	return time64;
}

time_t GetCurrentTimeMicroSec() {
	boost::int64_t time64 = (boost::posix_time::microsec_clock::universal_time() - epoch).total_microseconds();

	return time64;
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


std::string GBKToUTF8(const std::string& strGBK) {
	return boost::locale::conv::between(strGBK, "UTF-8", "GBK");
}

std::string Utf8ToAnsi(const std::string &strUTF8) {
	return boost::locale::conv::between(strUTF8, "GBK", "UTF-8");
}