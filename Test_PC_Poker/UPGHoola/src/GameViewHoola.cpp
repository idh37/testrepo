// GameView.cpp : implementation file
//

#include "stdafx.h"
#include "GameViewHoola.h"
#include "GlobalGame.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGameViewHoola

CGameViewHoola::CGameViewHoola(CGame *pGame)
: CGameViewBase(pGame), m_cPlayerDrawManager(pGame->GetMaxPlayer())
{
//	m_pGameViewDC = NULL;
	m_bOnceInit = FALSE;

	m_bPlayIntro = FALSE;	
	m_bIntroFinish = FALSE;	

}

CGameViewHoola::~CGameViewHoola()
{
}


BEGIN_MESSAGE_MAP(CGameViewHoola, CGameViewBase)
	//{{AFX_MSG_MAP(CGameViewHoola)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CHAR()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGameViewHoola message handlers

BOOL CGameViewHoola::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	//cs.style |= WS_CLIPCHILDREN;
	return CGameViewBase::PreCreateWindow(cs);
}

int CGameViewHoola::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if(CGameViewBase::OnCreate(lpCreateStruct) == -1)
		return -1;

	////////////////////////////////////////////////////////////////////////
	// 채팅창 초기화
	m_cChatWindow.Init(&g_cUIData, g_pGameView, NULL);
	g_pChatWinMan = &m_cChatWindow;	

	return 0;
}



void CGameViewHoola::OnInit(void)
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

	// 자리 지킴이
	g_SeatKeeper.Init(&g_cUIData, g_pChatWinMan);	

	g_cOneShotCharge.Init(&g_cUIData, g_GAMEPROCESS(), &g_SeatKeeper, &g_sprCloseButton);


	// 옵져버(관저) 초기화
	g_ObserverMan.Init(g_pGameView, &g_cUIData, &g_pGameView->m_cGameViewBtnMananager);	

	// 캐릭터창 초기화
	m_cPlayerDrawManager.Init(&g_cUIData, GetGame());
	m_cDisplayBoard.Init(&g_cUIData, GetGame());
	const int nMaxPlayer = g_GAMEPROCESS()->GetMaxPlayer();
	for(int nNum = 0; nNum < nMaxPlayer; ++nNum) 
	{	
		m_cPlayerDrawManager.AddPlayer(g_GAMEPROCESS()->GetPlayer(nNum), new CPlayerDraw_HA, nNum);
	}

	m_cTitle.Init(&g_cUIData, g_pGameView);
	m_cBackGround.Init(&g_cUIData, g_pGameView);

	// 이펙트관리자 초기화
	m_cEffectDrawMan.Init(&Page);

	// 게임 결과창 초기화
	m_cGameResultViewMan.Init(&Page, this);
}


