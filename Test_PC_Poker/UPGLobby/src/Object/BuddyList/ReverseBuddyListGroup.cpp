#include "StdAfx.h"
#include "ReverseBuddyListGroup.h"
#include "ReverseBuddyListItem.h"
#include "../popupmemuid.h"

bool ReverseBuddyListCompareFunction(LSY::CListItemData *pItem1, LSY::CListItemData *pItem2, bool bIncrement, LPARAM lParam)
{
	CReverseBuddy *pBuddy1 = (CReverseBuddy *)pItem1->GetItemData();
	CReverseBuddy *pBuddy2 = (CReverseBuddy *)pItem2->GetItemData();

	return strcmp(pBuddy1->GetNickName().c_str(), pBuddy2->GetNickName().c_str()) < 0;
}

CReverseBuddyListGroup::CReverseBuddyListGroup()
:	m_pBtnBuddyAdd(NULL),
	m_pCheckBoxShowNotFriend(NULL),
	m_pListBuddy(NULL)
{
}

CReverseBuddyListGroup::~CReverseBuddyListGroup()
{
}

void CReverseBuddyListGroup::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	bool bAddChild = false;
	switch(pObject->GetID())
	{
	case ID_LOBBY_BTN_MY_ADDFRIEND_ADD:
		m_pBtnBuddyAdd = (LSY::CButton *)pObject;
		bAddChild = true;
		break;
	case ID_LOBBY_CHECKBOX_MY_ADDFRIEND_SHOW_NOTFRIEND:
		m_pCheckBoxShowNotFriend = (LSY::CCheckBox *)pObject;
		break;
	case ID_LOBBY_LIST_MY_ADDFRIEND:
		m_pListBuddy = (LSY::CList *)pObject;
		break;
	case ID_LOBBY_RAD_MY_CLUB:
		m_pRadioClub = (LSY::CRadio *)pObject;
		break;
	}

	if(bAddChild)
	{
		pObject->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CReverseBuddyListGroup::OnClickButton));
	}

	__super::OnCreateChild(pObject);
}

void CReverseBuddyListGroup::OnCreated(void)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);

	CCommunityManager::Instance()->SetListObjectResersseBuddy(m_pListBuddy);

	m_pListBuddy->AddHandler(LSY::EM_O_LISTITEM_DBCLICK, LSY::Fnt(this, &CReverseBuddyListGroup::OnListItemDbClick));
	m_pListBuddy->AddHandler(LSY::EM_O_LISTITEM_RCLICK, LSY::Fnt(this, &CReverseBuddyListGroup::OnListItemRClick));
	//m_pListBuddy->SetCompareFunction(0, (LSY::CList::LISTCOMPAREFUNCTION)&ReverseBuddyListCompareFunction);
	//m_pListBuddy->SetSortType(0);

	//m_pCheckBoxShowNotFriend->AddHandler(LSY::EM_O_CHECKCHANGE, LSY::Fnt(this, &CReverseBuddyListGroup::OnCheckChange));
	
	
	__super::OnCreated();
}

LRESULT CReverseBuddyListGroup::OnCheckChange(LSY::CMessage *msg)
{
	#ifdef UPGRADE_10210619_ADD_CLICKLOG
		SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_FRIEND_ADDED_ME_SHOW_IGNORE_FRIEND);
	#endif //UPGRADE_10210619_ADD_CLICKLOG

	LSY::CMO_CheckChange *pMsg = (LSY::CMO_CheckChange *)msg;
	CCommunityManager::Instance()->SetShowIgnoreMyBuddy(pMsg->GetValue());
	return TRUE;
}

void CReverseBuddyListGroup::SetShowIgnoreMyBuddy(void)
{
	m_pCheckBoxShowNotFriend->SetCheck(CCommunityManager::Instance()->IsShowIgnoreMyBuddy());
}

