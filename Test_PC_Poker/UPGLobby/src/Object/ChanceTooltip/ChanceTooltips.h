#pragma once

class CChanceTooltips :
	public LSY::CGroup
{

	LSY::CSprite		*m_pSpr7Poker;
	LSY::CSprite		*m_pSprNewPoker;
	LSY::CSprite		*m_pSprBadugi;
	LSY::CSprite		*m_pSprSutda;
	LSY::CSprite		*m_pSprGisa7Poker;
	LSY::CSprite		*m_pSprGisaBadugi;
	LSY::CSprite		*m_pSprGisaNewPoker;
public:
	CChanceTooltips(void);
	~CChanceTooltips(void);

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	void ShowToolTip(int gamecode, int itemcode, int itemtype, LSY::CPoint pos);

};