LRESULT CGameViewHoola::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
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
			
			if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
			{
				return 1;
			}
			
			if(g_cPromotionMan.OnMouseMoveBottom(mxp, myp))
			{
				return TRUE;
			}
			
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
			
			if(m_cGameResultViewMan.OnMouseMove(mxp, myp))
			{
				return TRUE;
			}
			
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
			if(g_GAMEPROCESS()->OnMouseMove(mxp, myp)==TRUE) 
			{
				return 1;	
			}

			//관전UI
			g_ObserverMan.OnMouseMove(mxp, myp);

			if(m_cPlayerDrawManager.OnMouseMove(mxp, myp))
			{
				return TRUE;
			}
			m_cChatWindow.OnMouseMove(mxp,myp);
			if(g_cOwnLimit.OnMouseMove(mxp, myp)) 
			{
				return TRUE;
			}

			if(g_cPromotionMan.OnMouseMoveTop(mxp, myp))
			{
				return TRUE;
			}
		}
		return 1;

	case WM_LBUTTONDOWN :
		{	
			mxp = LOWORD(lParam);
			myp = HIWORD(lParam);

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

			if ( FALSE == g_GAMEPROCESS()->GetPlayScr()->m_bPracticsGame )
			{
				//자리 지킴이 - 위치 젤 위쪽
				if(g_SeatKeeper.OnLButtonDown(mxp, myp))
					return TRUE;
			}
			

			//재기충전
			if(g_cOneShotCharge.OnLButtonDown(mxp, myp))
				return TRUE;

			
			// 캐릭터 보기는 항상 클릭 가능
			g_pChatWinMan->OnLButtonDown(mxp, myp);			

			m_cAniSprListManager.OnLButtonDown(mxp,myp);

			if(g_GAMEPROCESS()->OnLButtonDown(mxp, myp)==TRUE)
			{
				return 1;
			}
			
			if ( m_cGameResultViewMan.OnLButtonDown(mxp, myp) )
			{
				return TRUE;
			}
			

			//게임 버튼들
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

			if ( FALSE == g_GAMEPROCESS()->GetPlayScr()->m_bPracticsGame )
			{
				//자리 지킴이 - 위치 젤 위쪽
				if(g_SeatKeeper.OnLButtonUp(mxp, myp))
					return TRUE;
			}

			//재기충전
			if(g_cOneShotCharge.OnLButtonUp(mxp, myp))
				return TRUE;

			g_pChatWinMan->OnLButtonUp(mxp, myp);

			if( m_cGameResultViewMan.OnLButtonUp(mxp, myp) )
			{
				return TRUE;
			}

			m_cGameViewBtnMananager.OnLButtonUp(mxp, myp);
			
			if(g_cPromotionMan.OnLButtonUpMiddle(mxp, myp)) return TRUE;

			//////////////////////
			m_cAniSprListManager.OnLButtonUp(mxp,myp);

			g_GAMEPROCESS()->OnLButtonUp(mxp, myp);

			//관전UI
			g_ObserverMan.OnLButtonUp(mxp, myp);

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

			g_GAMEPROCESS()->OnLButtonDblclk(mxp, myp);
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


void CGameViewHoola::OnPaint() 
{
	// 현재 게임이 아니면 그리지 않는다.
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
	{
		return;
	}

	CFont *pTempFont = NULL;
	CPaintDC dc(this);
	// 더블 버퍼링용 메모리 DC 생성
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);
 	
	// 메모리DC에 버퍼용 비트맵 핸들을 선택
	MemDC.SelectObject(Page.hBmp);
	MemDC.SetBkMode(TRANSPARENT);

	//각 게임마다 배경 이미지에 로고 다르게 표시 씬으로 표시	
	m_cBackGround.DrawBottom(&Page, &MemDC);

	m_cTitle.Draw(&Page, &MemDC);	
	m_cAniSprListManager.DrawBottom(&MemDC);
	m_cStayAniSprListManager.DrawBottom(&MemDC);
	m_cPlayerDrawManager.DrawBottom(&MemDC);
	g_cPromotionMan.DrawBottom( &MemDC, &Page );

	// 플레이어
	DrawPlayBottom(&MemDC);

	m_cAniSprListManager.DrawMiddle(&MemDC);	
	m_cStayAniSprListManager.DrawMiddle(&MemDC);

	// 전광판 그리기
	m_cDisplayBoard.Draw(&MemDC);

	// 플레이어
	DrawPlayMiddle(&MemDC);

	m_cPlayerDrawManager.DrawMiddle(&MemDC);	
	g_cPromotionMan.DrawMiddle(&MemDC, &Page);
	
	g_pChatWinMan->Draw(&MemDC);

	/* CPlayScr 그리기 문제 */
	g_GAMEPROCESS()->GetPlayScr()->DrawGold();					//골드 그리기
	g_GAMEPROCESS()->GetPlayScr()->Draw(&MemDC);
	g_GAMEPROCESS()->GetPlayScr()->DrawMissionComp(&MemDC);		//미션 달성 애니메이션
	g_GAMEPROCESS()->GetPlayScr()->DrawJackpotAni(&MemDC);		//잭팟 애니메이션	
	g_GAMEPROCESS()->GetPlayScr()->DrawHoola();					//훌라 이펙트
	DrawIntroAnimation();										//훌라 인트로
	DrawJackPot();												//훌라 잭팟 그리기
	//훌라 코드 

	//배팅 정보 룸정보	
	DrawButton( &MemDC );  // 버튼								//게임 컨트롤들. 훌라 카드 앞에 그린다

	g_GAMEPROCESS()->GetPlayScr()->DrawNoThankBtnSpr();			//땡큐버튼 앞에 노땡큐 버튼을 그린다

	g_cOwnLimit.DrawBottom();

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
	MemDC.TextOut(670,140, str);	
#endif

	// 플레이어
	DrawPlayTop(&MemDC);

	//자리 지킴이
	g_SeatKeeper.Draw(&Page, &MemDC);       // 제일 위쪽 레이어에 그린다.

	m_cPlayerDrawManager.DrawTop(&MemDC);

	g_cOwnLimit.DrawTop(&MemDC);

	//재기충전
	g_cOneShotCharge.Draw(&MemDC);		

	g_cPromotionMan.DrawTop(&MemDC, &Page);

	m_cPlayerDrawManager.DrawTopMost(&MemDC);

	//보더그리기
	Page.DrawEmptyBox(0,0,GetWidth()-1, GetHeight()-1, NMBASE::GRAPHICGDI::xSprite::RGB555(100,100,100));

	m_cEffectDrawMan.Draw(&MemDC);
	m_cGameResultViewMan.Draw(&MemDC);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &MemDC, 0, 0, SRCCOPY);

	MemDC.DeleteDC();
}


