#include "StdAfx.h"
#include "HoolaHand.h"

#define MAX_CARDCOUNT		8
#define MAX_CARD_VALUE		13
#define MAX_CARD_FACE		4
#define CARD_JOKER1			52
#define CARD_JOKER2			53
#define nFACE_CLOVER		0
#define nFACE_HEART			1
#define nFACE_DIAMOND		2
#define nFACE_SPADE			3


CHoolaHand::CHoolaHand(void)
{
	m_bClickedCardsHands = FALSE;
	m_bConsiderJoker = FALSE;
}

CHoolaHand::~CHoolaHand(void)
{
}

// ī�� ������ �ڽ��� �տ� ���
void CHoolaHand::PushCard(int nCardIndex)
{
	if ( std::count( m_dqCards.begin(), m_dqCards.end(), (BYTE)nCardIndex ) == 0 )
	{
		m_dqCards.push_back(nCardIndex);
	}
	else
	{
		TRACE(_T("�̹� �ִ� ī�带 ����־���."));
	}
}

// Ư�� ī����� �̾ƿ´�.
CCards CHoolaHand::PopCards(CCards cCards)
{
	CCards ccards;
	return ccards; 
}

// ī�� ������ ������Ʈ �Ѵ�.
void CHoolaHand::Update(void)
{
	m_cCards.ClearCards();
//	m_cCards.m_cDuplicateCards.ClearCards();

	m_cCards.m_dqStraight = FindStraightCards(m_dqCards);
	m_cCards.m_dqTogether = FindTogtherCards(m_dqCards);
	m_cCards.m_dqAttachStr = FindAttachStraightCards(m_dqCards);
	m_cCards.m_dqAttachTog = FindAttachTogtherCards(m_dqCards);
	m_cCards.m_dqSeven = FindSevenCards(m_dqCards);

//	m_cCards.m_cDuplicateCards = FindDuplicateCards(m_cCards);
}

//�ش� ť�� Ư������ ������ �ִ°�
BOOL CHoolaHand::IsDqHasTheCardIndex(BYTEDQ dq, BYTE cardIndex)
{
	if ( cardIndex < 60 )
	{
		if ( std::count( dq.begin(), dq.end(), cardIndex ) == 1 )
		{
			return TRUE;
		}
		else if ( std::count( dq.begin(), dq.end(), cardIndex ) > 1 )
		{
			return FALSE;
		}
	}
	return FALSE;
}

