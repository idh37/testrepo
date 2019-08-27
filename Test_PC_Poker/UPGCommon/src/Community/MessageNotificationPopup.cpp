#include "stdafx.h"
#include "MessageNotificationPopup.h"
#include "MessageNotificationPopupID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMessageNotificationPopup::CMessageNotificationPopup()
:	LSY::CLSYLibWindowsBase(),
	m_nStatus(EMNS_START_UNKNOWN),
	m_dwStartTime(0),
	m_dwEndTime(0),
	m_ptPos(LSY::CPointZero),
	m_rtRect(LSY::CRectZero),
	m_dwSlideTime(0),
	m_dwDelayTime(0),
	m_bFirst(true),
	m_bMouseOver(false),
	m_bMemo(true)
{
	m_cMemo.Clear();
}

BEGIN_MESSAGE_MAP(CMessageNotificationPopup, LSY::CLSYLibWindowsBase)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

int CMessageNotificationPopup::GetProjectID(void)
{
	return ID_MESSAGENOTIFICATIONPOPUP;
}

CPoint CMessageNotificationPopup::GetPos(void)
{
	return m_ptPos.GetPoint();
}

bool CMessageNotificationPopup::Create(CWnd *pParent, int x, int y)
{
	ASSERT(pParent != NULL);
	Init(GetObjectMan(), ID_MESSAGENOTIFICATIONPOPUP, true);
	if(!CLSYLibWindowsBase::CreateEx(0, AfxRegisterWndClass(CS_DBLCLKS), "MessageNotificationPopup", WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN, x, y, 100, 100, pParent->GetSafeHwnd(), NULL))
	{
		return false;
	}

	LSY::CObjectProject *pProject = m_pObjectMan->GetProjectFromID(ID_MESSAGENOTIFICATIONPOPUP);
	m_pObjectMan->SetMouseOverFunctor(LSY::Fnt(this, &CMessageNotificationPopup::OnMouseOver));
	m_pObjectMan->SetMouseLeaveFunctor(LSY::Fnt(this, &CMessageNotificationPopup::OnMouseLeave));

	m_ptPos.SetPoint((float)x, (float)y);
	m_rtRect.SetRect(0,0,(float)pProject->GetWidth(), (float)pProject->GetHeight());
	m_rtRect.OffsetRect(m_ptPos);

	SetWindowPos(&CWnd::wndTop, (int)m_rtRect.left, (int)m_rtRect.top, (int)m_rtRect.GetWidth(), 0, SWP_NOACTIVATE | SWP_SHOWWINDOW);
	ShowWindow(SW_HIDE);
	return true;
}

LRESULT CMessageNotificationPopup::OnTimer(LSY::CMessage *pMsg)
{
	LSY::CMO_Timer *msg = (LSY::CMO_Timer *)pMsg;
	DWORD dwCurTime = timeGetTime();

	if(msg->GetID() == TIMER_MSGNOTIPOP_SLIDE)
	{
		if(m_bFirst)
		{
			m_dwStartTime = dwCurTime;
			m_dwEndTime = m_dwStartTime + m_dwSlideTime;
			m_bFirst = false;
		}

		switch(m_nStatus)
		{
		case EMNS_START_SLIDE:
			{
				LSY::CRect rtRect = m_rtRect;

				if(dwCurTime >= m_dwEndTime)
				{
					m_dwStartTime = m_dwEndTime;
					m_dwEndTime = m_dwStartTime+m_dwDelayTime;

					m_nStatus = EMNS_SHOW_DELAY;
				}
				else
				{
					float fRate = 1.0f - float(dwCurTime - m_dwStartTime)/float(m_dwEndTime - m_dwStartTime);
					float fHeight = m_rtRect.GetHeight() * fRate;

					rtRect.top = rtRect.top + m_rtRect.GetHeight() - (m_rtRect.GetHeight() - fHeight);
				}

				SetWindowPos(&CWnd::wndTop, (int)rtRect.left, (int)rtRect.top, (int)rtRect.GetWidth(), (int)rtRect.GetHeight(), SWP_NOACTIVATE | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOZORDER);
			}
			break;
		case EMNS_SHOW_DELAY:
			{
				if(dwCurTime >= m_dwEndTime)
				{
					m_dwStartTime = m_dwEndTime;
					m_dwEndTime = m_dwStartTime + m_dwSlideTime;

					m_nStatus = EMNS_END_SLIDE;
				}
			}
			break;
		case EMNS_END_SLIDE:
			{
				LSY::CRect rtRect = m_rtRect;
				if(dwCurTime >= m_dwEndTime)
				{
					m_nStatus = EMNS_START_UNKNOWN;
					m_pObjectMan->KillTimer(TIMER_MSGNOTIPOP_SLIDE);
					SetWindowPos(&CWnd::wndTop, (int)m_rtRect.left, (int)m_rtRect.top, (int)m_rtRect.GetWidth(), 0, SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOZORDER);
					ShowWindow(SW_HIDE);
					GM().GetMainWnd()->PostMessage(CM_CLOSE_MEMO_NOTIFY_POPUP, 0, (LPARAM)this);
					break;
				}
				else
				{
					float fRate = float(dwCurTime - m_dwStartTime)/float(m_dwEndTime - m_dwStartTime);
					float fHeight = m_rtRect.GetHeight() * fRate;

					rtRect.top = rtRect.top + m_rtRect.GetHeight() - (m_rtRect.GetHeight() - fHeight);
				}

				SetWindowPos(&CWnd::wndTop, (int)rtRect.left, (int)rtRect.top, (int)rtRect.GetWidth(), (int)rtRect.GetHeight(), SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOZORDER);
			}
			break;
		}
	}

	return TRUE;
}

