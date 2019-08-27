#pragma once

class CSutdaRule
{
public:
	CSutdaRule();
	virtual ~CSutdaRule();

public:
	void Init(int GameMode);	// 초기화
	void Clear();				// 게임 재시작시 패 정보, 족보 정보 초기화

	void SetCard(int pNum, int cardIdx[], int cardCnt);		// 패 입력
	void CalcuRule(int nChoiceIdx);							// 족보 계산
	void CalcuRule();										// 모든 족보 계산

	int RankState();										// 내 등수 반환(RANKSTATE_WINNER, RANKSTATE_LOSER, RANKSTATE_FOLDER 로 구분)
	void RankState(int rankState) { m_nRankState = rankState; }

	int GameMode() const { return m_nGameMode; }
	void GameMode(int val) { m_nGameMode = val; }

	int ChoiceIdx() const { return m_nChoiceIdx; }
	void ChoiceIdx(int val);

	int ChoiceNormalJokbo() const { return m_nChoiceJokbo; }
	void ChoiceNormalJokbo(int val) { m_nChoiceJokbo = val; }

	int ChoiceSpecialJokbo() const { return m_nChoiceSpecialJokbo; }
	void ChoiceSpecialJokbo(int val) { m_nChoiceSpecialJokbo = val; }

	int ChoiceJokbo() const;			// 특수 족보가 있으면 특수 족보를 반환하고, 아니면 족보가 있으면 일반 족보를 반환한다.

	bool Valid() const { return m_bValid; }
	void Valid(bool val) { m_bValid = val; }

	int Ranking() const { return m_nRanking; }
	void Ranking(int val) { m_nRanking = val; }

	PENALTY_STATE PState() const { return m_nPState; }
	void PState(PENALTY_STATE val) { m_nPState = val; }

	double PenaltyRatio() const { return m_PenaltyRatio; }
	void PenaltyRatio(double val) { m_PenaltyRatio = val; }

	void Fold();

	int GetJokboIndex_First();
	int GetJokboIndex_Second();

private:
	int m_nCardIdx[SD_MAX_USER_CARD_COUNT];	// 받은 패
	int m_nTotalCardCnt;					// 받은 카드 수
	int m_nChoiceIdx;						// 선택된 족보(3장 섯다에서는 CSTATE_FIRST, CSTATE_SECOND, CSTATE_THIRD 중 하나, 2장 섯다에서는 무조건 CSTATE_FIRST)
public:
	int m_nJokbo[MAX_CSTATE];				// 3개의 족보 
private:
	int m_nChoiceJokbo;						// 선택한 족보
	int m_nChoiceSpecialJokbo;				// 선택한 특수 족보
	int m_nRankState;						// 내 등수
	int m_nGameMode;						// 게임 모드
private:
	bool m_bValid;
	int m_nPNum;

private:
	int m_nRanking;							// 게임 순위, (5 : 1위 ~ 1 : 5위, 0 : Fold)
	PENALTY_STATE m_nPState;				// 패널티 종류
	double m_PenaltyRatio;
};