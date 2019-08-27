#pragma once
#include "Chip.h"

#define MAX_CHIP (30)




class CChipBoard
{

	NMBASE::GRAPHICGDI::xSprite		*m_pSprBack;
	NMBASE::GRAPHICGDI::xSprite		m_sprChip;
	NMBASE::GRAPHICGDI::CPage		 m_BoardPage;
	NMBASE::GRAPHICGDI::CPage		*m_pMainPage;

	CChip m_Chip[MAX_CHIP];

	BOOL m_AllinBetting[MAX_PLAYER];
	
	//BYTE m_ChipCnt[MAX_CHIPCNT];	// 배팅된 칩의 개수(칩의 종류별)8
public:
	CChipBoard(void);
	~CChipBoard(void);


	void Init(NMBASE::GRAPHICGDI::CPage *pMainPage, NMBASE::GRAPHICGDI::xSprite* pSprBack);
	void Reset();
	void LoadImage();
	void OnTimer();
	void Draw_Bottom();
	void Draw_Top();

	void PreDrawCastedChip(INT64 llTotalBet);
	void CastChip(int pnum, INT64 nMarble, int sdelay, INT64 llCurMoney);



	//배팅에 따른 효과음 배팅칩소리
	void SetBetEft( INT64 r_money , INT64 llCurMoney);

};
