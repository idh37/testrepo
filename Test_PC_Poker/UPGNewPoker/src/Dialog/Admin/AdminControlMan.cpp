// AdminControlMan.cpp: implementation of the CAdminControlMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "AdminControlMan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/****************************************************************************/
/* 앞으로 관리자 기능(게임화면에 액션을 취하는 부분) 이 클래스에서 구현 한다*/
/* 2007-08-09 관리자가 게임방 입장시 도신렙을 마우스 오버시에 보여준다		*/
/****************************************************************************/

CAdminControlMan::CAdminControlMan()
{
	m_bActive = FALSE;
	m_pPlayerInfo = new CAdmin_UserInfo[GAME()->GetMaxPlayer()];
}

CAdminControlMan::~CAdminControlMan()
{
	m_bActive = FALSE;
	if(m_pPlayerInfo)
		delete [] m_pPlayerInfo;

	m_pPlayerInfo = NULL;
}

void CAdminControlMan::SetAdministrator()
{		 
	m_bActive = g_pMyInfo_NP->UI.AdminGrade;
}

void CAdminControlMan::OnMouseMove(int x, int y)
{
	if( IsAdministartor() == FALSE )return;		

	const int nMaxPlayer = GAME()->GetMaxPlayer();
	for ( int i = 0 ; i < nMaxPlayer ; i ++ ){
		m_pPlayerInfo[i].OnMouseMove(x,y);
	}
}

void CAdminControlMan::OnDraw(CDC *pDC)
{
	if( IsAdministartor() == FALSE )return;		
	if( pDC == NULL )return;

	COLORREF oldcolor = pDC->GetTextColor();

	pDC->SetTextColor( RGB(255,255,255) );

	const int nMaxPlayer = GAME()->GetMaxPlayer();
	for ( int i = 0 ; i < nMaxPlayer ; i ++ ){
		m_pPlayerInfo[i].OnDraw(pDC);
	}

	pDC->SetTextColor( oldcolor );
}

/************************************************************************/
/* 유저 정보를 보여줌                                                   */
/************************************************************************/

CAdmin_UserInfo::CAdmin_UserInfo()
{
	m_pPlayInfo = NULL;
	m_rtPlayerArea = NULL;
	m_bValid = FALSE;
	m_bShowPlayerInof = FALSE;
}

CAdmin_UserInfo::~CAdmin_UserInfo()
{

}
#define AV_W 75
#define AV_H 125
void CAdmin_UserInfo::Init(CPlayer_NP *pPlayer)
{
	if( pPlayer == NULL ){
		m_bValid = FALSE;
		return;
	}
	m_rtPlayerArea = NULL;
	m_pPlayInfo = NULL;
	m_pPlayInfo = pPlayer;

	m_rtPlayerArea.SetRect(0,0,AV_W,AV_H);
	switch(m_pPlayInfo->PNum) {
	case 0:
		m_rtPlayerArea.OffsetRect(331,560);
		break;
	case 1:
		{			
			m_rtPlayerArea.OffsetRect(50,354);			
		}		
		break;
	case 2:
		m_rtPlayerArea.OffsetRect(50,150);
		break;
	case 3:
		m_rtPlayerArea.OffsetRect(899,150);
		break;
	case 4:
		{			
			m_rtPlayerArea.OffsetRect(899-40,354-97);			
		}		
		break;	
	}

	m_bValid = TRUE;
}

void CAdmin_UserInfo::OnMouseMove(int x, int y)
{
	if( m_rtPlayerArea == NULL )return;	
	m_ptMouse.x = x ;
	m_ptMouse.y = y ;
	if( m_rtPlayerArea.PtInRect(m_ptMouse) ){
		m_bShowPlayerInof = TRUE;
	}
	else{
		m_bShowPlayerInof = FALSE;
	}
}

void CAdmin_UserInfo::OnDraw(CDC *pDC)
{
	if( !m_bShowPlayerInof )return;
	if( pDC == NULL ) return;	
	if( m_pPlayInfo == NULL )return;
	/************************/
	/* 유저 정보를 그려준다 */
	/************************/	

	CString str;
	str.Format(" DS_LV   : %d \n DS_Exp  : %2.2f \n DS_MAXM : %I64d \n DS_BETM : %I64d",m_pPlayInfo->UI.DS_Level,
	m_pPlayInfo->UI.DS_Exp,m_pPlayInfo->UI.DS_GetMaxMoney,m_pPlayInfo->UI.DS_GetBetMaxMoney);

	POINT pt = m_ptMouse;
	pt.y -= 70;
	
	CRect rt;
	rt.SetRect( 0,0, 300,70);
	rt.OffsetRect(pt);
	pDC->DrawText(str,rt,DT_LEFT);

	str.Format(" Win     : %d \n Lose    : %d \n Draw    : %d \n Allin   : %d \n RSF     : %d \n SF      : %d \n F       : %d",
		m_pPlayInfo->UI.GetMergedAllGameRecords().GetWinNum(),m_pPlayInfo->UI.GetMergedAllGameRecords().GetLoseNum(),m_pPlayInfo->UI.GetMergedAllGameRecords().GetFoldNum(),m_pPlayInfo->UI.AllinCnt,m_pPlayInfo->UI.PI.nRSF,
		m_pPlayInfo->UI.PI.nSF,m_pPlayInfo->UI.PI.nFCARD);

	rt.SetRect( 0,0, 300,110);
	
	pt.y += 70;
	rt.OffsetRect(pt);
	pDC->DrawText(str,rt,DT_LEFT);
}
