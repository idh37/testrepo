#pragma once

#include "PromotionBase.h"
#include <EventCommon.h>

class UPG_GAMEBASE_EXPORT CPromotionSuperMoon : public CPromotionBase
{
public:
	CPromotionSuperMoon( CPromotionManager *pManager, int nType );
	virtual ~CPromotionSuperMoon(void);

protected:
	NMBASE::GRAPHICGDI::xSprite	m_sprBack;
	CEffectBase m_cEffect_Moon;
	bool m_bDrawTip;

	int m_level;
	int m_count;
	int m_rewardCnt;
	bool m_giftOpen[4];
	bool m_isEnd;

public:
	void SetEventInfo( CSV_PLAYINFO_SUPERMOON *pInfo );	//판 진행시 마다
	void TextSetting(CDC *pDC, int x, int y);

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

	virtual void DrawEventTab(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, const CPoint &ptStart);

	virtual BOOL OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData);
};
