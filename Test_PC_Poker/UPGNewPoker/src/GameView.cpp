// GameView.cpp : implementation file
//

#include "stdafx.h"
#include "GameView.h"
#include "./AutoBet/AutoBetSetUpWnd.h"
#include "./SprDlg/SprDlgObjectMan.h"
#include "./AniListMan/AniFileListMan.h"
#include "./AniListMan/AniFileListMan.h"
#include <CommMsgDef_Game.h>
#include "./Module/GameHelper/JokboProbabilityNewPoker.h"
#include "./Module/GameHelper/OtherUserJokboCheckManNewPoker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGameViewNewPoker

CGameViewNewPoker::CGameViewNewPoker(CGame *pGame)
: CGameViewBase(pGame), m_cPlayerDrawManager(pGame->GetMaxPlayer())
{
	g_pGameView = this;
	m_bIsPlay = FALSE;

	// 마우스 상태변수
	m_bLClick = FALSE;

	m_strBtnMsgTip = "";
	m_nTipX = 0;
	m_nTipY = 0;

	m_nTipCnt = 0;
	m_nRoomKindCnt = 0 ;
	m_dwChangeRoomInfoMsgTime = 0;
	m_nChangeRoomInfoMsgKind = 0;

	m_bFirstLoad = true;

	m_cAniSprListMan.Destroy();
}

CGameViewNewPoker::~CGameViewNewPoker()
{
	// 쓰레드 타이머 제거
//	RefreshTimer.KillTimer();
}


BEGIN_MESSAGE_MAP(CGameViewNewPoker, CGameViewBase)
	//{{AFX_MSG_MAP(CGameViewNewPoker)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGameViewNewPoker message handlers

BOOL CGameViewNewPoker::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	//cs.style |= WS_CLIPCHILDREN;
	return CGameViewBase::PreCreateWindow(cs);
}

const int CGameViewNewPoker::GetWidth()
{
	return 1000;
}

const int CGameViewNewPoker::GetHeight()
{
	return 740;
}

int CGameViewNewPoker::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGameViewBase::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	// 더블 버퍼링용 메모리 DC 생성
	//MemDC.CreateCompatibleDC(0);
	// 메모리DC에 버퍼용 비트맵 핸들을 선택
	//MemDC.SelectObject(Page.hBmp);

	// 플레이어 클래스 초기화
	const int nMaxNewPlayer = GAME()->GetMaxNewPlayer();
	const int nMaxPlayer = GAME()->GetMaxPlayer();
	for(int i = 0; i < nMaxNewPlayer; i++)
	{	// 캐릭터창 초기화
		GAME()->GetPlayerNP(i)->SetPlayerNum(i);
	}

	g_pChatWinMan = &m_cChatWindow;
	m_cChatWindow.Init(&g_cUIData, g_pGameView, &g_pGameView->m_cGameHelper);
	
	m_bSkinKind = 0;
	return 0;
}

void CGameViewNewPoker::Init(void)
{
	if(m_bFirstLoad)
	{
		GAME()->Init(&Page);	
		m_cGameOverResult.Init(&Page);	
		AutoBetSetUpWnd().Init();

		g_pChatWinMan->InitButton(&m_cGameViewBtnMan.m_sprUIBottomBtn);
		// 단축키
		m_cGameViewBtnMan.Init(g_pGameView, &g_cUIData, g_pChatWinMan);
		m_bFirstLoad = false;
		m_cTitle.Init(&g_cUIData, g_pGameView);

		m_cGameHelper.Init(&g_cUIData, GetGame(), g_cUIData.m_nMaxJokbo, new COtherUserJokboCheckManNewPoker, new CJokboProbabilityNewPoker);
		m_cSidePotMoney.Init(GAME()->GetMaxPlayer(), &g_cUIData, g_pGameView);
		g_ObserverMan.Init(g_pGameView, &g_cUIData, &g_pGameView->m_cGameViewBtnMan);

		// 게임 초기화(전역변수)
		const int nMaxPlayer = GAME()->GetMaxPlayer();
		m_cPlayerDrawManager.Init(&g_cUIData, GetGame());
		for(int i = 0; i < nMaxPlayer; i++) {	// 캐릭터창 초기화
			m_cPlayerDrawManager.AddPlayer(GAME()->GetPlayer(i), new CPlayerDraw, i);
			m_cAdminCtrlMan.m_pPlayerInfo[i].Init(GAME()->GetPlayerNP(i));
		}

		//자리 지킴이
		g_SeatKeeper.Init(&g_cUIData, g_pChatWinMan);
		g_cOneShotCharge.Init(&g_cUIData, GAME(), &g_SeatKeeper, &g_sprClose);		//재기충전
		m_cDisplayBoard.Init(&g_cUIData, GetGame());
		m_cTotalBetMoney.Init(&g_cUIData, GAME(), &Page);
		m_cBackGround.Init(&g_cUIData, g_pGameView);
		GAME()->m_cSelectCardMan.Init(&g_cUIData, GetGame(), &g_sprBigCard, &g_sprDarkBigCard);
		GAME()->m_dwEndDrawJokerTime = g_sprJokerCardEffect.GetScenePlayTime(1);
	}
}

// ### [ Frame++ ] ###
void CGameViewNewPoker::MessagePump()
{
	MSG msg;
	while( ::PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
	{
		TranslateMessage( &msg );
		DispatchMessage( &msg );
	}
}


LRESULT CGameViewNewPoker::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	switch(message)
	{
	case WM_MOUSEWHEEL:
	case WM_MOUSEMOVE :
	case WM_LBUTTONDOWN :
	case WM_LBUTTONUP :
	case WM_LBUTTONDBLCLK :
	case WM_RBUTTONUP:
		if(GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME) return FALSE;
		break;
	}

	short mxp, myp;

	if( m_cSidePotMoney.WindowProc(message,wParam,lParam) ) return TRUE;

	switch(message)
	{
	case WM_NCMOUSEMOVE :
		//SetCursor(hCurArrow);
		break;
	case WM_MOUSEWHEEL:
		if(m_cChatWindow.OnMouseWheel(wParam, lParam)) return TRUE;
		break;
	case WM_MOUSEMOVE :
		{
			mxp = LOWORD(lParam);
			myp = HIWORD(lParam);

			m_cAdminCtrlMan.OnMouseMove(mxp , myp );
			//자리 지킴이 - 위치 젤 위쪽
			g_SeatKeeper.OnMouseMove(mxp, myp);

			CRect ert;
			ert.SetRect(302,687,302 + 109,687 + 30);
			CPoint p;
			p.x = mxp;
			p.y = myp;

			if(g_cPromotionMan.OnMouseMoveBottom(mxp, myp)) return TRUE;

			//재기충전
			if(g_cOneShotCharge.OnMouseMove(mxp, myp)) return TRUE;

			GAME()->m_cSelectCardMan.OnMouseMove(mxp,myp);

			//[vsmode]070314
			if(g_pChatWinMan->OnMouseMove(mxp,myp)) return TRUE;
			if(g_cOwnLimit.OnMouseMove(mxp, myp)) return TRUE;
			
			// 버튼 위에서 마우스 포인터를 손가락 모양으로 변경
			if(m_cGameViewBtnMan.OnMouseMove(mxp, myp)) return TRUE;

			if(g_cPromotionMan.OnMouseMoveMiddle(mxp, myp)) return TRUE;

			if(m_cPlayerDrawManager.OnMouseMove(mxp, myp)) return TRUE;

			//스프라이트 프로필
			m_cSprDlgMan.SprDlgMouseMove(mxp, myp);
			
			if(GAME()->OnMouseMove(mxp, myp)==TRUE) return 1;

			//관전UI
			g_ObserverMan.OnMouseMove(mxp, myp);
			g_cVipJackpot.OnMouseMove(mxp, myp);

			if(g_cPromotionMan.OnMouseMoveTop(mxp, myp)) return TRUE;


			SetCursor(GM().GetCurSelCursor());
		}
		return 1;

	case WM_LBUTTONDOWN :
		{	
			mxp = LOWORD(lParam);
			myp = HIWORD(lParam);

			if(TRUE==g_cVipJackpot.OnLButtonDown(mxp, myp)){
				return 1;
			}

			CRect rc;
			rc.SetRect( 0, 0, 712, 25 );
			POINT pt;
			pt.x = mxp;
			pt.y = myp;
			if( rc.PtInRect( pt ) )	{
				GM().SendMainWndMessage( WM_LBUTTONDOWN, MK_LBUTTON, lParam );
				return 1;
			}			

			POINT bpt;
			bpt.x = mxp;
			bpt.y = myp;
			
			m_bLClick = TRUE;		
			this->SetFocus();	
			SetCapture();

			if(g_cPromotionMan.OnLButtonDownBottom(mxp, myp)) return TRUE;

			//원샷충전
			if(g_cOneShotCharge.OnLButtonDown(mxp, myp)) return TRUE;
			
			//자리 지킴이  -위치 젤 위쪽
			if(g_SeatKeeper.OnLButtonDown(mxp, myp))return TRUE;

			//스프라이트 프로필
			m_cSprDlgMan.SprDlgLButtonDown(mxp, myp);	

			if(GAME()->OnLButtonDown(mxp, myp)==TRUE) return TRUE;
			if(m_cPlayerDrawManager.OnLButtonDown(mxp, myp)) return TRUE;
			if(m_cGameViewBtnMan.OnLButtonDown(mxp, myp)) return TRUE;

			if(g_cPromotionMan.OnLButtonDownMiddle(mxp, myp)) return TRUE;

			if(g_pChatWinMan->OnLButtonDown(mxp, myp)) return TRUE;
			if(g_ObserverMan.OnLButtonDown(mxp, myp)) return TRUE;

			if(g_cPromotionMan.OnLButtonDownTop(mxp, myp)) return TRUE;
		}
		return 1;

	case WM_LBUTTONUP :
		{
			mxp = LOWORD(lParam);
			myp = HIWORD(lParam);
			ReleaseCapture();

			if(g_cPromotionMan.OnLButtonUpBottom(mxp, myp)) return TRUE;

			//원샷충전
			if(g_cOneShotCharge.OnLButtonUp(mxp, myp)) return TRUE;
			//자리 지킴이 - 위치 젤 위쪽
			if( g_SeatKeeper.OnLButtonUp(mxp, myp) ) return TRUE;

			GAME()->m_cSelectCardMan.OnLButtonUp(mxp,myp);

			if(m_cPlayerDrawManager.OnLButtonUp(mxp, myp)) return TRUE;
			
			//[vsmode]070314
			if(g_pChatWinMan->OnLButtonUp(mxp, myp)) return TRUE;
			if(m_cGameViewBtnMan.OnLButtonUp(mxp, myp)) return TRUE;

			if(g_cPromotionMan.OnLButtonUpMiddle(mxp, myp)) return TRUE;

			m_cSprDlgMan.SprDlgLButtonUp(mxp, myp);
			GAME()->OnLButtonUp(mxp, myp);	
			g_ObserverMan.OnLButtonUp(mxp, myp);
			g_cVipJackpot.OnLButtonUp(mxp, myp);

			if(g_cPromotionMan.OnLButtonUpTop(mxp, myp)) return TRUE;
		}
		return 1;

	case WM_LBUTTONDBLCLK :
		{
			mxp = LOWORD(lParam);
			myp = HIWORD(lParam);
			GAME()->OnLButtonDblclk(mxp, myp);
		}
		return 1;
	case WM_RBUTTONUP:
		{
			
			g_Config.nSKinKind = !g_Config.nSKinKind;

			GM().ReLoadSprite(g_Config.nSKinKind);

#ifdef _DEBUG
			g_bDeBugModeFlg = !g_bDeBugModeFlg;
#endif
		
		}break;
	}
	
	return CGameViewBase::WindowProc(message, wParam, lParam);
}

