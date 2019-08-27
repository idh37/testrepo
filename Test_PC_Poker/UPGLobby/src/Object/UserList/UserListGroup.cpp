#include "StdAfx.h"
#include "UserListGroup.h"
#include "UserListItem.h"
#include "../../WidgetID.h"
#include "../../resource.h"
#include "../../PopupMemu.h"
#include "../../PopupMemuID.h"
#include "../../LobbyView.h"

#define ADD_MONEYRNAGE_TO_COMBO(min, max) mr[addcnt].MinMoney = min; mr[addcnt].MaxMoney = max; pMoneyCmbox->AddString(mr[addcnt].GetStrMoneyRange_for_Combobox2(true), (DWORD)&mr[addcnt]); addcnt++;

CUserListGroup::CUserListGroup()
:	m_pComboGame(NULL),
	m_pComboMoney(NULL),
	m_pHeader(NULL),
	m_pList(NULL),
	m_pSprLoading(NULL),
	m_pEditFindUser(NULL),
	m_pBtnFindUser(NULL),
	m_pEditFindRoomNumber(NULL),
	m_pBtnFindRoomNumber(NULL),
	m_pBtnHeaderNickName(NULL),
	m_pBtnHeaderHaveMoney(NULL),
	m_pBtnHeaderPosition(NULL)
{
	m_stMoneyRange.resize(3);
	m_stMoneyRange[0].MinMoney = 0;
	m_stMoneyRange[0].MaxMoney = 0;

	m_stMoneyRange[1].MinMoney = 1000000000000;
	m_stMoneyRange[1].MaxMoney = 0;

	m_stMoneyRange[2].MinMoney = 0;
	m_stMoneyRange[2].MaxMoney = 1000000000000-1;

	m_nCurSortType = DEF_USERLIST_SORT_UNKNOWN;
	m_mapSortTypeIncrement[DEF_USERLIST_SORT_NICKNAME] = true;
	m_mapSortTypeIncrement[DEF_USERLIST_SORT_HAVEMONEY] = true;
	m_mapSortTypeIncrement[DEF_USERLIST_SORT_POSITION] = true;

	MM().AddMsgH(UPG_CMK_SHOW_ADDUSERSELECTDLG, Fnt(this, &CUserListGroup::OnAddUserSelectDlg));
	MM().AddMsgH(UPG_CMK_CLOSE_ADDUSERSELECTDLG, Fnt(this, &CUserListGroup::OnSubUserSelectDlg));
	MM().AddMsgH(UPG_CMK_IS_SHOW_LOADING_USERLIST, Fnt(this, &CUserListGroup::OnIsShowLoadingUserList));
	MM().AddMsgH(UPG_CMK_ADDUSERSELECTDLG_SELCHANGE_GAME, Fnt(this, &CUserListGroup::OnAddUserSelectDlgSelChangeGame));
	MM().AddMsgH(UPG_CMK_ADDUSERSELECTDLG_SELCHANGE_MONEY, Fnt(this, &CUserListGroup::OnAddUserSelectDlgSelChangeMoney));

	m_nSelectRoomNum = 0;

	m_dwTime_refresh = 0;
}

CUserListGroup::~CUserListGroup()
{
}

LRESULT CUserListGroup::OnAddUserSelectDlg(WPARAM &wParam, LPARAM &lParam)
{
	CAddUserSelectDlg *pDlg = (CAddUserSelectDlg *)lParam;
	ASSERT(pDlg != NULL);
	std::list<CAddUserSelectDlg *>::iterator start = m_listAddUserSelectDlg.begin();
	std::list<CAddUserSelectDlg *>::iterator end = m_listAddUserSelectDlg.end();
	std::list<CAddUserSelectDlg *>::iterator findIt = find(start, end, pDlg);
	if(findIt != end) return TRUE;
	m_listAddUserSelectDlg.push_back(pDlg);
	int nIndex = m_pComboGame->GetCurSelectItem();
	pDlg->SetComboSynchronization((IDX_GAME)m_pComboGame->GetItemData(nIndex), m_pComboMoney->GetCurSelectItem());
	return TRUE;
}

