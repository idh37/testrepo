// SeatKeeper.cpp: implementation of the CSeatKeeper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SeatKeeper.h"
#include <UPGCommon.h>
#include <UserStruct.h>
#include "CommMsgDef_Game.h"
#include "../UIInfoData.h"
#include "../ChattingWindowMan/ChattingWindowMan.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//메세지 텍스트 색깔 
#define COLOR_TEXTMSG		RGB(240, 0, 0)

#define __USE_BUTTON__			0
#define __CANCEL_BUTTON__		0

static CSeatKeeper* s_pThis;	// 자신의 포인터

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSeatKeeper::CSeatKeeper()
{
	m_pBrowser = NULL;

	m_nLeftCnt = 0;
	m_nCancelHistory = 0;

	m_pUIInfoData = NULL;
	m_pGameView = NULL;

	Clear();
	
	// 자신의 포인터
	s_pThis = this;	
	m_bShowBtn = true;
}

CSeatKeeper::~CSeatKeeper()
{
	DestroyBrowser();
}

void CSeatKeeper::Clear()
{
	m_bInUse = FALSE;
	m_nReservePNum = -1;
	ZeroMemory(m_UserID, sizeof(m_UserID));
	m_nTurnCnt = 0;	
	m_nMaxCnt = 0;
}


BOOL CSeatKeeper::Init(CUIInfoData *pUIInfoData, CChattingWindowMan* pChatWin)
{
	// 게임 프로세스를 얻어온다.
	m_pGameProcess	= GM().GetCurrentProcess();
	m_pGameView		= GM().GetCurrentGameView();
	m_pUIInfoData	= pUIInfoData;	
	m_pChatWin		= pChatWin;	


	// 값을 확인해 주세요.
	if(!m_pUIInfoData || !m_pChatWin || !m_pGameProcess || !m_pGameView)
	{
		__asm int 3 
		return FALSE;
	}	
	
	
	// 아바타를 기준으로한 자리 지킴이 윈도우 상대좌표
	const int nMaxPlayer = m_pGameProcess->GetMaxPlayer();
	for(int i = 0; i < nMaxPlayer; ++i)
	{
		CPoint ptTemp = m_pUIInfoData->m_listAvatarPos[i] + m_pUIInfoData->m_listSeatKeeperWinPos[i];
		m_listSeatKeeperWinPos.push_back(ptTemp);
	}			

	// 자리지킴이 사용 버튼
	m_Button[SEATKEEPERBUTTON_USE].Init(m_pGameView, &m_pGameView->Page, m_pUIInfoData->m_listButtonPos[EBT_SEATKEEPER].x, m_pUIInfoData->m_listButtonPos[EBT_SEATKEEPER].y, &m_cSeatKeeperBtnSpr, 0, IDM_BTN_USESEATKEEPER);

	// 게임 참여 버튼(자리 지킴이 윈도우 기준)
	CPoint ptTemp = m_listSeatKeeperWinPos[0] + m_pUIInfoData->m_SeatKeeperJoinButPos;
	m_Button[SEATKEEPERBUTTON_BACKTOGAME].Init(m_pGameView, &m_pGameView->Page, ptTemp.x, ptTemp.y, &m_cSeatKeeperSpr, 0, IDM_BTN_BACKTOGAME);

	SetShowSeatKeeperBtn(true);

	return TRUE;
}

// 게임이 바뀔때마다 호출을 해서 this포인터 값을 변경해 준다.
void CSeatKeeper::SetChangeThisPoint()
{
	s_pThis = this;	
}

void CSeatKeeper::LoadImage()
{
	AddLoadImageList(&m_cSeatKeeperBtnSpr,".\\PokerCommon\\data\\SeatKeeperBtn.spr", true);
	AddLoadImageList(&m_cSeatKeeperSpr,   ".\\PokerCommon\\data\\sp_seatKeeper.spr", true);	
}

void CSeatKeeper::DestroyBrowser()
{
	if( m_pBrowser )
	{
		m_pBrowser->Destroy();
		delete m_pBrowser;
		m_pBrowser = NULL;
	}
}

