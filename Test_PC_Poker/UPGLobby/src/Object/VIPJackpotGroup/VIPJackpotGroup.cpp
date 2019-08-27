#include "StdAfx.h"
#include "VIPJackpotGroup.h"
#include "VIPJackpotGroupDisplay.h"
#include "../Jackpot/JackpotMoney.h"

#include "../../WidgetID.h"

CVIPJackpotGroup::CVIPJackpotGroup()
: m_pJackpotMoney(NULL)
, m_pBtnJackpotDetail(NULL)
, m_pDisplay(NULL)
{
	
}

CVIPJackpotGroup::~CVIPJackpotGroup()
{

}

void CVIPJackpotGroup::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) 
		return;

	bool bAddChild = false;

	switch(pObject->GetID())
	{
	case ID_LOBBY_NUMBER_VIPJACKPOT:
		{
			m_pJackpotMoney			= (CJackpotMoney *)pObject;
			m_pJackpotMoney->SetShow(true); 
		}
		break;
	case ID_LOBBY_BTN_VIPJACKPOT_DETAIL:
		{
			m_pBtnJackpotDetail		= (LSY::CButton *)pObject;
			bAddChild = true;
		}
		break;
	case ID_LOBBY_GRP_VIPJACKPOT_DISPLAY:
		{
			m_pDisplay = (CVIPJackpotGroupDisplay *)pObject;
			pObject->SetShow(false);
		}
		break;
	}

	if(bAddChild)
		pObject->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CVIPJackpotGroup::OnClickButton));

	__super::OnCreateChild(pObject);
}

LRESULT CVIPJackpotGroup::OnClickButton(LSY::CMessage *msg)
{
	LSY::CMO_MouseLClick *pMsg = (LSY::CMO_MouseLClick *)msg;
	LSY::CButton *pBtn = (LSY::CButton *)pMsg->GetObject();
	switch(pBtn->GetID())
	{
	case ID_LOBBY_BTN_VIPJACKPOT_DETAIL:
		{
			m_pDisplay->ShowDetailInfo();
		}
		break;
	}
	return TRUE;
}