#include "StdAfx.h"
#include "Cards.h"


CCards::CCards(void)
{
	ClearCards();
}

CCards::~CCards(void)
{
}

void CCards::ClearCards()
{
	m_dqAvailable.clear();

	m_dqStraight.clear();
	m_dqTogether.clear();
	m_dqAttachStr.clear();
	m_dqAttachTog.clear();
	m_dqSeven.clear();

//	m_cDuplicateCards.ClearCards();
}