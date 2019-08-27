#include "stdafx.h"
#include "AddUserSelectDlg.h"
#include "AddUserSelectDlgID.h"
#include "AddUserSelectDlgBuddyList.h"
#include "AddUserSelectDlgConnectUserList.h"
#include "BuddyListItem.h"
#include "ConnectUserListItem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CAddUserSelectDlg::CAddUserSelectDlg(const bool &bShowBuddy, CWnd* pParent/* = NULL*/)
:	LSY::CLSYLibDialogBase(pParent),
	m_pListBuddy(NULL),
	m_pListConnectUser(NULL),
	m_pCheckAllBuddy(NULL),
	m_pSprLoding(NULL),
	m_pComboGame(NULL),
	m_pComboMoney(NULL),
	m_pCheckAllConnectUser(NULL),
	m_pWndBack(NULL),
	m_bShowBuddy(bShowBuddy)
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
}

CAddUserSelectDlg::~CAddUserSelectDlg()
{
	if(m_pListBuddy)
	{
		if(m_bShowBuddy) CCommunityManager::Instance()->RemoveListObjectBuddyGroupEntry(m_pListBuddy);
		m_pListBuddy = NULL;

		SubSelectUserDlg(this);
	}
}

BEGIN_MESSAGE_MAP(CAddUserSelectDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

INT_PTR CAddUserSelectDlg::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_ADDUSERSELECTDLG);
}

bool CAddUserSelectDlg::OnCreatedProject(void)
{
	LSY::CObjectProject *pProject = m_pObjectMan->GetProjectFromLayer(0);
	m_pWndBack =(LSY::CWindows *)pProject->GetMainObject();

	LSY::CGroup *pGrpBuddy = (LSY::CTabControl *)GetObject(ID_ADDUSERSELECTDLG_GRP_BUDDY);
	LSY::CGroup *pGrpConnectUser = (LSY::CTabControl *)GetObject(ID_ADDUSERSELECTDLG_GRP_CONNECT_USER);

	if(m_bShowBuddy)
	{
		pGrpBuddy->SetShow(true);
		pGrpConnectUser->SetShow(false);
		m_pWndBack->SetIndex(0);
	}
	else
	{
		pGrpBuddy->SetShow(false);
		pGrpConnectUser->SetShow(true);
		m_pWndBack->SetIndex(1);
	}

	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_ADDUSERSELECTDLG_BTN_CANCEL);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CAddUserSelectDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_ADDUSERSELECTDLG_BTN_OK);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CAddUserSelectDlg::OnClickButton));

	m_pListBuddy = (LSY::CList *)GetObject(ID_ADDUSERSELECTDLG_LIST_BUDDY);
	m_pListBuddy->SetCompareFunction(0, BuddyGroupEntryListCompareFunction);
	m_pListBuddy->SetSortType(0);

	m_pCheckAllBuddy = (LSY::CCheckBox *)GetObject(ID_ADDUSERSELECTDLG_CHECKBOX_BUDDY_LIST_ALL_SELECT);
	m_pCheckAllBuddy->AddHandler(LSY::EM_O_CHECKCHANGE, LSY::Fnt(this, &CAddUserSelectDlg::OnCheckAllChange));
	m_pCheckAllBuddy->SetCheck(false);

	CBuddyListItem *pSorceItem = (CBuddyListItem *)m_pListBuddy->GetItem();
	pSorceItem->m_pCheckBuddySelect->AddHandler(LSY::EM_O_CHECKCHANGE, LSY::Fnt(pSorceItem, &CBuddyListItem::OnCheckChange));
	pSorceItem->m_pCheckGroupSelect->AddHandler(LSY::EM_O_CHECKCHANGE, LSY::Fnt(pSorceItem, &CBuddyListItem::OnCheckChange));

	m_pListConnectUser = (LSY::CList *)GetObject(ID_ADDUSERSELECTDLG_LIST_USER);

	CConnectUserListItem *pItem = (CConnectUserListItem *)m_pListConnectUser->GetItem();
	pItem->m_pCheckSelect->AddHandler(LSY::EM_O_CHECKCHANGE, LSY::Fnt(pItem, &CConnectUserListItem::OnCheckChange));

	m_pListConnectUser->SetCompareFunction(DEF_USERLIST_SORT_NICKNAME, (LSY::CList::LISTCOMPAREFUNCTION)&UserListNickNameCompareFunction);
	m_pListConnectUser->SetCompareFunction(DEF_USERLIST_SORT_HAVEMONEY, (LSY::CList::LISTCOMPAREFUNCTION)&UserListHaveMoneyCompareFunction);
	m_pListConnectUser->SetCompareFunction(DEF_USERLIST_SORT_POSITION, (LSY::CList::LISTCOMPAREFUNCTION)&UserListPositionCompareFunction);

	LSY::CTextButton *pTxtButton = (LSY::CTextButton *)GetObject(ID_ADDUSERSELECTDLG_TXTBTN_USERLIST_HEADER_NICKNAME);
	pTxtButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CAddUserSelectDlg::OnClickButton));

	m_pCheckAllConnectUser = (LSY::CCheckBox *)GetObject(ID_ADDUSERSELECTDLG_CHECKBOX_CONNECT_USER_ALL_SELECT);
	m_pCheckAllConnectUser->AddHandler(LSY::EM_O_CHECKCHANGE, LSY::Fnt(this, &CAddUserSelectDlg::OnCheckAllChange));
	m_pCheckAllConnectUser->SetCheck(false);

	pTxtButton = (LSY::CTextButton *)GetObject(ID_ADDUSERSELECTDLG_TXTBTN_USERLIST_HEADER_HAVEMONEY);
	pTxtButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CAddUserSelectDlg::OnClickButton));

	pTxtButton = (LSY::CTextButton *)GetObject(ID_ADDUSERSELECTDLG_TXTBTN_USERLIST_HEADER_POSITION);
	pTxtButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CAddUserSelectDlg::OnClickButton));

	m_pSprLoding = (LSY::CSprite *)GetObject(ID_ADDUSERSELECTDLG_SPR_USER_LOADING);
	m_pSprLoding->StopAnimation();

	m_pComboGame = (LSY::CCombo *)GetObject(ID_ADDUSERSELECTDLG_CB_USERLIST_GAME);
	m_pComboMoney = (LSY::CCombo *)GetObject(ID_ADDUSERSELECTDLG_CB_USERLIST_HAVEMONEY);

	m_pComboGame->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CAddUserSelectDlg::OnSelChangeGame));
	m_pComboMoney->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CAddUserSelectDlg::OnSelChangeMoney));

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

	if(m_bShowBuddy) CCommunityManager::Instance()->AddListObjectBuddyGroupEntry(m_pListBuddy, false);
	else SetCurConnectUserList();

	AddSelectUserDlg(this);
	SortDefault();

	return TRUE;
}

