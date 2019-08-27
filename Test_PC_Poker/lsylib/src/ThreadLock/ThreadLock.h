#pragma once

namespace LSY
{
class UPG_LSYLIB_EXPORT CThreadLock
{
protected:
	CRITICAL_SECTION	*m_pSection;
public:
	CThreadLock(LPCRITICAL_SECTION pSection);
	virtual ~CThreadLock();
};
} //namespace LSY