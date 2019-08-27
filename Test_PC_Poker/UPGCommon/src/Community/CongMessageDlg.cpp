#include "stdafx.h"
#include "CongMessageDlg.h"
#include "congmessagedlgid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CCongMessageDlg::CCongMessageDlg(CWnd* pParent)
:	LSY::CLSYLibDialogBase(pParent),
	m_pBtnSend(NULL),
	m_pBtnCancel(NULL),
	m_pTextID(NULL),
	m_pEdit(NULL)
{
}

BEGIN_MESSAGE_MAP(CCongMessageDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

INT_PTR CCongMessageDlg::DoModal(CWnd *pParent, std::string id)
{
	m_pParentWnd = pParent;
	m_id = id;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_CONGMESSAGEDLG);
}

bool CCongMessageDlg::OnCreatedProject(void)
{
	m_pBtnSend = (LSY::CButton *)GetObject(ID_CONGMESSAGEDLG_BTN_SEND);
	m_pBtnSend->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CCongMessageDlg::OnClickButton));

	m_pBtnCancel = (LSY::CButton *)GetObject(ID_CONGMESSAGEDLG_BTN_CANCEL);
	m_pBtnCancel->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CCongMessageDlg::OnClickButton));

	m_pTextID = (LSY::CText *)GetObject(ID_CONGMESSAGEDLG_TEXT_ID);
	m_pEdit = (LSY::CEdit *)GetObject(ID_CONGMESSAGEDLG_TEXT_EDIT);

	CString strID;
	strID.Format("%s´Ô", m_id.c_str());
	m_pTextID->SetText(strID.GetString());
	m_pEdit->SetText("");

	return true;
}

LRESULT CCongMessageDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_CONGMESSAGEDLG_BTN_SEND:
		{
			CCL_SEND_CONG_MSG sendMsg;
			sendMsg.Set(m_id.c_str(), m_pEdit->GetText().c_str());
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), sendMsg.pData, sendMsg.GetTotalSize());

			OnOK();
		}
		break;
	case ID_CONGMESSAGEDLG_BTN_CANCEL:
		OnCancel();
		break;
	}
	return TRUE;
}