LRESULT CUserListGroup::OnSubUserSelectDlg(WPARAM &wParam, LPARAM &lParam)
{
	CAddUserSelectDlg *pDlg = (CAddUserSelectDlg *)lParam;
	ASSERT(pDlg != NULL);
	std::list<CAddUserSelectDlg *>::iterator start = m_listAddUserSelectDlg.begin();
	std::list<CAddUserSelectDlg *>::iterator end = m_listAddUserSelectDlg.end();
	std::list<CAddUserSelectDlg *>::iterator findIt = find(start, end, pDlg);
	if(findIt == end) return TRUE;
	m_listAddUserSelectDlg.erase(findIt);
	return TRUE;
}

LRESULT CUserListGroup::OnIsShowLoadingUserList(WPARAM &wParam, LPARAM &lParam)
{
	return m_pSprLoading->IsAnimation()?1:0;
}

LRESULT CUserListGroup::OnAddUserSelectDlgSelChangeGame(WPARAM &wParam, LPARAM &lParam)
{
	IDX_GAME nGame = (IDX_GAME)lParam;

	int nCurSel = m_pComboGame->GetCurSelectItem();
	if(m_pComboGame->GetItemData(nCurSel) != nGame)
	{
		int nCount = m_pComboGame->GetItemCount();
		int nSel = 0;
		for(int i=0;i<nCount;++i)
		{
			if(m_pComboGame->GetItemData(i) == nGame)
			{
				nSel = i;
				break;
			}
		}
		m_pComboGame->SetCurSelectItem(nSel);

		/*ResetMoneyComboBox(nGame, GM().GetMyInfo()->UI.GetMoney(IsTournament()));*/
		/*ResetMoneyComboBox(nGame, GM().GetMyInfo()->UI.GetMoney());*/
		ResetMoneyComboBox(nGame, GM().GetMyInfo()->UI.GetRealPMoney());
	}

	std::list<CAddUserSelectDlg *>::iterator start = m_listAddUserSelectDlg.begin();
	std::list<CAddUserSelectDlg *>::iterator end = m_listAddUserSelectDlg.end();

	CAddUserSelectDlg *pDlg;
	for(;start != end;++start)
	{
		pDlg = *start;
		pDlg->SetComboSynchronization(nGame, m_pComboMoney->GetCurSelectItem());
	}

	CWnd *pWnd = GetObjectMan()->GetWnd();
	pWnd->SendMessage(LUV_COMBO_GAME, nGame);

	return TRUE;
}

LRESULT CUserListGroup::OnAddUserSelectDlgSelChangeMoney(WPARAM &wParam, LPARAM &lParam)
{
	m_pComboMoney->SetCurSelectItem((int)lParam);
	int nCurSel = m_pComboGame->GetCurSelectItem();
	IDX_GAME nGame = (IDX_GAME)m_pComboGame->GetItemData(nCurSel);

	std::list<CAddUserSelectDlg *>::iterator start = m_listAddUserSelectDlg.begin();
	std::list<CAddUserSelectDlg *>::iterator end = m_listAddUserSelectDlg.end();

	CAddUserSelectDlg *pDlg;
	for(;start != end;++start)
	{
		pDlg = *start;
		pDlg->SetComboSynchronization(nGame, m_pComboMoney->GetCurSelectItem());
	}

	CWnd *pWnd = GetObjectMan()->GetWnd();
	pWnd->SendMessage(LUV_COMBO_MONEY);

	return TRUE;
}


