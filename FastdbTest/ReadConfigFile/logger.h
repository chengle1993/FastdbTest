#ifndef _LOGGER_H_
#define _LOGGER_H_

#include <log4cxx/logger.h>
#include <boost/format.hpp>

//#declear LOGGER_DECLARATION
extern std::string logger_name_;
extern log4cxx::LoggerPtr logger_;

#define SBF(FORMAT, VALUES) boost::str(boost::format(FORMAT) % VALUES)

#define LOGGER_INITIALIZATION(loggername) \
    logger_name_ = "bond_server"; \
    logger_ = log4cxx::Logger::getLogger(logger_name_);

#define LOGGER_DEBUG(message) LOG4CXX_DEBUG(logger_, message)
#define LOGGER_TRACE(message) LOG4CXX_TRACE(logger_, message)
#define LOGGER_INFO(message) LOG4CXX_INFO(logger_, message)
#define LOGGER_WARN(message) LOG4CXX_WARN(logger_, message)
#define LOGGER_ERROR(message) LOG4CXX_ERROR(logger_, message)
#define LOGGER_ASSERT(condition, message) LOG4CXX_ASSERT(logger_, condition, message)
#define LOGGER_FATAL(message) LOG4CXX_FATAL(logger_, message)

#define LOGGER_TIME_COST_BEGIN(log_note)\
	clock_t t1, t2; \
	t1 = clock(); \

#define LOGGER_TIME_COST_END(log_note)\
	t2 = clock(); \
	LOGGER_INFO (log_note << "... - " <<  " Time costing: " << (t2 - t1)*1000.0/CLOCKS_PER_SEC << "ms");

#endif // _LOGGER_H_