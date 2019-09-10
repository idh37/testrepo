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

//�޼��� �ؽ�Ʈ ���� 
#define COLOR_TEXTMSG		RGB(240, 0, 0)

#define __USE_BUTTON__			0
#define __CANCEL_BUTTON__		0

static CSeatKeeper* s_pThis;	// �ڽ��� ������

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
	
	// �ڽ��� ������
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
	// ���� ���μ����� ���´�.
	m_pGameProcess	= GM().GetCurrentProcess();
	m_pGameView		= GM().GetCurrentGameView();
	m_pUIInfoData	= pUIInfoData;	
	m_pChatWin		= pChatWin;	


	// ���� Ȯ���� �ּ���.
	if(!m_pUIInfoData || !m_pChatWin || !m_pGameProcess || !m_pGameView)
	{
		__asm int 3 
		return FALSE;
	}	
	
	
	// �ƹ�Ÿ�� ���������� �ڸ� ��Ŵ�� ������ �����ǥ
	const int nMaxPlayer = m_pGameProcess->GetMaxPlayer();
	for(int i = 0; i < nMaxPlayer; ++i)
	{
		CPoint ptTemp = m_pUIInfoData->m_listAvatarPos[i] + m_pUIInfoData->m_listSeatKeeperWinPos[i];
		m_listSeatKeeperWinPos.push_back(ptTemp);
	}			

	// �ڸ���Ŵ�� ��� ��ư
	m_Button[SEATKEEPERBUTTON_USE].Init(m_pGameView, &m_pGameView->Page, m_pUIInfoData->m_listButtonPos[EBT_SEATKEEPER].x, m_pUIInfoData->m_listButtonPos[EBT_SEATKEEPER].y, &m_cSeatKeeperBtnSpr, 0, IDM_BTN_USESEATKEEPER);

	// ���� ���� ��ư(�ڸ� ��Ŵ�� ������ ����)
	CPoint ptTemp = m_listSeatKeeperWinPos[0] + m_pUIInfoData->m_SeatKeeperJoinButPos;
	m_Button[SEATKEEPERBUTTON_BACKTOGAME].Init(m_pGameView, &m_pGameView->Page, ptTemp.x, ptTemp.y, &m_cSeatKeeperSpr, 0, IDM_BTN_BACKTOGAME);

	SetShowSeatKeeperBtn(true);

	return TRUE;
}

// ������ �ٲ𶧸��� ȣ���� �ؼ� this������ ���� ������ �ش�.
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

	if(strURL.Find("_���ſϷ�_") > 0)
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
			s_pThis->AddChatMessage("�ڸ���Ŵ�� ���� �ƹ�Ÿ ���Ű� �Ϸ�Ǿ����ϴ�.\n", COLOR_TEXTMSG);
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

	//��ư ���� ( ��� )
	m_Button[ SEATKEEPERBUTTON_USE ].SetSprite( &m_cSeatKeeperBtnSpr, __USE_BUTTON__ );

	m_Button[SEATKEEPERBUTTON_USE].Show(TRUE);
	m_Button[SEATKEEPERBUTTON_USE].Enable(TRUE);
}


