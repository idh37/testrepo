
#include "StdAfx.h"
#include "ChuSeokSubPopup.h"
#include "ChuSeokSubPopupid.h"

CChuSeokSubPopup::CChuSeokSubPopup(CWnd* pParent/* = NULL*/) :
	LSY::CLSYLibDialogBase(pParent)
{
	m_sTxtChip = "";
}

CChuSeokSubPopup::~CChuSeokSubPopup()
{

}

BEGIN_MESSAGE_MAP(CChuSeokSubPopup, LSY::CLSYLibDialogBase)
	ON_WM_TIMER()
END_MESSAGE_MAP()

bool CChuSeokSubPopup::OnCreatedProject()
{
	InitLSYControl();

	return true;
}

INT_PTR CChuSeokSubPopup::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;

	return __super::DoModal(GetObjectMan(), ID_CHUSEOKSUBPOPUP, true, pParent);
}

BOOL CChuSeokSubPopup::PreTranslateMessage(MSG* pMsg)
{
	/*
	if(pMsg->message == WM_SYSKEYDOWN)
	{
		if(pMsg->wParam == VK_F4)
		{
			return TRUE;
		}
	}
	}*/

	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return CWnd::PreTranslateMessage( pMsg );
}

LRESULT CChuSeokSubPopup::OnClickButton(LSY::CMessage*pMsg)
{
	LSY::CObjectMessage* msg = (LSY::CObjectMessage*)pMsg;

	switch(msg->GetObject()->GetID())
	{
	case ID_CHUSEOKSUBPOPUP_BUTTON:
		OnOK();
		break;
	}

	return TRUE;
}

void CChuSeokSubPopup::HandleResponse(int signal, int totSize, char *lpData)
{
	switch(signal)
	{
		/*
	case OPCODE_SV_2018THANKSGIVING_ATTENDANCE_INFO:
		SetData(totSize, lpData);			
		break;
		*/
	}
}

void CChuSeokSubPopup::SetTxtChip(std::string sChip)
{
	m_sTxtChip = sChip;
}

void CChuSeokSubPopup::InitLSYControl()
{
	m_pTxtChip = (LSY::CText*)GetObject(ID_CHUSEOKSUBPOPUP_TXT_CHIP);
	m_pTxtChip->SetText(m_sTxtChip);

	m_pBtnOk = (LSY::CButton*)GetObject(ID_CHUSEOKSUBPOPUP_BUTTON);
	m_pBtnOk->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CChuSeokSubPopup::OnClickButton));
}

void CChuSeokSubPopup::SetData()
{

}

void CChuSeokSubPopup::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	/*
	switch(nIDEvent)
	{
	case TIMER_ID:
		{
			KillTimer(TIMER_ID);
		}
		return;
	}
	}*/
	
	CLSYLibDialogBase::OnTimer(nIDEvent);
}
