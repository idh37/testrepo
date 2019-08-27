#include "StdAfx.h"
#include "RoomListGroup.h"
#include "../../WidgetID.h"
#include "RoomListHeader.h"
#include "RoomListHeaderLowBD.h"
#include "RoomListHeaderHoola.h"
#include "RoomListHeaderJoin.h"
#include "RoomListHeaderLowBDJoin.h"
#include "RoomListHeaderHoolaJoin.h"

CRoomListGroup::CRoomListGroup()
:	m_pHeader(NULL),
	m_pHeaderLowBD(NULL),
	m_pHeaderHoola(NULL),
	m_pHeaderJoin(NULL),
	m_pHeaderLowBDJoin(NULL),
	m_pHeaderHoolaJoin(NULL),
	m_pSprLoading(NULL),
	m_i64EnterRoomMinimumMoney(0)
{
}

CRoomListGroup::~CRoomListGroup()
{
}

void CRoomListGroup::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;

	switch(pObject->GetID())
	{
	case ID_LOBBY_GRP_ROOMLIST_HEADER:				m_pHeader				= (CRoomListHeader *)pObject;			break;
	case ID_LOBBY_GRP_ROOMLIST_HEADER_LOWBD:		m_pHeaderLowBD			= (CRoomListHeaderLowBD *)pObject;		break;
	case ID_LOBBY_GRP_ROOMLIST_HEADER_HOOLA:		m_pHeaderHoola			= (CRoomListHeaderHoola *)pObject;		break;
	case ID_LOBBY_GRP_ROOMLIST_HEADER_JOIN:			m_pHeaderJoin			= (CRoomListHeaderJoin *)pObject;			break;
	case ID_LOBBY_GRP_ROOMLIST_HEADER_LOWBD_JOIN:	m_pHeaderLowBDJoin		= (CRoomListHeaderLowBDJoin *)pObject;		break;
	case ID_LOBBY_GRP_ROOMLIST_HEADER_HOOLA_JOIN:	m_pHeaderHoolaJoin		= (CRoomListHeaderHoolaJoin *)pObject;		break;
	case ID_LOBBY_CHECKBOX:							m_pJoinable_ckbtn		= (LSY::CCheckButton *)pObject;			break;
	case ID_LOBBY_SPR_LOBBY_LOADING:
		{
			m_pSprLoading			= (LSY::CSprite *)pObject;
			m_pSprLoading->StopAnimation();
		}
		break;
	}

	__super::OnCreateChild(pObject);
}

void CRoomListGroup::OnCreated(void)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
	
	SetGameType(GM().GetCurrentGameType());

	//g_pObjectMan
	m_pJoinable_ckbtn->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CRoomListGroup::OnRoomTypeSelChange));

	__super::OnCreated();
}

LRESULT CRoomListGroup::OnRoomTypeSelChange(LSY::CMessage *msg)
{
	SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_ROOMLIST_SHOW_ROOM_WITH_MYMONEY);
	bool bstate = m_pJoinable_ckbtn->IsCheck();
	
	m_pHeader->SetJoinable( bstate );
 	m_pHeaderLowBD->SetJoinable( bstate );
	m_pHeaderHoola->SetJoinable( bstate );
	m_pHeaderJoin->SetJoinable( bstate );
	m_pHeaderLowBDJoin->SetJoinable( bstate );
	m_pHeaderHoolaJoin->SetJoinable( bstate );
 	

	m_pHeader->AddDataRoomList();
	m_pHeaderLowBD->AddDataRoomList();
	m_pHeaderHoola->AddDataRoomList();
	m_pHeaderJoin->AddDataRoomList();
	m_pHeaderLowBDJoin->AddDataRoomList();
	m_pHeaderHoolaJoin->AddDataRoomList();


	return TRUE;
}


