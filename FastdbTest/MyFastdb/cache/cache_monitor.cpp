#include "cache_monitor.h"

#include "cache_center.h"
#include "include/config.h"
#include "../include/logger.h"
#include "../include/common.h"
#include <boost/bind.hpp>
#include <boost/function/function0.hpp>
#include <boost/thread/thread.hpp>
#include <cstdlib>

#include "controller/account_cachecontroller.h"

CacheMonitor* _instance = 0;
static bool isStarted = false;
static const std::string FASTDB_NODE_STATUS_TEXT[] = { "OFFLINE", "ONLINE", "ACTIVE", "STANDBY", "RECOVERED" };
static const std::string BOND_TABLES[] = { "SwSectorInfoCache", "IssuingInstitutionCache", "BondTraderCache", "BrokerBindingTraderCache", "ChinaBondDeviatedValueCache", "ProvinceInfoCache", "TradingInstitutionCache", "BondSmallViewCache", "AccountCache", "VersionCache", "SelectiveInfoCache", "AccountGroupCache", "AccountPermissionCache", "ConstantsCache", "IssuerInfoCache", "BondTypeSelectiveCache", "HolidayInfoCache", "SystemCompanyCache", "InstitutionRatingCache", "BondQuoteReferCache", "BondQuoteCache", "BondDealCache", "ChinaBondValuationCache" };

static int dbFailCount = 0;
CacheMonitor::CacheMonitor() {}


CacheMonitor::~CacheMonitor() {
	saveFile.close();
}

CacheMonitor* CacheMonitor::getInstance() {
	if (0 == _instance) {
		_instance = new CacheMonitor();
	}
	return _instance;
}

void CacheMonitor::start() {
	if (isStarted) {
		return;
	}
	LOGGER_INFO(getFastDBInfo());

	saveFile.open(saveFileName);

	boost::function0<void> f = boost::bind(&CacheMonitor::print, this);
	boost::thread t(f);
	isStarted = true;
}

void CacheMonitor::print() {
	boost::posix_time::ptime last = boost::posix_time::second_clock::local_time();
	boost::posix_time::time_duration diff;
	while (1) {
		Config::singleton()->load("FastDB"); // fresh config param
		int iInterval = Config::singleton()->getIntValue("FastDB.MonitorInterval", 600); // print fastdb status time interval, unit:sec, default:10min
		boost::this_thread::sleep(boost::get_system_time() + boost::posix_time::seconds(10));
		boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
		diff = now - last;

		if (!isMonitorEnable()) {
			continue;
		}

		if (diff.total_seconds() >= iInterval) {
			LOGGER_INFO(getFastDBInfo());
			checkFastDB();
			last = boost::posix_time::second_clock::local_time();
		}
		saveFastDBInfo();
	}
}

void CacheMonitor::saveFastDBInfo() {
	if (saveFile.is_open()) {
		std::stringstream ss;
		if (pDB && pDB->isOpen()) {
			dbFailCount = 0;

			ss << (pDB->getUsedDatabaseSize() / 1024 / 1024) << "M";
			dbReplicatedDatabase *rDB = dynamic_cast<dbReplicatedDatabase*>(pDB);
			if (rDB) {
				int nodeId = rDB->getCurrentNodeId();
				int nodeStatus = rDB->getNodeStatus(nodeId);
				ss << " " << rDB->getNumberOfOnlineNodes()
					<< " " << (nodeStatus < 0 ? "Unknown" : FASTDB_NODE_STATUS_TEXT[nodeStatus])
					<< std::endl;
			}
		} else {
			dbFailCount++;
			if (dbFailCount > 5) {
				ss << -1 << std::endl;
			}
		}

		saveFile.clear();
		saveFile.seekp(0);
		saveFile << ss.str();
		saveFile.flush();
	}
}