void CSeatKeeper::OnStartGame()
{
	// ������� ī��Ʈ ���� 
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

	//���Ϸ�
	//���� �ڸ���Ŵ�� ����ȸ xȸ�� ��� ����ϼ̽��ϴ�.

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
				strMsg.Format("[%s]���� �ڸ���Ŵ�̸� �����߽��ϴ�.\n", pNick);

				AddChatMessage((char*)strMsg.operator LPCTSTR(), COLOR_TEXTMSG);

				if ( m_nReservePNum == m_pGameProcess->GetPlayer( 0 )->ServPNum )
				{
					//��ư ���� ( ��� )
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
					m_nLeftCnt = *pSeatKeeperMsg->LeftCnt;	//��Ŵ�� ����ڰ� ������ ��츸 ����

					if(m_nLeftCnt <= 0)
					{
						CString strMsg;
						strMsg.Format("���� �ڸ���Ŵ�� ����ȸ %dȸ�� ��� ����ϼ̽��ϴ�.\n", m_nMaxCnt);
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
					strMsg.Format("[%s]���� ���ӿ� �ٽ� �����ϼ̽��ϴ�.\n", pNick);	
				}
				else
				{
					strMsg.Format("[%s]���� �ڸ���Ŵ�� ������ ����Ͽ����ϴ�.\n", pNick);
				}
				AddChatMessage((char*)strMsg.operator LPCTSTR(), COLOR_TEXTMSG);

				if ( m_nReservePNum == m_pGameProcess->GetPlayer( 0 )->ServPNum )
				{
					//��ư ���� ( ��� )
					m_Button[ SEATKEEPERBUTTON_USE ].SetSprite( &m_cSeatKeeperBtnSpr, __USE_BUTTON__ );
				}
			}			

			Clear();			
		}
		break;

	case CSV_SEATKEEPER::SEATKEEPER_SIG_COUNTINFO:
		{
			//�� ���� 
			m_nLeftCnt = *pSeatKeeperMsg->LeftCnt;

			if(m_nLeftCnt <= 0)
			{
				CString strMsg;
				strMsg.Format("���� �ڸ���Ŵ�� ����ȸ %dȸ�� ��� ����ϼ̽��ϴ�.\n", m_nMaxCnt);
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

			// ���¸� �����Ѵ�.
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

// �������� ���ư��ٴ� ��Ŷ�� ������.
BOOL CSeatKeeper::BackToGame()
{	
	//�������� �ϱ�
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

	//������϶� ��� �Ұ� 
	if(m_bInUse)
		return FALSE; 

	//���� �������ΰ�?
	if(m_pGameProcess->GetPlayer(0)->ServPNum != m_nReservePNum) 
		return FALSE;

	if(m_nCancelHistory >= 1)
	{
		AddChatMessage("�ڸ���Ŵ�� ������ ������Ҵ� 1ȸ�� �����մϴ�.\n", COLOR_TEXTMSG); 
		SetSeatKeeperIcon(IC_STATE_SEATKEEPER_REFUSE_TO_USE_FOR_ONE_CANCEL_ONLY);
		return FALSE;	///������Ҵ� �ѹ��� ����
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
		//���� ������� ��. 
		AddChatMessage("�ڸ���Ŵ�� ���񽺸� ������Դϴ�.\n", COLOR_TEXTMSG);
		return FALSE;
	}

	const int nMaxPlayer = m_pGameProcess->GetMaxPlayer();
	if(m_bInUse || (m_nReservePNum >= 0 && m_nReservePNum < nMaxPlayer) )
	{
		AddChatMessage("���� ���ӹ濡�� �ִ� 1����� �ڸ���Ŵ�� ���񽺸� ����� �� �ֽ��ϴ�.\n", COLOR_TEXTMSG);
		SetSeatKeeperIcon(IC_STATE_SEATKEEPER_REFUSE_TO_USE_FOR_MULTIPLE_USE);
		return FALSE;
	}

	//���� ������̶�� ���� �ȵ�
	if(g_RI.State == 0 || m_pGameProcess->GetPlayer(0)->JoinState == 0)		
	{
		AddChatMessage("�ڸ���Ŵ�� ���񽺴� �������� ��\n���� ����� �����մϴ�.\n", COLOR_TEXTMSG); 
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

		//���� ������ΰ�?
		if(g_bMyObserver == FALSE && m_pGameProcess->GetPlayer(0)->ServPNum == m_nReservePNum)
		{
			//ū ǥ����
			Draw_BigPanel(pPage, pDC);	
		}
		else if(m_nReservePNum >= 0 && m_nReservePNum < nMaxPlayer)
		{
			//�� ������ ������̴�.
			int pnum = m_pGameProcess->GetPNum_ByServPN(m_nReservePNum);				

			if(pnum > 0 && pnum < nMaxPlayer)
			{
				/// ���� ǥ���� 
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

// �� �ڽ� 
void CSeatKeeper::Draw_BigPanel(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC)
{
	// 1. Back�̹���
#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
		pPage->PutSprAuto(m_listSeatKeeperWinPos[0].x, m_listSeatKeeperWinPos[0].y + 44, &m_cSeatKeeperSpr, 11);
	else
		pPage->PutSprAuto(m_listSeatKeeperWinPos[0].x, m_listSeatKeeperWinPos[0].y, &m_cSeatKeeperSpr, 10);
#else //UPGRADE_20120308_TOCTOC_CHANNELING
	pPage->PutSprAuto(m_listSeatKeeperWinPos[0].x, m_listSeatKeeperWinPos[0].y, &m_cSeatKeeperSpr, 10);
#endif //UPGRADE_20120308_TOCTOC_CHANNELING

	// 2. �� �̹���
	for(int i = 0; i < m_nTurnCnt; ++i)
	{		
		CPoint ptPoint = m_listSeatKeeperWinPos[0] + m_pUIInfoData->m_listTurnDisplayPos[i];

		// �ڱ��ڽ��� ǥ�ô� 44��ŭ�� ���� ����
		ptPoint.y += 44;

		pPage->PutSprAuto(ptPoint.x, ptPoint.y, &m_cSeatKeeperSpr, 4 + i);
	}

	int lefttrun = max(MAX_SEATKEEPER_TURN - m_nTurnCnt, 0);

	// 3. �����ϼ�
	CString str;
	str.Format("%d", lefttrun);		
	CPoint ptText = m_listSeatKeeperWinPos[0] + m_pUIInfoData->m_TurnTextPos;

	// �ڱ��ڽ��� ǥ�ô� 44��ŭ�� ���� ����
	ptText.y += 44;

	CRect rt;
	pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_BOLD)); //����, ������ 10, ����ü
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_pUIInfoData->m_clrSeatKeeperTurn);
	//pDC->TextOut(ptText.x, ptText.y, str);
	rt.SetRect(ptText.x, ptText.y, ptText.x + 11, ptText.y + 11); 
	pDC->DrawText(str, &rt, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
}

// ���� �ڸ���Ŵ��(���� ��)
void CSeatKeeper::Draw_SmallPanel(int nPlayerPos, NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC)
{
	// 1. Back�̹���
	pPage->PutSprAuto(m_listSeatKeeperWinPos[nPlayerPos].x, m_listSeatKeeperWinPos[nPlayerPos].y, &m_cSeatKeeperSpr, 9);

	// 2. �� �̹���
	for(int i = 0; i < m_nTurnCnt; ++i)
	{		
		CPoint ptPoint = m_listSeatKeeperWinPos[nPlayerPos] + m_pUIInfoData->m_listTurnDisplayPos[i];

		pPage->PutSprAuto(ptPoint.x, ptPoint.y, &m_cSeatKeeperSpr, 4 + i);
	}

	int lefttrun = max(MAX_SEATKEEPER_TURN - m_nTurnCnt, 0);

	// 3. �� ���� ���
	CString str;
	str.Format("%d", lefttrun);
	CPoint ptText = m_listSeatKeeperWinPos[nPlayerPos] + m_pUIInfoData->m_TurnTextPos;

	CRect rt;
	pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_BOLD)); //����, ������ 10, ����ü
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_pUIInfoData->m_clrSeatKeeperTurn);
	rt.SetRect(ptText.x, ptText.y, ptText.x + 11, ptText.y + 11); 
	pDC->DrawText(str, &rt, DT_VCENTER|DT_CENTER|DT_SINGLELINE);
}

BOOL CSeatKeeper::ShouldEnableButton(SEATKEEPERBUTTON Button)
{	
	//�ڸ���Ŵ�� ��� ��ư�� Ȱ��ȭ �Ǿ���ϴ� �����ΰ�? 
	if(Button == SEATKEEPERBUTTON_USE)	
	{	
		//�����̾� ����̻� ��� ��ư �׽� ����
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

	//���� ���� ��ư�� Ȱ��ȭ �Ǿ���ϴ� �����ΰ�? 
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


//������ ��ư ���¸� �����Ѵ�. 
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
	// ** ���� �߿� **//
	if ( g_bMyObserver )
	{
		return IC_STATE_SEATKEEPER_HIDE;							// �����ڴ� �ڸ���Ŵ�� Hide						
	} 

	// �����̾� ����̻� ��� ��ư �׽� ����
	if(AbleToUseSeatKeeper(&GM().GetMyInfo()->UI) == FALSE)
	{
		return IC_STATE_SEATKEEPER_NORIGHT_TO_USE;					// �ڸ���Ŵ�� �̺���, ����йи� �̻� ��밡��
	}

	//������� �ƴҶ� 

	//���� �������̴�. 
	if(m_pGameProcess->GetPlayer(0)->ServPNum == m_nReservePNum)  
	{
		return IC_STATE_SEATKEEPER_READY_TO_CANCEL;					// �ڸ���Ŵ�� ������, Ŭ���� �������(���ӹ泻 1ȸ)
	}

	if(m_nLeftCnt <= 0)
	{
		return IC_STATE_SEATKEEPER_OUT_OF_USE;						// �ڸ���Ŵ�� 0ȸ ���� �� ��� Ƚ�� ��� �Ϸ�	
	}

	// �ٸ� ����� ����� üũ
	if ( m_bInUse )
	{		
		return IC_STATE_SEATKEEPER_REFUSE_TO_USE_FOR_MULTIPLE_USE;	// �ڸ���Ŵ�� 3ȸ ����, ���ӹ泻 ���û�� �Ұ�
	}

	return IC_STATE_SEATKEEPER_READY_TO_USE;						// �ڸ���Ŵ�� 3ȸ ����, Ŭ���� ��뿹��
}


void CSeatKeeper::AddChatMessage(char* pMsg, COLORREF color)
{
	if(m_pChatWin)
	{
		m_pChatWin->GetChatView()->AddText(pMsg, color);	//�泻 ä��â �ȳ� �޽��� �Լ� 
	}	
}

//�ڸ� ��Ŵ��
void CSeatKeeper::SetSeatKeeperState( IC_STATE_SEATKEEPER SeatKeeperState, int RemainCnt )
{
	//���� Ƚ��
	CString strText;

	switch(SeatKeeperState)
	{

	case IC_STATE_SEATKEEPER_READY_TO_USE:						// �ڸ���Ŵ�� 3ȸ ����: Ŭ���� ��뿹��
		{			
			m_eSeatKeeperState = ESK_ON;
			strText.Format("%d", RemainCnt);
		}
		break;

	case IC_STATE_SEATKEEPER_REFUSE_TO_USE_FOR_NOT_PLAYING:		// �ڸ���Ŵ�� 3ȸ ����: ���� ������ ���� ����
		{			
			m_eSeatKeeperState = ESK_ON;
			strText.Format("%d", RemainCnt);
		} 
		break;

	case IC_STATE_SEATKEEPER_REFUSE_TO_USE_FOR_MULTIPLE_USE:	// �ڸ���Ŵ�� 3ȸ ����: ���ӹ泻 ���û�� �Ұ�
		{
			m_eSeatKeeperState = ESK_ON;
			strText.Format("%d", RemainCnt);
		}
		break;

	case IC_STATE_SEATKEEPER_REFUSE_TO_USE_FOR_ONE_CANCEL_ONLY: // �ڸ���Ŵ�� ��� ������: ���� ��� 1ȸ ����
		{
			m_eSeatKeeperState = ESK_OFF;
		} 
		break;

	case IC_STATE_SEATKEEPER_READY_TO_CANCEL:					// �ڸ���Ŵ�� ������: Ŭ���� �������(���ӹ泻 1ȸ)
		{
			m_eSeatKeeperState = ESK_OFF;
		} 
		break;	

	case IC_STATE_SEATKEEPER_OUT_OF_USE:						// �ڸ���Ŵ�� 0ȸ ���� �� ��� Ƚ�� ��� �Ϸ�
		{
			m_eSeatKeeperState = ESK_NULL;
		}
		break;

	case IC_STATE_SEATKEEPER_NORIGHT_TO_USE:					// �ڸ���Ŵ�� �̺���: ����йи� �̻� ��밡��
		{
			m_eSeatKeeperState = ESK_NULL;
		} 
		break;

	case IC_STATE_SEATKEEPER_USING:
		{
			m_eSeatKeeperState = ESK_OFF;						// ������̴�. 
		}
		break;

	case IC_STATE_SEATKEEPER_HIDE:
		{
			m_eSeatKeeperState = ESK_DISABLE;					// �����ܼ��� 
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