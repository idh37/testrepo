#include "StdAfx.h"
#include "WhisperlistCombo.h"
#include "ChatGroup.h"

#define _DEF_MAX_WHIPER_ID			5
#define _DEF_WHISPERLIST_ALL_USER	"ÀüÃ¼´ëÈ­"

CWhisperlistCombo::CWhisperlistCombo(const bool &bCloseCombo, CChatGroup *pChatGroup)
:	LSY::CCombo(),
	m_bCloseCombo(bCloseCombo),
	m_pChatGroup(pChatGroup),
	m_pTab(NULL),
	m_pTxtListAllUser(NULL),
	m_pTxtListLately(NULL),
	m_pTxtListConnectBuddy(NULL),
	m_pConnectBuddyInfo(NULL),
	m_pConnectBuddyInfoPopup(NULL),
	m_pRadioConnectBuddy(NULL)
{
}

CWhisperlistCombo::~CWhisperlistCombo()
{
}

void CWhisperlistCombo::OnCreated(void)
{
	if(m_bCloseCombo) OnCreatedClose();
	else OnCreatedOpen();

	m_pTxtListAllUser->AddText(_DEF_WHISPERLIST_ALL_USER);
	m_pTxtListAllUser->SetCurSelectItem(0);
	m_pEdit->SetText(_DEF_WHISPERLIST_ALL_USER);

	m_pTxtListAllUser->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CWhisperlistCombo::OnSelChangeList));
	m_pTxtListLately->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CWhisperlistCombo::OnSelChangeList));
	m_pTxtListConnectBuddy->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CWhisperlistCombo::OnSelChangeList));
	m_pTxtListConnectBuddy->SetAutoSort(true);

	m_pTxtListAllUser->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CCombo::OnClickList));
	m_pTxtListLately->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CCombo::OnClickList));
	m_pTxtListConnectBuddy->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CCombo::OnClickList));

	m_pConnectBuddyInfo->AddHandler(LSY::EM_O_MOUSEOVER, LSY::Fnt(this, &CWhisperlistCombo::OnInfoMouseOver));
	m_pConnectBuddyInfo->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this, &CWhisperlistCombo::OnInfoMouseLeave));

	m_pConnectBuddyInfo->AddHandler(LSY::EM_O_LBUTTONDOWN, LSY::Fnt(this, &CWhisperlistCombo::OnPopupLButtonDown));
	m_pConnectBuddyInfo->AddHandler(LSY::EM_O_LBUTTONUP, LSY::Fnt(this, &CWhisperlistCombo::OnPopupLButtonUp));
	m_pConnectBuddyInfo->AddHandler(LSY::EM_O_LBUTTONDBCLICK, LSY::Fnt(this, &CWhisperlistCombo::OnPopupDBClick));
	m_pConnectBuddyInfo->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CWhisperlistCombo::OnPopupLButtonClick));

	m_pConnectBuddyInfoPopup->SetShow(false);
}

LRESULT CWhisperlistCombo::OnPopupLButtonDown(LSY::CMessage *msg)
{
	LSY::CMO_LButtonDown *pMsg = (LSY::CMO_LButtonDown *)msg;
	m_pRadioConnectBuddy->OnLButtonDown(pMsg->GetPos());
	return TRUE;
}

LRESULT CWhisperlistCombo::OnPopupLButtonUp(LSY::CMessage *msg)
{
	LSY::CMO_LButtonUp *pMsg = (LSY::CMO_LButtonUp *)msg;
	m_pRadioConnectBuddy->OnLButtonUP(pMsg->GetPos());
	return TRUE;
}

LRESULT CWhisperlistCombo::OnPopupDBClick(LSY::CMessage *msg)
{
	LSY::CMO_LButtonDbClick *pMsg = (LSY::CMO_LButtonDbClick *)msg;
	m_pRadioConnectBuddy->OnLButtonDbClick(pMsg->GetPos());
	return TRUE;
}

LRESULT CWhisperlistCombo::OnPopupLButtonClick(LSY::CMessage *msg)
{
	LSY::CMO_MouseLClick *pMsg = (LSY::CMO_MouseLClick *)msg;
	m_pRadioConnectBuddy->OnMouseLClick(pMsg->GetPos());
	return TRUE;
}

