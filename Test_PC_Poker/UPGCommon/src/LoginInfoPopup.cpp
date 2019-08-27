// ProfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoginInfoPopup.h"
#include "GlobalBase.h"
#include "logininfopopupid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLoginInfoPopup::CLoginInfoPopup(CWnd* pParent /*=NULL*/)
	: LSY::CLSYLibDialogBase(pParent)
{

}

BEGIN_MESSAGE_MAP(CLoginInfoPopup, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

bool CLoginInfoPopup::OnCreatedProject(void)
{
	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_LOGININFOPOPUP_BTN_EXPLAIN);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CLoginInfoPopup::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_LOGININFOPOPUP_BTN_CLOSE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CLoginInfoPopup::OnClickButton));

	return true;
}

BOOL CLoginInfoPopup::OnInitDialog() 
{
	CLSYLibDialogBase::OnInitDialog();
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CLoginInfoPopup::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseLClick *msg = (LSY::CMO_MouseLClick *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_LOGININFOPOPUP_BTN_EXPLAIN:
		{
			OnCancel() ;
		}
		break;
	case ID_LOGININFOPOPUP_BTN_CLOSE:
		{
			OnCancel() ;
		}		
		break;
	}

	return TRUE;
}

INT_PTR CLoginInfoPopup::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;

	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_LOGININFOPOPUP);
}
