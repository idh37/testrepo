#include "StdAfx.h"

#include "Define_Sutda.h"

CSutdaRule::CSutdaRule()
: m_nGameMode(DEFAULT_GAMEMODE)
{

}

CSutdaRule::~CSutdaRule()
{

}

void CSutdaRule::Init(int GameMode)	// 초기화
{
	m_nGameMode = GameMode;
	Clear();
}

void CSutdaRule::Clear()				// 게임 재시작시 패 정보, 족보 정보 초기화
{
	memset(m_nCardIdx, SD_EMPTY_CARD, sizeof(int)*SD_MAX_USER_CARD_COUNT);
	m_nTotalCardCnt = 0;
	m_nChoiceIdx = DEFAULT_CHOICE;
	memset(m_nJokbo, POINT_DIE, sizeof(int)*MAX_CSTATE);
	m_nChoiceJokbo = POINT_DIE;
	m_nChoiceSpecialJokbo = POINT_DIE;
	m_nRankState = 5;
	m_bValid = false;
	m_nPNum = -1;
	m_nRanking = 0;
	Valid(false);
	PState(PSTATE_NONE);
	PenaltyRatio(0.0);
}

void CSutdaRule::SetCard(int pNum, int cardIdx[], int cardCnt)	// 패 입력
{
	if(0 > pNum || MAX_PLAYER <= pNum)	return;
	if(NULL == cardIdx) return;
	if(cardCnt <= 0) return;

	Valid(true);
	m_nPNum = pNum;
	m_nTotalCardCnt = cardCnt;
	for(int i=0; i<cardCnt; ++i)
	{
		m_nCardIdx[i] = cardIdx[i];
	}
}

void CSutdaRule::CalcuRule(int nIndex)							// 족보 계산
{
	if(CSTATE_FIRST > nIndex || MAX_CSTATE <= nIndex) return;

	if(RANKSTATE_FOLDER == RankState())	// 기본은 RANKSTATE_LOSER 임, RANKSTAE_FOLDER 는 다이 한 유저
	{
		m_nChoiceJokbo = POINT_DIE;
		m_nChoiceSpecialJokbo = POINT_NONESPECIAL;
		m_nJokbo[nIndex] = POINT_DIE;
		return;
	}

	int firstCard = m_nCardIdx[CHOICE_ARRAY[nIndex][0]];
	int secondCard = m_nCardIdx[CHOICE_ARRAY[nIndex][1]];

	if(SD_EMPTY_CARD == firstCard || SD_EMPTY_CARD == secondCard)
	{
		m_nChoiceJokbo = POINT_ZERO;
		m_nChoiceSpecialJokbo = POINT_NONESPECIAL;
		m_nJokbo[nIndex] = POINT_ZERO;
		return;
	}

	int firstKind = (firstCard+1) / 2;
	int secondKind = (secondCard+1) / 2;


	int jokbo = POINT_ZERO;
	if((firstCard==5 && secondCard==15)||(firstCard==15 && secondCard==5))		// 삼팔광땡
		jokbo = POINT_38_KWANGDDANG;
	else if((firstCard==1 && secondCard==15)||(firstCard==15 && secondCard==1))	// 18광땡
		jokbo = POINT_18_KWANGDDANG;
	else if((firstCard==1 && secondCard==5)||(firstCard==5 && secondCard==1))	// 13광땡
		jokbo =POINT_13_KWANGDDANG;
	else if(firstKind==secondKind)												// 땡(1-10)
		jokbo = POINT_ALI+firstKind; // 16~
	else if((firstKind==2 && secondKind==1)||(firstKind==1 && secondKind==2))	// 얄리
		jokbo = POINT_ALI;
	else if((firstKind==1 && secondKind==4)||(firstKind==4 && secondKind==1))	// 독사
		jokbo = POINT_DOKSA;
	else if((firstKind==1 && secondKind==9)||(firstKind==9 && secondKind==1))	// 구삥
		jokbo = POINT_GUPING;
	else if((firstKind==10 && secondKind==1)||(firstKind==1 && secondKind==10))	// 장삥
		jokbo = POINT_JANGPING;
	else if((firstKind==10 && secondKind==4)||(firstKind==4 && secondKind==10))	// 장사
		jokbo = POINT_JANGSA;
	else if((firstKind==4 && secondKind==6)||(firstKind==6 && secondKind==4))	// 세륙
		jokbo = POINT_SERYUK;
	else																		// 끗
		jokbo = (firstKind + secondKind) % SD_MAX_CARD_KIND;

	//m_nChoiceJokbo = jokbo;
	m_nJokbo[nIndex] = jokbo;									// 해당 족보를 저장해 준다.


	//특수 족보
	int nSpecialJokbo = POINT_NONESPECIAL;
	if((firstCard==7 && secondCard==17)||(firstCard==17 && secondCard==7))		// 사구(멍텅구리)
		nSpecialJokbo = POINT_SAGU2;
	else if((firstKind==4 && secondKind==9)||(firstKind==9 && secondKind==4))	// 사구
		nSpecialJokbo = POINT_SAGU;
	else if((firstCard==7 && secondCard==13)||(firstCard==13 && secondCard==7))	//암행어사(광땡만잡음)
		nSpecialJokbo = POINT_PARKMOONSU;
	else if((firstCard==5 && secondCard==13)||(firstCard==13 && secondCard==5))	// 삼칠망통(땡잡이)
		nSpecialJokbo = POINT_DDANGKILLER;	
	else																		// 없음
		nSpecialJokbo = POINT_NONESPECIAL;

	//m_nChoiceSpecialJokbo = jokbo;

	// 49, 멍49, 암행어사, 땡잡이가 있을 경우 일반 족보는 망통으로 처리 한다.
	if(nSpecialJokbo != POINT_NONESPECIAL)
	{
		m_nJokbo[nIndex] = nSpecialJokbo;
	}
}

