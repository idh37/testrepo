// CardRule.cpp: implementation of the CCardRule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CardRule.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCardRule::CCardRule()
{
	Init();
}

CCardRule::~CCardRule()
{

}

void CCardRule::Init()
{
	ZeroMemory( &m_stRuleData, sizeof(m_stRuleData));
	ZeroMemory( m_sortCard, sizeof(m_sortCard));

	memset( m_RemainCard, -1, sizeof(m_RemainCard));
	memset( m_RuleComplteCard, -1, sizeof(m_RuleComplteCard));
	memset( m_Extra_WinCard, -1, sizeof(m_Extra_WinCard));
	memset( m_AllRuleCard, -1, sizeof(m_AllRuleCard));
	memset( m_RealCard, -1, sizeof(m_RealCard));
	memset( m_EqualCard, -1, sizeof(m_EqualCard));
	memset( m_EqualCard_low, -1, sizeof(m_EqualCard_low));
	
	
	memset( m_RemainCard_Low, -1, sizeof(m_RemainCard_Low));
	memset( m_RuleComplteCard_Low, -1, sizeof(m_RuleComplteCard_Low));
	memset( m_Extra_WinCard_Low, -1, sizeof(m_Extra_WinCard_Low));

	m_nTotalCard = 0;
	m_SevenValue = 0;
	m_SevenValue_low = -1;
	m_SecondSevenValue = 0;
	bValid = FALSE;
}

/* 카드 셋 */
void CCardRule::set_Card(int pCard[], int cardNum)
{
	bValid = TRUE;
	m_nTotalCard = cardNum;
	for(int i=0; i<cardNum; i++)
	{
		m_sortCard[i] =  pCard[i];
		m_RealCard[i] = pCard[i];
		m_sortCard_low[i] = pCard[i];
	}
}

/* 카드 정렬 오름차순 */
void CCardRule::sort_Card(int Start, int End)
{
	for(int i = Start; i < End; i++) //오름차순.
	{
		for(int j = i; j < End; j++)
		{
			// A
			if((m_sortCard[i]%13) > (m_sortCard[j]%13))
			{
				int temp = m_sortCard[i];
				m_sortCard[i] = m_sortCard[j];
				m_sortCard[j] = temp;
			}
			else if((m_sortCard[i]%13)==(m_sortCard[j]%13))
			{
				if(m_sortCard[i] > m_sortCard[j])
				{
					int temp = m_sortCard[i];
					m_sortCard[i] = m_sortCard[j];
					m_sortCard[j] = temp;
				}
			}
		}
	}	
}

/* 스트레이트.. 룰 구하기 */
void CCardRule::cal_straight_class()
{
	int card_i = m_nTotalCard - 1;
	int straight_cnt = 0;
	int preface, nextface, royalindex= -1;
	int pattern_cnt[4] = { 0,};	
	BOOL bBackStraight = FALSE;

	///////////////////////////////////////////////
	///////////////////////////////////////////////
	// Card face 판별....!! for poker, straight !!	
	for(int i=card_i; i > 0; i--)
	{
		//보유카드 중에 에이스카드가 있다면 
		if((m_sortCard[i]%13)==12)			// for Ace    last -> card_i
		{			
			for(int j=0; j<4;j++)
			{
				//2번 카드가 있는지 검사 ,2번카드는 4장 존재하므로 4번만 돌림??
				if( (m_sortCard[j]%13)==0 )	// for 2     first -> j = 0
				{
					straight_cnt=1; 
					preface =  is_Card_pattern( m_sortCard[j] );	//Ace카드 무늬
					nextface = is_Card_pattern( m_sortCard[i] );	//2번카드 무늬

					// pattern_cnt 배열의 인덱스초과검사
					if (preface < 0 || 4<= preface)
					{
						continue;
					}
					if(preface == nextface)		//ACE와 2번카드 무늬가 같으면 
					{
						pattern_cnt[preface]++;		//무늬카운트 증가..
					}
					bBackStraight = TRUE; // backstraight            /
				}
			}
		}
	}


	int	base =0;
	int index = 0;
	bool countnflag = TRUE;
	int m_backstraIndex = -1;
	BOOL bBackStraightFlush = FALSE;
	int tmpface=0;
	
	
	
	bool broyal = false;	// 로티플
	for(int j=1; j <= card_i; j++) // < 7
	{
		if(((m_sortCard[base]%13)+1) == (m_sortCard[j]%13))  // is Straight
		{
			if(countnflag) // 연속된 숫자
			{
				straight_cnt++;
				index = j;
			}
			preface =  is_Card_pattern( m_sortCard[base] );
			nextface = is_Card_pattern( m_sortCard[j] );
			
			// 무뉘가 같다
			if(countnflag && preface == nextface)
			{
				// pattern_cnt 배열의 인덱스초과검사
				if (preface < 0 || 4<= preface)
				{
					continue;
				}
				pattern_cnt[preface] ++;
				if(pattern_cnt[preface]>=4 && !broyal)
					royalindex = j;
			}
			else
			{	
				// 무뉘가 다르지만 pair 카드중 같은 무뉘가 존재할 수 있다.
				if(countnflag)
				{	
					// [j] 다음 카드 기준
					preface =  is_Card_pattern( m_sortCard[j] );
					for(int k=0; k<j; k++)
					{													//	    base (j 기준)
						if(m_sortCard[j]%13 == m_sortCard[k]%13+1)		// (3)   3   (4)   5   6
						{
							nextface = is_Card_pattern( m_sortCard[k] );
							if(preface == nextface)
							{
								// pattern_cnt 배열의 인덱스초과검사
								if (preface < 0 || 4<= preface)
								{
									continue;
								}
								pattern_cnt[preface]++;
								if(pattern_cnt[preface]>=4)
									royalindex = j;
							}
						}
					}
					
					// [base] 기준 (다음카드에 같은무늬가 존재하지 않는다면)
					bool bflush = false;
					preface =  is_Card_pattern( m_sortCard[base] );
					for(int k=j+1; k<= card_i; k++)
					{													//	   base	(base 기준)	
						if(m_sortCard[base]%13+1 == m_sortCard[k]%13)	// 3   (3)   (4)  (4)   5   6
						{
							nextface = is_Card_pattern( m_sortCard[k] );
							if(preface == nextface)
							{
								bflush = true;
							}
						}
					}
					
					if(!bflush)
					{	// 초기화 한다
						tmpface  = is_Card_pattern( m_sortCard[base] );
						// pattern_cnt 배열의 인덱스초과검사
						if (tmpface < 0 || 4<= tmpface)
						{
							continue;
						}
						if(pattern_cnt[tmpface] < 4)
						{
							pattern_cnt[tmpface] = 0;
						}
						else
							broyal = true;
					}
				}
				
			}


			// backstraight, backstrFlush
			if(straight_cnt == 4 && bBackStraight && !bBackStraightFlush)
			{
				//족보버그 수정
				//m_backstraIndex = j;
				
				if((pattern_cnt[0]>=4) || (pattern_cnt[1]>=4) || (pattern_cnt[2]>=4) || (pattern_cnt[3]>=4))
				{
					bBackStraightFlush =  TRUE;
					m_backstraIndex = j;
				}
				else
				{
					// 백스트레이트 이다.
					m_backstraIndex = card_i; //(백줄이라면 중 제일 높은 카드(A)를 대표카드로 보고 SortCard배열중 젤 높은 놈으로 가져온다.  )
				}
			}/*
			

			// backstraight, backstrFlush
			if(straight_cnt == 4 && bBackStraight && !bBackStraightFlush)
			{
				m_backstraIndex = j;
				if((pattern_cnt[0]>=4) || (pattern_cnt[1]>=4) || (pattern_cnt[2]>=4) || (pattern_cnt[3]>=4))
				{
					bBackStraightFlush =  TRUE;
				}
			}	*/	
			
			base = j;
		}
		else if(((m_sortCard[base]%13)+1) < (m_sortCard[j]%13))
		{
			// 연속된 숫자가 아니라면 초기화
			base = j;
			
			if(straight_cnt < 4)
			{
				straight_cnt=0;
				pattern_cnt[0] = 0;
				pattern_cnt[1] = 0;
				pattern_cnt[2] = 0;
				pattern_cnt[3] = 0;
				
				bBackStraight = FALSE;
			}
			else
			{
				countnflag=FALSE;
			}
		}
		else if((m_sortCard[base]%13) == (m_sortCard[j]%13)) // is Pair
		{
			bool bflush = false;
			preface =  is_Card_pattern( m_sortCard[base] );
			for(int k=j+1; k<= card_i; k++)
			{													//	   base	(base 기준)	
				if(m_sortCard[base]%13+1 == m_sortCard[k]%13)	// 3   (3)   (4)  (4)   5   6
				{
					nextface = is_Card_pattern( m_sortCard[k] );
					if(preface == nextface)
					{
						bflush = true;
					}
				}
			}
			
			if(!bflush)
			{
				tmpface = is_Card_pattern(m_sortCard[base]);
				// pattern_cnt 배열의 인덱스초과검사
				if (tmpface < 0 || 4<= tmpface)
				{
					continue;
				}
				if(pattern_cnt[tmpface] < 4)
				{
					pattern_cnt[tmpface] = 0;
				}
			}
			
			nextface = is_Card_pattern( m_sortCard[j] );
			for(int k=0; k<j; k++)
			{
				if(m_sortCard[j]%13 == m_sortCard[k]%13+1 )// AA 두번검사 방지
				{
					preface =  is_Card_pattern( m_sortCard[k] );
					if(preface == nextface && countnflag)
					{
						// pattern_cnt 배열의 인덱스초과검사
						if (preface < 0 || 4<= preface)
						{
							continue;
						}
						pattern_cnt[preface] ++;
						if(straight_cnt == 4 && bBackStraight && !bBackStraightFlush)
						{
							if(m_sortCard[k]%13 != 11) m_backstraIndex = j;
							if((pattern_cnt[0]>=4) || (pattern_cnt[1]>=4) || (pattern_cnt[2]>=4) || (pattern_cnt[3]>=4))
							{
								bBackStraightFlush =  TRUE;
							}
						}
						if(pattern_cnt[preface] >= 4)
							royalindex = j;
					}
					
				}
			}
			
			base = j;
			
			if(m_sortCard[index]%13 == m_sortCard[j]%13)
			{
				index = j;
			}
			
		}
	}
		
	// Royal, StrFrh, Straight
	if (straight_cnt >= 4)
	{
		if((pattern_cnt[0]>=4) || (pattern_cnt[1]>=4) || (pattern_cnt[2]>=4) || (pattern_cnt[3]>=4))
		{
			if(royalindex>0)
			{
				if((m_sortCard[royalindex]%13) == 12)	// is Royal
					m_stRuleData.Royal = m_sortCard[royalindex];
				else 
				{
					if(m_backstraIndex > 0 && bBackStraightFlush)
						m_stRuleData.BackStrFsh = m_sortCard[m_backstraIndex];
					else
						m_stRuleData.StrFsh = m_sortCard[royalindex];
				}
			}
			
			if(m_backstraIndex > 0)
			{				
				m_stRuleData.BStraight = m_sortCard[m_backstraIndex];				
			}
			else if( m_sortCard[index]%13 == 12)
			{
				m_stRuleData.MStraight = m_sortCard[index];
			}
			else
				m_stRuleData.Straight = m_sortCard[index];
		}
		else if(m_backstraIndex > 0)
		{
			//족버 버그 수정 (백스트레이트 일때 제일 높은 숫자 Ace)
			//m_stRuleData.BStraight = m_sortCard[m_backstraIndex];
			m_stRuleData.BStraight = m_sortCard[card_i];
		}
		else if( m_sortCard[index]%13 == 12)
		{
			m_stRuleData.MStraight = m_sortCard[index];
		}
		else
			m_stRuleData.Straight = m_sortCard[index];
	}

	///////////////////////////////////////////////
	///////////////////////////////////////////////

}

