#pragma once

#include "PromotionBase.h"

// SV_GAMEOVERRESULT ��Ŷ���� �ʰ� �������ϱ� ������ ���� ��Ŷ�� �������� �ٷ� ȭ�鿡 
// ����� ����� ���ָ� �ɰ� �����ϴ�.

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

	LRESULT SetJackpotEffectShow(WPARAM &wParam, LPARAM &lParam);			// �Ϲ� ����Ʈ�� ����ǰ� 0.6�� �Ŀ� ���� ����Ʈ�� ���� 

private:
	CString	m_strJackpotMoney;
	CString m_strNickName;
	CString m_strJokbo;
	CString	m_strGameName;			//
	int		m_nRoomNumber;			// �� ��ȣ
	
	BOOL	m_bJackpotShow;			// ������ �������� üũ
	BOOL	m_bMyRoomJackPot;		// �ڽ��� �濡�� ���� ����

	CRect	m_rtJackpotMoneyRect;

	NMBASE::GRAPHICGDI::xSprite m_sprEffect;

	DWORD	m_dwStartTime;
	DWORD	m_dwEndTime;	
	DWORD	m_dwCurTime;

	DWORD	m_dwWinDrawTime;

	BOOL    m_bJackpotEffectShow;	// ��������Ʈ�� ������ �Ǵ� ����

	BOOL	m_bJokboEffectStart;

	COLORREF m_sChatColor;
};
