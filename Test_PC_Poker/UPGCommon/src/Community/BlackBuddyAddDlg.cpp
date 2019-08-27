#include "stdafx.h"
#include "BlackBuddyAddDlg.h"
#include "BlackBuddyAddDlgID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBlackBuddyAddDlg::CBlackBuddyAddDlg(CWnd* pParent/* = NULL*/)
:	LSY::CLSYLibDialogBase(pParent),
	m_pWndBack(NULL),
	m_pTxtResult(NULL),
	m_pEditBlackBuddyIDorNickName(NULL),
	m_pBtnAddBlackBuddy(NULL),
	m_pBtnCancelBlackBuddy(NULL),
	m_pBtnClose(NULL),
	m_pBtnOK(NULL),
	m_pBtnCancel(NULL)
{
}

BEGIN_MESSAGE_MAP(CBlackBuddyAddDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

INT_PTR CBlackBuddyAddDlg::DoModal(BLACKBUDDYADDDLGTYPE nType, std::string strMessage, CWnd *pParent)
{
	m_pParentWnd = pParent;
	m_nStartType = nType;
	m_nCurType = nType;
	m_strMessage = strMessage;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_BLACKLISTADD);
}

bool CBlackBuddyAddDlg::OnCreatedProject(void)
{
	LSY::CObjectProject *pProject = m_pObjectMan->GetProjectFromID(m_nID);
	m_pWndBack = (LSY::CWindows *)pProject->GetMainObject();

	m_pTxtResult = (LSY::CText *)GetObject(ID_BLACKLISTADD_TXT_USERLIST_ADD);
	m_pTxtResult->SetText(m_strMessage);

	m_pEditBlackBuddyIDorNickName = (LSY::CEdit *)GetObject(ID_BLACKLISTADD_EDIT_BLACKLIST_NICKNAME);
	m_pEditBlackBuddyIDorNickName->SetText("");

	m_pBtnAddBlackBuddy = (LSY::CButton *)GetObject(ID_BLACKLISTADD_BTN_ADD_BLACKLIST);
	m_pBtnAddBlackBuddy->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBlackBuddyAddDlg::OnClickButton));

	m_pBtnCancelBlackBuddy = (LSY::CButton *)GetObject(ID_BLACKLISTADD_BTN_CANCEL_BLACKLIST);
	m_pBtnCancelBlackBuddy->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBlackBuddyAddDlg::OnClickButton));

	m_pBtnOK = (LSY::CButton *)GetObject(ID_BLACKLISTADD_BTN_OK);
	m_pBtnOK->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBlackBuddyAddDlg::OnClickButton));

	m_pBtnCancel = (LSY::CButton *)GetObject(ID_BLACKLISTADD_BTN_CANCEL);
	m_pBtnCancel->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBlackBuddyAddDlg::OnClickButton));

	m_pBtnClose = (LSY::CButton *)GetObject(ID_BLACKLISTADD_BTN_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBlackBuddyAddDlg::OnClickButton));

	m_pObjectMan->SetFocus(m_pEditBlackBuddyIDorNickName);

	SetCurType(m_nStartType);

	return TRUE;
}

void CBlackBuddyAddDlg::SetCurType(BLACKBUDDYADDDLGTYPE nType)
{
	switch(nType)
	{
	case BBADT_EDIT:
		m_pWndBack->SetIndex(2);
		m_pTxtResult->SetShow(false);
		m_pEditBlackBuddyIDorNickName->SetShow(true);
		m_pBtnAddBlackBuddy->SetShow(true);
		m_pBtnCancelBlackBuddy->SetShow(true);
		m_pBtnOK->SetShow(false);
		m_pBtnCancel->SetShow(false);
		m_pBtnClose->SetShow(false);
		break;
	case BBADT_QUESTION:
		m_pWndBack->SetIndex(1);
		m_pTxtResult->SetShow(true);
		m_pTxtResult->SetText("블랙리스트에 추가 하시겠습니까?");
		m_pEditBlackBuddyIDorNickName->SetShow(false);
		m_pBtnAddBlackBuddy->SetShow(false);
		m_pBtnCancelBlackBuddy->SetShow(false);
		m_pBtnOK->SetShow(true);
		m_pBtnCancel->SetShow(true);
		m_pBtnClose->SetShow(false);
		break;
	case BBADT_MESSAGE:
		m_pWndBack->SetIndex(1);
		m_pTxtResult->SetShow(true);
		m_pEditBlackBuddyIDorNickName->SetShow(false);
		m_pBtnAddBlackBuddy->SetShow(false);
		m_pBtnCancelBlackBuddy->SetShow(false);
		m_pBtnOK->SetShow(false);
		m_pBtnCancel->SetShow(false);
		m_pBtnClose->SetShow(true);
		break;
	}

	m_nCurType = nType;
}

