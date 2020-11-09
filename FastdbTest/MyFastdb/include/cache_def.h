#ifndef _CACHE_DEF_H_
#define _CACHE_DEF_H_

#include <fastdb/fastdb.h>
#include <fastdb/array.h>
#include <string>
#include "constants.h"
#include "common.h"
#include <iomanip>
#include "cppconn/resultset.h"
#include "logger.h"
#include <vector>
#include <math.h>
#include "db/dbconnector.h"
#include <memory>

enum dbIndexTypeExtern {
	FIELD_NOT_FROM_DISK_DATABASE = 1048576,
	FIELD_NOT_COMPARE = 2097152,// bit value to specify which field not compare
	FIELD_NOT_UPDATE_TO_DISK_DATABASE = 524288
};

#define FIELD_NOCOMPARE(x) KEY(x, FIELD_NOT_COMPARE)

// bit value to specify combined value, which not come(s) from disk database, 1048576 = 2<<20
#define COMBINED(x) KEY(x, FIELD_NOT_FROM_DISK_DATABASE)
#define COMBINED_KEY(x, index) KEY(x, index|FIELD_NOT_FROM_DISK_DATABASE)
#define COMBINED_NOCOMPARE(x) KEY(x, FIELD_NOT_FROM_DISK_DATABASE|FIELD_NOT_COMPARE)

// bit value to specify which field not update to disk database, 524288 = 2<<19, COMBINED field no need to specify
#define NOUPDATE(x) KEY(x, FIELD_NOT_UPDATE_TO_DISK_DATABASE)
#define NOUPDATE_KEY(x, index) KEY(x, index|FIELD_NOT_UPDATE_TO_DISK_DATABASE)
#define NOUPDATE_NOCOMPARE(x) KEY(x, FIELD_NOT_UPDATE_TO_DISK_DATABASE|FIELD_NOT_COMPARE)

#define PRECISION_VALUE 0.0000001

template<class T>
bool array_float_equal(const dbArray<T> &left, const dbArray<T> &right, double precision = PRECISION_VALUE)
{
    if (left.length() != right.length()) {
        return false;
    }

    for (size_t i = 0; i < left.length(); i++) {
        if (fabs(left[i] - right[i]) > precision) {
            return false;
        }
    }
    return true;
}

