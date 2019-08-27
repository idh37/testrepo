// BadugiRule.cpp: implementation of the CBadugiRule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "BadugiRule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBadugiRule::CBadugiRule()
{
	Clear();
}

CBadugiRule::~CBadugiRule()
{
	//Clear();
}

void CBadugiRule::Clear()
{
	//처음 셋팅된 카드 값	
	memset(m_Card, -1, sizeof(m_Card));
	ZeroMemory(m_RealSortCard,sizeof(m_RealSortCard));
	//족보를 이룬 카드 	
	memset(&m_RuleCompleteCard, -1, sizeof(m_RuleCompleteCard));
	ZeroMemory(m_sortCard,sizeof(m_sortCard));	
	
	m_MyValue =0 ;
	m_nTotalCard = 0;	
	PNum = -1;
	m_MyRanking = -1;
	m_bFold = FALSE;
	m_strMyValueName = _T("");
}

void CBadugiRule::Init(int mode)
{

	m_nGameMode = mode;	
	Clear();
}

//족보를 구하기 위해 카드 셋팅
void CBadugiRule::SetCard( int pnum, int pCard[],int CardNum )
{
	if( pCard == NULL )return;
	if( CardNum <= 0 )return;
	if( pnum < 0 || pnum >= 5 )return;

	Clear();

	PNum = pnum;

	m_nTotalCard = CardNum;

	for( int i = 0 ; i < CardNum ; i ++ ){
		m_Card[i] = pCard[i];		
	}	

	memcpy(m_RealSortCard, m_Card , sizeof(m_RealSortCard));

	sort_Card(0,m_nTotalCard);
}

/* 카드 정렬 오름차순 */
void CBadugiRule::sort_Card(int Start, int End)
{	
	for(int i = Start; i < End; i++) //오름차순.
	{
		for(int j = i; j < End; j++)
		{
			// A
			if((m_RealSortCard[i]%13) > (m_RealSortCard[j]%13))
			{
				int temp = m_RealSortCard[i];
				m_RealSortCard[i] = m_RealSortCard[j];
				m_RealSortCard[j] = temp;
			}
			else if((m_RealSortCard[i]%13)==(m_RealSortCard[j]%13))
			{
				if(m_RealSortCard[i] > m_RealSortCard[j])
				{
					int temp = m_RealSortCard[i];
					m_RealSortCard[i] = m_RealSortCard[j];
					m_RealSortCard[j] = temp;
				}
			}
		}
	}


	int p0 = 0;
	int p1 = 0;
	int p2 = 0;
	int p3 = 0;
	for( int i = 0 ; i < End ; i ++ )
	{
		int temp = m_RealSortCard[i]%13 + 1;
		switch(m_RealSortCard[i]/13){
		case 0:
			{
				m_sortCard[0].cardnumber[p0] = temp;
				m_sortCard[0].numberpoint[p0] = SEED_NUMBERPOINT - temp;
				p0++;				
				m_sortCard[0].curcardnum = p0;
				m_sortCard[0].pattern = 0;
			}break;
		case 1:
			{
				m_sortCard[1].cardnumber[p1] = temp;
				m_sortCard[1].numberpoint[p1] = SEED_NUMBERPOINT - temp;
				p1++;
				m_sortCard[1].curcardnum = p1;
				m_sortCard[1].pattern = 1;
			}break;
		case 2:
			{
				m_sortCard[2].cardnumber[p2] = temp;
				m_sortCard[2].numberpoint[p2] = SEED_NUMBERPOINT - temp;
				p2++;
				m_sortCard[2].curcardnum = p2;
				m_sortCard[2].pattern = 2;
			}break;
		case 3:
			{
				m_sortCard[3].cardnumber[p3] = temp;
				m_sortCard[3].numberpoint[p3] = SEED_NUMBERPOINT - temp;
				p3++;
				m_sortCard[3].curcardnum = p3;
				m_sortCard[3].pattern = 3;
			}break;
		}		
	}	
}