// ���������� ã�´�.
DUALBYTEDQ CHoolaHand::FindStraightCards(BYTEDQ dqCards)
{
	//��������� ���� ������ ������ ���� ã�´�.
	DUALBYTEDQ dqReturn;	//������ ��� ��
	DUALBYTEDQ dqNULL;		//���ش� �����Ͱ� ������� ���� ������

	dqNULL.clear();

	//3�� �̻��ϰ�츸 ���������� �ȴ�.
	if ( dqCards.size() <= 2 )
	{
		return dqNULL;
	}

	BYTEDQ dqSortedCards;	//���ĵ� ������
	BYTEDQ dqCardValue;		//ī�� ���ڸ� ����ִ� ������

	BYTEDQ dqReturn1; 
	

	dqSortedCards = dqCards;
	std::sort( dqSortedCards.begin(), dqSortedCards.end() );

	BYTE matrixCardFlag[MAX_CARD_FACE][MAX_CARD_VALUE];
	BYTE matrixCardIndex[MAX_CARD_FACE][MAX_CARD_VALUE];

	ZeroMemory( matrixCardFlag, sizeof(BYTE) * MAX_CARD_VALUE * MAX_CARD_FACE );
	ZeroMemory( matrixCardIndex, sizeof(BYTE) * MAX_CARD_VALUE * MAX_CARD_FACE );

	for ( int j = 0; j < dqSortedCards.size(); j++ )
	{	
		int nCardFace = dqSortedCards[j] / 13;
		int nCardValue = dqSortedCards[j] % 13;

		matrixCardFlag[ nCardFace ][ nCardValue ] = 1;
		matrixCardIndex[ nCardFace ][ nCardValue ] = dqSortedCards[j];
		dqCardValue.push_back( nCardValue );
	}

	DUALBYTEDQ dqResult;
	for ( int i = 0 ; i < MAX_CARD_FACE ; i++ )
	{
		BOOL bJoker2 = FALSE;
		if ( m_bConsiderJoker && (g_RI.FormKind != hoola::eFORM_KIND_NORMAL ) )
		{
			int nJoker1 = 0;
			int nJoker2 = 0;
			if ( std::count( dqSortedCards.begin(), dqSortedCards.end(), CARD_JOKER1 ) == 1 )
			{
				nJoker1 = CARD_JOKER1;
			}
			else 
			{
				for ( int j = 0; j < dqSortedCards.size(); j++ )
				{
					if ( dqSortedCards[j] >= 60 && dqSortedCards[j] < 120 )
					{
						nJoker1 = CARD_JOKER1;
					}
				}
			}

			if ( std::count( dqSortedCards.begin(), dqSortedCards.end(), CARD_JOKER2 ) == 1 )
			{
				nJoker2 = CARD_JOKER2;
			}
			else
			{
				for ( int j = 0; j < dqSortedCards.size(); j++ )
				{
					if ( dqSortedCards[j] >= 120 && dqSortedCards[j] < 180 )
					{
						nJoker2 = CARD_JOKER2;
					}
				}
			}

			if ( nJoker1 || nJoker2 )
			{
				//��Ŀ ī�尡 �ϳ��� �ִٸ�
				if ( nJoker1 && nJoker2 )
				{
					//��Ŀ ī�尡 �ΰ� �ִٸ�
					for ( int j = 0; j < MAX_CARD_VALUE; j++ )
					{
						if ( std::count( dqCardValue.begin(), dqCardValue.end(), j ) >= 2 )
						{
							//��Ŀī�尡 �����ְ� ���� ī�� ���ڰ� �ΰ������� ���Ͻ�Ų��?
							//�Լ��� �ܼ��ؾ� �ϴµ� �ʹ� ���� ��� �ϰ����� �̰� �״ٰ� ���߿� ó������
							return dqNULL;
						}
					}
//					dqResult = IsSerialJoker( matrixCardFlag[i], MAX_CARD_VALUE, nJoker1, nJoker2, nCardNum );
				}
				else
				{
					if ( dqSortedCards.size() == 2 )
					{
						int nJoker = nJoker1 ? nJoker1 : nJoker2;
						int nIndex = -1;

						BYTEDQ::iterator Iter;
						for ( Iter = dqSortedCards.begin(); Iter != dqSortedCards.end(); Iter++)
						{
							if ( *Iter < CARD_JOKER1 && ( *Iter / MAX_CARD_VALUE ) == i )
							{
//								dqResult = IsSerialJoker( m_CenterCard[i], MAX_CARD_VALUE, nJoker, *Iter );
								bJoker2 = TRUE;
								break;
							}
						}

// 						if ( dqResult.size() == 0 )
// 							dqResult = IsSerialJoker( matrixCardFlag[i], 13, nJoker1, nJoker2, nCardNum );
					}
					else
					{
//						dqResult = IsSerialJoker( matrixCardFlag[i], 13, nJoker1, nJoker2, nCardNum );
					}
				}
			} // if ( nJoker1 || nJoker2 )
			else
			{
//				dqResult = IsSerial( matrixCardFlag[i], 13 );
			}
		} // if ( m_bConsiderJoker && (g_RI.FormKind != hoola::eFORM_KIND_NORMAL )
		else
		{
//			dqResult = IsSerial( matrixCardFlag[i], 13 );
		}

		for ( int j = 0; j < dqResult.size(); j++ )
		{
			for ( int k = 0; k < dqResult[j].size() ; k++ )
			{
				//������ ���� ���Ͽ�
				int nPos = dqResult[j][k];

				if ( nPos >= 60 && nPos < 80 )
				{
					int nPos1 = nPos - 60;
					dqReturn1.push_back( 60 + i * 13 + nPos1 ); 

					BYTEDQ::iterator Iter;
					Iter = std::find( dqSortedCards.begin(), dqSortedCards.end(), CARD_JOKER1 );
					if ( Iter != dqSortedCards.end() )
					{
						dqSortedCards.erase( Iter );
					}
				}
				else if ( nPos >= 80 && nPos < 100 )
				{
					int nPos1 = nPos - 80;
					dqReturn1.push_back( 120 + i * 13 + nPos1 );

					BYTEDQ::iterator Iter;
					Iter = std::find( dqSortedCards.begin(), dqSortedCards.end(), CARD_JOKER2 );
					if ( Iter != dqSortedCards.end() )
					{
						dqSortedCards.erase( Iter );
					}
				}
				else
				{
					if ( bJoker2 )
					{
						dqReturn1.push_back( nPos );
					}
					else
					{
						if ( matrixCardFlag[i][nPos] )
						{
							dqReturn1.push_back( matrixCardIndex[i][nPos] );
						}
					}					
				}
			}
			static int duplicateCheckCnt = 0;
			if ( duplicateCheckCnt != j)
			{
				if ( dqReturn1.size() > 0 )
				{
					dqReturn.push_back( dqReturn1 );
					dqReturn1.clear();
				}
				duplicateCheckCnt = j;
			}
		}

		if ( !dqReturn1.empty() )
		{
			std::sort( dqReturn1.begin(), dqReturn1.end() );
			dqReturn.push_back( dqReturn1 );
		}
		dqReturn1.clear();
	}

	return dqReturn;
}

