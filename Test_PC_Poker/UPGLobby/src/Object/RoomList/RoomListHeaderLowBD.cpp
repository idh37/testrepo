#include "StdAfx.h"
#include "RoomListHeaderLowBD.h"
#include "RoomListItemLowBD.h"
#include "../../WidgetID.h"
#include "CompareFunction.h"


#define DEF_ROOMLIST_SORT_UNKNOWN			-1
#define DEF_ROOMLIST_SORT_ROOMNUMBER		0
#define DEF_ROOMLIST_SORT_MINIMUMMONEY		1
#define DEF_ROOMLIST_SORT_TITLE				2
#define DEF_ROOMLIST_SORT_SEEDMONEY			3
//#define DEF_ROOMLIST_SORT_GAMEMODE		4
#define DEF_ROOMLIST_SORT_USERCOUNT			4
#define DEF_ROOMLIST_SORT_TOTALMONEY		5
#define DEF_ROOMLIST_SORT_DEFAULT			6
#define DEF_ROOMLIST_SORT_ENTERLMITMONEY	7

CRoomListHeaderLowBD::CRoomListHeaderLowBD(void)
:	m_pTabRoomType(NULL),
	m_pBtnRoomNumber(NULL),
	m_pBtnTitle(NULL),
	m_pBtnMinimumMoney(NULL),
	m_pBtnSeedMoney(NULL),
	//m_pBtnGameMode(NULL),
	m_pBtnUserCount(NULL),
	//m_pBtnRoomTotalMoney(NULL),
	m_pBtnDefaultSort(NULL),
	m_pList(NULL),
	m_nCurRoomType(-1)
{
	m_bJoinable = false;

	m_nCurSortType = DEF_ROOMLIST_SORT_UNKNOWN;
	m_mapSortTypeIncrement[DEF_ROOMLIST_SORT_ROOMNUMBER]		= true;
	m_mapSortTypeIncrement[DEF_ROOMLIST_SORT_MINIMUMMONEY]		= true;
	m_mapSortTypeIncrement[DEF_ROOMLIST_SORT_TITLE]				= true;
	m_mapSortTypeIncrement[DEF_ROOMLIST_SORT_SEEDMONEY]			= true;
	m_mapSortTypeIncrement[DEF_ROOMLIST_SORT_USERCOUNT]			= true;
	m_mapSortTypeIncrement[DEF_ROOMLIST_SORT_TOTALMONEY]		= true;
	m_mapSortTypeIncrement[DEF_ROOMLIST_SORT_DEFAULT]			= true;
	m_mapSortTypeIncrement[DEF_ROOMLIST_SORT_ENTERLMITMONEY]	= true;
}

CRoomListHeaderLowBD::~CRoomListHeaderLowBD(void)
{

}

void CRoomListHeaderLowBD::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;

	bool bAddChild = false;
	switch(pObject->GetID())
	{
	case ID_LOBBY_TAB_ROOM_LIST_LOWBD:				m_pTabRoomType					= (LSY::CTabControl *)pObject;		break;
	case ID_LOBBY_TXTBTN_ROOMLIST_HEADER_NO_LOWBD:		m_pBtnRoomNumber			= (LSY::CTextButton *)pObject;	bAddChild = true;	break;
	case ID_LOBBY_TXTBTN_ROOMLIST_HEADER_MINMONEY_LOWBD:	m_pBtnMinimumMoney		= (LSY::CTextButton *)pObject;	bAddChild = true;	break;
	case ID_LOBBY_TXTBTN_ROOMLIST_HEADER_TITLE_LOWBD:		m_pBtnTitle				= (LSY::CTextButton *)pObject;	bAddChild = true;	break;
	case ID_LOBBY_TXTBTN_ROOMLIST_HEADER_SEEDMONEY_LOWBD:	m_pBtnSeedMoney			= (LSY::CTextButton *)pObject;	bAddChild = true;	break;
	case ID_LOBBY_TXTBTN_ROOMLIST_HEADER_USERCOUNT_LOWBD:	m_pBtnUserCount			= (LSY::CTextButton *)pObject;	bAddChild = true;	break;
	case ID_LOBBY_TXTBTN_ROOMLIST_HEADER_DEFAULT_SORT_LOWBD: m_pBtnDefaultSort		= (LSY::CTextButton *)pObject;	bAddChild = true;	break;
//	case ID_LOBBY_TXTBTN_ROOMLIST_HEADER_TOTALMONEY_LOWBD:m_pBtnRoomTotalMoney	= (LSY::CTextButton *)pObject;	bAddChild = true;	break;
//	case ID_LOBBY_BTN_ROOMLIST_HEADER_DEFAULT_SORT_LOWBD:	m_pBtnDefaultSort		= (LSY::CButton *)pObject;		bAddChild = true;	break;
	case ID_LOBBY_LIST_ROOM_LOWBD:							m_pList					= (LSY::CList *)pObject;			break;
	}

	if(bAddChild)
	{
		pObject->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CRoomListHeaderLowBD::OnClickButton));
	}

	__super::OnCreateChild(pObject);
}