void CGameViewNewPoker::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	if(GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME) return;

	bool bVIPRoom = IsVIPRoom(&g_RI);
	// 사운드 리소스 관리 및 플레이 리스트 연주
	NMBASE::SOUND::g_Sound.RegularCall();

	// 더블 버퍼링용 메모리 DC 생성
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);	

	// 메모리DC에 버퍼용 비트맵 핸들을 선택
	MemDC.SelectObject(Page.hBmp);
	MemDC.SetBkMode(TRANSPARENT);

	m_cBackGround.DrawBottom(&Page, &MemDC);
	
	//잭팟 드로우
	if(g_RI.ChipKind == 0)	DrawJackPot();
	g_cVipJackpot.DrawBottom(&Page, &MemDC);
	
	m_cTitle.Draw(&Page, &MemDC);
	m_cDisplayBoard.Draw(&MemDC);
	
	// 플레이어
	DrawPlayBottom(&MemDC);

	if(g_NowGroupNo >= GROUP_GRADE_PLATINUM_2 )
	{		
		CRect rt;
		rt.SetRect(0,0,91,20);
		rt.OffsetRect(729,582);
		CString strr;

		COLORREF color = MemDC.GetTextColor();
		MemDC.SetTextColor(RGB(255,255,255));	
		
		// 방정보 변경 예약 알림 메세지
		if(m_dwChangeRoomInfoMsgTime > 0)
		{		
			DWORD Etime = g_sprChangeRoomInfoMsg.GetScenePlayTime(0);
			
			DWORD DrawTime1 = timeGetTime() - m_dwChangeRoomInfoMsgTime;
			
			if(DrawTime1 < Etime && m_nChangeRoomInfoMsgKind > 0 && m_nChangeRoomInfoMsgKind < 3 ){
				
				Page.DrawScene(0,0,&g_sprChangeRoomInfoMsg,m_nChangeRoomInfoMsgKind - 1,DrawTime1);
				
				if(m_strChangeRoomInfoMsg.GetLength() > 0){
					DWORD st = 0;					
					DWORD et = 0;					
					g_sprChangeRoomInfoMsg.GetFrameTime(0,2,&st,&et);					
					if(DrawTime1 >= st && DrawTime1 <= et){					
						CRect rt;
						rt.SetRect(0,0,70,15);
						rt.OffsetRect(450,287);					
						MemDC.DrawText(m_strChangeRoomInfoMsg,&rt,DT_CENTER | DT_WORDBREAK);				
					}	
				}
			}
			else{
				m_dwChangeRoomInfoMsgTime = 0;
				m_strChangeRoomInfoMsg = "";
				m_nChangeRoomInfoMsgKind = 0;
			}
		}		
		MemDC.SetTextColor(color);
	}

	draw_btn_func( MemDC );  // 버튼	

	m_cTotalBetMoney.Draw(&MemDC);
	m_cPlayerDrawManager.DrawBottom(&MemDC);

	//도신지수 이벤트
	GAME()->m_cAniSprObjectMan.BottomDrawAll(&MemDC);	

	g_pChatWinMan->Draw(&MemDC);
	g_cPromotionMan.DrawBottom(&MemDC, &Page);
	g_cOwnLimit.DrawBottom();

	draw_card_func(&MemDC); // 카드	

	//카드 덱 그리기
	m_cBackGround.DrawMiddle(&Page, &MemDC);

	// 플레이어
	DrawPlayMiddle(&MemDC);

	g_cVipJackpot.DrawMiddle(&Page, &MemDC);

	m_cPlayerDrawManager.DrawMiddle(&MemDC);


	// 플레이 스크린 그리기
	DrawMyJokboAndJokerEffect(&MemDC);

	m_cGameOverResult.AngelDraw(&MemDC,&Page);	

	g_cPromotionMan.DrawMiddle(&MemDC, &Page);

	// 플레이어
	DrawPlayTop(&MemDC);

	m_cPlayerDrawManager.DrawTop(&MemDC);
	
	GAME()->m_cAllBetChip.DrawTop();
	m_cSidePotMoney.DrawBottom(&MemDC);

	//도신지수 이벤트
	GAME()->m_cAniSprObjectMan.TopDrawAll(&MemDC);

	m_cAdminCtrlMan.OnDraw(&MemDC);

	GAME()->m_cSelectCardMan.OnDraw(&MemDC);	

	AutoBetSetUpWnd().Draw_Top(&MemDC);	

	//재기충전
	g_cOneShotCharge.Draw(&MemDC);

	//관전UI
	g_ObserverMan.Draw(&Page, &MemDC);

	g_cPromotionMan.DrawTop(&MemDC, &Page);

	//스프라이트 프로필			
	m_cSprDlgMan.DrawAll(&MemDC);

	m_cSidePotMoney.DrawTop(&MemDC);

	if(g_pGameView->m_cPlayerDrawManager.IsGameOver())
	{
		GAME()->m_cResultFocus.Draw(&MemDC);
	}

	m_cAniSprListMan.DrawAll(&MemDC);
	g_cOwnLimit.DrawTop(&MemDC);
	//자리 지킴이
	g_SeatKeeper.Draw(&Page, &MemDC);       // 젤 위쪽 레이어로 그린다. (자리지킴이 이미지가 카드 위로 올라가야한다.)