// 잭팟 - 자신이 입장한 방의 Money구간에 해당하는 잭팟 금액만 출력을 한다. //훌라용 코드로 변경 작업
void CGameViewHoola::DrawJackPot()
{	
	// 좌측 상단 잭팟 금액 그리기
	INT64 &n64JackPotMoney	  = g_GAMEPROCESS()->GetOldJackPotMoney();
	INT64 &n64TmpJackPotMoney = g_GAMEPROCESS()->GetJackPotMoney();

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

	// 우측 하단 스톱잭팟 정보 그리기
	const int nJackpotStepIndex = 5;

	int nJackpotInfoIndex = 0; //인덱스는 해당 멀티씬의 스프라이트 인덱스값
	int nJackpotBackIndex = 0;
	if ( g_RI.cRuleType != 2 ) //2번 인덱스는 스톱옵션 적용방. 이넘값 있는지 확인하고 교체할것
	{
		nJackpotInfoIndex = 12;
		nJackpotBackIndex = 3;
	}
	if ( g_RI.llEnterLimitMoney <= 0 || g_RI.llEnterLimitMoney >= MONEY_JO ) //자유 경기장일경우
	{
		nJackpotInfoIndex = 13;
		nJackpotBackIndex = 2;
	}
	if ( ( g_RI.llEnterLimitMoney > 0 && g_RI.llEnterLimitMoney < MONEY_JO ) && ( g_RI.cRuleType == 2 ) ) //스톱 옵션 적용 방 일경우
	{
		nJackpotInfoIndex = 11;
		nJackpotBackIndex = 1;
	}


	// 배경 그리기
	Page.DrawMultiScene( 0, 0, &g_sprJackPotInfo, nJackpotBackIndex, timeGetTime() );
	

	DWORD dwTime = 0;
	if ( g_GAMEPROCESS()->GetPlayScr()->m_bJackpotConditionSucceed )
	{
		for ( int i = 0; i < g_GAMEPROCESS()->GetPlayScr()->m_nJackpotConditionStep; i++ )
		{
			dwTime = g_sprJackPotInfo.GetMScenePlayTime( i + nJackpotStepIndex ) - 1;
			Page.DrawMultiScene( 0, 0, &g_sprJackPotInfo, i + nJackpotStepIndex, dwTime );
		}
		dwTime = timeGetTime() - g_GAMEPROCESS()->GetPlayScr()->m_dwJackpotConditionTime;
		if ( dwTime <= g_sprJackPotInfo.GetMScenePlayTime( g_GAMEPROCESS()->GetPlayScr()->m_nJackpotConditionStep + nJackpotStepIndex - 1 ) )
		{
			Page.DrawMultiScene( 0, 0, &g_sprJackPotInfo, g_GAMEPROCESS()->GetPlayScr()->m_nJackpotConditionStep + nJackpotStepIndex - 1, dwTime );
		}
		else
		{
			g_GAMEPROCESS()->GetPlayScr()->m_bJackpotConditionSucceed = FALSE;

			if ( g_GAMEPROCESS()->GetPlayScr()->m_nJackpotConditionStep == 3 )
			{
				g_GAMEPROCESS()->GetPlayScr()->m_nJackpotConditionStep = 0;
			}
		}
	}
	else
	{
		for ( int i = 0; i < g_GAMEPROCESS()->GetPlayScr()->m_nJackpotConditionStep; i++ )
		{
			dwTime = g_sprJackPotInfo.GetMScenePlayTime( i + nJackpotStepIndex ) - 100;
			Page.DrawMultiScene( 0, 0, &g_sprJackPotInfo, i + nJackpotStepIndex, dwTime );
		}

		if ( g_GAMEPROCESS()->GetPlayScr()->m_nJackpotConditionStep > 0 )
		{
			Page.DrawMultiScene( 0, 0, &g_sprJackPotInfo, 9, timeGetTime() );
		}
	}


	if ( g_GAMEPROCESS()->GetPlayScr()->m_bJackpotMouseOver )
	{
		dwTime = timeGetTime() - g_GAMEPROCESS()->GetPlayScr()->m_dwJackpotMouseTime;
		if ( dwTime >= 2000 )
			dwTime = 2000;

		Page.DrawMultiScene( 0, 0, &g_sprJackPotInfo, nJackpotInfoIndex, dwTime  );
	}
	if ( g_GAMEPROCESS()->GetPlayScr()->m_bJackpotMouseOverOff )
	{
		dwTime = timeGetTime() - g_GAMEPROCESS()->GetPlayScr()->m_dwJackpotMouseTime + 2000;
		if ( dwTime <= g_sprJackPotInfo.GetMScenePlayTime( 1 )  )
		{
			Page.DrawMultiScene( 0, 0, &g_sprJackPotInfo, nJackpotInfoIndex, dwTime  );
		}
		else
		{
			g_GAMEPROCESS()->GetPlayScr()->m_bJackpotMouseOverOff = FALSE;
			g_GAMEPROCESS()->GetPlayScr()->m_dwJackpotMouseTime = 0;
		}
	}
}