// ���������� ã�´�,
DUALBYTEDQ CHoolaHand::FindTogtherCards(BYTEDQ dqCards)
{
	DUALBYTEDQ dqNULL;
	return dqNULL;
}

// ���� ���̱� ������ ã�´�,
BYTEDQ CHoolaHand::FindAttachTogtherCards(BYTEDQ dqCards)
{
	BYTEDQ dqNULL;
	return dqNULL;
}

// ���� ���̱� ������ ã�´�,
BYTEDQ CHoolaHand::FindAttachStraightCards(BYTEDQ dqCards)
{
	BYTEDQ dqNULL;
	return dqNULL;
}

// 7ī�带 ã�´�,
BYTEDQ CHoolaHand::FindSevenCards(BYTEDQ dqCards)
{
	BYTEDQ dqNULL;
	return dqNULL;
}

// ��� ������ ī����� �����´�.
CCards CHoolaHand::GetAvailableCards()
{
	return m_cCards;
}

// �ߺ��Ǵ� ī�� ���յ��� �ִ��� �˻��Ѵ�.
CCards CHoolaHand::FindDuplicateCards(CCards cCards)
{
	return m_cCards;
}

// Hand���� ��� �����͸� �ʱ�ȭ �Ѵ�.
void CHoolaHand::Reset()
{
	m_cCards.ClearCards();
}

// ��� ������ ī����� �����´�.
// CCards CHoolaHand::GetAvailableCards()
// {
// }

