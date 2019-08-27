#pragma once


#include "JokboTable.h"

class CPlayerDraw_SD :
	public CPlayerDraw
{

	CJokboTable m_cJokboTable;

	bool		m_bShowPenaltyMoney;
	int			m_TangDelayFlag;
	int			m_TangDy;
	
	
public:
	CPlayerDraw_SD(void);
	~CPlayerDraw_SD(void);

// 
	virtual void Reset(void);
	virtual void Init(CPlayer *pPlayer, int nNum, NMBASE::GRAPHICGDI::CPage *pPage, CPlayerDrawManager *pManager);
	virtual void InitUIData(void);

	virtual void DrawJokboName(CDC *pDC);
	virtual void UpdateJokboName();

	virtual void DrawResult(CDC *pDC);
	virtual void DrawTimer(CDC *pDC);


	virtual BOOL OnMouseMove(int x, int y);
	

	virtual void DrawBottom(CDC *pDC);
	virtual void DrawMiddle(CDC *pDC);
	virtual void DrawTop(CDC *pDC);
	virtual void DrawTopMost(CDC *pDC);

	virtual void DrawPenaltyMoney(CDC *pDC);	//¶¯°ª±×¸®±â
	virtual void ShowPenaltyMoney(bool bShow) { m_bShowPenaltyMoney= bShow; m_TangDelayFlag=0; m_TangDy=0; }
	
	
};
