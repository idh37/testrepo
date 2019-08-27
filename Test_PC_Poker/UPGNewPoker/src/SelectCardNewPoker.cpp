// SelectCard.cpp: implementation of the CSelectCard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SelectCardNewPoker.h"
#include <CommMsgDef_Game.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CSelectCardNewPoker::CSelectCardNewPoker()
{
}

CSelectCardNewPoker::~CSelectCardNewPoker()
{
}

void CSelectCardNewPoker::OnEndSelectCard(BOOL bautoselect)
{
	int cnt = 0;
	int i = 0 ;

	for(i=0;i<m_nMaxNum;i++){
		if( m_cChoiceCardDeck.nCard[i] == m_cChoiceCardDeck.nOPenCardNum )continue;
		if( m_cChoiceCardDeck.nCard[i] == m_cChoiceCardDeck.nDiscardnum )continue;				
		GAME()->GetPlayerNP(0)->m_listCard[cnt].SetCardNo(m_cChoiceCardDeck.nCard[i]); // 히든 카드( 현재 2장)
		cnt++;
	}

	for( i = m_cChoiceCardDeck.nDiscardindex; i < m_nMaxNum-1; i++) {
		m_cChoiceCardDeck.nCard[i] = m_cChoiceCardDeck.nCard[i+1];		
		m_cChoiceCardDeck.nCard[i+1] = 0;
	}
	
	for(i = 0; i < m_nMaxNum; i++) {
		if(m_cChoiceCardDeck.nCard[i] == m_cChoiceCardDeck.nOPenCardNum) // 카드값
		{	
			m_cChoiceCardDeck.nOPenCardIndex = i ;
			break;
		}
	}

	//2가 안되면 엿됨.ㅡㅡㅋ
	if( cnt == 2 ){
		GAME()->GetPlayerNP(0)->m_listCard[cnt].SetCardNo(m_cChoiceCardDeck.nOPenCardNum);
	}
	
	GAME()->GetPlayerNP(0)->m_listCard[3].Clear();
	GAME()->GetPlayerNP(0)->m_nTotalCardNum -= 1;
	
	GAME()->GetPlayerNP(0)->SetChioceCardMove(TRUE , 1);
	
	if( bautoselect == FALSE )
	{
		GAME()->GetPlayerNP(0)->m_nCardChoiceLimtCnt = 0;
	}

	if( !g_ObserverMan.IsMyObserver() )
	{
		GAME()->GetPlayerNP(0)->m_nThrowAwayCardNo = m_cChoiceCardDeck.nDiscardnum;
	}
}