LRESULT CBlackBuddyAddDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_BLACKLISTADD_BTN_ADD_BLACKLIST:
		{
			CString strName = m_pEditBlackBuddyIDorNickName->GetText().c_str();
			strName.Trim();

			if(strName == "")
			{
				m_pTxtResult->SetText("블랙리스트에 추가할 닉네임/아이디를 입력해주세요.");
				SetCurType(BBADT_MESSAGE);
			}
			else if(CCommunityManager::Instance()->IsConnect())
			{
				if(CCommunityManager::Instance()->GetMyID() == strName.GetString() || CCommunityManager::Instance()->GetMyNickName() == strName.GetString())
				{
					m_pTxtResult->SetText("자기 자신을 블랙리스트에 추가할 수 없습니다.");
					SetCurType(BBADT_MESSAGE);
				}
				else
				{
					SetCurType(BBADT_QUESTION);
				}
			}
			else
			{
				m_pTxtResult->SetText("커뮤니티 서버에 연결되어 있지 않아 블랙리스트에 추가할 수 없습니다.");
				SetCurType(BBADT_MESSAGE);
			}
		}
		break;
	case ID_BLACKLISTADD_BTN_CANCEL_BLACKLIST:
		OnCancel();
		break;
	case ID_BLACKLISTADD_BTN_OK:
		if(m_nStartType == BBADT_EDIT)
		{
			CString strName = m_pEditBlackBuddyIDorNickName->GetText().c_str();
			strName.Trim();

			list<std::string> listNickName;
			listNickName.push_back(strName.GetString());
			CCommunityManager::Instance()->SendGetUserInfo(CCommunityUserInfo::ECUIT_ADDBLACKLIST, NULL, listNickName, "");
		}
		OnOK();
		break;
	case ID_BLACKLISTADD_BTN_CANCEL:
		if(m_nStartType == BBADT_EDIT)
		{
			SetCurType(BBADT_EDIT);
		}
		else
		{
			OnCancel();
		}
		break;
	case ID_BLACKLISTADD_BTN_CLOSE:
		if(m_nStartType == BBADT_MESSAGE)
		{
			OnCancel();
		}
		else
		{
			SetCurType(BBADT_EDIT);
		}
		break;
	}
	return TRUE;
}

bool CBlackBuddyAddDlg::OnAddGroup(const std::string &strName)
{
	return CCommunityManager::Instance()->SendBuddyGroupAdd(strName);
}

BOOL CBlackBuddyAddDlg::PreTranslateMessage(MSG* pMsg)
{
	if(m_pObjectMan)
	{
		switch(pMsg->message)
		{
		case WM_SYSKEYDOWN:
			switch(pMsg->wParam)
			{
			case VK_F4:
				if((GetKeyState(VK_LMENU) & 0xff00) && (m_nStartType == BBADT_EDIT) && (m_nCurType == BBADT_QUESTION))
				{
					SetCurType(BBADT_EDIT);
					return TRUE;
				}
				break;
			}
			break;
		case WM_KEYDOWN:
			switch(pMsg->wParam)
			{
			case VK_RETURN:
				switch(m_nCurType)
				{
				case BBADT_EDIT:
					{
						LSY::CMO_MouseLClick msg(m_pBtnAddBlackBuddy, m_pBtnAddBlackBuddy->GetPos());
						OnClickButton(&msg);
					}
					return TRUE;
				case BBADT_QUESTION:
					{
						LSY::CMO_MouseLClick msg(m_pBtnOK, m_pBtnOK->GetPos());
						OnClickButton(&msg);
					}
					return TRUE;
				}
				break;
			case VK_ESCAPE:
				if((m_nStartType == BBADT_EDIT) && (m_nCurType == BBADT_QUESTION))
				{
					SetCurType(BBADT_EDIT);
					return TRUE;
				}
				break;
			}
			break;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}