BOOL CSeatKeeper::CreateBrowser()
{
	DestroyBrowser();
	
	RECT rt = 
	{ 
		m_listSeatKeeperWinPos[0].x + m_pUIInfoData->m_rtSeatKeeperWeb.left,
		m_listSeatKeeperWinPos[0].y + m_pUIInfoData->m_rtSeatKeeperWeb.top,
		m_listSeatKeeperWinPos[0].x + m_pUIInfoData->m_rtSeatKeeperWeb.right, 
		m_listSeatKeeperWinPos[0].y + m_pUIInfoData->m_rtSeatKeeperWeb.bottom
	};

	m_pBrowser = new NMBASE::UTIL::CNMSSOWebBrowser();
	if( !m_pBrowser->Create( WS_VISIBLE | WS_CHILD, rt, m_pGameView->GetSafeHwnd(), 2756 ) )
	{
		if( m_pBrowser )
		{
			delete m_pBrowser;
			m_pBrowser = NULL;
		}
		return FALSE;
	}
	m_pBrowser->Navigate( "about:blank" );
	m_pBrowser->SetHideBorder( TRUE );


	m_pBrowser->SetCallback_OnBeforeNavigate2( OnBeforeNavigate_SeatKeeper );
	m_pBrowser->Navigate(getGameURL(IDX_GAMEURL_SEATKEEPER));

	return TRUE;
}

void CSeatKeeper::OnBeforeNavigate_SeatKeeper(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, LPCTSTR lpszPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel)
{
	CString strURL = lpszURL;

	if(strURL.Find("_구매완료_") > 0)
	{
		if(pbCancel)
		{
			*pbCancel = TRUE;
		}

		CSV_ASK_GAME_CONTINUE msg;
		msg.Set(GM().GetMyInfo()->UI.ID, 0);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());

		if(s_pThis)
		{
			s_pThis->AddChatMessage("자리지킴이 전용 아바타 구매가 완료되었습니다.\n", COLOR_TEXTMSG);
		}
	}
}

void CSeatKeeper::OnOutRoom()
{
	Clear();
	DestroyBrowser();
}

void CSeatKeeper::OnEnterGame()
{

#ifdef BETAVER
	return;
#endif

	RefreshIconButton();

	//버튼 변경 ( 사용 )
	m_Button[ SEATKEEPERBUTTON_USE ].SetSprite( &m_cSeatKeeperBtnSpr, __USE_BUTTON__ );

	m_Button[SEATKEEPERBUTTON_USE].Show(TRUE);
	m_Button[SEATKEEPERBUTTON_USE].Enable(TRUE);
}


void CSeatKeeper::OnStartGame()
{
	// 예약취소 카운트 리셋 
	m_nCancelHistory = 0;		
}

char *CSeatKeeper::FindNickName(const char *pID)
{
	const int nMaxPlayer = m_pGameProcess->GetMaxPlayer();
	for(int i = 0; i < nMaxPlayer; ++i)
	{
		if(strncmp(m_pGameProcess->GetPlayer(i)->UI.ID, pID, 15) == 0)
		{
			return m_pGameProcess->GetPlayer(i)->UI.NickName;
		}
	}

	return NULL;
}