//족보 구하기
void CBadugiRule::GetMyRule()
{
	//메이드나 베이스가 아니면 투 베이스를 구한다.
	if( m_nGameMode == 0 ){
		if( !Made_4() ){
			if( !Base() ){
				if(!TwoBase()){
					Top();							//탑 구하기
				}			
			}
		}	
	}
	else {
		if( !Made_5() ){
			if( !Base_5() ){
				TwoBase();
				if(!TwoBase()){
					Top();							//탑 구하기
				}			
			}
		}	
	}
	
	Result_Sort();
}

//골프 구하기
BOOL CBadugiRule::Made_4()
{
	typedef struct {
		int card[5];
		int point;
		int pettern[5];
		int havenumcnt;		
	}TempCardInfo_4;

	TempCardInfo_4 tci_4;
	memset(&tci_4,-1,sizeof(tci_4));
	tci_4.havenumcnt = 0;	
	tci_4.point = 0;
	
	m_MyValue = 0;		
	
	for( int m = 0 ; m < 4 ; m ++ )
	{		
		int tc = m_RealSortCard[m] % 13 + 1;
		int tp = m_RealSortCard[m] / 13;			
		
		BOOL bSame = FALSE;
		for( int x = 0 ; x < 4 ; x ++ )
		{
			if( tc > 0 && (tci_4.card[x] == tc || tci_4.pettern[x] == tp) ){
				bSame = TRUE;
				break;
			}
		}
		
		if( !bSame )
		{
			tci_4.card[tci_4.havenumcnt] = tc;
			tci_4.pettern[tci_4.havenumcnt] = tp;
			tci_4.point += SEED_NUMBERPOINT - tc;
			tci_4.havenumcnt++;
		}		
	}

	
	if( tci_4.havenumcnt >= 4 ){

		int golf[5]={1,2,3,4};	
		
		BOOL bGolf = TRUE; 
		m_MyValue = 1;
		for( int i = 0 ; i < 4 ; i ++ ){
			if( tci_4.card[i] != golf[i]){
				bGolf = FALSE;
			}
		}		
		if( bGolf == FALSE )m_MyValue = 2;

		if( m_MyValue == 2 ){
			int sce[5]={1,2,3,5};
			
			BOOL bSce = TRUE; 		
			for( int i = 0 ; i < 4 ; i ++ ){
				if( tci_4.card[i] != sce[i]){
					bSce = FALSE;
				}
			}		
			if( bSce == FALSE )m_MyValue = 3;
		}
		
		
		if( m_MyValue == 3 ){
			int thi[5]={1,2,4,5};
			BOOL bthi = TRUE; 
			
			for(  int i = 0 ; i < 4 ; i ++ ){
				if( tci_4.card[i] != thi[i]){
					bthi = FALSE;
				}
			}		
			if( bthi == FALSE )m_MyValue = 4;
		}
		

		/*
		if( tci_4.point >= THIRD && tci_4.point <= GOLF ){
			
		
			
			switch(tci_4.point) {
			case 46:
				{
					m_MyValue = 1;				
					
				}				
				break;
			case 45:
				{
					
					m_MyValue = 2;
				}				
				break;
			case 44:
				{
					
					m_MyValue = 3;
				}				
				break;		
			}
			
		}
		else
		{
			m_MyValue = 4;
		}
		*/
		

		m_RuleCompleteCard.numberpoint = 0;
		if( m_MyValue > 0 )
		{
			m_RuleCompleteCard.m_RuleCompleteCnt = 0;
			for( int i = 0 ; i < BD_TOTAL_CARD ; i ++ ){
				if( tci_4.card[i] > 0 ){
					m_RuleCompleteCard.RealCard[i] = (tci_4.card[i]-1) + (13 * tci_4.pettern[i]);
					m_RuleCompleteCard.Card[i] = tci_4.card[i];
					m_RuleCompleteCard.m_RuleCompleteCnt++;
				}
			}
		//	int nPlusPoint = 0 ;
		//	nPlusPoint = m_RuleCompleteCard.Card[3] - m_RuleCompleteCard.Card[0];
		//	nPlusPoint += m_RuleCompleteCard.Card[3] - m_RuleCompleteCard.Card[1];
		//	nPlusPoint += m_RuleCompleteCard.Card[3] - m_RuleCompleteCard.Card[2];
		//	m_RuleCompleteCard.numberpoint += 1000 - nPlusPoint;			
			
			return TRUE;
		} 		
	}

	return FALSE;
}

