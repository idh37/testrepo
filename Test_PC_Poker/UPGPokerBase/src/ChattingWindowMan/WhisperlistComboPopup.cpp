#include "stdafx.h"
#include "WhisperlistComboPopup.h"
#include "WhisperlistComboPopupID.h"
#include "ChattingWindowMan.h"
#include "../GlobalPokerBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define _DEF_MAX_WHIPER_ID 5

IMPLEMENT_DYNAMIC(CWhisperlistComboPopup, LSY::CLSYLibWindowsBase)

BEGIN_MESSAGE_MAP(CWhisperlistComboPopup, LSY::CLSYLibWindowsBase)
	ON_WM_DESTROY()
	ON_WM_CREATE()
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CWhisperlistComboPopup, LSY::CLSYLibWindowsBase)
END_DISPATCH_MAP()

CWhisperlistComboPopup::CWhisperlistComboPopup()
:	LSY::CLSYLibWindowsBase(),
	m_pTab(NULL),
	m_pTxtListAllUser(NULL),
	m_pTxtListLately(NULL),
	m_pTxtListConnectBuddy(NULL),
	m_pConnectBuddyInfo(NULL),
	m_pRadioConnectBuddy(NULL),
	m_bNotHideKillFocus(false)
{
	MM().AppendMsgH(UPG_ADD_CHATTING_WHISPER_LIST, Fnt(this, &CWhisperlistComboPopup::OnAddWhisper));
	m_strText = _DEF_GAME_WHISPERLIST_ALL_USER;
}

CWhisperlistComboPopup::~CWhisperlistComboPopup()
{
	MM().RemoveAppendMsgH( UPG_ADD_CHATTING_WHISPER_LIST, Fnt( this, &CWhisperlistComboPopup::OnAddWhisper ) );
}

bool CWhisperlistComboPopup::Create(int x, int y)
{
	Init(GetObjectMan(), ID_WHISPERLISTPOPUP, true);
	if(!CLSYLibWindowsBase::CreateEx(0, AfxRegisterWndClass(CS_DBLCLKS), "WhisperlistComboPopup", WS_POPUP | WS_OVERLAPPED | WS_CLIPCHILDREN, x, y, 100, 100, GM().GetMainWnd()->GetSafeHwnd(), NULL))
	{
		return false;
	}

	SetWindowPos(&CWnd::wndTop, 0,0,0,0, SWP_NOMOVE | SWP_NOSIZE);
	return true;
}

bool CWhisperlistComboPopup::OnCreatedProject(void)
{
	m_pTab = (LSY::CTabControl *)GetObject(ID_WHISPERLISTPOPUP_TAB);
	m_pTxtListAllUser = (LSY::CTextList *)GetObject(ID_WHISPERLISTPOPUP_TEXTLIST_ALLUSER);
	m_pTxtListLately = (LSY::CTextList *)GetObject(ID_WHISPERLISTPOPUP_TEXTLIST_LATELY);
	m_pTxtListConnectBuddy = (LSY::CTextList *)GetObject(ID_WHISPERLISTPOPUP_TEXTLIST_CONNECT_BUDDY);
	m_pConnectBuddyInfo = (LSY::CImage *)GetObject(ID_WHISPERLISTPOPUP_IMG_WHISPER_CONNECT_BUDDY_INFO);
	m_pRadioConnectBuddy = (LSY::CRadio *)GetObject(ID_WHISPERLISTPOPUP_RADIO_CONNECT_BUDDY);

	m_pTab->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CWhisperlistComboPopup::OnSelChangeChatList));

	m_pTxtListAllUser->AddText(_DEF_GAME_WHISPERLIST_ALL_USER);
	m_pTxtListAllUser->SetCurSelectItem(0);
	SetText(_DEF_GAME_WHISPERLIST_ALL_USER);

	m_pTxtListAllUser->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CWhisperlistComboPopup::OnSelChangeList));
	m_pTxtListLately->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CWhisperlistComboPopup::OnSelChangeList));
	m_pTxtListConnectBuddy->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CWhisperlistComboPopup::OnSelChangeList));
	m_pTxtListConnectBuddy->SetAutoSort(true);

	m_pTxtListAllUser->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CWhisperlistComboPopup::OnClickList));
	m_pTxtListLately->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CWhisperlistComboPopup::OnClickList));
	m_pTxtListConnectBuddy->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CWhisperlistComboPopup::OnClickList));

	m_pConnectBuddyInfo->AddHandler(LSY::EM_O_MOUSEOVER, LSY::Fnt(this, &CWhisperlistComboPopup::OnInfoMouseOver));
	m_pConnectBuddyInfo->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this, &CWhisperlistComboPopup::OnInfoMouseLeave));

	m_pConnectBuddyInfo->AddHandler(LSY::EM_O_LBUTTONDOWN, LSY::Fnt(this, &CWhisperlistComboPopup::OnPopupLButtonDown));
	m_pConnectBuddyInfo->AddHandler(LSY::EM_O_LBUTTONUP, LSY::Fnt(this, &CWhisperlistComboPopup::OnPopupLButtonUp));
	m_pConnectBuddyInfo->AddHandler(LSY::EM_O_LBUTTONDBCLICK, LSY::Fnt(this, &CWhisperlistComboPopup::OnPopupDBClick));
	m_pConnectBuddyInfo->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CWhisperlistComboPopup::OnPopupLButtonClick));

	SetShow(false);

	m_cPopup.Create(296, 10, this);
	m_cPopup.SetShow(false);
	return TRUE;
}