void CSeatKeeper::ProcessPacket(CSV_SEATKEEPER *pSeatKeeperMsg)
{

#ifdef _BETAVER
	
	return;

#endif

	//사용완료
	//금일 자리지킴이 사용기회 x회를 모두 사용하셨습니다.

	if(pSeatKeeperMsg==NULL)
	{
		return;
	}

	m_nReservePNum	= *pSeatKeeperMsg->ServPNum;	
	m_nTurnCnt		= *pSeatKeeperMsg->TurnCnt;	
	m_nMaxCnt		= *pSeatKeeperMsg->MaxCnt;
	strncpy_s(m_UserID, sizeof(m_UserID), pSeatKeeperMsg->ReserveID, 15);

	switch(*pSeatKeeperMsg->Sig)
	{
	case CSV_SEATKEEPER::SEATKEEPER_SIG_RESERVE:
		{			
			m_bInUse = FALSE;
			char *pNick = FindNickName(m_UserID);

			if(pNick)
			{
				CString strMsg;
				strMsg.Format("[%s]님이 자리지킴이를 예약했습니다.\n", pNick);

				AddChatMessage((char*)strMsg.operator LPCTSTR(), COLOR_TEXTMSG);

				if ( m_nReservePNum == m_pGameProcess->GetPlayer( 0 )->ServPNum )
				{
					//버튼 변경 ( 취소 )
					m_Button[ SEATKEEPERBUTTON_USE ].SetSprite( &m_cSeatKeeperBtnSpr, __CANCEL_BUTTON__ );
				}
			}
		} 
		break;

	case CSV_SEATKEEPER::SEATKEEPER_SIG_USING:
		{	
			m_bInUse = TRUE;

			if(m_pBrowser == NULL)
			{

				if(g_bMyObserver == FALSE && m_pGameProcess->GetPlayer(0)->ServPNum == m_nReservePNum && GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
				{
					#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
						if (NMBASE::UTIL::GetCurSiteNo() != NMBASE::UTIL::SNO_TOCTOC)
							CreateBrowser();
					#else //UPGRADE_20120308_TOCTOC_CHANNELING
						CreateBrowser();
					#endif //UPGRADE_20120308_TOCTOC_CHANNELING
					m_nLeftCnt = *pSeatKeeperMsg->LeftCnt;	//지킴이 사용자가 본인일 경우만 업뎃

					if(m_nLeftCnt <= 0)
					{
						CString strMsg;
						strMsg.Format("금일 자리지킴이 사용기회 %d회를 모두 사용하셨습니다.\n", m_nMaxCnt);
						AddChatMessage((char*)strMsg.operator LPCTSTR(), COLOR_TEXTMSG);
					}
				}
			}
			else
			{
				//DestroyBrowser();
			}
		} 
		break;

	case CSV_SEATKEEPER::SEATKEEPER_SIG_CLOSE:
		{	
			char *pNick = FindNickName(m_UserID);
			if(pNick)
			{		
				CString strMsg;
				if(m_bInUse)
				{								
					strMsg.Format("[%s]님이 게임에 다시 참여하셨습니다.\n", pNick);	
				}
				else
				{
					strMsg.Format("[%s]님이 자리지킴이 예약을 취소하였습니다.\n", pNick);
				}
				AddChatMessage((char*)strMsg.operator LPCTSTR(), COLOR_TEXTMSG);

				if ( m_nReservePNum == m_pGameProcess->GetPlayer( 0 )->ServPNum )
				{
					//버튼 변경 ( 사용 )
					m_Button[ SEATKEEPERBUTTON_USE ].SetSprite( &m_cSeatKeeperBtnSpr, __USE_BUTTON__ );
				}
			}			

			Clear();			
		}
		break;

	case CSV_SEATKEEPER::SEATKEEPER_SIG_COUNTINFO:
		{
			//내 정보 
			m_nLeftCnt = *pSeatKeeperMsg->LeftCnt;

			if(m_nLeftCnt <= 0)
			{
				CString strMsg;
				strMsg.Format("금일 자리지킴이 사용기회 %d회를 모두 사용하셨습니다.\n", m_nMaxCnt);
				AddChatMessage((char*)strMsg.operator LPCTSTR(), COLOR_TEXTMSG);
			}
		} 
		break;

	default:
		{		
			Clear();
		} 
		break;
	}

	if(m_bInUse == FALSE)
	{
		DestroyBrowser();
	}

	RefreshIconButton();

}
BOOL CSeatKeeper::OnCommand(WPARAM wParam, LPARAM lParam)
{

#ifdef _BETAVER

	return FALSE;

#endif

	switch(wParam)
	{
	case IDM_BTN_USESEATKEEPER:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_MYCARDAREA_SEETKEEPER);
			#endif //UPGRADE_10210619_ADD_CLICKLOG			

			// 상태를 갱신한다.
			RefreshIconButton();			
			if(m_eSeatKeeperState == ESK_ON)
			{
				AskReserve();
			}
			else if(m_eSeatKeeperState == ESK_OFF)
			{
				CancelReserve();
			}			
		}	
		return TRUE;

	case IDM_BTN_BACKTOGAME:
		{
			BackToGame();
		}
		return TRUE;
	}

	return FALSE;
}

