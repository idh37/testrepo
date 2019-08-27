#include "stdafx.h"
#include "OtherUserJokboCheckManHighLow.h"
#include "../PlayerHighLow.h"
#include "../GlobalGame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

struct  MOUNT
{
	int CardNo[5];

	void Clear()
	{
		CardNo[0] = 14;
		CardNo[1] = 13;
		CardNo[2] = 12;
		CardNo[3] = 11;
		CardNo[4] = 10;
	}

	bool IsSameNumber( int number )
	{
		for( int i = 0 ; i < 5 ; i ++ )
		{
			if( CardNo[i] == number )
			{
				return true;
			}
		}
		return false;
	}
};

int POSSIBLE_RULE_USER::get_TotalCardNum(POSSIBLE_VALUE ps_value)
{
	if( ps_value <= en_Value_None || ps_value > en_Value_Rst_Flush )return 0;
	return m_listPossibleCard[ps_value].get_TotalCardNum();
}

void POSSIBLE_RULE_USER::set_Possible_CardInfo(POSSIBLE_VALUE ps_value , int total , int card[] )
{
	if( ps_value <= en_Value_None || ps_value > en_Value_Rst_Flush )return;

	m_listPossibleCard[ps_value].set_Possible_CardInfo(total,card);
}

int	*POSSIBLE_RULE_USER::get_PossibleCardNum(POSSIBLE_VALUE ps_value)
{
	if( ps_value <= en_Value_None || ps_value > en_Value_Rst_Flush )return NULL;
	return m_listPossibleCard[ps_value].get_PossibleCardNum();
}

int POSSIBLE_RULE_USER::get_PossibleCardNum(POSSIBLE_VALUE ps_value , int index )
{
	if( ps_value <= en_Value_None || ps_value > en_Value_Rst_Flush )return -1;
	if( index < 0 || index >= get_TotalCardNum(ps_value) )return -1;
	return m_listPossibleCard[ps_value].get_PossibleCardNum(index);
}

//같은 카드가 잇는지 
bool POSSIBLE_RULE_USER::IsSameCard( POSSIBLE_VALUE ps_value , int CardNo )
{
	if( ps_value <= en_Value_None || ps_value > en_Value_Rst_Flush )return false;
	if( CardNo < 0 || CardNo > 51 )return false;

	return m_listPossibleCard[ps_value].IsSameCard(CardNo);
}

POSSIBLE_CARD_COLOR POSSIBLE_RULE_USER::IsMade( POSSIBLE_VALUE ps_value ){
	int total = get_TotalCardNum(ps_value);

	int Color = 0;
	switch(ps_value)
	{
	case en_Value_One:
		{
			return total == 2 ? en_Value_Color_Red : en_Value_Color_None;
		}
		break;
	case en_Value_Two:
		{
			return total == 4 ? en_Value_Color_Red : en_Value_Color_None;
		}
		break;
	case en_Value_Three:
		{
			return total == 3 ? en_Value_Color_Red : en_Value_Color_None;
		}
		break;
	case en_Value_Four:
		{
			return total == 4 ? en_Value_Color_Red : en_Value_Color_None;
		}
		break;			
	case en_Value_Straight:
		{	
			//검사를 한번 한다. 5장이 연속된 숫자 인지..
			if( total == 5 ){
				int number_1 = -1;
				int number_2 = -1;
				for( int i = 0 ; i < total ; i ++ ){
					number_1 = m_listPossibleCard[en_Value_Straight].m_CardInfo.m_listNumBer[i] + 1;
					number_2 = m_listPossibleCard[en_Value_Straight].m_CardInfo.m_listNumBer[i+1];
					if( number_1 == -1 || number_2 == -1 )continue;
					if( number_1 != number_2 ){
						return en_Value_Color_Green;
					}
				}
				return en_Value_Color_Red;
			}
			else{
				return total >= 3 ? en_Value_Color_Green : en_Value_Color_None;
			}

		}
		break;
	case en_Value_House:
		{
			return total == 5 ? en_Value_Color_Red : en_Value_Color_None;
		}
		break;
	case en_Value_Flush:
		{
			return total == 5 ? en_Value_Color_Red :  total >= 3 ? en_Value_Color_Blue : en_Value_Color_None;
		}
		break;
	case en_Value_St_Flush:
		{
			return total == 5 ? en_Value_Color_Red :  total >= 3 ? en_Value_Color_Blue : en_Value_Color_None;
		}
		break;
	case en_Value_Rst_Flush:
		{
			return total == 5 ? en_Value_Color_Red : total >= 3 ? en_Value_Color_Blue : en_Value_Color_None ;
		}
		break;
	}
	return en_Value_Color_None;
}