DUALBYTEDQ CHoolaHand::IsSerialJoker( BYTE *pData, int nMaxIndex, int nJoker1, int nJoker2, int nCardNum )
{
	int nStart = -1;
	int nEnd = -1;
	BYTE *pData1 = new BYTE[nMaxIndex*2];
	BYTE *pData2 = new BYTE[nMaxIndex*2];
	CopyMemory( pData1, pData, sizeof(BYTE) * nMaxIndex );
	CopyMemory( pData1+nMaxIndex, pData, sizeof(BYTE) * nMaxIndex );
	CopyMemory( pData2, pData1, nMaxIndex*2 );

	int nMax = 0;
	int nResStart = -1;
	int nResEnd = -1;
	int nCount = 0;

	DUALBYTEDQ dqRet;

	if ( (nJoker1 && !nJoker2) || (!nJoker1 && nJoker2) )
	{

		for ( int nLoop = 0; nLoop < nMaxIndex; nLoop++ )
		{
			CopyMemory( pData1, pData2, nMaxIndex*2 );
			if ( !pData1[nLoop] )
			{
				pData1[nLoop] = 1;
				pData1[nLoop+nMaxIndex] = 1;
			}
			else
			{
				continue;
			}

			for ( int i = nMaxIndex; i < nMaxIndex * 2; i++ )
			{
				if ( pData1[i] )
				{
					nResStart = i;
					nResEnd = i;
					for ( int j = i - 1; j >= 0; j-- )
					{
						if ( pData1[j] )
						{
							nResStart = j;
						}
						else
						{
							break;
						}
					}

					for ( int j = i + 1; j < nMaxIndex * 2; j++ )
					{
						if ( pData1[j] )
						{
							nResEnd = j;
						}
						else
						{
							break;
						}
					}

					if ( (nResEnd - nResStart ) + 1 >= 3 )
					{
						BYTEDQ dqSer;
						dqSer.clear();
						for ( int k = nResStart; k <= nResEnd; k++ )
						{
							if ( pData2[k] )
							{
								dqSer.push_back( k % 13 );
							}
							else
							{
								if ( nJoker1 )
									dqSer.push_back( 60 + (k%13) );
								else if ( nJoker2 )
									dqSer.push_back( 80 + (k%13) );
							}
							pData1[k] = 0;
							if ( k < nMaxIndex )
								pData1[k+nMaxIndex] = 0;
						}

						if ( !dqRet.empty() )
						{
							if ( dqRet[0].size() < dqSer.size() )
							{
								dqRet.clear();
								dqRet.push_back( dqSer );
							}
						}
						else
						{
							dqRet.push_back( dqSer );
						}
					}
				}
			}
		}

		delete pData1, pData2;

		return dqRet;
	}
	else if ( nJoker1 && nJoker2 )
	{
		for ( int nLoop = 0; nLoop < nMaxIndex; nLoop++ )
		{
			CopyMemory( pData1, pData2, nMaxIndex*2 );
			if ( !pData1[nLoop] )
			{
				pData1[nLoop] = 1;
				pData1[nLoop+nMaxIndex] = 1;
				for ( int nRepeat = nLoop + 1; nRepeat < nMaxIndex; nRepeat++ )
				{
					if ( !pData1[nRepeat] )
					{
						pData1[nRepeat] = 1;
						pData1[nRepeat+nMaxIndex] = 1;
						break;
					}
					else
					{
						continue;
					}
				}
			}
			else
			{
				continue;
			}

			for ( int i = nMaxIndex; i < nMaxIndex * 2; i++ )
			{
				if ( pData1[i] )
				{
					nResStart = i;
					nResEnd = i;
					for ( int j = i - 1; j >= 0; j-- )
					{
						if ( pData1[j] )
						{
							nResStart = j;
						}
						else
						{
							break;
						}
					}

					for ( int j = i + 1; j < nMaxIndex * 2; j++ )
					{
						if ( pData1[j] )
						{
							nResEnd = j;
						}
						else
						{
							break;
						}
					}

					if ( (nResEnd - nResStart ) + 1 >= 3 )
					{
						BYTEDQ dqSer;
						dqSer.clear();
						BOOL bFirstInsert = FALSE;
						for ( int k = nResStart; k <= nResEnd; k++ )
						{
							if ( pData2[k] )
							{
								dqSer.push_back( k % 13 );
							}
							else
							{
								if ( !bFirstInsert )
								{
									dqSer.push_back( 60 + (k%13) );
									bFirstInsert = TRUE;
								}
								else
								{
									dqSer.push_back( 80 + (k%13) );
								}
							}
							pData1[k] = 0;
							if ( k < nMaxIndex )
								pData1[k+nMaxIndex] = 0;
						}

						if ( !dqRet.empty() )
						{
							if ( nCardNum != -1 )
							{
								if ( std::count( dqSer.begin(), dqSer.end(), nCardNum ) == 1 )
								{
									int nCount = 0;
									for ( int i = 0; i < dqSer.size(); i++ )
									{
										if ( dqSer[i] < CARD_JOKER1 )
											nCount++;
									}

									if ( nCount >= 2 )
									{
										if ( dqRet[0].size() < dqSer.size() )
										{
											dqRet.clear();
											dqRet.push_back( dqSer );
										}
									}
								}
							}
							else
							{
								if ( dqRet[0].size() < dqSer.size() )
								{
									dqRet.clear();
									dqRet.push_back( dqSer );
								}
							}
						}
						else
						{
							if ( nCardNum != -1 )
							{
								int nCount = 0;
								for ( int i = 0; i < dqSer.size(); i++ )
								{
									if ( dqSer[i] < CARD_JOKER1 )
										nCount++;
								}

								if ( nCount >= 2 )
								{
									if ( std::count( dqSer.begin(), dqSer.end(), nCardNum ) == 1 )
									{
										dqRet.push_back( dqSer );
									}
								}
							}
							else
							{
								dqRet.push_back( dqSer );
							}
						}
					}
				}
			}
		}
	}
	delete pData1, pData2;

	return dqRet;
}