// 게임으로 돌아간다는 패킷을 보낸다.
BOOL CSeatKeeper::BackToGame()
{	
	//게임참가 하기
	if(m_bInUse == FALSE)
		return FALSE;
	
	if(g_bMyObserver)
		return FALSE;

	if(m_nReservePNum != m_pGameProcess->GetPlayer(0)->ServPNum)
		return FALSE;

	CSV_SEATKEEPER smsg;
	smsg.Set(CSV_SEATKEEPER::SEATKEEPER_SIG_BACKTOGAME, GM().GetMyInfo()->UI.ID, 0, 0, 0, 0); 
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), smsg.pData, smsg.GetTotalSize());

	return TRUE;
}

BOOL CSeatKeeper::CancelReserve()
{
	if(GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return FALSE;
	
	if(g_bMyObserver)
		return FALSE;

	if(AbleToUseSeatKeeper(&GM().GetMyInfo()->UI) == FALSE) 
		return FALSE;

	//사용중일때 취소 불가 
	if(m_bInUse)
		return FALSE; 

	//내가 예약중인가?
	if(m_pGameProcess->GetPlayer(0)->ServPNum != m_nReservePNum) 
		return FALSE;

	if(m_nCancelHistory >= 1)
	{
		AddChatMessage("자리지킴이 서비스의 예약취소는 1회만 가능합니다.\n", COLOR_TEXTMSG); 
		SetSeatKeeperIcon(IC_STATE_SEATKEEPER_REFUSE_TO_USE_FOR_ONE_CANCEL_ONLY);
		return FALSE;	///예약취소는 한번만 가능
	}

	CSV_SEATKEEPER smsg;
	smsg.Set(CSV_SEATKEEPER::SEATKEEPER_SIG_BACKTOGAME, GM().GetMyInfo()->UI.ID, 0, 0, 0, 0); 
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), smsg.pData, smsg.GetTotalSize());

	m_nCancelHistory++;

	return TRUE;
}

BOOL CSeatKeeper::AskReserve()
{
	if(GM().GetCurrentWhere() != IDX_GAMEWHERE_GAME)
		return FALSE;
	
	if(m_bInUse && (m_nReservePNum == m_pGameProcess->GetPlayer(0)->ServPNum)) 
	{		
		//내가 사용중일 때. 
		AddChatMessage("자리지킴이 서비스를 사용중입니다.\n", COLOR_TEXTMSG);
		return FALSE;
	}

	const int nMaxPlayer = m_pGameProcess->GetMaxPlayer();
	if(m_bInUse || (m_nReservePNum >= 0 && m_nReservePNum < nMaxPlayer) )
	{
		AddChatMessage("동일 게임방에서 최대 1명까지 자리지킴이 서비스를 사용할 수 있습니다.\n", COLOR_TEXTMSG);
		SetSeatKeeperIcon(IC_STATE_SEATKEEPER_REFUSE_TO_USE_FOR_MULTIPLE_USE);
		return FALSE;
	}

	//현재 대기중이라면 예약 안됨
	if(g_RI.State == 0 || m_pGameProcess->GetPlayer(0)->JoinState == 0)		
	{
		AddChatMessage("자리지킴이 서비스는 게임진행 중\n예약 사용이 가능합니다.\n", COLOR_TEXTMSG); 
		SetSeatKeeperIcon(IC_STATE_SEATKEEPER_REFUSE_TO_USE_FOR_NOT_PLAYING);
		return FALSE; 
	}

	CSV_SEATKEEPER smsg;
	smsg.Set(CSV_SEATKEEPER::SEATKEEPER_SIG_ASKRESERVE, GM().GetMyInfo()->UI.ID, 0, 0, 0, 0); 
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), smsg.pData, smsg.GetTotalSize());	

	return TRUE;
}


