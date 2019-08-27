#include "StdAfx.h"
#include "UserListItem.h"
#include "../../WidgetID.h"

CUserListItem::CUserListItem()
:	m_pNickName(NULL),
	m_pHaveMoney(NULL),
	m_pGameWhere(NULL),
	m_pRoomNumber(NULL)
{
}

CUserListItem::~CUserListItem()
{
}

void CUserListItem::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	switch(pObject->GetID())
	{
	case ID_LOBBY_TXT_USERLISTITEM_NICKNAME:	m_pNickName				= (LSY::CText *)pObject;		break;
	case ID_LOBBY_TXTNB_USERLISTITEM_HAVEMONEY:	m_pHaveMoney			= (LSY::CTextNumber *)pObject;	break;
	case ID_LOBBY_TXT_USERLISTITEM_GAME:		m_pGameWhere			= (LSY::CText *)pObject;		break;
	case ID_LOBBY_TXT_USERLISTITEM_ROOMNUMBER:	m_pRoomNumber			= (LSY::CText *)pObject;		break;
	}
	__super::OnCreateChild(pObject);
}

void CUserListItem::OnCreated(void)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
	__super::OnCreated();
}

LSY::CObjectBase *CUserListItem::NewObject(void)
{
	return new CUserListItem;
}

void CUserListItem::UpdateData(const LPARAM &lParam)
{
	SMALLUSERINFO *pUserInfo = (SMALLUSERINFO *)m_pData->GetItemData();
	if(pUserInfo == NULL) return;

	USERINFO* pFullUserInfo = &GM().GetMyInfo()->UI;

	if(pFullUserInfo == NULL)
	{
		return;
	}	

	INT64 userMarkHaveMoney = g_CheckMarkedExceedingLimitHaveMoney(pFullUserInfo->GetRealPMoney(), pFullUserInfo->PremMenuNo);

	m_pNickName->SetText(pUserInfo->NickName);
	m_pHaveMoney->SetNumber(userMarkHaveMoney);
	IDX_GAME idxGame = (IDX_GAME)pUserInfo->nGameIndex;
	CGame *pGame = GM().GetGame(idxGame);
	if(pGame)
	{
		m_pGameWhere->SetTextColor(pGame->GetGameColor());

		m_pGameWhere->SetText(pGame->GetGameName().GetString());	
	}

	if(pUserInfo->RoomNum == 0)
	{
		m_pRoomNumber->SetText("대기실");
	}
	else
	{
		CString strText;
		strText.Format("%d번", pUserInfo->RoomNum);
		m_pRoomNumber->SetText(strText.GetString());
	}
}