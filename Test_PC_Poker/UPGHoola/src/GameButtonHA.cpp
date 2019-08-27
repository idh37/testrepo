// GameViewButtonMan.cpp: implementation of the CGameButtonHA class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameButtonHA.h"
#include "UIInfoDataHoola.h"
#include "SpriteList.h"
#include "GlobalGame.h"
#include "CommMsgDef_Game.h"

CGameButtonHA::CGameButtonHA()
{
	m_bInculdeBetttingBtn = false; //기존 포커타입의 베팅버튼을 사용하지 않는다.
	
	for ( int i = BTN_NULL + 1; i < BTN_ALL; i++ )
	{
		m_parrBtn[ i ] = NULL;
	}
	
	m_i64GiveUpMoney = 0;
	m_dlgGiveUp = NULL;
	
}

CGameButtonHA::~CGameButtonHA()
{
	for ( int i = BTN_NULL + 1; i < BTN_ALL; i++ )
	{
		SAFE_DELETE( m_parrBtn[ i ] );
	}
}

void CGameButtonHA::LoadImage(LPCTSTR strBetBtn_N, LPCTSTR strBetBtn_H, LPCTSTR strBetBtn_P)
{
	CGameViewButtonMan::LoadImage(strBetBtn_N, strBetBtn_H, strBetBtn_P);
	AddLoadImageList(&m_btnSprResultOK,".\\hoola\\data\\hoola\\ResultOkBtn.spr",true);
	AddLoadImageList(&m_btnSprGameStart,".\\hoola\\data\\hoola\\GameStartBtn.spr",true);
	AddLoadImageList(&m_btnSprPractics,".\\hoola\\data\\hoola\\PracticsBtn.spr",true);
	AddLoadImageList(&m_btnSprAll,".\\hoola\\data\\hoola\\AllButton.spr",true);
}

