#include "cache_def.h"

template <>
std::string getKeyEqualValue(bool &t) {
	return t ? " = 1" : " = 0";
}

template <>
std::string getStringValue(bool &t) {
	return t ? "1" : "0";
}

std::string arrayToStr(const dbArray<std::string>& strs) {
	std::string ret = "";
	for (size_t i = 0; i < strs.length(); i++) {
		if (i != 0)
			ret += ",";
		ret += strs[i];
	}
	return ret;
}
void strToArray(const std::string& str, dbArray<std::string>& arrays) {
	if (str.empty())
		return;
	std::vector<std::string> vecs = split(str, ',');
	for (size_t i = 0; i < vecs.size(); ++i) {
		arrays.append(vecs.at(i));
	}
}
