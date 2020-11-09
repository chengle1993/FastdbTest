#include "cache_attachguard.h"
#include "cache_center.h"

CacheAttachGuard::CacheAttachGuard(int flags) {
	m_detachflags = flags;
	CacheCenter::sharedInstance()->AttachCurrentThread();
}

CacheAttachGuard::~CacheAttachGuard() {
	CacheCenter::sharedInstance()->DetachCurrentThread(m_detachflags);
}
