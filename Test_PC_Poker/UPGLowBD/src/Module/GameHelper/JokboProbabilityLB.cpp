// Probability_AllCard.cpp: implementation of the CJokboProbability class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JokboProbabilityLB.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CJokboProbabilityLB::CJokboProbabilityLB()
{
}

CJokboProbabilityLB::~CJokboProbabilityLB()
{
}

void CJokboProbabilityLB::SetAllUserCardInfo()
{
}

void CJokboProbabilityLB::CalculationProbability()
{
}

int CJokboProbabilityLB::GetMyCurJokboIndex()
{
	CBadugiRule::BADUGI_CARD nValue = (CBadugiRule::BADUGI_CARD)g_PLAYER( 0 )->GetMyLowBDRule()->m_MyValue;
	int nIndex = 0;
	switch(nValue)
	{
	case CBadugiRule::B4C_TOP:
		nIndex = 0;
		break;
	case CBadugiRule::B4C_GOLF:
		nIndex = 6;
		break;
	case CBadugiRule::B4C_SECOND:
		nIndex = 5;
		break;
	case CBadugiRule::B4C_THIRD:
		nIndex = 4;
		break;
	case CBadugiRule::B4C_MADE:
		nIndex = 3;
		break;
	case CBadugiRule::B4C_BASE:
		nIndex = 2;
		break;
	case CBadugiRule::B4C_TWO_BASE:
		nIndex = 1;
		break;
	}
	return nIndex;
}

float CJokboProbabilityLB::GetProbability(int nJokbo)
{
	return 0.0f;
}