#ifdef _DEBUG
	// 프레임 레이트 체크
	static float FPS = 0;
	static int framecnt = 0;
	static DWORD lasttick = timeGetTime();
	DWORD nowtick = timeGetTime();
	DWORD deltatick = nowtick - lasttick;
	framecnt++;
	if(deltatick >= 1000) {
		lasttick = nowtick;
		float secrate = (deltatick / 1000.0f) - 1.0f;
		FPS = (float)framecnt - (float)framecnt*secrate;
		framecnt = 0;
	}
	
	// 마우스 좌표 체크
	POINT mpos;
	GetCursorPos(&mpos);
	ScreenToClient(&mpos);
	CString str;
	str.Format("%5.1f FPS, Mouse (%d, %d)", FPS, mpos.x, mpos.y);

	MemDC.SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12_NORMAL));
	MemDC.SetBkMode(TRANSPARENT);
	MemDC.SetTextColor(RGB(255,255,255));
// 	if(g_RI.bPlayer==0)
// 		MemDC.TextOut(327,72, str);
// 	else
// 		MemDC.TextOut(20,65, str);


	if(g_RI.State == 1){
		LONG TTTT = nowtick - g_DebugStartTick;
		
		CString sttt;
		sttt.Format("PlayTime = %d",TTTT/1000);
		MemDC.TextOut(491,123, sttt);
	}

	CString stt;
	if(g_bDeBugModeFlg == TRUE)
	{
		
		stt = "DEBUG MODE = 1";
		MemDC.TextOut(227,100, stt);
	}
	else 
	{
		stt = "DEBUG MODE = 0 ";
		MemDC.TextOut(227,100, stt);
	}


	switch(g_Poker.nRound) 
	{
	case 0: str.Format("0  = ?"); break;
	case 1: str.Format("1  = 49"); break;
	case 2: str.Format("2  = 59"); break;
	case 3: str.Format("3  = 69"); break;
	case 4: str.Format("4  = 79"); break;

	case 5: str.Format("5  = 79"); break;
	case 6: str.Format("6  = ?"); break;
	}
	MemDC.TextOut(527,72, str);

	str.Format("%d",GAME()->m_nDebugCnt);
	
	MemDC.TextOut(527,292, str);
#endif

	m_cPlayerDrawManager.DrawTopMost(&MemDC);
	g_cVipJackpot.DrawTop(&Page, &MemDC);

	//보더그리기
	Page.DrawEmptyBox(0,0,GetWidth()-1, GetHeight()-1, NMBASE::GRAPHICGDI::xSprite::RGB555(100,100,100));

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &MemDC, 0, 0, SRCCOPY);
	
	MemDC.DeleteDC();
}

void CGameViewNewPoker::Reset()
{
	m_bExitSubscript = FALSE;	


}

void CGameViewNewPoker::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	if(nIDEvent==GAMEVIEW_TIMER)
	{
		if(GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
		{
			GAME()->OnTimer();			

			//[vsmode]070314
			g_pChatWinMan->OnTimer();	

			int skipframe = m_syncfps.FrameSync(FALSE); // 동기화

			// 스킵된 프레임만큼 프레임 진행			
			for(int i=0; i<skipframe; i++) {
				GAME()->OnTimer();
			}

			if(skipframe > 0 )
			{
				m_cSprDlgMan.ProcessAll();
				m_cGameOverResult.OnTimer();
				CGameViewBase::OnTimer(nIDEvent);
				return;
			}			
			m_cSprDlgMan.ProcessAll();

			Invalidate(FALSE); 			

			m_cAniSprListMan.ProcessAll();
		}
	}

	m_cGameOverResult.OnTimer();
	CGameViewBase::OnTimer(nIDEvent);
}

void CGameViewNewPoker::OnDestroy() 
{
	CGameViewBase::OnDestroy();
	
	// TODO: Add your message handler code here

	// ### [ Frame++ ] ###
	m_cGameOverResult.Clear();
	if(m_hEventTimer) timeKillEvent(m_hEventTimer);
	m_hEventTimer = 0;

	m_cChatWindow.OnDestroy();
}


BOOL CGameViewNewPoker::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	
	// TODO: Add your specialized code here and/or call the base class
	if( g_cPromotionMan.OnCommand(wParam, lParam)) return TRUE;
	if( m_cGameHelper.OnCommand(wParam,lParam) ) return TRUE;
	if( m_cSidePotMoney.OnCommand(wParam,lParam) ) return TRUE;
	if( g_ObserverMan.OnCommand(wParam, lParam) ) return TRUE;
	if( m_cGameViewBtnMan.OnCommand(wParam,lParam)) return TRUE;
	if(g_cVipJackpot.OnCommand(wParam, lParam)) return TRUE;
	
	switch(wParam)
	{
	case IDM_BADUSER:
		{
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_CHATWINDOW_CHATTING_REPORT);
			GM().BadUserReportGameChat();
		}break;
	case IDM_GAMESTART:
		{
			if(m_cGameViewBtnMan.m_btnGameStart.GetButtonState() > -1 && g_RI.State == 0)
			{
				CSV_ASKGAMESTART gstart;
				gstart.Set(GM().GetMyInfo()->UI.ID, g_RI.RoomNum);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), gstart.pData, gstart.GetTotalSize());
			}
		} break;
	case IDM_DIE:
		{
			OnBtnFold();
		}
		break;
	case IDM_CHECK:
		{
			OnBtnCheck();	
		}
		break;
	case IDM_CALL:
		{
			OnBtnCall();			
		}
		break;
		
	case IDM_PING:
		{
			OnBtnPing();
		}
		break;
	case IDM_DADANG:
		{
			OnBtnDda();
		}
		break;	


	case IDM_FULL:
		{
			OnBtnDda();
		}
		break;	

	case IDM_HALF:
		{
			OnBtnMax();
			g_pGameView->m_cGameViewBtnMan.Reset();
		}
		break;		
/*	case IDM_TIPPREBTN:
		{
			CSprDlgTip* pnode = (CSprDlgTip*)m_cSprDlgMan.GetObject(SDLG_TIP);
			if(pnode == NULL)break;

			if(pnode->m_nCurPageIndex >= MAX_PAGENUM-1){
				pnode->m_nCurPageIndex = 0;
			}
			else pnode->m_nCurPageIndex ++;

			if ( pnode->m_nCurPageIndex == 6 )
				pnode->m_nCurPageIndex++;
			
			pnode->m_dwDrawStartTime = timeGetTime();			
			
		}break;
	case IDM_TIPNEXTBTN:
		{
			CSprDlgTip* pnode = (CSprDlgTip*)m_cSprDlgMan.GetObject(SDLG_TIP);
			if(pnode == NULL)break;			
			
			if(pnode->m_nCurPageIndex <= 0){
				pnode->m_nCurPageIndex = MAX_PAGENUM -1;
			}	
			else pnode->m_nCurPageIndex --;		

			if ( pnode->m_nCurPageIndex == 6 )
				pnode->m_nCurPageIndex--;

			pnode->m_dwDrawStartTime = timeGetTime();

		}break;
	case IDM_TIPOKBTN:
		{
			CSprDlgTip* pnode = (CSprDlgTip*)m_cSprDlgMan.GetObject(SDLG_TIP);
			if(pnode == NULL)break;
			pnode->m_nResult = SPRDLG_PROCESS_KILLME;

		}break;
*/
	case IDM_GRADE:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_GAME_USE_GRADE);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			CGradeSpr *pObj = new CGradeSpr;
			if( pObj == NULL )break;
			pObj->Init(0,&Page);
			GAME()->m_cAniSprObjectMan.AddObject(pObj);
		}break;
	case IDM_BTN_EXCESSMONEY:
			return true;
			// 본인 카드 오픈 버튼
	case IDM_CARD_OPEN:
		{
			// 버튼 spr 인덱스가 0이면 본인 카드 오픈 On, 1 이면 Off
			int nBtnNum = g_pGameView->m_cGameViewBtnMan.m_btnCardOpen.GetButtonNumber(); // 현재 버튼 상태(On or Off)
			//int nBtnNum = 0;
			bool bUIFoldOpen = false;
			
			// 버튼이 Off 면 On 시켜줌 -> 버튼 상태는 서버 결과에 따라 변경 하기로 함
			if (nBtnNum == 1)
			{
				bUIFoldOpen = true;
			}
			
			// 버튼 spr 인덱스를 Toggle 시킴
			//g_pGameView->m_cGameViewBtnMan.m_btnCardOpen.SetSprite(&(g_pGameView->m_cGameViewBtnMan.m_sprUICardOpenBtn), nBtnNum);
			
			// 서버에 알림
			CCL_ASK_CARDOPEN msg;
			msg.Set(bUIFoldOpen);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
	default:
		{
			//[vsmode]070314
			if(g_pChatWinMan != NULL )g_pChatWinMan->OnCommand(wParam);
			AutoBetSetUpWnd().OnCommand(wParam);
			
			//원샷충전
			if(g_cOneShotCharge.OnCommand(wParam, lParam)) return TRUE;
			
			//자리 지킴이
			if(g_SeatKeeper.OnCommand(wParam, lParam)) return TRUE;
		}
	}	
	
	
	return CGameViewBase::OnCommand(wParam, lParam);
}