void CUserListGroup::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	bool bAddChild = false;
	switch(pObject->GetID())
	{
	case ID_LOBBY_CB_USERLIST_GAME:					m_pComboGame			= (LSY::CCombo *)pObject;		break;
	case ID_LOBBY_CB_USERLIST_HAVEMONEY:			m_pComboMoney			= (LSY::CCombo *)pObject;		break;
	case ID_LOBBY_GRP_USERLIST_HEADER:				m_pHeader				= (LSY::CGroup *)pObject;		break;
	case ID_LOBBY_LIST_USER:						m_pList					= (LSY::CList *)pObject;		break;
	case ID_LOBBY_SPR_USER_LOADING:
		m_pSprLoading			= (LSY::CSprite *)pObject;
		m_pSprLoading->StopAnimation();
		break;
	case ID_LOBBY_EDIT_FIND_USER:					m_pEditFindUser			= (LSY::CEdit *)pObject;		break;
	case ID_LOBBY_BTN_FIND_USER:					m_pBtnFindUser			= (LSY::CButton *)pObject;		bAddChild = true; break;
	case ID_LOBBY_EDIT_FIND_ROOM_NUMBER:			m_pEditFindRoomNumber	= (LSY::CEdit *)pObject;		break;
	case ID_LOBBY_BTN_FIND_ROOM_NUMBER:				m_pBtnFindRoomNumber	= (LSY::CButton *)pObject;		bAddChild = true; break;
	case ID_LOBBY_TXTBTN_USERLIST_HEADER_NICKNAME:	m_pBtnHeaderNickName	= (LSY::CButton *)pObject;		bAddChild = true; break;
	case ID_LOBBY_TXTBTN_USERLIST_HEADER_HAVEMONEY:	m_pBtnHeaderHaveMoney	= (LSY::CButton *)pObject;		bAddChild = true; break;
	case ID_LOBBY_TXTBTN_USERLIST_HEADER_POSITION:	m_pBtnHeaderPosition	= (LSY::CButton *)pObject;		bAddChild = true; break;
	}

	if(bAddChild)
	{
		pObject->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CUserListGroup::OnClickButton));
	}

	__super::OnCreateChild(pObject);
}

void CUserListGroup::OnCreated(void)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);

	OnCreateChild(pProject->GetObject(ID_LOBBY_CB_USERLIST_GAME));
	OnCreateChild(pProject->GetObject(ID_LOBBY_CB_USERLIST_HAVEMONEY));
	OnCreateChild(pProject->GetObject(ID_LOBBY_GRP_USERLIST_HEADER));
	OnCreateChild(pProject->GetObject(ID_LOBBY_LIST_USER));
	OnCreateChild(pProject->GetObject(ID_LOBBY_SPR_USER_LOADING));

	int nCount = 1;
	int idxGame = 0;
	for(idxGame=0; idxGame<MAX_GAME; idxGame++ )
	{
		if (GM().GetGame((IDX_GAME) idxGame)) ++nCount;
	}

	m_pComboGame->SetTotalHeightUseLineHeight(nCount);
	m_pComboGame->AddText("전체게임");
	m_pComboGame->SetItemData(0, IDX_GAME_NULL);
	CGame *pGame = NULL;
	for(idxGame=IDX_GAME_SP; idxGame<MAX_GAME; ++idxGame )
	{
		pGame = GM().GetGame((IDX_GAME) idxGame);
		if (pGame)
		{
			nCount = m_pComboGame->AddText(pGame->GetGameName().GetString());
			m_pComboGame->SetItemData(nCount, idxGame);
		}
	}

	m_pComboGame->SetCurSelectItem(0);

	m_pComboGame->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CUserListGroup::OnSelChangeGame));
	m_pComboMoney->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CUserListGroup::OnSelChangeMoney));

	OnCreateChild(m_pHeader->GetFirstChildFromID(ID_LOBBY_TXTBTN_USERLIST_HEADER_NICKNAME));
	OnCreateChild(m_pHeader->GetFirstChildFromID(ID_LOBBY_TXTBTN_USERLIST_HEADER_HAVEMONEY));
	OnCreateChild(m_pHeader->GetFirstChildFromID(ID_LOBBY_TXTBTN_USERLIST_HEADER_POSITION));

	m_pList->SetCompareFunction(DEF_USERLIST_SORT_NICKNAME, (LSY::CList::LISTCOMPAREFUNCTION)&UserListNickNameCompareFunction);
	m_pList->SetCompareFunction(DEF_USERLIST_SORT_HAVEMONEY, (LSY::CList::LISTCOMPAREFUNCTION)&UserListHaveMoneyCompareFunction);
	m_pList->SetCompareFunction(DEF_USERLIST_SORT_POSITION, (LSY::CList::LISTCOMPAREFUNCTION)&UserListPositionCompareFunction);

	m_pList->AddHandler(LSY::EM_O_LISTITEM_DBCLICK, LSY::Fnt(this, &CUserListGroup::OnListItemDbClick));
	m_pList->AddHandler(LSY::EM_O_LISTITEM_RCLICK, LSY::Fnt(this, &CUserListGroup::OnListItemRClick));

	m_pEditFindUser->SetText("");
	m_pEditFindRoomNumber->SetText("");

	m_pEditFindUser->AddHandler(LSY::EM_O_RETURN, LSY::Fnt(this, &CUserListGroup::OnReturnEdit));
	m_pEditFindRoomNumber->AddHandler(LSY::EM_O_RETURN, LSY::Fnt(this, &CUserListGroup::OnReturnEdit));

	LSY::CTabControl *pTabUser = (LSY::CTabControl *)pProject->GetObject(ID_LOBBY_TAB_USERGROUP);
	pTabUser->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CUserListGroup::OnSelChangeTabUser));

	LSY::CTabControl *pTabFriend = (LSY::CTabControl *)pProject->GetObject(ID_LOBBY_TAB_FRIEND);
	pTabFriend->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CUserListGroup::OnSelChangeTabFriend));

	//m_cUserMenu.LoadMenu(IDR_USERMENU);

	__super::OnCreated();
}

