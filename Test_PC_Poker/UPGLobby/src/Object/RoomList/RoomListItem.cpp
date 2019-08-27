#include "StdAfx.h"
#include "RoomListItem.h"
#include "../../WidgetID.h"

#define DEF_MAX_USERS 5

CRoomListItem::CRoomListItem()
:	m_pRoomNumber(NULL),
	m_pMinmumMoney(NULL),
	m_pTitle(NULL),
	m_pGameMode(NULL),
	m_pSeedMoney(NULL),
	m_pRule(NULL),
	m_pPlayerCnt(NULL),
	m_pImg_Mode(NULL),
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

CRoomListItem::~CRoomListItem()
{
}

void CRoomListItem::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;

	switch(pObject->GetID())
	{
	case ID_LOBBY_TXTNB_ROOMLISTITEM_NO:			m_pRoomNumber			= (LSY::CTextNumber *)pObject;	break;
	case ID_LOBBY_TXT_ROOMLISTITEM_GAMESTYLE:		m_pGameStyle			= (LSY::CText *)pObject;		break;		
	case ID_LOBBY_TXT_ROOMLISTITEM_MINMONEY:		m_pMinmumMoney			= (LSY::CText *)pObject;		break;
	case ID_LOBBY_TXT_ROOMLISTITEM_TITLE:           m_pTitle				= (LSY::CText *)pObject;		break;
	case ID_LOBBY_TXT_ROOMLISTITEM_GAMEMODE:		m_pGameMode				= (LSY::CText *)pObject;		break;
	case ID_LOBBY_TXTNB_ROOMLISTITEM_SEEDMONEY:		m_pSeedMoney			= (LSY::CTextNumber *)pObject;	break;
	case ID_LOBBY_TXT_ROOMLISTITEM_RULE:			m_pRule					= (LSY::CText *)pObject;		break;
	case ID_LOBBY_TXTNB_ROOMLISTITEM_PLAYERCNT:		m_pPlayerCnt			= (LSY::CTextNumber *)pObject;	break;
	case ID_LOBBY_IMG_ROOMLISTITEM_MODE:			m_pImg_Mode				= (LSY::CImage *)pObject;		break;
	case ID_LOBBY_IMG_ROOMLISTITEM_GAMESTYLE:		m_pImg_Gamestyle		= (LSY::CImage *)pObject;		break;
	case ID_LOBBY_IMG_ROOMLISTITEM_SEEDMONEY:		m_pImg_SeedMoney		= (LSY::CImage *)pObject;		break;
	case ID_LOBBY_IMG_ROOMLISTITEM_FULLUSER:		m_pImg_FullUser			= (LSY::CImage *)pObject;		break;
	case ID_LOBBY_BTN_ROOMLISTITEM_ENTER_ROOM:		m_pBtnEnterRoom			= (LSY::CButton *)pObject;		break;
	case ID_LOBBY_BTN_ROOMLISTITEM_OBSERVER_ROOM:	m_pBtnObserberRoom		= (LSY::CButton *)pObject;		break;
	case ID_LOBBY_TXT_ROOMLISTITEM_PREVROOM:		m_pTitle_PrevRoom		= (LSY::CText *)pObject;		break;
	case ID_LOBBY_TXT_ROOMLISTITEM_PREVTITLE:		m_pTitle_PrevRoomTitle	= (LSY::CText *)pObject;		break;
	case ID_LOBBY_TXT_ROOMLISTITEM_CARDSETTING:		m_pCardSetting			= (LSY::CText *)pObject;		break;		
		
// 	case ID_LOBBY_IMG_ROOMLISTITEM_USER0:
// 	case ID_LOBBY_IMG_ROOMLISTITEM_USER1:
// 	case ID_LOBBY_IMG_ROOMLISTITEM_USER2:
// 	case ID_LOBBY_IMG_ROOMLISTITEM_USER3:
// 	case ID_LOBBY_IMG_ROOMLISTITEM_USER4:
// 		m_listUserCount[pObject->GetID() - ID_LOBBY_IMG_ROOMLISTITEM_USER0] = (LSY::CImage *)pObject;
// 		break;
	}

	__super::OnCreateChild(pObject);
}

