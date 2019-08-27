// Probability_AllCard.cpp: implementation of the CJokboProbability class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JokboProbability.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

PRO_VALUE::PRO_VALUE()
:	m_plistProValue(NULL),
	m_nTotal(0),
	m_nMaxValue(0)
{
}

PRO_VALUE::~PRO_VALUE()
{
	if(m_plistProValue)	delete [] m_plistProValue;
	m_plistProValue = NULL;
}

void PRO_VALUE::Init(int nMaxValue)
{
	if(m_plistProValue)	delete m_plistProValue;
	m_nMaxValue = nMaxValue;
	m_plistProValue = new double[m_nMaxValue];
}

void PRO_VALUE::set_Data( int index )
{
	if( index < 0 || index >= m_nMaxValue )return;

	m_plistProValue[index]++;
	m_nTotal++;
}

void PRO_VALUE::set_Data( int index , double pro )
{
	m_plistProValue[index] = pro;	
}

double PRO_VALUE::get_Data( int index )
{
	if( index < 0 || index >= m_nMaxValue )return 0.0f;
	return m_plistProValue[index];
}

void PRO_VALUE::Clear()
{
	m_nTotal = 0;
	for( int i = 0 ; i < m_nMaxValue ; i ++ ){
		m_plistProValue[i] = 0.0f;
	}
}

PRO_CARD::PRO_CARD()
{
	m_nTotal = 0;
}

void PRO_CARD::Clear(){
	m_nTotal = 0;
	memset(m_listCardNo,-1,sizeof(m_listCardNo));
}

void PRO_CARD::set_CardNo( int no )
{
	if( no < 0 || no >=MAX_DUMMY )return;		
	m_listCardNo[m_nTotal] = no;
	m_nTotal++;
}

BOOL PRO_CARD::IsSame_CardNo( int no )
{
	if( no < 0 || no >= MAX_DUMMY )return TRUE;

	for( int i = 0 ; i < m_nTotal ; i ++ )
	{
		if( m_listCardNo[i] == no )
		{
			return TRUE;
		}
	}

	return FALSE;
}

int PRO_CARD::get_CardNo( int index )
{
	if( index < 0 || index >= MAX_DUMMY )return -1;
	return m_listCardNo[index];
}

PRO_CARD::~PRO_CARD()
{
}

CJokboProbability::CJokboProbability(void)
{
	m_nMaxPlayer = 0;
}

CJokboProbability::~CJokboProbability(void)
{
}

void CJokboProbability::Init(int nMaxPlayer, int nMaxValue)
{
	m_nMaxPlayer = nMaxValue;
	m_cProValue.Init(nMaxValue);
	m_cProValue_CntDeck.Init(nMaxValue);
}

void CJokboProbability::Clear()
{
	m_cProValue.Clear();
	m_cProValue_CntDeck.Clear();
	m_cMyCardInfo.Clear();
	m_cOtherCardInfo.Clear();
	m_cCardDeck.Clear();
	m_nCurRound = en_No_None;
	m_nCompareCnt = 0;
}

void CJokboProbability::Reset()
{
	m_cProValue.Clear();
	m_cProValue_CntDeck.Clear();
	m_cMyCardInfo.Clear();
	m_cOtherCardInfo.Clear();
	m_nCompareCnt = 0;
	m_cCardDeck.Clear();
}