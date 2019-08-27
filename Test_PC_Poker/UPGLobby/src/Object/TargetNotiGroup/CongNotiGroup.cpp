#include "stdafx.h"
#include "CongNotiGroup.h"
#include "../../WidgetID.h"
#include "CommMsgDef_Game.h"

#define _DEF_CONGNOTI_TIMER 123489

CCongNotiGroup::CCongNotiGroup()
:   
m_pTextID(NULL),
m_pTextMessage(NULL)
{
	InitializeCriticalSection(&m_cLockQueue);
}

CCongNotiGroup::~CCongNotiGroup()
{
	DeleteCriticalSection(&m_cLockQueue);
	GetObjectMan()->KillTimer(_DEF_CONGNOTI_TIMER);
}

void CCongNotiGroup::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	bool bAddChild = false;
	switch(pObject->GetID())
	{
	case ID_LOBBY_TEXT_CONGNOTI_ID:
		{
			m_pTextID		= (LSY::CText *)pObject;
		}
		break;
	case ID_LOBBY_TEXT_CONGNOTI_MESSAGE:
		{
			m_pTextMessage		= (LSY::CText *)pObject;
		}
		break;
	}			

	__super::OnCreateChild(pObject);
}

void CCongNotiGroup::SetTextInfo(std::string id, std::string msg, int time)
{
	CThreadLock lock(&m_cLockQueue);
	if(m_QueData.empty())
	{
		m_pTextID->SetText(id);
		m_pTextMessage->SetText(msg);

		GetObjectMan()->SetTimer(_DEF_CONGNOTI_TIMER, time * 1000, LSY::Fnt(this, &CCongNotiGroup::OnTimer));
	}

	CongData data;
	data.id = id;
	data.message = msg;

	m_QueData.push(data);
}

LRESULT CCongNotiGroup::OnTimer(LSY::CMessage *msg)
{
	CThreadLock lock(&m_cLockQueue);
	if(m_QueData.empty())
		return FALSE;

	m_QueData.pop();
	if(m_QueData.empty())
	{
		GetObjectMan()->KillTimer(_DEF_CONGNOTI_TIMER);
		SetShow(false);
	}
	else
	{
		CongData data = m_QueData.front();
		m_pTextID->SetText(data.id);
		m_pTextMessage->SetText(data.message);
	}

	return TRUE;
}
