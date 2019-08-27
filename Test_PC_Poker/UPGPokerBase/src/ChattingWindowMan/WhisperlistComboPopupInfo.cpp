#include "stdafx.h"
#include "WhisperlistComboPopupInfo.h"
#include "WhisperlistComboPopupInfoID.h"
#include "ChattingWindowMan.h"
#include "../GlobalPokerBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _DEF_MAX_WHIPER_ID 5

IMPLEMENT_DYNAMIC(CWhisperlistComboPopupInfo, LSY::CLSYLibWindowsBase)

BEGIN_MESSAGE_MAP(CWhisperlistComboPopupInfo, LSY::CLSYLibWindowsBase)
	ON_WM_DESTROY()
	ON_WM_CREATE()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CWhisperlistComboPopupInfo, LSY::CLSYLibWindowsBase)
END_DISPATCH_MAP()

CWhisperlistComboPopupInfo::CWhisperlistComboPopupInfo()
:	LSY::CLSYLibWindowsBase()
{
}

CWhisperlistComboPopupInfo::~CWhisperlistComboPopupInfo()
{
}

bool CWhisperlistComboPopupInfo::Create(int x, int y, CWnd *pParent)
{
	Init(GetObjectMan(), ID_WHISPERLISTINFOPOPUP, true);
	if(!CLSYLibWindowsBase::CreateEx(0, AfxRegisterWndClass(CS_DBLCLKS), "WhisperlistComboPopup", WS_POPUP | WS_OVERLAPPED | WS_CLIPCHILDREN, x, y, 100, 100, pParent->GetSafeHwnd(), NULL))
	{
		return false;
	}

	SetWindowPos(&CWnd::wndTop, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
	return true;
}

LRESULT CWhisperlistComboPopupInfo::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
	switch(message)
	{
	case WM_MOUSEMOVE:
		{
			CWnd *pParent = GetParent();
			CPoint ptPos;
			GetCursorPos(&ptPos);
			pParent->ScreenToClient(&ptPos);
			pParent->SendMessage(message, wParam, MAKELPARAM(ptPos.x, ptPos.y));
		}
		return TRUE;
	}

	return LSY::CLSYLibWindowsBase::WindowProc(message, wParam, lParam);
}

void CWhisperlistComboPopupInfo::SetShow(bool bShow)
{
	if(IsShow() == bShow) return;
	if(bShow)
	{
		CWnd *pWnd = GM().GetCurrentGameView();

		CRect rtRect;
		pWnd->GetWindowRect(&rtRect);
		SetWindowPos(NULL, rtRect.left+65, rtRect.top+569, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_SHOWWINDOW);
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
}

bool CWhisperlistComboPopupInfo::IsShow(void)
{
	return IsWindowVisible()?true:false;
}