BOOL CBadugiRule::Base()
{
	m_MyValue = 0;
	int temp[4][3]={
		{0,1,2},
		{0,1,3},
		{0,2,3},
		{1,2,3},		
	};	

	typedef struct {
		int card[5];
		int point;
		int pettern[5];
		int havenumcnt;
		int totalpoint;
	}TempCardInfo;

	TempCardInfo tci[5];

	memset( tci , -1, sizeof(tci));
	for( int i = 0 ; i < 5 ;i++ ){
		tci[i].point = 0;
		tci[i].havenumcnt = 0;
	}
	
	
	for( int i = 0 ; i < 4 ; i ++ )
	{		
		tci[i].havenumcnt = 0;
		for ( int j = 0 ; j < 3 ; j ++ )
		{
			int index = temp[i][j];			
			int tc = m_RealSortCard[index] % 13 + 1;
			int tp = m_RealSortCard[index] / 13;

			BOOL bSame = FALSE;
			for( int x = 0 ; x < 3 ; x ++ )
			{
				if( tc > 0 && (tci[i].card[x] == tc || tci[i].pettern[x] == tp) ){
					bSame = TRUE;
					break;
				}
			}

			if( !bSame )
			{
				tci[i].card[tci[i].havenumcnt] = tc;
				tci[i].pettern[tci[i].havenumcnt] = tp;
				tci[i].point += SEED_NUMBERPOINT - tc;
				tci[i].havenumcnt++;
			}		
		}		
	}
	
	int index = -1;
	int point = -1;
	for ( int i = 0 ; i < 4 ; i ++ )
	{
		if( tci[i].havenumcnt >= 3 ) 
		{
			if( tci[i].point > point )
			{
				index = i;
				point = tci[i].point;				
			}
		}
	}

	m_RuleCompleteCard.numberpoint = 0;
	if( index > -1 && point > -1 )
	{
		m_MyValue = 5;
		m_RuleCompleteCard.m_RuleCompleteCnt = 0;
		
		int t = 0;
		for( int i = 0 ; i < 3 ; i ++ )
		{
			m_RuleCompleteCard.Card[i] = tci[index].card[i];
			m_RuleCompleteCard.RealCard[i] = (tci[index].card[i]-1) + (13 * tci[index].pettern[i]);		
			m_RuleCompleteCard.m_RuleCompleteCnt++;						
		}	

		/*
		//숫자들이 높은지 검사 하여 추가 점수를 준다.
		int npluspoint = 0;

		npluspoint = m_RuleCompleteCard.Card[2] - m_RuleCompleteCard.Card[0];
		npluspoint += m_RuleCompleteCard.Card[1] - m_RuleCompleteCard.Card[0];
		m_RuleCompleteCard.numberpoint += 300 - npluspoint;

	//	m_RuleCompleteCard.numberpoint = 300 + SEED_NUMBERPOINT - m_RuleCompleteCard.Card[2];
		*/
		return TRUE;
	}

	return FALSE;
}