void CWhisperlistCombo::OnCreatedClose(void)
{
	LSY::CObjectProject *pProject = (LSY::CObjectProject *)GetObjectMan()->GetProjectFromID(ID_LOBBY);
	m_pTab = (LSY::CTabControl *)pProject->GetObject(ID_LOBBY_TAB_WHISPERLIST_CLOSE);

	m_pTxtListAllUser = (LSY::CTextList *)pProject->GetObject(ID_LOBBY_TEXTLIST_WHISPERLIST_ALLUSER_CLOSE);
	m_pTxtListLately = (LSY::CTextList *)pProject->GetObject(ID_LOBBY_TEXTLIST_WHISPERLIST_LATELY_CLOSE);
	m_pTxtListConnectBuddy = (LSY::CTextList *)pProject->GetObject(ID_LOBBY_TEXTLIST_WHISPERLIST_CONNECT_BUDDY_CLOSE);

	m_pConnectBuddyInfo = (LSY::CImage *)pProject->GetObject(ID_LOBBY_IMG_WHISPERLIST_CONNECT_BUDDY_INFO_CLOSE);
	m_pConnectBuddyInfoPopup = (LSY::CImage *)pProject->GetObject(ID_LOBBY_IMG_WHISPERLIST_CONNECT_BUDDY_INFO_POPUP_CLOSE);

	m_pRadioConnectBuddy = (LSY::CRadio *)pProject->GetObject(ID_LOBBY_RADIO_WHISPERLIST_CONNECT_BUDDY_CLOSE);
}

void CWhisperlistCombo::OnCreatedOpen(void)
{
	LSY::CObjectProject *pProject = (LSY::CObjectProject *)GetObjectMan()->GetProjectFromID(ID_LOBBY);
	m_pTab = (LSY::CTabControl *)pProject->GetObject(ID_LOBBY_TAB_WHISPERLIST_OPEN);

	m_pTxtListAllUser = (LSY::CTextList *)pProject->GetObject(ID_LOBBY_TEXTLIST_WHISPERLIST_ALLUSER_OPEN);
	m_pTxtListLately = (LSY::CTextList *)pProject->GetObject(ID_LOBBY_TEXTLIST_WHISPERLIST_LATELY_OPEN);
	m_pTxtListConnectBuddy = (LSY::CTextList *)pProject->GetObject(ID_LOBBY_TEXTLIST_WHISPERLIST_CONNECT_BUDDY_OPEN);

	m_pConnectBuddyInfo = (LSY::CImage *)pProject->GetObject(ID_LOBBY_IMG_WHISPERLIST_CONNECT_BUDDY_INFO_OPEN);
	m_pConnectBuddyInfoPopup = (LSY::CImage *)pProject->GetObject(ID_LOBBY_IMG_WHISPERLIST_CONNECT_BUDDY_INFO_POPUP_OPEN);

	m_pRadioConnectBuddy = (LSY::CRadio *)pProject->GetObject(ID_LOBBY_RADIO_WHISPERLIST_CONNECT_BUDDY_OPEN);
}

LRESULT CWhisperlistCombo::OnSelChangeList(LSY::CMessage *msg)
{
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	LSY::CTextList *pTextList = (LSY::CTextList *)pMsg->GetObject();

	m_nCurSel = pMsg->GetValue();
	std::string strText = pTextList->GetItemString(m_nCurSel);
	if(m_pList)
	{
		m_pList->RemoveAll();
		m_nCurSel = m_pList->AddText(strText);
	}

	if(strText != _DEF_WHISPERLIST_ALL_USER)
	{
		strText = strText.substr(7);
		AddChattingWhisperList(strText.c_str(), true, false, false);
	}
	else
	{
		AddChattingWhisperList("", false, false, true);
	}

	if(m_pEdit) m_pEdit->SetText(strText);
	if(m_pProject) m_pProject->SetPopup(NULL, NULL);

	return TRUE;
}

LRESULT CWhisperlistCombo::OnClickButton(LSY::CMessage *pMsg)
{
	if(m_pProject->GetPopup() == m_pList) m_pProject->SetPopup(NULL, NULL);
	else
	{
		int nIndex = -1;
		int nTotal = m_pList->GetItemCount();
		std::string strOutput = "";
		if(m_pEdit) strOutput = m_pEdit->GetText();

		CCommunityManager::Instance()->SetConnectBuddyListToTextList(m_pTxtListConnectBuddy);

		if(strOutput == _DEF_WHISPERLIST_ALL_USER)
		{
			SetAllUser();
		}
		else
		{
			m_pTxtListAllUser->SetCurSelectItem(-1);

			std::string strID = strOutput;
			strID.insert(0, "[±Ó¸»] ");
		
			int nIndex = m_pTxtListConnectBuddy->FirstFind((LPARAM)strID.c_str());
			if(nIndex >= 0)
			{
				m_pTxtListConnectBuddy->SetCurSelectItem(nIndex);
				if(!m_pTxtListConnectBuddy->IsVisualItem(nIndex)) m_pTxtListConnectBuddy->SetVisibleItem(nIndex, DT_VCENTER);
			}
			else
			{
				m_pTxtListConnectBuddy->SetCurSelectItem(-1);
				m_pTxtListConnectBuddy->SetVisibleItem(0, DT_TOP);
			}

			nIndex = m_pTxtListLately->FirstFind((LPARAM)strID.c_str());
			m_pTxtListLately->SetCurSelectItem(nIndex);
		}

		m_pProject->SetPopup(m_pList, NULL);
		m_pList->SetShow(true);

		m_pConnectBuddyInfoPopup->SetShow(false);
		m_pTab->SetCurSelFromID(m_bCloseCombo?ID_LOBBY_RADIO_WHISPERLIST_LATELY_CLOSE:ID_LOBBY_RADIO_WHISPERLIST_LATELY_OPEN);
	}
	return TRUE;
}