template <class T>
bool isCacheEqual(const T &t1, const T &t2) {
	const T* dst = &t1;
	const T* src = &t2;
	dbFieldDescriptor* fd = dst->dbDescriptor.getFirstField();
	dbFieldDescriptor* fdFirst = fd;
	if (!fd)
		return false;

	do {

		// combined fields or not updated fields will passed
		if ((fd->indexType & FIELD_NOT_COMPARE))
			continue;

		long long dstOffs = (long long)dst + fd->appOffs;
		long long srcOffs = (long long)src + fd->appOffs;
		bool notEqual = false;

		switch (fd->appType) {
		case dbField::tpBool:
			notEqual = (*(bool*)dstOffs != *(bool*)srcOffs);
			break;
		case dbField::tpInt1:
			notEqual = (*(int1*)dstOffs != *(int1*)srcOffs);
			break;
		case dbField::tpInt2:
			notEqual = (*(int2*)dstOffs != *(int2*)srcOffs);
			break;
		case dbField::tpInt4:
			notEqual = (*(int4*)dstOffs != *(int4*)srcOffs);
			break;
		case dbField::tpInt8:
			notEqual = (*(db_int8*)dstOffs != *(db_int8*)srcOffs);
			break;
		case dbField::tpReal4:
			notEqual = (fabs(*(real4*)dstOffs - *(real4*)srcOffs) > PRECISION_VALUE);
			break;
		case dbField::tpReal8:
			notEqual = (fabs(*(real8*)dstOffs - *(real8*)srcOffs) > PRECISION_VALUE);
			break;
		case dbField::tpStdString:
			notEqual = (*(std::string*)dstOffs != *(std::string*)srcOffs);
			break;
        case dbField::tpArray:
        {
            if (fd->components) {
                int subType = fd->components->appType;
                if (subType == dbField::tpBool) {
                    dbArray<bool> *dst = (dbArray<bool>*)dstOffs;
                    dbArray<bool> *src = (dbArray<bool>*)srcOffs;
                    notEqual = (dst->compare(*src) != 0);
                    break;
                } else if (subType == dbField::tpInt1) {
                    dbArray<int1> *dst = (dbArray<int1>*)dstOffs;
                    dbArray<int1> *src = (dbArray<int1>*)srcOffs;
                    notEqual = (dst->compare(*src) != 0);
                    break;
                } else if (subType == dbField::tpInt2) {
                    dbArray<int2> *dst = (dbArray<int2>*)dstOffs;
                    dbArray<int2> *src = (dbArray<int2>*)srcOffs;
                    notEqual = (dst->compare(*src) != 0);
                    break;
                } else if (subType == dbField::tpInt4) {
                    dbArray<int4> *dst = (dbArray<int4>*)dstOffs;
                    dbArray<int4> *src = (dbArray<int4>*)srcOffs;
                    notEqual = (dst->compare(*src) != 0);
                    break;
                } else if (subType == dbField::tpInt8) {
                    dbArray<db_int8> *dst = (dbArray<db_int8>*)dstOffs;
                    dbArray<db_int8> *src = (dbArray<db_int8>*)srcOffs;
                    notEqual = (dst->compare(*src) != 0);
                    break;
                } else if (subType == dbField::tpReal4) {
                    dbArray<real4> *dst = (dbArray<real4>*)dstOffs;
                    dbArray<real4> *src = (dbArray<real4>*)srcOffs;
                    notEqual = !array_float_equal<real4>(*dst, *src);
                    break;
                } else if (subType == dbField::tpReal8) {
                    dbArray<real8> *dst = (dbArray<real8>*)dstOffs;
                    dbArray<real8> *src = (dbArray<real8>*)srcOffs;
                    notEqual = !array_float_equal<real8>(*dst, *src);
                    break;
                } else if (subType == dbField::tpStdString) {
                    dbArray<std::string> *dst = (dbArray<std::string>*)dstOffs;
                    dbArray<std::string> *src = (dbArray<std::string>*)srcOffs;
                    notEqual = (dst->compare(*src) != 0);
                    break;
                }
            }
        }
		case dbField::tpRawBinary:
		case dbField::tpString:
		case dbField::tpWString:
		case dbField::tpStdWString:
		case dbField::tpReference:
		case dbField::tpRectangle:
		case dbField::tpStructure:
		default:
			LOGGER_ERROR("ERROR: The field type used for fastdb not supported when compare cache! cache name: " << T::TABLE_NAME << ", column: " << fd->name);
			notEqual = true;
		}

		if (notEqual) {
			//LOGGER_INFO("cache [" << T::TABLE_NAME << "] not equal with field:[" << fd->name << "], src:[" << ( *(std::string*)srcOffs) << "], des:[" << ( *(std::string*)dstOffs) << "]");
			return false;
		}

	} while ((fd = fd->next) != fdFirst);

	return true;
}

template <class T>
bool insertCacheG(T *dst, sql::ResultSet *resSet) {
	dbFieldDescriptor* fd = dst->dbDescriptor.getFirstField();
	dbFieldDescriptor* fdFirst = fd;
	if (!fd)
		return false;

	do {
		// combined fields, not directly from database
		if (fd->indexType & FIELD_NOT_FROM_DISK_DATABASE)
			continue;

		std::string columnName = fd->name;

		long long dstOffs = (long long)dst + fd->appOffs;
		LOGGER_DEBUG("columnName:" << fd->name);
		LOGGER_DEBUG("dstOffs:" << dstOffs);
		LOGGER_DEBUG("appOffs:" << fd->appOffs);
		LOGGER_DEBUG("appType:" << fd->appType);
		switch (fd->appType) {
		case dbField::tpBool:
			*(bool*)dstOffs = resSet->getBoolean(columnName);
			break;
		case dbField::tpInt1:
			*(int1*)dstOffs = (int1)resSet->getInt(columnName);
			break;
		case dbField::tpInt2:
			*(int2*)dstOffs = (int2)resSet->getInt(columnName);
			break;
		case dbField::tpInt4:
			*(int4*)dstOffs = (int4)resSet->getInt(columnName);
			break;
		case dbField::tpInt8:
			*(db_int8*)dstOffs = resSet->getInt64(columnName);
			break;
		case dbField::tpReal4:
			*(real4*)dstOffs = (float)resSet->getDouble(columnName);
			break;
		case dbField::tpReal8:
			*(real8*)dstOffs = resSet->getDouble(columnName);
			break;
		case dbField::tpStdString:
			((std::string*)dstOffs)->assign(resSet->getString(columnName).asStdString());
			break;
		case dbField::tpRawBinary:
		case dbField::tpString:
		case dbField::tpWString:
		case dbField::tpStdWString:
		case dbField::tpArray:
		case dbField::tpReference:
		case dbField::tpRectangle:
		case dbField::tpStructure:
		default:
			LOGGER_ERROR("ERROR: The field type used for fastdb not supported when loading from database to memory!");
			return false;
		}
	} while ((fd = fd->next) != fdFirst);

	return true;
}