void CRoomListItem::OnCreated(void)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
	__super::OnCreated();
}

LSY::CObjectBase *CRoomListItem::NewObject(void)
{
	return new CRoomListItem;
}


void CRoomListItem::UpdateData(const LPARAM &lParam)
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

	//규제안 추가 나중에 서바이벌 모드 일때 수정 필요.
	//m_pGameStyle->SetText("일반") ;
		
	m_pImg_FullUser->SetShow( false );

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

		SetBarType( 48 );
		//m_pBtnEnterRoom->SetShow(true);
	}
	//Full
	else if (bJoinable)
	{	
		//m_pImg_FullUser->SetShow( true );

		if( bMouseOver )
		{
			clrText = RGB( 59, 59, 59 );
			clrText_fixed = RGB( 59, 59, 59 );
			
			//m_pImg_FullUser->SetIndex( 8 );
		}
		else
		{
			clrText = RGB( 85, 88, 93 );
			clrText_fixed = RGB( 85, 88, 93 );

			//m_pImg_FullUser->SetIndex( 7 );
		}

		SetBarType( 46 );
		//m_pBtnEnterRoom->SetShow(false);
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

		SetBarType( 46 );
		//m_pBtnEnterRoom->SetShow(false);
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

	/*
	//입장머니제한, buy in, 맥스방인 경우 별도 처리
	if( pRoomInfo->bIsMaxRoom ){

		//7포커인 경우-추후 맞포커만 제외하고 전부 반영될 여지 있음
		if( pRoomInfo->nGameIndex == 0 ){
			strText.Format("[입장머니방]%s", pRoomInfo->szTitle);
		}
	}
	*/
	
	m_pTitle->SetText( strText.GetString() );
	m_pTitle_PrevRoomTitle->SetText( strText.GetString() );	//직전방 타이틀

	bool bShowRoomInfoLabel = false;
	if (bPrevJoinedRoom || bSelectedRoom)
	{
		bShowRoomInfoLabel = true;
	}

	//규제안 : 표시제거
	//직전방(선택방) 표시여부
	m_pTitle_PrevRoomTitle->SetShow(bShowRoomInfoLabel);	//직전방일경우 직전방제목을 보여준다.
	m_pTitle_PrevRoom->SetShow(bShowRoomInfoLabel);		//직전방일경우 [직전방] 표시를 보여준다.
	m_pTitle->SetShow(!bShowRoomInfoLabel);				//직전방일경우 일반방제목은 감춘다.

	//pRoomInfo->cMemberRoom = 2;
	//모드
	strText = g_GetFormName( (IDX_GAME)pRoomInfo->nGameIndex, pRoomInfo->cFormKind );
	if( strText.IsEmpty() == false )
	{
		if( bJoinable && bEnablePlay )
		{
			//일반
			if( pRoomInfo->cFormKind == 0 ) 
			{
				m_pImg_Mode->SetIndex( 19 );
				clrText = RGB( 53, 76, 118 );
			}
			//초이스
			else if( pRoomInfo->cFormKind == 1 ) 
			{
				m_pImg_Mode->SetIndex( 20 );
				clrText = RGB( 67, 92, 0 );
			}
			//폭탄
			else if ( pRoomInfo->cFormKind == 2 ) 
			{
				m_pImg_Mode->SetIndex( 20 );
				clrText = RGB( 67, 92, 0 );
			}
			//조커
			else /*if ( pRoomInfo->cFormKind == 3 ) */
			{
				m_pImg_Mode->SetIndex( 19 );
				clrText = RGB( 53, 76, 118 );
			}
		}
		else
		{
			if( bMouseOver ) m_pImg_Mode->SetIndex( 18 );
			else m_pImg_Mode->SetIndex( 17 );

			clrText = clrText_fixed;
		}

		m_pGameMode->SetTextColor( clrText );
		m_pGameMode->SetText( strText.GetString() );
	}

	// 시드머니 and 배팅룰
	COLORREF colorOutLine = RGB( 1, 1, 1 );

	if( bJoinable && bEnablePlay )
	{
		m_pRule->SetTextDrawType( LSY::ETDT_OUTLINE );
		m_pSeedMoney->SetTextDrawType( LSY::ETDT_OUTLINE );
		m_pCardSetting->SetTextDrawType( LSY::ETDT_OUTLINE );

		// 일반, 서바이벌 모드 
		m_pImg_Gamestyle->SetShow(true) ;
		m_pGameStyle->SetTextColor( RGB(76,152,152));

		if( pRoomInfo->sAutoBetKind > 0 ) 
		{
			m_pImg_SeedMoney->SetIndex( 12 );

			colorOutLine  = RGB( 89, 125, 64 );
			m_pSeedMoney->SetShadowColor( colorOutLine );
			m_pCardSetting->SetShadowColor( colorOutLine );
			m_pRule->SetShadowColor( colorOutLine );
		}
		else
		{
			m_pImg_SeedMoney->SetIndex( 11 );

			colorOutLine  = RGB( 72, 112, 150 );
			m_pSeedMoney->SetShadowColor( colorOutLine );
			m_pCardSetting->SetShadowColor( colorOutLine );			
			m_pRule->SetShadowColor( colorOutLine );
		}

		clrText = RGB( 255, 255, 255 );	
		m_pSeedMoney->SetTextColor( clrText );
		m_pCardSetting->SetTextColor( clrText );
		m_pRule->SetTextColor( clrText );
	}
	else
	{
		m_pRule->SetTextDrawType( LSY::ETDT_NORMAL );
		m_pSeedMoney->SetTextDrawType( LSY::ETDT_NORMAL );
		m_pCardSetting->SetTextDrawType( LSY::ETDT_NORMAL );

		m_pImg_Gamestyle->SetShow(false) ;

		if( bMouseOver )
		{
			m_pImg_SeedMoney->SetIndex( 10 );
			
			clrText = RGB( 59, 59, 59 );
			m_pSeedMoney->SetTextColor( clrText );
			m_pCardSetting->SetTextColor( clrText );
			m_pRule->SetTextColor( clrText );

			colorOutLine  = RGB( 59, 59, 59 );
			m_pSeedMoney->SetShadowColor( colorOutLine );
			m_pCardSetting->SetShadowColor( colorOutLine );
			m_pRule->SetShadowColor( colorOutLine );
		}
		else
		{
			m_pImg_SeedMoney->SetIndex( 9 );
			
			clrText = RGB( 85, 88, 93 );
			m_pSeedMoney->SetTextColor( clrText );
			m_pCardSetting->SetTextColor( clrText );
			m_pRule->SetTextColor( clrText );

			colorOutLine  = RGB( 85, 88, 93 );
			m_pSeedMoney->SetShadowColor( colorOutLine );
			m_pCardSetting->SetShadowColor( colorOutLine );
			m_pRule->SetShadowColor( colorOutLine );
		}

		m_pGameStyle->SetTextColor(clrText);
	}
	if(1==pRoomInfo->cMemberRoom){
		m_pImg_Gamestyle->SetIndex(60);
		m_pGameStyle->SetText("다이아") ;
		m_pGameStyle->SetTextColor(RGB(154, 80, 186));
	}
	else{
		m_pImg_Gamestyle->SetIndex(58);
		m_pGameStyle->SetText("일반") ;
	}


	m_pSeedMoney->SetNumber( pRoomInfo->llSeedMoney );
	m_pSeedMoney->SetAddText(strChip_Count[pRoomInfo->ChipKind]);

	// 카드 장수. : 규제안 - 참가비,카드세팅 리스트 아이템 제목 변경
	if (GM().GetCurrentGameType() == IDX_GAME_SD)
	{
		/*strText.Format("%s",  GetBetMoneyMultiName(IDX_GAME_SD, pRoomInfo->fBetMoneyMulti ) );*/
		strText.Format("수동");
	} 
	else
	{
		/*strText.Format("%d장", pRoomInfo->cCardSetting );*/
		if(GM().GetCurrentGameType() == IDX_GAME_NP)
		{
			if(pRoomInfo->cCardSetting > 5)
				strText.Format("%d구", pRoomInfo->cCardSetting );
			else
				strText.Format("수동");
		}
		else
		{
			if(pRoomInfo->cCardSetting > 4)
				strText.Format("%d구", pRoomInfo->cCardSetting );
			else
				strText.Format("수동");	
		}
	}
	
	m_pCardSetting->SetText(strText.GetString()) ;

	strText.Format("%s", UPGGetAutoBettingRuleName((IDX_GAME)pRoomInfo->nGameIndex, pRoomInfo->sAutoBetKind, (BETRULE_KIND)pRoomInfo->sBetRuleKind ) );
	m_pRule->SetText( strText.GetString() );
	//strText.Format("%s", g_MakeShortMoneyStr( pRoomInfo->llSeedMoney ) );
	//m_pSeedMoney->SetText( strText.GetString() );

	//인원수
	//strText.Format( "%d/%d", m_RoomInfo.cNowUserNum, m_RoomInfo.cMaxUserNum );
	m_pPlayerCnt->SetTextColor(clrText_fixed);
	m_pPlayerCnt->SetNumber(pRoomInfo->cNowUserNum);
	strText.Format( "/%d", pRoomInfo->cMaxUserNum );
	m_pPlayerCnt->SetFirstUnit(strText.GetString());

// 	m_pMinmumMoney->SetTextColor( clrText_fixed );
// 	m_pMinmumMoney->SetText(strText.GetString());
// 
// 	m_pSeedMoney->SetTextColor(clrText);
// 	m_pSeedMoney->SetNumber(pRoomInfo->llSeedMoney);
// 	strText.Format("/%s",UPGGetAutoBettingRuleName((IDX_GAME)pRoomInfo->nGameIndex, pRoomInfo->sAutoBetKind));
// 	m_pSeedMoney->SetFirstUnit(strText.GetString());
// 
// 	CString strFormName = g_GetFormName((IDX_GAME)pRoomInfo->nGameIndex, pRoomInfo->cFormKind);
// 	if (strFormName.IsEmpty())
// 		strText = pRoomInfo->szTitle;
// 	else
// 		strText.Format("%s / %s", strFormName, pRoomInfo->szTitle);
// 
// 	m_pGameMode->SetTextColor(clrText);
// 	m_pGameMode->SetText(strText.GetString());

// 	m_pTatalMoney->SetTextColor(clrText);
// 	m_pTatalMoney->SetNumber(pRoomInfo->llTotalPMoneySum);

	//SetUserCount(pRoomInfo->cNowUserNum, pRoomInfo->szClientSideVar[CS_JOINABLE] != 0);
}

void CRoomListItem::SetBarType(int nStartIndex)
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

void CRoomListItem::OnMouseOver(void)
{
	__super::OnMouseOver();
	UpdateData(0);
}

void CRoomListItem::OnMouseLeave(void)
{
	__super::OnMouseLeave();
	UpdateData(0);
}

// void CRoomListItem::SetUserCount(int nUserCount, bool bJoinable)
// {
// 	for(int i =0; i< DEF_MAX_USERS; i++) 
// 	{
// 		if (nUserCount > i) m_listUserCount[i]->SetIndex(bJoinable?8:6);
// 		else m_listUserCount[i]->SetIndex(bJoinable?7:5);
// 	}
//}