LRESULT CWhisperlistComboPopup::OnSelChangeChatList(LSY::CMessage *msg)
{
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();

#ifdef UPGRADE_10210619_ADD_CLICKLOG
	if (nIndex == 0)
		SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_CHATWINDOW_CHATTING_RECENT_WHISPER_USER);
	else if (nIndex == 1)
		SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_CHATWINDOW_CHATTING_FRIEND);
#endif //UPGRADE_10210619_ADD_CLICKLOG

	return TRUE;
}

LRESULT CWhisperlistComboPopup::OnPopupLButtonDown(LSY::CMessage *msg)
{
	LSY::CMO_LButtonDown *pMsg = (LSY::CMO_LButtonDown *)msg;
	m_pRadioConnectBuddy->OnLButtonDown(pMsg->GetPos());
	return TRUE;
}

LRESULT CWhisperlistComboPopup::OnPopupLButtonUp(LSY::CMessage *msg)
{
	LSY::CMO_LButtonUp *pMsg = (LSY::CMO_LButtonUp *)msg;
	m_pRadioConnectBuddy->OnLButtonUP(pMsg->GetPos());
	return TRUE;
}

LRESULT CWhisperlistComboPopup::OnPopupDBClick(LSY::CMessage *msg)
{
	LSY::CMO_LButtonDbClick *pMsg = (LSY::CMO_LButtonDbClick *)msg;
	m_pRadioConnectBuddy->OnLButtonDbClick(pMsg->GetPos());
	return TRUE;
}

LRESULT CWhisperlistComboPopup::OnPopupLButtonClick(LSY::CMessage *msg)
{
	LSY::CMO_MouseLClick *pMsg = (LSY::CMO_MouseLClick *)msg;
	m_pRadioConnectBuddy->OnMouseLClick(pMsg->GetPos());
	return TRUE;
}

LRESULT CWhisperlistComboPopup::OnSelChangeList(LSY::CMessage *msg)
{
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	LSY::CTextList *pTextList = (LSY::CTextList *)pMsg->GetObject();

	int nCurSel = pMsg->GetValue();
	std::string strText = pTextList->GetItemString(nCurSel);

	if(strText != _DEF_GAME_WHISPERLIST_ALL_USER)
	{
		strText = strText.substr(7);
		AddChattingWhisperList(strText.c_str(), true, false, false);
	}
	else
	{
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_CHATWINDOW_CHATTING_TALK);
		#endif //UPGRADE_10210619_ADD_CLICKLOG

		AddChattingWhisperList("", false, false, true);
	}

	SetText(strText);
	SetShow(false);

	return TRUE;
}

LRESULT CWhisperlistComboPopup::OnClickList(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseLClick *msg = (LSY::CMO_MouseLClick *)pMsg;
	SetShow(false);
	return TRUE;
}

LRESULT CWhisperlistComboPopup::OnInfoMouseOver(LSY::CMessage *msg)
{
	m_cPopup.SetShow(true);
	m_pRadioConnectBuddy->OnMouseOver();
	return TRUE;
}

