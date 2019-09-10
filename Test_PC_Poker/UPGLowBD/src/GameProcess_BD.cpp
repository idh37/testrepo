// GameProcess_Base.cpp: implementation of the GameProcess_BD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameProcess_BD.h"

#include "AutoBetSetUpWnd.h"
#include "CommMsgDef_Game.h"

#include "./Effect/BetCmdEffect.h"
#include "./Effect/HighValueEffect.h"
#include "./Effect/CuttingStageMsgEffect.h"
#include "./Effect/CuttingStageIndicateEffect.h"
#include "./Effect/MadeEffect.h"
#include "./Effect/GradeEffect.h"

#include "BugSplat.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern __declspec(dllimport) MiniDmpSender *mpSender;
wchar_t* templog;

void CardEventCB( LPVOID pData, LPVOID pValue )
{
	CARDEVENT *pEvent = static_cast< CARDEVENT* >( pData );

	if ( pEvent == NULL )
	{
		return;
	}

	switch ( pEvent->EventCode )
	{
	case EVENT_BD_PREPARESTART_ONEFLY_END:  // ó�� 4�� �� ���徿 ���� �� ȣ��
		{
			int nPNum = pEvent->Option;
			int nIndex = pEvent->Option2;

			if ( nPNum < 0 || nPNum >= MAX_PLAYER )
			{
				break;
			}

			if ( !g_ObserverMan.IsMyObserver()  && nPNum == 0 )
			{
				g_PLAYER( nPNum )->SetFaceUp( nIndex );					
			}	

			g_PLAYER( nPNum )->ShowCard( nIndex );

		} break;

	case EVENT_BD_PREPARESTART_FOURFLY_END: // ó�� 4�� ��� �޾����� ( ������ �������� 4�徿 ��� �����ְ� ���� )
		{
			int nPNum = pEvent->Option;
			int nIndex = pEvent->Option2;

			if ( nPNum < 0 || nPNum >= MAX_PLAYER )
			{
				break;
			}

			if ( !g_ObserverMan.IsMyObserver() && g_GAME_BD()->m_bWindCardEnd  && g_PLAYER( 0 )->JoinState == 1 && g_PLAYER( 0 )->PlayState == 1)			
			{
				CCL_WINDCARDEND msg2;
				msg2.Set(g_RI.RoomNum, g_PLAYER( 0 )->ServPNum, g_PLAYER( 0 )->UI.UNum, g_PLAYER( 0 )->UI.ID );//int rnum, int pnum, int unum, char *id)
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg2.pData, msg2.GetTotalSize());

				//m_bFlyWindCard = FALSE;
				g_pGameView->m_cPlayerDrawManager.SetFlyWindCard( false );
				g_GAME_BD()->m_bWindCardEnd = false;	

				// ä�� ����				
				if(!g_GAME_BD()->m_bBlind)
				{
					g_GAME_BD()->AutoUp_Recommand(); // �ڵ� ī�� �ø�
					g_PLAYER( 0 )->SetCardResult();	//���� ����	
				}
				g_PLAYER( 0 )->m_bShowValue = true;
				g_PLAYER( 0 )->DrawEft_CompleteMadeValue( 0 , true );
			}

		} break;


	case EVENT_BD_CUTTING_WAIT: // Ŀ�� Ÿ�ӽ� ( ������ �ð��� 0 �϶� ȣ�� )
		{	
			int nPNum = pEvent->Option;
			int nIndex = pEvent->Option2;

			if ( nPNum < 0 || nPNum >= MAX_PLAYER )
			{
				break;
			}

			//������ ī�带 ��� �����.
			int nCardIndex = g_GAME_BD()->m_nPosIndex[ nPNum ][ nIndex ];

			CNMyCard* pCard = &g_PLAYER( nPNum )->m_cMyCard[ nCardIndex ];
			if ( pCard != NULL )
			{			
				pCard->SetStayState( true ); // ī�� Draw ����
			}

			MOVECARD_LIST *pCardData =  static_cast< MOVECARD_LIST* >( pValue );

			//���� ����
			if ( pCardData && pCardData->m_sCardMoveData.m_sFirstSound.bSoundUse )
			{
				g_GAME_BD()->SoundFXCutting( pCardData->m_sCardMoveData.m_sFirstSound.nCuttingNum, g_PLAYER( nPNum )->UI.Sex );
				pCardData->m_sCardMoveData.m_sFirstSound.Clear();
			}

			
		} break;

	case EVENT_BD_CUTTING_MOVE_UP: // Ŀ�� Ÿ�ӽ� ( Move Up ī�� ���徿 �������� ���� �Ϸ� )
		{
			int nPNum = pEvent->Option;
			int nIndex = pEvent->Option2;

			if ( nPNum < 0 || nPNum >= MAX_PLAYER )
			{
				break;
			}

			if ( strlen( g_PLAYER( nPNum )->UI.ID)>0 && g_PLAYER( nPNum )->JoinState == 1 && g_PLAYER( nPNum )->PlayState == 1 && !g_PLAYER( nPNum )->bFold )
			{
				int nCardIndex = g_GAME_BD()->m_nPosIndex[ nPNum ][ nIndex ];

				CNMyCard* pCard = &g_PLAYER( nPNum )->m_cMyCard[ nCardIndex ];
				if ( pCard != NULL )
				{
					if ( !g_ObserverMan.IsMyObserver() && 0 == nPNum )
					{
						pCard->SetCardNo( g_GAME_BD()->m_nCardIndex[ nPNum ][ nIndex ] );
					}
					else
					{
						if ( pCard->m_bFront )
						{
							pCard->SetCardNo( g_GAME_BD()->m_nCardIndex[ nPNum ][ nIndex ] );
						}
					}				
				}
			}

			//����
			if ( nPNum == 0 && !g_ObserverMan.IsMyObserver() )
			{
				g_GAME_BD()->AutoPos_Recommand();
			}

		} break;

	case EVENT_BD_CUTTING_MOVE_DOWN: // Ŀ�� Ÿ�ӽ� ( Move Down ���徿 �� ī�嵦���� )
		{
			int  nPNum = pEvent->Option;
			int  nIndex = pEvent->Option2;
			BOOL bShowValue = pEvent->Option3;

			if ( nPNum < 0 || nPNum >= MAX_PLAYER )
			{
				break;
			}	

			if ( strlen( g_PLAYER( nPNum )->UI.ID ) > 0 && g_PLAYER( nPNum )->JoinState == 1 && g_PLAYER( nPNum )->PlayState == 1 && !g_PLAYER( nPNum )->bFold )
			{
				/*
				int nCardIndex = g_GAME_BD()->m_nPosIndex[ nPNum ][ nIndex ];
				TPASSERT( nCardIndex != -1 );

				CNMyCard* pCard = NULL;
				pCard = &g_PLAYER( nPNum )->m_cMyCard[ nCardIndex ];
				*/
				
				GameProcess_BD * gameBD = g_GAME_BD();
                int nCardIndex = gameBD->m_nPosIndex[ nPNum ][ nIndex ];
                //TPASSERT( nCardIndex != -1 );

                CNMyCard* pCard = NULL;
				CPlayer_BD* playerBD = g_PLAYER( nPNum );
				
				/*
				CString logData;
				logData.Format("[Recv] UniqNo : %s ", playerBD->UI.UniqNo);
				logData.AppendFormat("CardIndex : %d\n", nCardIndex);

				// ���׷α� add logData
				NMBASE::UTIL::BugSplatWrapper::GetInstance().AddMemLog(logData);
				*/

                pCard = &playerBD->m_cMyCard[ nCardIndex ];




				if ( pCard != NULL) 
				{					
					//��ǥ ����ġ ( ���õ� ī�� ���� )
					POINT ptCardPos;
					ptCardPos.x = pCard->m_Xp;
					ptCardPos.y = ( ( nPNum != 0 )? pCard->m_Yp : g_PLAYER( nPNum )->m_sUIPos.PT_Card.y );
					pCard->SetPos( ptCardPos.x, ptCardPos.y );

					if ( pCard->IsCardStay() ) // �̵��Ϸ�
					{
						pCard->SetStayState( false );					
					}				

					if ( nPNum == 0 && !g_ObserverMan.IsMyObserver() )
					{
						pCard->m_bFront = true;

						if ( pCard->IsSelected() )
						{
							pCard->SetSelected( false );
						}

						if ( g_GAME_BD()->IsHiddenCardFlag() )
						{
							//����ī���� ���� ī�带 �׷���
							if ( pCard->IsHiddenCard() && !pCard->IsHiddenMovingComplete() ) 
							{
								pCard->SetHiddenMovingComplete( true );	// ���� �� ī�嵦�� ����ī�� �׸���
							}
						}
					}
				}
			}

		} break;

	case EVENT_BD_CUTTING_PASS:  // Ŀ�� Ÿ�ӽ� ( �н� ���常 ȣ�� )
		{
			int  nPNum = pEvent->Option;
			int  nIndex = pEvent->Option2;

			if ( nPNum < 0 || nPNum >= MAX_PLAYER )
			{
				break;
			}	

			MOVECARD_LIST *pCardData =  static_cast< MOVECARD_LIST* >( pValue );


			//���� ����
			if ( pCardData && pCardData->m_sCardMoveData.m_sFirstSound.bSoundUse  )
			{
				g_GAME_BD()->SoundFXCutting( pCardData->m_sCardMoveData.m_sFirstSound.nCuttingNum, g_PLAYER( nPNum )->UI.Sex );
				pCardData->m_sCardMoveData.m_sFirstSound.Clear();
			}


		} break;

	case EVENT_BD_CUTTING_ALL_DOWN: // Ŀ�� ü���� �Ϸ� ( ī�� ��� �̵��Ϸ� ##���� -> ��Ŷ)
	case EVENT_BD_CUTTING_ALL_PASS: // Ŀ�� �н� �Ϸ�
		{
			int nPNum = pEvent->Option;

			if ( nPNum < 0 || nPNum >= MAX_PLAYER )
			{
				break;
			}	

			if ( g_pGameView->m_cPlayerDrawManager.IsFlyWindCard() )
			{
				g_pGameView->m_cPlayerDrawManager.SetFlyWindCard( false );
			}

			if ( !g_ObserverMan.IsMyObserver() )
			{
				// �н�, ü���� ��� ������ ��Ŷ ������ ( ���� 2011_12_04 )
				if ( nPNum == 0 && g_GAME_BD()->GetCuttingStageInfo()->bMyTurn && g_GAME_BD()->GetCuttingStageInfo()->bChangeCardFlyEnd == true && g_GAME_BD()->GetCuttingStageInfo()->bChangeStage == false )
				{
					g_GAME_BD()->GetCuttingStageInfo()->Clear();

					CCL_BD_CARD_EXCHANGEEND cee;
					cee.Set( g_RI.RoomNum, g_PLAYER( 0 )->ServPNum, g_PLAYER( 0 )->UI.UNum, g_PLAYER( 0 )->UI.ID );//int rnum, int pnum, int unum, char *id)
					g_GAME_BD()->SetCardExchangeLog(cee);
					NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), cee.pData, cee.GetTotalSize());


					if ( g_PLAYER( 0 )->JoinState == 1 && g_PLAYER( 0 )->PlayState == 1 && !g_PLAYER( 0 )->bFold )
					{
						g_PLAYER( 0 )->m_bDontSelectCard = false;

						//���� ������ ���� �� �� ī�� ��ġ�� ���̻� ī�� ������ ����
						if ( g_RI.nChangeType == 3 )
						{
							g_PLAYER( 0 )->m_bDontSelectCard = true;
							g_PLAYER( 0 )->ReSetCardPostion();

						}
						else
						{
							g_PLAYER( 0 )->m_bDontSelectCard = false;

							// ���� Ŀ�� �ٲٱ�, �н� ���� ��� ���� Ŀ�� ���� ����� �ڵ����� ��� Ȯ�� �� ���� ��Ų��.
							if( g_Config.nAutoUpType == 1 ) // 0 : ������, 1 : �����, 2 : �����
							{
								g_GAME_BD()->AutoUp_Recommand(); // �ڵ��ø�
							}
						}


						if ( !g_GAME_BD()->IsHiddenCardFlag() )
						{
							if ( g_PLAYER( 0 )->JoinState == 1 && g_PLAYER( 0 )->PlayState == 1 && !g_PLAYER( 0 )->bFold )
							{
								// ���̵� ����Ʈ �ѹ��� �����ֱ� ���ؼ�
								int nOldPoint = g_PLAYER( 0 )->GetMyLowBDRule()->m_RuleCompleteCard.numberpoint;
								//���� ����		
								g_PLAYER( 0 )->SetCardResult();
								g_PLAYER( 0 )->m_bShowValue = true;
								g_PLAYER( 0 )->DrawEft_CompleteMadeValue( nOldPoint );
							}						
						}
					}
				}
			}		

			if ( strlen( g_PLAYER( nPNum )->UI.ID ) > 0 && g_PLAYER( nPNum )->JoinState == 1 && g_PLAYER( nPNum )->PlayState == 1 && !g_PLAYER( nPNum )->bFold )
			{
				//���⼭ ī�� �ٲ۳��� ����� ǥ�� �� �ش�.
				g_GAME_BD()->CuttingNumEffectDraw( nPNum );

				int nCuttingRoundType = g_RI.nChangeType - 1;

				if ( nCuttingRoundType >= 0 )
				{
					g_PLAYER( nPNum )->GetCutInfo().m_arrbShow[ nCuttingRoundType ] = true;
				}
			}

		} break;
	}
}


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

GameProcess_BD::GameProcess_BD(CGame *pGame)
: CGameProcess( pGame ),
m_pMovingCardMgr( NULL ),
m_pObserverDlg( NULL ),
m_bChangeState( false ),
m_bPassState( false ),
m_pCuttingStageInfo( NULL )
{
	SAFE_NEW( m_pMovingCardMgr, CNMovingCardMan );
	SAFE_NEW( m_pCuttingStageInfo, CUTTING_STAGE_INFO );
	
	m_i64JackPotMoney = 0;
	m_i64OldJackPotMoney = 0;
}

GameProcess_BD::~GameProcess_BD()
{
	m_pObserverDlg = NULL;
	SAFE_DELETE( m_pMovingCardMgr );
	SAFE_DELETE( m_pCuttingStageInfo );
	
}

bool GameProcess_BD::IsValid()
{
	return true;
}


CPlayer *GameProcess_BD::GetPlayer(int i)
{
	ASSERT ( i< GetMaxNewPlayer() && i >= 0 );
	return static_cast< CPlayer* >( &m_cPlayer[ i ] );
}


void GameProcess_BD::Init(NMBASE::GRAPHICGDI::CPage *ppage)
{
	if ( ppage == NULL )
	{
		return;
	}

	Clear();

	m_pObserverDlg = GetObserverDlg(); 

	// 
	for ( int i = 0 ; i < GetMaxNewPlayer() ; i++ )
	{
		m_cPlayer[ i ].Clear();
		m_cPlayer[ i ].SetPlayerNum( i );
		m_cPlayer[ i ].InitRoomData();
	}	

	//m_aryCardData[ TOTAL_CARD_BD ] �ʱ�ȭ
	for ( int i = 0 ; i < TOTAL_CARD_BD ; i++ )
	{
		m_aryCardData[ i ].bSelectFlag = false;
		m_aryCardData[ i ].nCardIndex = -1;
	}

	m_pPage = ppage;

	m_cAllBetChip.Init( ppage, &g_cUIData, this, &GLOBAL_SPR[ spr::GSPR_GAME_CHIP ] );

	set_GameSpeed();

	Initialize_ExceptionKey();
}

void GameProcess_BD::Initialize_ExceptionKey()
{
	m_vtExceptionKey_NotMsg.clear();
	m_vtExceptionKey_Msg.clear();

	//ä��â�� �޽����� ������
	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_NOR( rule::SKEY_SPACE ) );
	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_EXT( rule::SKEY_GRAY_UP ) );
	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_EXT( rule::SKEY_GRAY_RIGHT ) );
	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_EXT( rule::SKEY_GRAY_LEFT ) );
	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_EXT( rule::SKEY_GRAY_DOWN ) );

	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_EXT( rule::SKEY_GRAY_INS ) );
	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_EXT( rule::SKEY_GRAY_HOME ) );
	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_EXT( rule::SKEY_GRAY_PGUP ) );
	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_EXT( rule::SKEY_GRAY_DEL ) );
	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_EXT( rule::SKEY_GRAY_END ) );
	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_EXT( rule::SKEY_GRAY_PGDN ) );
	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_EXT( rule::SKEY_ENTER ) );

	// 	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_NOR( rule::SKEY_GRAY_END ) );  // VK_NUMPAD
	// 	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_NOR( rule::SKEY_GRAY_DOWN ) );
	// 	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_NOR( rule::SKEY_GRAY_PGDN ) );
	// 	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_NOR( rule::SKEY_GRAY_LEFT ) );
	// 	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_NOR( rule::SKEY_GRAY_CENTER ) );
	// 	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_NOR( rule::SKEY_GRAY_RIGHT ) );
	// 	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_NOR( rule::SKEY_GRAY_HOME ) );
	// 	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_NOR( rule::SKEY_GRAY_UP ) );
	// 	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_NOR( rule::SKEY_GRAY_PGUP ) );
	// 	m_vtExceptionKey_NotMsg.push_back( SET_VKEY_NOR( rule::SKEY_GRAY_DEL ) );


	//ä��â�� �޽����� ������
	m_vtExceptionKey_Msg.push_back( SET_VKEY_EXT( rule::SKEY_GRAY_INS ) );
	m_vtExceptionKey_Msg.push_back( SET_VKEY_EXT( rule::SKEY_GRAY_HOME ) );
	m_vtExceptionKey_Msg.push_back( SET_VKEY_EXT( rule::SKEY_GRAY_PGUP ) );
	m_vtExceptionKey_Msg.push_back( SET_VKEY_EXT( rule::SKEY_GRAY_DEL ) );
	m_vtExceptionKey_Msg.push_back( SET_VKEY_EXT( rule::SKEY_GRAY_END ) );
	m_vtExceptionKey_Msg.push_back( SET_VKEY_EXT( rule::SKEY_GRAY_PGDN ) );
	m_vtExceptionKey_Msg.push_back( SET_VKEY_EXT( rule::SKEY_ENTER ) );

	m_vtExceptionKey_Msg.push_back( SET_VKEY_NOR( rule::SKEY_F1 ) );
	m_vtExceptionKey_Msg.push_back( SET_VKEY_NOR( rule::SKEY_F2 ) );
	m_vtExceptionKey_Msg.push_back( SET_VKEY_NOR( rule::SKEY_F3 ) );
	m_vtExceptionKey_Msg.push_back( SET_VKEY_NOR( rule::SKEY_F4 ) );
	m_vtExceptionKey_Msg.push_back( SET_VKEY_NOR( rule::SKEY_F5 ) );
	m_vtExceptionKey_Msg.push_back( SET_VKEY_NOR( rule::SKEY_F6 ) );
	m_vtExceptionKey_Msg.push_back( SET_VKEY_NOR( rule::SKEY_F7 ) );
	m_vtExceptionKey_Msg.push_back( SET_VKEY_NOR( rule::SKEY_F8 ) );

	// 	m_vtExceptionKey_Msg.push_back( SET_VKEY_NOR( rule::SKEY_GRAY_END ) );  // VK_NUMPAD
	// 	m_vtExceptionKey_Msg.push_back( SET_VKEY_NOR( rule::SKEY_GRAY_DOWN ) );
	// 	m_vtExceptionKey_Msg.push_back( SET_VKEY_NOR( rule::SKEY_GRAY_PGDN ) );
	// 	m_vtExceptionKey_Msg.push_back( SET_VKEY_NOR( rule::SKEY_GRAY_LEFT ) );
	// 	m_vtExceptionKey_Msg.push_back( SET_VKEY_NOR( rule::SKEY_GRAY_CENTER ) );
	// 	m_vtExceptionKey_Msg.push_back( SET_VKEY_NOR( rule::SKEY_GRAY_RIGHT ) );
	// 	m_vtExceptionKey_Msg.push_back( SET_VKEY_NOR( rule::SKEY_GRAY_HOME ) );
	// 	m_vtExceptionKey_Msg.push_back( SET_VKEY_NOR( rule::SKEY_GRAY_UP ) );
	// 	m_vtExceptionKey_Msg.push_back( SET_VKEY_NOR( rule::SKEY_GRAY_PGUP ) );
	// 	m_vtExceptionKey_Msg.push_back( SET_VKEY_NOR( rule::SKEY_GRAY_DEL ) );
}

void GameProcess_BD::DoExitSubscript(int bexit)
{
	// ### [ ������� ] ###
	CCL_EXITSUBSCRIPT esmsg;	
	/*esmsg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID, bexit);*/
	// ������ �߰�.
	esmsg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID, bexit, 0);

	// ���μ������� ������ ���� ���¸� �˸�
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), esmsg.pData, esmsg.GetTotalSize());
}

void GameProcess_BD::chip_draw_func(int PNum, INT64 Money)
{
	if(PNum < 0 || PNum > 4) return;	
	if(Money < 0)return;

	m_cAllBetChip.SetMoney(GetRealBet());
	m_cPlayer[ PNum ].OnSetChip(Money);
}

int  GameProcess_BD::GetTotalCardNum()
{
	return m_nTotalCardNum;
}

INT64 GameProcess_BD::GetLastBetMoney(int sPNum,int betCmd)
{
	INT64 Betmoney = 0;
	INT64 CallMoney = 0;
	INT64 DadangMoney = 0;
	INT64 MaxMoney = 0;
	INT64 QuarterMoney = 0;
	INT64 FullMoney = 0;
	INT64 AllinMoney = 0;

	// ��ݾ�
	Betmoney = g_RI.CurSeedMoney;

	//�� �ݾ�
	CallMoney = g_Poker.RU[sPNum].nReCall;

	//���� �ݾ�
	DadangMoney = g_Poker.RU[sPNum].nCalcBat + CallMoney;	

	// ���� 
	MaxMoney = g_Poker.RU[sPNum].nReCall + g_Poker.RU[sPNum].nCalcBat;		

	if(betCmd == RAISE_LAST_CMD_FOLD)
	{
		return  0;
	}	
	else if( betCmd == RAISE_LAST_CMD_CHECK)
	{
		return 0;
	}
	else if( betCmd == RAISE_LAST_CMD_CALL) // �� �ݾ�
	{
		return  CallMoney;
	}
	else if(betCmd == RAISE_LAST_CMD_PING) // �� �ݾ�
	{
		return g_RI.CurSeedMoney;
	}
	else if(betCmd == RAISE_LAST_CMD_DADANG) // ���� �ݾ�
	{
		return DadangMoney;
	}
	else if( betCmd == RAISE_LAST_CMD_HALF ||betCmd == RAISE_LAST_CMD_QUARTER ||betCmd == RAISE_LAST_CMD_FULL || betCmd == RAISE_LAST_CMD_RACE || betCmd == RAISE_LAST_CMD_ALLIN ) // ���� �ݾ�
	{
		return MaxMoney	;
	}		


	return 0;
}

// ### [ Frame++ ] ###
void GameProcess_BD::SetWindCardEnd(BOOL bFirst)
{
	if ( !m_bWindCardEnd )
	{		
		if( bFirst ) // 3�� 3�� => 61�� 1����� 20��
		{
			m_nWindCardEndCnt = 3*200+(50*m_nStartUserNum);
		}
		else // 1�� 7��+
		{
			m_nWindCardEndCnt = 3*200+(30*m_nStartUserNum);
		}
		m_bWindCardEnd = true;
	}
}



//pg.nRaiseBat = m_Raise.GetRaiseBat();
INT64 GameProcess_BD::GetRaiseBat()
{
	INT64 nRaiseBat = 0;

	for(int i=0;i<MAX_PLAYER;i++)
		nRaiseBat = max(nRaiseBat,g_Poker.RU[i].nNowBat);

	return nRaiseBat;
}

//pg.nTotBat   = m_Raise.GetRealBet();
INT64 GameProcess_BD::GetRealBet()
{
	INT64 nRealBet = 0;

	for(int i=0;i<MAX_PLAYER;i++)
		if(g_Poker.RU[i].bInUse) nRealBet+= g_Poker.RU[i].nRealBat;

	return nRealBet;
}


INT64 GameProcess_BD::GetTotalBet()
{
	INT64 nRealBet = 0;

	for(int i=0;i<MAX_PLAYER;i++)
		if(g_Poker.RU[i].bInUse) nRealBet+= g_Poker.RU[i].nTotBat;

	return nRealBet;
}



// ### [���� �߰� �۾�] ###
void GameProcess_BD::SoundFxKind(int betkind, int sex, bool bMax)
{
	//������ : �ƽ� Ŀ�ǵ� �߰� : �ƽ�����Ʈ
	if ( bMax )
	{
		PBPlaySound(sex?true:false, SND_MAX);
	} 
	else
	{
		switch(betkind)
		{
		case RAISE_LAST_CMD_FOLD: //����
			PBPlaySound(sex?true:false, SND_DIE);
			break;
		case RAISE_LAST_CMD_CHECK:// üũ
			//������ ���� : ī�弼���߿� ����Ʈ ����
			if(CheckRoundForCardSet()){
				PBPlaySound(sex?true:false, SND_CHECK);
			}
			break;
		case RAISE_LAST_CMD_CALL: // ��
			//������ ���� : ī�弼���߿� ����Ʈ ����
			if(CheckRoundForCardSet()){
				PBPlaySound(sex?true:false, SND_CALL);
			}
			break;
		case RAISE_LAST_CMD_PING: // ��
			PBPlaySound(sex?true:false, SND_PING);
			break;
		case RAISE_LAST_CMD_DADANG: // ����
			PBPlaySound(sex?true:false, SND_DDANG);
			break;	
		case RAISE_LAST_CMD_HALF: // ����
			PBPlaySound(sex?true:false, SND_HALF);
			break;
		case RAISE_LAST_CMD_ALLIN: // ����
			PBPlaySound(sex?true:false, SND_ALLIN);
			break;
		case RAISE_LAST_CMD_FULL: // Ǯ
			PBPlaySound(sex?true:false, SND_FULL);
			break;
		}
	}
}

void GameProcess_BD::SoundFxKindBig(int betkind, int sex, bool bMax )
{
	//������ : �ƽ� Ŀ�ǵ� �߰� : �ƽ�����Ʈ
	if ( bMax )
	{
		PBPlaySound(sex?true:false, SND_MAX_H);
	} 
	else
	{
		switch(betkind)
		{
		case RAISE_LAST_CMD_FOLD: //����
			PBPlaySound(sex?true:false, SND_DIE_H);
			break;
		case RAISE_LAST_CMD_CHECK:// üũ
			//������ ���� : ī�弼���߿� ����Ʈ ����
			if(CheckRoundForCardSet()){
				PBPlaySound(sex?true:false, SND_CHECK_H);
			}
			
			break;
		case RAISE_LAST_CMD_CALL: // ��
			//������ ���� : ī�弼���߿� ����Ʈ ����
			if(CheckRoundForCardSet()){
				PBPlaySound(sex?true:false, SND_CALL_H);
			}
			break;
		case RAISE_LAST_CMD_PING: // ��
			PBPlaySound(sex?true:false, SND_PING_H);
			break;
		case RAISE_LAST_CMD_DADANG: // ����
			PBPlaySound(sex?true:false, SND_DDANG_H);
			break;	
		case RAISE_LAST_CMD_HALF: // ����
			PBPlaySound(sex?true:false, SND_HALF_H);
			break;
		case RAISE_LAST_CMD_ALLIN: // ����
			PBPlaySound(sex?true:false, SND_ALLIN_H);
			break;
		case RAISE_LAST_CMD_FULL: // Ǯ
			PBPlaySound(sex?true:false, SND_FULL_H);
			break;
		}
	}
}

void GameProcess_BD::SoundFXCutting( int nType, int nSex )
{
	switch( nType )
	{
	case 0:
		{
			PBPlaySound( nSex ? true : false, SND_PASS );
			break;
		}
	case 1:
		{
			PBPlaySound( nSex ? true : false, SND_CUT_1 );
			break;
		}
	case 2:
		{
			PBPlaySound( nSex ? true : false, SND_CUT_2 );
			break;
		}
	case 3:
		{
			PBPlaySound( nSex ? true : false, SND_CUT_3 );
			break;
		}
	case 4:
		{
			PBPlaySound( nSex ? true : false, SND_CUT_4 );
			break;
		}
		// ��ħ�Դϴ�.
	case RSTATE_GAME_ROUND_CHANGECARD_M:
		{
			PBPlaySound( nSex ? true : false, SND_MON );
			break;
		}
	case RSTATE_GAME_ROUND_CHANGECARD_A:
		{
			PBPlaySound( nSex ? true : false, SND_AFT );
			break;
		}
	case RSTATE_GAME_ROUND_CHANGECARD_N:
		{
			PBPlaySound( nSex ? true : false, SND_NIG );
			break;
		}
	}
}


//2006.1.4 ����� �ȳ� �޼��� ���� �κ�
void GameProcess_BD::Insert_EndingTextInformation(CString str,COLORREF tcolor)
{
	if( m_GameEndMsgData.TextMsgCnt >= 30) return;
	if( m_GameEndMsgData.TextMsgCnt < 0 )return;

	m_GameEndMsgData.TextMsg[m_GameEndMsgData.TextMsgCnt] = str;
	m_GameEndMsgData.TextColor[m_GameEndMsgData.TextMsgCnt] = tcolor;	
	m_GameEndMsgData.TextMsgCnt ++;	
}

void GameProcess_BD::Draw_TextInformationMsg()
{
	if( m_GameEndMsgData.TextMsgCnt <= 0)return;	

	for( int i = 0 ; i < m_GameEndMsgData.TextMsgCnt ; i ++ )
	{
		COLORREF tcolor = RGB(153,153,255);

		if( m_GameEndMsgData.TextColor[i] != NULL ){
			tcolor = m_GameEndMsgData.TextColor[i];
		}

	}	
}

/////////////////////////////////////////////////////////////////////
/*
���� �Լ� ���� �߻� ��ü
*/
/////////////////////////////////////////////////////////////////////
void GameProcess_BD::Clear()
{
	// Ŭ���̾�Ʈ ���� ����
	m_nGameState = 0;				// 0 ����, 1, 2, 3, 4, 5, 6 �°����� 7 �°���(���� ����)
	m_nNo1PosPNum = 0;
	m_nMyPosPNum = 0;		
	m_i64Double = 0;

	ResetFlg();
}

