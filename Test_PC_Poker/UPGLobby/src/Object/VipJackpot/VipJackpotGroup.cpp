#include "StdAfx.h"
#include "VipJackpotGroup.h"
#include "VipJackpotMoney.h"
#include "VipJackpotDlg.h"
#include "../../WidgetID.h"
#include "CommMsgDef_Game.h"

#define _DEF_JACKPOT_TIMER 123459

CVipJackpotGroup::CVipJackpotGroup()
:   m_pJackpotMoney(NULL),
	m_pJackpotRelease(NULL),
	m_pBtnJackpotDetail(NULL),
	m_pVipJackpotDlg(NULL)
{

}

CVipJackpotGroup::~CVipJackpotGroup()
{
	GetObjectMan()->KillTimer(_DEF_JACKPOT_TIMER);
}

void CVipJackpotGroup::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	bool bAddChild = false;
	switch(pObject->GetID())
	{
	case ID_LOBBY_NUM_VIPJACKPOTMONEY:
		{
			m_pJackpotMoney			= (CVipJackpotMoney *)pObject;
		}
		break;
	case ID_LOBBY_IMG_VIPJACKPOTTIME:
		{
			m_pJackpotRelease		= (LSY::CImage *)pObject;
			pObject->AddHandler(LSY::EM_O_MOUSEOVER, LSY::Fnt(this, &CVipJackpotGroup::OnReleaseMouseOver));
			pObject->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this, &CVipJackpotGroup::OnReleaseMouseLeave));
		}
		break;
	case ID_LOBBY_BTN_VIPJACKPOTDETAIL:			m_pBtnJackpotDetail		= (LSY::CButton *)pObject;		bAddChild = true; break;
	case ID_LOBBY_DLG_VIPJACKPOT:
		{
			m_pVipJackpotDlg		= (CVipJackpotDlg *)pObject;
			m_pVipJackpotDlg->SetShow(false);

			pObject->AddHandler(LSY::EM_O_LOCALCHILDNOTIFY, LSY::Fnt(this, &CVipJackpotGroup::OnDetailDlgLocalChildNotify));
		}
		break;
	}			

	if(bAddChild)
	{
		pObject->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CVipJackpotGroup::OnClickButton));
	}

	__super::OnCreateChild(pObject);
}

LRESULT CVipJackpotGroup::OnReleaseMouseOver(LSY::CMessage *msg)
{
	LSY::CMO_MouseOver *pMsg = (LSY::CMO_MouseOver *)msg;
	LSY::CImage *pImage = (LSY::CImage *)pMsg->GetObject();

	return TRUE;
}

LRESULT CVipJackpotGroup::OnReleaseMouseLeave(LSY::CMessage *msg)
{
	LSY::CMO_MouseLeave *pMsg = (LSY::CMO_MouseLeave *)msg;
	LSY::CImage *pImage = (LSY::CImage *)pMsg->GetObject();

	return TRUE;
}

LRESULT CVipJackpotGroup::OnDetailDlgLocalChildNotify(LSY::CMessage *msg)
{
	LSY::CMO_LocalChildNotify *pMsg = (LSY::CMO_LocalChildNotify *)msg;
	LSY::CMessage *pMsg2 = pMsg->GetMessage();

	switch(pMsg2->GetID())
	{
	case LSY::EM_O_MOUSEOVER:
		return OnReleaseMouseOver(pMsg2);
	case LSY::EM_O_MOUSELEAVE:
		return OnReleaseMouseLeave(pMsg2);
	}

	return FALSE;
}

LRESULT CVipJackpotGroup::OnClickButton(LSY::CMessage *msg)
{
	LSY::CMO_MouseLClick *pMsg = (LSY::CMO_MouseLClick *)msg;
	LSY::CButton *pBtn = (LSY::CButton *)pMsg->GetObject();
	CWnd *pWnd = GetObjectMan()->GetWnd();
	switch(pBtn->GetID())
	{
	case ID_LOBBY_BTN_VIPJACKPOTDETAIL:
		{
			if(false==m_pVipJackpotDlg->IsShow()){
				CCV_REQVIP_RANK_INFO msg;
				msg.Set();
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
				m_pVipJackpotDlg->SetShowDlg();
			}
			else{
				m_pVipJackpotDlg->SetShow(false);
			}
		}
		break;
	}
	return TRUE;
}


LRESULT CVipJackpotGroup::OnTimer(LSY::CMessage *msg)
{

	return TRUE ;
}
