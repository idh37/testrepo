#include "stdafx.h"
#include "MessageBox.h"
#include "MessageBoxID.h"
#include "MessageBoxImgFull.h"
#include "MessageBoxListItem.h"
#include "MemoNotifyDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _DEF_CHECK_FULL_MEMO	270
#define _DEF_CHECK_FULL_STORAGE	90

CMessageBox::CMessageBox(CWnd* pParent/* = NULL*/)
:	LSY::CLSYLibDialogBase(pParent),
	m_nSelID(-1),
	m_pBtnGoFirst(NULL),
	m_pBtnGoPrev(NULL),
	m_pBtnGoNext(NULL),
	m_pBtnGoLast(NULL),
	m_pGrpAll(NULL),
	m_pLoading(NULL),
	m_nCurPage(1),
	m_bRecevieMemoBox(false),
	m_bSendMemoBox(false),
	m_bSuccessMemoBox(false),
	m_bRecevieStorageMemoBox(false),
	m_bSendStorageMemoBox(false),
	m_bSuccessStorageMemoBox(false),
	m_bSearch(false),
	m_bSearchNickName(false),
	m_strSearch(""),
	m_nSendRemoveCount(0),
	m_nSendMemoListToStorageCount(0),
	m_pAllCheck(NULL),
	m_nCompareType(CCommunityManager::CMemoCompare::EMCT_DATE),
	m_bIncrement(true)
{
	for(int i=0;i<5;++i)
	{
		m_pListBtnGoNumber[i] = NULL;
		m_pListTxtBtnGoNumber[i] = NULL;
	}
}

BEGIN_MESSAGE_MAP(CMessageBox, LSY::CLSYLibDialogBase)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

INT_PTR CMessageBox::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;
	m_nSelID = -1;
	m_nCurPage = 1;
	m_bSearch = false;
	m_bSearchNickName = false;
	m_strSearch = "";
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_MESSAGEBOX);
}

LSY::CObjectBase *CMessageBox::OnCallCreateObject(const int &nID, const std::string &strName)
{
	switch(nID)
	{
	case ID_MESSAGEBOX_IMG_ALL_MESSAGE_FULL:
	case ID_MESSAGEBOX_IMG_SAVE_MESSAGE_FULL:
		return new CMessageBoxImgFull;
	case ID_MESSAGEBOX_LISTITEM_MESSAGE:
		return new CMessageBoxListItem;
	}
	return __super::OnCallCreateObject(nID, strName);
}

