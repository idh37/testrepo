// ProfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ResetQuickStarterDlg.h"
#include "GlobalBase.h"
#include "resetquickstarterdlgid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CResetQuickStarterDlg::CResetQuickStarterDlg(CWnd* pParent /*=NULL*/)
	: LSY::CLSYLibDialogBase(pParent)
{

}

BEGIN_MESSAGE_MAP(CResetQuickStarterDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

bool CResetQuickStarterDlg::OnCreatedProject(void)
{
	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_RESETQUICKSTARTERDLG_BTN_CLOSE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CResetQuickStarterDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_RESETQUICKSTARTERDLG_BTN_RESET);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CResetQuickStarterDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_RESETQUICKSTARTERDLG_BTN_CREATEROOM);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CResetQuickStarterDlg::OnClickButton));

	return true;
}

BOOL CResetQuickStarterDlg::OnInitDialog() 
{
	CLSYLibDialogBase::OnInitDialog();
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CResetQuickStarterDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseLClick *msg = (LSY::CMO_MouseLClick *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_RESETQUICKSTARTERDLG_BTN_CLOSE:
		{
			OnCancel() ;
		}
		break;
	case ID_RESETQUICKSTARTERDLG_BTN_RESET:
		{
			OnCancel() ;

			ShowSelectStartDlg();			
		}	
		break;
	case ID_RESETQUICKSTARTERDLG_BTN_CREATEROOM:
		{
			OnCancel() ;

			if(GM().GetMyInfo()->UI.GetRealPMoney() < 100000000)  // 토너먼트 추가로 변경된 함수
			{
				ShowNewAskChargeDlg(NEW_ASK_CHARGE_DLG_TYPE_FREE_CHARGE_INSU, "");
				break;
			}		

			// ### [ 관전기능 ] ###	
			GM().ShowCreateRooomDlg();			
		}	
		break;
	}

	return TRUE;
}

INT_PTR CResetQuickStarterDlg::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;

	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_RESETQUICKSTARTERDLG);
}
