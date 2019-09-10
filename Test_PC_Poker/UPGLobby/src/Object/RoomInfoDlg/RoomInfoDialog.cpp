#include "StdAfx.h"
#include "RoomInfoDialog.h"
#include "RoomInfoUserListItem.h"
#include "../../WidgetID.h"

#define DEF_MAX_USERS			5

bool RoomInfoUserListUpdateFunction(LSY::CListItem *pItem, LPARAM lParam)
{
	CRoomInfoUserListItem *pListItem = (CRoomInfoUserListItem *)pItem;
	pListItem->UpdateData();
	return true;
}

CRoomInfoDialog::CRoomInfoDialog()
:	m_pGameName(NULL),
	m_pRoomNumber(NULL),
	m_pRoomName(NULL),
	m_pMinimumMoney(NULL),
	m_pGameMode(NULL),
	m_pSeedMoney(NULL),
	m_pGameBack(NULL),
	m_pTabUserType(NULL),
	m_pScrollBack(NULL),
	m_pUserList(NULL),
	m_pBtnEnterRoom(NULL),
	m_pBtnObserverEnter(NULL),
	m_pBtnClose(NULL),
	m_pTotalMoney(NULL),
	m_pImgVIP(NULL),
	m_pImgFullUser(NULL),
	m_pGameType(NULL),
	m_bPlayerList(true)
{
	m_listUser.resize(DEF_MAX_USERS);
}

CRoomInfoDialog::~CRoomInfoDialog()
{
}

void CRoomInfoDialog::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;

	bool bAddChild = false;
	switch(pObject->GetID())
	{
	case ID_LOBBY_TXT_ROOMINFO_GAME:				m_pGameName				= (LSY::CText *)pObject;		break;
	case ID_LOBBY_TXTNB_ROOMINFO_ROOMNUMBER:		m_pRoomNumber			= (LSY::CTextNumber *)pObject;	break;
	case ID_LOBBY_TEXT_ROOMINFO_ROOMNAME:			m_pRoomName				= (LSY::CText *)pObject;		break;
	case ID_LOBBY_TXT_ROOMINFO_MINMONEY:			m_pMinimumMoney			= (LSY::CText *)pObject;		break;
	case ID_LOBBY_TXT_ROOMINFO_GAMEMODE:			m_pGameMode				= (LSY::CText *)pObject;		break;
	case ID_LOBBY_TXTNB_ROOMINFO_SEEDMONEY:			m_pSeedMoney			= (LSY::CTextNumber *)pObject;	break;
	case ID_LOBBY_IMG_ROOMINFO_BACK:				m_pGameBack				= (LSY::CImage *)pObject;		break;
	case ID_LOBBY_TAB_ROOMINFO:						m_pTabUserType			= (LSY::CTabControl *)pObject;	break;
	case ID_LOBBY_IMG_ROOMINFO_SCROLL_BACK:			m_pScrollBack			= (LSY::CImage *)pObject;		break;
	case ID_LOBBY_LIST_ROOMINFO_PLAYUSER:			m_pUserList				= (LSY::CList *)pObject;		break;
	case ID_LOBBY_BTN_ROOMINFO_ENTER_ROOM:			m_pBtnEnterRoom			= (LSY::CButton *)pObject;		bAddChild = true; break;
	case ID_LOBBY_BTN_ROOMINFO_OBSERVER_ENTER:		m_pBtnObserverEnter		= (LSY::CButton *)pObject;		bAddChild = true; break;
	case ID_LOBBY_BTN_ROOMINFO_CLOSE:				m_pBtnClose				= (LSY::CButton *)pObject;		bAddChild = true; break;

	case ID_LOBBY_TXTNB_ROOMINFO_TOTALMONEY:		m_pTotalMoney			= (LSY::CTextNumber *)pObject;	break;
	case ID_LOBBY_IMG_ROOMINFO_USER0:
	case ID_LOBBY_IMG_ROOMINFO_USER1:
	case ID_LOBBY_IMG_ROOMINFO_USER2:
	case ID_LOBBY_IMG_ROOMINFO_USER3:
	case ID_LOBBY_IMG_ROOMINFO_USER4:
		m_listUser[pObject->GetID() - ID_LOBBY_IMG_ROOMINFO_USER0]			= (LSY::CImage *)pObject;
		break;
	case ID_LOBBY_IMG_ROOMINFO_VIP:					m_pImgVIP				= (LSY::CImage *)pObject;		break;
	case ID_LOBBY_IMG_ROOMINFO_FULLUSER:
		{
			m_pImgFullUser				= (LSY::CImage *)pObject;		
			break;
		}
	}

	if(bAddChild)
	{
		pObject->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CRoomInfoDialog::OnClickButton));
	}

	__super::OnCreateChild(pObject);
}

