#include "StdAfx.h"
#include "ReverseBuddyListItem.h"

CReverseBuddyListItem::CReverseBuddyListItem()
:	m_pTxtBuddyNickName(NULL)
{
}

CReverseBuddyListItem::~CReverseBuddyListItem()
{
}

void CReverseBuddyListItem::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	switch(pObject->GetID())
	{
	case ID_LOBBY_TXT_MY_CLUB_NICKNAME:	m_pTxtBuddyNickName				= (LSY::CText *)pObject;			break;
	}
	__super::OnCreateChild(pObject);
}

void CReverseBuddyListItem::OnCreated(void)
{
	__super::OnCreated();
}

LSY::CObjectBase *CReverseBuddyListItem::NewObject(void)
{
	return new CReverseBuddyListItem;
}

void CReverseBuddyListItem::UpdateData(const LPARAM &lParam)
{
	//CReverseBuddy *pBuddy = (CReverseBuddy *)m_pData->GetItemData();
	//if(pBuddy == NULL) return;

	//m_pTxtBuddyNickName->SetText(pBuddy->GetNickName());
	
	LSY::CListItemData *pListItemData = (LSY::CListItemData*)GetData();
	if (pListItemData==NULL) return;

	CLUBMEMBERINFO *pMemberInfo = (CLUBMEMBERINFO*)pListItemData->GetItemData();
	if (pMemberInfo == NULL) return;
	
	CString str;
	str.Format("%s", pMemberInfo->MemberID.c_str());
	//m_pTxtNick->SetText(str.GetString());
	//m_pTxtMoney->SetText(NMBASE::UTIL::g_MakeShortMoneyStrEx(pMemberInfo->PMoney));
	//m_pTxtLastLogin->SetText(CClubDlg::MinuteToDHM(pMemberInfo->LastLoginTime));
	m_pTxtBuddyNickName->SetText(str.GetString());
}