bool POSSIBLE_RULE_USER::get_MadeCardInfo( POSSIBLE_VALUE &ps_value , POSSIBLE_CARD_COLOR &value_color)
{
	int i = en_Value_One; 
	POSSIBLE_CARD_COLOR hivalue_color = en_Value_Color_None;
	int	hivalue_Cardtotal = 0;
	POSSIBLE_VALUE hi_value = en_Value_None;
	for( i = 0 ; i < MAX_POSSIBLE_RULE ; i ++ )
	{
		POSSIBLE_CARD_COLOR compare_color = IsMade((POSSIBLE_VALUE)i);
		int compare_cardtotal = get_TotalCardNum((POSSIBLE_VALUE)i);

		if( compare_color == en_Value_Color_Red )
		{
			value_color = compare_color;
			ps_value = (POSSIBLE_VALUE)i;
			return true;
		}

		if( compare_color >= hivalue_color && compare_cardtotal >= hivalue_Cardtotal ){
			hivalue_color = compare_color;
			hivalue_Cardtotal = compare_cardtotal ;
			hi_value = (POSSIBLE_VALUE)i;
		}
	}

	if( hivalue_color > en_Value_Color_Red && hivalue_Cardtotal >= 3 ){

		value_color = hivalue_color;
		ps_value = hi_value;	 

		return true;
	}

	return false;
}

COtherUserJokboCheckManHighLow::COtherUserJokboCheckManHighLow()
{
	Clear();
	m_bUse = false;
}

COtherUserJokboCheckManHighLow::~COtherUserJokboCheckManHighLow()
{
}

bool COtherUserJokboCheckManHighLow::get_Pair( int pnum )
{
	if( pnum <= 0 || 
		pnum >= MAX_POSSIBLE_PLAYER || 
		m_Other_User[pnum].m_cBaseCardInfo.m_nTotalCardCount <= 1 )
	{
		if( pnum > 0 && pnum < MAX_POSSIBLE_PLAYER ){
			m_Other_User[pnum].m_listPossibleCard[en_Value_One].Clear();
		}		
		return false;
	}

	int opencardnum = m_Other_User[pnum].m_cBaseCardInfo.m_nTotalCardCount;

	// 한장 씩 검사 
	int i = 0 ;
	int j = 0 ;
	COMPARE_CARD Temp;
	Temp.Clear();

	for( i = 0 ; i < opencardnum ; i ++ ){
		int No_1 = m_Other_User[pnum].m_cBaseCardInfo.get_Number(i);
		for( j = i ; j < opencardnum ; j ++ ){
			if( i == j )continue;
			int No_2 = m_Other_User[pnum].m_cBaseCardInfo.get_Number(j);

			if( No_1 == No_2 ){
				Temp.set_BaseCardNo( m_Other_User[pnum].m_cBaseCardInfo.get_BaseCardNo(i) );
				Temp.set_BaseCardNo( m_Other_User[pnum].m_cBaseCardInfo.get_BaseCardNo(j) );				
				break;
			}
		}
	}

	if( Temp.m_nTotal == 2 ){
		m_Other_User[pnum].set_Possible_CardInfo(en_Value_One,Temp.m_nTotal,Temp.m_listCardNo);
	}

	if( Temp.m_nTotal == 4 ){

		int No_1 = Temp.m_listCardNo[0];
		int No_2 = Temp.m_listCardNo[2];
		No_1 = No_1 -  (ONEPATTERN_TOTAL*(No_1/ONEPATTERN_TOTAL))+2;
		No_2 = No_2 -  (ONEPATTERN_TOTAL*(No_2/ONEPATTERN_TOTAL))+2;

		//포카드인지 검사
		if( No_1 == No_2 ){
			m_Other_User[pnum].set_Possible_CardInfo(en_Value_Four,Temp.m_nTotal,Temp.m_listCardNo);
		}
		else{
			m_Other_User[pnum].set_Possible_CardInfo(en_Value_Two,Temp.m_nTotal,Temp.m_listCardNo);
		}
	}

	if( Temp.m_nTotal == 3 ){
		m_Other_User[pnum].set_Possible_CardInfo(en_Value_Three,Temp.m_nTotal,Temp.m_listCardNo);
	}

	if( Temp.m_nTotal == 5 ){
		m_Other_User[pnum].set_Possible_CardInfo(en_Value_House,Temp.m_nTotal,Temp.m_listCardNo);
	}
	return true;
}

