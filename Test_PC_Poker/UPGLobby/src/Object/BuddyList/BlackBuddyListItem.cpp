#include "StdAfx.h"
#include "BlackBuddyListItem.h"

CBlackBuddyListItem::CBlackBuddyListItem()
:	m_pTxtBuddyNickName(NULL)
{
}

CBlackBuddyListItem::~CBlackBuddyListItem()
{
}

void CBlackBuddyListItem::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	switch(pObject->GetID())
	{
	case ID_LOBBY_MY_BLACKLIST_LISTITEM_TXT_NICKNAME:	m_pTxtBuddyNickName				= (LSY::CText *)pObject;			break;
	}
	__super::OnCreateChild(pObject);
}

void CBlackBuddyListItem::OnCreated(void)
{
	__super::OnCreated();
}

LSY::CObjectBase *CBlackBuddyListItem::NewObject(void)
{
	return new CBlackBuddyListItem;
}

void CBlackBuddyListItem::UpdateData(const LPARAM &lParam)
{
	CBlackBuddy *pBuddy = (CBlackBuddy *)m_pData->GetItemData();
	if(pBuddy == NULL) return;

	m_pTxtBuddyNickName->SetText(pBuddy->GetNickName());
}