void GameProcess_BD::ResetFlg()
{
	m_nGameCount = 0;
	m_bGameStart = FALSE;

	g_pGameView->m_cPlayerDrawManager.SetGameOver( false, timeGetTime() );
	m_nCurPlayer = -1;	
	g_pGameView->m_cPlayerDrawManager.SetFlyWindCard( false );

	m_bBtnClick = FALSE;

	m_bBlind = FALSE;

	// ����
	m_nGameState = 0;		

	// [ ī�� �̺�Ʈ ������ ]
	m_bWindCardEnd	= false;
	m_nWindCardEndCnt = 0;
	m_nStartUserNum	= 0;
	m_nTotalCardNum = 0;
	//2006.1.4 ����� �ȳ� �޼��� �׸��� �κ�
	m_GameEndMsgData.Clear();
	m_stAfterEndingUserInfo.Clear();
	// ��� ����ü
	ZeroMemory(&m_stGameOverResultData,sizeof(m_stGameOverResultData));	
	m_stWinnerPNumData.Clear();	
	m_pMovingCardMgr->Destroy();

	m_BetCount = 0;
	//m_bHiddenCardShow = 0;
	m_bHiddenClick = false;
	m_ptHiddenClickPos.x =0;
	m_ptHiddenClickPos.y =0;

	m_i64JackPotActive = 0;

	//������ ä�� ���
	m_bObserverChatWarning = FALSE;

	m_pCuttingStageInfo->Clear();
	for ( int i = 0 ; i < MAX_PLAYER ; i ++ )
	{
		ZeroMemory(m_nPosIndex[i],sizeof(m_nPosIndex[i]));
		ZeroMemory(m_nCardIndex[i],sizeof(m_nCardIndex[i]));
		m_nChangedNum[i] = 0;
	}

	m_bHaveHiddenCard = false;

	m_bChangeState = false;
	m_bPassState   = false;

	//������ Delay
	m_nResultDelayCount = 0;
	m_bResultDelayStart = false;

	//��ħ�Դϴ�.. ����.. ����.. ���� Delay
	m_nCutInfoDelaySound = 0;
	m_bCutInfoDelaySound = false;
	m_bSex = false;


	ClearCuttingBackUpCard(); //< CYS 100705 >
}

void GameProcess_BD::ResetGame()
{
	// ���׷α� Clear()
	//NMBASE::UTIL::BugSplatWrapper::GetInstance().ClearMemLog();
	NMBASE::SOCK::CSockMan::SetSocketLogTxt(""); 

	ResetFlg();
	g_RI.State = 0;	
	g_RI.nChangeType = 0;	

	GM().GetMyInfo()->JoinState = 0;
	GM().GetMyInfo()->PlayState = 0;			

	g_Poker.nBossPNum = -1;
	g_pGameView->m_cAniSprListManager.Destroy(0);
	g_pGameView->m_cTotalBetMoney.Reset();
	g_pGameView->m_cGameViewBtnMan.Reset();

	for ( int i = 0; i < GetMaxNewPlayer(); i++ ) 
	{
		m_cPlayer[ i ].Reset();
	}

	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();
	g_pGameView->m_cPlayerDrawManager.Reset();
	g_pGameView->m_cGameViewBtnMan.SetEnableBtn( CGameButtonBD::BTN_START, FALSE ); //< CYS 101221 >
	g_pGameView->m_cGameViewBtnMan.SetShowBtn( CGameButtonBD::BTN_START, FALSE ); //< CYS 101221 >
	g_pGameView->m_cGameViewBtnMan.ResetGame();
	g_pGameView->m_cGameHelper.Reset();

	ZeroMemory(&g_Poker, sizeof(g_Poker));

	m_cAllBetChip.Reset();

	m_nActiveBetBtnDelayTime = 0;
	m_nBetCallMoneyClearCnt = 0;	//���ѱݾ׵� �����ֱ�
	g_pGameView->m_cGameViewBtnMan.SetBetBtnType(g_Config.eBetBtnType );
	g_Poker.nImageBoss = -1;

	// ���� Ĩ �������� �����
	m_i64JackPotActive = 0;	

	//������ : �����ư Ȱ��ȭ ���ɻ���
	ReserveState(RESERVESTATE_BET_NONE);
	CanReserve(true);
	g_nCardTotalCount = 0 ; // ī�� ���� �񱳰��� 0���� �ʱ�ȭ.
}


void GameProcess_BD::OnTimer()
{
	m_nGameCount++;

	// [ ī�� ������ ]
	if ( g_RI.State != 0 )
	{
		if( m_bWindCardEnd )
		{
			m_nWindCardEndCnt--;
			if ( m_nWindCardEndCnt <= 0)
			{
				m_bWindCardEnd = false;
				// ������ �޼��� ������.
				// ### [ ������� ] ###
				if(!g_ObserverMan.IsMyObserver() && !m_cPlayer[ 0 ].bFold && m_cPlayer[ 0 ].JoinState == 1 && m_cPlayer[ 0 ].PlayState == 1)
				{
					CCL_WINDCARDEND msg2;					
					msg2.Set(g_RI.RoomNum, GM().GetMyInfo()->ServPNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID);
					NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg2.pData, msg2.GetTotalSize());
				}
			}
		}		

		if ( m_nActiveBetBtnDelayTime > 0 )
		{
			m_nActiveBetBtnDelayTime--;
			if ( m_nActiveBetBtnDelayTime <= 0 )
			{
				m_nActiveBetBtnDelayTime = 0;
				m_bBtnClick = FALSE;
				//m_bFlyWindCard = FALSE; 
				g_pGameView->m_cPlayerDrawManager.SetFlyWindCard( false );
				g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable(false);
				LAYER_GLOBAL_AUTOBETWND()->Set_auto_betting_flag();
			}
		}

		//Ȥ�� ���� �ѹ� �� �˻�
		if ( m_nActiveBetBtnDelayTime < 0 )
		{
			m_nActiveBetBtnDelayTime = 0;
			m_bBtnClick = FALSE;
			//m_bFlyWindCard = FALSE;
			g_pGameView->m_cPlayerDrawManager.SetFlyWindCard( false );
			g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable(false);
			LAYER_GLOBAL_AUTOBETWND()->Set_auto_betting_flag();
		}

		if( m_nBetCallMoneyClearCnt > 0 )
		{
			m_nBetCallMoneyClearCnt--;
			if( m_nBetCallMoneyClearCnt == 0 )
			{
				m_nBetCallMoneyClearCnt = 0;
				//���� �ݾ� ǥ�� �Ѱ͵� �����.
			}
		}

		//�ڵ� ����
		// ī�� ������ �߿��� �׸��� �ʴ´�. �ڵ� ���� �����϶� �� ���ѽð� �ٵǰ��� �ݷ� ������.
		OnCheckTurnLimitTime();		

		/*
		if( m_bAutoBet ) {
		if( m_nAutoBetCnt-- < 0 ){
		m_nAutoBetCnt = 0;
		m_bAutoBet = FALSE;
		auto_betting_call();
		}
		}
		*/

		LAYER_GLOBAL_AUTOBETWND()->OnTimer();


		// Ŀ�ý� call -> Ŀ�û��� ���� ȣ��
		if ( m_bCutInfoDelaySound ) // �������� �������� �ٲ�� ��
		{
			if ( m_nCutInfoDelaySound > 0 )
			{
				m_nCutInfoDelaySound--;

				if ( m_nCutInfoDelaySound == 0 )
				{
					m_bCutInfoDelaySound = false;
					SoundFXCutting( g_Poker.nState, m_bSex );	
				}
			}
		}
	}

	// �� ���ñݾ� ����	
	for ( int i = 0 ; i < MAX_PLAYER; i++)
	{
		m_cPlayer[ i ].OnTimer();
	}

	m_cAllBetChip.OnTimer( );
	m_pMovingCardMgr->Process();

	// ��� ��¿� ���� Delay
	if ( m_bResultDelayStart )
	{
		if ( m_nResultDelayCount > 0 )
		{
			m_nResultDelayCount--;
			if ( m_nResultDelayCount <= 0 )
			{				
				OnResultGameOver();
				m_nResultDelayCount = 0;
				m_bResultDelayStart = false;
			}
		}		
	}
}


BOOL GameProcess_BD::OnMouseMove(int x, int y)
{
	for ( int i = 0 ; i < MAX_PLAYER ; i ++ )
	{			
		m_cPlayer[ i ].OnMouseMove( x, y );
	}

	if ( !g_ObserverMan.IsMyObserver() &&  m_cPlayer[ 0 ].PNum == 0 && m_cPlayer[ 0 ].JoinState != 0 && m_cPlayer[ 0 ].PlayState != 0 && m_cPlayer[ 0 ].m_nCardTotal == TOTAL_CARD_BD )
	{
		//���� ī�带 ������ �ִ��� �˻�
		if ( !m_bHaveHiddenCard )
		{
			return FALSE ;		
		}

		if ( g_pGameView->m_cPlayerDrawManager.IsHiddenCardShow() )
		{
			return FALSE;
		}

		if ( m_bHiddenClick  )
		{			 			
			if (m_ptHiddenClickPos.y < y) 
			{
				int tmp = y - m_ptHiddenClickPos.y;
				m_ptHiddenClickPos.y = y;

				for ( int i = 0; i < 4; i ++ )
				{
					if ( m_cPlayer[ 0 ].m_cMyCard[ i ].IsHiddenCard() )
					{
						m_cPlayer[ 0 ].m_cMyCard[ i ].m_nHinddenArrow += tmp;
					}
				}
			}
			else if(m_ptHiddenClickPos.y > y) 
			{
				int tmp = m_ptHiddenClickPos.y - y;
				m_ptHiddenClickPos.y = y;

				//
				for ( int i = 0; i < 4; i ++ )
				{
					if ( m_cPlayer[ 0 ].m_cMyCard[ i ].IsHiddenCard() )
					{
						m_cPlayer[ 0 ].m_cMyCard[ i ].m_nHinddenArrow -= tmp;
					}

					if ( m_cPlayer[ 0 ].m_cMyCard[ i ].m_nHinddenArrow < -100 )
					{
						m_cPlayer[ 0 ].m_cMyCard[ i ].m_nHinddenArrow = 0;
						m_bHiddenClick = false;
					}
				}
			}
		}		
	}

	return FALSE;
}

BOOL GameProcess_BD::OnLButtonDblclk(int x, int y)
{
	return FALSE;
}

BOOL GameProcess_BD::OnLButtonUp(int x, int y)
{
	m_cPlayer[ 0 ].OnLButtonUp( x, y );
	return FALSE;
}

BOOL GameProcess_BD::OnLButtonDown(int x, int y)
{
	m_cPlayer[ 0 ].OnLButtonDown(x,y);
	return FALSE;
}

void GameProcess_BD::OnGameKey(int nVKey, int nextend, int repeat)
{	
	if ( !IsValid() ) 
	{
		return;
	}

#ifdef _DEBUG

	if ( nVKey == rule::SKEY_F6 )
	{
		// ������ �۾�

		GAMERESULTDATA WinEftData;	//���� â�� �� ����ü
		WinEftData.Clear();

		WinEftData.valuekind = 1;
		WinEftData.pnum = 0;

		WinEftData.nResultMoney = 1004343400200;
		sprintf_s( WinEftData.str_value, "%s", "fdfdfdfe3" );
		WinEftData.nCard[ 0 ] = 1;
		WinEftData.nCard[ 1 ] = 11;
		WinEftData.nCard[ 2 ] = 21;
		WinEftData.nCard[ 3 ] = 31;
		WinEftData.nTotalCardNum = 4;

		strcpy( WinEftData.ID, g_PLAYER( 0 )->UI.ID );

		{
			effect::CHighValueEffect *pObj = new effect::CHighValueEffect;
			if( pObj != NULL )
			{		
				pObj->Init(&WinEftData, true );
				g_pGameView->m_cAniSprListManager.ADDObject(pObj);	

				PBPlayEffectSound(  SND_WIN_HIGH );
			}
		}
	}

	if ( nVKey == rule::SKEY_F8 )
	{
		for ( int i = 0; i < 5; i++ )
		{
			g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni( i, timeGetTime()); 
		}

	}
#endif

	//ä��â����
	if ( nVKey == rule::SKEY_TAB ) 
	{	
		g_pChatWinMan->SelectNextTab();
		return;
	}

	if ( g_RI.State == 0 )
	{
		return;
	}

	if ( m_cPlayer[ 0 ].PNum != 0 || g_ObserverMan.IsMyObserver() || m_cPlayer[ 0 ].JoinState==0 || m_cPlayer[ 0 ].bFold == TRUE ) 
	{
		return;	
	}

	GameKey_betting( nVKey, nextend );			//betting 
	GameKey_cutting( nVKey, nextend, repeat );	//cutting
	GameKey_changepass( nVKey, nextend, repeat );	//�ٲٱ�,�н�
	GameKey_HiddenOpen( nVKey, nextend, repeat ); //����ī�� ����
}

void GameProcess_BD::OnCheckTurnLimitTime()
{
	if( !IsValid() ) return;
	if(g_RI.State == 0)return;

	if ( g_RI.State == 0 )
	{
		return;
	}

	if ( g_pGameView->m_cPlayerDrawManager.GetStartTimerTime() <= 0 )
	{
		return;
	}

	g_pGameView->m_cPlayerDrawManager.SetCurTimerTime(timeGetTime());

	if ( g_Poker.nState == RSTATE_STARTGAME && !g_ObserverMan.IsMyObserver() )
	{
		if ( m_cPlayer[ 0 ].PlayState == 1 && m_nCurPlayer == m_cPlayer[ 0 ].ServPNum && !m_cPlayer[ 0 ].bFold ) // ī�� ������ �߿��� �׸��� �ʴ´�.
		{		
			int tcnt = g_pGameView->m_cPlayerDrawManager.GetTimerTimeGab() / 1000;	

			int nTimeCnt = PBTurnTimeOut( ETTT_BETTING );

			if ( tcnt >= nTimeCnt ) //�� ���� �ð� ������ ���� 
			{	
				if ( m_cPlayer[ 0 ].GetNowMoney() > 0 )
				{
					if ( g_pGameView->m_cGameViewBtnMan.m_bFold && FALSE == m_bBtnClick)
					{
						m_cPlayer[ 0 ].bFold = TRUE;
						GM().GetMyInfo()->bFold = TRUE;

						g_pGameView->m_cGameViewBtnMan.SetShowBtn( CGameButtonBD::BTNP_CHANGE, FALSE );
						g_pGameView->m_cGameViewBtnMan.SetShowBtn( CGameButtonBD::BTNP_PASS, FALSE );

						OnBtnFold( false );
					}
				}
				//���� ������ �ϰ� �ִ� ���� üũ,��
				else
				{
					if( g_pGameView->m_cGameViewBtnMan.m_bBCall )
					{
						OnBtnCall( false );
					}
					else if( g_pGameView->m_cGameViewBtnMan.m_bBCheck )
					{
						OnBtnCheck( false );
					}
				}

				// Ÿ�� �ƿ� ī��Ʈ ����
#ifndef _DEBUG
				m_cPlayer[ 0 ].IncTurnLimitCnt();
#endif
				g_pGameView->m_cDisplayBoard.SetText( EDBT_LIMITTIME );
			}
		}
	}	

	if ( g_Poker.nState >= RSTATE_GAME_ROUND_CHANGECARD_M && g_Poker.nState <= RSTATE_GAME_ROUND_CHANGECARD_N &&
		m_pCuttingStageInfo->bChangeStage == true && !g_ObserverMan.IsMyObserver() )
	{		
		if ( m_nCurPlayer == m_cPlayer[ 0 ].ServPNum && m_cPlayer[ 0 ].PlayState==1 && !m_cPlayer[0].bFold ) 
		{		
			int tcnt = g_pGameView->m_cPlayerDrawManager.GetTimerTimeGab() / 1000;

			//�� ���� �ð� ������ ����
			int nTimeCnt = PBTurnTimeOut( ETTT_CUTTING );
			if ( tcnt >= nTimeCnt )
			{
				if ( g_pGameView->m_cGameViewBtnMan.IsShowBtn( CGameButtonBD::BTNP_CHANGE )  
					&& g_pGameView->m_cGameViewBtnMan.IsShowBtn( CGameButtonBD::BTNP_PASS ) )
				{
					g_pGameView->m_cGameViewBtnMan.SetShowBtn( CGameButtonBD::BTNP_CHANGE, FALSE );
					g_pGameView->m_cGameViewBtnMan.SetShowBtn( CGameButtonBD::BTNP_PASS, FALSE );
					OnBtnPass();
				}
			}
		}
	}
}


void GameProcess_BD::EndWindCard()
{
	g_pGameView->m_cPlayerDrawManager.SetStartTimer();

	int nState = g_Poker.nState;
	if ( g_Poker.nState==RSTATE_STARTGAME)
	{ // ���������̴�
		// ���ý��� (ī�� �� ������ ���� ����)
		//  Ÿ�̸� ���̱�
		m_nCurPlayer = g_Poker.CurPlayer;
		g_pGameView->m_cPlayerDrawManager.SetStartTimer();

		int pn = GetPNum_ByServPN(m_nCurPlayer);
		// ������ �߰� ī�弼������ ���� �϶��� �� ǥ�� ����.
		if ( CheckRoundForCardSet() )
		{
			g_pGameView->m_cPlayerDrawManager.SetTurn(pn);
		}

		// ### [ ������� ] ###
		if(!g_ObserverMan.IsMyObserver() && m_cPlayer[0].ServPNum == m_nCurPlayer)
		{			
			m_bBtnClick = FALSE;

			// �ڵ����ýÿ��� ���ù�ư Ȱ��ȭ �����̸� ���� �����Ѵ�.
			if (g_Poker.nAutoBetCmd > 0)
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelayInAutoBet(0, 0);
			else
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelay( 0, 0 );  
		}

		// �ڽ��� ī������ �������� ������ ����
		g_pGameView->m_cPlayerDrawManager.MyJokboNameUpdate();

		//������ ���ຣ�� ���� �ʱ�ȭ 
		ReserveState(RESERVESTATE_BET_NONE);
	}	
}

//////////////////////////////////////////////////////////////////////////
/*
�޼��� ó�� 
*/
//////////////////////////////////////////////////////////////////////////

bool GameProcess_BD::Accept_CreateRoom(void *pMsgParam) // �游��� �㰡
{
	if (GetCurrentWhere()!=IDX_GAMEWHERE_LOBY) return false;

	CSV_ACCEPT_CREATEROOM *pMsg = (CSV_ACCEPT_CREATEROOM*)pMsgParam;//UGPCommon �۾�

	memcpy(&g_RI, pMsg->RI, sizeof(ROOMINFO));

	int nGame = g_RI.nGameIndex;

	ZeroMemory( &g_Poker,sizeof(POKERGAME) );

	m_cPlayer[ 0 ].SetNewPlayer(&GM().GetMyInfo()->UI);

	//���� ���� ���� ���ֱ� 	
	g_MyGameInfo.nGameMode = IDX_GAME_BD;	

	// �ʱ�ȭ
	ResetGame();
	GM().SetWaitResponse(FALSE);

	// ���� ���� ����ü�� �ʱ�ȭ �Ѵ�
	for ( int i = 1; i < MAX_PLAYER; i++ )
	{
		m_cPlayer[ i ].Clear();
	}

	m_cPlayer[ 0 ].JoinState = 0;
	m_cPlayer[ 0 ].MyIpCheck = -1;	

	m_nNo1PosPNum = 0;
	m_nMyPosPNum = 0;	 

	// ������ �÷��̾� ��ȣ ����
	for ( int i = 0; i < MAX_PLAYER; i++ )
	{ 
		m_cPlayer[ i ].ServPNum = i; 
	}

	// ### [ ������� ] ###
	memcpy( &m_cPlayer[ 0 ].UI, &GM().GetMyInfo()->UI, sizeof(GM().GetMyInfo()->UI));

	//Buy in �ƽ��� UI �Ӵϼ��� �游���
	if( g_RI.bIsMaxRoom )
	{
		//�����ӴϺ��� �ڻ��� ������� ���� �ӴϷ� �����Ѵ�.
		if( g_RI.ChipKind == 0 && g_RI.llInMoney > g_PLAYER(0)->UI.GetMoney() )
		{
			g_PLAYER(0)->UI.SetInMoney( g_PLAYER(0)->UI.GetMoney() );
			GM().GetMyInfo()->UI.SetInMoney( g_PLAYER(0)->UI.GetMoney() );
		}
		else if( g_RI.ChipKind == 1 && g_RI.llInMoney > GM().GetMyGold() )
		{
			g_PLAYER(0)->UI.SetInMoney( GM().GetMyGold() );
			GM().GetMyInfo()->UI.SetInMoney( GM().GetMyGold() );
		}
		else
		{
			g_PLAYER(0)->UI.SetInMoney( g_RI.llInMoney );
			GM().GetMyInfo()->UI.SetInMoney( g_RI.llInMoney );
		}
	}

	// ### [ ������� ] ###
	GM().GetMyInfo()->JoinState = 0;
	GM().GetMyInfo()->MyIpCheck = -1;
	GM().GetMyInfo()->ServPNum = m_cPlayer[ 0 ].ServPNum;

	g_MyPN = g_MastPN = 0;	// ���� ����

	m_nCurPlayer = 0;// ��ó�� �����ڰ� ��

	// ĳ����â �ʱ�ȭ
	if(g_RI.MaxUserNum > MAX_PLAYER) g_RI.MaxUserNum=MAX_PLAYER;
	if(g_RI.MaxUserNum < 2) g_RI.MaxUserNum=2;

	//��ư�� �ʱ�ȭ
	g_pGameView->m_cGameViewBtnMan.EnterRoom_Button_Init( g_RI.sBetRuleKind );

	//	g_pGameView->m_Roominfo.SetRoomInfo();

	g_pGameView->m_cPlayerDrawManager.OnEnterRoom();

	if(!g_ObserverMan.IsMyObserver())
	{
		LAYER_GLOBAL_AUTOBETWND()->OnEnterRoom();
	}

	g_cBanishMan.Clear();
	//UPG�۾� �߰�
	//OnEnterRoom(TRUE);

	g_pGameView->m_cGameHelper.OnEnterRoom();

	g_pGameView->m_cTitle.MakeTitle();

	g_cPromotionMan.Accept_CreateRoom();

	// ������ ���� ������ �սǸӴ� ���� �߰�.
	INT64 lossMoney = GM().GetLossMaxMoney() + GM().GetLossMoney();
	g_pGameView->m_cPlayerDrawManager.OnEnterRoomLimitMoney(0, lossMoney);

	//������ : ������ÿ� �սǱݾ� ��� ���� ���� üũ
	g_pGameView->m_cGameViewBtnMan.SetLossMoneyWarning(true) ;

	//������ : �������� �������� �����Ѵ�.
	g_pGameView->m_cGameViewBtnMan.m_bIsWaitUser = false;

	return true;
}


// �濡 ���� �㰡 ����
bool GameProcess_BD::Accept_EnterRoom(void *pMsgParam) 
{
	CSV_ACCEPT_ENTERROOM* pMsg= (CSV_ACCEPT_ENTERROOM*) pMsgParam;	//UGPCommon �۾�

	char *strMast = pMsg->EI->Ri.ID;

	g_RI.nGameIndex = pMsg->EI->Ri.nGameIndex;
	g_RI.FormKind = pMsg->EI->Ri.FormKind; // 1 : CHOICE_GAME	

	//���� ���� ���� ���ֱ� 
	g_MyGameInfo.nGameMode = IDX_GAME_BD;	

	//������ : �������� �������� �����Ѵ�.
	g_pGameView->m_cGameViewBtnMan.m_bIsWaitUser = (pMsg->EI->Ri.bWaitInRoom=='1');

	//������ : ������ÿ� �սǱݾ� ��� ���� ���� üũ
	g_pGameView->m_cGameViewBtnMan.SetLossMoneyWarning(true) ;

	m_cPlayer[ 0 ].SetNewPlayer(&GM().GetMyInfo()->UI);

	//�ƽ��� UI �Ӵϼ��� �����
	if( pMsg->EI->Ri.bIsMaxRoom )
	{
		//�����ӴϺ��� �ڻ��� ������� ���� �ӴϷ� �����Ѵ�.
		if( pMsg->EI->Ri.ChipKind == 0 && pMsg->EI->Ri.llInMoney > g_PLAYER(0)->UI.GetMoney() )
		{
			g_PLAYER(0)->UI.SetInMoney( g_PLAYER(0)->UI.GetMoney() );
			GM().GetMyInfo()->UI.SetInMoney( g_PLAYER(0)->UI.GetMoney() );
		}
		else if( pMsg->EI->Ri.ChipKind == 1 && pMsg->EI->Ri.llInMoney > GM().GetMyGold() )
		{
			g_PLAYER(0)->UI.SetInMoney( GM().GetMyGold()  );
			GM().GetMyInfo()->UI.SetInMoney( GM().GetMyGold()  );
		}
		else
		{
			g_PLAYER(0)->UI.SetInMoney( pMsg->EI->Ri.llInMoney );
			GM().GetMyInfo()->UI.SetInMoney( pMsg->EI->Ri.llInMoney );
		}
	}

	g_MyPN = 0;
	int MyID=-1, MastID=-1;
	int i = 0;
	int k = 0;	

	//	g_MyObserverStep = 0;

	// ### [ ������� ] ###
	for( i = 0; i < GetMaxNewPlayer(); i++ ) 
	{
		if ( strlen( pMsg->EI->Ui[ i ].ID ) > 0 ) 
		{
			if ( strcmp( strMast, pMsg->EI->Ui[ i ].ID)==0) MastID = i; // ���� ���̵� ã��
			if ( strcmp( GM().GetMyInfo()->UI.ID, pMsg->EI->Ui[ i ].ID ) == 0 ) MyID = i; // ���� ���̵�
		}
	}
	GM().SetWaitResponse(FALSE);

	// ���� Ȥ�� ���� ���̵� ã�� �� ���ų� ����� �� ���̵� �����ϴ�?
	if(MastID == -1 || MyID == -1 || MastID == MyID) {
		// ### [ ������� ] ###
		CSV_ASK_OUTROOM aormsg;
		aormsg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID, REASON_OUTROOM_ABNORMAL);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), aormsg.pData, aormsg.GetTotalSize());
		// ��������
		g_MastPN = 0;
		CloseMainMessageDlg(); // ��û�� �濡 �������Դϴ� �޽��� �ݱ�
		return false;
	}

	GM().GetMyInfo()->SetChangeRoomInfo(pMsg->EI->Cui[MyID]);
	if(pMsg->EI->Ui[MyID].cObserver == 1)  { g_ObserverMan.SetMyObserver(TRUE); }

	g_pGameView->m_cPlayerDrawManager.OnEnterRoom();

	ResetGame();
	g_MastPN = MastID;	
	// �� ���� ����
	memcpy(&g_RI, &pMsg->EI->Ri, sizeof(ROOMINFO));	
	if(g_RI.State == 1)
	{
		g_pGameView->m_cTotalBetMoney.Start();
	}

	if(g_RI.MaxUserNum > MAX_PLAYER) g_RI.MaxUserNum=MAX_PLAYER;
	if(g_RI.MaxUserNum < 2) g_RI.MaxUserNum = 2;//2,3,4,5

	// ���� �޼��� ��ȭâ �ݱ�
	CloseMainMessageDlg(); // ��û�� �濡 �������Դϴ� �޽��� �ݱ�

	ZeroMemory(&g_Poker,sizeof(g_Poker));
	memcpy(&g_Poker, &pMsg->EI->Pg,sizeof(g_Poker));

	if( !g_ObserverMan.IsMyObserver() ) // ������ �ƴ϶��
	{
		m_cPlayer[ 0 ].JoinState = 0;
		GM().GetMyInfo()->JoinState = 0;
		m_nMyPosPNum = MyID; // �����ʿ� �ε���
		m_nNo1PosPNum = GetPNum_ByServPN(0); // �������� 0 �� �ε���		
		// ���� ���� �ʱ�ȭ
		for ( i = 1; i < GetMaxNewPlayer(); i++ )
		{
			m_cPlayer[ i ].Clear();
		}

		// ������ �÷��̾� ��ȣ ����
		for ( i = 0; i < GetMaxNewPlayer(); i++ ) 
		{
			if ( i < MAX_PLAYER )
			{
				m_cPlayer[ i ].ServPNum = GetServPNum_ByPN(i);
			}
			else
			{
				m_cPlayer[ i ].ServPNum = i;
			}
		}

		GM().GetMyInfo()->ServPNum = m_cPlayer[ 0 ].ServPNum;	

		// �÷��̾� ������ �°� �ʱ�ȭ�Ѵ�
		for( i = 0; i < GetMaxNewPlayer(); i++ ) 
		{
			if ( i >= MAX_PLAYER ) 
			{ //������ �Ƶ����
				if ( strlen( pMsg->EI->Ui[ i ].ID ) > 0 ) 
				{
					m_cPlayer[ i ].SetNewPlayer( &pMsg->EI->Ui[ i ] );
					if ( pMsg->EI->Ui[ i ].AdminGrade == 0 ) 
					{
						// ������ ����Ʈ ���
						int ounum = m_cPlayer[ i ].UI.UNum;
						int sex  = m_cPlayer[ i ].UI.Sex;
						int level = m_cPlayer[ i ].UI.nIcon;
						/*INT64 Money = m_cPlayer[ i ].UI.GetMoney(IsTournament());*/
						INT64 Money = m_cPlayer[ i ].UI.GetMoney();
						int seq = 0;
						m_pObserverDlg->User_InsItem(ounum, m_cPlayer[ i ].UI.ID, m_cPlayer[ i ].UI.NickName, sex, level, Money, seq);
					}
				}
				continue;
			}

			int pn = GetPNum_ByServPN( i );
			if ( strlen( pMsg->EI->Ui[ i ].ID ) > 0 ) 
			{
				if ( pn != 0 )
				{
					m_cPlayer[ pn ].SetNewPlayer(&pMsg->EI->Ui[i]);	
				}

				m_cPlayer[ pn ].JoinState = pMsg->EI->Pg.PS[i].JoinState;
				m_cPlayer[ pn ].PlayState = pMsg->EI->Pg.PS[i].PlayState;
				m_cPlayer[ pn ].MyIpCheck = pMsg->EI->Pg.PS[i].nCheckIp;   // ### [ �ߺ��� IP�� ] ###
				m_cPlayer[ pn ].nSndFxKind = pMsg->EI->Pg.PS[i].nSndFxKind; // ### [���� �߰� �۾�] ###
				/*m_cPlayer[ pn ].UI.nIcon = GetMoneyGrade(pMsg->EI->Ui[i].GetTotalMoney(IsTournament()));*/
				m_cPlayer[ pn ].UI.nIcon = GetMoneyGrade(pMsg->EI->Ui[i].GetTotalMoney());
				m_cPlayer[ pn ].SetChangeRoomInfo(pMsg->EI->Cui[i]);
			}
		}
		// ��ư ���� ����
		m_nCurPlayer = g_Poker.CurPlayer;
		g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();
	}
	else //����
	{
		// ���� Ŭ���̾�Ʈ 1 : 1
		//g_MyIndex = MyID; ����
		m_nMyPosPNum = 0;						    // �����ʿ� �ε���
		m_nNo1PosPNum = GetPNum_ByServPN(0); // �������� 0 �� �ε���
		for(i=0; i<GetMaxNewPlayer(); i++) {
			m_cPlayer[ i ].Clear(); // ���� ���� �ʱ�ȭ
			m_cPlayer[ i ].ServPNum = i;
		}
		GM().GetMyInfo()->ServPNum = MyID;

		for(i=0; i<GetMaxNewPlayer(); i++) {		
			int pn = i;
			if(strlen(pMsg->EI->Ui[i].ID) > 0)	{
				m_cPlayer[ pn ].SetNewPlayer(&pMsg->EI->Ui[i]);
				if(i>=MAX_PLAYER) // ������ ����Ʈ ���
				{
					if(pMsg->EI->Ui[i].AdminGrade == 0) 
					{
						int ounum = m_cPlayer[ i ].UI.UNum;
						int sex  = m_cPlayer[ i ].UI.Sex;
						int level = m_cPlayer[ i ].UI.nIcon;
						/*INT64 Money = m_cPlayer[ i ].UI.GetMoney(IsTournament());*/
						INT64 Money = m_cPlayer[ i ].UI.GetMoney();
						int seq = 0;
						m_pObserverDlg->User_InsItem(ounum,m_cPlayer[ i ].UI.ID, m_cPlayer[ i ].UI.NickName, sex, level,Money, seq);
					}
					/*m_cPlayer[ pn ].UI.nIcon = GetMoneyGrade(pMsg->EI->Ui[i].GetTotalMoney(IsTournament()));*/
					m_cPlayer[ pn ].UI.nIcon = GetMoneyGrade(pMsg->EI->Ui[i].GetTotalMoney());
					m_cPlayer[ pn ].SetChangeRoomInfo(pMsg->EI->Cui[i]);
					continue;
				}

				m_cPlayer[ pn ].JoinState = pMsg->EI->Pg.PS[i].JoinState;
				m_cPlayer[ pn ].PlayState = pMsg->EI->Pg.PS[i].PlayState;
				m_cPlayer[ pn ].MyIpCheck = pMsg->EI->Pg.PS[i].nCheckIp; // ### [ �ߺ��� IP�� ] ###
				m_cPlayer[ pn ].nSndFxKind = pMsg->EI->Pg.PS[i].nSndFxKind;// ### [���� �߰� �۾�] ###
				/*m_cPlayer[ pn ].UI.nIcon = GetMoneyGrade(pMsg->EI->Ui[i].GetTotalMoney(IsTournament()));*/
				m_cPlayer[ pn ].UI.nIcon = GetMoneyGrade(pMsg->EI->Ui[i].GetTotalMoney());
				m_cPlayer[ pn ].SetChangeRoomInfo(pMsg->EI->Cui[i]);
			}
		}

		// ��ư ���� ����
		m_nCurPlayer = g_Poker.CurPlayer;
		g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();
		// �������

		//������ �̹��� �׸���


		//����UI
		g_ObserverMan.OnObserve();
	}

	if(!g_Poker.bDistCard && g_Poker.nAutoBetCmd <=0 && g_RI.State == 1 && g_Poker.nRound >= 1)
	{
		// ������ �߰� ī�弼������ ���� �϶��� �� ǥ�� ����.
		if ( CheckRoundForCardSet() )
		{
			g_pGameView->m_cPlayerDrawManager.SetTurn(GetPNum_ByServPN(m_nCurPlayer));
		}		
	}

	m_cAllBetChip.SetMoney(GetRealBet());	
	//��ư�� �ʱ�ȭ
	g_pGameView->m_cGameViewBtnMan.EnterRoom_Button_Init( g_RI.sBetRuleKind );

	// ������� ����
	if(!g_ObserverMan.IsMyObserver())
	{
		LAYER_GLOBAL_AUTOBETWND()->OnEnterRoom();
	}
	else
	{

	}

	g_pGameView->m_cGameHelper.OnEnterRoom();

	g_pGameView->m_cSidePotMoney.Check();
	g_pGameView->m_cTitle.MakeTitle();	

	// �÷��̾� ������ �°� ī�� �ʱ�ȭ�Ѵ�
	for ( i = 0; i < MAX_PLAYER; i++ )
	{
		int pn = GetPNum_ByServPN( i );
		if ( strlen(pMsg->EI->Ui[ i ].ID) > 0 ) 
		{
			// ���� ���� ��Ȳ �׸��� ī�� ������ ī�嵦���ٰ� ��
			if ( m_cPlayer[pn].JoinState == 1 ) 
			{
				int nTotCnt = pMsg->EI->Pg.PS[i].nCardTotal;
				int nOpCnt =  pMsg->EI->Pg.PS[i].nOpenTotal;

				m_cPlayer[pn].bFold = pMsg->EI->Pg.PS[i].bFold;
				m_cPlayer[pn].m_nCardTotal = nTotCnt;
				m_cPlayer[pn].m_nOpenTotal = nOpCnt;

				for ( k = 0; k < 4; k++ ) 
				{
					m_cPlayer[ pn ].SetNewCard( DUMY_CARD ); // ī��Ʈ �ڵ�����
					CPoint pt;
					pt.x = m_cPlayer[ pn ].m_cMyCard[ k ].m_Xp;
					pt.y = m_cPlayer[ pn ].m_cMyCard[ k ].m_Yp;
					m_cPlayer[ pn ].m_cMyCard[ k ].SetMovingCurPos( pt );
					m_cPlayer[ pn ].ShowCard(k);
				}

				if ( m_cPlayer[ pn ].bFold == TRUE && pn != 0 ) // ���� ������� ��Ӱ�
				{
					m_cPlayer[ pn ].FoldUser_CardClose();
				}

				//Ŀ������
				m_cPlayer[ pn ].GetCutInfo().Clear();
				for ( int k = 0; k < 3; k++ )
				{
					if ( pMsg->EI->Pg.PS[ i ].arrCuttingNum[ k ] != -1 )
					{
						m_cPlayer[ pn ].GetCutInfo().m_arrbShow[ k ] = true;
						m_cPlayer[ pn ].GetCutInfo().m_arrCutCount[ k ] = pMsg->EI->Pg.PS[ i ].arrCuttingNum[ k ];
					}
				}
			}
		}
	}

	if ( g_RI.State == 1 )
	{
		effect::CCuttingStageIndicateEffect* pEffect = NULL;
		SAFE_NEW( pEffect, effect::CCuttingStageIndicateEffect);

		pEffect->Init();
		g_pGameView->m_cAniSprListManager.ADDObject( pEffect );
	}

	g_cPromotionMan.Accept_EnterRoom();

	GM().GetCurrentGame()->GetGameView()->SetFocus();


	//����ε� �������̶�� ǥ��
	if(g_RI.FormKind == 5)
	{
		if(pMsg->EI->Pg.nRound <= 1 && pMsg->EI->Pg.nState <= 4)
		{
			BlindBetEffectDraw();
		}
	}

	return true;
}

