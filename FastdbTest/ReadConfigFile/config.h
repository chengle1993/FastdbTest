#ifndef DA_CONFIG_H
#define DA_CONFIG_H

#include <string>
#include <vector>
#include <boost/thread/thread.hpp>
#include <boost/thread/shared_mutex.hpp>
#include <time.h>
#include <map>


using std::string;
using std::map;
using std::vector;

class Config {
public:
	static Config* singleton();
	bool fail() { return !init_; };

	const string& get(const string &key);
	const vector<string> &get_vector(const string &key);
	int count(const string &key);
	void config(const std::string &config_file);
	bool load();
	// special load
	bool load(const std::string &topic);

	std::string getValue(const std::string &name);
	std::string getValue(const std::string &name, const std::string &default_value);
	int getIntValue(const std::string& name, const int& default_value);
	bool getBoolValue(const std::string& name, const bool& default_value);
private:
	Config();
	virtual ~Config();
	Config(const Config&);

	void run();
	void init();
	void clear();

	static Config* instance_;
	bool init_;
	map<string, vector<string> > m_setting;

	boost::shared_mutex mutex_;
	time_t t_;

	static string null_string; //for false return value
	static vector<string> null_vector;
};
#endif

