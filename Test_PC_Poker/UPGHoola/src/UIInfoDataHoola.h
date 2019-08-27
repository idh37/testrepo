// UIInfoData.h: interface for the CSprMoneyDraw class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>

enum ENUM_BUTTON_TYPE_HOOLA
{
	EBT_HOOL_STOP = 0,
	EBT_HOOL_REGIST,
	EBT_HOOL_THANK,
	EBT_HOOL_LOWTHANK,
	EBT_HOOL_HIGHTHANK,
	EBT_HOOL_SEVENFOURCARDTHANK,
	EBT_HOOL_THANKSELECT,
	EBT_HOOL_GAMESTART,
	EBT_HOOL_PRACTICS,
	EBT_HOOL_PASS,
	EBT_HOOL_GET,
	EBT_HOOL_DUMP,
	EBT_HOOL_SORT_STRAIGHT,
	EBT_HOOL_SORT_TOGETHER,
	EBT_HOOL_RESULT,
};
class CUIInfoDataHoola : public CUIInfoData
{
public:

	std::map<ENUM_BUTTON_TYPE_HOOLA, CPoint>		m_mapHoolaBtnPos;

public:
	CUIInfoDataHoola();
	virtual ~CUIInfoDataHoola();

	virtual void Init(void);

	CPoint GetHoolaBtnPos(ENUM_BUTTON_TYPE_HOOLA eHoolaPos);

	// public:
	// 	CPoint	m_CardPosGap[3];		// 카드 위치 변경에 따른 위치값 변화 (순수 카드 위치만 변경시키고 싶을경우에 0이외 값을 넣어 준다.)
};