BOOL CBadugiRule::Base_5()						//베이스 구하기
{
	m_MyValue = 0;
	int temp1[10][3]={
		{0,1,2},
		{0,1,3},
		{0,1,4},
		{0,2,3},
		{0,2,4},
		{0,3,4},
		{1,2,3},
		{1,2,4},
		{1,3,4},
		{2,3,4},
	};

	typedef struct {
		int card[5];
		int point;
		int pettern[5];
		int havenumcnt;
		int totalpoint;
	}TempCardInfo;

	TempCardInfo tci[10];

	memset( tci , -1, sizeof(tci));
	for( int i = 0 ; i < 10 ;i++ ){
		tci[i].point = 0;
		tci[i].havenumcnt = 0;
	}
	
	
	for( int i = 0 ; i < 10 ; i ++ )
	{	
		tci[i].havenumcnt = 0;				
		for ( int j = 0 ; j < 3 ; j ++ )
		{
			int index = temp1[i][j];
			int tc = m_RealSortCard[index] % 13 + 1;
			int tp = m_RealSortCard[index] / 13;
			
			BOOL bSame = FALSE;
			for( int x = 0 ; x < 3 ; x ++ )
			{
				if( tc > 0 && (tci[i].card[x] == tc || tci[i].pettern[x] == tp) ){
					bSame = TRUE;
					break;
				}
			}
			
			if( !bSame )
			{
				tci[i].card[tci[i].havenumcnt] = tc;
				tci[i].pettern[tci[i].havenumcnt] = tp;
				tci[i].point += SEED_NUMBERPOINT - tc;
				tci[i].havenumcnt++;
			}		
		}		
		
	}
	
	int index = -1;
	int point = -1;
	for ( int i = 0 ; i < 10 ; i ++ )
	{
		if( tci[i].havenumcnt >= 3 ) 
		{
			if( tci[i].point > point )
			{
				index = i;
				point = tci[i].point;				
			}
		}
	}

	m_RuleCompleteCard.numberpoint = 0;
	if( index > -1 && point > -1 )
	{
		m_MyValue = 5;
		m_RuleCompleteCard.m_RuleCompleteCnt = 0;
		
		int t = 0;
		for( int i = 0 ; i < 3 ; i ++ )
		{
			m_RuleCompleteCard.Card[i] = tci[index].card[i];
			m_RuleCompleteCard.RealCard[i] = (tci[index].card[i]-1) + (13 * tci[index].pettern[i]);		
			m_RuleCompleteCard.m_RuleCompleteCnt++;						
		}	

		/*
		//숫자들이 높은지 검사 하여 추가 점수를 준다.
		int npluspoint = 0;

		npluspoint = m_RuleCompleteCard.Card[2] - m_RuleCompleteCard.Card[0];
		npluspoint += m_RuleCompleteCard.Card[1] - m_RuleCompleteCard.Card[0];
		m_RuleCompleteCard.numberpoint += 300 - npluspoint;

	//	m_RuleCompleteCard.numberpoint = 300 + SEED_NUMBERPOINT - m_RuleCompleteCard.Card[2];
		*/
		return TRUE;
	}

	return FALSE;
}

