#pragma once

#include "PromotionBase.h"
#include <EventCommon.h>

class UPG_GAMEBASE_EXPORT CPromotionJackpotRelease : public CPromotionBase
{
public:

	CPromotionJackpotRelease( CPromotionManager *pManager, int nType );
	virtual~ CPromotionJackpotRelease();


protected:

	NMBASE::GRAPHICGDI::xSprite	m_sprEvent;
	
	CRect m_rtInfo;	

	bool m_bEventShow;
	int m_AniIndex;		//상황별 멀티씬 인덱스
	int m_AniIndex_DP;	

	int m_JackPokWinnerEft;	//잭팟 당첨자가 있다. ( 0 : 이펙트 나온적 없음, 1 : 이펙트 진행중, 2 : 이펙트 종료 )

	//
	BOOL m_bReleaseInfo;
	DWORD m_dwRelaseInfoTime;

	//마우스 오버시
	BOOL m_bMouseOver;
	DWORD m_dwMouseTimer;

	jackpot::JP_EVENT_GIFTTIME m_Jackpot_Info;

public:	

	void SetEventInfo();

	virtual void OnStartGame();
	virtual void Accept_OutRoom();
	virtual void Accept_CreateRoom();

	virtual BOOL OnMouseMoveBottom(int &x , int &y);

	virtual BOOL OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData);

	virtual void DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);

	virtual const bool IsShowTab(void);

};
