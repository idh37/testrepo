#include "StdAfx.h"
#include "RoomListItemLowBDJoin.h"
#include "../../WidgetID.h"
#include "./BugSplat/BugSplatWrapper.h"



#define DEF_MAX_USERS 5

CRoomListItemLowBDJoin::CRoomListItemLowBDJoin()
:	m_pRoomNumber(NULL),
	m_pMinmumMoney(NULL),
	m_pTitle(NULL),
	m_pSeedMoney(NULL),
	m_pRule(NULL),
	m_pPlayerCnt(NULL),
//	m_pTatalMoney(NULL),
	m_pImg_Limit(NULL),
	m_pImg_SeedMoney(NULL),
	m_pImg_FullUser(NULL),
	m_pBtnEnterRoom(NULL),
	m_pBtnObserberRoom(NULL),
	m_pTitle_PrevRoom(NULL),
	m_pTitle_PrevRoomTitle(NULL)
{
// 	m_listUserCount.resize(DEF_MAX_USERS);
// 	for(int i=0;i<DEF_MAX_USERS;++i)
// 	{
// 		m_listUserCount[i] = NULL;
// 	}
}

CRoomListItemLowBDJoin::~CRoomListItemLowBDJoin()
{
}

void CRoomListItemLowBDJoin::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;

	switch(pObject->GetID())
	{
	case ID_LOBBY_TXTNB_ROOMLISTITEM_NO_LOWBD_JOIN:			m_pRoomNumber			= (LSY::CTextNumber *)pObject;	break;
	case ID_LOBBY_TXT_ROOMLISTITEM_GAMESTYLE_LOWBD_JOIN:	m_pGameStyle			= (LSY::CText *)pObject;		break;		
	case ID_LOBBY_TXT_ROOMLISTITEM_MINMONEY_LOWBD_JOIN:		m_pMinmumMoney			= (LSY::CText *)pObject;		break;
	case ID_LOBBY_TXT_ROOMLISTITEM_TITLE_LOWBD_JOIN:          m_pTitle				= (LSY::CText *)pObject;		break;
	case ID_LOBBY_TXTNB_ROOMLISTITEM_SEEDMONEY_LOWBD_JOIN:	m_pSeedMoney			= (LSY::CTextNumber *)pObject;	break;
	case ID_LOBBY_TXT_ROOMLISTITEM_RULE_LOWBD_JOIN:			m_pRule					= (LSY::CText *)pObject;		break;
	//case ID_LOBBY_TXT_ROOMLISTITEM_GAMEMODE_LOWBD_JOIN:		m_pGameMode				= (LSY::CText *)pObject;		break;
	case ID_LOBBY_TXTNB_ROOMLISTITEM_PLAYERCNT_LOWBD_JOIN:   m_pPlayerCnt			= (LSY::CTextNumber *)pObject;	break;
//	case ID_LOBBY_TXTNB_ROOMLISTITEM_TOTOALMONEY_LOWBD_JOIN:	m_pTatalMoney			= (LSY::CTextNumber *)pObject;	break;
	case ID_LOBBY_IMG_ROOMLISTITEM_GAMESTYLE_LOWBD_JOIN:		m_pImg_Gamestyle		= (LSY::CImage *)pObject;		break;
	case ID_LOBBY_IMG_ROOMLISTITEM_LIMIT_LOWBD_JOIN:				m_pImg_Limit				= (LSY::CImage *)pObject;		break;
	case ID_LOBBY_IMG_ROOMLISTITEM_SEEDMONEY_LOWBD_JOIN:		m_pImg_SeedMoney		= (LSY::CImage *)pObject;		break;
	case ID_LOBBY_IMG_ROOMLISTITEM_FULLUSER_LOWBD_JOIN:		m_pImg_FullUser			= (LSY::CImage *)pObject;		break;
	case ID_LOBBY_BTN_ROOMLISTITEM_ENTER_ROOM_LOWBD_JOIN:	m_pBtnEnterRoom			= (LSY::CButton *)pObject;		break;
	case ID_LOBBY_BTN_ROOMLISTITEM_OBSERVER_ROOM_LOWBD_JOIN:	m_pBtnObserberRoom		= (LSY::CButton *)pObject;		break;
	case ID_LOBBY_TXT_ROOMLISTITEM_PREVROOM_LOWBD_JOIN:		m_pTitle_PrevRoom		= (LSY::CText *)pObject;		break;
	case ID_LOBBY_TXT_ROOMLISTITEM_PREVTITLE_LOWBD_JOIN:		m_pTitle_PrevRoomTitle	= (LSY::CText *)pObject;		break;
// 	case ID_LOBBY_IMG_ROOMLISTITEM_USER0_LOWBD:
// 	case ID_LOBBY_IMG_ROOMLISTITEM_USER1_LOWBD:
// 	case ID_LOBBY_IMG_ROOMLISTITEM_USER2_LOWBD:
// 	case ID_LOBBY_IMG_ROOMLISTITEM_USER3_LOWBD:
// 	case ID_LOBBY_IMG_ROOMLISTITEM_USER4_LOWBD:
// 		m_listUserCount[pObject->GetID() - ID_LOBBY_IMG_ROOMLISTITEM_USER0_LOWBD] = (LSY::CImage *)pObject; break;
	}

	__super::OnCreateChild(pObject);
}