bool COtherUserJokboCheckManHighLow::get_One( int pnum )
{
	if( pnum <= 0 || 
		pnum >= MAX_POSSIBLE_PLAYER || 
		m_Other_User[pnum].m_cBaseCardInfo.m_nTotalCardCount <= 1 )
	{
		if( pnum > 0 && pnum < MAX_POSSIBLE_PLAYER ){
			m_Other_User[pnum].m_listPossibleCard[en_Value_One].Clear();
		}		
		return false;
	}

	int opencardnum = m_Other_User[pnum].m_cBaseCardInfo.m_nTotalCardCount;

	// 한장 씩 검사 
	int i = 0 ;
	int j = 0 ;
	COMPARE_CARD Temp;
	Temp.Clear();

	for( i = 0 ; i < opencardnum ; i ++ ){
		int No_1 = m_Other_User[pnum].m_cBaseCardInfo.get_Number(i);
		for( j = i ; j < opencardnum ; j ++ ){
			if( i == j )continue;
			int No_2 = m_Other_User[pnum].m_cBaseCardInfo.get_Number(j);

			if( No_1 == No_2 ){
				Temp.set_BaseCardNo( m_Other_User[pnum].m_cBaseCardInfo.get_BaseCardNo(i) );
				Temp.set_BaseCardNo( m_Other_User[pnum].m_cBaseCardInfo.get_BaseCardNo(j) );				
				break;
			}
		}
	}

	if( Temp.m_nTotal >= 2 ){
		m_Other_User[pnum].set_Possible_CardInfo(en_Value_One,Temp.m_nTotal,Temp.m_listCardNo);
	}

	return true;
}

bool COtherUserJokboCheckManHighLow::get_Two( int pnum )
{
	if( pnum <= 0 || 
		pnum >= MAX_POSSIBLE_PLAYER || 
		m_Other_User[pnum].m_cBaseCardInfo.m_nTotalCardCount <= 3 )
	{
		if( pnum > 0 && pnum < MAX_POSSIBLE_PLAYER ){
			m_Other_User[pnum].m_listPossibleCard[en_Value_Two].Clear();
		}		
		return false;
	}

	int opencardnum = m_Other_User[pnum].m_cBaseCardInfo.m_nTotalCardCount;

	// 한장 씩 검사
	int i = 0;
	int j = 0;	

	COMPARE_CARD Temp;
	Temp.Clear();

	for( i = 0 ; i < opencardnum ; i ++ ){
		int No_1 = m_Other_User[pnum].m_cBaseCardInfo.get_Number(i);
		for( j = i ; j < opencardnum ; j ++ ){
			if( i == j )continue;
			int No_2 = m_Other_User[pnum].m_cBaseCardInfo.get_Number(j);

			if( No_1 == No_2 ){
				Temp.set_BaseCardNo( m_Other_User[pnum].m_cBaseCardInfo.get_BaseCardNo(i) );
				Temp.set_BaseCardNo( m_Other_User[pnum].m_cBaseCardInfo.get_BaseCardNo(j) );			
				break;
			}
		}
	}

	if( Temp.m_nTotal == 4 ){		
		m_Other_User[pnum].set_Possible_CardInfo(en_Value_Two,Temp.m_nTotal,Temp.m_listCardNo);
	}

	return false;
}