bool CMessageBox::OnCreatedProject(void)
{
	m_pGrpAll = (LSY::CGroup *)GetObject(ID_MESSAGEBOX_GRP_ALL);
	m_pLoading = (LSY::CSprite *)GetObject(ID_MESSAGEBOX_SPR_LODING);
	m_pLoading->StopAnimation();

	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_MESSAGEBOX_BTN_SEND_MESSAGE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageBox::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_MESSAGEBOX_BTN_CLOSE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageBox::OnClickButton));

	LSY::CTabControl *pTab = (LSY::CTabControl *)GetObject(ID_MESSAGEBOX_TAB_MESSAGEBOX);
	pTab->SetCurSelFromID(ID_MESSAGEBOX_RAD_ALL_MESSAGE);
	pTab->AddHandler(LSY::EM_O_LOCALCHILDNOTIFY, LSY::Fnt(this, &CMessageBox::OnTabChildNotify));

	LSY::CRadio *pRadio = (LSY::CRadio *)GetObject(ID_MESSAGEBOX_RAD_ALL_MESSAGE);

	LSY::CImage *pImage = (LSY::CImage *)GetObject(ID_MESSAGEBOX_IMG_ALL_MESSAGE_FULL);
	pImage->AddHandler(LSY::EM_O_MOUSEOVER, LSY::Fnt(this, &CMessageBox::OnMouseOver));
	pImage->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this, &CMessageBox::OnMouseLeave));
	((CMessageBoxImgFull *)pImage)->SetTab(pRadio);

	pRadio = (LSY::CRadio *)GetObject(ID_MESSAGEBOX_RAD_SAVE_MESSAGE);

	pImage = (LSY::CImage *)GetObject(ID_MESSAGEBOX_IMG_SAVE_MESSAGE_FULL);
	pImage->AddHandler(LSY::EM_O_MOUSEOVER, LSY::Fnt(this, &CMessageBox::OnMouseOver));
	pImage->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this, &CMessageBox::OnMouseLeave));
	((CMessageBoxImgFull *)pImage)->SetTab(pRadio);

	pImage = (LSY::CImage *)GetObject(ID_MESSAGEBOX_IMG_ALL_MESSAGE_POPUP);
	pImage->SetShow(false);

	pImage = (LSY::CImage *)GetObject(ID_MESSAGEBOX_IMG_SAVE_MESSAGE_POPUP);
	pImage->SetShow(false);

	LSY::CTextButton *pTxtButton = (LSY::CTextButton *)GetObject(ID_MESSAGEBOX_TXTBTN_SENDER);
	pTxtButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageBox::OnClickButton));

	pTxtButton = (LSY::CTextButton *)GetObject(ID_MESSAGEBOX_TXTBTN_MESSAGE);
	pTxtButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageBox::OnClickButton));

	pTxtButton = (LSY::CTextButton *)GetObject(ID_MESSAGEBOX_TXTBTN_DATE);
	pTxtButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageBox::OnClickButton));
	
	LSY::CCombo *pCombo = (LSY::CCombo *)GetObject(ID_MESSAGEBOX_COMBO_SEARCH_TYPE);
	int nIndex = pCombo->AddText("내용");
	pCombo->SetItemData(nIndex, 0);
	nIndex = pCombo->AddText("아이디/닉네임");
	pCombo->SetItemData(nIndex, 1);
	pCombo->SetCurSelectItem(0);
	pCombo->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CMessageBox::OnComboSearchTypeChange));

	LSY::CEdit *pEdit = (LSY::CEdit *)GetObject(ID_MESSAGEBOX_EDIT_SEARCH);
	pEdit->SetText("");
	pEdit->AddHandler(LSY::EM_O_RETURN, LSY::Fnt(this, &CMessageBox::OnReturn));

	pButton = (LSY::CButton *)GetObject(ID_MESSAGEBOX_BTN_MESSAGE_SEARCH);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageBox::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_MESSAGEBOX_BTN_MESSAGE_DELETE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageBox::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_MESSAGEBOX_BTN_MESSAGE_SAVE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageBox::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_MESSAGEBOX_BTN_MESSAGE_REPORT);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageBox::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_MESSAGEBOX_BTN_READ);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageBox::OnClickButton));

	LSY::CCheckButton *pCheck = (LSY::CCheckButton *)GetObject(ID_MESSAGEBOX_CHECK_LISTITEM_SELECT);
	pCheck->AddHandler(LSY::EM_O_CHECKCHANGE, LSY::Fnt(this, &CMessageBox::OnCheckChange));

	m_pAllCheck = (LSY::CCheckButton *)GetObject(ID_MESSAGEBOX_CHECK_HEADER_ALL_MESSAGE);
	m_pAllCheck->AddHandler(LSY::EM_O_CHECKCHANGE, LSY::Fnt(this, &CMessageBox::OnCheckChange));

	m_pBtnGoFirst = (LSY::CButton *)GetObject(ID_MESSAGEBOX_BTN_GO_FIRST);
	m_pBtnGoFirst->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageBox::OnClickButton));

	m_pBtnGoPrev = (LSY::CButton *)GetObject(ID_MESSAGEBOX_BTN_GO_PREV);
	m_pBtnGoPrev->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageBox::OnClickButton));

	m_pListBtnGoNumber[0] = (LSY::CButton *)GetObject(ID_MESSAGEBOX_BTN_GO_NUMBER0);
	m_pListBtnGoNumber[0]->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageBox::OnClickButton));

	m_pListBtnGoNumber[1] = (LSY::CButton *)GetObject(ID_MESSAGEBOX_BTN_GO_NUMBER1);
	m_pListBtnGoNumber[1]->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageBox::OnClickButton));

	m_pListBtnGoNumber[2] = (LSY::CButton *)GetObject(ID_MESSAGEBOX_BTN_GO_NUMBER2);
	m_pListBtnGoNumber[2]->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageBox::OnClickButton));

	m_pListBtnGoNumber[3] = (LSY::CButton *)GetObject(ID_MESSAGEBOX_BTN_GO_NUMBER3);
	m_pListBtnGoNumber[3]->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageBox::OnClickButton));

	m_pListBtnGoNumber[4] = (LSY::CButton *)GetObject(ID_MESSAGEBOX_BTN_GO_NUMBER4);
	m_pListBtnGoNumber[4]->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageBox::OnClickButton));

	m_pBtnGoNext = (LSY::CButton *)GetObject(ID_MESSAGEBOX_BTN_GO_NEXT);
	m_pBtnGoNext->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageBox::OnClickButton));

	m_pBtnGoLast = (LSY::CButton *)GetObject(ID_MESSAGEBOX_BTN_GO_LAST);
	m_pBtnGoLast->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageBox::OnClickButton));

	m_pListTxtBtnGoNumber[0] = (LSY::CTextButton *)GetObject(ID_MESSAGEBOX_TXTBTN_GO_NUMBER0);
	m_pListTxtBtnGoNumber[1] = (LSY::CTextButton *)GetObject(ID_MESSAGEBOX_TXTBTN_GO_NUMBER1);
	m_pListTxtBtnGoNumber[2] = (LSY::CTextButton *)GetObject(ID_MESSAGEBOX_TXTBTN_GO_NUMBER2);
	m_pListTxtBtnGoNumber[3] = (LSY::CTextButton *)GetObject(ID_MESSAGEBOX_TXTBTN_GO_NUMBER3);
	m_pListTxtBtnGoNumber[4] = (LSY::CTextButton *)GetObject(ID_MESSAGEBOX_TXTBTN_GO_NUMBER4);

	LSY::CCheckBox *pCheckBox = (LSY::CCheckBox *)GetObject(ID_MESSAGEBOX_CHECK_NO_RECEIVE_MESSAGE);
	pCheckBox->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageBox::OnClickButton));
	
	if(CCommunityManager::Instance()->IsConnect())
	{
		if(CCommunityManager::Instance()->GetOption(COM_ECOT_MEMO) != COM_ECOPT_OFF) 
			pCheckBox->SetCheck(false);
		else
			pCheckBox->SetCheck(true);
	}
	else
	{
		ShowMainMessageDlg("커뮤니티 서버와 접속되어있지 않아 쪽지 수신 여부가 적용되지 않았습니다.");
	}
	

	LSY::CList *pList = (LSY::CList *)GetObject(ID_MESSAGEBOX_LIST_MESSAGE_BOX);
	pList->AddHandler(LSY::EM_O_LISTITEM_DBCLICK, LSY::Fnt(this, &CMessageBox::OnLButtonDbClick));

	CMessageBoxListItem *pListItem = (CMessageBoxListItem *)pList->GetItem();
	pListItem->m_pTxtSender->AddHandler(LSY::EM_O_LBUTTONDBCLICK, LSY::Fnt(this, &CMessageBox::OnListItemNickNameDbClick));

	//** 수정필요
	if(CCommunityManager::Instance()->IsConnect())
	{
		CCommunityManager::Instance()->GetMomoBoxReceiveInfo(m_bSendMemoBox, m_bRecevieMemoBox, m_bSuccessMemoBox, m_bSendStorageMemoBox, m_bRecevieStorageMemoBox, m_bSuccessStorageMemoBox, m_nSendRemoveCount, m_nSendMemoListToStorageCount);
		//쪽지 리스트 요청를 한번도 보내지 않았을 때 또는 쪽지 리스트 받기에 실패 하였을 때 다시 요청한다.
		if((!m_bRecevieMemoBox && !m_bSendMemoBox) || (m_bRecevieMemoBox && !m_bSuccessMemoBox))
		{
			m_bSendMemoBox = true;
			CCommunityManager::Instance()->SendGetMemoList();
		}

		//보관쪽지 리스트 요청를 한번도 보내지 않았을 때 또는 보관쪽지 리스트 받기에 실패 하였을 때 다시 요청한다.
		if((!m_bRecevieStorageMemoBox && !m_bSendStorageMemoBox) || (m_bRecevieStorageMemoBox && !m_bSuccessStorageMemoBox))
		{
			m_bSendStorageMemoBox = true;
			CCommunityManager::Instance()->SendGetMemoStorageList();
		}
	}

	SetTabChange(ID_MESSAGEBOX_RAD_ALL_MESSAGE);

	return true;
}

