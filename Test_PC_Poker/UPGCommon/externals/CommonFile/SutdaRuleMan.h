#pragma once

class CSutdaRuleMan
{
public:
	CSutdaRuleMan();
	virtual ~CSutdaRuleMan();

public:
	void SD_Init(int nGameMode = SD_2CARD);
	void SD_Clear();
	void SD_SetUserCard(int pNum, int cardidx[], int cardCnt, bool bFirst = true);
	void SD_FoldUser(int pNum);

	void SD_SortRanking();					// 순위 계산
	void SD_CheckPenalty();					// 땡값, 사구박 체크

	int WinnerCnt() const { return m_nWinnerCnt; }
	void WinnerCnt(int val) { m_nWinnerCnt = val; }

	int WinnerJokbo() const { return m_nWinnerJokbo; }
	void WinnerJokbo(int val) { m_nWinnerJokbo = val; }

	bool IsRematch49() const { return m_IsRematch49; }
	void IsRematch49(bool val) { m_IsRematch49 = val; }

	bool IsRematchDraw() const { return m_IsRematchDraw; }
	void IsRematchDraw(bool val) { m_IsRematchDraw = val; }

	bool IsReEntry() const { return m_IsReEntry; }
	void IsReEntry(bool val) { m_IsReEntry = val; }

	//void GetJokbo(int PNum, char jokbo[3]);

	int GetWInner();
	void GetWinnerIndex(bool user[MAX_PLAYER]);

	int CurPlayerCnt() const { return m_nCurPlayerCnt; }
	void CurPlayerCnt(int val) { m_nCurPlayerCnt = val; }

	int IsSpecial_Rule(int nPNum);

public:
	CSutdaRule m_CSutdaRule[MAX_PLAYER];
	int m_nTotalPlayerCnt;					// 게임 시작시 참여한 플래이어 수
	int m_nCurPlayerCnt;					// 현에 참여중인 플래이어 수
	int m_nWinnerCnt;						// 승리자 수
	int m_nWinnerJokbo;						// 승리자 족보
private:
	bool m_IsRematch49;						// 49 재경기 판 체크
	bool m_IsRematchDraw;					// 동일패 재경기 판 체크
	bool m_IsReEntry;						// 재경기 참가 가능한지, (49 재경기면 참가 가능하지만 동일패 재경기 일 경우 참가 불능, ex>동일패 -> 49 재경기 일 경우 참가 불능)
};