void CRoomListHeaderLowBD::OnCreated(void)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);

	m_pList->SetCompareFunction(DEF_ROOMLIST_SORT_ROOMNUMBER, (LSY::CList::LISTCOMPAREFUNCTION)&RoomListRoomNumberCompareFunction);
	m_pList->SetCompareFunction(DEF_ROOMLIST_SORT_MINIMUMMONEY, (LSY::CList::LISTCOMPAREFUNCTION)&RoomListMinimumMoneyCompareFunction);
	m_pList->SetCompareFunction(DEF_ROOMLIST_SORT_TITLE, (LSY::CList::LISTCOMPAREFUNCTION)&RoomListTitleCompareFunction);
	m_pList->SetCompareFunction(DEF_ROOMLIST_SORT_SEEDMONEY, (LSY::CList::LISTCOMPAREFUNCTION)&RoomListSeedMoneyCompareFunction);
	m_pList->SetCompareFunction(DEF_ROOMLIST_SORT_USERCOUNT, (LSY::CList::LISTCOMPAREFUNCTION)&RoomListUserCountCompareFunction);
	m_pList->SetCompareFunction(DEF_ROOMLIST_SORT_TOTALMONEY, (LSY::CList::LISTCOMPAREFUNCTION)&RoomListTotalMoneyCompareFunction);
	m_pList->SetCompareFunction(DEF_ROOMLIST_SORT_DEFAULT, (LSY::CList::LISTCOMPAREFUNCTION)&RoomListDefaultCompareFunction);
	m_pList->SetCompareFunction(DEF_ROOMLIST_SORT_ENTERLMITMONEY, (LSY::CList::LISTCOMPAREFUNCTION)&RoomListEnterMinimumMoneyCompareFunction);

	m_pTabRoomType->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CRoomListHeaderLowBD::OnRoomTypeSelChange));

	m_pList->AddHandler(LSY::EM_O_LISTITEM_CLICK, LSY::Fnt(this, &CRoomListHeaderLowBD::OnListItemLClick));
	m_pList->AddHandler(LSY::EM_O_LISTITEM_DBCLICK, LSY::Fnt(this, &CRoomListHeaderLowBD::OnListItemLDbClick));

	LSY::CListItem *pListItem = m_pList->GetItem();
	LSY::CButton *pEnterRoom = (LSY::CButton *)pListItem->GetFirstChildFromID(ID_LOBBY_BTN_ROOMLISTITEM_ENTER_ROOM_LOWBD);
	pEnterRoom->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CRoomListHeaderLowBD::OnClickEnterRoom));

	LSY::CButton *pObserverRoom = (LSY::CButton *)pListItem->GetFirstChildFromID(ID_LOBBY_BTN_ROOMLISTITEM_OBSERVER_ROOM_LOWBD);
	pObserverRoom->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CRoomListHeaderLowBD::OnClickObserverRoom));

	ChangeRoomTab(ROOMLIST_TYPE_NORMAL);

	LSY::CSprite* pcSprite = (LSY::CSprite*)m_pTabRoomType->GetFirstChildFromID(ID_LOBBY_SPRITE_PROMOTION_VIP_COUPON_LOWBD);
	if(NULL!=pcSprite){
		if(GM().IsVIPCouponPromotion()){
			pcSprite->SetShow(true);
			pcSprite->StartAnimation(0);
		}
		else{
			pcSprite->SetShow(false);
			pcSprite->StopAnimation();
		}
	}
	LSY::CRadio* pcRadio1 = (LSY::CRadio*)m_pTabRoomType->GetRadioFromIndex(0);
	LSY::CRadio* pcRadio2 = (LSY::CRadio*)m_pTabRoomType->GetRadioFromIndex(1);
	if(NULL!=pcRadio1){
		int nArray1[8]={32, 33, 34, 35, 35, 35, 35, 35};
		pcRadio1->SetFreeStatusButton((int*)nArray1);
	}
	if(NULL!=pcRadio2){
		int nArray2[8]={36, 37, 38, 39, 39, 39, 39, 39};
		pcRadio2->SetFreeStatusButton((int*)nArray2);
	}
	__super::OnCreated();
}

