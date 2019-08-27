// g_cOneShotCharge.h: interface for the COneShotCharge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JEKICHARGE_H__B78F18D9_001B_48F9_9EFD_3A667BF88F35__INCLUDED_)
#define AFX_JEKICHARGE_H__B78F18D9_001B_48F9_9EFD_3A667BF88F35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "commmsgdef.h"


class COneShotCharge  
{
	// [재기충전 작업] [2009-02-12]
	//	PREMOPTION	m_PremOption;				// 프리미엄 옵션 상품

	xSprite m_JekiSpr;				// 재기충전 이팩트
	xSprite m_JekiChargeSprBtn;		// 재기충전 버튼

	NMBASE::SKINGDI::CGraphButton m_JekiChargeBtn;

public:
	static COneShotCharge& GetInstance();
	COneShotCharge();
	virtual ~COneShotCharge();


	void LoadSprites();
	BOOL Init(NMBASE::GRAPHICGDI::CPage *pPage);
	void OnPacketNotify(int Sid, int Signal, int TotSize, char *lpData);
	void OnChargeMoney(CSV_CHANGE_MONEY& msg); //G_TFT
	BOOL OnCommand(UINT nCmd);
	BOOL OnMouseMove(int x, int y);
	BOOL OnLButtonUp(int x, int y);
	BOOL OnLButtonDown(int x, int y);
	void OnDraw(CDC *pDC);
};

COneShotCharge& g_cOneShotCharge();

#endif // !defined(AFX_JEKICHARGE_H__B78F18D9_001B_48F9_9EFD_3A667BF88F35__INCLUDED_)