void CRoomListGroup::ShowLoadingRoom(bool bShow)
{
	if(bShow)
	{
		if(!m_pSprLoading->IsAnimation())
		{
			m_pSprLoading->StartAnimation();
			m_pHeader->SetListEnable(false);
			m_pHeaderLowBD->SetListEnable(false);
			m_pHeaderHoola->SetListEnable(false);
			m_pHeaderJoin->SetListEnable(false);
			m_pHeaderLowBDJoin->SetListEnable(false);
			m_pHeaderHoolaJoin->SetListEnable(false);
		}
	}
	else if(m_pSprLoading->IsAnimation())
	{
		m_pSprLoading->StopAnimation();
		m_pHeader->SetListEnable(true);
		m_pHeaderLowBD->SetListEnable(true);
		m_pHeaderHoola->SetListEnable(true);
		m_pHeaderJoin->SetListEnable(true);
		m_pHeaderLowBDJoin->SetListEnable(true);
		m_pHeaderHoolaJoin->SetListEnable(true);
	}
}

void CRoomListGroup::SortDefault(void)
{
	if( GM().GetCurrentGameType() == IDX_GAME_BD ) m_pHeaderLowBD->SortDefault();
	else if( GM().GetCurrentGameType() == IDX_GAME_HA ) m_pHeaderHoola->SortDefault();
	else m_pHeader->SortDefault();
}

void CRoomListGroup::RemoveAll(void)
{
	m_pHeader->RemoveAll();
	m_pHeaderLowBD->RemoveAll();
	m_pHeaderHoola->RemoveAll();
	m_pHeaderJoin->RemoveAll();
	m_pHeaderLowBDJoin->RemoveAll();
	m_pHeaderHoolaJoin->RemoveAll();
}

void CRoomListGroup::AddItem(SMALLROOMINFO *pRoomInfo)
{
	if( GM().GetCurrentGameType() == IDX_GAME_BD ){
		m_pHeaderLowBD->AddItem( pRoomInfo );
		m_pHeaderLowBDJoin->AddItem( pRoomInfo );
	}
	else if( GM().GetCurrentGameType() == IDX_GAME_HA ){
		m_pHeaderHoola->AddItem( pRoomInfo );
		m_pHeaderHoolaJoin->AddItem( pRoomInfo );
	}
	else{
		m_pHeader->AddItem(pRoomInfo);
		m_pHeaderJoin->AddItem(pRoomInfo);
	}
}

void CRoomListGroup::DelItem(SMALLROOMINFO *pRoomInfo)
{
	if( GM().GetCurrentGameType() == IDX_GAME_BD ){
		m_pHeaderLowBD->DelItem( pRoomInfo );
		m_pHeaderLowBDJoin->DelItem( pRoomInfo );
	}
	else if( GM().GetCurrentGameType() == IDX_GAME_HA ){
		m_pHeaderHoola->DelItem( pRoomInfo );
		m_pHeaderHoolaJoin->DelItem( pRoomInfo );
	}
	else{
		m_pHeader->DelItem(pRoomInfo);
		m_pHeaderJoin->DelItem(pRoomInfo);
	}
}

void CRoomListGroup::ChangeItem(SMALLROOMINFO *pRoomInfo)
{
	if( GM().GetCurrentGameType() == IDX_GAME_BD ){
		m_pHeaderLowBD->ChangeItem( pRoomInfo );
		m_pHeaderLowBDJoin->ChangeItem( pRoomInfo );
	}
	else if( GM().GetCurrentGameType() == IDX_GAME_HA ){
		m_pHeaderHoola->ChangeItem( pRoomInfo );
		m_pHeaderHoolaJoin->ChangeItem( pRoomInfo );
	}
	else{
		m_pHeader->ChangeItem(pRoomInfo);
		m_pHeaderJoin->ChangeItem(pRoomInfo);
	}
}
void CRoomListGroup::ChangeRoomListState(void)
{
	m_pHeader->ChangeRoomList();
	m_pHeaderLowBD->ChangeRoomList();
	m_pHeaderHoola->ChangeRoomList();
	m_pHeaderJoin->ChangeRoomList();
	m_pHeaderLowBDJoin->ChangeRoomList();
	m_pHeaderHoolaJoin->ChangeRoomList();
}

