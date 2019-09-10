#include "StdAfx.h"

#include "Define_Sutda.h"

CSutdaRuleMan::CSutdaRuleMan()
{

}

CSutdaRuleMan::~CSutdaRuleMan()
{

}

void CSutdaRuleMan::SD_Init(int nGameMode /*= SD_2CARD*/)
{
	SD_Clear();
}

void CSutdaRuleMan::SD_Clear()
{
	for(int i=0; i<MAX_PLAYER; ++i)
	{
		m_CSutdaRule[i].Clear();
	}
	m_nTotalPlayerCnt = 0;
	m_nCurPlayerCnt = 0;
	IsReEntry(true);	// ���� ������ ����
	IsRematch49(false);	// 49 ���� �� �ƴ�
	IsRematchDraw(false);	// ������ ���� �� �ƴ�
}

void CSutdaRuleMan::SD_SetUserCard(int pNum, int cardidx[], int cardCnt, bool bFirst/* = true*/)
{
	if(0 > pNum || MAX_PLAYER <= pNum) return;
	m_CSutdaRule[pNum].SetCard(pNum, cardidx, cardCnt);
	m_CSutdaRule[pNum].CalcuRule();

	if(bFirst) 
	{
		m_nTotalPlayerCnt++;
		m_nCurPlayerCnt++;
	}
}

void CSutdaRuleMan::SD_FoldUser(int pNum)
{
	if(0 > pNum || MAX_PLAYER <= pNum) return;
	if(RANKSTATE_FOLDER == m_CSutdaRule[pNum].RankState()) return;

	m_CSutdaRule[pNum].RankState( RANKSTATE_FOLDER ); //< CYS 111116 >
	m_CSutdaRule[pNum].Fold();
	m_nCurPlayerCnt--;
}

