
#include "stdafx.h"
#include "MsgBoxModelessDlg.h"
#include "msgboxmodelessdlgid.h"


CMsgBoxModelessDlg::CMsgBoxModelessDlg(CWnd* pParent/* = NULL*/)
: LSY::CLSYLibDialogBase(pParent),
m_forceOrder(false), m_pTextMsg(NULL), m_pImgBackground(NULL)
{
	
}

CMsgBoxModelessDlg::~CMsgBoxModelessDlg()
{
	
}

bool CMsgBoxModelessDlg::OnCreatedProject()
{
	LSY::CButton* pButton = (LSY::CButton *)GetObject(ID_MSGMODELESSBOX_ID_MODELESS_MSGBOX_BTN_OK);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMsgBoxModelessDlg::OnClickButton));		

	m_pTextMsg = (LSY::CText *)GetObject(ID_MSGMODELESSBOX_MODELESS_MSGBOX_TEXT);
	m_pTextMsg->AddHandler(LSY::EM_O_SETTXTLINECOUNT, LSY::Fnt(this, &CMsgBoxModelessDlg::OnRedrawTextRange));
	m_pTextMsg->SetText(m_msg);

	m_pImgBackground = (LSY::CImage *)GetObject(ID_MSGMODELESSBOX_MODELESS_MSGBOX_IMG_TOP);

	if(m_forceOrder)
	{		
		_ForceWinEnableOrdering(true);
	}

	return true;
}

BOOL CMsgBoxModelessDlg::ShowMsg(HWND parentWnd, LPCTSTR msg)
{
	if(parentWnd)
	{
		m_forceOrder = true;
	}

	m_msg = msg;	
	
	return __super::Create(GetObjectMan(), ID_MSGMODELESSBOX, FromHandle(parentWnd));
}

BOOL CMsgBoxModelessDlg::ShowWindow(bool bShow, LPCTSTR msg)
{
	m_msg = msg;	
	m_pTextMsg->SetText(m_msg);

	return __super::ShowWindow(bShow);
}

LRESULT CMsgBoxModelessDlg::OnClickButton(LSY::CMessage* pMsg)
{
	LSY::CMO_MouseLClick* msg = (LSY::CMO_MouseLClick*)pMsg;	

	switch(msg->GetObject()->GetID())
	{
	case ID_MSGMODELESSBOX_ID_MODELESS_MSGBOX_BTN_OK:		
		OnOK();		
		break;
	}

	_ForceWinEnableOrdering(false);

	return TRUE;
}

LRESULT CMsgBoxModelessDlg::OnRedrawTextRange(LSY::CMessage* pMsg)
{
	LSY::CMO_SetTextLineCount* msg = (LSY::CMO_SetTextLineCount*)pMsg;
	int numline = msg->GetValue();

	if(numline < 2) 
	{
		numline = 2;
	}

	int oldHeight = (int)m_pTextMsg->GetHeight();
	int newHeight = numline * HEIGHT_PER_LINE;

	if(oldHeight != newHeight)
	{
		int nWidth = (int)m_pTextMsg->GetWidth();

		m_pTextMsg->Create(nWidth, newHeight);
		m_pTextMsg->SetRedraw(false);

		m_pImgBackground->SetMiddleCount(numline);
		m_pObjectMan->ReSize(m_pImgBackground->GetLocalRect().GetSize());

		CRect rtRect;
		GetWindowRect(&rtRect);

		int nPosY = rtRect.top - (newHeight - oldHeight)/2;
		if (nPosY < 0) nPosY = 0;

		SetWindowPos(NULL, rtRect.left, nPosY, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW);
		InvalidateRect(NULL, FALSE);

		return TRUE;
	}

	return FALSE;
}

void CMsgBoxModelessDlg::_ForceWinEnableOrdering(bool beforePopup)
{
	CWnd* parentWnd = GetParent();

	while(parentWnd)
	{
		parentWnd->EnableWindow(FALSE);
		parentWnd = parentWnd->GetParent();
	}

	if(!beforePopup)
	{
		parentWnd = GetParent();
		parentWnd->EnableWindow(TRUE);
	}
	
	EnableWindow( TRUE );
}