//투베이스 구하기
BOOL CBadugiRule::TwoBase()
{
	m_MyValue = 0;
	
	typedef struct {
		int card[5];
		int point[5];
		int pettern[5];
		int havecardcnt;
	}TempCardInfo;
	
	typedef struct  {
		int Card[2];
		int Pattern[2];
		int Point;
	}TWOBASE;
	
	TWOBASE TData={0,};
	
	TempCardInfo tci;
	memset(&tci,-1,sizeof(TempCardInfo));
	TempCardInfo tci2;
	memset(&tci2,-1,sizeof(TempCardInfo));

	
	for( int h = 0 ; h < 5 ;h++ ){
		tci.point[h] = 0;
		tci2.point[h] = 0;
	}
	
	tci.havecardcnt = 0;
	tci2.havecardcnt = 0;
	BOOL bSamePettern = FALSE;
	int nSamePettern = 0;
	
	m_RuleCompleteCard.numberpoint = 0;
	m_RuleCompleteCard.m_RuleCompleteCnt = 0;
	

	//패턴 검사 무늬가 2장씩 같은지 검사.
	for( int i = 0 ; i < MAX_PATTERN ; i ++ ){
		if( m_sortCard[i].curcardnum >= 2 ){			
			
			if ( nSamePettern == 0 ){
				for( int j = 0 ; j < m_sortCard[i].curcardnum ; j ++ ){
					tci.card[j] = m_sortCard[i].cardnumber[j];
					tci.pettern[j] = m_sortCard[i].pattern;
					tci.havecardcnt = m_sortCard[i].curcardnum;
					tci.point[j] = m_sortCard[i].numberpoint[j];					
				}			
			}
			else if ( nSamePettern == 1 )
			{
				for( int j = 0 ; j < m_sortCard[i].curcardnum ; j ++ ){
					tci2.card[j] = m_sortCard[i].cardnumber[j];
					tci2.pettern[j] = m_sortCard[i].pattern;
					tci2.havecardcnt = m_sortCard[i].curcardnum;
					tci2.point[j] = m_sortCard[i].numberpoint[j];				
				}				
			}
			
			nSamePettern ++;
		}
	}	

	if( nSamePettern >= 2 ){		
	
		ZeroMemory(&TData,sizeof(TData));

		for( int i = 0 ; i < tci.havecardcnt ; i ++ )
		{
			for( int j = 0 ; j < tci2.havecardcnt ; j++ )
			{
				if( tci.card[i] == tci2.card[j] )continue;
				int point = tci.point[i] + tci2.point[j];

				if( point > TData.Point ){
					TData.Card[1] = tci.card[i];
					TData.Pattern[1]=tci.pettern[i];
					TData.Card[0] = tci2.card[j];
					TData.Pattern[0]=tci2.pettern[j];
					TData.Point = point;
				}
			}
		}

		if( TData.Point > 0 ){
			
			m_MyValue = 6;
			for( int i = 0 ; i < 2 ; i ++ ){
				if( TData.Card[i] > 0 ){
					m_RuleCompleteCard.RealCard[i] = (TData.Card[i]-1) + (13 * TData.Pattern[i]);					
					m_RuleCompleteCard.Card[i] =TData.Card[i];					
					m_RuleCompleteCard.m_RuleCompleteCnt++;
				}
			}			
			
			return TRUE;
		}
	}
	else{

		memset(&tci,-1,sizeof(TempCardInfo));
		memset(&tci2,-1,sizeof(TempCardInfo));
		tci.havecardcnt = 0;	//같은 숫자만 담는다.
		tci2.havecardcnt = 0;	//같은 무늬만 담는다.
		for( int h = 0 ; h < 5 ;h++ ){
			tci.point[h] = 0;
			tci2.point[h] = 0;
		}

		for(  int i = 0 ; i < MAX_PATTERN ; i ++ )
		{			
			if( m_sortCard[i].curcardnum == 0 )continue;			
			
			if( m_sortCard[i].curcardnum == 1 )
			{
				for( int x = 0 ;x < BD_TOTAL_CARD ; x ++ ){
					int tc = m_sortCard[i].cardnumber[x];
					if( tc == 0 )continue;
					tci.card[tci.havecardcnt] = m_sortCard[i].cardnumber[x];
					tci.pettern[tci.havecardcnt] = m_sortCard[i].pattern;
					tci.point[tci.havecardcnt] =  m_sortCard[i].numberpoint[x];
					tci.havecardcnt++;
				}				
			}			
			else if( m_sortCard[i].curcardnum == 2 || m_sortCard[i].curcardnum == 3 )
			{
				for( int x = 0 ;x < BD_TOTAL_CARD ; x ++ ){
					int tc = m_sortCard[i].cardnumber[x];
					if( tc == 0 )continue;
					tci2.card[tci2.havecardcnt] = m_sortCard[i].cardnumber[x];
					tci2.pettern[tci2.havecardcnt] = m_sortCard[i].pattern;
					tci2.point[tci2.havecardcnt] =  m_sortCard[i].numberpoint[x];
					
					tci2.havecardcnt++;
				}				
			}
			else {
				return FALSE;
			}
		}

		ZeroMemory(&TData,sizeof(TData));

		for(  int i = 0 ; i < tci.havecardcnt ; i ++ )
		{
			for( int j = 0 ; j < tci2.havecardcnt ; j++ )
			{
				if( tci.card[i] == tci2.card[j] )continue;
				if( tci.pettern[i] == tci2.pettern[j] )continue;
				int point = tci.point[i] + tci2.point[j];

				if( point > TData.Point ){
					TData.Card[1] = tci.card[i];
					TData.Pattern[1]=tci.pettern[i];
					TData.Card[0] = tci2.card[j];
					TData.Pattern[0]=tci2.pettern[j];
					TData.Point = point;
				}
			}
		}
		if( TData.Point > 0 ){
			
			m_MyValue = 6;
			for( int i = 0 ; i < 2 ; i ++ ){
				if( TData.Card[i] > 0 ){
					m_RuleCompleteCard.RealCard[i] = (TData.Card[i]-1) + (13 * TData.Pattern[i]);				
					m_RuleCompleteCard.Card[i] =TData.Card[i];
					m_RuleCompleteCard.m_RuleCompleteCnt++;						
				}
			}			
			return TRUE;
		}		
	}

	return FALSE;
}

