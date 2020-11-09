#include <fstream>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "boost/algorithm/string.hpp"
#include <time.h>
#include "config.h"
#include "logger.h"
using namespace std;

#define CHECK_INTERVAL 5 //5 sec
#define DEFAULT_SERVER_NUM 5
#define DEFAULT_TIMEOUT 30 //30 sec

#define READLOCK(a) boost::shared_lock<boost::shared_mutex> lock(a)
#define WRITELOCK(a) boost::unique_lock<boost::shared_mutex> lock(a)

Config *Config::instance_ = NULL;
string ConfigFile = "config.ini";
string Config::null_string; //for false return value
vector<string> Config::null_vector;

Config*
Config::singleton() {
	if (NULL == instance_) {
		instance_ = new Config;
	}
	return instance_;
}

Config::~Config() {
	if (NULL != instance_) {
		delete instance_;
	}
}

Config::Config() :init_(false), t_(0) {}

void Config::config(const std::string &config_file) {
	ConfigFile = config_file;
	init();
}

void Config::run() {
	while (1) {
		boost::this_thread::sleep(boost::posix_time::seconds(CHECK_INTERVAL));
		init();
	}
}

void Config::init() {
	WRITELOCK(mutex_);

	ifstream in(ConfigFile.c_str(), ios::in);
	if (in.fail()) {
		LOGGER_ERROR(__FUNCTION__ << " open fail " << ConfigFile.c_str());
		init_ = false;
		return;
	}

	try {
		clear();
		std::string line;
		while (!in.eof()) {
			std::getline(in, line);
			if (line[0] == '#') {
				continue;
			}
			std::string::size_type pos = line.find("=");
			if (pos == std::string::npos) {
				continue;
			}
			std::string title = line.substr(0, pos);
			std::string content = line.substr(pos + 1);
			boost::algorithm::trim(title);
			boost::algorithm::trim(content);

			m_setting[title].push_back(content);
		}
	} catch (...) {
		LOGGER_ERROR(__FUNCTION__ << " error happened");
		init_ = false;
	}
	init_ = true;
	LOGGER_INFO(ConfigFile.c_str() << " read");
	return;
}

void Config::clear() {
	m_setting.clear();
}

const string& Config::get(const string &key) {
	READLOCK(mutex_);
	map<string, vector<string> >::const_iterator it = m_setting.find(key);
	if (it == m_setting.end()) {
		return null_string;
	} else {
		return it->second[0];
	}
}

const vector<string>& Config::get_vector(const string &key) {
	READLOCK(mutex_);
	if (m_setting.count(key)) {
		return m_setting[key];
	}
	return null_vector;
}

int Config::count(const string &key) {
	if (0 == m_setting.count(key)) {
		return 0;
	}
	return m_setting[key].size();
}

std::string Config::getValue(const std::string &name) {
	READLOCK(mutex_);
	return get(name);
}

std::string Config::getValue(const std::string &name, const std::string &default_value) {
	READLOCK(mutex_);
	const std::string& value = get(name);
	if (value == null_string) {
		LOGGER_WARN(__FUNCTION__ << " " << name.c_str() << " default value: " << default_value.c_str() << " returned");
		return default_value;
	} else {

		return std::string(value);
	}
}
int Config::getIntValue(const std::string& name, const int& default_value) {
	READLOCK(mutex_);
	const std::string& value = get(name);
	if (value == null_string) {
		LOGGER_WARN(__FUNCTION__ << " " << name.c_str() << " default value: " << default_value << " returned");
		return default_value;
	} else {
		try {
			return boost::lexical_cast<int>(value);
		} catch (...) {
			LOGGER_WARN(__FUNCTION__ << " " << name.c_str() << " error happened, default value: " << default_value << " returned");
			return default_value;
		}
	}
}
bool Config::getBoolValue(const std::string& name, const bool& default_value) {
	READLOCK(mutex_);
	const std::string& value = get(name);
	if (value == null_string) {
		std::string tip = default_value ? "true" : "false";
		LOGGER_WARN(__FUNCTION__ << " " << name.c_str() << " default value: " << tip.c_str() << " returned");
		return default_value;
	} else {
		std::string tmp(value);
		boost::to_upper(tmp);
		return tmp == "TRUE" || tmp == "1";
	}
}

// default load
bool Config::load() {
	//留下这个函数是为了考虑代码, 兼容性.
	// load 操作在config()就完成了.
	return init_;
}

// special load
bool Config::load(const std::string &topic) {
	//留下这个函数是为了考虑代码, 兼容性.
	// load 操作在config()就完成了.
	return init_;
}
