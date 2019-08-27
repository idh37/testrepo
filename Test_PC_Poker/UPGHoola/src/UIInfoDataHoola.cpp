// SprMoneyDraw.cpp: implementation of the CSprMoneyDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UIInfoDataHoola.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CUIInfoDataHoola::CUIInfoDataHoola()
{
	Init();
}

CUIInfoDataHoola::~CUIInfoDataHoola()
{
}

void CUIInfoDataHoola::Init(void)
{
	m_clrBtnBetText = RGB(108, 170, 2556);
	m_clrBtnBetTextShadow = RGB(18, 57, 88);

	m_listCardStartPos.clear();
	m_listCardStartPos.push_back(CPoint(454, 548));
	m_listCardStartPos.push_back(CPoint(140, 331));
	m_listCardStartPos.push_back(CPoint(140, 136));
	m_listCardStartPos.push_back(CPoint(667, 136));
	m_listCardStartPos.push_back(CPoint(667, 331));

	m_listAvatarPos.clear();
	m_listAvatarPos.push_back(CPoint(323, 598));
	m_listAvatarPos.push_back(CPoint(16, 312));
	m_listAvatarPos.push_back(CPoint(16, 121));
	m_listAvatarPos.push_back(CPoint(911, 121));
	m_listAvatarPos.push_back(CPoint(911, 312));

	// 아바타를 기준으로한 상대좌표
	m_listSeatKeeperWinPos.clear();
	m_listSeatKeeperWinPos.push_back(CPoint(99, -48));
	m_listSeatKeeperWinPos.push_back(CPoint(-2, 114));
	m_listSeatKeeperWinPos.push_back(CPoint(-2, 114));
	m_listSeatKeeperWinPos.push_back(CPoint(-76, 114));
	m_listSeatKeeperWinPos.push_back(CPoint(-76, 114));

	m_rtJokoName						= CRect(454,521,454+200,521+14);

	m_listUserMoneyRect[0]				= CRect(454, 674, 454+163, 674+15);

	m_listButtonPos[EBT_SEATKEEPER] = CPoint(431 - 15, 710); // 자리지킴이 버튼 위치
	m_listButtonPos[EBT_ONESHOTCHARGE] = CPoint(492 - 15, 710); // 원샷충전 버튼 위치
	m_listButtonPos[EBT_GIVEUP] = CPoint(538 - 15, 709); // 기권 버튼 위치
	m_listButtonPos[EBT_SINOUT] = CPoint(576, 709); // 관전 버튼 위치
	m_listButtonPos[EBT_BOTTOM_EXIT] = CPoint(616, 709); // 나가기 버튼 위치
	m_listButtonPos[EBT_WAITUSER] = CPoint(657, 710); // 대기 버튼 위치

	m_rtResultJokboYouSize				= CRect(0,0,194,20);
	m_rtResultMoneyYouSize				= CRect(0,0,187,19);
	
	m_rtResultJokboMySize				= CRect(0,0,200,20);
	m_rtResultMoneyMySize				= CRect(0,0,193,19);

	m_listPlayerBackPos.clear();
	m_listPlayerBackPos.push_back(CPoint(301,513));
	m_listPlayerBackPos.push_back(CPoint(14,386));
	m_listPlayerBackPos.push_back(CPoint(14,195));
	m_listPlayerBackPos.push_back(CPoint(855,195));
	m_listPlayerBackPos.push_back(CPoint(855,385));

	//훌라 버튼 위치 설정
	m_mapHoolaBtnPos.insert( make_pair<ENUM_BUTTON_TYPE_HOOLA, CPoint>(EBT_HOOL_STOP,				CPoint(803, 643)));
	m_mapHoolaBtnPos.insert( make_pair<ENUM_BUTTON_TYPE_HOOLA, CPoint>(EBT_HOOL_REGIST,				CPoint(803, 583)));
	m_mapHoolaBtnPos.insert( make_pair<ENUM_BUTTON_TYPE_HOOLA, CPoint>(EBT_HOOL_THANK,				CPoint(715, 523)));
	m_mapHoolaBtnPos.insert( make_pair<ENUM_BUTTON_TYPE_HOOLA, CPoint>(EBT_HOOL_THANKSELECT,		CPoint(715, 523)));
	m_mapHoolaBtnPos.insert( make_pair<ENUM_BUTTON_TYPE_HOOLA, CPoint>(EBT_HOOL_LOWTHANK,			CPoint(715, 523)));
	m_mapHoolaBtnPos.insert( make_pair<ENUM_BUTTON_TYPE_HOOLA, CPoint>(EBT_HOOL_HIGHTHANK,			CPoint(715, 523)));
	m_mapHoolaBtnPos.insert( make_pair<ENUM_BUTTON_TYPE_HOOLA, CPoint>(EBT_HOOL_SEVENFOURCARDTHANK,	CPoint(715, 523)));
	m_mapHoolaBtnPos.insert( make_pair<ENUM_BUTTON_TYPE_HOOLA, CPoint>(EBT_HOOL_PASS,				CPoint(804, 643)));
	m_mapHoolaBtnPos.insert( make_pair<ENUM_BUTTON_TYPE_HOOLA, CPoint>(EBT_HOOL_GET,				CPoint(715, 643)));
	m_mapHoolaBtnPos.insert( make_pair<ENUM_BUTTON_TYPE_HOOLA, CPoint>(EBT_HOOL_DUMP,				CPoint(715, 643)));
	m_mapHoolaBtnPos.insert( make_pair<ENUM_BUTTON_TYPE_HOOLA, CPoint>(EBT_HOOL_SORT_STRAIGHT,		CPoint(715, 583)));
	m_mapHoolaBtnPos.insert( make_pair<ENUM_BUTTON_TYPE_HOOLA, CPoint>(EBT_HOOL_SORT_TOGETHER,		CPoint(715, 583)));
	m_mapHoolaBtnPos.insert( make_pair<ENUM_BUTTON_TYPE_HOOLA, CPoint>(EBT_HOOL_RESULT,				CPoint(434, 427)));
	m_mapHoolaBtnPos.insert( make_pair<ENUM_BUTTON_TYPE_HOOLA, CPoint>(EBT_HOOL_GAMESTART,			CPoint(406, 291)));
	m_mapHoolaBtnPos.insert( make_pair<ENUM_BUTTON_TYPE_HOOLA, CPoint>(EBT_HOOL_PRACTICS,			CPoint(408, 368)));

	m_ptGold = CRect(471, 694, 471+110, 694+11);
	//m_ptGold = CRect(472, 689, 472+110, 689+11);

	//채팅영역 이벤트 버튼 위치
	m_listButtonPos[EBT_EVENT] = CPoint(153, 514);
}


CPoint CUIInfoDataHoola::GetHoolaBtnPos(ENUM_BUTTON_TYPE_HOOLA eHoolaPos)
{	
	if (m_mapHoolaBtnPos.find(eHoolaPos) == m_mapHoolaBtnPos.end())
	{
		ASSERT(0); //정의 되지않은 버튼
		return CPoint(-1000, 1000);
	}

	return m_mapHoolaBtnPos[eHoolaPos];	
}
