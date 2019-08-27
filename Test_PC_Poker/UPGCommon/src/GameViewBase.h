#pragma once

///#include <UPGPlatform.h>
// CGameViewBase

class CGame;

class UPG_COMMON_EXPORT CGameViewBase : public CWnd
{
	DECLARE_DYNAMIC(CGameViewBase)
public:
	NMBASE::GRAPHICGDI::CPage			Page;
	CGame			*m_pGame;

	// ### [ Frame++ ] ###
	MMRESULT		m_hEventTimer;
	// 동기화	
	NMBASE::UTIL::CSyncFPS		m_syncfps;
	int				m_ReFresh_FPS;
	CDC*			m_pDC;
	BOOL			m_bExitSubscript;	//나가기 예약 상태
public:
	CGameViewBase(CGame *pGame);
	virtual ~CGameViewBase();

	virtual const int GetWidth() = 0;
	virtual const int GetHeight() = 0;

	void MessagePump();
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void	OnTimer(UINT_PTR nIDEvent);
	afx_msg int		OnCreate(LPCREATESTRUCT lpCreateStruct);
	inline	CGame	*GetGame(void){return m_pGame;}

	virtual void StartTimer(void);
	virtual void StopTimer(void);
public:
	//각 게임의 효과나 View 관련 기능ㄹ
	virtual void ShowEffect_AllowChancePurchase(int nChanceItemNo)=0;	// 찬스아이템 획득시 효과를 보여준다.
};


