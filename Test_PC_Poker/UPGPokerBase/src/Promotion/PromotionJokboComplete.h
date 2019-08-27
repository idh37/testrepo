#pragma once

#include "PromotionBase.h"
#include <EventCommon.h>

class UPG_GAMEBASE_EXPORT CPromotionJokboComplete : public CPromotionBase
{
protected:
	xSprite						m_sprBack;
	xSprite						m_sprCard;

	promotion::OCTOBER_ALRAM	m_cPromotionInfo;
	int							m_nPlayRound;
	int							m_nPlayCount;
public:
	CPromotionJokboComplete(CPromotionManager *pManager, int nType);
	virtual ~CPromotionJokboComplete();

	void SetStartInfo( promotion::OCTOBER_ALRAM *pInfo );	//접속시 & 이벤트 시작시	
	void SetEventInfo( promotion::OCTOBER_ALRAM *pInfo );	//판 진행시 마다
	void SetLimitCnt( int nCnt );

	virtual BOOL OnLButtonDownBottom(int &x , int &y);
	virtual BOOL OnLButtonUpBottom(int &x , int &y);
	virtual BOOL OnMouseMoveBottom(int &x , int &y);

	virtual BOOL OnLButtonDownMiddle(int &x , int &y);
	virtual BOOL OnLButtonUpMiddle(int &x , int &y);
	virtual BOOL OnMouseMoveMiddle(int &x , int &y);

	virtual BOOL OnLButtonDownTop(int &x , int &y);
	virtual BOOL OnLButtonUpTop(int &x , int &y);
	virtual BOOL OnMouseMoveTop(int &x , int &y);

	virtual BOOL OnEventTabLButtonDown(int &x , int &y);
	virtual BOOL OnEventTabLButtonUp(int &x , int &y);
	virtual BOOL OnEventTabMouseMove(int &x , int &y);

	virtual BOOL OnPacketNotify(int &Sid, int &Signal, int &TotSize, char *lpData);
	virtual BOOL OnCommand(WPARAM &wParam, LPARAM &lParam);
	
	virtual void DrawBottom(CDC *pDC, CPage *pPage);
	virtual void DrawMiddle(CDC *pDC, CPage *pPage);
	virtual void DrawTop(CDC *pDC, CPage *pPage);

	virtual void DrawEventTab(CDC *pDC, CPage *pPage, const CPoint &ptStart);
};