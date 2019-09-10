// GameView.cpp : implementation file
//

#include "stdafx.h"
#include "GameView.h"

#include "AutoBetSetUpWnd.h"
#include "./Module/GameHelper/OtherUserJokboCheckManLB.h"
#include "./Module/GameHelper/JokboProbabilityLB.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CGameViewLowBD

//g_Value = 0;

CGameViewLowBD::CGameViewLowBD( CGame *pGame )
: CGameViewBase( pGame ), m_cPlayerDrawManager(pGame->GetMaxPlayer())
{
	g_pGameView = this;

	m_bFirstLoad = true;

	LAYER_GLOBAL_AUTOBETWND(); 
}

CGameViewLowBD::~CGameViewLowBD()
{
	m_cAniSprListManager.Destroy();
	m_cStayAniSprListManager.Destroy();
	LAYER_GLOBAL_AUTOBETWND_DELETE(); // �ڵ����ð����� ����
}


BEGIN_MESSAGE_MAP(CGameViewLowBD, CGameViewBase)		//UPGCommon �۾�
	//{{AFX_MSG_MAP(CGameViewLowBD)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CGameViewLowBD message handlers

BOOL CGameViewLowBD::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	//cs.style |= WS_CLIPCHILDREN;
	return CGameViewBase::PreCreateWindow(cs);			//UPGCommon �۾�
}

int CGameViewLowBD::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CGameViewBase::OnCreate(lpCreateStruct) == -1)		//UPGCommon �۾�
		return -1;
	
	////////////////////////////////////////////////////////////////////////
	// ä��â �ʱ�ȭ
	m_cChatWindow.Init( &g_cUIData, g_pGameView, &g_pGameView->m_cGameHelper );
	g_pChatWinMan = &m_cChatWindow;	

	return 0;
}


