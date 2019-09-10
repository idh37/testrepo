#include "StdAfx.h"
#include "BuddyListGroup.h"
#include "BuddyListItem.h"
#include "../popupmemuid.h"

CBuddyListGroup::CBuddyListGroup()
:	m_pBtnBuddyAdd(NULL),
	m_pBtnBuddyDelete(NULL),
	m_pBtnBuddyMoveGroup(NULL),
	m_pBtnBuddySendMemo(NULL),
	m_pBtnGroupAdd(NULL),
	m_pCheckShowConnectBuddy(NULL),
	m_pListBuddy(NULL)
{
}

CBuddyListGroup::~CBuddyListGroup()
{
}

void CBuddyListGroup::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	bool bAddChild = false;
	switch(pObject->GetID())
	{
	case ID_LOBBY_BTN_MYFRIEND_ADD:					m_pBtnBuddyAdd				= (LSY::CButton *)pObject;		bAddChild = true; break;
	case ID_LOBBY_BTN_MYFRIEND_DELETE:				m_pBtnBuddyDelete			= (LSY::CButton *)pObject;		bAddChild = true; break;
	case ID_LOBBY_BTN_MYFRIEND_MOVE:				m_pBtnBuddyMoveGroup		= (LSY::CButton *)pObject;		bAddChild = true; break;
	case ID_LOBBY_BTN_MYFRIEND_GROUP_ADD:			m_pBtnGroupAdd				= (LSY::CButton *)pObject;		bAddChild = true; break;
	case ID_LOBBY_BTN_MYFRIEND_SEND_MEMO:			m_pBtnBuddySendMemo			= (LSY::CButton *)pObject;		bAddChild = true; break;
	case ID_LOBBY_CHECKBOX_MYFRIEND_CONNECTUSER:	m_pCheckShowConnectBuddy	= (LSY::CCheckBox *)pObject;	break;
	case ID_LOBBY_LIST_MY_FRIEND:					m_pListBuddy				= (LSY::CList *)pObject;		break;
	}

	if(bAddChild)
	{
		pObject->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBuddyListGroup::OnClickButton));
	}

	__super::OnCreateChild(pObject);
}

void CBuddyListGroup::OnCreated(void)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);

	CCommunityManager::Instance()->AddListObjectBuddyGroupEntry(m_pListBuddy, true);

	m_pListBuddy->AddHandler(LSY::EM_O_LISTITEM_DBCLICK, LSY::Fnt(this, &CBuddyListGroup::OnListItemDbClick));
	m_pListBuddy->AddHandler(LSY::EM_O_LISTITEM_RCLICK, LSY::Fnt(this, &CBuddyListGroup::OnListItemRClick));
	m_pListBuddy->SetCompareFunction(0, (LSY::CList::LISTCOMPAREFUNCTION)&BuddyGroupEntryListCompareFunction);
	m_pListBuddy->SetSortType(0);

	CBuddyListItem *pSorceItem = (CBuddyListItem *)m_pListBuddy->GetItem();
	pSorceItem->m_pExpandGroup->AddHandler(LSY::EM_O_CHECKCHANGE, LSY::Fnt(pSorceItem, &CBuddyListItem::OnCheckChangeBuddyGroupExpand));
	pSorceItem->m_pBtnGroupNameChange->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(pSorceItem, &CBuddyListItem::OnClickButton));
	pSorceItem->m_pBtnGroupDelete->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(pSorceItem, &CBuddyListItem::OnClickButton));
	pSorceItem->m_pBtnGroupNameEditOK->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(pSorceItem, &CBuddyListItem::OnClickButton));
	pSorceItem->m_pBtnGroupNameEditCancel->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(pSorceItem, &CBuddyListItem::OnClickButton));

	pSorceItem->m_pEditBuddyGroupName->AddHandler(LSY::EM_O_FOCUSOUT, LSY::Fnt(pSorceItem, &CBuddyListItem::OnEditFocusOut));

	m_pCheckShowConnectBuddy->AddHandler(LSY::EM_O_CHECKCHANGE, LSY::Fnt(this, &CBuddyListGroup::OnCheckChange));
	__super::OnCreated();
}