/////////////////////////////////////////////////////////////////////////////////

// [세븐 포커] 베팅 버튼
void CGameViewNewPoker::OnBtnPing()
{
	if(g_ObserverMan.IsMyObserver() == TRUE) return;
	CPlayer_NP *pPlayer = GAME()->GetPlayerNP(0);

	if( !m_cGameViewBtnMan.m_bBPing || pPlayer->PlayState == 0) return;
	m_cGameViewBtnMan.SetBettinBtnDiable();

	// 따당이 예약되어 있어도 상태방이 체크를 선택해서 따당이 실패하면 예약 취소를 위해서 넣어줌.
	GM().GetCurrentProcess()->SetReserveButton(RESERVESTATE_BET_NONE);

	POKERCLNT pc; //MakeBasic(pc);
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 3; // 배팅
	pc.nBtn   = BET_CMD_PING; // 삥
	pc.PlayTick = GetCurrentPlayTick(); 

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  pPlayer->UI.UNum, pPlayer->ServPNum, pPlayer->UI.ID, &pc);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), raise.pData, raise.GetTotalSize());

	g_pGameView->m_cGameViewBtnMan.SetButtonClick(true);
}

// 따당 -> 풀버튼 기능으로 변경(규제안관련 작업 2014.01)
void CGameViewNewPoker::OnBtnDda()
{
	if(g_ObserverMan.IsMyObserver() == TRUE) return;
	CPlayer_NP *pPlayer = GAME()->GetPlayerNP(0);

	if( !m_cGameViewBtnMan.m_bBDDa || pPlayer->PlayState == 0) return;

	// 뉴포커만 command 처리가 process가 아니라 view에서 처리해서 여기서 호출.
	// 규제안 베팅예약 공통화
	if(false == GM().GetCurrentProcess()->CheckMyTurn(GM().GetCurrentProcess()->GetPlayer(0)->ServPNum, GM().GetCurrentProcess()->m_nCurPlayer))
	{
#if defined(BUTTON_CHANGE_FULLTODADANG)
		if(true==GM().IsChangeFullDDang()){
			GM().GetCurrentProcess()->SetReserveButton(RESERVESTATE_BET_BET_CMD_FULL);
		}
		else{
			GM().GetCurrentProcess()->SetReserveButton(RESERVESTATE_BET_BET_CMD_DADANG);
		}
#else
		GM().GetCurrentProcess()->SetReserveButton(RESERVESTATE_BET_BET_CMD_DADANG);
#endif
		return;
	}
	GM().GetCurrentProcess()->SetReserveButton(RESERVESTATE_BET_NONE);

	m_cGameViewBtnMan.SetBettinBtnDiable();

	POKERCLNT pc; //MakeBasic(pc);
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 3; // 배팅
#if defined(BUTTON_CHANGE_FULLTODADANG)
	if(true==g_RI.bIsFriendRoom && pPlayer->UI.GetMoney() >= GM().GetLossMaxMoneyForGame()){
		pc.nBtn   = BET_CMD_MAX_BET;
	}
	else if(true==GM().IsChangeFullDDang()){
		pc.nBtn   = BET_CMD_FULL;
	}
	else{
		pc.nBtn   = BET_CMD_DADANG;//BET_CMD_FULL;//BET_CMD_DADANG; // 따당에서 풀로교체(규제안관련 작업 2014.01)->추후작업
	}
#else
	pc.nBtn   = BET_CMD_DADANG;//BET_CMD_FULL;//BET_CMD_DADANG; // 따당에서 풀로교체(규제안관련 작업 2014.01)->추후작업
#endif
	pc.PlayTick = GetCurrentPlayTick(); 

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  pPlayer->UI.UNum, pPlayer->ServPNum, pPlayer->UI.ID, &pc);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), raise.pData, raise.GetTotalSize());
	g_pGameView->m_cGameViewBtnMan.SetButtonClick(true);
}

void CGameViewNewPoker::OnBtnMax()
{
	if(g_ObserverMan.IsMyObserver() == TRUE) return;
	CPlayer_NP *pPlayer = GAME()->GetPlayerNP(0);

	if( !m_cGameViewBtnMan.m_bBMax || pPlayer->PlayState == 0 ) return;

	// 뉴포커만 command 처리가 process가 아니라 view에서 처리해서 여기서 호출.
	// 규제안 베팅예약 공통화
	if(false == GM().GetCurrentProcess()->CheckMyTurn(GM().GetCurrentProcess()->GetPlayer(0)->ServPNum, GM().GetCurrentProcess()->m_nCurPlayer))
	{
		GM().GetCurrentProcess()->SetReserveButton(RESERVESTATE_BET_BET_CMD_HALF);
		return;
	}
	GM().GetCurrentProcess()->SetReserveButton(RESERVESTATE_BET_NONE);

	m_cGameViewBtnMan.SetBettinBtnDiable();
	
	POKERCLNT pc;
	ZeroMemory(&pc, sizeof(POKERCLNT));
	
	pc.nStyle = 3; // 배팅
	int spnum = GAME()->GetServPNum_ByPN(0);			
	INT64 CallMoney = GAME()->GetRaiseBat() - g_Poker.RU[spnum].nNowBat;
	INT64 nTotal = GAME()->GetRealBet();
	if( pPlayer->UI.GetMoney() > CallMoney && pPlayer->UI.GetMoney() < GAME()->GetMaxMoney(nTotal)){
		pc.nBtn   = BET_CMD_ALLIN; // 맥스
	} 
	else{
		pc.nBtn   = BET_CMD_HALF; // 맥스
	}	
	
	pc.PlayTick = GetCurrentPlayTick(); 

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  pPlayer->UI.UNum, pPlayer->ServPNum, pPlayer->UI.ID, &pc);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), raise.pData, raise.GetTotalSize());
	g_pGameView->m_cGameViewBtnMan.SetButtonClick(true);
}

void CGameViewNewPoker::OnBtnFold()
{
	if(g_ObserverMan.IsMyObserver() == TRUE) return;
	CPlayer_NP *pPlayer = GAME()->GetPlayerNP(0);

	if(g_Poker.nState != RSTATE_PREPARESTART){
		if(!m_cGameViewBtnMan.m_bFold || pPlayer->PlayState == 0) return;
	}

	if( g_Poker.nAutoBetCmd > 0 && g_RI.nAutoBetKind > 0 ){
		return;
	}

	// 뉴포커만 command 처리가 process가 아니라 view에서 처리해서 여기서 호출.
	// 규제안 베팅예약 공통화
	if(false == GM().GetCurrentProcess()->CheckMyTurn(GM().GetCurrentProcess()->GetPlayer(0)->ServPNum, GM().GetCurrentProcess()->m_nCurPlayer))
	{
		GM().GetCurrentProcess()->SetReserveButton(RESERVESTATE_BET_BET_CMD_FOLD);
		return;
	}
	GM().GetCurrentProcess()->SetReserveButton(RESERVESTATE_BET_NONE);
	GM().GetCurrentProcess()->CanReserve(false);
		
	m_cGameViewBtnMan.SetBettinBtnDiable();

	POKERCLNT pc; 
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 0; // 게임포기
	pc.nBtn   = 0; // 삥
	pc.PlayTick = GetCurrentPlayTick(); 

	CCL_ASK_FOLD fold;
	fold.Set(g_RI.RoomNum,  pPlayer->UI.UNum, pPlayer->ServPNum, pPlayer->UI.ID, &pc);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), fold.pData, fold.GetTotalSize());
	g_pGameView->m_cGameViewBtnMan.SetButtonClick(true);
}

void CGameViewNewPoker::OnBtnCheck()
{
	if(g_ObserverMan.IsMyObserver() == TRUE) return;
	CPlayer_NP *pPlayer = GAME()->GetPlayerNP(0);

	if( !m_cGameViewBtnMan.m_bBCheck || pPlayer->PlayState == 0) return;

	m_cGameViewBtnMan.SetBettinBtnDiable();

	POKERCLNT pc; //MakeBasic(pc);
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 1; // 배팅
	pc.nBtn   = 0; // 삥
	pc.PlayTick = GetCurrentPlayTick(); 

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  pPlayer->UI.UNum, pPlayer->ServPNum, pPlayer->UI.ID, &pc);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), raise.pData, raise.GetTotalSize());
	g_pGameView->m_cGameViewBtnMan.SetButtonClick(true);
}

