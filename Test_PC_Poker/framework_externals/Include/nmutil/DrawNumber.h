#pragma once

#include "UtilDefine.h"

class NMUTIL_CLASS CDrawNumber
{
public:
	enum STYLE_DRAWNUMBER
	{
		STYLE_DRAWNUMBER_NULL,
		STYLE_DRAWNUMBER_MONEY_STRING1,
		STYLE_DRAWNUMBER_MONEY_COMMA_FROM_LEFT, 
		STYLE_DRAWNUMBER_MONEY_COMMA_FROM_RIGHT,

	};
private:
	STYLE_DRAWNUMBER m_style;

	INT64 m_llNumber;
	xSprite *m_pSprite;
	int m_nSprOffset;
	int	m_nDigitSpace;
	int m_nPosX, m_nPosY;

	CRect m_rtNumber;
	

	CString MakeMoneyShort(INT64 nMoney, BOOL bSign);
public:
	CDrawNumber();
	~CDrawNumber(void);

	void SetPosition(int nPosX, int nPosY);
	void SetNumber(INT64 llNumber);
	void SetDigitSpace(int nDigitSpace);
	void SetSprite(xSprite *pSprite, int nSprOffset);
	void SetStyle(STYLE_DRAWNUMBER style);
	void Draw(CPage &Page, CDC &DC);
	CRect GetNumberRect();
	

	void Draw_Style_MoneyString1(CPage &Page, CDC &DC);
	void Draw_Style_MoneyComma_Left(CPage &Page, CDC &DC);
	void Draw_Style_MoneyComma_Right(CPage &Page, CDC &DC);
};