LRESULT CAddUserSelectDlg::OnSelChangeGame(LSY::CMessage *msg)
{
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();
	IDX_GAME nGame = (IDX_GAME)m_pComboGame->GetItemData(nIndex);
	SelChangeGameAddSelectUserDlg(nGame);

	return TRUE;
}

LRESULT CAddUserSelectDlg::OnSelChangeMoney(LSY::CMessage *msg)
{
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();
	SelChangeMoneyAddSelectUserDlg(nIndex);
	return TRUE;
}

void CAddUserSelectDlg::SetComboSynchronization(IDX_GAME nSelGame, int nSelectMoney)
{
	int nCount = m_pComboGame->GetItemCount();
	int nSel = 0;
	for(int i=0;i<nCount;++i)
	{
		if(m_pComboGame->GetItemData(i) == nSelGame)
		{
			nSel = i;
			break;
		}
	}
	m_pComboGame->SetCurSelectItem(nSel);

	ResetMoneyComboBox(nSelGame, 0);
	m_pComboMoney->SetCurSelectItem(nSelectMoney);
}

LSY::CObjectBase *CAddUserSelectDlg::OnCallCreateObject(const int &nID, const std::string &strName)
{
	switch(nID)
	{
	case ID_ADDUSERSELECTDLG_LISTITEM_BUDDY:
		return new CBuddyListItem;
	case ID_ADDUSERSELECTDLG_LIST_BUDDY:
		return new CAddUserSelectDlgBuddyList(this);
	case ID_ADDUSERSELECTDLG_LISTITEM_CONNECT_USER:
		return new CConnectUserListItem;
	case ID_ADDUSERSELECTDLG_LIST_USER:
		return new CAddUserSelectDlgConnectUserList(this);
	}
	return LSY::CLSYLibDialogBase::OnCallCreateObject(nID, strName);
}

