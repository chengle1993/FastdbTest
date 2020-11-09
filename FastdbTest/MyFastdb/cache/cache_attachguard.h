#ifndef _CACHE_ATTACHGUARD_H_
#define _CACHE_ATTACHGUARD_H_

class CacheAttachGuard {
public:
	explicit CacheAttachGuard(int flags);
	~CacheAttachGuard();

private:
	/*
	对应database的DetachFlags
	0-precommit 只释放锁，而不把数据更新推送至fastDB
	1-commit 释放锁，并将数据推送至fastDB，在批量更新结束时候使用
	2-destroycontext 销毁线程上下文对象，一般线程结束使用
	3-1|2
	*/
	int m_detachflags;
};

#endif // _CACHE_ATTACHGUARD_H_
