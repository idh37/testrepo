#pragma once

#define IDM_OBSERVERUI (WM_USER+6300)
#define IDM_OBSERVERINFO (WM_USER+6301)

class CUIInfoData;
class CGameViewButtonMan;

class UPG_GAMEBASE_EXPORT CObserverMan  
{
	int				m_nMyReservePriority;	//나의 예약 우선순위?
	int				m_nTotalReserve;		//참여대기 숫자 

	CGameViewBase		*m_pGameView;
	NMBASE::GRAPHICGDI::CPage				*m_pPage;
	CUIInfoData			*m_pUIInfoData;
	CGameViewButtonMan	*m_pButtonMan;

	NMBASE::GRAPHICGDI::xSprite			m_sprBack;
	NMBASE::GRAPHICGDI::xSprite			m_sprReservationBtn;
	NMBASE::GRAPHICGDI::xSprite			m_sprCancelBtn;
	NMBASE::GRAPHICGDI::xSprite			m_sprNumber;

	NMBASE::SKINGDI::CGraphButton	m_btnReserve;
public:
	CObserverMan();
	virtual ~CObserverMan();	

	virtual void LoadImage();
	virtual void Init(CGameViewBase *pView, CUIInfoData *pUIInfoData, CGameViewButtonMan *pButtonMan);	

	virtual void Participation();		//관전->참여
	virtual void Emerge();				//참여->관전
	virtual void Draw(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC);
	virtual void OnReceiveReservationInfo(CSV_RESERVATIONINFO *pMsg);
	virtual BOOL OnCommand( WPARAM wParam, LPARAM lParam );

	virtual BOOL OnLButtonDown(int x, int y);
	virtual BOOL OnLButtonUp(int x, int y);
	virtual BOOL OnMouseMove(int x, int y);

	virtual void SetMyObserver(BOOL bMyObserver);
	virtual BOOL IsMyObserver(void);

	virtual void OnObserve();	//관전 입장시 
	virtual void OnEnterRoom();
};