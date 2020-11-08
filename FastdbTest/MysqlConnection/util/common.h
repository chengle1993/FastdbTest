#ifndef _COMMON_H_
#define _COMMON_H_

void split(const std::string& s, const std::string& delim, std::vector< std::string >* ret);
time_t GetCurrentTimeMilliSec();     // 获取毫秒级时间
time_t GetCurrentTimeMicroSec();     // 获取微秒级时间
std::vector<std::string> &split(const std::string &s, char delim, std::vector<std::string> &elems);
std::vector<std::string> split(const std::string &s, char delim);

std::string GBKToUTF8(const std::string& strGBK);
std::string Utf8ToAnsi(const std::string &strUTF8);
#endif