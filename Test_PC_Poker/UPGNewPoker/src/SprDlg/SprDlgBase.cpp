// SprDlgBase.cpp: implementation of the CSprDlgBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SprDlgBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSprDlgBase::CSprDlgBase()
{
	m_pPreNode = m_pNextNode = NULL;

	m_nSprDlgKind = SDLG_NONE;	//다이얼로그 종류
	m_nDrawStep = 0;		//그리기 순서

	m_nDXp = 0;		//그리는 시작점
	m_nDYp = 0;

	m_nMXp = 0;		//마우스 좌표
	m_nMYp = 0;		//마우스 좌표
	m_nResult = 0;

}

CSprDlgBase::~CSprDlgBase()
{
	m_nDrawStep = 0;
}
