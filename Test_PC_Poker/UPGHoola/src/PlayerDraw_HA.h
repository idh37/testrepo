#pragma once


#include "SpriteList.h"

class CPlayerDraw_HA : public CPlayerDraw
{

public:
	CPlayerDraw_HA(void);
	~CPlayerDraw_HA(void);

	NMBASE::SKINGDI::CNText			*m_pMoneyStr;
//	NMBASE::SKINGDI::CMyBitmap		bmpNickNameBack;

	virtual void Reset(void);
	virtual void Init(CPlayer *pPlayer, int nNum, NMBASE::GRAPHICGDI::CPage *pPage, CPlayerDrawManager *pManager);
	virtual void InitUIData(void);

	virtual void DrawResult(CDC *pDC);

	virtual BOOL OnMouseMove(int x, int y);
	virtual BOOL OnLButtonDown(int x, int y);
	virtual BOOL OnLButtonUp(int x, int y);


// 	virtual void DrawBottom(CDC *pDC);
// 	virtual void DrawMiddle(CDC *pDC);
// 	virtual void DrawTop(CDC *pDC);
// 	virtual void DrawTopMost(CDC *pDC);
	virtual void DrawTurn(CDC *pDC);
	virtual void DrawAvatar(CDC *pDC);
	virtual void DrawUserBack(CDC *pDC);
	virtual void DrawNickName(CDC *pDC);
	virtual void DrawMoney(CDC *pDC);
	virtual void DrawLevel(CDC *pDC);
	virtual void DrawBossMark(CDC *pDC);

	BOOL IsPracticUser(); //케릭터가 연습게임 대상자일경우를 확인한다.
};