LRESULT CMessageBox::OnReturn(LSY::CMessage *pMsg)
{
	OnSearch();
	return TRUE;
}

LRESULT CMessageBox::OnCheckChange(LSY::CMessage *pMsg)
{
	LSY::CMO_CheckChange *msg = (LSY::CMO_CheckChange *)pMsg;
	const bool &bCheck = msg->GetValue();

	switch(msg->GetObject()->GetID())
	{
	case ID_MESSAGEBOX_CHECK_LISTITEM_SELECT:
		{
			LSY::CList *pList = (LSY::CList *)GetObject(ID_MESSAGEBOX_LIST_MESSAGE_BOX);
			LSY::CListItemData *pData = pList->GetFirstItem();
			MEMO *pMemo = NULL;
			CMessageBoxListItem *pItem = NULL;

			bool bAllCheck = true;

			while(pData)
			{
				pItem = (CMessageBoxListItem *)pData->GetItem();
				if(pItem == NULL) return TRUE;
				if(!pItem->m_pCheck->IsCheck()) bAllCheck = false;
				pData = pList->GetNextItem();
			}
			
			m_pAllCheck->SetCheck(bAllCheck);
		}
		break;
	case ID_MESSAGEBOX_CHECK_HEADER_ALL_MESSAGE:
		{
			LSY::CList *pList = (LSY::CList *)GetObject(ID_MESSAGEBOX_LIST_MESSAGE_BOX);
			LSY::CListItemData *pData = pList->GetFirstItem();
			MEMO *pMemo = NULL;
			CMessageBoxListItem *pItem = NULL;

			while(pData)
			{
				pItem = (CMessageBoxListItem *)pData->GetItem();
				pItem->m_pCheck->SetCheck(bCheck);
				pData = pList->GetNextItem();
			}
		}
		break;
	}
	return TRUE;
}

LRESULT CMessageBox::OnLButtonDbClick(LSY::CMessage *pMsg)
{
	LSY::CMO_ListItemDbClick *msg = (LSY::CMO_ListItemDbClick *)pMsg;
	LSY::CListItemData *pData = msg->GetItemData();
	if(pData)
	{
		MEMO *pMemo = (MEMO *)pData->GetItemData();
		CCommunityManager::Instance()->SendReadMemo(*pMemo);
	}
	return TRUE;
}

LRESULT CMessageBox::OnListItemNickNameDbClick(LSY::CMessage *pMsg)
{
	LSY::CMO_LButtonDbClick *msg = (LSY::CMO_LButtonDbClick *)pMsg;
	LSY::CListItemData *pData = GetListItemData(msg->GetObject());
	if(pData)
	{
		MEMO *pMemo = (MEMO *)pData->GetItemData();
		if(CCommunityManager::IsAdminMemo(pMemo))
		{
			CCommunityManager::Instance()->SendReadMemo(*pMemo);
		}
		else
		{
			SMALLUSERINFO *pUserInfo = GM().GetDataUserList()->FindData(pMemo->senderID);
			if(pUserInfo) AskProfileToServer(pUserInfo->llUserKey, pUserInfo->ID);
			else
			{
				std::list<std::string> listIDorNickName;
				listIDorNickName.push_back(pMemo->senderID);

				CCommunityManager::Instance()->SendGetUserInfo(CCommunityUserInfo::ECUIT_MEMOBOX, (LPARAM)this, listIDorNickName, "");
			}
		}
	}
	return TRUE;
}

