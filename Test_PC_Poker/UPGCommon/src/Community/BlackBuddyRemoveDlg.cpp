#include "stdafx.h"
#include "BlackBuddyRemoveDlg.h"
#include "BlackBuddyRemoveDlgID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBlackBuddyRemoveDlg::CBlackBuddyRemoveDlg(CWnd* pParent/* = NULL*/)
:	LSY::CLSYLibDialogBase(pParent),
	m_pTxtResult(NULL),
	m_pBtnClose(NULL),
	m_pBtnOK(NULL),
	m_pBtnCancel(NULL)
{
}

BEGIN_MESSAGE_MAP(CBlackBuddyRemoveDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

INT_PTR CBlackBuddyRemoveDlg::DoModal(BLACKBUDDYREMOVEDLGTYPE nType, std::string strMessage, CWnd *pParent)
{
	m_pParentWnd = pParent;
	m_nStartType = nType;
	m_nCurType = nType;
	m_strMessage = strMessage;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_BLACKLISTDELETE);
}

bool CBlackBuddyRemoveDlg::OnCreatedProject(void)
{
	m_pTxtResult = (LSY::CText *)GetObject(ID_BLACKLISTDELETE_TXT_RESULT);
	m_pTxtResult->SetText(m_strMessage);

	m_pBtnOK = (LSY::CButton *)GetObject(ID_BLACKLISTDELETE_BTN_OK);
	m_pBtnOK->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBlackBuddyRemoveDlg::OnClickButton));

	m_pBtnCancel = (LSY::CButton *)GetObject(ID_BLACKLISTDELETE_BTN_CANCEL);
	m_pBtnCancel->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBlackBuddyRemoveDlg::OnClickButton));

	m_pBtnClose = (LSY::CButton *)GetObject(ID_BLACKLISTDELETE_BTN_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBlackBuddyRemoveDlg::OnClickButton));

	SetCurType(m_nStartType);

	return TRUE;
}

void CBlackBuddyRemoveDlg::SetCurType(BLACKBUDDYREMOVEDLGTYPE nType)
{
	switch(nType)
	{
	case BBRDT_QUESTION:
		m_pTxtResult->SetShow(true);
		m_pTxtResult->SetText("선택한 사용자를 블랙리스트에서 삭제하시겠습니까?");
		
		m_pBtnOK->SetShow(true);
		m_pBtnCancel->SetShow(true);
		m_pBtnClose->SetShow(false);
		break;
	case BBRDT_MESSAGE:
		m_pTxtResult->SetShow(true);

		m_pBtnOK->SetShow(false);
		m_pBtnCancel->SetShow(false);
		m_pBtnClose->SetShow(true);
		break;
	}

	m_nCurType = nType;
}

LRESULT CBlackBuddyRemoveDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_BLACKLISTDELETE_BTN_OK:
		OnOK();
		break;
	case ID_BLACKLISTDELETE_BTN_CANCEL:
		OnCancel();
		break;
	case ID_BLACKLISTDELETE_BTN_CLOSE:
		OnCancel();
		break;
	}
	return TRUE;
}