void CRoomInfoDialog::OnCreated(void)
{
	OnCreateChild(m_pGameBack->GetFirstChildFromID(ID_LOBBY_TXTNB_ROOMINFO_TOTALMONEY));
	OnCreateChild(m_pGameBack->GetFirstChildFromID(ID_LOBBY_IMG_ROOMINFO_USER0));
	OnCreateChild(m_pGameBack->GetFirstChildFromID(ID_LOBBY_IMG_ROOMINFO_USER1));
	OnCreateChild(m_pGameBack->GetFirstChildFromID(ID_LOBBY_IMG_ROOMINFO_USER2));
	OnCreateChild(m_pGameBack->GetFirstChildFromID(ID_LOBBY_IMG_ROOMINFO_USER3));
	OnCreateChild(m_pGameBack->GetFirstChildFromID(ID_LOBBY_IMG_ROOMINFO_USER4));
	OnCreateChild(m_pGameBack->GetFirstChildFromID(ID_LOBBY_IMG_ROOMINFO_VIP));
	OnCreateChild(m_pGameBack->GetFirstChildFromID(ID_LOBBY_IMG_ROOMINFO_FULLUSER));

	m_pUserList->SetUpdateFunction((LSY::CList::LISTITEMUPDATAFUNCTION)&RoomInfoUserListUpdateFunction);
	m_pUserList->AddHandler(LSY::EM_O_LISTITEM_DBCLICK, LSY::Fnt(this, &CRoomInfoDialog::OnListItemDbClick));

	m_pTabUserType->SetCurSelFromID(ID_LOBBY_RADIO_ROOMINFO_PLAYUSER);
	m_pTabUserType->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CRoomInfoDialog::OnTabSelChange));

	SetShow(false);

	__super::OnCreated();
}

LSY::CObjectBase *CRoomInfoDialog::NewObject(void)
{
	return new CRoomInfoDialog;
}

LRESULT CRoomInfoDialog::OnClickButton(LSY::CMessage *msg)
{
	LSY::CMO_MouseLClick *pMsg = (LSY::CMO_MouseLClick *)msg;
	LSY::CButton *pBtn = (LSY::CButton *)pMsg->GetObject();
	CWnd *pWnd = GetObjectMan()->GetWnd();
	switch(pBtn->GetID())
	{
	case ID_LOBBY_BTN_ROOMINFO_CLOSE:
		SetShow(false);
		RemoveTracking();
		break;
	case ID_LOBBY_BTN_ROOMINFO_ENTER_ROOM:

		//Ŭ���̾�Ʈ ���α� (���� / �������� ��ư)
		SendUILog_Basic(CLSTATS_UINO_POKER_LOBBY_ROOMINFO_JOIN);

		pWnd->SendMessage( ID_LOBBY_PROCESS_JOINROOM, (WPARAM)&m_stGameInfo.stSmallRoomInfo, FALSE);
		break;
	case ID_LOBBY_BTN_ROOMINFO_OBSERVER_ENTER:

		//Ŭ���̾�Ʈ ���α� (���� / ���� ��ư)
		SendUILog_Basic(CLSTATS_UINO_POKER_LOBBY_ROOMINFO_OBSERVE);

		pWnd->SendMessage( ID_LOBBY_PROCESS_JOINROOM, (WPARAM)&m_stGameInfo.stSmallRoomInfo, TRUE);
		break;
	}
	return TRUE;
}

LRESULT CRoomInfoDialog::OnTabSelChange(LSY::CMessage *msg)
{
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();
	LSY::CRadio *pRadio = m_pTabUserType->GetRadioFromIndex(nIndex);

	m_bPlayerList = (pRadio->GetID() == ID_LOBBY_RADIO_ROOMINFO_PLAYUSER)?true:false;

	NMBASE::SKINGDI::NMRESAM.PlayWav( "UI-SND_BUTTON_CLICK_C" );

	SetUserListType(m_bPlayerList);
	return TRUE;
}

LRESULT CRoomInfoDialog::OnListItemDbClick(LSY::CMessage *msg)
{
	LSY::CMO_ListItemDbClick *pMsg = (LSY::CMO_ListItemDbClick *)msg;
	LSY::CListItemData *pData = pMsg->GetItemData();

	SMALLROOMINFO *pUserInfo = (SMALLROOMINFO *)pData->GetItemData();
	CWnd *pWnd = GetObjectMan()->GetWnd();
	pWnd->SendMessage(ID_LOBBY_PROCESS_ASK_USERINFO, (WPARAM)pUserInfo);
	return TRUE;
}

