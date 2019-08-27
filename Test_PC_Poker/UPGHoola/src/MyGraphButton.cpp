// MyGraphButton.cpp: implementation of the CMyGraphButton class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyGraphButton.h"
//#include "mmsystem.h"
//#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyGraphButton::CMyGraphButton()
{

}

CMyGraphButton::~CMyGraphButton()
{

}

void CMyGraphButton::Draw(CDC *pDC, int opt, int grade)
{
	if(m_pPage == NULL) return;
	if(m_pSpr == NULL) return;
	if(m_bShow == FALSE) return;

	// 버튼 그리기
	if(m_bEnable==FALSE)
	{
		m_pPage->DrawMultiScene( m_Xp, m_Yp, m_pSpr, 3, NULL );
	}
	else if(m_bOver==TRUE && m_bClick==FALSE)
	{
		m_pPage->DrawMultiScene( m_Xp, m_Yp, m_pSpr, 1, timeGetTime() - m_dwStartTime );//abs 없앰
	}
	else if(m_bClick==FALSE) 
	{
		m_pPage->DrawMultiScene( m_Xp, m_Yp, m_pSpr, 0, timeGetTime() - m_dwStartTime );
	}
	else if(m_bClick==TRUE)
	{
		m_pPage->DrawMultiScene( m_Xp, m_Yp, m_pSpr, 2, timeGetTime() - m_dwStartTime );
	}

	return;
}

void CMyGraphButton::SetStartTime()
{
	m_dwStartTime = timeGetTime();
}


BOOL CMyGraphButton::OnLButtonDown(int x, int y)
{
	if(m_bEnable==FALSE) return FALSE;

	if(x >= m_Xp && x <= m_Xp+m_Width && y >= m_Yp && y <= m_Yp+m_Height) 
	{
		m_bClick = TRUE;
		SetStartTime();
	}
	else 
	{
		m_bClick = FALSE; 
		return FALSE; 
	}

	return TRUE;
}

BOOL CMyGraphButton::OnLButtonUp(int x, int y)
{
	if(m_bEnable==FALSE) return FALSE;

	if(m_bClick)
	{
		m_bClick = FALSE;
		if(x >= m_Xp && x <= m_Xp+m_Width && y >= m_Yp && y <= m_Yp+m_Height)
		{
			// 버튼 눌림 메세지를 보냄
			m_pParent->SendMessage(WM_COMMAND, m_CmndID, NULL);
			SetStartTime();

			return TRUE;
		}
	}
	m_bClick = FALSE;
	return FALSE;
}

void CMyGraphButton::Enable(BOOL bVal)
{
	if(bVal==FALSE)
	{
		m_bClick = FALSE;
		m_bOver = FALSE;
	}
	m_bEnable = bVal;

	SetStartTime();
}

void CMyGraphButton::Show(BOOL bVal)
{
	CMyGraphButton::Enable( bVal );

	m_bShow = bVal;
}

void CMyGraphButton::SetSize( int nWidth, int nHeight )
{
	m_Width = nWidth;
	m_Height = nHeight;
}