bool COtherUserJokboCheckManHighLow::get_Three( int pnum )
{
	if( pnum <= 0 || 
		pnum >= MAX_POSSIBLE_PLAYER || 
		m_Other_User[pnum].m_cBaseCardInfo.m_nTotalCardCount <= 2 )
	{
		if( pnum > 0 && pnum < MAX_POSSIBLE_PLAYER ){
			m_Other_User[pnum].m_listPossibleCard[en_Value_Three].Clear();
		}		
		return false;
	}

	int opencardnum = m_Other_User[pnum].m_cBaseCardInfo.m_nTotalCardCount;

	// 한장 씩 검사
	int i = 0;
	int j = 0;

	COMPARE_CARD Temp;
	Temp.Clear();

	for( i = 0 ; i < opencardnum ; i ++ ){
		int No_1 = m_Other_User[pnum].m_cBaseCardInfo.get_Number(i);
		for( j = i ; j < opencardnum ; j ++ ){
			if( i == j )continue;
			int No_2 = m_Other_User[pnum].m_cBaseCardInfo.get_Number(j);

			if( No_1 == No_2 ){
				Temp.set_BaseCardNo( m_Other_User[pnum].m_cBaseCardInfo.get_BaseCardNo(i) );
				Temp.set_BaseCardNo( m_Other_User[pnum].m_cBaseCardInfo.get_BaseCardNo(j) );			
				break;
			}
		}
	}

	if( Temp.m_nTotal == 3 ){		
		m_Other_User[pnum].set_Possible_CardInfo(en_Value_Three,Temp.m_nTotal,Temp.m_listCardNo);
	}	

	return true;
}

bool COtherUserJokboCheckManHighLow::get_Straight( int pnum )
{
	if( pnum <= 0 || 
		pnum >= MAX_POSSIBLE_PLAYER || 
		m_Other_User[pnum].m_cBaseCardInfo.m_nTotalCardCount <= 2 )
	{
		if( pnum > 0 && pnum < MAX_POSSIBLE_PLAYER ){
			m_Other_User[pnum].m_listPossibleCard[en_Value_Straight].Clear();
		}		
		return false;
	}

	int opencardnum = m_Other_User[pnum].m_cBaseCardInfo.m_nTotalCardCount;

	// 한장 씩 검사
	int i = 0;
	int j = 0;		
	int Complete_Card[MAX_TOTAL_CARDNUM];
	int Complete_CardCnt = 0;	
	memset(Complete_Card,-1,sizeof(Complete_Card));		

	COMPARE_CARD Temp_UniNo,Temp_Staight;
	Temp_UniNo.Clear();
	Temp_Staight.Clear();

	for( i = 0 ; i < opencardnum ; i ++){
		//같은 수가 있는지 체크
		Temp_UniNo.set_UniCardNo(m_Other_User[pnum].m_cBaseCardInfo.m_listNumBer[i],m_Other_User[pnum].m_cBaseCardInfo.m_listPattern[i]);
	}

	Temp_UniNo.ADD_Ace_Number();

	//카드 3장 체크 시 스트레이트를 이룰 수 있는 범위 내 최소
	//숫자와 최대 숫자를 체크하고 그 사이에 3번째 카드가 있는 경우 노출함

	//제일 높은 걸 넣는다.
	COMPARE_CARD Temp_HiCase[10];
	int nHiCaseCnt = 0;
	for( i = 0 ; i < 10 ; i ++ ){
		Temp_HiCase[i].Clear();
	}

	int nGapCnt = 0;
	for( i = 0 ; i < Temp_UniNo.m_nTotal ; i ++ ){
		Temp_HiCase[nHiCaseCnt].set_StraightCardNumber(Temp_UniNo.m_listCardNo[i],Temp_UniNo.m_listCardPa[i]);
		nGapCnt = 0;
		for( j = i+1 ; j < Temp_UniNo.m_nTotal ; j++ ){
			int nCompare = Temp_HiCase[nHiCaseCnt].get_LastCardNo();
			if( nCompare != -1 ){
				nCompare = Temp_UniNo.m_listCardNo[j] - nCompare;
			}

			if( nCompare >= 1 && nCompare <= 2 ){						
				if( nCompare == 2 ){
					nGapCnt ++;
				}
				//두번째 중간 값을 요구 하는 숫자면 무시
				if( nGapCnt <= 1 ){
					Temp_HiCase[nHiCaseCnt].set_StraightCardNumber(Temp_UniNo.m_listCardNo[j],Temp_UniNo.m_listCardPa[j]);
				}
			}					
		}

		if( Temp_HiCase[nHiCaseCnt].m_nTotal >= 3 ){
			nHiCaseCnt++;
		}
		else{
			Temp_HiCase[nHiCaseCnt].Clear();
		}
	}

	if( nHiCaseCnt >= 1 ){

		int nHiNo = -1;
		int nHiIndex = -1;
		for ( int x = 0 ; x < nHiCaseCnt ; x++ )
		{
			if( nHiNo == -1 ){
				nHiNo = Temp_HiCase[x].get_LastCardNo();
				nHiIndex = x;
			}
			else{
				int tempno = Temp_HiCase[x].get_LastCardNo();
				//제일 앞에 A 가 있을 지 몰라 
				if( tempno <= 5 ){
					if( Temp_HiCase[x].m_listCardNo[0] == 1 ){
						tempno = 14;
					}
				}	
				//뒤로 갈수록 높은 숫자당.
				if( tempno > nHiNo ){						
					nHiNo = tempno;
					nHiIndex = x;
				}
				else if( tempno == nHiNo ){
					if( Temp_HiCase[x].m_nTotal > Temp_HiCase[nHiIndex].m_nTotal ){
						nHiNo = tempno;
						nHiIndex = x;
					}
				}
			}
		}

		if( nHiIndex > -1 ){
			Temp_Staight.Clear();
			Temp_Staight = Temp_HiCase[nHiIndex];
		}		

		for( i = 0 ; i < Temp_Staight.m_nTotal ; i++ ){
			Complete_Card[i] = Temp_Staight.get_CardNumber(i);
		}
		m_Other_User[pnum].set_Possible_CardInfo(en_Value_Straight,Temp_Staight.m_nTotal,Complete_Card);
	}

	return true;
}