BOOL CGameViewLowBD::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if ( g_GAME_BD()->PreTranslateMessage( pMsg ) )
	{
		return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CGameViewLowBD::Init(void)
{
	if ( m_bFirstLoad)
	{
		m_bFirstLoad = false;

		// �߰������ �Ѵ�.
		m_cChatWindow.InitButton(&m_cGameViewBtnMan.m_sprUIBottomBtn);

		g_pGameView->m_cGameHelper.Init(&g_cUIData, GetGame(), g_cUIData.m_nMaxJokbo, new COtherUserJokboCheckManLB, new CJokboProbabilityLB); // ���� ���
		m_cAniSprListManager.Destroy();
		m_cStayAniSprListManager.Destroy();

		m_cGameViewBtnMan.Init( this, &g_cUIData, g_pChatWinMan );

		m_cPlayerDrawManager.Init(&g_cUIData, GetGame());
		for( int i = 0; i < MAX_PLAYER; i++ ) 
		{	// ĳ����â �ʱ�ȭ

			CPlayerDraw_BD* pPlayerDraw = NULL;
			SAFE_NEW( pPlayerDraw, CPlayerDraw_BD );

			m_cPlayerDrawManager.AddPlayer( g_PLAYER( i ), pPlayerDraw, i );
		}


		LAYER_GLOBAL_AUTOBETWND()->Initialize();

		// �ڸ� ��Ŵ��
		g_SeatKeeper.Init(&g_cUIData, g_pChatWinMan);	
		g_cOneShotCharge.Init(&g_cUIData, g_GAME_BD(), &g_SeatKeeper, &GLOBAL_SPR[ spr::GSPR_BTN_CLOSE ] );		//�������
		m_cSidePotMoney.Init(MAX_PLAYER, &g_cUIData, g_pGameView);
		m_cTotalBetMoney.Init(&g_cUIData, g_GAME_BD(), &Page);
		m_cDisplayBoard.Init(&g_cUIData, GetGame());
		m_cBackGround.Init(&g_cUIData, g_pGameView);

		// ������(����) �ʱ�ȭ
		g_ObserverMan.Init( this, &g_cUIData, &g_pGameView->m_cGameViewBtnMan);	
		m_cTitle.Init(&g_cUIData, g_pGameView);
	}

	// ���Ӻ���� �׻� �ʱ�ȭ
	g_pGameView->m_cGameViewBtnMan.InitializeKeyValue( g_Config.eBetBtnType );// �ٵ��� Ŀ��Ű �ʱ�ȭ����
	
	//���������� �ʴ��ſ� �ٸ��氡��� ����
	g_pGameView->m_cGameViewBtnMan.m_btnGotoOtherRoom.Show(g_RI.cafeID_==0);
	g_pGameView->m_cGameViewBtnMan.m_btnGotoOtherRoom.Enable(g_RI.cafeID_==0);

	g_pGameView->m_cGameViewBtnMan.m_btnSInOut.Show(TRUE);
	g_pGameView->m_cGameViewBtnMan.m_btnSInOut.Enable(TRUE);

}

LRESULT CGameViewLowBD::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	short mxp, myp;

	if( m_cSidePotMoney.WindowProc(message,wParam,lParam) ) return TRUE;

	
	switch(message)
	{
	case WM_MOUSEWHEEL:
		return 1;
	case WM_SETCURSOR :
		return 1;

	case WM_NCMOUSEMOVE :
		
		break;

	case WM_MOUSEMOVE :
		{
			mxp = LOWORD(lParam);
			myp = HIWORD(lParam);

			if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) return 1;

			//�ڸ� ��Ŵ�� - ��ġ �� ����
			if(g_SeatKeeper.OnMouseMove(mxp, myp)) return TRUE;
			if(g_pChatWinMan->OnMouseMove(mxp,myp)) return TRUE;
			// ��ư ������ ���콺 �����͸� �հ��� ������� ����
			if(m_cGameViewBtnMan.OnMouseMove(mxp, myp)) return TRUE;

			m_cAniSprListManager.OnMouseMove(mxp,myp);

			LAYER_GLOBAL_AUTOBETWND()->OnMouseMove(mxp, myp);
			if ( g_GAME_BD()->OnMouseMove( mxp, myp ) )
			{
				return 1;	
			}
			
			if(g_cPromotionMan.OnMouseMoveBottom(mxp, myp)) return TRUE;
			
			//�������
			g_cOneShotCharge.OnMouseMove(mxp, myp);
			
			if(g_cOwnLimit.OnMouseMove(mxp, myp)) return TRUE;
			
			if(m_cPlayerDrawManager.OnMouseMove(mxp, myp)) return TRUE;
			
			if(g_cPromotionMan.OnMouseMoveMiddle(mxp, myp)) return TRUE;
			
			//����UI
			g_ObserverMan.OnMouseMove(mxp, myp);
			g_cVipJackpot.OnMouseMove(mxp, myp);
			
			if(g_cPromotionMan.OnMouseMoveTop(mxp, myp)) return TRUE;
		}
		return 1;

	case WM_LBUTTONDOWN :
		{	
			mxp = LOWORD(lParam);
			myp = HIWORD(lParam);

			if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)return 1; 
			if(TRUE==g_cVipJackpot.OnLButtonDown(mxp, myp)){
				return 1;
			}

			CRect rc;		
			rc.SetRect( 0, 0, 712, 25 );
			POINT pt;
			pt.x = mxp;
			pt.y = myp;
			if( rc.PtInRect( pt ) )	{
				GM().GetMainWnd()->SendMessage( WM_LBUTTONDOWN, MK_LBUTTON, lParam );
				return 1;
			}

			if(g_cPromotionMan.OnLButtonDownBottom(mxp, myp)) return TRUE;
		

			//�ڵ� ���� ���� ��ư 

			POINT bpt;
			bpt.x = mxp;
			bpt.y = myp;		


			bLClick = TRUE;		
			this->SetFocus();	
			SetCapture();


			//�ڸ� ��Ŵ��  -��ġ �� ����
			if ( g_SeatKeeper.OnLButtonDown(mxp, myp) )
			{
				return 1;
			}

			//�������
			if ( g_cOneShotCharge.OnLButtonDown(mxp, myp) )
			{
				return 1;
			}

			// ĳ���� ����� �׻� Ŭ�� ����
			g_pChatWinMan->OnLButtonDown(mxp, myp);

			m_cAniSprListManager.OnLButtonDown(mxp,myp);
			
			if ( g_GAME_BD()->OnLButtonDown( mxp, myp ) )
			{
				return 1;
			}
			
			if ( m_cPlayerDrawManager.OnLButtonDown(mxp, myp) )
			{
				return 1;
			}

			if(g_cPromotionMan.OnLButtonDownMiddle(mxp, myp)) return TRUE;


			m_cGameViewBtnMan.OnLButtonDown(mxp, myp);	
			
			//����UI
			g_ObserverMan.OnLButtonDown(mxp, myp);

			if(g_cPromotionMan.OnLButtonDownTop(mxp, myp)) return TRUE;
			
		}
		return 1;

	case WM_LBUTTONUP :
		{
			mxp = LOWORD(lParam);
			myp = HIWORD(lParam);

			if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)return 1;
			ReleaseCapture();

			if(g_cPromotionMan.OnLButtonUpBottom(mxp, myp)) return TRUE;

			//�ڸ� ��Ŵ�� - ��ġ �� ����
			if ( g_SeatKeeper.OnLButtonUp(mxp, myp) )
			{
				return 1;
			}
			//�������
			if ( g_cOneShotCharge.OnLButtonUp(mxp, myp) )
			{
				return 1;
			}

			g_pChatWinMan->OnLButtonUp(mxp, myp);
			
			if ( g_GAME_BD()->OnLButtonUp(mxp, myp) )
			{
				return 1;
			}

			if ( m_cPlayerDrawManager.OnLButtonUp(mxp, myp) )
			{
				return 1;
			}

			if(g_cPromotionMan.OnLButtonUpMiddle(mxp, myp)) return TRUE;

			m_cGameViewBtnMan.OnLButtonUp(mxp, myp);
			//////////////////////
			m_cAniSprListManager.OnLButtonUp(mxp,myp);
			
			
			
			//����UI
			g_ObserverMan.OnLButtonUp(mxp, myp);
			g_cVipJackpot.OnLButtonUp(mxp, myp);

			if(g_cPromotionMan.OnLButtonUpTop(mxp, myp)) return TRUE;
		}
		return 1;

	case WM_LBUTTONDBLCLK :
		{
			mxp = LOWORD(lParam);
			myp = HIWORD(lParam);
			if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)return 1;


			/*
			if( !Game.GetCurGameProcess()->OnLButtonDblclk(mxp, myp) ){
				CRect rt;
				rt.SetRect(0,0,27,20);
				rt.OffsetRect(701,712);

				POINT pt;
				pt.x = mxp;
				pt.y = myp;

				if( rt.PtInRect(pt) ){
					PostMessage(WM_COMMAND ,IDM_OBSERVERVIEW,0);					
				}
			}
			*/
		}
		return 1;
	case WM_RBUTTONUP:
		{	
#ifdef _DEBUG
		//	g_bDeBugModeFlg = !g_bDeBugModeFlg;
		//	Game.GetCurGameProcess()->Set_auto_betting_flag();
#endif			
		}break;
	}
	
	return CGameViewBase::WindowProc(message, wParam, lParam);	//UPGCommon �۾�
}

