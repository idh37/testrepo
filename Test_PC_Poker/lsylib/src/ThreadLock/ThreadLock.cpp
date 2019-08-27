// WhisperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ThreadLock.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace LSY
{
CThreadLock::CThreadLock(LPCRITICAL_SECTION pSection)
{
	m_pSection = pSection;
	EnterCriticalSection(m_pSection);
}

CThreadLock::~CThreadLock()
{
	if(m_pSection) LeaveCriticalSection(m_pSection);
	m_pSection = NULL;
}
} //namespace LSY