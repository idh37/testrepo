// GameView.cpp : implementation file
//

#include "stdafx.h"
#include "GameViewHighLow.h"
#include "SelectCardHighLow.h"
#include "GlobalGame.h"
#include "AutoBetSetUpWnd.h"
#include "./GameHelper/JokboProbabilityHighLow.h"
#include "./GameHelper/OtherUserJokboCheckManHighLow.h"
#include "PlayerDraw_HL.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGameViewHighLow

CGameViewHighLow::CGameViewHighLow(CGame *pGame)
: CGameViewBase(pGame), m_cPlayerDrawManager(pGame->GetMaxPlayer())
{
//	m_pGameViewDC = NULL;
	m_bOnceInit = FALSE;
}

CGameViewHighLow::~CGameViewHighLow()
{
}


BEGIN_MESSAGE_MAP(CGameViewHighLow, CGameViewBase)
	//{{AFX_MSG_MAP(CGameViewHighLow)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CHAR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGameViewHighLow message handlers

BOOL CGameViewHighLow::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	//cs.style |= WS_CLIPCHILDREN;
	return CGameViewBase::PreCreateWindow(cs);
}

int CGameViewHighLow::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if(CGameViewBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	////////////////////////////////////////////////////////////////////////
	// 채팅창 초기화
	m_cChatWindow.Init(&g_cUIData, g_pGameView, &g_pGameView->m_cGameHelper);
	g_pChatWinMan = &m_cChatWindow;	

	return 0;
}



void CGameViewHighLow::OnInit(void)
{
	if(m_bOnceInit)
	{
		return;
	}
	m_bOnceInit = TRUE;

	// 추가해줘야 한다.
	m_cChatWindow.InitButton(&m_cGameViewBtnMananager.m_sprUIBottomBtn);

	m_cAniSprListManager.Destroy();
	m_cStayAniSprListManager.Destroy();
	//m_pGameViewDC = this->GetDC();	

	m_cGameViewBtnMananager.Init(g_pGameView, &g_cUIData, g_pChatWinMan);

	AutoBetSetUpWnd().Init();


	// 자리 지킴이
	g_SeatKeeper.Init(&g_cUIData, g_pChatWinMan);	

	g_cOneShotCharge.Init(&g_cUIData, g_GAMEPROCESS(), &g_SeatKeeper, &g_sprCloseButton);


	// 옵져버(관저) 초기화
	g_ObserverMan.Init(g_pGameView, &g_cUIData, &g_pGameView->m_cGameViewBtnMananager);	

	// 족보 출력
	m_cGameHelper.Init(&g_cUIData, GetGame(), g_cUIData.m_nMaxJokbo, new COtherUserJokboCheckManHighLow, new CJokboProbabilityHighLow);
	
	// 사이드팟
	m_cSidePotMoney.Init(GM().GetCurrentProcess()->GetMaxPlayer(), &g_cUIData, g_pGameView);

	// 화면 중앙 베팅금액 표시
	m_cTotalBetMoney.Init(&g_cUIData, g_GAMEPROCESS(), &Page); 
	g_SelectCard().Init(&g_cUIData, GetGame(), &g_sprBigCard, &g_sprDarkCard);

	// 캐릭터창 초기화
	m_cPlayerDrawManager.Init(&g_cUIData, GetGame());
	m_cDisplayBoard.Init(&g_cUIData, GetGame());
	const int nMaxPlayer = g_GAMEPROCESS()->GetMaxPlayer();
	for(int nNum = 0; nNum < nMaxPlayer; ++nNum) 
	{	
		m_cPlayerDrawManager.AddPlayer(g_GAMEPROCESS()->GetPlayer(nNum), new CPlayerDraw_HL, nNum);
	}

	m_cTitle.Init(&g_cUIData, g_pGameView);
	m_cBackGround.Init(&g_cUIData, g_pGameView);
}