void CMessageBox::CheckMessageCount(const int &nMemoCount, const int &nStorageCount)
{
	CString strTemp;
	int nCount = nMemoCount;
	LSY::CText *pText = NULL;
	LSY::CImage *pImage = NULL;
	if(nMemoCount >= 0)
	{
		nCount = nMemoCount;
		strTemp.Format("(%d/%d)", nCount, CCommunityManager::Instance()->GetMaxMemo());
		pText = (LSY::CText *)GetObject(ID_MESSAGEBOX_TXT_ALL_MESSAGE_COUNT);
		pText->SetText(strTemp.GetString());

		pImage = (LSY::CImage *)GetObject(ID_MESSAGEBOX_IMG_ALL_MESSAGE_FULL);
		pImage->SetShow((nCount >= _DEF_CHECK_FULL_MEMO)?true:false);
	}

	if(nStorageCount >= 0)
	{
		nCount = nStorageCount;
		strTemp.Format("(%d/%d)", nCount, CCommunityManager::Instance()->GetMaxStorageMemo());
		pText = (LSY::CText *)GetObject(ID_MESSAGEBOX_TXT_SAVE_MESSAGE_COUNT);
		pText->SetText(strTemp.GetString());

		pImage = (LSY::CImage *)GetObject(ID_MESSAGEBOX_IMG_SAVE_MESSAGE_FULL);
		pImage->SetShow((nCount >= _DEF_CHECK_FULL_STORAGE)?true:false);
	}
}

void CMessageBox::CheckTabColor(const int &nMemoCount, const int &nStorageCount)
{
	LSY::CText *pSelTab = NULL;
	LSY::CText *pSelCount = NULL;
	bool bFullSel = false;

	LSY::CText *pNoSelTab = NULL;
	LSY::CText *pNoSelCount = NULL;
	bool bFullNoSel = false;

	switch(m_nSelID)
	{
	case ID_MESSAGEBOX_RAD_ALL_MESSAGE:
		pSelTab = (LSY::CText *)GetObject(ID_MESSAGEBOX_TXT_ALL_MESSAGE_TAB);
		pSelCount = (LSY::CText *)GetObject(ID_MESSAGEBOX_TXT_ALL_MESSAGE_COUNT);

		if(nMemoCount > _DEF_CHECK_FULL_MEMO) bFullSel = true;

		pNoSelTab = (LSY::CText *)GetObject(ID_MESSAGEBOX_TXT_SAVE_MESSAGE_TAB);
		pNoSelCount = (LSY::CText *)GetObject(ID_MESSAGEBOX_TXT_SAVE_MESSAGE_COUNT);

		if(nStorageCount > _DEF_CHECK_FULL_STORAGE) bFullNoSel = true;
		break;
	case ID_MESSAGEBOX_RAD_SAVE_MESSAGE:
		pSelTab = (LSY::CText *)GetObject(ID_MESSAGEBOX_TXT_SAVE_MESSAGE_TAB);
		pSelCount = (LSY::CText *)GetObject(ID_MESSAGEBOX_TXT_SAVE_MESSAGE_COUNT);

		if(nStorageCount > _DEF_CHECK_FULL_STORAGE) bFullSel = true;

		pNoSelTab = (LSY::CText *)GetObject(ID_MESSAGEBOX_TXT_ALL_MESSAGE_TAB);
		pNoSelCount = (LSY::CText *)GetObject(ID_MESSAGEBOX_TXT_ALL_MESSAGE_COUNT);

		if(nMemoCount > _DEF_CHECK_FULL_MEMO) bFullNoSel = true;
		break;
	}

	pSelTab->SetTextColor(RGB(67,106,171));
	pNoSelTab->SetTextColor(RGB(124,129,132));

	if(bFullSel) pSelCount->SetTextColor(RGB(172,67,71));
	else pSelCount->SetTextColor(RGB(67,106,171));

	if(bFullNoSel) pNoSelCount->SetTextColor(RGB(172,67,71));
	else pNoSelCount->SetTextColor(RGB(124,129,132));
}

void CMessageBox::CheckGoButton(const int &nCount)
{
	if(nCount == 0)
	{
		m_nCurPage = 1;
		m_pBtnGoFirst->SetEnable(false);
		m_pBtnGoPrev->SetEnable(false);
		m_pBtnGoNext->SetEnable(false);
		m_pBtnGoLast->SetEnable(false);
		m_pBtnGoLast->SetUserData(0, 1);

		for(int i=0;i<5;++i)
		{
			m_pListBtnGoNumber[i]->SetShow(false);
			m_pListTxtBtnGoNumber[i]->SetShow(false);
		}

		return;
	}

	int nMaxPage = (nCount-1) / 10 + 1;
	int nMaxStepPage = (nMaxPage-1) / 5 + 1;
	int nCurStepPage = (m_nCurPage-1) / 5 + 1;

	if(m_nCurPage > nMaxPage)
		m_nCurPage = nMaxPage;

	m_pBtnGoLast->SetUserData(0, nMaxPage);

	if(m_nCurPage == 1)
	{
		m_pBtnGoFirst->SetEnable(false);
		m_pBtnGoPrev->SetEnable(false);
	}
	else
	{
		m_pBtnGoFirst->SetEnable(true);
		m_pBtnGoPrev->SetEnable(true);
	}

	if(nMaxPage == m_nCurPage)
	{
		m_pBtnGoLast->SetEnable(false);
		m_pBtnGoNext->SetEnable(false);
	}
	else
	{
		m_pBtnGoLast->SetEnable(true);
		m_pBtnGoNext->SetEnable(true);
	}

	int nSub = (nCurStepPage - 1) * 5;
	int nStart = nSub + 1;
	int nEnd = nStart + 4;
	if(nEnd > nMaxPage) nEnd = nMaxPage;

	int j = 0;
	CString strTemp;
	for(int i=nStart;i<=nEnd;++i,++j)
	{
		m_pListBtnGoNumber[i-1-nSub]->SetShow(true);
		m_pListBtnGoNumber[i-1-nSub]->SetEnable((m_nCurPage == i)?false:true);
		m_pListTxtBtnGoNumber[i-1-nSub]->SetShow(true);
		m_pListBtnGoNumber[i-1-nSub]->SetUserData(0, i);

		strTemp.Format("%ld",i);
		m_pListTxtBtnGoNumber[i-1-nSub]->SetText(strTemp.GetString());
	}

	for(;j<5;++j)
	{
		m_pListBtnGoNumber[j]->SetShow(false);
		m_pListTxtBtnGoNumber[j]->SetShow(false);
	}
}

