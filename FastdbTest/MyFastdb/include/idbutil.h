#ifndef _IDB_UTILS_H_
#define _IDB_UTILS_H_

#include <boost/random.hpp>
#include <boost/generator_iterator.hpp>
#include <boost/thread/shared_mutex.hpp>

using namespace std;
class IDBUtil {
public:
	static IDBUtil* getInstance();

	int getRandomInt(int min, int max);
	bool isSyncSchedule();
	void updateCacheSchema();

private:
	IDBUtil();
	virtual ~IDBUtil();

	static IDBUtil* instance_;
	boost::random::mt19937_64 rng;
	map<string, int> dcs_serials;
	boost::shared_mutex dcs_serial_mutex_;
};

#endif // _IDB_UTILS_H_