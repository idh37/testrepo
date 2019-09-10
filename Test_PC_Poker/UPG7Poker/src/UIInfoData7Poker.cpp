// SprMoneyDraw.cpp: implementation of the CSprMoneyDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UIInfoData7Poker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CUIInfoData7Poker::CUIInfoData7Poker()
{
	Init();
}

CUIInfoData7Poker::~CUIInfoData7Poker()
{
}

void CUIInfoData7Poker::Init(void)
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
	m_listAvatarPos.push_back(CPoint(354, 584));
	m_listAvatarPos.push_back(CPoint(36, 369));
	m_listAvatarPos.push_back(CPoint(36, 174));
	m_listAvatarPos.push_back(CPoint(902, 174));
	m_listAvatarPos.push_back(CPoint(902, 369));

	m_rtJokoName				= CRect(454,521,454+200,521+14);

	m_listUserMoneyRect[0] = CRect(455, 668, 455+135, 668+15);

	m_listButtonPos[EBT_SEATKEEPER] = CPoint(431, 710); // �ڸ���Ŵ�� ��ư ��ġ
	m_listButtonPos[EBT_ONESHOTCHARGE] = CPoint(492, 710); // �������� ��ư ��ġ
	m_listButtonPos[EBT_CARD_OPEN] = CPoint(538, 710); // OPEN ��ư ��ġ
	m_listButtonPos[EBT_SINOUT] = CPoint(576, 709); // ���� ��ư ��ġ
	m_listButtonPos[EBT_BOTTOM_EXIT] = CPoint(616, 709); // ������ ��ư ��ġ
	m_listButtonPos[EBT_WAITUSER] = CPoint(657, 710); // ��� ��ư ��ġ

	m_rtResultJokboYouSize			= CRect(0,0,194,20);
	m_rtResultMoneyYouSize			= CRect(0,0,187,19);

	m_rtResultJokboMySize			= CRect(0,0,200,20);
	m_rtResultMoneyMySize			= CRect(0,0,193,19);

	m_ptGold						= CRect(472, 689, 472+110, 689+11);
}