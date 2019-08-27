// SevenRuleMan.cpp: implementation of the CSevenRuleMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SevenRuleMan.h"
#include "UserStruct.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSevenRuleMan::CSevenRuleMan()
{
	Init();

	//	m_RuleKind= RULE_KIND_ORIGINAL_NETMARBLE;		//넷마블룰을 기본으로한다.
	m_RuleKind= RULE_KIND_HANGAME; //한게임룰을 기본으로 한다. (2008.06.17일 적용)

}

CSevenRuleMan::~CSevenRuleMan()
{

}

void CSevenRuleMan::Init()
{
	for(int i = 0; i < TOTAL_USER; i++) {
		m_cRule[i].Init();
	}

	ZeroMemory(m_Ranking, sizeof(m_Ranking));
	ZeroMemory(m_Ranking_low, sizeof(m_Ranking_low));
	
	m_BoardCard = -1;
}


void CSevenRuleMan::ClearAll()
{
	// 무효화 
	for(int i = 0; i < TOTAL_USER; i++) {
		m_cRule[i].set_Init_Valid();
	}
}

/* 유저별로 카드 셋 */
void CSevenRuleMan::set_total_Card(int PNum, int pCard[], int cardNum)
{
	if(PNum < 0 || PNum >= TOTAL_USER) return;
	m_cRule[PNum].Init();
	m_cRule[PNum].set_Card( &pCard[0], cardNum );
}
	
/* 모든유저 룰 계산 */
void CSevenRuleMan::get_cal_rule()
{
	for(int i=0; i < TOTAL_USER; i++)
	{
		if( m_cRule[i].isValid() ) 
		{
			m_cRule[i].cal_Card();
		}
	}
} 

/* 유저 각각 룰 계산 */
void CSevenRuleMan::get_cal_rule_user(int pnum)
{
	if( !m_cRule[pnum].isValid() )  return;
	
	m_cRule[pnum].cal_Card();
}

/* 유저 등수 알아오기  ex) 1등 */
int CSevenRuleMan::get_Winner(int rank /* = 1 */) 
{
	for(int i = 0; i < TOTAL_USER; i++) //오름차순.
	{
		if( !m_cRule[i].isValid() ) continue;
		if(m_Ranking[i] == rank) return i;
	}

	return -1;
}

int CSevenRuleMan::get_Rank(int pnum)
{
	for(int i = 0; i < TOTAL_USER; i++) //오름차순.
	{
		if( !m_cRule[i].isValid() ) continue;
		if(i == pnum) return m_Ranking[i];
	}

	return -1;
}

