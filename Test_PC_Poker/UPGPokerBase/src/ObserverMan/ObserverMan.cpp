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
	AddLoadImageList(&m_sprReservationBtn,".\\PokerCommon\\data\\ObserverReservationBtn.spr",true); // ���ӹ� ���ÿ��� ���� ��������
	AddLoadImageList(&m_sprCancelBtn,".\\PokerCommon\\data\\ObserverCancelBtn.spr",true); // ���ӹ� ���ÿ��� ���� �������
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

void CObserverMan::Participation()	//����->����
{
	CPlayer *pPlayer = GM().GetMyInfo();
	switch(GDM().GetINT64(GDK_OBSERVER_STEP))
	{
	case 2:
		//���� ���� ��....
		//�����̸� ������ �� �� ����. ���� ������ ����Ѵ�.
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_BETAREA_OBSERVE_RESERVE_CANCEL);
		#endif //UPGRADE_10210619_ADD_CLICKLOG

		if(strcmp(g_RI.ID, GM().GetMyInfo()->UI.ID) != 0) // ������ �ƴҶ�
		{
			CSV_ASK_EMERAGE emerge;
			emerge.Set(g_RI.RoomNum,  pPlayer->UI.UNum, pPlayer->ServPNum, pPlayer->UI.ID);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), emerge.pData, emerge.GetTotalSize() );					
		}
		break;
	case 1:
		//�������� �ش� ���¸� �������� ����.... ��𿡼� ����ϴ°���??
		{
			CSV_ASK_PARTICIPATION parti;
			parti.Set(g_RI.RoomNum,  pPlayer->UI.UNum, pPlayer->ServPNum, pPlayer->UI.ID, pPlayer->nSndFxKind);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), parti.pData, parti.GetTotalSize() );
		}
		break;
	case 0:
		//������ �϶� ���� ��û�� �Ѵ�.
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
void CObserverMan::Emerge()		//����->����
{
	CPlayer *pPlayer = GM().GetMyInfo();
	switch(GDM().GetINT64(GDK_OBSERVER_STEP))
	{
	case 1:
		//�������� �ش� ���¸� �������� ����.... ��𿡼� ����ϴ°���??
		{				
			CSV_ASK_PARTICIPATION parti;
			parti.Set(g_RI.RoomNum,  pPlayer->UI.UNum, pPlayer->ServPNum, pPlayer->UI.ID, pPlayer->nSndFxKind);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), parti.pData, parti.GetTotalSize() );
		}
		break;
	case 0:
		//���� �����ڰ� �ƴϰ�, ������ �ƴҶ�
		if(g_bMyObserver == FALSE && strcmp(g_RI.ID, GM().GetMyInfo()->UI.ID) != 0)
		{
			CGameViewBase *pGameView = GM().GetCurrentGameView();
			//������ ������ ��� ������ ������ ����Ѵ�.
			if(pGameView->m_bExitSubscript)
			{
				pGameView->SendMessage(WM_COMMAND, IDM_EXIT);
			}
			
			CSV_ASK_EMERAGE emerge;
			emerge.Set(g_RI.RoomNum,  pPlayer->UI.UNum, pPlayer->ServPNum, pPlayer->UI.ID);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), emerge.pData, emerge.GetTotalSize() );
			return;
		}

		//������ ��
		if(strcmp(g_RI.ID, GM().GetMyInfo()->UI.ID) == 0) 
		{
			CString str;
			str.Format("������ �������� �̵��� �� �����ϴ�\n");
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
		//���� ���������� �� �����̴�.
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
		//���� ���������� �� ���°� �ƴϴ�.
		pPage->DrawMultiScene(m_pUIInfoData->m_ptObserver.x, m_pUIInfoData->m_ptObserver.y, &m_sprBack, 0, dwCurTime);	
	}


	// �� ����..
	bool bIsMaxRoom = g_RI.bIsMaxRoom;
	int nChipKind = g_RI.ChipKind; // 0 : �Ӵ�, 1 : ���
	int NowUserNum = g_RI.NowUserNum; // ���� �����ο�
	int MaxUserNum = g_RI.MaxUserNum; // �ִ� �����ο�
	INT64 nSeedMoney = g_RI.CurSeedMoney;

	// ���� ���� ��ư �����ֱ� ����
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

	// ���� �̰� �õ�Ӵϰ� 1000, 3000, 5000 �߿� �ϳ� �̸� �濡 ���ڸ��� ������ �������� ��ư ����
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

//������ ���� �޽���
void CObserverMan::OnReceiveReservationInfo(CSV_RESERVATIONINFO *pMsg)
{		
	m_nTotalReserve			= *pMsg->TotNum;	//������� ���� 
	m_nMyReservePriority	= 0;				//���� ���� �켱����?

	for(int i=0; i < *pMsg->TotNum ; i++)
	{
		if (strncmp(pMsg->ReservedInfo[i].ID, GM().GetMyInfo()->UI.ID, 15) == 0)
		{
			m_nMyReservePriority = pMsg->ReservedInfo[i].Seq + 1;
		}
	}

	if (m_nMyReservePriority > 0)
	{
		m_btnReserve.SetSprite(&m_sprCancelBtn, 0);		//���� ��� ��ư		
	}
	else
	{
		m_btnReserve.SetSprite(&m_sprReservationBtn, 0);		//���� ���� ��ư 
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
	case IDM_PARTICIPATION: //������
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
	case IDM_OBSERVERUI:	//����UI
		{
			Participation();
		} 
		break;
	default: return FALSE;
	}

	return TRUE;
}