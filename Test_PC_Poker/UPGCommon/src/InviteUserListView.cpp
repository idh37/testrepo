// InviteUserListView.cpp : implementation file
//

#include "stdafx.h"
#include "InviteUserListView.h"
#include "InviteWidget.h"


CInviteUserListView*	g_pInviteUserListView = NULL;

const int INVITEVIEW_TIMER = 54345;
const int INVITEVIEW_FPS = 30;

void CALLBACK LobbyUpdateProc( UINT uTimerID, UINT uMsg, DWORD dwUser, DWORD dw1, DWORD dw2 ) 
{ 
	if( g_pInviteUserListView == NULL ) return;

	if( g_pInviteUserListView->GetSafeHwnd() ) 
	{
		static MSG wq_msg;
		if( ::PeekMessage( &wq_msg, NULL, WM_TIMER, WM_TIMER, PM_NOREMOVE ) != 0 )
		{ 
			// 이전에 포스트된 타이머 메시지가 아직도 처리되지 않은 경우라면 그냥 리턴 
			if( wq_msg.hwnd == g_pInviteUserListView->GetSafeHwnd() && wq_msg.wParam == INVITEVIEW_TIMER )
			{
				g_pInviteUserListView->MessageLooping();
				return;
			}
		}
		g_pInviteUserListView->PostMessage( WM_TIMER, INVITEVIEW_TIMER, 0 );
	}
}

//////////////////////////////////////////////////////////////////////////////
// CInviteUserListView

IMPLEMENT_DYNAMIC(CInviteUserListView, CWnd)

CInviteUserListView::CInviteUserListView()
{
	m_hEventTimer = NULL;	
	g_pInviteUserListView = this;	

}

CInviteUserListView::~CInviteUserListView()
{
}

BEGIN_MESSAGE_MAP(CInviteUserListView, CWnd)
	//{{AFX_MSG(CInviteUserListView)
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG.
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()




BOOL CInviteUserListView::PreTranslateMessage( MSG* pMsg )
{
	switch( pMsg->message )
	{
	case WM_SETCURSOR:// 메세지를 집어 삼킨다
		{
		} return TRUE;
	case WM_KEYDOWN:
		{
			if( OnKeyDownWidget( pMsg->wParam, pMsg->lParam ) )
				return TRUE;
		} break;
	}

	return CWnd::PreTranslateMessage( pMsg );
}

LRESULT CInviteUserListView::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{

	CPoint point = 0;
	UINT nFlags = 0;
	short zDelta = 0;

	if( message >= WM_USER )
	{
		if( OnCommandWidget( message, wParam, lParam ) )
			return TRUE;
	}

	if( WM_MOUSEFIRST <= message && message <= WM_MOUSELAST )
		SetCursor( AfxGetApp()->LoadCursor(IDC_CURSOR_SELECT) );

	switch( message )
	{
	case WM_MOUSEMOVE:
		{
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);			

			OnMouseMoveWidget( nFlags, point );

			TRACKMOUSEEVENT tme;
			tme.cbSize = sizeof(TRACKMOUSEEVENT);
			tme.dwFlags = TME_LEAVE;
			tme.dwHoverTime = 100;
			tme.hwndTrack = GetSafeHwnd();
			_TrackMouseEvent(&tme);
		} break;

	case WM_MOUSELEAVE:
		{
			OnMouseLeaveWidget( nFlags, point );
		} break;

	case WM_LBUTTONDOWN:
		{
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);

			OnLButtonDownWidget( nFlags, point );		



		} break;
	case WM_LBUTTONUP:
		{
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);

			OnLButtonUpWidget( nFlags, point );
		} break;
	case WM_LBUTTONDBLCLK:
		{
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);

			OnLButtonDbClkWidget( nFlags, point );
		} break;
	case WM_MOUSEWHEEL:
		{
			zDelta = HIWORD(wParam);
			nFlags = LOWORD(wParam);

			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);
			CPoint ptClient = point;
			ScreenToClient( &ptClient );

			OnMouseWheelWidget( nFlags, zDelta, ptClient );
		} break;
	case WM_RBUTTONDOWN:
		{
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);

			OnRButtonDownWidget( nFlags, point );
		} break;
	case WM_RBUTTONUP:
		{
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);

			OnRButtonUpWidget( nFlags, point );
		} break;
	case WM_RBUTTONDBLCLK:
		{
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);

			OnRButtonDbClkWidget( nFlags, point );
		} break;
	}

	return CWnd::WindowProc( message, wParam, lParam );
}

