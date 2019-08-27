// BadugiRuleMan.cpp: implementation of the CBadugiRuleMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BadugiRuleMan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBadugiRuleMan::CBadugiRuleMan()
{

}

CBadugiRuleMan::~CBadugiRuleMan()
{

}

void CBadugiRuleMan::BDR_Clear(int mode)
{
	m_PlayTotalNum = 0;
	m_nGameMode = 0;
	if( mode < 0 || mode > 1)return;

	m_nGameMode = mode;
	for( int i = 0 ;  i < 5 ;  i++ ){
		m_MycardRule[i].Init(m_nGameMode);
	}	
}

void CBadugiRuleMan::BDR_SetUserCard(int pnum , int card[] , int cardnum , BOOL bfirst )
{
	if( pnum < 0 || pnum >= BD_TOTAL_USER )return;
	m_MycardRule[pnum].SetCard(pnum,card,cardnum);
	m_MycardRule[pnum].GetMyRule();		

	if( bfirst )m_PlayTotalNum ++;
}

void CBadugiRuleMan::BDR_FoldUser(int pnum)
{
	if( pnum < 0 || pnum >= BD_TOTAL_USER )return;
	//m_MycardRule[pnum].Clear();
	if( m_MycardRule[pnum].m_bFold == TRUE)return;
	m_MycardRule[pnum].SetFold();	
	m_MycardRule[pnum].SetRanking(m_PlayTotalNum);
	m_PlayTotalNum -= 1;
}


void CBadugiRuleMan::BDR_GetWinner(int playnum ,int nwinner[] , int &nwinnernum ,int rank[])
{
	typedef struct  {
		int pnum;
		int rank;
		int point;
	}RANK;
	RANK RankData[5];
	RANK RankData1[5];
	ZeroMemory(RankData,sizeof(RankData));
	ZeroMemory(RankData1,sizeof(RankData));

	memset(RankData , -1,sizeof(RankData));

	m_nCurTotalUser = playnum;

	int cnt = 0;
	for( int i = 0 ; i < m_nCurTotalUser ; i ++ )
	{
		if( m_MycardRule[i].PNum == -1 )continue;
		
		if( m_MycardRule[i].m_bFold == FALSE ){
			if( m_MycardRule[i].m_MyValue >= 0 ){
				RankData[cnt].pnum = i;
				RankData[cnt].point = m_MycardRule[i].m_RuleCompleteCard.numberpoint;
				
				RankData1[cnt].pnum = i;
				RankData1[cnt].point = m_MycardRule[i].m_RuleCompleteCard.numberpoint;
				cnt ++;
			}		
		}		
	}	
	
	for( int i = 0; i < m_nCurTotalUser; i++) //오름차순.
	{
		for(int j = i; j < m_nCurTotalUser; j++)
		{
			// A
			if(RankData[j].point  >  RankData[i].point)
			{
				RANK temp;
				memcpy(&temp , &RankData[i] ,sizeof(RANK) );
				memcpy(&RankData[i], &RankData[j] ,sizeof(RANK) );
				memcpy(&RankData[j], &temp ,sizeof(RANK) );			
			}		
		}
	}

	int arank = 0;
	for( int i = 0; i < m_nCurTotalUser; i++) //오름차순.
	{
		if( RankData[i].pnum == -1 )continue;
		if( m_MycardRule[RankData[i].pnum].m_bFold == FALSE ){
			if( RankData[i].point == RankData[i-1].point && i-1 >= 0 && i > 0)
			{
				RankData[i].rank = arank;
			}
			else{
				RankData[i].rank = ++arank;
			}
			
			m_MycardRule[RankData[i].pnum].SetRanking(RankData[i].rank);
		}
	}

	for( int i = 0 ; i < playnum ; i ++ )
	{
#if !defined(__DUALPOKER__)
		if( m_MycardRule[i].PNum == -1 )continue;
#endif
		int nrank  = m_MycardRule[i].GetRanking();
		int pnum = m_MycardRule[i].PNum;
		if( nrank == 1 )
		{			
			
			nwinner[pnum] = pnum;
			nwinnernum ++;
		}
		rank[pnum] = nrank;
	}
}

int  CBadugiRuleMan::BDR_GetWinner()
{
	int pnum = -1;

	for(int i = 0 ; i < m_nCurTotalUser ; i ++ )
	{
		int rank  = m_MycardRule[i].GetRanking();
		if( rank == 1 )
		{			
			pnum = i;
			return pnum;
		}	
	}

	return pnum;
}

int  CBadugiRuleMan::BDR_GetWinnerCnt(int rank)
{
	int cnt = 0 ;

	for(int i = 0 ; i < 5 ; i ++ ){
		int rank  = m_MycardRule[i].GetRanking();
		if( rank == rank ){			
			cnt++;
		}	
	}

	return cnt;
}