// ������ �濡 ���� �˸�
void GameProcess_BD::User_EnterRoom(void *pMsgParam)
{
	CSV_USERENTERROOM* pMsg= (CSV_USERENTERROOM*) pMsgParam;//UGPCommon �۾�
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) return;

	PBPlayEffectSound(SND_USERIN);
	int i=0;
	int pnum = -1; // �÷��̾� ��ȣ

	if( !g_ObserverMan.IsMyObserver() && pMsg->UI->cObserver!=1 ) // 1) ���� ���������̰� ������ �� ����
	{
		for ( i = m_nNo1PosPNum; i < MAX_PLAYER + m_nNo1PosPNum; i++ ) 
		{// �������� 0���ε��� Ŭ���̾�Ʈ �ε��� ����
			int index = i;
			if(index>=MAX_PLAYER) index = index - MAX_PLAYER;
			if(index==0) continue; //��
			if(strlen( m_cPlayer[ index ].UI.ID)==0){pnum=index; break;}
		}

		if(pnum == -1) return; // ����ִ� ���� ã�� ��������

		// �� ������ �����ϰ� ���� ������ �÷��̾��� ������ ����
		g_RI.NowUserNum += 1;
		m_cPlayer[ pnum ].SetNewPlayer(pMsg->UI);

		m_cPlayer[ pnum ].ServPNum = GetServPNum_ByPN(pnum);	// ������ �÷��̾� ��ȣ ����
		g_pGameView->m_cPlayerDrawManager.OnEnterRoom(pnum);

		// ������ ���� ������ �սǸӴ� ���� �߰�.
		g_pGameView->m_cPlayerDrawManager.OnEnterRoomLimitMoney(pnum, *pMsg->llMaxLossMoney);

		CString str;
		str.Format("������: [%s]��\n", pMsg->UI->NickName);
		AddGameInfoViewEdit( str, GetChatColor( ECC2_NOTIFY ) );
	}	
	// 3)���� �������̰�  ������ �ϴ³� ����
	else if(pMsg->UI->cObserver!=1)
	{
		for(i=0; i<MAX_PLAYER; i++) {
			if(strlen(m_cPlayer[ i ].UI.ID)==0){pnum=i; break;}
		}
		if(pnum == -1) return; // ����ִ� ���� ã�� ��������

		m_nMyPosPNum = 0; // �����ʿ� �ε���
		m_nNo1PosPNum = GetPNum_ByServPN(0); // �������� 0 �� �ε���

		// �� ������ �����ϰ� ���� ������ �÷��̾��� ������ ����
		g_RI.NowUserNum += 1;
		m_cPlayer[ pnum ].SetNewPlayer(pMsg->UI);	
		m_cPlayer[ pnum ].ServPNum = GetServPNum_ByPN(pnum);// ������ �÷��̾� ��ȣ ����
		g_pGameView->m_cPlayerDrawManager.OnEnterRoom(pnum);

		// ������ ���� ������ �սǸӴ� ���� �߰�.
		g_pGameView->m_cPlayerDrawManager.OnEnterRoomLimitMoney(pnum, *pMsg->llMaxLossMoney);	

		CString str;
		str.Format("������: [%s]��\n", pMsg->UI->NickName);
		AddGameInfoViewEdit( str, GetChatColor( ECC2_NOTIFY ) );

	}	
	// 2)���� �����̰� �����γ� ����
	// 4)���� ���� �̰� �����γ� ����
	else
	{
		if(pMsg->UI->AdminGrade >0) {
			pnum = GetMaxNewPlayer()-1;//������
		}
		else {
			for(i=MAX_PLAYER; i<GetMaxNewPlayer()-1; i++) {
				if(strlen(m_cPlayer[ i ].UI.ID)==0){pnum=i; break;}
			}
		}
		if(pnum == -1) return;			

		m_cPlayer[ pnum ].SetNewPlayer(pMsg->UI);// �� ������ �����ϰ� ���� ������ �÷��̾��� ������ ����
		m_cPlayer[ pnum ].ServPNum = pnum;		// ������ �÷��̾� ��ȣ ����

		if( pMsg->UI->AdminGrade==0) 
		{// �����ڰ� �ƴϸ� 
			g_RI.NowObserverNum += 1;

			CString str;
			str.Format("������ ����: [%s]��\n", pMsg->UI->NickName);
			AddGameInfoViewEdit( str, GetChatColor(ECC2_NOTIFY));

			// ������ ����Ʈ ���
			int ounum = m_cPlayer[ pnum ].UI.UNum;
			int sex  = m_cPlayer[ pnum ].UI.Sex;
			int level = m_cPlayer[ pnum ].UI.nIcon;
			/*INT64 Money = m_cPlayer[ pnum ].UI.GetMoney(IsTournament());*/
			INT64 Money = m_cPlayer[ pnum ].UI.GetMoney();
			int seq = 0;
			if ( NULL != m_pObserverDlg )
			{
				m_pObserverDlg->User_InsItem(ounum,m_cPlayer[ pnum ].UI.ID, m_cPlayer[ pnum ].UI.NickName, sex, level, Money, seq);
			}
		}
	}

	// ### [ �ߺ��� IP�� ] ###
	for(i=0; i<MAX_PLAYER; i++) {
		int pn = GetPNum_ByServPN(i);
		if(strlen(m_cPlayer[ pn ].UI.ID) > 0) {
			m_cPlayer[ pn ].MyIpCheck = pMsg->IPA->IpIndex[i];
		}
	}

	g_pGameView->m_cTitle.MakeTitle();
	m_cPlayer[ pnum ].nSndFxKind = *pMsg->SndFxKind;// ### [���� �߰� �۾�] ###

	// �����ǿ� ���� ���
	if ( !g_ObserverMan.IsMyObserver() )
	{
		CheckMinimumUser();
	}

	//< CYS 101221
	if ( g_RI.NowUserNum >= g_RI.MaxUserNum ) 
	{
		g_pGameView->m_cGameViewBtnMan.SetEnableBtn( CGameButtonBD::BTN_START, FALSE );
		g_pGameView->m_cGameViewBtnMan.SetShowBtn( CGameButtonBD::BTN_START, FALSE );
	}
	//> CYS 101221
}

void GameProcess_BD::User_OutRoom(void *pMsgParam) // ������ �濡�� ���� �˸�
{
	CSV_USEROUTROOM* pMsg = (CSV_USEROUTROOM*) pMsgParam;//UGPCommon �۾�
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) return;

	PBPlayEffectSound(SND_USEROUT);
	int pnum=-1;
	for(int i=0; i<GetMaxNewPlayer(); i++) {
		if(strcmp(m_cPlayer[ i ].UI.ID, pMsg->ID)==0) {pnum=i; break;}
	}
	if(pnum == -1) return;

	// �г��Ӽ��� 
	CString NickName =  m_cPlayer[ pnum ].UI.NickName;

	if( pnum<MAX_PLAYER ) //�����γ�
	{
		// [ ī�� ������ ]
		if(m_cPlayer[ pnum ].bFold == FALSE && m_cPlayer[ pnum ].JoinState == 1 && m_cPlayer[ pnum ].PlayState == 1)
		{	//���� ������ �� ������.
			m_nStartUserNum--;
			if(m_nStartUserNum <0) m_nStartUserNum = 0;
		}


		m_cPlayer[ pnum ].Clear(); // UI �ֱ�ȭ		
		g_pGameView->m_cPlayerDrawManager.OnExitRoom(pnum);

		if(g_RI.NowUserNum > 0) g_RI.NowUserNum -= 1;

		CString str;		
		str.Format("������: [%s]��\n", NickName );
		AddGameInfoViewEdit( str, GetChatColor( ECC2_NOTIFY ) );

		// �г��Ӽ��� 	2006/10/30
		str.Format("## <ID: %s(%s)>�� ���� ##\n",pMsg->ID , NickName );
		//UPG�۾� �� �� ��ü
		AddLobbyHideChatText(&str);
	}
	else // �����γ�
	{
		if(m_cPlayer[ pnum ].UI.AdminGrade == 0) 
		{
			if ( NULL != m_pObserverDlg )
			{
				m_pObserverDlg->User_DelItem(m_cPlayer[ pnum ].UI.UNum, m_cPlayer[ pnum ].UI.ID);
			}

			if(g_RI.NowObserverNum > 0) g_RI.NowObserverNum -= 1;

			CString str;
			str.Format("������ ����:[%s]��\n", GetPlayer(pnum)->UI.NickName);
			AddGameInfoViewEdit( str, GetChatColor( ECC2_NOTIFY ) );
		}

		m_cPlayer[ pnum ].Clear(); // UI �ֱ�ȭ
		//	NMBASE::SOUND::g_Sound.PlayWav(SND_FX_2);
	}
	// ### [ �ߺ��� IP�� ] ###
	for(int i=0; i<MAX_PLAYER; i++)	{
		int pn = GetPNum_ByServPN(i);
		if(strlen(m_cPlayer[ pn ].UI.ID) > 0) {
			m_cPlayer[ pn ].MyIpCheck = pMsg->IPA->IpIndex[i];
		}
	}

	g_pGameView->m_cTitle.MakeTitle();

	// �����ǿ� ���� ���
	if ( !g_ObserverMan.IsMyObserver() )
	{
		CheckMinimumUser();
	}
}


void GameProcess_BD::DoPrepareGame(STARTINFO *pSC)
{

#ifdef _DEBUG
	set_GameSpeed();
#endif
	//	g_pGameDlg->ClearResetGameTimer();
	g_RI.State = 1;

	//���̽� ���� ���� ���� ���� �Ҷ� �� ���� �Ѵ�.
	g_fRaceBetRatio = (float) pSC->fRaceBetRatio;

	if( g_RI.CurSeedMoney != pSC->nSeedMoney ){
		g_RI.CurSeedMoney = pSC->nSeedMoney;		
		g_pGameView->m_cTitle.MakeTitle();
	}

	// ����, Ŭ���̾�Ʈ ���� �����Ͽ� ������ ����Ѵ�.
	ZeroMemory(&g_Poker, sizeof(g_Poker));
	memcpy(g_Poker.PS, pSC->Ps, sizeof(PLAYSTATE)*MAX_PLAYER);	


	//������ : ���� �ƽ� Ȱ�� ���� �ʱ�ȭ : �ƽ�����Ʈ
	for (int i=0; i<MAX_PLAYER; i++)
	{
		GM().g_bFirstMax[i] = false;
	}


	//������ : �����ư Ȱ��ȭ ���ɻ���
	ReserveState(RESERVESTATE_BET_NONE);
	CanReserve(true);
	g_nCardTotalCount = 0 ; // ī�� ���� �񱳰��� 0���� �ʱ�ȭ.

	g_Poker.nBossPNum = -1;	
	g_Poker.nImageBoss = -1;

	//���ӽ��۽�
	g_RI.RoomMasterType = pSC->RoomMasterType;

	// ���� ���� �÷��� ����
	m_bGameStart = TRUE;	
	for(int i=0; i<MAX_PLAYER; i++) 
	{		
		// �÷��̾� ��ũ�� �ʱ�ȭ
		m_cPlayer[ i ].Reset();
	}

	g_pGameView->m_cPlayerDrawManager.Reset();


	g_Poker.nState = RSTATE_PREPARESTART; // ������ �����̳�
	g_Poker.nDistUser = pSC->nDistUser;
	m_nCurPlayer = pSC->nDistUser;	

	g_DebugStartTick = timeGetTime();

	//�ڸ� ��Ŵ��
	g_SeatKeeper.OnStartGame();	

	PBPlayEffectSound(SND_START);

	BOOL bFalse = FALSE;
	m_nStartUserNum = 0;
	INT64 BetMoney = (INT64)((double)g_RI.CurSeedMoney * (double)pSC->fBetMoneyMulti);
	INT64 tbetmoney = 0;
	for(int i=0; i<MAX_PLAYER; i++) 
	{		
		if(strlen(pSC->JoinID[i]) >0)
		{
			int pn = GetPNum_ByServPN(i);
			if(strcmp(m_cPlayer[pn].UI.ID, pSC->JoinID[i]) != 0) { bFalse = TRUE; break; }

			// �÷��̾� ���� ����
			m_cPlayer[pn].JoinState = pSC->Ps[i].JoinState; //
			m_cPlayer[pn].PlayState = pSC->Ps[i].PlayState;
			m_cPlayer[pn].bFold     = FALSE;

			//m_cPlayer[pn].UI.SetMoney(m_cPlayer[pn].UI.GetMoney(IsTournament()) - BetMoney, IsTournament());//100;
			//g_Poker.RU[i].nPMoney = m_cPlayer[pn].UI.GetMoney(IsTournament());

			//MAX�� ó��.
			INT64 i64TempBetMoney = m_cPlayer[pn].GetNowMoney() - BetMoney;
			m_cPlayer[pn].SetNowMoney( i64TempBetMoney );

			g_Poker.RU[i].nPMoney = m_cPlayer[pn].GetNowMoney();
			
			m_cPlayer[pn].nSndFxKind = pSC->Ps[i].nSndFxKind;

			//�����丮 ���� �Ӵ� ���� 
			if(!g_ObserverMan.IsMyObserver() && m_cPlayer[0].JoinState == EGAME_JOININ && m_cPlayer[0].PlayState == EPLAYER_GAMEPLAYING && pn == 0 )
			{		
				/*m_cPlayer[0].nRoundPrevMoney = m_cPlayer[0].UI.GetMoney(IsTournament());*/
				m_cPlayer[0].nRoundPrevMoney = m_cPlayer[0].UI.GetMoney();
				//m_cPlayer[0].nRoundPrevMoney = m_cPlayer[0].GetNowMoney();
			}
			//yoo
			if(1 == pSC->Ps[i].nState){
				g_pGameView->m_cPlayerDrawManager.SetLimitMoney(pn, true);
			}

			// ���� ī�� ���� 3��
			if( pn == 0)
			{
				// ### [ ������� ] ###
				if( !g_ObserverMan.IsMyObserver() )
				{
					GM().GetMyInfo()->JoinState = m_cPlayer[0].JoinState;
					GM().GetMyInfo()->PlayState = m_cPlayer[0].PlayState;
					GM().GetMyInfo()->bFold = m_cPlayer[0].bFold;
					GM().GetMyInfo()->ServPNum = m_cPlayer[0].ServPNum;
				}

				m_cPlayer[0].SetNewCard( pSC->nCard[0]);
				m_cPlayer[0].SetNewCard( pSC->nCard[1]);
				m_cPlayer[0].SetNewCard( pSC->nCard[2]);				
				m_cPlayer[0].SetNewCard( pSC->nCard[3]);

			}
			else
			{
				m_cPlayer[pn].SetNewCard( DUMY_CARD); // 53
				m_cPlayer[pn].SetNewCard( DUMY_CARD);
				m_cPlayer[pn].SetNewCard( DUMY_CARD);				
				m_cPlayer[pn].SetNewCard( DUMY_CARD);

			}

			m_cPlayer[pn].m_nCardTotal = 4;			

			m_cPlayer[pn].m_nOpenTotal = 0; 
			g_Poker.RU[i].bInUse = TRUE;
			g_Poker.RU[i].nRealBat = BetMoney;

			// �⺻ ���� Ĩ�� ����
			chip_draw_func(pn, g_Poker.RU[i].nRealBat); // ���⼭�� 10�� ������!!
			tbetmoney += g_Poker.RU[i].nRealBat;
			m_nStartUserNum++;
			CString playerID;
			playerID.Format("%d %s\r\n", i, m_cPlayer[pn].UI.ID);
			NMBASE::SOCK::CSockMan::m_sSocketLogTxt.append(playerID.GetString());
		}
	}	

	g_cPromotionMan.OnStartGame();

	g_cVipJackpot.OnStartGame();

	//g_pGameView->m_sprPublicTotalMoney.SetMoney( tbetmoney, true );
	m_cAllBetChip.SetMoney(tbetmoney);	


	if(bFalse == TRUE) {
		// ���� ó��
		CString st[MAX_PLAYER], str;
		for(int i=0; i<MAX_PLAYER; i++) 
		{
			int pn = GetPNum_ByServPN(i);
			st[i].Format("%d: %s, %s\n",i, m_cPlayer[pn].UI.NickName, pSC->JoinID[i]);
			str += st[i];
		}
		//UGP�۾�
		//g_pGameDlg->ExitGame();
		ExitGameRequest();

		AfxMessageBox("�߸��� ���� ���������� ���Ͽ� �����մϴ�");
		AfxMessageBox(str);
		return;
	}	

	Card_Distribute();
	//��� �̹��� ��ȭ

	g_pGameView->m_cGameHelper.SetAllUserCardInfo();

	effect::CCuttingStageIndicateEffect* pEffect = NULL;
	SAFE_NEW( pEffect, effect::CCuttingStageIndicateEffect);

	pEffect->Init();
	g_pGameView->m_cAniSprListManager.ADDObject( pEffect );
}

void GameProcess_BD::ShowCard(char* nCard)
{
	m_cPlayer[0].SetCardNo( 0, nCard[0]);
	m_cPlayer[0].SetCardNo( 1, nCard[1]);
	m_cPlayer[0].SetCardNo( 2, nCard[2]);				
	m_cPlayer[0].SetCardNo( 3, nCard[3]);
}


void GameProcess_BD::Card_Distribute()
{
	//m_nCurPlayer = -1;
	if( !IsValid() ) return;
	if( g_Poker.nState == 0 )return;

	int nPlayNum = GetPNum_ByServPN( g_Poker.nDistUser ); // ī�� ������ ����
	if (nPlayNum < 0 || nPlayNum >= MAX_PLAYER) return;	

	POINT dest_pos;
	CPoint start_pos(FLY_CARD_POS_X,FLY_CARD_POS_Y);

	//����ī�� ���� �������� �ʴ´�.
	for ( int x = 0 ; x < MAX_PLAYER ; x ++ )
	{
		m_cPlayer[x].m_bShowValue = false;
	}	 

	NCARDMOVEDATA data;	
	data.Clear();

	g_pGameView->m_cPlayerDrawManager.SetFlyWindCard( true );

	// ó�� 4���� �ִϸ��̼� Scene���� �׸���
	const WORD wFirstSecond_UserDelay = ( int )g_cUIData.GetFloatBD( ui::EFLOAT_F4_USER_DELAY );   // ó�� 4�� ī�� �̵����( ������ ���� )
	const WORD wFirstSecond_CardDelay = ( int )g_cUIData.GetFloatBD( ui::EFLOAT_F4_CARD_DELAY );   // ó�� 4�� ī�� �̵����( ī��� ī�� )

	{		
		int nCountIndex = 1; // ī�� �����ִ� �������

		for ( int j = 0; j < TOTAL_CARD_BD; ++j)	 
		{
			for (int i = nPlayNum; i < (MAX_PLAYER + nPlayNum); ++i) 
			{
				int nIndex = i;
				if ( nIndex >= MAX_PLAYER )
				{
					nIndex = nIndex - MAX_PLAYER;
				}

				if ( strlen( m_cPlayer[ nIndex ].UI.ID) > 0 && m_cPlayer[ nIndex ].JoinState == EGAME_JOININ && m_cPlayer[ nIndex ].PlayState == EPLAYER_GAMEPLAYING)
				{
					nCountIndex++;

					CNMyCard* pCard = &m_cPlayer[ nIndex ].m_cMyCard[ j ];
					if ( pCard != NULL )
					{
						data.Clear();						

						dest_pos.x			= pCard->m_Xp;					
						dest_pos.y			= pCard->m_Yp;					

						data.m_nPlayNum		= nIndex;
						data.m_nIndex		= j;					

						data.m_fSpeed		  = g_cUIData.GetFloatBD( ui::EFLOAT_F4_SPEED );
						data.m_ptStartPos_2nd = start_pos;
						data.m_ptDestPos_2nd  = dest_pos;
						data.m_nSecondCmd	  = EVENT_BD_PREPARESTART_ONEFLY_END;
						data.m_nDelay		  = (  nCountIndex * wFirstSecond_UserDelay  ) + ( wFirstSecond_CardDelay * j );	 		

#ifdef UPGRADE_20120502_TIMING_TEST_CODE
						//#ifdef _DEBUG
						data.m_nDelay		  = (  nCountIndex * wFirstSecond_UserDelay  ) + ( GetPrivateProfileInt("LowBD", "BetDelayFrameNum", wFirstSecond_CardDelay, ".\\TimingTest.ini")  * j );	 		
						//#endif //_DEBUG
#endif //UPGRADE_20120502_TIMING_TEST_CODE

						CNMovingCard *pMoveCard = NULL;
						SAFE_NEW( pMoveCard, CNMovingCard );

						// ��ü ī�� ������ 
						// ó�� 4���� ���ÿ� Move ������Ʈ �����Ѵ� ����ó���ؾ���
						int nDelay = ( int )g_cUIData.GetFloatBD( ui::EFLOAT_F4_TOTAL_CARD_DELAY );   // ó�� 4�� ī�� �̵��� ���ð�

						pMoveCard->Initialize( &data, 1, nDelay, DIRECT_MOVE_DOWN, true, ( PFN_CARDEVENT ) CardEventCB );//ó�� ī�� �ٶ� �ִϻ��
						pMoveCard->nPNum = nIndex;
						pMoveCard->SetLastCMD( EVENT_BD_PREPARESTART_FOURFLY_END ); // ������ ���� ī�忡 �̺�Ʈ ��� 
						pMoveCard->SetLastCMD_User( nIndex );
						m_pMovingCardMgr->ADDCardF4( pMoveCard ); // ����� ���� Ŀ�ǵ� ��� �����.
					}
				}
			}
		}

		GameProcess_BD::SetWindCardEnd();
	}	
}