/* 랭킹 구하기 (Server용) */
void CSevenRuleMan::get_cal_Winner_Server()
{
	int cnt = 0;
	//int ncntlose = 0;
	//int ncntwin = 0;
	//int ncntequal = 0;

	ZeroMemory(m_Ranking, sizeof(m_Ranking));
	for(int i = 0; i < TOTAL_USER; i++) //오름차순.
	{
		cnt = 1;
		//ncntlose = 0;
		//ncntwin = 0;
		//ncntequal = 0;
		if( !m_cRule[i].isValid() ) {continue; }
	
		for(int j=0; j < TOTAL_USER; j++)
		{			
			if( i==j || !m_cRule[j].isValid() )	{ continue; }
			// 순위결정
			if(m_cRule[i].get_sevenValue() < m_cRule[j].get_sevenValue() )
			{
				cnt++;
			}
			else if(m_cRule[i].get_sevenValue() == m_cRule[j].get_sevenValue() )
			{
				// 조건검사
				switch( m_cRule[i].get_sevenValue() )
				{
				case CCardRule::N7C_CROYAL:
				case CCardRule::N7C_HROYAL:
				case CCardRule::N7C_DROYAL:
				case CCardRule::N7C_SROYAL:
					{
//#if defined(__NEWPOKER__)
//						if( !Remain_card_Winner_Server(i, j, cnt, ncntlose, ncntwin, ncntequal) )
//#else	// defined(__NEWPOKER__)
						if( !Remain_card_Winner_Server(i, j, cnt) )
//#endif	// defined(__NEWPOKER__)
						{
							// 카드가 모두 같다면 무뉘비교
							if(m_cRule[i].m_stRuleData.Royal < m_cRule[j].m_stRuleData.Royal)
							{
								cnt++;
							}
						}
						
					} break;
				case CCardRule::N7C_BSFLUSH:
					{
//#if defined(__NEWPOKER__)
//						if( !Remain_card_Winner_Server(i, j, cnt, ncntlose, ncntwin, ncntequal) )
//#else	// defined(__NEWPOKER__)
						if( !Remain_card_Winner_Server(i, j, cnt) )
//#endif	// defined(__NEWPOKER__)
						{
							// 카드가 모두 같다면 무뉘비교
							if(m_cRule[i].m_stRuleData.BackStrFsh < m_cRule[j].m_stRuleData.BackStrFsh)
							{
								cnt++;
							}
						}
						
					} break;
				case CCardRule::N7C_SFULSH:
					{
						if(m_cRule[i].m_stRuleData.StrFsh%13 < m_cRule[j].m_stRuleData.StrFsh%13)
						{
							cnt++;
						}
						else if(m_cRule[i].m_stRuleData.StrFsh%13 == m_cRule[j].m_stRuleData.StrFsh%13)
						{
//#if defined(__NEWPOKER__)
//							if( !Remain_card_Winner_Server(i, j, cnt, ncntlose, ncntwin, ncntequal) )
//#else	// defined(__NEWPOKER__)
							if( !Remain_card_Winner_Server(i, j, cnt) )
//#endif	// defined(__NEWPOKER__)
							{
								// 카드가 모두 같다면 무뉘비교
								if(m_cRule[i].m_stRuleData.StrFsh < m_cRule[j].m_stRuleData.StrFsh)
								{
									cnt++;
								}
							
							}
						}
						
					} break;
				case CCardRule::N7C_QUADS:
					{
						if(m_cRule[i].m_stRuleData.Poker[0]%13 < m_cRule[j].m_stRuleData.Poker[0]%13)
						{
							cnt++;
						}
						
						
					} break;
				case CCardRule::N7C_TITLE:
					{
						
						if(m_cRule[i].m_stRuleData.FullH[0]%13 < m_cRule[j].m_stRuleData.FullH[0]%13)
						{
							cnt++;
							break;
						} 
						else if(m_cRule[i].m_stRuleData.FullH[0]%13 == m_cRule[j].m_stRuleData.FullH[0]%13)
						{
							// 두번째 값비교
							if(m_cRule[i].m_stRuleData.FullH[1]%13 < m_cRule[j].m_stRuleData.FullH[1]%13)
							{
								cnt++;
								break;
							}
							else if(m_cRule[i].m_stRuleData.FullH[1]%13 == m_cRule[j].m_stRuleData.FullH[1]%13)
							{
								
//#if defined(__NEWPOKER__)
//								if( !Remain_card_Winner_Server(i, j, cnt, ncntlose, ncntwin,ncntequal) )
//#else	// defined(__NEWPOKER__)
								if( !Remain_card_Winner_Server(i, j, cnt) )
//#endif	// defined(__NEWPOKER__)
								{
									// 카드가 모두 같다면 무뉘비교
									if(m_cRule[i].m_stRuleData.FullH[0] < m_cRule[j].m_stRuleData.FullH[0])
									{
										cnt++;
									}
								}
							}
						}
						
						
					} break;
					
				case CCardRule::N7C_FLUSH:
					{
						bool bflush = false;
						if(m_cRule[i].m_stRuleData.Fsh%13 < m_cRule[j].m_stRuleData.Fsh%13)
						{
							cnt++;
						}
						else if(m_cRule[i].m_stRuleData.Fsh%13 == m_cRule[j].m_stRuleData.Fsh%13)
						{
											
							for(int k=1; k<5; k++)
							{
								if(m_cRule[i].m_RuleComplteCard[k]%13 < m_cRule[j].m_RuleComplteCard[k]%13)
								{
									cnt++;
									bflush = true;
									break;
								}	
								else if(m_cRule[i].m_RuleComplteCard[k]%13 > m_cRule[j].m_RuleComplteCard[k]%13)
								{
									bflush = true;
									break;
								}
								
							}

//#if defined(__NEWPOKER__)
//							if( !bflush && !Remain_card_Winner_Server(i, j, cnt, ncntlose, ncntwin, ncntequal) )
//#else	// defined(__NEWPOKER__)
							if( !bflush && !Remain_card_Winner_Server(i, j, cnt) )
//#endif	// defined(__NEWPOKER__)
							{
								// 카드가 모두 같다면 무뉘비교
								if(m_cRule[i].m_stRuleData.Fsh < m_cRule[j].m_stRuleData.Fsh)
								{
								
									cnt++;
								}
								
							}
						}
						
					} break;
				case CCardRule::N7C_MOUNT:
					{
//#if defined(__NEWPOKER__)
//						if( !Remain_card_Winner_Server(i, j, cnt, ncntlose, ncntwin, ncntequal) )
//#else	// defined(__NEWPOKER__)
						if( !Remain_card_Winner_Server(i, j, cnt) )
//#endif	// defined(__NEWPOKER__)
						{
							// 카드가 모두 같다면 무뉘비교
							if(m_cRule[i].m_stRuleData.MStraight < m_cRule[j].m_stRuleData.MStraight)
							{
								cnt++;
							
							}

							// 폭탄카드
							else if(m_cRule[i].m_stRuleData.MStraight > m_cRule[j].m_stRuleData.MStraight)
							{
							
							}
							else
							{
								if(m_cRule[i].m_RuleComplteCard[1] < m_cRule[j].m_RuleComplteCard[1])
								{
									cnt++;
								
								}
								
							}
						}
						
					} break;
					
				case CCardRule::N7C_BACKS:
					{
//#if defined(__NEWPOKER__)
//						if( !Remain_card_Winner_Server(i, j, cnt, ncntlose, ncntwin, ncntequal) )
//#else	// defined(__NEWPOKER__)
						if( !Remain_card_Winner_Server(i, j, cnt) )
//#endif	// defined(__NEWPOKER__)
						{
							// 카드가 모두 같다면 무뉘비교
							if(m_cRule[i].m_stRuleData.BStraight < m_cRule[j].m_stRuleData.BStraight)
							{
								cnt++;
							
							}
							else if(m_cRule[i].m_stRuleData.BStraight > m_cRule[j].m_stRuleData.BStraight)
							{
							
								
							}
							// 폭탄카드
							else
							{
							
								if(m_cRule[i].m_RuleComplteCard[1] < m_cRule[j].m_RuleComplteCard[1])
								{
									cnt++;
							
								}
							
							
							}
								
						} 
					} break;
					
				case CCardRule::N7C_STRAIGHT:
					{
						if(m_cRule[i].m_stRuleData.Straight%13 < m_cRule[j].m_stRuleData.Straight%13)
						{
							cnt++;
						}
						else if(m_cRule[i].m_stRuleData.Straight%13 == m_cRule[j].m_stRuleData.Straight%13)
						{
//#if defined(__NEWPOKER__)
//							if( !Remain_card_Winner_Server(i, j, cnt, ncntlose, ncntwin, ncntequal) )
//#else	// defined(__NEWPOKER__)
							if( !Remain_card_Winner_Server(i, j, cnt) )
//#endif	// defined(__NEWPOKER__)
							{
								// 카드가 모두 같다면 무뉘비교
								if(m_cRule[i].m_stRuleData.Straight < m_cRule[j].m_stRuleData.Straight)
								{
									cnt++;
								
								}
								else if(m_cRule[i].m_stRuleData.Straight > m_cRule[j].m_stRuleData.Straight)
								{
								
								}
								// 폭탄카드
								else
								{
								
									if(m_cRule[i].m_RuleComplteCard[1] < m_cRule[j].m_RuleComplteCard[1])
									{
										cnt++;
								
									}
								
									
								}
							} 
						}
						
					} break;
				case CCardRule::N7C_TRIPLE:
					{
						if(m_cRule[i].m_stRuleData.Trip[0]%13 < m_cRule[j].m_stRuleData.Trip[0]%13)
						{
							cnt++;
						}
						
					} break;
				case CCardRule::N7C_TWOPAIR:
					{
						if(m_cRule[i].m_stRuleData.Onep[0]%13 < m_cRule[j].m_stRuleData.Onep[0]%13)
						{
							cnt++;
							break;
						} 
						if(m_cRule[i].m_stRuleData.Onep[0]%13 == m_cRule[j].m_stRuleData.Onep[0]%13)
						{
							
							if(m_cRule[i].m_stRuleData.Onep[1]%13 < m_cRule[j].m_stRuleData.Onep[1]%13)
							{
								cnt++;
								break;
							}
							else if(m_cRule[i].m_stRuleData.Onep[1]%13 == m_cRule[j].m_stRuleData.Onep[1]%13)
							{
								
//#if defined(__NEWPOKER__)
//								if( !Remain_card_Winner_Server(i, j, cnt, ncntlose, ncntwin, ncntequal) )
//#else	// defined(__NEWPOKER__)
								if( !Remain_card_Winner_Server(i, j, cnt) )
//#endif	// defined(__NEWPOKER__)
								{
									// 카드가 모두 같다면 무뉘비교
									if(m_cRule[i].m_stRuleData.Onep[0] < m_cRule[j].m_stRuleData.Onep[0])
									{
										cnt++;
									
									}
									else if(m_cRule[i].m_stRuleData.Onep[0] > m_cRule[j].m_stRuleData.Onep[0])
									{
									
									}
									// 폭탄카드
									else
									{
									
										if(m_cRule[i].m_RuleComplteCard[1] < m_cRule[j].m_RuleComplteCard[1])
										{
											cnt++;
									
										}
									
									}
				
								}
							}
						}
						
					} break;
				case CCardRule::N7C_ONEPAIR:
					{
						if(m_cRule[i].m_stRuleData.Onep[0]%13 < m_cRule[j].m_stRuleData.Onep[0]%13)
						{
							cnt++;
							break;
						} 
						else if(m_cRule[i].m_stRuleData.Onep[0]%13 == m_cRule[j].m_stRuleData.Onep[0]%13)
						{
							
//#if defined(__NEWPOKER__)
//							if( !Remain_card_Winner_Server(i, j, cnt, ncntlose, ncntwin, ncntequal) )
//#else	// defined(__NEWPOKER__)
							if( !Remain_card_Winner_Server(i, j, cnt) )
//#endif	// defined(__NEWPOKER__)
							{
								// 카드가 모두 같다면 무뉘비교
								if(m_cRule[i].m_stRuleData.Onep[0] < m_cRule[j].m_stRuleData.Onep[0])
								{
									cnt++;
								
								}
								else if(m_cRule[i].m_stRuleData.Onep[0] > m_cRule[j].m_stRuleData.Onep[0])
								{
								
								}
								// 폭탄카드
								else
								{
								
									if(m_cRule[i].m_RuleComplteCard[1] < m_cRule[j].m_RuleComplteCard[1])
									{
										cnt++;
								
									}
								

								}
							}
						}
						
					} break;
				case CCardRule::N7C_NOPAIR:
					{
						if(m_cRule[i].m_stRuleData.mang%13 < m_cRule[j].m_stRuleData.mang%13)
						{
							cnt++;
							break;
						} 
						else if(m_cRule[i].m_stRuleData.mang%13 == m_cRule[j].m_stRuleData.mang%13)
						{
							
//#if defined(__NEWPOKER__)
//							if( !Remain_card_Winner_Server(i, j, cnt, ncntlose, ncntwin, ncntequal) )
//#else	// defined(__NEWPOKER__)
							if( !Remain_card_Winner_Server(i, j, cnt) )
//#endif	// defined(__NEWPOKER__)
							{
								// 카드가 모두 같다면 무뉘비교
								if(m_cRule[i].m_stRuleData.mang < m_cRule[j].m_stRuleData.mang)
								{
									cnt++;
								}
								else if(m_cRule[i].m_stRuleData.mang > m_cRule[j].m_stRuleData.mang)
								{
									
								}
								// 폭탄카드
								else
								{
									if(m_cRule[i].m_RemainCard[0] < m_cRule[j].m_RemainCard[0])
									{
										cnt++;
									
									}
								}
							}
						}
						
					} break;
				}
			}
		}
		m_Ranking[i] = cnt;
	}
}