/* 플러쉬 룰 구하기 */
void CCardRule::cal_flush_class()
{
	int pattern_cnt[4] = { 0,};	
	int index = 0;
	int card_i = m_nTotalCard - 1;
	for(int j=0;j<4;j++)		// for Fsh !!    j:face
	{
		//추가
		for(int k=0; k <= card_i; k++) //7
		{
			if((m_sortCard[k] >= (j*13))&&(m_sortCard[k] <= (j*13)+12))
			{
				pattern_cnt[j]++;
				index = k;
			}
		}
		if(pattern_cnt[j] >=5)
		{
			m_stRuleData.Fsh = m_sortCard[index];
		}
	}
	
}

/* 페어.. 룰 구하기 */
void CCardRule::cal_pair_class()
{
	/// Four FullH Triple pair 
	int same_cnt=0;	
	int index = 0;
	int card_i = m_nTotalCard - 1;
	for(int j=0; j < card_i; j++) // < 6   -1 번 보드 카드는 7장인뎅...
	{
		same_cnt = 0;
		for(int k=j+1; k <= card_i; k++) // < 7 
		{
			if((m_sortCard[j]%13)==(m_sortCard[k]%13))
			{
				same_cnt++;
				index = k;
			}
		}
		
		if(same_cnt == 1) // Pair
		{
			if(m_stRuleData.Trip[0]>0)
			{
				m_stRuleData.FullH[0] = m_stRuleData.Trip[0];
				m_stRuleData.FullH[1] = m_sortCard[index];
			}
		
			if(m_stRuleData.Onep[0]>0) // new
			{
				if((m_stRuleData.Onep[0]%13) < (m_sortCard[index]%13))
				{
					// 0 > 1 > 2
					m_stRuleData.Onep[3] = m_stRuleData.Onep[2];
					m_stRuleData.Onep[2] = m_stRuleData.Onep[1];
					m_stRuleData.Onep[1] = m_stRuleData.Onep[0];
					m_stRuleData.Onep[0] = m_sortCard[index];
				}
				else if((m_stRuleData.Onep[1]%13) < (m_sortCard[index]%13))
				{
					m_stRuleData.Onep[1] = m_sortCard[index];
				}
			}
			else
			{
				m_stRuleData.Onep[0] = m_sortCard[index];
			}
			j=index;
		}
		else if(same_cnt == 2) // Triple
		{
			if((m_stRuleData.Trip[0]>0) || (m_stRuleData.Onep[0]>0))
			{
				if((m_stRuleData.Trip[0]%13) >= (m_stRuleData.Onep[0])%13)
					m_stRuleData.FullH[1] = m_stRuleData.Trip[0];
				else
					m_stRuleData.FullH[1] = m_stRuleData.Onep[0];
				
				m_stRuleData.FullH[0] = m_sortCard[index];
			}
		
			if((m_stRuleData.Trip[0]%13) < (m_sortCard[index]%13)) //New
			{
				m_stRuleData.Trip[1] = m_stRuleData.Trip[0];
				m_stRuleData.Trip[0] = m_sortCard[index];
			}
			
			j=index;
		}
		else if(same_cnt == 3)  // Four card
		{
			if((m_stRuleData.Poker[0]%13) < (m_sortCard[index]%13)) //New
			{
				m_stRuleData.Poker[1] = m_stRuleData.Poker[0];
				m_stRuleData.Poker[0] = m_sortCard[index];
			}
			else
				m_stRuleData.Poker[0] = m_sortCard[index];
			
			j=index;
		}
	}


}


/* 족보 구하기 */
void CCardRule::cal_Card()
{
	memset(&m_stRuleData, EMPTY_VALUE, sizeof(m_stRuleData));
	
	// 현재 카드 장수
	sort_Card(0, m_nTotalCard);	
	sort_Card_low(0, m_nTotalCard);	


	if(m_nTotalCard >= 5)
	{
		cal_straight_class();   /// straight..
		cal_flush_class();		/// flush
	}
	cal_pair_class();			/// pair, triple, four, full

	// 페어 족보를 제외한 최대 카드값
	int card_i = m_nTotalCard - 1;
	if(m_stRuleData.Onep[0] > 0)
	{	
		int temp = -1;
		for(int i=0; i<= card_i; i++)
		{
			if(m_stRuleData.Onep[0]%13== m_sortCard[i]%13) continue;
			if(m_stRuleData.Onep[1] > 0 && m_stRuleData.Onep[1]%13== m_sortCard[i]%13) continue;
			temp = m_sortCard[i];
		}
		if(temp >= 0)
			m_stRuleData.mang = temp;
	}
	else
		m_stRuleData.mang = m_sortCard[card_i];


	// 일반룰 
//	get_cal_sevenRule();
	
	// 이벤트용
	get_cal_AllsevenRule();
	get_cal_sevenLowRule();

}