LRESULT CAddUserSelectDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_ADDUSERSELECTDLG_BTN_CANCEL:
		OnCancel();
		break;
	case ID_ADDUSERSELECTDLG_BTN_OK:
		OnCheckSelectNickName();
		OnOK();
		break;
	case ID_ADDUSERSELECTDLG_TXTBTN_USERLIST_HEADER_NICKNAME:
		Sort(DEF_USERLIST_SORT_NICKNAME);
		break;
	case ID_ADDUSERSELECTDLG_TXTBTN_USERLIST_HEADER_HAVEMONEY:
		Sort(DEF_USERLIST_SORT_HAVEMONEY);
		break;
	case ID_ADDUSERSELECTDLG_TXTBTN_USERLIST_HEADER_POSITION:
		Sort(DEF_USERLIST_SORT_POSITION);
		break;
	}
	return TRUE;
}

void CAddUserSelectDlg::ClearAllConnectUserCheck() 
{
	LSY::CList *pList = NULL;
	pList = m_pListConnectUser;

	LSY::CListItemData *pItemData = pList->GetFirstItem();
	while(pItemData)
	{	
		if(pItemData->GetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK))
		{
			pItemData->SetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK, false);
			pList->UpdateItemFromParam(pItemData->GetItemData());
		}

		pItemData = pList->GetNextItem();
	}
}

void CAddUserSelectDlg::ClearAllBuddyCheck()
{
	LSY::CList *pList = NULL;
	pList = m_pListBuddy;

	LSY::CListItemData *pItemData = pList->GetFirstItem();;

	while(pItemData)
	{	
		if(pItemData->GetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK))
		{
			pItemData->SetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK, false);
			pList->UpdateItemFromParam(pItemData->GetItemData());
		}

		pItemData = pList->GetNextItem();
	}
}

void CAddUserSelectDlg::Sort(int nSortType)
{
	if(m_nCurSortType == nSortType && nSortType != DEF_USERLIST_SORT_UNKNOWN)
	{
		m_mapSortTypeIncrement[nSortType] = m_mapSortTypeIncrement[nSortType]?false:true;
	}

	m_nCurSortType = nSortType;

	if(nSortType != DEF_USERLIST_SORT_UNKNOWN)
	{
		m_pListConnectUser->SetSortType(nSortType);
		m_pListConnectUser->SetIncrement(m_mapSortTypeIncrement[nSortType]);
		m_pListConnectUser->Sort();
	}
}

void CAddUserSelectDlg::SortDefault(void)
{
	m_nCurSortType = DEF_USERLIST_SORT_HAVEMONEY;
	m_mapSortTypeIncrement[DEF_USERLIST_SORT_HAVEMONEY] = false;

	m_pListConnectUser->SetSortType(DEF_USERLIST_SORT_HAVEMONEY);
	m_pListConnectUser->SetIncrement(false);
	m_pListConnectUser->Sort();
}

LRESULT CAddUserSelectDlg::OnCheckAllChange(LSY::CMessage *pMsg)
{
	LSY::CMO_CheckChange *msg = (LSY::CMO_CheckChange *)pMsg;
	LPARAM lParam = msg->GetValue()?1:0;

	LSY::CList *pList = NULL;
	switch(msg->GetObject()->GetID())
	{
	case ID_ADDUSERSELECTDLG_CHECKBOX_BUDDY_LIST_ALL_SELECT: pList = m_pListBuddy; break;
	case ID_ADDUSERSELECTDLG_CHECKBOX_CONNECT_USER_ALL_SELECT: pList = m_pListConnectUser; break;
	}
	
	LSY::CListItemData *pItemData = pList->GetFirstItem();
	while(pItemData)
	{	
		if(pItemData->GetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK) != lParam)
		{
			pItemData->SetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK, lParam);
			pList->UpdateItemFromParam(pItemData->GetItemData());
		}

		pItemData = pList->GetNextItem();
	}

	return TRUE;
}

bool CAddUserSelectDlg::IsCheckBuddyAllSelectItem(void)
{
	return m_pCheckAllBuddy->IsCheck();
}

void CAddUserSelectDlg::SetCheckBuddyAllSelectItem(bool bCheck)
{
	m_pCheckAllBuddy->SetCheck(bCheck);
}

