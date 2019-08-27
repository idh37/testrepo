#include "stdafx.h"
#include "BuddyGroupRemoveDlg.h"
#include "buddygroupremovedlgid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBuddyGroupRemoveDlg::CBuddyGroupRemoveDlg(CWnd* pParent/* = NULL*/)
:	LSY::CLSYLibDialogBase(pParent),
	m_pTxtResult(NULL),
	m_pBtnClose(NULL),
	m_pBtnOK(NULL),
	m_pBtnCancel(NULL)
{
}

BEGIN_MESSAGE_MAP(CBuddyGroupRemoveDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

INT_PTR CBuddyGroupRemoveDlg::DoModal(BUDDYGROUPREMOVEDLGTYPE nType, std::string strMessage, CWnd *pParent)
{
	m_pParentWnd = pParent;
	m_nStartType = nType;
	m_nCurType = nType;
	m_strMessage = strMessage;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_FRIENDGROUPREMOVE);
}

bool CBuddyGroupRemoveDlg::OnCreatedProject(void)
{
	m_pTxtResult = (LSY::CText *)GetObject(ID_FRIENDGROUPREMOVE_TXT_RESULT);
	m_pTxtResult->SetText(m_strMessage);

	m_pBtnOK = (LSY::CButton *)GetObject(ID_FRIENDGROUPREMOVE_BTN_OK);
	m_pBtnOK->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBuddyGroupRemoveDlg::OnClickButton));

	m_pBtnCancel = (LSY::CButton *)GetObject(ID_FRIENDGROUPREMOVE_BTN_CANCEL);
	m_pBtnCancel->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBuddyGroupRemoveDlg::OnClickButton));

	m_pBtnClose = (LSY::CButton *)GetObject(ID_FRIENDGROUPREMOVE_BTN_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBuddyGroupRemoveDlg::OnClickButton));

	SetCurType(m_nStartType);

	return TRUE;
}

void CBuddyGroupRemoveDlg::SetCurType(BUDDYGROUPREMOVEDLGTYPE nType)
{
	switch(nType)
	{
	case BGRDT_QUESTION:
		m_pTxtResult->SetShow(true);
		m_pTxtResult->SetText("해당 그룹을 삭제하시겠습니까?");
		
		m_pBtnOK->SetShow(true);
		m_pBtnCancel->SetShow(true);
		m_pBtnClose->SetShow(false);
		break;
	case BGRDT_MESSAGE:
		m_pTxtResult->SetShow(true);

		m_pBtnOK->SetShow(false);
		m_pBtnCancel->SetShow(false);
		m_pBtnClose->SetShow(true);
		break;
	}

	m_nCurType = nType;
}

LRESULT CBuddyGroupRemoveDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_FRIENDGROUPREMOVE_BTN_OK:
		OnOK();
		break;
	case ID_FRIENDGROUPREMOVE_BTN_CANCEL:
		OnCancel();
		break;
	case ID_FRIENDGROUPREMOVE_BTN_CLOSE:
		OnCancel();
		break;
	}
	return TRUE;
}