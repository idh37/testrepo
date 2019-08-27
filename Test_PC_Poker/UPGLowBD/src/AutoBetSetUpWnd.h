// AutoBetSetUpWnd.h: interface for the CAutoBetSetUpWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUTOBETSETUPWND_H__1FB08E7B_E2F5_4481_AC95_17621CE48FE6__INCLUDED_)
#define AFX_AUTOBETSETUPWND_H__1FB08E7B_E2F5_4481_AC95_17621CE48FE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAutoBetSetUpWnd  
{
public:
	CAutoBetSetUpWnd();
	virtual ~CAutoBetSetUpWnd();
	
	NMBASE::GRAPHICGDI::CPage			*m_pPage;

	BOOL			m_bShow;
	NMBASE::SKINGDI::CGraphButtonEx	m_AutobetBtn[3]; // [자동베팅] 0:AUTO 1:OFF 2:49 3:59 4:69
	NMBASE::SKINGDI::CGraphButtonEx	m_AutoSetupBtn;
	
	// [자동베팅] 2004.07.15
	int m_IsAutoKindBet; // 0:X  1:49 2:59 3:69
	BOOL m_bAutoBet;
	int  m_nAutoBetCnt;	
	int  nAutoBet;	//설정된 값이다.

	void Initialize();
	BOOL IsShow(){
		return m_bShow;
	}
	BOOL IsChan_Valid();
	BOOL IsObserver_Valid();
	void Set_auto_betting_flag();
	inline int GetIsAKindBet() { return m_IsAutoKindBet; }
	void SetIsAKindBet(int value);
	void set_auto_betting_btn(BOOL bClear = FALSE);	
	void auto_betting_func();
	void auto_betting_call();

	void Draw(CDC *pDC);
	void Draw_Top(CDC *pDC);
	bool OnCommand( WPARAM wParam ); //< CYS 100705 > 수정
	void OnLButtonDown(int x, int y);
	void OnLButtonUp(int x, int y);
	void OnMouseMove(int x, int y);
	void OnTimer();

	void SetShowWnd(BOOL bshow);
	void set_GameMode();
	void OnEnterRoom();
	
	//static CAutoBetSetUpWnd &GetInstance();
};

extern CAutoBetSetUpWnd* GetAutoBetWnd();
extern void DeleteAutoBetWnd();

#define LAYER_GLOBAL_AUTOBETWND()		 GetAutoBetWnd()
#define LAYER_GLOBAL_AUTOBETWND_DELETE() DeleteAutoBetWnd()


//CAutoBetSetUpWnd& GLOBAL_AutoBetSetUpWnd();

#endif // !defined(AFX_AUTOBETSETUPWND_H__1FB08E7B_E2F5_4481_AC95_17621CE48FE6__INCLUDED_)