void CGameViewLowBD::OnPaint() 
{
	// ���� ������ �ƴϸ� �׸��� �ʴ´�.
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
	{
		return;
	}

	CFont *pTempFont = NULL;

	CPaintDC dc(this); // device context for painting

	// ���� ���ҽ� ���� �� �÷��� ����Ʈ ����
	//NMBASE::SOUND::g_Sound.RegularCall();

	// ���� ���۸��� �޸� DC ����
	CDC MemDC;
	MemDC.CreateCompatibleDC(&dc);

	// �޸�DC�� ���ۿ� ��Ʈ�� �ڵ��� ����
	MemDC.SelectObject(Page.hBmp);
	MemDC.SetBkMode(TRANSPARENT);
	
	//�� ���Ӹ��� ��� �̹����� �ΰ� �ٸ��� ǥ�� ������ ǥ��
	m_cBackGround.DrawBottom(&Page, &MemDC);

	//�������
	draw_btn_func( &MemDC );  // ��ư

	if(g_RI.ChipKind == 0)	DrawJackPot(); // ����

	g_pChatWinMan->Draw( &MemDC );	

	m_cDisplayBoard.Draw( &MemDC ); // �ֻ��� ��������

	m_cTotalBetMoney.Draw(&MemDC);  // �� ���� �ݾ� �� �ݾ׼���
	//m_Roominfo.DrawRoomInfo(&Page); // ���� ���� ������	

	m_cAniSprListManager.DrawBottom(&MemDC); // ����Ʈ

	g_GAME_BD()->m_cAllBetChip.DrawBottom(); // Ĩ
	
	m_cStayAniSprListManager.DrawBottom(&MemDC);
	g_cPromotionMan.DrawBottom(&MemDC, &Page);
	g_cOwnLimit.DrawBottom();
	
	m_cPlayerDrawManager.DrawBottom( &MemDC );

	g_cVipJackpot.DrawBottom(&Page, &MemDC);

	m_cAniSprListManager.DrawMiddle(&MemDC);	
	m_cStayAniSprListManager.DrawMiddle(&MemDC);	
	
	m_cPlayerDrawManager.Draw_CardAll( &MemDC ); //ī��
	
	m_cBackGround.DrawMiddle(&Page, &MemDC);

	m_cDisplayBoard.Draw( &MemDC ); // �ֻ��� ��������

	m_cTitle.Draw(&Page, &MemDC); // ī�� ���� �Ŀ� �׸���.

	g_cVipJackpot.DrawMiddle(&Page, &MemDC);

	m_cSidePotMoney.DrawBottom(&MemDC);	     // ���̵���
	m_cPlayerDrawManager.DrawMiddle(&MemDC);
	g_cPromotionMan.DrawMiddle(&MemDC, &Page);
	
	LAYER_GLOBAL_AUTOBETWND()->Draw_Top(&MemDC);	

	m_cSidePotMoney.DrawTop(&MemDC);
	m_cPlayerDrawManager.DrawTop(&MemDC);
	g_GAME_BD()->m_cAllBetChip.DrawTop();
	g_cOwnLimit.DrawTop(&MemDC);

	COLORREF color = MemDC.SetTextColor( RGB(187, 171, 145) );
	pTempFont = MemDC.SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_NORMAL));
	
	//����UI
	g_ObserverMan.Draw(&Page, &MemDC);

	g_cPromotionMan.DrawTop(&MemDC, &Page);

	//�ڸ� ��Ŵ��
	g_SeatKeeper.Draw(&Page, &MemDC);       // �� ���� ���̾�� �׸���.
	g_cOneShotCharge.Draw( &MemDC );
	m_cAniSprListManager.DrawTop(&MemDC);	
	m_cStayAniSprListManager.DrawTop(&MemDC);	