void CInviteUserListView::OnOK()
{
}

void CInviteUserListView::OnCancel()
{
}

int CInviteUserListView::OnCreate( LPCREATESTRUCT lpCreateStruct ) 
{
	if( CWnd::OnCreate( lpCreateStruct ) == -1 )
		return -1;

	CRect rt;
	GetWindowRect(&rt);
	if( m_Page.Init( rt.Width(), rt.Height(), 16 ) == FALSE )
	{
		AfxMessageBox( "ViewPage 생성 실패" );
		return -1;
	}

	//NMRESSM.RegistSprite();
	//	NMRESAM.Initialize(GetSafeHwnd());

	if( CreateWidget( this ) == FALSE )
	{
		AfxMessageBox( "CreateWidget 생성 실패" );
		return -1;
	}

	if( !m_hEventTimer )
	{
		TIMECAPS caps; memset( &caps, 0, sizeof(caps) );
		timeGetDevCaps( &caps, sizeof(caps) ); 
		m_hEventTimer = timeSetEvent( 1000/INVITEVIEW_FPS, max(1, caps.wPeriodMin), LobbyUpdateProc, 0, TIME_PERIODIC|TIME_CALLBACK_FUNCTION );
	}

	return TRUE;
}

void CInviteUserListView::OnPaint() 
{
	CPaintDC dc(this);

	CDC MemDC;
	MemDC.CreateCompatibleDC( &dc );
	if( MemDC.m_hDC == NULL ) return;
	MemDC.SelectObject( m_Page.hBmp );
	MemDC.SetBkMode( TRANSPARENT );

	DrawWidget( m_Page, MemDC );

	dc.BitBlt( 0, 0,
		GAMEVIEW_WIDTH, GAMEVIEW_HEIGHT,
		&MemDC,
		0, 0,
		SRCCOPY );
	//	dc.StretchBlt( 0, 0, 800, 600,
	//				   &MemDC,
	//				   0, 0,
	//				   GAMEWND_WIDTH, GAMEWND_HEIGHT,
	//				   SRCCOPY );

	MemDC.DeleteDC();

//	NMRESAM.RegularCall();

	//	LobbyUpdateProc(0,0,0,0,0);
}

void CInviteUserListView::OnTimer( UINT nIDEvent ) 
{
	if( nIDEvent == INVITEVIEW_TIMER )
	{
		UpdateWidget( GetTickCount() );
		Invalidate( FALSE );
	}

	CWnd::OnTimer(nIDEvent);
}

void CInviteUserListView::MessageLooping()
{
	// 누적된 메시지를 모두 처리함(윈도우 핸들을 NULL로 주어 쓰레드의 모든 메시지를 처리하게 함)
	static MSG wndq_msg;
	while( ::PeekMessage( &wndq_msg, NULL, 0U, 0U, PM_REMOVE ) ) 
	{
		::TranslateMessage( &wndq_msg );
		::DispatchMessage( &wndq_msg );
	}
}

void CInviteUserListView::OnDestroy() 
{
	if( m_hEventTimer ) 
	{
		timeKillEvent( m_hEventTimer );
		m_hEventTimer = NULL;
	}

	DestroyWidget();

	CWnd::OnDestroy();
}