std::string CacheMonitor::getFastDBInfo() {
	if (!pDB)
		return "";
	std::ostringstream ostream;
	try {
		ostream << "fastdb isOpen: " << (pDB->isOpen() ? "true" : "false");
		if (pDB->isOpen()) {
			ostream << " version: " << pDB->getVersion()
				<< " databaseSize: " << (pDB->getDatabaseSize() / 1024 / 1024) << "M"
				<< " usedDatabaseSize: " << (pDB->getUsedDatabaseSize() / 1024 / 1024) << "M"
				<< " allocatedSize: " << (pDB->getAllocatedSize() / 1024 / 1024) << "M"
				<< " users: " << pDB->getNumberOfUsers()
				<< " writers: " << pDB->getNumberOfWriters()
				<< " blockedWriters: " << pDB->getNumberOfBlockedWriters()
				<< " readers: " << pDB->getNumberOfReaders()
				<< " blockedReaders: " << pDB->getNumberOfBlockedReaders()
				<< " proceedTransactions: " << pDB->getNumberOfProceededTransactions();
		} else {
			ostream << " allocatedSize: " << (pDB->getAllocatedSize() / 1024 / 1024) << "M";
		}
		dbReplicatedDatabase *rDB = dynamic_cast<dbReplicatedDatabase*>(pDB);
		if (rDB && rDB->isOpen()) {
			int nodeId = rDB->getCurrentNodeId();
			int nodeStatus = rDB->getNodeStatus(nodeId);
			ostream << " onlineNodes: " << rDB->getNumberOfOnlineNodes()
				<< " activeNodeId: " << rDB->getActiveNodeId()
				<< " masterNodeId: " << rDB->getMasterNodeId()
				<< " currentNodeId: " << nodeId
				<< " currentNodeStatus: " << (nodeStatus < 0 ? "Unknown" : FASTDB_NODE_STATUS_TEXT[nodeStatus])
				<< " isServerEnable: " << (isFastDBEnable() ? "true" : "false");

			if (isFastDBEnable()) {
				dbQuery q("");
				AccountCacheController acc;;
                dbQuery validBestQuoteQuery;
                validBestQuoteQuery = "having_quote = 'Y'";

				ostream << " accountSize: " << acc.getSizeByQueryInThreadSafty(q)
/*					<< " quoteSize: " << bqcc.getSizeByQueryInThreadSafty(q)
					<< " bestQuoteSize: " << bbqcc.getSizeByQueryInThreadSafty(q)
                    << " validBestQuoteSize: " << bbqcc.getSizeByQueryInThreadSafty(validBestQuoteQuery)
					<< " referQuoteSize: " << bqrcc.getSizeByQueryInThreadSafty(q)
					<< " dealSize: " << bdcc.getSizeByQueryInThreadSafty(q)
					<< " dcsDealSize: " << dbdcc.getSizeByQueryInThreadSafty(q)*/;
			}
		}
        ostream << " ProcessMemory: " << getFormatRss();
	} catch (dbException e) {
		LOGGER_ERROR(e.what());
	} catch (...) {
		LOGGER_ERROR("unknown error occur when print fastdb info!!!");
	}
	return ostream.str();
}

void CacheMonitor::checkFastDB() {
	dbReplicatedDatabase *rDB = dynamic_cast<dbReplicatedDatabase*>(pDB);
	if (rDB && rDB->isOpen()) {
		if (rDB->getNumberOfUsers() < 1) {
			LOGGER_WARN("no user connect to current server!!!");
		}
	} else {
		LOGGER_ERROR("server status error, please check!!!");
	}
}

bool CacheMonitor::isFastDBEnable() {
	dbReplicatedDatabase *rDB = dynamic_cast<dbReplicatedDatabase*>(pDB);
	if (rDB) {
		if (CacheCenter::sharedInstance()->isFastDBOpened() && rDB->isOpen()) {
			int iNodeId = rDB->getCurrentNodeId();
			int iStatus = rDB->getNodeStatus(iNodeId);
			return 2 == iStatus;    // ST_ACTIVE
		}
	}
	return false;
}

void CacheMonitor::setDataBase(dbDatabase* db) {
	pDB = db;
}

void CacheMonitor::setSaveFileName(const std::string& filename) {
	saveFileName = "log/" + filename;
}

bool changed = false;
bool CacheMonitor::isMonitorEnable() {
	std::string disableTimes = Config::singleton()->getValue("FastDB.MonitorDisabledTimes");
	if (disableTimes.empty()) {
		return true;
	}
	std::vector<std::string> vecs;
	split(disableTimes, "-", &vecs);
	if (vecs.size() < 2) {
		return true;
	}
	int startHour = atoi(vecs[0].c_str());
	int endHour = atoi(vecs[1].c_str());
	boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
	int h = now.time_of_day().hours();
	if (h >= startHour && h < endHour) {
		if (changed)
			return false;
		if (pDB && pDB->isOpen()) {
			dbReplicatedDatabase *rDB = dynamic_cast<dbReplicatedDatabase*>(pDB);
			int iStatus = rDB && rDB->isOpen() ? rDB->getNodeStatus(rDB->getCurrentNodeId()) : -1;
			if (rDB && rDB->isOpen() && rDB->getNumberOfOnlineNodes() > 1 && 2 != iStatus) { // slave node
				//if (rDB && rDB->isOpen()) { // slave node
				LOGGER_INFO("disable fastdb slave node. close ...");
				CacheCenter::sharedInstance()->Close();
				LOGGER_INFO("fastdb slave node close success.");
				changed = true;
			}
		}
	} else {
		if (changed && (!pDB || !pDB->isOpen())) {
			LOGGER_INFO("reopen fastdb slave node. openning ...");
			CacheCenter::sharedInstance()->Open();
			LOGGER_INFO("reopen fastdb success. " << getFastDBInfo());
			changed = false;
		}
	}
	return true;
}
