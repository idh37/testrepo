// UIInfoData.h: interface for the CSprMoneyDraw class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>


enum ENUM_BUTTON_TYPE_HIGHLOW
{
	EBT_HILO_HIGH,
	EBT_HILO_LOW,
	EBT_HILO_SWING,
};
class CUIInfoDataHighLow : public CUIInfoData
{
public:

	std::map<ENUM_BUTTON_TYPE_HIGHLOW, CPoint>		m_mapHIghLowBtnPos;
	CRect											m_rtLowJokoName;			//로우 족보명 표시 영역
	COLORREF										m_clrLowJokboNameColor;		//로우족보 이름을 표시할때의 글자색
	COLORREF										m_clrSwingJokboNameColor;		//스윙족보 이름을 표시할때의 글자색


	CPoint											m_ptLowJokboHelpPercentGab;		//로우족보 퍼센트 표시 오프셋
	CPoint											m_ptSwingJokboHelpPercentGab;	//스윙족보(포카드이상) 퍼센트 표시 오프셋
public:
	CUIInfoDataHighLow();
	virtual ~CUIInfoDataHighLow();

	virtual void Init(void);

	CPoint GetHighLowBtnPos(ENUM_BUTTON_TYPE_HIGHLOW eHighlowPos);

// public:
// 	CPoint	m_CardPosGap[3];		// 카드 위치 변경에 따른 위치값 변화 (순수 카드 위치만 변경시키고 싶을경우에 0이외 값을 넣어 준다.)
};