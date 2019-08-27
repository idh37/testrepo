#include "StdAfx.h"
#include "LobbyNotifyDlg.h"
#include "LobbyNotifydlgid.h"

CLobbyNotifyDlg::CLobbyNotifyDlg(CWnd* pParent/* = NULL*/) : 
	LSY::CLSYLibDialogBase(pParent)
{

}

CLobbyNotifyDlg::~CLobbyNotifyDlg()
{
}

bool CLobbyNotifyDlg::OnCreatedProject()
{
	InitLSYControl();

	return true;
}

INT_PTR CLobbyNotifyDlg::DoModal()
{
	int RecentPopupDay = COleDateTime::GetCurrentTime().GetDay();

	if(RecentPopupDay == NMBASE::UTIL::GetProfile_int(GM().GetMyInfo()->UI.ID, "DoNotShowLobbyNotifyDlgThisDay", -1))
	{
		return IDOK;
	}

	return __super::DoModal(GetObjectMan(), ID_LOBBYNOTIFY, true, AfxGetMainWnd());
}

BOOL CLobbyNotifyDlg::PreTranslateMessage(MSG* pMsg)
{
	/*
	if(pMsg->message == WM_SYSKEYDOWN)
	{
		if(pMsg->wParam == VK_F4)
		{
			return TRUE;
		}
	}

	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}
	*/

	return CWnd::PreTranslateMessage(pMsg);
}

void CLobbyNotifyDlg::OnOK()
{
	if (true == m_pChkToday->IsCheck())
	{
		int RecentPopupDay = COleDateTime::GetCurrentTime().GetDay();
		NMBASE::UTIL::WriteProfile_int(GM().GetMyInfo()->UI.ID, "DoNotShowLobbyNotifyDlgThisDay", RecentPopupDay);
	}

	__super::OnOK();
}

void CLobbyNotifyDlg::OnCancel()
{

	__super::OnCancel();
}

LRESULT CLobbyNotifyDlg::OnClickButton(LSY::CMessage*pMsg)
{
	LSY::CObjectMessage* msg = (LSY::CObjectMessage*)pMsg;

	switch (msg->GetObject()->GetID())
	{
	case ID_LOBBYNOTIFY_BTN_OK:
		OnOK();
		break;
	}

	return TRUE;
}

void CLobbyNotifyDlg::InitLSYControl()
{
	m_pBtnOK = (LSY::CButton*)GetObject(ID_LOBBYNOTIFY_BTN_OK);
	m_pBtnOK->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CLobbyNotifyDlg::OnClickButton));

	m_pChkToday = (LSY::CCheckBox*)GetObject(ID_LOBBYNOTIFY_CHKBOX_TODAY);
}

void CLobbyNotifyDlg::SetData()
{

}