bool GameProcess_BD::Raise_InfoAndSound(INT64 llLastPlayerBetMoney)
{
	// ##### [ ���� â�� �޼��� && ���� ���] #####

	CPoint cpEffectPos=CPoint(0, 0);
	if ( g_Poker.LastPlayer >= 0 && g_Poker.LastPlayer < MAX_PLAYER )
	{
		int LastPnum = g_Poker.LastPlayer;
		int nSNum = GetPNum_ByServPN( LastPnum );
		if ( g_Poker.RU[ LastPnum ].nLastCmd > 0 && nSNum >= 0 && nSNum < MAX_PLAYER) 
		{				
			// ����� Ÿ�ӹ� �����
			int nLastCmd = g_Poker.RU[ LastPnum ].nLastCmd;

			//���� �̹����� �� ������� �����ɷ� �ٲ� �ְ� �� �����.
			g_pGameView->m_cAniSprListManager.BetCmd_ChangeCmd( nSNum );

			if (nLastCmd<RAISE_LAST_CMD_LIMIT_1X) //����Ʈ �����ϰ�쿡�� �Ʒ��ʿ��� ȣ��ȴ�.
			{
				g_pGameView->m_cPlayerDrawManager.SetBettingMoney(nSNum, nLastCmd, llLastPlayerBetMoney);
			}

			if(2==g_Poker.RU[LastPnum].bMaxBet){
				g_pGameView->m_cPlayerDrawManager.SetLimitMoney(nSNum, true);
			}

			//������ : �ƽ� Ŀ�ǵ� �߰� : �ƽ�����Ʈ
			if (g_Poker.RU[LastPnum].bMaxBet && GM().g_bFirstMax[LastPnum] == false)
			{
				g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);

				if ( m_cPlayer[ nSNum ].GetNowMoney() == 0  )
				{
					g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
				}

				// �ƽ�����Ʈ
				effect::CBetCmdEffect *pObj = NULL;
				SAFE_NEW( pObj, effect::CBetCmdEffect );

				pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_MAX);		
				cpEffectPos=pObj->GetEffectPos();		
				g_pGameView->m_cAniSprListManager.ADDObject(pObj);
				g_cVipJackpot.OnBetGame(cpEffectPos, false);

				if ( g_Poker.nRound == 4)
				{
					SoundFxKindBig(nLastCmd,g_PLAYER(nSNum)->UI.Sex, true);
				}
				else
				{
					SoundFxKind(nLastCmd,g_PLAYER(nSNum)->UI.Sex, true);
				}
				GM().g_bFirstMax[LastPnum] = true;
				//�Ϲ� ���� Ŀ�ǵ�
			}else{

				switch( nLastCmd )
				{
				case RAISE_LAST_CMD_FOLD: // foldUser ���� ó��
					{
						effect::CBetCmdEffect *pObj = NULL;
						SAFE_NEW( pObj, effect::CBetCmdEffect );

						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_DIE);		
						cpEffectPos=pObj->GetEffectPos();	
						g_cVipJackpot.OnBetGame(cpEffectPos, false);	
						g_pGameView->m_cAniSprListManager.ADDObject(pObj);
					} break; // ����
				case RAISE_LAST_CMD_CHECK:
					{					
						if ( g_Poker.bDistCard )
						{
							break;
						}

						//������ �۾� : ī�弼�� ���̸� ����Ʈ�� �������� �ʴ´�. 2014.01
						if ( CheckRoundForCardSet() )
						{
							SoundFxKind(nLastCmd, m_cPlayer[ nSNum ].UI.Sex);

							effect::CBetCmdEffect *pObj = NULL;
							SAFE_NEW( pObj, effect::CBetCmdEffect );

							pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_CHECK);	
							cpEffectPos=pObj->GetEffectPos();
							g_cVipJackpot.OnBetGame(cpEffectPos, false);			
							g_pGameView->m_cAniSprListManager.ADDObject(pObj);				
						}

							

					}break; // üũ
				case RAISE_LAST_CMD_CALL: 
					{	
						if ( g_Poker.bDistCard )
						{
							break;
						}		
						
						//������ �۾� : ī�弼�� ���̸� ����Ʈ�� �������� �ʴ´�. 2014.01
						if ( CheckRoundForCardSet() )
						{
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);

							if ( m_cPlayer[ nSNum ].GetNowMoney() == 0  )
							{
								g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
							}

							effect::CBetCmdEffect *pObj = NULL;
							SAFE_NEW( pObj, effect::CBetCmdEffect );

							pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_CALL);		
							cpEffectPos=pObj->GetEffectPos();		
							g_cVipJackpot.OnBetGame(cpEffectPos, false);
							g_pGameView->m_cAniSprListManager.ADDObject(pObj);
						}
						
					} break; // ��
				case RAISE_LAST_CMD_PING:  
					{	
						if ( g_Poker.bDistCard )
						{
							break;
						}				


						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);

						if ( m_cPlayer[ nSNum ].GetNowMoney() == 0  )
						{
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}

						effect::CBetCmdEffect *pObj = NULL;
						SAFE_NEW( pObj, effect::CBetCmdEffect );

						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_PING);
						cpEffectPos=pObj->GetEffectPos();
						g_cVipJackpot.OnBetGame(cpEffectPos, false);
						g_pGameView->m_cAniSprListManager.ADDObject(pObj);

					}  break; // ��

				case RAISE_LAST_CMD_DADANG:
					{					
						if ( g_Poker.bDistCard )
						{
							break;
						}			

						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);

						if ( m_cPlayer[ nSNum ].GetNowMoney() == 0  )
						{
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}

						effect::CBetCmdEffect *pObj = NULL;
						SAFE_NEW( pObj, effect::CBetCmdEffect );

						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_DADANG);	
						cpEffectPos=pObj->GetEffectPos();			
						g_cVipJackpot.OnBetGame(cpEffectPos, false);
						g_pGameView->m_cAniSprListManager.ADDObject(pObj);
					}  break; // ����

				case RAISE_LAST_CMD_FULL:
					{					
						if ( g_Poker.bDistCard )
						{
							break;
						}			

						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);

						if ( m_cPlayer[ nSNum ].GetNowMoney() == 0  )
						{
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}

						effect::CBetCmdEffect *pObj = NULL;
						SAFE_NEW( pObj, effect::CBetCmdEffect );

						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_FULL);				
						cpEffectPos=pObj->GetEffectPos();
						g_cVipJackpot.OnBetGame(cpEffectPos, true);
						g_pGameView->m_cAniSprListManager.ADDObject(pObj);
					}  break; // ����

				case RAISE_LAST_CMD_HALF:
					{					
						if ( g_Poker.bDistCard )
						{
							break;
						}

						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);

						if ( m_cPlayer[ nSNum ].GetNowMoney() == 0  )
						{
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}

						effect::CBetCmdEffect *pObj = NULL;
						SAFE_NEW( pObj, effect::CBetCmdEffect );

						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_HALF,  ( g_Poker.nRound == 4 ? true : false ) );
						cpEffectPos=pObj->GetEffectPos();
						g_cVipJackpot.OnBetGame(cpEffectPos, false);
						g_pGameView->m_cAniSprListManager.ADDObject(pObj);
					} break; // �ƽ�


				case RAISE_LAST_CMD_ALLIN:
					{					
						if ( g_Poker.bDistCard )
						{
							break;
						}

						g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);

						if ( m_cPlayer[ nSNum ].GetNowMoney() == 0  )
						{
							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
						}

						effect::CBetCmdEffect *pObj = NULL;
						SAFE_NEW( pObj, effect::CBetCmdEffect );

						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_ALLIN, ( g_Poker.nRound == 4 ? true : false ) );
						cpEffectPos=pObj->GetEffectPos();
						g_cVipJackpot.OnBetGame(cpEffectPos, false);
						g_pGameView->m_cAniSprListManager.ADDObject(pObj);
					} break; // ����
					/*
					// 1��
					case RAISE_LAST_CMD_LIMIT_1X:
					{
					if( g_Poker.bDistCard == TRUE ) break;		

					effect::CBetCmdEffect *pObj = NULL;
					SAFE_NEW( pObj, effect::CBetCmdEffect );

					pObj->Init( nSNum, ANF_BET_CMD, ANF_BET_RAISE_X1 );
					g_pGameView->m_cAniSprListManager.ADDObject( pObj );

					g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni( nSNum, 0 );
					if( g_PLAYER(nSNum)->UI.GetMoney(IsTournament()) == 0 )
					{
					g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
					}
					}break;
					// 2��
					case RAISE_LAST_CMD_LIMIT_2X:
					{
					if( g_Poker.bDistCard == TRUE ) break;		

					effect::CBetCmdEffect *pObj = NULL;
					SAFE_NEW( pObj, effect::CBetCmdEffect );

					pObj->Init( nSNum, ANF_BET_CMD, ANF_BET_RAISE_X1 );
					g_pGameView->m_cAniSprListManager.ADDObject( pObj );

					g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni( nSNum, 0 );
					if( g_PLAYER(nSNum)->UI.GetMoney(IsTournament()) == 0 )
					{
					g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
					}
					}break;
					// 5��
					case RAISE_LAST_CMD_LIMIT_5X:
					{
					if( g_Poker.bDistCard == TRUE ) break;		

					effect::CBetCmdEffect *pObj = NULL;
					SAFE_NEW( pObj, effect::CBetCmdEffect );

					pObj->Init( nSNum, ANF_BET_CMD, ANF_BET_RAISE_X2 );
					g_pGameView->m_cAniSprListManager.ADDObject( pObj );

					g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni( nSNum, 0 );
					if( g_PLAYER(nSNum)->UI.GetMoney(IsTournament()) == 0 )
					{
					g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
					}
					}break;
					// 10��
					case RAISE_LAST_CMD_LIMIT_10X:
					{
					if( g_Poker.bDistCard == TRUE ) break;		

					effect::CBetCmdEffect *pObj = NULL;
					SAFE_NEW( pObj, effect::CBetCmdEffect );

					if( g_Poker.nRound == 1 || g_Poker.nRound == 2 )
					{
					pObj->Init( nSNum, ANF_BET_CMD, ANF_BET_RAISE_X3 );
					}
					else
					{
					pObj->Init( nSNum, ANF_BET_CMD, ANF_BET_RAISE_X2 );
					}

					g_pGameView->m_cAniSprListManager.ADDObject( pObj );

					g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni( nSNum, 0 );
					if( g_PLAYER(nSNum)->UI.GetMoney(IsTournament()) == 0 )
					{
					g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
					}
					}break;
					// 20��
					case RAISE_LAST_CMD_LIMIT_20X:
					{
					if( g_Poker.bDistCard == TRUE ) break;		

					effect::CBetCmdEffect *pObj = NULL;
					SAFE_NEW( pObj, effect::CBetCmdEffect );

					pObj->Init( nSNum, ANF_BET_CMD, ANF_BET_RAISE_X3 );
					g_pGameView->m_cAniSprListManager.ADDObject( pObj );

					g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni( nSNum, 0 );
					if( g_PLAYER(nSNum)->UI.GetMoney(IsTournament()) == 0 )
					{
					g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
					}
					}break;	
					*/

				default:
					{
						//����Ʈ ����
						if (nLastCmd>=RAISE_LAST_CMD_LIMIT_1X)
						{
							if( g_Poker.bDistCard == TRUE ) break;		


							effect::CBetCmdEffect *pObj = NULL;
							SAFE_NEW( pObj, effect::CBetCmdEffect );

							if ( LastPnum < 0 || LastPnum >=MAX_PLAYER ) 
								break;

							//�������� ������ ó�������� ������� ��� BET��������, ���� ������ �����ϴ� ��� RAISE�������� ǥ���Ѵ�.
							if (g_Poker.RU[LastPnum].nReCall==0)
							{
								g_pGameView->m_cPlayerDrawManager.SetBettingMoney(nSNum, nLastCmd, llLastPlayerBetMoney); //���� + �ݾ�

								eLIMIT_X eLastLimitX = g_pGameView->m_cGameViewBtnMan.GetLimitMultiflierWithLastCmd((RAISE_LAST_CMD)nLastCmd);
								if (g_pGameView->m_cGameViewBtnMan.GetLimitMultiflierWithBetStep(g_Poker.nRound, LIMIT_BET_STEP_1)==eLastLimitX)	///������������ üũ
								{								
									pObj->Init( nSNum, ANF_BET_CMD, ANF_BET_LIMIT_SMALL_BET );	//������
								}
								else
								{
									pObj->Init( nSNum, ANF_BET_CMD, ANF_BET_LIMIT_BIG_BET );	//��
								}
							}
							else
							{
								g_pGameView->m_cPlayerDrawManager.SetBettingMoney(nSNum, nLastCmd, g_Poker.RU[LastPnum].nCalcBat, true); //�ް� + �ݾ� + ��
								pObj->Init( nSNum, ANF_BET_CMD, ANF_BET_LIMIT_RAISE );
							}
							cpEffectPos=pObj->GetEffectPos();
							g_cVipJackpot.OnBetGame(cpEffectPos, false);
							g_pGameView->m_cAniSprListManager.ADDObject( pObj );						

							g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni( nSNum, 0 );
							if( g_PLAYER(nSNum)->GetNowMoney() == 0 )
							{
								g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
							}

						}

					} break;
				}

				//���� ���ǵ忡 ���� ���� ��� 
				if ( nLastCmd >= RAISE_LAST_CMD_CHECK && nLastCmd <= RAISE_LAST_CMD_ALLIN && g_Poker.bDistCard == false )
				{
					if ( g_Poker.nRound == 4)
					{
						SoundFxKindBig(nLastCmd,g_PLAYER(nSNum)->UI.Sex, false);
					}
					else
					{
						SoundFxKind(nLastCmd,g_PLAYER(nSNum)->UI.Sex, false);
					}

					if ( m_cPlayer[ nSNum ].GetNowMoney() == 0 )
					{
						//���� ��ũ�� ��ο� �Ǵ°��� ������ ��ο� ���
					}
				}
			}
		}

		//������ ���� �۾����� �ּ�ó�� 2014.01->�����۾�
		//���� �ݾ� ���ϱ�
		if( g_Poker.RU[LastPnum].nLastCmd != RAISE_LAST_CMD_FOLD )
		{
			m_i64Double = ( g_Poker.RU[ g_Poker.LastPlayer ].nCalcBat + g_Poker.RU[ g_Poker.LastPlayer ].nReCall ) * 2;
		}
		

		if ( g_Poker.bDistCard )
		{
			m_BetCount = -1;			

			g_pGameView->m_cAniSprListManager.BetCmd_DelDelay(55);
			m_nBetCallMoneyClearCnt = 55;

			if (g_Poker.nState == RSTATE_STARTGAME ) 
			{
				g_pGameView->m_cAniSprListManager.Del_AniListOBj(ANF_BG_CUTINGINFO);				
			}

			//������ : ī�尡 ���ƿ��� ������ �ʱ�ȭ
			SetReserveButton(RESERVESTATE_BET_NONE);			
				
			return true;
		}
	}	

	return false;
}


void GameProcess_BD::Raise(POKERGAME *pMsg)
{
	if ( pMsg == NULL )
	{
		return;
	}


	// ���� ������ ����� ���ñݾ��� �˾ƿ��� ���� ���� �������� �ӽ� ����
	RAISEUSER RU_Before[MAX_PLAYER];
	memcpy(RU_Before, g_Poker.RU, sizeof(RAISEUSER) * MAX_PLAYER);

	memcpy(&g_Poker,(char*)pMsg, sizeof(g_Poker));	
	g_pGameView->m_cPlayerDrawManager.SetTurn( -1 );
	g_pGameView->m_cGameViewBtnMan.Reset();

	int nImageBoss = g_Poker.nImageBoss;

	// ### [ ����ڸӴ� ���� ] ### 
	for ( int i = 0; i < MAX_PLAYER; i++ )
	{
		int pn = GetPNum_ByServPN(i);
		if(strlen(m_cPlayer[ pn ].UI.ID)>0 && m_cPlayer[ pn ].JoinState == 1 && m_cPlayer[ pn ].PlayState == 1) // �����̸�
		{  
			//m_cPlayer[ pn ].UI.SetMoney(g_Poker.RU[i].nPMoney, IsTournament());// ����� �Ӵ� ��
			//MAX�� �Լ� ȣ��
			m_cPlayer[ pn ].SetNowMoney( g_Poker.RU[i].nPMoney );// ����� �Ӵ� ��
			if( g_Poker.PS[i].bFold ) m_cPlayer[ pn ].bFold = TRUE;

			// �߰� 
			//if(m_cPlayer[ pn ].UI.GetMoney(IsTournament()) < 0) m_cPlayer[ pn ].UI.SetMoney(0, IsTournament());
			// �߰� 
			if(m_cPlayer[ pn ].GetNowMoney() < 0) 
			{
				m_cPlayer[ pn ].SetNowMoney( 0 );
			}

			// ### [ ������� ] ###
			/*m_cPlayer[ pn ].UI.nIcon = GetMoneyGrade(m_cPlayer[ pn ].UI.GetTotalMoney(IsTournament()) );*/
			m_cPlayer[ pn ].UI.nIcon = GetMoneyGrade(m_cPlayer[ pn ].UI.GetTotalMoney() );
		}
	}

	INT64 llLastPlayerBetMoney = g_Poker.RU[g_Poker.LastPlayer].nRealBat - RU_Before[g_Poker.LastPlayer].nRealBat;
	if ( Raise_InfoAndSound(llLastPlayerBetMoney) )
	{
		g_pGameView->m_cSidePotMoney.Check();
		return;
	}

	// ���� ���θ�� �׽�Ʈ �ڵ�.
	int betCmd = g_Poker.RU[g_Poker.LastPlayer].nLastCmd ;
	if( betCmd == RAISE_LAST_CMD_HALF ||betCmd == RAISE_LAST_CMD_QUARTER ||betCmd == RAISE_LAST_CMD_FULL) // ���� �ݾ�
	{
		if(g_RI.State == 1 && g_Poker.nAutoBetCmd <= 0)
			g_cPromotionMan.SetHalfJackpotCountPlus() ;
	}

	// ���������̴� ���ý���  �����̸� ��ư Ȱ��ȭ
	if ( g_Poker.nState == RSTATE_STARTGAME ) 
	{
		m_nCurPlayer = g_Poker.CurPlayer;

		if ( m_nCurPlayer < 0 || m_nCurPlayer >= MAX_PLAYER ) 
		{
			return;
		}

		int pn = GetPNum_ByServPN( m_nCurPlayer );

		if ( g_Poker.CurPlayer == g_Poker.nBossPNum && g_Poker.bBtnBet == TRUE )
		{
			m_BetCount++;
		}

		if ( g_Poker.LastStyle == -1 )
		{
			INT64 nRaiseBat = GetRaiseBat();
			chip_draw_func(pn, nRaiseBat);
			g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();

			//������ : ������ �ʱ�ȭ
			SetReserveButton(RESERVESTATE_BET_NONE);

			int LastPlayer = g_Poker.LastPlayer;
			int bet = GetPNum_ByServPN(LastPlayer);	
			m_cPlayer[ bet ].OnSetBetDrawMoney( GetLastBetMoney(LastPlayer,g_Poker.RU[g_Poker.LastPlayer].nLastCmd),
				0,g_Poker.RU[LastPlayer].nLastCmd );		


			if (g_Poker.nState == RSTATE_STARTGAME ) 
			{
				g_pGameView->m_cAniSprListManager.Del_AniListOBj(ANF_BG_CUTINGINFO);				
			}

			g_pGameView->m_cSidePotMoney.Check();
			return;
		}

		if ( g_Poker.LastStyle >= 2 ) // ���ýÿ���
		{
			int LastPlayer = g_Poker.LastPlayer;
			if(LastPlayer < 0 || LastPlayer >= MAX_PLAYER)
			{
				return;
			}

			int bet = GetPNum_ByServPN(LastPlayer);			

			if(g_Poker.RU[LastPlayer].nLastCmd == 3){
				INT64 nRaiseBat = GetRaiseBat();
				chip_draw_func(bet, nRaiseBat);				
			}
			else{
				INT64 nRaiseBat = GetRaiseBat();
				chip_draw_func(bet, nRaiseBat);
			}

			m_cPlayer[ bet ].OnSetBetDrawMoney(GetLastBetMoney(LastPlayer,g_Poker.RU[g_Poker.LastPlayer].nLastCmd),
				0,g_Poker.RU[LastPlayer].nLastCmd);	
		}


		g_pGameView->m_cPlayerDrawManager.SetFlyWindCard( false );
		g_pGameView->m_cPlayerDrawManager.SetStartTimer();

		// ### [ ������� ] ###
		//���� ����(���� ��ư Ȱ��)�� �ٽ��̴�. m_nActiveBetBtnDelayTime ������ �����̸� �ش�.
		if ( !g_ObserverMan.IsMyObserver() && m_cPlayer[ 0 ].ServPNum == m_nCurPlayer )
		{
			g_pGameView->m_cGameViewBtnMan.SetShowBetMony(); //< CYS 100705 >
			m_bBtnClick = FALSE;

			// �ڵ����ýÿ��� ���ù�ư Ȱ��ȭ �����̸� ���� �����Ѵ�.
			if (g_Poker.nAutoBetCmd > 0)
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelayInAutoBet(0, 0);
			else
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelay( 54, g_RI.nGameIndex );
		}

		// ������ ���� ���� �߰�.(������ �ƴ� ������ ��ư Ȱ��ȭ�� �ȿ�)
		if(!g_ObserverMan.IsMyObserver() && m_cPlayer[ 0 ].ServPNum != m_nCurPlayer)
		{
			// �ڵ����ýÿ��� ���ù�ư Ȱ��ȭ �����̸� ���� �����Ѵ�.
			if (g_Poker.nAutoBetCmd > 0)
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelayInAutoBet(0, 0);
			else
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelay( 54, g_RI.nGameIndex );
		}

		// ������ ���
		int nRound =g_Poker.nRound;
		if ( g_Poker.nAutoBetCmd > 0 )
		{
			g_pGameView->m_cDisplayBoard.SetText( EDBT_AUTOBETTING ); // �ڵ�����
		}
		else
		{
			// ���ø޽���
			g_pGameView->m_cDisplayBoard.SetText( ENUM_DISPLAY_BOARD_TEXT((int)EDBT_BEFORE_MORNING_BETTING + nRound - 1 ));
		}
		//

		// ������ �߰� ī�弼������ ���� �϶��� �� ǥ�� ����.
		if ( CheckRoundForCardSet() )
		{
			g_pGameView->m_cPlayerDrawManager.SetTurn( pn );
		}
		g_pGameView->m_cAniSprListManager.BetCmd_ClearCmd( pn, 20 ); // �ٷ� �������� �ʰ� ��� �� ����

		//����Ʈ���� �ʱ�ȭ( ��ī�� ���� ���� )
		g_pGameView->m_cGameViewBtnMan.SetLastCmdBackup( RAISE_LAST_CMD_NULL, true );

	}

	g_pGameView->m_cSidePotMoney.Check();

	if (g_Poker.nState == RSTATE_STARTGAME ) 
	{
		g_pGameView->m_cAniSprListManager.Del_AniListOBj(ANF_BG_CUTINGINFO);				
	}
}


