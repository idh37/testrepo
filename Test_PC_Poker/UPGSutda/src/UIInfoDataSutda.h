// UIInfoData.h: interface for the CSprMoneyDraw class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include <vector>

class CUIInfoDataSutda : public CUIInfoData
{
public:
	CUIInfoDataSutda();
	virtual ~CUIInfoDataSutda();

	virtual void Init(void);


	std::vector<CPoint>		m_listParkmoonsuPos;				//각 플레이어의 암행어사 이펙트 위치를 가지고 있는 리스트
	std::vector<CPoint>		m_listTangKillerPos;				//각 플레이어의 땡잡이 이펙트 위치를 가지고 있는 리스트
	std::vector<CPoint>		m_listResultStampPos;				//각 플레이어의 결과시 도장(승, 사구박, 올인 등) 이펙트 위치를 가지고 있는 리스트
	std::vector<CPoint>		m_listPenaltyMoneyPos;				//각 플레이어의 결과시 땡값머니 표시 위치를 가지고 있는 리스트
	std::vector<CPoint>		m_listChipStartPos;					//각 플레이어의 베팅시 칩 던지는 시작위치를 가지고 있는 리스트
	std::vector<CRect>		m_listJokboNameRect;				//각 플레이어의 결과시 족보이름 표시 위치를 가지고 있는 리스트


	CPoint		m_ptRematchNotice;								//재경기 알림창 애니메이션 위치

	CPoint		m_ptBetBoard;									//화면 중간 칩 베팅영역 위치

// public:
// 	CPoint	m_CardPosGap[3];		// 카드 위치 변경에 따른 위치값 변화 (순수 카드 위치만 변경시키고 싶을경우에 0이외 값을 넣어 준다.)
};