// SprMoneyDraw.h: interface for the CSprMoneyDraw class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class UPG_GAMEBASE_EXPORT CSprMoneyDraw  
{
protected:
	NMBASE::GRAPHICGDI::CPage	*m_pPage;				// 페이지	
	NMBASE::GRAPHICGDI::xSprite *m_pNumberSpr;			// 스프라이트 
	NMBASE::GRAPHICGDI::xSprite *m_pGoldSpr;			// 골드 스프라이트 

	INT64	m_i64Money;				// 머니
	int		m_listMoneyDeck[30];	// 스프라이트 인덱스
	int		m_nTotalMoneyIndex;		// 머니 인덱스
	int		m_nCutUnit;

	bool	m_bShow;
	UINT	m_uFormat;
	bool	m_bShowZero;
	CRect	m_rtDrawRect;
	CRect	m_rtRect;
	int		m_nSpaceBetweenLetters;	//자간
public:
	CSprMoneyDraw();
	virtual ~CSprMoneyDraw();

	virtual void	Init(NMBASE::GRAPHICGDI::CPage *page, NMBASE::GRAPHICGDI::xSprite *spr, CRect rtRect, UINT uFormat, int nCutUnit, bool bShowZero, int nSpaceBetweenLetters = 0, NMBASE::GRAPHICGDI::xSprite *goldSpr = NULL);

	virtual void	ReSet();
	virtual void	Clear();

	virtual void	SetMoney(INT64 money, bool bShowZeroUnit);
	virtual void	Draw(int chipKind = 0);

	virtual void	SetRect(CRect rtRect);

	virtual bool	IsIn(const CPoint &ptPos);
	virtual CPoint	GetTopCenterPos(void);
	virtual CPoint	GetTopLeftPos(void);	
	virtual CRect   GetDrawRect(void){ return m_rtDrawRect; }
};