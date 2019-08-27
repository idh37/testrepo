#pragma once

#include "PromotionBase.h"

// SV_GAMEOVERRESULT 패킷보다 늦게 도착을하기 때문에 잭팟 패킷을 받은다음 바로 화면에 
// 결과를 출력을 해주면 될것 같습니다.

class UPG_GAMEBASE_EXPORT PromotionJokboJackpot : public CPromotionBase
{
public:
	PromotionJokboJackpot(CPromotionManager *pManager, int nType);
	~PromotionJokboJackpot(void);

public:
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

	virtual BOOL OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData);
	virtual BOOL OnCommand(WPARAM &wParam, LPARAM &lParam);

	virtual void DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);

	virtual void DrawEventTab(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, const CPoint &ptStart);

	virtual const bool IsShowTab(void);

	LRESULT SetJackpotEffectShow(WPARAM &wParam, LPARAM &lParam);			// 일반 이펙트가 노출되고 0.6초 후에 잭팟 이펙트를 노출 

private:
	CString	m_strJackpotMoney;
	CString m_strNickName;
	CString m_strJokbo;
	CString	m_strGameName;			//
	int		m_nRoomNumber;			// 방 번호
	
	BOOL	m_bJackpotShow;			// 잭팟이 터졌는지 체크
	BOOL	m_bMyRoomJackPot;		// 자신의 방에서 잭팟 터짐

	CRect	m_rtJackpotMoneyRect;

	NMBASE::GRAPHICGDI::xSprite m_sprEffect;

	DWORD	m_dwStartTime;
	DWORD	m_dwEndTime;	
	DWORD	m_dwCurTime;

	DWORD	m_dwWinDrawTime;

	BOOL    m_bJackpotEffectShow;	// 잭팟이펙트를 보여도 되는 시점

	BOOL	m_bJokboEffectStart;

	COLORREF m_sChatColor;
};
