// CharacterView.cpp : implementation file
//

#include "stdafx.h"
#include "CharView.h"
#include "GlobalBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CHARVIEWREPAINT_TIMER	3234

/////////////////////////////////////////////////////////////////////////////
// CCharView

CCharView::CCharView()
{
	UseMode = 0;
	CharNum = 0;
	bNowSeleted = FALSE;
	ZeroMemory(&UI, sizeof(USERINFO));
	bEvent = FALSE;

	m_colorBackGround=0;
}

CCharView::~CCharView()
{
}


BEGIN_MESSAGE_MAP(CCharView, CWnd)
	//{{AFX_MSG_MAP(CCharView)
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CCharView message handlers

void CCharView::Create(CWnd *pParentWnd, CRect &rect, int usemode, int nID)
{
	Page.Init(rect.Width(), rect.Height(), 16);
	UseMode = usemode;
	CWnd::Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pParentWnd, nID);
	ShowWindow(SW_SHOW);

	SetTimer(CHARVIEWREPAINT_TIMER, 100, NULL);
}

void CCharView::SetCharacter(USERINFO *pUI)
{
	memcpy(&UI, pUI, sizeof(USERINFO));
	Avatar.SetAvatar(pUI->AvatarInfo);
	// 랜덤하게 에니메이션 시간을 설정
	Avatar.SetAnimationTime((WORD)rand());

	COleDateTime CurTime = COleDateTime::GetCurrentTime();
	bEvent = FALSE;
	COleDateTime EndEventDate;
	EndEventDate.SetDateTime(2007,11,10,16,0,0);

	COleDateTimeSpan span = EndEventDate - CurTime;

	if( EndEventDate > 0 ){
		bEvent = TRUE;
	}
}


void CCharView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if(Page.lpBit == NULL) return;
	

	Page.DrawFillBox(0,0,Page.width, Page.height, m_colorBackGround);

	// TODO: Add your message handler code here
	Avatar.Draw(0, 0, Page.width, Page.height, (WORD*)Page.lpBit, Page.lPitch, FALSE);

	Page.FlipToGDI(dc.m_hDC);

	// Do not call CWnd::OnPaint() for painting messages
}

BOOL CCharView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default


	SetCursor(GM().GetCurHandCursor());

	return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

void CCharView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
// 	if(UseMode == 0)
// 	{
// 		//GetParent()->SendMessage(WM_COMMAND, GetDlgCtrlID(), (long)GetSafeHwnd());
// 		
// 		// 사운드 출력
// 		NMBASE::SOUND::g_Sound.PlayWav(SND_BUTTON_CLICK);
// 		
//  		GM().GetCurrentProcess()->ShowProfile(&GM().GetMyInfo()->UI);
// // 		ShowProfileDialog(&GM().GetMyInfo()->UI);
// 		g_pLobyDlg->UserInfoInvalidate();
// 	}
// 	if(UseMode == 3)
// 	{
// 		GetParent()->SendMessage(WM_COMMAND, GetDlgCtrlID(), (long)GetSafeHwnd());
// 	}
// 	if(UseMode == 4)
// 	{
// 		if(strlen(UI.ID) > 0)
// 		{
// 			// 사운드 출력
// 			NMBASE::SOUND::g_Sound.PlayWav(SND_BUTTON_CLICK);
// 
// 			GM().GetCurrentProcess()->ShowProfile(&UI);
// // 		ShowProfileDialog(&UI);
// 			if( strcmp( GM().GetMyInfo()->UI.ID ,UI.ID) == 0 )g_pLobyDlg->UserInfoInvalidate();
// 		}
// 	}
	CWnd::OnLButtonUp(nFlags, point);
}

void CCharView::OnDestroy() 
{
	CWnd::OnDestroy();
	
	// TODO: Add your message handler code here
	KillTimer(CHARVIEWREPAINT_TIMER);
}

void CCharView::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	Invalidate(FALSE);
	CWnd::OnTimer(nIDEvent);
}
