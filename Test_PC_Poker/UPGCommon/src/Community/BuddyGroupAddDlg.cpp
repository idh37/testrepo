#include "stdafx.h"
#include "BuddyGroupAddDlg.h"
#include "BuddyGroupAddDlgID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBuddyGroupAddDlg::CBuddyGroupAddDlg(CWnd* pParent/* = NULL*/)
:	LSY::CLSYLibDialogBase(pParent),
	m_pEditName(NULL)
{
}

BEGIN_MESSAGE_MAP(CBuddyGroupAddDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

INT_PTR CBuddyGroupAddDlg::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_FRIENDGROUPADD);
}

bool CBuddyGroupAddDlg::OnCreatedProject(void)
{
	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_FRIENDGROUPADD_BTN_ADD_GROUP);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBuddyGroupAddDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_FRIENDGROUPADD_BTN_CANCEL);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBuddyGroupAddDlg::OnClickButton));

	m_pEditName = (LSY::CEdit *)GetObject(ID_FRIENDGROUPADD_EDIT_ADD_GROUP);
	m_pEditName->SetText("");
	m_pEditName->AddHandler(LSY::EM_O_RETURN, LSY::Fnt(this, &CBuddyGroupAddDlg::OnReturn));

	m_pObjectMan->SetFocus(m_pEditName);

	return TRUE;
}

LRESULT CBuddyGroupAddDlg::OnReturn(LSY::CMessage *pMsg)
{
	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_FRIENDGROUPADD_BTN_ADD_GROUP);
	LSY::CMO_MouseLClick msg(pButton, pButton->GetPos());
	OnClickButton(&msg);

	return TRUE;
}

LRESULT CBuddyGroupAddDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_FRIENDGROUPADD_BTN_ADD_GROUP:
		{
			CString strName = m_pEditName->GetText().c_str();
			strName = strName.Trim();

			CMsgBoxDlg dlg(this);
			if(strName == "")
			{
				dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "그룹명을 입력해 주십시요.");
			}
			else if(OnAddGroup(strName.GetString()))
			{
				OnOK();
			}
			else
			{
				dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "중복되는 그룹명이 존재합니다. 다른 그룹명을 입력해 주세요.");
			}
		}
		break;
	case ID_FRIENDGROUPADD_BTN_CANCEL:
		OnCancel();
		break;
	}
	return TRUE;
}

bool CBuddyGroupAddDlg::OnAddGroup(const std::string &strName)
{
	return CCommunityManager::Instance()->SendBuddyGroupAdd(strName);
}