#pragma once
//사용안함
#define IDM_HOOLA_RESULTOK			(WM_USER+7110)		

class CGameResultViewMan
{
public:
	CGameResultViewMan(void);

	~CGameResultViewMan(void);

	void Init(NMBASE::GRAPHICGDI::CPage *ppage, CWnd *pParent);
	
	void SetGameOverResult(GAMEOVERRESULT gameResult) { m_GameResult = gameResult ;}
	void Reset();
	void OnGameOver(GAMEOVERRESULT* pGOR);

	void DrawResultView( CDC *pDC );
	void Draw( CDC *pDC );
	void Show( BOOL bFlag );

	BOOL OnLButtonDown(int x , int y);
	BOOL OnLButtonUp(int x , int y);
	BOOL OnMouseMove(int x , int y);

	void SetResultTimer(DWORD dwTime = 1880);
	void StopResultTimer();

	void SetJackpotDelay(){ m_bJackpotDelay = TRUE; }
	DWORD GetCalculatedResultDelay(GAMEOVERRESULT* pGOR);
	void PlayWinLoseSound();
	void PlayGameOverEffect();
	void OnTimer();

	void StopAllResultView();
protected:
	GAMEOVERRESULT m_GameResult;
	NMBASE::GRAPHICGDI::CPage *m_pPage;
	CWnd *m_pParent;
	CFont m_cFont;
	NMBASE::SKINGDI::CGraphButton	m_btnResultOK;
	BOOL m_bShow;

	DWORD m_dwDelayEndTime;
	DWORD m_dwDelayStartTime;
	BOOL m_bTimer;

	BOOL m_bJackpotDelay;


private:	
	/*void SetResultShowDelay( DWORD dwTime = 0 );*/
	inline BOOL IsWinCaseHoola(short nWinCase) { return ( nWinCase == 1 ) || ( nWinCase == 2 );}

};