#ifdef _DEBUG
	// ������ ����Ʈ üũ
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

	// ���콺 ��ǥ üũ
	POINT mpos;
	GetCursorPos(&mpos);
	ScreenToClient(&mpos);
	CString str;
	str.Format("%5.1f FPS, Mouse (%d, %d)", FPS, mpos.x, mpos.y);

	MemDC.SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_NORMAL));
	MemDC.SetBkMode(TRANSPARENT);
	MemDC.SetTextColor(RGB(255,255,255));
	MemDC.TextOut( 57, 38, str);

#endif

	m_cPlayerDrawManager.DrawTopMost(&MemDC);

	g_cVipJackpot.DrawTop(&Page, &MemDC);

	//�����׸���
	Page.DrawEmptyBox(0,0,GetWidth()-1, GetHeight()-1, NMBASE::GRAPHICGDI::xSprite::RGB555(100,100,100));

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &MemDC, 0, 0, SRCCOPY);

	MemDC.DeleteDC();
	
}

void CGameViewLowBD::Reset()
{
	m_bExitSubscript = FALSE;	
}

void CGameViewLowBD::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default

	if ( nIDEvent == GAMEVIEW_TIMER )
	{
		if ( GetCurrentWhere() == IDX_GAMEWHERE_GAME )
		{
			m_cAniSprListManager.AniSprProcess();
			m_cStayAniSprListManager.AniSprProcess();

			g_pChatWinMan->OnTimer();
		}
	}

	CGameViewBase::OnTimer(nIDEvent);		//UPGCommon �۾�
}

void CGameViewLowBD::OnDestroy() 
{
	CGameViewBase::OnDestroy();		//UPGCommon �۾�

	// ### [ Frame++ ] ###
	if(m_hEventTimer) 
	{
		timeKillEvent(m_hEventTimer);
	}

	m_hEventTimer = 0;

	m_cChatWindow.OnDestroy();
}


BOOL CGameViewLowBD::OnCommand(WPARAM wParam, LPARAM lParam) 
{	
	if( g_cPromotionMan.OnCommand(wParam, lParam)) return TRUE;
	if ( GetCurrentWhere() == IDX_GAMEWHERE_GAME )
	{
		if (g_GAME_BD()->OnCommand( wParam , lParam ) )
		{
			return true;
		}

		if ( m_cSidePotMoney.OnCommand(wParam,lParam) )	return TRUE;
		if ( m_cGameViewBtnMan.OnCommand(wParam,lParam)) return TRUE;
		if ( g_ObserverMan.OnCommand(wParam, lParam) ) return TRUE;
		if(g_cVipJackpot.OnCommand(wParam, lParam)) return TRUE;
	}	
	
	return CGameViewBase::OnCommand(wParam, lParam);		//UPGCommon �۾�
}

