
#pragma once

#include "PromotionBase.h"


class UPG_GAMEBASE_EXPORT CPromotionChuSeokGoldCoin : public CPromotionBase
{
private:	

	static INT64 RUN_LOWER_LIMIT;
	static INT64 RUN_UPPER_LIMIT;

	static const int MAX_NUMBER_EFFECT = 30;
	static const int UNIT_ONE_EFFECT = 5;

	NMBASE::GRAPHICGDI::xSprite m_resource;
	CEffectBase m_goldCoinEffect;

	int m_curEventCnt;
	int m_playCnt;
	int m_curRemainAnimTime;
	int m_coinIndex;
	int m_rewardIndex;

	bool m_drawToolToip;

public:

	CPromotionChuSeokGoldCoin(CPromotionManager *pManager, int nType);
	virtual ~CPromotionChuSeokGoldCoin();	

	virtual void DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);	

	virtual BOOL OnMouseMoveMiddle(int& x , int& y);
	virtual void OnTimer();
	virtual BOOL OnPacketNotify(char* pName, int& signal, int& totSize, char* lpData);

	LRESULT ShowGoldCoinAnim(WPARAM& wParam, LPARAM& lParam);

private:

	void _DrawTextOfPromotionProgress(CDC* pDC, int x, int y);

	void _SetData(int totSize, char* lpData);
	void _SetDataLimits(int totSize, char* lpData);
	bool _IsEventRoom();
};