/* (Server용) */
//#if defined(__NEWPOKER__)
//bool CSevenRuleMan::Remain_card_Winner_Server(int i, int j, int &cnt, int &ncntlose, int &ncntwin, int &ncntequal)
//#else	// defined(__NEWPOKER__)
bool CSevenRuleMan::Remain_card_Winner_Server(int i, int j, int &cnt)
//#endif	// defined(__NEWPOKER__)
{
	bool bExWincard = false;

	// 비교할 카드 갯수
	int nCompareCard = 0;
	
	if (m_RuleKind==RULE_KIND_ORIGINAL_NETMARBLE)
	{
		// 정통 넷마블 룰 (동일족보비교 -> 모든 나머지카드 들의 숫자를 비교 -> 패를 이루는 카드들의 무늬 비교)
		// 넷마블룰은 남아있는 모든 카드들의 숫자를 비교한다.

		// 모든카드 비교 
		nCompareCard = m_cRule[i].m_nTotalCard;

	}
	else if (m_RuleKind==RULE_KIND_HANGAME)
	{
		// 한게임 룰 (동일족보비교 -> 나머지카드 중 낮은 숫자 2장 버리고 난 카드들의 숫자비교 -> 패를 이루는 카드들의 무늬 비교)
		// 한게임은 남아있는 카드 중 낮은 숫자 2장은 버리고 비교한다.
		ASSERT(m_cRule[i].get_ramain_card_number()==m_cRule[j].get_ramain_card_number());
		ASSERT(m_cRule[i].get_TotalCard()-m_cRule[i].get_ramain_card_number()==m_cRule[j].get_rule_complete_card_number());
		
		nCompareCard = min(m_cRule[i].get_TotalCard(),5) - m_cRule[i].get_rule_complete_card_number();		

		ASSERT(nCompareCard>=0);
		if (nCompareCard<=0) 
			return false;

	}

	for(int k=0; k<nCompareCard; k++)
	{
		if(m_cRule[i].m_RemainCard[k]%13 < m_cRule[j].m_RemainCard[k]%13)
		{
			cnt++;
			bExWincard = true;
			break;
		}
		else if(m_cRule[i].m_RemainCard[k]%13 > m_cRule[j].m_RemainCard[k]%13)
		{
			bExWincard = true;
			break;
		}
		
	}

	return 	bExWincard;
}


