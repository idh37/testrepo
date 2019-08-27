#include "stdafx.h"
#include "PromotionBase.h"
#include "PromotionManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CPromotionBase::CPromotionBase(CPromotionManager *pManager, int nType)
:	m_bActive(false),
	m_bDraw(false),
	m_bComplete(false),
	m_pManager(pManager),
	m_bShowTab(false),
	m_nType(nType)
{
	m_pManager = pManager;
}

CPromotionBase::~CPromotionBase()
{
}

const bool CPromotionBase::IsShowTab(void)
{
	return m_bShowTab;
}

void CPromotionBase::SetShowTab( bool show )
{
	m_bShowTab = show;
}

BOOL CPromotionBase::OnLButtonDownBottom(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionBase::OnLButtonUpBottom(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionBase::OnMouseMoveBottom(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionBase::OnLButtonDownMiddle(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionBase::OnLButtonUpMiddle(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionBase::OnMouseMoveMiddle(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionBase::OnLButtonDownTop(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionBase::OnLButtonUpTop(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionBase::OnMouseMoveTop(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionBase::OnEventTabLButtonDown(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionBase::OnEventTabLButtonUp(int &x , int &y)
{
	return FALSE;
}

BOOL CPromotionBase::OnEventTabMouseMove(int &x , int &y)
{
	return FALSE;
}


BOOL CPromotionBase::OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData)
{
	return FALSE;
}

BOOL CPromotionBase::OnCommand(WPARAM &wParam, LPARAM &lParam)
{
	return FALSE;
}

void CPromotionBase::OnTimer()
{

}


void CPromotionBase::OnStartGame()
{

}

void CPromotionBase::Accept_OutRoom()
{

}

void CPromotionBase::Accept_CreateRoom()
{

}

void CPromotionBase::Accept_EnterRoom()
{

}

void CPromotionBase::OnEndGame()
{

}


void CPromotionBase::DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
}

void CPromotionBase::DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
}

void CPromotionBase::DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
{
}

void CPromotionBase::DrawEventTab(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, const CPoint &ptStart)
{
}

TYPE_ENDPOPUP CPromotionBase::GetEndPopupType(void)
{
	return TYPE_ENDPOPUP_NO_EVENT;
}

int CPromotionBase::GetPriorities(void)
{
	return 100000;
}

bool CPromotionBase::InitEndPopup(CEndPopup *pEndPopup, CPoint ptPos)
{
	m_ptEndBenner = ptPos;
	return true;
}

bool CPromotionBase::OnPaintEndPopup(CPaintDC *pDC)
{
	return true;
}

bool CPromotionBase::OnClickEndPopup(UINT nID)
{
	return false;
}