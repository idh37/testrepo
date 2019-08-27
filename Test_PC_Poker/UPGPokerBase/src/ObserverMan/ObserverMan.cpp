#include "stdafx.h"
#include "ObserverMan.h"
#include "CommMsgDef.h"
#include "../UIInfoData.h"
#include "../GameButtonMan/GameViewButtonMan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CObserverMan::CObserverMan()
{
	m_nMyReservePriority=0;
	m_nTotalReserve=0;
	m_pGameView = NULL;
	m_pPage = NULL;
	m_pUIInfoData = NULL;
	m_pButtonMan = NULL;
}

CObserverMan::~CObserverMan()
{
}

void CObserverMan::LoadImage()
{
	AddLoadImageList(&m_sprBack,".\\PokerCommon\\data\\ObserverBack.spr",true);
	AddLoadImageList(&m_sprReservationBtn,".\\PokerCommon\\data\\ObserverReservationBtn.spr",true); // 게임방 베팅영역 관전 참여예약
	AddLoadImageList(&m_sprCancelBtn,".\\PokerCommon\\data\\ObserverCancelBtn.spr",true); // 게임방 베팅영역 관전 예약취소
	AddLoadImageList(&m_sprNumber,".\\PokerCommon\\data\\ObserverNumber.spr",true);
	return;
}

void CObserverMan::SetMyObserver(BOOL bMyObserver)
{
	if(g_bMyObserver == bMyObserver) return;
	g_bMyObserver = bMyObserver;
	m_pButtonMan->m_btnSInOut.Enable(g_bMyObserver?FALSE:TRUE);
}

BOOL CObserverMan::IsMyObserver(void)
{
	return g_bMyObserver;
}

void CObserverMan::Init(CGameViewBase *pView, CUIInfoData *pUIInfoData, CGameViewButtonMan *pButtonMan)
{
	m_pGameView = pView;
	m_pUIInfoData = pUIInfoData;
	m_pPage = &m_pGameView->Page;
	m_pButtonMan = pButtonMan;

	CPoint ptPos = m_pUIInfoData->m_ptObserver +  m_pUIInfoData->m_ptObserverBtnGab;
	m_btnReserve.Init(m_pGameView, m_pPage, ptPos.x, ptPos.y, &m_sprReservationBtn, 0, IDM_OBSERVERUI);
}

void CObserverMan::OnObserve()
{
	m_nMyReservePriority=0;
	m_btnReserve.SetSprite(&m_sprReservationBtn, 0);
	INT64 nChip = 0;

	if(g_RI.ChipKind == 1)
	{
		nChip = GM().GetMyGold();
	}
	else
	{
		nChip = GM().GetMyInfo()->UI.GetRealPMoney();
	}
	
	if(g_RI.MoneyRange.IsInRange(nChip))
	{
		m_btnReserve.Show(TRUE);
	}
	else
	{
		m_btnReserve.Show(FALSE);
	}
}

