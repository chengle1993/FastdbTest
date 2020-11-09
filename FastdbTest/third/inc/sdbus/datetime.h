
#ifndef __SDBUSAPI_DATETIME_H
#define __SDBUSAPI_DATETIME_H 1

#include "sdbus/compile.h"
#include "sdbus/type.h"
#include "sdbus/string.h"
#include <time.h>

namespace sdbus{

/*
	由于是同一天，所以没有必要在时间中保存日期的信息，将日期单独是个明智的处理。
	对于时间来说，每天3600*24 = 86400秒，是个常量。

	这里年月日，都是表示本地时间。

	关于time.h中几个重要函数解释。
	time(NULL) ；返回的值，同本地时间到EPOCH的秒数相同
	mktime(struct tm)；也是基于本地时间进行计算
	除非特别指出，这里的时间秒数，都是基于本地时间

    32位的linux的time_t是32位，windows和64位linux都是64位。请注意在32位linux下的溢出问题。    
*/
class SDBUSAPI Date{
public:
    Date() ;
    Date(const Date& date) ;
    Date(time_t ts) ;
    Date(int year , int month , int day) ;
    void Reset() ;

    Date& operator=(time_t ts) ;
    bool operator==(time_t ts) const;
    bool operator!=(time_t ts) const;
    bool operator<(time_t ts) const;
    bool operator<=(time_t ts) const;
    bool operator>(time_t ts) const;
    bool operator>=(time_t ts) const;
    
    Date& operator=(const Date& date) ;
	bool operator==(const Date& date) const;
	bool operator!=(const Date& date) const;
	bool operator<(const Date& date) const;
	bool operator<=(const Date& date) const;
	bool operator>(const Date& date) const;
	bool operator>=(const Date& date) const;


    const sdbus::string& ToString(char separator = 0) const ;
    time_t Time() const;
    void Time(time_t ts) ;
    time_t GMTime() const ;

    inline int Year() const {return year_ ;}
    inline int Month() const {return month_ ;}
    inline int Day() const {return day_ ;}
    inline int WDay() const {return wday_ ;}

    Date& NextDay(int dc = 1) ;

    void CalcNow() ;

    static const Date& Today() ;
    static int TimeZone() ;

private:
    time_t time_ ;
    int year_ ;     //年 2014
    int month_ ;    //月 1...12
    int day_ ;      //日期1...31
    int wday_ ;     //周 1..7

    mutable sdbus::string datestr_ ;
    mutable int stage_ ;

    int Compare(const Date& date) const ;
    int Compare(time_t t) const ;

    void Assign(const Date& date) ;
    void Assign(time_t t) ;
} ;

class SDBUSAPI STime {
public:
    STime() ;
    STime(time_t t) ;
    STime(const STime& st) ;

    ~STime() ;

    STime& operator=(time_t t) ;
    bool operator==(time_t t) const ;
    bool operator!=(time_t t) const ;
    bool operator <(time_t t) const ;
    bool operator<=(time_t t) const ;
    bool operator >(time_t t) const ;
    bool operator>=(time_t t) const ;

    STime& operator=(const STime& st) ;
    bool operator==(const STime& st) const ;
    bool operator!=(const STime& st) const ;
    bool operator <(const STime& st) const ;
    bool operator<=(const STime& st) const ;
    bool operator >(const STime& st) const ;
    bool operator>=(const STime& st) const ;

    void Reset() ;
    void CalcNow() ;

    inline time_t Time() const {return time_ ;}
    inline int Hour() const {return hour_ ;}
    inline int Minute() const {return minute_ ;}
    inline int Second() const {return second_ ;}

    const sdbus::string& ToString() const ;

    /*
        返回距离当天0点的秒数
    */
    static time_t Now() ;
private:
    time_t time_ ;
    int hour_ ;
    int minute_ ;
    int second_ ;

    mutable sdbus::string timestr_ ;
    mutable int stage_ ;

    int Compare(time_t t) const ;
    int Compare(const STime& st) const ;
    void Assign(time_t t) ;
    void Assign(const STime& st) ;
} ;

SDBUSAPI bool LocalTime(struct tm * tm , time_t t) ;
SDBUSAPI bool GMTime(struct tm * tm , time_t t) ;
SDBUSAPI time_t MakeTime(const struct tm * tm) ;

/*
    将时间秒数t转换成本地时间，当天零点时的秒数。
*/
SDBUSAPI bool DateStartTime(time_t t , time_t& ts , int& year , int& month , int& day , int& wday) ;
SDBUSAPI bool DateStartTime(time_t t , time_t& ts) ;

/*
    计算从当天0点开始到该时刻的秒数
*/
SDBUSAPI bool DateLeftTime(time_t t , time_t& left) ;

/*
    解析日期时间。支持格式为"YYYY-MM-DD HH:MM:SS.mmm"或者"YYYYMMDD HH:MM:SS.mmm" 或者"YYYY/MM/DD HH:MM:SS.mmm"
    如果日期有分隔符，那么可以不补充0
*/
SDBUSAPI bool ParseTimeStr(const char * str , int&year , int& month , int& day , int& hour , int& minute , int& second , int& msec) ;

SDBUSAPI time_t MakeTime(int year , int month , int day , int hour = 0, int minute = 0, int second = 0) ;


}

#endif  /** __SDBUSAPI_DATETIME_H */