void CRoomInfoDialog::ShowGameRoomInfo(GAMEINFO *pGI)
{
	memcpy(&m_stGameInfo, pGI, sizeof(GAMEINFO));
	SetShow(true);

	COLORREF gamecolor[MAX_GAME+5]=
	{	
		RGB(150,195,255),	//7��Ŀ							
		RGB(175,203,120),	//�ٵ���
		RGB(134,206,227),	//����Ŀ
		RGB(226,210,148),	//����Ŀ
		RGB(225,170,179),	//���̷ο�
		RGB(189,177,38),		//����
		RGB(255,255,255),		//�Ƕ� �� Ȯ�� �ʿ�

		0,0,0,0,0

		
	};

	CGame *pGame = GM().GetGame((IDX_GAME)pGI->stSmallRoomInfo.nGameIndex);

	if(pGame)
	{
		m_pGameName->SetTextColor(gamecolor[pGI->stSmallRoomInfo.nGameIndex]);
		m_pGameName->SetText(pGame->GetGameName().GetString());	
		/*
		//�ƽ���,buy in, �������ѹ濡�� [����ӴϹ�]����
		if ( pGI->stSmallRoomInfo.bIsMaxRoom )
		{
			//7��Ŀ�� ��� ���� ���� ����Ŀ�� ������ ��� ���� ���� ���ɼ� ���� 
			if ( pGI->stSmallRoomInfo.nGameIndex == 0 )
			{
				CString str = TEXT("");
				str.Format("%s [����ӴϹ�]", pGame->GetGameName().GetString());
				m_pGameName->SetText(str.GetString());	
			}
		}else{
			m_pGameName->SetText(pGame->GetGameName().GetString());	
		}
		*/
		

		m_pGameName->SetShow(true);
	}
	else
	{
		m_pGameName->SetShow(false);
	}

	m_pRoomNumber->SetNumber(pGI->stSmallRoomInfo.sRoomNum);
	m_pRoomName->SetText(pGI->stSmallRoomInfo.szTitle);
	

	CString strText;
	strText = g_GetRoomName(GM().GetCurrentGameType(), &pGI->stSmallRoomInfo);
	m_pMinimumMoney->SetText(strText.GetString());

	if (GM().GetCurrentGameType() == IDX_GAME_HA)
	{
		strText = g_GetFormName((IDX_GAME)pGI->stSmallRoomInfo.nGameIndex, pGI->stSmallRoomInfo.cFormKind);
		CString strRuleType;
		CString strMaxUserNum;
		CString strDisplayText;
		if ( pGI->stSmallRoomInfo.cRuleType == hoola::eRULE_TYPE_STOP)
		{
			strRuleType.Format(_T("����ON"));
		}
		else
		{
			strRuleType.Format(_T("����OFF"));
		}

		if (pGI->stSmallRoomInfo.cMaxUserNum == 5 )
		{
			strMaxUserNum.Format(_T("5��"));
		}
		else
		{
			strMaxUserNum.Format(_T("2��"));
		}
		strDisplayText.Format( _T("%s(%s/%s)"),strMaxUserNum.GetString(),strText.GetString(),strRuleType.GetString());
		m_pGameMode->SetText(strDisplayText.GetString());
	}
	else
	{
		strText = g_GetFormName((IDX_GAME)pGI->stSmallRoomInfo.nGameIndex, pGI->stSmallRoomInfo.cFormKind);
		m_pGameMode->SetText(strText.GetString());
	}
	strText = UPGGetAutoBettingRuleName( (IDX_GAME)pGI->stSmallRoomInfo.nGameIndex, pGI->stSmallRoomInfo.sAutoBetKind, (BETRULE_KIND)pGI->stSmallRoomInfo.sBetRuleKind );
	m_pSeedMoney->SetNumber(pGI->stSmallRoomInfo.llSeedMoney);
	if (GM().GetCurrentGameType() == IDX_GAME_HA)
	{
		m_pSeedMoney->SetFirstUnit("");
	}
	else if (GM().GetCurrentGameType() == IDX_GAME_SD)
	{
		//������ : ���÷� ��� ������� ���� // ������ ������.
		/*strText = "/" + GetBetMoneyMultiName(IDX_GAME_SD, pGI->stSmallRoomInfo.fBetMoneyMulti);*/
		strText.Format("%s/����", strChip_Name[pGI->stSmallRoomInfo.ChipKind]);
		m_pSeedMoney->SetFirstUnit(strText.GetString());
	}
	else
	{
		//������ : ���÷� ��� ī�弼������ ����

		strText.Format("%s/%s", strChip_Name[pGI->stSmallRoomInfo.ChipKind], GetCardSettingName(GM().GetCurrentGameType(), pGI->stSmallRoomInfo.cCardSetting));
		m_pSeedMoney->SetFirstUnit(strText.GetString());
	}
	

	bool bEnableJoin = pGI->stSmallRoomInfo.szClientSideVar[CS_JOINABLE];
	BOOL bEnablePlay = ( pGI->stSmallRoomInfo.cNowUserNum < pGI->stSmallRoomInfo.cMaxUserNum ) ;

	m_pImgFullUser->SetShow( false );

	// ������ �۾� ��ư ���� ����
	if (GM().IsJoinViewRoom(&pGI->stSmallRoomInfo))
	{
		if (!bEnablePlay)
		{
			m_pBtnEnterRoom->SetShow(false);
			m_pImgFullUser->SetShow( true );
		}
		else
		{
			m_pBtnEnterRoom->SetShow(true);
			bEnableJoin = true;
		}
		m_pBtnObserverEnter->SetShow(true);
	}
	else{
		m_pBtnEnterRoom->SetShow(false);
		m_pBtnObserverEnter->SetShow(false);
	}

	std::vector<LSY::CImage *> *plistTemp = NULL;

	if(bEnableJoin) m_pGameBack->SetIndex(39 + pGI->stSmallRoomInfo.nGameIndex);
	else m_pGameBack->SetIndex(47);

	m_pGameBack->SetShow(true);
	m_pTotalMoney->SetNumber(pGI->stSmallRoomInfo.llTotalPMoneySum);
	m_pTotalMoney->SetAddText(strChip_Name[pGI->stSmallRoomInfo.ChipKind]);

	m_pImgVIP->SetShow(IsVIPRoom(&pGI->stSmallRoomInfo)?true:false);

	plistTemp = &m_listUser;

	int nMaxPlayer = DEF_MAX_USERS;
	if (pGame)	nMaxPlayer = pGame->GetMaxPlayer();

	for(int i=0; i<nMaxPlayer; i++)
	{ 
		(*plistTemp)[i]->SetShow(false);
		if (pGI->stSmallUserInfo[i].ID[0]==0)
		{
			continue;
		}

		if (pGI->stSmallUserInfo[i].Sex==0 || pGI->stSmallUserInfo[i].Sex==1)
		{
			(*plistTemp)[i]->SetShow(true);
			
			int nIndex = 37;
			if (bEnableJoin) nIndex = pGI->stSmallUserInfo[i].Sex?36:35;

			(*plistTemp)[i]->SetIndex(nIndex);
		}
	}

	SetUserListType(m_bPlayerList);

	SetTracking(true);
}

