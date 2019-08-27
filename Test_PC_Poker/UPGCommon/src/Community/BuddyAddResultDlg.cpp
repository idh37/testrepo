#include "stdafx.h"
#include "BuddyAddResultDlg.h"
#include "BuddyAddResultDlgID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBuddyAddResultDlg::CBuddyAddResultDlg(CWnd* pParent/* = NULL*/)
:	LSY::CLSYLibDialogBase(pParent),
	m_pTxtMessage(NULL),
	m_pBtnClose(NULL)
{
}

BEGIN_MESSAGE_MAP(CBuddyAddResultDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

INT_PTR CBuddyAddResultDlg::DoModal(std::string strMessage, CWnd *pParent)
{
	m_strMessage = strMessage;
	m_pParentWnd = pParent;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_FRIENDADDRESULTDLG);
}

bool CBuddyAddResultDlg::OnCreatedProject(void)
{
	m_pTxtMessage = (LSY::CText *)GetObject(ID_FRIENDADDRESULTDLG_TXT_MESSAGE);
	m_pTxtMessage->SetText(m_strMessage);

	m_pBtnClose = (LSY::CButton *)GetObject(ID_FRIENDADDRESULTDLG_BTN_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBuddyAddResultDlg::OnClickButton));

	return TRUE;
}

LRESULT CBuddyAddResultDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_FRIENDADDRESULTDLG_BTN_CLOSE:
		OnCancel();
		break;
	}
	return TRUE;
}