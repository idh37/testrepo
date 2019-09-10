
#include "StdAfx.h"
#include "ChuSeokPopup.h"
#include "chuseokpopupid.h"

CChuSeokPopup::CChuSeokPopup(CWnd* pParent/* = NULL*/) :
	LSY::CLSYLibDialogBase(pParent)
{
	// �������� �޴� ��
	m_RewardDay = 0;		// ���� ����
	m_RewardMoney = 0;	// �����
	
#ifdef _DEBUG
	m_bTest = false;
#else
	m_bTest = false;
#endif
}

CChuSeokPopup::~CChuSeokPopup()
{

}

BEGIN_MESSAGE_MAP(CChuSeokPopup, LSY::CLSYLibDialogBase)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

bool CChuSeokPopup::OnCreatedProject()
{
	InitLSYControl();

	return true;
}

INT_PTR CChuSeokPopup::DoModal()
{
	return __super::DoModal(GetObjectMan(), ID_CHUSEOKPOPUP, true, AfxGetMainWnd());
}

BOOL CChuSeokPopup::PreTranslateMessage(MSG* pMsg)
{
	// Alt + F4
	if (pMsg->message == WM_SYSKEYDOWN)
	{
		if (pMsg->wParam == VK_F4)
		{
			return TRUE;
		}
	}

	// Esc
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return CWnd::PreTranslateMessage( pMsg );
}

void CChuSeokPopup::OnDestroy()
{
	CLSYLibDialogBase::OnDestroy();

	// TODO: Add your message handler code here
	GM().SetPlayDailyGoldEffect(false);
}

LRESULT CChuSeokPopup::OnClickButton(LSY::CMessage*pMsg)
{
	LSY::CObjectMessage* msg = (LSY::CObjectMessage*)pMsg;

	switch(msg->GetObject()->GetID())
	{
	case ID_CHUSEOKPOPUP_BTN_OK:
		{
			if (m_bTest)
			{
				bool bModaled = m_popChuSeokSub.GetSafeHwnd();

				if (bModaled == false)
				{
					m_popChuSeokSub.SetTxtChip(m_pTxtChip->GetText());
					m_popChuSeokSub.DoModal(this);
				}

				OnOK();

				break;
			}

			GM().SetPlayDailyGoldEffect(true);

			CCL_PM_2019HAVEST_REQUEST_REWARD msg;
			msg.Set();
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
	}

	return TRUE;
}

void CChuSeokPopup::HandleResponse(int signal, int totSize, char *lpData)
{
	switch(signal)
	{
	case SV_PM_2019HAVEST_INFO:
		{
			CSV_PM_2019HAVEST_INFO msg;
			msg.Get(lpData, totSize);

			m_RewardDay = *msg.m_RewardDay; // ���� ����
			m_RewardMoney = *msg.m_RewardMoney; // �����
		}
		break;
	case SV_PM_2019HAVEST_REQUEST_RESULT:
		{
			bool bModaled = m_popChuSeokSub.GetSafeHwnd();

			if (bModaled == false)
			{
				m_popChuSeokSub.SetTxtChip(m_pTxtChip->GetText());
				m_popChuSeokSub.DoModal(this);
			}

			GM().SetPlayDailyGoldEffect(false);

			OnOK();
		}
		break;
	}
}

void CChuSeokPopup::InitLSYControl()
{
	m_pTxtDay = (LSY::CText*)GetObject(ID_CHUSEOKPOPUP_TXT_DAY);
	m_pTxtChip = (LSY::CText*)GetObject(ID_CHUSEOKPOPUP_TXT_CHIP);

	CString day = "";
	CString chip = "";

	day.Format("%d", m_RewardDay);
	chip = g_GetNumberWithComma(m_RewardMoney) + "Ĩ";

	m_pTxtDay->SetText((LPCTSTR)day);
	m_pTxtChip->SetText((LPCTSTR)chip);

	m_pBtnOk = (LSY::CButton*)GetObject(ID_CHUSEOKPOPUP_BTN_OK);
	m_pBtnOk->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CChuSeokPopup::OnClickButton));
}

void CChuSeokPopup::SetData()
{

}

void CChuSeokPopup::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

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