bool CAddUserSelectDlg::IsCheckConnectUserAllSelectItem(void)
{
	return m_pCheckAllConnectUser->IsCheck();
}

void CAddUserSelectDlg::SetCheckConnectUserAllSelectItem(bool bCheck)
{
	m_pCheckAllConnectUser->SetCheck(bCheck);
}

void CAddUserSelectDlg::OnAddIDAndNIckName(const std::string &strID, const std::string &strNickName)
{
	CPlayer *pMyInfo = GM().GetMyInfo();
	if(pMyInfo == NULL) return;
	if(strID == pMyInfo->UI.ID) return;
	MAPSTRING::iterator findIt = m_mapIDAndNickName.find(strID);
	if(findIt != m_mapIDAndNickName.end()) return;
	m_mapIDAndNickName[strID] = strNickName;
}

void CAddUserSelectDlg::OnCheckSelectNickName(void)
{
	m_mapIDAndNickName.clear();

	CBuddyBase *pBuddyBase = NULL;
	CBuddyFriend *pBuddy = NULL;

	LSY::CListItemData *pItemData = m_pListBuddy->GetFirstItem();
	while(pItemData)
	{
		if(pItemData->GetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK))
		{
			pBuddyBase = (CBuddyBase *)pItemData->GetItemData();
			if(!pBuddyBase->IsGroup())
			{
				pBuddy = (CBuddyFriend *)pBuddyBase;

				OnAddIDAndNIckName(pBuddy->GetID(), pBuddy->GetNickName());
			}
		}
		pItemData = m_pListBuddy->GetNextItem();
	}

	SMALLUSERINFO *pUserInfo = NULL;
	pItemData = m_pListConnectUser->GetFirstItem();
	while(pItemData)
	{
		if(pItemData->GetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK))
		{
			pUserInfo = (SMALLUSERINFO *)pItemData->GetItemData();
			OnAddIDAndNIckName(pUserInfo->ID, pUserInfo->NickName);
		}
		pItemData = m_pListConnectUser->GetNextItem();
	}
}

void CAddUserSelectDlg::RemoveAllConnectUser(void)
{
	m_pCheckAllConnectUser->SetCheck(false);
	m_pListConnectUser->DeleteAllItem();
}

void CAddUserSelectDlg::ShowLoading(bool bShow)
{
	if(bShow)
	{
		if(!m_pSprLoding->IsAnimation())
		{
			m_pSprLoding->StartAnimation();
			m_pListConnectUser->SetEnable(false);
		}
	}
	else if(m_pSprLoding->IsAnimation())
	{
		m_pSprLoding->StopAnimation();
		m_pListConnectUser->SetEnable(true);
		SortDefault();	
	}
}

void CAddUserSelectDlg::SetCurConnectUserList(void)
{
	if(IsShowLoadingUserList())
	{
		ShowLoading(true);
	}
	else
	{
		CDataUserList::DATAPOSITION pos;
		SMALLUSERINFO *pUserInfo = GM().GetDataUserList()->GetFirstData(pos);
		while(pUserInfo)
		{
			AddConnectUser(pUserInfo);
			pUserInfo = GM().GetDataUserList()->GetNextData(pos);
		}
	}
}

void CAddUserSelectDlg::AddConnectUser(SMALLUSERINFO *pUserInfo)
{
	m_pListConnectUser->AddItem((LPARAM)pUserInfo);
}

void CAddUserSelectDlg::DelConnectUser(SMALLUSERINFO *pUserInfo)
{
	m_pListConnectUser->DeleteItem((LPARAM)pUserInfo);
}

void CAddUserSelectDlg::ChangeConnectUser(SMALLUSERINFO *pUserInfo)
{
	m_pListConnectUser->UpdateItemFromParam((LPARAM)pUserInfo);
}

void CAddUserSelectDlg::ResetGameComboBox(IDX_GAME nGame, INT64 i64MyMoney)
{
	if(nGame < 0 || nGame >= MAX_GAME)
		return;

	IDX_GAME nSelGame = IDX_GAME_NULL;
	//맞포커인 경우 게임콤보의 맞포커 선택
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
		ResetMoneyComboBox(nGame, i64MyMoney);
	}
}

void CAddUserSelectDlg::ResetMoneyComboBox(IDX_GAME nGame, INT64 i64Money)
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
}