bool COtherUserJokboCheckManHighLow::get_Flush( int pnum )
{
	if( pnum <= 0 || 
		pnum >= MAX_POSSIBLE_PLAYER || 
		m_Other_User[pnum].m_cBaseCardInfo.m_nTotalCardCount <= 2 )
	{
		if( pnum > 0 && pnum < MAX_POSSIBLE_PLAYER ){
			m_Other_User[pnum].m_listPossibleCard[en_Value_Flush].Clear();
		}		
		return false;
	}

	int opencardnum = m_Other_User[pnum].m_cBaseCardInfo.m_nTotalCardCount;

	// 한장 씩 검사
	int i = 0;
	int j = 0;	

	COMPARE_CARD Temp;
	Temp.Clear();

	for( i = 0 ; i < opencardnum ; i ++ ){
		int Pa_1 = m_Other_User[pnum].m_cBaseCardInfo.m_listPattern[ i ];
		for( j = 0 ; j < opencardnum ; j ++ ){
			if( i == j )continue;
			int Pa_2 = m_Other_User[pnum].m_cBaseCardInfo.m_listPattern[ j ];
			if( Pa_1 == Pa_2 ){
				int base_number = ( Pa_1 * ONEPATTERN_TOTAL ) + m_Other_User[pnum].m_cBaseCardInfo.m_listNumBer[i] - 2;
				Temp.set_BaseCardNo(base_number);
				base_number = ( Pa_2 * ONEPATTERN_TOTAL ) + m_Other_User[pnum].m_cBaseCardInfo.m_listNumBer[j] - 2;
				Temp.set_BaseCardNo(base_number);
			}
		}

		if( Temp.m_nTotal >= 3 ){
			break;
		}
		else{
			Temp.Clear();
		}
	}

	if( Temp.m_nTotal >= 3 ){		
		m_Other_User[pnum].set_Possible_CardInfo(en_Value_Flush,Temp.m_nTotal,Temp.m_listCardNo);
	}

	return true;
}