std::string arrayToStr(const dbArray<std::string>& strs);
void strToArray(const std::string& str, dbArray<std::string>& arrays);

//const std::string kSchemaAccount("idb_account");
//const std::string kSchemaBond("idb_bond");
//const std::string kSchemaBase("base_data");

#define SET_PROPERTIES_BEGIN(TableName)\
	if (!CacheMonitor::getInstance()->isFastDBEnable()) {\
		LOGGER_ERROR(TableName::TABLE_NAME << " - fastdb is not open!!!");\
		return;\
		}\
    TableName cache;\
    if(!insertCacheG(&cache, resSet))\
    LOGGER_ERROR(TableName::TABLE_NAME << " - Get fields from sql failed!");\


#define SET_PROPERTIES_END\
	try{\
		CacheCenter::sharedInstance()->AttachCurrentThread();\
		CacheCenter::sharedInstance()->getFastDB()->insert(cache); \
		CacheCenter::sharedInstance()->DetachCurrentThread();\
	} catch (dbException e) {\
		LOGGER_ERROR(e.getMsg());\
	}\

#define INSERT_DATABASE_BEGIN(table_name)\
	std::vector<std::string> fieldList;			 \
	std::vector<std::string> conditionList;			 \
	std::string sqlStr = "insert into " + std::string(table_name) + " set ";\


#define INSERT_STRING_FIELD(field_name)									\
{																		\
	std::ostringstream s;												\
																		\
	s << #field_name << " = " << "\'" << newCache->field_name << "\'";	\
	fieldList.push_back(s.str());										\
}																		\

#define INSERT_NULLABLE_STRING_FIELD(field_name)						\
{																		\
	if (newCache->field_name != "")							            \
		{																	\
		INSERT_STRING_FIELD(field_name)								    \
		}																	\
}																	    \

#define INSERT_DECIMAL_FIELD(field_name)								\
{																		\
	std::ostringstream s;												\
																		\
	s << #field_name << " = " << std::setprecision(16) << newCache->field_name;					\
	fieldList.push_back(s.str());										\
}																		\


#define INSERT_NULLABLE_DECIMAL_FIELD(field_name)						\
{																		\
	if (!doubleEqual(newCache->field_name, DECIMAL_NULL))							\
		{																	\
		INSERT_DECIMAL_FIELD(field_name)								\
		}																	\
}																		\


#define INSERT_INT_FIELD(field_name)								\
	INSERT_DECIMAL_FIELD(field_name)								\


#define INSERT_NULLABLE_INT_FIELD(field_name)						\
	INSERT_NULLABLE_DECIMAL_FIELD(field_name)						\


#define INSERT_DOUBLE_FIELD(field_name)								\
	INSERT_DECIMAL_FIELD(field_name)								\


#define INSERT_NULLABLE_DOUBLE_FIELD(field_name)							\
	INSERT_NULLABLE_DECIMAL_FIELD(field_name)								\


#define INSERT_BOOLEAN_FIELD(field_name)							\
	INSERT_DECIMAL_FIELD(field_name)								\




#define INSERT_DATABASE_END(table_name, schema)         \
	if (fieldList.size() == 0)                 \
		{                                           \
		return 0;                               \
		}                                           \
	std::string fieldString = join(fieldList, ",");	\
	sqlStr += fieldString;						\
	return DBConnector::getInstance()->executeUpdate(sqlStr, schema);						    \


template <class T>
std::string getKeyEqualValue(T &t) {
	std::ostringstream s;
	s << " = " << "\"" << t << "\"";
	return s.str();
}

template <> std::string getKeyEqualValue(bool &t);

template <class T>
std::string getStringValue(T &t) {
	std::ostringstream s;
	s << "\'" << t << "\'";
	return s.str();
}

template <> std::string getStringValue(bool &t);