void CSutdaRuleMan::SD_SortRanking()
{
	int nJokbo[MAX_PLAYER] = {POINT_DIE,};
	memset(nJokbo, POINT_DIE, sizeof(int)*MAX_PLAYER);
	int nJokboCount = 0;	// ���� ���� ��


	//< CYS 111111 ������ ���⸦ �������� ������ ������	�й��� ������ ��� �Ǿ����ϴ�.
	int nRemainUser = 0;
	int _iIndex = -1;

	for ( int i = 0; i < MAX_PLAYER; i++ )
	{
		m_CSutdaRule[ i ].Ranking( MAX_PLAYER );

		if ( m_CSutdaRule[ i ].Valid() && m_CSutdaRule[ i ].RankState() != RANKSTATE_FOLDER )
		{
			nRemainUser++;
			_iIndex = i;
		}
	}

	if ( 1 == nRemainUser )
	{
		m_CSutdaRule[ _iIndex ].Ranking( 0 ); // ���� ��

		m_nWinnerJokbo =  m_CSutdaRule[ _iIndex ].ChoiceJokbo();	// ������ ����
		m_nWinnerCnt = 1;
		return;
	}
	//> CYS 111111


	// �Ϲ� ���� ����
	for(int i=0; i<MAX_PLAYER; i++)
	{
		m_CSutdaRule[i].Ranking(MAX_PLAYER);
		if(m_CSutdaRule[i].Valid() && m_CSutdaRule[i].RankState() != RANKSTATE_FOLDER)
		{
			bool IsSame = false;
			int nCurJokbo = m_CSutdaRule[i].ChoiceNormalJokbo();
			for(int j=0; j<MAX_PLAYER; j++)
			{
				if(nJokbo[j] == nCurJokbo)
				{
					IsSame = true;
					break;
				}
			}

			if(IsSame == false)
			{
				nJokbo[i] = nCurJokbo;
				nJokboCount++;
			}
		}
	}

	// �Ϲ� ���� ����
	for(int i=0; i<MAX_PLAYER-1; i++)
	{
		for(int j=i+1; j<MAX_PLAYER; j++)
		{
			if(nJokbo[i] < nJokbo[j])
			{
				int nTemp = nJokbo[i];
				nJokbo[i] = nJokbo[j];
				nJokbo[j] = nTemp;
			}
		}
	}

	// Ư�� ���� �ߵ� ������ �Ǵ��� üũ
	int nTopJokbo = POINT_NONESPECIAL;
	for(int i=0; i<MAX_PLAYER; i++)
	{
		int nSpecialJokbo = m_CSutdaRule[i].ChoiceSpecialJokbo();
		switch(nSpecialJokbo)
		{
		case POINT_DDANGKILLER:
			if(nJokbo[0] >= POINT_ONEDDANG && nJokbo[0] < POINT_TENDDANG)
			{
				if(nTopJokbo != POINT_SAGU2)	// ��49�� ������ ��� �����̷� ��� �� �� ����.
				{
					nTopJokbo = POINT_DDANGKILLER;
				}
			}
			break;
		case POINT_PARKMOONSU:
			if(nJokbo[0] == POINT_13_KWANGDDANG || nJokbo[0] == POINT_18_KWANGDDANG)
			{
				nTopJokbo = POINT_PARKMOONSU;
			}
			break;
		case POINT_SAGU:
			if(nJokbo[0] <= POINT_ALI)
			{
				if(nTopJokbo != POINT_SAGU2)
				{
					nTopJokbo = POINT_SAGU;
				}
			}
			break;
		case POINT_SAGU2:
			if(nJokbo[0] <= POINT_TENDDANG)
			{
				nTopJokbo = POINT_SAGU2;
			}
			break;
		}
	}

	// Ư�� ������ �� �߻� ���� ��� �Ϲ� ���� ������ �Ʒ��� �о��. Ư�� ������ ���� ��� �Ϲ������� �������� ó�� �Ǳ� ������ ������ ���� �������� �Ϲ������� �����ϰ� �Ǽ� ������ ���� ���̸� �о�� �������.
	if(nTopJokbo != POINT_NONESPECIAL)
	{
		for(int i=nJokboCount-1; i>0; i--)
		{
			nJokbo[i] = nJokbo[i-1];
		}
		nJokbo[0] = nTopJokbo;
	}

	// ��ŷ�� ��� �� �ش�.
	int nRank = 0;
	for(int i=0; i<nJokboCount; i++)
	{
		int nCount = 0;
		for(int j=0; j<MAX_PLAYER; j++)
		{
			if(m_CSutdaRule[j].Valid() && m_CSutdaRule[j].RankState() != RANKSTATE_FOLDER)
			{
				if(nJokbo[i] == m_CSutdaRule[j].ChoiceJokbo())
				{
					m_CSutdaRule[j].Ranking(nRank);
					nCount++;
				}
			}
		}
		nRank += nCount;

		// Ȥ�� ���� �־��.
		if(nRank <= 0)
		{
			break;
		}
	}

	// ���� ī��Ʈ, ������ ���ڰ� ���� ��� 2�� �̻��� ���ڰ� �� �� �ִ�.
	m_nWinnerJokbo = nJokbo[0];	// ������ ����
	m_nWinnerCnt = 0;
	for(int i=0; i<MAX_PLAYER; i++)
	{
		if(m_CSutdaRule[i].RankState() != RANKSTATE_FOLDER && m_CSutdaRule[i].Ranking() == 0)
		{
			m_nWinnerCnt++;
		}
	}
}

