/***************************************************************************************************
  *Copyright(C),2010-2017,Sumscope
  *FileName	:  helper.h
  *Author	:  scofined.qi
  *Version	:  1.0
  *Date		:  2017/10/27
  *Desc		:  //用于主要说明此程序文件完成的主要功能
  *Relation :  
  *Others	:  //其他内容说明
  *Function :  //主要函数列表，每条记录应包含函数名及功能简要说明
  *History	:  //修改历史记录列表，每条修改记录应包含修改日期、修改者及修改内容简介
***************************************************************************************************/
#ifndef HELPER_H_5BAA5D36_47EE_4D3E_BD24_E3BF13A30200
#define HELPER_H_5BAA5D36_47EE_4D3E_BD24_E3BF13A30200
#include "sdbus/compile.h"
namespace sdbus{
	enum  kLogLevel{
		kLogLevelEmergency = 0,	// 紧急,用于无论何种条件都必须写日志的场合
		kLogLevelCritical,		// 致命,默认会导致映射文件重新刷新
		kLogLevelError,			// 错误,发生某个错误的情况
		kLogLevelAlert,			// 严重,
		kLogLevelWarn,			// 警告
		kLogLevelNotice,		// 通知
		kLogLevelInfo,			// 信息
		kLogLevelDebug,			// 调试
		kLogLevelDebugview,		// 工具,新增特殊等级日志,不受等级个数限制,只受kBaseConfigLogToDebugView控制
		kLogLevelMax
	};
	SDBUSAPI void raise_exception(const char *errmsg_);
	SDBUSAPI void write_log(bool raise, int level, char* file, char* func, int line, const char* format, ...);

	class SDBUSAPI thread_name_t{
		const char*		m_file;
		const char*		m_func;
		int				m_line;
		unsigned int	m_tid;
		double			m_time;
		char			m_name[128];
	public:
		thread_name_t(unsigned int tid, const char* file, const char* func, int line, const char* format, ...);
		~thread_name_t();
	};
	class SDBUSAPI code_metrics_t{
		double	tick_start_;
		char	message_[1024];
	public:
		code_metrics_t(char* file, char* func, int line, const char* format, ...);
		~code_metrics_t();
	};
}
#define sdbus_code_metric(fmt,...)	sdbus::code_metrics_t __localCodeMetrics(__FILE__,__FUNCTION__,__LINE__,fmt,__VA_ARGS__);
#define sdbuslog(level,fmt,...)	sdbus::write_log(false,level,__FILE__,__FUNCTION__,__LINE__,fmt,__VA_ARGS__);
#define sdbus_thread_name_ex(tid,fmt,...)  sdbus::thread_name_t _thread_name_(tid,__FILE__,__FUNCTION__,__LINE__,fmt,__VA_ARGS__)
#define sdbus_thread_name(fmt,...)  sdbus::thread_name_t _thread_name_(-1,__FILE__,__FUNCTION__,__LINE__,fmt,__VA_ARGS__)


#endif
