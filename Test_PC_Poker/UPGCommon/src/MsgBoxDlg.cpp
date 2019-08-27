#include "stdafx.h"
#include "MsgBoxDlg.h"
#include "MsgBoxDlgID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMsgBoxDlg::CMsgBoxDlg(CWnd* pParent /*=NULL*/)
:	LSY::CLSYLibDialogBase(pParent),
	m_bClearMessage(false),
	m_bTimerMessage(false),
	m_pImgBack(NULL),
	m_pTextMessage(NULL)
{
}

BEGIN_MESSAGE_MAP(CMsgBoxDlg, LSY::CLSYLibDialogBase)
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CMsgBoxDlg::SetTimerCancel(int nTime)
{
	SetTimer(TIMER_MSGBOX_CANCEL, nTime, NULL);

	m_bTimerMessage = false;
	m_nTimerCancel = 0;
}

INT_PTR CMsgBoxDlg::DoModal(const ENUM_MSGBOX_TYPE &nType, LPCTSTR strMsg, int nTimerCancel)
{
	m_nType = nType;
	m_listMessage.push_back(strMsg);
	m_bTimerMessage = false;

	if (GetSafeHwnd())
	{
		if (m_bClearMessage)
		{
			LSY::CText *pText = (LSY::CText *)GetObject(ID_MSGBOX_TXT_MSG);
			pText->SetText(strMsg);
			m_listMessage.clear();
			m_bClearMessage = false;
		}

		return IDCANCEL;
	}

	m_nTimerCancel = nTimerCancel;
	int nReturn = __super::DoModal(GetObjectMan(), ID_MSGBOX);
	return nReturn;
}

bool CMsgBoxDlg::OnCreatedProject(void)
{
	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_MSGBOX_BTN_YES);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMsgBoxDlg::OnClickButton));
	pButton->SetShow(m_nType == EMBT_QUESTION?true:false);

	pButton = (LSY::CButton *)GetObject(ID_MSGBOX_BTN_NO);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMsgBoxDlg::OnClickButton));
	pButton->SetShow(m_nType == EMBT_QUESTION?true:false);

	pButton = (LSY::CButton *)GetObject(ID_MSGBOX_BTN_OK);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMsgBoxDlg::OnClickButton));
	pButton->SetShow(m_nType == EMBT_NOTICE?true:false);

	pButton = (LSY::CButton *)GetObject(ID_MSGBOX_BTN_ENTER_ROOM);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMsgBoxDlg::OnClickButton));
	pButton->SetShow(m_nType == EMBT_ENTERROOM?true:false);

	pButton = (LSY::CButton *)GetObject(ID_MSGBOX_BTN_CANCEL);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMsgBoxDlg::OnClickButton));
	pButton->SetShow((m_nType==EMBT_CREATEROOM || m_nType == EMBT_ENTERROOM || m_nType == EMBT_OKCANCEL || m_nType == EMBT_EXIT)?true:false);

	pButton = (LSY::CButton *)GetObject(ID_MSGBOX_BTN_CREATE_ROOM);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMsgBoxDlg::OnClickButton));
	pButton->SetShow(m_nType == EMBT_CREATEROOM?true:false);

	pButton = (LSY::CButton *)GetObject(ID_MSGBOX_BTN_CONFORM);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMsgBoxDlg::OnClickButton));
	pButton->SetShow(m_nType == EMBT_CONFORMREFUSE?true:false);

	pButton = (LSY::CButton *)GetObject(ID_MSGBOX_BTN_REFUSE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMsgBoxDlg::OnClickButton));
	pButton->SetShow(m_nType == EMBT_CONFORMREFUSE?true:false);

	pButton = (LSY::CButton *)GetObject(ID_MSGBOX_BTN_OK_CANCEL);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMsgBoxDlg::OnClickButton));
	pButton->SetShow(m_nType == EMBT_OKCANCEL?true:false);

	pButton = (LSY::CButton *)GetObject(ID_MSGBOX_BTN_EXIT);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMsgBoxDlg::OnClickButton));
	pButton->SetShow(m_nType == EMBT_EXIT?true:false);

	// @ 훓라 기권방 기권하기 메시지 박스 버튼
	pButton = (LSY::CButton *)GetObject(ID_MSGBOX_BTN_GIVEUP);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMsgBoxDlg::OnClickButton));
	pButton->SetShow(m_nType == EMBT_GIVEUP ? true : false);

	pButton = (LSY::CButton *)GetObject(ID_MSGBOX_BTN_CANCEL2);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMsgBoxDlg::OnClickButton));
	pButton->SetShow(m_nType == EMBT_GIVEUP ? true : false);
	// @
	
	m_pTextMessage = (LSY::CText *)GetObject(ID_MSGBOX_TXT_MSG);
	m_pTextMessage->SetText(m_listMessage.front());
	m_pTextMessage->AddHandler(LSY::EM_O_SETTXTLINECOUNT, LSY::Fnt(this, &CMsgBoxDlg::OnSetTextLineCount));
	m_listMessage.pop_front();
	
	m_pImgBack = (LSY::CImage *)GetObject(ID_MSGBOX_IMG_BACK_TOP);

	if (0 != m_nTimerCancel)
	{
		SetTimerCancel(m_nTimerCancel);
	}

	return TRUE;
}