LRESULT CUserListGroup::OnSelChangeGame(LSY::CMessage *msg)
{
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();
	IDX_GAME nGame = (IDX_GAME)m_pComboGame->GetItemData(nIndex);
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)nGame;
	OnAddUserSelectDlgSelChangeGame(wParam, lParam);
	return TRUE;
}

LRESULT CUserListGroup::OnSelChangeMoney(LSY::CMessage *msg)
{
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();
	WPARAM wParam = 0;
	LPARAM lParam = (LPARAM)nIndex;
	OnAddUserSelectDlgSelChangeMoney(wParam, lParam);
	return TRUE;
}

void CUserListGroup::ResetGameComboBox(IDX_GAME nGame, INT64 i64MyMoney)
{
	if(nGame < 0 || nGame >= MAX_GAME)
		return;

	IDX_GAME nSelGame = IDX_GAME_NULL;
	//맞포커인 경우 게임콤보의 맞포커 선택
	int nCurSel = m_pComboGame->GetCurSelectItem();
	
	/*
	if(m_pComboGame->GetItemData(nCurSel) != nGame)
	{
		int nCount = m_pComboGame->GetItemCount();
		int nSel = 0;
		for(int i=0;i<nCount;++i)
		{
			if(m_pComboGame->GetItemData(i) == nGame)
			{
				nSel = i;
				break;
			}
		}

		//본인이 실행한 게임의 접속자 유저 리스트를 불러온다.
		m_pComboGame->SetCurSelectItem(nSel);

		//입장머니 콤보박스 리셋
		ResetMoneyComboBox(nGame, i64MyMoney);
	}
	*/

	//Reset이 실행되면 무조건 전체게임 리스트로 보여준다:2012.10.19 YS.Jeong
	//1 작업유저의 목록이 우선순위로 보여지는 것을 제거하기 위함.
	//2 접속중인 유저가 많아 보이는 효과
	int nCount = m_pComboGame->GetItemCount();
	int nSel = 0;
	for(int i=0;i<nCount;++i)
	{
		if(m_pComboGame->GetItemData(i) == nGame)
		{
			nSel = i;
			break;
		}
	}
	
	//콤보의 0번 아이템(전체게임)을 불러온다.
	m_pComboGame->SetCurSelectItem(0);

	//입장머니 콤보박스 리셋
	ResetMoneyComboBox(nGame, i64MyMoney);



	std::list<CAddUserSelectDlg *>::iterator start = m_listAddUserSelectDlg.begin();
	std::list<CAddUserSelectDlg *>::iterator end = m_listAddUserSelectDlg.end();

	CAddUserSelectDlg *pDlg;
	for(;start != end;++start)
	{
		pDlg = *start;
		pDlg->ResetGameComboBox(nGame, i64MyMoney);
	}
}

