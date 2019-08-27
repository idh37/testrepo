// ProfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DailyPopup.h"
#include "GlobalBase.h"
#include "daiilypopupid.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CDailyPopup::CDailyPopup(CWnd* pParent /*=NULL*/)
	: LSY::CLSYLibDialogBase(pParent)
{

}

BEGIN_MESSAGE_MAP(CDailyPopup, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

bool CDailyPopup::OnCreatedProject(void)
{

	LSY::CObjectProject *pProject = m_pObjectMan->GetProjectFromID(ID_DAILYPOPUP);
	m_pWndBack = (LSY::CWindows *)pProject->GetMainObject();

	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_DAILYPOPUP_CLOSEBTN);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CDailyPopup::OnClickButton));

	LSY::CCheckBox* pCheck = (LSY::CCheckBox *)GetObject(ID_DAILYPOPUP_CHECK_SHOWDAILY);
	pCheck->SetCheck(false) ;

	return true;
}

BOOL CDailyPopup::OnInitDialog() 
{
	CLSYLibDialogBase::OnInitDialog();
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CDailyPopup::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseLClick *msg = (LSY::CMO_MouseLClick *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_DAILYPOPUP_CLOSEBTN:
		{
			LSY::CCheckBox* pCheck = (LSY::CCheckBox *)GetObject(ID_DAILYPOPUP_CHECK_SHOWDAILY);
			if(true == pCheck->IsCheck())
			{
				int RecentPopupDay = COleDateTime::GetCurrentTime().GetDay();
				NMBASE::UTIL::WriteProfile_int(GM().GetMyInfo()->UI.ID, "ShowDailyPopup",RecentPopupDay);
			}

			OnCancel() ;
		}
		break;
	}
	return TRUE;
}

INT_PTR CDailyPopup::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_NETMARBLE)
	{
		if(false == GM().Get1995EventInfo().bIsEventTarget)
		{
			return IDOK;
		}

		int RecentPopupDay = COleDateTime::GetCurrentTime().GetDay();
		if(RecentPopupDay == NMBASE::UTIL::GetProfile_int(GM().GetMyInfo()->UI.ID, "ShowDailyPopup",-1))
		{
			return IDOK;
		}

		return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_DAILYPOPUP);
	}

	return IDOK;
}