/* 룰 구하기 (High Rule 만) */
void CCardRule::get_cal_sevenRule()
{
	int card_i = m_nTotalCard - 1;

	int rcnt = 0;

	memset(m_RuleComplteCard,-1,sizeof(m_RuleComplteCard));

	if(m_stRuleData.Royal > 0)
	{
		int pattern = is_Card_pattern( m_stRuleData.Royal );
		switch( pattern	 )
		{
			case 0: m_SevenValue = static_cast<int>(N7C_CROYAL); break;
			case 1: m_SevenValue = static_cast<int>(N7C_HROYAL); break;
			case 2: m_SevenValue = static_cast<int>(N7C_DROYAL); break;
			case 3: m_SevenValue = static_cast<int>(N7C_SROYAL); break;
		}

		m_SecondSevenValue = m_SevenValue;

		m_SevenValue = static_cast<int>(N7C_CROYAL);

		int tmp = 0;
		int cnt = 0;

		// 족보를 제외한 카드
		for(int i = card_i; i >= 0; i--)
		{
			tmp = m_sortCard[i]%13;
			if( pattern == is_Card_pattern(m_sortCard[i]) && tmp <= 12 && tmp >= 8) {

				m_RuleComplteCard[rcnt++] = m_sortCard[i];
				continue;
			}
			m_RemainCard[cnt++] = m_sortCard[i];
		}

	}
	else if(m_stRuleData.BackStrFsh > 0)
	{
		m_SevenValue = static_cast<int>(N7C_BSFLUSH);

		int tmp = 0;
		int cnt = 0;
		int pattern = is_Card_pattern( m_stRuleData.BackStrFsh );
		// 족보를 제외한 카드
		for(int i = card_i; i >= 0; i--)
		{
			tmp = m_sortCard[i]%13;
			if( pattern == is_Card_pattern(m_sortCard[i]) && tmp <= 3 && tmp >= 0 
				|| pattern == is_Card_pattern(m_sortCard[i]) && tmp == 12) 
			{
				m_RuleComplteCard[rcnt++] = m_sortCard[i];
				continue;
			}
			m_RemainCard[cnt++] = m_sortCard[i];
		}
	}
	else if(m_stRuleData.StrFsh > 0)
	{
		m_SevenValue = static_cast<int>(N7C_SFULSH);

		int tmp = 0;
		int cnt = 0;
		int pattern = is_Card_pattern( m_stRuleData.StrFsh );
		int org = m_stRuleData.StrFsh % 13;
		// 족보를 제외한 카드
		for(int i = card_i; i >= 0; i--)
		{
			tmp = m_sortCard[i]%13;
			if( pattern == is_Card_pattern(m_sortCard[i]) && org >= tmp && org-4 <= tmp) 
			{
				m_RuleComplteCard[rcnt++] = m_sortCard[i];
				continue;
			}
			m_RemainCard[cnt++] = m_sortCard[i];
		}
	}
	else if(m_stRuleData.Poker[0] > 0)
	{
		m_SevenValue = static_cast<int>(N7C_QUADS);

		int tmp = 0;
		int cnt = 0;
		int org = m_stRuleData.Poker[0] % 13;
		// 족보를 제외한 카드
		for(int i = card_i; i >= 0; i--)
		{
			tmp = m_sortCard[i]%13;
			if( org == tmp )
			{
				m_RuleComplteCard[rcnt++] = m_sortCard[i];
				continue;
			}
			m_RemainCard[cnt++] = m_sortCard[i];
		}
	}
	else if(m_stRuleData.FullH[0] > 0)
	{
		m_SevenValue = static_cast<int>(N7C_TITLE);

		int tmp = 0;
		int cnt = 0;
		
		int ncnt = 0;	
		int ncnt1 = 0;	
		int org  = m_stRuleData.FullH[0] % 13;
		int org1 = m_stRuleData.FullH[1] % 13;

		// 족보를 제외한 카드
		for(int i = card_i; i >= 0; i--)
		{
			tmp = m_sortCard[i]%13;
			if( org == tmp && ncnt <= 3) 
			{ 
				ncnt++; 

				m_RuleComplteCard[rcnt++] = m_sortCard[i];
				continue; 
			}
			if( org1 == tmp && ncnt1 <= 2) { 
				ncnt1++; 
				m_RuleComplteCard[rcnt++] = m_sortCard[i];
				continue; 
			}

			m_RemainCard[cnt++] = m_sortCard[i];
		}
	}
	else if(m_stRuleData.Fsh > 0)
	{
		m_SevenValue = static_cast<int>(N7C_FLUSH);

		int tmp = 0;
		int cnt = 0;
		int ncnt = 0;
		int pattern = is_Card_pattern( m_stRuleData.Fsh );
		
		// 족보를 제외한 카드
		for(int i = card_i; i >= 0; i--)
		{
			tmp = is_Card_pattern( m_sortCard[i] );
			if( pattern == tmp && ncnt<=4) 
			{ 
				ncnt++; 
				m_RuleComplteCard[rcnt++] = m_sortCard[i];
				continue; 
			}
			m_RemainCard[cnt++] = m_sortCard[i];
		}
	}
	else if(m_stRuleData.MStraight > 0)
	{
		m_SevenValue = static_cast<int>(N7C_MOUNT);

		int tmp = 0;
		int cnt = 0;
		
		int ncnt = 0;	
		int org  = m_stRuleData.MStraight % 13;

		// 족보를 제외한 카드
		for(int i = card_i; i >= 0; i--)
		{
			tmp = m_sortCard[i]%13;
			if( org-ncnt == tmp && ncnt<=4 ) 
			{ 
				ncnt++; 
				m_RuleComplteCard[rcnt++] = m_sortCard[i];
				continue; 
			}
			m_RemainCard[cnt++] = m_sortCard[i];
		}
	}
	else if(m_stRuleData.BStraight > 0)
	{
		m_SevenValue = static_cast<int>(N7C_BACKS);

		int tmp = 0;
		int cnt = 0;
		int org =  m_stRuleData.BStraight%13;
		int ncnt = 0;
		// 족보를 제외한 카드
		m_RuleComplteCard[rcnt++] = m_sortCard[card_i];
		for(int i = card_i-1; i >= 0; i--)
		{
			tmp = m_sortCard[i]%13;
			if( 3-ncnt == tmp   ) 
			{
				ncnt++;
				m_RuleComplteCard[rcnt++] = m_sortCard[i];
				continue;
			}

			m_RemainCard[cnt++] = m_sortCard[i];
		}

	}
	else if(m_stRuleData.Straight > 0)
	{
		m_SevenValue = static_cast<int>(N7C_STRAIGHT);

		int tmp = 0;
		int cnt = 0;
		
		int ncnt = 0;	
		int org  = m_stRuleData.Straight % 13;

		// 족보를 제외한 카드
		for(int i = card_i; i >= 0; i--)
		{
			tmp = m_sortCard[i]%13;
			if( org-ncnt == tmp && ncnt<=4 ) 
			{ 
				ncnt++; 
				m_RuleComplteCard[rcnt++] = m_sortCard[i];
				continue; 

			}
			m_RemainCard[cnt++] = m_sortCard[i];
		}


	}
	else if(m_stRuleData.Trip[0] > 0)
	{
		m_SevenValue = static_cast<int>(N7C_TRIPLE);

		int tmp = 0;
		int cnt = 0;
		int org = m_stRuleData.Trip[0] % 13;
		// 족보를 제외한 카드
		for(int i = card_i; i >= 0; i--)
		{
			tmp = m_sortCard[i]%13;
			if( org == tmp ) 
			{
				m_RuleComplteCard[rcnt++] = m_sortCard[i];
				continue;
			}
			m_RemainCard[cnt++] = m_sortCard[i];
		}


	}
	else if(m_stRuleData.Onep[0] > 0)
	{
		if(m_stRuleData.Onep[1] > 0) // TWO Pair
		{
			m_SevenValue = static_cast<int>(N7C_TWOPAIR);

			int tmp = 0;
			int cnt = 0;
			int org = m_stRuleData.Onep[0] % 13;
			int org1 = m_stRuleData.Onep[1] % 13;
			// 족보를 제외한 카드
			for(int i = card_i; i >= 0; i--)
			{
				tmp = m_sortCard[i]%13;
				if( org == tmp || org1 == tmp) 
				{
					m_RuleComplteCard[rcnt++] = m_sortCard[i];
					continue;
				}
				m_RemainCard[cnt++] = m_sortCard[i];
			}

		}
		else
		{	
			m_SevenValue = static_cast<int>(N7C_ONEPAIR);

			int tmp = 0;
			int cnt = 0;
			int org = m_stRuleData.Onep[0] % 13;
			// 족보를 제외한 카드
			for(int i = card_i; i >= 0; i--)
			{
				tmp = m_sortCard[i]%13;
				if( org == tmp ) {
					m_RuleComplteCard[rcnt++] = m_sortCard[i];
					continue;
				}
				m_RemainCard[cnt++] = m_sortCard[i];
			}


		}
	}
	else if(m_stRuleData.mang >= 0)
	{
		m_SevenValue = static_cast<int>(N7C_NOPAIR);

		int cnt = 0;
		for(int i = card_i-1; i >= 0; i--)
		{
			m_RemainCard[cnt++] = m_sortCard[i];
			
		}
		
		m_RuleComplteCard[rcnt++] = m_sortCard[card_i];


	}
}

