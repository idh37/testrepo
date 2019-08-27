#include "StdAfx.h"
#include "TitleButton.h"
#include "../../WidgetID.h"

CTitleButtonGroup::CTitleButtonGroup()
:	m_pBtnPCRoom(NULL),
	m_pBtnCapture(NULL),
	m_pBtnSetup(NULL),
	m_pBtnInfo(NULL),
	m_pBtnGrade(NULL),
	m_pBtnMinimize(NULL),
	m_pBtnWindow(NULL),
	m_pBtnMaxmize(NULL),
	m_pBtnClose(NULL),
	m_pBtnVIPCounsel(NULL)
{
}

CTitleButtonGroup::~CTitleButtonGroup()
{
}

void CTitleButtonGroup::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	bool bAddChild = false;
	switch(pObject->GetID())
	{
	case ID_LOBBY_BTN_VIP_COUNSEL:
		{
			m_pBtnVIPCounsel	= (LSY::CButton *)pObject;
			m_pBtnVIPCounsel->SetShow(false);
			bAddChild = true;
		}
		break;
	case ID_LOBBY_BTN_PCROOM:
		{
			m_pBtnPCRoom	= (LSY::CButton *)pObject;
			m_pBtnPCRoom->SetShow(false);
			bAddChild = true;
		}
		break;
	case ID_LOBBY_BTN_CAPTUER:	m_pBtnCapture	= (LSY::CButton *)pObject; bAddChild = true; break;
	case ID_LOBBY_BTN_SETUP:	m_pBtnSetup		= (LSY::CButton *)pObject; bAddChild = true; break;
	case ID_LOBBY_BTN_INFO:		m_pBtnInfo		= (LSY::CButton *)pObject; bAddChild = true; break;
	case ID_LOBBY_BTN_GRADE:	m_pBtnGrade		= (LSY::CButton *)pObject; bAddChild = true; break;
	case ID_LOBBY_BTN_MINIMIZE: m_pBtnMinimize	= (LSY::CButton *)pObject; bAddChild = true; break;
	case ID_LOBBY_BTN_WINDOW:	m_pBtnWindow	= (LSY::CButton *)pObject; bAddChild = true; break;
	case ID_LOBBY_BTN_CLOSE:	m_pBtnClose		= (LSY::CButton *)pObject; bAddChild = true; break;
	case ID_LOBBY_BTN_MAXIMIZE: m_pBtnMaxmize	= (LSY::CButton *)pObject; bAddChild = true; break;
	case ID_LOBBY_BTN_TIME_NOTIFIER: m_pBtnTimeNotifier	= (LSY::CButton *)pObject; bAddChild = true; break;
	}

	if(bAddChild)
	{
		pObject->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CTitleButtonGroup::OnClickButton));
	}
}

LRESULT CTitleButtonGroup::OnClickButton(LSY::CMessage *msg)
{
	LSY::CMO_MouseLClick *pMsg = (LSY::CMO_MouseLClick *)msg;
	LSY::CButton *pBtn = (LSY::CButton *)pMsg->GetObject();

	switch(pBtn->GetID())
	{
	case ID_LOBBY_BTN_CLOSE:
		NMBASE::SKINGDI::NMRESAM.PlayWav( "UI-SND_BUTTON_CLICK_C" );
		break;
	}

	CWnd *pWnd = GetObjectMan()->GetWnd();
	switch(pBtn->GetID())
	{
	case ID_LOBBY_BTN_CLOSE:
		pWnd->SendMessage(LBG_BUTTON_ID_EXIT);
		break;
	case ID_LOBBY_BTN_SETUP:
		{
			SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_TITLEBAR_OPTION);

			pWnd->SendMessage(LBG_BUTTON_ID_OPTION);
		} break;
	case ID_LOBBY_BTN_INFO:
		SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_TITLEBAR_GAMERULE);
		pWnd->SendMessage(LBG_BUTTON_ID_GAMEMANUAL);
		break;
	case ID_LOBBY_BTN_GRADE:
		pWnd->SendMessage(LBG_BUTTON_ID_GRADE);
		break;
	case ID_LOBBY_BTN_MINIMIZE:
		pWnd->SendMessage(LBG_BUTTON_ID_MINIMIZE);
		break;
	case ID_LOBBY_BTN_MAXIMIZE:
	case ID_LOBBY_BTN_WINDOW:
		pWnd->SendMessage(LBG_BUTTON_ID_CHANGESCREEN);
		break;
	case ID_LOBBY_BTN_PCROOM:
		pWnd->SendMessage(LBG_BUTTON_ID_PCROOM);
		break;
	case ID_LOBBY_BTN_VIP_COUNSEL:
		pWnd->SendMessage(LBG_BUTTON_ID_VIPCOUNSEL);
		break;
	case ID_LOBBY_BTN_CAPTUER:
		SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_TITLEBAR_CAPTURE);	
		SaveScreenCapture();
		break;
	case ID_LOBBY_BTN_TIME_NOTIFIER:
		#ifdef UPGRADE_10210619_ADD_CLICKLOG
			SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_TIMEALARM);
		#endif //UPGRADE_10210619_ADD_CLICKLOG
		pWnd->SendMessage(LBG_BUTTON_ID_TIMENOTIFIER);
		break;
	}
	return TRUE;
}

void CTitleButtonGroup::SetVIPCounselShow(bool bShow)
{
	m_pBtnVIPCounsel->SetShow(bShow);
}

void CTitleButtonGroup::SetChangeDisplayMode(bool bFull)
{
	if(bFull)
	{
		m_pBtnWindow->SetShow(true);
		m_pBtnMaxmize->SetShow(false);
	}
	else
	{
		m_pBtnWindow->SetShow(false);
		m_pBtnMaxmize->SetShow(true);
	}
}

void CTitleButtonGroup::SetPCRoomBtn(int nPCRoomType)
{
	int nSprNo = 0;

#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
	{
		m_pBtnPCRoom->SetShow(false);
		return;
	}
#endif //UPGRADE_20120308_TOCTOC_CHANNELING

	if ( nPCRoomType == CODE_PCROOM_NOT )
	{
		nSprNo=0;
	}
	else if ( nPCRoomType == CODE_PCROOM_BASIC)
	{
		nSprNo=12;
	}
	else if ( nPCRoomType == CODE_PCROOM_PLUS )
	{
		nSprNo=12;
	}

	m_pBtnPCRoom->SetFourStatusButton(nSprNo);
	m_pBtnPCRoom->SetShow(true);
}