LRESULT CMessageNotificationPopup::OnMouseOver(LSY::CMessage *pMsg)
{
	if(m_nStatus == EMNS_START_SLIDE)
	{
		m_dwStartTime = timeGetTime();
		m_dwEndTime = m_dwStartTime + m_dwDelayTime;
		m_nStatus = EMNS_SHOW_DELAY;
	}
	else if(m_nStatus == EMNS_END_SLIDE)
	{
		m_dwStartTime = timeGetTime();
		m_dwEndTime = m_dwStartTime;
		m_nStatus = EMNS_SHOW_DELAY;
	}

	m_bMouseOver = true;

	m_pObjectMan->KillTimer(TIMER_MSGNOTIPOP_SLIDE);
	SetWindowPos(&CWnd::wndTop, (int)m_rtRect.left, (int)m_rtRect.top, (int)m_rtRect.GetWidth(), (int)m_rtRect.GetHeight(), SWP_NOACTIVATE);
	return TRUE;
}

LRESULT CMessageNotificationPopup::OnMouseLeave(LSY::CMessage *pMsg)
{
	DWORD dwCurTime = timeGetTime();
	if(m_dwEndTime <= dwCurTime)
	{
		m_dwStartTime = dwCurTime;
		m_dwEndTime = m_dwStartTime + m_dwSlideTime;
		m_nStatus = EMNS_SHOW_DELAY;
	}

	m_bMouseOver = false;

	m_pObjectMan->SetTimer(TIMER_MSGNOTIPOP_SLIDE, 1000/30, LSY::Fnt(this, &CMessageNotificationPopup::OnTimer));
	LSY::CMO_Timer msg(TIMER_MSGNOTIPOP_SLIDE, 0, 0);
	OnTimer(&msg);
	return TRUE;
}

bool CMessageNotificationPopup::OnCreatedProject(void)
{
	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_MESSAGENOTIFICATIONPOPUP_BTN_CLOSE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageNotificationPopup::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_MESSAGENOTIFICATIONPOPUP_BTN_BUDDY_CLOSE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageNotificationPopup::OnClickButton));


	pButton = (LSY::CButton *)GetObject(ID_MESSAGENOTIFICATIONPOPUP_BTN_SHOW_OPTION);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CMessageNotificationPopup::OnClickButton));

	return TRUE;
}

LRESULT CMessageNotificationPopup::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseLClick *msg = (LSY::CMO_MouseLClick *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_MESSAGENOTIFICATIONPOPUP_BTN_SHOW_OPTION:
		GM().GetMainWnd()->PostMessage(CM_SHOW_OPTION);
	case ID_MESSAGENOTIFICATIONPOPUP_BTN_CLOSE:
	case ID_MESSAGENOTIFICATIONPOPUP_BTN_BUDDY_CLOSE:
		SetWindowPos(&CWnd::wndTop, (int)m_rtRect.left, (int)m_rtRect.top, (int)m_rtRect.GetWidth(), 0, SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOZORDER);
		ShowWindow(SW_HIDE);
		m_pObjectMan->KillTimer(TIMER_MSGNOTIPOP_SLIDE);
		GM().GetMainWnd()->PostMessage(CM_CLOSE_MEMO_NOTIFY_POPUP, 0, (LPARAM)this);
		break;
	}
	return TRUE;
}