void CGameViewNewPoker::OnBtnCall()
{
	if(g_ObserverMan.IsMyObserver() == TRUE) return;
	CPlayer_NP *pPlayer = GAME()->GetPlayerNP(0);

	if( !m_cGameViewBtnMan.m_bBCall || pPlayer->PlayState == 0) return;

	// 뉴포커만 command 처리가 process가 아니라 view에서 처리해서 여기서 호출.
	// 규제안 베팅예약 공통화
	if(false == GM().GetCurrentProcess()->CheckMyTurn(GM().GetCurrentProcess()->GetPlayer(0)->ServPNum, GM().GetCurrentProcess()->m_nCurPlayer))
	{
		GM().GetCurrentProcess()->SetReserveButton(RESERVESTATE_BET_BET_CMD_CALL);
		return;
	}
	GM().GetCurrentProcess()->SetReserveButton(RESERVESTATE_BET_NONE);

	m_cGameViewBtnMan.SetBettinBtnDiable();

	POKERCLNT pc; 
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 2; // 배팅
	pc.nBtn   = 0; // 삥
	pc.PlayTick = GetCurrentPlayTick(); 

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  pPlayer->UI.UNum, pPlayer->ServPNum, pPlayer->UI.ID, &pc);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), raise.pData, raise.GetTotalSize());
	g_pGameView->m_cGameViewBtnMan.SetButtonClick(true);
}

// ### [ 관전기능 ] ###
//관전, 참여

void CGameViewNewPoker::draw_card_func(CDC *mdc)
{	
	if(g_RI.State == 1 || g_pGameView->m_cPlayerDrawManager.IsPlayerGameOver(0))	
	{
		//Page.SetTempClip(0,23, GM().GetWidth(),768-23);
		//Page.EnableTempClip(TRUE);

		GAME()->m_cAllBetChip.DrawBottom();

		const int nMaxPlayer = GAME()->GetMaxPlayer();
// 		for(int x = 0 ; x < nMaxPlayer ; x ++)
// 		{					
// 			GAME()->GetPlayerNP(x)->OnDraw(mdc);
// 		}

		if(g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME){
			GAME()->m_cBombCard.Draw(&Page);
		}

		// 유저 카드 그린다.
		CPlayer_NP *pPlayer = NULL;
		for(int i=0; i<nMaxPlayer; i++) 
		{
			pPlayer = GAME()->GetPlayerNP(i);
			if(pPlayer->JoinState != 0 )	
			{
				pPlayer->OnCardDraw(&Page, pPlayer->bFold, pPlayer->bFoldOpen);
			}
		}
	}
}

void CGameViewNewPoker::draw_btn_func(CDC &MemDC)
{
	m_cGameViewBtnMan.DrawBtn(&MemDC);
}

void CGameViewNewPoker::DrawPlayBottom(CDC *pDC)
{
	GAME()->GetPlayerNP(2)->OnDraw_Bottom(pDC);
	GAME()->GetPlayerNP(3)->OnDraw_Bottom(pDC);
	GAME()->GetPlayerNP(4)->OnDraw_Bottom(pDC);
	GAME()->GetPlayerNP(1)->OnDraw_Bottom(pDC);
	GAME()->GetPlayerNP(0)->OnDraw_Bottom(pDC);
}
void CGameViewNewPoker::DrawPlayMiddle(CDC *pDC)
{
	GAME()->GetPlayerNP(2)->OnDraw_Middle(pDC);
	GAME()->GetPlayerNP(3)->OnDraw_Middle(pDC);
	GAME()->GetPlayerNP(4)->OnDraw_Middle(pDC);
	GAME()->GetPlayerNP(1)->OnDraw_Middle(pDC);
	GAME()->GetPlayerNP(0)->OnDraw_Middle(pDC);
}
void CGameViewNewPoker::DrawPlayTop(CDC *pDC)
{
	GAME()->GetPlayerNP(2)->OnDraw_Top(pDC);
	GAME()->GetPlayerNP(3)->OnDraw_Top(pDC);
	GAME()->GetPlayerNP(4)->OnDraw_Top(pDC);
	GAME()->GetPlayerNP(1)->OnDraw_Top(pDC);
	GAME()->GetPlayerNP(0)->OnDraw_Top(pDC);
}

//2006.1.9 스피드핵
DWORD CGameViewNewPoker::GetCurrentPlayTick()
{
	return timeGetTime()-g_nStartTick;
}

// [ ###잭팟### ] 
void CGameViewNewPoker::DrawJackPot()
{

#if defined(_JACKPOT_NON_CHANNEL_MONEY)
	INT64 &n64JackPotMoney	  = GDM().GetINT64(GDK_JACKPOT_MONEY);
	INT64 &n64TmpJackPotMoney = GDM().GetINT64(GDK_JACKPOT_MONEY_TEMP);
#else
	INT64 &n64JackPotMoney	   = GAME()->GetOldJackPotMoney();
	INT64 &n64TmpJackPotMoney  = GAME()->GetJackPotMoney();
#endif

	INT64 minus = n64TmpJackPotMoney - n64JackPotMoney;
	INT64 plus = 0;

	if(n64TmpJackPotMoney > n64JackPotMoney)
	{
		if(		minus > 10000000000)plus = 987654321;
		else if(minus > 1000000000) plus = 98765431;
		else if(minus > 100000000)  plus = 9876541;
		else if(minus > 10000000)   plus = 987651;
		else if(minus > 1000000)    plus = 98761;
		else if(minus > 100000)	    plus = 9871;
		else if(minus > 10000)	    plus = 981;
		else if(minus > 1000)       plus = 91; 
		else if(minus > 50 )        plus = 9;
		else plus = 1;

		n64JackPotMoney += plus;// 증가 보여주기
	}
	else  if(n64TmpJackPotMoney < n64JackPotMoney)
	{
		if(		minus < -100000000000000000) plus = 9999999987654321;
		else if(minus < -10000000000000000)  plus = 999999987654321;
		else if(minus < -1000000000000000)   plus = 99999987654321;
		else if(minus < -100000000000000)    plus = 9999987654321;
		else if(minus < -10000000000000)     plus = 999987654321;
		else if(minus < -1000000000000)      plus = 99987654321;
		else if(minus < -100000000000)       plus = 9987654321;
		else if(minus < -10000000000)        plus = 987654321;
		else if(minus < -1000000000)         plus = 98765431;
		else if(minus < -100000000)	         plus = 9876521;
		else if(minus < -10000000)           plus = 987651;
		else if(minus < -1000000)            plus = 98761;
		else if(minus < -100000)	         plus = 9871;
		else if(minus < -10000)	             plus = 981;
		else if(minus < -1000)               plus = 91; 
		else if(minus < -50 )                plus = 9;
		else plus = 1;

		n64JackPotMoney -= plus;// 증가 보여주기
	}
	
	INT64 tmp = n64JackPotMoney;
	int kk = 0;			
	int addx = 0;
	int count = 0;	
	
	Page.PutSprAuto( g_cUIData.m_ptJacpotBack.x, g_cUIData.m_ptJacpotBack.y, &g_sprJackPot,11); 
	for( int j = 0; j < 20 ; j ++ ) 
	{
		if( tmp <= 0 ) 	
		{				
			// 잭팟을 받은경우 초기화 0으로 한다.
			if(tmp == 0 && 0 == n64JackPotMoney && 0 == n64TmpJackPotMoney)
			{
				Page.PutSprAuto( g_cUIData.m_ptJacpotNumberStart.x + addx - (j + 1) * g_cUIData.m_nJacpotNumberWidth, g_cUIData.m_ptJacpotNumberStart.y, &g_sprJackPot, 0 );
			}
			break;
		}
		if(kk >= 3)	{
			kk = 0;
			Page.PutSprAuto( g_cUIData.m_ptJacpotNumberStart.x + addx - (j + 1) * g_cUIData.m_nJacpotNumberWidth, g_cUIData.m_ptJacpotNumberStart.y, &g_sprJackPot, 10 ); // ,
			addx -= 6;
			count++;
		}
		Page.PutSprAuto( g_cUIData.m_ptJacpotNumberStart.x + addx - (j + 1) * g_cUIData.m_nJacpotNumberWidth, g_cUIData.m_ptJacpotNumberStart.y, &g_sprJackPot,(int)(tmp%(INT64)10) );
		tmp = tmp / 10;
		kk++;
	}
}

void CGameViewNewPoker::MakeAVI(CDC *pDC)
{
}