LRESULT CMsgBoxDlg::OnSetTextLineCount(LSY::CMessage *pMsg)
{
	LSY::CMO_SetTextLineCount *msg = (LSY::CMO_SetTextLineCount *)pMsg;
	int nLine = msg->GetValue();
	if (nLine < 2) nLine = 2;

	int nOldHeight = (int)m_pTextMessage->GetHeight();
	int nNewHeight = nLine * 13;

	if (nOldHeight != nNewHeight)
	{
		int nWidth = (int)m_pTextMessage->GetWidth();
		m_pTextMessage->Create(nWidth, nNewHeight);
		m_pTextMessage->SetRedraw(false);
		
		m_pImgBack->SetMiddleCount(nLine);
		m_pObjectMan->ReSize(m_pImgBack->GetLocalRect().GetSize());
		
		CRect rtRect;
		GetWindowRect(&rtRect);
		
		int nPosY = rtRect.top - (nNewHeight - nOldHeight)/2;
		if (nPosY < 0) nPosY = 0;
		
		SetWindowPos(NULL, rtRect.left, nPosY, 0, 0, SWP_NOZORDER|SWP_NOSIZE|SWP_NOREDRAW);
		InvalidateRect(NULL, FALSE);
		
		return TRUE;
	}
	
	return FALSE;
}

LRESULT CMsgBoxDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseLClick *msg = (LSY::CMO_MouseLClick *)pMsg;

	if(m_listMessage.size() != 0)
	{
		LSY::CText *pText = (LSY::CText *)GetObject(ID_MSGBOX_TXT_MSG);
		pText->SetText(m_listMessage.front());
		m_listMessage.pop_front();		
		return TRUE;
	}

	switch(msg->GetObject()->GetID())
	{
	case ID_MSGBOX_BTN_YES:
	case ID_MSGBOX_BTN_OK:
	case ID_MSGBOX_BTN_ENTER_ROOM:
	case ID_MSGBOX_BTN_CREATE_ROOM:
	case ID_MSGBOX_BTN_CONFORM:
	case ID_MSGBOX_BTN_OK_CANCEL:
	case ID_MSGBOX_BTN_EXIT:
	case ID_MSGBOX_BTN_GIVEUP:
		OnOK();
		break;
	case ID_MSGBOX_BTN_NO:
	case ID_MSGBOX_BTN_CANCEL:
	case ID_MSGBOX_BTN_REFUSE:
	case ID_MSGBOX_BTN_CANCEL2:
		OnCancel();
		break;
	}
	
	return TRUE;
}

void CMsgBoxDlg::OnOK()
{
	__super::OnOK();
}

void CMsgBoxDlg::OnCancel()
{
	__super::OnCancel();
}

void CMsgBoxDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case TIMER_MSGBOX_CANCEL:
		{
			KillTimer(TIMER_MSGBOX_CANCEL);

			m_bTimerMessage = true;

			OnCancel();
		}
		
		return;
	}
	CLSYLibDialogBase::OnTimer(nIDEvent);
}

void CMsgBoxDlg::OnDestroy()
{
	CLSYLibDialogBase::OnDestroy();

	// TODO: Add your message handler code here
	KillTimer(TIMER_MSGBOX_CANCEL);
}

void CMsgBoxDlg::ClearMessage(void)
{
	m_listMessage.clear();
	m_bClearMessage = true;
}

void CMsgBoxDlg::CloseDlg(void)
{
	OnCancel();
}