LRESULT CGameViewHighLow::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
	case WM_MOUSEWHEEL:
	case WM_MOUSEMOVE :
	case WM_LBUTTONDOWN :
	case WM_LBUTTONUP :
	case WM_LBUTTONDBLCLK :
	case WM_RBUTTONUP:
		if(GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME)
			return FALSE;
		break;
	}


	short mxp, myp;

	// IME 메세지이면 처리권을 IME클래스로 넘기고 리턴
	// if(Ime.OnMessage(message, wParam, lParam)==TRUE) return 1;

	/*if( g_pGameView && g_pGameView->m_cChatWindow.WindowProc(message,wParam,lParam) )
	{
		return 1;
	}*/

	if( m_cSidePotMoney.WindowProc(message, wParam, lParam) ) 
	{
		return TRUE;
	}

	switch(message)
	{
	case WM_MOUSEWHEEL:
		//g_pChatWinMan->m_pGameInfo->OnMouseMove()
		//	ChatBox.OnMouseWheel(wParam, lParam);	
		if(m_cChatWindow.OnMouseWheel(wParam, lParam))
			return TRUE;
		break;

	case WM_SETCURSOR :
		// 메세지를 집어 삼킨다
		//SetCursor(hCurSel);
		//break;
		return 1;

	case WM_NCMOUSEMOVE :
		//SetCursor(hCurArrow);
		break;

	
	case WM_MOUSEMOVE :
		{
			mxp = LOWORD(lParam);
			myp = HIWORD(lParam);			
			/*if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
			{
				return 1;
			}*/

			if(g_cPromotionMan.OnMouseMoveBottom(mxp, myp)) return TRUE;
			//재기충전
			if(g_cOneShotCharge.OnMouseMove(mxp, myp))
			{
				return TRUE;
			}

			// 자리 지킴이 - 위치 젤 위쪽
			if(g_SeatKeeper.OnMouseMove(mxp, myp))
			{
				return TRUE;
			}
			
			if(g_pChatWinMan->OnMouseMove(mxp,myp))
			{
				return TRUE;
			}
			
			// 버튼 위에서 마우스 포인터를 손가락 모양으로 변경
			if(m_cGameViewBtnMananager.OnMouseMove(mxp, myp))
			{
				return TRUE;
			}
			
			m_cAniSprListManager.OnMouseMove(mxp,myp);
			
			if(g_cPromotionMan.OnMouseMoveMiddle(mxp, myp))
			{
				return TRUE;
			}
			
// 			for( int i = 0 ; i < MAX_PLAYER ; ++i )
// 			{				
// 				if( strlen(g_PLAYER(i)->UI.ID) > 0 )
// 				{
// 					CRect rt;
// 					rt.SetRect( g_PLAYER(i)->m_ptAvatarStart.x, g_PLAYER(i)->m_ptAvatarStart.y, 
// 								g_PLAYER(i)->m_ptAvatarStart.x + 75, g_PLAYER(i)->m_ptAvatarStart.y + 125 );
// 					POINT pt;
// 					pt.x = mxp ; 
// 					pt.y = myp ; 
// 					if( rt.PtInRect(pt) )
// 					{
// 						SetCursor( GM().GetCurHandCursor() );
// 						break;
// 					}
// 					else
// 					{
// 						SetCursor( GM().GetCurSelCursor() );
// 					}
// 				}
// 			}			

			//게임 방 공지 마우스 오버 
#ifdef _DEBUG
			NoticAreaMouseOver(mxp,myp);
#endif
			AutoBetSetUpWnd().OnMouseMove(mxp, myp);
			if(g_GAMEPROCESS()->OnMouseMove(mxp, myp)==TRUE) 
			{
				return 1;	
			}

			//관전UI
			g_ObserverMan.OnMouseMove(mxp, myp);
			g_cVipJackpot.OnMouseMove(mxp, myp);

			if(m_cPlayerDrawManager.OnMouseMove(mxp, myp))
			{
				return TRUE;
			}
			m_cChatWindow.OnMouseMove(mxp,myp);
			if(g_cOwnLimit.OnMouseMove(mxp, myp)) 
			{
				return TRUE;
			}

			if(g_cPromotionMan.OnMouseMoveTop(mxp, myp)) return TRUE;
		}
		return 1;

	case WM_LBUTTONDOWN :
		{	
			mxp = LOWORD(lParam);
			myp = HIWORD(lParam);

			if(TRUE==g_cVipJackpot.OnLButtonDown(mxp, myp)){
				return 1;
			}

			/*if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
				return 1; */

			CRect rc;		
			rc.SetRect( 0, 0, 712, 25 );
			POINT pt;
			pt.x = mxp;
			pt.y = myp;
			if( rc.PtInRect( pt ) )
			{
				GM().GetMainWnd()->SendMessage( WM_LBUTTONDOWN, MK_LBUTTON, lParam );
				return 1;
			}			

			//자동 베팅 설정 버튼 
			POINT bpt;
			bpt.x = mxp;
			bpt.y = myp;		

			this->SetFocus();	
			SetCapture();

			if(g_cPromotionMan.OnLButtonDownBottom(mxp, myp)) return TRUE;

			//자리 지킴이 - 위치 젤 위쪽
			if(g_SeatKeeper.OnLButtonDown(mxp, myp))
				return TRUE;

			//재기충전
			if(g_cOneShotCharge.OnLButtonDown(mxp, myp))
				return TRUE;

			
			// 캐릭터 보기는 항상 클릭 가능
			g_pChatWinMan->OnLButtonDown(mxp, myp);			

			m_cAniSprListManager.OnLButtonDown(mxp,myp);

			AutoBetSetUpWnd().OnLButtonDown(mxp, myp);
			if(g_GAMEPROCESS()->OnLButtonDown(mxp, myp)==TRUE)
			{
				return 1;
			}

			m_cGameViewBtnMananager.OnLButtonDown(mxp, myp);	
	
			if(g_cPromotionMan.OnLButtonDownMiddle(mxp, myp)) return TRUE;
			
			//관전UI
			g_ObserverMan.OnLButtonDown(mxp, myp);

			if(m_cPlayerDrawManager.OnLButtonDown(mxp, myp)) 
			{
				return TRUE;
			}

			if(g_cPromotionMan.OnLButtonDownMiddle(mxp, myp)) return TRUE;
		}
		return 1;

	case WM_LBUTTONUP :
		{
			mxp = LOWORD(lParam);
			myp = HIWORD(lParam);
			/*if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
				return 1;*/
			
			if(g_cPromotionMan.OnLButtonUpBottom(mxp, myp)) return TRUE;

			ReleaseCapture();

			//자리 지킴이 - 위치 젤 위쪽
			if(g_SeatKeeper.OnLButtonUp(mxp, myp))
				return TRUE;

			//재기충전
			if(g_cOneShotCharge.OnLButtonUp(mxp, myp))
				return TRUE;

			g_pChatWinMan->OnLButtonUp(mxp, myp);

			m_cGameViewBtnMananager.OnLButtonUp(mxp, myp);
			
			if(g_cPromotionMan.OnLButtonUpMiddle(mxp, myp)) return TRUE;

			//////////////////////
			m_cAniSprListManager.OnLButtonUp(mxp,myp);

			AutoBetSetUpWnd().OnLButtonUp(mxp, myp);

			g_GAMEPROCESS()->OnLButtonUp(mxp, myp);

			//관전UI
			g_ObserverMan.OnLButtonUp(mxp, myp);
			g_cVipJackpot.OnLButtonUp(mxp, myp);

			if(m_cPlayerDrawManager.OnLButtonUp(mxp, myp)) 
			{
				return TRUE;
			}
			
			if(g_cPromotionMan.OnLButtonUpTop(mxp, myp)) return TRUE;
		}		
		return 1;

	case WM_LBUTTONDBLCLK :
		{
			mxp = LOWORD(lParam);
			myp = HIWORD(lParam);
			/*if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
			{
				return 1;			
			}*/
		}
		return 1;

	default:
		break;

	}

	return CGameViewBase::WindowProc(message, wParam, lParam);
}