void CRoomListGroup::SetGameType(IDX_GAME nType)
{
	m_pHeader->ChangeRoomList();
	m_pHeaderLowBD->ChangeRoomList();
	m_pHeaderHoola->ChangeRoomList();
	m_pHeaderJoin->ChangeRoomList();
	m_pHeaderLowBDJoin->ChangeRoomList();
	m_pHeaderHoolaJoin->ChangeRoomList();
	switch(nType)
	{
	case IDX_GAME_SP:
	case IDX_GAME_NP:
	case IDX_GAME_SD:
		{
			m_pHeaderJoin->SetShow( GM().getRoomListTabState()!=1 );
			m_pHeaderLowBDJoin->SetShow( false );
			m_pHeaderHoolaJoin->SetShow( false );
			m_pHeader->SetShow( GM().getRoomListTabState()==1 );
			m_pHeaderLowBD->SetShow( false );
			m_pHeaderHoola->SetShow( false );
 			
			LSY::CTextButton *pButton = (LSY::CTextButton *)m_pHeader->GetFirstChildFromID(ID_LOBBY_TXTBTN_ROOMLIST_HEADER_SEEDMONEY);
  			if(nType == IDX_GAME_SD)
 			{
 				pButton->SetText("시드");
 			}
 			else
 			{
 				pButton->SetText("시드/게임룰");
			}

			pButton = (LSY::CTextButton *)m_pHeaderJoin->GetFirstChildFromID(ID_LOBBY_TXTBTN_ROOMLIST_HEADER_SEEDMONEY_JOIN);
			if(pButton){
				if(nType == IDX_GAME_SD)
				{
					pButton->SetText("시드");
				}
				else
				{
					pButton->SetText("시드/게임룰");
				}
			}

		} break;

	case IDX_GAME_HL:
		{
			m_pHeaderJoin->SetShow( GM().getRoomListTabState()!=1 );
			m_pHeaderLowBDJoin->SetShow( false );
			m_pHeaderHoolaJoin->SetShow( false );
			m_pHeader->SetShow( GM().getRoomListTabState()==1 );
			m_pHeaderLowBD->SetShow( false );
			m_pHeaderHoola->SetShow( false );
			LSY::CTextButton *pButton = (LSY::CTextButton *)m_pHeader->GetFirstChildFromID(ID_LOBBY_TXTBTN_ROOMLIST_HEADER_SEEDMONEY);
			pButton->SetText("시드/게임룰");

			pButton = (LSY::CTextButton *)m_pHeaderJoin->GetFirstChildFromID(ID_LOBBY_TXTBTN_ROOMLIST_HEADER_SEEDMONEY_JOIN);
			pButton->SetText("시드/게임룰");
		} break;

	case IDX_GAME_BD:
		{
			m_pHeaderJoin->SetShow( false );
			m_pHeaderLowBDJoin->SetShow( GM().getRoomListTabState()!=1 );
			m_pHeaderHoolaJoin->SetShow( false );
			m_pHeader->SetShow( false );
			m_pHeaderLowBD->SetShow( GM().getRoomListTabState()==1 );
			m_pHeaderHoola->SetShow( false );
		} break;

	case IDX_GAME_HA:
		{
			m_pHeaderJoin->SetShow( false );
			m_pHeaderLowBDJoin->SetShow( false );
			m_pHeaderHoolaJoin->SetShow( false );
			m_pHeader->SetShow( false );
			m_pHeaderLowBD->SetShow( false );
			m_pHeaderHoola->SetShow( true );
		}
		break;
	default:
		{
			m_pHeaderJoin->SetShow( false );
			m_pHeaderLowBDJoin->SetShow( false );
			m_pHeaderHoolaJoin->SetShow( false );
			m_pHeader->SetShow(true);
			m_pHeaderLowBD->SetShow( false );
			m_pHeaderHoola->SetShow( false );
		} break;
	}
}