/* 랭킹 구하기 (Client용)*/
void CSevenRuleMan::get_cal_Winner_Client()
{
	int cnt = 0;
	int ncntlose = 0;
	int ncntwin = 0;
	int ncntequal = 0;

	ZeroMemory(m_Ranking, sizeof(m_Ranking));
	for(int i = 0; i < TOTAL_USER; i++) //오름차순.
	{
		cnt = 1;
		ncntlose = 0;
		ncntwin = 0;
		ncntequal = 0;
		if( !m_cRule[i].isValid() ) {continue; }
	
		for(int j=0; j < TOTAL_USER; j++)
		{			
			if( i==j || !m_cRule[j].isValid() )	{ continue; }
			// 순위결정
			if(m_cRule[i].get_sevenValue() < m_cRule[j].get_sevenValue() )
			{
				cnt++;
			}
			else if(m_cRule[i].get_sevenValue() == m_cRule[j].get_sevenValue() )
			{
				// 조건검사
				switch( m_cRule[i].get_sevenValue() )
				{
				case CCardRule::N7C_CROYAL:
				case CCardRule::N7C_HROYAL:
				case CCardRule::N7C_DROYAL:
				case CCardRule::N7C_SROYAL:
					{
						if( !Remain_card_Winner_Client(i, j, cnt, ncntlose, ncntwin, ncntequal) )
						{
							// 카드가 모두 같다면 무뉘비교
							if(m_cRule[i].m_stRuleData.Royal < m_cRule[j].m_stRuleData.Royal)
							{
								cnt++;
								m_cRule[i].m_Extra_WinCard[LOSE][ncntlose++] = m_cRule[i].m_stRuleData.Royal;
							}
							else
								m_cRule[i].m_Extra_WinCard[WIN][ncntwin++] = m_cRule[i].m_stRuleData.Royal;
						}
						
					} break;
				case CCardRule::N7C_BSFLUSH:
					{
						if( !Remain_card_Winner_Client(i, j, cnt, ncntlose, ncntwin, ncntequal) )
						{
							// 카드가 모두 같다면 무뉘비교
							if(m_cRule[i].m_stRuleData.BackStrFsh < m_cRule[j].m_stRuleData.BackStrFsh)
							{
								cnt++;
								m_cRule[i].m_Extra_WinCard[LOSE][ncntlose++] = m_cRule[i].m_stRuleData.BackStrFsh;
							}
							else
								m_cRule[i].m_Extra_WinCard[WIN][ncntwin++] = m_cRule[i].m_stRuleData.BackStrFsh;
						}
						
					} break;
				case CCardRule::N7C_SFULSH:
					{
						if(m_cRule[i].m_stRuleData.StrFsh%13 < m_cRule[j].m_stRuleData.StrFsh%13)
						{
							cnt++;
						}
						else if(m_cRule[i].m_stRuleData.StrFsh%13 == m_cRule[j].m_stRuleData.StrFsh%13)
						{
							if( !Remain_card_Winner_Client(i, j, cnt, ncntlose, ncntwin, ncntequal) )
							{
								// 카드가 모두 같다면 무뉘비교
								if(m_cRule[i].m_stRuleData.StrFsh < m_cRule[j].m_stRuleData.StrFsh)
								{
									m_cRule[i].m_Extra_WinCard[LOSE][ncntlose++] = m_cRule[i].m_stRuleData.StrFsh;
									cnt++;
								}
								else
									m_cRule[i].m_Extra_WinCard[WIN][ncntwin++] = m_cRule[i].m_stRuleData.StrFsh;

							}
						}
						
					} break;
				case CCardRule::N7C_QUADS:
					{
						if(m_cRule[i].m_stRuleData.Poker[0]%13 < m_cRule[j].m_stRuleData.Poker[0]%13)
						{
							cnt++;
						}
						
						
					} break;
				case CCardRule::N7C_TITLE:
					{
						
						if(m_cRule[i].m_stRuleData.FullH[0]%13 < m_cRule[j].m_stRuleData.FullH[0]%13)
						{
							cnt++;
							break;
						} 
						else if(m_cRule[i].m_stRuleData.FullH[0]%13 == m_cRule[j].m_stRuleData.FullH[0]%13)
						{
							// 두번째 값비교
							if(m_cRule[i].m_stRuleData.FullH[1]%13 < m_cRule[j].m_stRuleData.FullH[1]%13)
							{
								cnt++;
								break;
							}
							else if(m_cRule[i].m_stRuleData.FullH[1]%13 == m_cRule[j].m_stRuleData.FullH[1]%13)
							{
								
								if( !Remain_card_Winner_Client(i, j, cnt, ncntlose, ncntwin,ncntequal) )
								{
									// 카드가 모두 같다면 무뉘비교
									if(m_cRule[i].m_stRuleData.FullH[0] < m_cRule[j].m_stRuleData.FullH[0])
									{
										cnt++;
									}
								}
							}
						}
						
						
					} break;
					
				case CCardRule::N7C_FLUSH:
					{
						bool bflush = false;
						if(m_cRule[i].m_stRuleData.Fsh%13 < m_cRule[j].m_stRuleData.Fsh%13)
						{
							cnt++;
						}
						else if(m_cRule[i].m_stRuleData.Fsh%13 == m_cRule[j].m_stRuleData.Fsh%13)
						{
							
							m_cRule[i].m_Extra_WinCard[EQUAL][ncntequal++] = m_cRule[i].m_stRuleData.Fsh;
							
							for(int k=1; k<5; k++)
							{
								if(m_cRule[i].m_RuleComplteCard[k]%13 < m_cRule[j].m_RuleComplteCard[k]%13)
								{
									cnt++;
									bflush = true;
									m_cRule[i].m_Extra_WinCard[LOSE][ncntlose++] = m_cRule[i].m_RuleComplteCard[k];
									break;
								}	
								else if(m_cRule[i].m_RuleComplteCard[k]%13 > m_cRule[j].m_RuleComplteCard[k]%13)
								{
									bflush = true;
									m_cRule[i].m_Extra_WinCard[WIN][ncntwin++] = m_cRule[i].m_RuleComplteCard[k];
									break;
								}
								else
									m_cRule[i].m_Extra_WinCard[EQUAL][ncntequal++] = m_cRule[i].m_stRuleData.Fsh;

							}


							if( !bflush && !Remain_card_Winner_Client(i, j, cnt, ncntlose, ncntwin, ncntequal) )
							{
								// 카드가 모두 같다면 무뉘비교
								if(m_cRule[i].m_stRuleData.Fsh < m_cRule[j].m_stRuleData.Fsh)
								{
									m_cRule[i].m_Extra_WinCard[LOSE][ncntlose++] = m_cRule[i].m_stRuleData.Fsh;
									cnt++;
								}
								else
									m_cRule[i].m_Extra_WinCard[WIN][ncntwin++] = m_cRule[i].m_stRuleData.Fsh;
							}
						}
						
					} break;
				case CCardRule::N7C_MOUNT:
					{
						if( !Remain_card_Winner_Client(i, j, cnt, ncntlose, ncntwin, ncntequal) )
						{
							// 카드가 모두 같다면 무뉘비교
							if(m_cRule[i].m_stRuleData.MStraight < m_cRule[j].m_stRuleData.MStraight)
							{
								cnt++;
								m_cRule[i].m_Extra_WinCard[LOSE][ncntlose++] = m_cRule[i].m_stRuleData.MStraight;
							}

							// 폭탄카드
							else if(m_cRule[i].m_stRuleData.MStraight > m_cRule[j].m_stRuleData.MStraight)
							{
								m_cRule[i].m_Extra_WinCard[WIN][ncntwin++] = m_cRule[i].m_stRuleData.MStraight;
							}
							else
							{
								m_cRule[i].m_Extra_WinCard[EQUAL][ncntequal++] = m_cRule[i].m_stRuleData.MStraight;
								if(m_cRule[i].m_RuleComplteCard[1] < m_cRule[j].m_RuleComplteCard[1])
								{
									cnt++;
									m_cRule[i].m_Extra_WinCard[LOSE][ncntlose++] = m_cRule[i].m_RuleComplteCard[1];
								}
								else
									m_cRule[i].m_Extra_WinCard[WIN][ncntwin++] = m_cRule[i].m_RuleComplteCard[1];
							}
						}
						
					} break;
					
				case CCardRule::N7C_BACKS:
					{
						if( !Remain_card_Winner_Client(i, j, cnt, ncntlose, ncntwin, ncntequal) )
						{
							// 카드가 모두 같다면 무뉘비교
							if(m_cRule[i].m_stRuleData.BStraight < m_cRule[j].m_stRuleData.BStraight)
							{
								cnt++;
								m_cRule[i].m_Extra_WinCard[LOSE][ncntlose++] = m_cRule[i].m_stRuleData.BStraight;
							}
							else if(m_cRule[i].m_stRuleData.BStraight > m_cRule[j].m_stRuleData.BStraight)
							{
								m_cRule[i].m_Extra_WinCard[WIN][ncntwin++] = m_cRule[i].m_stRuleData.BStraight;
								
							}
							// 폭탄카드
							else
							{
								m_cRule[i].m_Extra_WinCard[EQUAL][ncntequal++] = m_cRule[i].m_stRuleData.BStraight;
								if(m_cRule[i].m_RuleComplteCard[1] < m_cRule[j].m_RuleComplteCard[1])
								{
									cnt++;
									m_cRule[i].m_Extra_WinCard[LOSE][ncntlose++] = m_cRule[i].m_RuleComplteCard[1];
								}
								else
									m_cRule[i].m_Extra_WinCard[WIN][ncntwin++] = m_cRule[i].m_RuleComplteCard[1];
							}
								
						} 
					} break;
					
				case CCardRule::N7C_STRAIGHT:
					{
						if(m_cRule[i].m_stRuleData.Straight%13 < m_cRule[j].m_stRuleData.Straight%13)
						{
							cnt++;
						}
						else if(m_cRule[i].m_stRuleData.Straight%13 == m_cRule[j].m_stRuleData.Straight%13)
						{
							if( !Remain_card_Winner_Client(i, j, cnt, ncntlose, ncntwin, ncntequal) )
							{
								// 카드가 모두 같다면 무뉘비교
								if(m_cRule[i].m_stRuleData.Straight < m_cRule[j].m_stRuleData.Straight)
								{
									cnt++;
									m_cRule[i].m_Extra_WinCard[LOSE][ncntlose++] = m_cRule[i].m_stRuleData.Straight;
								}
								else if(m_cRule[i].m_stRuleData.Straight > m_cRule[j].m_stRuleData.Straight)
								{
									m_cRule[i].m_Extra_WinCard[WIN][ncntwin++] = m_cRule[i].m_stRuleData.Straight;
								}
								// 폭탄카드
								else
								{
									m_cRule[i].m_Extra_WinCard[EQUAL][ncntequal++] = m_cRule[i].m_stRuleData.Straight;
									if(m_cRule[i].m_RuleComplteCard[1] < m_cRule[j].m_RuleComplteCard[1])
									{
										cnt++;
										m_cRule[i].m_Extra_WinCard[LOSE][ncntlose++] = m_cRule[i].m_RuleComplteCard[1];
									}
									else
										m_cRule[i].m_Extra_WinCard[WIN][ncntwin++] = m_cRule[i].m_RuleComplteCard[1];
									
								}
							} 
						}
						
					} break;
				case CCardRule::N7C_TRIPLE:
					{
						if(m_cRule[i].m_stRuleData.Trip[0]%13 < m_cRule[j].m_stRuleData.Trip[0]%13)
						{
							cnt++;
						}
						
					} break;
				case CCardRule::N7C_TWOPAIR:
					{
						if(m_cRule[i].m_stRuleData.Onep[0]%13 < m_cRule[j].m_stRuleData.Onep[0]%13)
						{
							cnt++;
							break;
						} 
						if(m_cRule[i].m_stRuleData.Onep[0]%13 == m_cRule[j].m_stRuleData.Onep[0]%13)
						{
							
							if(m_cRule[i].m_stRuleData.Onep[1]%13 < m_cRule[j].m_stRuleData.Onep[1]%13)
							{
								cnt++;
								break;
							}
							else if(m_cRule[i].m_stRuleData.Onep[1]%13 == m_cRule[j].m_stRuleData.Onep[1]%13)
							{
								
								if( !Remain_card_Winner_Client(i, j, cnt, ncntlose, ncntwin, ncntequal) )
								{
									// 카드가 모두 같다면 무뉘비교
									if(m_cRule[i].m_stRuleData.Onep[0] < m_cRule[j].m_stRuleData.Onep[0])
									{
										cnt++;
										m_cRule[i].m_Extra_WinCard[LOSE][ncntlose++] = m_cRule[i].m_stRuleData.Onep[0];
									}
									else if(m_cRule[i].m_stRuleData.Onep[0] > m_cRule[j].m_stRuleData.Onep[0])
									{
										m_cRule[i].m_Extra_WinCard[WIN][ncntwin++] = m_cRule[i].m_stRuleData.Onep[0];
									}
									// 폭탄카드
									else
									{
										m_cRule[i].m_Extra_WinCard[EQUAL][ncntequal++] = m_cRule[i].m_stRuleData.Onep[0];
										if(m_cRule[i].m_RuleComplteCard[1] < m_cRule[j].m_RuleComplteCard[1])
										{
											cnt++;
											m_cRule[i].m_Extra_WinCard[LOSE][ncntlose++] = m_cRule[i].m_RuleComplteCard[1];
										}
										else
											m_cRule[i].m_Extra_WinCard[WIN][ncntwin++] = m_cRule[i].m_RuleComplteCard[1];
									}
				
								}
							}
						}
						
					} break;
				case CCardRule::N7C_ONEPAIR:
					{
						if(m_cRule[i].m_stRuleData.Onep[0]%13 < m_cRule[j].m_stRuleData.Onep[0]%13)
						{
							cnt++;
							break;
						} 
						else if(m_cRule[i].m_stRuleData.Onep[0]%13 == m_cRule[j].m_stRuleData.Onep[0]%13)
						{
							
							if( !Remain_card_Winner_Client(i, j, cnt, ncntlose, ncntwin, ncntequal) )
							{
								// 카드가 모두 같다면 무뉘비교
								if(m_cRule[i].m_stRuleData.Onep[0] < m_cRule[j].m_stRuleData.Onep[0])
								{
									cnt++;
									m_cRule[i].m_Extra_WinCard[LOSE][ncntlose++] = m_cRule[i].m_stRuleData.Onep[0];
								}
								else if(m_cRule[i].m_stRuleData.Onep[0] > m_cRule[j].m_stRuleData.Onep[0])
								{
									m_cRule[i].m_Extra_WinCard[WIN][ncntwin++] = m_cRule[i].m_stRuleData.Onep[0];
								}
								// 폭탄카드
								else
								{
									m_cRule[i].m_Extra_WinCard[EQUAL][ncntequal++] = m_cRule[i].m_stRuleData.Onep[0];
									if(m_cRule[i].m_RuleComplteCard[1] < m_cRule[j].m_RuleComplteCard[1])
									{
										cnt++;
										m_cRule[i].m_Extra_WinCard[LOSE][ncntlose++] = m_cRule[i].m_RuleComplteCard[1];
									}
									else
										m_cRule[i].m_Extra_WinCard[WIN][ncntwin++] = m_cRule[i].m_RuleComplteCard[1];

								}
							}
						}
						
					} break;
				case CCardRule::N7C_NOPAIR:
					{
						if(m_cRule[i].m_stRuleData.mang%13 < m_cRule[j].m_stRuleData.mang%13)
						{
							cnt++;
							break;
						} 
						else if(m_cRule[i].m_stRuleData.mang%13 == m_cRule[j].m_stRuleData.mang%13)
						{
							
							if( !Remain_card_Winner_Client(i, j, cnt, ncntlose, ncntwin, ncntequal) )
							{
								// 카드가 모두 같다면 무뉘비교
								if(m_cRule[i].m_stRuleData.mang < m_cRule[j].m_stRuleData.mang)
								{
									cnt++;
								}
								else if(m_cRule[i].m_stRuleData.mang > m_cRule[j].m_stRuleData.mang)
								{
									m_cRule[i].m_Extra_WinCard[WIN][ncntwin++] = m_cRule[i].m_stRuleData.mang;
								}
								// 폭탄카드
								else
								{
									m_cRule[i].m_Extra_WinCard[EQUAL][ncntequal++] = m_cRule[i].m_stRuleData.mang;
									if(m_cRule[i].m_RemainCard[0] < m_cRule[j].m_RemainCard[0])
									{
										cnt++;
										m_cRule[i].m_Extra_WinCard[LOSE][ncntlose++] = m_cRule[i].m_RemainCard[0];
									}
									else
										m_cRule[i].m_Extra_WinCard[WIN][ncntwin++] = m_cRule[i].m_RemainCard[0];

								}
							}
						}
						
					} break;
				}
			}
		}
		m_Ranking[i] = cnt;
	}
}


