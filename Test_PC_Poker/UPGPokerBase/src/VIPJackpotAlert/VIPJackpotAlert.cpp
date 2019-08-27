#include "stdafx.h"
#include "VIPJackpotAlert.h"
#include "VIPJackpotAlertID.h"
#include "VIPJackpotAlertGuide.h"
#include "../GlobalPokerBase.h"

IMPLEMENT_DYNAMIC(CVIPJackpotAlert, LSY::CLSYLibWindowsBase)

BEGIN_MESSAGE_MAP(CVIPJackpotAlert, LSY::CLSYLibWindowsBase)
	ON_WM_DESTROY()
	ON_WM_CREATE()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CVIPJackpotAlert, LSY::CLSYLibWindowsBase)
END_DISPATCH_MAP()

CVIPJackpotAlert::CVIPJackpotAlert()
: LSY::CLSYLibWindowsBase()
, m_pGuide(NULL)
{

}

CVIPJackpotAlert::~CVIPJackpotAlert()
{

}

bool CVIPJackpotAlert::Create( int x, int y )
{
	Init(GetObjectMan(), ID_VIPJACKPOTALERT, true);
// 	if(!CLSYLibWindowsBase::CreateEx(0, AfxRegisterWndClass(CS_DBLCLKS), "VIPJackpotAlert", WS_POPUP | WS_OVERLAPPED | WS_CLIPCHILDREN, x, y, 100, 100, GM().GetMainWnd()->GetSafeHwnd(), NULL))
// 	{
// 		return false;
// 	}

//	SetWindowPos(&CWnd::wndTop, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
	return true;

}

void CVIPJackpotAlert::SetShow( bool bShow )
{
	if(IsShow() == bShow) 
		return;

	if (bShow)
	{
		m_pObjectMan->SetMouseCapture();
		CWnd *pWnd = GM().GetCurrentGameView();
		SetParent(pWnd);

		SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_SHOWWINDOW);
	}
	else
	{
		SetParent(GM().GetMainWnd());
		ShowWindow(SW_HIDE);
		m_pObjectMan->ReleaseMouseCapture();

	}

}

bool CVIPJackpotAlert::IsShow( void )
{
	return IsWindowVisible()?true:false;
}

bool CVIPJackpotAlert::OnCreatedProject( void )
{
	m_pGuide = static_cast<CVIPJackpotAlertGuide*>(GetObject(ID_VIPJACKPOTALERT_DLG_INFO));
	if (m_pGuide)
		m_pGuide->SetShow(false);

	return TRUE;
}

LRESULT CVIPJackpotAlert::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
	LRESULT nRet = LSY::CLSYLibWindowsBase::WindowProc(message, wParam, lParam);
	return nRet;
}