LRESULT CWhisperlistCombo::OnEditReturn(LSY::CMessage *pMsg)
{
	return TRUE;
}

void CWhisperlistCombo::AddWhisper(const std::string &strWhisper, const bool &bSetCurWhisper, const bool &bStartAnimation)
{
	int nCount = m_pTxtListLately->GetItemCount();
	int nIndex = -1;
	int nCurSel = m_pTxtListLately->GetCurSelectItem();

	bool bReposition = true;

	std::string strOldWhisper = strWhisper;
	std::string strOldSelect = GetText();
	std::string strNewWhisper = strWhisper;
	strNewWhisper.insert(0, "[±Ó¸»] ");
	nIndex = m_pTxtListLately->FirstFind((LPARAM)strNewWhisper.c_str());

	if(bSetCurWhisper)
	{
		m_pTxtListAllUser->SetCurSelectItem(-1);
		m_pEdit->SetText(strOldWhisper);
	}

	if(nIndex != -1)
	{
		if(nIndex == nCount - 1)
		{
			if(!bSetCurWhisper && bStartAnimation)
				m_pChatGroup->StartRecevieChatAnimation();

			return;
		}

		m_pTxtListLately->RemoveText(nIndex);
		m_pTxtListLately->AddText(strNewWhisper);

		if(bSetCurWhisper || (nCurSel == nIndex))
		{
			m_pTxtListLately->SetCurSelectItem(nCount-1);
		}
		else if(nCurSel > nIndex)
		{
			m_pTxtListLately->SetCurSelectItem(nIndex-1);
		}
	}
	else if(nCount >= _DEF_MAX_WHIPER_ID)
	{
		bReposition = false;
		if(nCurSel != 0) nIndex = 0;
		else nIndex = 1;

		m_pTxtListLately->RemoveText(nIndex);
		m_pTxtListLately->AddText(strNewWhisper);

		if(bSetCurWhisper || (nCurSel == nIndex))
		{
			m_pTxtListLately->SetCurSelectItem(nCount-1);
		}
		else if(nCurSel > nIndex)
		{
			m_pTxtListLately->SetCurSelectItem(nIndex-1);
		}
	}
	else
	{
		m_pTxtListLately->AddText(strNewWhisper);

		if(bSetCurWhisper)
		{
			m_pTxtListLately->SetCurSelectItem(nCount);
		}
	}

	if(bSetCurWhisper)
	{
		SetText(strOldWhisper);
	}
	else
	{
		SetText(strOldSelect);
	}

	if(bStartAnimation)
		m_pChatGroup->StartRecevieChatAnimation();
}

void CWhisperlistCombo::SetAllUser(void)
{
	m_pTxtListAllUser->SetCurSelectItem(0);
	m_pTxtListLately->SetCurSelectItem(-1);
	m_pTxtListConnectBuddy->SetCurSelectItem(-1);

	m_pEdit->SetText(_DEF_WHISPERLIST_ALL_USER);
}

bool CWhisperlistCombo::IsAllUser(void)
{
	if(m_pEdit->GetText() == _DEF_WHISPERLIST_ALL_USER && m_pTxtListAllUser->GetCurSelectItem() == 0)
		return true;

	return false;
}

LRESULT CWhisperlistCombo::OnInfoMouseOver(LSY::CMessage *msg)
{
	m_pConnectBuddyInfoPopup->SetShow(true);
	m_pRadioConnectBuddy->OnMouseOver();
	return TRUE;
}

LRESULT CWhisperlistCombo::OnInfoMouseLeave(LSY::CMessage *msg)
{
	m_pConnectBuddyInfoPopup->SetShow(false);
	m_pRadioConnectBuddy->OnMouseLeave();
	return TRUE;
}