void CUserListGroup::ResetMoneyComboBox(IDX_GAME nGame, INT64 i64Money)
{
	m_pComboMoney->RemoveAll();
	m_pComboMoney->SetTotalHeightUseLineHeight(2);

	int nCount = (int)m_stMoneyRange.size();
	int nSel = 0;
	for(int i=1;i<nCount;++i)
	{
		m_pComboMoney->AddText(m_stMoneyRange[i].GetStrMoneyRange_for_Combobox2(true).GetString());
		m_pComboMoney->SetItemData(i-1, (DWORD_PTR)(&m_stMoneyRange[i]));
		if(m_stMoneyRange[i].IsInRange(i64Money))
		{
			nSel = i-1;
		}
	}

	m_pComboMoney->SetCurSelectItem(nSel);

	std::list<CAddUserSelectDlg *>::iterator start = m_listAddUserSelectDlg.begin();
	std::list<CAddUserSelectDlg *>::iterator end = m_listAddUserSelectDlg.end();

	CAddUserSelectDlg *pDlg;
	for(;start != end;++start)
	{
		pDlg = *start;
		pDlg->ResetMoneyComboBox(nGame, i64Money);
	}
}

MONEYRANGE *CUserListGroup::GetCurrentUserListMoneyRange(void)
{
	int nSel = m_pComboMoney->GetCurSelectItem();
	if(nSel < 0) return NULL;
	return (MONEYRANGE *)m_pComboMoney->GetItemData(nSel);
}

IDX_GAME CUserListGroup::GetCurrentUserListGame()
{
	int nSel = m_pComboGame->GetCurSelectItem();
	if(nSel < 0) return IDX_GAME_NULL;
	return (IDX_GAME)m_pComboGame->GetItemData(nSel);
}

void CUserListGroup::ShowLoadingUser(bool bShow)
{
	if(bShow)
	{
		if(!m_pSprLoading->IsAnimation())
		{
			m_pSprLoading->StartAnimation();
			m_pList->SetEnable(false);
		}
	}
	else if(m_pSprLoading->IsAnimation())
	{
		m_pSprLoading->StopAnimation();
		m_pList->SetEnable(true);

		//접속중 유저 리스트의 기본값을 셔플로 보여준다. 2012.10.22 YS.Jeong
		//SortDefault();
		SortShuffle();
	}

	std::list<CAddUserSelectDlg *>::iterator start = m_listAddUserSelectDlg.begin();
	std::list<CAddUserSelectDlg *>::iterator end = m_listAddUserSelectDlg.end();

	CAddUserSelectDlg *pDlg;
	for(;start != end;++start)
	{
		pDlg = *start;
		pDlg->ShowLoading(bShow);
	}
}

void CUserListGroup::RemoveAll(void)
{
	m_pList->DeleteAllItem();

	std::list<CAddUserSelectDlg *>::iterator start = m_listAddUserSelectDlg.begin();
	std::list<CAddUserSelectDlg *>::iterator end = m_listAddUserSelectDlg.end();

	CAddUserSelectDlg *pDlg;
	for(;start != end;++start)
	{
		pDlg = *start;
		pDlg->RemoveAllConnectUser();
	}
}

void CUserListGroup::AddItem(SMALLUSERINFO *pUserInfo)
{
	m_pList->AddItem((LPARAM)pUserInfo);
	
	std::list<CAddUserSelectDlg *>::iterator start = m_listAddUserSelectDlg.begin();
	std::list<CAddUserSelectDlg *>::iterator end = m_listAddUserSelectDlg.end();
	
	CAddUserSelectDlg *pDlg;
	for(;start != end;++start)
	{
		pDlg = *start;
		pDlg->AddConnectUser(pUserInfo);
	}
}

void CUserListGroup::DelItem(SMALLUSERINFO *pUserInfo)
{
	m_pList->DeleteItem((LPARAM)pUserInfo);

	std::list<CAddUserSelectDlg *>::iterator start = m_listAddUserSelectDlg.begin();
	std::list<CAddUserSelectDlg *>::iterator end = m_listAddUserSelectDlg.end();

	CAddUserSelectDlg *pDlg;
	for(;start != end;++start)
	{
		pDlg = *start;
		pDlg->DelConnectUser(pUserInfo);
	}
}

void CUserListGroup::ChangeItem(SMALLUSERINFO *pUserInfo)
{
	m_pList->UpdateItemFromParam((LPARAM)pUserInfo);

	std::list<CAddUserSelectDlg *>::iterator start = m_listAddUserSelectDlg.begin();
	std::list<CAddUserSelectDlg *>::iterator end = m_listAddUserSelectDlg.end();

	CAddUserSelectDlg *pDlg;
	for(;start != end;++start)
	{
		pDlg = *start;
		pDlg->ChangeConnectUser(pUserInfo);
	}
}

