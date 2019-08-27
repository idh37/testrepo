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
	case EVENT_BD_PREPARESTART_ONEFLY_END:  // 처음 4장 중 한장씩 도착 후 호출
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

	case EVENT_BD_PREPARESTART_FOURFLY_END: // 처음 4장 모두 받았을때 ( 마지막 유저까지 4장씩 모두 나눠주고 나서 )
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

				// 채널 제한				
				if(!g_GAME_BD()->m_bBlind)
				{
					g_GAME_BD()->AutoUp_Recommand(); // 자동 카드 올림
					g_PLAYER( 0 )->SetCardResult();	//족보 산출	
				}
				g_PLAYER( 0 )->m_bShowValue = true;
				g_PLAYER( 0 )->DrawEft_CompleteMadeValue( 0 , true );
			}

		} break;


	case EVENT_BD_CUTTING_WAIT: // 커팅 타임시 ( 딜레이 시간이 0 일때 호출 )
		{	
			int nPNum = pEvent->Option;
			int nIndex = pEvent->Option2;

			if ( nPNum < 0 || nPNum >= MAX_PLAYER )
			{
				break;
			}

			//도착한 카드를 잠시 감춘다.
			int nCardIndex = g_GAME_BD()->m_nPosIndex[ nPNum ][ nIndex ];

			CNMyCard* pCard = &g_PLAYER( nPNum )->m_cMyCard[ nCardIndex ];
			if ( pCard != NULL )
			{			
				pCard->SetStayState( true ); // 카드 Draw 유무
			}

			MOVECARD_LIST *pCardData =  static_cast< MOVECARD_LIST* >( pValue );

			//사운드 파일
			if ( pCardData && pCardData->m_sCardMoveData.m_sFirstSound.bSoundUse )
			{
				g_GAME_BD()->SoundFXCutting( pCardData->m_sCardMoveData.m_sFirstSound.nCuttingNum, g_PLAYER( nPNum )->UI.Sex );
				pCardData->m_sCardMoveData.m_sFirstSound.Clear();
			}

			
		} break;

	case EVENT_BD_CUTTING_MOVE_UP: // 커팅 타임시 ( Move Up 카드 한장씩 위쪽으로 도착 완료 )
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

			//정렬
			if ( nPNum == 0 && !g_ObserverMan.IsMyObserver() )
			{
				g_GAME_BD()->AutoPos_Recommand();
			}

		} break;

	case EVENT_BD_CUTTING_MOVE_DOWN: // 커팅 타임시 ( Move Down 한장씩 내 카드덱으로 )
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

				// 버그로그 add logData
				NMBASE::UTIL::BugSplatWrapper::GetInstance().AddMemLog(logData);
				*/

                pCard = &playerBD->m_cMyCard[ nCardIndex ];




				if ( pCard != NULL) 
				{					
					//좌표 정위치 ( 선택된 카드 내림 )
					POINT ptCardPos;
					ptCardPos.x = pCard->m_Xp;
					ptCardPos.y = ( ( nPNum != 0 )? pCard->m_Yp : g_PLAYER( nPNum )->m_sUIPos.PT_Card.y );
					pCard->SetPos( ptCardPos.x, ptCardPos.y );

					if ( pCard->IsCardStay() ) // 이동완료
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
							//히든카드인 더미 카드를 그려라
							if ( pCard->IsHiddenCard() && !pCard->IsHiddenMovingComplete() ) 
							{
								pCard->SetHiddenMovingComplete( true );	// 현재 내 카드덱에 더미카드 그린다
							}
						}
					}
				}
			}

		} break;

	case EVENT_BD_CUTTING_PASS:  // 커팅 타임시 ( 패스 사운드만 호출 )
		{
			int  nPNum = pEvent->Option;
			int  nIndex = pEvent->Option2;

			if ( nPNum < 0 || nPNum >= MAX_PLAYER )
			{
				break;
			}	

			MOVECARD_LIST *pCardData =  static_cast< MOVECARD_LIST* >( pValue );


			//사운드 파일
			if ( pCardData && pCardData->m_sCardMoveData.m_sFirstSound.bSoundUse  )
			{
				g_GAME_BD()->SoundFXCutting( pCardData->m_sCardMoveData.m_sFirstSound.nCuttingNum, g_PLAYER( nPNum )->UI.Sex );
				pCardData->m_sCardMoveData.m_sFirstSound.Clear();
			}


		} break;

	case EVENT_BD_CUTTING_ALL_DOWN: // 커팅 체인지 완료 ( 카드 모두 이동완료 ##서버 -> 패킷)
	case EVENT_BD_CUTTING_ALL_PASS: // 커팅 패스 완료
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
				// 패스, 체인지 모두 서버로 패킷 보낸다 ( 수정 2011_12_04 )
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

						//저녁 컷팅이 끝난 후 면 카드 위치로 더이상 카드 선택을 못함
						if ( g_RI.nChangeType == 3 )
						{
							g_PLAYER( 0 )->m_bDontSelectCard = true;
							g_PLAYER( 0 )->ReSetCardPostion();

						}
						else
						{
							g_PLAYER( 0 )->m_bDontSelectCard = false;

							// 이전 커팅 바꾸기, 패스 선택 상관 없이 커팅 연출 종료시 자동선택 기능 확인 후 동작 시킨다.
							if( g_Config.nAutoUpType == 1 ) // 0 : 사용안함, 1 : 사용중, 2 : 사용전
							{
								g_GAME_BD()->AutoUp_Recommand(); // 자동올림
							}
						}


						if ( !g_GAME_BD()->IsHiddenCardFlag() )
						{
							if ( g_PLAYER( 0 )->JoinState == 1 && g_PLAYER( 0 )->PlayState == 1 && !g_PLAYER( 0 )->bFold )
							{
								// 메이드 이펙트 한번만 보여주기 위해서
								int nOldPoint = g_PLAYER( 0 )->GetMyLowBDRule()->m_RuleCompleteCard.numberpoint;
								//족보 산출		
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
				//여기서 카드 바꾼넘의 장수를 표현 해 준다.
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

	//m_aryCardData[ TOTAL_CARD_BD ] 초기화
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

	//채팅창에 메시지가 있을때
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


	//채팅창에 메시지가 없을때
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
	// ### [ 관전기능 ] ###
	CCL_EXITSUBSCRIPT esmsg;	
	/*esmsg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID, bexit);*/
	// 규제안 추가.
	esmsg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID, bexit, 0);

	// 메인서버에게 나가기 예약 상태를 알림
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

	// 삥금액
	Betmoney = g_RI.CurSeedMoney;

	//콜 금액
	CallMoney = g_Poker.RU[sPNum].nReCall;

	//따당 금액
	DadangMoney = g_Poker.RU[sPNum].nCalcBat + CallMoney;	

	// 하프 
	MaxMoney = g_Poker.RU[sPNum].nReCall + g_Poker.RU[sPNum].nCalcBat;		

	if(betCmd == RAISE_LAST_CMD_FOLD)
	{
		return  0;
	}	
	else if( betCmd == RAISE_LAST_CMD_CHECK)
	{
		return 0;
	}
	else if( betCmd == RAISE_LAST_CMD_CALL) // 콜 금액
	{
		return  CallMoney;
	}
	else if(betCmd == RAISE_LAST_CMD_PING) // 삥 금액
	{
		return g_RI.CurSeedMoney;
	}
	else if(betCmd == RAISE_LAST_CMD_DADANG) // 따당 금액
	{
		return DadangMoney;
	}
	else if( betCmd == RAISE_LAST_CMD_HALF ||betCmd == RAISE_LAST_CMD_QUARTER ||betCmd == RAISE_LAST_CMD_FULL || betCmd == RAISE_LAST_CMD_RACE || betCmd == RAISE_LAST_CMD_ALLIN ) // 하프 금액
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
		if( bFirst ) // 3장 3명 => 61초 1사람당 20초
		{
			m_nWindCardEndCnt = 3*200+(50*m_nStartUserNum);
		}
		else // 1장 7초+
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



// ### [사운드 추가 작업] ###
void GameProcess_BD::SoundFxKind(int betkind, int sex, bool bMax)
{
	//규제안 : 맥스 커맨드 추가 : 맥스이펙트
	if ( bMax )
	{
		PBPlaySound(sex?true:false, SND_MAX);
	} 
	else
	{
		switch(betkind)
		{
		case RAISE_LAST_CMD_FOLD: //다이
			PBPlaySound(sex?true:false, SND_DIE);
			break;
		case RAISE_LAST_CMD_CHECK:// 체크
			//규제안 관련 : 카드세팅중에 이펙트 제거
			if(CheckRoundForCardSet()){
				PBPlaySound(sex?true:false, SND_CHECK);
			}
			break;
		case RAISE_LAST_CMD_CALL: // 콜
			//규제안 관련 : 카드세팅중에 이펙트 제거
			if(CheckRoundForCardSet()){
				PBPlaySound(sex?true:false, SND_CALL);
			}
			break;
		case RAISE_LAST_CMD_PING: // 삥
			PBPlaySound(sex?true:false, SND_PING);
			break;
		case RAISE_LAST_CMD_DADANG: // 따당
			PBPlaySound(sex?true:false, SND_DDANG);
			break;	
		case RAISE_LAST_CMD_HALF: // 하프
			PBPlaySound(sex?true:false, SND_HALF);
			break;
		case RAISE_LAST_CMD_ALLIN: // 올인
			PBPlaySound(sex?true:false, SND_ALLIN);
			break;
		case RAISE_LAST_CMD_FULL: // 풀
			PBPlaySound(sex?true:false, SND_FULL);
			break;
		}
	}
}

void GameProcess_BD::SoundFxKindBig(int betkind, int sex, bool bMax )
{
	//규제안 : 맥스 커맨드 추가 : 맥스이펙트
	if ( bMax )
	{
		PBPlaySound(sex?true:false, SND_MAX_H);
	} 
	else
	{
		switch(betkind)
		{
		case RAISE_LAST_CMD_FOLD: //다이
			PBPlaySound(sex?true:false, SND_DIE_H);
			break;
		case RAISE_LAST_CMD_CHECK:// 체크
			//규제안 관련 : 카드세팅중에 이펙트 제거
			if(CheckRoundForCardSet()){
				PBPlaySound(sex?true:false, SND_CHECK_H);
			}
			
			break;
		case RAISE_LAST_CMD_CALL: // 콜
			//규제안 관련 : 카드세팅중에 이펙트 제거
			if(CheckRoundForCardSet()){
				PBPlaySound(sex?true:false, SND_CALL_H);
			}
			break;
		case RAISE_LAST_CMD_PING: // 삥
			PBPlaySound(sex?true:false, SND_PING_H);
			break;
		case RAISE_LAST_CMD_DADANG: // 따당
			PBPlaySound(sex?true:false, SND_DDANG_H);
			break;	
		case RAISE_LAST_CMD_HALF: // 하프
			PBPlaySound(sex?true:false, SND_HALF_H);
			break;
		case RAISE_LAST_CMD_ALLIN: // 올인
			PBPlaySound(sex?true:false, SND_ALLIN_H);
			break;
		case RAISE_LAST_CMD_FULL: // 풀
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
		// 아침입니다.
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


//2006.1.4 종료시 안내 메세지 삽입 부분
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
이하 함수 들은 추상 객체
*/
/////////////////////////////////////////////////////////////////////
void GameProcess_BD::Clear()
{
	// 클라이언트 현재 상태
	m_nGameState = 0;				// 0 없음, 1, 2, 3, 4, 5, 6 승결정중 7 승결정(게임 오버)
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

	// 상태
	m_nGameState = 0;		

	// [ 카드 이벤트 에러시 ]
	m_bWindCardEnd	= false;
	m_nWindCardEndCnt = 0;
	m_nStartUserNum	= 0;
	m_nTotalCardNum = 0;
	//2006.1.4 종료시 안내 메세지 그리는 부분
	m_GameEndMsgData.Clear();
	m_stAfterEndingUserInfo.Clear();
	// 결과 구조체
	ZeroMemory(&m_stGameOverResultData,sizeof(m_stGameOverResultData));	
	m_stWinnerPNumData.Clear();	
	m_pMovingCardMgr->Destroy();

	m_BetCount = 0;
	//m_bHiddenCardShow = 0;
	m_bHiddenClick = false;
	m_ptHiddenClickPos.x =0;
	m_ptHiddenClickPos.y =0;

	m_i64JackPotActive = 0;

	//관전자 채팅 경고
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

	//결과출력 Delay
	m_nResultDelayCount = 0;
	m_bResultDelayStart = false;

	//아침입니다.. 점심.. 저녁.. 사운드 Delay
	m_nCutInfoDelaySound = 0;
	m_bCutInfoDelaySound = false;
	m_bSex = false;


	ClearCuttingBackUpCard(); //< CYS 100705 >
}

void GameProcess_BD::ResetGame()
{
	// 버그로그 Clear()
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
	m_nBetCallMoneyClearCnt = 0;	//콜한금액들 지워주기
	g_pGameView->m_cGameViewBtnMan.SetBetBtnType(g_Config.eBetBtnType );
	g_Poker.nImageBoss = -1;

	// 배팅 칩 게임판을 지운다
	m_i64JackPotActive = 0;	

	//규제안 : 예약버튼 활성화 가능상태
	ReserveState(RESERVESTATE_BET_NONE);
	CanReserve(true);
	g_nCardTotalCount = 0 ; // 카드 세팅 비교값을 0으로 초기화.
}


void GameProcess_BD::OnTimer()
{
	m_nGameCount++;

	// [ 카드 날리기 ]
	if ( g_RI.State != 0 )
	{
		if( m_bWindCardEnd )
		{
			m_nWindCardEndCnt--;
			if ( m_nWindCardEndCnt <= 0)
			{
				m_bWindCardEnd = false;
				// 서버로 메세지 보낸다.
				// ### [ 관전기능 ] ###
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

		//혹시 몰라 한번 더 검사
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
				//배팅 금액 표시 한것들 지운다.
			}
		}

		//자동 베팅
		// 카드 날리는 중에는 그리지 않는다. 자동 배팅 설정일때 턴 제한시간 다되가면 콜로 보낸다.
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


		// 커팅시 call -> 커팅사운드 연속 호출
		if ( m_bCutInfoDelaySound ) // 서버에서 지연으로 바꿔야 함
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

	// 총 배팅금액 셋팅	
	for ( int i = 0 ; i < MAX_PLAYER; i++)
	{
		m_cPlayer[ i ].OnTimer();
	}

	m_cAllBetChip.OnTimer( );
	m_pMovingCardMgr->Process();

	// 결과 출력에 대한 Delay
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
		//히든 카드를 가지고 있는지 검사
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
		// 사운드팀 작업

		GAMERESULTDATA WinEftData;	//승패 창에 들어갈 구조체
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

	//채팅창변경
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
	GameKey_changepass( nVKey, nextend, repeat );	//바꾸기,패스
	GameKey_HiddenOpen( nVKey, nextend, repeat ); //히든카드 오픈
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
		if ( m_cPlayer[ 0 ].PlayState == 1 && m_nCurPlayer == m_cPlayer[ 0 ].ServPNum && !m_cPlayer[ 0 ].bFold ) // 카드 날리는 중에는 그리지 않는다.
		{		
			int tcnt = g_pGameView->m_cPlayerDrawManager.GetTimerTimeGab() / 1000;	

			int nTimeCnt = PBTurnTimeOut( ETTT_BETTING );

			if ( tcnt >= nTimeCnt ) //턴 제한 시간 가지고 오기 
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
				//올인 베팅을 하고 있는 유저 체크,콜
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

				// 타임 아웃 카운트 증가
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

			//턴 제한 시간 가지고 오기
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
	{ // 레이즈모드이다
		// 베팅시작 (카드 다 날리고 베팅 시작)
		//  타이머 보이기
		m_nCurPlayer = g_Poker.CurPlayer;
		g_pGameView->m_cPlayerDrawManager.SetStartTimer();

		int pn = GetPNum_ByServPN(m_nCurPlayer);
		// 규제안 추가 카드세팅으로 딜링 일때는 턴 표시 안함.
		if ( CheckRoundForCardSet() )
		{
			g_pGameView->m_cPlayerDrawManager.SetTurn(pn);
		}

		// ### [ 관전기능 ] ###
		if(!g_ObserverMan.IsMyObserver() && m_cPlayer[0].ServPNum == m_nCurPlayer)
		{			
			m_bBtnClick = FALSE;

			// 자동베팅시에는 베팅버튼 활성화 딜레이를 따로 설정한다.
			if (g_Poker.nAutoBetCmd > 0)
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelayInAutoBet(0, 0);
			else
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelay( 0, 0 );  
		}

		// 자신의 카드위에 족보명을 보위기 위함
		g_pGameView->m_cPlayerDrawManager.MyJokboNameUpdate();

		//규제안 예약베팅 셋팅 초기화 
		ReserveState(RESERVESTATE_BET_NONE);
	}	
}

//////////////////////////////////////////////////////////////////////////
/*
메세지 처리 
*/
//////////////////////////////////////////////////////////////////////////

bool GameProcess_BD::Accept_CreateRoom(void *pMsgParam) // 방만들기 허가
{
	if (GetCurrentWhere()!=IDX_GAMEWHERE_LOBY) return false;

	CSV_ACCEPT_CREATEROOM *pMsg = (CSV_ACCEPT_CREATEROOM*)pMsgParam;//UGPCommon 작업

	memcpy(&g_RI, pMsg->RI, sizeof(ROOMINFO));

	int nGame = g_RI.nGameIndex;

	ZeroMemory( &g_Poker,sizeof(POKERGAME) );

	m_cPlayer[ 0 ].SetNewPlayer(&GM().GetMyInfo()->UI);

	//게임 정보 변경 해주기 	
	g_MyGameInfo.nGameMode = IDX_GAME_BD;	

	// 초기화
	ResetGame();
	GM().SetWaitResponse(FALSE);

	// 게임 정보 구조체를 초기화 한다
	for ( int i = 1; i < MAX_PLAYER; i++ )
	{
		m_cPlayer[ i ].Clear();
	}

	m_cPlayer[ 0 ].JoinState = 0;
	m_cPlayer[ 0 ].MyIpCheck = -1;	

	m_nNo1PosPNum = 0;
	m_nMyPosPNum = 0;	 

	// 서버측 플레이어 번호 지정
	for ( int i = 0; i < MAX_PLAYER; i++ )
	{ 
		m_cPlayer[ i ].ServPNum = i; 
	}

	// ### [ 관전기능 ] ###
	memcpy( &m_cPlayer[ 0 ].UI, &GM().GetMyInfo()->UI, sizeof(GM().GetMyInfo()->UI));

	//Buy in 맥스방 UI 머니셋팅 방만들기
	if( g_RI.bIsMaxRoom )
	{
		//설정머니보다 자산이 많을경우 설정 머니로 셋팅한다.
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

	// ### [ 관전기능 ] ###
	GM().GetMyInfo()->JoinState = 0;
	GM().GetMyInfo()->MyIpCheck = -1;
	GM().GetMyInfo()->ServPNum = m_cPlayer[ 0 ].ServPNum;

	g_MyPN = g_MastPN = 0;	// 내가 방장

	m_nCurPlayer = 0;// 방처음 개설자가 선

	// 캐릭터창 초기화
	if(g_RI.MaxUserNum > MAX_PLAYER) g_RI.MaxUserNum=MAX_PLAYER;
	if(g_RI.MaxUserNum < 2) g_RI.MaxUserNum=2;

	//버튼들 초기화
	g_pGameView->m_cGameViewBtnMan.EnterRoom_Button_Init( g_RI.sBetRuleKind );

	//	g_pGameView->m_Roominfo.SetRoomInfo();

	g_pGameView->m_cPlayerDrawManager.OnEnterRoom();

	if(!g_ObserverMan.IsMyObserver())
	{
		LAYER_GLOBAL_AUTOBETWND()->OnEnterRoom();
	}

	g_cBanishMan.Clear();
	//UPG작업 추가
	//OnEnterRoom(TRUE);

	g_pGameView->m_cGameHelper.OnEnterRoom();

	g_pGameView->m_cTitle.MakeTitle();

	g_cPromotionMan.Accept_CreateRoom();

	// 규제안 입장 유저의 손실머니 정보 추가.
	INT64 lossMoney = GM().GetLossMaxMoney() + GM().GetLossMoney();
	g_pGameView->m_cPlayerDrawManager.OnEnterRoomLimitMoney(0, lossMoney);

	//규제안 : 방입장시에 손실금액 경고 문구 상태 체크
	g_pGameView->m_cGameViewBtnMan.SetLossMoneyWarning(true) ;

	//규제안 : 대기상태의 방정보를 설정한다.
	g_pGameView->m_cGameViewBtnMan.m_bIsWaitUser = false;

	return true;
}


// 방에 입장 허가 받음
bool GameProcess_BD::Accept_EnterRoom(void *pMsgParam) 
{
	CSV_ACCEPT_ENTERROOM* pMsg= (CSV_ACCEPT_ENTERROOM*) pMsgParam;	//UGPCommon 작업

	char *strMast = pMsg->EI->Ri.ID;

	g_RI.nGameIndex = pMsg->EI->Ri.nGameIndex;
	g_RI.FormKind = pMsg->EI->Ri.FormKind; // 1 : CHOICE_GAME	

	//게임 정보 변경 해주기 
	g_MyGameInfo.nGameMode = IDX_GAME_BD;	

	//규제안 : 대기상태의 방정보를 설정한다.
	g_pGameView->m_cGameViewBtnMan.m_bIsWaitUser = (pMsg->EI->Ri.bWaitInRoom=='1');

	//규제안 : 방입장시에 손실금액 경고 문구 상태 체크
	g_pGameView->m_cGameViewBtnMan.SetLossMoneyWarning(true) ;

	m_cPlayer[ 0 ].SetNewPlayer(&GM().GetMyInfo()->UI);

	//맥스방 UI 머니셋팅 입장시
	if( pMsg->EI->Ri.bIsMaxRoom )
	{
		//설정머니보다 자산이 많을경우 설정 머니로 셋팅한다.
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

	// ### [ 관전기능 ] ###
	for( i = 0; i < GetMaxNewPlayer(); i++ ) 
	{
		if ( strlen( pMsg->EI->Ui[ i ].ID ) > 0 ) 
		{
			if ( strcmp( strMast, pMsg->EI->Ui[ i ].ID)==0) MastID = i; // 방장 아이디 찾기
			if ( strcmp( GM().GetMyInfo()->UI.ID, pMsg->EI->Ui[ i ].ID ) == 0 ) MyID = i; // 나의 아이디
		}
	}
	GM().SetWaitResponse(FALSE);

	// 방장 혹은 나의 아이디를 찾을 수 없거나 방장과 내 아이디가 동일하다?
	if(MastID == -1 || MyID == -1 || MastID == MyID) {
		// ### [ 관전기능 ] ###
		CSV_ASK_OUTROOM aormsg;
		aormsg.Set(g_RI.RoomNum, GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID, REASON_OUTROOM_ABNORMAL);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), aormsg.pData, aormsg.GetTotalSize());
		// 연결종료
		g_MastPN = 0;
		CloseMainMessageDlg(); // 요청한 방에 입장중입니다 메시지 닫기
		return false;
	}

	GM().GetMyInfo()->SetChangeRoomInfo(pMsg->EI->Cui[MyID]);
	if(pMsg->EI->Ui[MyID].cObserver == 1)  { g_ObserverMan.SetMyObserver(TRUE); }

	g_pGameView->m_cPlayerDrawManager.OnEnterRoom();

	ResetGame();
	g_MastPN = MastID;	
	// 방 정보 복사
	memcpy(&g_RI, &pMsg->EI->Ri, sizeof(ROOMINFO));	
	if(g_RI.State == 1)
	{
		g_pGameView->m_cTotalBetMoney.Start();
	}

	if(g_RI.MaxUserNum > MAX_PLAYER) g_RI.MaxUserNum=MAX_PLAYER;
	if(g_RI.MaxUserNum < 2) g_RI.MaxUserNum = 2;//2,3,4,5

	// 접속 메세지 대화창 닫기
	CloseMainMessageDlg(); // 요청한 방에 입장중입니다 메시지 닫기

	ZeroMemory(&g_Poker,sizeof(g_Poker));
	memcpy(&g_Poker, &pMsg->EI->Pg,sizeof(g_Poker));

	if( !g_ObserverMan.IsMyObserver() ) // 관전이 아니라면
	{
		m_cPlayer[ 0 ].JoinState = 0;
		GM().GetMyInfo()->JoinState = 0;
		m_nMyPosPNum = MyID; // 서버쪽에 인덱스
		m_nNo1PosPNum = GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스		
		// 게임 정보 초기화
		for ( i = 1; i < GetMaxNewPlayer(); i++ )
		{
			m_cPlayer[ i ].Clear();
		}

		// 서버측 플레이어 번호 지정
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

		// 플레이어 순서에 맞게 초기화한다
		for( i = 0; i < GetMaxNewPlayer(); i++ ) 
		{
			if ( i >= MAX_PLAYER ) 
			{ //관전자 아디셋팅
				if ( strlen( pMsg->EI->Ui[ i ].ID ) > 0 ) 
				{
					m_cPlayer[ i ].SetNewPlayer( &pMsg->EI->Ui[ i ] );
					if ( pMsg->EI->Ui[ i ].AdminGrade == 0 ) 
					{
						// 관전자 리스트 출력
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
				m_cPlayer[ pn ].MyIpCheck = pMsg->EI->Pg.PS[i].nCheckIp;   // ### [ 중복된 IP가 ] ###
				m_cPlayer[ pn ].nSndFxKind = pMsg->EI->Pg.PS[i].nSndFxKind; // ### [사운드 추가 작업] ###
				/*m_cPlayer[ pn ].UI.nIcon = GetMoneyGrade(pMsg->EI->Ui[i].GetTotalMoney(IsTournament()));*/
				m_cPlayer[ pn ].UI.nIcon = GetMoneyGrade(pMsg->EI->Ui[i].GetTotalMoney());
				m_cPlayer[ pn ].SetChangeRoomInfo(pMsg->EI->Cui[i]);
			}
		}
		// 버튼 상태 설정
		m_nCurPlayer = g_Poker.CurPlayer;
		g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();
	}
	else //관전
	{
		// 서버 클라이언트 1 : 1
		//g_MyIndex = MyID; 삭제
		m_nMyPosPNum = 0;						    // 서버쪽에 인덱스
		m_nNo1PosPNum = GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스
		for(i=0; i<GetMaxNewPlayer(); i++) {
			m_cPlayer[ i ].Clear(); // 게임 정보 초기화
			m_cPlayer[ i ].ServPNum = i;
		}
		GM().GetMyInfo()->ServPNum = MyID;

		for(i=0; i<GetMaxNewPlayer(); i++) {		
			int pn = i;
			if(strlen(pMsg->EI->Ui[i].ID) > 0)	{
				m_cPlayer[ pn ].SetNewPlayer(&pMsg->EI->Ui[i]);
				if(i>=MAX_PLAYER) // 관전자 리스트 출력
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
				m_cPlayer[ pn ].MyIpCheck = pMsg->EI->Pg.PS[i].nCheckIp; // ### [ 중복된 IP가 ] ###
				m_cPlayer[ pn ].nSndFxKind = pMsg->EI->Pg.PS[i].nSndFxKind;// ### [사운드 추가 작업] ###
				/*m_cPlayer[ pn ].UI.nIcon = GetMoneyGrade(pMsg->EI->Ui[i].GetTotalMoney(IsTournament()));*/
				m_cPlayer[ pn ].UI.nIcon = GetMoneyGrade(pMsg->EI->Ui[i].GetTotalMoney());
				m_cPlayer[ pn ].SetChangeRoomInfo(pMsg->EI->Cui[i]);
			}
		}

		// 버튼 상태 설정
		m_nCurPlayer = g_Poker.CurPlayer;
		g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();
		// 여기까지

		//관전자 이미지 그리기


		//관전UI
		g_ObserverMan.OnObserve();
	}

	if(!g_Poker.bDistCard && g_Poker.nAutoBetCmd <=0 && g_RI.State == 1 && g_Poker.nRound >= 1)
	{
		// 규제안 추가 카드세팅으로 딜링 일때는 턴 표시 안함.
		if ( CheckRoundForCardSet() )
		{
			g_pGameView->m_cPlayerDrawManager.SetTurn(GetPNum_ByServPN(m_nCurPlayer));
		}		
	}

	m_cAllBetChip.SetMoney(GetRealBet());	
	//버튼들 초기화
	g_pGameView->m_cGameViewBtnMan.EnterRoom_Button_Init( g_RI.sBetRuleKind );

	// 오토배팅 셋팅
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

	// 플레이어 순서에 맞게 카드 초기화한다
	for ( i = 0; i < MAX_PLAYER; i++ )
	{
		int pn = GetPNum_ByServPN( i );
		if ( strlen(pMsg->EI->Ui[ i ].ID) > 0 ) 
		{
			// 게임 진행 상황 그릴것 카드 정보를 카드덱에다가 셋
			if ( m_cPlayer[pn].JoinState == 1 ) 
			{
				int nTotCnt = pMsg->EI->Pg.PS[i].nCardTotal;
				int nOpCnt =  pMsg->EI->Pg.PS[i].nOpenTotal;

				m_cPlayer[pn].bFold = pMsg->EI->Pg.PS[i].bFold;
				m_cPlayer[pn].m_nCardTotal = nTotCnt;
				m_cPlayer[pn].m_nOpenTotal = nOpCnt;

				for ( k = 0; k < 4; k++ ) 
				{
					m_cPlayer[ pn ].SetNewCard( DUMY_CARD ); // 카운트 자동증가
					CPoint pt;
					pt.x = m_cPlayer[ pn ].m_cMyCard[ k ].m_Xp;
					pt.y = m_cPlayer[ pn ].m_cMyCard[ k ].m_Yp;
					m_cPlayer[ pn ].m_cMyCard[ k ].SetMovingCurPos( pt );
					m_cPlayer[ pn ].ShowCard(k);
				}

				if ( m_cPlayer[ pn ].bFold == TRUE && pn != 0 ) // 폴드 유저라면 어둡게
				{
					m_cPlayer[ pn ].FoldUser_CardClose();
				}

				//커팅정보
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


	//블라인드 베팅중이라면 표시
	if(g_RI.FormKind == 5)
	{
		if(pMsg->EI->Pg.nRound <= 1 && pMsg->EI->Pg.nState <= 4)
		{
			BlindBetEffectDraw();
		}
	}

	return true;
}

// 유저가 방에 입장 알림
void GameProcess_BD::User_EnterRoom(void *pMsgParam)
{
	CSV_USERENTERROOM* pMsg= (CSV_USERENTERROOM*) pMsgParam;//UGPCommon 작업
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) return;

	PBPlayEffectSound(SND_USERIN);
	int i=0;
	int pnum = -1; // 플레이어 번호

	if( !g_ObserverMan.IsMyObserver() && pMsg->UI->cObserver!=1 ) // 1) 내가 겜참여중이고 겜참여 넘 입장
	{
		for ( i = m_nNo1PosPNum; i < MAX_PLAYER + m_nNo1PosPNum; i++ ) 
		{// 서버쪽의 0번인덱스 클라이언트 인덱스 부터
			int index = i;
			if(index>=MAX_PLAYER) index = index - MAX_PLAYER;
			if(index==0) continue; //나
			if(strlen( m_cPlayer[ index ].UI.ID)==0){pnum=index; break;}
		}

		if(pnum == -1) return; // 비어있는 곳을 찾지 못했으면

		// 방 정보를 수정하고 새로 참여한 플레이어의 정보를 저장
		g_RI.NowUserNum += 1;
		m_cPlayer[ pnum ].SetNewPlayer(pMsg->UI);

		m_cPlayer[ pnum ].ServPNum = GetServPNum_ByPN(pnum);	// 서버측 플레이어 번호 저장
		g_pGameView->m_cPlayerDrawManager.OnEnterRoom(pnum);

		// 규제안 입장 유저의 손실머니 정보 추가.
		g_pGameView->m_cPlayerDrawManager.OnEnterRoomLimitMoney(pnum, *pMsg->llMaxLossMoney);

		CString str;
		str.Format("▶입장: [%s]님\n", pMsg->UI->NickName);
		AddGameInfoViewEdit( str, GetChatColor( ECC2_NOTIFY ) );
	}	
	// 3)내가 관전중이고  겜참여 하는넘 입장
	else if(pMsg->UI->cObserver!=1)
	{
		for(i=0; i<MAX_PLAYER; i++) {
			if(strlen(m_cPlayer[ i ].UI.ID)==0){pnum=i; break;}
		}
		if(pnum == -1) return; // 비어있는 곳을 찾지 못했으면

		m_nMyPosPNum = 0; // 서버쪽에 인덱스
		m_nNo1PosPNum = GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스

		// 방 정보를 수정하고 새로 참여한 플레이어의 정보를 저장
		g_RI.NowUserNum += 1;
		m_cPlayer[ pnum ].SetNewPlayer(pMsg->UI);	
		m_cPlayer[ pnum ].ServPNum = GetServPNum_ByPN(pnum);// 서버측 플레이어 번호 저장
		g_pGameView->m_cPlayerDrawManager.OnEnterRoom(pnum);

		// 규제안 입장 유저의 손실머니 정보 추가.
		g_pGameView->m_cPlayerDrawManager.OnEnterRoomLimitMoney(pnum, *pMsg->llMaxLossMoney);	

		CString str;
		str.Format("▶입장: [%s]님\n", pMsg->UI->NickName);
		AddGameInfoViewEdit( str, GetChatColor( ECC2_NOTIFY ) );

	}	
	// 2)내가 겜중이고 관전인넘 입장
	// 4)내가 관전 이고 관전인넘 입장
	else
	{
		if(pMsg->UI->AdminGrade >0) {
			pnum = GetMaxNewPlayer()-1;//관리자
		}
		else {
			for(i=MAX_PLAYER; i<GetMaxNewPlayer()-1; i++) {
				if(strlen(m_cPlayer[ i ].UI.ID)==0){pnum=i; break;}
			}
		}
		if(pnum == -1) return;			

		m_cPlayer[ pnum ].SetNewPlayer(pMsg->UI);// 방 정보를 수정하고 새로 참여한 플레이어의 정보를 저장
		m_cPlayer[ pnum ].ServPNum = pnum;		// 서버측 플레이어 번호 저장

		if( pMsg->UI->AdminGrade==0) 
		{// 관리자가 아니면 
			g_RI.NowObserverNum += 1;

			CString str;
			str.Format("▶관전 입장: [%s]님\n", pMsg->UI->NickName);
			AddGameInfoViewEdit( str, GetChatColor(ECC2_NOTIFY));

			// 관전자 리스트 출력
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

	// ### [ 중복된 IP가 ] ###
	for(i=0; i<MAX_PLAYER; i++) {
		int pn = GetPNum_ByServPN(i);
		if(strlen(m_cPlayer[ pn ].UI.ID) > 0) {
			m_cPlayer[ pn ].MyIpCheck = pMsg->IPA->IpIndex[i];
		}
	}

	g_pGameView->m_cTitle.MakeTitle();
	m_cPlayer[ pnum ].nSndFxKind = *pMsg->SndFxKind;// ### [사운드 추가 작업] ###

	// 전광판에 내용 출력
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

void GameProcess_BD::User_OutRoom(void *pMsgParam) // 유저가 방에서 나감 알림
{
	CSV_USEROUTROOM* pMsg = (CSV_USEROUTROOM*) pMsgParam;//UGPCommon 작업
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) return;

	PBPlayEffectSound(SND_USEROUT);
	int pnum=-1;
	for(int i=0; i<GetMaxNewPlayer(); i++) {
		if(strcmp(m_cPlayer[ i ].UI.ID, pMsg->ID)==0) {pnum=i; break;}
	}
	if(pnum == -1) return;

	// 닉네임수정 
	CString NickName =  m_cPlayer[ pnum ].UI.NickName;

	if( pnum<MAX_PLAYER ) //겜중인넘
	{
		// [ 카드 날리기 ]
		if(m_cPlayer[ pnum ].bFold == FALSE && m_cPlayer[ pnum ].JoinState == 1 && m_cPlayer[ pnum ].PlayState == 1)
		{	//현재 겜중인 넘 나갔다.
			m_nStartUserNum--;
			if(m_nStartUserNum <0) m_nStartUserNum = 0;
		}


		m_cPlayer[ pnum ].Clear(); // UI 최기화		
		g_pGameView->m_cPlayerDrawManager.OnExitRoom(pnum);

		if(g_RI.NowUserNum > 0) g_RI.NowUserNum -= 1;

		CString str;		
		str.Format("◁퇴장: [%s]님\n", NickName );
		AddGameInfoViewEdit( str, GetChatColor( ECC2_NOTIFY ) );

		// 닉네임수정 	2006/10/30
		str.Format("## <ID: %s(%s)>님 퇴장 ##\n",pMsg->ID , NickName );
		//UPG작업 한 줄 교체
		AddLobbyHideChatText(&str);
	}
	else // 관전인넘
	{
		if(m_cPlayer[ pnum ].UI.AdminGrade == 0) 
		{
			if ( NULL != m_pObserverDlg )
			{
				m_pObserverDlg->User_DelItem(m_cPlayer[ pnum ].UI.UNum, m_cPlayer[ pnum ].UI.ID);
			}

			if(g_RI.NowObserverNum > 0) g_RI.NowObserverNum -= 1;

			CString str;
			str.Format("◁관전 퇴장:[%s]님\n", GetPlayer(pnum)->UI.NickName);
			AddGameInfoViewEdit( str, GetChatColor( ECC2_NOTIFY ) );
		}

		m_cPlayer[ pnum ].Clear(); // UI 최기화
		//	NMBASE::SOUND::g_Sound.PlayWav(SND_FX_2);
	}
	// ### [ 중복된 IP가 ] ###
	for(int i=0; i<MAX_PLAYER; i++)	{
		int pn = GetPNum_ByServPN(i);
		if(strlen(m_cPlayer[ pn ].UI.ID) > 0) {
			m_cPlayer[ pn ].MyIpCheck = pMsg->IPA->IpIndex[i];
		}
	}

	g_pGameView->m_cTitle.MakeTitle();

	// 전광판에 내용 출력
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

	//레이스 베팅 비율 게임 시작 할때 만 갱신 한다.
	g_fRaceBetRatio = (float) pSC->fRaceBetRatio;

	if( g_RI.CurSeedMoney != pSC->nSeedMoney ){
		g_RI.CurSeedMoney = pSC->nSeedMoney;		
		g_pGameView->m_cTitle.MakeTitle();
	}

	// 서버, 클라이언트 각각 선언하여 별도로 사용한다.
	ZeroMemory(&g_Poker, sizeof(g_Poker));
	memcpy(g_Poker.PS, pSC->Ps, sizeof(PLAYSTATE)*MAX_PLAYER);	


	//규제안 : 최초 맥스 활성 여부 초기화 : 맥스이펙트
	for (int i=0; i<MAX_PLAYER; i++)
	{
		GM().g_bFirstMax[i] = false;
	}


	//규제안 : 예약버튼 활성화 가능상태
	ReserveState(RESERVESTATE_BET_NONE);
	CanReserve(true);
	g_nCardTotalCount = 0 ; // 카드 세팅 비교값을 0으로 초기화.

	g_Poker.nBossPNum = -1;	
	g_Poker.nImageBoss = -1;

	//게임시작시
	g_RI.RoomMasterType = pSC->RoomMasterType;

	// 게임 시작 플래그 세팅
	m_bGameStart = TRUE;	
	for(int i=0; i<MAX_PLAYER; i++) 
	{		
		// 플레이어 스크린 초기화
		m_cPlayer[ i ].Reset();
	}

	g_pGameView->m_cPlayerDrawManager.Reset();


	g_Poker.nState = RSTATE_PREPARESTART; // 서버쪽 셋팅이나
	g_Poker.nDistUser = pSC->nDistUser;
	m_nCurPlayer = pSC->nDistUser;	

	g_DebugStartTick = timeGetTime();

	//자리 지킴이
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

			// 플레이어 상태 세팅
			m_cPlayer[pn].JoinState = pSC->Ps[i].JoinState; //
			m_cPlayer[pn].PlayState = pSC->Ps[i].PlayState;
			m_cPlayer[pn].bFold     = FALSE;

			//m_cPlayer[pn].UI.SetMoney(m_cPlayer[pn].UI.GetMoney(IsTournament()) - BetMoney, IsTournament());//100;
			//g_Poker.RU[i].nPMoney = m_cPlayer[pn].UI.GetMoney(IsTournament());

			//MAX방 처리.
			INT64 i64TempBetMoney = m_cPlayer[pn].GetNowMoney() - BetMoney;
			m_cPlayer[pn].SetNowMoney( i64TempBetMoney );

			g_Poker.RU[i].nPMoney = m_cPlayer[pn].GetNowMoney();
			
			m_cPlayer[pn].nSndFxKind = pSC->Ps[i].nSndFxKind;

			//히스토리 관련 머니 셋팅 
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

			// 유저 카드 셋팅 3장
			if( pn == 0)
			{
				// ### [ 관전기능 ] ###
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

			// 기본 배팅 칩을 던짐
			chip_draw_func(pn, g_Poker.RU[i].nRealBat); // 여기서의 10은 단위다!!
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
		// 강퇴 처리
		CString st[MAX_PLAYER], str;
		for(int i=0; i<MAX_PLAYER; i++) 
		{
			int pn = GetPNum_ByServPN(i);
			st[i].Format("%d: %s, %s\n",i, m_cPlayer[pn].UI.NickName, pSC->JoinID[i]);
			str += st[i];
		}
		//UGP작업
		//g_pGameDlg->ExitGame();
		ExitGameRequest();

		AfxMessageBox("잘못된 게임 시작정보로 인하여 퇴장합니다");
		AfxMessageBox(str);
		return;
	}	

	Card_Distribute();
	//배경 이미지 변화

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

	int nPlayNum = GetPNum_ByServPN( g_Poker.nDistUser ); // 카드 날리는 순서
	if (nPlayNum < 0 || nPlayNum >= MAX_PLAYER) return;	

	POINT dest_pos;
	CPoint start_pos(FLY_CARD_POS_X,FLY_CARD_POS_Y);

	//히든카드 족보 보여주지 않는다.
	for ( int x = 0 ; x < MAX_PLAYER ; x ++ )
	{
		m_cPlayer[x].m_bShowValue = false;
	}	 

	NCARDMOVEDATA data;	
	data.Clear();

	g_pGameView->m_cPlayerDrawManager.SetFlyWindCard( true );

	// 처음 4장은 애니메이션 Scene으로 그린다
	const WORD wFirstSecond_UserDelay = ( int )g_cUIData.GetFloatBD( ui::EFLOAT_F4_USER_DELAY );   // 처음 4장 카드 이동대기( 유저와 유저 )
	const WORD wFirstSecond_CardDelay = ( int )g_cUIData.GetFloatBD( ui::EFLOAT_F4_CARD_DELAY );   // 처음 4장 카드 이동대기( 카드와 카드 )

	{		
		int nCountIndex = 1; // 카드 나눠주는 순서대로

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

						// 전체 카드 딜레이 
						// 처음 4장은 동시에 Move 업데이트 진행한다 예외처리해야함
						int nDelay = ( int )g_cUIData.GetFloatBD( ui::EFLOAT_F4_TOTAL_CARD_DELAY );   // 처음 4장 카드 이동전 대기시간

						pMoveCard->Initialize( &data, 1, nDelay, DIRECT_MOVE_DOWN, true, ( PFN_CARDEVENT ) CardEventCB );//처음 카드 줄때 애니사용
						pMoveCard->nPNum = nIndex;
						pMoveCard->SetLastCMD( EVENT_BD_PREPARESTART_FOURFLY_END ); // 마지막 유저 카드에 이벤트 등록 
						pMoveCard->SetLastCMD_User( nIndex );
						m_pMovingCardMgr->ADDCardF4( pMoveCard ); // 등록전 이전 커맨드 모두 지운다.
					}
				}
			}
		}

		GameProcess_BD::SetWindCardEnd();
	}	
}

bool GameProcess_BD::Raise_InfoAndSound(INT64 llLastPlayerBetMoney)
{
	// ##### [ 정보 창에 메세지 && 사운드 출력] #####

	CPoint cpEffectPos=CPoint(0, 0);
	if ( g_Poker.LastPlayer >= 0 && g_Poker.LastPlayer < MAX_PLAYER )
	{
		int LastPnum = g_Poker.LastPlayer;
		int nSNum = GetPNum_ByServPN( LastPnum );
		if ( g_Poker.RU[ LastPnum ].nLastCmd > 0 && nSNum >= 0 && nSNum < MAX_PLAYER) 
		{				
			// 전사람 타임바 지우기
			int nLastCmd = g_Poker.RU[ LastPnum ].nLastCmd;

			//베팅 이미지를 내 전사람은 작은걸로 바꿔 주고 난 지운다.
			g_pGameView->m_cAniSprListManager.BetCmd_ChangeCmd( nSNum );

			if (nLastCmd<RAISE_LAST_CMD_LIMIT_1X) //리미트 베팅일경우에는 아래쪽에서 호출된다.
			{
				g_pGameView->m_cPlayerDrawManager.SetBettingMoney(nSNum, nLastCmd, llLastPlayerBetMoney);
			}

			if(2==g_Poker.RU[LastPnum].bMaxBet){
				g_pGameView->m_cPlayerDrawManager.SetLimitMoney(nSNum, true);
			}

			//규제안 : 맥스 커맨드 추가 : 맥스이펙트
			if (g_Poker.RU[LastPnum].bMaxBet && GM().g_bFirstMax[LastPnum] == false)
			{
				g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, 0);

				if ( m_cPlayer[ nSNum ].GetNowMoney() == 0  )
				{
					g_pGameView->m_cPlayerDrawManager.SetPlayAllinAni(nSNum, timeGetTime());
				}

				// 맥스이펙트
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
				//일반 배팅 커맨드
			}else{

				switch( nLastCmd )
				{
				case RAISE_LAST_CMD_FOLD: // foldUser 에서 처리
					{
						effect::CBetCmdEffect *pObj = NULL;
						SAFE_NEW( pObj, effect::CBetCmdEffect );

						pObj->Init(nSNum,ANF_BET_CMD,ANF_BET_DIE);		
						cpEffectPos=pObj->GetEffectPos();	
						g_cVipJackpot.OnBetGame(cpEffectPos, false);	
						g_pGameView->m_cAniSprListManager.ADDObject(pObj);
					} break; // 다이
				case RAISE_LAST_CMD_CHECK:
					{					
						if ( g_Poker.bDistCard )
						{
							break;
						}

						//규제안 작업 : 카드세팅 중이면 이펙트를 노출하지 않는다. 2014.01
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

							

					}break; // 체크
				case RAISE_LAST_CMD_CALL: 
					{	
						if ( g_Poker.bDistCard )
						{
							break;
						}		
						
						//규제안 작업 : 카드세팅 중이면 이펙트를 노출하지 않는다. 2014.01
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
						
					} break; // 콜
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

					}  break; // 삥

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
					}  break; // 따당

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
					}  break; // 따당

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
					} break; // 맥스


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
					} break; // 올인
					/*
					// 1배
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
					// 2배
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
					// 5배
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
					// 10배
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
					// 20배
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
						//리미트 배팅
						if (nLastCmd>=RAISE_LAST_CMD_LIMIT_1X)
						{
							if( g_Poker.bDistCard == TRUE ) break;		


							effect::CBetCmdEffect *pObj = NULL;
							SAFE_NEW( pObj, effect::CBetCmdEffect );

							if ( LastPnum < 0 || LastPnum >=MAX_PLAYER ) 
								break;

							//각구에서 베팅을 처음실행한 사용저의 경우 BET아이콘을, 이후 베팅을 실행하는 경우 RAISE아이콘을 표시한다.
							if (g_Poker.RU[LastPnum].nReCall==0)
							{
								g_pGameView->m_cPlayerDrawManager.SetBettingMoney(nSNum, nLastCmd, llLastPlayerBetMoney); //베팅 + 금액

								eLIMIT_X eLastLimitX = g_pGameView->m_cGameViewBtnMan.GetLimitMultiflierWithLastCmd((RAISE_LAST_CMD)nLastCmd);
								if (g_pGameView->m_cGameViewBtnMan.GetLimitMultiflierWithBetStep(g_Poker.nRound, LIMIT_BET_STEP_1)==eLastLimitX)	///스몰베팅인지 체크
								{								
									pObj->Init( nSNum, ANF_BET_CMD, ANF_BET_LIMIT_SMALL_BET );	//스몰벳
								}
								else
								{
									pObj->Init( nSNum, ANF_BET_CMD, ANF_BET_LIMIT_BIG_BET );	//빅벳
								}
							}
							else
							{
								g_pGameView->m_cPlayerDrawManager.SetBettingMoney(nSNum, nLastCmd, g_Poker.RU[LastPnum].nCalcBat, true); //받고 + 금액 + 더
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

				//배팅 컨맨드에 따른 사운드 재생 
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
						//올인 마크가 드로우 되는것이 없으면 드로우 명령
					}
				}
			}
		}

		//규제안 관련 작업으로 주석처리 2014.01->추후작업
		//따당 금액 구하기
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

			//규제안 : 카드가 날아오면 예약기능 초기화
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


	// 현재 베팅한 사람의 베팅금액을 알아오기 위해 이전 베팅정보 임시 저장
	RAISEUSER RU_Before[MAX_PLAYER];
	memcpy(RU_Before, g_Poker.RU, sizeof(RAISEUSER) * MAX_PLAYER);

	memcpy(&g_Poker,(char*)pMsg, sizeof(g_Poker));	
	g_pGameView->m_cPlayerDrawManager.SetTurn( -1 );
	g_pGameView->m_cGameViewBtnMan.Reset();

	int nImageBoss = g_Poker.nImageBoss;

	// ### [ 사용자머니 변경 ] ### 
	for ( int i = 0; i < MAX_PLAYER; i++ )
	{
		int pn = GetPNum_ByServPN(i);
		if(strlen(m_cPlayer[ pn ].UI.ID)>0 && m_cPlayer[ pn ].JoinState == 1 && m_cPlayer[ pn ].PlayState == 1) // 겜중이면
		{  
			//m_cPlayer[ pn ].UI.SetMoney(g_Poker.RU[i].nPMoney, IsTournament());// 사용자 머니 셋
			//MAX방 함수 호출
			m_cPlayer[ pn ].SetNowMoney( g_Poker.RU[i].nPMoney );// 사용자 머니 셋
			if( g_Poker.PS[i].bFold ) m_cPlayer[ pn ].bFold = TRUE;

			// 추가 
			//if(m_cPlayer[ pn ].UI.GetMoney(IsTournament()) < 0) m_cPlayer[ pn ].UI.SetMoney(0, IsTournament());
			// 추가 
			if(m_cPlayer[ pn ].GetNowMoney() < 0) 
			{
				m_cPlayer[ pn ].SetNowMoney( 0 );
			}

			// ### [ 관전기능 ] ###
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

	// 잭팟 프로모션 테스트 코드.
	int betCmd = g_Poker.RU[g_Poker.LastPlayer].nLastCmd ;
	if( betCmd == RAISE_LAST_CMD_HALF ||betCmd == RAISE_LAST_CMD_QUARTER ||betCmd == RAISE_LAST_CMD_FULL) // 하프 금액
	{
		if(g_RI.State == 1 && g_Poker.nAutoBetCmd <= 0)
			g_cPromotionMan.SetHalfJackpotCountPlus() ;
	}

	// 레이즈모드이다 베팅시작  내턴이면 버튼 활성화
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

			//규제안 : 예약기능 초기화
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

		if ( g_Poker.LastStyle >= 2 ) // 베팅시에만
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

		// ### [ 관전기능 ] ###
		//배팅 시작(베팅 버튼 활성)의 핵심이다. m_nActiveBetBtnDelayTime 값으로 딜레이를 준다.
		if ( !g_ObserverMan.IsMyObserver() && m_cPlayer[ 0 ].ServPNum == m_nCurPlayer )
		{
			g_pGameView->m_cGameViewBtnMan.SetShowBetMony(); //< CYS 100705 >
			m_bBtnClick = FALSE;

			// 자동베팅시에는 베팅버튼 활성화 딜레이를 따로 설정한다.
			if (g_Poker.nAutoBetCmd > 0)
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelayInAutoBet(0, 0);
			else
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelay( 54, g_RI.nGameIndex );
		}

		// 규제안 예약 베팅 추가.(방장이 아닌 유저는 버튼 활성화가 안옴)
		if(!g_ObserverMan.IsMyObserver() && m_cPlayer[ 0 ].ServPNum != m_nCurPlayer)
		{
			// 자동베팅시에는 베팅버튼 활성화 딜레이를 따로 설정한다.
			if (g_Poker.nAutoBetCmd > 0)
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelayInAutoBet(0, 0);
			else
				m_nActiveBetBtnDelayTime = g_GameSpeed.get_BetCmdActiveDelay( 54, g_RI.nGameIndex );
		}

		// 전광판 출력
		int nRound =g_Poker.nRound;
		if ( g_Poker.nAutoBetCmd > 0 )
		{
			g_pGameView->m_cDisplayBoard.SetText( EDBT_AUTOBETTING ); // 자동베팅
		}
		else
		{
			// 베팅메시지
			g_pGameView->m_cDisplayBoard.SetText( ENUM_DISPLAY_BOARD_TEXT((int)EDBT_BEFORE_MORNING_BETTING + nRound - 1 ));
		}
		//

		// 규제안 추가 카드세팅으로 딜링 일때는 턴 표시 안함.
		if ( CheckRoundForCardSet() )
		{
			g_pGameView->m_cPlayerDrawManager.SetTurn( pn );
		}
		g_pGameView->m_cAniSprListManager.BetCmd_ClearCmd( pn, 20 ); // 바로 삭제하지 않고 대기 후 삭제

		//리미트배팅 초기화( 새카드 오는 시점 )
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
	// 게임 결과 정보 
	ZeroMemory(&m_stGameOverResultData,sizeof(m_stGameOverResultData));	

	//규제안 관련하여 예약기능 모두 초기화.
	ReserveState(RESERVESTATE_BET_NONE);
	CanReserve(false);

	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();		// 버튼 초기화

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

	//베팅 커멘드 지움
	g_pGameView->m_cAniSprListManager.BetCmd_DelDelay(55);

	memcpy(&g_Poker,&pGOR->Pg,sizeof(g_Poker));
	memcpy(&m_stGameOverResultData,pGOR,sizeof(m_stGameOverResultData));

	g_pGameView->m_cAniSprListManager.Del_AniListOBj(ANF_BG_CUTINGINFO); // 바꿀 카드를

	for ( int i = 0; i < MAX_PLAYER ; i++ )
	{
		g_Poker.RU[ i ].nLastCmd = 0;				
		g_PLAYER( i )->ClearObjectEffect(ANF_PLAYER_ALLINMARK);
	}
	//카드 날아 가는거 중지 
	m_pMovingCardMgr->Destroy();

	g_pGameView->m_cPlayerDrawManager.SetHiddenCardShow( true );

	// 히든카드 위에 더미 카드 지운다.
	for ( int i = 0; i < TOTAL_CARD_BD; i++ )
	{
		m_cPlayer[ 0 ].m_cMyCard[ i ].SetHiddenCard( false );
	}

	for( int i=0; i<MAX_PLAYER; i++)
	{
		if ( strlen( m_stGameOverResultData.Ui[ i ].ID ) > 0 ) //  겜참여자.
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

			// 레벨상점 폐쇄
			/*m_cPlayer[ pnum ].UI.nIcon = GetMoneyGrade(m_cPlayer[ pnum ].UI.GetTotalMoney(IsTournament()));	*/
			m_cPlayer[ pnum ].UI.nIcon = GetMoneyGrade(m_cPlayer[ pnum ].UI.GetTotalMoney());	

			if (pnum == 0 && !g_ObserverMan.IsMyObserver() )
			{								
				m_cPlayer[ pnum ].UI.CopyAllGameRecords( GM().GetMyInfo()->UI.AllGameRecord );
				GM().GetMyInfo()->SetChangeRoomInfo(*m_cPlayer[pnum].GetChangeRoomInfo());
				/*GM().GetMyInfo()->UI.SetMoney(m_cPlayer[ pnum ].UI.GetMoney(IsTournament()), IsTournament());*/
				GM().GetMyInfo()->UI.SetMoney(m_cPlayer[ pnum ].UI.GetMoney());
				GM().GetMyInfo()->UI.nIcon   =  m_cPlayer[ pnum ].UI.nIcon;				
				//족보 표시를 안한다.. 결과 때는
				m_cPlayer[ pnum ].m_bShowValue = false;
			}
		}
	}

	m_pCuttingStageInfo->Clear(); //< CYS 100720 >
	
	//////////////////////////////////////////////////////////////////////////
	// 기권오픈 확인

	// 버튼 spr 인덱스가 0이면 본인 카드 오픈 On, 1 이면 Off
	bool bUIFoldOpen = false;
	if ( g_pGameView->m_cGameViewBtnMan.m_btnCardOpen.GetButtonNumber() == 0 ) bUIFoldOpen = true;

	for( int i = 0; i < MAX_PLAYER; i++ )
	{
		if ( strlen( pGOR->Ui[ i ].ID ) > 0 ) //  겜참여자.
		{
			int pnum = GetPlayerPNum( pGOR->Ui[ i ].ID );
			if( pnum == -1 ) continue;

			// 클라이언트 0번 자리는 보통 본인기준으로 본인 카드 정보가 들어 오기 때문에 UI정보를 확인해준다.
			if ( pnum == 0 && !g_ObserverMan.IsMyObserver() )
			{	
				if( !bUIFoldOpen ) continue;
			}
			
			// 기권이지만 카드 정보가 모두 0이 아니라면 기권오픈 상태로 카드 정보가들어 왔는걸로 간주
			int nCount = 0;
			
			for ( int k = 0; k < TOTAL_CARD_BD; k++ )
			{
				if( g_Poker.BC[ i ].nCard[ k ] > 0 ) nCount++;
			}
			
			// 본인카드오픈 상태를 확인한다.
			if( nCount > 0 && nCount < TOTAL_CARD_BD + 1 )
			{
				if( g_pGameView->m_cPlayerDrawManager.IsOpen() )
				{
					// 정상종료 : 기권하지 않는 유저의 모든 카드정보가 기본적으로 들어온다.( 기권을 했는데 카드 정보가 왔다면 본인카드오픈 상태 )
					// 2018.09.27 이슈 발생으로 기권 오픈 기능 제거
					//if( m_cPlayer[ pnum ].bFold ) m_cPlayer[ pnum ].bFoldOpen = true;	
				}
				else
				{
					// 기권승패 : 본인을 제외한 모든 유저의 카드 정보가 없다.( 정상종료 아닌데 카드 정보가 온다면 본인카드오픈 상태 )
					// 2018.09.27 이슈 발생으로 기권 오픈 기능 제거
					//m_cPlayer[ pnum ].bFoldOpen = true;	
				}
			}
		}
	}
	
	//////////////////////////////////////////////////////////////////////////
	// 결과 정보 입력
	for( int i = 0; i < MAX_PLAYER; i++ )
	{
		if ( strlen(pGOR->Ui[ i ].ID ) > 0 ) //  겜참여자.
		{
			int pnum = GetPlayerPNum(pGOR->Ui[ i ].ID);
			if(pnum == -1) continue;

			m_cPlayer[ pnum ].ReSetCardPostion();
			
			// IsOpen()은 정상적으로 게임을 끝까지 진행한 유무라서 게임을 끝까지 끝내더라도 기권오픈 상태가 있을 수 있어서 폴드와 같이 검사.
			if( g_pGameView->m_cPlayerDrawManager.IsOpen() && !m_cPlayer[ pnum ].bFold )
			{
				for ( int k = 0; k < TOTAL_CARD_BD; k++ )
				{
					m_cPlayer[ pnum ].m_cMyCard[ k ].SetCardNo( g_Poker.BC[ i ].nCard[ k ] ); // 값 변경
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
					m_cPlayer[ pnum ].m_cMyCard[ k ].SetCardNo( g_Poker.BC[ i ].nCard[ k ] ); // 값 변경
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
					// 기권오픈 상태에 따라 기권 유저라도 카드정보를 보여줄 수 있다.
					if( m_cPlayer[ pnum ].bFoldOpen )
					{
						for ( int k = 0; k < TOTAL_CARD_BD; k++ ) m_cPlayer[ pnum ].SetFaceUp( k );

						AutoSort_Recommand(); //< CYS 101201 >

						m_cPlayer[ pnum ].SetCardResult(); // 족보처리
						m_cPlayer[ pnum ].Draw_DarkCard(); // 족보에 따른 카드 회색처리
						m_cPlayer[ pnum ].FoldUser_CardOpen(); // 접힌 카드 연출을 다시 펴기위해 사용

						m_cPlayer[ pnum ].m_bShowValue = true;
					}
					else
					{
						// 기본적으로 카드를 덮은 상태
						for ( int k = 0; k < TOTAL_CARD_BD; k++ ) m_cPlayer[ pnum ].SetFaceDown( k );
					}
				}
				
			}
		}
	}	

	// 전광판 출력
	g_pGameView->m_cDisplayBoard.SetText(EDBT_RESULT, (LPARAM)&m_stGameOverResultData); // 다른 유저 족보 계산후 호출

	// 결과 출력 Delay를 주기 위함 
	m_nResultDelayCount = 20;
	m_bResultDelayStart = true;

	g_pGameView->m_cPlayerDrawManager.CheckResultValue();

	OnResultGameOverInfo(); // 결과에 보여줄 이펙트정보및 채팅창정보 미리저장
}

// Backup
void GameProcess_BD::OnResultGameOverInfo()
{
	int nWinnerNum = m_stGameOverResultData.nWinnerNum;
	if ( nWinnerNum < 0 || nWinnerNum >= 5)
	{
		return;
	}

	//윈 효과 
	const int FOLD = 2;
	const int WIN  = 1;
	const int LOSE = 0;	

	m_stPlayRTInfo.Clear();

	int nPlayerIndex = 0;	

	// 결과 저장
	for ( int i = 0; i < nWinnerNum ; i++ )
	{
		int nwinpnum = GetPNum_ByServPN( m_stGameOverResultData.nNowWinner[ i ] );

		if ( nwinpnum < 0 || nwinpnum >= 5 )
		{
			continue;
		}

		//하프 잭팟 프로모션 때문에 승자 정보를 저장.
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
			m_stPlayRTInfo.Data[ nPlayerIndex ].strValueName ="기권 승";
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

	//승패 그리기 패자 쪽	
	for( int i = 0 ; i < MAX_PLAYER ; i++ )
	{
		if ( strlen( m_stGameOverResultData.Ui[ i ].ID ) > 0 ) //  겜참여자.
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
				m_stPlayRTInfo.Data[ nPlayerIndex ].strValueName = "포기";	
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
	m_bGameStart = FALSE;				// 게임 시작 플래그 세팅	

	int nWinnerNum =m_stGameOverResultData.nWinnerNum;
	if( nWinnerNum < 0 || nWinnerNum >= 5)return;

	g_pGameView->m_cPlayerDrawManager.SetGameOver(true, timeGetTime());

	g_pGameView->m_cSidePotMoney.Check();

	int i = 0;
	//윈 효과 
	const int FOLD = 2;
	const int WIN  = 1;
	const int LOSE = 0;	

	MAP_RANKING mapRanking;
	for(int i=0; i< GetMaxPlayer(); i++)
	{
		if(strlen(m_stGameOverResultData.Ui[i].ID)>0) //  겜참여자.
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
		if ( m_stPlayRTInfo.Data[ i ].nPNum < 0 || m_stPlayRTInfo.Data[ i ].nServPNum < 0 ) // 유효한 유저
		{
			continue;
		}

		int nwinpnum = m_stPlayRTInfo.Data[ i ].nPNum;

		if ( nwinpnum < 0 || nwinpnum >= 5 )
		{
			continue;
		}

		if ( m_stPlayRTInfo.Data[ i ].nWinType == WIN ) // 승자
		{
			g_pGameView->m_cPlayerDrawManager.AddLowBDWinnerPNum( m_stPlayRTInfo.Data[ i ].nServPNum ); 

			//하프 잭팟 프로모션 때문에 승자 정보를 저장.
			int nWinPNum = GetPNum_ByServPN(m_stPlayRTInfo.Data[ i ].nServPNum);
			g_cPromotionMan.SetWinnerNum(nWinPNum) ;

			// 이펙트 그리기
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
				if ( nwinpnum == 0 ) // 본인만 사운드 겹치는 무제
				{
					bPlaySound = true;
					PBPlayEffectSound(  SND_WIN_LOW );
				}
			}
		}
		else // 패자
		{
			if ( nwinpnum == 0 ) // 본인만
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

	// 게임에 참여하지 않은 유저
	if ( !bPlaySound )
	{
		PBPlayEffectSound( SND_LOSE_NORMAL );
	}

	// 채팅창에 정보 찍기
	CString str="";
	CString strM="";

	for ( i = 0; i < MAX_PLAYER ; i++ )
	{
		if ( m_stPlayRTInfo.Data[ i ].nPNum < 0 ) // 유효한 유저
		{
			continue;
		}

		PLAYRESULTINFO *pData =	&m_stPlayRTInfo.Data[ i ];

		if ( !g_pGameView->m_cPlayerDrawManager.IsOpen() ) // 카드 오픈이 아니면 승자만 찍는다. 1명!!!
		{
			if ( pData->nWinType == WIN ) // 
			{
				CString str1;	
				//str1.Format(" ----------------------------\n%s ----------------------------\n",g_StrMan.Get(_T("WIN_MESINFO")));
				str1.Format(" %s",g_StrMan.Get(_T("WIN_MESINFO")));
				str.Format(str1, pData->strID.c_str() );		
				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT));	

				strM = (CString)NMBASE::UTIL::g_MoneyMark( pData->llResultMoney ); // 하이 머니 

				strM.Append(strChip_Name[g_RI.ChipKind]);

				// 골드 경기장일 경우
				if(g_RI.ChipKind==1)
				{					
					// 딜러비 할인 이벤트 중일 경우 표시
					if (m_stGameOverResultData.CommitionDiscount)
					{
						strM.Append("(딜러비 할인 이벤트중)");
					}
				}

				//str.Format(g_StrMan.Get(_T("BET_WIN")), strM); // 하이 승자 아디 ## 하이승:%s(+%s)		
				str.Format("◈ 획득금액:%s \n", strM); // 하이 승자 아디 ## 하이승:%s(+%s)	
				AddGameInfoViewEdit(str,GetChatColor(ECC2_RESULT_WINNER));

				str.Format("◈ 족보: 기권승 \n" );
				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_JOKBO));

				// 				str.Format(" ──────────────\n");
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
			if ( pData->nWinType == WIN ) // 카드 오픈승 승자 여러명 될 수 있다.
			{
				CString str1;
				//str1.Format(" ----------------------------\n%s ----------------------------\n",g_StrMan.Get(_T("WIN_MESINFO")));
				str1.Format(" %s",g_StrMan.Get(_T("WIN_MESINFO")));
				str.Format(str1, pData->strID.c_str());		

				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT));	

				INT64 nTotBat = GetRealBet();

				strM = (CString)NMBASE::UTIL::g_MoneyMark( pData->llResultMoney ); // 하이 머니 
				
				strM.Append(strChip_Name[g_RI.ChipKind]);

				// 골드 경기장일 경우
				if(g_RI.ChipKind==1)
				{					
					// 딜러비 할인 이벤트 중일 경우 표시
					if (m_stGameOverResultData.CommitionDiscount)
					{
						strM.Append("(딜러비 할인 이벤트중)");
					}
				}
				
				str.Format("◈ 획득금액:%s \n", strM); // 하이 승자 아디 ## 하이승:%s(+%s)	
				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_WINNER));

				str = "";
				str.Format("◈ 족보: %s \n", pData->strValueName.c_str() );
				AddGameInfoViewEdit(str, GetChatColor(ECC2_RESULT_JOKBO));

				// 				str.Format(" ──────────────\n");
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
			g_pGameView->m_cPlayerDrawManager.SetPlayerGameOver( i, true );// 게임 종료 상태
		}		
	}

	//종료 딜레이 때문에 일찍 도착한 보디가드 or 슈퍼 아바타  or 골든칩 정보를 수정한다.
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

	//잭팟 공지나 이벤트 공지 (카드 펼친후에 나와야 하는 공지들)
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
			//UPG작업 기존꺼 다 지우고 아래 한 줄 추가
			GM().BadUserReportGameChat();
		}break;
		/*=====================//베팅 커멘드=====================*/
	case IDM_DIE:
		{
			// 사운드 출력		
			OnBtnFold();
		}
		break;

	case IDM_CHECK:
		{
			// 사운드 출력		
			OnBtnCheck();		
		}
		break;

	case IDM_CALL:
		{
			// 사운드 출력		
			OnBtnCall();

		}
		break;

	case IDM_PING:
		{
			// 사운드 출력		
			OnBtnPing();
		}
		break;

	case IDM_DADANG:
		{
			// 사운드 출력		
			OnBtnDda();
		}
		break;	

	case IDM_FULL:
		{
			// 사운드 출력		
			OnBtnDda();
		}
		break;	

	case IDM_HALF:
		{
			// 사운드 출력			
			OnBtnMax();
			g_pGameView->m_cGameViewBtnMan.Reset();
		}
		break;

		//바둑이
	case IDM_CHANGE_BTN:
		{
			OnBtnChange();
			//규제안 : 방어코드
			g_pGameView->m_cGameViewBtnMan.m_bChangePassState = false;

			break;
		}

	case IDM_PASS_BTN:
		{
			OnBtnPass();
			//규제안 : 방어코드
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

		//카드족보 자동선택
	case IDM_GAME_AUTOUP:
		{
			NMBASE::SKINGDI::CGraphButton* pAutoUpBtn = g_pGameView->m_cGameViewBtnMan.GetGraphicBtn( CGameButtonBD::BTN_AUTOUP );

			// 변수없이 스프라이트 No로 사용유무 체크한다.
			if ( pAutoUpBtn )
			{
				int iButtonNumber = pAutoUpBtn->GetButtonNumber();  // Spr Index 
				pAutoUpBtn->SetSprite( &GLOBAL_SPR[ spr::GSPR_BTN_AUTOUP ], !iButtonNumber ); // 0 카드선택ON, 1 카드선택OFF 				
				g_Config.nAutoUpType = iButtonNumber; // 0 : 사용않함 1: 사용 2 : 선택 전
				NMBASE::UTIL::WriteProfile_int( GM().GetMyInfo()->UI.ID, "AutoUpType", g_Config.nAutoUpType ); 

				//클릭로그를 남긴다.
				if ( g_Config.nAutoUpType == TRUE ){
					SendUILog_Basic( CSSTATUS_UINO_LOWBD_ROOM_MYCARDAREA_AUTO_SELECT_TO_ON );
				}else{
					SendUILog_Basic( CSSTATUS_UINO_LOWBD_ROOM_MYCARDAREA_AUTO_SELECT_TO_OFF );
				}
			}
			break;
		}
		//자동정렬 버튼추가:2012.09.27 YS.Jeong
	case IDM_GAME_AUTOSORT:
		{
			NMBASE::SKINGDI::CGraphButton* pAutoSortBtn = g_pGameView->m_cGameViewBtnMan.GetGraphicBtn( CGameButtonBD::BTN_AUTOSORT );

			// 변수없이 스프라이트 No로 사용유무 체크한다.
			if ( pAutoSortBtn )
			{
				int iButtonNumber = pAutoSortBtn->GetButtonNumber();  // Spr Index 
				pAutoSortBtn->SetSprite( &GLOBAL_SPR[ spr::GSPR_BTN_AUTOSORT ], !iButtonNumber ); // 0 카드정렬ON, 1 카드정렬OFF 
				g_Config.nAutoSortType = iButtonNumber; // 0 : 사용않함 1: 사용 2 : 선택 전
				NMBASE::UTIL::WriteProfile_int( GM().GetMyInfo()->UI.ID, "AutoSortType", g_Config.nAutoSortType ); 

				AutoSort_Recommand(0, TRUE); 

				//클릭로그를 남긴다.
				if ( g_Config.nAutoSortType == TRUE ){
					SendUILog_Basic( CSSTATUS_UINO_LOWBD_ROOM_MYCARDAREA_AUTO_SORT_TO_ON );
				}else{
					SendUILog_Basic( CSSTATUS_UINO_LOWBD_ROOM_MYCARDAREA_AUTO_SORT_TO_OFF );
				}
			}
			break;
		}
	// 본인 카드 오픈 버튼
	case IDM_CARD_OPEN:
		{
			// 버튼 spr 인덱스가 0이면 본인 카드 오픈 On, 1 이면 Off
			int nPrevBtnNum = g_pGameView->m_cGameViewBtnMan.m_btnCardOpen.GetButtonNumber(); // 현재 버튼 상태(On or Off)
			//int nBtnNum = 0;
			bool bUIFoldOpen = false;

			// 버튼이 Off 면 On 시켜줌 -> 버튼 상태 버튼은 서버 결과에 따라 하기로 함
			if (nPrevBtnNum == 1)
			{
				bUIFoldOpen = true;
			}
			
			// 버튼 spr 인덱스를 Toggle 시킴
			//g_pGameView->m_cGameViewBtnMan.m_btnCardOpen.SetSprite(&(g_pGameView->m_cGameViewBtnMan.m_sprUICardOpenBtn), nBtnNum);
			
			// 서버에 알림
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

			//원샷충전
			if(g_cOneShotCharge.OnCommand(wParam, lParam)) return TRUE;

			//자리 지킴이
			if(g_SeatKeeper.OnCommand(wParam, lParam)) return TRUE;

			return false;
		}
	}

	return TRUE;
}