void GameProcess_BD::OnGameOver(GAMEOVERRESULT *pGOR)
{	
	NMBASE::SOUND::g_Sound.Stop(SND_HIDDEN_BGM);
	m_nCurPlayer = -1;
	// ���� ��� ���� 
	ZeroMemory(&m_stGameOverResultData,sizeof(m_stGameOverResultData));	

	//������ �����Ͽ� ������ ��� �ʱ�ȭ.
	ReserveState(RESERVESTATE_BET_NONE);
	CanReserve(false);

	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();		// ��ư �ʱ�ȭ

	g_pGameView->m_cPlayerDrawManager.SetOpen(pGOR->bOpen);
	g_pGameView->m_cPlayerDrawManager.ResetLowBDWinnerPNum();
	g_pGameView->m_cPlayerDrawManager.SetTurn(-1);

	g_pGameView->m_cGameHelper.Reset();

	m_stWinnerPNumData.Clear();

	m_stWinnerPNumData.nWinnerNumCount = pGOR->nWinnerNum;
	m_stWinnerPNumData.bOneWinnerFlag = ( m_stWinnerPNumData.nWinnerNumCount > 1 ) ? false : true;
	memcpy( m_stWinnerPNumData.WinnerSPNum , pGOR->nNowWinner , sizeof(m_stWinnerPNumData.WinnerSPNum));


	for ( int i = 0; i < MAX_PLAYER; i++ )
	{
		m_cPlayer[ i ].GetEffectObjectList()->DelObject( ANF_CUTINFO_NUM );
	}

	//���� Ŀ��� ����
	g_pGameView->m_cAniSprListManager.BetCmd_DelDelay(55);

	memcpy(&g_Poker,&pGOR->Pg,sizeof(g_Poker));
	memcpy(&m_stGameOverResultData,pGOR,sizeof(m_stGameOverResultData));

	g_pGameView->m_cAniSprListManager.Del_AniListOBj(ANF_BG_CUTINGINFO); // �ٲ� ī�带

	for ( int i = 0; i < MAX_PLAYER ; i++ )
	{
		g_Poker.RU[ i ].nLastCmd = 0;				
		g_PLAYER( i )->ClearObjectEffect(ANF_PLAYER_ALLINMARK);
	}
	//ī�� ���� ���°� ���� 
	m_pMovingCardMgr->Destroy();

	g_pGameView->m_cPlayerDrawManager.SetHiddenCardShow( true );

	// ����ī�� ���� ���� ī�� �����.
	for ( int i = 0; i < TOTAL_CARD_BD; i++ )
	{
		m_cPlayer[ 0 ].m_cMyCard[ i ].SetHiddenCard( false );
	}

	for( int i=0; i<MAX_PLAYER; i++)
	{
		if ( strlen( m_stGameOverResultData.Ui[ i ].ID ) > 0 ) //  ��������.
		{
			int pnum = GetPlayerPNum(m_stGameOverResultData.Ui[i].ID);
			if(pnum == -1) continue;
			m_cPlayer[ pnum ].m_llResultMoney = m_stGameOverResultData.Ui[ i ].nResultMoney;		
			memcpy( &g_PLAYER(pnum)->UI.AllGameRecord, &m_stGameOverResultData.Ui[i].AllGameRecord, sizeof(m_stGameOverResultData.Ui[i].AllGameRecord) );
			m_cPlayer[ pnum ].SetChangeRoomInfo(m_stGameOverResultData.Cui[i]);
			/*m_cPlayer[ pnum ].UI.SetMoney(m_stGameOverResultData.Ui[i].PMoney, IsTournament());*/
			if(g_RI.ChipKind != 1)	m_cPlayer[ pnum ].UI.SetMoney(m_stGameOverResultData.Ui[i].PMoney);
			/*if(	m_cPlayer[ pnum ].UI.GetMoney(IsTournament()) <= 0)*/
			if(	m_cPlayer[ pnum ].GetNowMoney() <= 0)
			{
				/*m_cPlayer[ pnum ].UI.SetMoney(0, IsTournament());*/
				m_cPlayer[ pnum ].SetNowMoney(0);
			}

			// �������� ���
			/*m_cPlayer[ pnum ].UI.nIcon = GetMoneyGrade(m_cPlayer[ pnum ].UI.GetTotalMoney(IsTournament()));	*/
			m_cPlayer[ pnum ].UI.nIcon = GetMoneyGrade(m_cPlayer[ pnum ].UI.GetTotalMoney());	

			if (pnum == 0 && !g_ObserverMan.IsMyObserver() )
			{								
				m_cPlayer[ pnum ].UI.CopyAllGameRecords( GM().GetMyInfo()->UI.AllGameRecord );
				GM().GetMyInfo()->SetChangeRoomInfo(*m_cPlayer[pnum].GetChangeRoomInfo());
				/*GM().GetMyInfo()->UI.SetMoney(m_cPlayer[ pnum ].UI.GetMoney(IsTournament()), IsTournament());*/
				GM().GetMyInfo()->UI.SetMoney(m_cPlayer[ pnum ].UI.GetMoney());
				GM().GetMyInfo()->UI.nIcon   =  m_cPlayer[ pnum ].UI.nIcon;				
				//���� ǥ�ø� ���Ѵ�.. ��� ����
				m_cPlayer[ pnum ].m_bShowValue = false;
			}
		}
	}

	m_pCuttingStageInfo->Clear(); //< CYS 100720 >
	
	//////////////////////////////////////////////////////////////////////////
	// ��ǿ��� Ȯ��

	// ��ư spr �ε����� 0�̸� ���� ī�� ���� On, 1 �̸� Off
	bool bUIFoldOpen = false;
	if ( g_pGameView->m_cGameViewBtnMan.m_btnCardOpen.GetButtonNumber() == 0 ) bUIFoldOpen = true;

	for( int i = 0; i < MAX_PLAYER; i++ )
	{
		if ( strlen( pGOR->Ui[ i ].ID ) > 0 ) //  ��������.
		{
			int pnum = GetPlayerPNum( pGOR->Ui[ i ].ID );
			if( pnum == -1 ) continue;

			// Ŭ���̾�Ʈ 0�� �ڸ��� ���� ���α������� ���� ī�� ������ ��� ���� ������ UI������ Ȯ�����ش�.
			if ( pnum == 0 && !g_ObserverMan.IsMyObserver() )
			{	
				if( !bUIFoldOpen ) continue;
			}
			
			// ��������� ī�� ������ ��� 0�� �ƴ϶�� ��ǿ��� ���·� ī�� ��������� �Դ°ɷ� ����
			int nCount = 0;
			
			for ( int k = 0; k < TOTAL_CARD_BD; k++ )
			{
				if( g_Poker.BC[ i ].nCard[ k ] > 0 ) nCount++;
			}
			
			// ����ī����� ���¸� Ȯ���Ѵ�.
			if( nCount > 0 && nCount < TOTAL_CARD_BD + 1 )
			{
				if( g_pGameView->m_cPlayerDrawManager.IsOpen() )
				{
					// �������� : ������� �ʴ� ������ ��� ī�������� �⺻������ ���´�.( ����� �ߴµ� ī�� ������ �Դٸ� ����ī����� ���� )
					// 2018.09.27 �̽� �߻����� ��� ���� ��� ����
					//if( m_cPlayer[ pnum ].bFold ) m_cPlayer[ pnum ].bFoldOpen = true;	
				}
				else
				{
					// ��ǽ��� : ������ ������ ��� ������ ī�� ������ ����.( �������� �ƴѵ� ī�� ������ �´ٸ� ����ī����� ���� )
					// 2018.09.27 �̽� �߻����� ��� ���� ��� ����
					//m_cPlayer[ pnum ].bFoldOpen = true;	
				}
			}
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	// ��� ���� �Է�
	for( int i = 0; i < MAX_PLAYER; i++ )
	{
		if ( strlen(pGOR->Ui[ i ].ID ) > 0 ) //  ��������.
		{
			int pnum = GetPlayerPNum(pGOR->Ui[ i ].ID);
			if(pnum == -1) continue;

			m_cPlayer[ pnum ].ReSetCardPostion();
			
			// IsOpen()�� ���������� ������ ������ ������ ������ ������ ������ �������� ��ǿ��� ���°� ���� �� �־ ����� ���� �˻�.
			if( g_pGameView->m_cPlayerDrawManager.IsOpen() && !m_cPlayer[ pnum ].bFold )
			{
				for ( int k = 0; k < TOTAL_CARD_BD; k++ )
				{
					m_cPlayer[ pnum ].m_cMyCard[ k ].SetCardNo( g_Poker.BC[ i ].nCard[ k ] ); // �� ����
					m_cPlayer[ pnum ].SetFaceUp( k );
				}

				if ( pnum == 0 ) m_cPlayer[ pnum ].m_bShowValue = true;
				else			 m_cPlayer[ pnum ].m_bShowValue = false;

				AutoSort_Recommand( pnum ); //< CYS 101201 >

				m_cPlayer[ pnum ].SetCardResult();
				m_cPlayer[ pnum ].Draw_DarkCard();
			} 
			else
			{
				for ( int k = 0; k < TOTAL_CARD_BD; k++ )
				{
					m_cPlayer[ pnum ].m_cMyCard[ k ].SetCardNo( g_Poker.BC[ i ].nCard[ k ] ); // �� ����
				}
				
				if ( pnum == 0 && !g_ObserverMan.IsMyObserver() )
				{
					for ( int k = 0; k < TOTAL_CARD_BD; k++ ) m_cPlayer[ pnum ].SetFaceUp( k );

					AutoSort_Recommand(); //< CYS 101201 >

					m_cPlayer[pnum].SetCardResult();
					m_cPlayer[pnum].Draw_DarkCard();

					m_cPlayer[pnum].m_bShowValue = true;
				}
				else
				{
					// ��ǿ��� ���¿� ���� ��� ������ ī�������� ������ �� �ִ�.
					if( m_cPlayer[ pnum ].bFoldOpen )
					{
						for ( int k = 0; k < TOTAL_CARD_BD; k++ ) m_cPlayer[ pnum ].SetFaceUp( k );

						AutoSort_Recommand(); //< CYS 101201 >

						m_cPlayer[ pnum ].SetCardResult(); // ����ó��
						m_cPlayer[ pnum ].Draw_DarkCard(); // ������ ���� ī�� ȸ��ó��
						m_cPlayer[ pnum ].FoldUser_CardOpen(); // ���� ī�� ������ �ٽ� ������� ���

						m_cPlayer[ pnum ].m_bShowValue = true;
					}
					else
					{
						// �⺻������ ī�带 ���� ����
						for ( int k = 0; k < TOTAL_CARD_BD; k++ ) m_cPlayer[ pnum ].SetFaceDown( k );
					}
				}
				
			}
		}
	}	

	// ������ ���
	g_pGameView->m_cDisplayBoard.SetText(EDBT_RESULT, (LPARAM)&m_stGameOverResultData); // �ٸ� ���� ���� ����� ȣ��

	// ��� ��� Delay�� �ֱ� ���� 
	m_nResultDelayCount = 20;
	m_bResultDelayStart = true;

	g_pGameView->m_cPlayerDrawManager.CheckResultValue();

	OnResultGameOverInfo(); // ����� ������ ����Ʈ������ ä��â���� �̸�����
}

// Backup
void GameProcess_BD::OnResultGameOverInfo()
{
	int nWinnerNum = m_stGameOverResultData.nWinnerNum;
	if ( nWinnerNum < 0 || nWinnerNum >= 5)
	{
		return;
	}

	//�� ȿ�� 
	const int FOLD = 2;
	const int WIN  = 1;
	const int LOSE = 0;	

	m_stPlayRTInfo.Clear();

	int nPlayerIndex = 0;	

	// ��� ����
	for ( int i = 0; i < nWinnerNum ; i++ )
	{
		int nwinpnum = GetPNum_ByServPN( m_stGameOverResultData.nNowWinner[ i ] );

		if ( nwinpnum < 0 || nwinpnum >= 5 )
		{
			continue;
		}

		//���� ���� ���θ�� ������ ���� ������ ����.
		g_cPromotionMan.SetWinnerNum(nwinpnum) ;

		m_stPlayRTInfo.Data[ nPlayerIndex ].nPNum = nwinpnum;
		m_stPlayRTInfo.Data[ nPlayerIndex ].nServPNum = m_stGameOverResultData.nNowWinner[ i ];

		m_stPlayRTInfo.Data[ nPlayerIndex ].nWinType =WIN;
		m_stPlayRTInfo.Data[ nPlayerIndex ].strID = GetPlayerNickName( nwinpnum );
		m_stPlayRTInfo.Data[ nPlayerIndex ].strValueName = m_cPlayer[ nwinpnum ].GetValueString();
		m_stPlayRTInfo.Data[ nPlayerIndex ].nValueKind =  m_cPlayer[ nwinpnum ].GetMyLowBDRule()->m_MyValue;

		int spnum = m_stGameOverResultData.nNowWinner[ i ];
		m_stPlayRTInfo.Data[ nPlayerIndex ].llResultMoney = m_stGameOverResultData.Ui[ spnum ].nResultMoney;/*m_cPlayer[ nwinpnum ].m_llResultMoney*/;
		m_stPlayRTInfo.Data[ nPlayerIndex ].fGoryRatio = m_stGameOverResultData.Ui[spnum].fGoryRatio;
		m_stPlayRTInfo.Data[ nPlayerIndex ].nGoryMoney = m_stGameOverResultData.Ui[spnum].GameFeeDP;
		m_stPlayRTInfo.Data[ nPlayerIndex ].PremMenuNo = m_cPlayer[nwinpnum].UI.PremMenuNo;
		if( !g_pGameView->m_cPlayerDrawManager.IsOpen() )
		{
			m_stPlayRTInfo.Data[ nPlayerIndex ].strValueName ="��� ��";
		}

		for ( int j = 0 ; j < TOTAL_CARD_BD ; j ++ )
		{
			int cno = m_cPlayer[ nwinpnum ].GetMyLowBDRule()->m_RuleCompleteCard.RealCard[ j ];
			if ( cno > -1 )
			{
				m_stPlayRTInfo.Data[ nPlayerIndex ].arrCard[ j ] = cno;
			}
		}

		nPlayerIndex++;
	}	

	//���� �׸��� ���� ��	
	for( int i = 0 ; i < MAX_PLAYER ; i++ )
	{
		if ( strlen( m_stGameOverResultData.Ui[ i ].ID ) > 0 ) //  ��������.
		{
			bool bWinID = false;
			std::string strWinID = m_stGameOverResultData.Ui[ i ].ID;

			for ( int j = 0 ; j < nWinnerNum ; j++ )
			{
				if ( strcmp( strWinID.c_str(), m_stGameOverResultData.WinID[ j ] ) == 0 )
				{
					bWinID = true;
					break;
				}
			}

			if ( bWinID ) 
			{
				continue;
			}

			int nPNum = GetPlayerPNum( m_stGameOverResultData.Ui[ i ].ID );
			if ( nPNum == -1 )
			{
				continue;			
			}

			m_stPlayRTInfo.Data[ nPlayerIndex ].nPNum = nPNum;
			m_stPlayRTInfo.Data[ nPlayerIndex ].nServPNum = i;
			m_stPlayRTInfo.Data[ nPlayerIndex ].llResultMoney = m_cPlayer[nPNum ].m_llResultMoney;
			m_stPlayRTInfo.Data[ nPlayerIndex ].strID = GetPlayerNickName(nPNum );
			m_stPlayRTInfo.Data[ nPlayerIndex ].PremMenuNo = m_cPlayer[ nPNum ].UI.PremMenuNo;
			m_stPlayRTInfo.Data[ nPlayerIndex ].fGoryRatio = m_stGameOverResultData.Ui[i].fGoryRatio;
			m_stPlayRTInfo.Data[ nPlayerIndex ].nGoryMoney = m_stGameOverResultData.Ui[i].GameFeeDP;

			/*if ( m_cPlayer[ nPNum ].UI.GetMoney(IsTournament()) < 100 )*/
			if ( m_cPlayer[ nPNum ].UI.GetMoney() < 100 )
			{
				m_stPlayRTInfo.Data[ nPlayerIndex ].bAllIn = true;
			}
			else
			{
				m_stPlayRTInfo.Data[nPlayerIndex].bAllIn = false;
			}

			if ( m_cPlayer[ nPNum ].bFold )
			{
				m_stPlayRTInfo.Data[ nPlayerIndex ].nWinType = FOLD;
				m_stPlayRTInfo.Data[ nPlayerIndex ].strValueName = "����";	
			}
			else
			{
				m_stPlayRTInfo.Data[ nPlayerIndex ].nWinType = LOSE;
				m_stPlayRTInfo.Data[ nPlayerIndex ].strValueName = m_cPlayer[ nPNum ].GetValueString();		
				m_stPlayRTInfo.Data[ nPlayerIndex ].nValueKind =  m_cPlayer[ nPNum ].GetMyLowBDRule()->m_MyValue;
			}

			nPlayerIndex++;
		}
	}

}

void GameProcess_BD::OnResultGameOver()
{	
	GM().SetWaitResponse(FALSE); 
	g_RI.State = 0;
	m_bGameStart = FALSE;				// ���� ���� �÷��� ����	

	int nWinnerNum =m_stGameOverResultData.nWinnerNum;
	if( nWinnerNum < 0 || nWinnerNum >= 5)return;

	g_pGameView->m_cPlayerDrawManager.SetGameOver(true, timeGetTime());

	g_pGameView->m_cSidePotMoney.Check();

	int i = 0;
	//�� ȿ�� 
	const int FOLD = 2;
	const int WIN  = 1;
	const int LOSE = 0;	

	MAP_RANKING mapRanking;
	for(int i=0; i< GetMaxPlayer(); i++)
	{
		if(strlen(m_stGameOverResultData.Ui[i].ID)>0) //  ��������.
		{
			int pnum = GetPlayerPNum(m_stGameOverResultData.Ui[i].ID);
			if(pnum == -1) continue;
			if(m_stGameOverResultData.Ui[i].nRank < 0) continue;

			mapRanking[m_stGameOverResultData.Ui[i].nRank][pnum] = m_stGameOverResultData.Ui[i].nResultMoney;
		}
	}
	g_pGameView->m_cSidePotMoney.SetGameOver(mapRanking);

	if ( m_stWinnerPNumData.nWinnerNumCount > -1 )
	{		
		m_cAllBetChip.SetWinner(&g_pGameView->m_cSidePotMoney);
	}

	//////////////////////////////////////////////////////////////////////////	

	bool bPlaySound = false;
	GAMERESULTDATA WinEftData;

	for ( int i = 0; i < MAX_PLAYER ; i++ )
	{
		if ( m_stPlayRTInfo.Data[ i ].nPNum < 0 || m_stPlayRTInfo.Data[ i ].nServPNum < 0 ) // ��ȿ�� ����
		{
			continue;
		}

		int nwinpnum = m_stPlayRTInfo.Data[ i ].nPNum;

		if ( nwinpnum < 0 || nwinpnum >= 5 )
		{
			continue;
		}

		if ( m_stPlayRTInfo.Data[ i ].nWinType == WIN ) // ����
		{
			g_pGameView->m_cPlayerDrawManager.AddLowBDWinnerPNum( m_stPlayRTInfo.Data[ i ].nServPNum ); 

			//���� ���� ���θ�� ������ ���� ������ ����.
			int nWinPNum = GetPNum_ByServPN(m_stPlayRTInfo.Data[ i ].nServPNum);
			g_cPromotionMan.SetWinnerNum(nWinPNum) ;

			// ����Ʈ �׸���
			WinEftData.Clear();
			WinEftData.wincase = m_stPlayRTInfo.Data[ i ].nWinType;
			WinEftData.pnum = nwinpnum;
			WinEftData.valuekind = m_stPlayRTInfo.Data[ i ].nValueKind;
			strcpy( WinEftData.str_value , m_stPlayRTInfo.Data[ i ].strValueName.c_str() );		
			strcpy( WinEftData.ID, m_stPlayRTInfo.Data[ i ].strID.c_str() );			
			WinEftData.nResultMoney = m_stPlayRTInfo.Data[ i ].llResultMoney;			

			for ( int j = 0 ; j < TOTAL_CARD_BD ; j ++ )
			{
				WinEftData.nCard[ j ] = m_stPlayRTInfo.Data[ i ].arrCard[ j ];
			}

			WinEftData.nTotalCardNum = TOTAL_CARD_BD;	

			if ( WinEftData.valuekind >= CBadugiRule::B4C_GOLF && WinEftData.valuekind <= CBadugiRule::B4C_THIRD && g_pGameView->m_cPlayerDrawManager.IsOpen() )
			{
				effect::CHighValueEffect* pObj = NULL;
				SAFE_NEW( pObj, effect::CHighValueEffect );
				pObj->Init(&WinEftData);
				g_pGameView->m_cAniSprListManager.ADDObject(pObj);	

				PBPlayEffectSound(  SND_WIN_HIGH );
			}
			else 
			{
				if((CPromotionManager::ENUM_PROMOTION_TYPE)GM().GetPromotionData()->nCnts[0] == CPromotionManager::EPT_POKER_2018_CHUSEOK)
				{
					MM().Call(UPG_CMK_SHOW_CHUSEOK_GOLDCOIN, WPARAM(0), LPARAM(0));
				}
				else
				{				
					MM().Call(UPG_CMK_SHOW_LUCKYWHEEL_POPUP, (WPARAM)0, (LPARAM)TRUE);
				}
				if ( nwinpnum == 0 ) // ���θ� ���� ��ġ�� ����
				{
					bPlaySound = true;
					PBPlayEffectSound(  SND_WIN_LOW );
				}
			}
		}
		else // ����
		{
			if ( nwinpnum == 0 ) // ���θ�
			{
				bPlaySound = true;

				if ( m_stPlayRTInfo.Data[ i ].bAllIn )
				{
					PBPlayEffectSound( SND_LOSE_ALLIN );
				}
				else
				{
					PBPlayEffectSound( SND_LOSE_NORMAL );
				}
			}
		}
	}

	// ���ӿ� �������� ���� ����
	if ( !bPlaySound )
	{
		PBPlayEffectSound( SND_LOSE_NORMAL );
	}

	// ä��â�� ���� ���
	CString str="";
	CString strM="";

	for ( i = 0; i < MAX_PLAYER ; i++ )
	{
		if ( m_stPlayRTInfo.Data[ i ].nPNum < 0 ) // ��ȿ�� ����
		{
			continue;
		}

		PLAYRESULTINFO *pData =	&m_stPlayRTInfo.Data[ i ];

		if ( !g_pGameView->m_cPlayerDrawManager.IsOpen() ) // ī�� ������ �ƴϸ� ���ڸ� ��´�. 1��!!!
		{
			if ( pData->nWinType == WIN ) // 
			{
				CString str1;	
				//str1.Format(" ----------------------------\n%s ----------------------------\n",g_StrMan.Get(_T("WIN_MESINFO")));
				str1.Format(" %s",g_StrMan.Get(_T("WIN_MESINFO")));
				str.Format(str1, pData->strID.c_str() );		
				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT));	

				strM = (CString)NMBASE::UTIL::g_MoneyMark( pData->llResultMoney ); // ���� �Ӵ� 

				strM.Append(strChip_Name[g_RI.ChipKind]);

				// ��� ������� ���
				if(g_RI.ChipKind==1)
				{					
					// ������ ���� �̺�Ʈ ���� ��� ǥ��
					if (m_stGameOverResultData.CommitionDiscount)
					{
						strM.Append("(������ ���� �̺�Ʈ��)");
					}
				}

				//str.Format(g_StrMan.Get(_T("BET_WIN")), strM); // ���� ���� �Ƶ� ## ���̽�:%s(+%s)		
				str.Format("�� ȹ��ݾ�:%s \n", strM); // ���� ���� �Ƶ� ## ���̽�:%s(+%s)	
				AddGameInfoViewEdit(str,GetChatColor(ECC2_RESULT_WINNER));

				str.Format("�� ����: ��ǽ� \n" );
				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_JOKBO));

				// 				str.Format(" ����������������������������\n");
				// 				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT));	

			}
			else
			{
				INT64 lostmoney = -pData->llResultMoney;
				strM = (CString)NMBASE::UTIL::g_MoneyMark( pData->llResultMoney );
				str.Format(" %s : ", pData->strID.c_str());
				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_LOSER));
				strM.Append(strChip_Name[g_RI.ChipKind]);
				str.Format("%s\n", strM );
				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_LOSEMONEY));
			}
		}
		else
		{
			if ( pData->nWinType == WIN ) // ī�� ���½� ���� ������ �� �� �ִ�.
			{
				CString str1;
				//str1.Format(" ----------------------------\n%s ----------------------------\n",g_StrMan.Get(_T("WIN_MESINFO")));
				str1.Format(" %s",g_StrMan.Get(_T("WIN_MESINFO")));
				str.Format(str1, pData->strID.c_str());		

				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT));	

				INT64 nTotBat = GetRealBet();

				strM = (CString)NMBASE::UTIL::g_MoneyMark( pData->llResultMoney ); // ���� �Ӵ� 
				
				strM.Append(strChip_Name[g_RI.ChipKind]);

				// ��� ������� ���
				if(g_RI.ChipKind==1)
				{					
					// ������ ���� �̺�Ʈ ���� ��� ǥ��
					if (m_stGameOverResultData.CommitionDiscount)
					{
						strM.Append("(������ ���� �̺�Ʈ��)");
					}
				}
				
				str.Format("�� ȹ��ݾ�:%s \n", strM); // ���� ���� �Ƶ� ## ���̽�:%s(+%s)	
				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_WINNER));

				str = "";
				str.Format("�� ����: %s \n", pData->strValueName.c_str() );
				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_JOKBO));

				// 				str.Format(" ����������������������������\n");
				// 				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT));	
			}
			else
			{
				INT64 lostmoney = -pData->llResultMoney;
				strM = (CString)NMBASE::UTIL::g_MoneyMark( pData->llResultMoney );
				str.Format(" %s : ", pData->strID.c_str());
				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_LOSER));
				strM.Append(strChip_Name[g_RI.ChipKind]);
				str.Format("%s\n", strM );
				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_LOSEMONEY));
			}
		}
	}


	for(int i = 0; i < MAX_PLAYER; ++i)
	{
		if ( m_cPlayer[ i ].JoinState == EGAME_JOININ )
		{
			g_pGameView->m_cPlayerDrawManager.SetPlayerGameOver( i, true );// ���� ���� ����
		}		
	}

	//���� ������ ������ ���� ������ ���𰡵� or ���� �ƹ�Ÿ  or ���Ĩ ������ �����Ѵ�.
	if ( m_stAfterEndingUserInfo.bChanged == 1 )
	{
		for( int i = 0 ; i < MAX_PLAYER ; i ++ )
		{
			if ( strlen(m_cPlayer[i].UI.ID) == 0 )continue;
			if ( strcmp(m_cPlayer[i].UI.ID,m_stAfterEndingUserInfo.EndUserInfo[i].ID) == 0 )
			{
				if ( m_stAfterEndingUserInfo.EndUserInfo[i].nPMoney > 0 )
				{
					/*m_cPlayer[i].UI.SetMoney(m_stAfterEndingUserInfo.EndUserInfo[i].nPMoney, IsTournament());*/
					m_cPlayer[i].UI.SetMoney(m_stAfterEndingUserInfo.EndUserInfo[i].nPMoney);

					if ( strcmp(GM().GetMyInfo()->UI.ID , m_stAfterEndingUserInfo.EndUserInfo[i].ID) == 0 && !g_ObserverMan.IsMyObserver())
					{
						/*GM().GetMyInfo()->UI.SetMoney(m_stAfterEndingUserInfo.EndUserInfo[i].nPMoney, IsTournament());*/
						GM().GetMyInfo()->UI.SetMoney(m_stAfterEndingUserInfo.EndUserInfo[i].nPMoney);
					}
				}				
			}
		}
	}	

	//���� ������ �̺�Ʈ ���� (ī�� ��ģ�Ŀ� ���;� �ϴ� ������)
	Draw_TextInformationMsg();
}

BOOL GameProcess_BD::OnCommand(WPARAM wParam ,  LPARAM  lParam )
{
	// TODO: Add your specialized code here and/or call the base class
	switch(wParam)
	{
	case IDM_BADUSER:
		{
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_CHATWINDOW_CHATTING_REPORT);
			//UPG�۾� ������ �� ����� �Ʒ� �� �� �߰�
			GM().BadUserReportGameChat();
		}break;
		/*=====================//���� Ŀ���=====================*/
	case IDM_DIE:
		{
			// ���� ���		
			OnBtnFold();
		}
		break;

	case IDM_CHECK:
		{
			// ���� ���		
			OnBtnCheck();		
		}
		break;

	case IDM_CALL:
		{
			// ���� ���		
			OnBtnCall();

		}
		break;

	case IDM_PING:
		{
			// ���� ���		
			OnBtnPing();
		}
		break;

	case IDM_DADANG:
		{
			// ���� ���		
			OnBtnDda();
		}
		break;	

	case IDM_FULL:
		{
			// ���� ���		
			OnBtnDda();
		}
		break;	

	case IDM_HALF:
		{
			// ���� ���			
			OnBtnMax();
			g_pGameView->m_cGameViewBtnMan.Reset();
		}
		break;

		//�ٵ���
	case IDM_CHANGE_BTN:
		{
			OnBtnChange();
			//������ : ����ڵ�
			g_pGameView->m_cGameViewBtnMan.m_bChangePassState = false;

			break;
		}

	case IDM_PASS_BTN:
		{
			OnBtnPass();
			//������ : ����ڵ�
			g_pGameView->m_cGameViewBtnMan.m_bChangePassState = false;

			break;	
		}

	case IDM_GRADE:
		{
#ifdef UPGRADE_10210619_ADD_CLICKLOG
			SendUILog_Basic(CSSTATUS_UINO_POKER_ROOM_TITLEBAR_GAME_USE_GRADE);
#endif //UPGRADE_10210619_ADD_CLICKLOG

			effect::CGradeEffect* pEffect = NULL;
			SAFE_NEW( pEffect, effect::CGradeEffect );
			pEffect->Init(0,&g_pGameView->Page);
			g_pGameView->m_cAniSprListManager.ADDObject( pEffect );

		} break;
	case IDM_BTN_EXCESSMONEY:
		{
			break;
		}
		//< CYS 101221
	case IDM_GAME_START:
		{
			if ( g_RI.State == 0 )
			{
				CSV_ASKGAMESTART gstart;
				gstart.Set( GM().GetMyInfo()->UI.ID, g_RI.RoomNum );
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), gstart.pData, gstart.GetTotalSize());
			}
			break;
		}
		//> CYS 101221

		//ī������ �ڵ�����
	case IDM_GAME_AUTOUP:
		{
			NMBASE::SKINGDI::CGraphButton* pAutoUpBtn = g_pGameView->m_cGameViewBtnMan.GetGraphicBtn( CGameButtonBD::BTN_AUTOUP );

			// �������� ��������Ʈ No�� ������� üũ�Ѵ�.
			if ( pAutoUpBtn )
			{
				int iButtonNumber = pAutoUpBtn->GetButtonNumber();  // Spr Index 
				pAutoUpBtn->SetSprite( &GLOBAL_SPR[ spr::GSPR_BTN_AUTOUP ], !iButtonNumber ); // 0 ī�弱��ON, 1 ī�弱��OFF 				
				g_Config.nAutoUpType = iButtonNumber; // 0 : ������ 1: ��� 2 : ���� ��
				NMBASE::UTIL::WriteProfile_int( GM().GetMyInfo()->UI.ID, "AutoUpType", g_Config.nAutoUpType ); 

				//Ŭ���α׸� �����.
				if ( g_Config.nAutoUpType == TRUE ){
					SendUILog_Basic( CSSTATUS_UINO_LOWBD_ROOM_MYCARDAREA_AUTO_SELECT_TO_ON );
				}else{
					SendUILog_Basic( CSSTATUS_UINO_LOWBD_ROOM_MYCARDAREA_AUTO_SELECT_TO_OFF );
				}
			}
			break;
		}
		//�ڵ����� ��ư�߰�:2012.09.27 YS.Jeong
	case IDM_GAME_AUTOSORT:
		{
			NMBASE::SKINGDI::CGraphButton* pAutoSortBtn = g_pGameView->m_cGameViewBtnMan.GetGraphicBtn( CGameButtonBD::BTN_AUTOSORT );

			// �������� ��������Ʈ No�� ������� üũ�Ѵ�.
			if ( pAutoSortBtn )
			{
				int iButtonNumber = pAutoSortBtn->GetButtonNumber();  // Spr Index 
				pAutoSortBtn->SetSprite( &GLOBAL_SPR[ spr::GSPR_BTN_AUTOSORT ], !iButtonNumber ); // 0 ī������ON, 1 ī������OFF 
				g_Config.nAutoSortType = iButtonNumber; // 0 : ������ 1: ��� 2 : ���� ��
				NMBASE::UTIL::WriteProfile_int( GM().GetMyInfo()->UI.ID, "AutoSortType", g_Config.nAutoSortType ); 

				AutoSort_Recommand(0, TRUE); 

				//Ŭ���α׸� �����.
				if ( g_Config.nAutoSortType == TRUE ){
					SendUILog_Basic( CSSTATUS_UINO_LOWBD_ROOM_MYCARDAREA_AUTO_SORT_TO_ON );
				}else{
					SendUILog_Basic( CSSTATUS_UINO_LOWBD_ROOM_MYCARDAREA_AUTO_SORT_TO_OFF );
				}
			}
			break;
		}
	// ���� ī�� ���� ��ư
	case IDM_CARD_OPEN:
		{
			// ��ư spr �ε����� 0�̸� ���� ī�� ���� On, 1 �̸� Off
			int nPrevBtnNum = g_pGameView->m_cGameViewBtnMan.m_btnCardOpen.GetButtonNumber(); // ���� ��ư ����(On or Off)
			//int nBtnNum = 0;
			bool bUIFoldOpen = false;

			// ��ư�� Off �� On ������ -> ��ư ���� ��ư�� ���� ����� ���� �ϱ�� ��
			if (nPrevBtnNum == 1)
			{
				bUIFoldOpen = true;
			}
			
			// ��ư spr �ε����� Toggle ��Ŵ
			//g_pGameView->m_cGameViewBtnMan.m_btnCardOpen.SetSprite(&(g_pGameView->m_cGameViewBtnMan.m_sprUICardOpenBtn), nBtnNum);
			
			// ������ �˸�
			CCL_ASK_CARDOPEN msg;
			msg.Set(bUIFoldOpen);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
	default:
		{
			if ( LAYER_GLOBAL_AUTOBETWND()->OnCommand(wParam) )
			{
				return true;
			}

			if ( g_pChatWinMan )
			{
				g_pChatWinMan->OnCommand(wParam);
			}

			//��������
			if(g_cOneShotCharge.OnCommand(wParam, lParam)) return TRUE;

			//�ڸ� ��Ŵ��
			if(g_SeatKeeper.OnCommand(wParam, lParam)) return TRUE;

			return false;
		}
	}

	return TRUE;
}

// [���� ��Ŀ] ���� ��ư
void GameProcess_BD::OnBtnPing( bool bUserAction )
{
	if(g_ObserverMan.IsMyObserver() == TRUE) return;
	if( !g_pGameView->m_cGameViewBtnMan.m_bBPing || g_PLAYER(0)->PlayState == 0) return;

	// Ÿ�Ӿƿ� ī��Ʈ �ʱ�ȭ
	if ( bUserAction )
	{
		m_cPlayer[ 0 ].ResetTurnLimitCnt();
	}

	// ������ ����Ǿ� �־ ���¹��� üũ�� �����ؼ� ������ �����ϸ� ���� ��Ҹ� ���ؼ� �־���.
	SetReserveButton(RESERVESTATE_BET_NONE);

	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();

	POKERCLNT pc; //MakeBasic(pc);
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 3; // ����
	pc.PlayTick = g_pGameView->GetCurrentPlayTick(); 


	pc.nBtn   = BET_CMD_PING ; // ��

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  g_PLAYER(0)->UI.UNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.ID, &pc);
	SetCardExchangeLog(raise);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), raise.pData, raise.GetTotalSize());

	m_bBtnClick = TRUE;

}

// ���� -> Ǯ��ư ������� ����(�����Ȱ��� �۾� 2014.01)
void GameProcess_BD::OnBtnDda( bool bUserAction )
{
	if(g_ObserverMan.IsMyObserver() == TRUE) return;
	if( !g_pGameView->m_cGameViewBtnMan.m_bBDDa || g_PLAYER(0)->PlayState == 0) return;

	// 2014.2.7 crpark
	// ������ ���ÿ��� ����ȭ
	if(false == CheckMyTurn(GetPlayer(0)->ServPNum, m_nCurPlayer))
	{
#if defined(BUTTON_CHANGE_FULLTODADANG)
		if(true==GM().IsChangeFullDDang()){
			SetReserveButton(RESERVESTATE_BET_BET_CMD_FULL);
		}
		else{
			SetReserveButton(RESERVESTATE_BET_BET_CMD_DADANG);
		}
#else
		SetReserveButton(RESERVESTATE_BET_BET_CMD_DADANG);
#endif
		return;
	}
	SetReserveButton(RESERVESTATE_BET_NONE);

	// Ÿ�Ӿƿ� ī��Ʈ �ʱ�ȭ
	if ( bUserAction )
	{
		m_cPlayer[ 0 ].ResetTurnLimitCnt();
	}

	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();

	POKERCLNT pc; //MakeBasic(pc);
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 3; // ����
	pc.PlayTick = g_pGameView->GetCurrentPlayTick(); 

#if defined(BUTTON_CHANGE_FULLTODADANG)
	if(true==g_RI.bIsFriendRoom && m_cPlayer[ 0 ].UI.GetMoney() >= GM().GetLossMaxMoneyForGame()){
		pc.nBtn   = BET_CMD_MAX_BET;
	}
	else if(true==GM().IsChangeFullDDang()){
		pc.nBtn   = BET_CMD_FULL;
	}
	else{
		pc.nBtn   = BET_CMD_DADANG;//BET_CMD_FULL;//BET_CMD_DADANG ; // ���翡�� Ǯ�α�ü(�����Ȱ��� �۾� 2014.01)->�����۾�
	}
#else
	pc.nBtn   = BET_CMD_DADANG;//BET_CMD_FULL;//BET_CMD_DADANG ; // ���翡�� Ǯ�α�ü(�����Ȱ��� �۾� 2014.01)->�����۾�
#endif

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  g_PLAYER(0)->UI.UNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.ID, &pc);
	SetCardExchangeLog(raise);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), raise.pData, raise.GetTotalSize());
	m_bBtnClick = TRUE;
}


void GameProcess_BD::OnBtnFold( bool bUserAction )
{
	if(g_ObserverMan.IsMyObserver() == TRUE) return;
	if(!g_pGameView->m_cGameViewBtnMan.m_bFold || g_PLAYER(0)->PlayState == 0) return;


	if( g_Poker.nAutoBetCmd > 0 && g_RI.nAutoBetKind > 0 ){
		return;
	}

	// 2014.2.7 crpark
	// ������ ���ÿ��� ����ȭ
	if(false == CheckMyTurn(GetPlayer(0)->ServPNum, m_nCurPlayer))
	{
		SetReserveButton(RESERVESTATE_BET_BET_CMD_FOLD);
		return;
	}

	SetReserveButton(RESERVESTATE_BET_NONE);
	//������ :  ���� ���ɿ��� ����
	CanReserve(false);
	
	// Ÿ�Ӿƿ� ī��Ʈ �ʱ�ȭ
	if ( bUserAction )
	{
		m_cPlayer[ 0 ].ResetTurnLimitCnt();
	}

	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();

	POKERCLNT pc; 
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 0; // ��������
	pc.nBtn   = 0; // ��
	pc.PlayTick = g_pGameView->GetCurrentPlayTick(); 

	CCL_ASK_FOLD fold;
	fold.Set(g_RI.RoomNum,  g_PLAYER(0)->UI.UNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.ID, &pc);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), fold.pData, fold.GetTotalSize());
	m_bBtnClick = TRUE;	
}

void GameProcess_BD::OnBtnCheck( bool bUserAction )
{
	if(g_ObserverMan.IsMyObserver() == TRUE) return;
	if( !g_pGameView->m_cGameViewBtnMan.m_bBCheck || g_PLAYER(0)->PlayState == 0) return;

	// Ÿ�Ӿƿ� ī��Ʈ �ʱ�ȭ
	if ( bUserAction )
	{
		m_cPlayer[ 0 ].ResetTurnLimitCnt();
	}

	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();

	POKERCLNT pc; //MakeBasic(pc);
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 1; // ����
	pc.nBtn   = 0; // ��
	pc.PlayTick = g_pGameView->GetCurrentPlayTick(); 

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  g_PLAYER(0)->UI.UNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.ID, &pc);
	SetCardExchangeLog(raise);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), raise.pData, raise.GetTotalSize());
	m_bBtnClick = TRUE;
}

void GameProcess_BD::OnBtnCall( bool bUserAction )
{
	if(g_ObserverMan.IsMyObserver() == TRUE) return;
	if( !g_pGameView->m_cGameViewBtnMan.m_bBCall || g_PLAYER(0)->PlayState == 0) return;

	// 2014.2.7 crpark
	// ������ ���ÿ��� ����ȭ
	if(false == CheckMyTurn(GetPlayer(0)->ServPNum, m_nCurPlayer))
	{
		SetReserveButton(RESERVESTATE_BET_BET_CMD_CALL);
		return;
	}
	SetReserveButton(RESERVESTATE_BET_NONE);


	// Ÿ�Ӿƿ� ī��Ʈ �ʱ�ȭ
	if ( bUserAction )
	{
		m_cPlayer[ 0 ].ResetTurnLimitCnt();
	}

	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();

	POKERCLNT pc; 
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 2; // ����
	pc.nBtn   = 0; // ��
	pc.PlayTick = g_pGameView->GetCurrentPlayTick(); 

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  g_PLAYER(0)->UI.UNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.ID, &pc);
	SetCardExchangeLog(raise);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), raise.pData, raise.GetTotalSize());
	m_bBtnClick = TRUE;
}

//����
void GameProcess_BD::OnBtnMax( bool bUserAction )
{
	if(g_ObserverMan.IsMyObserver() == TRUE) return;
	if( !g_pGameView->m_cGameViewBtnMan.m_bBMax || g_PLAYER(0)->PlayState == 0 ) return;

	// 2014.2.7 crpark
	// ������ ���ÿ��� ����ȭ
	if(false == CheckMyTurn(GetPlayer(0)->ServPNum, m_nCurPlayer))
	{
		SetReserveButton(RESERVESTATE_BET_BET_CMD_HALF);
		return;
	}
	SetReserveButton(RESERVESTATE_BET_NONE);


	// Ÿ�Ӿƿ� ī��Ʈ �ʱ�ȭ
	if ( bUserAction )
	{
		m_cPlayer[ 0 ].ResetTurnLimitCnt();
	}

	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();

	POKERCLNT pc; //MakeBasic(pc);
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 3; // ����

	int spnum = GetServPNum_ByPN(0);			
	INT64 CallMoney = GetRaiseBat() - g_Poker.RU[spnum].nNowBat;
	INT64 nTotal = GetRealBet();

	if( m_cPlayer[ 0 ].GetNowMoney() > CallMoney && m_cPlayer[ 0 ].GetNowMoney() < GetMaxMoney( nTotal ))
	{
		pc.nBtn   = BET_CMD_ALLIN; // ����
	} 
	else{
		pc.nBtn   = BET_CMD_HALF; // �ƽ�
	}

	pc.PlayTick = g_pGameView->GetCurrentPlayTick(); 

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  g_PLAYER(0)->UI.UNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.ID, &pc);
	SetCardExchangeLog(raise);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), raise.pData, raise.GetTotalSize());
	m_bBtnClick = TRUE;	
}