void CSutdaRuleMan::SD_CheckPenalty()
{
	PENALTY_STATE nPState = PSTATE_NONE;
	double dPenaltyRatio = 0.0;
	if(m_nWinnerJokbo >= POINT_ONEDDANG && m_nWinnerJokbo < POINT_TENDDANG)
	{
		nPState = PSTATE_DDANG;
		dPenaltyRatio = 0.01;
		int nLastSagu = MAX_PLAYER;
		for(int i=0; i<MAX_PLAYER; i++)
		{
			int nCurJokbo = m_CSutdaRule[i].ChoiceJokbo();
			if(nCurJokbo > POINT_DIE && nCurJokbo <= POINT_ALI)
			{
				m_CSutdaRule[i].PState(nPState);
				m_CSutdaRule[i].PenaltyRatio(dPenaltyRatio);
			}else if(nCurJokbo == POINT_PARKMOONSU)
			{
				m_CSutdaRule[i].PState(nPState);
				m_CSutdaRule[i].PenaltyRatio(dPenaltyRatio);
			}else if(nCurJokbo == POINT_SAGU)
			{
				m_CSutdaRule[i].PState(PSTATE_49);
				m_CSutdaRule[i].PenaltyRatio(dPenaltyRatio);
				nLastSagu = i;
			}else if(nCurJokbo == POINT_SAGU2)
			{
				m_CSutdaRule[i].PState(PSTATE_49);
				m_CSutdaRule[i].PenaltyRatio(dPenaltyRatio);
				if(nLastSagu < MAX_PLAYER)
				{
					m_CSutdaRule[nLastSagu].PState(nPState);
				}
			}else if(nCurJokbo == m_nWinnerJokbo)
			{
				m_CSutdaRule[i].PState(MAX_PSTATE);
				m_CSutdaRule[i].PenaltyRatio(0.0);
			}
		}
	}else if(m_nWinnerJokbo == POINT_TENDDANG)
	{
		nPState = PSTATE_38GWANGDDANG;
		dPenaltyRatio = 0.01;
		int nLastSagu = MAX_PLAYER;
		for(int i=0; i<MAX_PLAYER; i++)
		{
			int nCurJokbo = m_CSutdaRule[i].ChoiceJokbo();
			if(nCurJokbo > POINT_DIE && nCurJokbo <= POINT_ALI)
			{
				m_CSutdaRule[i].PState(nPState);
				m_CSutdaRule[i].PenaltyRatio(dPenaltyRatio);
			}else if(nCurJokbo == POINT_DDANGKILLER)
			{
				m_CSutdaRule[i].PState(nPState);
				m_CSutdaRule[i].PenaltyRatio(dPenaltyRatio);
			}else if(nCurJokbo == POINT_PARKMOONSU)
			{
				m_CSutdaRule[i].PState(nPState);
				m_CSutdaRule[i].PenaltyRatio(dPenaltyRatio);
			}else if(nCurJokbo == POINT_SAGU)
			{
				m_CSutdaRule[i].PState(PSTATE_49);
				m_CSutdaRule[i].PenaltyRatio(dPenaltyRatio);
				nLastSagu = i;
			}else if(nCurJokbo == POINT_SAGU2)
			{
				m_CSutdaRule[i].PState(PSTATE_49);
				m_CSutdaRule[i].PenaltyRatio(dPenaltyRatio);
				if(nLastSagu < MAX_PLAYER)
				{
					m_CSutdaRule[nLastSagu].PState(nPState);
				}
			}else if(nCurJokbo == m_nWinnerJokbo)
			{
				m_CSutdaRule[i].PState(MAX_PSTATE);
				m_CSutdaRule[i].PenaltyRatio(0.0);
			}
		}
	}
	else if(m_nWinnerJokbo == POINT_13_KWANGDDANG || m_nWinnerJokbo == POINT_18_KWANGDDANG)
	{
		nPState = PSTATE_GWANGDDANG;
		dPenaltyRatio = 0.05;
		int nLastSagu = MAX_PLAYER;
		for(int i=0; i<MAX_PLAYER; i++)
		{
			int nCurJokbo = m_CSutdaRule[i].ChoiceJokbo();
			if(nCurJokbo > POINT_DIE && nCurJokbo <= POINT_ALI)
			{
				m_CSutdaRule[i].PState(nPState);
				m_CSutdaRule[i].PenaltyRatio(dPenaltyRatio);
			}else if(nCurJokbo == POINT_DDANGKILLER)
			{
				m_CSutdaRule[i].PState(nPState);
				m_CSutdaRule[i].PenaltyRatio(dPenaltyRatio);
			}else if(nCurJokbo == POINT_SAGU)
			{
				m_CSutdaRule[i].PState(PSTATE_49);
				m_CSutdaRule[i].PenaltyRatio(dPenaltyRatio);
				nLastSagu = i;
			}else if(nCurJokbo == POINT_SAGU2)
			{
				m_CSutdaRule[i].PState(PSTATE_49);
				m_CSutdaRule[i].PenaltyRatio(dPenaltyRatio);
				if(nLastSagu < MAX_PLAYER)
				{
					m_CSutdaRule[nLastSagu].PState(nPState);
				}
			}else if(nCurJokbo == m_nWinnerJokbo)
			{
				m_CSutdaRule[i].PState(MAX_PSTATE);
				m_CSutdaRule[i].PenaltyRatio(0.0);
			}
		}
	}else if(m_nWinnerJokbo == POINT_38_KWANGDDANG)
	{
		nPState = PSTATE_38GWANGDDANG;
		dPenaltyRatio = 0.1;
		int nLastSagu = MAX_PLAYER;
		for(int i=0; i<MAX_PLAYER; i++)
		{
			int nCurJokbo = m_CSutdaRule[i].ChoiceJokbo();
			if(nCurJokbo > POINT_DIE && nCurJokbo <= POINT_ALI)
			{
				m_CSutdaRule[i].PState(nPState);
				m_CSutdaRule[i].PenaltyRatio(dPenaltyRatio);
			}else if(nCurJokbo == POINT_DDANGKILLER)
			{
				m_CSutdaRule[i].PState(nPState);
				m_CSutdaRule[i].PenaltyRatio(dPenaltyRatio);
			}else if(nCurJokbo == POINT_PARKMOONSU)
			{
				m_CSutdaRule[i].PState(nPState);
				m_CSutdaRule[i].PenaltyRatio(dPenaltyRatio);
			}else if(nCurJokbo == POINT_SAGU)
			{
				m_CSutdaRule[i].PState(PSTATE_49);
				m_CSutdaRule[i].PenaltyRatio(dPenaltyRatio);
				nLastSagu = i;
			}else if(nCurJokbo == POINT_SAGU2)
			{
				m_CSutdaRule[i].PState(PSTATE_49);
				m_CSutdaRule[i].PenaltyRatio(dPenaltyRatio);
				if(nLastSagu < MAX_PLAYER)
				{
					m_CSutdaRule[nLastSagu].PState(nPState);
				}
			}else if(nCurJokbo == m_nWinnerJokbo)
			{
				m_CSutdaRule[i].PState(MAX_PSTATE);
				m_CSutdaRule[i].PenaltyRatio(0.0);
			}
		}
	}


// 	// ������ ���� �� ���
// 	if(nPState != PSTATE_NONE)
// 	{
// 		int nLastSagu = MAX_PLAYER;
// 		for(int i=0; i<MAX_PLAYER; i++)
// 		{
// 			int nCurJokbo = m_CSutdaRule[i].ChoiceJokbo();
// 			if(nCurJokbo > POINT_DIE && nCurJokbo <= POINT_ALI)
// 			{
// 				m_CSutdaRule[i].PState(nPState);
// 				m_CSutdaRule[i].PenaltyRatio(dPenaltyRatio);
// 			}else if(nCurJokbo == POINT_SAGU)
// 			{
// 				m_CSutdaRule[i].PState(PSTATE_49);
// 				m_CSutdaRule[i].PenaltyRatio(dPenaltyRatio);
// 				nLastSagu = i;
// 			}else if(nCurJokbo == POINT_SAGU2)
// 			{
// 				m_CSutdaRule[i].PState(PSTATE_49);
// 				m_CSutdaRule[i].PenaltyRatio(dPenaltyRatio);
// 				if(nLastSagu < MAX_PLAYER)
// 				{
// 					m_CSutdaRule[nLastSagu].PState(nPState);
// 				}
// 			}else if(nCurJokbo == m_nWinnerJokbo)
// 			{
// 				m_CSutdaRule[i].PState(MAX_PSTATE);
// 				m_CSutdaRule[i].PenaltyRatio(0.0);
// 			}
// 		}
// 	}
}