void CRoomListGroup::ChangeRoomTab(ROOMLIST_TYPE eRoomListType)
{
	m_pHeader->ChangeRoomTab( eRoomListType );
	m_pHeaderLowBD->ChangeRoomTab( eRoomListType );
	m_pHeaderHoola->ChangeRoomTab( eRoomListType );
	m_pHeaderJoin->ChangeRoomTab( eRoomListType );
	m_pHeaderLowBDJoin->ChangeRoomTab( eRoomListType );
	m_pHeaderHoolaJoin->ChangeRoomTab( eRoomListType );
	SortDefault();
}

void CRoomListGroup::ChangeGoldTabState(bool bGold, int listType)
{
	if(m_pHeaderJoin==NULL || m_pHeader==NULL || m_pHeaderLowBDJoin==NULL || m_pHeaderLowBD==NULL)
		return;

	int tabIndex = listType-1;

	switch(GM().GetCurrentGameType())
	{
	case IDX_GAME_SP:
	case IDX_GAME_NP:
	case IDX_GAME_SD:
	case IDX_GAME_HL:
		{
			m_pHeaderJoin->SetShow( bGold );
			m_pHeader->SetShow( !bGold );

			if(bGold)
			{
				m_pHeaderJoin->m_pTabRoomType->SetCurSelFromIndex(tabIndex);
				m_pHeaderJoin->ChangeRoomTab((ROOMLIST_TYPE)listType);
			}
			else
			{
				m_pHeader->m_pTabRoomType->SetCurSelFromIndex(tabIndex);
				m_pHeader->ChangeRoomTab((ROOMLIST_TYPE)listType);
			}
		} break;
	case IDX_GAME_BD:
		{
			m_pHeaderLowBDJoin->SetShow( bGold );
			m_pHeaderLowBD->SetShow( !bGold );

			if(bGold)
			{
				m_pHeaderLowBDJoin->m_pTabRoomType->SetCurSelFromIndex(tabIndex);
				m_pHeaderLowBDJoin->ChangeRoomTab((ROOMLIST_TYPE)listType);
			}
			else
			{
				m_pHeaderLowBD->m_pTabRoomType->SetCurSelFromIndex(tabIndex);
				m_pHeaderLowBD->ChangeRoomTab((ROOMLIST_TYPE)listType);
			}
		} break;
	}
}

void CRoomListGroup::SortEnterRoomMinimumMoney(INT64 i64Money)
{
	if( GM().GetCurrentGameType() == IDX_GAME_BD ){
		m_pHeaderLowBD->SortEnterRoomMinimumMoney( &i64Money );
		m_pHeaderLowBDJoin->SortEnterRoomMinimumMoney( &i64Money );
	}
	else if( GM().GetCurrentGameType() == IDX_GAME_HA ){
		m_pHeaderHoola->SortEnterRoomMinimumMoney( &i64Money );
		m_pHeaderHoolaJoin->SortEnterRoomMinimumMoney( &i64Money );
	}
	else{
		m_pHeader->SortEnterRoomMinimumMoney(&i64Money);
		m_pHeaderJoin->SortEnterRoomMinimumMoney(&i64Money);
	}
}

//

void CRoomListGroup::RemoveSelectedRoom(void)
{
	//선택방을 지운다.
	if( GM().GetCurrentGameType() == IDX_GAME_BD ){
		m_pHeaderLowBD->RemoveSelectedRoom();
		m_pHeaderLowBDJoin->RemoveSelectedRoom();
	}
	else if( GM().GetCurrentGameType() == IDX_GAME_HA ){
		m_pHeaderHoola->RemoveSelectedRoom();
		m_pHeaderHoolaJoin->RemoveSelectedRoom();
	}
	else{
		m_pHeader->RemoveSelectedRoom();
		m_pHeaderJoin->RemoveSelectedRoom();
	}

}