void CRoomListItemLowBDJoin::OnCreated(void)
{
	//LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
	__super::OnCreated();
}

LSY::CObjectBase *CRoomListItemLowBDJoin::NewObject(void)
{
	return new CRoomListItemLowBDJoin;
}

void CRoomListItemLowBDJoin::UpdateData(const LPARAM &lParam)
{
	LSY::CListItemData *pListItemData = (LSY::CListItemData*)GetData();
	if (pListItemData==NULL)
	{
		return;
	}

	SMALLROOMINFO *pRoomInfo = (SMALLROOMINFO *)pListItemData->GetItemData();
	if(pRoomInfo == NULL) return;

	bool bMouseOver = IsMouseOver();

	BOOL bEnablePlay = ( pRoomInfo->cNowUserNum < pRoomInfo->cMaxUserNum ) ;
	BOOL bJoinable =(BOOL) pRoomInfo->szClientSideVar[CS_JOINABLE];
	bool bPrevJoinedRoom = (bool) (pRoomInfo->szClientSideVar[CS_PREV_ROOM]!=0);	//������
	bool bSelectedRoom = (bool) (pRoomInfo->szClientSideVar[CS_SELECTED_ROOM]!=0);  //���ù�
	bool bJoinView = GM().IsJoinViewRoom(pRoomInfo);

	COLORREF clrText = RGB(0,0,0);
	COLORREF clrText_fixed = RGB(0,0,0);
	COLORREF clrText_friend = RGB(41,99,152);

	//������ �߰� ���߿� �����̹� ��� �϶� ���� �ʿ�.

	m_pImg_FullUser->SetShow( false );

	// ������ ����
	//���� ����
	if (bJoinable && bEnablePlay)
	{
		if( bMouseOver )
		{
			clrText = RGB( 42, 72, 128 );
			clrText_fixed = RGB( 42, 72, 128 );
		}
		else
		{
			clrText  = RGB( 43, 50, 58 );
			clrText_fixed = RGB( 43, 50, 58 );
		}

		SetBarType( 3 );
		m_pBtnEnterRoom->SetShow(bJoinView);
	}
	//Full
	else if (bJoinable)
	{
		m_pImg_FullUser->SetShow( true );

		if( bMouseOver )
		{
			clrText = RGB( 59, 59, 59 );
			clrText_fixed = RGB( 59, 59, 59 );

			m_pImg_FullUser->SetIndex( 8 );
		}
		else
		{
			clrText = RGB( 85, 88, 93 );
			clrText_fixed = RGB( 85, 88, 93 );

			m_pImg_FullUser->SetIndex( 7 );
		}

		SetBarType( 1 );
		m_pBtnEnterRoom->SetShow(false);
	}
	//�����Ұ�
	else
	{
		if( bMouseOver )
		{
			clrText = RGB( 59, 59, 59 );
			clrText_fixed = RGB( 59, 59, 59 );
		}
		else
		{
			clrText = RGB( 85, 88, 93 );
			clrText_fixed = RGB( 85, 88, 93 );
		}

		SetBarType( 1 );
		m_pBtnEnterRoom->SetShow(false);
	}

	//if (bSelectedRoom)
	//{
	//	m_pTitle_PrevRoom->SetText(_T("[���ù�]"));				
	//}
	////�������� ����� ���
	//else if (bPrevJoinedRoom)
	//{
	//	clrText  = RGB( 43, 50, 58 );
	//	clrText_fixed = RGB( 43, 50, 58 );
	//	SetBarType( 25 ); //����� Bar

	//	bJoinable = TRUE; 		//������ RoomListItem�� Look�� bJoinable�� FALSE�� TRUEó�� ���̰��Ѵ�.
	//	m_pTitle_PrevRoom->SetText(_T("[������]"));		
	//}

	// ������ ����
	////���̹��ϰ�� ���� ��ư ��Ȱ��ȭ
	m_pBtnObserberRoom->SetShow(!IsDummyRoomKey(pRoomInfo->llRoomKey)&&bJoinView);

	if (bPrevJoinedRoom)
	{
		clrText  = RGB( 43, 50, 58 );
		clrText_fixed = RGB( 43, 50, 58 );

		bJoinable = TRUE; 		//������ RoomListItem�� Look�� bJoinable�� FALSE�� TRUEó�� ���̰��Ѵ�.
		m_pTitle_PrevRoom->SetText(_T("[������]"));
	}

	//���ȣ
	m_pRoomNumber->SetTextColor( clrText_fixed );
	m_pRoomNumber->SetNumber(pRoomInfo->sRoomNum);

	//�������ѸӴ�
	clrText = RGB( 130, 108, 0 );

	if( bJoinable && bEnablePlay ) m_pMinmumMoney->SetTextColor( clrText );
	else m_pMinmumMoney->SetTextColor( clrText_fixed );

	CString strText = g_GetRoomName(GM().GetCurrentGameType(), pRoomInfo );
	m_pMinmumMoney->SetText( strText.GetString() );

	clrText = RGB( 0, 0, 0 );
	//������
	if( bJoinable && bEnablePlay ) m_pTitle->SetTextColor( clrText );
	else m_pTitle->SetTextColor( clrText_fixed );

	strText.Format("%s", pRoomInfo->szTitle);
	m_pTitle->SetText( strText.GetString() );
	m_pTitle_PrevRoomTitle->SetText( strText.GetString() );	//������ Ÿ��Ʋ


	bool bShowRoomInfoLabel = false;
	if (bPrevJoinedRoom || bSelectedRoom)
	{
		bShowRoomInfoLabel = true;
	}

	//������(���ù�) ǥ�ÿ���	
	m_pTitle_PrevRoomTitle->SetShow(bShowRoomInfoLabel);	//�������ϰ�� ������������ �����ش�.
	m_pTitle_PrevRoom->SetShow(bShowRoomInfoLabel);		//�������ϰ�� [������] ǥ�ø� �����ش�.
	m_pTitle->SetShow(!bShowRoomInfoLabel);				//�������ϰ�� �Ϲݹ������� �����.

	m_pImg_Limit->SetShow(false);
	// �õ�Ӵ� and ���÷�
	COLORREF colorOutLine = RGB( 1, 1, 1 );

	if( bJoinable && bEnablePlay )
	{
		m_pRule->SetTextDrawType( LSY::ETDT_OUTLINE );
		m_pSeedMoney->SetTextDrawType( LSY::ETDT_OUTLINE );

		m_pImg_Gamestyle->SetShow(true) ;
		m_pGameStyle->SetTextColor( RGB(76,152,152));

		if( pRoomInfo->sAutoBetKind > 0 ) 
		{
			m_pImg_SeedMoney->SetIndex( 12 );

			colorOutLine  = RGB( 89, 125, 64 );
			m_pSeedMoney->SetShadowColor( colorOutLine );
			m_pRule->SetShadowColor( colorOutLine );
		}
		else
		{
			m_pImg_SeedMoney->SetIndex( 11 );

			colorOutLine  = RGB( 72, 112, 150 );
			m_pSeedMoney->SetShadowColor( colorOutLine );
			m_pRule->SetShadowColor( colorOutLine );
		}

		if( pRoomInfo->ChipKind == 1 && pRoomInfo->llEnterLimitMoney == 0)
		{
			m_pImg_Limit->SetShow(true);
			m_pImg_Limit->SetIndex(60);
			m_pMinmumMoney->SetTextColor( RGB( 212, 151, 11 ) );
		}
		else if( pRoomInfo->bIsFriendRoom )
		{
			m_pImg_Limit->SetShow(true);
			m_pImg_Limit->SetIndex(64);
			m_pMinmumMoney->SetTextColor( clrText_friend );
		}

		clrText = RGB( 255, 255, 255 );	
		m_pSeedMoney->SetTextColor( clrText );
		m_pRule->SetTextColor( clrText );
	}
	else
	{
		m_pRule->SetTextDrawType( LSY::ETDT_NORMAL );
		m_pSeedMoney->SetTextDrawType( LSY::ETDT_NORMAL );

		m_pImg_Gamestyle->SetShow(false) ;

		if( bMouseOver )
		{
			m_pImg_SeedMoney->SetIndex( 10 );

			clrText = RGB( 59, 59, 59 );
			m_pSeedMoney->SetTextColor( clrText );
			m_pRule->SetTextColor( clrText );

			colorOutLine  = RGB( 59, 59, 59 );
			m_pSeedMoney->SetShadowColor( colorOutLine );
			m_pRule->SetShadowColor( colorOutLine );
		}
		else
		{
			m_pImg_SeedMoney->SetIndex( 9 );

			clrText = RGB( 85, 88, 93 );
			m_pSeedMoney->SetTextColor( clrText );
			m_pRule->SetTextColor( clrText );

			colorOutLine  = RGB( 85, 88, 93 );
			m_pSeedMoney->SetShadowColor( colorOutLine );
			m_pRule->SetShadowColor( colorOutLine );
		}

		m_pGameStyle->SetTextColor(clrText);
	}
	if(1==pRoomInfo->cMemberRoom){
		m_pImg_Gamestyle->SetIndex(60);
		m_pGameStyle->SetText("���̾�") ;
		m_pGameStyle->SetTextColor(RGB(154, 80, 186));
	}
	else if(5==pRoomInfo->cFormKind)
	{
		m_pImg_Gamestyle->SetIndex(62);
		m_pGameStyle->SetText("����ε�");
		m_pGameStyle->SetTextColor(RGB(143, 136, 214));
	}
	else{
		m_pImg_Gamestyle->SetIndex(58);
		m_pGameStyle->SetText("�Ϲ�") ;

	}

	m_pSeedMoney->SetNumber( pRoomInfo->llSeedMoney );
	m_pSeedMoney->SetAddText(strChip_Count[pRoomInfo->ChipKind]);

	// ������ �� �������� ���θ� ����.
	/*strText.Format("%s", UPGGetAutoBettingRuleName((IDX_GAME)pRoomInfo->nGameIndex, pRoomInfo->sAutoBetKind, (BETRULE_KIND)pRoomInfo->sBetRuleKind ) );
	m_pRule->SetText( strText.GetString() );*/

	if(pRoomInfo->cCardSetting == 0 ) // 0�̸� �⺻.
	{
		// ���� ���϶� �⺻���� ǥ���ϴ� �κ� �������� ����.
		/*strText.Format("�⺻");*/
		strText.Format("����");
	}else if( pRoomInfo->cCardSetting == 1) // 1�̸� ��ħ
	{
		strText.Format("��ħ");
	}
	
	m_pRule->SetText( strText.GetString() );

	//�ο���
	//strText.Format( "%d/%d", m_RoomInfo.cNowUserNum, m_RoomInfo.cMaxUserNum );
	m_pPlayerCnt->SetTextColor(clrText_fixed);
	m_pPlayerCnt->SetNumber(pRoomInfo->cNowUserNum);
	strText.Format( "/%d", pRoomInfo->cMaxUserNum );
	m_pPlayerCnt->SetFirstUnit(strText.GetString());
/*

	m_pSeedMoney->SetTextColor(clrText);
	m_pSeedMoney->SetNumber(pRoomInfo->llSeedMoney);
	strText.Format("/%s",UPGGetAutoBettingRuleName((IDX_GAME)pRoomInfo->nGameIndex, pRoomInfo->sAutoBetKind));
	m_pSeedMoney->SetFirstUnit(strText.GetString());

	CString strFormName = g_GetFormName((IDX_GAME)pRoomInfo->nGameIndex, pRoomInfo->cFormKind);
	if (strFormName.IsEmpty())
		strText = pRoomInfo->szTitle;
	else
		strText.Format("%s / %s", strFormName, pRoomInfo->szTitle);

// 	m_pGameMode->SetTextColor(clrText);
// 	m_pGameMode->SetText(strText.GetString());

	m_pPlayerCnt->SetTextColor(clrText);
	m_pPlayerCnt->SetNumber(pRoomInfo->cNowUserNum);
	strText.Format( "/%d", pRoomInfo->cMaxUserNum );
	m_pPlayerCnt->SetFirstUnit(strText.GetString());
//	m_pTatalMoney->SetTextColor(clrText);
//	m_pTatalMoney->SetNumber(pRoomInfo->llTotalPMoneySum);

	//�ο�����
	//SetUserCount(pRoomInfo->cNowUserNum, pRoomInfo->szClientSideVar[CS_JOINABLE] != 0);
	*/
}