LRESULT CMessageBox::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_MESSAGEBOX_BTN_READ:
		if(m_nCompareType == CCommunityManager::CMemoCompare::EMCT_READMEMO)
		{
			m_bIncrement = m_bIncrement?false:true;
		}
		else
		{
			m_nCompareType = CCommunityManager::CMemoCompare::EMCT_READMEMO;
			m_bIncrement = true;
		}
		
		SetChangePage(1);
		break;
	case ID_MESSAGEBOX_TXTBTN_SENDER:
		if(m_nCompareType == CCommunityManager::CMemoCompare::EMCT_SENDER)
		{
			m_bIncrement = m_bIncrement?false:true;
		}
		else
		{
			m_nCompareType = CCommunityManager::CMemoCompare::EMCT_SENDER;
			m_bIncrement = true;
		}

		SetChangePage(1);
		break;
	case ID_MESSAGEBOX_TXTBTN_MESSAGE:
		if(m_nCompareType == CCommunityManager::CMemoCompare::EMCT_MEMO)
		{
			m_bIncrement = m_bIncrement?false:true;
		}
		else
		{
			m_nCompareType = CCommunityManager::CMemoCompare::EMCT_MEMO;
			m_bIncrement = true;
		}

		SetChangePage(1);
		break;
	case ID_MESSAGEBOX_TXTBTN_DATE:
		if(m_nCompareType == CCommunityManager::CMemoCompare::EMCT_DATE)
		{
			m_bIncrement = m_bIncrement?false:true;
		}
		else
		{
			m_nCompareType = CCommunityManager::CMemoCompare::EMCT_DATE;
			m_bIncrement = true;
		}

		SetChangePage(1);
		break;
	case ID_MESSAGEBOX_BTN_SEND_MESSAGE:
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND);
		#endif //UPGRADE_10210619_ADD_CLICKLOG

		//ShowSendMessageDialog("");
		break;
	case ID_MESSAGEBOX_BTN_CLOSE:
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			if (m_nSelID == ID_MESSAGEBOX_RAD_ALL_MESSAGE)
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND_RECEIVEDMEMO_CLOSE);
			else if (m_nSelID == ID_MESSAGEBOX_RAD_SAVE_MESSAGE)
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND_MEMO_CUBBYHOLE_CLOSE);
		#endif //UPGRADE_10210619_ADD_CLICKLOG

		OnCancel();
		break;
	case ID_MESSAGEBOX_BTN_GO_FIRST:
		SetChangePage(1);
		break;
	case ID_MESSAGEBOX_BTN_GO_PREV:
		SetChangePage(m_nCurPage-1);
		break;
	case ID_MESSAGEBOX_BTN_GO_NEXT:
		SetChangePage(m_nCurPage+1);
		break;
	case ID_MESSAGEBOX_BTN_GO_LAST:
	case ID_MESSAGEBOX_BTN_GO_NUMBER0:
	case ID_MESSAGEBOX_BTN_GO_NUMBER1:
	case ID_MESSAGEBOX_BTN_GO_NUMBER2:
	case ID_MESSAGEBOX_BTN_GO_NUMBER3:
	case ID_MESSAGEBOX_BTN_GO_NUMBER4:
		{
			SetChangePage(msg->GetObject()->GetUserData(0));
		}
		break;
	case ID_MESSAGEBOX_BTN_MESSAGE_DELETE:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				if (m_nSelID == ID_MESSAGEBOX_RAD_ALL_MESSAGE)
					SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND_RECEIVEDMEMO_DELETE);
				else if (m_nSelID == ID_MESSAGEBOX_RAD_SAVE_MESSAGE)
					SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND_MEMO_CUBBYHOLE_DELETE);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			LSY::CList *pList = (LSY::CList *)GetObject(ID_MESSAGEBOX_LIST_MESSAGE_BOX);
			LSY::CListItemData *pData = pList->GetFirstItem();
			MEMO *pMemo = NULL;
			CMessageBoxListItem *pItem = NULL;

			std::list<std::string> listNode;

			while(pData)
			{
				pItem = (CMessageBoxListItem *)pData->GetItem();
				if(pItem->m_pCheck->IsCheck())
				{
					pMemo = (MEMO *)pData->GetItemData();
					listNode.push_back(pMemo->node);
				}
				pData = pList->GetNextItem();
			}

			CMsgBoxDlg dlg(this);

			if(listNode.size() == 0)
			{
				dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "선택된 쪽지가 없습니다.");
				break;
			}
			else
			{
				CMemoNotifyDlg dlgNotify;
				if(dlgNotify.DoModal(this, CMemoNotifyDlg::EMNT_MEMO_DELETE) == IDCANCEL) break;
			}

			if(CCommunityManager::Instance()->IsConnect())
			{
				CCommunityManager::Instance()->SendRemoveMemo(listNode);
			}
			else
			{
				dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "커뮤니티 서버와 연결되어 있지 않아 선택된 쪽지를 삭제하지 못했습니다.");
			}
		}
		break;
	case ID_MESSAGEBOX_BTN_MESSAGE_SAVE:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				if (m_nSelID == ID_MESSAGEBOX_RAD_ALL_MESSAGE)
					SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND_RECEIVEDMEMO_KEEP);
				else if (m_nSelID == ID_MESSAGEBOX_RAD_SAVE_MESSAGE)
					SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND_MEMO_CUBBYHOLE_KEEP);
			#endif //UPGRADE_10210619_ADD_CLICKLOG

			LSY::CList *pList = (LSY::CList *)GetObject(ID_MESSAGEBOX_LIST_MESSAGE_BOX);
			LSY::CListItemData *pData = pList->GetFirstItem();
			MEMO *pMemo = NULL;
			CMessageBoxListItem *pItem = NULL;

			std::list<std::string> listNode;

			while(pData)
			{
				pItem = (CMessageBoxListItem *)pData->GetItem();
				if(pItem->m_pCheck->IsCheck())
				{
					pMemo = (MEMO *)pData->GetItemData();
					listNode.push_back(pMemo->node);
				}
				pData = pList->GetNextItem();
			}

			int nSize = (int)listNode.size();
			int nStorageCount = CCommunityManager::Instance()->GetTotalStorageMemoCount(); 

			CMsgBoxDlg dlg(this);

			if(nSize == 0)
			{
				dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "선택된 쪽지가 없습니다.");
				break;
			}
			else if(nSize + nStorageCount > CCommunityManager::Instance()->GetMaxStorageMemo())
			{
				CMemoNotifyDlg dlgNotify;
				dlgNotify.DoModal(this, CMemoNotifyDlg::EMNT_MEMO_STORAGE_FULL);
				break;
			}

			if(CCommunityManager::Instance()->IsConnect())
			{
				CCommunityManager::Instance()->SendStorageMoveMemo(listNode);
			}
			else
			{
				dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "커뮤니티 서버와 연결되어 있지 않아 선택된 쪽지를 쪽지보관함으로 이동하지 못했습니다.");
			}
		}
		break;
	case ID_MESSAGEBOX_BTN_MESSAGE_SEARCH:
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			if (m_nSelID == ID_MESSAGEBOX_RAD_ALL_MESSAGE)
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND_RECEIVEDMEMO_SEARCH);
			else if (m_nSelID == ID_MESSAGEBOX_RAD_SAVE_MESSAGE)
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND_MEMO_CUBBYHOLE_SEARCH);
		#endif //UPGRADE_10210619_ADD_CLICKLOG

		OnSearch();
		break;
	case ID_MESSAGEBOX_BTN_MESSAGE_REPORT:
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			if (m_nSelID == ID_MESSAGEBOX_RAD_ALL_MESSAGE)
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND_RECEIVEDMEMO_REPORT);
			else if (m_nSelID == ID_MESSAGEBOX_RAD_SAVE_MESSAGE)
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND_MEMO_CUBBYHOLE_REPORT);
		#endif //UPGRADE_10210619_ADD_CLICKLOG

		OnReport();
		break;
	}
	return TRUE;
}