BOOL CGameViewNewPoker::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_SYSKEYDOWN)
	{
		if((pMsg->lParam & 0x20000000) > 0 && (pMsg->wParam == '1' || pMsg->wParam == '2' || pMsg->wParam == '3' ||	
			pMsg->wParam == '4' || pMsg->wParam == '5')) 
		{			
			int num = (pMsg->wParam - 49)+5;
			if(num >= 0 && num < 10){
				CString str = g_ChatMacro.Macrostr[num];
				g_pChatWinMan->SystemKeyReturn(str);
			}
		}		
	}

	switch(pMsg->message)
	{
	case WM_CHAR:
		break;
	case WM_KEYDOWN:
		{
			// 메크로 
			if(GetAsyncKeyState(VK_CONTROL) && (pMsg->wParam == '1' || pMsg->wParam == '2' || pMsg->wParam == '3' || pMsg->wParam == '4' || pMsg->wParam == '5'))
			{
				int num = (pMsg->wParam - 49);
				if(num >= 0 && num < 10){
					CString str = g_ChatMacro.Macrostr[num];
					g_pChatWinMan->SystemKeyReturn(str);
				}
			} 		
			
			// 센드 에디트가 아닌 다른곳에 포커스가 있다면 게임키로 인식
			if(!g_pChatWinMan->IsEditFocus())
			{
				//////////
				// 숫자키패드 게임키 추가 2004.07.21
				int scankey = 0, extended = 0, previous = 0;
				BOOL bGameKey = IsSacnCode(pMsg->lParam, scankey, extended, previous);

				if((pMsg->wParam == VK_RETURN && extended == 0 )|| pMsg->wParam == VK_ESCAPE) {				
			
					g_pGameView->m_cSprDlgMan.DelObject();
					//2009.11.17 정책 변경
					g_pChatWinMan->SetEditFocus();
					return TRUE;
				}

	#ifdef _TEST
				switch( pMsg->wParam )
				{
				case VK_F9:
					OwnLimit().ShowAniEffect(0);
					OwnLimit().ShowAniEffect(1);
					OwnLimit().ShowAniEffect(2);
					OwnLimit().ShowAniEffect(3);
					OwnLimit().ShowAniEffect(4);
					return TRUE;

				case VK_F11:
					g_FamilyIcon.SetExcessMoneyState(ICS_ON, 987654321);
					return TRUE;

				case VK_F12:
					g_FamilyIcon.SetExcessMoneyState(ICS_OFF, 123456789 );
					return TRUE;
				}
	#endif

				
				if(pMsg->wParam == VK_SPACE && g_RI.State == 0)
				{
					g_pGameView->SendMessage(WM_COMMAND,IDM_GAMESTART,0);
					return TRUE;
				}
				
				if ( GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME )
				{
					GAME()->OnGameKey( scankey, extended, previous );
				}			
				return TRUE;
			}
			else {
				// 숫자키패드게임키추가2004.07.21
				int scankey = 0, extended = 0, previous = 0;
				BOOL bGameKey = IsSacnCode(pMsg->lParam, scankey, extended, previous);

				// 복사 붙여넣기 막는 작업.
				bool bPushCtrl = false;
				bool bPushShift = false;
				if(GetAsyncKeyState(VK_LCONTROL) & 0x8000) bPushCtrl = true;
				if(GetAsyncKeyState(VK_LSHIFT) & 0x8000) bPushShift = true;

				if(bPushCtrl)
				{
					switch( scankey )
					{
					case VK_S_V:		
						return TRUE;
					case VK_S_C:		
						return TRUE;
					case VK_S_X:		
						return TRUE;
					}
				}

				// 채팅 창에서 예외 키
				CString str = g_pChatWinMan->GetEditText();
				if(str == "" && (pMsg->wParam == VK_UP || pMsg->wParam == VK_SPACE||
					pMsg->wParam == VK_RIGHT || pMsg->wParam == VK_LEFT || pMsg->wParam == VK_DOWN))			
				{
					g_pGameView->SetFocus();
					if(GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
					{
						GAME()->OnGameKey( scankey, extended, previous );
					}
				
					if(pMsg->wParam == VK_SPACE && g_RI.State == 0)
					{
						g_pGameView->SendMessage(WM_COMMAND,IDM_GAMESTART,0);
						return TRUE;
					}
					
					return TRUE;
				}
				
				
				// 채팅 창에서 예외 키
				if(pMsg->wParam == VK_INSERT || pMsg->wParam == VK_HOME || pMsg->wParam == 33 ||
					pMsg->wParam ==  VK_DELETE ||pMsg->wParam == VK_END ||pMsg->wParam ==  34 ||
					pMsg->wParam == 0x09 || pMsg->wParam == VK_F2 || pMsg->wParam == VK_F3    ||
					pMsg->wParam == VK_F4 || pMsg->wParam == VK_F5 || pMsg->wParam == VK_F6   ||
					pMsg->wParam == VK_F7 || pMsg->wParam == VK_F8 || pMsg->wParam == VK_DOWN)
				{
					if(GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
					{
						GAME()->OnGameKey( scankey, extended, previous );
					}

					return TRUE;
				}
				
				// ### [ 관전기능 ] ###
				if(g_pMyInfo_NP->PlayState == 1 && g_Poker.nRound==4 && (pMsg->wParam == VK_UP /*|| pMsg->wParam == VK_NUMPAD5*/) )
				{
					if(GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
					{
						GAME()->OnGameKey( scankey, extended, previous );
					}

					return TRUE;
				}
				
				if(pMsg->wParam == VK_ESCAPE) {
					// 게임뷰에 포커스를 옮긴다
					g_pGameView->m_cSprDlgMan.DelObject();
					SetFocus();
					return TRUE;
				}
			}		
		}
		break;
	}
		
	return CWnd::PreTranslateMessage(pMsg);
}

BOOL CGameViewNewPoker::IsSacnCode(int nScanCode, int &r_scankey, int &r_extended, int &r_previous)
{
	// 스캔코드를분석함
	// 스캔키코드
	r_scankey = (nScanCode >> 16) & 0x000000FF; 
	// 확장키인지여부(1이면확장키임- 숫자키패드방향키는비확장키보드임)
	r_extended = (nScanCode >> 24) & 0x00000001; 
	// 키의이전상태(1이면이전에눌려진상태임)
	r_previous = (nScanCode >> 30) & 0x00000001; 
	
	// 게임에서사용하는키인가검사
	if( ( r_scankey == 79 && r_extended == 0 ) || // 키패드1		가로삥		| 세로따당
		( r_scankey == 80 && r_extended == 0 ) || // 키패드2		가로따당	| 세로쿼터
		( r_scankey == 81 && r_extended == 0 ) || // 키패드3		가로쿼터	|
		( r_scankey == 75 && r_extended == 0 ) || // 키패드4		가로다이	| 세로삥
		( r_scankey == 76 && r_extended == 0 ) || // 키패드5		가로체크	| 세로체크
		( r_scankey == 77 && r_extended == 0 ) || // 키패드6		가로콜		|
		( r_scankey == 71 && r_extended == 0 ) || // 키패드7					| 세로콜
		( r_scankey == 72 && r_extended == 0 ) || // 키패드8					| 세로다이
		( r_scankey == 73 && r_extended == 0 ) || // 키패드9					| 
		( r_scankey == 82 && r_extended == 0 ) || // 키패드0					| 세로하프
		( r_scankey == 83 && r_extended == 0 ) || // 키패드.					| 세로하프
		( r_scankey == 53 && r_extended == 1 ) || // 키패드/					| 세로하프
		( r_scankey == 55 && r_extended == 0 ) || // 키패드*					| 세로하프
		( r_scankey == 74 && r_extended == 0 ) || // 키패드-					| 세로하프
		( r_scankey == 78 && r_extended == 0 ) || // 키패드+		가로하프	|
		( r_scankey == 28 && r_extended == 1 )   // 키패드enter	가로풀		|
		)  // 콜
	{
		return true;
	}
	return FALSE;
}

void CGameViewNewPoker::ShowEffect_AllowChancePurchase(int nChanceItemNo)		// 찬스아이템 획득시 효과를 보여준다.
{
	//행운찬스는 채팅창에 알려주지 않는다.
// 	if( nChanceItemNo == ITEM_SCODE_REWARD_LUCKAVATA )return;
// 
// 	CString str;
// 	str.Format( "◈[%s]님 %s \n구입자격 획득.\n", 
// 		GAME()->GetPlayerNP(0)->UI.NickName, RewardItemMan().GetRewardItemName( nChanceItemNo ));
// 
// 	AddGameInfoViewEdit(str, GetChatColor(ECC2_USEITEM), false);

	//찬스 이펙트
	CChance_Msg_EffectSpr *pObj = new CChance_Msg_EffectSpr;
	if( pObj != NULL && g_pGameView != NULL)
	{
		// 어떻게 이겼는가?(모두 죽어서 홀로 이겼는가 FALSE, 끝까지 베팅했는가 TRUE) - 기획팀에서 기권등에도 나오도록 해달라한다.
		//if(g_pGameView->m_cPlayerDrawManager.IsOpen())
		{
			pObj->Init(nChanceItemNo);
			//g_PLAYER(0)->m_cPlayerEffectList.AddObject(pObj);
			GAME()->m_cAniSprObjectMan.AddObject(pObj);
		}		
	}

}

BOOL CGameViewNewPoker::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return TRUE;
}

void CGameViewNewPoker::SelectChoiceCard(int index,  int discard , BOOL bautoselect/* = FALSE*/)
{
	if( GAME()->m_cSelectCardMan.IsChoiceCard() == FALSE )return;
	GAME()->m_cSelectCardMan.SetChoiceCard(false);
	// 사용자카드재배치
	
	GAME()->m_nDebugCnt ++;
	
	int aBuf[4] ={0,};
	aBuf[0] = GAME()->GetPlayerNP(0)->GetCardNo(0);
	aBuf[1] = GAME()->GetPlayerNP(0)->GetCardNo(1);
	aBuf[2] = GAME()->GetPlayerNP(0)->GetCardNo(2);
		
	aBuf[3] = GAME()->GetPlayerNP(0)->GetCardNo(3);	
	int select = GAME()->GetPlayerNP(0)->GetCardNo( index-1 );
	
	int i=0;
	if(discard != 4) {
		for( i = discard-1; i < 3; i++) {
			aBuf[i] = aBuf[i+1];
			GAME()->GetPlayerNP(0)->m_listCard[i].SetCardNo(aBuf[i]);
			aBuf[i+1] = 0;
		}
	}
	for(i = 0; i < 3; i++) {
		if(aBuf[i] == select) // 카드값
		{	
			index = i+1;
			break;
		}
	}
	
	// 기존 방법
	int cnt = 0;
	for( i=0;i<3;i++) {
		if(index!=i+1){
			GAME()->GetPlayerNP(0)->m_listCard[cnt].SetCardNo(aBuf[i]);
			cnt++;
		}
	}
	
	//#################################################
	// 카드덱에서 제거
	GAME()->GetPlayerNP(0)->m_listCard[3].Clear();
	GAME()->GetPlayerNP(0)->m_nTotalCardNum -= 1;
	//#################################################
	
	//서버로 내가 선택한 그림 보내준다.
	POKERCLNT pc={0,}; 
	pc.nStyle = 3; // 배팅
	pc.nBtn   = index; // 선택카드 (인덱스 1 , 2, 3)
	
	CCL_ASK_SELECTCARD scard;
	scard.Set(g_RI.RoomNum,  GAME()->GetPlayerNP(0)->UI.UNum, GAME()->GetPlayerNP(0)->ServPNum, GAME()->GetPlayerNP(0)->UI.ID, &pc, discard);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), scard.pData, scard.GetTotalSize());	
	
	GAME()->m_cSelectCardMan.SetChoiceCard(false);
	GAME()->GetPlayerNP(0)->SetChioceCardMove(TRUE , 1);

	//2006.3.20
	//자동 선택이 아닐 경우 초기화 한다
	if( bautoselect == FALSE ){
		GAME()->GetPlayerNP(0)->m_nCardChoiceLimtCnt = 0;
	}
}