/* 이벤트용 모든룰 구하기 */
void CCardRule::get_cal_AllsevenRule()
{
int card_i = m_nTotalCard - 1;
	int rcnt = 0;
	int nall = 0;
	int count=0;

	memset( m_AllRuleCard, -1, sizeof(m_AllRuleCard));
	
	bool bRuleHave = false;

	if(m_stRuleData.Royal > 0)
	{
		int pattern = is_Card_pattern( m_stRuleData.Royal );
		switch( pattern	 )
		{
			case 0: m_SevenValue = static_cast<int>(N7C_CROYAL); break;
			case 1: m_SevenValue = static_cast<int>(N7C_HROYAL); break;
			case 2: m_SevenValue = static_cast<int>(N7C_DROYAL); break;
			case 3: m_SevenValue = static_cast<int>(N7C_SROYAL); break;
		}


		m_SecondSevenValue = m_SevenValue;
	
		int tmp = 0;
		int cnt = 0;
		count=0;
	
		// 족보를 제외한 카드
		for(int i = card_i; i >= 0; i--)
		{
			tmp = m_sortCard[i]%13;
			if( pattern == is_Card_pattern(m_sortCard[i]) && tmp <= 12 && tmp >= 8) {
				
				m_RuleComplteCard[rcnt++] = m_sortCard[i];
				m_AllRuleCard[N7C_CROYAL][nall++] = m_sortCard[i];
				continue;
			}
			else if( pattern == is_Card_pattern(m_sortCard[i]) && tmp == (7 - count) ) {
				count++;
				m_AllRuleCard[N7C_CROYAL][nall++] = m_sortCard[i];
			}
			m_RemainCard[cnt++] = m_sortCard[i];
		}

		m_SevenValue = static_cast<int>(N7C_CROYAL);
		bRuleHave = true;
	}

	
	if(m_stRuleData.BackStrFsh > 0)
	{
		
		rcnt = 0;
		int tmp = 0;
		int cnt = 0;
		int pattern = is_Card_pattern( m_stRuleData.BackStrFsh );

		count=0;
		nall = 0;

		if(!bRuleHave)
		{
			bRuleHave = true;
			m_SevenValue = static_cast<int>(N7C_BSFLUSH);

			// 족보를 제외한 카드
			for(int i = card_i; i >= 0; i--)
			{
				tmp = m_sortCard[i]%13;
				if( pattern == is_Card_pattern(m_sortCard[i]) && tmp <= 3 && tmp >= 0 
					|| pattern == is_Card_pattern(m_sortCard[i]) && tmp == 12) 
				{
					m_RuleComplteCard[rcnt++] = m_sortCard[i];
					m_AllRuleCard[N7C_BSFLUSH][nall++] = m_sortCard[i];
					continue;
				}
				m_RemainCard[cnt++] = m_sortCard[i];
			}

			for(int i = 0; i <= card_i; i++)
			{
				tmp = m_sortCard[i]%13;
				if( pattern == is_Card_pattern(m_sortCard[i]) && tmp == (4 + count)
					|| pattern == is_Card_pattern(m_sortCard[i]) && tmp != 12) 
				{
					count++;
					m_AllRuleCard[N7C_BSFLUSH][nall++] = m_sortCard[i];
				}
			}
		}
		else
		{
				// 족보를 제외한 카드
			for(int i = card_i; i >= 0; i--)
			{
				tmp = m_sortCard[i]%13;
				if( pattern == is_Card_pattern(m_sortCard[i]) && tmp <= 3 && tmp >= 0 
					|| pattern == is_Card_pattern(m_sortCard[i]) && tmp == 12) 
				{
					m_AllRuleCard[N7C_BSFLUSH][nall++] = m_sortCard[i];
					continue;
				}
			}

			for(int i = 0; i <= card_i; i++)
			{
				tmp = m_sortCard[i]%13;
				if( pattern == is_Card_pattern(m_sortCard[i]) && tmp == (4 + count)
					|| pattern == is_Card_pattern(m_sortCard[i]) && tmp != 12) 
				{
					count++;
					m_AllRuleCard[N7C_BSFLUSH][nall++] = m_sortCard[i];
				}
			}

		}
	}
	
	
	if(m_stRuleData.StrFsh > 0)
	{
		rcnt = 0;
		int tmp = 0;
		int cnt = 0;
		int pattern = is_Card_pattern( m_stRuleData.StrFsh );
		int org = m_stRuleData.StrFsh % 13;

		count=0;
		nall = 0;
		
		if( !bRuleHave ) {
		
			bRuleHave = true;
			m_SevenValue = static_cast<int>(N7C_SFULSH);
					
			// 족보를 제외한 카드
			for(int i = card_i; i >= 0; i--)
			{
				tmp = m_sortCard[i]%13;
				if( pattern == is_Card_pattern(m_sortCard[i]) && org >= tmp && org-4 <= tmp) 
				{
					m_RuleComplteCard[rcnt++] = m_sortCard[i];
					m_AllRuleCard[N7C_SFULSH][nall++] = m_sortCard[i];
					continue;
				}
				else if( pattern == is_Card_pattern(m_sortCard[i]) &&  (org - 5 - count) == tmp) 
				{
					count++;
					m_AllRuleCard[N7C_SFULSH][nall++] = m_sortCard[i];
				}
				
				m_RemainCard[cnt++] = m_sortCard[i];
			}
		}
		else
		{
			for(int i = card_i; i >= 0; i--)
			{
				tmp = m_sortCard[i]%13;
				if( pattern == is_Card_pattern(m_sortCard[i]) && org >= tmp && org-4 <= tmp) 
				{
					m_AllRuleCard[N7C_SFULSH][nall++] = m_sortCard[i];
					continue;
				}
				else if( pattern == is_Card_pattern(m_sortCard[i]) &&  (org - 5 - count) == tmp) 
				{
					count++;
					m_AllRuleCard[N7C_SFULSH][nall++] = m_sortCard[i];
				}
			}
		}
	}


	if(m_stRuleData.Poker[0] > 0)
	{
		rcnt = 0;
		int tmp = 0;
		int cnt = 0;
		int org = m_stRuleData.Poker[0] % 13;

		count = -2;
		if(m_stRuleData.Poker[1]>0)
		{
			count = m_stRuleData.Poker[1] % 13;
		}
		nall = 0;
		
		if( !bRuleHave ) {

			bRuleHave = true;
			m_SevenValue = static_cast<int>(N7C_QUADS);

			// 족보를 제외한 카드
			for(int i = card_i; i >= 0; i--)
			{
				tmp = m_sortCard[i]%13;
				if( org == tmp )
				{
					m_RuleComplteCard[rcnt++] = m_sortCard[i];
					m_AllRuleCard[N7C_QUADS][nall++] = m_sortCard[i];
					continue;
				}
				else if(count == tmp)
				{
					m_AllRuleCard[N7C_QUADS][nall++] = m_sortCard[i];
				}
				m_RemainCard[cnt++] = m_sortCard[i];
			}
		}
		else
		{
			for(int i = card_i; i >= 0; i--)
			{
				tmp = m_sortCard[i]%13;
				if( org == tmp || count == tmp)
				{
					m_AllRuleCard[N7C_QUADS][nall++] = m_sortCard[i];
					continue;
				}
			}

		}
	}
	
	if(m_stRuleData.FullH[0] > 0)
	{
		int tmp = 0;
		int cnt = 0;
		
		int ncnt = 0;	
		int ncnt1 = 0;	
		int org  = m_stRuleData.FullH[0] % 13;
		int org1 = m_stRuleData.FullH[1] % 13;

		count=0;
		nall = 0;

		if( !bRuleHave ) {
			
			bRuleHave = true;
			m_SevenValue = static_cast<int>(N7C_TITLE);

			// 족보를 제외한 카드
			for(int i = card_i; i >= 0; i--)
			{
				tmp = m_sortCard[i]%13;
				if( org == tmp && ncnt <= 3) 
				{ 
					ncnt++; 
					m_RuleComplteCard[rcnt++] = m_sortCard[i];
					m_AllRuleCard[N7C_TITLE][nall++] = m_sortCard[i];
					continue; 
				}
				else if( org1 == tmp && ncnt1 <= 2) { 
					ncnt1++; 
					m_RuleComplteCard[rcnt++] = m_sortCard[i];
					m_AllRuleCard[N7C_TITLE][nall++] = m_sortCard[i];
					continue; 
				}
				
				m_RemainCard[cnt++] = m_sortCard[i];
			}
		}
		else
		{
			// 족보를 제외한 카드
			for(int i = card_i; i >= 0; i--)
			{
				tmp = m_sortCard[i]%13;
				if( org == tmp && ncnt <= 3) 
				{ 
					ncnt++; 
					m_AllRuleCard[N7C_TITLE][nall++] = m_sortCard[i];
					continue; 
				}
				else if( org1 == tmp && ncnt1 <= 2) { 
					ncnt1++; 
					m_AllRuleCard[N7C_TITLE][nall++] = m_sortCard[i];
					continue; 
				}
			}
		}
	}
	
	if(m_stRuleData.Fsh > 0)
	{
		int tmp = 0;
		int cnt = 0;
		int ncnt = 0;
		int pattern = is_Card_pattern( m_stRuleData.Fsh );
		count=0;
		nall = 0;
		
		if( !bRuleHave ) {

			bRuleHave = true;
			m_SevenValue = static_cast<int>(N7C_FLUSH);

			// 족보를 제외한 카드
			for(int i = card_i; i >= 0; i--)
			{
				tmp = is_Card_pattern( m_sortCard[i] );
				if( pattern == tmp && ncnt<=4) 
				{ 
					ncnt++; 
					m_RuleComplteCard[rcnt++] = m_sortCard[i];
					m_AllRuleCard[N7C_FLUSH][nall++] = m_sortCard[i];
					continue; 
				}
				else if(pattern == tmp)
				{
					m_AllRuleCard[N7C_FLUSH][nall++] = m_sortCard[i];
				}
				
				m_RemainCard[cnt++] = m_sortCard[i];
			}
		}
		else
		{
			for(int i = card_i; i >= 0; i--)
			{
				tmp = is_Card_pattern( m_sortCard[i] );
				if( pattern == tmp && ncnt<=4) 
				{ 
					ncnt++; 
					m_AllRuleCard[N7C_FLUSH][nall++] = m_sortCard[i];
					continue; 
				}
				else if(pattern == tmp)
				{
					m_AllRuleCard[N7C_FLUSH][nall++] = m_sortCard[i];
				}
				
			}

		}
	}

	if(m_stRuleData.MStraight > 0)
	{
	
		int tmp = 0;
		int cnt = 0;
		int ncnt = 0;	
		int org  = m_stRuleData.MStraight % 13;

		count=0;
		nall = 0;
		int pre = -1;

		if( !bRuleHave ) {
			
			bRuleHave = true;
			m_SevenValue = static_cast<int>(N7C_MOUNT);

			// 족보를 제외한 카드
			for(int i = card_i; i >= 0; i--)
			{
				tmp = m_sortCard[i]%13;
				if( org-ncnt == tmp && ncnt<=4 ) 
				{ 
					ncnt++; 
					m_RuleComplteCard[rcnt++] = m_sortCard[i];
					m_AllRuleCard[N7C_MOUNT][nall++] = m_sortCard[i];
					pre = tmp; 
					continue; 
				}
				else if( pre == tmp)
				{
					m_AllRuleCard[N7C_MOUNT][nall++] = m_sortCard[i];
				}
				else if( 7 - count == tmp) 
				{
					count++;
					m_AllRuleCard[N7C_MOUNT][nall++] = m_sortCard[i];
					pre = tmp;
				}
				
				m_RemainCard[cnt++] = m_sortCard[i];
			}
		}
		else
		{
			for(int i = card_i; i >= 0; i--)
			{
				tmp = m_sortCard[i]%13;
				if( org-ncnt == tmp && ncnt<=4 ) 
				{ 
					ncnt++; 
					m_AllRuleCard[N7C_MOUNT][nall++] = m_sortCard[i];
					pre = tmp; 
					continue; 
				}
				else if( pre == tmp)
				{
					m_AllRuleCard[N7C_MOUNT][nall++] = m_sortCard[i];
				}
				else if( 7 - count == tmp) 
				{
					count++;
					m_AllRuleCard[N7C_MOUNT][nall++] = m_sortCard[i];
					pre = tmp; 
				}
				
			}
		}

	}
	
	if(m_stRuleData.BStraight > 0)
	{
		int tmp = 0;
		int cnt = 0;
		int org =  m_stRuleData.BStraight%13;
		int ncnt = 0;

		count=0;
		nall = 0;

		int pre = -1;

		if( !bRuleHave ) {
			
			bRuleHave = true;
			m_SevenValue = static_cast<int>(N7C_BACKS);
			
			// 족보를 제외한 카드
			m_RuleComplteCard[rcnt++] = m_sortCard[card_i];
			m_AllRuleCard[N7C_BACKS][nall++] = m_sortCard[card_i];

			pre = m_sortCard[card_i] % 13;

			for(int i = card_i-1; i >= 0; i--)
			{
				tmp = m_sortCard[i]%13;
				if( 3-ncnt == tmp   ) 
				{
					ncnt++;
					m_RuleComplteCard[rcnt++] = m_sortCard[i];
					m_AllRuleCard[N7C_BACKS][nall++] = m_sortCard[i];
					pre = tmp;
					continue;
				}
				else if(pre == tmp)
				{
					m_AllRuleCard[N7C_BACKS][nall++] = m_sortCard[i];
				}
				
				m_RemainCard[cnt++] = m_sortCard[i];
			}
			
			pre = -1;
			for(int i = 0; i <= card_i; i++)
			{
				tmp = m_sortCard[i]%13;
				if( tmp == 4 + count )
				{
					count++;
					m_AllRuleCard[N7C_BACKS][nall++] = m_sortCard[i];
					pre = tmp;
				}
				else if(pre == tmp)
				{
					m_AllRuleCard[N7C_BACKS][nall++] = m_sortCard[i];
				}

			}
		}
		else
		{
			// 족보를 제외한 카드
			m_RuleComplteCard[rcnt++] = m_sortCard[card_i];
			pre = m_sortCard[card_i] % 13;

			for(int i = card_i-1; i >= 0; i--)
			{
				tmp = m_sortCard[i]%13;
				if( 3-ncnt == tmp   ) 
				{
					ncnt++;
					m_AllRuleCard[N7C_BACKS][nall++] = m_sortCard[i];
					pre = tmp;
					continue;
				}
				else if(pre==tmp)
				{
					m_AllRuleCard[N7C_BACKS][nall++] = m_sortCard[i];
				}
			}
			
			pre = -1;
			for(int i = 0; i <= card_i; i++)
			{
				tmp = m_sortCard[i]%13;
				if( tmp == 4 + count )
				{
					count++;
					m_AllRuleCard[N7C_BACKS][nall++] = m_sortCard[i];
					pre = tmp;
				}
				else if(pre == tmp)
				{
					m_AllRuleCard[N7C_BACKS][nall++] = m_sortCard[i];
				}

			}

		}
	}

	if(m_stRuleData.Straight > 0)
	{
		int tmp = 0;
		int cnt = 0;
		
		int ncnt = 0;	
		int org  = m_stRuleData.Straight % 13;

		count=0;
		nall = 0;

		int pre = -1;

		if( !bRuleHave ) {

			bRuleHave = true;
			m_SevenValue = static_cast<int>(N7C_STRAIGHT);
			
			// 족보를 제외한 카드
			for(int i = card_i; i >= 0; i--)
			{
				tmp = m_sortCard[i]%13;
				if( org-ncnt == tmp && ncnt<=4 ) 
				{ 
					ncnt++; 
					m_RuleComplteCard[rcnt++] = m_sortCard[i];
					m_AllRuleCard[N7C_STRAIGHT][nall++] = m_sortCard[i];
					pre = tmp;
					continue; 
				}
				else if(pre == tmp)
				{
					m_AllRuleCard[N7C_STRAIGHT][nall++] = m_sortCard[i];
				}
				else if( (org - 5 - count) == tmp) 
				{
					count++;
					m_AllRuleCard[N7C_STRAIGHT][nall++] = m_sortCard[i];
					pre = tmp;
				}
				
				m_RemainCard[cnt++] = m_sortCard[i];
			}
		}
		else
		{
			// 족보를 제외한 카드
			for(int i = card_i; i >= 0; i--)
			{
				tmp = m_sortCard[i]%13;
				if( org-ncnt == tmp && ncnt<=4 ) 
				{ 
					ncnt++; 
					m_AllRuleCard[N7C_STRAIGHT][nall++] = m_sortCard[i];
					pre = tmp;
					continue; 
				}
				else if(pre == tmp)
				{
					m_AllRuleCard[N7C_STRAIGHT][nall++] = m_sortCard[i];
				}
				else if( (org - 5 - count) == tmp) 
				{
					count++;
					m_AllRuleCard[N7C_STRAIGHT][nall++] = m_sortCard[i];
					pre = tmp;
				}
			}
		}
	}
	
	if(m_stRuleData.Trip[0] > 0)
	{
	

		int tmp = 0;
		int cnt = 0;
		int org = m_stRuleData.Trip[0] % 13;


		count=-2;
		if(m_stRuleData.Trip[1] > 0)
		{
			count = m_stRuleData.Trip[1] % 13;
		}
		nall = 0;

		if( !bRuleHave ) {
			bRuleHave = true;
			m_SevenValue = static_cast<int>(N7C_TRIPLE);
			
			// 족보를 제외한 카드
			for(int i = card_i; i >= 0; i--)
			{
				tmp = m_sortCard[i]%13;
				if( org == tmp ) 
				{
					m_RuleComplteCard[rcnt++] = m_sortCard[i];
					m_AllRuleCard[N7C_TRIPLE][nall++] = m_sortCard[i];
					continue;
				}
				else if(count==org)
				{
					m_AllRuleCard[N7C_TRIPLE][nall++] = m_sortCard[i];
				}
				m_RemainCard[cnt++] = m_sortCard[i];
			}
		}
		else
		{
				// 족보를 제외한 카드
			for(int i = card_i; i >= 0; i--)
			{
				tmp = m_sortCard[i]%13;
				if( org == tmp ) 
				{
					m_AllRuleCard[N7C_TRIPLE][nall++] = m_sortCard[i];
					continue;
				}
				else if(count==org)
				{
					m_AllRuleCard[N7C_TRIPLE][nall++] = m_sortCard[i];
				}
			}
		}
	}

	if(m_stRuleData.Onep[0] > 0)
	{
		count=0;
		nall = 0;

		if(m_stRuleData.Onep[1] > 0) // TWO Pair
		{
			

			int tmp = 0;
			int cnt = 0;
			int org = m_stRuleData.Onep[0] % 13;
			int org1 = m_stRuleData.Onep[1] % 13;

			int org2=-2;
			int org3=-2;

			if(m_stRuleData.Onep[2] > 0)
			{
				org2 = m_stRuleData.Onep[2]%13;
				if(m_stRuleData.Onep[3] > 0)
				{
					org3 = m_stRuleData.Onep[3]%13;
				}
			}
			
			if( !bRuleHave ) {
				bRuleHave = true;
				m_SevenValue = static_cast<int>(N7C_TWOPAIR);
				
				// 족보를 제외한 카드
				for(int i = card_i; i >= 0; i--)
				{
					tmp = m_sortCard[i]%13;
					if( org == tmp || org1 == tmp) 
					{
						m_RuleComplteCard[rcnt++] = m_sortCard[i];
						m_AllRuleCard[N7C_TWOPAIR][nall++] = m_sortCard[i];
						continue;
					}
					else if( org2 == tmp || org3 == tmp) 
					{
						m_AllRuleCard[N7C_TWOPAIR][nall++] = m_sortCard[i];
					}
					
					m_RemainCard[cnt++] = m_sortCard[i];
				}
			}
			else
			{
				// 족보를 제외한 카드
				for(int i = card_i; i >= 0; i--)
				{
					tmp = m_sortCard[i]%13;
					if( org == tmp || org1 == tmp) 
					{
						m_AllRuleCard[N7C_TWOPAIR][nall++] = m_sortCard[i];
						continue;
					}
					else if( org2 == tmp || org3 == tmp) 
					{
						m_AllRuleCard[N7C_TWOPAIR][nall++] = m_sortCard[i];
					}
				}
			}

		}
		else
		{	
	

			int tmp = 0;
			int cnt = 0;
			int org = m_stRuleData.Onep[0] % 13;

			if( !bRuleHave ) {
				bRuleHave = true;
				m_SevenValue = static_cast<int>(N7C_ONEPAIR);
				
				
				// 족보를 제외한 카드
				for(int i = card_i; i >= 0; i--)
				{
					tmp = m_sortCard[i]%13;
					if( org == tmp ) 
					{
						m_RuleComplteCard[rcnt++] = m_sortCard[i];
						m_AllRuleCard[N7C_ONEPAIR][nall++] = m_sortCard[i];
						continue;
					}
					m_RemainCard[cnt++] = m_sortCard[i];
				}
			}
			else
			{
				// 족보를 제외한 카드
				for(int i = card_i; i >= 0; i--)
				{
					tmp = m_sortCard[i]%13;
					if( org == tmp ) 
					{
						m_AllRuleCard[N7C_ONEPAIR][nall++] = m_sortCard[i];
						continue;
					}
				}

			}
		}
	}
	
	if(m_stRuleData.mang >= 0)
	{
		if( !bRuleHave ) {
			bRuleHave = true;
			m_SevenValue = static_cast<int>(N7C_NOPAIR);
		

			int cnt = 0;
			for(int i = card_i-1; i >= 0; i--)
			{
				m_RemainCard[cnt++] = m_sortCard[i];
				
			}
			
			m_RuleComplteCard[rcnt++] = m_sortCard[card_i];
		}

	}
}