void CGameViewHoola::Reset()
{
	m_bExitSubscript = FALSE;	
}

void CGameViewHoola::OnTimer(UINT nIDEvent) 
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

void CGameViewHoola::OnDestroy() 
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


BOOL CGameViewHoola::OnCommand(WPARAM wParam, LPARAM lParam) 
{	
	if( g_cPromotionMan.OnCommand(wParam, lParam)) return TRUE;
	if( GetCurrentWhere() == IDX_GAMEWHERE_GAME )
	{
//		if(m_cGameHelper.OnCommand(wParam, lParam)) return TRUE;
		if(g_SeatKeeper.OnCommand(wParam, lParam)) return TRUE;
		if(g_GAMEPROCESS()->OnCommand(wParam, lParam)) return TRUE;
	}	

	return CGameViewBase::OnCommand(wParam, lParam);
}

void CGameViewHoola::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	CGameViewBase::OnChar(nChar, nRepCnt, nFlags);
}

/////////////////////////////////////////////////////////////////////////////////

void CGameViewHoola::DrawPlayCard(CDC *pDC)
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

void CGameViewHoola::DrawPlayBottom(CDC *pDC)
{
	g_PLAYER(2)->OnDraw_Bottom(pDC);
	g_PLAYER(3)->OnDraw_Bottom(pDC);
	g_PLAYER(4)->OnDraw_Bottom(pDC);
	g_PLAYER(1)->OnDraw_Bottom(pDC);
	g_PLAYER(0)->OnDraw_Bottom(pDC);	
}
void CGameViewHoola::DrawPlayMiddle(CDC *pDC)
{
	g_PLAYER(2)->OnDraw_Middle(pDC);
	g_PLAYER(3)->OnDraw_Middle(pDC);
	g_PLAYER(4)->OnDraw_Middle(pDC);
	g_PLAYER(1)->OnDraw_Middle(pDC);
	g_PLAYER(0)->OnDraw_Middle(pDC);	
}
void CGameViewHoola::DrawPlayTop(CDC *pDC)
{
	g_PLAYER(2)->OnDraw_Top(pDC);
	g_PLAYER(3)->OnDraw_Top(pDC);
	g_PLAYER(4)->OnDraw_Top(pDC);
	g_PLAYER(1)->OnDraw_Top(pDC);
	g_PLAYER(0)->OnDraw_Top(pDC);	
}


