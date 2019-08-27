#include "stdafx.h"
#include "VIPJackpotAlertGroup.h"
#include "VIPJackpotAlertGuide.h"
#include "../GameOverlayid.h"


CVIPJackpotAlertGroup::CVIPJackpotAlertGroup()
: m_pBtnGuide(NULL)
, m_pGuide(NULL)
{

}

CVIPJackpotAlertGroup::~CVIPJackpotAlertGroup()
{

}

void CVIPJackpotAlertGroup::OnCreateChild( LSY::CObjectBase *pObject )
{
	if(pObject == NULL) 
		return;

	switch(pObject->GetID())
	{
	case ID_GAMEOVERLAY_BTN_VIPJACKPOT_INFO:
		m_pBtnGuide = (LSY::CButton *)pObject;
		break;
	case ID_GAMEOVERLAY_DLG_VIPJACKPOT_INFO:
		m_pGuide = (CVIPJackpotAlertGuide*)pObject;
		m_pGuide->SetShow(false);
		break;
	}

	__super::OnCreateChild(pObject);
}

LRESULT CVIPJackpotAlertGroup::OnClickButton(LSY::CMessage *msg)
{
	LSY::CMO_MouseLClick *pMsg = (LSY::CMO_MouseLClick *)msg;
	LSY::CButton *pBtn = (LSY::CButton *)pMsg->GetObject();
	switch(pBtn->GetID())
	{
	case ID_GAMEOVERLAY_BTN_VIPJACKPOT_INFO:
		break;

	}
	return TRUE;
}