BOOL CInviteUserListView::CreateWidget( CWnd* pParent )
{


	///////////////////////////////////////////////////////////
	// widget setting
	CInviteTab* pInviteTab = new CInviteTab();	
	pInviteTab->Initialize(INVITE_WIDGET_ID_INVITETAB, INVITE_WIDGET_ID_INVITETAB_STATE);
	AttachWidget( INVITE_WIDGET_ID_INVITETAB, pInviteTab );

/*

	CLobbyGameTab *pLobbyGameTab = new CLobbyGameTab();
	pLobbyGameTab->Initialize(LOBBY_WIDGET_ID_GAMETAB, LOBBY_WIDGET_ID_LOBBYGAMESTATE);	
	AttachWidget( LOBBY_WIDGET_ID_GAMETAB, pLobbyGameTab);	


	CUserTab *pUserTab = new CUserTab();
	pUserTab->Initialize(LOBBY_WIDGET_ID_USER_TAB, LOBBY_WIDGET_ID_USER_STATE);
	AttachWidget(LOBBY_WIDGET_ID_USER_TAB, pUserTab);


	CMyInfoWidget *pMyInfo = new CMyInfoWidget();
	//pMyInfo->Initialize(LOBBY_WIDGET_ID_MYINFO, &GM().GetMyInfo()->UI);
	AttachWidget(LOBBY_WIDGET_ID_MYINFO, pMyInfo);


	CChatViewWidget *pChatView = new CChatViewWidget();
	AttachWidget(LOBBY_WIDGET_ID_CHATVIEW, pChatView);


	CShopTab *pShopTab = new CShopTab();
	pShopTab->Initialize(LOBBY_WIDGET_ID_SHOPTAB, LOBBY_WIDGET_ID_SHOPTAB_STATE);
	AttachWidget(LOBBY_WIDGET_ID_SHOPTAB, pShopTab);

	CRoomTab *pRoomTab = new CRoomTab();
	pRoomTab->Initialize(LOBBY_WIDGET_ID_ROOM_TAB, LOBBY_WIDGET_ID_ROOM_TAB_STATE);
	AttachWidget(LOBBY_WIDGET_ID_ROOM_TAB, pRoomTab);


*/


	return CNMWidgetComposite::CreateWidget( pParent );
}

void CInviteUserListView::EnterWidget()
{
	// 	g_pLobbyDlg->RefreshChatList();
	// 	g_pLobbyDlg->ShowItemState();
	// 
	// 	g_pLobbyDlg->RefreshRoomList();
	// 	g_pLobbyDlg->RefreshUserList();
	/*
	if( g_pMainDlg->m_RecvInviteDlg.GetSafeHwnd() == NULL )
	g_pMainDlg->m_RecvInviteDlg.Create( g_pMainDlg->m_RecvInviteDlg.IDD, g_pLobbyDlg );

	ROOMINFO tTestInfo;
	tTestInfo.Super = 1;
	tTestInfo.bSpeed = 1;
	tTestInfo.bDaebak = 1;
	tTestInfo.GwangGame = 0;
	tTestInfo.RoomNum = 999;
	tTestInfo.UNum = Play[0].UI.UNum;
	wsprintf( tTestInfo.ID[ID_INDEX], Play[0].UI.ID[ID_INDEX] );
	wsprintf( tTestInfo.ID[NICK_INDEX], Play[0].UI.ID[NICK_INDEX] );
	tTestInfo.NowUserNum = rand()%5;
	tTestInfo.MaxUserNum = 5;
	tTestInfo.State = 0;
	ZeroMemory( tTestInfo.Pass, sizeof(tTestInfo.Pass) );
	tTestInfo.BetMoney = 1000;
	tTestInfo.UserMoney[0] = 498765432 + rand()%999999999;
	tTestInfo.UserMoney[1] = 498765432 + rand()%999999999;
	tTestInfo.UserMoney[2] = 999999999999;
	tTestInfo.Sex[0] = rand()%2;
	tTestInfo.Sex[1] = rand()%2;
	tTestInfo.Sex[2] = rand()%2;
	wsprintf( tTestInfo.Title, "가나다라마바사아자차카타파하거" );
	tTestInfo.Secret = 1;
	tTestInfo.MinMoney = 350000000;

	g_pMainDlg->m_RecvInviteDlg.Init( &Play[0].UI,
	"가갸갸갸캬캬캬캬캬캬캬캬ㅑㅋ캬캬캬깍깍깍깍깍깍깍깍깍깍깍",
	&tTestInfo );
	g_pMainDlg->m_RecvInviteDlg.ShowWindow( SW_SHOW );
	g_pMainDlg->m_RecvInviteDlg.Invalidate( FALSE );
	/**/
	SetCursor( AfxGetApp()->LoadCursor(IDC_CURSOR_SELECT) );

	CNMWidgetComposite::EnterWidget();
}

void CInviteUserListView::DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
{
	CNMWidgetComposite::DrawWidget( Page, DC );
}


