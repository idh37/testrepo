#include "StdAfx.h"
#include "RoomListItemHoolaJoin.h"
#include "../../WidgetID.h"

#define DEF_MAX_USERS 5

CRoomListItemHoolaJoin::CRoomListItemHoolaJoin()
:	m_pRoomNumber(NULL),
	m_pMinmumMoney(NULL),
	m_pTitle(NULL),
	m_pSeedMoney(NULL),
//	m_pRule(NULL),
	m_pPlayerCnt(NULL),
	m_pImg_SeedMoney(NULL),
	m_pImg_FullUser(NULL),
	m_pBtnEnterRoom(NULL),
	m_pBtnObserberRoom(NULL),
	m_pTitle_PrevRoom(NULL),
	m_pTitle_PrevRoomTitle(NULL)
{
}

CRoomListItemHoolaJoin::~CRoomListItemHoolaJoin()
{
}

void CRoomListItemHoolaJoin::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;

	switch(pObject->GetID())
	{
	case ID_LOBBY_TXTNB_ROOMLISTITEM_NO_HOOLA_JOIN:			m_pRoomNumber			= (LSY::CTextNumber *)pObject;	break;
	//case ID_LOBBY_TXT_ROOMLISTITEM_GAMESTYLE_HOOLA_JOIN:		m_pGameStyle			= (LSY::CText *)pObject;		break;	
	case ID_LOBBY_TXT_ROOMLISTITEM_MINMONEY_HOOLA_JOIN:		m_pMinmumMoney			= (LSY::CText *)pObject;		break;
	case ID_LOBBY_TXT_ROOMLISTITEM_TITLE_HOOLA_JOIN:          m_pTitle				= (LSY::CText *)pObject;		break;
	case ID_LOBBY_TXTNB_ROOMLISTITEM_SEEDMONEY_HOOLA_JOIN:	m_pSeedMoney			= (LSY::CTextNumber *)pObject;	break;
	case ID_LOBBY_TXT_ROOMLISTITEM_GAMEMODE_HOOLA_JOIN:		m_pStopMode				= (LSY::CText *)pObject;		break;
	case ID_LOBBY_TXT_ROOMLISTITEM_GAMETYPE_HOOLA_JOIN:		m_pJokerMode			= (LSY::CText *)pObject;		break;
	case ID_LOBBY_IMG_ROOMLISTITEM_MODE_HOOLA_JOIN:			m_pImg_Mode				= (LSY::CImage *)pObject;		break;
	case ID_LOBBY_TXTNB_ROOMLISTITEM_PLAYERCNT_HOOLA_JOIN:   m_pPlayerCnt			= (LSY::CTextNumber *)pObject;	break;
	//case ID_LOBBY_IMG_ROOMLISTITEM_GAMESTYLE_HOOLA_JOIN:		m_pImg_Gamestyle		= (LSY::CImage *)pObject;		break;
	case ID_LOBBY_IMG_ROOMLISTITEM_SEEDMONEY_HOOLA_JOIN:		m_pImg_SeedMoney		= (LSY::CImage *)pObject;		break;
	case ID_LOBBY_IMG_ROOMLISTITEM_FULLUSER_HOOLA_JOIN:		m_pImg_FullUser			= (LSY::CImage *)pObject;		break;
	case ID_LOBBY_BTN_ROOMLISTITEM_ENTER_ROOM_HOOLA_JOIN:	m_pBtnEnterRoom			= (LSY::CButton *)pObject;		break;
	case ID_LOBBY_BTN_ROOMLISTITEM_OBSERVER_ROOM_HOOLA_JOIN:	m_pBtnObserberRoom		= (LSY::CButton *)pObject;		break;
	case ID_LOBBY_TXT_ROOMLISTITEM_PREVROOM_HOOLA_JOIN:		m_pTitle_PrevRoom		= (LSY::CText *)pObject;		break;
	case ID_LOBBY_TXT_ROOMLISTITEM_PREVTITLE_HOOLA_JOIN:		m_pTitle_PrevRoomTitle	= (LSY::CText *)pObject;		break;
	}

	__super::OnCreateChild(pObject);
}

void CRoomListItemHoolaJoin::OnCreated(void)
{
	__super::OnCreated();
}