LRESULT CReverseBuddyListGroup::OnClickButton(LSY::CMessage *msg)
{
	LSY::CObjectMessage *pMsg = (LSY::CObjectMessage *)msg;

	switch(pMsg->GetObject()->GetID())
	{
	case ID_LOBBY_BTN_MY_ADDFRIEND_ADD:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_FRIEND_ADDED_ME_ADD_FRIEND);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			LSY::CListItemData *pItemData = m_pListBuddy->GetFirstSelectItem();
			if(pItemData == NULL)
			{
				ShowMainMessageDlg("선택된 친구가 없습니다.");
				break;
			}

			CReverseBuddy *pBuddy = (CReverseBuddy *)pItemData->GetItemData();
			if(pBuddy == NULL)
			{
				ShowMainMessageDlg("선택된 친구가 없습니다.");
				break;
			}

			if(!CCommunityManager::Instance()->IsConnect())
			{
				ShowMainMessageDlg("커뮤니티 서버와 연결되어 있지 않아 선택된 친구를 내친구 목록에 추가할 수 없습니다.");
				break;
			}

			CBuddyAddDlg dlg;
			dlg.DoModal(pBuddy->GetNickName(), GM().GetMainWnd());
		}
		break;
	}

	return TRUE;
}

LRESULT CReverseBuddyListGroup::OnListItemDbClick(LSY::CMessage *msg)
{
	LSY::CMO_ListItemDbClick *pMsg = (LSY::CMO_ListItemDbClick *)msg;
	LSY::CListItemData *pData = pMsg->GetItemData();
	CReverseBuddy *pBuddy = (CReverseBuddy *)pData->GetItemData();

	if(pBuddy)
	{
		std::list<std::string> listNIckName;
		listNIckName.push_back(pBuddy->GetID());
		CCommunityManager::Instance()->SendGetUserInfo(CCommunityUserInfo::ECUIT_SHOWPROFILE, NULL, listNIckName,"");
	}

	return TRUE;
}

LRESULT CReverseBuddyListGroup::OnListItemRClick(LSY::CMessage *msg)
{
	LSY::CMO_ListItemRClick *pMsg = (LSY::CMO_ListItemRClick *)msg;
	LSY::CListItemData *pData = pMsg->GetItemData();
	//SMALLUSERINFO *pUserInfo = (SMALLUSERINFO *)pData->GetItemData();

	CLUBMEMBERINFO *pUserInfo = (CLUBMEMBERINFO*)pData->GetItemData();
	if (pUserInfo == NULL) return FALSE;

	memcpy(&m_stSelectUserInfo, pUserInfo, sizeof(SMALLUSERINFO));
	CWnd *pWnd = GetObjectMan()->GetWnd();

	LSY::CPoint ptPos = GetObjectMan()->GetCurMousePos();
	POINT ptTempPos = ptPos.GetPoint();
	pWnd->ClientToScreen(&ptTempPos);

	CPopupMemu *pPopupMemu = new CPopupMemu;
	pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_PROFILE_CLUB, "프로필보기", true);
	
	// 나를 선택
	if(strcmp(pUserInfo->MemberID.c_str(), GM().GetMyInfo()->UI.ID) == 0)
	{
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_SEND_MEMO, "쪽지보내기", false);
		//pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_SEND_WHISPER, "귓말보내기", false);
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_ADD_BUDDY, "친구추가", false);
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_ADD_BLACK_LIST, "블랙리스트추가", false);
	}
	else
	{
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_SEND_MEMO, "쪽지보내기", true);
		//pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_SEND_WHISPER, "귓말보내기", true);
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_ADD_BUDDY, "친구추가", CCommunityManager::Instance()->IsExistBuddy(pUserInfo->MemberID)?false:true);
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_ADD_BLACK_LIST, "블랙리스트추가", CCommunityManager::Instance()->IsExistBlackBuddy(pUserInfo->MemberID)?false:true);
	}

	//관리자 아이디이면 아이디 복사하기 메뉴를 보이도록 한다.
	if(GM().GetMyInfo()->UI.AdminGrade)
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_COPY_ID, "아이디복사하기", true);

	CLobbyView *pLobbyWnd = (CLobbyView *)GetLobbyWindow();
	pLobbyWnd->SetSelectID(pUserInfo->MemberID);
	pLobbyWnd->SetSelectNickName(pUserInfo->MemberID);

	//SetSelectID(pUserInfo->ID);
	//SetSelectNickName(pUserInfo->NickName);
	pPopupMemu->Create(ptTempPos.x, ptTempPos.y);
	GM().GetMainWnd()->SendMessage(UPGM_POPUPMEMU_OPEN, 0, (LPARAM)pPopupMemu);