LRESULT CRoomListHeaderLowBD::OnClickButton(LSY::CMessage *msg)
{
	LSY::CMO_MouseLClick *pMsg = (LSY::CMO_MouseLClick *)msg;
	LSY::CButton *pBtn = (LSY::CButton *)pMsg->GetObject();
	CWnd *pWnd = GetObjectMan()->GetWnd();

	//소팅버튼클릭시 선택방을 제거한다.
	pWnd->SendMessage(ID_LOBBY_PROCESS_REMOVE_SELECTED_ROOM);

	switch(pBtn->GetID())
	{
		case ID_LOBBY_TXTBTN_ROOMLIST_HEADER_NO_LOWBD:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_ROOMLIST_TAB_NO);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			Sort(DEF_ROOMLIST_SORT_ROOMNUMBER);
		} break;
		case ID_LOBBY_TXTBTN_ROOMLIST_HEADER_MINMONEY_LOWBD:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_ROOMLIST_TAB_ENTER_LIMIT);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			Sort(DEF_ROOMLIST_SORT_MINIMUMMONEY);
		} break;
		case ID_LOBBY_TXTBTN_ROOMLIST_HEADER_TITLE_LOWBD:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_ROOMLIST_TAB_ROOMNAME);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			Sort(DEF_ROOMLIST_SORT_TITLE);
		} break;
		case ID_LOBBY_TXTBTN_ROOMLIST_HEADER_SEEDMONEY_LOWBD:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_ROOMLIST_TAB_SEED_BETRULE);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			Sort(DEF_ROOMLIST_SORT_SEEDMONEY);
		} break;
		case ID_LOBBY_TXTBTN_ROOMLIST_HEADER_USERCOUNT_LOWBD:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_ROOMLIST_TAB_PERSON_NUM);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			Sort(DEF_ROOMLIST_SORT_USERCOUNT);
		} break;
		case ID_LOBBY_TXTBTN_ROOMLIST_HEADER_DEFAULT_SORT_LOWBD:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_ROOMLIST_TAB_CAN_ENTER_ROOM);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			Sort( DEF_ROOMLIST_SORT_DEFAULT );
		} break;
	//	case ID_LOBBY_TXTBTN_ROOMLIST_HEADER_TOTALMONEY_LOWBD:Sort(DEF_ROOMLIST_SORT_TOTALMONEY);		break;
	//	case ID_LOBBY_BTN_ROOMLIST_HEADER_DEFAULT_SORT_LOWBD:	Sort(DEF_ROOMLIST_SORT_DEFAULT);		break;
	}
	return TRUE;
}

void CRoomListHeaderLowBD::Sort(int nSortType)
{
	if(m_nCurSortType == nSortType && nSortType != DEF_ROOMLIST_SORT_UNKNOWN)
	{
		m_mapSortTypeIncrement[nSortType] = m_mapSortTypeIncrement[nSortType]?false:true;
	}

	m_nCurSortType = nSortType;

	if(nSortType != DEF_ROOMLIST_SORT_UNKNOWN)
	{
		m_pList->SetSortType(nSortType);
		m_pList->SetIncrement(m_mapSortTypeIncrement[nSortType]);
		m_pList->Sort();
	}
}


void CRoomListHeaderLowBD::SortDefault(void)
{
	m_nCurSortType = DEF_ROOMLIST_SORT_DEFAULT;
	m_mapSortTypeIncrement[DEF_ROOMLIST_SORT_DEFAULT] = false;

	m_pList->SetSortType(DEF_ROOMLIST_SORT_DEFAULT);
	m_pList->Sort();
}

void CRoomListHeaderLowBD::SortEnterRoomMinimumMoney(INT64 *i64Money)
{
	m_nCurSortType = DEF_ROOMLIST_SORT_ENTERLMITMONEY;
	m_mapSortTypeIncrement[DEF_ROOMLIST_SORT_ENTERLMITMONEY] = false;

	m_pList->SetSortType(DEF_ROOMLIST_SORT_ENTERLMITMONEY);
	m_pList->SetCompareFunctionParam(DEF_ROOMLIST_SORT_ENTERLMITMONEY, (LPARAM)i64Money);
	m_pList->SetIncrement(false);
	m_pList->Sort();
}