bool COtherUserJokboCheckManHighLow::get_House( int pnum )
{
	if( pnum <= 0 || 
		pnum >= MAX_POSSIBLE_PLAYER || 
		m_Other_User[pnum].m_cBaseCardInfo.m_nTotalCardCount <= 4 )
	{
		if( pnum > 0 && pnum < MAX_POSSIBLE_PLAYER ){
			m_Other_User[pnum].m_listPossibleCard[en_Value_House].Clear();
		}		
		return false;
	}

	int opencardnum = m_Other_User[pnum].m_cBaseCardInfo.m_nTotalCardCount;

	// 한장 씩 검사
	int i = 0;
	int j = 0;	

	COMPARE_CARD Temp;
	Temp.Clear();

	for( i = 0 ; i < opencardnum ; i ++ ){
		int No_1 = m_Other_User[pnum].m_cBaseCardInfo.get_Number(i);
		for( j = i ; j < opencardnum ; j ++ ){
			if( i == j )continue;
			int No_2 = m_Other_User[pnum].m_cBaseCardInfo.get_Number(j);

			if( No_1 == No_2 ){
				Temp.set_BaseCardNo( m_Other_User[pnum].m_cBaseCardInfo.get_BaseCardNo(i) );
				Temp.set_BaseCardNo( m_Other_User[pnum].m_cBaseCardInfo.get_BaseCardNo(j) );			
				break;
			}
		}
	}

	if( Temp.m_nTotal == 4 ){		
		m_Other_User[pnum].set_Possible_CardInfo(en_Value_House,Temp.m_nTotal,Temp.m_listCardNo);
	}

	return false;
}

bool COtherUserJokboCheckManHighLow::get_Four( int pnum )
{
	if( pnum <= 0 || 
		pnum >= MAX_POSSIBLE_PLAYER || 
		m_Other_User[pnum].m_cBaseCardInfo.m_nTotalCardCount <= 3 )
	{
		if( pnum > 0 && pnum < MAX_POSSIBLE_PLAYER ){
			m_Other_User[pnum].m_listPossibleCard[en_Value_Four].Clear();
		}		
		return false;
	}

	int opencardnum = m_Other_User[pnum].m_cBaseCardInfo.m_nTotalCardCount;


	// 한장 씩 검사
	int i = 0;
	int j = 0;	
	int m_listNumBer[MAX_TOTAL_CARDNUM];
	int m_listPattern[MAX_TOTAL_CARDNUM];
	int Complete_Card[MAX_TOTAL_CARDNUM];
	int Complete_CardCnt = 0;

	memset( Complete_Card,-1,sizeof(Complete_Card) );
	memcpy( m_listNumBer , m_Other_User[pnum].m_cBaseCardInfo.m_listNumBer ,sizeof(m_listNumBer));
	memcpy( m_listPattern , m_Other_User[pnum].m_cBaseCardInfo.m_listPattern ,sizeof(m_listPattern));

	for( i = 0 ; i < opencardnum ; i ++){

		int cardno_1 = m_listNumBer[i];
		Complete_CardCnt = 0;
		Complete_Card[Complete_CardCnt++] = ( m_listPattern[i] * ONEPATTERN_TOTAL ) + m_listNumBer[i] - 2;

		for( j = 0 ; j < opencardnum ; j ++ ){
			if( i == j )continue;
			int cardno_2 = m_listNumBer[j];

			if( cardno_1 == cardno_2 ){
				Complete_Card[Complete_CardCnt++] = ( m_listPattern[j] * ONEPATTERN_TOTAL ) + m_listNumBer[j] - 2;
			}
		}

		if( Complete_CardCnt != 4 ){
			memset(Complete_Card,-1,sizeof(Complete_Card));
			Complete_CardCnt = 0;
		}
		else{
			break;
		}
	}

	if( Complete_CardCnt == 4 ){
		m_Other_User[pnum].set_Possible_CardInfo(en_Value_Four,Complete_CardCnt,Complete_Card);
	}
	return true;
}

