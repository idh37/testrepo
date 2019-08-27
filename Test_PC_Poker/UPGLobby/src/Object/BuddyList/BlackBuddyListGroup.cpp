#include "StdAfx.h"
#include "BlackBuddyListGroup.h"
#include "BlackBuddyListItem.h"
#include "../popupmemuid.h"

bool BlackBuddyListCompareFunction(LSY::CListItemData *pItem1, LSY::CListItemData *pItem2, bool bIncrement, LPARAM lParam)
{
	CBlackBuddy *pBuddy1 = (CBlackBuddy *)pItem1->GetItemData();
	CBlackBuddy *pBuddy2 = (CBlackBuddy *)pItem2->GetItemData();

	return strcmp(pBuddy1->GetNickName().c_str(), pBuddy2->GetNickName().c_str()) < 0;
}

CBlackBuddyListGroup::CBlackBuddyListGroup()
:	m_pBtnBuddyAdd(NULL),
	m_pBtnBuddyDelete(NULL),
	m_pListBuddy(NULL)
{
}

CBlackBuddyListGroup::~CBlackBuddyListGroup()
{
}

void CBlackBuddyListGroup::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	bool bAddChild = false;
	switch(pObject->GetID())
	{
	case ID_LOBBY_BTN_MY_BLACKLIST_ADD:			m_pBtnBuddyAdd				= (LSY::CButton *)pObject;		bAddChild = true; break;
	case ID_LOBBY_BTN_MY_BLACKLIST_DELETE:		m_pBtnBuddyDelete			= (LSY::CButton *)pObject;		bAddChild = true; break;
	case ID_LOBBY_LIST_MY_BLACKLIST:			m_pListBuddy				= (LSY::CList *)pObject;		break;
	}

	if(bAddChild)
	{
		pObject->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBlackBuddyListGroup::OnClickButton));
	}

	__super::OnCreateChild(pObject);
}

void CBlackBuddyListGroup::OnCreated(void)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
	
	CCommunityManager::Instance()->SetListObjectBlackBuddy(m_pListBuddy);
	
	m_pListBuddy->AddHandler(LSY::EM_O_LISTITEM_DBCLICK, LSY::Fnt(this, &CBlackBuddyListGroup::OnListItemDbClick));
	m_pListBuddy->AddHandler(LSY::EM_O_LISTITEM_RCLICK, LSY::Fnt(this, &CBlackBuddyListGroup::OnListItemRClick));
	m_pListBuddy->SetCompareFunction(0, (LSY::CList::LISTCOMPAREFUNCTION)&BlackBuddyListCompareFunction);
	m_pListBuddy->SetSortType(0);
	
	__super::OnCreated();
}

LRESULT CBlackBuddyListGroup::OnClickButton(LSY::CMessage *msg)
{
	LSY::CObjectMessage *pMsg = (LSY::CObjectMessage *)msg;

	switch(pMsg->GetObject()->GetID())
	{
	case ID_LOBBY_BTN_MY_BLACKLIST_ADD:
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_BLACKLIST_ADD_BLACKLIST);
		#endif //UPGRADE_10210619_ADD_CLICKLOG

		if(CCommunityManager::Instance()->IsConnect())
		{
			CBlackBuddyAddDlg dlg;
			dlg.DoModal(CBlackBuddyAddDlg::BBADT_EDIT, "", GM().GetMainWnd());
		}
		else
		{
			ShowMainMessageDlg("커뮤니티 서버에 연결되어 있지 않아 블랙리스트를 추가할수 없습니다.");
		}
		break;
	case ID_LOBBY_BTN_MY_BLACKLIST_DELETE:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_BLACKLIST_DELETE_BLACKLIST);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			CBlackBuddyRemoveDlg dlg;

			LSY::CListItemData *pItemData = m_pListBuddy->GetFirstSelectItem();
			if(pItemData == NULL)
			{
				dlg.DoModal(CBlackBuddyRemoveDlg::BBRDT_MESSAGE, "선택한 사용자가 없습니다.\n삭제할 사용자를 선택 후 사용해주세요.", GM().GetMainWnd());
				break;
			}

			CReverseBuddy *pBuddy = (CReverseBuddy *)pItemData->GetItemData();
			if(pBuddy == NULL)
			{
				dlg.DoModal(CBlackBuddyRemoveDlg::BBRDT_MESSAGE, "선택한 사용자가 없습니다.\n삭제할 사용자를 선택 후 사용해주세요.", GM().GetMainWnd());
				break;
			}

			if(!CCommunityManager::Instance()->IsConnect())
			{
				dlg.DoModal(CBlackBuddyRemoveDlg::BBRDT_MESSAGE, "선택한 사용자가 없습니다.\n삭제할 사용자를 선택 후 사용해주세요.", GM().GetMainWnd());
				break;
			}

			if(dlg.DoModal(CBlackBuddyRemoveDlg::BBRDT_QUESTION, "", GM().GetMainWnd()) == IDOK)
			{
				CCommunityManager::Instance()->SendRemoveBlackBuddyList(pBuddy->GetID());
			}
		}
		break;
	}

	return TRUE;
}

LRESULT CBlackBuddyListGroup::OnListItemRClick(LSY::CMessage *msg)
{
	LSY::CMO_ListItemRClick *pMsg = (LSY::CMO_ListItemRClick *)msg;
	LSY::CListItemData *pData = pMsg->GetItemData();
	CBlackBuddy *pBlackBuddy = (CBlackBuddy *)pData->GetItemData();

	m_pListBuddy->SetItemSelectFromIndex(pData->GetItemIndex(), true);

	CWnd *pWnd = GetObjectMan()->GetWnd();

	LSY::CPoint ptPos = GetObjectMan()->GetCurMousePos();
	POINT ptTempPos = ptPos.GetPoint();
	pWnd->ClientToScreen(&ptTempPos);

	CPopupMemu *pPopupMemu = new CPopupMemu;
	pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_BLACKBUDDY_PROFILE, "프로필보기", true);
	pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_BLACKBUDDY_REMOVE, "블랙리스트삭제", true);

	CLobbyView *pLobbyWnd = (CLobbyView *)GetLobbyWindow();
	pLobbyWnd->SetSelectID(pBlackBuddy->GetID());
	pLobbyWnd->SetSelectNickName(pBlackBuddy->GetNickName());

	pPopupMemu->Create(ptTempPos.x, ptTempPos.y);

	GM().GetMainWnd()->SendMessage(UPGM_POPUPMEMU_OPEN, 0, (LPARAM)pPopupMemu);

	return TRUE;
}

LRESULT CBlackBuddyListGroup::OnListItemDbClick(LSY::CMessage *msg)
{
	LSY::CMO_ListItemDbClick *pMsg = (LSY::CMO_ListItemDbClick *)msg;
	LSY::CListItemData *pData = pMsg->GetItemData();
	CBlackBuddy *pBuddy = (CBlackBuddy *)pData->GetItemData();

	if(pBuddy)
	{
		std::list<std::string> listNIckName;
		listNIckName.push_back(pBuddy->GetID());
		CCommunityManager::Instance()->SendGetUserInfo(CCommunityUserInfo::ECUIT_SHOWPROFILE, NULL, listNIckName,"");
	}

	return TRUE;
}

void CBlackBuddyListGroup::OnReceiveBlackBuddyList(void)
{
	CCommunityManager::Instance()->SetBlackBuddyListToListObject();
}