void CGameViewLowBD::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CGameViewBase::OnChar(nChar, nRepCnt, nFlags);			//UPGCommon �۾�
}

/////////////////////////////////////////////////////////////////////////////////

// void CGameViewLowBD::draw_play_card_func(CDC *pDC)
// {
// 	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)return;
// 
// 	//if ( g_GAME_BD()->m_cAllBetChip.m_bMoveStart == FALSE )
// 	{
// 		g_GAME_BD()->m_cAllBetChip.DrawAllBetMoney();
// 	}
// }


void CGameViewLowBD::draw_btn_func(CDC *pDC)
{
	m_cGameViewBtnMan.DrawBtn(pDC);
}


//2006.1.9 ���ǵ���
DWORD CGameViewLowBD::GetCurrentPlayTick()
{
	return timeGetTime()-g_nStartTick;
}



//UPG �۾� �߰�
void CGameViewLowBD::ShowEffect_AllowChancePurchase(int nItemCode)
{
	CChance_Msg_EffectSpr *pObj = NULL;
	SAFE_NEW( pObj, CChance_Msg_EffectSpr );

	pObj->Init(nItemCode);
	g_PLAYER(0)->GetEffectObjectList()->AddObject( pObj );
}

void CGameViewLowBD::OnExitRoom(void)
{
}

const int CGameViewLowBD::GetWidth()
{
	return 1000;
}

const int CGameViewLowBD::GetHeight()
{
	return 740;
}


void CGameViewLowBD::DrawJackPot()
{		

#if defined(_JACKPOT_NON_CHANNEL_MONEY)
	INT64 &n64JackPotMoney	  = GDM().GetINT64(GDK_JACKPOT_MONEY);
	INT64 &n64TmpJackPotMoney = GDM().GetINT64(GDK_JACKPOT_MONEY_TEMP);
#else
	INT64 &n64JackPotMoney	  = g_GAME_BD()->GetOldJackPotMoney();
	INT64 &n64TmpJackPotMoney = g_GAME_BD()->GetJackPotMoney();
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
		
		n64JackPotMoney += plus; // ���� �����ֱ�
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

		n64JackPotMoney -= plus; // ���� �����ֱ�
	}

	INT64 tmp = n64JackPotMoney;
	int kk = 0;			
	int addx = 0;
	int count = 0;	

	int bnum = (g_GAME_BD()->m_nGameCount/30) % 2;	

	for( int j = 0; j < 20 ; j ++ ) {
		if( tmp <= 0 ) 
		{				
			// ������ ������� �ʱ�ȭ 0���� �Ѵ�.
			if(tmp == 0 && 0 == n64JackPotMoney && 0 == n64TmpJackPotMoney)
			{
				Page.PutSprAuto( g_cUIData.m_ptJacpotNumberStart.x + addx - (j + 1) * g_cUIData.m_nJacpotNumberWidth, g_cUIData.m_ptJacpotNumberStart.y, &GLOBAL_SPR[ spr::GSPR_GAME_JACKPOT ], 0 );
			}
			break;
		}
		if(kk >= 3)	{
			kk = 0;
			Page.PutSprAuto( g_cUIData.m_ptJacpotNumberStart.x + addx - (j + 1) * g_cUIData.m_nJacpotNumberWidth, g_cUIData.m_ptJacpotNumberStart.y, &GLOBAL_SPR[ spr::GSPR_GAME_JACKPOT ], 10 ); // ,
			addx -= 6;
			count++;
		}
		Page.PutSprAuto( g_cUIData.m_ptJacpotNumberStart.x + addx - (j + 1) * g_cUIData.m_nJacpotNumberWidth, g_cUIData.m_ptJacpotNumberStart.y, &GLOBAL_SPR[ spr::GSPR_GAME_JACKPOT ],(int)(tmp%(INT64)10) );
		tmp = tmp / 10;
		kk++;
	}
}


BOOL CGameViewLowBD::OnEraseBkgnd(CDC* pDC)
{	
	OnPaint();

	return TRUE;
}