/* (Client용) */
bool CSevenRuleMan::Remain_card_Winner_Client(int i, int j, int &cnt, int &ncntlose, int &ncntwin, int &ncntequal)
{
	bool bExWincard = false;

	// 비교할 카드 갯수
	int nCompareCard = 0;
	
	if (m_RuleKind==RULE_KIND_ORIGINAL_NETMARBLE)
	{
		// 정통 넷마블 룰 (동일족보비교 -> 모든 나머지카드 들의 숫자를 비교 -> 패를 이루는 카드들의 무늬 비교)
		// 넷마블룰은 남아있는 모든 카드들의 숫자를 비교한다.

		// 모든카드 비교 
		nCompareCard = m_cRule[i].m_nTotalCard;

	}
	else if (m_RuleKind==RULE_KIND_HANGAME)
	{
		// 한게임 룰 (동일족보비교 -> 나머지카드 중 낮은 숫자 2장 버리고 난 카드들의 숫자비교 -> 패를 이루는 카드들의 무늬 비교)
		// 한게임은 남아있는 카드 중 낮은 숫자 2장은 버리고 비교한다.

	//	ASSERT(m_cRule[i].get_ramain_card_number()==m_cRule[j].get_ramain_card_number());
	//	ASSERT(m_cRule[i].get_TotalCard()-m_cRule[i].get_ramain_card_number()==m_cRule[j].get_rule_complete_card_number());
		
		nCompareCard = min(m_cRule[i].get_TotalCard(),5) - m_cRule[i].get_rule_complete_card_number();		

	//	ASSERT(nCompareCard>=0);
		if (nCompareCard<=0) 
			return false;

	}


	for(int k=0; k<nCompareCard; k++)
	{
		if(m_cRule[i].m_RemainCard[k]%13 < m_cRule[j].m_RemainCard[k]%13)
		{
			cnt++;
			
			m_cRule[i].m_Extra_WinCard[LOSE][ncntlose++] = m_cRule[i].m_RemainCard[k];
			bExWincard = true;
			break;
		}
		else if(m_cRule[i].m_RemainCard[k]%13 > m_cRule[j].m_RemainCard[k]%13)
		{
			
			m_cRule[i].m_Extra_WinCard[WIN][ncntwin++] = m_cRule[i].m_RemainCard[k];
			bExWincard = true;
			break;
		}
		else
		{
			m_cRule[i].m_Extra_WinCard[EQUAL][ncntequal++] = m_cRule[i].m_RemainCard[k];
		}
	}

	return 	bExWincard;
}





/* 족보를 이루는 카드   (Client사용) */
BOOL CSevenRuleMan::IsRuleComplate(int pnum, int ncard)
{
	if(pnum < 0 || pnum >= TOTAL_USER) return FALSE;
	for( int i=0; i<CCardRule::TOTAL_CARD; i++)
	{
		if(m_cRule[pnum].m_RuleComplteCard[i] == ncard) return TRUE;
	}

	return FALSE;
}

/* 같은 족보일때 마지막 비교카드시 승 (Client사용) */
BOOL CSevenRuleMan::IsExtraCardWin(int pnum, int ncard)
{
	if(pnum < 0 || pnum >= TOTAL_USER) return FALSE;
	for( int i=0; i<CCardRule::TOTAL_CARD; i++)
	{
		if(m_cRule[pnum].m_Extra_WinCard[WIN][i] == ncard) return TRUE;
	}
	return FALSE;
}

