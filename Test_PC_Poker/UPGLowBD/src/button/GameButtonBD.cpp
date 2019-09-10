// GameViewButtonMan.cpp: implementation of the CGameButtonBD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameButtonBD.h"
#include "../UIInfoData/UIInfoData_BD.h"

CGameButtonBD::CGameButtonBD()
{
	for ( int i = 0; i < BTN_ALL; i++ )
	{
		m_parrBtn[ i ] = NULL;
	}

}

CGameButtonBD::~CGameButtonBD()
{
	for ( int i = 0; i < BTN_ALL; i++ )
	{
		SAFE_DELETE( m_parrBtn[ i ] );
	}
}

void CGameButtonBD::Init( CGameViewBase *pView, CUIInfoData *pUIInfoData, CChattingWindowMan *pChatWndMan )
{	
	CGameViewButtonMan::Init( pView,  pUIInfoData, pChatWndMan );

	// �ٵ��� ��Ʈ �÷�
	for ( int i = 0; i < NT_ALL; i++ )
	{
		if ( m_pNTMoney[ i ] )
		{
			m_pNTMoney[ i ]->SetOutLine( static_cast< ui::CUIInfoData_BD* >( pUIInfoData )->GetRGBBD( ui::ERGB_BETBTNTEXT_SHADOW ) );
			m_pNTMoney[ i ]->SetColor( static_cast< ui::CUIInfoData_BD* >( pUIInfoData )->GetRGBBD( ui::ERGB_BETBTNTEXT ) );
		}
	}

	for ( int i = 0; i < BTN_ALL; i++ )
	{
		SAFE_NEW( m_parrBtn[ i ], NMBASE::SKINGDI::CGraphButtonEx );
	}

	CPoint ptPos = static_cast< ui::CUIInfoData_BD* >( pUIInfoData )->GetPosBD( ui::EPOS_CHANGE );
	m_parrBtn[ BTNP_CHANGE ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &GLOBAL_SPR[ spr::GSPR_BTN_CHANGE_PASS ], 0, IDM_CHANGE_BTN );
	m_parrBtn[ BTNP_CHANGE ]->Show( FALSE );
	m_ctrlBetBtnList.Add( static_cast< NMBASE::SKINGDI::CGraphButtonEx* >( m_parrBtn[ BTNP_CHANGE ] ));

	ptPos = static_cast< ui::CUIInfoData_BD* >( pUIInfoData )->GetPosBD( ui::EPOS_PASS );
	m_parrBtn[ BTNP_PASS ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &GLOBAL_SPR[ spr::GSPR_BTN_CHANGE_PASS ], 1, IDM_PASS_BTN);
	m_parrBtn[ BTNP_PASS ]->Show( FALSE );
	m_ctrlBetBtnList.Add( static_cast< NMBASE::SKINGDI::CGraphButtonEx* >( m_parrBtn[ BTNP_PASS ] ) );	

	ptPos = static_cast< ui::CUIInfoData_BD* >( pUIInfoData )->GetPosBD( ui::EPOS_START_BTN );
	m_parrBtn[ BTN_START ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &GLOBAL_SPR[ spr::GSPR_BTN_GAMESTART ], 0, IDM_GAME_START );
	m_parrBtn[ BTN_START ]->Show( FALSE );
	m_ctrlBtnList.Add( m_parrBtn[ BTN_START ] );	

	ptPos = static_cast< ui::CUIInfoData_BD* >( pUIInfoData )->GetPosBD( ui::EPOS_AUTOUP_BTN );
	m_parrBtn[ BTN_AUTOUP ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &GLOBAL_SPR[ spr::GSPR_BTN_AUTOUP ], 0, IDM_GAME_AUTOUP, 3);
	m_ctrlBtnList.Add( m_parrBtn[ BTN_AUTOUP ] );	

	//�ڵ����� ��ư�߰�:2012.09.27 YS.Jeong 
	ptPos = static_cast< ui::CUIInfoData_BD* >( pUIInfoData )->GetPosBD( ui::EPOS_AUTOSORT_BTN );
	m_parrBtn[ BTN_AUTOSORT ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &GLOBAL_SPR[ spr::GSPR_BTN_AUTOSORT ], 0, IDM_GAME_AUTOSORT, 3);
	m_ctrlBtnList.Add( m_parrBtn[ BTN_AUTOSORT ] );	

}