/* 클라이언트 */
CString CCardRule::get_RuleValue_to_Name()
{
	static const char rulename[20][50] = { "TT","탑", "원페어", "투페어", "트리플", "스트레이트", "백스트레이트", "마운틴", 
						  "플러시", "풀하우스", "포카드", "스트레이트 플러시", "백스트레이트 플러시",
						  "크로바 로티플", "하트 로티플", "다이아 로티플","로얄 스트레이트 플러시" 
	};

	static const char cardname[14][5] = { "2","3","4","5","6","7","8","9","10","J","Q","K","Ace" };

	int value = m_SevenValue;
	if(m_SecondSevenValue > 0)
	{
		value = m_SecondSevenValue;
	}
	

	CString str=_T("");
	switch( value )
	{
	case N7C_CROYAL:
	case N7C_HROYAL:
	case N7C_DROYAL:
	case N7C_SROYAL:
		{
			str = rulename[value];
		} break;
	case N7C_BSFLUSH:
		{
			str = rulename[value];
		} break;
	case N7C_SFULSH:
		{
			str.Format("%s %s", cardname[m_stRuleData.StrFsh%13], rulename[value]);
		} break;
	case N7C_QUADS:
		{
			str.Format("%s %s", cardname[m_stRuleData.Poker[0]%13], rulename[value]);
		} break;
	case N7C_TITLE:
		{
			str.Format("%s, %s %s", cardname[m_stRuleData.FullH[0]%13], cardname[m_stRuleData.FullH[1]%13], rulename[value]);
		} break;

	case N7C_FLUSH:
		{
			str.Format("%s %s", cardname[m_stRuleData.Fsh%13], rulename[value]);
		} break;
	case N7C_MOUNT:
	case N7C_BACKS:
		{
			str.Format("%s", rulename[value]);
		} break;
	case N7C_STRAIGHT:
		{
			str.Format("%s %s", cardname[m_stRuleData.Straight%13], rulename[value]);
		} break;
	case N7C_TRIPLE:
		{
			str.Format("%s %s", cardname[m_stRuleData.Trip[0]%13], rulename[value]);
		} break;
	case N7C_TWOPAIR:
		{
			str.Format("%s, %s %s", cardname[m_stRuleData.Onep[0]%13], cardname[m_stRuleData.Onep[1]%13], rulename[value]);
		} break;
	case N7C_ONEPAIR:
		{
			str.Format("%s %s", cardname[ m_stRuleData.Onep[0]%13], rulename[value]);
		} break;
	case N7C_NOPAIR:
		{
			str.Format("%s%s", cardname[m_stRuleData.mang%13], rulename[value]);
		} break;
	}
	return str;
}