// bPaticipation = TRUE  ���� -> ����
// bPaticipation = FALSE ���� -> ����
void GameProcess_BD::SetPlayInfo(int nowpnum, int newpnum, BOOL bParticipation )
{
	//����� - ������ ����ұ�?
	USERINFO tmpUi[MAX_PLAYER];
	PLAYSTATE tmpPs[MAX_PLAYER] = {0,};
	BACKCARD  tmpBC[MAX_PLAYER] = {0,};
	CHANGE_USERINFO_ROOM tmpCUi[MAX_PLAYER];

	ZeroMemory(tmpUi, sizeof(USERINFO) * MAX_PLAYER);
	ZeroMemory(tmpCUi, sizeof(CHANGE_USERINFO_ROOM) * MAX_PLAYER);

	struct tempcarddeck{
		char nCard[10];

		CUTCOUNT_INFO stCutInfo; //Ŀ�� Ƚ�� ����
	};

	int				tmpSelectCard[MAX_PLAYER] = {0,};
	tempcarddeck	tCardDeck[MAX_PLAYER] = {0,};	

	if( !bParticipation ) {
		//���� ������ ���̱� ������ 0 �� �ε���
		//�⺻ ���� �ʱ�ȭ
		m_cPlayer[0].Clear();
	}	

	// ���� �÷��� ���� backup
	int pn = 0, k;
	for(int i=0; i<MAX_PLAYER; i++) {
		if(strlen(m_cPlayer[i].UI.ID)==0) continue;
		if(bParticipation )// ���� -> ����
			pn = i;
		else
			pn = GetServPNum_ByPN(i);
		memcpy(&tmpUi[pn], &m_cPlayer[i].UI, sizeof(USERINFO));
		memcpy(&tmpCUi[pn], m_cPlayer[i].GetChangeRoomInfo(), sizeof(CHANGE_USERINFO_ROOM) );
		tmpPs[pn].JoinState = m_cPlayer[i].JoinState;
		tmpPs[pn].PlayState = m_cPlayer[i].PlayState;

		tmpPs[pn].bFold = m_cPlayer[i].bFold;
		tmpPs[pn].nCardTotal= m_cPlayer[i].m_nCardTotal;				

		//�����κ��� �ٽ� �޴´�
		// ### [ �ߺ��� IP�� ] ###
		//tmpPs[pn].nCheckIp = m_cPlayer[i].MyIpCheck;
		// ### [���� �߰� �۾�] ###
		tmpPs[pn].nSndFxKind = m_cPlayer[i].nSndFxKind;
		tmpPs[pn].nOpenTotal= m_cPlayer[i].m_nOpenTotal;

		int tcnt = m_cPlayer[i].m_nCardTotal;
		for(k = 0; k < tcnt ; k++) {
			tCardDeck[pn].nCard[k] = m_cPlayer[i].m_cMyCard[ k ].GetCardNo();
		}
		
		// Ŀ�� ���� ����( Ŀ�� Ƚ�� �����ΰ��� ��� 3ȸ �Է� )
		for(int c = 0; c < 3 ; c++) 
		{
			tCardDeck[ pn ].stCutInfo.m_arrCutCount[ c ] = m_cPlayer[ i ].GetCutInfo().m_arrCutCount[ c ];
			tCardDeck[ pn ].stCutInfo.m_arrbShow[ c ] = m_cPlayer[ i ].GetCutInfo().m_arrbShow[ c ];
		}
	}

	//�⺻ ���� �ʱ�ȭ
	if(bParticipation) {   // ���� -> ����
		GM().GetMyInfo()->JoinState = 0;
		m_nMyPosPNum = newpnum; // �����ʿ� ���ο� �ε���
		m_nNo1PosPNum = GetPNum_ByServPN(0); // �������� 0 �� �ε���

		m_cPlayer[nowpnum].Clear();
	}
	else {	//���� -> ����
		GM().GetMyInfo()->JoinState = 0;
		m_nMyPosPNum = 0; // �����ʿ� ���ο� �ε���
		m_nNo1PosPNum = GetPNum_ByServPN(0); // �������� 0 �� �ε���
	}

	g_pGameView->m_cPlayerDrawManager.OnExitRoom();

	// ���� ���� ��� �ʱ�ȭ
	for( int i=0; i<MAX_PLAYER; i++) {
		m_cPlayer[i].Clear();
		m_cPlayer[i].ServPNum = GetServPNum_ByPN(i); // �ʱ�ȭ ���� �ʴ´�.
	}


	if(bParticipation ) {	// ���� -> ����
		GM().GetMyInfo()->UI.cObserver = 0;
		g_ObserverMan.SetMyObserver(FALSE);
		GM().GetMyInfo()->Reset();
		GM().GetMyInfo()->nSndFxKind = g_Config.SndFxKind;
		m_cPlayer[0].SetNewPlayer(&GM().GetMyInfo()->UI);
		m_cPlayer[0].SetChangeRoomInfo(*GM().GetMyInfo()->GetChangeRoomInfo());
		m_cPlayer[0].nSndFxKind = g_Config.SndFxKind;

		g_RI.NowUserNum++;
		if(g_RI.NowObserverNum > 0) g_RI.NowObserverNum--;

		GM().GetMyInfo()->ServPNum = m_cPlayer[0].ServPNum;

		//������ �̹��� �׸��� ����
		g_pGameView->m_cStayAniSprListManager.Del_AniListOBj(ANF_PLAYER_OBSERVER);		
	}
	else {   //���� -> ����
		GM().GetMyInfo()->UI.cObserver = 1;
		g_ObserverMan.SetMyObserver(TRUE);
		GM().GetMyInfo()->Reset();

		m_cPlayer[newpnum].SetNewPlayer(&GM().GetMyInfo()->UI);
		m_cPlayer[newpnum].SetChangeRoomInfo(*GM().GetMyInfo()->GetChangeRoomInfo());
		m_cPlayer[newpnum].nSndFxKind = GM().GetMyInfo()->nSndFxKind;

		GM().GetMyInfo()->ServPNum = newpnum;
		m_cPlayer[newpnum].ServPNum = newpnum;

		g_RI.NowObserverNum++;
		if(g_RI.NowUserNum > 0) g_RI.NowUserNum--;

		//������ �̹��� �׸���


		//����UI		
		g_ObserverMan.OnObserve();
	}

	// �÷��̾� ������ �°� �ʱ�ȭ�Ѵ�
	for( int i=0; i<MAX_PLAYER; i++) 
	{
		int pn = GetPNum_ByServPN(i);
		if ( strlen(tmpUi[i].ID) > 0 )	
		{
			if ( bParticipation ) 
			{
				if(pn!=0) m_cPlayer[pn].SetNewPlayer(&tmpUi[i]);
			}
			else 
			{
				m_cPlayer[pn].SetNewPlayer(&tmpUi[i]);
			}

			m_cPlayer[pn].JoinState = tmpPs[i].JoinState;
			m_cPlayer[pn].PlayState = tmpPs[i].PlayState;			
			m_cPlayer[pn].nSndFxKind = tmpPs[i].nSndFxKind; // ### [���� �߰� �۾�] ###

			// �������� ���
			/*m_cPlayer[pn].UI.nIcon = GetMoneyGrade(tmpUi[i].GetTotalMoney(IsTournament()));*/
			m_cPlayer[pn].UI.nIcon = GetMoneyGrade(tmpUi[i].GetTotalMoney());
			m_cPlayer[pn].SetChangeRoomInfo(tmpCUi[i]);
			
			// Ŀ�� ���� ����( Ŀ�� Ƚ�� �����ΰ��� ��� 3ȸ �Է� )
			for(int c = 0; c < 3 ; c++) 
			{
				m_cPlayer[ pn ].GetCutInfo().m_arrCutCount[ c ] = tCardDeck[ i ].stCutInfo.m_arrCutCount[ c ];
				m_cPlayer[ pn ].GetCutInfo().m_arrbShow[ c ] = tCardDeck[ i ].stCutInfo.m_arrbShow[ c ];
			}

			if ( g_Poker.nState <= RSTATE_RESULT ) //< CYS 101207 > ���Ӱ������ ī�尡 �ִٸ� �����ش�.
			{
				// ���� ���� ��Ȳ �׸��� ī�� ������ ī�嵦���ٰ� ��
				if ( m_cPlayer[pn].JoinState == 1 ) 
				{
					int nTotCnt = tmpPs[i].nCardTotal;
					int nOpCnt =  tmpPs[i].nOpenTotal;

					m_cPlayer[pn].bFold = tmpPs[i].bFold;
					m_cPlayer[pn].m_nCardTotal = nTotCnt;
					m_cPlayer[pn].m_nOpenTotal = nOpCnt;
					for(k = 0; k < nTotCnt; k++) {
						m_cPlayer[pn].SetNewCard(DUMY_CARD); // ī��Ʈ �ڵ�����
						CPoint pt;
						pt.x = m_cPlayer[pn].m_cMyCard[ k].m_Xp;
						pt.y = m_cPlayer[pn].m_cMyCard[ k].m_Yp;
						m_cPlayer[pn].m_cMyCard[ k].SetMovingCurPos(pt);
						m_cPlayer[pn].ShowCard(k);
					}				
					for(k = 0; k < nTotCnt; k++) 
					{
						m_cPlayer[pn].m_cMyCard[ k].SetCardNo( tCardDeck[i].nCard[k] );			
					}				
				}
			}			
		}
	}

	for( int i=0; i<MAX_PLAYER; i++) 
	{
		if ( m_cPlayer[ i ].UI.ID[0] != 0 )
		{
			g_pGameView->m_cPlayerDrawManager.OnEnterRoom(i);
		}
	}

	// bPaticipation = TRUE  ���� -> ����
	// bPaticipation = FALSE ���� -> ����
	if(bParticipation) {
		m_pObserverDlg->User_DelItem(m_cPlayer[0].UI.UNum, m_cPlayer[0].UI.ID);
	}
	else {
		int ounum = m_cPlayer[newpnum].UI.UNum;
		int sex  = m_cPlayer[newpnum].UI.Sex;
		int level = m_cPlayer[newpnum].UI.nIcon;
		/*INT64 Money = m_cPlayer[newpnum].UI.GetMoney(IsTournament());*/
		INT64 Money = m_cPlayer[newpnum].UI.GetMoney();
		int seq = 0;
		m_pObserverDlg->User_InsItem(ounum,m_cPlayer[newpnum].UI.ID, m_cPlayer[newpnum].UI.NickName, sex, level, Money, seq);
	}

	//	g_pGameView->m_cGameViewBtnMan.SetObserverBtn();

	CString str;

	if ( bParticipation ) 
	{
		str.Format("���� �� ����\n");
	}
	else 
	{
		str.Format("���� �� ����\n");
	}
	AddGameInfoViewEdit( str, GetChatColor( ECC2_NOTIFY ) );


	// ������� ����
	if ( !g_ObserverMan.IsMyObserver() )
	{		
		LAYER_GLOBAL_AUTOBETWND()->OnEnterRoom();
	}

	//< CYS 110124 ������ ��ư ����
	NMBASE::SKINGDI::CGraphButton* pAutoUpBtn = g_pGameView->m_cGameViewBtnMan.GetGraphicBtn( CGameButtonBD::BTN_AUTOUP );

	if ( pAutoUpBtn )
	{
		if ( g_ObserverMan.IsMyObserver() )
		{
			pAutoUpBtn->Show( false );
		}
		else
		{
			pAutoUpBtn->Show( true );
		}
	}
	//> CYS 110124

	
	//������ �ڵ����Ĺ�ư ����:2012.09.27 YS.Jeong
	NMBASE::SKINGDI::CGraphButton* pAutoSortBtn = g_pGameView->m_cGameViewBtnMan.GetGraphicBtn( CGameButtonBD::BTN_AUTOSORT );

	if ( pAutoSortBtn )
	{
		if ( g_ObserverMan.IsMyObserver() )
		{
			pAutoSortBtn->Show( false );
		}
		else
		{
			pAutoSortBtn->Show( true );
		}
	}
	

}

BOOL GameProcess_BD::IsPlayingNow()
{
	return m_bGameStart;
}

//G_TFT
void GameProcess_BD::Change_Money(CSV_CHANGE_MONEY *pMsg)
{	
	const char *pUserID = pMsg->ID;
	const char *pUMCode = pMsg->UMCode;
	INT64 ChangeMoney = *pMsg->ChangeMoney;
	INT64 PMoney = *pMsg->PMoney;

	//pUserID : �Ӵ� ��ȭ�� �Ͼ �������̵� (���� �ɼ��� �ְ� ���� �ִ� ���� �ٸ� ������ �� �����ִ�.)
	//pUMCode : �Ӵ� ��ȭ�� �Ͼ ��Ÿ�ڵ�
	//ChnageMoney : ��ȭ�� �ӴϷ�
	//PMoney : ��ȭ �� �Ӵ� 

	if(strcmp(pUserID, GM().GetMyInfo()->UI.ID) == 0) //���Ӵ� ���?
	{
		/*GM().GetMyInfo()->UI.SetMoney(PMoney, IsTournament());*/
		/*GM().GetMyInfo()->UI.SetMoney(PMoney);*/
		GM().GetMyInfo()->UI.SetRealPMoney(PMoney);
		if( !g_ObserverMan.IsMyObserver() ) 
		{
			/*g_PLAYER(0)->UI.SetMoney(PMoney, IsTournament());*/
			/*g_PLAYER(0)->UI.SetMoney(PMoney);*/
			g_PLAYER(0)->UI.SetRealPMoney(PMoney);
			//g_PLAYER(0)->PrevMoney = g_PLAYER(0)->UI.GetPMoney();//PMONEY , PMONEY2 ���սÿ� �Ʒ� �츮�� //@#$%
		}
	}
	else	//�����̶��?
	{
		int pnum = GetPlayerPNum(pUserID);
		if(pnum != -1) {								
			/*m_cPlayer[ pnum ].UI.SetMoney(PMoney, IsTournament());*/
			/*m_cPlayer[ pnum ].UI.SetMoney(PMoney);*/
			m_cPlayer[ pnum ].UI.SetRealPMoney(PMoney);
		}
	}

	CString strUMCode = pUMCode;
	/////////////////////////////////////////////////////////////
	//�� ���̽����� �ִϸ��̼� ȿ�� , �˾� ���� �����ش�.
	// �ѷ��� �޼����� �����Ѵ�.
	CString sysmsg;	

	if( strUMCode == "ABB02019" || strUMCode == "ABB02020" || strUMCode == "ABB02021" ) //����(���)���� ��Ÿ�ڵ� 
	{
		//����(���)����
		g_cOneShotCharge.OnChargeMoney(*pMsg);
	}


	if(!sysmsg.IsEmpty()) {
		if(GetCurrentWhere()==IDX_GAMEWHERE_LOBY)
		{
			//UPG�۾� �߰�
			AddLobbyChatViewEdit(sysmsg, RGB(0,44,93));
		}
		if(GetCurrentWhere()==IDX_GAMEWHERE_GAME)
		{
			//g_pGameView->m_cChatWindow.GetChatView()->AddText( &sysmsg, RGB(250,250,0) );

			AddGameInfoViewEdit( sysmsg, GetChatColor(ECC2_SERVMSG) );
		}
	}
	if(GetCurrentWhere()==IDX_GAMEWHERE_LOBY){
		MM().Call(UPG_CMK_RESET_GAMEMONEY, 0, 0);
	}

}

//UPG�۾� �߰�
void GameProcess_BD::OnEnterRoom(BOOL bCreation)
{
	//���� �濡 ���� (bCreation�� ���� �����ؼ� �����ϴ��� ����)

	// ���� ī�� ���� ��ư ���� Off�� �ʱ�ȭ(1�̸� Off, 0�̸� On)
	g_pGameView->m_cGameViewBtnMan.m_btnCardOpen.SetSprite(&(g_pGameView->m_cGameViewBtnMan.m_sprUICardOpenBtn), 1);

	// ���Ӻ���� �׻� �ʱ�ȭ
	g_pGameView->m_cGameViewBtnMan.InitializeKeyValue( g_Config.eBetBtnType );// �ٵ��� Ŀ��Ű �ʱ�ȭ����

	std::string strMsg;
#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
		strMsg = "���� �ο�ٵ���";
	else
		strMsg = "������ �ο�ٵ���";
#else //UPGRADE_20120308_TOCTOC_CHANNELING
	strMsg = "������ �ο�ٵ���";
#endif //UPGRADE_20120308_TOCTOC_CHANNELING

	SetMainWindowText( strMsg.c_str() );

	g_pGameView->m_cBackGround.OnEnterRoom();

	g_pGameView->m_cSidePotMoney.Reset();
	g_pGameView->m_cGameViewBtnMan.m_btnExitBottom.Enable(TRUE);
	g_pGameView->m_cGameViewBtnMan.InitObserverBtn();
	//ä��
	g_pChatWinMan->SetViewPage(CChattingWindowMan::ECTT_CHAT);
	g_pChatWinMan->ResetChatting_STR();

	// ��ʸ�Ʈ�϶����� �Ӹ� ��� ����Ʈ ���� ��ư enable
	//�Ӹ� ����
	/*
	if (IsTournament())
		g_pChatWinMan->SetLiftBtnEnable(false);
	else
		g_pChatWinMan->SetLiftBtnEnable(true);
	*/

	// ��Ŀ��
	g_pGameView->SetFocus();
	g_pGameView->Reset();	
	strMsg = "�ٵ��򸻡�\n1. ����â->ä��â ��Ŀ�� �̵�\n����Ű - Enter, ESC\n2. ä��â->����â ��Ŀ�� �̵�\n����Ű - ESC\n";		
	AddGameInfoViewEdit( strMsg.c_str(), GetChatColor( ECC2_NOTIFY ));

	g_pGameView->m_cGameViewBtnMan.SetBetBtnType( g_Config.eBetBtnType, true );
	g_pGameView->m_cGameViewBtnMan.EnterRoom_Button_Init( g_RI.sBetRuleKind );
	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();

	strMsg = "���������� ��������(¥��ġ��) �� ���༺ ���� ����(�ŷ�,����)���� �߻��� �� ���� �� �Ű� ��ư�� �̿��Ͽ�, �Ű��� �ֽñ� �ٶ��ϴ�.\n";
	AddGameInfoViewEdit( strMsg.c_str(), GetChatColor(ECC2_NOTIFY) );

	//�ڸ���Ŵ��
	g_SeatKeeper.OnEnterGame();
	g_cOneShotCharge.OnEnterRoom();
	g_ObserverMan.OnEnterRoom();

	g_cVipJackpot.OnEnterRoom();

	//�����ѵ�
	g_cOwnLimit.OnEnterRoom(&g_cUIData, g_GAME_BD());

	//������ ä�� ���
	m_bObserverChatWarning = FALSE;

	// �����ǿ� ���� ���
	if ( !g_ObserverMan.IsMyObserver() )
	{
		CheckMinimumUser();
	}

	CGameProcess::OnEnterRoom(bCreation);

	CString str;
	if(g_ObserverMan.IsMyObserver())
	{
		str.Format("������ ����: [%s]��\n", GM().GetMyInfo()->UI.NickName);
	}
	else
	{
		str.Format("������: [%s]��\n", GM().GetMyInfo()->UI.NickName);
	}

	AddGameInfoViewEdit(str, GetChatColor(ECC2_NOTIFY));

	str.Format("## <ID: %s(%s)>�� ���� ##\n", GM().GetMyInfo()->UI.ID , GM().GetMyInfo()->UI.NickName);
	AddLobbyHideChatText( &str );

	// ���̱ݾ��� �������ش�. 
	// ( ���� ��Ŷ�� �������� ��Ŷ���� �ʰ� �ü� �־ ���� ������ ���ǿ��� ���� ���� �ݾ����� ������ ���ش�. )
	if(GetJackPotMoney() <= 0)
	{
		JackPotMoneyUpdate(&g_JackPotMoneyInfo);
	}
	g_cPromotionMan.OnEnterRoom();
}

//UPG�۾� �߰�
void GameProcess_BD::OnExitRoom()
{
	g_ObserverMan.SetMyObserver(FALSE);

	// ### [ ������� ] ###
	for(int i=1; i<GetMaxNewPlayer(); i++) 
		m_cPlayer[ i ].Clear();


	// �� ��� �� ����� ��� ����Ʈ�� �䱸	
	g_pGameView->m_cAniSprListManager.Destroy();
	g_pGameView->m_cStayAniSprListManager.Destroy();
	g_pGameView->m_cTotalBetMoney.Reset();

	//�ڸ� ��Ŵ��
	g_SeatKeeper.OnOutRoom();
	g_cVipJackpot.OnOutRoom();

	g_pChatWinMan->ResetChatting_STR();

	NMBASE::SOUND::g_Sound.Stop(SND_HIDDEN_BGM);

	m_i64JackPotMoney = 0;
	m_i64OldJackPotMoney = 0;

	// ��ʸ�Ʈ�� ��쿡 �Ӹ� ���� ����Ʈ ��ư�� disable �ϱ� ������ �濡�� ������ ������ ��������Ʈ ��ư�� enable ���ش�.
	//�Ӹ� ����
	//g_pChatWinMan->SetLiftBtnEnable(true);

	CGameProcess::OnExitRoom();

	// ���׷α� Clear()
	//NMBASE::UTIL::BugSplatWrapper::GetInstance().ClearMemLog();
	NMBASE::SOCK::CSockMan::SetSocketLogTxt("");

	//�����ѵ�
	g_cOwnLimit.OnExitRoom();
}


//UPG�۾� �߰� (���� GameDlg::PreTranslateMessage() �� �ִ� �κ��� ������)
BOOL GameProcess_BD::PreTranslateMessage(MSG* pMsg) 
{
	if ( GetCurrentWhere() != IDX_GAMEWHERE_GAME)
	{
		return TRUE;
	}

	///////////////////////////////////////////////////////////////
	if ( pMsg->message == WM_SYSKEYDOWN )
	{
		if((pMsg->lParam & 0x20000000) > 0 && (pMsg->wParam == '1' || pMsg->wParam == '2' || pMsg->wParam == '3' ||	
			pMsg->wParam == '4' || pMsg->wParam == '5')) 
		{			
			int num = (pMsg->wParam - 49)+5;
			if(num >= 0 && num < 10)
			{
				CString str = g_ChatMacro.Macrostr[num];
				g_pChatWinMan->SystemKeyReturn(str);
			}
		}		
	}

	if ( pMsg->message == WM_MOUSEWHEEL ) // ä��â���� ���콺 ��
	{
		if( g_pChatWinMan != NULL && g_pChatWinMan->GetChatView() != NULL )
		{
			g_pChatWinMan->GetChatView()->OnMouseWheel(pMsg->wParam, pMsg->lParam);			
		}		
	} 
	///////////////////////////////////////////////////////////////
	else if ( pMsg->message == WM_KEYDOWN ) 
	{	
		// ����Ű�е� ����Ű �߰� 2004.07.21
		int scankey = 0, extended = 0, previous = 0;
		bool bGameKey = IsSacnCode(pMsg->lParam, scankey, extended, previous);

		// ���� �ٿ��ֱ� ���� �۾�.
		bool bPushCtrl = false;
		bool bPushShift = false;
		if(GetAsyncKeyState(VK_LCONTROL) & 0x8000) bPushCtrl = true;
		if(GetAsyncKeyState(VK_LSHIFT) & 0x8000) bPushShift = true;

		if(bPushCtrl)
		{
			switch( scankey )
			{
			case VK_S_V:		
				return TRUE;
			case VK_S_C:		
				return TRUE;
			case VK_S_X:		
				return TRUE;
			}
		}

		// ���� ����Ʈ�� �ƴ� �ٸ����� ��Ŀ���� �ִٸ� ����Ű�� �ν�
		//if ( g_pGameView->GetFocus() != (CWnd*)g_pChatWinMan->GetSendEdit()->pSendEdit )
		if(!g_pChatWinMan->IsEditFocus())
		{
			//////////
			if ( scankey == rule::SKEY_ENTER && extended == 0 || scankey == rule::SKEY_ESC )
			{
				g_pChatWinMan->GetSendEdit()->pSendEdit->SetFocus();
				return TRUE;
			}

			//�߰�
			OnGameKey( scankey, extended, previous );

			return TRUE;
		}
		else 
		{
			// ��ȭ ������ ������
			// ä�� â���� ���� Ű
			char szChatMsg[ MAX_PATH ] = { 0,};//��ȭ ���� �о� ���� 			

			g_pChatWinMan->GetSendEdit()->pSendEdit->GetWindowText( szChatMsg, 128 );

			if ( strlen( szChatMsg ) == 0 )
			{
				std::vector< DWORD >::iterator iterNotMsg = m_vtExceptionKey_NotMsg.begin();

				while ( iterNotMsg != m_vtExceptionKey_NotMsg.end() )
				{
					if ( IS_VKEY( *iterNotMsg, scankey, extended ) )
					{
						g_pGameView->SetFocus();
						OnGameKey( scankey, extended, previous ); //����
						return TRUE;
					}
					++iterNotMsg;
				}
			}

			// ��ȭ ������ �־
			// ä�� â���� ���� Ű
			std::vector< DWORD >::iterator iterMsg = m_vtExceptionKey_Msg.begin();

			while ( iterMsg != m_vtExceptionKey_Msg.end() )
			{
				if ( IS_VKEY( *iterMsg, scankey, extended ) )
				{
					OnGameKey( scankey, extended, previous ); //����
					return TRUE;
				}
				++iterMsg;
			}

			if ( scankey == rule::SKEY_ESC ) 
			{
				// ���Ӻ信 ��Ŀ���� �ű��
				g_pGameView->SetFocus();
				return TRUE;
			}
		}			
	}
	else if ( pMsg->message == WM_KEYUP )
	{
	}

	return FALSE;
}

//UPG�۾� �Լ��߰�
bool GameProcess_BD::IsSacnCode(int nScanCode, int &r_scankey, int &r_extended, int &r_previous)
{
	// ��ĵ�ڵ带�м���
	// ��ĵŰ�ڵ�
	r_scankey = (nScanCode >> 16) & 0x000000FF; 
	// Ȯ��Ű��������(1�̸�Ȯ��Ű��- ����Ű�е����Ű�º�Ȯ��Ű������)
	r_extended = (nScanCode >> 24) & 0x00000001; 
	// Ű����������(1�̸�������������������)
	r_previous = (nScanCode >> 30) & 0x00000001; 

	// ���ӿ�������ϴ�Ű�ΰ��˻�
	if( ( r_scankey == 79 && r_extended == 0 ) || // Ű�е�1		���λ�		| ���ε���
		( r_scankey == 80 && r_extended == 0 ) || // Ű�е�2		���ε���	| ��������
		( r_scankey == 81 && r_extended == 0 ) || // Ű�е�3		��������	|
		( r_scankey == 75 && r_extended == 0 ) || // Ű�е�4		���δ���	| ���λ�
		( r_scankey == 76 && r_extended == 0 ) || // Ű�е�5		����üũ	| ����üũ
		( r_scankey == 77 && r_extended == 0 ) || // Ű�е�6		������		|
		( r_scankey == 71 && r_extended == 0 ) || // Ű�е�7					| ������
		( r_scankey == 72 && r_extended == 0 ) || // Ű�е�8					| ���δ���
		( r_scankey == 73 && r_extended == 0 ) || // Ű�е�9					| 
		( r_scankey == 82 && r_extended == 0 ) || // Ű�е�0					| ��������
		( r_scankey == 83 && r_extended == 0 ) || // Ű�е�.					| ��������
		( r_scankey == 53 && r_extended == 1 ) || // Ű�е�/					| ��������
		( r_scankey == 55 && r_extended == 0 ) || // Ű�е�*					| ��������
		( r_scankey == 74 && r_extended == 0 ) || // Ű�е�-					| ��������
		( r_scankey == 78 && r_extended == 0 ) || // Ű�е�+		��������	|
		( r_scankey == 28 && r_extended == 1 )   // Ű�е�enter	����Ǯ		|
		)  // ��
	{
		return true;
	}
	return false;

}


void GameProcess_BD::set_GameSpeed()
{
	g_GameSpeed.Clear();

	int i = 0;
	int j = 0;

	g_GameSpeed.set_CardDistributeDelay( i, j, 20 ); // ó�� ���� ���ð� �� ī�� ������
	g_GameSpeed.set_CardSpeed( i, j, 7.1f );
	g_GameSpeed.set_CardFlyDelay( i, j, 4 );		// ù��° ī��� �ι�° ī�� ������
	//	g_GameSpeed.set_BetCmdActiveDelay( i, j, 4 );
	//	g_GameSpeed.set_BetCmdActiveDelay( i, j, 5 );			// ���ӻ�� 1���� ��û���� ����
	g_GameSpeed.set_BetCmdActiveDelay( i, j, 6 );			// ���ӻ�� 1���� ��û���� ����. 2012. 8. 14 ���¹�
	g_GameSpeed.set_BetCmdActiveDelayInAutoBet(i, j, 4);
	g_GameSpeed.set_AutoBetActiveDelay( i, j, 13 );

#ifdef UPGRADE_20120502_TIMING_TEST_CODE
	//#ifdef _DEBUG
	int nBetCmdActiveDelay = GetPrivateProfileInt("LowBD", "BetBtnActiveFrameNum", 4, ".\\TimingTest.ini");
	g_GameSpeed.set_BetCmdActiveDelay(i, j, nBetCmdActiveDelay);

	nBetCmdActiveDelay = GetPrivateProfileInt("LowBD", "BetBtnActiveInAutoBetFrameNum", 4, ".\\TimingTest.ini");
	g_GameSpeed.set_BetCmdActiveDelayInAutoBet(i, j, nBetCmdActiveDelay);
	//#endif //_DEBUG
#endif //UPGRADE_20120502_TIMING_TEST_CODE

	const int CARDMOVESPEED = 1;
	g_GameSpeed.nDebug_ModeKind = CARDMOVESPEED;
	g_GameSpeed.bShowBetMoney = FALSE;
}


