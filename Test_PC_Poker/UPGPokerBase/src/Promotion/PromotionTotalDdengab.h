#pragma once

#include "PromotionBase.h"
#define IDM_NEWDDENGBANNER	(WM_USER+6230)

class UPG_GAMEBASE_EXPORT CPromotionTotalDdengab : public CPromotionBase
{
protected:
	NMBASE::GRAPHICGDI::xSprite				m_sprBack;
	int										m_roomIdx;
	BOOL									m_isOnBtn;
	BOOL									m_isClickBtn;
	BOOL									m_isRightRoom;
	BOOL									m_isShowReward;
	BOOL									m_isMyReward;
	
	std::vector<CPoint>						m_listRewardPos;
	int										m_curRewardIdx;
	int										m_scoreCode;
	INT64									m_rewardMoney;
	int										m_remainCnt;
	int										m_premState;
	
public:
	CPromotionTotalDdengab(CPromotionManager *pManager, int nType);
	virtual ~CPromotionTotalDdengab();
	
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
	void SetCurrentRoomInfo();
	void TextSetting(CDC *pDC, int x, int y);
	CString GetScoreName(int nValue);
	int GetScoreIdx(int nScore, int nSubScore);
	int GetPremIdx(int nValue);
};