DUALBYTEDQ CHoolaHand::IsSerialJoker( int *pData, int nMaxIndex, int nJoker, int nCardIndex )
{
	int *pData1 = new int[nMaxIndex*2];
	CopyMemory( pData1, pData, sizeof(int) * nMaxIndex );
	CopyMemory( pData1+nMaxIndex, pData, sizeof(int) * nMaxIndex );

	int nIndex = nCardIndex % 13;

	DUALBYTEDQ dqRet;
	BYTEDQ dqSer;

	CString str;

	if ( nIndex < 2 )
	{
		if ( pData1[nIndex - 2 + 13] )
		{
			if ( nIndex-1 < 0 )
				nIndex+= 13;

			if ( nJoker == CARD_JOKER1 )
				dqSer.push_back( 60 + nIndex-1 );
			else if ( nJoker == CARD_JOKER2 )
				dqSer.push_back( 80 + nIndex-1 );

			dqSer.push_back( nCardIndex );
		}
		else if ( pData1[nIndex + 2] ) 
		{
			if ( nJoker == CARD_JOKER1 )
				dqSer.push_back( 60 + nIndex+1 );
			else if ( nJoker == CARD_JOKER2 )
				dqSer.push_back( 80 + nIndex+1 );

			dqSer.push_back( nCardIndex );
		}
	}
	else if ( nIndex >= 2 && nIndex <= 11 )
	{
		if ( pData1[nIndex - 2] )
		{
			if ( nJoker == CARD_JOKER1 )
				dqSer.push_back( 60 + nIndex-1 );
			else if ( nJoker == CARD_JOKER2 )
				dqSer.push_back( 80 + nIndex-1 );

			dqSer.push_back( nCardIndex );
		}
		else if ( pData1[nIndex + 2] ) 
		{
			if ( nJoker == CARD_JOKER1 )
				dqSer.push_back( 60 + nIndex+1 );
			else if ( nJoker == CARD_JOKER2 )
				dqSer.push_back( 80 + nIndex+1 );

			dqSer.push_back( nCardIndex );
		}
	}
	else
	{
		if ( pData1[nIndex - 2] )
		{
			if ( nJoker == CARD_JOKER1 )
				dqSer.push_back( 60 + nIndex-1 );
			else if ( nJoker == CARD_JOKER2 )
				dqSer.push_back( 80 + nIndex-1 );

			dqSer.push_back( nCardIndex );
		}
		else if ( pData1[nIndex + 2 - 13] ) 
		{
			if ( nIndex+1 >= 13 )
				nIndex -= 13;

			if ( nJoker == CARD_JOKER1 )
				dqSer.push_back( 60 + nIndex+1 );
			else if ( nJoker == CARD_JOKER2 )
				dqSer.push_back( 80 + nIndex+1 );

			dqSer.push_back( nCardIndex );
		}
	}

	delete pData1;

	dqRet.push_back(dqSer);

	return dqRet;
}

DUALBYTEDQ CHoolaHand::GetStraightSet( BYTE *pData, int nMaxIndex )
{
	int nStart = -1;
	int nEnd = -1;

	BYTE *pData1 = new BYTE[ nMaxIndex * 2 ];
	CopyMemory( pData1, pData, sizeof(BYTE) * nMaxIndex );
	CopyMemory( pData1 + nMaxIndex, pData, sizeof(BYTE) * nMaxIndex );

	int nMax = 0;
	int nResStart = -1;
	int nResEnd = -1;
	int nCount = 0;

	DUALBYTEDQ dqRet;

	for ( int i = nMaxIndex; i < nMaxIndex * 2; i++ )
	{
		if ( pData1[i] )
		{
			nResStart = i;
			nResEnd = i;
			for ( int j = i - 1; j >= 0; j-- )
			{
				if ( pData1[j] )
				{
					nResStart = j;
				}
				else
				{
					break;
				}
			}

			for ( int j = i + 1; j < nMaxIndex * 2; j++ )
			{
				if ( pData1[j] )
				{
					nResEnd = j;
				}
				else
				{
					break;
				}
			}

			if ( (nResEnd - nResStart ) + 1 >= 3 )
			{
				BYTEDQ dqSer;
				dqSer.clear();
				for ( int k = nResStart; k <= nResEnd; k++ )
				{
					dqSer.push_back( k % 13 );
					pData1[k] = 0;
					if ( k < nMaxIndex )
						pData1[k+nMaxIndex] = 0;
				}
				dqRet.push_back( dqSer );
			}
		}
	}
	delete pData1;

	return dqRet;
}