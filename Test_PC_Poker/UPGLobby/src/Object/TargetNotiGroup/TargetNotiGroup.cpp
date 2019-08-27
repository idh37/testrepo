#include "stdafx.h"
#include "TargetNotiGroup.h"
#include "../../WidgetID.h"
#include "CommMsgDef_Game.h"

#define _DEF_TARGETNOTI_TIMER 123479

CTargetNotiGroup::CTargetNotiGroup()
:   m_pBtnCong(NULL),
m_pBtnAddFriend(NULL),
m_pTextID(NULL),
m_pTextMoney(NULL)
{
	InitializeCriticalSection(&m_cLockQueue);
}

CTargetNotiGroup::~CTargetNotiGroup()
{
	DeleteCriticalSection(&m_cLockQueue);
	GetObjectMan()->KillTimer(_DEF_TARGETNOTI_TIMER);
}

void CTargetNotiGroup::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	bool bAddChild = false;
	switch(pObject->GetID())
	{
	case ID_LOBBY_BTN_TARGETNOTI_CONG:
		{
			m_pBtnCong			= (LSY::CButton *)pObject;
			m_pBtnCong->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CTargetNotiGroup::OnClickButton));
		}
		break;
	case ID_LOBBY_BTN_TARGETNOTI_ADD_FRIEND:
		{
			m_pBtnAddFriend		= (LSY::CButton *)pObject;
			m_pBtnAddFriend->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CTargetNotiGroup::OnClickButton));
		}
		break;
	case ID_LOBBY_TEXT_TARGETNOTI_ID:
		{
			m_pTextID		= (LSY::CText *)pObject;
		}
		break;
	case ID_LOBBY_TEXT_TARGETNOTI_MONEY:
		{
			m_pTextMoney		= (LSY::CText *)pObject;
		}
		break;
	}			

	__super::OnCreateChild(pObject);
}


LRESULT CTargetNotiGroup::OnClickButton(LSY::CMessage *msg)
{
	LSY::CMO_MouseLClick *pMsg = (LSY::CMO_MouseLClick *)msg;
	LSY::CButton *pBtn = (LSY::CButton *)pMsg->GetObject();
	CWnd *pWnd = GetObjectMan()->GetWnd();
	switch(pBtn->GetID())
	{
	case ID_LOBBY_BTN_TARGETNOTI_CONG:
		{
			TargetData data = m_QueData.front();
			CCongMessageDlg dlg;
			dlg.DoModal(GM().GetMainWnd(), data.id);
		}
		break;
	case ID_LOBBY_BTN_TARGETNOTI_ADD_FRIEND:
		{
			CBuddyAddDlg dlg;
			dlg.DoModal(m_pTextID->GetText(), GM().GetMainWnd());
		}
		break;
	}
	return TRUE;
}

void CTargetNotiGroup::SetTextInfo(std::string id, int money, int time)
{
	CThreadLock lock(&m_cLockQueue);
	CString str;
	str.Format("%d 조", money);
	CString strID;
	strID.Format("%s", id.c_str());
	if(m_QueData.empty())
	{
		m_pTextID->SetText(strID.GetString());
		m_pTextMoney->SetText(str.GetString());
		GetObjectMan()->SetTimer(_DEF_TARGETNOTI_TIMER, 5 * 1000, LSY::Fnt(this, &CTargetNotiGroup::OnTimer));	//서버시간과 독립적
	}

	TargetData	data;
	data.id = id;
	data.money = str.GetString();
	
	m_QueData.push(data);
}

LRESULT CTargetNotiGroup::OnTimer(LSY::CMessage *msg)
{
	CThreadLock lock(&m_cLockQueue);
	if(m_QueData.empty())
		return FALSE;

	m_QueData.pop();
	if(m_QueData.empty())
	{
		GetObjectMan()->KillTimer(_DEF_TARGETNOTI_TIMER);
		SetShow(false);
	}
	else
	{
		TargetData data = m_QueData.front();
		m_pTextID->SetText(data.id);
		m_pTextMoney->SetText(data.money);
	}

	return TRUE ;
}