/*
	CReverseBuddy *pBuddy = (CReverseBuddy *)pData->GetItemData();

	m_pListBuddy->SetItemSelectFromIndex(pData->GetItemIndex(), true);

	CWnd *pWnd = GetObjectMan()->GetWnd();

	LSY::CPoint ptPos = GetObjectMan()->GetCurMousePos();
	POINT ptTempPos = ptPos.GetPoint();
	pWnd->ClientToScreen(&ptTempPos);

	bool bBuddy = CCommunityManager::Instance()->IsExistBuddy(pBuddy->GetNickName())?true:false;
	bool bBlackList = CCommunityManager::Instance()->IsExistBlackBuddy(pBuddy->GetNickName())?true:false;

	CPopupMemu *pPopupMemu = new CPopupMemu;
	pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_REVERSE_BUDDY_PROFILE, "프로필보기", true);
	pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_REVERSE_BUDDY_ADD_BUDDY, "친구추가", bBuddy?false:true);
	pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_REVERSE_BUDDY_ADD_BLACKLIST, "블랙리스트추가", bBlackList?false:true);

	CLobbyView *pLobbyWnd = (CLobbyView *)GetLobbyWindow();
	pLobbyWnd->SetSelectID(pBuddy->GetID());
	pLobbyWnd->SetSelectNickName(pBuddy->GetNickName());

	pPopupMemu->Create(ptTempPos.x, ptTempPos.y);

	GM().GetMainWnd()->SendMessage(UPGM_POPUPMEMU_OPEN, 0, (LPARAM)pPopupMemu);
*/
	return TRUE;
}

void CReverseBuddyListGroup::OnReceiveReverseBuddyList(void)
{
	CCommunityManager::Instance()->SetReverseBuddyListToListObject();
}

/// <summary>
/// 서버에서 보내는 패킷(응답) 처리
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CReverseBuddyListGroup::HandleResponse(int Signal, int TotSize, char *lpData)
{
	switch(Signal)
	{
	case SV_CLUB_RESPONSE_DETAIL_INFO:
		{
			RemoveAllMember();
			
			CSV_CLUB_RESPONSE_DETAIL_INFO msg;
			msg.Get(lpData, TotSize);
			
			for(int i = 0; i < *msg.m_Club_MemberCount; i++)					
			{
				if (msg.m_ArrayClubMemberInfo == NULL)
				{
					ShowMainMessageDlg("정보를 가져올 수 없습니다.");
					
					continue;
				}
				AddMember(&msg.m_ArrayClubMemberInfo[i]); // 클럽원 리스트
			}
		}
		break;
	}
}

/// <summary>
/// 클럽원 리스트 전체 삭제
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CReverseBuddyListGroup::RemoveAllMember(void)
{
	m_pListBuddy->DeleteAllItem();
}

/// <summary>
/// 클럽원 리스트에 컬럼 추가
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CReverseBuddyListGroup::AddMember(CLUB_MEMBER_INFO *pMemInfo)
{
	CLUBMEMBERINFO* memInfo = new CLUBMEMBERINFO();
	memInfo->MemberID = pMemInfo->MemberID;
	//memInfo->PMoney = pMemInfo->PMoney;
	//memInfo->LastLoginTime = pMemInfo->LastLoginTime;

	

	m_pListBuddy->AddItem((LPARAM)memInfo);
}