// [세븐 포커] 베팅 버튼
void GameProcess_BD::OnBtnPing( bool bUserAction )
{
	if(g_ObserverMan.IsMyObserver() == TRUE) return;
	if( !g_pGameView->m_cGameViewBtnMan.m_bBPing || g_PLAYER(0)->PlayState == 0) return;

	// 타임아웃 카운트 초기화
	if ( bUserAction )
	{
		m_cPlayer[ 0 ].ResetTurnLimitCnt();
	}

	// 따당이 예약되어 있어도 상태방이 체크를 선택해서 따당이 실패하면 예약 취소를 위해서 넣어줌.
	SetReserveButton(RESERVESTATE_BET_NONE);

	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();

	POKERCLNT pc; //MakeBasic(pc);
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 3; // 배팅
	pc.PlayTick = g_pGameView->GetCurrentPlayTick(); 


	pc.nBtn   = BET_CMD_PING ; // 삥

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  g_PLAYER(0)->UI.UNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.ID, &pc);
	SetCardExchangeLog(raise);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), raise.pData, raise.GetTotalSize());

	m_bBtnClick = TRUE;

}

// 따당 -> 풀버튼 기능으로 변경(규제안관련 작업 2014.01)
void GameProcess_BD::OnBtnDda( bool bUserAction )
{
	if(g_ObserverMan.IsMyObserver() == TRUE) return;
	if( !g_pGameView->m_cGameViewBtnMan.m_bBDDa || g_PLAYER(0)->PlayState == 0) return;

	// 2014.2.7 crpark
	// 규제안 베팅예약 공통화
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

	// 타임아웃 카운트 초기화
	if ( bUserAction )
	{
		m_cPlayer[ 0 ].ResetTurnLimitCnt();
	}

	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();

	POKERCLNT pc; //MakeBasic(pc);
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 3; // 배팅
	pc.PlayTick = g_pGameView->GetCurrentPlayTick(); 

#if defined(BUTTON_CHANGE_FULLTODADANG)
	if(true==g_RI.bIsFriendRoom && m_cPlayer[ 0 ].UI.GetMoney() >= GM().GetLossMaxMoneyForGame()){
		pc.nBtn   = BET_CMD_MAX_BET;
	}
	else if(true==GM().IsChangeFullDDang()){
		pc.nBtn   = BET_CMD_FULL;
	}
	else{
		pc.nBtn   = BET_CMD_DADANG;//BET_CMD_FULL;//BET_CMD_DADANG ; // 따당에서 풀로교체(규제안관련 작업 2014.01)->추후작업
	}
#else
	pc.nBtn   = BET_CMD_DADANG;//BET_CMD_FULL;//BET_CMD_DADANG ; // 따당에서 풀로교체(규제안관련 작업 2014.01)->추후작업
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
	// 규제안 베팅예약 공통화
	if(false == CheckMyTurn(GetPlayer(0)->ServPNum, m_nCurPlayer))
	{
		SetReserveButton(RESERVESTATE_BET_BET_CMD_FOLD);
		return;
	}

	SetReserveButton(RESERVESTATE_BET_NONE);
	//규제안 :  예약 가능여부 설정
	CanReserve(false);
	
	// 타임아웃 카운트 초기화
	if ( bUserAction )
	{
		m_cPlayer[ 0 ].ResetTurnLimitCnt();
	}

	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();

	POKERCLNT pc; 
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 0; // 게임포기
	pc.nBtn   = 0; // 삥
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

	// 타임아웃 카운트 초기화
	if ( bUserAction )
	{
		m_cPlayer[ 0 ].ResetTurnLimitCnt();
	}

	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();

	POKERCLNT pc; //MakeBasic(pc);
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 1; // 배팅
	pc.nBtn   = 0; // 삥
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
	// 규제안 베팅예약 공통화
	if(false == CheckMyTurn(GetPlayer(0)->ServPNum, m_nCurPlayer))
	{
		SetReserveButton(RESERVESTATE_BET_BET_CMD_CALL);
		return;
	}
	SetReserveButton(RESERVESTATE_BET_NONE);


	// 타임아웃 카운트 초기화
	if ( bUserAction )
	{
		m_cPlayer[ 0 ].ResetTurnLimitCnt();
	}

	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();

	POKERCLNT pc; 
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 2; // 배팅
	pc.nBtn   = 0; // 삥
	pc.PlayTick = g_pGameView->GetCurrentPlayTick(); 

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  g_PLAYER(0)->UI.UNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.ID, &pc);
	SetCardExchangeLog(raise);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), raise.pData, raise.GetTotalSize());
	m_bBtnClick = TRUE;
}