void CGameViewHighLow::OnPaint() 
{
	// 현재 게임이 아니면 그리지 않는다.
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
	{
		return;
	}

	CFont *pTempFont = NULL;

	CPaintDC dc(this);

	// 사운드 리소스 관리 및 플레이 리스트 연주
//	NMBASE::SOUND::g_Sound.RegularCall();

	// 더블 버퍼링용 메모리 DC 생성
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);

	// 메모리DC에 버퍼용 비트맵 핸들을 선택
	MemDC.SelectObject(Page.hBmp);
	MemDC.SetBkMode(TRANSPARENT);

	// 각 게임마다 배경 이미지에 로고 다르게 표시 씬으로 표시	
	
	m_cBackGround.DrawBottom(&Page, &MemDC);

	m_cTitle.Draw(&Page, &MemDC);
	
	m_cAniSprListManager.DrawBottom(&MemDC);
	m_cStayAniSprListManager.DrawBottom(&MemDC);

	m_cPlayerDrawManager.DrawBottom(&MemDC);
	g_cPromotionMan.DrawBottom( &MemDC, &Page );

	// 잭팟 그리기
	DrawJackPot();
	g_cVipJackpot.DrawBottom(&Page, &MemDC);

	g_GAMEPROCESS()->m_AllBetChip.DrawBottom();	
	

	// 플레이어
	DrawPlayBottom(&MemDC);

	m_cAniSprListManager.DrawMiddle(&MemDC);	
	m_cStayAniSprListManager.DrawMiddle(&MemDC);

	//배팅 정보 룸정보	
	DrawButton( &MemDC );  // 버튼

	m_cTotalBetMoney.Draw(&MemDC);

	// 전광판 그리기
	m_cDisplayBoard.Draw(&MemDC);

	// 카드	
	DrawPlayCard(&MemDC); 

	m_cBackGround.DrawMiddle(&Page, &MemDC);

	// 플레이어
	DrawPlayMiddle(&MemDC);
	g_cVipJackpot.DrawMiddle(&Page, &MemDC);

	m_cPlayerDrawManager.DrawMiddle(&MemDC);	
	g_cPromotionMan.DrawMiddle(&MemDC, &Page);
	g_pChatWinMan->Draw(&MemDC);

	g_cOwnLimit.DrawBottom();

	// 사이드팟
	m_cSidePotMoney.DrawBottom(&MemDC);

	m_cAniSprListManager.DrawTop(&MemDC);	
	m_cStayAniSprListManager.DrawTop(&MemDC);	

	COLORREF color = MemDC.SetTextColor( RGB(187, 171, 145) );
	pTempFont = MemDC.SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_NORMAL));
	
	//관전UI
	g_ObserverMan.Draw(&Page, &MemDC);