void CGameButtonHA::Init( CGameViewBase *pView, CUIInfoData *pUIInfoData, CChattingWindowMan *pChatWndMan )
{
	CGameViewButtonMan::Init( pView,  pUIInfoData, pChatWndMan );
	
	for ( int i = BTN_NULL + 1; i < BTN_ALL; i++ )
	{
		switch (i)
		{
		case BTN_STOP :
		case BTN_REGIST :
		case BTN_PASS :
		case BTN_GETCARD :
		case BTN_DUMPCARD :
		case BTN_SORTCARD_STRAIGHT :
		case BTN_SORTCARD_TOGETHER :
		case BTN_RESULTOK :
		case BTN_THANK :
		case BTN_LOWTHANK :
		case BTN_HIGHTHANK :
		case BTN_SEVENFOURTHANK :
		case BTN_THANKSELECT:
			SAFE_NEW( m_parrBtn[ i ], NMBASE::SKINGDI::CGraphButton );
			break;
		case BTN_GAMESTART :
		case BTN_PRACTICS :
			SAFE_NEW( m_parrBtn[ i ], CMyGraphButton );
			break;
		}
	}	

	CUIInfoDataHoola *pHoolaUIData = dynamic_cast<CUIInfoDataHoola*>(m_pUIInfoData);
	ASSERT(pHoolaUIData);

	if (pHoolaUIData)
	{
		// 게임진행 버튼 클래스
		CPoint ptPos;

		ptPos = pHoolaUIData->GetHoolaBtnPos(EBT_HOOL_STOP);
		m_parrBtn[ BTN_STOP ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &m_btnSprAll, 40/4, IDM_HOOLA_STOP );
		m_ctrlHoolaBtnList.Add(m_parrBtn[ BTN_STOP ]);

		ptPos = pHoolaUIData->GetHoolaBtnPos(EBT_HOOL_REGIST);
		m_parrBtn[ BTN_REGIST ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &m_btnSprAll, 12/4, IDM_HOOLA_REGIST );
		m_ctrlHoolaBtnList.Add(m_parrBtn[ BTN_REGIST ]);

		ptPos = pHoolaUIData->GetHoolaBtnPos(EBT_HOOL_SEVENFOURCARDTHANK);
		m_parrBtn[ BTN_SEVENFOURTHANK ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &m_btnSprAll, 0, IDM_HOOLA_SEVENFOURCARDTHANK );
		m_parrBtn[ BTN_SEVENFOURTHANK ]->Show( FALSE );
		m_ctrlHoolaBtnList.Add(m_parrBtn[ BTN_SEVENFOURTHANK ]);

		ptPos = pHoolaUIData->GetHoolaBtnPos(EBT_HOOL_HIGHTHANK);
		m_parrBtn[ BTN_HIGHTHANK ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &m_btnSprAll, 4/4, IDM_HOOLA_HIGHTHANK );
		m_parrBtn[ BTN_HIGHTHANK ]->Show( FALSE );
		m_ctrlHoolaBtnList.Add(m_parrBtn[ BTN_HIGHTHANK ]);

		ptPos = pHoolaUIData->GetHoolaBtnPos(EBT_HOOL_LOWTHANK);
		m_parrBtn[ BTN_LOWTHANK ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &m_btnSprAll, 32/4, IDM_HOOLA_LOWTHANK );
		m_parrBtn[ BTN_LOWTHANK ]->Show( FALSE );
		m_ctrlHoolaBtnList.Add(m_parrBtn[ BTN_LOWTHANK ]);

		ptPos = pHoolaUIData->GetHoolaBtnPos(EBT_HOOL_THANK);
		m_parrBtn[ BTN_THANK ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &m_btnSprAll, 16/4, IDM_HOOLA_THANK );
		m_ctrlHoolaBtnList.Add(m_parrBtn[ BTN_THANK ]);

		ptPos = pHoolaUIData->GetHoolaBtnPos(EBT_HOOL_THANKSELECT);
		m_parrBtn[ BTN_THANKSELECT ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &m_btnSprAll, 56/4, IDM_HOOLA_THANKSELECT );
		m_parrBtn[ BTN_THANKSELECT ]->Show( FALSE );
		m_ctrlHoolaBtnList.Add(m_parrBtn[ BTN_THANKSELECT ]);

		ptPos = pHoolaUIData->GetHoolaBtnPos(EBT_HOOL_GAMESTART);
		m_parrBtn[ BTN_GAMESTART ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &m_btnSprGameStart, 0, IDM_HOOLA_GAMESTART );
		m_parrBtn[ BTN_GAMESTART ]->Show( FALSE );
		((CMyGraphButton*)m_parrBtn[ BTN_GAMESTART ])->SetSize(197, 186);
		m_ctrlHoolaBtnList.Add(m_parrBtn[ BTN_GAMESTART ]);

		ptPos = pHoolaUIData->GetHoolaBtnPos(EBT_HOOL_PRACTICS);
		m_parrBtn[ BTN_PRACTICS ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &m_btnSprPractics, 0, IDM_HOOLA_PRACTICS );
		m_parrBtn[ BTN_PRACTICS ]->Show( FALSE );
		((CMyGraphButton*)m_parrBtn[ BTN_PRACTICS ])->SetSize(200, 113);
		m_ctrlHoolaBtnList.Add(m_parrBtn[ BTN_PRACTICS ]);

		ptPos = pHoolaUIData->GetHoolaBtnPos(EBT_HOOL_PASS);
		m_parrBtn[ BTN_PASS ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &m_btnSprAll, 48/4, IDM_HOOLA_PASS );
		m_parrBtn[ BTN_PASS ]->Show( FALSE );
		m_ctrlHoolaBtnList.Add(m_parrBtn[ BTN_PASS ]);

		ptPos = pHoolaUIData->GetHoolaBtnPos(EBT_HOOL_GET);
		m_parrBtn[ BTN_GETCARD ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &m_btnSprAll, 24/4, IDM_HOOLA_GETCARD );
		m_parrBtn[ BTN_GETCARD ]->Show( FALSE );
		m_ctrlHoolaBtnList.Add(m_parrBtn[ BTN_GETCARD ]);

		ptPos = pHoolaUIData->GetHoolaBtnPos(EBT_HOOL_DUMP);
		m_parrBtn[ BTN_DUMPCARD ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &m_btnSprAll, 28/4, IDM_HOOLA_DUMPCARD );
		m_parrBtn[ BTN_DUMPCARD ]->Show( FALSE );
		m_ctrlHoolaBtnList.Add(m_parrBtn[ BTN_DUMPCARD ]);

		// 숫자 정렬
		ptPos = pHoolaUIData->GetHoolaBtnPos(EBT_HOOL_SORT_STRAIGHT);
		m_parrBtn[ BTN_SORTCARD_STRAIGHT ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &m_btnSprAll, 36/4, IDM_HOOLA_SORT_STRAIGHT );
		m_parrBtn[ BTN_SORTCARD_STRAIGHT ]->Enable( FALSE );
		m_ctrlHoolaBtnList.Add(m_parrBtn[ BTN_SORTCARD_STRAIGHT ]);

		// 무늬 정렬
		ptPos = pHoolaUIData->GetHoolaBtnPos(EBT_HOOL_SORT_TOGETHER);
		m_parrBtn[ BTN_SORTCARD_TOGETHER ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &m_btnSprAll, 20/4, IDM_HOOLA_SORT_TOGETHER );
		m_parrBtn[ BTN_SORTCARD_TOGETHER ]->Enable( FALSE );
		m_ctrlHoolaBtnList.Add(m_parrBtn[ BTN_SORTCARD_TOGETHER ]);

		ptPos = pHoolaUIData->GetHoolaBtnPos(EBT_HOOL_RESULT);
		m_parrBtn[ BTN_RESULTOK ]->Init( m_pParent, m_pPage, ptPos.x, ptPos.y, &m_btnSprResultOK, 0, IDM_HOOLA_RESULTOK );
		m_parrBtn[ BTN_RESULTOK ]->Show( FALSE );
		m_ctrlHoolaBtnList.Add(m_parrBtn[ BTN_RESULTOK ]);
	}
}

