// SprDlgObjectMan.cpp: implementation of the CSprDlgObjectMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SprDlgObjectMan.h"
#include "NewDSLvExpMan.h"//새로운 도신지수 레벨 2006.2.22
#include "../resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////

CSprDlgTip::CSprDlgTip()
{
	m_pPage = NULL;
}

CSprDlgTip::~CSprDlgTip()
{

}

BOOL CSprDlgTip::Init(CWnd *pWnd,NMBASE::GRAPHICGDI::CPage *page , int x, int y, ENUM_SDLGKIND kind)		//시작 좌표 넣는다.
{
	if(page ==NULL){m_nResult = SPRDLG_PROCESS_KILLME;return FALSE;}
	if(pWnd ==NULL){m_nResult = SPRDLG_PROCESS_KILLME;return FALSE;}	

// 	if( g_TipManDlg.TipBackSpr.GetTotalSpr() <= 0 ){m_nResult = SPRDLG_PROCESS_KILLME;return FALSE;}
// 	
// 	m_nWidth = g_TipManDlg.TipBackSpr.spr[13].xl;
// 	m_nHeight= g_TipManDlg.TipBackSpr.spr[13].yl;

	if( m_nHeight <= 0 || m_nWidth <= 0 ){m_nResult = SPRDLG_PROCESS_KILLME;return FALSE;}	

	m_nSprDlgKind = kind;	//다이얼로그 종류
	m_pParent = pWnd;
	m_pPage = page;	

	m_nDXp = 324;		//그리는 시작점
	m_nDYp = 211;				

// 	m_btnPre.Init(m_pParent , m_pPage, m_nDXp+10, m_nDYp+5, &g_TipManDlg.TipBackSpr, 0, IDM_TIPPREBTN);				//확인 버튼 		
// 	m_btnNext.Init(m_pParent , m_pPage, m_nDXp+100, m_nDYp+5, &g_TipManDlg.TipBackSpr, 1, IDM_TIPNEXTBTN);				//확인 버튼 		
// 	m_btnOK.Init(m_pParent , m_pPage, m_nDXp+263, m_nDYp+226, &g_TipManDlg.TipBackSpr, 2, IDM_TIPOKBTN);				//확인 버튼 		
	
	m_bSDlgMove = FALSE;
	m_ptM = NULL;
//	m_nDrawStep = 1;
	m_nCurPageIndex = 0;

	m_bNeverShowAgain = g_Config.bShowTip;

	m_rtShowTip.SetRect(0,0,17,17);
	m_rtShowTip.OffsetRect(m_nDXp+18,m_nDYp+230);

	return TRUE;
}

BOOL CSprDlgTip::ClickCheck(int x, int y)
{
	

	if(x >= m_nDXp && x <= m_nDXp + m_nWidth && y >= m_nDYp && y <= m_nDYp + m_nHeight){
		return TRUE;
	}
	return FALSE;
}
BOOL CSprDlgTip::SDlg_LButtonDown(int x, int y)
{
	if(!ClickCheck(x,y))return FALSE;
	
	
	if(!m_bSDlgMove)
	{
		m_ptM.x = x;
		m_ptM.y = y;
		m_bSDlgMove = TRUE;
	}

	if( m_btnOK.OnLButtonDown(x,y) )return TRUE;
	if( m_btnPre.OnLButtonDown(x,y))return TRUE;
	if( m_btnNext.OnLButtonDown(x,y))return TRUE;
	return FALSE;
}

BOOL CSprDlgTip::SDlg_LButtonUp(int x, int y)
{
	m_bSDlgMove = FALSE;
	m_ptM = NULL;	
	
	
	CPoint pt;
	pt.x = x ;
	pt.y = y ;
	if(m_rtShowTip.PtInRect(pt)){		
		m_bNeverShowAgain = !m_bNeverShowAgain;
		
		g_Config.bShowTip = m_bNeverShowAgain;
		if(m_bNeverShowAgain){
			ShowMainMessageDlg("다음부터는 Tip을 표시하지 않도록 설정하셨습니다.\n상단의 Tip버튼을 누르면 Tip을 다시 볼 수 있습니다.");
		}
	}

	if(!ClickCheck(x,y))return FALSE ;
	if( m_btnOK.OnLButtonUp(x,y) ) return TRUE;	
	if( m_btnPre.OnLButtonUp(x,y) ) return TRUE;
	if( m_btnNext.OnLButtonUp(x,y) )return TRUE;

	return FALSE;
}

BOOL CSprDlgTip::SDlg_OnMouseMove(int x, int y)
{
	
	// SDlg 이동 한다..처리하자
	if(m_bSDlgMove){
		int px = x - m_ptM.x;
		int py = y - m_ptM.y;
		SDlg_SetPosition(px,py);
		m_ptM.x = x;
		m_ptM.y = y;
	}

	if(!ClickCheck(x,y))return FALSE ;
	if( m_btnOK.OnMouseMove(x,y) )return TRUE;	
	if( m_btnPre.OnMouseMove(x,y) ) return TRUE;
	if( m_btnNext.OnMouseMove(x,y) ) return TRUE;

	return FALSE;

}

void CSprDlgTip::SDlg_SetPosition(int x, int y)
{	
	int xp = m_nDXp;
	int yp = m_nDYp;

	int w = m_nWidth;
	int h = m_nHeight;
	
	if((xp + x) <=0)
	{
		m_nDXp = 0;
		x= 0;		
	}
	else if((xp + x) >=(1024-w))
	{
		m_nDXp = 1024-w;
		x= 0;		
	}
	else m_nDXp += x;

	if((yp + y) <=25)
	{
		m_nDYp = 25;
		y = 0;		
	}
	else if((yp + y) >=(768-h))
	{
		m_nDYp = 768-h;
		y = 0;
	}
	else m_nDYp += y;	

	m_btnOK.MoveWindow(m_nDXp+263,m_nDYp+226);	
	m_btnPre.MoveWindow(m_nDXp+10,m_nDYp+5);
	m_btnNext.MoveWindow(m_nDXp+100,m_nDYp+5);
	m_rtShowTip.SetRect(0,0,17,17);
	m_rtShowTip.OffsetRect(m_nDXp+18,m_nDYp+230);
}

void CSprDlgTip::SDlg_Draw(CDC *pDC)
{
	if(pDC == NULL){m_nResult = SPRDLG_PROCESS_KILLME;}
///////////////////////////////////////////////////////////////////////////////////////////////////////////

	// 배경 그리기
// 	m_pPage->PutSprAuto(m_nDXp,m_nDYp,&g_TipManDlg.TipBackSpr,13);
// 	
// 	DWORD drawtime = timeGetTime() - m_dwDrawStartTime;
// 	m_pPage->DrawScene(m_nDXp + 15,m_nDYp + 38,&g_TipManDlg.TipSpr,m_nCurPageIndex,drawtime);
// 	// 버튼 그리기 
// 	m_btnOK.Draw(pDC);
// 	m_btnPre.Draw(pDC);
// 	m_btnNext.Draw(pDC);
// 
// 	// 채크 버튼 그리기 
// 	if(m_bNeverShowAgain){
// 		m_pPage->PutSprAuto(m_nDXp + 18,m_nDYp + 230,&g_TipManDlg.TipBackSpr,12);	
// 	}	
}

void CSprDlgTip::SDlg_Destroy()
{
}

int  CSprDlgTip::SDlg_ProcessObject()
{
	return m_nResult;
}