/* 같은 족보일때 마지막 비교카드시 패  (Client사용) */
BOOL CSevenRuleMan::IsExtraCardLose(int pnum, int ncard)
{
	if(pnum < 0 || pnum >= TOTAL_USER) return FALSE;
	for( int i=0; i<CCardRule::TOTAL_CARD; i++)
	{
		if(m_cRule[pnum].m_Extra_WinCard[LOSE][i] == ncard) return TRUE;
	}

	return FALSE;
}

/* 같은 족보일때 카드가 같음 (Client사용) */
BOOL CSevenRuleMan::IsExtraCardEqual(int pnum, int ncard)
{
	if(pnum < 0 || pnum >= TOTAL_USER) return FALSE;
	for( int i=0; i<CCardRule::TOTAL_CARD; i++)
	{
		if(m_cRule[pnum].m_Extra_WinCard[EQUAL][i] == ncard) return TRUE;
	}
	return FALSE;
}

/* 이벤트 (모든 족보를 이루었다면) */
BOOL CSevenRuleMan::IsHaveEventCard(int pnum, int ncard, int range_GTE /*= CCardRule::N7C_DUMMY  all rule*/)
{
	if(pnum < 0 || pnum >= TOTAL_USER) return FALSE;
	/* range 0 ~ 13*/
	for( int i=range_GTE; i<CCardRule::N7C_CROYAL/* 족보*/; i++)
	{
		for( int j=0; j<CCardRule::TOTAL_CARD; j++)
		{
			if(m_cRule[pnum].m_AllRuleCard[i][j] == ncard) return TRUE;
		}
	}
	return FALSE;
}

/* 이벤트 (모든 족보를 이루었다면) */
BOOL CSevenRuleMan::IsHaveEventCardEx(int pnum, int pkind,BOOL *blucky,char pcard[],char ppcard[],int cardnum,int jokercard, int range_GTE /*= CCardRule::N7C_DUMMY  all rule*/)
{
	if(pnum < 0 || pnum >= TOTAL_USER) return FALSE;

	int ncard = -1;
	int ncardp = -1;
	BOOL bevent = FALSE;

	for(int x = 0 ; x < cardnum ; x ++)
	{
		ncard = pcard[x];
		ncardp = ppcard[x];

		if(jokercard == ncard)continue;
		/* range 0 ~ 13*/
		for( int i=range_GTE; i<CCardRule::N7C_CROYAL/* 족보*/; i++)
		{
			for( int j=0; j<CCardRule::TOTAL_CARD; j++)
			{
				if(m_cRule[pnum].m_AllRuleCard[i][j] == ncard){

					bevent =  TRUE;
					if(ncardp == pkind){
						*blucky = 1;
					}
				}
			}
		}
	}
	
	return bevent;
}

// 해당 족보에 이벤트가 있는지 검사
BOOL CSevenRuleMan::IsHaveCurrentEvent(int pnum,int ncard, int range_GTE)
{
	if(pnum < 0 || pnum >= TOTAL_USER) return FALSE;		
	
	for( int j=0; j<CCardRule::TOTAL_CARD; j++)
	{
		if(m_cRule[pnum].m_AllRuleCard[range_GTE][j] == ncard) return TRUE;
	}	
	return FALSE;
}

// 해당 족보에 이벤트가 있는지 검사
BOOL CSevenRuleMan::IsHaveCurrentEventEx(int pnum,char pcard[],int cardnum, int jokercard, int range_GTE)
{
	if(pnum < 0 || pnum >= TOTAL_USER) return FALSE;		
	
	int ncard = -1;
	
	for(int x = 0 ; x < cardnum ; x ++)
	{
		ncard = pcard[x];
		if(jokercard == ncard)continue;

		for( int j=0; j<CCardRule::TOTAL_CARD; j++)
		{
			if(m_cRule[pnum].m_AllRuleCard[range_GTE][j] == ncard) return TRUE;
		}	
	}
	return FALSE;
}

// 족보가 아닌 카드들 
BOOL CSevenRuleMan::IsRemainCard(int pnum,int ncard)
{
	if(pnum < 0 || pnum >= TOTAL_USER) return FALSE;		
	
	for( int j=0; j<CCardRule::TOTAL_CARD; j++)
	{
		if(m_cRule[pnum].m_RemainCard[j] == ncard) return TRUE;
	}	
	return FALSE;
}

/* 스페셜  = (  로티 : 3 스티 : 2 포카드 : 1 )*/
int CSevenRuleMan::IsSpecial_Rule(int pnum)
{
	if(pnum < 0 || pnum >= TOTAL_USER) return -1;
	if( !m_cRule[pnum].isValid() ) return -1;

	switch( m_cRule[pnum].get_sevenValue() )
	{
#if defined(__NEWPOKER__)
	case CCardRule::N7C_TITLE:
		return CODE_SPECIAL_FULLHOUSE;
#endif	// defined(__NEWPOKER__)
	case CCardRule::N7C_CROYAL:
	case CCardRule::N7C_HROYAL:
	case CCardRule::N7C_DROYAL:
	case CCardRule::N7C_SROYAL: 
		return CODE_SPECIAL_RSF;				 // 로티
	case CCardRule::N7C_BSFLUSH:
	case CCardRule::N7C_SFULSH:
		return CODE_SPECIAL_SF;				 // 스티
	case CCardRule::N7C_QUADS:
		return CODE_SPECIAL_FCARD;				 // 포카드	
	}

	return CODE_SPECIAL_NONE;
}

/* 현재 나의 족보 */
int CSevenRuleMan::get_MyRule(int pnum, bool bUseLog /* = false */ ) // 로티플 이상에서는 모두 클로버로티플로 반환하는문제( 로그에서는 실제 로티플값으로 반환 )
{
	if(pnum < 0 || pnum >= TOTAL_USER) return -1;
	if( !m_cRule[pnum].isValid() ) return -1;

	return m_cRule[ pnum ].get_sevenValue( bUseLog );
}

int CSevenRuleMan::get_MyRule_Low(int pnum)
{
	if(pnum < 0 || pnum >= TOTAL_USER) return -1;
	if( !m_cRule[pnum].isValid() ) return -1;

	return m_cRule[pnum].get_sevenLowValue();
}

BOOL CSevenRuleMan::is_value_equal()
{
	int nWinnerPNum = get_Winner();
	if(nWinnerPNum < 0 || nWinnerPNum >= TOTAL_USER) return FALSE;
	if( !m_cRule[nWinnerPNum].isValid() ) return FALSE;

	BOOL bOK = m_cRule[nWinnerPNum].is_value_equal(0);	
	if( bOK )return bOK;	

	bOK = m_cRule[nWinnerPNum].is_value_equal(1);
	if( bOK )return bOK;

	 bOK = m_cRule[nWinnerPNum].is_value_equal(2);
	if( bOK )return bOK;

	return bOK;
}


/* 유저 등수 알아오기  ex) 1등 */
int CSevenRuleMan::get_Low_Winner(int rank /* = 1 */) 
{
	for(int i = 0; i < TOTAL_USER; i++) //오름차순.
	{
		if( !m_cRule[i].isValid() ) continue;
		if(m_Ranking_low[i] == rank) return i;
	}

	return -1;
}

//동일패가 있는가?
BOOL CSevenRuleMan::is_Low_value_equal()
{
	int nWinnerPNum = get_Low_Winner();
	if(nWinnerPNum < 0 || nWinnerPNum >= TOTAL_USER) return FALSE;
	if( !m_cRule[nWinnerPNum].isValid() ) return FALSE;

	BOOL bOK = m_cRule[nWinnerPNum].is_Low_value_equal(0);	
	if( bOK )return bOK;	

	bOK = m_cRule[nWinnerPNum].is_Low_value_equal(1);
	if( bOK )return bOK;

	 bOK = m_cRule[nWinnerPNum].is_Low_value_equal(2);
	if( bOK )return bOK;

	return bOK;
}