template <class T>
bool insertDatabase(const std::string& table_name, const std::string& schema, T *newCache) {
	std::vector<std::string> fieldList;
	std::string sqlStr = "insert into " + std::string(table_name) + " set ";

	const T* dst = newCache;
	dbFieldDescriptor* fd = dst->dbDescriptor.getFirstField();
	dbFieldDescriptor* fdFirst = fd;
	if (!fd)
		return false;

	do {
		std::string columnName = fd->name;
		long long dstOffs = (long long)dst + fd->appOffs;
		bool equal = false;

		// combined fields or not updated fields will passed
		if ((fd->indexType & FIELD_NOT_FROM_DISK_DATABASE) || (fd->indexType & FIELD_NOT_UPDATE_TO_DISK_DATABASE))
			continue;

		std::string s = columnName;
		std::ostringstream ss;
		switch (fd->appType) {
		case dbField::tpBool:
			s += getKeyEqualValue(*(bool*)dstOffs);
			break;
		case dbField::tpInt1:
			s += getKeyEqualValue(*(int1*)dstOffs);
			break;
		case dbField::tpInt2:
			s += getKeyEqualValue(*(int2*)dstOffs);
			break;
		case dbField::tpInt4:
			if (*(int4*)dstOffs == DECIMAL_NULL)
				s += " = NULL";
			else
				s += getKeyEqualValue(*(int4*)dstOffs);
			break;
		case dbField::tpInt8:
			if (*(db_int8*)dstOffs == DECIMAL_NULL)
				s += " = NULL";
			else
				s += getKeyEqualValue(*(db_int8*)dstOffs);
			break;
		case dbField::tpReal4:
			if (fabs(*(real4*)dstOffs - DECIMAL_NULL) < PRECISION_VALUE)
				ss << columnName << "= null";
			else
				ss << columnName << "=" << std::setprecision(16) << *(real4*)dstOffs;
			s = ss.str();
			break;
		case dbField::tpReal8:
			if (fabs(*(real8*)dstOffs - DECIMAL_NULL) < PRECISION_VALUE)
				ss << columnName << "= null";
			else
				ss << columnName << "=" << std::setprecision(16) << *(real8*)dstOffs;
			s = ss.str();
			break;
		case dbField::tpStdString:
			if (((std::string*)dstOffs)->empty())
				s += " = NULL";
			else
				s += getKeyEqualValue(*(std::string*)dstOffs);
			break;
		case dbField::tpRawBinary:
		case dbField::tpString:
		case dbField::tpWString:
		case dbField::tpStdWString:
		case dbField::tpArray:
		case dbField::tpReference:
		case dbField::tpRectangle:
		case dbField::tpStructure:
		default:
			LOGGER_ERROR("ERROR: The field type used for fastdb not supported when update database!");
			equal = true;
		}

		fieldList.push_back(s);

	} while ((fd = fd->next) != fdFirst);

	if (fieldList.size() == 0) {
		LOGGER_INFO("No fields to insert into table - [" << table_name << "]");
		return 0;
	}
	std::string fieldString = join(fieldList, ",");
	sqlStr += fieldString;
	//LOGGER_INFO(sqlStr);
	return DBConnector::getInstance()->executeUpdate(sqlStr, schema);
}