bool COtherUserJokboCheckManHighLow::get_St_Flush( int pnum )
{
	if( pnum <= 0 || 
		pnum >= MAX_POSSIBLE_PLAYER || 
		m_Other_User[pnum].m_cBaseCardInfo.m_nTotalCardCount <= 2 )
	{
		if( pnum > 0 && pnum < MAX_POSSIBLE_PLAYER ){
			m_Other_User[pnum].m_listPossibleCard[en_Value_St_Flush].Clear();
		}		
		return false;
	}


	// get_Straight 에서 스트레이트를 검사 했을 것이다. 
	// 스트레이트가 아닌 경우는 스티플을 검사 할 필요가 없다.
	POSSIBLE_CARD_COLOR value_color = m_Other_User[pnum].IsMade(en_Value_Straight);

	if( value_color == en_Value_Color_None )return false;

	int m_nTotal = m_Other_User[pnum].m_listPossibleCard[en_Value_Straight].get_TotalCardNum();
	if( m_nTotal < 3 )return false;

	int i = 0; 
	int j = 0;
	int x = 0;	

	int BasePattern = m_Other_User[pnum].m_listPossibleCard[en_Value_Straight].m_CardInfo.get_Pattern(0);
	if( BasePattern < 0 )return false;

	//스트레이트의 최상위 숫자들이 무늬가 다 같은지를 검사 한다.
	bool bAllSamePattern = true;
	for ( i = 0 ; i < m_nTotal ; i ++ )
	{
		int Compare_Pattern = m_Other_User[pnum].m_listPossibleCard[en_Value_Straight].m_CardInfo.get_Pattern(i);
		if( BasePattern != Compare_Pattern ){
			bAllSamePattern = false;
			break;
		}		
	}

	if( bAllSamePattern == false )return false;

	m_Other_User[pnum].set_Possible_CardInfo(en_Value_St_Flush,m_nTotal,m_Other_User[pnum].m_listPossibleCard[en_Value_Straight].m_CardInfo.m_listPossibleCard);

	return true;
}

bool COtherUserJokboCheckManHighLow::get_Rst_Flush( int pnum )
{
	if( pnum <= 0 || 
		pnum >= MAX_POSSIBLE_PLAYER || 
		m_Other_User[pnum].m_cBaseCardInfo.m_nTotalCardCount <= 2 )
	{
		if( pnum > 0 && pnum < MAX_POSSIBLE_PLAYER ){
			m_Other_User[pnum].m_listPossibleCard[en_Value_Rst_Flush].Clear();
		}		
		return false;
	}

	int opencardnum = m_Other_User[pnum].m_cBaseCardInfo.m_nTotalCardCount;

	int Complete_Card[MAX_TOTAL_CARDNUM];
	int Complete_CardCnt = 0;

	memset( Complete_Card,-1,sizeof(Complete_Card) );

	// 한장 씩 검사
	int i = 0;
	int j = 0;
	int x = 0;	

	MOUNT Compare_Mount;
	Compare_Mount.Clear();


	// get_Sf_Flush 에서 스트레이트를 검사 했을 것이다. 마운틴까지 검사 함
	// 스트레이트가 아닌 경우는 노티플을 검사 할 필요가 없다.
	POSSIBLE_CARD_COLOR value_color = m_Other_User[pnum].IsMade(en_Value_St_Flush);

	if( value_color == en_Value_Color_None )return false;

	int m_nTotal = m_Other_User[pnum].m_listPossibleCard[en_Value_St_Flush].get_TotalCardNum();
	if( m_nTotal < 3 )return false;

	COMPARE_CARD Temp_Rsf;
	Temp_Rsf.Clear();
	//스티에서 노티에 해당 되는 숫자만 가지고 온다.
	//스티플에 들어가 있는 숫자 중 마운틴에 해당 되는 숫자가 있는지 검사 하여 걸러 낸다.
	for( i = 0 ; i < m_nTotal ; i ++ ){
		int number = m_Other_User[pnum].m_listPossibleCard[en_Value_St_Flush].m_CardInfo.get_Number(i);
		int pattern = m_Other_User[pnum].m_listPossibleCard[en_Value_St_Flush].m_CardInfo.get_Pattern(i);
		if(Compare_Mount.IsSameNumber(number)){
			Temp_Rsf.set_UniCardNo(number,pattern);
		}
	}

	if( Temp_Rsf.m_nTotal < 3 )return false;

	for( i = 0 ; i < Temp_Rsf.m_nTotal ; i ++ ){
		Complete_Card[i] = Temp_Rsf.get_CardNumber(i);
	}

	m_Other_User[pnum].set_Possible_CardInfo(en_Value_Rst_Flush,Temp_Rsf.m_nTotal,Complete_Card);	

	return true;
}