#ifdef _DEBUG

	// 프레임 레이트 체크
	static float FPS = 0;
	static int framecnt = 0;
	static DWORD lasttick = timeGetTime();
	DWORD nowtick = timeGetTime();
	DWORD deltatick = nowtick - lasttick;
	framecnt++;
	if(deltatick >= 1000) 
	{
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

	MemDC.SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_NORMAL));
	MemDC.SetBkMode(TRANSPARENT);
	MemDC.SetTextColor(RGB(255,255,255));
	MemDC.TextOut(100,3, str);	
#endif


	// 플레이어
	DrawPlayTop(&MemDC);

	//자리 지킴이
	g_SeatKeeper.Draw(&Page, &MemDC);       // 제일 위쪽 레이어에 그린다.


	// 사이드팟
	m_cSidePotMoney.DrawTop(&MemDC);

	AutoBetSetUpWnd().Draw_Top(&MemDC);	

	m_cPlayerDrawManager.DrawTop(&MemDC);
	
	g_GAMEPROCESS()->m_AllBetChip.DrawTop();

	g_cOwnLimit.DrawTop(&MemDC);

	AutoBetSetUpWnd().Draw(&MemDC);

	//재기충전
	g_cOneShotCharge.Draw(&MemDC);		

	g_cPromotionMan.DrawTop(&MemDC, &Page);

	m_cPlayerDrawManager.DrawTopMost(&MemDC);

	g_cVipJackpot.DrawTop(&Page, &MemDC);

	//보더그리기
	Page.DrawEmptyBox(0,0,GetWidth()-1, GetHeight()-1, NMBASE::GRAPHICGDI::xSprite::RGB555(100,100,100));

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &MemDC, 0, 0, SRCCOPY);

	MemDC.DeleteDC();
}