BOOL CGameViewNewPoker::OnCardLButtonDown(int xp, int yp)
{
	if(!g_ObserverMan.IsMyObserver() && !g_pGameView->m_cPlayerDrawManager.IsFlyWindCard() && GAME()->GetPlayerNP(0)->JoinState != 0 && GAME()->GetPlayerNP(0)->PlayState != 0 && GAME()->GetPlayerNP(0)->m_nCardTotal == GAME()->GetTotalCardNum())	
	{
		POINT pt;
		pt.x = xp;
		pt.y = yp;

		if(g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME)
		{
			if( GAME()->GetPlayerNP(0)->m_listCard[7].PtInCard(xp, yp) )
			{
				GAME()->SetMouseHiddenState( 1 );
				GAME()->SetMouseHiddenClick(TRUE);
				GAME()->SetHiddenClickPos(pt);

// 				GAME()->m_bHiddenClick = TRUE;
// 				GAME()->m_ptMouse.x = xp;
// 				GAME()->m_ptMouse.y = yp;
				GAME()->GetPlayerNP(0)->SetFaceUp(7); // 보이기
				return TRUE;
			}				
		}
		else 
		{
			if( GAME()->GetPlayerNP(0)->m_listCard[6].PtInCard(xp, yp) )
			{
				GAME()->SetMouseHiddenState( 1 );
				GAME()->SetMouseHiddenClick(TRUE);
				GAME()->SetHiddenClickPos(pt);
			
// 				GAME()->m_bHiddenClick = TRUE;
// 				GAME()->m_ptMouse.x = xp;
// 				GAME()->m_ptMouse.y = yp;
				GAME()->GetPlayerNP(0)->SetFaceUp(6); // 보이기
				return TRUE;
			}				
		}
		GAME()->SetMouseHiddenClick(FALSE);
		//GAME()->m_bHiddenClick = FALSE;
	}

	return FALSE;
}

BOOL CGameViewNewPoker::OnCardLButtonUp(int xp, int yp)
{
	////////////////////////////
	if(!g_ObserverMan.IsMyObserver() && !g_pGameView->m_cPlayerDrawManager.IsFlyWindCard() && GAME()->GetPlayerNP(0)->JoinState != 0 && GAME()->GetPlayerNP(0)->PlayState != 0 
		&& GAME()->GetPlayerNP(0)->m_nCardTotal == GAME()->GetTotalCardNum() && !g_pGameView->m_cPlayerDrawManager.IsHiddenCardShow() )
	{
		//if( GAME()->m_bHiddenClick ) 
		if( GAME()->IsMouseHiddenClick() )
		{
			GAME()->GetPlayerNP(0)->HiddenOpne();
		}
	}
	////////////////////////////
	return FALSE;
}

BOOL CGameViewNewPoker::OnCardMouseMove(int xp, int yp)
{
	// 다이 일때 내카드 열기
	if(!g_ObserverMan.IsMyObserver() && GAME()->GetPlayerNP(0)->JoinState != 0 && GAME()->GetPlayerNP(0)->PlayState == 0 && GAME()->GetPlayerNP(0)->bFold == TRUE)
	{	
		CPoint pt;
		CRect rt;
		int tc = GAME()->GetPlayerNP(0)->m_nCardTotal;
		int w = (g_cUIData.m_nCardGab * (tc-1)) + g_szCard.cx;		

		rt.SetRect(0,0,w,g_szCard.cy);
		rt.OffsetRect(g_cUIData.m_listCardStartPos[0]);
		pt.x = xp;
		pt.y = yp;

		if(rt.PtInRect(pt)){
			for(int i = 0 ; i < GAME()->GetPlayerNP(0)->m_nTotalCardNum ; i ++){
				if(GAME()->GetPlayerNP(0)->GetFace(i) == FALSE){
					GAME()->GetPlayerNP(0)->SetFaceUp(i);
				}
			}
		}
		else{
			for(int i = 0 ; i < GAME()->GetPlayerNP(0)->m_nTotalCardNum ; i ++){
				if(GAME()->GetPlayerNP(0)->GetFace(i) == TRUE){
					GAME()->GetPlayerNP(0)->SetFaceDown(i);
				}
			}
		}		
	}	

	////////////////////////////
	if(!g_ObserverMan.IsMyObserver() && !g_pGameView->m_cPlayerDrawManager.IsFlyWindCard() && GAME()->GetPlayerNP(0)->JoinState != 0 && GAME()->GetPlayerNP(0)->PlayState != 0 
		&& GAME()->GetPlayerNP(0)->m_nCardTotal == GAME()->GetTotalCardNum() && !g_pGameView->m_cPlayerDrawManager.IsHiddenCardShow() )
	{
		//if( GAME()->m_bHiddenClick ) 
		if( GAME()->IsMouseHiddenClick() && g_pGameView->m_cPlayerDrawManager.IsHiddenCardShow() == FALSE )
		{

			GAME()->GetPlayerNP(0)->GetHiddenCardMoveWay( xp, yp );

// 			if(GAME()->m_ptMouse.y < yp) 
// 			{
// 				int tmp = yp - GAME()->m_ptMouse.y;
// 				GAME()->m_nHinddenArrow += tmp;
// 				GAME()->m_ptMouse.y = yp;
// 			}
// 			else if(GAME()->m_ptMouse.y > yp) 
// 			{
// 				int tmp = GAME()->m_ptMouse.y - yp;
// 				GAME()->m_nHinddenArrow -= tmp;
// 				if(GAME()->m_nHinddenArrow <=0) 
// 				{
// 					GAME()->m_nHinddenArrow =0;
// 					GAME()->m_bHiddenClick = FALSE;
// 				}
// 				GAME()->m_ptMouse.y = yp;
// 			}
		}
	}
	
	return FALSE;
}

