// SprMoneyDraw.h: interface for the CSprMoneyDraw class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

class UPG_GAMEBASE_EXPORT CSprMoneyDraw  
{
protected:
	NMBASE::GRAPHICGDI::CPage	*m_pPage;				// ������	
	NMBASE::GRAPHICGDI::xSprite *m_pNumberSpr;			// ��������Ʈ 
	NMBASE::GRAPHICGDI::xSprite *m_pGoldSpr;			// ��� ��������Ʈ 

	INT64	m_i64Money;				// �Ӵ�
	int		m_listMoneyDeck[30];	// ��������Ʈ �ε���
	int		m_nTotalMoneyIndex;		// �Ӵ� �ε���
	int		m_nCutUnit;

	bool	m_bShow;
	UINT	m_uFormat;
	bool	m_bShowZero;
	CRect	m_rtDrawRect;
	CRect	m_rtRect;
	int		m_nSpaceBetweenLetters;	//�ڰ�
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