// 잭팟 - 자신이 입장한 방의 Money구간에 해당하는 잭팟 금액만 출력을 한다.
void CGameViewHighLow::DrawJackPot()
{		
#if defined(_JACKPOT_NON_CHANNEL_MONEY)
	INT64 &n64JackPotMoney	  = GDM().GetINT64(GDK_JACKPOT_MONEY);
	INT64 &n64TmpJackPotMoney = GDM().GetINT64(GDK_JACKPOT_MONEY_TEMP);
#else
	INT64 &n64JackPotMoney	  = g_GAMEPROCESS()->GetOldJackPotMoney();
	INT64 &n64TmpJackPotMoney = g_GAMEPROCESS()->GetJackPotMoney();
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

		n64JackPotMoney+=plus; // 증가 보여주기
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

		n64JackPotMoney-=plus; // 증가 보여주기
	}

	INT64 tmp = n64JackPotMoney;
	int kk    = 0;			
	int addx  = 0;
	int count = 0;	

	int bnum = (g_GAMEPROCESS()->m_nGameCount/30) % 2;	

	for( int j = 0; j < 20 ; j ++ ) 
	{
		if( tmp <= 0 ) 
		{				
			// 잭팟을 받은경우 초기화 0으로 한다.
			if(tmp == 0 && 0 == n64JackPotMoney && 0 == n64TmpJackPotMoney)
			{
				Page.PutSprAuto( g_cUIData.m_ptJacpotNumberStart.x + addx - (j + 1) * g_cUIData.m_nJacpotNumberWidth, g_cUIData.m_ptJacpotNumberStart.y, &g_sprJackpotNumber, 0 );
			}
			break;
		}

		if(kk >= 3)	
		{
			kk = 0;
			Page.PutSprAuto( g_cUIData.m_ptJacpotNumberStart.x + addx - (j + 1) * g_cUIData.m_nJacpotNumberWidth, g_cUIData.m_ptJacpotNumberStart.y, &g_sprJackpotNumber, 10 );
			addx -= 6;
			count++;
		}
		Page.PutSprAuto( g_cUIData.m_ptJacpotNumberStart.x + addx - (j + 1) * g_cUIData.m_nJacpotNumberWidth, g_cUIData.m_ptJacpotNumberStart.y, &g_sprJackpotNumber,(int)(tmp%(INT64)10) );
		tmp = tmp / 10;
		kk++;
	}
}

void CGameViewHighLow::Reset()
{
	m_bExitSubscript = FALSE;	
}

void CGameViewHighLow::OnTimer(UINT nIDEvent) 
{
	if(nIDEvent == GAMEVIEW_TIMER)
	{
		if(GetCurrentWhere() == IDX_GAMEWHERE_GAME)
		{			
			m_cAniSprListManager.AniSprProcess();
			m_cStayAniSprListManager.AniSprProcess();

			g_pChatWinMan->OnTimer();
		}		
	}

	CGameViewBase::OnTimer(nIDEvent);
}

void CGameViewHighLow::OnDestroy() 
{
	CGameViewBase::OnDestroy();

	// ### [ Frame++ ] ###
	if(m_hEventTimer) 
	{
		timeKillEvent(m_hEventTimer);
	}

	m_hEventTimer = 0;
		
	m_cChatWindow.OnDestroy();
}


BOOL CGameViewHighLow::OnCommand(WPARAM wParam, LPARAM lParam) 
{	
	if( g_cPromotionMan.OnCommand(wParam, lParam)) return TRUE;
	if( GetCurrentWhere() == IDX_GAMEWHERE_GAME )
	{
		if(m_cGameHelper.OnCommand(wParam, lParam)) return TRUE;
		if(g_SeatKeeper.OnCommand(wParam, lParam)) return TRUE;
		if(g_GAMEPROCESS()->OnCommand(wParam, lParam)) return TRUE;
		if(g_cVipJackpot.OnCommand(wParam, lParam)) return TRUE;
	}	

	return CGameViewBase::OnCommand(wParam, lParam);
}

void CGameViewHighLow::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CGameViewBase::OnChar(nChar, nRepCnt, nFlags);
}

/////////////////////////////////////////////////////////////////////////////////