LRESULT CBuddyListGroup::OnCheckChange(LSY::CMessage *msg)
{
	#ifdef UPGRADE_10210619_ADD_CLICKLOG
		SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_MYFRIEND_SHOW_ONLINE);
	#endif //UPGRADE_10210619_ADD_CLICKLOG

	LSY::CMO_CheckChange *pMsg = (LSY::CMO_CheckChange *)msg;
	CCommunityManager::Instance()->SetShowConnectBuddy(pMsg->GetValue());
	return TRUE;
}

void CBuddyListGroup::SetShowConnectBuddy(void)
{
	m_pCheckShowConnectBuddy->SetCheck(CCommunityManager::Instance()->IsShowConnectBuddy());
}

LRESULT CBuddyListGroup::OnClickButton(LSY::CMessage *msg)
{
	LSY::CObjectMessage *pMsg = (LSY::CObjectMessage *)msg;

	switch(pMsg->GetObject()->GetID())
	{
	case ID_LOBBY_BTN_MYFRIEND_ADD:
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_MYFRIEND_ADD_FRIEND);
		#endif //UPGRADE_10210619_ADD_CLICKLOG

		if(CCommunityManager::Instance()->IsConnect())
		{
			CBuddyAddDlg dlg;
			dlg.DoModal("", GM().GetMainWnd());
		}
		else
		{
			ShowMainMessageDlg("Ŀ�´�Ƽ ������ ����Ǿ� ���� �ʾ� ģ���� �߰��� �� �����ϴ�.");
		}
		break;
	case ID_LOBBY_BTN_MYFRIEND_DELETE:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_MYFRIEND_DELETE_SELECTED);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			LSY::CListItemData *pSelectItemData = m_pListBuddy->GetFirstSelectItem();
			if(pSelectItemData == NULL)
			{
				ShowMainMessageDlg("���õ� ģ���� �����ϴ�.");
				break;
			}

			CBuddyBase *pBuddyBase = (CBuddyBase *)pSelectItemData->GetItemData();
			if(pBuddyBase == NULL)
			{
				ShowMainMessageDlg("���õ� ģ���� �����ϴ�.");
				break;
			}

			if(!CCommunityManager::Instance()->IsConnect())
			{
				ShowMainMessageDlg("Ŀ�´�Ƽ ������ ����Ǿ� ���� �ʾ� ������ ģ���� ��ģ�� ��Ͽ��� ������ �� �����ϴ�.");
				break;
			}

			if(pBuddyBase->IsGroup()) break;
			CBuddyFriend *pBuddy = (CBuddyFriend *)pBuddyBase;

			CBuddyRemoveDlg dlg;
			if(dlg.DoModal(CBuddyRemoveDlg::BRDT_QUESTION,"",GM().GetMainWnd()) == IDOK)
			{
				std::map<std::string, std::string> mapIDAndGroup;
				CBuddyGroup *pGroup = pBuddy->GetGroup();
				mapIDAndGroup[pBuddy->GetID()] = pGroup->IsSendChangeName()?pGroup->GetEditName():pGroup->GetName();
				CCommunityManager::Instance()->SendRemoveBuddyFromGroup(mapIDAndGroup);
			}
		}
		break;
	case ID_LOBBY_BTN_MYFRIEND_MOVE:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_MYFRIEND_MOVE_SELECTED);
			#endif //UPGRADE_10210619_ADD_CLICKLOG		

			LSY::CListItemData *pSelectItemData = m_pListBuddy->GetFirstSelectItem();
			if(pSelectItemData == NULL)
			{
				ShowMainMessageDlg("���õ� ģ���� �����ϴ�.");
				break;
			}

			CBuddyBase *pBuddyBase = (CBuddyBase *)pSelectItemData->GetItemData();
			if(pBuddyBase == NULL)
			{
				ShowMainMessageDlg("���õ� ģ���� �����ϴ�.");
				break;
			}

			if(!CCommunityManager::Instance()->IsConnect())
			{
				ShowMainMessageDlg("Ŀ�´�Ƽ ������ ����Ǿ� ���� �ʾ� ���õ� ģ���� �׷��� �̵��� �� �����ϴ�.");
				break;
			}

			if(pBuddyBase->IsGroup()) break;
			CBuddyFriend *pBuddy = (CBuddyFriend *)pBuddyBase;
			CBuddyGroup *pGroup = pBuddy->GetGroup();

			CBuddyMoveGroupDlg dlg;
			dlg.DoModal(pBuddy->GetID(), pGroup->IsSendChangeName()?pGroup->GetEditName():pGroup->GetName(), GM().GetMainWnd());
		}
		break;
	case ID_LOBBY_BTN_MYFRIEND_GROUP_ADD:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_MYFRIEND_ADD_GROUP);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			CBuddyGroupAddDlg dlg;
			dlg.DoModal(GM().GetMainWnd());
		}
		break;
	case ID_LOBBY_BTN_MYFRIEND_SEND_MEMO:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_MYFRIEND_SEND_MEMO);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			if(!CCommunityManager::Instance()->IsConnect())
			{
				ShowMainMessageDlg("Ŀ�´�Ƽ ������ ����Ǿ� ���� �ʾ� ���õ� ģ������ ������ ���� �� �����ϴ�.");
				break;
			}

			LSY::CListItemData *pSelectItemData = m_pListBuddy->GetFirstSelectItem();
			if(pSelectItemData == NULL)
			{
				ShowMainMessageDlg("���õ� ģ���� �����ϴ�.");
				break;
			}

			CBuddyBase *pBuddyBase = (CBuddyBase *)pSelectItemData->GetItemData();
			if(pBuddyBase == NULL)
			{
				ShowMainMessageDlg("���õ� ģ���� �����ϴ�.");
				break;
			}			

			if(pBuddyBase->IsGroup()) break;
			CBuddyFriend *pBuddy = (CBuddyFriend *)pBuddyBase;
			CBuddyGroup *pGroup = pBuddy->GetGroup();

			ShowSendMessageDialog( pBuddy->GetNickName().c_str() );
		}
		break;
	}

	return TRUE;
}