//하프
void GameProcess_BD::OnBtnMax( bool bUserAction )
{
	if(g_ObserverMan.IsMyObserver() == TRUE) return;
	if( !g_pGameView->m_cGameViewBtnMan.m_bBMax || g_PLAYER(0)->PlayState == 0 ) return;

	// 2014.2.7 crpark
	// 규제안 베팅예약 공통화
	if(false == CheckMyTurn(GetPlayer(0)->ServPNum, m_nCurPlayer))
	{
		SetReserveButton(RESERVESTATE_BET_BET_CMD_HALF);
		return;
	}
	SetReserveButton(RESERVESTATE_BET_NONE);


	// 타임아웃 카운트 초기화
	if ( bUserAction )
	{
		m_cPlayer[ 0 ].ResetTurnLimitCnt();
	}

	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();

	POKERCLNT pc; //MakeBasic(pc);
	ZeroMemory(&pc, sizeof(POKERCLNT));
	pc.nStyle = 3; // 배팅

	int spnum = GetServPNum_ByPN(0);			
	INT64 CallMoney = GetRaiseBat() - g_Poker.RU[spnum].nNowBat;
	INT64 nTotal = GetRealBet();

	if( m_cPlayer[ 0 ].GetNowMoney() > CallMoney && m_cPlayer[ 0 ].GetNowMoney() < GetMaxMoney( nTotal ))
	{
		pc.nBtn   = BET_CMD_ALLIN; // 올인
	} 
	else{
		pc.nBtn   = BET_CMD_HALF; // 맥스
	}

	pc.PlayTick = g_pGameView->GetCurrentPlayTick(); 

	CCL_ASK_RAISE raise;
	raise.Set(g_RI.RoomNum,  g_PLAYER(0)->UI.UNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.ID, &pc);
	SetCardExchangeLog(raise);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), raise.pData, raise.GetTotalSize());
	m_bBtnClick = TRUE;	
}