void CGameButtonHA::ClearFlag()
{
	if ( GetCurrentWhere() != IDX_GAMEWHERE_GAME )
	{
		return;
	}

	CGameViewButtonMan::ClearFlag();
}

// 사용되어질 키 셋팅
void CGameButtonHA::InitializeKeyValue( CONFIG_BETBTN_TYPE eBtnDirectionType )
{
	CGameViewButtonMan::InitializeKeyValue( eBtnDirectionType );
	
// 	//카드선택  Cutting
// 	if ( g_Config.bCutStyle )
// 	{
// 		// F1 ~ F4
// 		G_KEYDEF[ KEY_CUT_1 ][ 0 ] = SET_VKEY_NOR( rule::SKEY_F1 );
// 		G_KEYDEF[ KEY_CUT_2 ][ 0 ] = SET_VKEY_NOR( rule::SKEY_F2 );
// 		G_KEYDEF[ KEY_CUT_3 ][ 0 ] = SET_VKEY_NOR( rule::SKEY_F3 );
// 		G_KEYDEF[ KEY_CUT_4 ][ 0 ] = SET_VKEY_NOR( rule::SKEY_F4 );
// 	}
// 	else
// 	{
// 		// 1 ~ 4
// 		G_KEYDEF[ KEY_CUT_1 ][ 1 ] = SET_VKEY_NOR( rule::SKEY_1 );
// 		G_KEYDEF[ KEY_CUT_2 ][ 1 ] = SET_VKEY_NOR( rule::SKEY_2 );
// 		G_KEYDEF[ KEY_CUT_3 ][ 1 ] = SET_VKEY_NOR( rule::SKEY_3 );
// 		G_KEYDEF[ KEY_CUT_4 ][ 1 ] = SET_VKEY_NOR( rule::SKEY_4 );
// 
// 	
// 		G_KEYDEF[ KEY_CUT_1 ][ 2 ] = SET_VKEY_EXT( rule::SKEY_GRAY_DEL );
// 		G_KEYDEF[ KEY_CUT_1 ][ 3 ] = SET_VKEY_NOR( rule::SKEY_GRAY_END );   //VK_NUMPAD1
// 
// 		G_KEYDEF[ KEY_CUT_2 ][ 2 ] = SET_VKEY_EXT( rule::SKEY_GRAY_END );
// 		G_KEYDEF[ KEY_CUT_2 ][ 3 ] = SET_VKEY_NOR( rule::SKEY_GRAY_DOWN );  //VK_NUMPAD2
// 
// 		G_KEYDEF[ KEY_CUT_3 ][ 2 ] = SET_VKEY_EXT( rule::SKEY_GRAY_PGDN );
// 		G_KEYDEF[ KEY_CUT_3 ][ 3 ] = SET_VKEY_NOR( rule::SKEY_GRAY_PGDN );  //VK_NUMPAD3 
// 
// 		G_KEYDEF[ KEY_CUT_4 ][ 2 ] = SET_VKEY_EXT( rule::SKEY_GRAY_INS );
// 		G_KEYDEF[ KEY_CUT_4 ][ 3 ] = SET_VKEY_NOR( rule::SKEY_GRAY_LEFT );  //VK_NUMPAD4
// 
// 	}
// 
// 	// 바꾸기, 패스 단축키
// 	G_KEYDEF[ KEY_CUT_CHANGE ][ 0 ] = SET_VKEY_EXT( rule::SKEY_GRAY_HOME ); 
// 	G_KEYDEF[ KEY_CUT_CHANGE ][ 1 ] = SET_VKEY_NOR( rule::SKEY_GRAY_CENTER );//VK_NUMPAD5
// 	G_KEYDEF[ KEY_CUT_CHANGE ][ 2 ] = SET_VKEY_NOR( rule::SKEY_5 );          //5
// 
// 	G_KEYDEF[ KEY_CUT_PASS ][ 0 ] = SET_VKEY_EXT( rule::SKEY_GRAY_PGUP );
// 	G_KEYDEF[ KEY_CUT_PASS ][ 1 ] = SET_VKEY_NOR( rule::SKEY_GRAY_RIGHT ); //VK_NUMPAD6;
// 	G_KEYDEF[ KEY_CUT_PASS ][ 2 ] = SET_VKEY_NOR( rule::SKEY_6 ); //6;
}