template <class T>
bool insertDatabase(const std::vector<std::shared_ptr<T> > &newCacheVecPtr, const int& iCommitCount = 1000) {
	LOGGER_INFO("Begin insert into database..");
	std::vector<std::string> columnList;
	std::vector<std::string> recordsList;
	std::string columnString;
	std::string recordsString;
	std::string sqlStr = "insert into " + T::TABLE_NAME;
	std::vector<std::string> sqls;
	for (int i = 0; i != newCacheVecPtr.size(); ++i) {
		if (i != 0 && i%iCommitCount == 0) {
			LOGGER_INFO("Insert " << iCommitCount << " records")
				if (columnString.empty())
					columnString = " (" + join(columnList, ",") + ") ";

			recordsString = join(recordsList, ",");
			if (!columnString.empty() && !recordsString.empty()) {
				//DBConnector::getInstance()->executeUpdate(sqlStr + columnString + " values " + recordsString, T::SCHEMA_NAME);
				sqls.push_back(sqlStr + columnString + " values " + recordsString);
			}

			recordsList.clear();
			recordsString.clear();
		}

		const T* dst = newCacheVecPtr[i].get();
		dbFieldDescriptor* fd = dst->dbDescriptor.getFirstField();
		dbFieldDescriptor* fdFirst = fd;
		if (!fd)
			return false;

		std::vector<std::string> record;
		do {
			// combined fields or not updated fields will passed
			if ((fd->indexType & FIELD_NOT_FROM_DISK_DATABASE) || (fd->indexType & FIELD_NOT_UPDATE_TO_DISK_DATABASE))
				continue;

			std::string columnName = fd->name;
			if (i == 0)
				columnList.push_back(columnName);

			long long dstOffs = (long long)dst + fd->appOffs;

			std::string value;
			std::ostringstream ss;
			switch (fd->appType) {
			case dbField::tpBool:
				value = getStringValue(*(bool*)dstOffs);
				break;
			case dbField::tpInt1:
				value = getStringValue(*(int1*)dstOffs);
				break;
			case dbField::tpInt2:
				value = getStringValue(*(int2*)dstOffs);
				break;
			case dbField::tpInt4:
				if (*(int4*)dstOffs == DECIMAL_NULL)
					value = "NULL";
				else
					value = getStringValue(*(int4*)dstOffs);
				break;
			case dbField::tpInt8:
				if (*(db_int8*)dstOffs == DECIMAL_NULL)
					value = "NULL";
				else
					value = getStringValue(*(db_int8*)dstOffs);
				break;
			case dbField::tpReal4:
				if (fabs(*(real4*)dstOffs - DECIMAL_NULL) < PRECISION_VALUE)
					ss << "NULL";
				else
					ss << std::setprecision(16) << *(real4*)dstOffs;
				value = ss.str();
				break;
			case dbField::tpReal8:
				if (fabs(*(real8*)dstOffs - DECIMAL_NULL) < PRECISION_VALUE)
					ss << "NULL";
				else
					ss << std::setprecision(16) << *(real8*)dstOffs;
				value = ss.str();
				break;
			case dbField::tpStdString:
				if (((std::string*)dstOffs)->empty())
					value = "NULL";
				else
					value = getStringValue(*(std::string*)dstOffs);
				break;
			case dbField::tpRawBinary:
			case dbField::tpString:
			case dbField::tpWString:
			case dbField::tpStdWString:
			case dbField::tpArray:
			case dbField::tpReference:
			case dbField::tpRectangle:
			case dbField::tpStructure:
			default:
				LOGGER_ERROR("ERROR: The field type used for fastdb not supported when update database!");
			}

			record.push_back(value);

		} while ((fd = fd->next) != fdFirst);

		recordsList.push_back(" (" + join(record, ",") + ") ");
	}
	if (columnString.empty())
		columnString = " (" + join(columnList, ",") + ") ";
	recordsString = join(recordsList, ",");
	if (!columnString.empty() && !recordsString.empty()) {
		LOGGER_INFO("Insert " << recordsList.size() << " records");
		sqlStr += columnString + " values " + recordsString;
		sqls.push_back(sqlStr);
		//bool ret = DBConnector::getInstance()->executeUpdate(sqlStr, T::SCHEMA_NAME);
	}
	if (!sqls.empty())
		DBConnector::getInstance()->executeUpdate(sqls, T::SCHEMA_NAME);
	LOGGER_INFO("End insert into database");

	return true;
}

template <class T>
bool updateDatabase(const std::string& table_name, const std::string& schema, T *newCache, T *oldCache, const std::string& conditionColumn) {
	std::vector<std::string> conditionColumns;
	conditionColumns.push_back(conditionColumn);
	return updateDatabase(table_name, schema, newCache, oldCache, &conditionColumns);
}