CString CCardRule::get_RuleValue_to_Name2()
{
	static const char rulename[20][50] = { "TT","탑", "원페어", "투페어", "트리플", "줄", "백줄", "마운틴", 
						  "플러시", "집", "포카드", "스티플", "백스티플",
						  "♧로티플", "♡로티플", "◇로티플","로티플" 
	};

	static const char cardname[14][5] = { "2","3","4","5","6","7","8","9","10","J","Q","K","Ace" };

	int value = m_SevenValue;
	if(m_SecondSevenValue > 0)
	{
		value = m_SecondSevenValue;
	}
	

	CString str=_T("");
	switch( value )
	{
	case N7C_CROYAL:
	case N7C_HROYAL:
	case N7C_DROYAL:
	case N7C_SROYAL:
		{
			str = rulename[value];
		} break;
	case N7C_BSFLUSH:
		{
			str = rulename[value];
		} break;
	case N7C_SFULSH:
		{
			str.Format("%s %s", cardname[m_stRuleData.StrFsh%13], rulename[value]);
		} break;
	case N7C_QUADS:
		{
			str.Format("%s %s", cardname[m_stRuleData.Poker[0]%13], rulename[value]);
		} break;
	case N7C_TITLE:
		{
			str.Format("%s, %s %s", cardname[m_stRuleData.FullH[0]%13], cardname[m_stRuleData.FullH[1]%13], rulename[value]);
		} break;

	case N7C_FLUSH:
		{
			str.Format("%s %s", cardname[m_stRuleData.Fsh%13], rulename[value]);
		} break;
	case N7C_MOUNT:
	case N7C_BACKS:
		{
			str.Format("%s", rulename[value]);
		} break;
	case N7C_STRAIGHT:
		{
			str.Format("%s %s", cardname[m_stRuleData.Straight%13], rulename[value]);
		} break;
	case N7C_TRIPLE:
		{
			str.Format("%s %s", cardname[m_stRuleData.Trip[0]%13], rulename[value]);
		} break;
	case N7C_TWOPAIR:
		{
			str.Format("%s, %s %s", cardname[m_stRuleData.Onep[0]%13], cardname[m_stRuleData.Onep[1]%13], rulename[value]);
		} break;
	case N7C_ONEPAIR:
		{
			str.Format("%s %s", cardname[ m_stRuleData.Onep[0]%13], rulename[value]);
		} break;
	case N7C_NOPAIR:
		{
			str.Format("%s%s", cardname[m_stRuleData.mang%13], rulename[value]);
		} break;
	}
	return str;
}

//win 결과 창 족보
CString CCardRule::get_RuleValue_to_Name3()
{
	set_EqualCard();

	static const char rulename[20][50] = { "TT","탑", "원페어", "투페어", "트리플", "줄", "백줄", "마운틴", 
						  "플러시", "집", "포카드", "스티플", "백스티플",
						  "♧로티플", "♡로티플", "◇로티플","로티플" 
	};

	static const char cardname[14][5] = { "2","3","4","5","6","7","8","9","10","J","Q","K","Ace" };

	int value = m_SevenValue;
	if(m_SecondSevenValue > 0)
	{
		value = m_SecondSevenValue;
	}
	

	CString str=_T("");
	switch( value )
	{
	case N7C_CROYAL:
	case N7C_HROYAL:
	case N7C_DROYAL:
	case N7C_SROYAL:
	case N7C_BSFLUSH:
	case N7C_SFULSH:
	case N7C_FLUSH:
	case N7C_MOUNT:
	case N7C_BACKS:
	case N7C_STRAIGHT:
		{
			str.Format("[%s,%s,%s,%s,%s] %s",cardname[m_EqualCard[0]%13],
										 cardname[m_EqualCard[1]%13],
										 cardname[m_EqualCard[2]%13],
										 cardname[m_EqualCard[3]%13],
										 cardname[m_EqualCard[4]%13], rulename[value]);
		} break;
	case N7C_QUADS:
		{
			str.Format("%s %s", cardname[m_stRuleData.Poker[0]%13], rulename[value]);
		} break;
	case N7C_TITLE:
		{
			str.Format("%s, %s %s", cardname[m_stRuleData.FullH[0]%13], cardname[m_stRuleData.FullH[1]%13], rulename[value]);
		} break;		
	case N7C_TRIPLE:
		{
			str.Format("%s %s", cardname[m_stRuleData.Trip[0]%13], rulename[value]);
		} break;
	case N7C_TWOPAIR:
		{
			str.Format("%s, %s %s", cardname[m_stRuleData.Onep[0]%13], cardname[m_stRuleData.Onep[1]%13], rulename[value]);
		} break;
	case N7C_ONEPAIR:
		{
			str.Format("%s %s", cardname[ m_stRuleData.Onep[0]%13], rulename[value]);
		} break;
	case N7C_NOPAIR:
		{
			str.Format("%s%s", cardname[m_stRuleData.mang%13], rulename[value]);
		} break;
	}
	return str;
}


