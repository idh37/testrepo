#if !defined(AFX_MYMISC_H__225A2DD6_5B1B_4782_9C08_540FE4904B6A__INCLUDED_)
#define AFX_MYMISC_H__225A2DD6_5B1B_4782_9C08_540FE4904B6A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <Afx.h>
#include <afxmt.h>

class AutoLock
{
public:
	CCriticalSection* m_pCS;

	AutoLock(CCriticalSection* pCS)
	{
		m_pCS = pCS;
		m_pCS->Lock();
	}

	~AutoLock()
	{
		m_pCS->Unlock();
	}
};

BOOL CreateMultipleDirectory(CString dd);
long RemoveDirectoryAll(CString cstrBasePath, BOOL bRecursive=TRUE);


#endif // !defined(AFX_MYMISC_H__225A2DD6_5B1B_4782_9C08_540FE4904B6A__INCLUDED_)