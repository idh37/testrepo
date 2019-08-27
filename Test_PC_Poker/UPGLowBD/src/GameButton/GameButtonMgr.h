#pragma once

#include <GameButtonMan/GameViewButtonMan.h>



class CGameButtonMgr : public CGameViewButtonMan
{
public:
	CGameButtonMgr();
	virtual ~CGameButtonMgr();


public:

	enum _PLAY_BTN
	{
		BTNP_CHANGE,
		BTNP_PASS,

		BTN_ALL
	};

	BOOL IsShowBtn( _PLAY_BTN ePlayBtn ) 
	{
		if ( ePlayBtn < BTNP_CHANGE || ePlayBtn >= BTN_ALL )
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
		if ( ePlayBtn < BTNP_CHANGE || ePlayBtn >= BTN_ALL )
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
		if ( ePlayBtn < BTNP_CHANGE || ePlayBtn >= BTN_ALL )
		{
			return FALSE;
		}

		if ( !m_parrBtn[ ePlayBtn ] )
		{
			return FALSE;	
		}

		return m_parrBtn[ ePlayBtn ]->IsEnable();
	}

	NMBASE::SKINGDI::CGraphButtonEx* GetGraphicBtn( _PLAY_BTN ePlayBtn )
	{
		if ( ePlayBtn < BTNP_CHANGE || ePlayBtn >= BTN_ALL )
		{
			return NULL;
		}

		return m_parrBtn[ ePlayBtn ];
	}

	void Init( CGameViewBase *pView, CUIInfoData *pUIInfoData );
	void ShowChangeBtn_BD(BOOL bshow = FALSE);	
	virtual void ClearFlag();
	void SetBtnDirect();

private:
	NMBASE::SKINGDI::CGraphButtonEx*   m_parrBtn[ BTN_ALL ];
};