/* 같은 족보일때 마지막 비교카드시 승 (Client사용) */
BOOL CSevenRuleMan::IsExtraCardWin_low(int pnum, int ncard)
{
	if(pnum < 0 || pnum >= TOTAL_USER) return FALSE;
	for( int i=0; i<CCardRule::TOTAL_CARD; i++)
	{
		if(m_cRule[pnum].m_Extra_WinCard_Low[WIN][i] == ncard) return TRUE;
	}
	return FALSE;
}

/* 같은 족보일때 마지막 비교카드시 패  (Client사용) */
BOOL CSevenRuleMan::IsExtraCardLose_low(int pnum, int ncard)
{
	if(pnum < 0 || pnum >= TOTAL_USER) return FALSE;
	for( int i=0; i<CCardRule::TOTAL_CARD; i++)
	{
		if(m_cRule[pnum].m_Extra_WinCard_Low[LOSE][i] == ncard) return TRUE;
	}

	return FALSE;
}

/* 같은 족보일때 카드가 같음 (Client사용) */
BOOL CSevenRuleMan::IsExtraCardEqual_low(int pnum, int ncard)
{
	if(pnum < 0 || pnum >= TOTAL_USER) return FALSE;
	for( int i=0; i<CCardRule::TOTAL_CARD; i++)
	{
		if(m_cRule[pnum].m_Extra_WinCard_Low[EQUAL][i] == ncard) return TRUE;
	}
	return FALSE;
}

BOOL CSevenRuleMan::IsLowRuleComplate(int pnum, int ncard)
{
	if(pnum < 0 || pnum >= TOTAL_USER) return FALSE;
	for( int i=0; i<CCardRule::TOTAL_CARD; i++)
	{
		if(m_cRule[pnum].m_RuleComplteCard_Low[i] == ncard) return TRUE;
	}

	return FALSE;	
}
/* 랭킹 구하기 (클라이언트용)*/
void CSevenRuleMan::get_cal_Low_Winner_Client()
{
	int cnt = 0;
	int ncntlose = 0;
	int ncntwin = 0;
	int ncntequal = 0;

	ZeroMemory(m_Ranking_low, sizeof(m_Ranking_low));
	for(int i = 0; i < TOTAL_USER; i++) //오름차순.
	{
		cnt = 1;
		ncntlose = 0;
		ncntwin = 0;
		ncntequal = 0;
		if( !m_cRule[i].isValid() ) {continue; }
	
		for(int j=0; j < TOTAL_USER; j++)
		{			
			if( i==j || !m_cRule[j].isValid() )	{ continue; }
			// 순위결정
			if( m_cRule[i].get_sevenLowValue() == -1  && m_cRule[j].get_sevenLowValue() == -1 )
			{				
				if(!Remain_Low_card_Winner_Client(i, j, cnt, ncntlose, ncntwin, ncntequal)){
					ASSERT(0);					
				}
			}
			else if( m_cRule[i].get_sevenLowValue() == -1 ){
				cnt++;
			}
			else if(m_cRule[j].get_sevenLowValue() == -1){
				continue;
			}
			else if(m_cRule[i].get_sevenLowValue()%13 > m_cRule[j].get_sevenLowValue()%13 )
			{
				cnt++;
			}
			else if(m_cRule[i].get_sevenLowValue()%13 == m_cRule[j].get_sevenLowValue()%13 )
			{				
				if(!Remain_Low_card_Winner_Client(i, j, cnt, ncntlose, ncntwin, ncntequal)){
					ASSERT(0);
				}
			}
		}
		m_Ranking_low[i] = cnt;
	}
}

/*
bool CSevenRuleMan::Remain_Low_card_Winner_Client(int i, int j, int &cnt, int &ncntlose, int &ncntwin, int &ncntequal)
{
	bool bExWincard = false;

	// 비교할 카드 갯수
	int nCompareCard = 5;

	ASSERT(nCompareCard>=0);
	if (nCompareCard<=0) 
		return false;
	
	for(int k=3; k >=0 ; k--)
	{	
		int nN1 = m_cRule[i].m_RuleComplteCard_Low[k]%13;
		if( m_cRule[i].m_RuleComplteCard_Low[k] == -1){
			return false;
		}
		int nP1 = m_cRule[i].m_RuleComplteCard_Low[k]/13;

	
		int nN2 = m_cRule[j].m_RuleComplteCard_Low[k]%13;
		if( m_cRule[j].m_RuleComplteCard_Low[k] == -1){
			return false;
		}
		int nP2 = m_cRule[j].m_RuleComplteCard_Low[k]/13;		

		if( nN1 < nN2 )
		{			
			m_cRule[i].m_Extra_WinCard_Low[WIN][ncntwin++] = m_cRule[i].m_RuleComplteCard_Low[k];
			bExWincard = true;
			break;
		}
		else if(nN1 > nN2 )
		{
			cnt++;			
			m_cRule[i].m_Extra_WinCard_Low[LOSE][ncntlose++] = m_cRule[i].m_RuleComplteCard_Low[k];
			bExWincard = true;
			break;
		}
		
	}

	return 	bExWincard;
}
*/