//[1,2,3,4,5]식의 족보 
//게임정보창, 전적에 사용 함
CString CCardRule::get_RuleValue_to_Name_HaveNumber()
{
	set_EqualCard();

	static const char rulename[20][50] = { "TT","탑", "원페어", "투페어", "트리플", "줄", "백줄", "마운틴", 
						  "플러시", "집", "포카드", "스티플", "백줄플러시",
						  "♧로티플", "♡로티플", "◇로티플","로티플" 
	};

	static const char cardname[14][5] = { "2","3","4","5","6","7","8","9","10","J","Q","K","A" };

	CString str=_T("");
	int value = m_SevenValue;
	if(m_SecondSevenValue > 0)
	{
		value = m_SecondSevenValue;		
	}
	

	
	switch( value )
	{
	case N7C_CROYAL:
	case N7C_HROYAL:
	case N7C_DROYAL:
	case N7C_SROYAL:
	case N7C_BSFLUSH:		
	case N7C_SFULSH:
	case N7C_TITLE:
	case N7C_FLUSH:
	case N7C_MOUNT:	
	case N7C_STRAIGHT:		
	case N7C_TWOPAIR:
	case N7C_ONEPAIR:	
		{
			str.Format("[%s,%s,%s,%s,%s] %s",cardname[m_EqualCard[0]%13],
										 cardname[m_EqualCard[1]%13],
										 cardname[m_EqualCard[2]%13],
										 cardname[m_EqualCard[3]%13],
										 cardname[m_EqualCard[4]%13], rulename[value]);
		}break;

	case N7C_BACKS:
		{
			str.Format("[%s,%s,%s,%s,%s] %s",cardname[m_EqualCard[0]%13],
										 cardname[m_EqualCard[4]%13],
										 cardname[m_EqualCard[3]%13],
										 cardname[m_EqualCard[2]%13],
										 cardname[m_EqualCard[1]%13], rulename[value]);
		}break;
		
	case N7C_QUADS:
		{
			str.Format("%s %s", cardname[m_stRuleData.Poker[0]%13], rulename[value]);
		} break;	
		
	case N7C_TRIPLE:
		{
			str.Format("%s %s", cardname[m_stRuleData.Trip[0]%13], rulename[value]);
		} break;
		
	
	case N7C_NOPAIR:
		{
			str.Format("%s%s", cardname[m_stRuleData.mang%13], rulename[value]);
		} break;		
	}
	
	return str;
}

CString CCardRule::get_RuleValue_to_Name_for_LogDB(int nOneCard)
{
	//LOG DB 에 족보 남기기 위한 3자리 족보 코드 생성(2005.06.28)
	//인수 nOneCard = 족보를 이루는 카드중 한장 (스티플, 플러쉬 무늬 판별을 위해 필요)
	/*	
	-저장 방식 
	
	로티플:			공백,무늬,족보
	스티플:	    	숫자,무늬,족보
	포카드:   		숫자,공백,족보
	풀하우스:	 	숫자,숫자,족보
	플러쉬:     	숫자,무늬,족보
	스트레이트:		숫자,공백,족보
	트리플:      	숫자,공백,족보
	투페어:        	숫자,숫자,족보
	원페어:        	숫자,공백,족보
	노페어:         숫자,공백,족보
	
	-족보명칭
	R (로티플)	P (플러쉬)	O (원페어)
	S (스티플)	T (스트레이트)	N (노페어)
	F (포카드)	B (봉)	
	H (풀하우스)	W (투페어)
	
	하트(h)	스패이드(s) 크로바(c) 다이아(d)
	*/


	static const char rulename[20][50] = { "TT",", ,N", ", ,O", ",W", ", ,B", ", ,T", "5, ,T", "A, ,T", 
						  "P", ",H", ", ,F", "S", "S",
						  " ,c,R", " ,h,R", " ,d,R"," ,s,R" 
	};

	static const char cardname[14][5] = { "2","3","4","5","6","7","8","9","0","J","Q","K","A" };

	int value = m_SevenValue;
	if(m_SecondSevenValue > 0)
	{
		value = m_SecondSevenValue;
	}
	

	CString str=_T("");
	switch( value )
	{
	case N7C_CROYAL:
	case N7C_HROYAL:
	case N7C_DROYAL:
	case N7C_SROYAL:
		{
			str = rulename[value];
		} break;
	case N7C_BSFLUSH:
		{
			//무늬는 
			char cSuit=' ';
			if (nOneCard<0) cSuit=' ';
			else if (nOneCard/13==0) cSuit='c';
			else if (nOneCard/13==1) cSuit='h';
			else if (nOneCard/13==2) cSuit='d';
			else if (nOneCard/13==3) cSuit='s';

			str.Format("5,%c,%s",cSuit, rulename[value]);
			//str = rulename[value];
		} break;
	case N7C_SFULSH:
		{
			//무늬는 
			char cSuit=' ';
			if (nOneCard<0) cSuit=' ';
			else if (nOneCard/13==0) cSuit='c';
			else if (nOneCard/13==1) cSuit='h';
			else if (nOneCard/13==2) cSuit='d';
			else if (nOneCard/13==3) cSuit='s';
			str.Format("%s,%c,%s", cardname[m_stRuleData.StrFsh%13], cSuit, rulename[value]);
		} break;
	case N7C_QUADS:
		{
			str.Format("%s%s", cardname[m_stRuleData.Poker[0]%13], rulename[value]);
		} break;
	case N7C_TITLE:
		{
			str.Format("%s,%s%s", cardname[m_stRuleData.FullH[0]%13], cardname[m_stRuleData.FullH[1]%13], rulename[value]);
		} break;

	case N7C_FLUSH:
		{
			//무늬는 
			char cSuit=' ';
			if (nOneCard<0) cSuit=' ';
			else if (nOneCard/13==0) cSuit='c';
			else if (nOneCard/13==1) cSuit='h';
			else if (nOneCard/13==2) cSuit='d';
			else if (nOneCard/13==3) cSuit='s';
			str.Format("%s,%c,%s", cardname[m_stRuleData.Fsh%13], cSuit, rulename[value]);
		} break;
	case N7C_MOUNT:
	case N7C_BACKS:
		{
			str.Format("%s", rulename[value]);
		} break;
	case N7C_STRAIGHT:
		{
			str.Format("%s%s", cardname[m_stRuleData.Straight%13], rulename[value]);
		} break;
	case N7C_TRIPLE:
		{
			str.Format("%s%s", cardname[m_stRuleData.Trip[0]%13], rulename[value]);
		} break;
	case N7C_TWOPAIR:
		{
			str.Format("%s,%s%s", cardname[m_stRuleData.Onep[0]%13], cardname[m_stRuleData.Onep[1]%13], rulename[value]);
		} break;
	case N7C_ONEPAIR:
		{
			str.Format("%s%s", cardname[ m_stRuleData.Onep[0]%13], rulename[value]);
		} break;
	case N7C_NOPAIR:
		{
			str.Format("%s%s", cardname[m_stRuleData.mang%13], rulename[value]);
		} break;
	}
	return str;
}

int	CCardRule::get_ramain_card_number()	//족보를 이루지 못한 카드 장수
{
	int cnt=0;
	for(int i=0; i<TOTAL_CARD; i++) 
	{
		if (m_RemainCard[i]==EMPTY_CARD) 
			return cnt;
		cnt++;
	}
	return cnt;
}

int CCardRule::get_rule_complete_card_number() //족보를 이루는 카드 장수 
{
	int cnt=0;
	for(int i=0; i<TOTAL_CARD; i++) 
	{
		if (m_RuleComplteCard[i]==EMPTY_CARD) 
			return cnt;
		cnt++;
	}
	return cnt;

}


int CCardRule::get_Second_Value()
{

	int value = m_SevenValue;
	if(m_SecondSevenValue > 0)
	{
		value = m_SecondSevenValue;
	}
	
	int nval = 0;

	CString str=_T("");
	switch( value )
	{
	case N7C_CROYAL:
	case N7C_HROYAL:
	case N7C_DROYAL:
	case N7C_SROYAL:
		{
			nval = 13;
		} break;
	case N7C_BSFLUSH:
		{
			nval = 13;
		} break;
	case N7C_SFULSH:
		{
			nval = m_stRuleData.StrFsh%13;
		} break;
	case N7C_QUADS:
		{
			nval = m_stRuleData.Poker[0]%13;
		} break;
	case N7C_TITLE:
		{
			nval = (m_stRuleData.FullH[0]%13) * 100  + m_stRuleData.FullH[1]%13;
		} break;

	case N7C_FLUSH:
		{
			nval = m_stRuleData.Fsh%13;
		} break;
	case N7C_MOUNT:
		{
			nval = 13;
		}break;
	case N7C_BACKS:
		{
			nval = 13;
		} break;
	case N7C_STRAIGHT:
		{
			nval = m_stRuleData.Straight%13;
		} break;
	case N7C_TRIPLE:
		{
			nval = m_stRuleData.Trip[0]%13;
		} break;
	case N7C_TWOPAIR:
		{
			nval = (m_stRuleData.Onep[0]%13) * 100  + m_stRuleData.Onep[1]%13;
		} break;
	case N7C_ONEPAIR:
		{
			nval = m_stRuleData.Onep[0]%13;
		} break;
	case N7C_NOPAIR:
		{
			nval = m_stRuleData.mang%13;
		} break;
	}

	return nval;
}