void CMessageNotificationPopup::ShowNotification(Camel::Dar2::SS::Memo *pMemo, DWORD dwSlideTime/* = 1000*/, DWORD dwDelayTime/* = 2000*/)
{
	m_bMemo = true;
	m_cMemo.Copy(*pMemo);
	InitData();

	m_dwSlideTime = dwSlideTime;
	m_dwDelayTime = dwDelayTime;

	m_bFirst = true;

	m_nStatus = EMNS_START_SLIDE;
	m_pObjectMan->SetTimer(TIMER_MSGNOTIPOP_SLIDE, 1000/30, LSY::Fnt(this, &CMessageNotificationPopup::OnTimer));

	ShowWindow(SW_SHOW);
}

void CMessageNotificationPopup::ShowNotification(std::string strConnectBuddy, DWORD dwSlideTime/* = 1000*/, DWORD dwDelayTime/* = 2000*/)
{
	m_bMemo = false;
	m_strConnectUserNickName = strConnectBuddy;
	InitData();

	m_dwSlideTime = dwSlideTime;
	m_dwDelayTime = dwDelayTime;

	m_bFirst = true;

	m_nStatus = EMNS_START_SLIDE;
	m_pObjectMan->SetTimer(TIMER_MSGNOTIPOP_SLIDE, 1000/30, LSY::Fnt(this, &CMessageNotificationPopup::OnTimer));

	ShowWindow(SW_SHOW);
}


void CMessageNotificationPopup::InitData(void)
{
	LSY::CObjectProject *pProject = m_pObjectMan->GetProjectFromLayer(0);
	LSY::CWindows *pMainWnd = (LSY::CWindows *)pProject->GetMainObject();

	LSY::CGroup *pGroupMemo = (LSY::CGroup *)GetObject(ID_MESSAGENOTIFICATIONPOPUP_GRP_MEMO);
	LSY::CGroup *pGroupBuddy = (LSY::CGroup *)GetObject(ID_MESSAGENOTIFICATIONPOPUP_GRP_BUDDY);

	pGroupMemo->SetShow(m_bMemo?true:false);
	pGroupBuddy->SetShow(m_bMemo?false:true);

	if(m_bMemo)
	{
		LSY::CText *pText = (LSY::CText *)GetObject(ID_MESSAGENOTIFICATIONPOPUP_TXT_SENDER);
		pText->SetText(m_cMemo.senderID);
		pMainWnd->SetIndex(0);
	}
	else
	{
		LSY::CText *pText = (LSY::CText *)GetObject(ID_MESSAGENOTIFICATIONPOPUP_TXT_CONNECT_BUDDY);
		pText->SetText(m_strConnectUserNickName);
		pMainWnd->SetIndex(5);
	}
}

void CMessageNotificationPopup::OnDestroy()
{
	m_pObjectMan->KillTimer(TIMER_MSGNOTIPOP_SLIDE);
	__super::OnDestroy();
}

LRESULT CMessageNotificationPopup::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if(__super::WindowProc(message, wParam, lParam)) return TRUE;

	switch(message)
	{
	case WM_LBUTTONDOWN:
		SetWindowPos(&CWnd::wndTop, (int)m_rtRect.left, (int)m_rtRect.top, (int)m_rtRect.GetWidth(), 0, SWP_NOACTIVATE | SWP_SHOWWINDOW | SWP_NOZORDER);
		ShowWindow(SW_HIDE);
		m_pObjectMan->KillTimer(TIMER_MSGNOTIPOP_SLIDE);

		if(m_bMemo)
		{
			if(!m_cMemo.read) CCommunityManager::Instance()->SendReadMemo(m_cMemo);
		}
		else
		{
			SetUserListTabType(ESUT_MY_BUDDY);
		}

		GM().GetMainWnd()->PostMessage(CM_CLOSE_MEMO_NOTIFY_POPUP, 0, (LPARAM)this);

		break;
	}

	return FALSE;
}

BOOL CMessageNotificationPopup::OnEraseBkgnd(CDC* pDC)
{
	CDC *pMemDC = m_pObjectMan->GetMemDC();
	pDC->BitBlt(0, 0, (int)m_rtRect.GetWidth(), (int)m_rtRect.GetHeight(), pMemDC, 0, 0, SRCCOPY);
	return FALSE;
}