LRESULT CMessageBox::OnComboSearchTypeChange(LSY::CMessage *pMsg)
{
	#ifdef UPGRADE_10210619_ADD_CLICKLOG
		LSY::CMO_SelChange *pSelChangeMessage = (LSY::CMO_SelChange*)pMsg;
		if (pSelChangeMessage->GetValue() == 0)
		{
			if (m_nSelID == ID_MESSAGEBOX_RAD_ALL_MESSAGE)
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND_RECEIVEDMEMO_CONTENT);
			else if (m_nSelID == ID_MESSAGEBOX_RAD_SAVE_MESSAGE)
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND_MEMO_CUBBYHOLE_CONTENT);
		}
		else if (pSelChangeMessage->GetValue() == 1)
		{
			if (m_nSelID == ID_MESSAGEBOX_RAD_ALL_MESSAGE)
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND_RECEIVEDMEMO_ID_NICKNAME);		
			else if (m_nSelID == ID_MESSAGEBOX_RAD_SAVE_MESSAGE)
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND_MEMO_CUBBYHOLE_ID_NICKNAME);
		}
	#endif //UPGRADE_10210619_ADD_CLICKLOG

	return TRUE;
}

void CMessageBox::SetTabChange(int nID, bool bCheckSameID/* = true*/)
{
	if(bCheckSameID && (m_nSelID == nID)) return;

	bool bEnable = true;

	m_nSelID = nID;
	if(m_nSendRemoveCount != 0 || m_nSendMemoListToStorageCount != 0)
	{
		bEnable = false;
	}
	else
	{
		switch (m_nSelID)
		{
		case ID_MESSAGEBOX_RAD_ALL_MESSAGE:
			if(m_bSendMemoBox) bEnable = false;
			break;
		case ID_MESSAGEBOX_RAD_SAVE_MESSAGE:
			if(m_bSendStorageMemoBox) bEnable = false;
			break;
		}
	}

	m_pGrpAll->SetEnable(bEnable);
	if(bEnable) m_pLoading->StopAnimation();
	else m_pLoading->StartAnimation();

	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_MESSAGEBOX_BTN_MESSAGE_SAVE);
	pButton->SetEnable((m_nSelID == ID_MESSAGEBOX_RAD_ALL_MESSAGE)?true:false);

	m_bSearch = false;
	m_bSearchNickName = false;
	m_strSearch = "";

	m_nCompareType = CCommunityManager::CMemoCompare::EMCT_DATE;
	m_bIncrement = false; //날짜 최신순으로 정렬

	SetChangePage(1);
}

