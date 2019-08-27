// VipService.cpp: implementation of the CVipService class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Resource.h"
#include "VipService.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVipService::CVipService()
{
	
	

	m_bLoaded = FALSE;
}

CVipService::~CVipService()
{

}

CVipService& CVipService::GetInstance()
{
	static CVipService EventClient;
	return EventClient;
}

CVipService& VipService()
{
	return CVipService::GetInstance();
}

void CVipService::DoModalServiceDlg()
{
	if( m_ServiceDlg.GetSafeHwnd() == NULL )
	{
		m_ServiceDlg.Create(IDD_DIALOG_VIP_SERVICE, NULL);
		m_ServiceDlg.ShowWindow( SW_HIDE );		
	}

	// ConnectFaile이면 패스한다. - 일과시간이 지난경우는 담당자가 퇴근 했기 때문이다.
	if( m_ServiceDlg.m_nLogin == 2 )
		return;

	// 초기화가 안되었는데 서버에 Connect되어 있다면 문제가 있는것으로 패스한다.
	if( m_ServiceDlg.m_nLogin == 0 && m_ServiceDlg.IsConnectServer())
		return;

	if( m_ServiceDlg.m_nLogin == 0 )
		m_ServiceDlg.ConnectServer();
	else
		m_ServiceDlg.ShowWindow( SW_SHOW );

	CRect rc1, rc2;
	m_ServiceDlg.GetWindowRect( &rc1 );
	GM().GetMainWnd()->GetWindowRect(&rc2);

	int sx = rc2.left + (rc2.Width() - rc1.Width()) / 2;
	int sy = rc2.top + (rc2.Height() - rc1.Height()) / 2;

	int width = rc1.Width();
	int height = rc1.Height();

	rc1.SetRect( sx, sy, sx+width, sy+height );
	m_ServiceDlg.MoveWindow( &rc1 );
}

BOOL CVipService::IsService()
{
	return (IsAbleToVIPCounsel( UPGCheckPremiumState(&GM().GetMyInfo()->UI) ) );	
}

void CVipService::OnLogin()
{

#ifdef _DEBUG
	return;
#endif

	//추후 수정 %%%
/*
	if( IsService() == TRUE )
		g_pLobyDlg->m_VipBtn.ShowWindow( SW_SHOW );
	else
		g_pLobyDlg->m_VipBtn.ShowWindow( SW_HIDE );
*/
}

void CVipService::OnEnterGame()
{

#ifdef _DEBUG
	return;
#endif

/*	//추후 수정%%%
	if( IsService() == TRUE )
		g_pGameView->VipBtn.Show(TRUE);
	else
		g_pGameView->VipBtn.Show(FALSE);
*/

}