void CRoomListItemLowBDJoin::SetBarType(int nStartIndex)
{
	int	listStatusIndex[LSY::EBS_MAX];
	listStatusIndex[LSY::EBS_NORMAL]	= nStartIndex;
	listStatusIndex[LSY::EBS_OVER]		= nStartIndex+1;
	listStatusIndex[LSY::EBS_DOWN]		= nStartIndex+1;
	listStatusIndex[LSY::EBS_DISABLE]	= nStartIndex;
	listStatusIndex[LSY::EBS_SNORMAL]	= nStartIndex;
	listStatusIndex[LSY::EBS_SOVER]		= nStartIndex+1;
	listStatusIndex[LSY::EBS_SDOWN]		= nStartIndex+1;
	listStatusIndex[LSY::EBS_SDISABLE]	= nStartIndex;
	SetFreeStatusButton(listStatusIndex);
}

void CRoomListItemLowBDJoin::OnMouseOver(void)
{
	__super::OnMouseOver();
	UpdateData(0);
}

void CRoomListItemLowBDJoin::OnMouseLeave(void)
{
	__super::OnMouseLeave();
	UpdateData(0);
}

// void CRoomListItemLowBDJoin::SetUserCount(int nUserCount, bool bJoinable)
// {
// 	for(int i =0; i< DEF_MAX_USERS; i++) 
// 	{
// 		if (nUserCount > i) m_listUserCount[i]->SetIndex(bJoinable?8:6);
// 		else m_listUserCount[i]->SetIndex(bJoinable?7:5);
// 	}
// }
