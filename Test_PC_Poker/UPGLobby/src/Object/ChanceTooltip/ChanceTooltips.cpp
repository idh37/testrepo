#include "StdAfx.h"
#include "ChanceTooltips.h"

CChanceTooltips::CChanceTooltips(void)
{
	m_pSpr7Poker = NULL;
	m_pSprNewPoker = NULL;
	m_pSprBadugi = NULL;
	m_pSprSutda = NULL;
	m_pSprGisa7Poker = NULL;
	m_pSprGisaBadugi = NULL;
	m_pSprGisaNewPoker = NULL;
}

CChanceTooltips::~CChanceTooltips(void)
{
}

void CChanceTooltips::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;

	bool bAddChild = false;
	switch(pObject->GetID())
	{
	case ID_LOBBY_SPR_CHANCE_TOOLTIP_SP_HL:			m_pSpr7Poker		= (LSY::CSprite *)pObject;		break;
	case ID_LOBBY_SPR_CHANCE_TOOLTIP_BD:			m_pSprBadugi		= (LSY::CSprite *)pObject;		break;
	case ID_LOBBY_SPR_CHANCE_TOOLTIP_NP:			m_pSprNewPoker		= (LSY::CSprite *)pObject;		break;
	case ID_LOBBY_SPR_CHANCE_TOOLTIP_SD:			m_pSprSutda			= (LSY::CSprite *)pObject;		break;
	case ID_LOBBY_SPR_GISACHANCE_TOOLTIP_SP:		m_pSprGisa7Poker		= (LSY::CSprite *)pObject;		break;
	case ID_LOBBY_SPR_GISACHANCE_TOOLTIP_BD:		m_pSprGisaBadugi		= (LSY::CSprite *)pObject;		break;
	case ID_LOBBY_SPR_GISAHANCE_TOOLTIP_NP:			m_pSprGisaNewPoker			= (LSY::CSprite *)pObject;		break;
	
	}	
	__super::OnCreateChild(pObject);

}
void CChanceTooltips::OnCreated(void)
{
	
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
	OnCreateChild(pProject->GetObject(ID_LOBBY_SPR_CHANCE_TOOLTIP_SP_HL));
	OnCreateChild(pProject->GetObject(ID_LOBBY_SPR_CHANCE_TOOLTIP_BD));
	OnCreateChild(pProject->GetObject(ID_LOBBY_SPR_CHANCE_TOOLTIP_NP));
	OnCreateChild(pProject->GetObject(ID_LOBBY_SPR_CHANCE_TOOLTIP_SD));
	OnCreateChild(pProject->GetObject(ID_LOBBY_SPR_GISACHANCE_TOOLTIP_SP));
	OnCreateChild(pProject->GetObject(ID_LOBBY_SPR_GISACHANCE_TOOLTIP_BD));
	OnCreateChild(pProject->GetObject(ID_LOBBY_SPR_GISAHANCE_TOOLTIP_NP));

	__super::OnCreated();

}

void CChanceTooltips::ShowToolTip(int gamecode, int itemcode, int itemtype, LSY::CPoint pos)
{
	SetShow(true);

	m_pSpr7Poker->SetShow(false);
	m_pSprNewPoker->SetShow(false);
	m_pSprBadugi->SetShow(false);
	m_pSprSutda->SetShow(false);
	m_pSprGisa7Poker->SetShow(false);
	m_pSprGisaBadugi->SetShow(false);
	m_pSprGisaNewPoker->SetShow(false);

	LSY::CSprite *pSprite = NULL;

	switch(gamecode)
	{
	case GAME_CODE_SP:
	case GAME_CODE_HL:
		{
			if(itemtype != CRewardItemMan::REWARD_TYPE_REGEN)
				pSprite = m_pSpr7Poker;
			else
				pSprite = m_pSprGisa7Poker ;
		} break;
	case GAME_CODE_BD:
		{
			if(itemtype != CRewardItemMan::REWARD_TYPE_REGEN)
				pSprite = m_pSprBadugi;
			else
				pSprite = m_pSprGisaBadugi;
		} break;
	case GAME_CODE_NP:
		{
			if(itemtype != CRewardItemMan::REWARD_TYPE_REGEN)
				pSprite = m_pSprNewPoker;
			else
				pSprite = m_pSprGisaNewPoker;
		} break;
	case GAME_CODE_SD:
		{
			pSprite = m_pSprSutda;
		} break;	
	}

	if (pSprite)
	{
		if(itemtype != CRewardItemMan::REWARD_TYPE_REGEN)
		{
			if (itemcode==ITEM_SCODE_REWARD_VVIP)
			{
				pSprite->SetPlayIndex(1);
			}
			else
			{
				pSprite->SetPlayIndex(0);
			}
		}

		pSprite->SetShow(true);
		pSprite->StartAnimation();
	}

	SetPos(pos.x + 3, pos.y + 27);
}