void CMessageBox::SetChangePage(int nPage)
{
	m_nCurPage = nPage;
	int nMemoCount = 0;
	int nStorageMemoCount = 0;
	int nCount = 0;

	if(m_pGrpAll->IsEnable())
	{
		LSY::CList *pList = (LSY::CList *)GetObject(ID_MESSAGEBOX_LIST_MESSAGE_BOX);
		pList->DeleteAllItem();
		if(m_bSearch) nCount = CCommunityManager::Instance()->GetSearchPage((m_nSelID == ID_MESSAGEBOX_RAD_ALL_MESSAGE)?true:false, pList, m_nCompareType, m_bIncrement, m_bSearchNickName, m_strSearch, m_nCurPage, nMemoCount, nStorageMemoCount);
		else nCount = CCommunityManager::Instance()->GetPage((m_nSelID == ID_MESSAGEBOX_RAD_ALL_MESSAGE)?true:false, pList, m_nCompareType, m_bIncrement, m_nCurPage, nMemoCount, nStorageMemoCount);
	}
	else
	{
		m_nCurPage = 1;
	}

	CheckMessageCount(nMemoCount, nStorageMemoCount);
	CheckTabColor(nMemoCount, nStorageMemoCount);
	CheckGoButton(nCount);
}

LRESULT CMessageBox::OnTabChildNotify(LSY::CMessage *pMsg)
{
	LSY::CMO_LocalChildNotify *msg = (LSY::CMO_LocalChildNotify *)pMsg;
	LSY::CMessage *pChildMsg = msg->GetMessage();

	if(pChildMsg->GetID() == LSY::EM_O_MOUSELCLICK)
	{
		LSY::CMO_MouseLClick *msgChild = (LSY::CMO_MouseLClick *)pChildMsg;
		int nID = msgChild->GetObject()->GetID();
		switch(nID)
		{
		case ID_MESSAGEBOX_RAD_ALL_MESSAGE:
		case ID_MESSAGEBOX_RAD_SAVE_MESSAGE:
			if(m_nSelID == nID)
			{
				m_bSearch = false;
				m_bSearchNickName = false;
				m_strSearch = "";

				m_nCompareType = CCommunityManager::CMemoCompare::EMCT_DATE;
				m_bIncrement = true;

				SetChangePage(1);
			}
			else
			{
				#ifdef UPGRADE_10210619_ADD_CLICKLOG
					if (nID == ID_MESSAGEBOX_RAD_ALL_MESSAGE)
						SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND_RECEIVEDMEMO);
					else if (nID == ID_MESSAGEBOX_RAD_SAVE_MESSAGE)
						SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_PERSONAL_AREA_POKER_MEMOSEND_MEMO_CUBBYHOLE);
				#endif //UPGRADE_10210619_ADD_CLICKLOG
				SetTabChange(nID);
			}
			return TRUE;
		}
	}

	return FALSE;
}

LRESULT CMessageBox::OnMouseOver(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;

	LSY::CImage *pImage = NULL;
	switch(msg->GetObject()->GetID())
	{
	case ID_MESSAGEBOX_IMG_ALL_MESSAGE_FULL:
		pImage = (LSY::CImage *)GetObject(ID_MESSAGEBOX_IMG_ALL_MESSAGE_POPUP);
		break;
	case ID_MESSAGEBOX_IMG_SAVE_MESSAGE_FULL:
		pImage = (LSY::CImage *)GetObject(ID_MESSAGEBOX_IMG_SAVE_MESSAGE_POPUP);
		break;
	}
	if(pImage) pImage->SetShow(true);
	return TRUE;
}

LRESULT CMessageBox::OnMouseLeave(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;

	LSY::CImage *pImage = NULL;
	switch(msg->GetObject()->GetID())
	{
	case ID_MESSAGEBOX_IMG_ALL_MESSAGE_FULL:
		pImage = (LSY::CImage *)GetObject(ID_MESSAGEBOX_IMG_ALL_MESSAGE_POPUP);
		break;
	case ID_MESSAGEBOX_IMG_SAVE_MESSAGE_FULL:
		pImage = (LSY::CImage *)GetObject(ID_MESSAGEBOX_IMG_SAVE_MESSAGE_POPUP);
		break;
	}
	if(pImage) pImage->SetShow(false);
	return TRUE;
}

void CMessageBox::ReceiveMemoList(bool bMemoBox)
{
	bool bEnable = false;
	if(m_nSendRemoveCount == 0 && m_nSendMemoListToStorageCount == 0)
	{
		switch (m_nSelID)
		{
		case ID_MESSAGEBOX_RAD_ALL_MESSAGE:
			if(bMemoBox) bEnable = true;
			break;
		case ID_MESSAGEBOX_RAD_SAVE_MESSAGE:
			if(!bMemoBox) bEnable = true;
			break;
		}
	}

	if(bMemoBox) m_bSendMemoBox = false;
	else m_bSendStorageMemoBox = false;

	if(bEnable)
	{
		m_pGrpAll->SetEnable(true);
		m_pLoading->StopAnimation();

		SetChangePage(1);
	}
	//탭 숫자 변경 표시
	else
	{
		int nCount = 0;
		switch (m_nSelID)
		{
		case ID_MESSAGEBOX_RAD_ALL_MESSAGE:
			if(!bMemoBox)
			{
				CCommunityManager::Instance()->MoveReciveStorageMemoListToStorageMomoList(&nCount);
				CheckMessageCount(-1, nCount);
			}
			break;
		case ID_MESSAGEBOX_RAD_SAVE_MESSAGE:
			if(bMemoBox)
			{
				int nCount = 0;
				CCommunityManager::Instance()->MoveReciveMemoListToMomoList(&nCount);
				CheckMessageCount(nCount, -1);
			}
			break;
		}
	}
}

