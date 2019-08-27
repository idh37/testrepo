// BombCard.h: interface for the CBombCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOMBCARD_H__5B093562_1BCE_4055_9D69_92A9ACD23316__INCLUDED_)
#define AFX_BOMBCARD_H__5B093562_1BCE_4055_9D69_92A9ACD23316__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MovementEx.h"

class CBombCard
{
protected:
	enum BOMB_CARD_STATUS
	{
		BCS_NOSHOW = -1,		//아직 화면에 보이지 않을때
		BCS_DEALING,			//처음 폭탄 카드가 딜링할때
		BCS_BOMBSET_EFFECT,		//딜링이 끝나고 폭탄 이펙트가 나올때
		BCS_NOTDEAL_BOMBSET,	//카드가 분배되기전 폭탄카드가 셋팅됬을때
		BCS_DEAL_SHOW,			//폭탄 카드 분배시 화면의 중앙으로 이동중
		BCS_DEAL_JOKER_EFFECT,	//폭탄 카드 분배시 화면의 중앙에서 카드오픈후 조커카드이면 나타나는 이펙트
		BCS_DEAL_BOMBSET_EFFECT,//폭탄 카드 분배시 화면의 중앙에서 카드오픈후 조커카드가 아닐때 나타나는 폭탄 이펙트
		BCS_DEAL_BOMBRESET,		//중앙에서 폭탄카드가 다시 셋팅되러 돌아갈때
		BCS_BOMBRESET,			//폭탄 카드가 오픈되고 보일때
	};

	BOMB_CARD_STATUS	m_nStatus;
	CPoint				m_ptPos;	// 카드 위치
	int					m_nCardNo;	// 카드 넘버

	CMovementEx			m_cMove;	// 이동

	int					m_nTotalFrame;
	DWORD				m_dwStartTime;
	DWORD				m_dwTimeGab;
public:
	CBombCard();
	virtual ~CBombCard();	
	
	void Clear();
	void ForceSet(int nCardNo);

	void OnSetBombCard(DWORD dwTimeGab);
	void OnShowBombCard(DWORD dwTimeGab);
	void OnShowJokerBombCard(void);

	void Draw(NMBASE::GRAPHICGDI::CPage *pPage);
	void SetBombCard();
	void SetOPenBombCard(char cardno);
	void RePtBombCard(void);
	void OnTimer();
};

#endif // !defined(AFX_BOMBCARD_H__5B093562_1BCE_4055_9D69_92A9ACD23316__INCLUDED_)