void COtherUserJokboCheckManHighLow::Init(NMBASE::GRAPHICGDI::xSprite *pArrowSpr)
{
	COtherUserJokboCheckMan::Init(pArrowSpr);
	m_bUse = true;
}

void COtherUserJokboCheckManHighLow::Clear(void)
{
	m_Other_User[0].Clear();
	m_Other_User[1].Clear();
	m_Other_User[2].Clear();
	m_Other_User[3].Clear();
	m_Other_User[4].Clear();
}

void COtherUserJokboCheckManHighLow::Draw(CDC *pDC , NMBASE::GRAPHICGDI::CPage *pPage)
{
	if( pPage == NULL )return;
	if( m_pArrowSpr == NULL )return;
	int i = 0; 
	int j = 0;

	const int start_display_arrow = 2;	//3장 부터 화살표 표시
	int max_display_arrow = 7;			//7장 카드 (세븐포커)
	CPoint ptArrowPos;

	//자기 자신 빼고 카드 정보(오픈된 카드) 다 넣음
	CPlayerHighLow *pPlayer = NULL;
	for( i = 1 ; i < MAX_POSSIBLE_PLAYER ; i++ )
	{
		pPlayer = g_PLAYER(i);
		if ( pPlayer->PlayState == 1 && pPlayer->JoinState == 1 && pPlayer->bFold == FALSE )
		{
			POSSIBLE_CARD_COLOR hivalue_color = en_Value_Color_None;
			POSSIBLE_VALUE hi_value = en_Value_None;


			if( m_Other_User[i].get_MadeCardInfo( hi_value ,hivalue_color ) )
			{
				//3번째 ~ 6번째 까지 화살표 표시 
				for( j = start_display_arrow ; j < max_display_arrow ; j ++ )
				{
					if( !m_Other_User[i].IsSameCard( hi_value , pPlayer->m_cMyCard[j].GetCardNum() ) )
					{
						continue;
					}
					ptArrowPos = pPlayer->m_cMyCard[j].GetPos() + g_cUIData.m_ptJokboArrowGab;
					pPage->PutSprAuto( ptArrowPos.x , ptArrowPos.y, m_pArrowSpr, hivalue_color);
				}				
			}			
		}
	}
}

void COtherUserJokboCheckManHighLow::CheckJokbo(void)
{
	if( g_ObserverMan.IsMyObserver() || g_PLAYER(0)->JoinState == 0 ){
		Clear();
		return;
	}

	const int nMaxPlayr = g_GAMEPROCESS()->GetMaxPlayer();
	Clear();
	//자기 자신 빼고 카드 정보(오픈된 카드) 다 넣음
	for( int i = 1 ; i < nMaxPlayr ; i++ ){		
		if ( g_PLAYER(i)->PlayState == 1 && g_PLAYER(i)->JoinState == 1 && g_PLAYER(i)->bFold == FALSE )
		{
			int pnum = g_GAMEPROCESS()->GetServPNum_ByPN(i);
			int card_cnt = 0;
			int m_listCard[MAX_TOTAL_CARDNUM];
			memset( m_listCard , -1 , sizeof(m_listCard ) );
			for( int j = 0 ; j < g_Poker.PS[pnum].nOpenTotal ; j++ ){				

				if( j == 4 ){
					if( g_Poker.nBombCard != 100 ){
						m_listCard[card_cnt++] = g_Poker.nBombCard;
					}					
				}
				else{
					if( g_Poker.PS[pnum].nCard[j] < 0 )continue;
					m_listCard[card_cnt++] = g_Poker.PS[pnum].nCard[j];	
				}
			}

			if( card_cnt > 0 )
			{			
				m_Other_User[i].m_cBaseCardInfo.set_CardInfo( card_cnt , m_listCard );
			}		

			get_Pair( i );
			get_Straight( i );
			get_Flush( i );
			get_St_Flush( i );
			get_Rst_Flush( i );				
		}
	}
}