void CRoomListHeaderLowBD::ChangeRoomTab( ROOMLIST_TYPE eRoomListType )
{
	UINT nID = ID_LOBBY_RAD_VIP_ROOM_LIST_LOWBD;

	if (eRoomListType==ROOMLIST_TYPE_NORMAL)
	{
		GM().SetRoomListTabState(1) ;
		nID = ID_LOBBY_RAD_NORMAL_ROOM_LIST_LOWBD;
	}
	else if (eRoomListType==ROOMLIST_TYPE_FREE)
	{
		GM().SetRoomListTabState(0) ;
		nID = ID_LOBBY_RAD_FREE_ROOM_LIST_LOWBD;
	}
	else if (eRoomListType==ROOMLIST_TYPE_VIP_GOLD)
	{
		GM().SetRoomListTabState(2) ;
		nID = ID_LOBBY_RAD_VIP_GOLD_ROOM_LIST_LOWBD;
	}
	else if (eRoomListType==ROOMLIST_TYPE_NORMAL_GOLD)
	{
		GM().SetRoomListTabState(2) ;
		nID = ID_LOBBY_RAD_NORMAL_GOLD_ROOM_LIST_LOWBD;
	}
	else
	{
		GM().SetRoomListTabState(1) ;
	}

	if(nID == m_nCurRoomType) return;

	if (GM().getRoomListTabState()!=1)
	{
		MM().Call(UPG_CMK_ROOMLISTHEADER_CHANGE, true, eRoomListType);
	}

	m_pTabRoomType->SetCurSelFromID(nID);
	m_nCurRoomType = nID;
	GM().SetRoomListTabIndex((int)eRoomListType - 1);

	AddDataRoomList();

// 	RemoveAll();
// 	CDataRoomList::DATAPOSITION pos;
// 	SMALLROOMINFO *pRoomInfo = GM().GetDataRoomList()->GetFirstData(pos);
// 	while(pRoomInfo)
// 	{
// 		if( m_bJoinable && pRoomInfo->szClientSideVar[CS_JOINABLE] == false ) 
// 		{
// 			pRoomInfo = GM().GetDataRoomList()->GetNextData(pos);
// 			continue;
// 		}
// 		else
// 		{
// 
// 			AddItem(pRoomInfo);
// 			pRoomInfo = GM().GetDataRoomList()->GetNextData(pos);
// 		}
// 	}

//	SortDefault();
}

void CRoomListHeaderLowBD::AddDataRoomList()
{
	RemoveAll();
	CDataRoomList::DATAPOSITION pos;
	SMALLROOMINFO *pRoomInfo = GM().GetDataRoomList()->GetFirstData(pos);
	while(pRoomInfo)
	{
		AddItem(pRoomInfo);
		pRoomInfo = GM().GetDataRoomList()->GetNextData(pos);
	}

	SortDefault();
}

LRESULT CRoomListHeaderLowBD::OnRoomTypeSelChange(LSY::CMessage *msg)
{
	NMBASE::SKINGDI::NMRESAM.PlayWav( "UI-SND_BUTTON_CLICK_A" );
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();
	LSY::CRadio *pRadio = m_pTabRoomType->GetRadioFromIndex(nIndex);
	int nID = pRadio->GetID();

	switch(nID)
	{
	case ID_LOBBY_RAD_VIP_ROOM_LIST_LOWBD:
		SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_ROOMLIST_VIP);
		ChangeRoomTab(ROOMLIST_TYPE_VIP);
		break;
	case ID_LOBBY_RAD_NORMAL_ROOM_LIST_LOWBD:
		SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_ROOMLIST_NOMAL);
		ChangeRoomTab(ROOMLIST_TYPE_NORMAL);
		break;
	case ID_LOBBY_RAD_FREE_ROOM_LIST_LOWBD:
		SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_ROOMLIST_FREE);
		ChangeRoomTab(ROOMLIST_TYPE_FREE);
		break;
	case ID_LOBBY_RAD_VIP_GOLD_ROOM_LIST_LOWBD:
		SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_ROOMLIST_VIP_GOLD);
		ChangeRoomTab(ROOMLIST_TYPE_VIP_GOLD);
		break;
	case ID_LOBBY_RAD_NORMAL_GOLD_ROOM_LIST_LOWBD:
		SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_ROOMLIST_NORMAL_GOLD);
		ChangeRoomTab(ROOMLIST_TYPE_NORMAL_GOLD);
		break;
	}

	return TRUE;
}