void CGameViewHighLow::DrawPlayCard(CDC *pDC)
{	
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
	{
		return;
	}

	g_PLAYER(2)->OnCardDraw(&Page);
	g_PLAYER(3)->OnCardDraw(&Page);
	g_PLAYER(4)->OnCardDraw(&Page);
	g_PLAYER(1)->OnCardDraw(&Page);
	g_PLAYER(0)->OnCardDraw(&Page);
}

void CGameViewHighLow::DrawPlayBottom(CDC *pDC)
{
	g_PLAYER(2)->OnDraw_Bottom(pDC);
	g_PLAYER(3)->OnDraw_Bottom(pDC);
	g_PLAYER(4)->OnDraw_Bottom(pDC);
	g_PLAYER(1)->OnDraw_Bottom(pDC);
	g_PLAYER(0)->OnDraw_Bottom(pDC);	
}
void CGameViewHighLow::DrawPlayMiddle(CDC *pDC)
{
	g_PLAYER(2)->OnDraw_Middle(pDC);
	g_PLAYER(3)->OnDraw_Middle(pDC);
	g_PLAYER(4)->OnDraw_Middle(pDC);
	g_PLAYER(1)->OnDraw_Middle(pDC);
	g_PLAYER(0)->OnDraw_Middle(pDC);	
}
void CGameViewHighLow::DrawPlayTop(CDC *pDC)
{
	g_PLAYER(2)->OnDraw_Top(pDC);
	g_PLAYER(3)->OnDraw_Top(pDC);
	g_PLAYER(4)->OnDraw_Top(pDC);
	g_PLAYER(1)->OnDraw_Top(pDC);
	g_PLAYER(0)->OnDraw_Top(pDC);	
}


void CGameViewHighLow::DrawButton(CDC *pDC)
{
	m_cGameViewBtnMananager.DrawBtn(pDC);
}


//2006.1.9 스피드핵
DWORD CGameViewHighLow::GetCurrentPlayTick()
{
	return timeGetTime()-g_nStartTick;
}


BOOL CGameViewHighLow::NoticAreaMouseOver(int x, int y )
{	
	return FALSE;
}


void CGameViewHighLow::SetupByConfig()
{

}


// 찬스아이템 이펙트 출력 (마지막 까지 베팅을 해야 보여준다.) 
void CGameViewHighLow::ShowEffect_AllowChancePurchase(int nItemCode)
{
	CChance_Msg_EffectSpr *pObj = new CChance_Msg_EffectSpr;
	if( pObj != NULL && g_pGameView != NULL)
	{
		// 어떻게 이겼는가?(모두 죽어서 홀로 이겼는가 FALSE, 끝까지 베팅했는가 TRUE) - 기획팀에서 기권등에도 나오도록 해달라한다.
		//if(g_pGameView->m_cPlayerDrawManager.IsOpen())
		{
			pObj->Init(nItemCode);
			g_PLAYER(0)->m_cPlayerEffectList.AddObject(pObj);
		}		
	}
}


// 보유한도 달성 이펙트
void CGameViewHighLow::ShowEffect_OwnLimitEffect(int nPnum)
{
	//기존 COwnLimit에 ShowAniEffect(int p) 함수 내용을 가져온다.
	if(nPnum < 0 || nPnum >= MAX_PLAYER) 
	{
		return;
	}

	// PokerBase쪽으로 이동
}

//UPG 작업 추가
void CGameViewHighLow::ShowEffect_OwnLimitIcon(BOOL bON, INT64 llMoney)
{
}


void CGameViewHighLow::OnExitRoom(void)
{
}

const int CGameViewHighLow::GetWidth()
{
	return 1000;
}

const int CGameViewHighLow::GetHeight()
{
	return 740;
}

BOOL CGameViewHighLow::OnEraseBkgnd(CDC* pDC)
{	
	OnPaint();

	return TRUE;
}


BOOL CGameViewHighLow::PreTranslateMessage(MSG* pMsg)
{
	if(g_GAMEPROCESS()->PreTranslateMessage(pMsg))
	{
		return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}


void CGameViewHighLow::GameViewSetFocus()
{
	if(this)
	{
		this->SetFocus(); 
	}
}