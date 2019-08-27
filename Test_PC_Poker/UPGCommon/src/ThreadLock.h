#pragma once

class UPG_COMMON_EXPORT CThreadLock
{
protected:
	CRITICAL_SECTION	*m_pSection;
public:
	CThreadLock(LPCRITICAL_SECTION pSection);
	virtual ~CThreadLock();
};