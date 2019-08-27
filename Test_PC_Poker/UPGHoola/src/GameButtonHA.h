#pragma once
#include "MyGraphButton.h"

//임시 단축키 지정(하이로우 단축키)
// 게임 진행 관련
#define IDM_HOOLA_STOP				(WM_USER+7100)
#define IDM_HOOLA_REGIST			(WM_USER+7101)
#define IDM_HOOLA_THANK				(WM_USER+7102)
#define IDM_HOOLA_PASS				(WM_USER+7103)
// [훌라 개선 작업] 2003.11.19
#define IDM_HOOLA_GETCARD			(WM_USER+7104)
#define IDM_HOOLA_DUMPCARD			(WM_USER+7105)
#define IDM_HOOLA_SORT_STRAIGHT		(WM_USER+7106)
#define IDM_HOOLA_LARROW			(WM_USER+7107)
#define IDM_HOOLA_RARROW			(WM_USER+7108)
#define IDM_HOOLA_SELECTOK			(WM_USER+7109)
#define IDM_HOOLA_RESULTOK			(WM_USER+7110)
#define IDM_HOOLA_SHOWHELP			(WM_USER+7111)
#define IDM_HOOLA_STOPHELP			(WM_USER+7112)
// ### [ 관전기능 ] ###
#define IDM_HOOLA_PARTICIPATION 	(WM_USER+7116)
#define IDM_HOOLA_EMERGE 			(WM_USER+7117)
#define IDM_HOOLA_OBSERVERVIEW		(WM_USER+7118)
#define IDM_HOOLA_LOWTHANK			(WM_USER+7119)

// ### [ 추가  ] ###
#define IDM_HOOLA_GAMESTART 			(WM_USER+7120)
#define IDM_HOOLA_PRACTICS 				(WM_USER+7121)
#define IDM_HOOLA_HIGHTHANK				(WM_USER+7122)
#define IDM_HOOLA_SEVENFOURCARDTHANK	(WM_USER+7224)
#define IDM_HOOLA_SORT_TOGETHER			(WM_USER+7225)
#define IDM_HOOLA_THANKSELECT			(WM_USER+7226)




class CGameButtonHA : public CGameViewButtonMan
{
public:
	INT64 m_i64GiveUpMoney;
	CMsgBoxDlg* m_dlgGiveUp;
	
public:
	enum _PLAY_BTN
	{
		BTN_NULL = -1,
		BTN_STOP,
		BTN_REGIST,
		BTN_PASS,
		BTN_GETCARD,
		BTN_DUMPCARD,
		BTN_SORTCARD_STRAIGHT,
		BTN_SORTCARD_TOGETHER,
		BTN_RESULTOK,
		BTN_THANK,
		BTN_LOWTHANK,
		BTN_SEVENFOURTHANK,
		BTN_HIGHTHANK,
		BTN_THANKSELECT,
		BTN_GAMESTART, 
		BTN_PRACTICS,
		BTN_ALL
	};
	CGameButtonHA();
	~CGameButtonHA();

	void LoadImage(LPCTSTR strBetBtn_N, LPCTSTR strBetBtn_H, LPCTSTR strBetBtn_P);

	BOOL IsShowBtn( _PLAY_BTN ePlayBtn ) 
	{
		if ( ePlayBtn < BTN_NULL || ePlayBtn >= BTN_ALL )
		{
			return FALSE;
		}

		if ( !m_parrBtn[ ePlayBtn ] )
		{
			return FALSE;	
		}

		return m_parrBtn[ ePlayBtn ]->IsShow();
	}

	void SetShowBtn( _PLAY_BTN ePlayBtn, BOOL bFlag ) 
	{
		if ( ePlayBtn < BTN_NULL || ePlayBtn >= BTN_ALL )
		{
			return;
		}

		if ( !m_parrBtn[ ePlayBtn ] )
		{
			return;	
		}

		m_parrBtn[ ePlayBtn ]->Show( bFlag );
	}

	BOOL IsEnableBtn( _PLAY_BTN ePlayBtn ) 
	{
		if ( ePlayBtn < BTN_NULL || ePlayBtn >= BTN_ALL )
		{
			return FALSE;
		}

		if ( !m_parrBtn[ ePlayBtn ] )
		{
			return FALSE;	
		}

		return m_parrBtn[ ePlayBtn ]->IsEnable();
	}

	void SetEnableBtn( _PLAY_BTN ePlayBtn, BOOL bFlag ) 
	{
		if ( ePlayBtn < BTN_NULL || ePlayBtn >= BTN_ALL )
		{
			return;
		}

		if ( !m_parrBtn[ ePlayBtn ] )
		{
			return;	
		}

		m_parrBtn[ ePlayBtn ]->Enable( bFlag );
	}

	void SetGiveUpMoney(INT64 i64GiveUpMoney);
	void CloseGiveUpDlg();

	NMBASE::SKINGDI::CGraphButton* GetGraphicBtn( _PLAY_BTN ePlayBtn )
	{
		if ( ePlayBtn < BTN_NULL || ePlayBtn >= BTN_ALL )
		{
			return NULL;
		}

		return m_parrBtn[ ePlayBtn ];
	}

	void Init( CGameViewBase *pView, CUIInfoData *pUIInfoData, CChattingWindowMan *pChatWndMan );

	virtual void ClearFlag();
	virtual void InitializeKeyValue( CONFIG_BETBTN_TYPE eBtnDirectionType );	
	virtual void DrawBtn(CDC *pDC);


	BOOL OnLButtonDown(int x , int y);
	BOOL OnLButtonUp(int x , int y);
	BOOL OnMouseMove(int x , int y);
	virtual BOOL OnCommand( WPARAM wParam, LPARAM lParam );

	void ResetGameButton();

	BOOL IsSortBtnEnableState();

protected:
	
	NMBASE::SKINGDI::CGraphButtonList m_ctrlHoolaBtnList;
	NMBASE::SKINGDI::CGraphButton*   m_parrBtn[ BTN_ALL ];
public:

	//스프라이트
	NMBASE::GRAPHICGDI::xSprite m_btnSprResultOK;
	NMBASE::GRAPHICGDI::xSprite m_btnSprGameStart;
	NMBASE::GRAPHICGDI::xSprite m_btnSprPractics;

	//리뉴얼 모든 버튼들
	NMBASE::GRAPHICGDI::xSprite m_btnSprAll;
};