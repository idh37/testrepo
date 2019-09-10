// Probability_AllCard.cpp: implementation of the CJokboProbability class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JokboProbabilityHighLow.h"
#include "../GlobalGame.h"
#include "../SelectCardHighLow.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define  MAX_CARDINDEX	7
#define  MAX_DEPTH		6	//7��Ŀ�� 6 , 8��Ŀ�� 7


void CJokboProbabilityHighLow::Clear()
{
	CJokboProbability::Clear();


	//�ο� ����
	m_ProValue_low.Clear();
	m_ProValue_CntDeck_low.Clear();

}
void CJokboProbabilityHighLow::Reset()
{
	CJokboProbability::Reset();


	//�ο� ����
	m_ProValue_low.Clear();
	m_ProValue_CntDeck_low.Clear();

	

}


void CJokboProbabilityHighLow::Init(int nMaxPlayer, int nMaxValue)
{

	
	m_cProValue.Init(en_Max);
	m_cProValue_CntDeck.Init(en_Max);


	m_ProValue_low.Init(en_Low_Max);
	m_ProValue_CntDeck_low.Init(en_Low_Max);

}


CJokboProbabilityHighLow::CJokboProbabilityHighLow()
{
	Clear();
}

CJokboProbabilityHighLow::~CJokboProbabilityHighLow()
{
}

void CJokboProbabilityHighLow::set_MyCard_Info( int no )
{
	if( no < 0 || no >= MAX_DUMMY )return;	
	m_cMyCardInfo.set_CardNo( no );	
}

void CJokboProbabilityHighLow::set_OtherCard_Info( int card[] , int total )
{
	for (int i = 0 ; i < total ; i++ ){
		m_cOtherCardInfo.set_CardNo( card[i] );
	}
}

void CJokboProbabilityHighLow::set_OtherCard_Info( int card )
{
	if( card < 0 || card >= MAX_DUMMY )return;	
	m_cOtherCardInfo.set_CardNo( card );
}

void CJokboProbabilityHighLow::set_RoundInfo( int round )
{	
	if( round >= en_8_Round )return;

	m_nCurRound = round;
}

void CJokboProbabilityHighLow::SetAllUserCardInfo()
{
	const int nMaxPlayer = g_GAMEPROCESS()->GetMaxPlayer();
	set_RoundInfo(g_PLAYER(0)->m_nTotalCardCount);

	// ��� ���� �Ұ�
	for(int i=0; i<nMaxPlayer; i++) 
	{			
		if(strlen(g_PLAYER(i)->UI.ID)>0 && g_PLAYER(i)->JoinState == 1 && g_PLAYER(i)->PlayState == 1 && !g_PLAYER(i)->bFold ) // �����̸�
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

	//����ī�� ����
	set_OtherCard_Info( g_SelectCard().GetDisCard() );
}

void CJokboProbabilityHighLow::CalculationProbability()
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

	//�ο� ����
	if( m_ProValue_CntDeck_low.GetTotal() > 0 )
	{
		for( i = 0 ; i < en_Low_Max ; i ++ ){
			m_ProValue_low.set_Data( i , (double)((m_ProValue_CntDeck_low.get_Data(i) / (double)m_nCompareCnt) * 100) );
		}
	}
}

int CJokboProbabilityHighLow::GetValueToIndex( int value )
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

int CJokboProbabilityHighLow::get_value_Index_Low( int value )
{
	switch(value)
	{
	default:
		{
			return en_Low_None;
		}
		break;
	case 5:
		{
			return en_5_Top;
		}
		break;
	case 6:
		{
			return en_6_Top;
		}
		break;
	case 7:
		{
			return en_7_Top;
		}
		break;
	case 8:		
		{
			return en_8_Top;
		}
		break;
	case 9:
		{
			return en_9_Top;
		}
		break;
	case 10:
		{
			return en_10_Top;
		}
		break;
	case 11:
		{
			return en_J_Top;
		}
		break;
	case 12:
		{
			return en_Q_Top;
		}
		break;
	case 13:
		{
			return en_K_Top;
		}
		break;
	;	
	}

	return en_Low_None;

}

BOOL CJokboProbabilityHighLow::OnWhile( int start, int end ,int end_depth ,int cur_depth ,int cardno[])
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

			//�ο� ����
			{
				rule.m_cRule[0].get_cal_sevenLowRule();
				Index = rule.get_MyRule_Low(0);

				Index = Index % 13;
				Index+=2;
				Index = get_value_Index_Low(Index);		
				if( Index == en_Low_None )continue;

				m_ProValue_CntDeck_low.set_Data(Index);
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

int CJokboProbabilityHighLow::GetMyCurJokboIndex()
{
	if(NULL == g_GAMEPROCESS())
		return -1;

	return GetValueToIndex(g_GAMEPROCESS()->GetSevenRuleManager().get_MyRule(0));
}

int CJokboProbabilityHighLow::GetMyCurJokboIndex_low()
{
	if(NULL == g_GAMEPROCESS())
		return -1;

	int nValue_low = g_GAMEPROCESS()->GetSevenRuleManager().get_MyRule_Low(0);
	nValue_low = nValue_low % 13;
	nValue_low+=2;

	return get_value_Index_Low(nValue_low);
}



int CJokboProbabilityHighLow::GetHighJokboIndex(int nJokbo)	//�������� �ε��� �ΰ�?
{
	//nJokbo : 0~11 ������ ��������, 12~21������ �ο�����(�ο칫ž����)
	if (nJokbo>=0 && nJokbo<en_Max)	//���������̸�?
	{
		return nJokbo;
	}

	return -1;
}
int CJokboProbabilityHighLow::GetLowJokboIndex(int nJokbo)	//�ο����� �ε��� �ΰ�?
{
	//nJokbo : 0~11 ������ ��������, 12~21������ �ο�����(�ο칫ž����)
	int nLowIdx = nJokbo - en_Max;
	if  (nLowIdx>=0 && nLowIdx<en_Low_Max)	//�ο������̸�?
	{
		return nLowIdx;
	}

	return -1;
}

float CJokboProbabilityHighLow::GetProbability(int nJokbo)
{
	//nJokbo : 0~11 ������ ��������, 12~21������ �ο�����(�ο칫ž����)

	int nJokboIndex = GetHighJokboIndex(nJokbo);
	if (nJokboIndex>=0)
	{
		return (float)m_cProValue.get_Data(nJokboIndex);
	}
		//�ο������̴�.
	nJokboIndex = GetLowJokboIndex(nJokbo);

	if (nJokboIndex>=0)
	{
		//�ο������ϰ�� �� ���������� ������ ǥ���� ���� �ʴ´�. return 0.0f;
		if (GetMyCurJokboIndex_low()>=nJokboIndex)
			return 0.0f;

		return (float)m_ProValue_low.get_Data(nJokboIndex);
	}

	

	return .0f;

	
}