void CGameViewNewPoker::DrawMyJokboAndJokerEffect(CDC *pDC)
{
	CFont *oldf = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
/*
	if(!g_ObserverMan.IsMyObserver() && GAME()->GetPlayerNP(0)->PlayState != 0 
		&& GAME()->GetPlayerNP(0)->m_nCardTotal == GAME()->GetTotalCardNum() && !g_pGameView->m_cPlayerDrawManager.IsFlyWindCard())
	{		
		if( !g_pGameView->m_cPlayerDrawManager.IsHiddenCardShow())
		{
			CPoint ptPos;
			int nCardIndex = 6;
			switch(g_RI.FormKind)
			{
				case 0:
				case 1:
					nCardIndex = 6;
					ptPos = GAME()->GetPlayerNP(0)->m_listCard[6].GetPos();
					break;
				case 2:
				case 3:
					nCardIndex = 7;
					ptPos = GAME()->GetPlayerNP(0)->m_listCard[7].GetPos();
					break;
			}
		
			int hX = GAME()->GetPlayerNP(0)->m_listCard[ nCardIndex ].GetPos().x + GAME()->GetHiddenArrowPosX();			
			int hY = GAME()->GetPlayerNP(0)->m_listCard[ nCardIndex ].GetPos().y + GAME()->GetHiddenArrowPosY();			

			if( m_bHiddenScene )
			{
				DWORD dwTemp = 0;

				int mxp = abs( GAME()->GetHiddenArrowPosX() );			
				int myp = abs( GAME()->GetHiddenArrowPosY() );	

				if( mxp > myp )
				{
					dwTemp = mxp * 28;
				}
				else
				{
					dwTemp = myp * 28;
				}

				//히든 오픈 애니메이션이 끝났다.
				if( dwTemp >= 1300 ) 
				{
					GAME()->SetMouseHiddenState( 2 );
					GAME()->SetHiddenSceneDrawTime();
				}
				else
				{
					Page.DrawMultiScene( ptPos.x, ptPos.y, &g_sprLastCardOpen, 0, dwTemp, FALSE );
				}
			}
			else
			{
				// 히든 카드이미지
				Page.PutSprAuto( hX, hY, &g_sprBigCard, 52);		
			}

			if( GAME()->IsMouseHiddenClick() == FALSE && GAME()->IskeyboardHiddenClick() == FALSE )
			{
				pDC->SetTextColor(RGB(255,225,255));
				pDC->TextOut(ptPos.x + 23,550,"↑↓");

				// 히든카드 오픈 단축키 이미지를 보여준다.
				Page.PutSprAuto( ptPos.x + 9, ptPos.y + 62, &GLOBAL_SPR[ spr::GSPR_V_H_OPEN ], 0 ); //< CYS 100705 >
			}

			//오픈애니메이션추가
			if( GAME()->GetCardOpneGuideIndex() != -1 && GAME()->GetCardOpneGuideDraw() )
			{
				Page.DrawMultiScene( ptPos.x, ptPos.y, &g_sprLastCardOpen, GAME()->GetCardOpneGuideIndex(), timeGetTime() - GAME()->Getm_dwCardOpenGuideTime(), FALSE );

				if( g_sprLastCardOpen.IsMSceneTimeOver(  GAME()->GetCardOpneGuideIndex(), timeGetTime() - GAME()->Getm_dwCardOpenGuideTime() ) )
				{
					GAME()->EndCardOpenGuide();
				}
			}

// 			ptPos.y += GAME()->m_nHinddenArrow;
// 			int sprno = 52;	
// 			Page.PutSprAuto(ptPos.x, ptPos.y, &g_sprBigCard, sprno);
// 				
// 			pDC->SetTextColor(RGB(255,225,255));
// 			pDC->TextOut(ptPos.x + 23,550,"↑↓");
// 
// 			// 히든이미지 추가
// 			Page.PutSprAuto( ptPos.x + 9, ptPos.y + 62, &GLOBAL_SPR[ spr::GSPR_V_H_OPEN ], 0 ); //< CYS 100705 >
		}
	}


	//히든카드 오픈 마무리 애니메이션  (히든관련 확인 후 이동 해야 될듯 ...)
	if( GAME()->GetMouseHiddenState() == 2 )
	{
		Page.DrawMultiScene( ptPos.x, ptPos.y, &g_sprLastCardOpen, 1, timeGetTime() - GAME()->GetHiddenSceneDrawTime(), FALSE );

		if( g_sprLastCardOpen.IsMSceneTimeOver( 1, timeGetTime() - GAME()->GetHiddenSceneDrawTime() ) )
		{
			GAME()->SetMouseHiddenState( 0 );
			GAME()->SetHiddenSceneDrawTime();
		}
	}
*/
	//조커 카드 받거나 변경될때 애니 그린다.
	if(g_bGetJokerCard && GAME()->m_dwStartDrawJokerTime > 0)
	{
		DWORD DrawTime = timeGetTime() - GAME()->m_dwStartDrawJokerTime;

		if(DrawTime >= GAME()->m_dwEndDrawJokerTime)
		{
			GAME()->m_dwStartDrawJokerTime = 0;
		}	
		else
		{
			if(GAME()->GetPlayerNP(0)->JoinState == 1 && GAME()->GetPlayerNP(0)->PlayState == 1)
				Page.DrawScene(0,0,&g_sprJokerCardEffect,1,DrawTime);
		}
	}		

	pDC->SelectObject(oldf);
}

/*
int CGameViewNewPoker::GetHiddenCardMoveWay( int xp, int yp )
{
	int moveway = 0;

	// 
	int nMoveGapX = xp - GAME()->GetHiddenClickPos().x;
	int nMoveGapY = yp - GAME()->GetHiddenClickPos().y;

	//일정픽셀 이상 이동했으면 히든 오픈한다.
	if( abs( GAME()->GetHiddenArrowPosX() ) > 50 || abs( GAME()->GetHiddenArrowPosY() ) > 80 )	
	{
		HiddenOpne();
		return 0;
	}

	int nxp = abs( nMoveGapX );
	int nyp = abs( nMoveGapY );

	//	4	|	1
	//------------------------
	//	3	|	2

	if( ( nMoveGapX > 0 && nMoveGapY > 0 ) || ( nMoveGapX > 0 && nMoveGapY == 0 ) ) 
	{
		if( xp > yp )
		{
			moveway = 1;
		}
		else if( xp < yp )
		{
			moveway = 2;
		}
		else
		{
			moveway = 1;
		}
	}
	else if( ( nMoveGapX > 0 && nMoveGapY < 0 ) || ( nMoveGapX == 0 && nMoveGapY < 0 ) )
	{
		if( nxp > nyp )
		{
			moveway = 1;
		}
		else if( nxp < nyp )
		{
			moveway = 4;
		}
		else
		{
			moveway = 4;
		}
	}
	else if( ( nMoveGapX < 0 && nMoveGapY > 0 )  || ( nMoveGapX == 0 && nMoveGapY > 0 )  )
	{
		if( nxp > nyp )
		{
			moveway = 3;
		}
		else if( nxp < nyp )
		{
			moveway = 2;
		}
		else
		{
			moveway = 3;
		}
	}
	else if( ( nMoveGapX < 0 && nMoveGapY < 0 )  || ( nMoveGapX == 0 && nMoveGapY < 0 ) )
	{
		m_bHiddenScene = TRUE;

		//대각선 히든 오픈 체크
		if( nxp > nyp )
		{

		}
		else if( nxp < nyp )
		{

		}
		else
		{

		}
	}


	GAME()->SetHiddenArrowPosX( GAME()->GetHiddenArrowPosX() + nMoveGapX );
	GAME()->SetHiddenArrowPosY( GAME()->GetHiddenArrowPosY() + nMoveGapY );

	POINT pt;
	pt.x = xp;
	pt.y = yp;

	GAME()->SetHiddenClickPos( pt );

	return moveway;
}

void CGameViewNewPoker::HiddenOpne()
{
	PBPlayEffectSound(SND_HIDDEN_OPEN);

	GAME()->GetPlayerNP(0)->m_bHiddenOpen = TRUE; // 카드 클래스에서 보이기 한다.
	g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(false);
	if(g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME)
	{
		GAME()->GetPlayerNP(0)->SetFaceUp(7); // 보이기
	}
	else
	{
		GAME()->GetPlayerNP(0)->SetFaceUp(6); // 보이기
	}

	g_pGameView->m_cPlayerDrawManager.SetHiddenCardShow(true);

	int nOldPoint = GAME()->m_cSvRule.get_MyRule(0);

	GAME()->SetJokerCard();
	m_cGameHelper.CalculationProbability();

	GAME()->m_bHiddenClick = FALSE;
	g_pGameView->m_cPlayerDrawManager.MyJokboNameUpdate();

	GAME()->GetPlayerNP(0)->DrawEft_CompleteMadeValue( nOldPoint );
}
*/