void CObserverMan::Participation()	//관전->참여
{
	CPlayer *pPlayer = GM().GetMyInfo();
	switch(GDM().GetINT64(GDK_OBSERVER_STEP))
	{
	case 2:
		//관전 예약 중....
		//방장이면 예약을 할 수 없다. 관전 예약을 취소한다.
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_BETAREA_OBSERVE_RESERVE_CANCEL);
		#endif //UPGRADE_10210619_ADD_CLICKLOG

		if(strcmp(g_RI.ID, GM().GetMyInfo()->UI.ID) != 0) // 겜중이 아닐때
		{
			CSV_ASK_EMERAGE emerge;
			emerge.Set(g_RI.RoomNum,  pPlayer->UI.UNum, pPlayer->ServPNum, pPlayer->UI.ID);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), emerge.pData, emerge.GetTotalSize() );					
		}
		break;
	case 1:
		//서버에서 해당 상태를 보내주지 않음.... 어디에서 사용하는거지??
		{
			CSV_ASK_PARTICIPATION parti;
			parti.Set(g_RI.RoomNum,  pPlayer->UI.UNum, pPlayer->ServPNum, pPlayer->UI.ID, pPlayer->nSndFxKind);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), parti.pData, parti.GetTotalSize() );
		}
		break;
	case 0:
		//관전자 일때 참여 신청을 한다.
		if(g_bMyObserver)
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_BETAREA_OBSERVE_RESERVE);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			CSV_ASK_PARTICIPATION parti;
			parti.Set(g_RI.RoomNum,  pPlayer->UI.UNum, pPlayer->ServPNum, pPlayer->UI.ID, pPlayer->nSndFxKind);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), parti.pData, parti.GetTotalSize() );
		}
		break;
	}
			
}
void CObserverMan::Emerge()		//참여->관전
{
	CPlayer *pPlayer = GM().GetMyInfo();
	switch(GDM().GetINT64(GDK_OBSERVER_STEP))
	{
	case 1:
		//서버에서 해당 상태를 보내주지 않음.... 어디에서 사용하는거지??
		{				
			CSV_ASK_PARTICIPATION parti;
			parti.Set(g_RI.RoomNum,  pPlayer->UI.UNum, pPlayer->ServPNum, pPlayer->UI.ID, pPlayer->nSndFxKind);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), parti.pData, parti.GetTotalSize() );
		}
		break;
	case 0:
		//내가 관전자가 아니고, 방장이 아닐때
		if(g_bMyObserver == FALSE && strcmp(g_RI.ID, GM().GetMyInfo()->UI.ID) != 0)
		{
			CGameViewBase *pGameView = GM().GetCurrentGameView();
			//나가기 예약일 경우 나가기 예약을 취소한다.
			if(pGameView->m_bExitSubscript)
			{
				pGameView->SendMessage(WM_COMMAND, IDM_EXIT);
			}
			
			CSV_ASK_EMERAGE emerge;
			emerge.Set(g_RI.RoomNum,  pPlayer->UI.UNum, pPlayer->ServPNum, pPlayer->UI.ID);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), emerge.pData, emerge.GetTotalSize() );
			return;
		}

		//방장일 때
		if(strcmp(g_RI.ID, GM().GetMyInfo()->UI.ID) == 0) 
		{
			CString str;
			str.Format("방장은 관전으로 이동할 수 없습니다\n");
			AddGameInfoViewEdit(str, m_pUIInfoData->m_listChatColor[ECC2_NOTIFY]);
		}
		break;
	}
}