void CGameButtonHA::ResetGameButton()
{
	m_parrBtn[ BTN_STOP ]->Enable( FALSE );
	m_parrBtn[ BTN_REGIST ]->Enable( FALSE );
	m_parrBtn[ BTN_SEVENFOURTHANK ]->Enable( FALSE );
	m_parrBtn[ BTN_HIGHTHANK ]->Enable( FALSE );
	m_parrBtn[ BTN_LOWTHANK ]->Enable( FALSE );
	m_parrBtn[ BTN_THANK ]->Enable( FALSE );
	m_parrBtn[ BTN_PASS ]->Enable( FALSE );
	m_parrBtn[ BTN_GETCARD ]->Enable( FALSE );
	m_parrBtn[ BTN_DUMPCARD ]->Enable( FALSE );
	m_parrBtn[ BTN_SORTCARD_STRAIGHT ]->Enable( FALSE );
	m_parrBtn[ BTN_SORTCARD_TOGETHER ]->Enable( FALSE );
	m_parrBtn[ BTN_RESULTOK ]->Enable( FALSE );
}

BOOL CGameButtonHA::IsSortBtnEnableState()
{
	if ( g_ObserverMan.IsMyObserver() )
		return FALSE;
	if ( g_PLAYER(0)->PlayState != EPLAYER_GAMEPLAYING )
		return FALSE;
	if ( g_PLAYER(0)->m_nTotalCardCount <= 1 )
		return FALSE;
	if ( g_GAMEPROCESS()->GetPlayScr()->m_bStartFrame ) 
		return FALSE;

	return TRUE;
}

void CGameButtonHA::DrawBtn(CDC *pDC)
{
	CGameViewButtonMan::DrawBtn(pDC);

	if ( FALSE == IsSortBtnEnableState() )
	{
		m_parrBtn[ BTN_SORTCARD_STRAIGHT ]->Enable( FALSE );
		m_parrBtn[ BTN_SORTCARD_TOGETHER ]->Enable( FALSE );
	}
	else
	{
		m_parrBtn[ BTN_SORTCARD_STRAIGHT ]->Enable( TRUE );
		m_parrBtn[ BTN_SORTCARD_TOGETHER ]->Enable( TRUE );
	}
	
	m_ctrlHoolaBtnList.Draw(pDC);
}

BOOL CGameButtonHA::OnLButtonDown(int x , int y)
{
	if (CGameViewButtonMan::OnLButtonDown(x, y))
		return TRUE;

	return m_ctrlHoolaBtnList.OnLButtonDown(x,y);
}

BOOL CGameButtonHA::OnLButtonUp(int x , int y)
{
	if (CGameViewButtonMan::OnLButtonUp(x, y))
		return TRUE;

	return m_ctrlHoolaBtnList.OnLButtonUp(x,y);
}

BOOL CGameButtonHA::OnMouseMove(int x , int y)
{
	if (CGameViewButtonMan::OnMouseMove(x, y))
		return TRUE;

	return m_ctrlHoolaBtnList.OnMouseMove(x,y);
}

