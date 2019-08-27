// GameViewButtonMan.cpp: implementation of the CGameButtonMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameButtonMgr.h"
#include "../UIInfoData/UIInfoData_BD.h"

CGameButtonMgr::CGameButtonMgr()
{
	//< CYS 100705
	// 베팅버튼
	for ( int i = 0; i < BTN_ALL; i++ )
	{
		m_parrBtn[ i ] = NULL;
	}
	
}

CGameButtonMgr::~CGameButtonMgr()
{
	
	for ( int i = 0; i < BTN_ALL; i++ )
	{
		if ( m_parrBtn [ i ] )
		{
			SAFE_DELETE( m_parrBtn[ i ] );
		}
		m_parrBtn[ i ] = NULL;
	}
}

void CGameButtonMgr::Init( CGameViewBase *pView, CUIInfoData *pUIInfoData )
{	
	CGameViewButtonMan::Init( pView,  pUIInfoData );

	for ( int i = 0; i < BTN_ALL; i++ )
	{
		SAFE_NEW( m_parrBtn[ i ], NMBASE::SKINGDI::CGraphButtonEx );
	}

	CPoint ptPos = static_cast< ui::CUIInfoData_BD* >( pUIInfoData )->GetPosBD( ui::EPOS_CHANGE );
	m_parrBtn[ BTNP_CHANGE ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &GLOBAL_SPR[ spr::GSPR_BTN_CHANGE ], 0, IDM_CHANGE_BTN );
	m_parrBtn[ BTNP_CHANGE ]->Show( FALSE );
	m_ctrlBetBtnList.Add( m_parrBtn[ BTNP_CHANGE ] );

	ptPos = static_cast< ui::CUIInfoData_BD* >( pUIInfoData )->GetPosBD( ui::EPOS_PASS );
	m_parrBtn[ BTNP_PASS ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &GLOBAL_SPR[ spr::GSPR_BTN_CHANGE ], 1, IDM_PASS_BTN);
	m_parrBtn[ BTNP_PASS ]->Show( FALSE );
	m_ctrlBetBtnList.Add( m_parrBtn[ BTNP_PASS ] );	

}


void CGameButtonMgr::ClearFlag()
{
	if ( GetCurrentWhere() != IDX_GAMEWHERE_GAME )
	{
		return;
	}
	
	//GAME()->ClearBetFlg();
	CGameViewButtonMan::ClearFlag();
	
	m_btnDie.Show( TRUE );
	m_btnCheck.Show( TRUE );
	m_btnCall.Show( TRUE );
	m_btnPing.Show( TRUE );
	m_btnDadang.Show( TRUE );
	m_btnHalf.Show( TRUE );

	m_btnDie.Enable( FALSE );
	m_btnCheck.Enable( FALSE );
	m_btnCall.Enable( FALSE );
	m_btnPing.Enable( FALSE );
	m_btnDadang.Enable( FALSE );
	m_btnHalf.Enable( FALSE );


	m_parrBtn[ BTNP_CHANGE ]->Enable(FALSE);
	m_parrBtn[ BTNP_PASS ]->Enable(FALSE);

	m_parrBtn[ BTNP_CHANGE ]->Show(FALSE);
	m_parrBtn[ BTNP_PASS ]->Show(FALSE);
	

}

void CGameButtonMgr::ShowChangeBtn_BD( BOOL bshow )
{
	if ( GetCurrentWhere() != IDX_GAMEWHERE_GAME )
	{
		return;	
	}

	if ( g_RI.Game != IDX_GAME_BD )
	{
		return;
	}

	if( bshow == TRUE )
	{
		if( !g_bMyObserver && GLOBAL_PLAYER(0)->JoinState == 1 && GLOBAL_PLAYER(0)->PlayState == 1 && !GLOBAL_PLAYER(0)->bFold )
		{
			// 모든 베팅 버튼숨기기
			m_btnDie.Show( FALSE );
			m_btnCheck.Show( FALSE );
			m_btnCall.Show( FALSE );
			m_btnPing.Show( FALSE );
			m_btnDadang.Show( FALSE );
			m_btnHalf.Show( FALSE );
			

			CPlayer_BD* pPlayer =  ( CPlayer_BD* )GLOBAL_PLAYER( 0 );
			if ( pPlayer != NULL )
			{
				m_parrBtn[ BTNP_CHANGE ]->Show( TRUE );
				m_parrBtn[ BTNP_PASS ]->Show( TRUE );

				//바꿀 카드를 미리 몇장이나 선택해 뒀는지
				int cnt = pPlayer->IsCutCardSelected();
				//한장 이상이다.
				if ( cnt > 0 )
				{					
					GAME()->m_bBChange = TRUE;
					GAME()->m_bBPass = FALSE;
					m_parrBtn[ BTNP_CHANGE ]->Enable(TRUE);
					m_parrBtn[ BTNP_PASS ]->Enable(FALSE);
				}
				//미리 선택 해두지 않음
				else
				{
					GAME()->m_bBChange = FALSE;
					GAME()->m_bBPass = TRUE;
					m_parrBtn[ BTNP_CHANGE ]->Enable(FALSE);
					m_parrBtn[ BTNP_PASS ]->Enable(TRUE);
				}
			}
		}
	}
	else
	{
		m_parrBtn[ BTNP_CHANGE ]->Show( FALSE );
		m_parrBtn[ BTNP_PASS ]->Show( FALSE );
		m_parrBtn[ BTNP_CHANGE ]->Enable( FALSE );
		m_parrBtn[ BTNP_PASS ]->Enable( FALSE );
		GAME()->m_bBChange = FALSE;
		GAME()->m_bBPass = FALSE;
	}
}

void CGameButtonMgr::SetBtnDirect()
{
	if( g_Config.bBetBtnType == enum_BTN_V )
	{
		g_Config.bBetBtnType = enum_BTN_H;
	}
	else
	{
		g_Config.bBetBtnType = enum_BTN_V;
	}

	//SetKeyDirect( g_Config.bBetBtnType );
	GAME_BD()->SetKeyDirect( g_Config.bBetBtnType );

	set_BetBtnState(g_Config.bBetBtnType);

	g_Config.bBetBtnType = g_Config.bBetBtnType;
	NMBASE::UTIL::WriteProfile_int(GM().GetMyInfo()->UI.ID, "BetBtnType", g_Config.bBetBtnType);
}