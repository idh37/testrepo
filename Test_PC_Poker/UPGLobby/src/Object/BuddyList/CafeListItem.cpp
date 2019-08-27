#include "StdAfx.h"
#include "CafeListItem.h"

CCafeListItem::CCafeListItem(void)
{
	m_bExpFnd = false;
}

CCafeListItem::~CCafeListItem(void)
{
}

void CCafeListItem::OnCreateChild(LSY::CObjectBase *pObject)
{
	if (pObject == NULL)
		return;

	switch (pObject->GetID())
	{
	case ID_LOBBY_GRP_MY_CAFE:
		m_pGrpCafe = (LSY::CGroup *)pObject;
		break;
	case ID_LOBBY_CHKBTN_MY_CAFE_EXPAND_GROUP:
		m_pChkBtnCafeExpandGroup = (LSY::CCheckButton *)pObject;
		break;
	case ID_LOBBY_GRP_MY_CAFE_SUB:
		m_pGrpCafeSub = (LSY::CGroup *)pObject;
		break;
	case ID_LOBBY_TXT_MY_CAFE_GRP_NAME:
		m_pTxtCafeGrpName = (LSY::CText *)pObject;
		break;
	case ID_LOBBY_GRP_MY_CAFE_MEM:
		m_pGrpCafeMem = (LSY::CGroup *)pObject;
		break;
	case ID_LOBBY_IMG_MY_CAFE_ONOFFLINE:
		m_pImgCafeOnOffline = (LSY::CImage *)pObject;
		break;
	case ID_LOBBY_TXT_MY_CAFE_MEM_NAME:
		m_pTxtCafeMemName = (LSY::CText *)pObject;
		break;
	case ID_LOBBY_TXT_MY_CAFE_OFFLINE:
		m_pTxtCafeOffline = (LSY::CText *)pObject;
		break;
	}

	__super::OnCreateChild(pObject);
}

void CCafeListItem::OnCreated(void)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);

	OnCreateChild(m_pGrpCafe->GetFirstChildFromID(ID_LOBBY_CHKBTN_MY_CAFE_EXPAND_GROUP));
	OnCreateChild(m_pGrpCafe->GetFirstChildFromID(ID_LOBBY_GRP_MY_CAFE_SUB));
	OnCreateChild(m_pGrpCafeSub->GetFirstChildFromID(ID_LOBBY_TXT_MY_CAFE_GRP_NAME));

	OnCreateChild(m_pGrpCafeMem->GetFirstChildFromID(ID_LOBBY_IMG_MY_CAFE_ONOFFLINE));
	OnCreateChild(m_pGrpCafeMem->GetFirstChildFromID(ID_LOBBY_TXT_MY_CAFE_MEM_NAME));
	OnCreateChild(m_pGrpCafeMem->GetFirstChildFromID(ID_LOBBY_TXT_MY_CAFE_OFFLINE));

	__super::OnCreated();
}

LSY::CObjectBase *CCafeListItem::NewObject(void)
{
	return new CCafeListItem;
}

void CCafeListItem::UpdateData(const LPARAM &lParam)
{
	CAFEMEMBERINFO *pMemInfo = (CAFEMEMBERINFO *)m_pData->GetItemData();

	if (pMemInfo == NULL)
		return;

	if (pMemInfo->bGroup)
	{
		UpdateCafeGroup((CAFEMEMBERINFO *)pMemInfo);
	}
	else
	{
		UpdateCafeMember((CAFEMEMBERINFO *)pMemInfo);
	}
}

LRESULT CCafeListItem::OnCheckChangeCafeGroupExpand(LSY::CMessage *pMsg)
{
	LSY::CMO_CheckChange *pChange = (LSY::CMO_CheckChange *)pMsg;
	CCafeListItem *pItem = (CCafeListItem *)LSY::GetListItem(pChange);

	if (pItem == NULL)
		return FALSE;

	CAFEMEMBERINFO *pMemInfo = (CAFEMEMBERINFO *)pItem->GetData()->GetItemData();

	if (pMemInfo == NULL)
		return FALSE;

	bool bExpend = pItem->m_pChkBtnCafeExpandGroup->IsCheck();

	ChangeExpendList(pMemInfo->typeGroup, bExpend);

	AllAddCafe(pItem->GetList());

	return TRUE;
}