LRESULT CWhisperlistComboPopup::OnInfoMouseLeave(LSY::CMessage *msg)
{
	m_cPopup.SetShow(false);
	m_pRadioConnectBuddy->OnMouseLeave();
	return TRUE;
}

LRESULT CWhisperlistComboPopup::WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
{
	if(message == WM_KILLFOCUS)
	{
		if(GetFocus() == &m_cPopup) return TRUE;
		if(GetFocus() == this) return TRUE;
		if(m_bNotHideKillFocus)	m_bNotHideKillFocus = false;
		else SetShow(false);
		return TRUE;
	}

	LRESULT nRet = LSY::CLSYLibWindowsBase::WindowProc(message, wParam, lParam);

	if(nRet == FALSE)
	{
		switch(message)
		{
		case WM_LBUTTONDOWN:
		case WM_LBUTTONDBLCLK:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONDBLCLK:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONDBLCLK:
			{
				CPoint ptPos;
				GetCursorPos(&ptPos);
				if(!m_rtRect.PtInRect(ptPos))
				{
					CWnd *pWnd = WindowFromPoint(ptPos);
					pWnd->ScreenToClient(&ptPos);
					if(PBIsComboPopupClose(ptPos)) SetShow(false);
					else m_bNotHideKillFocus = true;
					pWnd->SendMessage(message, wParam, MAKELPARAM(ptPos.x, ptPos.y));
				}
			}
			return TRUE;
		}
	}

	return nRet;
}

void CWhisperlistComboPopup::SetShow(bool bShow)
{
	if(IsShow() == bShow) return;
	m_bNotHideKillFocus = false;

	if(bShow)
	{
		m_pObjectMan->SetMouseCapture();
		CWnd *pWnd = GM().GetCurrentGameView();
		SetParent(pWnd);

		SetWindowPos(NULL, 12, 588, 0, 0, SWP_NOSIZE|SWP_NOZORDER|SWP_SHOWWINDOW);
		GetWindowRect(&m_rtRect);

		int nIndex = -1;
		std::string strOutput = GetText();

		CCommunityManager::Instance()->SetConnectBuddyListToTextList(m_pTxtListConnectBuddy);

		if(strOutput == _DEF_GAME_WHISPERLIST_ALL_USER)
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

		m_cPopup.SetShow(false);
		m_pTab->SetCurSelFromID(ID_WHISPERLISTPOPUP_RADIO_LATELY);
	}
	else 
	{
		SetParent(GM().GetMainWnd());
		ShowWindow(SW_HIDE);
		m_pObjectMan->ReleaseMouseCapture();
	}
}

bool CWhisperlistComboPopup::IsShow(void)
{
	return IsWindowVisible()?true:false;
}

void CWhisperlistComboPopup::SetText(const std::string &strText)
{
	if(m_strText == strText) return;
	m_strText = strText;

	CChattingWindowMan *pChattingMan = GetCurrentChttingWindowMan();
	if(pChattingMan) pChattingMan->OnSetWhisper();
}

void CWhisperlistComboPopup::AddWhisper(const std::string &strWhisper, const bool &bSetCurWhisper, const bool &bStartAnimation)
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
		SetText(strOldWhisper);
	}

	if(nIndex != -1)
	{
		if(nIndex == nCount - 1)
		{
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
}

void CWhisperlistComboPopup::SetAllUser(void)
{
	m_pTxtListAllUser->SetCurSelectItem(0);
	m_pTxtListLately->SetCurSelectItem(-1);
	m_pTxtListConnectBuddy->SetCurSelectItem(-1);

	SetText(_DEF_GAME_WHISPERLIST_ALL_USER);
}

bool CWhisperlistComboPopup::IsAllUser(void)
{
	if(GetText() == _DEF_GAME_WHISPERLIST_ALL_USER && m_pTxtListAllUser->GetCurSelectItem() == 0)
		return true;

	return false;
}

LRESULT CWhisperlistComboPopup::OnAddWhisper(WPARAM &wParam, LPARAM &lParam)
{
	bool bSetCurSelect = (wParam & 0x00000001)?true:false;
	bool bStartAnimation = (wParam & 0x00000002)?true:false;
	bool bAllUser = (wParam & 0x00000004)?true:false;

	if(bAllUser) SetAllUser();
	else AddWhisper((LPCTSTR)lParam, bSetCurSelect, bStartAnimation);

	return TRUE;
}