// void CSutdaRuleMan::GetJokbo(int PNum, char jokbo[3])
// {
// 	for(int i=0; i<MAX_CSTATE; i++)
// 	{
// 		jokbo[i] = m_CSutdaRule[PNum].m_nJokbo[i];
// 	}
// }

int CSutdaRuleMan::GetWInner()
{
	for(int i=0; i<MAX_PLAYER; i++)
	{
		if(0 == m_CSutdaRule[i].Ranking())
		{
			return i;
		}
	}
	return 0;
}

void CSutdaRuleMan::GetWinnerIndex(bool user[MAX_PLAYER])
{
	for(int i=0; i<MAX_PLAYER; i++)
	{
		if(0 == m_CSutdaRule[i].Ranking())
		{
			user[i] = true;
		}else
		{
			user[i] = false;
		}
	}
}

int CSutdaRuleMan::IsSpecial_Rule(int nPNum)
{
	if(nPNum < 0 || nPNum >= MAX_PLAYER) return -1;
	if(!m_CSutdaRule[nPNum].Valid()) return -1;

	switch(m_CSutdaRule[nPNum].ChoiceJokbo())
	{
	case POINT_ONEDDANG:
	case POINT_TWODDANG:
	case POINT_THREEDDANG:
	case POINT_FOURDDANG:
	case POINT_FIVEDDANG:
	case POINT_SIXDDANG:
	case POINT_SEVENDDANG:
	case POINT_EIGHTDDANG:
	case POINT_NINEDDANG:
	case POINT_TENDDANG:
		return CODE_SPECIAL_DDANG;
		break;
	case POINT_13_KWANGDDANG:
		return CODE_SPECIAL_GWANG;
		break;
	case POINT_18_KWANGDDANG:
		return CODE_SPECIAL_GWANG;
		break;
	case POINT_38_KWANGDDANG:
		return CODE_SPECIAL_38GWANG;
		break;
	}
	return CODE_SPECIAL_NONE;
}