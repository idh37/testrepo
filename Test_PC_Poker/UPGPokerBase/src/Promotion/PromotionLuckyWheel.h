#pragma once

#include "PromotionBase.h"
#include <EventCommon.h>

class UPG_GAMEBASE_EXPORT CPromotionLuckyWheel : public CPromotionBase
{
public:
	CPromotionLuckyWheel( CPromotionManager *pManager, int nType );
	virtual ~CPromotionLuckyWheel(void);

protected:
	NMBASE::GRAPHICGDI::xSprite	m_sprBack;
	promotion::VISITE_PROMOTION_ALARM m_PromotionInfo;
	CEffectBase m_cEffect_WheelPopup;
	CString m_sMoney;
	bool m_bDrawTip;
	bool m_bJokboEffectStart;
	int m_nPlayTime;
public:
	LRESULT ShowRewardPopup(WPARAM &wParam, LPARAM &lParam);
	void SetEventInfo( promotion::VISITE_PROMOTION_ALARM *pInfo );	//판 진행시 마다
	bool IsEndEvent(void);
	bool IsEventRoom(void);

	virtual void DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	
	virtual void Accept_OutRoom();
	virtual void Accept_CreateRoom();
	virtual void Accept_EnterRoom();
	virtual void OnEnterRoom();
	virtual void OnEndGame();

	virtual BOOL OnLButtonDownMiddle(int &x , int &y);
	virtual BOOL OnLButtonUpMiddle(int &x , int &y);
	virtual BOOL OnMouseMoveMiddle(int &x , int &y);

	void OnTimer();
	void CurrentRewardTextDraw(CDC *pDC, int x, int y);

	virtual BOOL OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData);
};