LRESULT CBuddyListGroup::OnListItemDbClick(LSY::CMessage *msg)
{
	LSY::CMO_ListItemDbClick *pMsg = (LSY::CMO_ListItemDbClick *)msg;
	LSY::CListItemData *pData = pMsg->GetItemData();
	CBuddyBase *pBuddyBase = (CBuddyBase *)pData->GetItemData();

	if(pBuddyBase->IsGroup())
	{
	}
	else
	{
		CBuddyFriend *pBuddy = (CBuddyFriend *)pBuddyBase;

		std::list<std::string> listNIckName;
		listNIckName.push_back(pBuddy->GetID());
		CCommunityManager::Instance()->SendGetUserInfo(CCommunityUserInfo::ECUIT_SHOWPROFILE, NULL, listNIckName,"");
	}

	return TRUE;
}

LRESULT CBuddyListGroup::OnListItemRClick(LSY::CMessage *msg)
{
	LSY::CMO_ListItemRClick *pMsg = (LSY::CMO_ListItemRClick *)msg;
	LSY::CListItemData *pData = pMsg->GetItemData();
	CBuddyBase *pBuddyBase = (CBuddyBase *)pData->GetItemData();
	
	m_pListBuddy->SetItemSelectFromIndex(pData->GetItemIndex(), true);
	
	CWnd *pWnd = GetObjectMan()->GetWnd();
	
	LSY::CPoint ptPos = GetObjectMan()->GetCurMousePos();
	POINT ptTempPos = ptPos.GetPoint();
	pWnd->ClientToScreen(&ptTempPos);
	
	CPopupMemu *pPopupMemu = new CPopupMemu;
	CLobbyView *pLobbyWnd = (CLobbyView *)GetLobbyWindow();
	
	if(pBuddyBase->IsGroup())
	{
		CBuddyGroup *pBuddyGroup = (CBuddyGroup *)pBuddyBase;
		bool bEmpty = pBuddyGroup->IsEmpty();
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_BUDDYGROUP_SENDMEMO, "�׷������߼�", bEmpty?false:true);
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_BUDDYGROUP_SENDMULTICHAT, "�׷��ȭ", bEmpty?false:true);

		pLobbyWnd->SetSelectID("");
		pLobbyWnd->SetSelectNickName(pBuddyGroup->GetName());

		pPopupMemu->Create(ptTempPos.x, ptTempPos.y);
	}
	else
	{
		CBuddyFriend *pBuddy = (CBuddyFriend *)pBuddyBase;
		
		bool bOffline = pBuddy->IsOffline();
		bool bBlackList = CCommunityManager::Instance()->IsExistBlackBuddy(pBuddy->GetNickName())?true:false;
		bool bInGameRoom = bOffline?false:(pBuddy->GetRoomNumber()==0)?false:true;

		/*
		// Ŭ�� ��� ����
		int nMyGrade = GM().GetMyInfo()->UI.ClubMemberGrade;
		int nUserGrade = pUserInfo->ClubMemberGrade;	// ��� ��������................
		// ���� ������ �ƴҰ�� Ŭ�� ���Կ�û ��ư���� ����
		CString csInviteClub = "Ŭ���ʴ�";
		if (nMyGrade != CLUB_MEMBER_GRADE_BOSS)
			csInviteClub = "Ŭ�� ���Կ�û";
		// ���� ������ ��밡 �̰��� ���¸� Ŭ���ʴ� ��ư Ȱ��ȭ
		bool bActiveInviteClub = false;
		if (bOffline)
			bActiveInviteClub = false;
		else if (nMyGrade == CLUB_MEMBER_GRADE_BOSS &&
			nUserGrade == CLUB_MEMBER_GRADE_NONE)
			bActiveInviteClub = true;
		// ���� Ŭ�� �̰��� �����̰� ��밡 �����϶� Ŭ�� ���Կ�û ��ư Ȱ��ȭ
		else if (nMyGrade == CLUB_MEMBER_GRADE_NONE &&
			nUserGrade == CLUB_MEMBER_GRADE_BOSS)
			bActiveInviteClub = true;
		*/
		
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_PROFILE, "�����ʺ���", true);
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_WHISPER, "�Ӹ��ϱ�", false);
		//pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_INVITE_CLUB, "Ŭ���ʴ�", bOffline?false:true);
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_MEMO, "����������", true);
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_DELETE, "ģ������", true);
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_MOVE, "ģ���̵�", true);

		if(bBlackList) pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_REMOVE_BLACKLIST, "������Ʈ����", true);
		else pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_ADD_BLACKLIST, "������Ʈ�߰�", true);

		// ���ھ� ���� �����ϱ� ������ ���� ����.
		// pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_BUDDY_TOGETHER_GAME, "����������", bInGameRoom);

		pLobbyWnd->SetSelectID(pBuddy->GetID());
		pLobbyWnd->SetSelectNickName(pBuddy->GetNickName());

		pPopupMemu->Create(ptTempPos.x, ptTempPos.y);
	}

	GM().GetMainWnd()->SendMessage(UPGM_POPUPMEMU_OPEN, 0, (LPARAM)pPopupMemu);

	return TRUE;
}

void CBuddyListGroup::OnReceiveBuddyGroupEntryList(void)
{
	CCommunityManager::Instance()->SetBuddyGroupListEntryToListObject();
}

void CBuddyListGroup::OnReceiveBuddyGroupNameChange(void)
{
	CCommunityManager::Instance()->SetBuddyGroupNameChange();
}

void CBuddyListGroup::OnReceiveBuddyGroupAdd(void)
{
	CCommunityManager::Instance()->SetAddBuddyGroupToListObject();
}

void CBuddyListGroup::OnReceiveBuddyGroupRemove(void)
{
	CCommunityManager::Instance()->SetRemoveBuddyGroupFromListObject();
}