BOOL CGameButtonHA::OnCommand( WPARAM wParam, LPARAM lParam )
{		
	// TODO: Add your specialized code here and/or call the base class
	switch(wParam)
	{
	case IDM_HOOLA_PRACTICS:
		g_GAMEPROCESS()->GetPlayScr()->ReqPracticsGameStart();
		break;
	case IDM_HOOLA_GAMESTART:
		g_GAMEPROCESS()->GetPlayScr()->ReqGameStart();
		break;
	case IDM_HOOLA_STOP:
		g_GAMEPROCESS()->GetPlayScr()->StopBtnClick();
		break;
	case IDM_HOOLA_REGIST:
		g_GAMEPROCESS()->GetPlayScr()->RegisBtnClick();
		break;
	case IDM_HOOLA_THANK:
	case IDM_HOOLA_THANKSELECT:
		g_GAMEPROCESS()->GetPlayScr()->ThankBtnClick();
		break;
	case IDM_HOOLA_PASS:
		g_GAMEPROCESS()->GetPlayScr()->PassBtnClick();
		break;
	case IDM_HOOLA_GETCARD:
		g_GAMEPROCESS()->GetPlayScr()->GetCardBtnClick();
		break;
	case IDM_HOOLA_DUMPCARD:
		g_GAMEPROCESS()->GetPlayScr()->DumpCardBtnClick();
		break;
	case IDM_HOOLA_SORT_STRAIGHT:
	case IDM_HOOLA_SORT_TOGETHER:
		g_GAMEPROCESS()->GetPlayScr()->SortBtnClick();
		break;
	case IDM_HOOLA_LARROW:
		break;
	case IDM_HOOLA_RARROW:
		break;
	case IDM_HOOLA_SELECTOK:
		break;
	case IDM_HOOLA_RESULTOK:
		g_GAMEPROCESS()->GetPlayScr()->ResultOkBtnClick();
		break;
	case IDM_HOOLA_SHOWHELP:
		break;
	case IDM_HOOLA_STOPHELP:
		break;
	case IDM_HOOLA_EMERGE:
		break;
	case IDM_HOOLA_OBSERVERVIEW:
		break;
	case IDM_HOOLA_SEVENFOURCARDTHANK:
		if ( !(g_GAMEPROCESS()->GetPlayScr()->m_bWind || g_GAMEPROCESS()->GetPlayScr()->m_bDrag) )
		{
			g_GAMEPROCESS()->GetPlayScr()->StopCard(CCL_STOPGAME::eSTOP_TYPE_SEVEN4);
			g_GAMEPROCESS()->GetPlayScr()->SetNullButton();
		}
		break;
	case IDM_HOOLA_HIGHTHANK:
		if ( !(g_GAMEPROCESS()->GetPlayScr()->m_bWind || g_GAMEPROCESS()->GetPlayScr()->m_bDrag) )
		{
			g_GAMEPROCESS()->GetPlayScr()->StopCard(CCL_STOPGAME::eSTOP_TYPE_DABANG);
			g_GAMEPROCESS()->GetPlayScr()->SetNullButton();
		}
		break;
	case IDM_HOOLA_LOWTHANK:
		if ( !(g_GAMEPROCESS()->GetPlayScr()->m_bWind || g_GAMEPROCESS()->GetPlayScr()->m_bDrag) )
		{
			g_GAMEPROCESS()->GetPlayScr()->StopCard(CCL_STOPGAME::eSTOP_TYPE_SOBANG);
 			g_GAMEPROCESS()->GetPlayScr()->SetNullButton();
		}
		break;
// 	case IDM_EXIT:
// 	case IDM_EXIT_BOTTOM:
// 		{
// // 			if((m_pParent->m_bExitSubscript == FALSE) && (GDM().GetINT64(GDK_OBSERVER_STEP) == 2))
// // 			{
// // 				m_pParent->SendMessage(WM_COMMAND, IDM_PARTICIPATION, 0);
// // 			}
// // 
// // 			CPlayer *pPlayer = GM().GetMyInfo();
// // 			bool bExit = true;
// // 
// // 			CCL_EXITSUBSCRIPT esmsg;
// // 			esmsg.Set(g_RI.RoomNum, pPlayer->UI.UNum, pPlayer->UI.ID, m_pParent->m_bExitSubscript?2:1);
// // 			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), esmsg.pData, esmsg.GetTotalSize());
// 		}
// 		break;
	case IDM_GIVEUP:
		{
			if (GM().GetCurrentGameType() == IDX_GAME_HA)
			{
				CMsgBoxDlg dlg(GM().GetMainWnd());
				m_dlgGiveUp = &dlg;
				CString csMsg;
				csMsg.Format("보유한 금액 중 %s이 차감되어 상대방에게 지급됩니다. 정말로 기권하시겠습니까?",
					NMBASE::UTIL::g_NumberToHangul(m_i64GiveUpMoney));
				
				if (m_dlgGiveUp->DoModal(CMsgBoxDlg::EMBT_GIVEUP, csMsg) == IDOK)
				{
					// 기권 했을 경우
					g_GAMEPROCESS()->SendCL_PRESS_GIVEUP();
				}
			}
		}
		break;
	}
	
	return CGameViewButtonMan::OnCommand(wParam, lParam);
}

void CGameButtonHA::SetGiveUpMoney(INT64 i64GiveUpMoney)
{
	m_i64GiveUpMoney = i64GiveUpMoney;
}

void CGameButtonHA::CloseGiveUpDlg()
{
	if (::IsWindow(m_dlgGiveUp->GetSafeHwnd()) == false)
		return;
	
	m_dlgGiveUp->CloseDlg();
}