BOOL CInviteUserListView::OnCommandWidget( int nCommand, WPARAM wParam, LPARAM lParam )
{
#ifdef _DEBUG
	//	nCommand List : control에 대한 모든 명령 저장
#endif

	switch(nCommand)
	{
	case INVITE_BUTTON_ID_INVITELIST_SORT_NICKNAME:
		{
			//유저정렬 (닉네임)
			GM().GetDataUserList()->SortBy_Nickname((BOOL)lParam);
			RefreshUserList();
		} break;
	case INVITE_BUTTON_ID_INVITELIST_SORT_POSITION:
		{
			//유저정렬 (위치)
			GM().GetDataUserList()->SortBy_Position((BOOL)lParam);			
			RefreshUserList();
		} break;
	case INVITE_BUTTON_ID_INVITELIST_SORT_MONEY:
		{
			//유저정렬 (머니)
			GM().GetDataUserList()->SortBy_Money((BOOL)lParam);
			RefreshUserList();
		} break;
	case INVITE_BUTTON_ID_INVITETAB_ONEGAME:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_BETAREA_INVITE_NOWGAME);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
		} break;
	case INVITE_BUTTON_ID_INVITETAB_ALLGAME:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_BETAREA_INVITE_ALLGAME);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
		} break;
	}

	CNMWidgetComposite::OnCommandWidget( nCommand, wParam, lParam );

	return FALSE;
}

int CInviteUserListView::GetCurSelIndex()
{
	CInviteTab *pInviteTab = dynamic_cast<CInviteTab*>(GetWidget(INVITE_WIDGET_ID_INVITETAB));
	int nRetVal = pInviteTab->GetCurSelIndex();

	return nRetVal;
}

BOOL CInviteUserListView::OnLButtonDownWidget( UINT nFlags, CPoint point )
{
	BOOL bRet = CNMWidgetComposite::OnLButtonDownWidget( nFlags, point );
	if( bRet == FALSE )
		SetFocus();

	return bRet;
}


////////////////////////////////////////////////////////////////////////////////////
BOOL CInviteUserListView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return TRUE;
}


void CInviteUserListView::ClearDataUserList()									// 유저리스트 정보를 클리어시킨다.
{
// 	CUserTab *pUserTab = dynamic_cast<CUserTab*> (GetWidget(LOBBY_WIDGET_ID_USER_TAB));
// 	ASSERT(pUserTab);
// 	CUserListWidget *pUserListWidget = dynamic_cast<CUserListWidget*> (pUserTab->GetWidget(LOBBY_WIDGET_ID_USERLIST));
// 	ASSERT(pUserListWidget);
// 	pUserListWidget->ClearDataList();

}
void CInviteUserListView::SetDataUserList(CDataUserList *pDataUserList)			// 유저리스트 정보를 세팅한다.
{

// 	if(pDataUserList==NULL) return;
// 
// 	CUserTab *pUserTab = dynamic_cast<CUserTab*> (GetWidget(LOBBY_WIDGET_ID_USER_TAB));
// 	ASSERT(pUserTab);
// 	CUserListWidget *pUserListWidget = dynamic_cast<CUserListWidget*> (pUserTab->GetWidget(LOBBY_WIDGET_ID_USERLIST));
// 	ASSERT(pUserListWidget);
// 
// 	pUserListWidget->SetDataList(pDataUserList);

}

BOOL CInviteUserListView::RefreshUserList()
{

	CInviteTab *pInviteTab = dynamic_cast<CInviteTab*>(GetWidget(INVITE_WIDGET_ID_INVITETAB));
	ASSERT(pInviteTab);

	if(pInviteTab)
	{
		return pInviteTab->RefreshInviteList();
	}

	return NULL;	
}


void CInviteUserListView::SetGameFilter(CDataUserList::KEY_FILTER KeyFilter)
{

	CInviteListWidget *pInviteList = dynamic_cast<CInviteListWidget*>(FindWidget(INVITE_WIDGET_ID_INVITELIST_ONEGAME));
	ASSERT(pInviteList);

	if(pInviteList)
	{
		return pInviteList->SetFilter(KeyFilter);
	}
}


map<string, UINT64> *CInviteUserListView::GetCheckedUser()
{
	CInviteTab *pInviteTab = dynamic_cast<CInviteTab*>(GetWidget(INVITE_WIDGET_ID_INVITETAB));
	ASSERT(pInviteTab);

	if(pInviteTab)
	{
		return pInviteTab->GetCheckedUser();
	}

	return NULL;
}


void CInviteUserListView::OnAskInviteUserList()
{
	//초대할 유저리스트를 서버에 요청한 시점
	CInviteTab *pInviteTab = dynamic_cast<CInviteTab*>(GetWidget(INVITE_WIDGET_ID_INVITETAB));
	ASSERT(pInviteTab);

	if(pInviteTab)
	{
		pInviteTab->ClearCheckedUser();
	}
}