#pragma once

#include "PromotionBase.h"
#include <EventCommon.h>


#define MAX_ROUND_CNT	5

//�ڸ� ��Ŵ��
#define IDM_BTN_CLOSE	(WM_USER+6502)
#define IDM_BTN_OPEN	(WM_USER+6503)



class UPG_GAMEBASE_EXPORT CPromotionLotteryEvent : public CPromotionBase
{
public:
	CPromotionLotteryEvent( CPromotionManager *pManager, int nType );
	virtual ~CPromotionLotteryEvent(void);

protected:

	NMBASE::GRAPHICGDI::xSprite	m_sprBack;					//���

	promotion::BETTING_ALRAM m_PcRoomMact_Info;

	BOOL m_bDomantUser;					//�ű����� ����� ȹ��
	
	int m_nIndex_Coupon;
	int m_nIndex_CouponTab;

	
	BOOL m_bShowEft;					//�����ȹ�� �� ��������
	DWORD m_dwShowEftTimer;

	BOOL m_bGetCoupon;					//����ȹ��
	DWORD m_dwGetCouponTimer;


	NMBASE::SKINGDI::CMyBitmap m_bitEndBanner;
	NMBASE::SKINGDI::CMyButton m_BtnEndBanner;
public:
	
	void Clear_Info();
	void ShowComplete( int nGetCoupon = 1 );						//�����ȹ���	
 	void SetEventInfo( promotion::BETTING_ALRAM *pInfo );	//�� ����� ����
	

	virtual void SetActive(const bool &bActive);
	virtual void DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	
	virtual void Accept_OutRoom();
	virtual void Accept_CreateRoom();
	virtual void Accept_EnterRoom();

	virtual BOOL OnLButtonDownMiddle(int &x , int &y);
	virtual BOOL OnLButtonUpMiddle(int &x , int &y);
	virtual BOOL OnMouseMoveMiddle(int &x , int &y);

	virtual void DrawEventTab(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage, const CPoint &ptStart);
	virtual BOOL OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData);



	virtual bool IsEndPopup(void);
	virtual bool InitEndPopup(CEndPopup *pEndPopup, CPoint ptPos);
	virtual bool OnPaintEndPopup(CPaintDC *pDC);
	virtual bool OnClickEndPopup(UINT nID);
	virtual void OnDestroyEndPopup(void);


	LRESULT OnIsActive(WPARAM &wParam, LPARAM &lParam);
};