//골프 구하기
BOOL CBadugiRule::Made_5()
{
	typedef struct {
		int card[5];
		int point;
		int pettern[5];
		int havenumcnt;		
	}TempCardInfo_5;

	TempCardInfo_5 tci_5[5];
	memset(tci_5,-1,sizeof(tci_5));

	for( int s = 0 ; s < 5 ; s ++ )
	{
		tci_5[s].havenumcnt = 0;
		tci_5[s].point =0;
	}
	
	m_MyValue = 0;

	int IndexAry[5][4]={
		{0,1,2,3},
		{0,1,2,4},
		{0,1,3,4},
		{0,2,3,4},
		{1,2,3,4},
	};

	for( int n = 0 ;  n < 5 ; n ++ )
	{
		for( int m = 0 ; m < 4 ; m ++ )
		{			
			int index = IndexAry[n][m];
			int tc = m_RealSortCard[index] % 13 + 1;
			int tp = m_RealSortCard[index] / 13;			
			
			BOOL bSame = FALSE;
			for( int x = 0 ; x < 4 ; x ++ )
			{
				if( tc > 0 && (tci_5[n].card[x] == tc || tci_5[n].pettern[x] == tp) ){
					bSame = TRUE;
					break;
				}
			}
			
			if( !bSame )
			{
				tci_5[n].card[tci_5[n].havenumcnt] = tc;
				tci_5[n].pettern[tci_5[n].havenumcnt] = tp;
				tci_5[n].point += SEED_NUMBERPOINT - tc;
				tci_5[n].havenumcnt++;
			}		
		}
	}


	for(int i = 0; i < 5; i++) //오름차순.
	{
		for(int j = i; j < 5; j++)
		{
			// A
			if(tci_5[j].point > tci_5[i].point)
			{
				TempCardInfo_5 temp;
				memcpy(&temp , &tci_5[i] ,sizeof(TempCardInfo_5) );
				memcpy(&tci_5[i], &tci_5[j] ,sizeof(TempCardInfo_5) );
				memcpy(&tci_5[j], &temp ,sizeof(TempCardInfo_5) );				
			}		
		}
	}
	
	if( tci_5[0].havenumcnt >= 4 ){

		/*
		if( tci_5[0].point >= THIRD && tci_5[0].point <= GOLF ){
			
			switch(tci_5[0].point) {
			case 46:
				m_MyValue = 1;			
				break;
			case 45:
				m_MyValue = 2;
				break;
			case 44:
				m_MyValue = 3;
				break;		
			}	
		}
		else
		{
			m_MyValue = 4;				
		}
		*/

		int golf[5]={1,2,3,4};	
		
		BOOL bGolf = TRUE; 
		m_MyValue = 1;
		for( int i = 0 ; i < 4 ; i ++ ){
			if( tci_5[0].card[i] != golf[i]){
				bGolf = FALSE;
			}
		}		
		if( bGolf == FALSE )m_MyValue = 2;

		if( m_MyValue == 2 ){
			int sce[5]={1,2,3,5};
			
			BOOL bSce = TRUE; 		
			for(  int i = 0 ; i < 4 ; i ++ ){
				if( tci_5[0].card[i] != sce[i]){
					bSce = FALSE;
				}
			}		
			if( bSce == FALSE )m_MyValue = 3;
		}
		
		
		if( m_MyValue == 3 ){
			int thi[5]={1,2,4,5};
			BOOL bthi = TRUE; 
			
			for( int i = 0 ; i < 4 ; i ++ ){
				if( tci_5[0].card[i] != thi[i]){
					bthi = FALSE;
				}
			}		
			if( bthi == FALSE )m_MyValue = 4;
		}		
		
		if( m_MyValue > 0 )
		{
			m_RuleCompleteCard.m_RuleCompleteCnt = 0;
			for( int i = 0 ; i < BD_TOTAL_CARD ; i ++ ){
				if( tci_5[0].card[i] > 0 ){
					m_RuleCompleteCard.RealCard[i] = (tci_5[0].card[i]-1) + (13 * tci_5[0].pettern[i]);
					m_RuleCompleteCard.Card[i] = tci_5[0].card[i];
					m_RuleCompleteCard.m_RuleCompleteCnt++;
				}			
			}			
			
			return TRUE;
		} 		
	}
	
	return FALSE;
}

