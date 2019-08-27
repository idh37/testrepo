#include "stdafx.h"
#include "BuddyAddDlg.h"
#include "BuddyAddResultDlg.h"
#include "BuddyAddDlgID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBuddyAddDlg::CBuddyAddDlg(CWnd* pParent/* = NULL*/)
:	LSY::CLSYLibDialogBase(pParent),
	m_pEditNickName(NULL),
	m_pComboGroup(NULL),
	m_pBtnOK(NULL),
	m_pBtnClose(NULL)
{
}

BEGIN_MESSAGE_MAP(CBuddyAddDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

INT_PTR CBuddyAddDlg::DoModal(std::string strNickName, CWnd *pParent)
{
	m_strNickName = strNickName;
	m_pParentWnd = pParent;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_FRIENDADDDLG);
}

bool CBuddyAddDlg::OnCreatedProject(void)
{
	m_pEditNickName = (LSY::CEdit *)GetObject(ID_FRIENDADDDLG_EDIT_NICKNAME);
	m_pEditNickName->SetText("");
	m_pEditNickName->AddHandler(LSY::EM_O_RETURN, LSY::Fnt(this, &CBuddyAddDlg::OnReturn));

	m_pComboGroup = (LSY::CCombo *)GetObject(ID_FRIENDADDDLG_COMBO_GROUP);

	m_pBtnOK = (LSY::CButton *)GetObject(ID_FRIENDADDDLG_BTN_OK);
	m_pBtnOK->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBuddyAddDlg::OnClickButton));

	m_pBtnClose = (LSY::CButton *)GetObject(ID_FRIENDADDDLG_BTN_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBuddyAddDlg::OnClickButton));

	m_pEditNickName->SetText(m_strNickName);
	CCommunityManager::Instance()->SetComboGroupList(m_pComboGroup, _DEF_DEFAULT_BUDDY_GROUP);

	m_pObjectMan->SetFocus(m_pEditNickName);

	return TRUE;
}

LRESULT CBuddyAddDlg::OnReturn(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseLClick msg(m_pBtnOK, m_pBtnOK->GetPos());
	OnClickButton(&msg);

	return TRUE;
}

LRESULT CBuddyAddDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_FRIENDADDDLG_BTN_OK:
		{
			CString strName = g_AnsiToUTF8(m_pEditNickName->GetText().c_str());
			strName.Trim();

			CBuddyAddResultDlg dlg;
			if(strName == "")
			{
				dlg.DoModal("내친구 목록에 추가할 아이디를 입력해주세요", this);
				break;
			}

			if(CCommunityManager::Instance()->IsConnect())
			{
				if(CCommunityManager::Instance()->GetMyID() == strName.GetString() || CCommunityManager::Instance()->GetMyNickName() == strName.GetString())
				{
					dlg.DoModal("자기 자신을 내친구 목록에 추가할 수 없습니다.", this);
					break;
				}
				else
				{
					std::list<std::string> listIDorNickName;
					listIDorNickName.push_back(strName.GetString());
					CCommunityManager::Instance()->SendGetUserInfo(CCommunityUserInfo::ECUIT_ADDBUDDY, NULL, listIDorNickName, m_pComboGroup->GetText());
				}
			}
			else
			{
				dlg.DoModal("커뮤니티 서버에 연결되어 있지 않아 선택한 친구를 내친구 목록에 추가할 수 없습니다.", this);
			}
			OnOK();
		}
		break;
	case ID_FRIENDADDDLG_BTN_CLOSE:
		OnCancel();
		break;
	}
	return TRUE;
}