//접속중 유저리스트의 버튼처리(닉네임,보유머니,위치,사용자검색,방번호검색버튼)
LRESULT CUserListGroup::OnClickButton(LSY::CMessage *msg)
{
	LSY::CMO_MouseLClick *pMsg = (LSY::CMO_MouseLClick *)msg;
	LSY::CButton *pBtn = (LSY::CButton *)pMsg->GetObject();
	CWnd *pWnd = GetObjectMan()->GetWnd();
	switch(pBtn->GetID())
	{
	case ID_LOBBY_TXTBTN_USERLIST_HEADER_NICKNAME:
		//유저정렬 (닉네임) : 정렬기능 제거:2012.10.19 YS.Jeong
		Sort(DEF_USERLIST_SORT_NICKNAME);
		break;
	case ID_LOBBY_TXTBTN_USERLIST_HEADER_HAVEMONEY:
		//유저정렬 (머니) : 정렬기능 제거:2012.10.19 YS.Jeong
		Sort(DEF_USERLIST_SORT_HAVEMONEY);
		break;
	case ID_LOBBY_TXTBTN_USERLIST_HEADER_POSITION:
		//유저정렬 (위치) : 정렬기능 제거:2012.10.19 YS.Jeong
		Sort(DEF_USERLIST_SORT_POSITION);
		break;
	case ID_LOBBY_BTN_FIND_USER:
		SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_USERLIST_FIND_USER);
		FindUser();
		break;
	case ID_LOBBY_BTN_FIND_ROOM_NUMBER:
		SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_USERLIST_FIND_ROOMNO);
		FindRoomNumber();
		break;
	}
	return TRUE;
}

LRESULT CUserListGroup::OnReturnEdit(LSY::CMessage *msg)
{
	LSY::CMO_Return *pMsg = (LSY::CMO_Return *)msg;
	LSY::CEdit *pEdit = (LSY::CEdit *)pMsg->GetObject();
	switch(pEdit->GetID())
	{
	case ID_LOBBY_EDIT_FIND_USER:
		FindUser();
		break;
	case ID_LOBBY_EDIT_FIND_ROOM_NUMBER:
		FindRoomNumber();
		break;
	}
	return TRUE;
}

LRESULT CUserListGroup::OnListItemDbClick(LSY::CMessage *msg)
{
	LSY::CMO_ListItemDbClick *pMsg = (LSY::CMO_ListItemDbClick *)msg;
	LSY::CListItemData *pData = pMsg->GetItemData();
	SMALLUSERINFO *pUserInfo = (SMALLUSERINFO *)pData->GetItemData();
	CWnd *pWnd = GetObjectMan()->GetWnd();
	pWnd->SendMessage(ID_LOBBY_PROCESS_ASK_USERINFO, (WPARAM)pUserInfo);
	return TRUE;
}