void CGameButtonBD::ClearFlag()
{
	if ( GetCurrentWhere() != IDX_GAMEWHERE_GAME )
	{
		return;
	}

	CGameViewButtonMan::ClearFlag();

	m_btnDie.Show( TRUE );
	m_btnCheck.Show( TRUE );
	m_btnCall.Show( TRUE );
	m_btnPing.Show( TRUE );
#if defined(BUTTON_CHANGE_FULLTODADANG)
	if(true==GM().IsChangeFullDDang()){
		m_btnFull.Show( TRUE );
	}
	else{
		m_btnDadang.Show( TRUE );
	}
#else
	m_btnDadang.Show( TRUE );
#endif
	m_btnHalf.Show( TRUE );

	m_btnDie.Enable( FALSE );
	m_btnCheck.Enable( FALSE );
	m_btnCall.Enable( FALSE );
	m_btnPing.Enable( FALSE );
#if defined(BUTTON_CHANGE_FULLTODADANG)
	if(true==GM().IsChangeFullDDang()){
		m_btnFull.Enable( FALSE );
	}
	else{
		m_btnDadang.Enable( FALSE );
	}
#else
	m_btnDadang.Enable( FALSE );
#endif
	m_btnHalf.Enable( FALSE );

	m_parrBtn[ BTNP_CHANGE ]->Enable(FALSE);
	m_parrBtn[ BTNP_PASS ]->Enable(FALSE);

	m_parrBtn[ BTNP_CHANGE ]->Show(FALSE);
	m_parrBtn[ BTNP_PASS ]->Show(FALSE);
}

void CGameButtonBD::SetBetBtnShowAll( bool bShow )
{
	m_btnDie.Show( bShow );
	m_btnCheck.Show( bShow );
	m_btnCall.Show( bShow );
	m_btnPing.Show( bShow );
#if defined(BUTTON_CHANGE_FULLTODADANG)
	if(true==GM().IsChangeFullDDang()){
		m_btnFull.Show( bShow );
	}
	else{
		m_btnDadang.Show( bShow );
	}
#else
	m_btnDadang.Show( bShow );
#endif
	m_btnHalf.Show( bShow );
}

void CGameButtonBD::SetCutBtnShowAll( bool bShow )
{
	if(bShow){
		//������ : �ٲٱ⳪ �н��� ������ ��ư�� �ݾ������� �����Ѵ�.
		m_bBCall = false;
		m_bBCheck = false;
		m_bBDDa = false;
		m_bBMax = false;
		m_bBPing = false;
		m_bFold = false;
		SetShowBetMony();

		m_bChangePassState = true;
	}
	m_parrBtn[ BTNP_CHANGE ]->Show( bShow );
	m_parrBtn[ BTNP_PASS ]->Show( bShow );
}


// ���Ǿ��� Ű ����
void CGameButtonBD::InitializeKeyValue( CONFIG_BETBTN_TYPE eBtnDirectionType  )
{
	CGameViewButtonMan::InitializeKeyValue( eBtnDirectionType );

	//ī�弱��  Cutting
	CuttingKeyValue();
}



void CGameButtonBD::CuttingKeyValue()
{
	//ī�弱��  Cutting
	if ( g_Config.bCutStyle )
	{
		// F1 ~ F4
		G_KEYDEF[ KEY_CUT_1 ][ 0 ] = SET_VKEY_NOR( rule::SKEY_F1 );
		G_KEYDEF[ KEY_CUT_2 ][ 0 ] = SET_VKEY_NOR( rule::SKEY_F2 );
		G_KEYDEF[ KEY_CUT_3 ][ 0 ] = SET_VKEY_NOR( rule::SKEY_F3 );
		G_KEYDEF[ KEY_CUT_4 ][ 0 ] = SET_VKEY_NOR( rule::SKEY_F4 );

		G_KEYDEF[ KEY_CUT_1 ][ 1 ] = SET_VKEY_EXT( 0 );
		G_KEYDEF[ KEY_CUT_1 ][ 2 ] = SET_VKEY_NOR( 0 );   //VK_NUMPAD1

		G_KEYDEF[ KEY_CUT_2 ][ 1 ] = SET_VKEY_EXT( 0 );
		G_KEYDEF[ KEY_CUT_2 ][ 2 ] = SET_VKEY_NOR( 0 );  //VK_NUMPAD2

		G_KEYDEF[ KEY_CUT_3 ][ 1 ] = SET_VKEY_EXT( 0 );
		G_KEYDEF[ KEY_CUT_3 ][ 2 ] = SET_VKEY_NOR( 0 );  //VK_NUMPAD3 

		G_KEYDEF[ KEY_CUT_4 ][ 1 ] = SET_VKEY_EXT( 0 );
		G_KEYDEF[ KEY_CUT_4 ][ 2 ] = SET_VKEY_NOR( 0 );  //VK_NUMPAD4
	}
	else
	{
		// 1 ~ 4
		G_KEYDEF[ KEY_CUT_1 ][ 0 ] = SET_VKEY_NOR( rule::SKEY_1 );
		G_KEYDEF[ KEY_CUT_2 ][ 0 ] = SET_VKEY_NOR( rule::SKEY_2 );
		G_KEYDEF[ KEY_CUT_3 ][ 0 ] = SET_VKEY_NOR( rule::SKEY_3 );
		G_KEYDEF[ KEY_CUT_4 ][ 0 ] = SET_VKEY_NOR( rule::SKEY_4 );


		G_KEYDEF[ KEY_CUT_1 ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_DEL );
		G_KEYDEF[ KEY_CUT_1 ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_END );   //VK_NUMPAD1

		G_KEYDEF[ KEY_CUT_2 ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_END );
		G_KEYDEF[ KEY_CUT_2 ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_DOWN );  //VK_NUMPAD2

		G_KEYDEF[ KEY_CUT_3 ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_PGDN );
		G_KEYDEF[ KEY_CUT_3 ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_PGDN );  //VK_NUMPAD3 

		G_KEYDEF[ KEY_CUT_4 ][ 1 ] = SET_VKEY_EXT( rule::SKEY_GRAY_INS );
		G_KEYDEF[ KEY_CUT_4 ][ 2 ] = SET_VKEY_NOR( rule::SKEY_GRAY_LEFT );  //VK_NUMPAD4

	}

	// �ٲٱ�, �н� ����Ű
	G_KEYDEF[ KEY_CUT_CHANGE ][ 0 ] = SET_VKEY_EXT( rule::SKEY_GRAY_HOME ); 
	G_KEYDEF[ KEY_CUT_CHANGE ][ 1 ] = SET_VKEY_NOR( rule::SKEY_GRAY_CENTER );//VK_NUMPAD5
	G_KEYDEF[ KEY_CUT_CHANGE ][ 2 ] = SET_VKEY_NOR( rule::SKEY_5 );          //5

	G_KEYDEF[ KEY_CUT_PASS ][ 0 ] = SET_VKEY_EXT( rule::SKEY_GRAY_PGUP );
	G_KEYDEF[ KEY_CUT_PASS ][ 1 ] = SET_VKEY_NOR( rule::SKEY_GRAY_RIGHT ); //VK_NUMPAD6;
	G_KEYDEF[ KEY_CUT_PASS ][ 2 ] = SET_VKEY_NOR( rule::SKEY_6 ); //6;
}