bool CSevenRuleMan::Remain_Low_card_Winner_Client(int i, int j, int &cnt, int &ncntlose, int &ncntwin, int &ncntequal)
{
	
	int total_count_i=m_cRule[i].get_rule_complete_Low_card_number();
	int total_count_j=m_cRule[j].get_rule_complete_Low_card_number();


	//숫자를 비교한다.
	int idx_card_i = total_count_i;
	int idx_card_j = total_count_j;

	for(int k=4; k >=0 ; k--)
	{			
		idx_card_i--;
		idx_card_j--;

		if(idx_card_i<0 && idx_card_j<0)
		{
			return false;
		}		
		else if(idx_card_i<0)
		{
			cnt++;
			m_cRule[i].m_Extra_WinCard_Low[LOSE][ncntlose++] = m_cRule[i].m_RuleComplteCard_Low[0];
			return true;	
		}
		else if (idx_card_j<0)
		{
			m_cRule[i].m_Extra_WinCard_Low[WIN][ncntwin++] = m_cRule[i].m_RuleComplteCard_Low[0];
			return true;
		}
	


		if( m_cRule[i].m_RuleComplteCard_Low[idx_card_i] == -1){
			return false;
		}
		if( m_cRule[j].m_RuleComplteCard_Low[idx_card_j] == -1){
			return false;
		}

		int nN1 = m_cRule[i].m_RuleComplteCard_Low[idx_card_i]%13;
		
		if (nN1==12) nN1=0;
		else nN1 +=1;	
		

		int nN2 = m_cRule[j].m_RuleComplteCard_Low[idx_card_j]%13;
		if (nN2==12) nN2=0;
		else nN2 +=1;	
		

		if( nN1 < nN2 )
		{	
			m_cRule[i].m_Extra_WinCard_Low[WIN][ncntwin++] = m_cRule[i].m_RuleComplteCard_Low[idx_card_i];
			return true;			
		}
		else if(nN1 > nN2 )
		{
			cnt++;			
			m_cRule[i].m_Extra_WinCard_Low[LOSE][ncntlose++] = m_cRule[i].m_RuleComplteCard_Low[idx_card_i];
			return true;			
		}		
		
	}


	///무늬를 비교한다. 
	idx_card_i = total_count_i;
	idx_card_j = total_count_j;

	for(int k=4; k >=0 ; k--)
	{
		idx_card_i--;
		idx_card_j--;
		
		if(idx_card_i<0 && idx_card_j<0)
		{
			return false;
		}
		if(idx_card_i<0)
		{
			cnt++;
			return true;	
		}
		else if (idx_card_j<0)
		{
			return true;
		}

		
		if( m_cRule[i].m_RuleComplteCard_Low[idx_card_i] == -1){
			return false;
		}
		if( m_cRule[j].m_RuleComplteCard_Low[idx_card_j] == -1){
			return false;
		}

		int nP1 = m_cRule[i].m_RuleComplteCard_Low[idx_card_i]/13;	
		int nP2 = m_cRule[j].m_RuleComplteCard_Low[idx_card_j]/13;		

		if( nP1 < nP2 )
		{
			m_cRule[i].m_Extra_WinCard_Low[WIN][ncntwin++] = m_cRule[i].m_RuleComplteCard_Low[idx_card_i];
			return true;
		}
		else if(nP1 > nP2 )
		{
			cnt++;	
			m_cRule[i].m_Extra_WinCard_Low[LOSE][ncntlose++] = m_cRule[i].m_RuleComplteCard_Low[idx_card_i];
			return true;
		}		
		
	}

	return 	false;
}
void CSevenRuleMan::get_cal_Low_Winner_Server()
{
	int cnt = 0;
	//int ncntlose =0;
	//int ncntwin =0;
	//int ncntequal =0;

	ZeroMemory(m_Ranking_low, sizeof(m_Ranking_low));
	for(int i = 0; i < TOTAL_USER; i++) //오름차순.
	{
		cnt = 1;
	
		if( !m_cRule[i].isValid() ) {continue; }
	
		for(int j=0; j < TOTAL_USER; j++)
		{			
			if( i==j || !m_cRule[j].isValid() )	{ continue; }
			
			// 순위결정
			if(m_cRule[i].get_sevenValue() >= CCardRule::N7C_QUADS && m_cRule[j].get_sevenValue() >= CCardRule::N7C_QUADS)
			{
				if (m_cRule[i].get_sevenValue() < m_cRule[j].get_sevenValue())
				{
					cnt++;
				}
				else if (m_cRule[i].get_sevenValue() == m_cRule[j].get_sevenValue())
				{
					// 조건검사
					switch( m_cRule[i].get_sevenValue() )
					{
					case CCardRule::N7C_CROYAL:
					case CCardRule::N7C_HROYAL:
					case CCardRule::N7C_DROYAL:
					case CCardRule::N7C_SROYAL:
						{
//#if defined(__NEWPOKER__)
//							if( !Remain_card_Winner_Server(i, j, cnt, ncntlose, ncntwin, ncntequal) )
//#else	// defined(__NEWPOKER__)
							if( !Remain_card_Winner_Server(i, j, cnt) )
//#endif	// defined(__NEWPOKER__)
							{
								// 카드가 모두 같다면 무뉘비교
								if(m_cRule[i].m_stRuleData.Royal < m_cRule[j].m_stRuleData.Royal)
								{
									cnt++;
								}
							}
							
						} break;
					case CCardRule::N7C_BSFLUSH:
						{
//#if defined(__NEWPOKER__)
//							if( !Remain_card_Winner_Server(i, j, cnt, ncntlose, ncntwin, ncntequal) )
//#else	// defined(__NEWPOKER__)
							if( !Remain_card_Winner_Server(i, j, cnt) )
//#endif	// defined(__NEWPOKER__)
							{
								// 카드가 모두 같다면 무뉘비교
								if(m_cRule[i].m_stRuleData.BackStrFsh < m_cRule[j].m_stRuleData.BackStrFsh)
								{
									cnt++;
								}
							}
							
						} break;
					case CCardRule::N7C_SFULSH:
						{
							if(m_cRule[i].m_stRuleData.StrFsh%13 < m_cRule[j].m_stRuleData.StrFsh%13)
							{
								cnt++;
							}
							else if(m_cRule[i].m_stRuleData.StrFsh%13 == m_cRule[j].m_stRuleData.StrFsh%13)
							{
//#if defined(__NEWPOKER__)
//								if( !Remain_card_Winner_Server(i, j, cnt, ncntlose, ncntwin, ncntequal) )
//#else	// defined(__NEWPOKER__)
								if( !Remain_card_Winner_Server(i, j, cnt) )
//#endif	// defined(__NEWPOKER__)
								{
									// 카드가 모두 같다면 무뉘비교
									if(m_cRule[i].m_stRuleData.StrFsh < m_cRule[j].m_stRuleData.StrFsh)
									{
										cnt++;
									}
								
								}
							}
							
						} break;
					case CCardRule::N7C_QUADS:
						{
							if(m_cRule[i].m_stRuleData.Poker[0]%13 < m_cRule[j].m_stRuleData.Poker[0]%13)
							{
								cnt++;
							}
							
							
						} break;
					}
				}
			}
			else if (m_cRule[i].get_sevenValue() >= CCardRule::N7C_QUADS)
			{
				continue;
			}
			else if (m_cRule[j].get_sevenValue() >= CCardRule::N7C_QUADS)
			{
				cnt++;
			}			
			else if( m_cRule[i].get_sevenLowValue() == -1  && m_cRule[j].get_sevenLowValue() == -1 )
			{				
				if(!Remain_Low_card_Winner_Server(i, j, cnt)){
					ASSERT(0);
					
				}
			}
			else if( m_cRule[i].get_sevenLowValue() == -1 ){
				cnt++;
			}
			else if(m_cRule[j].get_sevenLowValue() == -1){
				continue;
			}
			else if(m_cRule[i].get_sevenLowValue()%13 > m_cRule[j].get_sevenLowValue()%13 )
			{
				cnt++;
			}
			else if(m_cRule[i].get_sevenLowValue()%13 == m_cRule[j].get_sevenLowValue()%13 )
			{				
				if(!Remain_Low_card_Winner_Server(i, j, cnt)){
					ASSERT(0);
				
				}
			}
		}


		m_Ranking_low[i] = cnt;
	}
}


bool CSevenRuleMan::Remain_Low_card_Winner_Server(int i, int j, int &cnt)
{
	
	int total_count_i=m_cRule[i].get_rule_complete_Low_card_number();
	int total_count_j=m_cRule[j].get_rule_complete_Low_card_number();


	//숫자를 비교한다.
	int idx_card_i = total_count_i;
	int idx_card_j = total_count_j;

	for(int k=4; k >=0 ; k--)
	{			
		idx_card_i--;
		idx_card_j--;

		if(idx_card_i<0 && idx_card_j<0)
		{
			return false;
		}		
		else if(idx_card_i<0)
		{
			cnt++;
			return true;	
		}
		else if (idx_card_j<0)
		{
			return true;
		}
	


		if( m_cRule[i].m_RuleComplteCard_Low[idx_card_i] == -1){
			return false;
		}
		if( m_cRule[j].m_RuleComplteCard_Low[idx_card_j] == -1){
			return false;
		}

		int nN1 = m_cRule[i].m_RuleComplteCard_Low[idx_card_i]%13;
		
		if (nN1==12) nN1=0;
		else nN1 +=1;	
		

		int nN2 = m_cRule[j].m_RuleComplteCard_Low[idx_card_j]%13;
		if (nN2==12) nN2=0;
		else nN2 +=1;	
		

		if( nN1 < nN2 )
		{	
			return true;			
		}
		else if(nN1 > nN2 )
		{
			cnt++;			
			return true;			
		}		
		
	}


	///무늬를 비교한다. 
	idx_card_i = total_count_i;
	idx_card_j = total_count_j;

	for(int k=4; k >=0 ; k--)
	{
		idx_card_i--;
		idx_card_j--;
		
		if(idx_card_i<0 && idx_card_j<0)
		{
			return false;
		}
		if(idx_card_i<0)
		{
			cnt++;
			return true;	
		}
		else if (idx_card_j<0)
		{
			return true;
		}

		
		if( m_cRule[i].m_RuleComplteCard_Low[idx_card_i] == -1){
			return false;
		}
		if( m_cRule[j].m_RuleComplteCard_Low[idx_card_j] == -1){
			return false;
		}

		int nP1 = m_cRule[i].m_RuleComplteCard_Low[idx_card_i]/13;	
		int nP2 = m_cRule[j].m_RuleComplteCard_Low[idx_card_j]/13;		

		if( nP1 < nP2 )
		{
			return true;
		}
		else if(nP1 > nP2 )
		{
			cnt++;	
			return true;
		}		
		
	}

	return 	false;
}