LRESULT CUserListGroup::OnListItemRClick(LSY::CMessage *msg)
{
	LSY::CMO_ListItemRClick *pMsg = (LSY::CMO_ListItemRClick *)msg;
	LSY::CListItemData *pData = pMsg->GetItemData();
	SMALLUSERINFO *pUserInfo = (SMALLUSERINFO *)pData->GetItemData();
	
	memcpy(&m_stSelectUserInfo, pUserInfo, sizeof(SMALLUSERINFO));
	CWnd *pWnd = GetObjectMan()->GetWnd();

	LSY::CPoint ptPos = GetObjectMan()->GetCurMousePos();
	POINT ptTempPos = ptPos.GetPoint();
	pWnd->ClientToScreen(&ptTempPos);

	CPopupMemu *pPopupMemu = new CPopupMemu;
	pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_PROFILE, "프로필보기", true);
	
	// 클럽 등급 세팅
	int nMyGrade = GM().GetMyInfo()->UI.ClubMemberGrade;
	int nUserGrade = pUserInfo->ClubMemberGrade;
	// 내가 보스가 아닐경우 클럽 가입요청으로 변경
	CString csInviteClub = "클럽초대";
	if (nMyGrade != CLUB_MEMBER_GRADE_BOSS)
		csInviteClub = "클럽 가입요청";
	// 내가 보스고 상대가 미가입 상태면 클럽초대 버튼 활성화
	bool bActiveInviteClub = false;
	if (nMyGrade == CLUB_MEMBER_GRADE_BOSS &&
		nUserGrade == CLUB_MEMBER_GRADE_NONE)
		bActiveInviteClub = true;
	// 내가 클럽 미가입 상태이고 상대가 보스일때 클럽 가입요청 버튼 활성화
	else if (nMyGrade == CLUB_MEMBER_GRADE_NONE &&
		nUserGrade == CLUB_MEMBER_GRADE_BOSS)
		bActiveInviteClub = true;
	
	// 게임 중 일때?
	if(pUserInfo->RoomNum != 0)
	{
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_SEND_MEMO, "쪽지보내기", true);
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_SEND_WHISPER, "귓말보내기", false);
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_INVITE_CLUB, csInviteClub, bActiveInviteClub);
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_ADD_BUDDY, "친구추가", CCommunityManager::Instance()->IsExistBuddy(pUserInfo->NickName)?false:true);
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_ADD_BLACK_LIST, "블랙리스트추가", CCommunityManager::Instance()->IsExistBlackBuddy(pUserInfo->NickName)?false:true);
	}
	// 나를 선택
	else if(strcmp(pUserInfo->ID, GM().GetMyInfo()->UI.ID) == 0)
	{
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_SEND_MEMO, "쪽지보내기", false);
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_SEND_WHISPER, "귓말보내기", false);
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_INVITE_CLUB, csInviteClub, bActiveInviteClub);
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_ADD_BUDDY, "친구추가", false);
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_ADD_BLACK_LIST, "블랙리스트추가", false);
	}
	else
	{
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_SEND_MEMO, "쪽지보내기", true);
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_SEND_WHISPER, "귓말보내기", false);
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_INVITE_CLUB, csInviteClub, bActiveInviteClub);
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_ADD_BUDDY, "친구추가", CCommunityManager::Instance()->IsExistBuddy(pUserInfo->NickName)?false:true);
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_ADD_BLACK_LIST, "블랙리스트추가", CCommunityManager::Instance()->IsExistBlackBuddy(pUserInfo->NickName)?false:true);
	}

	//관리자 아이디이면 아이디 복사하기 메뉴를 보이도록 한다.
	if(GM().GetMyInfo()->UI.AdminGrade)
		pPopupMemu->AddMemu(ID_LOBBYPOPUPMENU_TXTBTN_COPY_ID, "아이디복사하기", true);

	SetSelectID(pUserInfo->ID);
	SetSelectNickName(pUserInfo->NickName);
	pPopupMemu->Create(ptTempPos.x, ptTempPos.y);
	GM().GetMainWnd()->SendMessage(UPGM_POPUPMEMU_OPEN, 0, (LPARAM)pPopupMemu);

	return TRUE;
}

LRESULT CUserListGroup::OnSelChangeTabUser(LSY::CMessage *msg)
{
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();

	#ifdef UPGRADE_10210619_ADD_CLICKLOG
		if (nIndex == 0)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_USERTAB);
		else if (nIndex == 1)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB);
	#endif //UPGRADE_10210619_ADD_CLICKLOG

	return TRUE;
}