void CObserverMan::Draw(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC)
{
	if (g_bMyObserver == FALSE) return;

	CFont *pOldFont = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12_BOLD));

	int sprno_panel = -1;
	DWORD dwCurTime = timeGetTime();
	//m_nMyReservePriority = 18;
	if (m_nMyReservePriority>0) 
	{
		//내가 참여예약이 된 상태이다.
		pPage->DrawMultiScene(m_pUIInfoData->m_ptObserver.x, m_pUIInfoData->m_ptObserver.y, &m_sprBack, 1, dwCurTime);	
		
		CPoint ptPos = m_pUIInfoData->m_ptObserver + m_pUIInfoData->m_ptObserverReservePriorityGab;

		int nNumber = m_nMyReservePriority/10;
		if(nNumber > 0)
		{
			pPage->PutSprAuto(ptPos.x, ptPos.y, &m_sprNumber, nNumber, AB);
		}

		ptPos.x += m_sprNumber.spr[0].xl;
		nNumber = m_nMyReservePriority%10;
		pPage->PutSprAuto(ptPos.x, ptPos.y, &m_sprNumber, nNumber, AB);
	}
	else
	{
		//내가 참여예약이 된 상태가 아니다.
		pPage->DrawMultiScene(m_pUIInfoData->m_ptObserver.x, m_pUIInfoData->m_ptObserver.y, &m_sprBack, 0, dwCurTime);	
	}


	// 방 정보..
	bool bIsMaxRoom = g_RI.bIsMaxRoom;
	int nChipKind = g_RI.ChipKind; // 0 : 머니, 1 : 골드
	int NowUserNum = g_RI.NowUserNum; // 현재 참여인원
	int MaxUserNum = g_RI.MaxUserNum; // 최대 참여인원
	INT64 nSeedMoney = g_RI.CurSeedMoney;

	// 참여 예약 버튼 보여주기 여부
	bool bBtnShow = true;
	
	/*
#ifdef _DEBUG
	TRACE("\n=======================================================\n");
	TRACE("m_nTotalReserve : %d, m_nMyReservePriority : %d\n",
		m_nTotalReserve, m_nMyReservePriority);
	TRACE("bIsMaxRoom : %d, ChipKind : %d, UserNum : %d/%d\n",
		bIsMaxRoom, nChipKind, NowUserNum, MaxUserNum);
	TRACE("=======================================================\n\n");
#endif
	*/

	// 골드방 이고 시드머니가 1000, 3000, 5000 중에 하나 이며 방에 빈자리가 없을때 참여예약 버튼 감춤
	if (nChipKind && (nSeedMoney == 1000 || nSeedMoney == 3000 || nSeedMoney == 5000) && (NowUserNum >= MaxUserNum))
		bBtnShow = false;
	
	if (bBtnShow)
	{
		m_btnReserve.Enable(true);
		m_btnReserve.Draw(pDC);
	}
	else
	{
		m_btnReserve.Enable(false);
	}

	pDC->SelectObject(pOldFont);
}

BOOL CObserverMan::OnLButtonDown(int x, int y)
{
	if (g_bMyObserver == FALSE) return FALSE;

	return m_btnReserve.OnLButtonDown(x,y);
}

BOOL CObserverMan::OnLButtonUp(int x, int y)
{
	if (g_bMyObserver == FALSE) return FALSE;

	return m_btnReserve.OnLButtonUp(x,y);
}

BOOL CObserverMan::OnMouseMove(int x, int y)
{
	if (g_bMyObserver == FALSE) return FALSE;

	if(m_btnReserve.OnMouseMove(x,y))
	{
		SetCursor(GM().GetCurHandCursor());
		return TRUE;
	}
	return FALSE;
}

//관전중 오는 메시지
void CObserverMan::OnReceiveReservationInfo(CSV_RESERVATIONINFO *pMsg)
{		
	m_nTotalReserve			= *pMsg->TotNum;	//참여대기 숫자 
	m_nMyReservePriority	= 0;				//나의 예약 우선순위?

	for(int i=0; i < *pMsg->TotNum ; i++)
	{
		if (strncmp(pMsg->ReservedInfo[i].ID, GM().GetMyInfo()->UI.ID, 15) == 0)
		{
			m_nMyReservePriority = pMsg->ReservedInfo[i].Seq + 1;
		}
	}

	if (m_nMyReservePriority > 0)
	{
		m_btnReserve.SetSprite(&m_sprCancelBtn, 0);		//참여 취소 버튼		
	}
	else
	{
		m_btnReserve.SetSprite(&m_sprReservationBtn, 0);		//참여 예약 버튼 
	}
}

void CObserverMan::OnEnterRoom()
{
	GDM().SetBool(GDK_OBSERVER_INFO_MSG, false);
}

BOOL CObserverMan::OnCommand( WPARAM wParam, LPARAM lParam )
{
	switch(wParam)
	{
	case IDM_PARTICIPATION: //겜참여
		{
			Participation();
		}
		break;
	case IDM_EMERGE:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_MYCARDAREA_OBSERVE);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			Emerge();
		} 
		break;
	case IDM_OBSERVERUI:	//관전UI
		{
			Participation();
		} 
		break;
	default: return FALSE;
	}

	return TRUE;
}