LRESULT CCafeListItem::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseLClick *pClick = (LSY::CMO_MouseLClick *)pMsg;
	CCafeListItem *pItem = (CCafeListItem *)LSY::GetListItem(pClick);

	if (pItem == NULL)
		return FALSE;

	CBuddyGroup *pBuddyGroup = (CBuddyGroup *)pItem->GetData()->GetItemData();

	if (pBuddyGroup == NULL)
		return FALSE;

	/*
	switch(pMsg->GetObject()->GetID())
	{
	case ID_LOBBY_BTN_FRIEND_LISTITEM_GROUP_NAME_CHANGE:
		if(pBuddyGroup->IsSendChangeName())
		{
			ShowMainMessageDlg("그룹명 변경 요청중인 그룹으로 서버에서 처리가 이루어지지 않아 그룹명을 변경할 수 없습니다. 잠시 후 다시 시도해주세요.");
		}
		else if(CCommunityManager::Instance()->IsConnect())
		{
			pBuddyGroup->SetEdit(true);
			pItem->UpdateBuddyGroup(pBuddyGroup);
		}
		else
		{
			ShowMainMessageDlg("커뮤니티 서버와의 연결이 되어있지 않아 그룹명 변경할 수 없습니다.");
		}
		break;
	}
	*/

	return TRUE;
}

void CCafeListItem::AllAddCafe(LSY::CList *pList)
{
	pList->DeleteAllItem();

	std::list<CAFEMEMBERINFO *>::iterator start = m_listMemInfo.begin();
	std::list<CAFEMEMBERINFO *>::iterator end = m_listMemInfo.end();
	CAFEMEMBERINFO *pMemInfo = NULL;

	for (; start != end; ++start)
	{
		pMemInfo = *start;
		
		if ((pMemInfo->bGroup == true || pMemInfo->bExpend == true)
			&& (m_bExpFnd == false || pMemInfo->bBuddy == false))
			pList->AddItem((LPARAM)pMemInfo);
	}
}

void CCafeListItem::UpdateCafeGroup(CAFEMEMBERINFO *pMemInfo)
{
	m_pGrpCafe->SetShow(true);
	m_pGrpCafeMem->SetShow(false);

	m_pChkBtnCafeExpandGroup->SetCheck(pMemInfo->bExpend);

	m_pTxtCafeGrpName->SetText(pMemInfo->sName);
}

void CCafeListItem::UpdateCafeMember(CAFEMEMBERINFO *pMemInfo)
{
	m_pGrpCafe->SetShow(false);
	m_pGrpCafeMem->SetShow(true);

	bool bOnline = (pMemInfo->nRoomNum >= 0);
	bool bBlackList = CCommunityManager::Instance()->IsExistBlackBuddyByID(pMemInfo->sName) ? true : false;

	if (bOnline)
	{
		m_pImgCafeOnOffline->SetIndex(bBlackList ? 47 : 45);
		m_pTxtCafeOffline->SetShow(false);
	}
	else
	{
		m_pImgCafeOnOffline->SetIndex(bBlackList ? 48 : 46);
		m_pTxtCafeOffline->SetShow(true);
	}
	
	m_pTxtCafeMemName->SetText(pMemInfo->sName);
}

void CCafeListItem::ChangeExpendList(CafeSystem::CafeKind::Type groupType, bool bExpend)
{
	std::list<CAFEMEMBERINFO *>::iterator start = m_listMemInfo.begin();
	std::list<CAFEMEMBERINFO *>::iterator end = m_listMemInfo.end();
	CAFEMEMBERINFO *pMemInfo = NULL;

	for (; start != end; ++start)
	{
		pMemInfo = *start;

		if (pMemInfo->typeGroup == groupType)
			pMemInfo->bExpend = bExpend;
	}
}