void CGameViewHoola::DrawButton(CDC *pDC)
{
	m_cGameViewBtnMananager.DrawBtn(pDC);
}


//2006.1.9 스피드핵
DWORD CGameViewHoola::GetCurrentPlayTick()
{
	return timeGetTime()-g_nStartTick;
}


BOOL CGameViewHoola::NoticAreaMouseOver(int x, int y )
{	
	return FALSE;
}


void CGameViewHoola::SetupByConfig()
{
	if ( strcmp(g_PLAYER(0)->UI.ID, GM().GetMyInfo()->UI.ID) == 0 )
	{
		//0번 인덱스의 사용자와 내 정보가 일치할 경우
		g_PLAYER(0)->nSndFxKind = g_Config.SndFxKind;
	}

	
}


// 찬스아이템 이펙트 출력 (마지막 까지 베팅을 해야 보여준다.) 
void CGameViewHoola::ShowEffect_AllowChancePurchase(int nItemCode)
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
void CGameViewHoola::ShowEffect_OwnLimitEffect(int nPnum)
{
	//기존 COwnLimit에 ShowAniEffect(int p) 함수 내용을 가져온다.
	if(nPnum < 0 || nPnum >= MAX_PLAYER) 
	{
		return;
	}

	// PokerBase쪽으로 이동
}

//UPG 작업 추가
void CGameViewHoola::ShowEffect_OwnLimitIcon(BOOL bON, INT64 llMoney)
{

}


void CGameViewHoola::OnExitRoom(void)
{
}

const int CGameViewHoola::GetWidth()
{
	return 1000;
}

const int CGameViewHoola::GetHeight()
{
	return 740;
}

BOOL CGameViewHoola::OnEraseBkgnd(CDC* pDC)
{	
	OnPaint();

	return TRUE;
}


BOOL CGameViewHoola::PreTranslateMessage(MSG* pMsg)
{
	if(g_GAMEPROCESS()->PreTranslateMessage(pMsg))
	{
		return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}


void CGameViewHoola::GameViewSetFocus()
{
	if(this)
	{
		this->SetFocus(); 
	}
}

void CGameViewHoola::PlayIntroAnimation()
{
	m_bPlayIntro = TRUE;
	m_dwIntroTime = timeGetTime();

	AutoPlayMSceneSound( &g_sprIntro, 0 );
}

void CGameViewHoola::DrawIntroAnimation()
{
	if ( m_bPlayIntro )
	{
		DWORD dwTime = timeGetTime() - m_dwIntroTime;
		if ( dwTime <= g_sprIntro.GetMScenePlayTime(0) )
		{
			Page.DrawMultiScene( 0, 0, &g_sprIntro, 0, dwTime );
		}
		else
		{
			m_bIntroFinish = TRUE;
			m_bPlayIntro = FALSE;
		}
	}
	if ( (!m_bPlayIntro && m_bIntroFinish) || (!m_bPlayIntro && !m_bIntroFinish) )
	{
		//프로모션 버튼과 위치가 겹쳐서 일단 숨김. 7월프로모션 이후 롤백.
		//2012.7.10 류승민
		Page.DrawMultiScene( 0, 0, &g_sprIntro, 1, timeGetTime() );
	}
}