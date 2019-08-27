#include "StdAfx.h"
#include "VIPJackpotGroupDisplay.h"
#include "VIPJackpotInfoDlg.h"
#include "VIPJackpotRankDlg.h"
#include "../../WidgetID.h"


CVIPJackpotGroupDisplay::CVIPJackpotGroupDisplay()
: m_pTabMenu(NULL)
, m_pInfoDlg(NULL)
, m_pRankDlg(NULL)
{
	m_nMotionType = EMotionType_SpreadDown;
}

CVIPJackpotGroupDisplay::~CVIPJackpotGroupDisplay()
{

}

void CVIPJackpotGroupDisplay::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	bool bAddChild = false;
	switch(pObject->GetID())
	{
	case ID_LOBBY_BTN_VIPJACKPOT_CLOSE:
		bAddChild = true;
		break;
	case ID_LOBBY_TAB_VIPJACKPOT_MENU:
		m_pTabMenu = (LSY::CTabControl *)pObject;
		m_pTabMenu->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CVIPJackpotGroupDisplay::OnRoomTypeSelChange));
		break;
	case ID_LOBBY_DLG_VIPJACKPOT_INFO:
		m_pInfoDlg = (CVIPJackpotInfoDlg *)pObject;
		break;
	case ID_LOBBY_DLG_VIPJACKPOT_RANK:
		m_pRankDlg = (CVIPJackpotRankDlg *)pObject;
		m_pRankDlg->SetShow(false);
		break;
	}

	if(bAddChild)
	{
		pObject->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CVIPJackpotGroupDisplay::OnClickButton));
	}

	__super::OnCreateChild(pObject);
}

LRESULT CVIPJackpotGroupDisplay::OnClickButton(LSY::CMessage *msg)
{
	LSY::CMO_MouseLClick *pMsg = (LSY::CMO_MouseLClick *)msg;
	LSY::CButton *pBtn = (LSY::CButton *)pMsg->GetObject();
	switch(pBtn->GetID())
	{
	case ID_LOBBY_BTN_VIPJACKPOT_CLOSE:
		{
			m_nState = EState_Closing;
		}
		break;
	}
	return TRUE;
}
LRESULT CVIPJackpotGroupDisplay::OnRoomTypeSelChange(LSY::CMessage *msg)
{
	NMBASE::SKINGDI::NMRESAM.PlayWav( "UI-SND_BUTTON_CLICK_A" );
	LSY::CMO_SelChange *pMsg = (LSY::CMO_SelChange *)msg;
	int nIndex = pMsg->GetValue();
	LSY::CRadio *pRadio = m_pTabMenu->GetRadioFromIndex(nIndex);
	int nID = pRadio->GetID();

	switch(nID)
	{
	case ID_LOBBY_RDO_VIPJACKPOT_INFO:
		m_pInfoDlg->SetShow(true);
		m_pRankDlg->SetShow(false);
		break;
	case ID_LOBBY_RDO_VIPJACKPOT_RANK:
		m_pInfoDlg->SetShow(false);
		m_pRankDlg->SetShow(true);
		break;
	}

	return TRUE;
}


void CVIPJackpotGroupDisplay::ShowDetailInfo()
{
	m_nState = EState_Openning;
	CObjectBase::SetShow(true);
}