void CBadugiRule::Top()							//탑 구하기
{
	m_MyValue = 0;
	int cnt = 0 ;
	for(int i = 1 ; i < m_nTotalCard ; i++){		
		if(m_Card[i] > -1){
			m_RuleCompleteCard.Card[cnt] = m_RealSortCard[i] % 13 + 1;
			m_RuleCompleteCard.RealCard[cnt] = m_RealSortCard[i];
			cnt++;
		}
	}

	m_RuleCompleteCard.numberpoint = 0;	
}

CString CBadugiRule::GetNumString(int num)
{
	CString str="";
	if( num <= 0 || num >=14 )return str;
	char valuename[14][6]={
		{""},
		{"A"},
		{"2"},
		{"3"},
		{"4"},
		{"5"},
		{"6"},
		{"7"},
		{"8"},
		{"9"},
		{"10"},		
		{"J"},
		{"Q"},
		{"K"},
	};
	
	str.Format("%s",valuename[num]);
	return str;
}

CString CBadugiRule::GetValueName()			//내 족보 이름 구하기
{
	CString str="";
	if ( m_MyValue < 0 ) return str;	
	
	if (m_MyValue >= 1 &&  m_MyValue <= 4 )
	{	
		switch(m_MyValue) 
		{
		case 1:
			{
				str = "골프";
			} break;
		case 2:
			{
				str = "세컨드";
				break;
			}
		case 3:
			{
				str = "써드";
				break;
			}

		case 4:			
			{
				str = GetNumString( m_RuleCompleteCard.Card[ 3 ] );		
				str += "메이드";
			} break;
		default:
			{
				break;
			}
		}			
	}
	else if ( m_MyValue == 5 )
	{
		str = GetNumString(GetMyValueTopNum());
		str += "베이스";
	}
	else if ( m_MyValue == 6 )
	{		
		str = GetNumString(GetMyValueTopNum());
		str += "투베이스";
	}
	else
	{
		/*
		str = "[";
		str += GetNumString(m_RealSortCard[0]%13 + 1);
		str += GetNumString(m_RealSortCard[1]%13 + 1);	
		str += GetNumString(m_RealSortCard[2]%13 + 1);
		str += GetNumString(m_RealSortCard[3]%13 + 1);
		str += "]";
		*/
		str = "무탑";
	}
	
	return str;
}