void CSutdaRule::CalcuRule()										// 모든 족보 계산
{
	for(int i=0; i<MAX_CSTATE; ++i)
	{
		CalcuRule(i);
	}
}

int CSutdaRule::RankState()										// 내 등수 반환(RANKSTATE_WINNER, RANKSTATE_LOSER, RANKSTATE_FOLDER 로 구분)
{
	return m_nRankState;
}

void CSutdaRule::ChoiceIdx(int val) 
{ 
	m_nChoiceIdx = val;
	if(m_nJokbo[m_nChoiceIdx] < POINT_NONESPECIAL)	// 일반 족보 이면 특수 족보를 POINT_NONESPECIAL 로 처리
	{
		m_nChoiceJokbo = m_nJokbo[m_nChoiceIdx];
		m_nChoiceSpecialJokbo = POINT_NONESPECIAL;
	}else											// 특수 족보 이면 일반 족보를 POINT_ZERO 로 처리
	{
		m_nChoiceJokbo = POINT_ZERO;
		m_nChoiceSpecialJokbo = m_nJokbo[m_nChoiceIdx];
	}
}

int CSutdaRule::ChoiceJokbo() const
{
	int nJokbo = ChoiceSpecialJokbo();
	if(nJokbo == POINT_NONESPECIAL)
	{
		nJokbo = ChoiceNormalJokbo();
	}

	return nJokbo;
}

void CSutdaRule::Fold()
{
	memset(m_nJokbo, POINT_DIE, sizeof(int)*MAX_CSTATE);
	ChoiceNormalJokbo(POINT_DIE);
	ChoiceSpecialJokbo(POINT_DIE);

}

int CSutdaRule::GetJokboIndex_First()
{
	int nIndex = 0;
	nIndex = m_nCardIdx[CHOICE_ARRAY[m_nChoiceIdx][0]];
	return nIndex;
}

int CSutdaRule::GetJokboIndex_Second()
{
	int nIndex = 0;
	nIndex = m_nCardIdx[CHOICE_ARRAY[m_nChoiceIdx][1]];
	return nIndex;
}