void CRoomListHeaderLowBD::SetJoinable( bool bstate )
{
	m_bJoinable = bstate;
}


void CRoomListHeaderLowBD::RemoveAll(void)
{
	m_pList->DeleteAllItem();
}

void CRoomListHeaderLowBD::AddItem(SMALLROOMINFO *pRoomInfo)
{
	//직전방은 참여불가방이라도 무조건 보여준다.
	if( m_bJoinable && pRoomInfo->szClientSideVar[CS_JOINABLE] == false && pRoomInfo->szClientSideVar[CS_PREV_ROOM] == false) 
		return;

	long long llVipMoney = VIPRoom_Money(pRoomInfo->nGameIndex);
	switch(m_nCurRoomType)
	{
	case ID_LOBBY_RAD_VIP_ROOM_LIST_LOWBD:
		if(pRoomInfo->ChipKind == 0 && pRoomInfo->llEnterLimitMoney >= llVipMoney) 
			m_pList->AddItem((LPARAM)pRoomInfo);
		break;
	case ID_LOBBY_RAD_NORMAL_ROOM_LIST_LOWBD:
		if(pRoomInfo->ChipKind == 0 && pRoomInfo->llEnterLimitMoney > 0 && pRoomInfo->llEnterLimitMoney < llVipMoney) 
			m_pList->AddItem((LPARAM)pRoomInfo);
		break;
	case ID_LOBBY_RAD_FREE_ROOM_LIST_LOWBD:
		if(pRoomInfo->llEnterLimitMoney == 0) 
			m_pList->AddItem((LPARAM)pRoomInfo);
		break;
	case ID_LOBBY_RAD_VIP_GOLD_ROOM_LIST_LOWBD:
		if(pRoomInfo->ChipKind == 1 && pRoomInfo->llEnterLimitMoney >= GOLD_SEPERATOR_FOR_VIP_ENTER) 
			m_pList->AddItem((LPARAM)pRoomInfo);
		break;
	case ID_LOBBY_RAD_NORMAL_GOLD_ROOM_LIST_LOWBD:
		if(pRoomInfo->ChipKind == 1 && pRoomInfo->llEnterLimitMoney > 0 && pRoomInfo->llSeedMoney < GOLD_SEPERATOR_FOR_VIP_SEED) 
			m_pList->AddItem((LPARAM)pRoomInfo);
		break;
	}
}

void CRoomListHeaderLowBD::DelItem(SMALLROOMINFO *pRoomInfo)
{
	m_pList->DeleteItem((LPARAM)pRoomInfo);
}

void CRoomListHeaderLowBD::ChangeItem(SMALLROOMINFO *pRoomInfo)
{
	m_pList->UpdateItemFromParam((LPARAM)pRoomInfo);
}

void CRoomListHeaderLowBD::SetListEnable(bool bEnable)
{
	m_pList->SetEnable(bEnable);
}

LRESULT CRoomListHeaderLowBD::OnListItemLClick(LSY::CMessage *msg)
{
	LSY::CMO_ListItemClick *pMsg = (LSY::CMO_ListItemClick *)msg;
	LSY::CListItemData *pData = pMsg->GetItemData();
	SMALLROOMINFO *pRoomInfo = (SMALLROOMINFO *)pData->GetItemData();

	CWnd *pWnd = GetObjectMan()->GetWnd();
	pWnd->SendMessage(ID_LOBBY_PROCESS_ASKROOMINFO, (WPARAM)pRoomInfo);

	return TRUE;
}

