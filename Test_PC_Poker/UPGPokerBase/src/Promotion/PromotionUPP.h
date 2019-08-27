#pragma once


#include "PromotionBase.h"
#include <EventCommon.h>

class UPG_GAMEBASE_EXPORT CPromotionUPP : public CPromotionBase
{
enum eExit_Type
{
	eExit_None = 0,
	eExit_Timer = 1,	//시간 알림이에 의한 종료
	eExit_Force = 2,	//판수 소진에 의한 종료
};

public:
	CPromotionUPP( CPromotionManager *pManager, int nType );
	virtual ~CPromotionUPP(void);

protected:
	CRect m_rtRemainTime;
	CRect m_rtRemainPan;

	int m_nLevel ; // UPP 레벨 
	int m_bAgree ; // UPP 정책에 agree 했는지 여부 
	int m_nWarning; // 경고 창 간격 ( 초단위 ) 
	int m_nExit   ; // 종료 예약 ( 초단위 )

	int m_nRemainTimeSec;	//잔여시간
	int m_nRemainRound;		//잔여판수

	eExit_Type m_eExitType;

	BOOL m_bForceExitSubscript;

	NMBASE::GRAPHICGDI::xSprite	m_sprBack;					//배경
public:
	//로그인시 들어오는 UPP데이터를 저장
	void SetUPPData(CSV_UPPINFO* msg);
	BOOL CheckUPPAgreement();
	void ShowUPPInfoDlg();

	virtual const bool IsShowTab(void) { return false; }

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
	
	LRESULT  OnShowEndingPopup(WPARAM &wParam, LPARAM &lParam);

	LRESULT OnIsActive(WPARAM &wParam, LPARAM &lParam);
	virtual BOOL OnCommand(WPARAM &wParam, LPARAM &lParam);

	void Test();
};
