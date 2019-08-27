#pragma once

#include "PromotionBase.h"
#include <EventCommon.h>


#define MAX_ROUND_CNT	5

//ÀÚ¸® ÁöÅ´ÀÌ
#define IDM_BTN_CLOSE	(WM_USER+6502)
#define IDM_BTN_OPEN	(WM_USER+6503)



class UPG_GAMEBASE_EXPORT CPromotionLotteryEvent : public CPromotionBase
{
public:
	CPromotionLotteryEvent( CPromotionManager *pManager, int nType );
	virtual ~CPromotionLotteryEvent(void);

protected:

	NMBASE::GRAPHICGDI::xSprite	m_sprBack;					//¹è°æ

	promotion::BETTING_ALRAM m_PcRoomMact_Info;

	BOOL m_bDomantUser;					//½Å±ÔÀ¯Àú ÀÀ¸ð±Ç È¹µæ
	
	int m_nIndex_Coupon;
	int m_nIndex_CouponTab;

	
	BOOL m_bShowEft;					//ÀÀ¸ð±ÇÈ¹µæ ¹× ¶ó¿îµåÁõ°¡
	DWORD m_dwShowEftTimer;

	BOOL m_bGetCoupon;					//ÄíÆùÈ¹µæ
	DWORD m_dwGetCouponTimer;


	NMBASE::SKINGDI::CMyBitmap m_bitEndBanner;
	NMBASE::SKINGDI::CMyButton m_BtnEndBanner;
public:
	
	void Clear_Info();
	void ShowComplete( int nGetCoupon = 1 );						//ÀÀ¸ð±ÏÈ¹µæ½Ã	
 	void SetEventInfo( promotion::BETTING_ALRAM *pInfo );	//ÆÇ ÁøÇà½Ã ¸¶´Ù
	

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
