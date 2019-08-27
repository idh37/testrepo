// UIInfoData.h: interface for the CSprMoneyDraw class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>

class CUIInfoData7Poker : public CUIInfoData
{
public:
	CUIInfoData7Poker();
	virtual ~CUIInfoData7Poker();

	virtual void Init(void);

// public:
// 	CPoint	m_CardPosGap[3];		// 카드 위치 변경에 따른 위치값 변화 (순수 카드 위치만 변경시키고 싶을경우에 0이외 값을 넣어 준다.)
};