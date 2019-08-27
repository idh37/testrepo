// Probability_AllCard.cpp: implementation of the CJokboProbability class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JokboProbability7Poker.h"
#include "../GlobalGame.h"
#include "../SelectCard7Poker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define  MAX_CARDINDEX	7
#define  MAX_DEPTH		6	//7포커는 6 , 8포커는 7

CJokboProbability7Poker::CJokboProbability7Poker()
{
	Clear();
}

CJokboProbability7Poker::~CJokboProbability7Poker()
{
}

void CJokboProbability7Poker::set_MyCard_Info( int no )
{
	if( no < 0 || no >= MAX_DUMMY )return;	
	m_cMyCardInfo.set_CardNo( no );	
}

void CJokboProbability7Poker::set_OtherCard_Info( int card[] , int total )
{
	for (int i = 0 ; i < total ; i++ ){
		m_cOtherCardInfo.set_CardNo( card[i] );
	}
}

void CJokboProbability7Poker::set_OtherCard_Info( int card )
{
	if( card < 0 || card >= MAX_DUMMY )return;	
	m_cOtherCardInfo.set_CardNo( card );
}

void CJokboProbability7Poker::set_RoundInfo( int round )
{	
	if( round >= en_8_Round )return;

	m_nCurRound = round;
}

void CJokboProbability7Poker::SetAllUserCardInfo()
{
	const int nMaxPlayer = g_GAMEPROCESS()->GetMaxPlayer();
	set_RoundInfo(g_PLAYER(0)->m_nTotalCardCount);

	// 장수 증가 할것
	for(int i=0; i<nMaxPlayer; i++) 
	{			
		if(strlen(g_PLAYER(i)->UI.ID)>0 && g_PLAYER(i)->JoinState == 1 && g_PLAYER(i)->PlayState == 1 && !g_PLAYER(i)->bFold ) // 겜중이면
		{
			if( i == 0 )
			{
				for( int c = 0 ; c < g_PLAYER(0)->m_nTotalCardCount ; c++ )
				{
					set_MyCard_Info( g_PLAYER(0)->m_cMyCard[c].GetCardNum());
				}
			}
			else
			{
				for( int c = 0 ; c < g_PLAYER(i)->m_nTotalCardCount ; c++ )
				{
					int no = g_PLAYER(i)->m_cMyCard[c].GetCardNum();
					if( no >= MAX_DUMMY )
						continue;
					set_OtherCard_Info( no );
				}
			}
		}
	}

	//버린카드 저장
	set_OtherCard_Info( g_SelectCard().GetDisCard() );
}

void CJokboProbability7Poker::CalculationProbability()
{
	if( m_nCurRound <= en_No_None || m_nCurRound >= en_8_Round )return;
	if( m_cMyCardInfo.GetTotal() <= 0 )return;

	m_cCardDeck.Clear();
	int i = 0 ;
	for( i = 0 ; i < MAX_DUMMY ; i++ ){
		if( m_cMyCardInfo.IsSame_CardNo(i) || m_cOtherCardInfo.IsSame_CardNo(i) ){
			continue;			
		}
		else{
			m_cCardDeck.set_CardNo(i);
		}
	}

	m_cProValue.Clear();
	m_cProValue_CntDeck.Clear();

	int t_Card[MAX_CARDNUM];
	memset( t_Card,-1,sizeof(t_Card) );

	for( i = 0 ; i < m_nCurRound ; i ++ ){
		t_Card[i] = m_cMyCardInfo.get_CardNo(i);
	}	

	OnWhile(0,m_cCardDeck.GetTotal(),MAX_DEPTH,m_nCurRound,t_Card);

	if( m_cProValue_CntDeck.GetTotal() > 0 && m_nCompareCnt > 0 ){
		for( i = 0 ; i < en_Max ; i ++ )
		{
			m_cProValue.set_Data( i , (double)((m_cProValue_CntDeck.get_Data(i) / (double)m_nCompareCnt) * 100) );
		}
	}
}

int CJokboProbability7Poker::GetValueToIndex( int value )
{
	switch(value)
	{
	default:
	case CCardRule::N7C_DUMMY:
		{
			return en_None;
		}
		break;
	case CCardRule::N7C_ONEPAIR:
		{
			return en_One;
		}
		break;
	case CCardRule::N7C_TWOPAIR:
		{
			return en_Two;
		}
		break;
	case CCardRule::N7C_TRIPLE:
		{
			return en_Three;
		}
		break;
	case CCardRule::N7C_STRAIGHT:
		{
			return en_Straight;
		}
		break;
	case CCardRule::N7C_BACKS:
		{
			return en_BackStraight;
		}
		break;
	case CCardRule::N7C_MOUNT:
		{
			return en_Mount;
		}
		break;
	case CCardRule::N7C_FLUSH:
		{
			return en_Flush;
		}
		break;
	case CCardRule::N7C_TITLE:
		{
			return en_FullHouse;
		}
		break;
	case CCardRule::N7C_QUADS:
		{
			return en_FourCard;
		}
		break;
	case CCardRule::N7C_SFULSH:
		{
			return en_SF;
		}
		break;	
	case CCardRule::N7C_CROYAL:
	case CCardRule::N7C_DROYAL:
	case CCardRule::N7C_HROYAL:
	case CCardRule::N7C_SROYAL:
		{
			return en_RSF;
		}
		break;
	}

	return en_None;
}

BOOL CJokboProbability7Poker::OnWhile( int start, int end ,int end_depth ,int cur_depth ,int cardno[])
{		
	int i = 0;	
	for( i = start ; i < end ; i ++ )
	{
		if( cur_depth == end_depth )
		{
			m_nCompareCnt ++;

			cardno[cur_depth]=m_cCardDeck.get_CardNo(i);

			CSevenRuleMan rule;
			rule.ClearAll();

			rule.set_total_Card(0,cardno,end_depth+1);
			rule.get_cal_rule_user(0);
			int value = rule.get_MyRule(0);

			int Index = GetValueToIndex(value);
			if( Index > en_None )
			{
				m_cProValue_CntDeck.set_Data(Index);
			}
		}
		else
		{
			cardno[cur_depth] = m_cCardDeck.get_CardNo(i);
			OnWhile(i+1,end,end_depth,cur_depth+1,cardno);
		}
	}	

	return FALSE;
}

int CJokboProbability7Poker::GetMyCurJokboIndex()
{
	if(NULL == g_GAMEPROCESS())
		return -1;

	return GetValueToIndex(g_GAMEPROCESS()->GetSevenRuleManager().get_MyRule(0));
}

float CJokboProbability7Poker::GetProbability(int nJokbo)
{
	return (float)m_cProValue.get_Data(nJokbo);
}