void CGameButtonBD::EnterRoom_Button_Init( int BetRuleKind )
{	
	CGameViewButtonMan::EnterRoom_Button_Init( BetRuleKind );

	// ������� ��ư ��
	if ( m_parrBtn[ BTN_AUTOUP ] )
	{
		// ������ ��ư �����
		if ( g_ObserverMan.IsMyObserver() )
		{
			m_parrBtn[ BTN_AUTOUP ]->Show( false );
		}
		else
		{
			m_parrBtn[ BTN_AUTOUP ]->Show( true );
		}

		if ( g_Config.nAutoUpType == 2 ) // ó�� �α��νù�ư Ŭ������ ��������  0 : ������ 1: ��� 2 : ���� ��
		{
			bool bDefaultValue = true;

			if ( IsVIPRoom(&g_RI) ) 
			{
				bDefaultValue = false; // vip �ڵ��ø� ����
			}

			m_parrBtn[ BTN_AUTOUP ]->SetSprite( &GLOBAL_SPR[ spr::GSPR_BTN_AUTOUP ], bDefaultValue ? 0 : 1 ); // 0 ī�弱��ON, 1 ī�弱��OFF

			//�������� �ڵ� ���� Ȱ��ȭ 2012.11.02 YS.Jeong
			if( g_RI.llEnterLimitMoney <= 100000000000 ){
				g_Config.nAutoUpType = 1;
			}else{
				g_Config.nAutoUpType = 0;
			}


			
		}else{
			m_parrBtn[ BTN_AUTOUP ]->SetSprite( &GLOBAL_SPR[ spr::GSPR_BTN_AUTOUP ], g_Config.nAutoUpType == 1 ? 0 : 1 ); // 0 ī�弱��ON, 1 ī�弱��OFF
		}

		
	}


	// ������� ��ư ��
	//�ڵ����� ��ư�߰�:2012.09.27 YS.Jeong 
	if ( m_parrBtn[ BTN_AUTOSORT ] )
	{
		// ������ ��ư �����
		if ( g_ObserverMan.IsMyObserver() )
		{
			m_parrBtn[ BTN_AUTOSORT ]->Show( false );
		}
		else
		{
			m_parrBtn[ BTN_AUTOSORT ]->Show( true );
		}

		if ( g_Config.nAutoSortType == 2 ) // ó�� �α��νù�ư Ŭ������ ��������  0 : ������ 1: ��� 2 : ���� ��
		{
			bool bDefaultValue = true;
			m_parrBtn[ BTN_AUTOSORT ]->SetSprite( &GLOBAL_SPR[ spr::GSPR_BTN_AUTOSORT ], bDefaultValue ? 0 : 1 ); // 0 ī������ON, 1 ī������OFF
			g_Config.nAutoSortType = 1;
		}else{
			m_parrBtn[ BTN_AUTOSORT ]->SetSprite( &GLOBAL_SPR[ spr::GSPR_BTN_AUTOSORT ], g_Config.nAutoSortType == 1 ? 0 : 1 ); // 0 ī������ON, 1 ī������OFF
		}

	}
	
}