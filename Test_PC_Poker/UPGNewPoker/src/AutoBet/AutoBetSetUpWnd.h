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
	
	NMBASE::GRAPHICGDI::CPage				*m_pPage;

	BOOL				m_bShow;
	// [자동베팅] 2004.07.15
	int					m_IsAutoKindBet; // 0:X  1:49 2:59 3:69
	BOOL				m_bAutoBet;
	int					m_nAutoBetCnt;	

	void Init();
	BOOL IsValid(){
		return m_bShow;
	}
	void Set_auto_betting_flag();
	inline int GetIsAKindBet() { return m_IsAutoKindBet; }
	void SetIsAKindBet(int value);
	void auto_betting_func();
	void auto_betting_call();
	void  ReSetAutoBettingButton(int num);

	void Draw_Top(CDC *pDC);
	bool OnCommand( WPARAM wParam );
	void OnTimer();

	void SetShowWnd(BOOL bshow);
	void OnEnterRoom();
	
	static CAutoBetSetUpWnd &GetInstance();
};

CAutoBetSetUpWnd & AutoBetSetUpWnd();

#endif // !defined(AFX_AUTOBETSETUPWND_H__1FB08E7B_E2F5_4481_AC95_17621CE48FE6__INCLUDED_)