void CMessageBox::OnSearch(void)
{
	LSY::CEdit *pEdit = (LSY::CEdit *)GetObject(ID_MESSAGEBOX_EDIT_SEARCH);
	CString strText = pEdit->GetText().c_str();
	strText.Trim();

	bool bOldSearch = m_bSearch;
	bool bOldSearchNickName = m_bSearchNickName;
	std::string strOldSearch = m_strSearch;

	m_bSearch = (strText == "")?false:true;
	if(m_bSearch)
	{
		LSY::CCombo *pCombo = (LSY::CCombo *)GetObject(ID_MESSAGEBOX_COMBO_SEARCH_TYPE);
		m_bSearchNickName = pCombo->GetCurSelectItem()?true:false;
		m_strSearch = strText.GetString();

		if(m_bSearch != bOldSearch || m_bSearchNickName != bOldSearchNickName || m_strSearch != strOldSearch)
		{
			SetChangePage(1);
			
			LSY::CList *pList = (LSY::CList *)GetObject(ID_MESSAGEBOX_LIST_MESSAGE_BOX);
			if(pList->GetItemCount() == 0)
			{
				CMsgBoxDlg dlg(this);
				dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "검색 결과가 없습니다.");
			}
		}
	}
	else
	{
		m_bSearchNickName = false;
		m_strSearch = "";

		if(m_bSearch != bOldSearch)
		{
			SetChangePage(1);
		}
	}

	pEdit->SetText("");
}

void CMessageBox::OnReport(void)
{
	LSY::CList *pList = (LSY::CList *)GetObject(ID_MESSAGEBOX_LIST_MESSAGE_BOX);
	LSY::CListItemData *pData = pList->GetFirstItem();
	MEMO *pMemo = NULL;
	CMessageBoxListItem *pItem = NULL;

	std::list<MEMO *> listMemo;

	CString strTemp;
	bool bAdmin = false;

	while(pData)
	{
		pItem = (CMessageBoxListItem *)pData->GetItem();
		if(pItem->m_pCheck->IsCheck())
		{
			pMemo = (MEMO *)pData->GetItemData();
			if(!CCommunityManager::IsAdminMemo(pMemo))
			{
				listMemo.push_back(pMemo);
			}
			else bAdmin = true;
		}
		pData = pList->GetNextItem();
	}

	if(listMemo.size() == 0)
	{
		CMsgBoxDlg dlg(this);

		if(bAdmin) dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "운영자 메시지는 신고할 수 없습니다.");
		else dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "선택된 쪽지가 없습니다.");
		return;
	}

	GM().BadUserReportMemoBox(listMemo);
}

void CMessageBox::OnReadMemoList(void)
{
	MEMO memo;
	LSY::CList *pList = (LSY::CList *)GetObject(ID_MESSAGEBOX_LIST_MESSAGE_BOX);
	if(CCommunityManager::Instance()->GetReadMemo(memo, pList))
	{
		ShowReceiveMessageDialog(&memo, this);
	}
}

void CMessageBox::ResultRemoveMemo(const bool &bSuccess)
{
	bool bRemoveMemoBox = false;
	bool bRemoveStorageMemoBox = false;
	CCommunityManager::Instance()->ResultRemoveMemo(bSuccess, bRemoveMemoBox, bRemoveStorageMemoBox, m_nSendRemoveCount);

	bool bEnable = false;
	if(m_nSendRemoveCount == 0 && m_nSendMemoListToStorageCount == 0 && !m_bSendMemoBox && !m_bSendStorageMemoBox)
	{
		bEnable = true;
	}

	if(bEnable)
	{
		m_pGrpAll->SetEnable(true);
		m_pLoading->StopAnimation();

		if(bSuccess)
		{
			m_pAllCheck->SetCheck(false);
			if(bRemoveMemoBox && (m_nSelID == ID_MESSAGEBOX_RAD_ALL_MESSAGE)) SetChangePage(m_nCurPage);
			if(bRemoveStorageMemoBox && (m_nSelID == ID_MESSAGEBOX_RAD_SAVE_MESSAGE)) SetChangePage(m_nCurPage);
		}
	}
}

void CMessageBox::ResultMoveBoxToStrageMemo(const bool &bSuccess)
{
	CCommunityManager::Instance()->ResultMoveBoxToStrageMemo(bSuccess, m_nSendMemoListToStorageCount);

	bool bEnable = false;
	if(m_nSendRemoveCount == 0 && m_nSendMemoListToStorageCount == 0 && !m_bSendMemoBox && !m_bSendStorageMemoBox)
	{
		bEnable = true;
	}

	if(bEnable)
	{
		m_pGrpAll->SetEnable(true);
		m_pLoading->StopAnimation();

		if(bSuccess)
		{
			m_pAllCheck->SetCheck(false);
			SetChangePage(m_nCurPage);

			CMemoNotifyDlg dlgNotify;
			dlgNotify.DoModal(this, CMemoNotifyDlg::EMNT_MEMO_MOVE_STORAGE);
		}
	}
}

void CMessageBox::DisableList(void)
{
	m_pGrpAll->SetEnable(false);
	m_pLoading->StartAnimation();
}

void CMessageBox::OnDestroy()
{
	LSY::CCheckBox *pCheckBox = (LSY::CCheckBox *)GetObject(ID_MESSAGEBOX_CHECK_NO_RECEIVE_MESSAGE);
	if(pCheckBox->IsCheck())
	{
		if(CCommunityManager::Instance()->IsConnect())
		{
			if(CCommunityManager::Instance()->GetOption(COM_ECOT_MEMO) != COM_ECOPT_OFF) CCommunityManager::Instance()->SetOption(COM_ECOT_MEMO, COM_ECOPT_OFF);
		}
		else
		{
			ShowMainMessageDlg("커뮤니티 서버와 접속되어있지 않아 쪽지 수신 여부가 적용되지 않았습니다.");
		}
	}

	__super::OnDestroy();
}