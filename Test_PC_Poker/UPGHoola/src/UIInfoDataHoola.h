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
	// 	CPoint	m_CardPosGap[3];		// ī�� ��ġ ���濡 ���� ��ġ�� ��ȭ (���� ī�� ��ġ�� �����Ű�� ������쿡 0�̿� ���� �־� �ش�.)
};