// SprDlgObjectMan.h: interface for the CSprDlgObjectMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRDLGOBJECTMAN_H__CEB8BDD4_B24E_4C31_9C5E_BBE4BC8CFA37__INCLUDED_)
#define AFX_SPRDLGOBJECTMAN_H__CEB8BDD4_B24E_4C31_9C5E_BBE4BC8CFA37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SprDlgBase.h"
#include "UserStruct.h"
#include "../Player/Player_NP.h"

//TipDlg
#define IDM_TIPPREBTN			(WM_USER+12100)
#define IDM_TIPNEXTBTN			(WM_USER+12101)
#define IDM_TIPOKBTN			(WM_USER+12102)

class CSprDlgTip  : public CSprDlgBase 
{
public:
	CSprDlgTip();	
	virtual ~CSprDlgTip();

	CWnd			*m_pParent;
	NMBASE::GRAPHICGDI::CPage			*m_pPage;

	NMBASE::SKINGDI::CGraphButton	m_btnPre;
	NMBASE::SKINGDI::CGraphButton	m_btnNext;
	NMBASE::SKINGDI::CGraphButton	m_btnOK;

	int				m_nWidth;
	int				m_nHeight;
	
	BOOL			m_bSDlgMove;
	CPoint			m_ptM;	

	int				m_nCurPageIndex;
	BOOL			m_bNeverShowAgain;
	CRect			m_rtShowTip;
	DWORD			m_dwDrawStartTime;
	
	BOOL Init(CWnd *pWnd,NMBASE::GRAPHICGDI::CPage *page , int x, int y, ENUM_SDLGKIND kind);		//시작 좌표 넣는다.
	BOOL ClickCheck(int x, int y);

	virtual void SDlg_SetPosition(int x, int y);
	virtual	BOOL SDlg_LButtonDown(int x, int y);
	virtual	BOOL SDlg_LButtonUp(int x, int y);
	virtual	BOOL SDlg_OnMouseMove(int x, int y);
	virtual void SDlg_Draw(CDC *pDC);
	virtual void SDlg_Destroy();
	virtual int  SDlg_ProcessObject();		
};

#endif // !defined(AFX_SPRDLGOBJECTMAN_H__CEB8BDD4_B24E_4C31_9C5E_BBE4BC8CFA37__INCLUDED_)
