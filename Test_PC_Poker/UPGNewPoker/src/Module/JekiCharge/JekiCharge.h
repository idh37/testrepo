// JekiCharge.h: interface for the CJekiCharge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JEKICHARGE_H__B78F18D9_001B_48F9_9EFD_3A667BF88F35__INCLUDED_)
#define AFX_JEKICHARGE_H__B78F18D9_001B_48F9_9EFD_3A667BF88F35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "commmsgdef.h"
//재기충전
#define IDM_JEKI_CHARGE (WM_USER+9202)

class CJekiCharge  
{
	xSprite m_sprJeki, m_sprJekiChargeBtn;		//재기충전 이팩트 , 버튼

public:
	static CJekiCharge& GetInstance();
	CJekiCharge();
	virtual ~CJekiCharge();


	void LoadSprites();
	BOOL Init(NMBASE::GRAPHICGDI::CPage *pPage);
	void OnPacketNotify(int Sid, int Signal, int TotSize, char *lpData);
	void OnCommand(UINT nCmd);
	BOOL OnMouseMove(int x, int y);
	BOOL OnLButtonUp(int x, int y);
	BOOL OnLButtonDown(int x, int y);
	void OnDraw(CDC *pDC);

	void OnChargeMoney(CSV_CHANGE_MONEY& msg); //G_TFT
};

CJekiCharge& JekiCharge();

#endif // !defined(AFX_JEKICHARGE_H__B78F18D9_001B_48F9_9EFD_3A667BF88F35__INCLUDED_)