LRESULT CUserListGroup::OnSelChangeTabFriend(LSY::CMessage *pMsg)
{
	LSY::CMO_SelChange *pChange = (LSY::CMO_SelChange *)pMsg;
	int nIndex = pChange->GetValue();
	
	switch (nIndex)
	{
	case 0: // 내친구 탭
		break;
	case 1: // 클럽 탭
		{
			GM().SetLobbyFriendTabRequest(true);
			CCL_CLUB_REQUEST_DETAIL_INFO msg;
			msg.Set();
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
	case 2: // 카페 탭
		{
			DWORD dwTime_current = timeGetTime();

			// 1분이 지나지 않았으면 요청을 보내지 않음
			if (dwTime_current - m_dwTime_refresh < 5 * 1000)
				break;

			int nUniteID = GM().GetMyInfo()->UI.unitedCafeInfo_.cafeID_;
			int nCafeID = GM().GetMyInfo()->UI.cafeInfo_.cafeID_;

			if (nUniteID > 0)
			{
				// 연합 멤버 요청
				CCL_CAFE_MEMBERLIST msg;
				msg.Set(CafeSystem::CafeKind::UNITED_CAFE, nUniteID);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			}
			// 연합 멤버일 경우 응답 처리하는 과정에서 카페 멤버 요청을 하므로
			// 연합맴버가 아닐 경우에만 카페 멤버를 요청
			else if (nCafeID > 0)
			{
				// 카페 멤버 요청
				CCL_CAFE_MEMBERLIST msg;
				msg.Set(CafeSystem::CafeKind::CAFE, nCafeID);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			}

			m_dwTime_refresh = dwTime_current;
		}
		break;
	}

	#ifdef UPGRADE_10210619_ADD_CLICKLOG
		if (nIndex == 0)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_MYFRIEND);
		else if (nIndex == 1)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_FRIEND_ADDED_ME);
		else if (nIndex == 2)
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_USERLIST_POKERFRIENDTAB_BLACKLIST);
	#endif //UPGRADE_10210619_ADD_CLICKLOG

	return TRUE;
}

void CUserListGroup::FindUser(void)
{
	CString strText = m_pEditFindUser->GetText().c_str();
	CWnd *pWnd = GetObjectMan()->GetWnd();
	pWnd->SendMessage(LUV_EDIT_SEARCH_USER, 0, (LPARAM)(&strText));

	if(GetObjectMan()->GetFocus() == m_pEditFindUser)
		GetObjectMan()->SetFocus(NULL);

	m_pEditFindUser->SetText("");
}

void CUserListGroup::FindRoomNumber(void)
{
	CString strText = m_pEditFindRoomNumber->GetText().c_str();
	CWnd *pWnd = GetObjectMan()->GetWnd();
	pWnd->SendMessage(LUV_EDIT_SEARCH_ROOM, 0, (LPARAM)(&strText));

	if(GetObjectMan()->GetFocus() == m_pEditFindRoomNumber)
		GetObjectMan()->SetFocus(NULL);

	m_pEditFindRoomNumber->SetText("");
}

void CUserListGroup::SetFocusFindUser(void)
{
	if(GetObjectMan()->GetFocus() != m_pEditFindUser)
		GetObjectMan()->SetFocus(m_pEditFindUser);
}

void CUserListGroup::SetFocusFindRoomNumber(void)
{
	if(GetObjectMan()->GetFocus() != m_pEditFindRoomNumber)
		GetObjectMan()->SetFocus(m_pEditFindRoomNumber);
}

void CUserListGroup::Sort(int nSortType)
{
	if(m_nCurSortType == nSortType && nSortType != DEF_USERLIST_SORT_UNKNOWN)
	{
		m_mapSortTypeIncrement[nSortType] = m_mapSortTypeIncrement[nSortType]?false:true;
	}

	m_nCurSortType = nSortType;

	if(nSortType != DEF_USERLIST_SORT_UNKNOWN)
	{
		m_pList->SetSortType(nSortType);
		m_pList->SetIncrement(m_mapSortTypeIncrement[nSortType]);
		m_pList->Sort();
	}
	
}

void CUserListGroup::SortDefault(void)
{
	m_nCurSortType = DEF_USERLIST_SORT_HAVEMONEY;
	m_mapSortTypeIncrement[DEF_USERLIST_SORT_HAVEMONEY] = false;

	m_pList->SetSortType(DEF_USERLIST_SORT_HAVEMONEY);
	m_pList->SetIncrement(false);
	m_pList->Sort();
}

//접속중인 유저 리스트 셔플기능 추가:2012.10.22 YS.Jeong
void CUserListGroup::SortShuffle(void)
{
	m_pList->SortShuffle();
}

void CUserListGroup::SetSelectID(const std::string &strID)
{
	m_strSelectID = strID;
}

void CUserListGroup::SetSelectNickName(const std::string &strNickName)
{
	m_strSelectNickName = strNickName;
}

void CUserListGroup::SetSelectRoomNum(int nRoomNum)
{
	m_nSelectRoomNum = nRoomNum;
}

SMALLUSERINFO* CUserListGroup::GetSelectedUserInfo(void)
{
	return &m_stSelectUserInfo;
}