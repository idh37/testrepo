#pragma once



class CPlayerDrawManager_SD :
	public CPlayerDrawManager
{

	NMBASE::GRAPHICGDI::xSprite m_JokboTableSpr;
public:
	CPlayerDrawManager_SD( int nMaxPlayer );
	~CPlayerDrawManager_SD(void);
	
	virtual void Reset();
	virtual void Reset(int nPNum);
	virtual void CheckResultValue(void);
	
	virtual void LoadImage(LPCTSTR lpPathTimer, 
							NMBASE::GRAPHICGDI::xSprite *pBetCmdSpr, 
							NMBASE::GRAPHICGDI::xSprite *pWinLoseBackSpr, 
							LPCTSTR lpAllinPath, 
							NMBASE::GRAPHICGDI::xSprite *pTurnSpr = NULL, 
							LPCTSTR szUserBGPath = NULL,
							LPCTSTR szLevelPath = NULL);

	virtual void ShowPenaltyMoney(int nPnum, bool bShow);


	virtual void PlayerJokboNameUpdate(int nPnum);
	friend CPlayerDraw_SD;
	
};