BOOL CCardRule::is_value_equal(int wincase)
{
	if (wincase<0 || wincase>=3) return FALSE;
	for (int i = 0; i < TOTAL_CARD ; i++){
		if( m_Extra_WinCard[wincase][i] > -1 ){			
			return TRUE;
		}
	}
	return FALSE;
}

//동일 족보 5장을 담아 둔다. 원페어 투페어 때문
void CCardRule::set_EqualCard()
{	
	memset( m_EqualCard, -1, sizeof(m_EqualCard));

	int i = 0 ;

	int value = m_SevenValue;

	if(m_SecondSevenValue > 0){
		value = m_SecondSevenValue;
	}

	switch( value )
	{
	case N7C_CROYAL:
	case N7C_HROYAL:
	case N7C_DROYAL:
	case N7C_SROYAL:	
	case N7C_BSFLUSH:		
	case N7C_SFULSH:		
	case N7C_QUADS:		
	case N7C_TITLE:
	case N7C_FLUSH:		
	case N7C_MOUNT:		
	case N7C_BACKS:		
	case N7C_STRAIGHT:		
	case N7C_TRIPLE:		
		{
			for( i = 0 ; i < TOTAL_CARD ; i ++ ){
				m_EqualCard[i] = m_RuleComplteCard[i];
			}
		} break;
	case N7C_TWOPAIR:
		{
			int cnt = 0;
			for( i = 0 ; i < TOTAL_CARD ; i ++ ){
				if( m_RuleComplteCard[i] > -1 ){
					m_EqualCard[i] = m_RuleComplteCard[i];
				}
			}

			m_EqualCard[4] = m_RuleComplteCard[0];

		} break;
	case N7C_ONEPAIR:
		{
			int cnt = 0;
			for( i = 0 ; i < TOTAL_CARD ; i ++ ){
				if( m_RuleComplteCard[i] > -1 ){
					m_EqualCard[i] = m_RuleComplteCard[i];
				}
			}			
			
			for( i = 0 ; i < 3; i ++ ){
				if( m_RemainCard[i+2] > -1 ){
					m_EqualCard[i+2] = m_RemainCard[i];		
				}
			}
		} break;
	case N7C_NOPAIR:
		{
			for( i = 0 ; i < 5 ; i ++ ){
				if( m_RemainCard[i] > -1 ){
					m_EqualCard[i+2] = m_RemainCard[i];
				}
			}
			
		} break;
	}	
}


void CCardRule::sort_Card_low(int Start, int End)
{
	for(int i = Start; i < End; i++) //오름차순.
	{
		for(int j = i; j < End; j++)
		{
			int nN1 = m_sortCard_low[i] % 13;
			int nP1 = m_sortCard_low[i] / 13;
			if( nN1 == 12 ){
				nN1 = 0;
			}
			else{
				nN1 = nN1 + 1;
			}		


			int nN2 = m_sortCard_low[j] % 13;
			int nP2 = m_sortCard_low[j] / 13;
			if( nN2 == 12 ){
				nN2 = 0;
			}
			else{
				nN2 = nN2 + 1;
			}
			
			// A
			if( nN1 > nN2 ){
				int temp = m_sortCard_low[i];
				m_sortCard_low[i] = m_sortCard_low[j];
				m_sortCard_low[j] = temp;	
			}				
			else if( nN1 == nN2 )
			{
				if(nP1 > nP2 )
				{
					int temp = m_sortCard_low[i];
					m_sortCard_low[i] = m_sortCard_low[j];
					m_sortCard_low[j] = temp;
				}
			}
		}
	}
	
	int x = 0;
}

int CCardRule::get_rule_complete_Low_card_number() //족보를 이루는 카드 장수 
{
	int cnt=0;
	for(int i=0; i<TOTAL_CARD; i++) 
	{
		if (m_RuleComplteCard_Low[i]==EMPTY_CARD) 
			return cnt;
		cnt++;
	}
	return cnt;

}

void CCardRule::get_cal_sevenLowRule()
{
	if( m_nTotalCard <= 0 )return;

//	m_RuleComplteCard_Low[TOTAL_CARD];  // 족보를 이루는 카드
//	m_RemainCard_Low[TOTAL_CARD];	    // 족보를 제외한 카드    

	memset( m_RemainCard_Low, -1, sizeof(m_RemainCard_Low));
	memset( m_RuleComplteCard_Low, -1, sizeof(m_RuleComplteCard_Low));
	m_SevenValue_low =-1;

	int cnt = 1 ;
	//한장 받고 시작
	m_RuleComplteCard_Low[0] = m_sortCard_low[0];

	BOOL bOK = FALSE;//족보를 이룰 카드를 다 구했는지
	
	for( int i = 0 ; i < m_nTotalCard ; i ++ ){
		
		int nN1 = m_sortCard_low[i] % 13;
		int nP1 = m_sortCard_low[i] / 13;
		if( nN1 == 12 ){
			nN1 = 0;
		}
		else{
			nN1 = nN1 + 1;
		}			

		bool Push =TRUE;
		for( int j = 0 ; j < cnt ; j ++ ){			
			
			int nN2 = m_RuleComplteCard_Low[j] % 13;
			int nP2 = m_RuleComplteCard_Low[j] / 13;
			if( nN2 == 12 ){
				nN2 = 0;
			}
			else{
				nN2 = nN2 + 1;
			}
			
			//같은 숫자가 아닌것들을 담는다. 

			if( nN1 < nN2 ){
				m_RuleComplteCard_Low[j] = m_sortCard_low[i];
				Push = FALSE;
			}
			else if ( nN1 == nN2 ) {
				if( nP2 > nP1 ){
					m_RuleComplteCard_Low[j] = m_sortCard_low[i];					
				}
				Push = FALSE;				
			}			
		}
		
		if( Push == TRUE ){
			m_RuleComplteCard_Low[cnt] = m_sortCard_low[i];
			cnt++;
			if( cnt == 5 ){
				bOK = TRUE;
			}
		}
		if( bOK )break;
	}

	//탑이 되는 숫자를 담아 둔다.
	if( cnt == 5 ){
		m_SevenValue_low = m_RuleComplteCard_Low[cnt-1];
	}	

	//족보를 이루고 남은 카드를 담아 둔다.
	int re_cnt = 0;

	for(int i = 0 ; i < m_nTotalCard ; i ++ ){
		BOOL bSame = FALSE;
		for( int j = 0 ; j < cnt ; j ++){
			if( m_RuleComplteCard_Low[j] == m_sortCard_low[i] ){
				bSame = TRUE;
			}
		}

		if( bSame == FALSE ){
			m_RemainCard_Low[re_cnt] = m_sortCard_low[i];
			re_cnt ++;
		}
	}
}
CString CCardRule::get_LowRuleValue_to_Name()
{
	CString strName = "";
	if( m_SevenValue_low < 0 || m_SevenValue_low > 51 )return strName = "무탑";

	static const char cardname[14][5] = { "2","3","4","5","6","7","8","9","10","J","Q","K","A" };
	
	strName = cardname[m_SevenValue_low%13];
	strName +="탑";

	return strName;
}

//동일 족보 5장을 담아 둔다.
void CCardRule::set_Low_EqualCard()
{	
	memset( m_EqualCard_low, -1, sizeof(m_EqualCard_low));
	int i = 0 ;	
	for( i = 0 ; i < TOTAL_CARD ; i ++ ){

		if( m_RuleComplteCard_Low[i] > -1 ){
			m_EqualCard_low[i] = m_RuleComplteCard_Low[i];
		}		
	}	
}

//동일패가 있는가?
BOOL CCardRule::is_Low_value_equal(int wincase)
{
	if (wincase<0 || wincase>=3) return FALSE;
	for (int i = 0; i < TOTAL_CARD ; i++){
		if( m_Extra_WinCard_Low[wincase][i] > -1 ){			
			return TRUE;
		}
	}
	return FALSE;
}
	
//[1,2,3,4,5]식의 족보 	
CString CCardRule::get_Low_RuleValue_to_Name_HaveNumber()
{
	set_Low_EqualCard();
	CString strName = "";
	static const char cardname[14][5] = { "2","3","4","5","6","7","8","9","10","J","Q","K","A" };
	
	if( m_SevenValue_low  > -1 ){

		strName.Format("[%s,%s,%s,%s,%s] %s",cardname[m_EqualCard_low[0]%13],
		cardname[m_EqualCard_low[1]%13],
		cardname[m_EqualCard_low[2]%13],
		cardname[m_EqualCard_low[3]%13],
		cardname[m_EqualCard_low[4]%13], get_LowRuleValue_to_Name());
	}
	else{

		strName="[";
		CString str,str1;
		for( int i = 0 ; i < TOTAL_CARD ; i ++ ){
			if( m_EqualCard_low[i] > -1 ){
				str.Format("%s",cardname[m_EqualCard_low[i]%13]);
				str+=",";
				strName +=str;
			}
		}
		strName.Delete(strName.GetLength()-1);	
		strName +="]";
		str1 = get_LowRuleValue_to_Name();
		strName +=str1;
	}

	return strName;
}