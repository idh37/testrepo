#include "StdAfx.h"
#include "ChanceButton.h"
#include "../../WidgetID.h"

CChanceButton::CChanceButton(void)
{
	m_pText = NULL;
	m_nRewardItemCode = 0;
	m_dwLastUpdateTime = 0;
	m_bEventChanceActive = FALSE;
}

CChanceButton::~CChanceButton(void)
{
}

void CChanceButton::OnCreated(void)
{
	__super::OnCreated();
}


void CChanceButton::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	bool bAddChild = false;
	switch(pObject->GetID())
	{	
	case ID_LOBBY_TEXT_MADE:		
		m_pText		= (LSY::CText *)pObject;	m_pText->SetText("");	break;	
	case ID_LOBBY_TEXT_REGEN:	
		m_pText		= (LSY::CText *)pObject;	m_pText->SetText("");	break;
	}	
	__super::OnCreateChild(pObject);
}



void CChanceButton::SetChanceInfo(int nRewardItemType, int nRewardItemCode, COleDateTime &timeExpire )
{
	m_nRewardItemType = nRewardItemType;
	m_nRewardItemCode = nRewardItemCode;
	m_timeExpire = timeExpire;
	m_dwLastUpdateTime = 0;

	MakeSetText();

	SetEnable(true);

	SetUseLogic(true);	
}
void CChanceButton::OnExpire(void)
{
	ClearChance();

	CWnd *pWnd = GetObjectMan()->GetWnd();
	pWnd->SendMessage(ID_LOBBY_PROCESS_REFRESHCHANCEITEM);
}
void CChanceButton::MakeSetText(void)
{
	//남은시간은?
	COleDateTimeSpan timeLeft =  m_timeExpire - g_GetSyncTime();
	CString strText;	
	/*
	strText.Format(	"%s %02d:%02d:%02d", 
		CRewardItemMan::GetRewardItemName(m_nRewardItemCode), 
		(int)timeLeft.GetTotalHours(), 
		timeLeft.GetMinutes(), 
		timeLeft.GetSeconds());
		*/
	strText.Format(	"%02d:%02d:%02d", 
		(int)timeLeft.GetTotalHours(), 
		timeLeft.GetMinutes(), 
		timeLeft.GetSeconds());

	m_pText->SetText(strText.GetString());
}
void CChanceButton::ClearChance(void)
{
	m_nRewardItemCode = 0;
	m_timeExpire = g_GetSyncTime();
	SetEnable(false);
	SetUseLogic(false);
	m_pText->SetText("");
}

void CChanceButton::Logic(const DWORD &dwTime)
{
	__super::Logic(dwTime);

	//1초마다 갱신한다.
	if ((dwTime - m_dwLastUpdateTime) >=1000)
	{
		m_dwLastUpdateTime = dwTime;
		if (g_GetSyncTime()>=m_timeExpire)
		{
			OnExpire();
		}
		else
		{
			MakeSetText();
		}
	}
}

int CChanceButton::GetRewartItemType()
{
	return m_nRewardItemType;
}

int CChanceButton::GetRewartItemCode()
{
	return m_nRewardItemCode;
}
