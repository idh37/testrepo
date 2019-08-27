#include "stdafx.h"
#include "MsgBoxChkDlg.h"
#include "MsgBoxChkDlgID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMsgBoxChkDlg::CMsgBoxChkDlg(CWnd* pParent /*=NULL*/)
: LSY::CLSYLibDialogBase(pParent), m_pImgBack(NULL), m_pTextMsg(NULL), m_pChkAgree(NULL), m_bChecked(false)
{
}

INT_PTR CMsgBoxChkDlg::DoModal(LPCTSTR strMsg, int nType)
{
	m_nType = static_cast<ENUM_MSGBOX_TYPE>(nType);
	m_strMsg = strMsg;
	
	int nReturn = __super::DoModal(GetObjectMan(), ID_MSGBOXCHK);
	
	if (nReturn == 1 && m_bChecked)
		nReturn = 3;
	
	return nReturn;
}

bool CMsgBoxChkDlg::OnCreatedProject(void)
{
	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_MSGBOXCHK_BTN_AGREE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMsgBoxChkDlg::OnClickButton));
	
	pButton = (LSY::CButton *)GetObject(ID_MSGBOXCHK_BTN_CANCEL);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMsgBoxChkDlg::OnClickButton));
	
	m_pChkAgree = (LSY::CCheckBox *)GetObject(ID_MSGBOXCHK_CHK_MSG);
	m_pChkAgree->AddHandler(LSY::EM_O_CHECKCHANGE, LSY::Fnt(this, &CMsgBoxChkDlg::OnCheckChange));
	m_pChkAgree->SetShow(m_nType == EMBT_NOCHECK ? false : true);
	
	m_pTextMsg = (LSY::CText *)GetObject(ID_MSGBOXCHK_TXT_MSG);
	m_pTextMsg->SetText(m_strMsg);
	
	return TRUE;
}

LRESULT CMsgBoxChkDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseLClick *pClick = (LSY::CMO_MouseLClick *)pMsg;
	
	switch(pClick->GetObject()->GetID())
	{
	case ID_MSGBOXCHK_BTN_AGREE:
		OnOK();
		break;
	case ID_MSGBOXCHK_BTN_CANCEL:
		OnCancel();
		break;
	}
	
	return TRUE;
}

LRESULT CMsgBoxChkDlg::OnCheckChange(LSY::CMessage *pMsg)
{
	LSY::CMO_CheckChange *pChange = (LSY::CMO_CheckChange *)pMsg;

	switch(pChange->GetObject()->GetID())
	{
	case ID_MSGBOXCHK_CHK_MSG:
		{
			m_bChecked = pChange->GetValue();
		}
		break;
	}

	return TRUE;
}

void CMsgBoxChkDlg::OnOK()
{
	__super::OnOK();
}

void CMsgBoxChkDlg::OnCancel()
{
	__super::OnCancel();
}

void CMsgBoxChkDlg::CloseDlg(void)
{
	OnCancel();
}