CString CBadugiRule::GetValueNumString()			//내 족보 이름 구하기
{
	CString str="";
	if ( m_MyValue < 0 )return str;	
	
	if (m_MyValue >= 1 &&  m_MyValue <= 4 )
	{	
		switch(m_MyValue) {
		case 1:			
		case 2:			
		case 3:			
		case 4:
			str = "[";
			str += GetNumString(m_RuleCompleteCard.Card[0]);		
			str += ",";
			str += GetNumString(m_RuleCompleteCard.Card[1]);		
			str += ",";
			str += GetNumString(m_RuleCompleteCard.Card[2]);		
			str += ",";
			str += GetNumString(m_RuleCompleteCard.Card[3]);		
			str += "]";			
			break;		
		}			
	}
	else if ( m_MyValue == 5 )
	{
		str = "[";		
		str += GetNumString(m_RuleCompleteCard.Card[0]);
		str += ",";
		str += GetNumString(m_RuleCompleteCard.Card[1]);
		str += ",";
		str += GetNumString(m_RuleCompleteCard.Card[2]);		
		str += "]";		
	}
	else if ( m_MyValue == 6 )
	{
		str = "[";		
		str += GetNumString(m_RuleCompleteCard.Card[0]);
		str += ",";
		str += GetNumString(m_RuleCompleteCard.Card[1]);
		str += "]";		
	}
	else{
		str = "[";
		str += GetNumString(m_RealSortCard[0]%13 + 1);
		str += GetNumString(m_RealSortCard[1]%13 + 1);	
		str += GetNumString(m_RealSortCard[2]%13 + 1);
		str += GetNumString(m_RealSortCard[3]%13 + 1);
		str += "]";
	}
	
	//m_strMyValueName = str;
	return str;
}
CString CBadugiRule::GetFullValueName()
{
	m_strMyValueName = GetValueNumString() + GetValueName();
	return m_strMyValueName;
}
//내 족보에 제일 높은 숫자를 리턴한다.
int	 CBadugiRule::GetMyValueTopNum()
{
	int resultnum = 0;
	if ( m_MyValue == 4 )
	{	
		return resultnum = m_RuleCompleteCard.Card[3];		
	}
	else if ( m_MyValue == 5 )
	{
		return resultnum = m_RuleCompleteCard.Card[2];
	}
	else if( m_MyValue == 6)
	{
		return resultnum = m_RuleCompleteCard.Card[1];
	}

	return 0;
}

void CBadugiRule::Result_Sort()
{

	int cnt = 0 ;
	for( int i = 0 ; i < 5 ; i++ )
	{
		if( m_RuleCompleteCard.RealCard[i] > -1)
		{
			cnt++;
		}
	}

	m_RuleCompleteCard.m_RuleCompleteCnt = cnt;

	for( int i = 0; i < m_RuleCompleteCard.m_RuleCompleteCnt; i++) //오름차순.
	{
		for(int j = i; j < m_RuleCompleteCard.m_RuleCompleteCnt; j++)
		{
			// A
			if((m_RuleCompleteCard.RealCard[i]%13) > (m_RuleCompleteCard.RealCard[j]%13))
			{
				int temp  = m_RuleCompleteCard.RealCard[i];
				int temp1 = m_RuleCompleteCard.Card[i];

				m_RuleCompleteCard.RealCard[i] = m_RuleCompleteCard.RealCard[j];
				m_RuleCompleteCard.Card[i] = m_RuleCompleteCard.Card[j];				

				m_RuleCompleteCard.RealCard[j] = temp;
				m_RuleCompleteCard.Card[j] = temp1;				
			}
			else if((m_RuleCompleteCard.RealCard[i]%13)==(m_RuleCompleteCard.RealCard[j]%13))
			{
				if(m_RuleCompleteCard.RealCard[i] > m_RuleCompleteCard.RealCard[j])
				{
					int temp  = m_RuleCompleteCard.RealCard[i];
					int temp1 = m_RuleCompleteCard.Card[i];
					
					m_RuleCompleteCard.RealCard[i] = m_RuleCompleteCard.RealCard[j];
						m_RuleCompleteCard.Card[i] = m_RuleCompleteCard.Card[j];				
					
					m_RuleCompleteCard.RealCard[j] = temp;
					m_RuleCompleteCard.Card[j] = temp1;	
				}
			}
		}
	}
	int temp[5]={0,14,140,1400};
	for( int i = 0 ; i < m_RuleCompleteCard.m_RuleCompleteCnt ; i ++ ){
		if( m_RuleCompleteCard.Card[i] > 0 ){			
			m_RuleCompleteCard.numberpoint += (temp[i] * (SEED_NUMBERPOINT - m_RuleCompleteCard.Card[i]) + (SEED_NUMBERPOINT - m_RuleCompleteCard.Card[i]));
		}			
	}
	switch(m_MyValue)
	{
	case 1:
	case 2:
	case 3:
	case 4:
		m_RuleCompleteCard.numberpoint += 30000;	
		break;
	case 5:
		m_RuleCompleteCard.numberpoint += 20000;	
		break;
	case 6:
		m_RuleCompleteCard.numberpoint += 10000;	
		break;	
	}	
}