void CSeatKeeper::Draw(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC)
{

#ifdef _BETAVER

	return;

#endif

	if(m_bInUse)
	{
		const int nMaxPlayer = m_pGameProcess->GetMaxPlayer();

		//내가 사용중인가?
		if(g_bMyObserver == FALSE && m_pGameProcess->GetPlayer(0)->ServPNum == m_nReservePNum)
		{
			//큰 표지판
			Draw_BigPanel(pPage, pDC);	
		}
		else if(m_nReservePNum >= 0 && m_nReservePNum < nMaxPlayer)
		{
			//딴 유저가 사용중이다.
			int pnum = m_pGameProcess->GetPNum_ByServPN(m_nReservePNum);				

			if(pnum > 0 && pnum < nMaxPlayer)
			{
				/// 작은 표지판 
				Draw_SmallPanel(pnum, pPage, pDC);
			}
		}
	}

	for(int i = 0; i < MAX_SEATKEEPERBUTTON; ++i)
	{
		if((i == SEATKEEPERBUTTON_USE) && !m_bShowBtn) continue;
		if(ShouldEnableButton((SEATKEEPERBUTTON)i))
		{
			m_Button[i].Draw(pDC);
		}
	}	

	//m_Button[0].Draw(pDC); // Test LDH
}


BOOL CSeatKeeper::OnMouseMove(int x, int y)
{

#ifdef _BETAVER

	return FALSE;

#endif

	for ( int i = 0; i < MAX_SEATKEEPERBUTTON; ++i )
	{
		if ( ShouldEnableButton((SEATKEEPERBUTTON)i))
		{
			if((i == SEATKEEPERBUTTON_USE) && !m_bShowBtn) continue;
			if ( m_Button[i].OnMouseMove(x,y) )
			{
				SetCursor(GM().GetCurHandCursor());
				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CSeatKeeper::OnLButtonDown(int x, int y)
{

#ifdef _BETAVER

	return FALSE;

#endif

	for ( int i = 0; i < MAX_SEATKEEPERBUTTON; ++i )
	{
		if ( ShouldEnableButton((SEATKEEPERBUTTON)i) )
		{
			if((i == SEATKEEPERBUTTON_USE) && !m_bShowBtn) continue;
			if ( m_Button[i].OnLButtonDown(x,y) )
			{
				return TRUE;
			}
		}
	}

	return FALSE;

}

BOOL CSeatKeeper::OnLButtonUp(int x, int y)
{

#ifdef _BETAVER

	return FALSE;

#endif

	for ( int i = 0; i < MAX_SEATKEEPERBUTTON; ++i )
	{
		if ( ShouldEnableButton((SEATKEEPERBUTTON)i))
		{
			if((i == SEATKEEPERBUTTON_USE) && !m_bShowBtn) continue;
			if ( m_Button[i].OnLButtonUp( x, y ) )
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}

// 내 자신 
void CSeatKeeper::Draw_BigPanel(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC)
{
	// 1. Back이미지
#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
		pPage->PutSprAuto(m_listSeatKeeperWinPos[0].x, m_listSeatKeeperWinPos[0].y + 44, &m_cSeatKeeperSpr, 11);
	else
		pPage->PutSprAuto(m_listSeatKeeperWinPos[0].x, m_listSeatKeeperWinPos[0].y, &m_cSeatKeeperSpr, 10);
#else //UPGRADE_20120308_TOCTOC_CHANNELING
	pPage->PutSprAuto(m_listSeatKeeperWinPos[0].x, m_listSeatKeeperWinPos[0].y, &m_cSeatKeeperSpr, 10);
#endif //UPGRADE_20120308_TOCTOC_CHANNELING

	// 2. 턴 이미지
	for(int i = 0; i < m_nTurnCnt; ++i)
	{		
		CPoint ptPoint = m_listSeatKeeperWinPos[0] + m_pUIInfoData->m_listTurnDisplayPos[i];

		// 자기자신의 표시는 44만큼의 갭이 존재
		ptPoint.y += 44;

		pPage->PutSprAuto(ptPoint.x, ptPoint.y, &m_cSeatKeeperSpr, 4 + i);
	}

	int lefttrun = max(MAX_SEATKEEPER_TURN - m_nTurnCnt, 0);

	// 3. 남은턴수
	CString str;
	str.Format("%d", lefttrun);		
	CPoint ptText = m_listSeatKeeperWinPos[0] + m_pUIInfoData->m_TurnTextPos;

	// 자기자신의 표시는 44만큼의 갭이 존재
	ptText.y += 44;

	CRect rt;
	pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_BOLD)); //굴림, 사이즈 10, 볼드체
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_pUIInfoData->m_clrSeatKeeperTurn);
	//pDC->TextOut(ptText.x, ptText.y, str);
	rt.SetRect(ptText.x, ptText.y, ptText.x + 11, ptText.y + 11); 
	pDC->DrawText(str, &rt, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
}

// 상대방 자리지킴이(작은 거)
void CSeatKeeper::Draw_SmallPanel(int nPlayerPos, NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC)
{
	// 1. Back이미지
	pPage->PutSprAuto(m_listSeatKeeperWinPos[nPlayerPos].x, m_listSeatKeeperWinPos[nPlayerPos].y, &m_cSeatKeeperSpr, 9);

	// 2. 턴 이미지
	for(int i = 0; i < m_nTurnCnt; ++i)
	{		
		CPoint ptPoint = m_listSeatKeeperWinPos[nPlayerPos] + m_pUIInfoData->m_listTurnDisplayPos[i];

		pPage->PutSprAuto(ptPoint.x, ptPoint.y, &m_cSeatKeeperSpr, 4 + i);
	}

	int lefttrun = max(MAX_SEATKEEPER_TURN - m_nTurnCnt, 0);

	// 3. 턴 숫자 출력
	CString str;
	str.Format("%d", lefttrun);
	CPoint ptText = m_listSeatKeeperWinPos[nPlayerPos] + m_pUIInfoData->m_TurnTextPos;

	CRect rt;
	pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_BOLD)); //굴림, 사이즈 10, 볼드체
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_pUIInfoData->m_clrSeatKeeperTurn);
	rt.SetRect(ptText.x, ptText.y, ptText.x + 11, ptText.y + 11); 
	pDC->DrawText(str, &rt, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
}

BOOL CSeatKeeper::ShouldEnableButton(SEATKEEPERBUTTON Button)
{	
	//자리지킴이 사용 버튼이 활성화 되어야하는 상태인가? 
	if(Button == SEATKEEPERBUTTON_USE)	
	{	
		//프리미엄 골드이상 사용 버튼 항시 노출
		if(g_bMyObserver)
		{
			return FALSE;
		}


// 		if(m_pGameProcess->GetPlayer(0)->ServPNum == m_nReservePNum) 
// 		{
// 			return FALSE;
// 		}

		return AbleToUseSeatKeeper(&GM().GetMyInfo()->UI);
	}

	//게임 참여 버튼이 활성화 되어야하는 상태인가? 
	if(Button == SEATKEEPERBUTTON_BACKTOGAME)
	{
		if(m_bInUse == FALSE) 
		{
			return FALSE;
		}

		if(g_bMyObserver) 
		{
			return FALSE;
		}

		if(AbleToUseSeatKeeper(&GM().GetMyInfo()->UI) == FALSE) 
		{
			return FALSE;
		}

		return (m_pGameProcess->GetPlayer(0)->ServPNum == m_nReservePNum);
	}	

	return FALSE;
}


//아이콘 버튼 상태를 갱신한다. 
void CSeatKeeper::RefreshIconButton() 
{	
	SetSeatKeeperIcon(GetSeatKeeperState());
}

void CSeatKeeper::SetSeatKeeperIcon(IC_STATE_SEATKEEPER SeatKeeperState)
{	
	SetSeatKeeperState(SeatKeeperState, m_nLeftCnt);
}

IC_STATE_SEATKEEPER CSeatKeeper::GetSeatKeeperState()
{	
	// ** 순서 중요 **//
	if ( g_bMyObserver )
	{
		return IC_STATE_SEATKEEPER_HIDE;							// 관전자는 자리지킴이 Hide						
	} 

	// 프리미엄 골드이상 사용 버튼 항시 노출
	if(AbleToUseSeatKeeper(&GM().GetMyInfo()->UI) == FALSE)
	{
		return IC_STATE_SEATKEEPER_NORIGHT_TO_USE;					// 자리지킴이 미보유, 골드패밀리 이상 사용가능
	}

	//사용중이 아닐때 

	//내가 예약중이다. 
	if(m_pGameProcess->GetPlayer(0)->ServPNum == m_nReservePNum)  
	{
		return IC_STATE_SEATKEEPER_READY_TO_CANCEL;					// 자리지킴이 예약중, 클릭시 예약취소(게임방내 1회)
	}

	if(m_nLeftCnt <= 0)
	{
		return IC_STATE_SEATKEEPER_OUT_OF_USE;						// 자리지킴이 0회 남음 일 사용 횟수 사용 완료	
	}

	// 다른 사람이 사용중 체크
	if ( m_bInUse )
	{		
		return IC_STATE_SEATKEEPER_REFUSE_TO_USE_FOR_MULTIPLE_USE;	// 자리지킴이 3회 남음, 게임방내 동시사용 불가
	}

	return IC_STATE_SEATKEEPER_READY_TO_USE;						// 자리지킴이 3회 남음, 클릭시 사용예약
}


void CSeatKeeper::AddChatMessage(char* pMsg, COLORREF color)
{
	if(m_pChatWin)
	{
		m_pChatWin->GetChatView()->AddText(pMsg, color);	//방내 채팅창 안내 메시지 함수 
	}	
}

//자리 지킴이
void CSeatKeeper::SetSeatKeeperState( IC_STATE_SEATKEEPER SeatKeeperState, int RemainCnt )
{
	//남은 횟수
	CString strText;

	switch(SeatKeeperState)
	{

	case IC_STATE_SEATKEEPER_READY_TO_USE:						// 자리지킴이 3회 남음: 클릭시 사용예약
		{			
			m_eSeatKeeperState = ESK_ON;
			strText.Format("%d", RemainCnt);
		}
		break;

	case IC_STATE_SEATKEEPER_REFUSE_TO_USE_FOR_NOT_PLAYING:		// 자리지킴이 3회 남음: 게임 진쟁중 예약 가능
		{			
			m_eSeatKeeperState = ESK_ON;
			strText.Format("%d", RemainCnt);
		} 
		break;

	case IC_STATE_SEATKEEPER_REFUSE_TO_USE_FOR_MULTIPLE_USE:	// 자리지킴이 3회 남음: 게임방내 동시사용 불가
		{
			m_eSeatKeeperState = ESK_ON;
			strText.Format("%d", RemainCnt);
		}
		break;

	case IC_STATE_SEATKEEPER_REFUSE_TO_USE_FOR_ONE_CANCEL_ONLY: // 자리지킴이 사용 예약중: 예약 취소 1회 가능
		{
			m_eSeatKeeperState = ESK_OFF;
		} 
		break;

	case IC_STATE_SEATKEEPER_READY_TO_CANCEL:					// 자리지킴이 예약중: 클릭시 예약취소(게임방내 1회)
		{
			m_eSeatKeeperState = ESK_OFF;
		} 
		break;	

	case IC_STATE_SEATKEEPER_OUT_OF_USE:						// 자리지킴이 0회 남음 일 사용 횟수 사용 완료
		{
			m_eSeatKeeperState = ESK_NULL;
		}
		break;

	case IC_STATE_SEATKEEPER_NORIGHT_TO_USE:					// 자리지킴이 미보유: 골드패밀리 이상 사용가능
		{
			m_eSeatKeeperState = ESK_NULL;
		} 
		break;

	case IC_STATE_SEATKEEPER_USING:
		{
			m_eSeatKeeperState = ESK_OFF;						// 사용중이다. 
		}
		break;

	case IC_STATE_SEATKEEPER_HIDE:
		{
			m_eSeatKeeperState = ESK_DISABLE;					// 아이콘숨김 
		}
		break;
	}

	if ( ESK_NULL == m_eSeatKeeperState )
	{
		m_Button[ SEATKEEPERBUTTON_USE ].Enable( false );
	}
	else
	{
		m_Button[ SEATKEEPERBUTTON_USE ].Enable( true );
	}
}