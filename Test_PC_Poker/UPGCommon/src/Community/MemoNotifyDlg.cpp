#include "stdafx.h"
#include "MemoNotifyDlg.h"
#include "MemoNotifyDlgID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMemoNotifyDlg::CMemoNotifyDlg(CWnd* pParent/* = NULL*/)
:	LSY::CLSYLibDialogBase(pParent),
	m_pBtnDelete(NULL),
	m_pBtnCancel(NULL),
	m_pBtnOK(NULL),
	m_pBack(NULL),
	m_nType(EMNT_MEMO_DELETE)
{
}

BEGIN_MESSAGE_MAP(CMemoNotifyDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

INT_PTR CMemoNotifyDlg::DoModal(CWnd *pParent, const ENUM_MEMONOTIFY_TYPE &nType)
{
	m_nType = nType;
	m_pParentWnd = pParent;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_MEMONOTIFY);
}

bool CMemoNotifyDlg::OnCreatedProject(void)
{
	m_pBtnDelete = (LSY::CButton *)GetObject(ID_MEMONOTIFY_BTN_DELETE);
	m_pBtnDelete->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMemoNotifyDlg::OnClickButton));

	m_pBtnCancel = (LSY::CButton *)GetObject(ID_MEMONOTIFY_BTN_CANCEL);
	m_pBtnCancel->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMemoNotifyDlg::OnClickButton));

	m_pBtnOK = (LSY::CButton *)GetObject(ID_MEMONOTIFY_BTN_OK);
	m_pBtnOK->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMemoNotifyDlg::OnClickButton));

	LSY::CObjectProject *pProject = m_pObjectMan->GetProjectFromLayer(0);
	m_pBack  = (LSY::CWindows *)pProject->GetMainObject();

	m_pBack->SetIndex((int)m_nType);

	if(m_nType == EMNT_MEMO_DELETE)
	{
		m_pBtnDelete->SetShow(true);
		m_pBtnCancel->SetShow(true);
		m_pBtnOK->SetShow(false);
	}
	else
	{
		m_pBtnDelete->SetShow(false);
		m_pBtnCancel->SetShow(false);
		m_pBtnOK->SetShow(true);
	}

	return true;
}

LRESULT CMemoNotifyDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_MEMONOTIFY_BTN_DELETE:
		OnOK();
		break;
	case ID_MEMONOTIFY_BTN_CANCEL:
		OnCancel();
		break;
	case ID_MEMONOTIFY_BTN_OK:
		OnCancel();
		break;
	}
	return TRUE;
}