bool CRoomInfoDialog::OnClickOutside(CObjectBase *pFocus)
{
	__super::OnClickOutside(pFocus);
	if(pFocus)
	{
		LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
		LSY::CList *pList1 = (LSY::CList *)pProject->GetObject(ID_LOBBY_LIST_ROOM);
		LSY::CList *pList2 = (LSY::CList *)pProject->GetObject(ID_LOBBY_LIST_ROOM_LOWBD);

		if(pFocus->IsParent(pList1) || pFocus->IsParent(pList2) )
		{
			return false;
		}
	}
	
	SetShow(false);
	return true;
}

void CRoomInfoDialog::SetUserListType(bool bPlayUser)
{
	m_pUserList->DeleteAllItem();
	LSY::CScrollBar *pScroll = m_pUserList->GetScrollBar();

	if(bPlayUser)
	{
		for(int i=0; i<MAX_PLAYER; i++)
		{			
			if (m_stGameInfo.stSmallUserInfo[i].ID[0]==0)
				continue;

			m_pUserList->AddItem((LPARAM)&m_stGameInfo.stSmallUserInfo[i]);
		}

		m_pScrollBack->SetShow(false);
		pScroll->SetShow(false);
	}
	else
	{
		// �����ڰ� �������� �����ϸ� �迭�� ���� �������� ��ġ�ϰ� �ش� ������
		// ��Ͽ� ǥ������ �ʰ� �־��µ� AdminGrade�� 0�� �ƴϸ� ǥ������ �ʰ� ����. 2018.8 ������
		for(int i=MAX_PLAYER; i<MAX_NEWPLAYER; i++)
		{
			if (m_stGameInfo.stSmallUserInfo[i].ID[0]==0 || m_stGameInfo.stSmallUserInfo[i].AdminGrade > 0)
				continue;
			m_pUserList->AddItem((LPARAM)&m_stGameInfo.stSmallUserInfo[i]);		
		}

		m_pScrollBack->SetShow(true);
		pScroll->SetShow(true);
	}	
}