LRESULT CRoomListHeaderLowBD::OnListItemLDbClick(LSY::CMessage *msg)
{
	//규제안 : 대기실에서 클릭으로 방입장 금지
	//return TRUE;


	//관리자는 리스트 클릭 입장 가능
	if(GM().GetMyInfo()->UI.AdminGrade > 0) 
	{

		LSY::CMO_ListItemClick *pMsg = (LSY::CMO_ListItemClick *)msg;
		LSY::CListItemData *pData = pMsg->GetItemData();
		SMALLROOMINFO *pRoomInfo = (SMALLROOMINFO *)pData->GetItemData();

		CWnd *pWnd = GetObjectMan()->GetWnd();

		BOOL bObserve = ( (pRoomInfo->szClientSideVar[CS_JOINABLE]==0) || (pRoomInfo->cMaxUserNum <= pRoomInfo->cNowUserNum) );
		pWnd->SendMessage(ID_LOBBY_PROCESS_JOINROOM, (WPARAM)pRoomInfo, bObserve);

		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_ROOMLIST_ENTER_ROOM_DBCLK);
		#endif //UPGRADE_10210619_ADD_CLICKLOG
	}
	else if( GM().GetMyInfo()->UI.AdminGrade == 0)
	{
#if defined(_DEBUG)
		//방번호 클릭 접속
		LSY::CMO_ListItemClick *pMsg = (LSY::CMO_ListItemClick *)msg;
		LSY::CListItemData *pData = pMsg->GetItemData();
		SMALLROOMINFO *pRoomInfo = (SMALLROOMINFO *)pData->GetItemData();

		CWnd *pWnd = GetObjectMan()->GetWnd();
		BOOL bEnablePlay = ( pRoomInfo->cNowUserNum < pRoomInfo->cMaxUserNum ) ;
		BOOL bJoinable =(BOOL) pRoomInfo->szClientSideVar[CS_JOINABLE];
		BOOL bObserve = ( (pRoomInfo->szClientSideVar[CS_JOINABLE]==0) || (pRoomInfo->cMaxUserNum <= pRoomInfo->cNowUserNum) );
		if (bJoinable && bEnablePlay){
			pWnd->SendMessage(ID_LOBBY_PROCESS_JOINROOM, (WPARAM)pRoomInfo, bObserve);
		}
#endif
	}
	return TRUE;
}

LRESULT CRoomListHeaderLowBD::OnClickEnterRoom(LSY::CMessage *msg)
{
	//클라이언트 통계로그 (방목록 / 참여가능 버튼)
	SendUILog_Basic(CLSTATS_UINO_POKER_LOBBY_ROOMLIST_JOIN);

	NMBASE::SKINGDI::NMRESAM.PlayWav( "UI-SND_BUTTON_CLICK_B" );
	LSY::CListItemData *pData = GetListItemData(msg);
	if(pData == NULL) return FALSE;

	SMALLROOMINFO *pRoomInfo = (SMALLROOMINFO *)pData->GetItemData();

	CWnd *pWnd = GetObjectMan()->GetWnd();
	pWnd->SendMessage(ID_LOBBY_PROCESS_JOINROOM, (WPARAM)pRoomInfo, FALSE);

	return TRUE;
}

LRESULT CRoomListHeaderLowBD::OnClickObserverRoom(LSY::CMessage *msg)
{
	//클라이언트 통계로그 (방목록 / 관전 버튼)
	SendUILog_Basic(CLSTATS_UINO_POKER_LOBBY_ROOMLIST_OBSERVE);

	NMBASE::SKINGDI::NMRESAM.PlayWav( "UI-SND_BUTTON_CLICK_B" );
	LSY::CListItemData *pData = GetListItemData(msg);
	if(pData == NULL) return FALSE;

	SMALLROOMINFO *pRoomInfo = (SMALLROOMINFO *)pData->GetItemData();

	CWnd *pWnd = GetObjectMan()->GetWnd();
	pWnd->SendMessage(ID_LOBBY_PROCESS_JOINROOM, (WPARAM)pRoomInfo, TRUE);

	return TRUE;
}


void CRoomListHeaderLowBD::RemoveSelectedRoom(void)
{
	LSY::CListItemData* pListItemData = m_pList->GetFirstItem();

	while(pListItemData)
	{
		SMALLROOMINFO* pSRI = (SMALLROOMINFO*)pListItemData->GetItemData();
		if (pSRI->szClientSideVar[CS_SELECTED_ROOM])
		{
			pSRI->szClientSideVar[CS_SELECTED_ROOM] = FALSE;	
			ChangeItem(pSRI);
		}

		pListItemData = m_pList->GetNextItem();
	}
}


void CRoomListHeaderLowBD::ChangeRoomList()
{

	LSY::CSprite* pcSprite = (LSY::CSprite*)m_pTabRoomType->GetFirstChildFromID(ID_LOBBY_SPRITE_PROMOTION_VIP_COUPON_LOWBD);
	if(NULL!=pcSprite){
		if(GM().IsVIPCouponPromotion()){
			pcSprite->SetShow(true);
			pcSprite->StartAnimation(0);
		}
		else{
			pcSprite->SetShow(false);
			pcSprite->StopAnimation();
		}
	}
}