bool GameProcess_BD::GameKey_betting(int nVKey, int nExtend)
{
	if ( g_Poker.nState >= RSTATE_GAME_ROUND_CHANGECARD_M && g_Poker.nState <= RSTATE_GAME_ROUND_CHANGECARD_N )
	{
		return false;
	}

	//< CYS 100705
	//������ ���� ���� : �������� �ȴ� ���¿��� Ű�� ������ �����ϵ��� ���� (������)
	if ( g_RI.State != 0 &&  /*g_Poker.CurPlayer == m_cPlayer[ 0 ].ServPNum &&*/ m_cPlayer[ 0 ].JoinState == 1 && m_cPlayer[ 0 ].PlayState == 1 /*&& m_cPlayer[ 0 ].PNum == 0 */)
	{
		for ( int i = KEY_BET_DIE; i <= KEY_BET_HAF; i++ )
		{
			for ( int j = 0; j < MAX_GAME_KEY; j++ )
			{
				if ( IS_VKEY( G_KEYDEF[ i ][ j ], nVKey, nExtend ) )
				{
					switch( i )
					{
					case KEY_BET_DIE:
						{
							if ( g_Poker.nAutoBetNodie != 1 ) 
							{
								OnBtnFold();
							}
							break;
						}
					case KEY_BET_CHK:
						{
							if ( g_Poker.nAutoBetCmd <= 0)
							{
								OnBtnCheck();
							}
							break;
						}
					case KEY_BET_CAL:
						{
							if ( g_Poker.nAutoBetCmd <= 0)
							{
								OnBtnCall();
							}
							break;
						}
					case KEY_BET_PNG:
						{
							if ( g_Poker.nAutoBetCmd <= 0)
							{
								OnBtnPing();
							}
							break;
						}
					case KEY_BET_DAD:
						{
							if ( g_Poker.nAutoBetCmd <= 0)
							{
								OnBtnDda();
							}
							break;
						}
					case KEY_BET_HAF:
						{
							if ( g_Poker.nAutoBetCmd <= 0)
							{						
								//	if (  m_bBMax == TRUE )
								{
									OnBtnMax();
								}
								// 								else if (  m_bAllin == TRUE )
								// 								{
								// 									pGameProcess->OnBtnAllin();
								// 								}
							}
							break;
						}
					}
				}
			}
		}

	}
	//> CYS 100705

	return true;
}

bool GameProcess_BD::GameKey_cutting(int nVKey, int nExtend, int repeat)
{
	// �������̶�麣��Ű�Ͱ�ġ�ºκ�������ó��!
	if ( m_cPlayer[ 0 ].JoinState == 1 &&  m_cPlayer[ 0 ].PlayState == 1 && ! m_cPlayer[ 0 ].bFold &&  m_cPlayer[ 0 ].PNum == 0 )
	{
		for ( int i = KEY_CUT_1; i <= KEY_CUT_4; i++ )
		{
			int nMaxKey = MAX_GAME_KEY;

			//< CYS 100720 >
			if ( g_Poker.nState == RSTATE_STARTGAME ) // ����
			{
				nMaxKey = 1; // [ 0 ] F1, F2.... ��üũ
			}

			// 1 ~ 4 & Ŀ��Ÿ�� & �̹� �ߴٸ� ���̻� ���þʵ�
			if ( g_Config.bCutStyle == FALSE && g_Poker.nState >= RSTATE_GAME_ROUND_CHANGECARD_M && g_Poker.nState <= RSTATE_GAME_ROUND_CHANGECARD_N )
			{	
				if ( m_pCuttingStageInfo->bChangeStage == false )
				{
					nMaxKey = 0;
				}
			}

			for ( int j = 0; j < nMaxKey; j++ )
			{
				if ( IS_VKEY( G_KEYDEF[ i ][ j ], nVKey, nExtend ) )
				{
					m_cPlayer[ 0 ].OnChangeSelect( NULL, i );
					break;
				}
			}
		}
	}


	return true;
}

bool GameProcess_BD::GameKey_changepass(int nVKey, int nExtend, int repeat)
{
	if ( g_Poker.nState >= RSTATE_GAME_ROUND_CHANGECARD_M && g_Poker.nState <= RSTATE_GAME_ROUND_CHANGECARD_N && repeat == 0 )
	{
		for ( int i = KEY_CUT_CHANGE; i <= KEY_CUT_PASS; i++ )
		{
			for ( int j = 0; j < MAX_GAME_KEY; j++ )
			{
				if ( IS_VKEY( G_KEYDEF[ i ][ j ], nVKey, nExtend ) )
				{
					if ( i == KEY_CUT_CHANGE )
					{
						if ( g_pGameView->m_cGameViewBtnMan.IsEnableBtn( CGameButtonBD::BTNP_CHANGE ) )
						{
							OnBtnChange();
							break;
						}
					}
					else if ( i == KEY_CUT_PASS )
					{
						if ( g_pGameView->m_cGameViewBtnMan.IsEnableBtn( CGameButtonBD::BTNP_PASS ) )
						{
							OnBtnPass();
							break;
						}	
					}

				}
			}
		}
	}

	return true;

}


bool GameProcess_BD::GameKey_HiddenOpen( int nVKey, int nExtend, int repeat)
{
	// ���� ī�� ���� ���	
	if ( m_pMovingCardMgr->IsCardMoving( 0 ) == false )
	{		
		//���� ī�带 ������ �������� 
		if ( !m_bHaveHiddenCard )
		{
			return false;		
		}

		// ���¿Ϸ�
		if ( g_pGameView->m_cPlayerDrawManager.IsHiddenCardShow() )
		{
			return false;
		}

		bool bHave = false;

		for ( int i = KEY_HIDDEN_OPEN; i <= KEY_HIDDEN_SLOW; i++ )
		{
			for ( int j = 0; j < MAX_GAME_KEY; j++ )
			{
				if ( IS_VKEY( G_KEYDEF[ i ][ j ], nVKey, nExtend ) )
				{
					switch ( i )
					{
					case KEY_HIDDEN_OPEN:
						{
							// 							if ( j == 1 && g_Poker.CurPlayer == m_cPlayer[ 0 ].ServPNum) //< CYS 100705 > // ���Ͽ� delete Ű�� ���ý� ����
							// 							{
							// 								continue;
							// 							}

							PBPlayEffectSound(SND_HIDDEN_OPEN);

							g_pGameView->m_cPlayerDrawManager.SetHiddenCardShow( true );

							for ( int i = 0; i < TOTAL_CARD_BD; i++ )
							{
								m_cPlayer[ 0 ].SetFaceUp( i );
								m_cPlayer[ 0 ].m_cMyCard[ i ].SetHiddenCard( false );
							}


							AutoSort_Recommand(); //< CYS 101201 >
							m_cPlayer[ 0 ].SetCardResult();
							m_cPlayer[ 0 ].m_bShowValue = true;
							m_cPlayer[ 0 ].DrawEft_CompleteMadeValue( 0, true );

							bHave = true;
							break;
						}
					case KEY_HIDDEN_SLOW:
						{
							bool bHiddenOpen = false;
							for ( int i = 0; i < TOTAL_CARD_BD; i++ )
							{
								if ( m_cPlayer[ 0 ].m_cMyCard[ i ].IsHiddenCard() )
								{
									m_cPlayer[ 0 ].m_cMyCard[ i ].m_nHinddenArrow += 4;

									if ( m_cPlayer[ 0 ].m_cMyCard[ i ].m_nHinddenArrow > 20 )
									{
										PBPlayEffectSound(SND_HIDDEN_OPEN);
										g_pGameView->m_cPlayerDrawManager.SetHiddenCardShow( true );
										bHiddenOpen = true;
										break;
									}
								}
							}

							if ( bHiddenOpen )									
							{
								for ( int i = 0; i < TOTAL_CARD_BD; i++ )
								{
									m_cPlayer[ 0 ].SetFaceUp( i );
									m_cPlayer[ 0 ].m_cMyCard[ i ].SetHiddenCard( false );
								}

								AutoSort_Recommand(); //< CYS 101201 >
								m_cPlayer[ 0 ].SetCardResult();
								m_cPlayer[ 0 ].m_bShowValue = true;		// ���� ���δ�.			
								m_cPlayer[ 0 ].DrawEft_CompleteMadeValue( 0, true );
							}

							bHave = true;
							break;
						}
					}
				}
			}

			if ( bHave )
			{
				// �Ǽ��� ����Ű ���ý� 2�� ȣ�����
				break;
			}
		}
	}

	return true;
}

bool GameProcess_BD::OnPrePress()
{
	if ( m_cPlayer[ 0 ].bFold || m_cPlayer[ 0 ].PlayState == 0 )
	{
		return false;
	}

	if ( g_ObserverMan.IsMyObserver() == TRUE ) 
	{
		return false;
	}

	int nPNum = GetPNum_ByServPN( m_nCurPlayer );
	if ( m_cPlayer[ 0 ].PNum != nPNum )
	{
		return false;
	}

	//�̸� ������
	if ( !m_bReserveChange )
	{
		return false;
	}

	m_pCuttingStageInfo->Clear();

	if ( m_eCHMODE == E_CHANGE )
	{
		CCL_BD_CARD_EXCHANGE msg;
		msg.Set( g_RI.RoomNum, m_cPlayer[ 0 ].ServPNum, m_cPlayer[ 0 ].UI.UNum, m_cPlayer[ 0 ].UI.ID, m_nBackUpChangedNum, m_nBackUpPosIndex, m_nBackUpCardIndex  );

		SetCardExchangeLog(msg);

		NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());	
		
		// ���� �ڱ� �� ����
		m_pCuttingStageInfo->bMyTurn = true;			// ���� �� ��
		m_pCuttingStageInfo->bChangeStage = false;		// Ŀ������
		m_pCuttingStageInfo->bChangeCardFlyEnd = true; // Ŀ���ؼ� ī�� ��������

		m_cPlayer[ 0 ].m_bShowValue = false; // ���� �����
	}
	else  // pass
	{
		int PosIndex[ 5 ] = { 0,};
		int CardIndex[ 5 ] = { 0,};

		CCL_BD_CARD_EXCHANGE msg;
		msg.Set( g_RI.RoomNum, m_cPlayer[ 0 ].ServPNum, m_cPlayer[ 0 ].UI.UNum, m_cPlayer[ 0 ].UI.ID, 0, PosIndex, CardIndex );

		SetCardExchangeLog(msg);

		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());	

		// ##### ���� �ڱ� �� ����
		// ##### m_pCuttingStageInfo->Clear();  // ��Ŷ���� ��ġ�Ѵ�. ������ �Ϸ�޽��� ������ �ʴ´�. 
		// ##### ������ ��Ŷ �����°ŷ� ����!

		// ���� �ڱ� �� ����
		m_pCuttingStageInfo->bMyTurn = true;			// ���� �� ��
		m_pCuttingStageInfo->bChangeStage = false;		// Ŀ������
		m_pCuttingStageInfo->bChangeCardFlyEnd = true; // Ŀ���ؼ� ī�� ��������
	}

	return true;
}


void GameProcess_BD::OnBtnChange()
{	
	if ( g_ObserverMan.IsMyObserver() ) 
	{
		return;
	}

	if ( m_pCuttingStageInfo->bChangeStage == false )
	{
		return;
	}

	if ( m_cPlayer[0].bFold || m_cPlayer[0].PlayState == 0 )
	{
		return;
	}

	if ( !m_bChangeState )
	{
		return;
	}

	int nCardIndex[5], nPosIndex[5];
	ZeroMemory( nCardIndex, sizeof(int)*5 );
	ZeroMemory( nPosIndex, sizeof(int)*5 );

	int nIndex = 0;	
	for ( int i = 0 ; i < m_cPlayer[ 0 ].m_nTotalCardNum; i++ )
	{
		if ( m_cPlayer[ 0 ].m_cMyCard[ i ].IsSelected() )
		{
			nPosIndex[ nIndex ]  = i;
			m_nBackUpPosIndex[ nIndex ] = i; // ����, Ŭ�� ī�� �ε����� �ٸ���. ( ����� ���� ��� )

			nCardIndex[ nIndex ] = m_cPlayer[ 0 ].m_cMyCard[ i ].GetCardNo();
			nIndex++;
		}
	}

	//�ƹ��͵� �������� ������ ������ �ʴ´�.
	if ( nIndex <= 0 )return;

	m_bChangeState = false;

	SetEnableCuttingBtn( false );
	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable(true);

	//< CYS 100705
	int nPNum = GetPNum_ByServPN( m_nCurPlayer );
	if ( m_cPlayer[ 0 ].PNum == nPNum ) // ���� ���̸� ���� Ŀ������
	{
		CCL_BD_CARD_EXCHANGE msg;
		msg.Set(g_RI.RoomNum,m_cPlayer[0].ServPNum,m_cPlayer[0].UI.UNum,m_cPlayer[0].UI.ID, nIndex, nPosIndex, nCardIndex);

		SetCardExchangeLog(msg);

		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());	
	}
	else // ���
	{
		m_bReserveChange =  true; // �̸�����
		m_eCHMODE = E_CHANGE;

		for ( int i = 0; i < nIndex; i++ )
		{
			m_nBackUpPosIndex[ i ] = nPosIndex[ i ];
			m_nBackUpCardIndex[ i ] = nCardIndex[ i ];
		}

		m_nBackUpChangedNum = nIndex; //< CYS 100705 >

		m_pCuttingStageInfo->bChangeCardFlyEnd = false; // ����
	}
	//> CYS 100705	


	m_cPlayer[ 0 ].m_bShowValue = false;		// ī�� ���� �����
	m_cPlayer[ 0 ].m_bDontSelectCard = true;	// ���̻� ī�� ���� �Ұ�
	// ���� �ڱ� �� ����
	m_pCuttingStageInfo->bChangeStage = false;  // Ŀ�� ���� ( Ŀ���ߴ� )
}

void GameProcess_BD::OnBtnPass()
{
	if( m_pCuttingStageInfo->bChangeStage == FALSE ) return;
	if(g_ObserverMan.IsMyObserver() == TRUE) return;
	if( m_cPlayer[0].bFold || m_cPlayer[0].PlayState == 0 )return;

	SetEnableCuttingBtn( false );
	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable(true);


	int PosIndex[5] = {0,};
	int CardIndex[5] = {0,};

	m_bPassState = false;


	//< CYS 100705
	int nPNum = GetPNum_ByServPN( m_nCurPlayer );
	if ( m_cPlayer[ 0 ].PNum == nPNum ) // ���� ���̸� ���� Ŀ������
	{
		CCL_BD_CARD_EXCHANGE msg;
		msg.Set(g_RI.RoomNum,m_cPlayer[0].ServPNum,m_cPlayer[0].UI.UNum,m_cPlayer[0].UI.ID,0,PosIndex,CardIndex);

		SetCardExchangeLog(msg);

		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());	
	}
	else
	{
		m_bReserveChange =  true; // �̸�����
		m_eCHMODE =  E_PASS;
	}
	//> CYS 100705

	// ##### ���� �ڱ� �� ����
	// ##### m_pCuttingStageInfo->Clear();  // ��Ŷ���� ��ġ�Ѵ�. ������ �Ϸ�޽��� ������ �ʴ´�. 
	// ##### ������ ��Ŷ �����°ŷ� ����!

	m_cPlayer[ 0 ].m_bDontSelectCard = true;
	// ���� �ڱ� �� ����
	m_pCuttingStageInfo->bChangeStage = false;  // Ŀ�� ���� ( Ŀ���ߴ� )
}


void GameProcess_BD::ClearCuttingBackUpCard()
{
	memset( m_nBackUpPosIndex, 0, sizeof( m_nBackUpPosIndex ));
	memset( m_nBackUpCardIndex, 0, sizeof( m_nBackUpCardIndex ));
	m_nBackUpChangedNum = 0;
	m_bReserveChange = false;
}

// Ŀ�ù�ư Ȱ��ȭ�� �����
void GameProcess_BD::SetEnableCuttingBtn( bool bEnable )
{
	if ( GetCurrentWhere() != IDX_GAMEWHERE_GAME )
	{
		return;	
	}

	// 	if ( g_RI.nGameIndex != IDX_GAME_BD )
	// 	{
	// 		return;
	// 	}

	if ( true == bEnable )
	{
		if ( !g_ObserverMan.IsMyObserver() && m_cPlayer[ 0 ].JoinState == 1 && m_cPlayer[ 0 ].PlayState == 1 && !m_cPlayer[ 0 ].bFold )
		{
			CGameButtonBD* pGameBtnMgr = 
				static_cast< CGameButtonBD* >( &g_pGameView->m_cGameViewBtnMan );

			pGameBtnMgr->SetBetBtnShowAll( false ); // ���� ��ư �����
			pGameBtnMgr->SetCutBtnShowAll( true );  // Ŀ�� ��ư ���δ�

			int cnt = m_cPlayer[ 0 ].IsCutCardSelectedNum(); //�ٲ� ī�带 �̸� �����̳� ������ �״���

			if ( cnt > 0 ) 	//���� �̻��̴�.
			{					
				SetChangeState( true );
				SetPassState( false);

				pGameBtnMgr->SetEnableBtn( CGameButtonBD::BTNP_CHANGE, true );
				pGameBtnMgr->SetEnableBtn( CGameButtonBD::BTNP_PASS,   false );
			}
			else //�̸� ���� �ص��� ����
			{
				SetChangeState( false );
				SetPassState( true );

				pGameBtnMgr->SetEnableBtn( CGameButtonBD::BTNP_CHANGE, false );
				pGameBtnMgr->SetEnableBtn( CGameButtonBD::BTNP_PASS,   true );
			}
		}
	}
	else
	{
		CGameButtonBD* pGameBtnMgr = 
			static_cast< CGameButtonBD* >( &g_pGameView->m_cGameViewBtnMan );

		pGameBtnMgr->SetShowBtn( CGameButtonBD::BTNP_CHANGE,   false );
		pGameBtnMgr->SetShowBtn( CGameButtonBD::BTNP_PASS,     false );
		pGameBtnMgr->SetEnableBtn( CGameButtonBD::BTNP_CHANGE, false );
		pGameBtnMgr->SetEnableBtn( CGameButtonBD::BTNP_PASS,   false );
		SetChangeState( false );
		SetPassState( false );
	}
}

bool SORT_CARD( int  first, int second )
{
	//�ڵ� ���ü���(true��ȯ)
	if ( first % 13 < second % 13 )
	{
		return true;
	}
	else if (  first % 13 == second % 13 )
	{
		if ( first < second )
		{
			return true;
		}
	}

	return false;
}


// ī�����ý� ��ġ ����
void GameProcess_BD::AutoPos_Recommand()
{
	return;

	// ������
	if ( m_nChangedNum[ 0 ] > 0 )
	{
		std::vector< int > vtCard;

		for ( int i = 0; i < TOTAL_CARD_BD; i ++ )
		{
			vtCard.push_back( m_cPlayer[ 0 ].GetCardNo( i ) );
		}

		for ( int j = 0; j < m_nChangedNum[ 0 ]; j++ )
		{
			for ( int i = 0; i < TOTAL_CARD_BD; i ++ )
			{
				if ( i == m_nPosIndex[ 0 ][ j ]  )
				{
					vtCard[ i ] = 64;
				}
			}
		}

		
		
		//�ڵ������� �����Ǿ� ������ �����Ѵ� : 2012.09.27 YS.Jeong 
		if (g_Config.nAutoSortType == TRUE)
		{
			std::sort( vtCard.begin(), vtCard.end(), SORT_CARD );
		} 
		


		for ( int i = 0; i < TOTAL_CARD_BD; i++ )
		{
			if ( vtCard[ i ] != 64 )
			{
				m_cPlayer[ 0 ].SetCardNo( i, vtCard[ i ] );
			}
			else
			{
				m_cPlayer[ 0 ].SetCardNo( i, -1 );
			}
		}
	}
}

// ī�� ���� ����, ��ư�� ���� �� ����
void GameProcess_BD::AutoSort_Recommand( int nPNum, BOOL bClicked )
{
	std::vector< int > vtCard;


	//������ ������ ���� vtCard�� ���� �ݿ�
	for ( int i = 0; i < TOTAL_CARD_BD; i ++ )
	{
		vtCard.push_back( m_cPlayer[ nPNum ].GetCardNo( i ) );

		//Ŀ�÷������� ���� �����ϹǷ� �����͸� �����Ѵ�.
		m_aryCardData[ i ].bSelectFlag = m_cPlayer[ nPNum ].m_cMyCard[ i ].IsSelected();
		m_aryCardData[ i ].nCardIndex = m_cPlayer[ nPNum ].GetCardNo( i );
	}

	
	//�ڵ������� �����Ǿ� ������ �����Ѵ�:2012.09.27 YS.Jeong
	if (g_Config.nAutoSortType == TRUE)
	{
		std::sort( vtCard.begin(), vtCard.end(), SORT_CARD );
		
		//����ڰ� �����ϰ� �ִ� �����͸� �����ϰ� vtCard�� ����ȭ �Ѵ�.
		ARRAY_UPCARD_DATA *pUpCardTmp = new ARRAY_UPCARD_DATA(); 
		for ( int i = 0; i < TOTAL_CARD_BD; i ++ )
		{
			for ( int j = 0; j < TOTAL_CARD_BD; j ++ )
			{
				if ( (int)vtCard[i] == (int)m_aryCardData[j].nCardIndex )
				{
					pUpCardTmp->bSelectFlag = m_aryCardData[i].bSelectFlag;
					pUpCardTmp->nCardIndex = m_aryCardData[i].nCardIndex;
					m_aryCardData[i].bSelectFlag = m_aryCardData[j].bSelectFlag;
					m_aryCardData[i].nCardIndex = m_aryCardData[j].nCardIndex;
					m_aryCardData[j].bSelectFlag = pUpCardTmp->bSelectFlag;
					m_aryCardData[j].nCardIndex = pUpCardTmp->nCardIndex;
				}
			}
		}

		delete pUpCardTmp; 
	} 
	
		
	
	//ȭ�鿡 �׸��� �ִ� �����Ϳ� �ݿ�
	for ( int i = 0; i < TOTAL_CARD_BD; i ++ )
	{
		m_cPlayer[ nPNum ].SetCardNo( i, vtCard[i] );

		//����ڰ� �����ϰ� �ִ� �����͸� �����ϰ� vtCard�� ����ȭ �Ѵ�.
		m_cPlayer[ nPNum ].m_cMyCard[i].SetSelected( m_aryCardData[i].bSelectFlag );			
	}


	//��ư�� ������ ���� Ŀ���Լ��� ����
	if ( bClicked == TRUE )
	{
		//�ڵ����� ��ư�� ������ Ŀ�� ȿ���� �ٽ� �����Ͽ� ī����ġ�� ����ȭ �Ѵ�.
		AutoUp_Recommand( TRUE );

	}
	
	
}

// ī�� �ڵ��ø�
void GameProcess_BD::AutoUp_Recommand( BOOL bClicked ) 
{
	//���� �� ���ù�ư�� ������ Ŀ���� �ٽ� �� ���� ���İ� �ʱ�ȭ�� ���� �ʴ´�.
	if ( bClicked == FALSE )
	{
		// ī�� ����
		AutoSort_Recommand();

		//�����ͱ��� �ʱ�ȭ
		m_cPlayer [ 0 ].ReSetCardPostion();
	}else{
		//��ġ�� �ʱ�ȭ
		m_cPlayer [ 0 ].ReSetCardPostionOnly();
	}
	

	// g_Config.nAutoUpType �ʱⰪ 2 �� ���Ǿ� üũ������ ������� �ʴ´�.
	NMBASE::SKINGDI::CGraphButton* pAutoUpBtn =
		static_cast< NMBASE::SKINGDI::CGraphButton* >( g_pGameView->m_cGameViewBtnMan.GetGraphicBtn( CGameButtonBD::BTN_AUTOUP ) );

	//�Ʒ����� g_Config.nAutoUpType������ AI�ݿ����θ� �б��Ѵ�:2012.10.30 YS.Jeong
	//���ù�ư�� �߰��Ǿ� ���� �� ���õ� ī���� ��ġ�� �����Ͽ��ٰ� UP�ݿ��Ͽ��� �Ѵ�.
	/*
	// �������� ��������Ʈ Index�� ������� üũ�Ѵ�.
	if ( pAutoUpBtn && pAutoUpBtn->GetButtonNumber() == 1 ) // ��ư �ε���  
	{
		// ��ư �������� �̹��� =>  0 : ī���ڵ�����ON( �������� ), 1 : ī���ڵ�����OFF( �������� )
		return;
	}
	*/

	std::vector< int > vtCard;
	//�ڵ� �����ϱ� �� ī���� ������ �����Ѵ�(�ڵ������� OFF�� ��� �� ���� ���):2012.10.26 YS.Jeong 
	std::vector< int > vtCardTmp;

	for ( int i = 0; i < TOTAL_CARD_BD; i ++ )
	{
		vtCard.push_back( m_cPlayer[ 0 ].GetCardNo( i ) );
		vtCardTmp.push_back( m_cPlayer[ 0 ].GetCardNo( i ) );
	}


	//Ŀ���� ���� ������ �����Ѵ�.(�˻� ������ ����� ���� ī�常�� �˻��Ѵ�.)
	std::sort( vtCard.begin(), vtCard.end(), SORT_CARD );
	
	

	//Ŀ�� �˻� ����
	int  arrnUpValue[ TOTAL_CARD_BD ] = { 0,};  // ī�� �ε���
	bool arrbUpFlag[ TOTAL_CARD_BD ] = { 0,};  // flag

	int nUpCnt = 0;

	for ( int i = 0; i < TOTAL_CARD_BD; i++ )
	{
		if ( arrbUpFlag[ i ] )
		{
			continue;
		}


		//���� i��°�� ���� ī��� ���Ѵ�.
		for ( int j = i + 1; j < TOTAL_CARD_BD; j++ )
		{
			if ( arrbUpFlag[ j ] )
			{
				continue;
			}

			// ���ڰ� ����. 
			if ( vtCard[ i ] % 13 == vtCard[ j ] % 13 )
			{
				int nf1 = 0;
				int nf2 = 0;

				// ���� ī��Ʈ
				for ( int k = i + 1; k < TOTAL_CARD_BD; k++ )
				{
					if ( arrbUpFlag[ k ] )
					{
						continue;
					}

					if ( vtCard[ i ] / 13 == vtCard[ k ] / 13)
					{
						nf1++; 
					}
				}

				// ���� ī��Ʈ
				for ( int k = j + 1; k < TOTAL_CARD_BD; k++ )
				{
					if ( arrbUpFlag[ k ] )
					{
						continue;
					}

					if ( vtCard[ j ] / 13 == vtCard[ k ] / 13)
					{
						nf2++; 
					}
				}

				// ���� ���̰� ������ �ø���.
				if ( nf1 > nf2 )
				{
					arrbUpFlag[ i ] = true;
					arrnUpValue[ nUpCnt++ ] = vtCard[ i ];
					i = j; // ������ġ
				}
				else
				{
					arrbUpFlag[ j ] = true;
					arrnUpValue[ nUpCnt++ ] = vtCard[ j ];
				}
			}
			// ���̰� ���ٸ�
			else if ( vtCard[ i ] / 13 == vtCard[ j ] / 13 ) 
			{
				arrbUpFlag[ j ] = true;
				arrnUpValue[ nUpCnt++ ] = vtCard[ j ];
			}
		}
	}
	

	//ȭ���� �׸��� �����Ϳ� ���� ���� ���� �ݿ�
	if ( nUpCnt != 0 )
	{
		for ( int i = 0; i < nUpCnt; i++ )
		{
			for ( int j = 0; j < TOTAL_CARD_BD; j++ )
			{
				if ( arrnUpValue[ i ] == m_cPlayer[ 0 ].GetCardNo( j ) )
				{
					if ( !m_cPlayer[ 0 ].m_cMyCard[ j ].IsCardMoving() ) 
					{
						POINT ptStart, ptEnd; 

						bool bSelect = m_cPlayer[ 0 ].m_cMyCard[ j ].IsSelected();

						// ���� �Ǿ� ���� ����ī�� ���� ���·� ��ȯ:2012.10.30 YS.Jeong
						if ( !bSelect && bClicked == FALSE && g_Config.nAutoUpType == 1 ) 
						{
							m_cPlayer[ 0 ].m_cMyCard[ j ].SetSelected( true );			

							int gab = CARD_GAB_Y_SELECT;
							//���� �Ѱ� �ø�
							ptStart = CPoint( m_cPlayer[ 0 ].m_cMyCard[ j ].m_Xp, m_cPlayer[ 0 ].m_cMyCard[ j ].m_Yp );		
							ptEnd.x = m_cPlayer[ 0 ].m_cMyCard[ j ].m_Xp;
							ptEnd.y = m_cPlayer[ 0 ].m_cMyCard[ j ].m_Yp - gab;
							m_cPlayer[ 0 ].m_cMyCard[ j ].StraightMoveTo( ptStart, ptEnd, 2 );
						}
						else
						{
							int k;
							k = 0;
						}
					}
					else
					{
						int k;
						k = 0;

					}
				}
			}
		}
	}


	//�ڵ����� ���¿��� ������ ī�� ���·� �����Ѵ� : 2012.09.27 YS.Jeong 
	//�Ǵ� ������ ���õ� ī�� ������ �����Ͽ� �÷��ش�.
	for ( int i = 0; i < TOTAL_CARD_BD; i ++ )
	{
		m_cPlayer[ 0 ].SetCardNo( i , vtCardTmp[ i ]);

		if ( m_cPlayer[ 0 ].m_cMyCard[ i ].IsSelected() == true ) 
		{
			int gab = CARD_GAB_Y_SELECT;
			//���� �Ѱ� �ø�
			POINT ptStart, ptEnd;
			ptStart = CPoint( m_cPlayer[ 0 ].m_cMyCard[ i ].m_Xp, m_cPlayer[ 0 ].m_cMyCard[ i ].m_Yp );		
			ptEnd.x = m_cPlayer[ 0 ].m_cMyCard[ i ].m_Xp;
			ptEnd.y = m_cPlayer[ 0 ].m_cMyCard[ i ].m_Yp - gab;
			m_cPlayer[ 0 ].m_cMyCard[ i ].StraightMoveTo( ptStart, ptEnd, 2 );
		}
	}


}
//> CYS 100722

void GameProcess_BD::FoldUserProcess( int nPNum )
{
	// ���� ���� ������
	if ( m_cPlayer[ nPNum ].JoinState != 1 || m_cPlayer[ nPNum ].PlayState != 1 )
	{
		return;
	}

	m_cPlayer[ nPNum ].bFold = true;
	m_cPlayer[ nPNum ].PlayState = 0;

	if ( nPNum == 0 )
	{
		m_pCuttingStageInfo->Clear();// �� ���¿��� ����� ��ư �ʱ�ȭ

		if ( !g_ObserverMan.IsMyObserver() )
		{
			GM().GetMyInfo()->bFold = TRUE;
			GM().GetMyInfo()->PlayState = 0;
		}
	}
	else
	{
		m_cPlayer[ nPNum ].FoldUser_CardClose();
	}

	//���� ������ �� ������.
	m_nStartUserNum--;
	if ( m_nStartUserNum < 0 )
	{
		m_nStartUserNum = 0;			
	}

	// ī�� ����
	m_cPlayer[ nPNum ].ReSetCardPostion();
	m_cPlayer[ nPNum ].GetCutInfo().Clear();
	m_cPlayer[ nPNum ].GetMyLowBDRule()->Clear();
	m_cPlayer[ nPNum ].GetEffectObjectList()->Destroy();

	// ��Ӱ�
	if ( m_cPlayer[ nPNum ].m_nCardTotal > 0 )
	{
		for ( int  i = 0; i < m_cPlayer[ nPNum ].m_nCardTotal; i++ ) 
		{
			m_cPlayer[ nPNum ].SetFaceDown( i );							
		}
	}

	SoundFxKind( 1, m_cPlayer[ nPNum ].UI.Sex );// ### [���� �߰� �۾�] ### ����

	char szMsg[ MAX_PATH ] = { 0,};
	CString str;
	sprintf_s( szMsg, g_StrMan.Get(_T("FOLD_USER")), m_cPlayer[ nPNum ].UI.NickName );
	AddGameInfoViewEdit( szMsg, GetChatColor(ECC2_NOTIFY) );

	if ( g_Poker.nState == RSTATE_PREPARESTART )
	{
		//ī�� ���ý� �ð� �ʰ��� ���� ������ �̹����� ���� ����ش�.
		effect::CBetCmdEffect *pObj = NULL;
		SAFE_NEW( pObj, effect::CBetCmdEffect );
		pObj->Init( nPNum, ANF_BET_CMD, ANF_BET_DIE );
		g_pGameView->m_cAniSprListManager.ADDObject( pObj );
	}
}