template <class T>
bool updateDatabase(const std::string& table_name, const std::string& schema, T *newCache, T *oldCache, std::vector<std::string> *conditionColumns) {
	std::vector<std::string> fieldList;
	std::vector<std::string> conditionList;
	std::string sqlStr = "update " + std::string(table_name) + " set ";

	const T* dst = newCache;
	const T* src = oldCache;
	dbFieldDescriptor* fd = dst->dbDescriptor.getFirstField();
	dbFieldDescriptor* fdFirst = fd;
	if (!fd)
		return false;

	do {
		std::string columnName = fd->name;
		long long dstOffs = (long long)dst + fd->appOffs;
		long long srcOffs = (long long)src + fd->appOffs;
		bool equal = false;

		// combined fields or not updated fields will passed
		if ((fd->indexType & FIELD_NOT_FROM_DISK_DATABASE) || (fd->indexType & FIELD_NOT_UPDATE_TO_DISK_DATABASE))
			continue;

		bool isCondition = ((conditionColumns != NULL) && (std::find(conditionColumns->begin(), conditionColumns->end(), columnName) != conditionColumns->end()));

		std::string s = columnName;
		std::ostringstream ss;
		switch (fd->appType) {
		case dbField::tpBool:
			equal = *(bool*)dstOffs == *(bool*)srcOffs;
			if (isCondition) {
				s += getKeyEqualValue(*(bool*)srcOffs);
				break;
			}
			s += getKeyEqualValue(*(bool*)dstOffs);
			break;
		case dbField::tpInt1:
			equal = *(int1*)dstOffs == *(int1*)srcOffs;
			if (isCondition) {
				s += getKeyEqualValue(*(int1*)srcOffs);
				break;
			}
			s += getKeyEqualValue(*(int1*)dstOffs);
			break;
		case dbField::tpInt2:
			equal = *(int2*)dstOffs == *(int2*)srcOffs;
			if (isCondition) {
				s += getKeyEqualValue(*(int2*)srcOffs);
				break;
			}
			s += getKeyEqualValue(*(int2*)dstOffs);
			break;
		case dbField::tpInt4:
			equal = *(int4*)dstOffs == *(int4*)srcOffs;
			if (isCondition) {
				s += getKeyEqualValue(*(int4*)srcOffs);
				break;
			}
			if (*(int4*)dstOffs == DECIMAL_NULL)
				s += " = NULL";
			else
				s += getKeyEqualValue(*(int4*)dstOffs);
			break;
		case dbField::tpInt8:
			equal = *(db_int8*)dstOffs == *(db_int8*)srcOffs;
			if (isCondition) {
				s += getKeyEqualValue(*(db_int8*)srcOffs);
				break;
			}
			if (*(db_int8*)dstOffs == DECIMAL_NULL)
				s += " = NULL";
			else
				s += getKeyEqualValue(*(db_int8*)dstOffs);
			break;
		case dbField::tpReal4:
			equal = fabs(*(real4*)dstOffs - *(real4*)srcOffs) < PRECISION_VALUE;
			if (isCondition) {
				s += getKeyEqualValue(*(real4*)srcOffs);
				break;
			}

			if (fabs(*(real4*)dstOffs - DECIMAL_NULL) < PRECISION_VALUE)
				ss << columnName << "= null";
			else
				ss << columnName << "=" << std::setprecision(16) << *(real4*)dstOffs;
			s = ss.str();
			break;
		case dbField::tpReal8:
			equal = fabs(*(real8*)dstOffs - *(real8*)srcOffs) < PRECISION_VALUE;
			if (isCondition) {
				s += getKeyEqualValue(*(real8*)srcOffs);
				break;
			}

			if (fabs(*(real8*)dstOffs - DECIMAL_NULL) < PRECISION_VALUE)
				ss << columnName << "= null";
			else
				ss << columnName << "=" << std::setprecision(16) << *(real8*)dstOffs;
			s = ss.str();
			break;
		case dbField::tpStdString:
			equal = *(std::string*)dstOffs == *(std::string*)srcOffs;
			if (isCondition) {
				s += getKeyEqualValue(*(std::string*)srcOffs);
				break;
			}

			if (((std::string*)dstOffs)->empty())
				s += " = NULL";
			else
				s += getKeyEqualValue(*(std::string*)dstOffs);
			break;
		case dbField::tpRawBinary:
		case dbField::tpString:
		case dbField::tpWString:
		case dbField::tpStdWString:
		case dbField::tpArray:
		case dbField::tpReference:
		case dbField::tpRectangle:
		case dbField::tpStructure:
		default:
			LOGGER_ERROR("ERROR: The field type used for fastdb not supported when update database!");
			equal = true;
		}
		if (isCondition)
			conditionList.push_back(s);

		if (equal)
			continue;

		fieldList.push_back(s);

	} while ((fd = fd->next) != fdFirst);

	if (fieldList.size() == 0) {
		return 0;
	}
	std::string fieldString = join(fieldList, ",");
	sqlStr += fieldString;
	sqlStr += " Where ";
	std::string conditionString = join(conditionList, " AND ");
	sqlStr += conditionString;
	//LOGGER_INFO(sqlStr);
	return DBConnector::getInstance()->executeUpdate(sqlStr, schema);
}

template <class T>
std::string getUpdateDBSQL(const std::string& table_name, const std::string& schema, T *newCache, T *oldCache, const std::string& conditionColumn) {
	std::vector<std::string> conditionColumns;
	conditionColumns.push_back(conditionColumn);
	return getUpdateDBSQL(table_name, schema, newCache, oldCache, &conditionColumns);
}