// bPaticipation = TRUE  관전 -> 참여
// bPaticipation = FALSE 참여 -> 관전
void GameProcess_BD::SetPlayInfo(int nowpnum, int newpnum, BOOL bParticipation )
{
	//백업용 - 포인터 사용할까?
	USERINFO tmpUi[MAX_PLAYER];
	PLAYSTATE tmpPs[MAX_PLAYER] = {0,};
	BACKCARD  tmpBC[MAX_PLAYER] = {0,};
	CHANGE_USERINFO_ROOM tmpCUi[MAX_PLAYER];

	ZeroMemory(tmpUi, sizeof(USERINFO) * MAX_PLAYER);
	ZeroMemory(tmpCUi, sizeof(CHANGE_USERINFO_ROOM) * MAX_PLAYER);

	struct tempcarddeck{
		char nCard[10];

		CUTCOUNT_INFO stCutInfo; //커팅 횟수 저장
	};

	int				tmpSelectCard[MAX_PLAYER] = {0,};
	tempcarddeck	tCardDeck[MAX_PLAYER] = {0,};	

	if( !bParticipation ) {
		//현재 겜참여 중이기 때문에 0 번 인덱스
		//기본 정보 초기화
		m_cPlayer[0].Clear();
	}	

	// 기존 플레이 정보 backup
	int pn = 0, k;
	for(int i=0; i<MAX_PLAYER; i++) {
		if(strlen(m_cPlayer[i].UI.ID)==0) continue;
		if(bParticipation )// 관전 -> 참여
			pn = i;
		else
			pn = GetServPNum_ByPN(i);
		memcpy(&tmpUi[pn], &m_cPlayer[i].UI, sizeof(USERINFO));
		memcpy(&tmpCUi[pn], m_cPlayer[i].GetChangeRoomInfo(), sizeof(CHANGE_USERINFO_ROOM) );
		tmpPs[pn].JoinState = m_cPlayer[i].JoinState;
		tmpPs[pn].PlayState = m_cPlayer[i].PlayState;

		tmpPs[pn].bFold = m_cPlayer[i].bFold;
		tmpPs[pn].nCardTotal= m_cPlayer[i].m_nCardTotal;				

		//서버로부터 다시 받는다
		// ### [ 중복된 IP가 ] ###
		//tmpPs[pn].nCheckIp = m_cPlayer[i].MyIpCheck;
		// ### [사운드 추가 작업] ###
		tmpPs[pn].nSndFxKind = m_cPlayer[i].nSndFxKind;
		tmpPs[pn].nOpenTotal= m_cPlayer[i].m_nOpenTotal;

		int tcnt = m_cPlayer[i].m_nCardTotal;
		for(k = 0; k < tcnt ; k++) {
			tCardDeck[pn].nCard[k] = m_cPlayer[i].m_cMyCard[ k ].GetCardNo();
		}
		
		// 커팅 정보 복사( 커팅 횟수 디파인값이 없어서 3회 입력 )
		for(int c = 0; c < 3 ; c++) 
		{
			tCardDeck[ pn ].stCutInfo.m_arrCutCount[ c ] = m_cPlayer[ i ].GetCutInfo().m_arrCutCount[ c ];
			tCardDeck[ pn ].stCutInfo.m_arrbShow[ c ] = m_cPlayer[ i ].GetCutInfo().m_arrbShow[ c ];
		}
	}

	//기본 정보 초기화
	if(bParticipation) {   // 관전 -> 참여
		GM().GetMyInfo()->JoinState = 0;
		m_nMyPosPNum = newpnum; // 서버쪽에 새로운 인덱스
		m_nNo1PosPNum = GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스

		m_cPlayer[nowpnum].Clear();
	}
	else {	//참여 -> 관전
		GM().GetMyInfo()->JoinState = 0;
		m_nMyPosPNum = 0; // 서버쪽에 새로운 인덱스
		m_nNo1PosPNum = GetPNum_ByServPN(0); // 서버쪽의 0 번 인덱스
	}

	g_pGameView->m_cPlayerDrawManager.OnExitRoom();

	// 게임 정보 모두 초기화
	for( int i=0; i<MAX_PLAYER; i++) {
		m_cPlayer[i].Clear();
		m_cPlayer[i].ServPNum = GetServPNum_ByPN(i); // 초기화 되지 않는다.
	}


	if(bParticipation ) {	// 관전 -> 참여
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

		//관전자 이미지 그리기 삭제
		g_pGameView->m_cStayAniSprListManager.Del_AniListOBj(ANF_PLAYER_OBSERVER);		
	}
	else {   //참여 -> 관전
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

		//관전자 이미지 그리기


		//관전UI		
		g_ObserverMan.OnObserve();
	}

	// 플레이어 순서에 맞게 초기화한다
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
			m_cPlayer[pn].nSndFxKind = tmpPs[i].nSndFxKind; // ### [사운드 추가 작업] ###

			// 레벨상점 폐쇄
			/*m_cPlayer[pn].UI.nIcon = GetMoneyGrade(tmpUi[i].GetTotalMoney(IsTournament()));*/
			m_cPlayer[pn].UI.nIcon = GetMoneyGrade(tmpUi[i].GetTotalMoney());
			m_cPlayer[pn].SetChangeRoomInfo(tmpCUi[i]);
			
			// 커팅 정보 복사( 커팅 횟수 디파인값이 없어서 3회 입력 )
			for(int c = 0; c < 3 ; c++) 
			{
				m_cPlayer[ pn ].GetCutInfo().m_arrCutCount[ c ] = tCardDeck[ i ].stCutInfo.m_arrCutCount[ c ];
				m_cPlayer[ pn ].GetCutInfo().m_arrbShow[ c ] = tCardDeck[ i ].stCutInfo.m_arrbShow[ c ];
			}

			if ( g_Poker.nState <= RSTATE_RESULT ) //< CYS 101207 > 게임결과때도 카드가 있다면 보여준다.
			{
				// 게임 진행 상황 그릴것 카드 정보를 카드덱에다가 셋
				if ( m_cPlayer[pn].JoinState == 1 ) 
				{
					int nTotCnt = tmpPs[i].nCardTotal;
					int nOpCnt =  tmpPs[i].nOpenTotal;

					m_cPlayer[pn].bFold = tmpPs[i].bFold;
					m_cPlayer[pn].m_nCardTotal = nTotCnt;
					m_cPlayer[pn].m_nOpenTotal = nOpCnt;
					for(k = 0; k < nTotCnt; k++) {
						m_cPlayer[pn].SetNewCard(DUMY_CARD); // 카운트 자동증가
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

	// bPaticipation = TRUE  관전 -> 참여
	// bPaticipation = FALSE 참여 -> 관전
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
		str.Format("관전 → 참여\n");
	}
	else 
	{
		str.Format("참여 → 관전\n");
	}
	AddGameInfoViewEdit( str, GetChatColor( ECC2_NOTIFY ) );


	// 오토배팅 셋팅
	if ( !g_ObserverMan.IsMyObserver() )
	{		
		LAYER_GLOBAL_AUTOBETWND()->OnEnterRoom();
	}

	//< CYS 110124 관전자 버튼 숨김
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

	
	//관전자 자동정렬버튼 숨김:2012.09.27 YS.Jeong
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

	//pUserID : 머니 변화가 일어난 유저아이디 (내가 될수도 있고 내가 있는 방의 다른 유저가 될 수도있다.)
	//pUMCode : 머니 변화가 일어난 메타코드
	//ChnageMoney : 변화된 머니량
	//PMoney : 변화 후 머니 

	if(strcmp(pUserID, GM().GetMyInfo()->UI.ID) == 0) //내머니 라면?
	{
		/*GM().GetMyInfo()->UI.SetMoney(PMoney, IsTournament());*/
		/*GM().GetMyInfo()->UI.SetMoney(PMoney);*/
		GM().GetMyInfo()->UI.SetRealPMoney(PMoney);
		if( !g_ObserverMan.IsMyObserver() ) 
		{
			/*g_PLAYER(0)->UI.SetMoney(PMoney, IsTournament());*/
			/*g_PLAYER(0)->UI.SetMoney(PMoney);*/
			g_PLAYER(0)->UI.SetRealPMoney(PMoney);
			//g_PLAYER(0)->PrevMoney = g_PLAYER(0)->UI.GetPMoney();//PMONEY , PMONEY2 통합시에 아래 살리기 //@#$%
		}
	}
	else	//상대방이라면?
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
	//각 케이스별로 애니메이션 효과 , 팝업 등을 보여준다.
	// 뿌려줄 메세지도 설정한다.
	CString sysmsg;	

	if( strUMCode == "ABB02019" || strUMCode == "ABB02020" || strUMCode == "ABB02021" ) //원샷(재기)충전 메타코드 
	{
		//원샷(재기)충전
		g_cOneShotCharge.OnChargeMoney(*pMsg);
	}


	if(!sysmsg.IsEmpty()) {
		if(GetCurrentWhere()==IDX_GAMEWHERE_LOBY)
		{
			//UPG작업 추가
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

//UPG작업 추가
void GameProcess_BD::OnEnterRoom(BOOL bCreation)
{
	//내가 방에 들어간다 (bCreation은 방을 생성해서 입장하는지 여부)

	// 본인 카드 오픈 버튼 상태 Off로 초기화(1이면 Off, 0이면 On)
	g_pGameView->m_cGameViewBtnMan.m_btnCardOpen.SetSprite(&(g_pGameView->m_cGameViewBtnMan.m_sprUICardOpenBtn), 1);

	// 게임변경시 항상 초기화
	g_pGameView->m_cGameViewBtnMan.InitializeKeyValue( g_Config.eBetBtnType );// 바둑이 커팅키 초기화문제

	std::string strMsg;
#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
		strMsg = "톡톡 로우바둑이";
	else
		strMsg = "윈조이 로우바둑이";
#else //UPGRADE_20120308_TOCTOC_CHANNELING
	strMsg = "윈조이 로우바둑이";
#endif //UPGRADE_20120308_TOCTOC_CHANNELING

	SetMainWindowText( strMsg.c_str() );

	g_pGameView->m_cBackGround.OnEnterRoom();

	g_pGameView->m_cSidePotMoney.Reset();
	g_pGameView->m_cGameViewBtnMan.m_btnExitBottom.Enable(TRUE);
	g_pGameView->m_cGameViewBtnMan.InitObserverBtn();
	//채팅
	g_pChatWinMan->SetViewPage(CChattingWindowMan::ECTT_CHAT);
	g_pChatWinMan->ResetChatting_STR();

	// 토너먼트일때에는 귓말 상대 리스트 노출 버튼 enable
	//귓말 삭제
	/*
	if (IsTournament())
		g_pChatWinMan->SetLiftBtnEnable(false);
	else
		g_pChatWinMan->SetLiftBtnEnable(true);
	*/

	// 포커스
	g_pGameView->SetFocus();
	g_pGameView->Reset();	
	strMsg = "☆도움말☆\n1. 게임창->채팅창 포커스 이동\n단축키 - Enter, ESC\n2. 채팅창->게임창 포커스 이동\n단축키 - ESC\n";		
	AddGameInfoViewEdit( strMsg.c_str(), GetChatColor( ECC2_NOTIFY ));

	g_pGameView->m_cGameViewBtnMan.SetBetBtnType( g_Config.eBetBtnType, true );
	g_pGameView->m_cGameViewBtnMan.EnterRoom_Button_Init( g_RI.sBetRuleKind );
	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable();

	strMsg = "비정상적인 게임행위(짜고치기) 및 사행성 조장 행위(거래,광고)등이 발생할 시 게임 내 신고 버튼을 이용하여, 신고해 주시기 바랍니다.\n";
	AddGameInfoViewEdit( strMsg.c_str(), GetChatColor(ECC2_NOTIFY) );

	//자리지킴이
	g_SeatKeeper.OnEnterGame();
	g_cOneShotCharge.OnEnterRoom();
	g_ObserverMan.OnEnterRoom();

	g_cVipJackpot.OnEnterRoom();

	//보유한도
	g_cOwnLimit.OnEnterRoom(&g_cUIData, g_GAME_BD());

	//관전자 채팅 경고
	m_bObserverChatWarning = FALSE;

	// 전광판에 내용 출력
	if ( !g_ObserverMan.IsMyObserver() )
	{
		CheckMinimumUser();
	}

	CGameProcess::OnEnterRoom(bCreation);

	CString str;
	if(g_ObserverMan.IsMyObserver())
	{
		str.Format("▶관전 입장: [%s]님\n", GM().GetMyInfo()->UI.NickName);
	}
	else
	{
		str.Format("▶입장: [%s]님\n", GM().GetMyInfo()->UI.NickName);
	}

	AddGameInfoViewEdit(str, GetChatColor(ECC2_NOTIFY));

	str.Format("## <ID: %s(%s)>님 입장 ##\n", GM().GetMyInfo()->UI.ID , GM().GetMyInfo()->UI.NickName);
	AddLobbyHideChatText( &str );

	// 잭팟금액을 세팅해준다. 
	// ( 잭팟 패킷이 게임입장 패킷보다 늦게 올수 있어서 값이 없으면 대기실에서 받은 잭팟 금액으로 세팅을 해준다. )
	if(GetJackPotMoney() <= 0)
	{
		JackPotMoneyUpdate(&g_JackPotMoneyInfo);
	}
	g_cPromotionMan.OnEnterRoom();
}

//UPG작업 추가
void GameProcess_BD::OnExitRoom()
{
	g_ObserverMan.SetMyObserver(FALSE);

	// ### [ 관전기능 ] ###
	for(int i=1; i<GetMaxNewPlayer(); i++) 
		m_cPlayer[ i ].Clear();


	// 방 목록 및 사용자 목록 리스트를 요구	
	g_pGameView->m_cAniSprListManager.Destroy();
	g_pGameView->m_cStayAniSprListManager.Destroy();
	g_pGameView->m_cTotalBetMoney.Reset();

	//자리 지킴이
	g_SeatKeeper.OnOutRoom();
	g_cVipJackpot.OnOutRoom();

	g_pChatWinMan->ResetChatting_STR();

	NMBASE::SOUND::g_Sound.Stop(SND_HIDDEN_BGM);

	m_i64JackPotMoney = 0;
	m_i64OldJackPotMoney = 0;

	// 토너먼트일 경우에 귓말 유저 리스트 버튼을 disable 하기 때문에 방에서 나갈때 무조건 유저리스트 버튼을 enable 해준다.
	//귓말 삭제
	//g_pChatWinMan->SetLiftBtnEnable(true);

	CGameProcess::OnExitRoom();

	// 버그로그 Clear()
	//NMBASE::UTIL::BugSplatWrapper::GetInstance().ClearMemLog();
	NMBASE::SOCK::CSockMan::SetSocketLogTxt("");

	//보유한도
	g_cOwnLimit.OnExitRoom();
}


//UPG작업 추가 (기존 GameDlg::PreTranslateMessage() 에 있던 부분을 가져옴)
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

	if ( pMsg->message == WM_MOUSEWHEEL ) // 채팅창에서 마우스 휠
	{
		if( g_pChatWinMan != NULL && g_pChatWinMan->GetChatView() != NULL )
		{
			g_pChatWinMan->GetChatView()->OnMouseWheel(pMsg->wParam, pMsg->lParam);			
		}		
	} 
	///////////////////////////////////////////////////////////////
	else if ( pMsg->message == WM_KEYDOWN ) 
	{	
		// 숫자키패드 게임키 추가 2004.07.21
		int scankey = 0, extended = 0, previous = 0;
		bool bGameKey = IsSacnCode(pMsg->lParam, scankey, extended, previous);

		// 복사 붙여넣기 막는 작업.
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

		// 센드 에디트가 아닌 다른곳에 포커스가 있다면 게임키로 인식
		//if ( g_pGameView->GetFocus() != (CWnd*)g_pChatWinMan->GetSendEdit()->pSendEdit )
		if(!g_pChatWinMan->IsEditFocus())
		{
			//////////
			if ( scankey == rule::SKEY_ENTER && extended == 0 || scankey == rule::SKEY_ESC )
			{
				g_pChatWinMan->GetSendEdit()->pSendEdit->SetFocus();
				return TRUE;
			}

			//추가
			OnGameKey( scankey, extended, previous );

			return TRUE;
		}
		else 
		{
			// 대화 내용이 없을때
			// 채팅 창에서 예외 키
			char szChatMsg[ MAX_PATH ] = { 0,};//대화 내용 읽어 오기 			

			g_pChatWinMan->GetSendEdit()->pSendEdit->GetWindowText( szChatMsg, 128 );

			if ( strlen( szChatMsg ) == 0 )
			{
				std::vector< DWORD >::iterator iterNotMsg = m_vtExceptionKey_NotMsg.begin();

				while ( iterNotMsg != m_vtExceptionKey_NotMsg.end() )
				{
					if ( IS_VKEY( *iterNotMsg, scankey, extended ) )
					{
						g_pGameView->SetFocus();
						OnGameKey( scankey, extended, previous ); //수정
						return TRUE;
					}
					++iterNotMsg;
				}
			}

			// 대화 내용이 있어도
			// 채팅 창에서 예외 키
			std::vector< DWORD >::iterator iterMsg = m_vtExceptionKey_Msg.begin();

			while ( iterMsg != m_vtExceptionKey_Msg.end() )
			{
				if ( IS_VKEY( *iterMsg, scankey, extended ) )
				{
					OnGameKey( scankey, extended, previous ); //수정
					return TRUE;
				}
				++iterMsg;
			}

			if ( scankey == rule::SKEY_ESC ) 
			{
				// 게임뷰에 포커스를 옮긴다
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

//UPG작업 함수추가
bool GameProcess_BD::IsSacnCode(int nScanCode, int &r_scankey, int &r_extended, int &r_previous)
{
	// 스캔코드를분석함
	// 스캔키코드
	r_scankey = (nScanCode >> 16) & 0x000000FF; 
	// 확장키인지여부(1이면확장키임- 숫자키패드방향키는비확장키보드임)
	r_extended = (nScanCode >> 24) & 0x00000001; 
	// 키의이전상태(1이면이전에눌려진상태임)
	r_previous = (nScanCode >> 30) & 0x00000001; 

	// 게임에서사용하는키인가검사
	if( ( r_scankey == 79 && r_extended == 0 ) || // 키패드1		가로삥		| 세로따당
		( r_scankey == 80 && r_extended == 0 ) || // 키패드2		가로따당	| 세로쿼터
		( r_scankey == 81 && r_extended == 0 ) || // 키패드3		가로쿼터	|
		( r_scankey == 75 && r_extended == 0 ) || // 키패드4		가로다이	| 세로삥
		( r_scankey == 76 && r_extended == 0 ) || // 키패드5		가로체크	| 세로체크
		( r_scankey == 77 && r_extended == 0 ) || // 키패드6		가로콜		|
		( r_scankey == 71 && r_extended == 0 ) || // 키패드7					| 세로콜
		( r_scankey == 72 && r_extended == 0 ) || // 키패드8					| 세로다이
		( r_scankey == 73 && r_extended == 0 ) || // 키패드9					| 
		( r_scankey == 82 && r_extended == 0 ) || // 키패드0					| 세로하프
		( r_scankey == 83 && r_extended == 0 ) || // 키패드.					| 세로하프
		( r_scankey == 53 && r_extended == 1 ) || // 키패드/					| 세로하프
		( r_scankey == 55 && r_extended == 0 ) || // 키패드*					| 세로하프
		( r_scankey == 74 && r_extended == 0 ) || // 키패드-					| 세로하프
		( r_scankey == 78 && r_extended == 0 ) || // 키패드+		가로하프	|
		( r_scankey == 28 && r_extended == 1 )   // 키패드enter	가로풀		|
		)  // 콜
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

	g_GameSpeed.set_CardDistributeDelay( i, j, 20 ); // 처음 시작 대기시간 후 카드 날리기
	g_GameSpeed.set_CardSpeed( i, j, 7.1f );
	g_GameSpeed.set_CardFlyDelay( i, j, 4 );		// 첫번째 카드와 두번째 카드 딜레이
	//	g_GameSpeed.set_BetCmdActiveDelay( i, j, 4 );
	//	g_GameSpeed.set_BetCmdActiveDelay( i, j, 5 );			// 게임사업 1팀의 요청으로 변경
	g_GameSpeed.set_BetCmdActiveDelay( i, j, 6 );			// 게임사업 1팀의 요청으로 변경. 2012. 8. 14 류승민
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
	//규제안 관련 수정 : 본인턴이 안닌 상태에서 키를 누르면 반응하도록 수정 (예약기능)
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
	// 현재턴이라면베팅키와겹치는부분은예외처리!
	if ( m_cPlayer[ 0 ].JoinState == 1 &&  m_cPlayer[ 0 ].PlayState == 1 && ! m_cPlayer[ 0 ].bFold &&  m_cPlayer[ 0 ].PNum == 0 )
	{
		for ( int i = KEY_CUT_1; i <= KEY_CUT_4; i++ )
		{
			int nMaxKey = MAX_GAME_KEY;

			//< CYS 100720 >
			if ( g_Poker.nState == RSTATE_STARTGAME ) // 베팅
			{
				nMaxKey = 1; // [ 0 ] F1, F2.... 만체크
			}

			// 1 ~ 4 & 커팅타임 & 이미 했다면 더이상 선택않됨
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
	// 히든 카드 오픈 방법	
	if ( m_pMovingCardMgr->IsCardMoving( 0 ) == false )
	{		
		//히든 카드를 가지고 있을때만 
		if ( !m_bHaveHiddenCard )
		{
			return false;		
		}

		// 오픈완료
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
							// 							if ( j == 1 && g_Poker.CurPlayer == m_cPlayer[ 0 ].ServPNum) //< CYS 100705 > // 내턴에 delete 키는 베팅시 제외
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
								m_cPlayer[ 0 ].m_bShowValue = true;		// 족보 보인다.			
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
				// 실수로 같은키 셋팅시 2번 호출방지
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

	//미리 눌렀다
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
		
		// 현재 자기 턴 구분
		m_pCuttingStageInfo->bMyTurn = true;			// 현재 내 턴
		m_pCuttingStageInfo->bChangeStage = false;		// 커팅유무
		m_pCuttingStageInfo->bChangeCardFlyEnd = true; // 커팅해서 카드 날리는중

		m_cPlayer[ 0 ].m_bShowValue = false; // 족보 숨기기
	}
	else  // pass
	{
		int PosIndex[ 5 ] = { 0,};
		int CardIndex[ 5 ] = { 0,};

		CCL_BD_CARD_EXCHANGE msg;
		msg.Set( g_RI.RoomNum, m_cPlayer[ 0 ].ServPNum, m_cPlayer[ 0 ].UI.UNum, m_cPlayer[ 0 ].UI.ID, 0, PosIndex, CardIndex );

		SetCardExchangeLog(msg);

		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());	

		// ##### 현재 자기 턴 구분
		// ##### m_pCuttingStageInfo->Clear();  // 패킷으로 대치한다. 서버로 완료메시지 보내지 않는다. 
		// ##### 서버로 패킷 보내는거로 수정!

		// 현재 자기 턴 구분
		m_pCuttingStageInfo->bMyTurn = true;			// 현재 내 턴
		m_pCuttingStageInfo->bChangeStage = false;		// 커팅유무
		m_pCuttingStageInfo->bChangeCardFlyEnd = true; // 커팅해서 카드 날리는중
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
			m_nBackUpPosIndex[ nIndex ] = i; // 서버, 클라 카드 인덱스가 다르다. ( 백업용 같이 사용 )

			nCardIndex[ nIndex ] = m_cPlayer[ 0 ].m_cMyCard[ i ].GetCardNo();
			nIndex++;
		}
	}

	//아무것도 선택하지 않으면 보내지 않는다.
	if ( nIndex <= 0 )return;

	m_bChangeState = false;

	SetEnableCuttingBtn( false );
	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable(true);

	//< CYS 100705
	int nPNum = GetPNum_ByServPN( m_nCurPlayer );
	if ( m_cPlayer[ 0 ].PNum == nPNum ) // 현재 턴이면 정상 커팅진행
	{
		CCL_BD_CARD_EXCHANGE msg;
		msg.Set(g_RI.RoomNum,m_cPlayer[0].ServPNum,m_cPlayer[0].UI.UNum,m_cPlayer[0].UI.ID, nIndex, nPosIndex, nCardIndex);

		SetCardExchangeLog(msg);

		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());	
	}
	else // 백업
	{
		m_bReserveChange =  true; // 미리진행
		m_eCHMODE = E_CHANGE;

		for ( int i = 0; i < nIndex; i++ )
		{
			m_nBackUpPosIndex[ i ] = nPosIndex[ i ];
			m_nBackUpCardIndex[ i ] = nCardIndex[ i ];
		}

		m_nBackUpChangedNum = nIndex; //< CYS 100705 >

		m_pCuttingStageInfo->bChangeCardFlyEnd = false; // 예약
	}
	//> CYS 100705	


	m_cPlayer[ 0 ].m_bShowValue = false;		// 카드 족보 숨긴다
	m_cPlayer[ 0 ].m_bDontSelectCard = true;	// 더이상 카드 선택 불가
	// 현재 자기 턴 구분
	m_pCuttingStageInfo->bChangeStage = false;  // 커팅 유무 ( 커팅했다 )
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
	if ( m_cPlayer[ 0 ].PNum == nPNum ) // 현재 턴이면 정상 커팅진행
	{
		CCL_BD_CARD_EXCHANGE msg;
		msg.Set(g_RI.RoomNum,m_cPlayer[0].ServPNum,m_cPlayer[0].UI.UNum,m_cPlayer[0].UI.ID,0,PosIndex,CardIndex);

		SetCardExchangeLog(msg);

		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());	
	}
	else
	{
		m_bReserveChange =  true; // 미리진행
		m_eCHMODE =  E_PASS;
	}
	//> CYS 100705

	// ##### 현재 자기 턴 구분
	// ##### m_pCuttingStageInfo->Clear();  // 패킷으로 대치한다. 서버로 완료메시지 보내지 않는다. 
	// ##### 서버로 패킷 보내는거로 수정!

	m_cPlayer[ 0 ].m_bDontSelectCard = true;
	// 현재 자기 턴 구분
	m_pCuttingStageInfo->bChangeStage = false;  // 커팅 유무 ( 커팅했다 )
}


void GameProcess_BD::ClearCuttingBackUpCard()
{
	memset( m_nBackUpPosIndex, 0, sizeof( m_nBackUpPosIndex ));
	memset( m_nBackUpCardIndex, 0, sizeof( m_nBackUpCardIndex ));
	m_nBackUpChangedNum = 0;
	m_bReserveChange = false;
}

// 커팅버튼 활성화및 숨기기
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

			pGameBtnMgr->SetBetBtnShowAll( false ); // 베팅 버튼 숨기고
			pGameBtnMgr->SetCutBtnShowAll( true );  // 커팅 버튼 보인다

			int cnt = m_cPlayer[ 0 ].IsCutCardSelectedNum(); //바꿀 카드를 미리 몇장이나 선택해 뒀는지

			if ( cnt > 0 ) 	//한장 이상이다.
			{					
				SetChangeState( true );
				SetPassState( false);

				pGameBtnMgr->SetEnableBtn( CGameButtonBD::BTNP_CHANGE, true );
				pGameBtnMgr->SetEnableBtn( CGameButtonBD::BTNP_PASS,   false );
			}
			else //미리 선택 해두지 않음
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
	//자동 소팅설정(true반환)
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


// 카드컷팅시 위치 정렬
void GameProcess_BD::AutoPos_Recommand()
{
	return;

	// 사용않함
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

		
		
		//자동소팅이 설정되어 있으면 소팅한다 : 2012.09.27 YS.Jeong 
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

// 카드 숫자 정렬, 버튼을 누를 때 실행
void GameProcess_BD::AutoSort_Recommand( int nPNum, BOOL bClicked )
{
	std::vector< int > vtCard;


	//소팅을 연산을 위해 vtCard에 값을 반영
	for ( int i = 0; i < TOTAL_CARD_BD; i ++ )
	{
		vtCard.push_back( m_cPlayer[ nPNum ].GetCardNo( i ) );

		//커팅로직보다 먼저 실행하므로 데이터를 저장한다.
		m_aryCardData[ i ].bSelectFlag = m_cPlayer[ nPNum ].m_cMyCard[ i ].IsSelected();
		m_aryCardData[ i ].nCardIndex = m_cPlayer[ nPNum ].GetCardNo( i );
	}

	
	//자동소팅이 설정되어 있으면 소팅한다:2012.09.27 YS.Jeong
	if (g_Config.nAutoSortType == TRUE)
	{
		std::sort( vtCard.begin(), vtCard.end(), SORT_CARD );
		
		//사용자가 선택하고 있는 데이터를 저장하고 vtCard와 동기화 한다.
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
	
		
	
	//화면에 그리고 있는 데이터에 반영
	for ( int i = 0; i < TOTAL_CARD_BD; i ++ )
	{
		m_cPlayer[ nPNum ].SetCardNo( i, vtCard[i] );

		//사용자가 선택하고 있는 데이터를 저장하고 vtCard와 동기화 한다.
		m_cPlayer[ nPNum ].m_cMyCard[i].SetSelected( m_aryCardData[i].bSelectFlag );			
	}


	//버튼을 눌렀을 때만 커팅함수를 실행
	if ( bClicked == TRUE )
	{
		//자동정렬 버튼을 누를때 커팅 효과를 다시 실행하여 카드위치를 동기화 한다.
		AutoUp_Recommand( TRUE );

	}
	
	
}

// 카드 자동올림
void GameProcess_BD::AutoUp_Recommand( BOOL bClicked ) 
{
	//게임 중 소팅버튼을 눌러서 커팅을 다시 할 때는 정렬과 초기화를 하지 않는다.
	if ( bClicked == FALSE )
	{
		// 카드 정렬
		AutoSort_Recommand();

		//데이터까지 초기화
		m_cPlayer [ 0 ].ReSetCardPostion();
	}else{
		//위치만 초기화
		m_cPlayer [ 0 ].ReSetCardPostionOnly();
	}
	

	// g_Config.nAutoUpType 초기값 2 가 사용되어 체크변수로 사용하지 않는다.
	NMBASE::SKINGDI::CGraphButton* pAutoUpBtn =
		static_cast< NMBASE::SKINGDI::CGraphButton* >( g_pGameView->m_cGameViewBtnMan.GetGraphicBtn( CGameButtonBD::BTN_AUTOUP ) );

	//아래에서 g_Config.nAutoUpType값으로 AI반영여부를 분기한다:2012.10.30 YS.Jeong
	//소팅버튼이 추가되어 게임 중 선택된 카드의 위치를 저장하였다가 UP반영하여야 한다.
	/*
	// 변수없이 스프라이트 Index로 사용유무 체크한다.
	if ( pAutoUpBtn && pAutoUpBtn->GetButtonNumber() == 1 ) // 버튼 인덱스  
	{
		// 버튼 보여지는 이미지 =>  0 : 카드자동선택ON( 켜진상태 ), 1 : 카드자동선택OFF( 꺼진상태 )
		return;
	}
	*/

	std::vector< int > vtCard;
	//자동 소팅하기 전 카드의 순서를 저장한다(자동소팅이 OFF인 경우 이 값을 사용):2012.10.26 YS.Jeong 
	std::vector< int > vtCardTmp;

	for ( int i = 0; i < TOTAL_CARD_BD; i ++ )
	{
		vtCard.push_back( m_cPlayer[ 0 ].GetCardNo( i ) );
		vtCardTmp.push_back( m_cPlayer[ 0 ].GetCardNo( i ) );
	}


	//커팅을 위해 무조건 소팅한다.(검사 로직이 현재와 다음 카드만을 검사한다.)
	std::sort( vtCard.begin(), vtCard.end(), SORT_CARD );
	
	

	//커팅 검사 로직
	int  arrnUpValue[ TOTAL_CARD_BD ] = { 0,};  // 카드 인덱스
	bool arrbUpFlag[ TOTAL_CARD_BD ] = { 0,};  // flag

	int nUpCnt = 0;

	for ( int i = 0; i < TOTAL_CARD_BD; i++ )
	{
		if ( arrbUpFlag[ i ] )
		{
			continue;
		}


		//현재 i번째의 다음 카드와 비교한다.
		for ( int j = i + 1; j < TOTAL_CARD_BD; j++ )
		{
			if ( arrbUpFlag[ j ] )
			{
				continue;
			}

			// 숫자가 같다. 
			if ( vtCard[ i ] % 13 == vtCard[ j ] % 13 )
			{
				int nf1 = 0;
				int nf2 = 0;

				// 무늬 카운트
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

				// 무늬 카운트
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

				// 같은 무늬가 많은걸 올린다.
				if ( nf1 > nf2 )
				{
					arrbUpFlag[ i ] = true;
					arrnUpValue[ nUpCnt++ ] = vtCard[ i ];
					i = j; // 다음위치
				}
				else
				{
					arrbUpFlag[ j ] = true;
					arrnUpValue[ nUpCnt++ ] = vtCard[ j ];
				}
			}
			// 무늬가 같다면
			else if ( vtCard[ i ] / 13 == vtCard[ j ] / 13 ) 
			{
				arrbUpFlag[ j ] = true;
				arrnUpValue[ nUpCnt++ ] = vtCard[ j ];
			}
		}
	}
	

	//화면을 그리는 데이터에 연산 후의 값을 반영
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

						// 선택 되어 있지 않은카드 선택 상태로 전환:2012.10.30 YS.Jeong
						if ( !bSelect && bClicked == FALSE && g_Config.nAutoUpType == 1 ) 
						{
							m_cPlayer[ 0 ].m_cMyCard[ j ].SetSelected( true );			

							int gab = CARD_GAB_Y_SELECT;
							//선택 한걸 올림
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


	//자동정렬 상태에서 원래의 카드 형태로 복귀한다 : 2012.09.27 YS.Jeong 
	//또는 이전에 선택된 카드 정보를 참고하여 올려준다.
	for ( int i = 0; i < TOTAL_CARD_BD; i ++ )
	{
		m_cPlayer[ 0 ].SetCardNo( i , vtCardTmp[ i ]);

		if ( m_cPlayer[ 0 ].m_cMyCard[ i ].IsSelected() == true ) 
		{
			int gab = CARD_GAB_Y_SELECT;
			//선택 한걸 올림
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
	// 현재 게임 참여중
	if ( m_cPlayer[ nPNum ].JoinState != 1 || m_cPlayer[ nPNum ].PlayState != 1 )
	{
		return;
	}

	m_cPlayer[ nPNum ].bFold = true;
	m_cPlayer[ nPNum ].PlayState = 0;

	if ( nPNum == 0 )
	{
		m_pCuttingStageInfo->Clear();// 컷 상태에서 포기시 버튼 초기화

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

	//현재 겜중인 넘 나갔다.
	m_nStartUserNum--;
	if ( m_nStartUserNum < 0 )
	{
		m_nStartUserNum = 0;			
	}

	// 카드 정렬
	m_cPlayer[ nPNum ].ReSetCardPostion();
	m_cPlayer[ nPNum ].GetCutInfo().Clear();
	m_cPlayer[ nPNum ].GetMyLowBDRule()->Clear();
	m_cPlayer[ nPNum ].GetEffectObjectList()->Destroy();

	// 어둡게
	if ( m_cPlayer[ nPNum ].m_nCardTotal > 0 )
	{
		for ( int  i = 0; i < m_cPlayer[ nPNum ].m_nCardTotal; i++ ) 
		{
			m_cPlayer[ nPNum ].SetFaceDown( i );							
		}
	}

	SoundFxKind( 1, m_cPlayer[ nPNum ].UI.Sex );// ### [사운드 추가 작업] ### 다이

	char szMsg[ MAX_PATH ] = { 0,};
	CString str;
	sprintf_s( szMsg, g_StrMan.Get(_T("FOLD_USER")), m_cPlayer[ nPNum ].UI.NickName );
	AddGameInfoViewEdit( szMsg, GetChatColor(ECC2_NOTIFY) );

	if ( g_Poker.nState == RSTATE_PREPARESTART )
	{
		//카드 선택시 시간 초과로 다이 했을때 이미지를 따로 찍어준다.
		effect::CBetCmdEffect *pObj = NULL;
		SAFE_NEW( pObj, effect::CBetCmdEffect );
		pObj->Init( nPNum, ANF_BET_CMD, ANF_BET_DIE );
		g_pGameView->m_cAniSprListManager.ADDObject( pObj );
	}
}


//바꾸기 알림 아침 점심 저녁
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

	// 전광판 출력
	g_pGameView->m_cDisplayBoard.SetText( ENUM_DISPLAY_BOARD_TEXT( (int) EDBT_MORNING_CUTTING + nChangeType - 1 ) ); // 아침, 점심, 저녁


	if ( nChangeType < 0 || nChangeType > 3 )
	{
		//게임 종료 호출
		OnBtnFold();
		return;
	}	

	if ( g_RI.nChangeType > nChangeType )
	{
		//게임 종료 호출
		OnBtnPass();
		return;
	}

	g_RI.nChangeType = nChangeType;

	for ( int i = 0 ; i < MAX_PLAYER ; i ++ )
	{
		m_cPlayer[ i ].GetEffectObjectList()->DelObject( ANF_CUTINFO_NUM );
	}


	//배팅 커멘드를 딜레이 후 지운다.
	g_pGameView->m_cAniSprListManager.BetCmd_DelDelay( 5 );

	// 베팅머니 표시되는 부분을 초기화 한다.
	// 2012. 04. 25 최종학
	for (int i = 0; i < GetMaxPlayer(); ++i)
	{
		g_pGameView->m_cPlayerDrawManager.ClearBettingMoney(i);
	}

	SetEnableCuttingBtn( false );
	g_pGameView->m_cGameViewBtnMan.SetBettinBtnDiable(true);

	ClearCuttingBackUpCard(); //< CYS 100705 >

	//다음 사람 
	int nServerChangerPNum = *pMsg->NextChangerPNum;

	if( nServerChangerPNum > -1 && !g_ObserverMan.IsMyObserver() )
	{
		int ClientChangerPNum = GetPNum_ByServPN(nServerChangerPNum);

		if ( m_cPlayer[ 0 ].PNum == ClientChangerPNum ) // 나
		{
			m_pCuttingStageInfo->SetCuttingTimeFlag();				 // 커팅타임 활성화
			m_pCuttingStageInfo->bMyTurn = true;
			SetEnableCuttingBtn( true );
		}
		else
		{
			// 현재턴이 내가 아니어도 내가 참여중이면 활성화 시키다. 현재 겜참여 안하는 유저는 버튼 활성화 제외
			if ( m_cPlayer[ 0 ].JoinState == 1 && m_cPlayer[ 0 ].PlayState == 1 )
			{
				// 카드 커팅시 같이 진행한다.
				m_pCuttingStageInfo->SetCuttingTimeFlag();
				SetEnableCuttingBtn( true );
			}
		}

		// 타이머  활성		
		g_pGameView->m_cPlayerDrawManager.ClearTimer();
		g_pGameView->m_cPlayerDrawManager.SetStartTimer();

		if ( !m_cPlayer[ ClientChangerPNum ].bFold &&  m_cPlayer[ ClientChangerPNum ].PlayState == 1 )
		{
			// 카드 컷팅
			// 타이머  활성		
		}
	}

	//보스 턴을 바꾸기 순서대로  넣어 준다.
	g_Poker.nState = RSTATE_GAME_ROUND_CHANGECARD_M + ( g_RI.nChangeType - 1);
	m_nCurPlayer = nServerChangerPNum;

	effect::CCuttingStageMsgEffect *pStateInfo = NULL;
	SAFE_NEW( pStateInfo, effect::CCuttingStageMsgEffect );
	pStateInfo->Init( g_RI.nChangeType );
	g_pGameView->m_cAniSprListManager.ADDObject( pStateInfo );


	//2009.01,07 히든 후 족보 표기 오류 관련 해서 초기화 해줌
	for( int i = 0 ; i < MAX_PLAYER ; i ++ )
	{
		ZeroMemory(m_nPosIndex[i],sizeof(m_nPosIndex[i]));
		ZeroMemory(m_nCardIndex[i],sizeof(m_nCardIndex[i]));
		m_nChangedNum[i] = 0;
	}

	// 아침, 점심, 저녁 입니다 사운드 현재 딜러의 성별 ==> 호출 예약
	int nPNum = GetPNum_ByServPN(nServerChangerPNum);
	m_bSex = m_cPlayer[ nPNum ].UI.Sex > 0 ? true : false;
	m_bCutInfoDelaySound = true;
	m_nCutInfoDelaySound = 20;

	int pn = GetPNum_ByServPN( m_nCurPlayer );
	// 규제안 추가 카드세팅으로 딜링 일때는 턴 표시 안함.
	if ( CheckRoundForCardSet() )
	{
		g_pGameView->m_cPlayerDrawManager.SetTurn( pn );
	}

	//규제안 : 예약기능 초기화
	SetReserveButton(RESERVESTATE_BET_NONE);
}

bool GameProcess_BD::ReceiveUserCutting_Pass( int nPNum, int nNextTurnNum )
{
	int nPassBeforDelay = ( int )g_cUIData.GetFloatBD( ui::EFLOAT_CUT_PASS_DELAY );	 // 커팅 패스 이동 전 대기시간

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
	data.m_sFirstSound.nCuttingNum = 0; // 패스
	data.m_nFirstCmd = EVENT_BD_CUTTING_PASS;

	CNMovingCard *pMoveCard = NULL;
	SAFE_NEW( pMoveCard, CNMovingCard );

	pMoveCard->Initialize( &data, 1, nPassBeforDelay, DIRECT_MOVE_PASS , false, ( PFN_CARDEVENT ) CardEventCB ); // 사운드 이벤트
	pMoveCard->nPNum = nPNum;
	pMoveCard->SetLastCMD( EVENT_BD_CUTTING_ALL_PASS );
	pMoveCard->SetLastCMD_User( nPNum );
	m_pMovingCardMgr->ADDCard( pMoveCard, true ); // 내카드이동이 여러개일경우 마지막에 커맨드이벤트 등록 true

	if ( nPNum == 0 )
	{
		g_pGameView->m_cPlayerDrawManager.SetFlyWindCard( true ); // 이벤트등록이 됬기때문에 이동중으로 셋팅한다.
	}

	//다음 사람 
	if ( nNextTurnNum > -1 && !g_ObserverMan.IsMyObserver() )
	{
		int ClientChangerPNum = GetPNum_ByServPN( nNextTurnNum );

		if ( m_cPlayer[ 0 ].PNum == ClientChangerPNum )
		{
			// 미리 커팅한 유저 자동진행
			if ( m_bReserveChange )
			{
				// 미리 눌렀다
				m_nCurPlayer = nNextTurnNum;

				if ( OnPrePress() ) // 카드 이벤트와 싱크 안맞음. 카드 이벤트 완료후 호출
				{
					g_pGameView->m_cPlayerDrawManager.ClearTimer();
					return true;
				}
			}

			m_pCuttingStageInfo->bMyTurn = true;  // 실제 내턴
		}

		// 타이머  활성		
		g_pGameView->m_cPlayerDrawManager.ClearTimer();
		g_pGameView->m_cPlayerDrawManager.SetStartTimer();

		//패스 일때 다음 사람 턴임을 알려준다.
		m_nCurPlayer = nNextTurnNum;
	}
	else
	{
		return false; // 절대 않됨!
	}

	return true;
}

bool GameProcess_BD::ReceiveUserCutting_Change( void* pValue, int nPNum, int nNextTurnNum )
{
	CSV_BD_GIVETOUSER_NEWCHANGECARD* pMsg 
		= static_cast< CSV_BD_GIVETOUSER_NEWCHANGECARD* > ( pValue );
	SetCardExchangeLog(*pMsg);
	m_nChangedNum[ nPNum ] = *pMsg->ChangedNum; // 카드 장수
	TPASSERT( m_nChangedNum[ nPNum ] >= 0 && m_nChangedNum[ nPNum ] <= TOTAL_CARD_BD );

	if ( g_Poker.nState == RSTATE_GAME_ROUND_CHANGECARD_N )
	{
		if ( nPNum == 0 && !g_ObserverMan.IsMyObserver() )
		{
			if ( m_nChangedNum[ nPNum ] > 0 )
			{
				//히든 카드 보유 셋팅
				m_bHaveHiddenCard = true;
			}
		}
	}

	///////////////////////////////////////////////////////////////

	for ( int nIndex = 0; nIndex < m_nChangedNum[ nPNum ]; nIndex++ )
	{
		m_nPosIndex[ nPNum ][ nIndex ]  = pMsg->PosIndex[ nIndex ];
		m_nCardIndex[ nPNum ][ nIndex ] = pMsg->CardIndex[ nIndex ];

		//< CYS 101201  카드 정렬
		if ( nPNum == 0 && !g_ObserverMan.IsMyObserver() )
		{
			// 카드 정렬로 인해 인덱스가 다르다.
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

	int nCardGabDelay = ( int )g_cUIData.GetFloatBD( ui::EFLOAT_CUT_CARD_DELAY );	 // 첫번째 카드와 두번째 카드 
	int nTotalDelay   = ( int )g_cUIData.GetFloatBD( ui::EFLOAT_CUT_TOTAL_DELAY ); // 카드 이동 전 총 대기시간

	//사업팀 요청에 의해서 5에서 6으로 변경. 
	//2012.7.10 류승민
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
			data[j].m_nDelay =  ( j * nCardGabDelay );	// 첫번째 카드와 두번째 카드 딜레이
			data[j].m_ptStartPos_2nd = start_pos; // move down
			data[j].m_ptDestPos_2nd = ptNewDest_pos;   // 새로운 자리 좌표
			data[j].m_nFirstCmd     = EVENT_BD_CUTTING_MOVE_UP;
			data[j].m_nSecondCmd    = EVENT_BD_CUTTING_MOVE_DOWN;

#ifdef UPGRADE_20120502_TIMING_TEST_CODE
			//#ifdef _DEBUG
			data[j].m_fSpeed = GetPrivateProfileInt("LowBD", "CutCardSpeedFrameNum", 7, ".\\TimingTest.ini");
			//#endif //_DEBUG
#endif //UPGRADE_20120502_TIMING_TEST_CODE

			if ( j == 0 ) 
			{
				//첫번째 장
				data[ j ].m_sFirstSound.bSoundUse = true;
				data[ j ].m_sFirstSound.nCuttingNum = m_nChangedNum[ nPNum ];
			}

			if ( nPNum == 0 && !g_ObserverMan.IsMyObserver() )
			{
				if ( m_bHaveHiddenCard )
				{
					// 히든카드
					m_cPlayer[ nPNum ].m_cMyCard[ nCardIndex ].SetHiddenCard( true );
				}
			}			
		}
	}

	CNMovingCard *pMoveCard = NULL;
	SAFE_NEW( pMoveCard, CNMovingCard );

	pMoveCard->Initialize( data, m_nChangedNum[ nPNum ], nTotalDelay, DIRECT_MOVE_UP, false, ( PFN_CARDEVENT ) CardEventCB );// 컷팅카드는 애니사용 않함
	pMoveCard->nPNum = nPNum;
	pMoveCard->SetLastCMD( EVENT_BD_CUTTING_ALL_DOWN );
	pMoveCard->SetLastCMD_User( nPNum );
	m_pMovingCardMgr->ADDCard( pMoveCard, true ); // 내카드이동이 여러개일경우 마지막에 커맨드이벤트 등록 true

	if ( nPNum == 0 )
	{
		g_pGameView->m_cPlayerDrawManager.SetFlyWindCard( true ); // 이벤트등록이 됬기때문에 이동중으로 셋팅한다.
	}

	//다음 사람 
	if ( nNextTurnNum > -1 && !g_ObserverMan.IsMyObserver() )
	{
		int ClientChangerPNum = GetPNum_ByServPN( nNextTurnNum );		
		if ( m_cPlayer[ 0 ].PNum == ClientChangerPNum )
		{
			// 미리 커팅한 유저 자동진행
			if ( m_bReserveChange ) // 미리 눌렀다
			{
				m_nCurPlayer = nNextTurnNum;
				if ( OnPrePress() ) 
				{
					g_pGameView->m_cPlayerDrawManager.ClearTimer();
					return true;
				}
			}

			m_pCuttingStageInfo->bMyTurn = true;  // 실제 내턴
		}

		// 타이머  활성	
		g_pGameView->m_cPlayerDrawManager.ClearTimer();
		g_pGameView->m_cPlayerDrawManager.SetStartTimer();
	}
	else
	{
		return false;
	}

	//보스 턴을 바꾸기 순서대로  넣어 준다.
	m_nCurPlayer = nNextTurnNum;

	return true;
}

// 카드 컷팅 Client 요청결과 컷팅애니메이션과 다음 컷팅 유저 인덱스.
void GameProcess_BD::OnReceiveUserCutting( void* pValue )
{	
	CSV_BD_GIVETOUSER_NEWCHANGECARD* pMsg = static_cast< CSV_BD_GIVETOUSER_NEWCHANGECARD* > ( pValue );
	SetCardExchangeLog(*pMsg);
	int nChangedNum = *pMsg->ChangedNum;

	if ( nChangedNum == 100 )
	{
		//에러 메세지 
		return;
	}

	int nSPNum = *pMsg->PNum; //  현재 대상자
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
			//컷팅한 장수를 표시한다 라운드 별로 저장
			m_cPlayer[ nPNum ].GetCutInfo().m_arrCutCount[ g_RI.nChangeType - 1 ] = nChangedNum;
		}
	}

	if ( nChangedNum <= 0 )
	{	
		// 패스
		if ( !ReceiveUserCutting_Pass( nPNum, *pMsg->NextChangerPNum ) )
		{
		}
	}
	else
	{
		// 체인지
		if ( !ReceiveUserCutting_Change( pValue, nPNum, *pMsg->NextChangerPNum ) )
		{
		}
	}

	int pn = GetPNum_ByServPN( m_nCurPlayer );
	// 규제안 추가 카드세팅으로 딜링 일때는 턴 표시 안함.
	if ( CheckRoundForCardSet() )
	{
		g_pGameView->m_cPlayerDrawManager.SetTurn( pn );
	}	
}

// 컷팅 장수 이펙트 그리기
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

// 블라인드베팅 이펙트 그리기
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
	// 게임 타입
	IDX_GAME eGameType = (IDX_GAME)g_RI.nGameIndex;

	// 자신이 입장한 방의 최소 금액
	INT64 i64RoomMoney = g_RI.llEnterLimitMoney;

	if(i64RoomMoney <= 0)	//자유방 제외
		return;

	// 테스트 코드
	//	ROOMINFO sRoomInfo;
	//	memcpy(&sRoomInfo, &g_RI, sizeof(ROOMINFO));

	//뉴포커, 로우, 7포커
	ASSERT (eGameType > IDX_GAME_NULL);
	const int INDEX_ARRAY_JACKPOT_GAME_LOW = 1; //바둑이 인덱스 1

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
		// 이전 잭팟 금액
		SetOldJackPotMoney(GetJackPotMoney());

		// 새로운 잭팟 금액
		SetJackPotMoney(i64JackPotMoney);
	}
}


void GameProcess_BD::ClearJackpotMoney(int nGameCode, INT64 i64LimitMoney)
{
	// 게임 타입
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
			// 이곳에 들어가면 초기화 할필요가 없으므로 리턴한다.
			return;
		}
	}

	if(i64LimitMoney <= 0)
		return;


	//1. 게임타입 검사
	//2. 입장한 방 머니 검사   
	//3. 두가지 값이 동일하면 현재 방의 잭팟 머니를 초기화 한다.
	int nGameIndexTemp		= g_RI.nGameIndex;
	INT64 i64LimitMoneyTemp = g_RI.MoneyRange.GetDispMinMoney();

	if(eGameType == nGameIndexTemp && i64LimitMoney == i64LimitMoneyTemp)
	{
		SetOldJackPotMoney(0);
		SetJackPotMoney(0);

		// 전역변수의 해당 머니 구간의 값도 0으로 초기화 한다.
		// 현재 방에 대당하는 구간의 잭팟금액을 초기화 한다.
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



//규제안 예액기능 추가로 인해 GameProcess에 추가된 가상 함수
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
	// 버그로그 add logData
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