//�ٲٱ� �˸� ��ħ ���� ����
void GameProcess_BD::OnNotifyCardExChange( void *pValue ) 
{
	if ( GetCurrentWhere() != IDX_GAMEWHERE_GAME )
	{
		return;
	}

	TPASSERT( pValue != NULL );

	CSV_BD_NOTIFY_CARDEXCHANGE* pMsg = static_cast< CSV_BD_NOTIFY_CARDEXCHANGE* >( pValue );
	SetCardExchangeLog(*pMsg);
	int nChangeType = *pMsg->pCurrType;

	// ������ ���
	g_pGameView->m_cDisplayBoard.SetText( ENUM_DISPLAY_BOARD_TEXT( (int) EDBT_MORNING_CUTTING + nChangeType - 1 ) ); // ��ħ, ����, ����


	if ( nChangeType < 0 || nChangeType > 3 )
	{
		//���� ���� ȣ��
		OnBtnFold();
		return;
	}	

	if ( g_RI.nChangeType > nChangeType )
	{
		//���� ���� ȣ��
		OnBtnPass();
		return;
	}

	g_RI.nChangeType = nChangeType;

	for ( int i = 0 ; i < MAX_PLAYER ; i ++ )
	{
		m_cPlayer[ i ].GetEffectObjectList()->DelObject( ANF_CUTINFO_NUM );
	}


	//���� Ŀ��带 ������ �� �����.
	g_pGameView->m_cAniSprListManager.BetCmd_DelDelay( 5 );

	// ���øӴ� ǥ�õǴ� �κ��� �ʱ�ȭ �Ѵ�.
	// 2012. 04. 25 ������
	for (int i = 0; i < GetMaxPlayer(); ++i)
	{
		g_pGameView->m_cPlayerDrawManager.ClearBettingMoney(i);
	}

	SetEnableCuttingBtn( false );
	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable(true);

	ClearCuttingBackUpCard(); //< CYS 100705 >

	//���� ��� 
	int nServerChangerPNum = *pMsg->NextChangerPNum;

	if( nServerChangerPNum > -1 && !g_ObserverMan.IsMyObserver() )
	{
		int ClientChangerPNum = GetPNum_ByServPN(nServerChangerPNum);

		if ( m_cPlayer[ 0 ].PNum == ClientChangerPNum ) // ��
		{
			m_pCuttingStageInfo->SetCuttingTimeFlag();				 // Ŀ��Ÿ�� Ȱ��ȭ
			m_pCuttingStageInfo->bMyTurn = true;
			SetEnableCuttingBtn( true );
		}
		else
		{
			// �������� ���� �ƴϾ ���� �������̸� Ȱ��ȭ ��Ű��. ���� ������ ���ϴ� ������ ��ư Ȱ��ȭ ����
			if ( m_cPlayer[ 0 ].JoinState == 1 && m_cPlayer[ 0 ].PlayState == 1 )
			{
				// ī�� Ŀ�ý� ���� �����Ѵ�.
				m_pCuttingStageInfo->SetCuttingTimeFlag();
				SetEnableCuttingBtn( true );
			}
		}

		// Ÿ�̸�  Ȱ��		
		g_pGameView->m_cPlayerDrawManager.ClearTimer();
		g_pGameView->m_cPlayerDrawManager.SetStartTimer();

		if ( !m_cPlayer[ ClientChangerPNum ].bFold &&  m_cPlayer[ ClientChangerPNum ].PlayState == 1 )
		{
			// ī�� ����
			// Ÿ�̸�  Ȱ��		
		}
	}

	//���� ���� �ٲٱ� �������  �־� �ش�.
	g_Poker.nState = RSTATE_GAME_ROUND_CHANGECARD_M + ( g_RI.nChangeType - 1);
	m_nCurPlayer = nServerChangerPNum;

	effect::CCuttingStageMsgEffect *pStateInfo = NULL;
	SAFE_NEW( pStateInfo, effect::CCuttingStageMsgEffect );
	pStateInfo->Init( g_RI.nChangeType );
	g_pGameView->m_cAniSprListManager.ADDObject( pStateInfo );


	//2009.01,07 ���� �� ���� ǥ�� ���� ���� �ؼ� �ʱ�ȭ ����
	for( int i = 0 ; i < MAX_PLAYER ; i ++ )
	{
		ZeroMemory(m_nPosIndex[i],sizeof(m_nPosIndex[i]));
		ZeroMemory(m_nCardIndex[i],sizeof(m_nCardIndex[i]));
		m_nChangedNum[i] = 0;
	}

	// ��ħ, ����, ���� �Դϴ� ���� ���� ������ ���� ==> ȣ�� ����
	int nPNum = GetPNum_ByServPN(nServerChangerPNum);
	m_bSex = m_cPlayer[ nPNum ].UI.Sex > 0 ? true : false;
	m_bCutInfoDelaySound = true;
	m_nCutInfoDelaySound = 20;

	int pn = GetPNum_ByServPN( m_nCurPlayer );
	// ������ �߰� ī�弼������ ���� �϶��� �� ǥ�� ����.
	if ( CheckRoundForCardSet() )
	{
		g_pGameView->m_cPlayerDrawManager.SetTurn( pn );
	}

	//������ : ������ �ʱ�ȭ
	SetReserveButton(RESERVESTATE_BET_NONE);
}

bool GameProcess_BD::ReceiveUserCutting_Pass( int nPNum, int nNextTurnNum )
{
	int nPassBeforDelay = ( int )g_cUIData.GetFloatBD( ui::EFLOAT_CUT_PASS_DELAY );	 // Ŀ�� �н� �̵� �� ���ð�

	NCARDMOVEDATA data;
	data.Clear();						
	data.m_nPlayNum = nPNum;
	data.m_nIndex = 0;
	data.m_fSpeed = g_cUIData.GetFloatBD( ui::EFLOAT_CUT_SPEED );
	data.m_nDelay = 0;	

#ifdef UPGRADE_20120502_TIMING_TEST_CODE
	//#ifdef _DEBUG
	data.m_fSpeed = GetPrivateProfileInt("LowBD", "CutCardSpeedFrameNum", 7, ".\\TimingTest.ini");
	//#endif //_DEBUG
#endif //UPGRADE_20120502_TIMING_TEST_CODE

	data.m_sFirstSound.bSoundUse = true;
	data.m_sFirstSound.nCuttingNum = 0; // �н�
	data.m_nFirstCmd = EVENT_BD_CUTTING_PASS;

	CNMovingCard *pMoveCard = NULL;
	SAFE_NEW( pMoveCard, CNMovingCard );

	pMoveCard->Initialize( &data, 1, nPassBeforDelay, DIRECT_MOVE_PASS , false, ( PFN_CARDEVENT ) CardEventCB ); // ���� �̺�Ʈ
	pMoveCard->nPNum = nPNum;
	pMoveCard->SetLastCMD( EVENT_BD_CUTTING_ALL_PASS );
	pMoveCard->SetLastCMD_User( nPNum );
	m_pMovingCardMgr->ADDCard( pMoveCard, true ); // ��ī���̵��� �������ϰ�� �������� Ŀ�ǵ��̺�Ʈ ��� true

	if ( nPNum == 0 )
	{
		g_pGameView->m_cPlayerDrawManager.SetFlyWindCard( true ); // �̺�Ʈ����� ��⶧���� �̵������� �����Ѵ�.
	}

	//���� ��� 
	if ( nNextTurnNum > -1 && !g_ObserverMan.IsMyObserver() )
	{
		int ClientChangerPNum = GetPNum_ByServPN( nNextTurnNum );

		if ( m_cPlayer[ 0 ].PNum == ClientChangerPNum )
		{
			// �̸� Ŀ���� ���� �ڵ�����
			if ( m_bReserveChange )
			{
				// �̸� ������
				m_nCurPlayer = nNextTurnNum;

				if ( OnPrePress() ) // ī�� �̺�Ʈ�� ��ũ �ȸ���. ī�� �̺�Ʈ �Ϸ��� ȣ��
				{
					g_pGameView->m_cPlayerDrawManager.ClearTimer();
					return true;
				}
			}

			m_pCuttingStageInfo->bMyTurn = true;  // ���� ����
		}

		// Ÿ�̸�  Ȱ��		
		g_pGameView->m_cPlayerDrawManager.ClearTimer();
		g_pGameView->m_cPlayerDrawManager.SetStartTimer();

		//�н� �϶� ���� ��� ������ �˷��ش�.
		m_nCurPlayer = nNextTurnNum;
	}
	else
	{
		return false; // ���� �ʵ�!
	}

	return true;
}

bool GameProcess_BD::ReceiveUserCutting_Change( void* pValue, int nPNum, int nNextTurnNum )
{
	CSV_BD_GIVETOUSER_NEWCHANGECARD* pMsg 
		= static_cast< CSV_BD_GIVETOUSER_NEWCHANGECARD* > ( pValue );
	SetCardExchangeLog(*pMsg);
	m_nChangedNum[ nPNum ] = *pMsg->ChangedNum; // ī�� ���
	TPASSERT( m_nChangedNum[ nPNum ] >= 0 && m_nChangedNum[ nPNum ] <= TOTAL_CARD_BD );

	if ( g_Poker.nState == RSTATE_GAME_ROUND_CHANGECARD_N )
	{
		if ( nPNum == 0 && !g_ObserverMan.IsMyObserver() )
		{
			if ( m_nChangedNum[ nPNum ] > 0 )
			{
				//���� ī�� ���� ����
				m_bHaveHiddenCard = true;
			}
		}
	}

	///////////////////////////////////////////////////////////////

	for ( int nIndex = 0; nIndex < m_nChangedNum[ nPNum ]; nIndex++ )
	{
		m_nPosIndex[ nPNum ][ nIndex ]  = pMsg->PosIndex[ nIndex ];
		m_nCardIndex[ nPNum ][ nIndex ] = pMsg->CardIndex[ nIndex ];

		//< CYS 101201  ī�� ����
		if ( nPNum == 0 && !g_ObserverMan.IsMyObserver() )
		{
			// ī�� ���ķ� ���� �ε����� �ٸ���.
			m_nPosIndex[ 0 ][ nIndex ] = m_nBackUpPosIndex[ nIndex ];
		}
		//> CYS 101201

	}
	///////////////////////////////////////////////////////////////

	NCARDMOVEDATA data[ 5 ];
	for ( int nIndex = 0 ; nIndex < 5 ; nIndex++ )
	{
		data[ nIndex ].Clear();
	}

	POINT dest_pos;
	CPoint start_pos( FLY_CARD_POS_X, FLY_CARD_POS_Y );	

	int nCardGabDelay = ( int )g_cUIData.GetFloatBD( ui::EFLOAT_CUT_CARD_DELAY );	 // ù��° ī��� �ι�° ī�� 
	int nTotalDelay   = ( int )g_cUIData.GetFloatBD( ui::EFLOAT_CUT_TOTAL_DELAY ); // ī�� �̵� �� �� ���ð�

	//����� ��û�� ���ؼ� 5���� 6���� ����. 
	//2012.7.10 ���¹�
	nCardGabDelay = 6 ;

#ifdef UPGRADE_20120502_TIMING_TEST_CODE
	//#ifdef _DEBUG
	nCardGabDelay = GetPrivateProfileInt("LowBD", "CutCardStartDelayFrameNum", 5, ".\\TimingTest.ini");
	//#endif //_DEBUG
#endif //UPGRADE_20120502_TIMING_TEST_CODE

	for ( int j = 0; j < m_nChangedNum[ nPNum ]; j++ )
	{
		if ( strlen( m_cPlayer[nPNum].UI.ID )>0 && m_cPlayer[ nPNum ].JoinState == 1 && m_cPlayer[nPNum].PlayState == 1 )
		{
			int nCardIndex = m_nPosIndex[ nPNum ][ j ];

			POINT ptNewDest_pos;

			dest_pos.x = m_cPlayer[ nPNum ].m_cMyCard[ nCardIndex ].m_Xp;				
			if( nPNum ==  0 ) //
			{
				dest_pos.y = m_cPlayer[ 0 ].m_sUIPos.PT_Card.y;
				ptNewDest_pos = dest_pos;

			}
			else
			{
				dest_pos.y = m_cPlayer[ nPNum ].m_cMyCard[ nCardIndex ].m_Yp;
				ptNewDest_pos = dest_pos;
			}

			data[j].m_nPlayNum = nPNum;
			data[j].m_nIndex = j;
			data[j].m_fSpeed = g_cUIData.GetFloatBD( ui::EFLOAT_CUT_SPEED );
			data[j].m_ptStartPos = dest_pos; // move up
			data[j].m_ptDestPos = start_pos; 
			data[j].m_nDelay =  ( j * nCardGabDelay );	// ù��° ī��� �ι�° ī�� ������
			data[j].m_ptStartPos_2nd = start_pos; // move down
			data[j].m_ptDestPos_2nd = ptNewDest_pos;   // ���ο� �ڸ� ��ǥ
			data[j].m_nFirstCmd     = EVENT_BD_CUTTING_MOVE_UP;
			data[j].m_nSecondCmd    = EVENT_BD_CUTTING_MOVE_DOWN;

#ifdef UPGRADE_20120502_TIMING_TEST_CODE
			//#ifdef _DEBUG
			data[j].m_fSpeed = GetPrivateProfileInt("LowBD", "CutCardSpeedFrameNum", 7, ".\\TimingTest.ini");
			//#endif //_DEBUG
#endif //UPGRADE_20120502_TIMING_TEST_CODE

			if ( j == 0 ) 
			{
				//ù��° ��
				data[ j ].m_sFirstSound.bSoundUse = true;
				data[ j ].m_sFirstSound.nCuttingNum = m_nChangedNum[ nPNum ];
			}

			if ( nPNum == 0 && !g_ObserverMan.IsMyObserver() )
			{
				if ( m_bHaveHiddenCard )
				{
					// ����ī��
					m_cPlayer[ nPNum ].m_cMyCard[ nCardIndex ].SetHiddenCard( true );
				}
			}			
		}
	}

	CNMovingCard *pMoveCard = NULL;
	SAFE_NEW( pMoveCard, CNMovingCard );

	pMoveCard->Initialize( data, m_nChangedNum[ nPNum ], nTotalDelay, DIRECT_MOVE_UP, false, ( PFN_CARDEVENT ) CardEventCB );// ����ī��� �ִϻ�� ����
	pMoveCard->nPNum = nPNum;
	pMoveCard->SetLastCMD( EVENT_BD_CUTTING_ALL_DOWN );
	pMoveCard->SetLastCMD_User( nPNum );
	m_pMovingCardMgr->ADDCard( pMoveCard, true ); // ��ī���̵��� �������ϰ�� �������� Ŀ�ǵ��̺�Ʈ ��� true

	if ( nPNum == 0 )
	{
		g_pGameView->m_cPlayerDrawManager.SetFlyWindCard( true ); // �̺�Ʈ����� ��⶧���� �̵������� �����Ѵ�.
	}

	//���� ��� 
	if ( nNextTurnNum > -1 && !g_ObserverMan.IsMyObserver() )
	{
		int ClientChangerPNum = GetPNum_ByServPN( nNextTurnNum );		
		if ( m_cPlayer[ 0 ].PNum == ClientChangerPNum )
		{
			// �̸� Ŀ���� ���� �ڵ�����
			if ( m_bReserveChange ) // �̸� ������
			{
				m_nCurPlayer = nNextTurnNum;
				if ( OnPrePress() ) 
				{
					g_pGameView->m_cPlayerDrawManager.ClearTimer();
					return true;
				}
			}

			m_pCuttingStageInfo->bMyTurn = true;  // ���� ����
		}

		// Ÿ�̸�  Ȱ��	
		g_pGameView->m_cPlayerDrawManager.ClearTimer();
		g_pGameView->m_cPlayerDrawManager.SetStartTimer();
	}
	else
	{
		return false;
	}

	//���� ���� �ٲٱ� �������  �־� �ش�.
	m_nCurPlayer = nNextTurnNum;

	return true;
}

// ī�� ���� Client ��û��� ���þִϸ��̼ǰ� ���� ���� ���� �ε���.
void GameProcess_BD::OnReceiveUserCutting( void* pValue )
{	
	CSV_BD_GIVETOUSER_NEWCHANGECARD* pMsg = static_cast< CSV_BD_GIVETOUSER_NEWCHANGECARD* > ( pValue );
	SetCardExchangeLog(*pMsg);
	int nChangedNum = *pMsg->ChangedNum;

	if ( nChangedNum == 100 )
	{
		//���� �޼��� 
		return;
	}

	int nSPNum = *pMsg->PNum; //  ���� �����
	if ( nSPNum < 0 )
	{
		return;
	}

	int nPNum = GetPNum_ByServPN( nSPNum );

	if ( nPNum < 0 || nPNum >= MAX_PLAYER)
	{
		return;
	}

	if ( m_cPlayer[ nPNum ].JoinState == 0 ) return;

	if ( nChangedNum >= 0 && nChangedNum <= 4 )
	{			
		if ( g_RI.nChangeType - 1 >= 0 )
		{
			//������ ����� ǥ���Ѵ� ���� ���� ����
			m_cPlayer[ nPNum ].GetCutInfo().m_arrCutCount[ g_RI.nChangeType - 1 ] = nChangedNum;
		}
	}

	if ( nChangedNum <= 0 )
	{	
		// �н�
		if ( !ReceiveUserCutting_Pass( nPNum, *pMsg->NextChangerPNum ) )
		{
		}
	}
	else
	{
		// ü����
		if ( !ReceiveUserCutting_Change( pValue, nPNum, *pMsg->NextChangerPNum ) )
		{
		}
	}

	int pn = GetPNum_ByServPN( m_nCurPlayer );
	// ������ �߰� ī�弼������ ���� �϶��� �� ǥ�� ����.
	if ( CheckRoundForCardSet() )
	{
		g_pGameView->m_cPlayerDrawManager.SetTurn( pn );
	}	
}

// ���� ��� ����Ʈ �׸���
void GameProcess_BD::CuttingNumEffectDraw( int nPNum )
{	
	int nCuttingRoundType = g_RI.nChangeType - 1;

	if ( nCuttingRoundType >= 0 && nCuttingRoundType <= 2 )
	{		
		if ( nPNum != 0  )
		{
			CCuttingInfo_BD *pObj = NULL;
			SAFE_NEW( pObj, CCuttingInfo_BD );
			pObj->Init( nPNum, m_cPlayer[ nPNum ].GetCutInfo().m_arrCutCount[ nCuttingRoundType ] );
			m_cPlayer[ nPNum ].GetEffectObjectList()->AddObject( pObj );
		}
	}
}

// ����ε庣�� ����Ʈ �׸���
void GameProcess_BD::BlindBetEffectDraw()
{	
	CPlayer *pPlayer = NULL;
	CBlindBet_BD *pObj = NULL;
	for(int i=0; i<GetMaxNewPlayer(); i++) {
		pPlayer = GetPlayer(i);

		if(pPlayer->PlayState == EPLAYER_GAMEPLAYING && pPlayer->JoinState == EGAME_JOININ)
		{
			SAFE_NEW( pObj, CBlindBet_BD );
			pObj->Init( i );
			m_cPlayer[ i ].GetEffectObjectList()->AddObject( pObj );
		}
	}
}

void GameProcess_BD::DellBlindBetEffect()
{
	for(int i=0; i<GetMaxNewPlayer(); i++) {
		m_cPlayer[ i ].GetEffectObjectList()->DelObject(ANF_PLAYER_BLIND);
	}
}

void GameProcess_BD::CheckMinimumUser(void)
{
	int nPlayer = 0;

	for(int i = 0; i < MAX_PLAYER; ++i) 
	{
		if( strlen(m_cPlayer[i].UI.ID) > 0 && m_cPlayer[i].JoinState != EGAME_JOINOUT)
		{
			++nPlayer;
		}
	}

	if(nPlayer < 2) 
	{
		g_pGameView->m_cDisplayBoard.SetText(EDBT_MINUSER);
	}
}

void  GameProcess_BD::SetJackPotMoney(INT64 i64Money)
{
	m_i64JackPotMoney = i64Money;
	g_cVipJackpot.SetShowJackpotMoney(m_i64JackPotMoney);
}


void  GameProcess_BD::SetOldJackPotMoney(INT64 i64Money)
{
	m_i64OldJackPotMoney = i64Money;
	g_cVipJackpot.SetShowOldJackpotMoney(m_i64JackPotMoney);
}

void GameProcess_BD::JackPotMoneyUpdate( jackpot::JP_MONEYINFO* pInfo)
{
	// ���� Ÿ��
	IDX_GAME eGameType = (IDX_GAME)g_RI.nGameIndex;

	// �ڽ��� ������ ���� �ּ� �ݾ�
	INT64 i64RoomMoney = g_RI.llEnterLimitMoney;

	if(i64RoomMoney <= 0)	//������ ����
		return;

	// �׽�Ʈ �ڵ�
	//	ROOMINFO sRoomInfo;
	//	memcpy(&sRoomInfo, &g_RI, sizeof(ROOMINFO));

	//����Ŀ, �ο�, 7��Ŀ
	ASSERT (eGameType > IDX_GAME_NULL);
	const int INDEX_ARRAY_JACKPOT_GAME_LOW = 1; //�ٵ��� �ε��� 1

	INT64 i64JackPotMoney = 0; 

	for(int i = 0; i < jackpot::DEF_MONEYRANGE_NUM; ++i)
	{
		if(LIMITMONEY_INT_INT64(pInfo->stJPBase[ INDEX_ARRAY_JACKPOT_GAME_LOW].nLimitMoney[i]) == i64RoomMoney)
		{
			i64JackPotMoney = pInfo->stJPBase[INDEX_ARRAY_JACKPOT_GAME_LOW].llJackPotMoney[ i ];//  llJackpotMoney[eGameType][i];  
			break;
		}
	}


	if(i64JackPotMoney > 0)
	{
		// ���� ���� �ݾ�
		SetOldJackPotMoney(GetJackPotMoney());

		// ���ο� ���� �ݾ�
		SetJackPotMoney(i64JackPotMoney);
	}
}


void GameProcess_BD::ClearJackpotMoney(int nGameCode, INT64 i64LimitMoney)
{
	// ���� Ÿ��
	IDX_GAME eGameType = IDX_GAME_NULL;

	switch(nGameCode)
	{
	case 42: eGameType = IDX_GAME_SP;
		break;

	case 54: eGameType = IDX_GAME_BD;
		break;

	case 61: eGameType = IDX_GAME_NP;
		break;

	default:
		{
			// �̰��� ���� �ʱ�ȭ ���ʿ䰡 �����Ƿ� �����Ѵ�.
			return;
		}
	}

	if(i64LimitMoney <= 0)
		return;


	//1. ����Ÿ�� �˻�
	//2. ������ �� �Ӵ� �˻�   
	//3. �ΰ��� ���� �����ϸ� ���� ���� ���� �Ӵϸ� �ʱ�ȭ �Ѵ�.
	int nGameIndexTemp		= g_RI.nGameIndex;
	INT64 i64LimitMoneyTemp = g_RI.MoneyRange.GetDispMinMoney();

	if(eGameType == nGameIndexTemp && i64LimitMoney == i64LimitMoneyTemp)
	{
		SetOldJackPotMoney(0);
		SetJackPotMoney(0);

		// ���������� �ش� �Ӵ� ������ ���� 0���� �ʱ�ȭ �Ѵ�.
		// ���� �濡 ����ϴ� ������ ���̱ݾ��� �ʱ�ȭ �Ѵ�.
		for(int i = 0; i < jackpot::DEF_MONEYRANGE_NUM; ++i)
		{
			if(g_RoomMoneyRanges[i] >= i64LimitMoneyTemp)
			{
				g_JackPotMoneyInfo.stJPBase[ eGameType ].llJackPotMoney[ i ] = 0;//  llJackpotMoney[eGameType][i] = 0;  
				break;
			}
		}
	}
}



//������ ���ױ�� �߰��� ���� GameProcess�� �߰��� ���� �Լ�
void GameProcess_BD::SendCallButton()
{
	OnBtnCall();
}

void GameProcess_BD::SendCheckButton()
{
	OnBtnCheck();
}

void GameProcess_BD::SendDieButton()
{
	OnBtnFold();
}

void GameProcess_BD::SendHalfButton()
{
	OnBtnMax();
}

void GameProcess_BD::SendDdaButton()
{
	OnBtnDda();
}

void GameProcess_BD::SendPingButton()
{

}

void GameProcess_BD::SetCardExchangeLog(CSV_BD_NOTIFY_CARDEXCHANGE msg)
{
	CString logData;
	logData.Format("[Recv]SV_BD_NOTIFY_CARDEXCHANGE pCurrType : %d ", *msg.pCurrType);
	logData.AppendFormat("UniqNo : %s ", GM().GetMyInfo()->UI.UniqNo);
	logData.AppendFormat("NextChangerPNum : %d ", *msg.NextChangerPNum);
	logData.Append("\r\n");

	NMBASE::SOCK::CSockMan::m_sSocketLogTxt.append(logData.GetString());
}

void GameProcess_BD::SetCardExchangeLog(CSV_BD_GIVETOUSER_NEWCHANGECARD msg)
{
	CString logData;
	logData.Format("[Recv]SV_BD_GIVETOUSER_NEWCHANGECARD PNum : %d ", *msg.PNum);
	logData.AppendFormat("UniqNo : %s ", GM().GetMyInfo()->UI.UniqNo);
	logData.AppendFormat("ChangedNum : %d ", *msg.ChangedNum);
	logData.Append("PosInd :");
	for (int i = 0; i < 5; i++)
	{
		logData.AppendFormat(" %d", msg.PosIndex[i]);
	}
	logData.Append(" CardInd :");
	for (int i = 0; i < 5; i++)
	{
		logData.AppendFormat(" %d", msg.CardIndex[i]);
	}
	logData.AppendFormat(" nChangeKind : %d ", *msg.nChangeKind);
	logData.AppendFormat("NextChangerPNum : %d ", *msg.NextChangerPNum);
	logData.AppendFormat("bLastChanger : %d ", *msg.bLastChanger);
	logData.Append("\r\n");

	NMBASE::SOCK::CSockMan::m_sSocketLogTxt.append(logData.GetString());
}

void GameProcess_BD::SetCardExchangeLog(CCL_BD_CARD_EXCHANGE msg)
{
	CString logData;
	logData.Format("[Send]CL_BD_CARD_EXCHANGE RoomNum : %d ", *msg.RoomNum);
	logData.AppendFormat("UniqNo : %s ", GM().GetMyInfo()->UI.UniqNo);
	logData.AppendFormat("PNum : %d ", *msg.PNum);
	logData.AppendFormat("UNum : %d ", *msg.UNum);
	logData.AppendFormat("ID : %s ", msg.id);
	logData.AppendFormat("ChangedNum : %d ", *msg.ChangedNum);
	logData.Append("PosInd :");
	for (int i = 0; i < 5; i++)
	{
		logData.AppendFormat(" %d", msg.PosIndex[i]);
	}
	logData.Append(" CardInd :");
	for (int i = 0; i < 5; i++)
	{
		logData.AppendFormat(" %d", msg.CardIndex[i]);
	}
	logData.Append("\r\n");

	NMBASE::SOCK::CSockMan::m_sSocketLogTxt.append(logData.GetString());
	// ���׷α� add logData
	//NMBASE::UTIL::BugSplatWrapper::GetInstance().AddMemLog(logData);
}

void GameProcess_BD::SetCardExchangeLog(CCL_BD_CARD_EXCHANGEEND msg)
{
	CString logData;
	logData.Format("[Send]CL_BD_CARD_EXCHANGEEND RoomNum : %d ", *msg.RoomNum);
	logData.AppendFormat("UniqNo : %s ", GM().GetMyInfo()->UI.UniqNo);
	logData.AppendFormat("PNum : %d ", *msg.PNum);
	logData.AppendFormat("UNum : %d ", *msg.UNum);
	logData.AppendFormat("ID : %s ", msg.ID);

	logData.Append("\r\n");

	NMBASE::SOCK::CSockMan::m_sSocketLogTxt.append(logData.GetString());
}

void GameProcess_BD::SetCardExchangeLog(CSV_RAISE msg)
{
	CString logData;
	POKERGAME pg = *msg.pPG;
	logData.Format("[Recv]SV_RAISE UniqNo : %s ", GM().GetMyInfo()->UI.UniqNo);
	logData.AppendFormat("nDistUser : %d ", pg.nDistUser);
	logData.AppendFormat("nRound : %d ", pg.nRound);
	logData.AppendFormat("nState : %d ", pg.nState);
	logData.AppendFormat("nBossPNum : %d ", pg.nBossPNum);
	logData.Append("\r\nPLAYSTATE :\r\n");
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		logData.AppendFormat("%d Player  JoinState : %d ", i, pg.PS[i].JoinState);
		logData.AppendFormat("PlayState : %d ", pg.PS[i].PlayState);
		logData.AppendFormat("nState : %d ", pg.PS[i].nState);
		logData.AppendFormat("bOnGame : %d ", pg.PS[i].bOnGame);
		logData.AppendFormat("bFold : %d ", pg.PS[i].bFold);
		logData.AppendFormat("arrCuttingNum : %d %d %d \r\n", pg.PS[i].arrCuttingNum[0], pg.PS[i].arrCuttingNum[1], pg.PS[i].arrCuttingNum[2]);
	}
	logData.Append("RAISEUSER :\r\n");
	for (int i = 0; i < MAX_PLAYER; i++)
	{
		logData.AppendFormat("%d Player  nState : %d ", pg.RU[i].nUNum, pg.RU[i].nState);
		logData.AppendFormat("nStyle : %d ", pg.RU[i].nStyle);
		logData.AppendFormat("nBtn : %d ", pg.RU[i].nBtn);
		logData.AppendFormat("bFold : %d ", pg.RU[i].bFold);
		logData.AppendFormat("nNowBat : %I64d ", pg.RU[i].nNowBat);
		logData.AppendFormat("nTotBat : %I64d \r\n", pg.RU[i].nTotBat);
	}

	NMBASE::SOCK::CSockMan::m_sSocketLogTxt.append(logData.GetString());
}

void GameProcess_BD::SetCardExchangeLog(CCL_ASK_RAISE msg)
{
	CString logData;
	logData.Format("[Send]CL_ASK_RAISE RoomNum : %d ", *msg.RoomNum);
	logData.AppendFormat("UniqNo : %s ", GM().GetMyInfo()->UI.UniqNo);
	logData.AppendFormat("UNum : %d ", *msg.UNum);
	logData.AppendFormat("PNum : %d ", *msg.PNum);
	logData.AppendFormat("ID : %s ", msg.ID);
	logData.AppendFormat("nStyle : %d ", msg.pRA->nStyle);
	logData.AppendFormat("nBtn : %d ", msg.pRA->nBtn);
	logData.AppendFormat("PlayTick : %lu ", msg.pRA->PlayTick);

	logData.Append("\r\n");

	NMBASE::SOCK::CSockMan::m_sSocketLogTxt.append(logData.GetString());
}