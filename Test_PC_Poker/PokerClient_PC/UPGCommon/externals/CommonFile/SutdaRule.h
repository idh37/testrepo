#pragma once

class CSutdaRule
{
public:
	CSutdaRule();
	virtual ~CSutdaRule();

public:
	void Init(int GameMode);	// �ʱ�ȭ
	void Clear();				// ���� ����۽� �� ����, ���� ���� �ʱ�ȭ

	void SetCard(int pNum, int cardIdx[], int cardCnt);		// �� �Է�
	void CalcuRule(int nChoiceIdx);							// ���� ���
	void CalcuRule();										// ��� ���� ���

	int RankState();										// �� ��� ��ȯ(RANKSTATE_WINNER, RANKSTATE_LOSER, RANKSTATE_FOLDER �� ����)
	void RankState(int rankState) { m_nRankState = rankState; }

	int GameMode() const { return m_nGameMode; }
	void GameMode(int val) { m_nGameMode = val; }

	int ChoiceIdx() const { return m_nChoiceIdx; }
	void ChoiceIdx(int val);

	int ChoiceNormalJokbo() const { return m_nChoiceJokbo; }
	void ChoiceNormalJokbo(int val) { m_nChoiceJokbo = val; }

	int ChoiceSpecialJokbo() const { return m_nChoiceSpecialJokbo; }
	void ChoiceSpecialJokbo(int val) { m_nChoiceSpecialJokbo = val; }

	int ChoiceJokbo() const;			// Ư�� ������ ������ Ư�� ������ ��ȯ�ϰ�, �ƴϸ� ������ ������ �Ϲ� ������ ��ȯ�Ѵ�.

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
	int m_nCardIdx[SD_MAX_USER_CARD_COUNT];	// ���� ��
	int m_nTotalCardCnt;					// ���� ī�� ��
	int m_nChoiceIdx;						// ���õ� ����(3�� ���ٿ����� CSTATE_FIRST, CSTATE_SECOND, CSTATE_THIRD �� �ϳ�, 2�� ���ٿ����� ������ CSTATE_FIRST)
public:
	int m_nJokbo[MAX_CSTATE];				// 3���� ���� 
private:
	int m_nChoiceJokbo;						// ������ ����
	int m_nChoiceSpecialJokbo;				// ������ Ư�� ����
	int m_nRankState;						// �� ���
	int m_nGameMode;						// ���� ���
private:
	bool m_bValid;
	int m_nPNum;

private:
	int m_nRanking;							// ���� ����, (5 : 1�� ~ 1 : 5��, 0 : Fold)
	PENALTY_STATE m_nPState;				// �г�Ƽ ����
	double m_PenaltyRatio;
};