template <class T>
std::string getUpdateDBSQL(const std::string& table_name, const std::string& schema, T *newCache, T *oldCache, std::vector<std::string> *conditionColumns) {
	std::vector<std::string> fieldList;
	std::vector<std::string> conditionList;
	std::string sqlStr = "update " + std::string(table_name) + " set ";

	const T* dst = newCache;
	const T* src = oldCache;
	dbFieldDescriptor* fd = dst->dbDescriptor.getFirstField();
	dbFieldDescriptor* fdFirst = fd;
	if (!fd)
		return "";

	do {
		std::string columnName = fd->name;
		long long dstOffs = (long long)dst + fd->appOffs;
		long long srcOffs = (long long)src + fd->appOffs;
		bool equal = false;

		// combined fields or not updated fields will passed
		if ((fd->indexType & FIELD_NOT_FROM_DISK_DATABASE) || (fd->indexType & FIELD_NOT_UPDATE_TO_DISK_DATABASE))
			continue;

		bool isCondition = ((conditionColumns != NULL) && (std::find(conditionColumns->begin(), conditionColumns->end(), columnName) != conditionColumns->end()));

		std::string s = columnName;
		std::ostringstream ss;
		switch (fd->appType) {
		case dbField::tpBool:
			equal = *(bool*)dstOffs == *(bool*)srcOffs;
			if (isCondition) {
				s += getKeyEqualValue(*(bool*)srcOffs);
				break;
			}
			s += getKeyEqualValue(*(bool*)dstOffs);
			break;
		case dbField::tpInt1:
			equal = *(int1*)dstOffs == *(int1*)srcOffs;
			if (isCondition) {
				s += getKeyEqualValue(*(int1*)srcOffs);
				break;
			}
			s += getKeyEqualValue(*(int1*)dstOffs);
			break;
		case dbField::tpInt2:
			equal = *(int2*)dstOffs == *(int2*)srcOffs;
			if (isCondition) {
				s += getKeyEqualValue(*(int2*)srcOffs);
				break;
			}
			s += getKeyEqualValue(*(int2*)dstOffs);
			break;
		case dbField::tpInt4:
			equal = *(int4*)dstOffs == *(int4*)srcOffs;
			if (isCondition) {
				s += getKeyEqualValue(*(int4*)srcOffs);
				break;
			}
			if (*(int4*)dstOffs == DECIMAL_NULL)
				s += " = NULL";
			else
				s += getKeyEqualValue(*(int4*)dstOffs);
			break;
		case dbField::tpInt8:
			equal = *(db_int8*)dstOffs == *(db_int8*)srcOffs;
			if (isCondition) {
				s += getKeyEqualValue(*(db_int8*)srcOffs);
				break;
			}
			if (*(db_int8*)dstOffs == DECIMAL_NULL)
				s += " = NULL";
			else
				s += getKeyEqualValue(*(db_int8*)dstOffs);
			break;
		case dbField::tpReal4:
			equal = fabs(*(real4*)dstOffs - *(real4*)srcOffs) < PRECISION_VALUE;
			if (isCondition) {
				s += getKeyEqualValue(*(real4*)srcOffs);
				break;
			}

			if (fabs(*(real4*)dstOffs - DECIMAL_NULL) < PRECISION_VALUE)
				ss << columnName << "= null";
			else
				ss << columnName << "=" << std::setprecision(16) << *(real4*)dstOffs;
			s = ss.str();
			break;
		case dbField::tpReal8:
			equal = fabs(*(real8*)dstOffs - *(real8*)srcOffs) < PRECISION_VALUE;
			if (isCondition) {
				s += getKeyEqualValue(*(real8*)srcOffs);
				break;
			}

			if (fabs(*(real8*)dstOffs - DECIMAL_NULL) < PRECISION_VALUE)
				ss << columnName << "= null";
			else
				ss << columnName << "=" << std::setprecision(16) << *(real8*)dstOffs;
			s = ss.str();
			break;
		case dbField::tpStdString:
			equal = *(std::string*)dstOffs == *(std::string*)srcOffs;
			if (isCondition) {
				s += getKeyEqualValue(*(std::string*)srcOffs);
				break;
			}

			if (((std::string*)dstOffs)->empty())
				s += " = NULL";
			else
				s += getKeyEqualValue(*(std::string*)dstOffs);
			break;
		case dbField::tpRawBinary:
		case dbField::tpString:
		case dbField::tpWString:
		case dbField::tpStdWString:
		case dbField::tpArray:
		case dbField::tpReference:
		case dbField::tpRectangle:
		case dbField::tpStructure:
		default:
			LOGGER_ERROR("ERROR: The field type used for fastdb not supported when update database!");
			equal = true;
		}
		if (isCondition)
			conditionList.push_back(s);

		if (equal)
			continue;

		fieldList.push_back(s);

	} while ((fd = fd->next) != fdFirst);

	if (fieldList.size() == 0) {
		return "";
	}
	std::string fieldString = join(fieldList, ",");
	sqlStr += fieldString;
	sqlStr += " Where ";
	std::string conditionString = join(conditionList, " AND ");
	sqlStr += conditionString;
	return sqlStr;
}

#define UPDATE_DATABASE_BEGIN(table_name)\
	std::vector<std::string> fieldList;			 \
	std::vector<std::string> conditionList;			 \
	std::string sqlStr = "update " + std::string(table_name) + " set ";\


