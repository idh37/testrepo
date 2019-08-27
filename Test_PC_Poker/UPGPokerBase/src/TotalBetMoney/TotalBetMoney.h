#pragma once

#include "../Player/SprMoneyDraw.h"

class CUIInfoData;

class UPG_GAMEBASE_EXPORT CTotalBetMoney
{
protected:
 	CSprMoneyDraw	m_sprPublicTotalMoney;
 	CSprMoneyDraw	m_sprPublicCallMoney;

	CUIInfoData		*m_pUIInfoData;
	CGameProcess	*m_pGameProcess;

	NMBASE::GRAPHICGDI::xSprite			m_sprTotalMoney;
	NMBASE::GRAPHICGDI::xSprite			m_sprTotalGold;
	NMBASE::GRAPHICGDI::xSprite			m_sprCallMoney;
	NMBASE::GRAPHICGDI::xSprite			m_sprCallGold;
	bool			m_bDraw;
public:
	CTotalBetMoney();
	virtual ~CTotalBetMoney();

	virtual void LoadImage(LPCTSTR szPathTotalMoney=NULL, LPCTSTR szPathCallMoney=NULL);
	virtual void Init(CUIInfoData *pUIInfoData, CGameProcess *pGameProcess, NMBASE::GRAPHICGDI::CPage *pPage);

	virtual void Draw(CDC *pDC);
	virtual void Reset(void);
	virtual void Start(void);
};