LSY::CObjectBase *CRoomListItemHoolaJoin::NewObject(void)
{
	return new CRoomListItemHoolaJoin;
}

void CRoomListItemHoolaJoin::UpdateData(const LPARAM &lParam)
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
	bool bPrevJoinedRoom = (bool) (pRoomInfo->szClientSideVar[CS_PREV_ROOM]!=0);	//직전방
	bool bSelectedRoom = (bool) (pRoomInfo->szClientSideVar[CS_SELECTED_ROOM]!=0);  //선택방

	COLORREF clrText = RGB(0,0,0);
	COLORREF clrText_fixed = RGB(0,0,0);

	m_pImg_FullUser->SetShow( false );

	switch( (hoola::EGAME_RULE_TYPE)pRoomInfo->cRuleType )
	{
	case hoola::eRULE_TYPE_WAR :
		m_pStopMode->SetText(_T("스톱off"));
		break;
	case hoola::eRULE_TYPE_STOP :
		m_pStopMode->SetText(_T("스톱on"));
		break;
	case hoola::eRULE_TYPE_NORMAL :
		break;
	default:
		break;
	}
	switch( (hoola::EGAME_RULE_TYPE)pRoomInfo->cFormKind )
	{
	case hoola::eFORM_KIND_NORMAL :
		m_pJokerMode->SetText(_T("일반"));
		break;
	case hoola::eFORM_KIND_JOKER :
		m_pJokerMode->SetText(_T("조커"));
		break;
	case hoola::eFORM_KIND_MAGIC :
		break;
	default:
		break;
	}

	//규제안 추가 나중에 서바이벌 모드 일때 수정 필요.

	// 규제안 수정.
	//참여 가능
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
		m_pBtnEnterRoom->SetShow(true);
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
	//참여불가
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
	//	m_pTitle_PrevRoom->SetText(_T("[선택방]"));				
	//}
	////직전방은 노랑색 배경
	//else if (bPrevJoinedRoom)
	//{
	//	clrText  = RGB( 43, 50, 58 );
	//	clrText_fixed = RGB( 43, 50, 58 );
	//	SetBarType( 25 ); //노란색 Bar

	//	bJoinable = TRUE; 		//직전방 RoomListItem의 Look은 bJoinable이 FALSE라도 TRUE처럼 보이게한다.
	//	m_pTitle_PrevRoom->SetText(_T("[직전방]"));		
	//}

	if (bPrevJoinedRoom)
	{
		clrText  = RGB( 43, 50, 58 );
		clrText_fixed = RGB( 43, 50, 58 );

		bJoinable = TRUE; 		//직전방 RoomListItem의 Look은 bJoinable이 FALSE라도 TRUE처럼 보이게한다.
		m_pTitle_PrevRoom->SetText(_T("[직전방]"));
	}

	// 규제안 수정.
	//더미방일경우 관전 버튼 비활성화
	//m_pBtnObserberRoom->SetShow(IsDummyRoomKey(pRoomInfo->llRoomKey)?false:true);

	//방번호
	m_pRoomNumber->SetTextColor( clrText_fixed );
	m_pRoomNumber->SetNumber(pRoomInfo->sRoomNum);

	//입장제한머니
	clrText = RGB( 130, 108, 0 );

	if( bJoinable && bEnablePlay ) m_pMinmumMoney->SetTextColor( clrText );
	else m_pMinmumMoney->SetTextColor( clrText_fixed );

	CString strText = g_GetRoomName( GM().GetCurrentGameType(), pRoomInfo );
	m_pMinmumMoney->SetText( strText.GetString() );

	clrText = RGB( 0, 0, 0 );
	//방제목
	if( bJoinable && bEnablePlay ) m_pTitle->SetTextColor( clrText );
	else m_pTitle->SetTextColor( clrText_fixed );

	strText.Format("%s", pRoomInfo->szTitle);
	m_pTitle->SetText( strText.GetString() );
	m_pTitle_PrevRoomTitle->SetText( strText.GetString() );	//직전방 타이틀


	bool bShowRoomInfoLabel = false;
	if (bPrevJoinedRoom || bSelectedRoom)
	{
		bShowRoomInfoLabel = true;
	}

	
	//직전방(선택방) 표시여부	
	m_pTitle_PrevRoomTitle->SetShow(bShowRoomInfoLabel);	//직전방일경우 직전방제목을 보여준다.
	m_pTitle_PrevRoom->SetShow(bShowRoomInfoLabel);		//직전방일경우 [직전방] 표시를 보여준다.
	m_pTitle->SetShow(!bShowRoomInfoLabel);				//직전방일경우 일반방제목은 감춘다.
	


	// 시드머니 and 배팅룰
	COLORREF colorOutLine = RGB( 1, 1, 1 );

	if( bJoinable && bEnablePlay )
	{
//		m_pRule->SetTextDrawType( LSY::ETDT_OUTLINE );
		m_pSeedMoney->SetTextDrawType( LSY::ETDT_OUTLINE );

		//m_pImg_Gamestyle->SetShow(true) ;
		//m_pGameStyle->SetTextColor( RGB(76,152,152));

		if( pRoomInfo->sAutoBetKind > 0 ) 
		{
			m_pImg_SeedMoney->SetIndex( 32 );
			m_pImg_Mode->SetIndex( 29 );

			colorOutLine  = RGB( 89, 125, 64 );
			m_pSeedMoney->SetShadowColor( colorOutLine );
//			m_pRule->SetShadowColor( colorOutLine );
		}
		else
		{
			m_pImg_SeedMoney->SetIndex( 32 );
			m_pImg_Mode->SetIndex( 29 );

			colorOutLine  = RGB( 72, 112, 150 );
			m_pSeedMoney->SetShadowColor( colorOutLine );
//			m_pRule->SetShadowColor( coUpdatelorOutLine );
		}

		clrText = RGB( 255, 255, 255 );	
		m_pSeedMoney->SetTextColor( clrText );
//		m_pRule->SetTextColor( clrText );
	}
	else
	{
//		m_pRule->SetTextDrawType( LSY::ETDT_NORMAL );
		m_pSeedMoney->SetTextDrawType( LSY::ETDT_NORMAL );

		//m_pImg_Gamestyle->SetShow(false) ;

		if( bMouseOver )
		{
			m_pImg_SeedMoney->SetIndex( 31 );
			m_pImg_Mode->SetIndex( 28 );

			clrText = RGB( 59, 59, 59 );
			m_pSeedMoney->SetTextColor( clrText );
//			m_pRule->SetTextColor( clrText );

			colorOutLine  = RGB( 59, 59, 59 );
			m_pSeedMoney->SetShadowColor( colorOutLine );
//			m_pRule->SetShadowColor( colorOutLine );
		}
		else
		{
			m_pImg_SeedMoney->SetIndex( 30 );
			m_pImg_Mode->SetIndex( 27 );

			clrText = RGB( 85, 88, 93 );
			m_pSeedMoney->SetTextColor( clrText );
//			m_pRule->SetTextColor( clrText );

			colorOutLine  = RGB( 85, 88, 93 );
			m_pSeedMoney->SetShadowColor( colorOutLine );
//			m_pRule->SetShadowColor( colorOutLine );
		}

		//m_pGameStyle->SetTextColor(clrText);
	}

	m_pSeedMoney->SetNumber( pRoomInfo->llSeedMoney );

	strText.Format("%s", UPGGetAutoBettingRuleName((IDX_GAME)pRoomInfo->nGameIndex, pRoomInfo->sAutoBetKind, (BETRULE_KIND)pRoomInfo->sBetRuleKind ) );
//	m_pRule->SetText( strText.GetString() );

	//인원수
	//strText.Format( "%d/%d", m_RoomInfo.cNowUserNum, m_RoomInfo.cMaxUserNum );
	m_pPlayerCnt->SetTextColor(clrText_fixed);
	m_pPlayerCnt->SetNumber(pRoomInfo->cNowUserNum);
	strText.Format( "/%d", pRoomInfo->cMaxUserNum );
	m_pPlayerCnt->SetFirstUnit(strText.GetString());
}

void CRoomListItemHoolaJoin::SetBarType(int nStartIndex)
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

void CRoomListItemHoolaJoin::OnMouseOver(void)
{
	__super::OnMouseOver();
	UpdateData(0);
}

void CRoomListItemHoolaJoin::OnMouseLeave(void)
{
	__super::OnMouseLeave();
	UpdateData(0);
}