#define UPDATE_STRING_FIELD(field_name)									\
if (newCache->field_name != oldCache->field_name)						\
{																		\
	std::ostringstream s;												\
																		\
	s << #field_name << " = " << "\'" << newCache->field_name << "\'";  \
	fieldList.push_back(s.str());										\
}																		\

#define UPDATE_NULLABLE_STRING_FIELD(field_name)						\
if (newCache->field_name != oldCache->field_name)				    	\
{																		\
    std::ostringstream s;												\
                                                                        \
    if (newCache->field_name == "") {                                   \
        s << #field_name << " = NULL";                                  \
	    }                                                                   \
	    else {                                                              \
        s << #field_name << " = " << "\'" << newCache->field_name << "\'";  \
    }                                                                   \
    fieldList.push_back(s.str());										\
}																		\

#define CONDITION_STRING_FIELD(field_name)								\
{																		\
	std::ostringstream s;												\
																		\
	s << #field_name << " = " << "\'" << oldCache->field_name << "\'";  \
	conditionList.push_back(s.str());									\
}																		\



#define UPDATE_DECIMAL_FIELD(field_name)				\
if (!doubleEqual(newCache->field_name, oldCache->field_name))		\
{														\
	std::ostringstream s;								\
														\
	s << #field_name << "=" << std::setprecision(16) << newCache->field_name; 	\
	fieldList.push_back(s.str());						\
}														\

#define UPDATE_NULLABLE_DECIMAL_FIELD(field_name)		\
if (!doubleEqual(newCache->field_name, oldCache->field_name))		\
{														\
	std::ostringstream s;								\
	if (doubleEqual(newCache->field_name, DECIMAL_NULL)) {			\
		s << #field_name << "= null";					\
		}													\
		else {												\
		s << #field_name << "=" << std::setprecision(7) << newCache->field_name; 	\
	}													\
	fieldList.push_back(s.str());						\
}														\




#define UPDATE_DATABASE_END(table_name, schema)         \
    if (fieldList.size() == 0)                 \
	    {                                           \
        return 0;                               \
	    }                                           \
	std::string fieldString = join(fieldList, ",");	\
	sqlStr += fieldString;						\
	sqlStr += " Where ";						\
	std::string conditionString = join(conditionList, " AND ");								\
    sqlStr += conditionString;															\
	return DBConnector::getInstance()->executeUpdate(sqlStr, schema);						    \


#define UPDATE_INT_FIELD(field_name)\
	UPDATE_DECIMAL_FIELD(field_name) \

#define UPDATE_NULLABLE_INT_FIELD(field_name)\
	UPDATE_NULLABLE_DECIMAL_FIELD(field_name) \


#define UPDATE_DOUBLE_FIELD(field_name)\
	UPDATE_DECIMAL_FIELD(field_name)\

#define UPDATE_NULLABLE_DOUBLE_FIELD(field_name)\
	UPDATE_NULLABLE_DECIMAL_FIELD(field_name)\


#define UPDATE_BOOLEAN_FIELD(field_name)\
	UPDATE_DECIMAL_FIELD(field_name)\


#define DELETE_DATABASE_BEGIN(table_name)\
	std::vector<std::string> fieldList;			 \
	std::vector<std::string> conditionList;			 \
	std::string sqlStr = "delete from " + std::string(table_name) + " ";\


#define DELETE_DATABASE_END(primary_name, schema)         \
	sqlStr += " Where " + primary_name + " in ";						\
	std::string conditionString = join(primaryKeyList, ",");								\
	sqlStr += "(" + conditionString + ")";															\
	return DBConnector::getInstance()->executeUpdate(sqlStr, schema);						    \


#define TIME_COST_BEGIN(log_note)\
    clock_t t1, t2; \
    t1 = clock(); \

#define TIME_COST_END(log_note)\
    t2 = clock(); \
    LOGGER_INFO(log_note << "... - " <<  " Time costing: " << (t2 - t1)*1000.0/CLOCKS_PER_SEC << "ms");



#define OVERLOAD_ASSIGNMENT_OPERATOR_BEGIN(ClassName)\
inline ClassName& operator = (const ClassName& cache) {      \
    if (&cache == this)                                      \
	    {                                                      \
        return *this;                                      \
	    }                                                      \


#define OVERLOAD_ASSIGNMENT_OPERATOR_PROPERTY(property_name)\
    this->property_name = cache.property_name; \


#define OVERLOAD_ASSIGNMENT_OPERATOR_END                   \
    return *this;                                           \
}                                                           \


#define SAFE_DELTE_PROPERTY(property_name)    \
if (property_name != NULL)            \
{                                \
    delete property_name;            \
    property_name = NULL;            \
}									\


#endif // _CACHE_DEF_H_

