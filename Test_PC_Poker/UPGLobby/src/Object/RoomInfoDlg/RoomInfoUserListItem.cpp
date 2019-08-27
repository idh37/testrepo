#include "StdAfx.h"
#include "RoomInfoUserListItem.h"
#include "../../WidgetID.h"

CRoomInfoUserListItem::CRoomInfoUserListItem()
:	m_pNickName(NULL),
	m_pHaveMoney(NULL)
{
}

CRoomInfoUserListItem::~CRoomInfoUserListItem()
{
}

void CRoomInfoUserListItem::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	switch(pObject->GetID())
	{
	case ID_LOBBY_TXT_ROOMINFO_LISTITEM_NICKNAME:		m_pNickName				= (LSY::CText *)pObject;		break;
	case ID_LOBBY_TXTNB_ROOMINFO_LISTITEM_HAVEMONEY:	m_pHaveMoney			= (LSY::CTextNumber *)pObject;	break;
	}
	__super::OnCreateChild(pObject);
}

void CRoomInfoUserListItem::OnCreated(void)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
	__super::OnCreated();
}

LSY::CObjectBase *CRoomInfoUserListItem::NewObject(void)
{
	return new CRoomInfoUserListItem;
}

void CRoomInfoUserListItem::UpdateData(void)
{
	SMALLUSERINFO *pUserInfo = (SMALLUSERINFO *)m_pData->GetItemData();
	if(pUserInfo == NULL) return;

	m_pNickName->SetText((IDX_GAME)pUserInfo->nGameIndex == IDX_GAME_HA ? pUserInfo->ID : pUserInfo->NickName);
	m_pHaveMoney->SetNumber(pUserInfo->PMoney);
	m_pHaveMoney->SetAddText(strChip_Name[GM().getRoomListTabState() == 2 ? 1 : 0]);
}