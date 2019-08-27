#pragma once

class CGameButtonBD : public CGameViewButtonMan
{
public:
	CGameButtonBD();
	virtual ~CGameButtonBD();

public:

	enum _PLAY_BTN
	{
		BTNP_CHANGE,
		BTNP_PASS,
		BTN_START, //< CYS 101221 >

		BTN_AUTOUP,
		BTN_AUTOSORT,// 자동 정렬:2012.09.27 YS.Jeong 

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

	void SetEnableBtn( _PLAY_BTN ePlayBtn, BOOL bFlag ) 
	{
		if ( ePlayBtn < BTNP_CHANGE || ePlayBtn >= BTN_ALL )
		{
			return;
		}

		if ( !m_parrBtn[ ePlayBtn ] )
		{
			return;	
		}

		m_parrBtn[ ePlayBtn ]->Enable( bFlag );
	}

	NMBASE::SKINGDI::CGraphButton* GetGraphicBtn( _PLAY_BTN ePlayBtn )
	{
		if ( ePlayBtn < BTNP_CHANGE || ePlayBtn >= BTN_ALL )
		{
			return NULL;
		}

		return m_parrBtn[ ePlayBtn ];
	}

	void Init( CGameViewBase *pView, CUIInfoData *pUIInfoData, CChattingWindowMan *pChatWndMan );
	void CuttingKeyValue();

	virtual void ClearFlag();
	virtual void InitializeKeyValue( CONFIG_BETBTN_TYPE eBtnDirectionType  );
	virtual void EnterRoom_Button_Init( int BetRuleKind );

	void SetBetBtnShowAll( bool bShow ); // 모든 베팅버튼
	void SetCutBtnShowAll( bool bShow ); // 모든 커팅버튼

private:
	NMBASE::SKINGDI::CGraphButton*   m_parrBtn[ BTN_ALL ];
};