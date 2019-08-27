#include "stdafx.h"
#include "GlobalGame.h"
#include "PlayScr.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define DOTPERSEC 2400.0

template < class ForwardIterator >
void DeleteAll( ForwardIterator first, ForwardIterator last )
{
	while ( first != last )
	{
		if ( (*first) != NULL )
		{
			delete (*first++);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//	CardInfo Class
//////////////////////////////////////////////////////////////////////////
CCardInfo::CCardInfo()
{
	Reset();
}
CCardInfo::~CCardInfo()
{
	Reset();
}
void CCardInfo::Reset()
{
	m_dqStraight.clear();
	m_dqTogether.clear();
	m_dqSelect.clear();
	m_dqAttachStraight.clear();
	m_dqAttachTogether.clear();
	m_dqSeven.clear();
	m_dqUpCard.clear();
}

//////////////////////////////////////////////////////////////////////////
//	PlayScr Class
//////////////////////////////////////////////////////////////////////////
CPlayScr::CPlayScr()
{
	m_nDiscardTest = -1;

	Reset();
	m_pParent = NULL;
	m_pPage = NULL;
	m_bGoldState = FALSE;
	m_nGoldStage = 1;
	m_bEndGoldStage = FALSE;
	m_bJackpotData = FALSE;
	m_bPlayMusicAfter = FALSE;
	m_bJackpotConditionSucceed = FALSE;
	m_nJackpotConditionStep = 0;
	m_dwJackpotConditionTime = 0;
	m_bJackpotMouseOver = FALSE;
	m_dwJackpotMouseTime = 0;
	m_bJackpotMouseOverOff = FALSE;
	m_pGameButtonHA = NULL;
	m_bResultDelay = FALSE;
	m_eCardSelectType = eCARDSELECT_NONE;

	ZeroMemory( &m_SameNumCard, sizeof(int) * MAX_CARD_FACE * MAX_CARD_VALUE );
	ZeroMemory( &m_CenterCard, sizeof(int) * MAX_CARD_FACE * MAX_CARD_VALUE );
}

CPlayScr::~CPlayScr()
{
}

void CPlayScr::Init(CWnd *pWnd, NMBASE::GRAPHICGDI::CPage *ppage, int pnum, CGameButtonHA *pGameButtonHA)
{
	int xy[MAX_PLAYER][2] = {454, 548, 17, 426,  17, 236, 860, 236, 860, 426 };

	m_pParent = pWnd;
	m_pPage = ppage;
	memcpy( CardXY, xy, sizeof( int ) * MAX_PLAYER * 2 );	
	m_pGameButtonHA = pGameButtonHA;
	m_dqSameCardPosIndex.clear();

	BYTEDQ dqIndex;
	dqIndex.push_back( 1 );
	m_dqSameCardPosIndex.push_back( dqIndex );
	dqIndex.clear();
	dqIndex.push_back( 1 );
	dqIndex.push_back( 7 );
	m_dqSameCardPosIndex.push_back( dqIndex );
	dqIndex.clear();
	dqIndex.push_back( 1 );
	dqIndex.push_back( 7 );
	dqIndex.push_back( 13 );
	m_dqSameCardPosIndex.push_back( dqIndex );
	dqIndex.clear();
	dqIndex.push_back( 1 );
	dqIndex.push_back( 5 );
	dqIndex.push_back( 9 );
	dqIndex.push_back( 13 );
	m_dqSameCardPosIndex.push_back( dqIndex );
	dqIndex.clear();
	dqIndex.push_back( 1 );
	dqIndex.push_back( 4 );
	dqIndex.push_back( 7 );
	dqIndex.push_back( 10 );
	dqIndex.push_back( 13 );
	m_dqSameCardPosIndex.push_back( dqIndex );
	dqIndex.clear();
	dqIndex.push_back( 1 );
	dqIndex.push_back( 3 );
	dqIndex.push_back( 5 );
	dqIndex.push_back( 7 );
	dqIndex.push_back( 9 );
	dqIndex.push_back( 11 );
	m_dqSameCardPosIndex.push_back( dqIndex );
	dqIndex.clear();
	dqIndex.push_back( 1 );
	dqIndex.push_back( 3 );
	dqIndex.push_back( 5 );
	dqIndex.push_back( 7 );
	dqIndex.push_back( 9 );
	dqIndex.push_back( 11 );
	dqIndex.push_back( 13 );
	m_dqSameCardPosIndex.push_back( dqIndex );
	dqIndex.clear();
}

void CPlayScr::Reset()
{
	m_eSortBtnType = eSORTBTN_STRAIGHT;

	m_bGameOver = FALSE;

	//플레이어 턴 카운트가 세팅이 되지 않아 첫번째 턴에 4번인덱스에서 버리기 표시가 되는 문제가 있음  Accept_EnterRoom이 실행될때 해당 값 세팅이 필요함
	m_nTurnCnt = -1;		
	GameResultFlag = false;
	m_nTurnIndex = 0;
	m_nDisCardCnt = 0;
	m_bShowGameResult = FALSE;
	m_nUpCardType = 0;
	m_bWind = FALSE;
	m_bJackpot = FALSE;
	m_dwJackpot = 0;
	m_bMissionStart = FALSE;
	m_bMission = FALSE;
	m_nMissionNum = 0;
	m_dwMission = 0;
	m_bMissionComp = FALSE;
	m_dwMissionComp = 0;
	m_sMissionCompID = "";
	m_bStartFrame = FALSE;
	m_nStartFrameStep = 0;
	m_dwStartFrame = 0;
	m_bPressThankBtn = FALSE;
	m_dwThankBtnTime = 0;
	//조커 카드 배열
	m_JokerCard[0] = 0;
	m_JokerCard[1] = 0;
	std::deque< MOVECARD * >::iterator DelIter;
	DelIter = m_dqMoveCard.begin();
	DeleteAll( m_dqMoveCard.begin(), m_dqMoveCard.end() );
	m_dqMoveCard.clear();
	m_bLClick = FALSE;
	m_bDrag = FALSE;
	m_dwHelpTime = 0;
	m_nHelpIndex = -1;
	m_bSelectDraw = FALSE;
	m_bMissionComp = FALSE;
	m_dwMissionComp = 0;
	m_bHoolaSound = FALSE;
	m_bJackpotSound = FALSE;
	m_bHoolaWin = FALSE;
	m_bPracticsGame = FALSE;
	m_bResult = FALSE;
	m_bResultDelay = FALSE;
	m_dwResultDelay = 0;
	m_bResult = FALSE;
	m_eCardSelectType = eCARDSELECT_NONE;

	// 훌라 개선작업
	ResetTurnFlag();
	ResetLimitTime();
	//선고르기
	m_cSunPickup.Reset();

	ResetMoveCard();

	if ( g_RI.FormKind == hoola::eFORM_KIND_JOKER ) m_nCurStockCnt = 54;
	else m_nCurStockCnt = 52;
	
	// 훌라 개선 작업
	memset( m_PlayerCard, -1, sizeof( m_PlayerCard ));
	memset( m_CenterCard,  0, sizeof( int ) * MAX_CARD_FACE * MAX_CARD_VALUE );		   // 중앙에 등록 카드및 사이드카드
	memset( m_SameNumCard, 0, sizeof( int ) * MAX_CARD_FACE * MAX_CARD_VALUE );	       // 같은 숫자 카드
	ZeroMemory(&m_GameResult, sizeof(GAMEOVERRESULT));

}
void CPlayScr::CheckJokerCardProblem()
{
	int nJokerCardCount = 0;

	for ( int i = 0 ; i < MAX_CARD_FACE ; i++ )
	{
		for ( int j = 0 ; j < MAX_CARD_VALUE ; j++ )
		{
			if (m_CenterCard[i][j] > 1)
			{
				++nJokerCardCount;
			}
		}
	}
	for ( int i = 0 ; i < MAX_CARD_VALUE ; i++ )
	{
		for ( int j = 0 ; j < MAX_CARD_FACE ; j++ )
		{
			if (m_SameNumCard[i][j] > 1)
			{
				++nJokerCardCount;
			}
		}
	}
	for ( int i = 0 ; i < MAX_CARDCOUNT ; i++ )
	{
		if (m_PlayerCard[0][i] > 51)
		{
			++nJokerCardCount;
		}
	}
#ifdef _DEBUG
	if ( nJokerCardCount > 2 )
	{
		CString str;
		str.Format("버그)ID:%s 문제:조커가 두장 이상\n",GM().GetMyInfo()->UI.ID);
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
	}
#endif

}

void CPlayScr::OnTimer()
{
	if ( m_bResultDelay )
	{
		//게임 종료시 게임 결과 이펙트 딜레시 세팅
		if ( (timeGetTime() - m_dwResultDelay) > g_sprEffect.GetMScenePlayTime( 1 ) )
		{
			m_bResultDelay = FALSE;
			m_dwResultDelay = 0;
			g_GAMEPROCESS()->OnGameOver(&m_GameResult);
		}
	}
}

//연습게임 진행중이고 관전중인 사용자일 경우를 확인
BOOL CPlayScr::IsMyPracticeGameObserver()
{
	BOOL bPractice = FALSE;	
	for ( int i = 0; i < MAX_PLAYER; i++ )
	{
		if ( strcmp( g_PLAYER(i)->UI.ID, "_PRACTICSUSER_" ) == 0 )
		{	
			bPractice = TRUE;
			break;
		}
	}

	if ( FALSE == bPractice )
		return FALSE;

	if (g_ObserverMan.IsMyObserver() )
		return TRUE;
	else if ( g_PLAYER(0)->PlayState == EPLAYER_GAMEEND )
		return TRUE;

	return FALSE;
}

//숫자키로 카드를 선택했을 때의 프로세스
void CPlayScr::KeyDownCardShortcut(int nVKey)
{
	// 카드 애니메이션이 진행중이거나 마우스 드래그로 카드를 옮기고 있을 경우
	if ( m_bWind || m_bDrag ) return;

	int nIndex = nVKey - rule::SKEY_1;
	char cCardIndex = m_PlayerCard[0][nIndex];

	if ( g_RI.FormKind == hoola::eFORM_KIND_JOKER )
	{
		if ( cCardIndex < 0 || cCardIndex > CARD_JOKER2 )
			return;
	}
	else
	{
		//조커 미 포함 유효한 카드가 아니면 반환
		if ( cCardIndex < 0 || cCardIndex > 51 ) 			
			return;
	}

	if ( m_bThankSelect && m_bPressThank || m_bThankStep2 )
	{
		BYTEDQ::iterator Iter;
		Iter = std::find( m_dqSelectedCard.begin(), m_dqSelectedCard.end(), cCardIndex );
		BOOL bExist = FALSE;
		if ( cCardIndex == CARD_JOKER1 )
		{
			for ( int i = 0; i < m_dqSelectedCard.size(); i++ )
			{
				if ( m_dqSelectedCard[i] >= 60 && m_dqSelectedCard[i] < 120 )
				{
					bExist = TRUE;
					return;
				}
			}
		}
		else if ( cCardIndex == CARD_JOKER2 )
		{
			for ( int i = 0; i < m_dqSelectedCard.size(); i++ )
			{
				if ( m_dqSelectedCard[i] >= 120 && m_dqSelectedCard[i] < 180 )
				{
					bExist = TRUE;
					return;
				}
			}
		}

		if ( bExist || Iter != m_dqSelectedCard.end() )
		{
			if ( Iter != m_dqSelectedCard.end() )
			{
				m_dqSelectedCard.erase( Iter );
			}

			if ( m_dqSelectedCard.empty() )
			{
				m_bClickRegCard = FALSE;
				if ( m_bThankSelect )
				{
//					EnableSelect();
				}
			}
			else
			{
				BYTEDQ dqTemp = m_dqSelectedCard;
				CheckThank( dqTemp );
			}
			if ( FALSE == IsMyPracticeGameObserver() )
				NMBASE::SOUND::g_Sound.PlayWav( SND16 );
		}
		else
		{
			m_bClickRegCard = TRUE;
			m_dqSelectedCard.push_back( cCardIndex );
			BYTEDQ dqTemp = m_dqSelectedCard;
			CheckThank( dqTemp );
			ResetSelect();

			if ( FALSE == IsMyPracticeGameObserver() )
				NMBASE::SOUND::g_Sound.PlayWav( SND16 );
		}
		return;
	}
	else if ( (m_nTurnCnt == g_PLAYER(0)->ServPNum && m_nTurnIndex == 1) )
	{
		BYTEDQ::iterator Iter;
		Iter = std::find( m_dqSelectedCard.begin(), m_dqSelectedCard.end(), cCardIndex );
		if ( Iter != m_dqSelectedCard.end() )
		{
			m_dqSelectedCard.erase( Iter );

			if ( m_dqSelectedCard.empty() )
			{
				m_bClickRegCard = FALSE;

// 				if ( m_bRegSelect )
// 				{
// //					EnableSelect();
// 				}
// 				else if ( CheckReg() )
// 				{
// 					SetRegCardButton();
// 				}

				if ( CheckReg() )
				{
					SetRegCardButton();
				}
			}
			else
			{
				BYTEDQ dqTemp = m_dqSelectedCard;

				if ( m_nTurnCnt == g_PLAYER(0)->ServPNum )
				{
					if ( CheckReg( dqTemp ) )
					{
						SetRegCardButton();
					}
					else
					{
						SetDumpCardButton();
					}
				}
			}

			if ( FALSE == IsMyPracticeGameObserver() )
				NMBASE::SOUND::g_Sound.PlayWav( SND16 );
		}
		else
		{
			m_bClickRegCard = TRUE;
			m_dqSelectedCard.push_back( cCardIndex );
			BYTEDQ dqTemp = m_dqSelectedCard;

			if ( CheckReg( dqTemp ) )
			{
				SetRegCardButton();
			}
			else
			{
				SetDumpCardButton();
			}
			ResetSelect();

			if ( FALSE == IsMyPracticeGameObserver() )
				NMBASE::SOUND::g_Sound.PlayWav( SND16 );
		}
	}
}
void CPlayScr::OnGameKey( int nVKey, int nextended, int repeat )
{
	// 게임중이거나 연습모드가 아니면 리턴

	if(g_ObserverMan.IsMyObserver() == TRUE ) 
		return;// ### [ 관전기능 ] ###

	if ( nextended == 0 )
	{
		switch(nVKey)
		{
		case 79: KeyDownCardShortcut(rule::SKEY_1); return;
		case 80: KeyDownCardShortcut(rule::SKEY_2); return;
		case 81: KeyDownCardShortcut(rule::SKEY_3); return;
		case 75: KeyDownCardShortcut(rule::SKEY_4); return;
		case 76: KeyDownCardShortcut(rule::SKEY_5); return;
		case 77: KeyDownCardShortcut(rule::SKEY_6); return;
		case 71: KeyDownCardShortcut(rule::SKEY_7); return;
		case 72: KeyDownCardShortcut(rule::SKEY_8); return;
		}
	}

	switch(nVKey)
	{
	case rule::SKEY_1:
	case rule::SKEY_2:
	case rule::SKEY_3:
	case rule::SKEY_4:
	case rule::SKEY_5:
	case rule::SKEY_6:
	case rule::SKEY_7:
	case rule::SKEY_8:
		{
			KeyDownCardShortcut(nVKey);
		}
		break;
	case rule::SKEY_ENTER:
	case rule::SKEY_F1:
	case rule::SKEY_ESC:
		{
			if ( m_bShowGameResult )
			{
				ResultOkBtnClick();
			}
		}
		break;

	case rule::SKEY_GRAY_HOME:
	case rule::SKEY_S:
		{
			if ( m_bRegSelect && m_bSelectDraw && m_cCardInfo.m_dqSelect.size() == 4 && nVKey == rule::SKEY_GRAY_HOME )
			{
				m_nSelectCurrent = 1;
				SetSelectRegCard( m_nSelectCurrent );
				RegisBtnClick();
				break;
			}

			if ( m_pGameButtonHA->IsEnableBtn(CGameButtonHA::BTN_REGIST))
			{
				RegisBtnClick();
			}
		} break;
	case rule::SKEY_GRAY_INS: 
	case rule::SKEY_A:
		{
			if ( m_bRegSelect && m_bSelectDraw && m_cCardInfo.m_dqSelect.size() == 4 && nVKey == rule::SKEY_GRAY_INS )
			{
				m_nSelectCurrent = 0;
				SetSelectRegCard( m_nSelectCurrent );
				RegisBtnClick();
				break;
			}
			SortBtnClick();
		}
		break;
	case rule::SKEY_GRAY_DEL:
	case rule::SKEY_Z:
		{
			if ( m_bThankSelect && m_bSelectDraw && nVKey == rule::SKEY_GRAY_DEL )
			{
				m_nSelectCurrent = 0;
				ThankBtnClick();
				break;
			}
			if ( m_bRegSelect && m_bSelectDraw && m_cCardInfo.m_dqSelect.size() <= 3 && nVKey == rule::SKEY_GRAY_DEL )
			{
				if ( m_nSelectCurrent != 0 )
				{
					m_nSelectCurrent = 0;
					SetSelectRegCard( m_nSelectCurrent );
					RegisBtnClick();
				}
				break;
			}
			if ( m_bRegSelect && m_bSelectDraw && m_cCardInfo.m_dqSelect.size() == 4 && nVKey == rule::SKEY_GRAY_DEL )
			{
				if ( m_nSelectCurrent != 2 )
				{
					m_nSelectCurrent = 2;
					SetSelectRegCard( m_nSelectCurrent );
					RegisBtnClick();
				}
				break;
			}
			if ( m_pGameButtonHA->IsEnableBtn(CGameButtonHA::BTN_GETCARD) )
			{
				GetCardBtnClick();
			}
			else if ( m_pGameButtonHA->IsEnableBtn(CGameButtonHA::BTN_DUMPCARD) )
			{
				DumpCardBtnClick();
			}
		} break;

	case rule::SKEY_GRAY_END: 
	case rule::SKEY_X:
		{
			if ( m_bThankSelect && m_bSelectDraw && nVKey == rule::SKEY_GRAY_END )
			{
				m_nSelectCurrent = 1;
				ThankBtnClick();
				break;
			}
			if ( m_bRegSelect && m_bSelectDraw && m_cCardInfo.m_dqSelect.size() <= 3 && nVKey == rule::SKEY_GRAY_END )
			{
				m_nSelectCurrent = 1;
				SetSelectRegCard( m_nSelectCurrent );
				RegisBtnClick();
				break;
			}
			else if ( m_bRegSelect && m_bSelectDraw && m_cCardInfo.m_dqSelect.size() == 4 && nVKey == rule::SKEY_GRAY_END )
			{
				m_nSelectCurrent = 3;
				SetSelectRegCard( m_nSelectCurrent );
				RegisBtnClick();
				break;
			}
		
			if ( m_pGameButtonHA->GetGraphicBtn(CGameButtonHA::BTN_STOP)->IsEnable() )
			{
				StopBtnClick();
			}
			else if ( m_pGameButtonHA->IsEnableBtn(CGameButtonHA::BTN_PASS) )
			{
				PassBtnClick();
			}
		} break;

	case rule::SKEY_GRAY_PGDN: // VK_PAGE_DOWN:
		{
			if ( m_bRegSelect && m_bSelectDraw && m_cCardInfo.m_dqSelect.size() == 3 && nVKey == rule::SKEY_GRAY_PGDN )
			{
				m_nSelectCurrent = 2;
				SetSelectRegCard( m_nSelectCurrent );
				RegisBtnClick();
				break;
			}
		}
		break;

	case rule::SKEY_GRAY_PGUP: // VK_PAGE_UP: 
	case rule::SKEY_Q: 
		{
			if ( m_pGameButtonHA->IsEnableBtn(CGameButtonHA::BTN_THANK) )
			{
				if ( !m_bPressThankBtn )
				{
					ThankBtnClick();
				}
			}
			else if ( m_pGameButtonHA->IsShowBtn(CGameButtonHA::BTN_HIGHTHANK) )
			{
				StopCard(CCL_STOPGAME::eSTOP_TYPE_DABANG);
				SetNullButton();
			}
			else if ( m_pGameButtonHA->IsShowBtn(CGameButtonHA::BTN_LOWTHANK) )
			{
				StopCard(CCL_STOPGAME::eSTOP_TYPE_SOBANG);
				SetNullButton();
			}
			else if ( m_pGameButtonHA->IsShowBtn(CGameButtonHA::BTN_SEVENFOURTHANK) )
			{
				StopCard(CCL_STOPGAME::eSTOP_TYPE_SEVEN4);
				SetNullButton();
			}
			else if ( !m_bPressThankBtn )
			{
				AutoPlayMSceneSound( &g_sprNoThankBtn, 0 );
				m_dwThankBtnTime = timeGetTime();
				m_bPressThankBtn = TRUE;
			}
		}
		break;
	case rule::SKEY_SPACE:
		{
			if ( m_bShowGameResult )
			{
				ResultOkBtnClick();			
				break;
			}
			if ( m_pGameButtonHA->GetGraphicBtn(CGameButtonHA::BTN_GAMESTART)->IsEnable() || m_pGameButtonHA->GetGraphicBtn(CGameButtonHA::BTN_GAMESTART)->IsShow() )
			{
				ReqGameStart();
				break;
			}
		} break;
	}
}
void CPlayScr::SortMyCard( BOOL bTypeStraight )
{
	if ( bTypeStraight ) //숫자 정렬
	{
		int cnt = 0;
		memset( m_PlayerCard[0], -1, sizeof( char ) * MAX_CARDCOUNT );
		for( int i = 0; i < MAX_CARD_VALUE; i ++ )
		{
			for( int j = 0; j < MAX_CARD_FACE; j ++ )
			{
				if( m_MyCard[i][j] )
				{
					m_PlayerCard[0][cnt++] = j * MAX_CARD_VALUE + i;
				}
			}
		}

		if ( m_JokerCard[0] )
		{
			m_PlayerCard[0][cnt++] = CARD_JOKER1;
		}
		if ( m_JokerCard[1] )
		{
			m_PlayerCard[0][cnt++] = CARD_JOKER2;
		}

		g_PLAYER(0)->SetCardCount( cnt );
	}
	else //무늬 정렬
	{
		int cnt = 0;

		memset( m_PlayerCard[0], -1, sizeof( char ) * MAX_CARDCOUNT );
		for( int j = 0; j < 4; j ++ )
		{
			for( int i = 0; i < 13; i ++ )
			{
				if( m_MyCard[i][j] )
				{
					m_PlayerCard[0][cnt++] = j * 13 + i;
				}
			}
		}

		if ( m_JokerCard[0] )
		{
			m_PlayerCard[0][cnt++] = CARD_JOKER1;
		}
		if ( m_JokerCard[1] )
		{
			m_PlayerCard[0][cnt++] = CARD_JOKER2;
		}

		g_PLAYER(0)->SetCardCount( cnt );
	}
}

void CPlayScr::SortMyCard()
{
	int cnt = 0;
	
	memset( m_PlayerCard[0], -1, sizeof( char ) * 8 );
	
	for( int i = 0; i < 13; i ++ )
	{
		for( int j = 0; j < 4; j ++ )
		{
			if( m_MyCard[i][j] )
			{
				m_PlayerCard[0][cnt++] = j * 13 + i;
			}
		}
	}

	if ( m_JokerCard[0] )
	{
		m_PlayerCard[0][cnt++] = CARD_JOKER1;
	}
	if ( m_JokerCard[1] )
	{
		m_PlayerCard[0][cnt++] = CARD_JOKER2;
	}
	g_PLAYER(0)->SetCardCount( cnt );
}

void CPlayScr::SortByFace()
{
	int cnt = 0;
	
	memset( m_PlayerCard[0], -1, sizeof( char ) * MAX_CARDCOUNT );

	for( int j = 0; j < 4; j ++ )
	{
		for( int i = 0; i < 13; i ++ )
		{
			if( m_MyCard[i][j] ) 
				m_PlayerCard[0][cnt++] = j * 13 + i;
		}
	}

	if ( m_JokerCard[0] )
	{
		m_PlayerCard[0][cnt++] = CARD_JOKER1;
	}
	if ( m_JokerCard[1] )
	{
		m_PlayerCard[0][cnt++] = CARD_JOKER2;
	}

	g_PLAYER(0)->SetCardCount( cnt );
}

void CPlayScr::PassThankyou()
{
	g_GAMEPROCESS()->SendCL_ASK_CONTINUE( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID );
}

int CPlayScr::CheckStopOK( int &nCardSum )
{
	int okflag = 0;
	if( g_PLAYER(0)->GetCardCount() == 0 ) 
		return okflag;

	int CardSum = 0;
	int SevenNum = 0;
	int joker = 0;	
	int cnt, i, j;

	char PlayerCard[8];
	memset( PlayerCard, -1, sizeof( char ) * MAX_CARDCOUNT );
	cnt = 0;

	for( i = 0; i < 13; i ++ )
	{
		for( j = 0; j < 4; j ++ )
		{
			if( m_MyCard[i][j] ) 
				PlayerCard[cnt++] = j * 13 + i;
		}
	}

	for( i = 0; i < cnt; i++ )
	{
		CardSum += PlayerCard[i]%13 + 1;
		if ( PlayerCard[i] % 13 == 6 )
		{
			CardSum += 7;
			SevenNum++;
		}
	}
	if ( SevenNum == 4 )
	{
		okflag = 1;
		CardSum = 100;
		nCardSum = CardSum;
		return okflag;
	}

	if ( m_JokerCard[0] )
	{
		joker++;
	}
	if ( m_JokerCard[1] )
	{
		joker++;
	}
	
	if( (CardSum + joker * 14) >= 83 ) 
	{
		okflag = 2;
	}
	else 
	{
		if ( TRUE == g_PLAYER(0)->m_bRegist )
		{
			int nStopPoint = 21;
			int nRealUser = 0;

			for ( int i = 0; i < MAX_PLAYER; i++ )
			{
				if ( g_PLAYER(i)->PlayState == EPLAYER_GAMEPLAYING && g_PLAYER(i)->JoinState == EGAME_JOININ )
				{
					nRealUser++;
				}
			}

			if ( nRealUser == 5 )
			{
				nStopPoint = 21;
			}
			else if ( nRealUser == 4 )
			{
				nStopPoint = 15;
			}
			else if ( nRealUser == 3 )
			{
				nStopPoint = 10;
			}
			else if ( nRealUser == 2 )
			{
				nStopPoint = 5;
			}
			else
			{
				nStopPoint = 0;
			}
				
			if ( ( CardSum + 14 * joker ) <= nStopPoint )
				okflag = 4;
		}
		else
		{
			if ( ( CardSum + joker * 14 ) <= 15 )
			{
				okflag = 3;
			}
		}
	}
	
	nCardSum = CardSum;
	if ( okflag )
		return okflag;

	// 스톱룰 추가
	if ( g_RI.cRuleType == 2 ) // 스톱 방이면
		return 5;

	return okflag;
}


void CPlayScr::StopCard(char type)
{
	g_GAMEPROCESS()->SendCL_STOPGAME( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, type );
}
BOOL CPlayScr::OnMouseMove( int xp, int yp )
{
	BOOL bOldJackpotMouseOver = m_bJackpotMouseOver;
	m_bJackpotMouseOver = FALSE;

	CRect rect;

	//잭팟 마우스 오버 좌표
	rect.SetRect( 900, 522, 900 + 79, 522 + 175 );
	if ( rect.PtInRect( CPoint( xp, yp ) ) )
	{
		m_bJackpotMouseOver = TRUE;
		if ( bOldJackpotMouseOver == FALSE )
			m_dwJackpotMouseTime = timeGetTime();
	}
	else
	{
		if ( bOldJackpotMouseOver )
		{
			m_bJackpotMouseOverOff = TRUE;
			m_dwJackpotMouseTime = timeGetTime();
		}
	}

	if ( g_ObserverMan.IsMyObserver() )
	{
		return FALSE;
	}

	if ( m_bThankSelect )
	{
		rect.SetRect( 345, 241, 345+156, 241+114 );
		if ( rect.PtInRect( CPoint( xp, yp ) ) )
		{
			m_nSelectCurrent = 0;
			return TRUE;
		}
		rect.SetRect( 501, 241, 501+156, 241+114 );
		if ( rect.PtInRect( CPoint( xp, yp ) ) )
		{
			m_nSelectCurrent = 1;
			return TRUE;
		}
	}
	else if ( m_bRegSelect )
	{
		if ( m_cCardInfo.m_dqSelect.size() >= 2 )
		{
			rect.SetRect( 345, 241, 345+156, 241+114 );
			if ( rect.PtInRect( CPoint( xp, yp ) ) )
			{
				m_nSelectCurrent = 0;
				return TRUE;
			}
			rect.SetRect( 501, 241, 501+156, 241+114 );
			if ( rect.PtInRect( CPoint( xp, yp ) ) )
			{
				m_nSelectCurrent = 1;
				return TRUE;
			}
 		}
	}
	else if (m_nUpCardType == 3 && m_nTurnCnt == g_PLAYER(0)->ServPNum && m_nTurnIndex == 1 )
	{
		rect.SetRect( 345, 241, 345+156, 241+114 );
		if ( rect.PtInRect( CPoint( xp, yp ) ) )
		{
			m_nSelectCurrent = 0;
			return TRUE;
		}
		rect.SetRect( 501, 241, 501+156, 241+114 );
		if ( rect.PtInRect( CPoint( xp, yp ) ) )
		{
			m_nSelectCurrent = 1;
			return TRUE;
		}				
	}


	if ( m_bLClick )
	{
		int nX = xp - m_ptDrag.x;
		int nY = yp - m_ptDrag.y;

		if ( m_bDrag == FALSE )
		{
			if ( abs( nX ) >= 8 || abs( nY ) >= 8 )
			{
				m_bDrag = TRUE;

				m_ptDrag.x = xp;
				m_ptDrag.y = yp;
				m_ptDragCard.x += nX;
				m_ptDragCard.y += nY;
				return TRUE;
			}
		}
		else
		{
			m_ptDrag.x = xp;
			m_ptDrag.y = yp;
			m_ptDragCard.x += nX;
			m_ptDragCard.y += nY;

			return TRUE;
		}
	}
	return FALSE;
}


BOOL CPlayScr::OnLButtonUp(int xp, int yp)
{
	// ### [ 관전기능 ] ###
	if(g_ObserverMan.IsMyObserver()) 
		return FALSE;

	//선고르기
	m_cSunPickup.OnLButtonUp( xp, yp );

	if ( m_bDrag )
	{
		int nPos = -1;
		int dist1 = 20;

		std::deque< char > dqSort;
		std::deque< char >::iterator Iter;

		for ( int i = 0; i < g_PLAYER(0)->GetCardCount(); i++ )
		{
			if (  m_PlayerCard[0][i] >= 0 )
			{
				dqSort.push_back( m_PlayerCard[0][i] );
			}
		}// for ( int i = 0; i < g_PLAYER(0)->GetCardCount(); i++ )

		for ( int i = g_PLAYER(0)->GetCardCount() - 1; i >= 0; i-- )
		{
			char cCardIndex = m_PlayerCard[0][i];
			int nAddY = 0;
			if ( cCardIndex >= 0 && m_nLClickType >= 0 )
			{
				if ( (m_ptDragCard.x >= CardXY[0][0] + (i-1) * dist1) && (m_ptDragCard.x < CardXY[0][0] + (i-1) * dist1 + 74) )
				{
					if ( (m_ptDragCard.y >= CardXY[0][1] - nAddY - 30 ) && (m_ptDragCard.y < CardXY[0][1] - nAddY + 99) )
					{
						Iter = std::find( dqSort.begin(), dqSort.end(), m_nLClickType );
						if ( Iter != dqSort.end() )
							dqSort.erase( Iter );

						if ( i != 0 )
						{
							Iter = std::find( dqSort.begin(), dqSort.end(), cCardIndex );						
							dqSort.insert( Iter, m_nLClickType );
						}
						else
						{
							dqSort.push_front( m_nLClickType );
						}
						memset( m_PlayerCard[0], -1, 8 );
						for ( int j = 0; j < dqSort.size(); j++ )
						{
							m_PlayerCard[0][j] = dqSort[j];
						}
						
						break;
					}
				}
			}
		}// for ( int i = g_PLAYER(0)->GetCardCount() - 1; i >= 0; i-- )
		m_bLClick = FALSE;
		m_nLClickType = 0;
		m_bDrag = FALSE;
		return TRUE;
	}

	if ( (m_nTurnCnt == g_PLAYER(0)->ServPNum && m_nTurnIndex == 1) )
	{
		int dist1 = 20;

		for ( int i = g_PLAYER(0)->GetCardCount() - 1; i >= 0; i-- )
		{
			char cCardIndex = m_PlayerCard[0][i];

			int nAddY = 0;
			if ( IsSelected( cCardIndex ) )
			{
				nAddY = 13;
			}

			if ( (xp >= CardXY[0][0] + i * dist1) && (xp < CardXY[0][0] + i * dist1 + 74) )
			{
				if ( (yp >= CardXY[0][1] - nAddY) && (yp < CardXY[0][1] - nAddY + 99) )
				{
					//선택된 카드의 x,y 좌표
					if ( m_bLClick && m_nLClickType == cCardIndex )
					{
						//등록가능 상황 확인
						BYTEDQ::iterator Iter;
						Iter = std::find( m_dqSelectedCard.begin(), m_dqSelectedCard.end(), cCardIndex );
						if ( Iter != m_dqSelectedCard.end() )
						{
							//카드 값을 찾았을때
							m_dqSelectedCard.erase( Iter );
							if ( m_dqSelectedCard.empty() )
							{
								m_bClickRegCard = FALSE;
// 								if ( m_bRegSelect )
// 								{
// //									EnableSelect();
// 									break;
// 								}
								if ( CheckReg() )
								{
									SetRegCardButton();
								}
								else
								{
									SetDumpCardButton();
								}
							}
							else
							{
								BYTEDQ dqTemp = m_dqSelectedCard;
								if ( CheckReg( dqTemp ) )
								{
									SetRegCardButton();
								}
								else
								{
									SetDumpCardButton();
								}
							}
							if ( FALSE == IsMyPracticeGameObserver() )
								NMBASE::SOUND::g_Sound.PlayWav( SND16 );
						}
						else
						{
							m_bClickRegCard = TRUE;
							m_dqSelectedCard.push_back( cCardIndex );
							BYTEDQ dqTemp = m_dqSelectedCard;
							if ( dqTemp.empty() )
							{
								if ( CheckReg( ) )
								{
									SetRegCardButton();
								}
								else
								{
									SetDumpCardButton();
								}
								ResetSelect();

								if ( FALSE == IsMyPracticeGameObserver() )
									NMBASE::SOUND::g_Sound.PlayWav( SND16 );
							}
							else
							{
								if ( CheckReg( dqTemp ) )
								{
									SetRegCardButton();
								}
								else
								{
									SetDumpCardButton();
								}
//								ResetSelect();
								if ( FALSE == IsMyPracticeGameObserver() )
									NMBASE::SOUND::g_Sound.PlayWav( SND16 );
							}
						}
					}
					break;
				}
			}
		}
	}
	else 
	{
		if ( (m_bThankSelect && m_bPressThank) || m_bThankStep2 )
		{
			int dist1 = 20;

			for ( int i = g_PLAYER(0)->GetCardCount() - 1; i >= 0; i-- )
			{
				char cCardIndex = m_PlayerCard[0][i];

				int nAddY = 0;
				if ( IsSelected( cCardIndex ) )
				{
					nAddY = 13;
				}

				if ( (xp >= CardXY[0][0] + i * dist1) && (xp < CardXY[0][0] + i * dist1 + 74) )
				{
					if ( (yp >= CardXY[0][1] - nAddY) && (yp < CardXY[0][1] - nAddY + 99) )
					{
						if ( m_bLClick && m_nLClickType == cCardIndex )
						{
							BYTEDQ::iterator Iter;
							Iter = std::find( m_dqSelectedCard.begin(), m_dqSelectedCard.end(), cCardIndex );
							BOOL bExist = FALSE;
							if ( cCardIndex == CARD_JOKER1 )
							{
								for ( int j = 0; j < m_dqSelectedCard.size(); j++ )
								{
									if ( m_dqSelectedCard[j] >= 60 && m_dqSelectedCard[j] < 120 )
									{
										bExist = TRUE;
										break;
									}
								}
							}
							else if ( cCardIndex == CARD_JOKER2 )
							{
								for ( int j = 0; j < m_dqSelectedCard.size(); j++ )
								{
									if ( m_dqSelectedCard[j] >= 120 && m_dqSelectedCard[j] < 180 )
									{
										bExist = TRUE;
										break;
									}
								}
							}

							if ( bExist || Iter != m_dqSelectedCard.end() )
							{
								if ( Iter != m_dqSelectedCard.end() )
									m_dqSelectedCard.erase( Iter );

								if ( m_dqSelectedCard.empty() )
								{
									m_bClickRegCard = FALSE;
									if ( m_bThankSelect )
									{
//										EnableSelect();
									}
								}
								else
								{
									BYTEDQ dqTemp = m_dqSelectedCard;
									CheckThank( dqTemp );
								}

								if ( FALSE == IsMyPracticeGameObserver() )
									NMBASE::SOUND::g_Sound.PlayWav( SND16 );
							}
							else
							{
								m_bClickRegCard = TRUE;
								m_dqSelectedCard.push_back( cCardIndex );
								BYTEDQ dqTemp = m_dqSelectedCard;
								CheckThank( dqTemp );
								ResetSelect();

								if ( FALSE == IsMyPracticeGameObserver() )
									NMBASE::SOUND::g_Sound.PlayWav( SND16 );
							}
						}
						break;
					}
				}
			}
		}
	}
	m_bLClick = FALSE;
	m_nLClickType = 0;
	m_bDrag = FALSE;

	return FALSE;
}


BOOL CPlayScr::OnLButtonDown(int xp, int yp)
{
	if(g_ObserverMan.IsMyObserver())
		return FALSE;

	if ( m_bThankSelect )
	{
		CRect rect;
		rect.SetRect( 345, 241, 345+156, 241+114 );
		if ( rect.PtInRect( CPoint( xp, yp ) ) )
		{
			m_nSelectCurrent = 0;
			ThankBtnClick();
			return TRUE;
		}
		rect.SetRect( 501, 241, 501+156, 241+114 );
		if ( rect.PtInRect( CPoint( xp, yp ) ) )
		{
			m_nSelectCurrent = 1;
			ThankBtnClick();
			return TRUE;
		}
	}
	else if ( m_bRegSelect )
	{ 
		CRect rect;
		rect.SetRect( 345, 241, 345+156, 241+114 );
		if ( rect.PtInRect( CPoint( xp, yp ) ) )
		{
			RegistByCardSelect( 0 );
			return TRUE;
		}
		rect.SetRect( 501, 241, 501+156, 241+114 );
		if ( rect.PtInRect( CPoint( xp, yp ) ) )
		{
			RegistByCardSelect( 1 );
			return TRUE;
		}
	}

	if ( m_pGameButtonHA->IsEnableBtn(CGameButtonHA::BTN_REGIST) )
	{
		if ( m_nUpCardType == 3 && !m_bGameOver )
		{

			CRect rect;
			rect.SetRect( 345, 241, 345+156, 241+114 );
			if ( rect.PtInRect( CPoint( xp, yp ) ) )
			{
				RegistStraight();;
				return TRUE;
			}
			rect.SetRect( 501, 241, 501+156, 241+114 );
			if ( rect.PtInRect( CPoint( xp, yp ) ) )
			{
				RegistTogether();
				return TRUE;
			}
		}
	}

	if ( g_PLAYER(0)->JoinState == EGAME_JOININ && g_PLAYER(0)->PlayState == EPLAYER_GAMEPLAYING )
	{
		if ( m_bThankSelect && m_bPressThank )
		{
			int dist1 = 20;

			for ( int i = g_PLAYER(0)->GetCardCount() - 1; i >= 0; i-- )
			{
				char cCardIndex = m_PlayerCard[0][i];

				int nAddY = 0;
				if ( IsSelected( cCardIndex ) )
				{
					nAddY = 13;
				}

				if ( (xp >= CardXY[0][0] + i * dist1) && (xp < CardXY[0][0] + i * dist1 + 87) )
				{
					if ( (yp >= CardXY[0][1] - nAddY - 30 ) && (yp < CardXY[0][1] - nAddY + 114) )
					{
						m_bLClick = TRUE;
						m_nLClickType = cCardIndex;
						m_ptDrag.x = xp;
						m_ptDrag.y = yp;
						m_ptDragCard.x = CardXY[0][0] + i * dist1;
						m_ptDragCard.y = CardXY[0][1] - nAddY;
						break;
					}
				}
			}
		}
		else
		{
			int dist1 = 20;
			for ( int i = g_PLAYER(0)->GetCardCount() - 1; i >= 0; i-- )
			{
				char cCardIndex = m_PlayerCard[0][i];

				if ( cCardIndex >= 0 )
				{
					int nAddY = 0;
					if ( IsSelected( cCardIndex ) )
					{
						nAddY = 13;
					}

					if ( (xp >= CardXY[0][0] + i * dist1) && (xp < CardXY[0][0] + i * dist1 + 87) )
					{
						if ( (yp >= CardXY[0][1] - nAddY - 30 ) && (yp < CardXY[0][1] - nAddY + 114) )
						{
							m_bLClick = TRUE;
							m_nLClickType = cCardIndex;
							m_ptDrag.x = xp;
							m_ptDrag.y = yp;
							m_ptDragCard.x = CardXY[0][0] + i * dist1;
							m_ptDragCard.y = CardXY[0][1] - nAddY;
							break;
						}
					}
				}
			}
		}

		// 탱큐 버튼 마우스
//		if ((CGameButtonHA*) m_pGameButtonHA->GetGraphicBtn(CGameButtonHA::BTN_THANK)->OnLButtonDown(xp,yp))
		if ( (xp >= 715 && xp < 715+170) && (yp >= 525 && yp < 525+CARD_JOKER2) )
		{
			//버튼 좌표를 직접 보여주고 있는데 이거 수정할 수 있는 방법 있으면 좋겠네
			if ( !m_pGameButtonHA->IsEnableBtn(CGameButtonHA::BTN_THANK) 
				&& !m_pGameButtonHA->IsShowBtn(CGameButtonHA::BTN_HIGHTHANK)
				&& !m_pGameButtonHA->IsShowBtn(CGameButtonHA::BTN_LOWTHANK)
				&& !m_pGameButtonHA->IsShowBtn(CGameButtonHA::BTN_SEVENFOURTHANK)
				&& !m_pGameButtonHA->IsEnableBtn(CGameButtonHA::BTN_THANKSELECT) )
			{
				if ( !m_bPressThankBtn )
				{
					AutoPlayMSceneSound( &g_sprNoThankBtn, 0 );
					m_dwThankBtnTime = timeGetTime();
					m_bPressThankBtn = TRUE;
				}
			}
		}
	}
	return FALSE;
}

void CPlayScr::ResetREGISTERCARD(REGISTERCARD* RC)
{
	ZeroMemory( RC, sizeof( REGISTERCARD ) );
	for (int i = 0 ; i < 4 ; i++)
	{
		RC->arrSevenCardValue[i] = (char) -1;
	}
	for (int i = 0 ; i < MAX_CARDCOUNT ; i++)
	{
		RC->AttachStCard[i] = (BYTE) -1; 
		RC->AttachToCard[i] = (BYTE) -1;
		RC->StraightCard[0][i] = (BYTE) -1;
		RC->StraightCard[1][i] = (BYTE) -1;
		RC->TogetherCard[0][i] = (BYTE) -1;
		RC->TogetherCard[1][i] = (BYTE) -1;
	}

}
void CPlayScr::RegistStraight()
{
	//선택상황 일단 두가지만 고려하자
	//1개의 낱장 카드가 왼쪽에 붙을수도 있고 오른쪽에 붙을수 있는 경우
	//3장 이상의 7카드가 왼쪽에 붙거나 오른쪽에 붙을 수 있는 경우
	
	if ( m_cClickCardInfo.m_dqAttachStraight.size() == 1 )
	{
		// 1장의 낱장 카드의 선택이 정해지기전 Default는 왼쪽면에 붙는 형태로 되어있음 그러므로 그냥 쏴주자
		REGISTERCARD RC;
		ResetREGISTERCARD(&RC);
		RC.AttachStCard[0] = m_cClickCardInfo.m_dqAttachStraight[0];
		RC.nAtSCount = m_cClickCardInfo.m_dqAttachStraight.size();

		// RC 보내기
		g_GAMEPROCESS()->SendCL_REGIST( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, &RC );

		SetNullButton();

		m_nUpCardType = 0;
		ResetSelect();
	}
	else if ( FALSE == m_cCardInfo.m_dqSeven.empty() )
	{
		if ( m_cCardInfo.m_dqSeven.size() >= 3 )
		{
			//3장 이상의 7카드가 들어올경우

			REGISTERCARD RC;
			//		ZeroMemory( &RC, sizeof( REGISTERCARD ) );
			ResetREGISTERCARD(&RC);

			for ( int i = 0; i < m_cCardInfo.m_dqSeven.size() ; i++ )
			{
				if ( IsSevenCard( m_cCardInfo.m_dqSeven[i]) )
				{
					RC.arrSevenCardValue[i] = (char) m_cCardInfo.m_dqSeven[i];
				}
				else
				{
					//원하는 카드값이 아니므로 리턴
					return;
				}
			}
			RC.nSevenCount =  m_cCardInfo.m_dqSeven.size();

			// RC 보내기
			g_GAMEPROCESS()->SendCL_REGIST( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, &RC );

			SetNullButton();

			m_nUpCardType = 0;

			ResetSelect();
		}		
	}
}
void CPlayScr::RegistTogether()
{
	if ( (m_cClickCardInfo.m_dqAttachStraight.size() == 1) )
	{
		//1장의 낱장카드가 가지는 기본 세팅은
		//왼쪽에 붙는것(묶음 낱장 선택상황에서)
		//고로 위치를 변경해준다

		REGISTERCARD RC;
		ResetREGISTERCARD(&RC);
		
		RC.nAtTCount = (char) m_cClickCardInfo.m_dqAttachStraight.size();
		RC.AttachToCard[0] = m_cClickCardInfo.m_dqAttachStraight[0];

		// RC 보내기
		g_GAMEPROCESS()->SendCL_REGIST( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, &RC );

		SetNullButton();

		m_nUpCardType = 0;

		ResetSelect();
	}
	else if ( FALSE == m_cCardInfo.m_dqSeven.empty() )
	{
		if ( m_cCardInfo.m_dqSeven.size() >= 3 )
		{
			REGISTERCARD RC;
			ResetREGISTERCARD(&RC);

			for( int i = 0 ; i < m_cCardInfo.m_dqSeven.size() ; i++ )
			{
				RC.TogetherCard[0][i] = m_cCardInfo.m_dqSeven[i];
			}
			RC.nToCount[0] = m_cCardInfo.m_dqSeven.size();

			// RC 보내기
			g_GAMEPROCESS()->SendCL_REGIST( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, &RC );

			SetNullButton();

			m_nUpCardType = 0;

			ResetSelect();
		}		
	}
}

void CPlayScr::StopBtnClick()
{
	if ( m_bWind || m_bDrag )
		return;

	StopCard();
	SetNullButton();
}

BOOL CPlayScr::RegisBtnClick( int nType )
{
	if( m_nUpCardType == 3 )
	{
		if ( m_nTurnCnt == g_PLAYER(0)->ServPNum && m_nTurnIndex == 1 )
		{
			//0번 인덱스의 플레이어 정보일경우 선택창이 그려진다.
			//해당 플레그로 함수 정의 조정 필요
			EnableSelect();
		}
		return FALSE;
	}
	if ( TRUE == m_bRegSelect )
	{
		if ( m_nTurnCnt == g_PLAYER(0)->ServPNum && m_nTurnIndex == 1 )
		{
			//0번 인덱스의 플레이어 정보일경우 선택창이 그려진다.
			//해당 플레그로 함수 정의 조정 필요
			EnableSelect();
		}
		return FALSE;
	}


	if ( m_bWind || m_bDrag )
		return FALSE;


	REGISTERCARD RC;
	ZeroMemory( &RC, sizeof( REGISTERCARD ) );
	if ( m_bClickRegCard )
	{
		if ( !m_cClickCardInfo.m_dqUpCard.empty() )
		{
			if ( !m_cClickCardInfo.m_dqSeven.empty() )
			{
				RC.nSevenCount = m_cClickCardInfo.m_dqSeven.size();
				for ( int i = 0; i < RC.nSevenCount; i++ )
				{
					RC.arrSevenCardValue[i] = m_cClickCardInfo.m_dqSeven[i];
				}
			}

			if ( !m_cClickCardInfo.m_dqStraight.empty() )
			{
				if ( m_cClickCardInfo.m_dqStraight.size() == 1 )
				{
					if ( m_cClickCardInfo.m_dqStraight[0].size() == 2 )
					{
						if ( m_cClickCardInfo.m_dqStraight[0][0] >= CARD_JOKER1 && m_cClickCardInfo.m_dqStraight[0][1] < CARD_JOKER1 )
						{
							RC.flag = 1;
							m_cClickCardInfo.m_dqAttachStraight = m_cClickCardInfo.m_dqStraight[0];
							m_cClickCardInfo.m_dqStraight.clear();	
						}
						else if ( m_cClickCardInfo.m_dqStraight[0][0] < CARD_JOKER1 && m_cClickCardInfo.m_dqStraight[0][1] >= CARD_JOKER1 )
						{
							RC.flag = 1;
							m_cClickCardInfo.m_dqAttachStraight = m_cClickCardInfo.m_dqStraight[0];
							m_cClickCardInfo.m_dqStraight.clear();

						}
					}
				}
				
				for ( int i = 0; i < m_cClickCardInfo.m_dqStraight.size(); i++ )
				{
					RC.nStCount[i] = m_cClickCardInfo.m_dqStraight[i].size();
					for ( int j = 0; j < RC.nStCount[i]; j++ )
					{
						RC.StraightCard[i][j] = m_cClickCardInfo.m_dqStraight[i][j];
					}
				}
			}

			if ( !m_cClickCardInfo.m_dqTogether.empty() )
			{
				for ( int i = 0; i < m_cClickCardInfo.m_dqTogether.size(); i++ )
				{
					RC.nToCount[i] = m_cClickCardInfo.m_dqTogether[i].size();
					for ( int j = 0; j < RC.nToCount[i]; j++ )
					{
						RC.TogetherCard[i][j] = m_cClickCardInfo.m_dqTogether[i][j];
					}
				}
			}

			if ( !m_cClickCardInfo.m_dqAttachStraight.empty() )
			{
				RC.nAtSCount = m_cClickCardInfo.m_dqAttachStraight.size();
				for ( int i = 0; i < RC.nAtSCount; i++ )
				{
					RC.AttachStCard[i] = m_cClickCardInfo.m_dqAttachStraight[i];
				}
			}

			if ( !m_cClickCardInfo.m_dqAttachTogether.empty() )
			{
				RC.nAtTCount = m_cClickCardInfo.m_dqAttachTogether.size();
				for ( int i = 0; i < RC.nAtTCount; i++ )
				{
					RC.AttachToCard[i] = m_cClickCardInfo.m_dqAttachTogether[i];
				}
			}

			if ( nType == 3 && RC.nAtSCount == 1 && RC.nAtTCount == 0 )
			{
				RC.nAtSCount = 0;
				char cCI = RC.AttachStCard[0];
				RC.AttachStCard[0] = -1;
				RC.nAtTCount = 1;
				RC.AttachToCard[0] = cCI;
			}
			else if ( nType == 4 && ( RC.nToCount[0] == 3 || RC.nToCount[0] == 4 ) && RC.nToCount[1] == 0 )
			{
				BOOL bExist = TRUE;
				for ( int i = 0; i < RC.nToCount[0]; i++ )
				{
					if ( RC.TogetherCard[0][i] % 13 != 6 )
						bExist = FALSE;
				}

				if ( bExist )
				{
					RC.nSevenCount = RC.nToCount[0];
					for ( int i = 0; i < RC.nToCount[0]; i++ )
					{
						RC.arrSevenCardValue[i] = RC.TogetherCard[0][i];
						RC.TogetherCard[0][i] = -1;
					}
					RC.nToCount[0] = 0;
				}
			}
			m_nUpCardType = 0;

			// RC 보내기			
			g_GAMEPROCESS()->SendCL_REGIST(g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, &RC );			
			SetNullButton();

			m_dqSelectedCard.clear();

			return TRUE;
		}
	}
	else
	{
		if ( m_bRegSelect )
		{
		}
		else
		{
			//자동등록
			if ( !m_cCardInfo.m_dqUpCard.empty() )
			{
				if ( !m_cCardInfo.m_dqSeven.empty() )
				{
					RC.nSevenCount = m_cCardInfo.m_dqSeven.size();
					for ( int i = 0; i < RC.nSevenCount; i++ )
					{
						RC.arrSevenCardValue[i] = m_cCardInfo.m_dqSeven[i];
					}
				}

				if ( !m_cCardInfo.m_dqStraight.empty() )
				{
					for ( int i = 0; i < m_cCardInfo.m_dqStraight.size(); i++ )
					{
						RC.nStCount[i] = m_cCardInfo.m_dqStraight[i].size();
						for ( int j = 0; j < RC.nStCount[i]; j++ )
						{
							RC.StraightCard[i][j] = m_cCardInfo.m_dqStraight[i][j];
						}
					}
				}

				if ( !m_cCardInfo.m_dqTogether.empty() )
				{
					for ( int i = 0; i < m_cCardInfo.m_dqTogether.size(); i++ )
					{
						RC.nToCount[i] = m_cCardInfo.m_dqTogether[i].size();
						for ( int j = 0; j < RC.nToCount[i]; j++ )
						{
							RC.TogetherCard[i][j] = m_cCardInfo.m_dqTogether[i][j];
						}
					}
				}

				if ( !m_cCardInfo.m_dqAttachStraight.empty() )
				{
					RC.nAtSCount = m_cCardInfo.m_dqAttachStraight.size();
					for ( int i = 0; i < RC.nAtSCount; i++ )
					{
						RC.AttachStCard[i] = m_cCardInfo.m_dqAttachStraight[i];
					}
				}

				if ( !m_cCardInfo.m_dqAttachTogether.empty() )
				{
					RC.nAtTCount = m_cCardInfo.m_dqAttachTogether.size();
					for ( int i = 0; i < RC.nAtTCount; i++ )
					{
						RC.AttachToCard[i] = m_cCardInfo.m_dqAttachTogether[i];
					}
				}

				m_nUpCardType = 0;

				// RC 보내기
				g_GAMEPROCESS()->SendCL_REGIST(g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, &RC );

				SetNullButton();
				m_dqSelectedCard.clear();

				return TRUE;
			}
		}
	}
	return FALSE;
}

void CPlayScr::RegistByCardSelect(int nCardDeckIndex)
{
	if ( m_cCardInfo.m_dqSelect.empty() )
	{
		return;
	}
	if ( nCardDeckIndex == -1 || m_nSelectMax == -1 )
	{
		return;
	}
	if ( nCardDeckIndex >= m_cCardInfo.m_dqSelect.size() )
	{
		return;
	}



	REGISTERCARD RC;
	ResetREGISTERCARD(&RC);
	
	BOOL bStraight = FALSE;


	for ( int i = 0; i < m_cCardInfo.m_dqSelect[nCardDeckIndex].size() - 1; i++ )
	{
		if ( m_cCardInfo.m_dqSelect[nCardDeckIndex][i] % 13 != m_cCardInfo.m_dqSelect[nCardDeckIndex][i+1] % 13 )
		{
			bStraight = TRUE;
			break;
		}
	}
	if ( bStraight )
	{
		RC.nStCount[0] = m_cCardInfo.m_dqSelect[nCardDeckIndex].size();
		for ( int j = 0; j < RC.nStCount[0]; j++ )
		{
			RC.StraightCard[0][j] = m_cCardInfo.m_dqSelect[nCardDeckIndex][j];
		}
	}
	else
	{
		RC.nToCount[0] = m_cCardInfo.m_dqSelect[nCardDeckIndex].size();
		for ( int j = 0; j < RC.nToCount[0]; j++ )
		{
			RC.TogetherCard[0][j] = m_cCardInfo.m_dqSelect[nCardDeckIndex][j];
		}
	}

	m_nUpCardType = 0;

	g_GAMEPROCESS()->SendCL_REGIST(g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, &RC );

	ResetSelect();
	SetNullButton();
	m_bRegSelect = FALSE;

	m_dqSelectedCard.clear();

	return;
}

void CPlayScr::ThankBtnClick()
{
	if ( m_bWind || m_bDrag )
		return;

	if ( m_bThankReserved )
	{
		if ( !m_bPressThank )
		{
			char MyCard[8];
			memset( MyCard, -1, sizeof(char)*8 );
			for ( int i = 0; i < 8; i++ )
			{
				if ( m_PlayerCard[0][i] != -1 )
				{
					MyCard[i] = m_PlayerCard[0][i];
				}
			}
			CCL_PRESS_THANK msg;
			msg.Set( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, MyCard );
			NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());

			m_bPressThank = TRUE;
			m_bThankStep2 = FALSE;

			if ( !m_bThankSelect )
			{
				SetNullButton();
			}
		}
		else
		{
			SelectOkBtnClick();
		}
	} 
}

void CPlayScr::PassBtnClick()
{
	if ( m_bWind || m_bDrag )
		return;

	PassThankyou();
	SetNullButton();
	if ( m_pGameButtonHA->IsShowBtn(CGameButtonHA::BTN_LOWTHANK) )
		m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_LOWTHANK, FALSE);
}

void CPlayScr::DrawCommonCard()
{
	int i, j, dist, cnt, index;

	int grade = 80;

	const int CardXp = 176;
	const int CardYp = 218;
	const int distx = 16;
	const int disty = 42;


	for( int i = 0; i < 4; i ++ )
	{
		for( int j = 0; j < 13; j ++ )
		{
			if( m_CenterCard[i][j] > 0 )
			{
				int nIndex = 0;
				if ( m_CenterCard[i][j] == 2 )
				{
					nIndex = CARD_JOKER1;
				}
				else if ( m_CenterCard[i][j] == 3 )
				{
					nIndex = CARD_JOKER2;
				}
				else
				{
					nIndex = i * 13 + j;	
				}

				m_pPage->PutSprAuto( CardXp + j * distx, CardYp + i * disty, &g_sprBigCard, nIndex );
				if ( IsNewCard( nIndex ) )
				{
					m_pPage->DrawMultiScene( CardXp + j * distx, CardYp + i * disty, &g_sprBigCard, 1, timeGetTime() );
				}
			}
		}
	}	

	int nCount = 0;
	for ( int i = 0; i < 13; i++ )
	{
		int nSameNumCardCnt = 0;
		for( int j = 0 ; j < 4;j ++ )
		{
			if( m_SameNumCard[i][j] )
			{
				nSameNumCardCnt++;
			}
		}
		if ( nSameNumCardCnt >= 1 )
		{
			nCount++;
		}
	}

	const int CardXp_Same = 560;

	if ( nCount >= 1 )
	{
		int nOrder = 0;
		for( i = 0, dist = 0; i < 13; i ++ )
		{		
			int nX, nY;
			for( j = 0, cnt = 0; j < 4; j ++ )
			{
				if( m_SameNumCard[i][j] )
				{
					if ( m_SameNumCard[i][j] == 2 )
					{
						index = CARD_JOKER1;
					}
					else if ( m_SameNumCard[i][j] == 3 )
					{
						index = CARD_JOKER2;
					}
					else
					{
						index = j * 13 + i;	
					}

					if ( nCount > 7 )
					{
						nX = CardXp_Same + dist * distx;
					}
					else
					{
						int nDist = m_dqSameCardPosIndex[nCount-1][dist];
						nX = CardXp_Same + (nDist-1) * distx;
					}

					nY = CardYp + cnt * disty;
					m_pPage->PutSprAuto( nX, nY, &g_sprBigCard, index );

					if ( TRUE == IsNewCard(index) )
					{
						m_pPage->DrawMultiScene( nX, nY, &g_sprBigCard, 1, timeGetTime() );
					}

					cnt++;
				}
			}

			if ( cnt >= 1 )
			{
				dist++;
			}
		}
	}
}

void CPlayScr::DrawMyCardShortcut()
{	
	//카드 하단에 단축키를 표시한다. 단축키는 1부터 8까지가 표시되고 카드 갯수 만큼만 그린다.
	//자신이 옵저버가 아니고 카드를 가지고 있을 경우만 표시된다.
// 	if ( g_ObserverMan.IsMyObserver() || ( TRUE == m_bGoldState ) )
// 	{
// 		// 자신이 관전자 이거나, 골드 스테이지 애니메이션이 노출중일 경우 
// 		// 카드 단축키는 노출시키지 않는다.
// 		return;
// 	}
	if (g_ObserverMan.IsMyObserver())
	{
		return;
	}
	if ( TRUE == m_bStartFrame)
	{
		return;
	}
	if ( g_RI.State == EGAME_WAITING )
	{
		return;
	}

	static const int nCardInterval = 20; 
	static const int nAddPosY_ByCardPos = 98;
	static const int nPlayerNum = 0;

	int nDrawShortcutCnt = 0; //단축키는 카드 갯수 만큼만 그린다

	
	nDrawShortcutCnt = g_PLAYER(0)->GetCardCount();
	for ( int i = 0 ; i < nDrawShortcutCnt ; i++)
	{
		m_pPage->PutSprAuto( CardXY[nPlayerNum][0] + i * nCardInterval, CardXY[nPlayerNum][1] + nAddPosY_ByCardPos, &g_SprCardShortcut,i );
	}
}
void CPlayScr::DrawRemainCardCount( CDC *pDC )
{
	if( g_RI.State == EGAME_PLAYING	 && m_nCurStockCnt >= 0 )
	{		
		// 덱에 남은 카드의 갯수을 그린다.
		m_pPage->PutSprAuto( 464, 304, &g_sprCardNum, 10 );

		int n1 = m_nCurStockCnt / 10;
		int n2 = m_nCurStockCnt % 10;

		m_pPage->PutSprAuto( 467, 305, &g_sprCardNum, n1 );
		m_pPage->PutSprAuto( 479, 305, &g_sprCardNum, n2 );

		// 카드덱에 남은 카드가 5장보다 적을 경우,
		// 남은 카드에 대한 텍스트를 노출한다.
		if( m_nCurStockCnt <= 5 )
		{
			pDC->SelectObject( NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12_BOLD) );

			CRect rc;
			rc.SetRect( 470, 202, 470 + 65, 202 + 15 );

			CString str;
			str.Format( "게임종료" );
			pDC->SetTextColor( RGB( 242 ,242, 242 ) );
			pDC->DrawText( str, &rc, DT_CENTER );

			rc.SetRect( 470, 219, 470 + 60, 219 + 15 );
			str.Format( " %d장 전", n2 );
			pDC->SetTextColor( RGB( 255 ,234, 0 ) );
			pDC->DrawText( str, &rc, DT_CENTER );	
		}
	}
}

void CPlayScr::DrawPlayerCard( CDC *pDC )
{
	if( GM().GetMyInfo()->PlayState == EPLAYER_GAMEEND && m_bPracticsGame )
	{
		//내가 게임 진행중이 아니고, 방에서 연습게임이 진행되고 있다면 카드를 그리지 않는다.
		return;
	}

	int ch, i;
	int dist1, dist2;
	int st = 329;

	if( m_nDisCardCnt > 0 && m_bStartFrame == FALSE )	
	{
		m_pPage->PutSprAuto( 464, 344, &g_sprBigCard, m_DisCard[m_nDisCardCnt-1] );
	}

	int pnum;

	DWORD dwTime = timeGetTime();

	if ( m_bStartFrame == FALSE )
	{
		for( ch = 0; ch < MAX_PLAYER; ch++)
		{
			dist1 = 20; 
			dist2 = 11;

			if ( strcmp( g_PLAYER(ch)->UI.ID, "_PRACTICSUSER_" ) != 0 )
			{
				if( g_PLAYER(ch)->UI.UNum == 0 ) 
				{
					continue;
				}
			}

			int nServPNum = g_GAMEPROCESS()->GetServPNum_ByPN( ch );
			pnum = ch;


			if( pnum == 0 )
			{
				// 중앙 하단의 뷰에 그리기
				// 중앙 하단의 뷰에는 자신이 관전중일 경우 ServerPNum이 0인 대상을 그리고 아닐경우 자신을 그린다

				int nCardSum = 0;
				int nJoker = 0;
				if( !GameResultFlag && g_PLAYER(pnum)->JoinState == EGAME_JOININ && g_PLAYER(pnum)->PlayState == EPLAYER_GAMEPLAYING )
				{
					if ( m_nTurnCnt == g_PLAYER(0)->ServPNum || m_bThankReserved )
					{
						for( i = 0; i < g_PLAYER(pnum)->GetCardCount(); i ++ )
						{
							char cCardIndex = m_PlayerCard[0][i];

							if ( FALSE == IsJokerCard( cCardIndex ))
							{
								if ( TRUE == IsSevenCard( cCardIndex ) )
								{
									nCardSum += 14;
								}
								else
								{
									nCardSum += (cCardIndex%13)+1;
								}
							}
							else 
								nJoker++;

							if ( m_bDrag && cCardIndex == m_nLClickType )
								continue;

							int nType = 0;
							int nAddY = 0;
							if ( IsSelected( cCardIndex ) )
							{
								nAddY = 13;
							}
							

							if ( !g_ObserverMan.IsMyObserver() )
							{
								m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist1, CardXY[pnum][1] - nAddY, &g_sprMyCard, m_PlayerCard[0][i]);

								if ( (nType = CanRegistedCard( cCardIndex )) > 0 )
								{
									if ( nType == 1 ) // 등록 가능
									{
										m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist1 + 2, CardXY[pnum][1] - nAddY - 18, &g_sprFocus, 0 );
									}
									if ( nType == 2 ) // 등록 가능하나 선택 되지 아니함
									{
										m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist1 + 2, CardXY[pnum][1] - nAddY - 18, &g_sprFocus, 0, HB );
									}
								}
							}
							else // 관전중이면 뒷면만 보여준다
							{
								if ( m_bDrawGold && m_nGoldStage0 > 1)
								{
									m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist1, CardXY[pnum][1], &g_sprMyCard, 55 );
								}
								else
								{
									m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist1, CardXY[pnum][1], &g_sprMyCard, 54 );
								}
							}
						}
					}
					else 
					{

						for( i = 0; i < g_PLAYER(pnum)->GetCardCount(); i ++ )
						{
							if ( !g_ObserverMan.IsMyObserver() )
							{
								//자신이 관전자가 아닐경우
								if ( FALSE == IsJokerCard( m_PlayerCard[0][i] ) )
								{
									if ( TRUE == IsSevenCard( m_PlayerCard[0][i] ) )
									{
										nCardSum += 14;
									}
									else
									{
										nCardSum += (m_PlayerCard[0][i]%13)+1;
									}
								}
								else
								{
									nJoker++;
								}									

								if ( m_bDrag && m_nLClickType == m_PlayerCard[0][i] )
									continue;

								m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist1, CardXY[pnum][1], &g_sprMyCard, m_PlayerCard[0][i]);
								
								if ( CanRegistedCard( m_PlayerCard[0][i] ) > 0 )
								{
									m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist1 + 2, CardXY[pnum][1] - 18, &g_sprFocus, 1 );
								}
							}
							else
							{
								if ( m_bDrawGold && m_nGoldStage0 > 1)
								{
									m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist1, CardXY[pnum][1], &g_sprMyCard, 55 );
								}
								else
								{
									m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist1, CardXY[pnum][1], &g_sprMyCard, 54 );
								}
							}
						}
					}
					if ( m_bDrag )
					{
						m_pPage->PutSprAuto( m_ptDragCard.x, m_ptDragCard.y, &g_sprMyCard, m_nLClickType );						
					}
				}
				else if (  GameResultFlag && g_PLAYER(pnum)->JoinState == EGAME_JOININ && g_PLAYER(pnum)->PlayState == EPLAYER_GAMEEND )
				{
					// 자신이 게임방에 있고 게임을 진행하고 있지 않을경우
					// GameResultFlag는 어떤 용도로 쓰이는 것인가?

					int nCount = 0;

					for( i = 0; i < MAX_CARDCOUNT ; i ++ )
					{
						if ( m_PlayerCard[0][i] >= 0 )
						{
							if ( FALSE == IsJokerCard( m_PlayerCard[0][i] ) )
							{
								if ( TRUE == IsSevenCard( m_PlayerCard[0][i] ))
								{
									nCardSum += 14;
								}
								else
								{
									nCardSum += (m_PlayerCard[0][i]%13)+1;
								}
							}
							else 
								nJoker++;

							m_pPage->PutSprAuto( CardXY[0][0] + nCount * dist1, CardXY[0][1], &g_sprMyCard, m_PlayerCard[0][i]);
							nCount++;
						}
					}
				}
				
				if ( !g_ObserverMan.IsMyObserver() && g_PLAYER(0)->JoinState == EGAME_JOININ && g_PLAYER(0)->PlayState == EPLAYER_GAMEPLAYING )
				{
					// 자신이 관전중이 아니고, 게임에 참여하고 있으며, 게임을 진행 중일때
					// 자신의 게임상황에서 카드 합계 그리기

					nCardSum += 14 * nJoker;
					
					if ( nCardSum >= 100 )
						nCardSum = 99;

					if ( nCardSum > 0 )
					{
						CString strCardSumLabel;
						CString strCardSumNumber;
						CRect rtRect;
						strCardSumLabel.Format(_T("내 카드 합계:"));
						strCardSumNumber.Format("%d",nCardSum);
						COLORREF oldColor = pDC->SetTextColor(RGB( 190, 167, 88 ));
						rtRect.SetRect( 326, 529, 326 + 70, 529 + 10);
						CFont *pOldFont = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
						pDC->DrawText( strCardSumLabel, &rtRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

						rtRect.SetRect( 392, 529, 392 + 20, 529 + 10);
						pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_BOLD));
						pDC->DrawText( strCardSumNumber, &rtRect, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
						
						pDC->SetTextColor(oldColor);
						pDC->SelectObject(pOldFont);
					}
				}
			}
			else
			{
				// PlayerNumber가 0이 아닌 경우 - 가운데 하단 카드를 그리는 것이 아닌 다른 위치의 카드들을 그린다.
				if( !GameResultFlag && g_PLAYER(pnum)->JoinState == EGAME_JOININ && g_PLAYER(pnum)->PlayState == EPLAYER_GAMEPLAYING )
				{
					// 내가 사용자가 게임방에 참여하였고 게임을 진행중이며
					// 이때 다른 사용자의 카드를 그리며 게임이 진행중이므로 카드는 뒷면만 표현된다.

					dist2 = 11;
					CRect rect;

					CPlayerHoola* pPlayer = g_PLAYER( pnum );

					for( i = 0; i < g_PLAYER(pnum)->GetCardCount(); i ++ )
					{
						if ( m_bDrawGold && m_nGoldStage0 > 1)
						{
							m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist2 , CardXY[pnum][1], &g_sprSmallCard, 55 );
						}
						else
						{
							m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist2 , CardXY[pnum][1], &g_sprSmallCard, 54 );
						}
					}
					
					int CXY[MAX_PLAYER][2] = { 0, 0, 14, 460, 14, 269, 860, 269, 860, 460 };
					
					if ( pnum < 3 )
					{
						CPlayerHoola* pPlayer = g_PLAYER( pnum );

						if ( g_PLAYER(pnum)->GetCardCount() > 0 )
						{
							if ( g_PLAYER(pnum)->GetCardCount() != m_nOldCardCnt[pnum] )
							{
								m_nOldCardCnt[pnum] = g_PLAYER(pnum)->GetCardCount();
								m_nOldCardCntTime[pnum] = dwTime;
							}

							m_pPage->DrawScene( CXY[pnum][0], CXY[pnum][1], &g_sprCardCount, g_PLAYER(pnum)->GetCardCount() + 9, dwTime - m_nOldCardCntTime[pnum] );
						}
					}
					else 
					{
						if ( g_PLAYER(pnum)->GetCardCount() > 0 )
						{
							if ( g_PLAYER(pnum)->GetCardCount() != m_nOldCardCnt[pnum] )
							{
								m_nOldCardCnt[pnum] = g_PLAYER(pnum)->GetCardCount();
								m_nOldCardCntTime[pnum] = dwTime;
							}
							m_pPage->DrawScene( CXY[pnum][0]-3, CXY[pnum][1], &g_sprCardCount, g_PLAYER(pnum)->GetCardCount() + 9, dwTime - m_nOldCardCntTime[pnum] );
						}
					}

				}
				else if ( GameResultFlag && g_PLAYER(pnum)->JoinState == EGAME_JOININ && g_PLAYER(pnum)->PlayState == EPLAYER_GAMEEND )
				{
					dist2 = 11;
					CRect rect;
					int nCount = 0;
					char CardTemp[8];
					ZeroMemory( CardTemp, 8 );
					
					for( i = 0; i < 8; i ++ )
					{
						if ( m_PlayerCard[pnum][i] >= 0 )
						{
							m_pPage->PutSprAuto( CardXY[pnum][0] + nCount * dist2, CardXY[pnum][1], &g_sprSmallCard, m_PlayerCard[pnum][i] /*, TEMPCLIP*/ );
							nCount++;
						}
					}
					

					int CXY[MAX_PLAYER][2] = { 0, 0, 14, 460, 14, 269, 860, 269, 860, 460 };
					if ( pnum < 3 )
					{
						if ( g_PLAYER(pnum)->GetCardCount() > 0 )
						{
							if ( g_PLAYER(pnum)->GetCardCount() != m_nOldCardCnt[pnum] )
							{
								m_nOldCardCnt[pnum] = g_PLAYER(pnum)->GetCardCount();
								m_nOldCardCntTime[pnum] = dwTime;
							}
							m_pPage->DrawScene( CXY[pnum][0], CXY[pnum][1], &g_sprCardCount, g_PLAYER(pnum)->GetCardCount() + 9, dwTime - m_nOldCardCntTime[pnum] );
						}
					}
					else 
					{
						if ( g_PLAYER(pnum)->GetCardCount() > 0 )
						{
							if ( g_PLAYER(pnum)->GetCardCount() != m_nOldCardCnt[pnum] )
							{
								m_nOldCardCnt[pnum] = g_PLAYER(pnum)->GetCardCount();
								m_nOldCardCntTime[pnum] = dwTime;
							}
							m_pPage->DrawScene( CXY[pnum][0]-3, CXY[pnum][1], &g_sprCardCount, g_PLAYER(pnum)->GetCardCount() + 9, dwTime - m_nOldCardCntTime[pnum] );
						}
					}
				}
			}
		}
	}
}

void CPlayScr::Draw(CDC *pDC)
{
	// 게임이 진행중일때 표시되는 이미지들을 그린다.
	// 사용자의 상태에 따라 표현되는 이미지가 다르므로 구분한다.

	if ( g_ObserverMan.IsMyObserver() == TRUE && m_bPracticsGame == TRUE )
	{	//자신이 관전중이고, 방에서 연습게임이 진행중이라면 게임 정보를 노출시키지 않는다.
		return;
	}
	else if (  g_PLAYER(0)->JoinState == EGAME_JOINOUT && g_PLAYER(0)->PlayState == EPLAYER_GAMEEND && g_RI.State == EGAME_WAITING)
	{	//게임방이 진행중이지 않고 자신이 게임을 진행중이지 않다면 대기중 표시와 함께 게임 정보를 노출시키지 않는다.
		m_pPage->DrawMultiScene( 451, 585, &g_sprWaiting, 1, timeGetTime() );
		return;
	}

	COLORREF crOld = pDC->GetTextColor();

	//게임 도움 텍스트 출력
	DrawHelpMsg();
	//미션 시작 메시지 출력
	DrawMissionStart();
	//미션 메시지 노출
	DrawMission(pDC);
	//바닥패 노출
	DrawCommonCard();
	//덱에 남은 카드 그리기
	DrawRemainCardCount(pDC);
	//사용자들의 카드를 그린다
	DrawPlayerCard(pDC);
	//선고르기
	m_cSunPickup.Draw( m_pPage );
	//게임시작 상황에서의 카드 정렬 애니메이션 노출
	DrawStartFrame();
	//남은 시간 표시 타이머를 그린다.
	DrawProcessTimer();
	//땡큐 상황에서의 카드 움직임을 그린다.
	DrawThankMove();
	//등록 상황에서의 카드 움직임을 그린다.
	DrawRegMove();
	//받기 상황에서의 카드 움직임을 그린다.
	DrawGetMove();
	//버리기 상황에서의 카드 움직임을 그린다.
	DrawDumpMove();
	//카드 선택 창을 그린다.
	DrawSelectCard();
	//특수 상황의 카드 선택 창을 그린다.
	DrawSelectCardSpecialCase();
	//저장해둔 예전 폰트 색상으로 변경
	pDC->SetTextColor( crOld );
	//카드 숏컷 그리기, 차상위 노출
	DrawMyCardShortcut();
	//게임 이펙트 노출, 최상위 노출
	DrawAniEffect();
}

void CPlayScr::DrawNoThankBtnSpr()
{
	// 땡큐 버튼이 눌렸을 때, 땡큐가 가능한 상황이 아닐경우 bool플래그가 TRUE가 된다. 
	// 일정 시간동안 해당 땡큐 애니메이션이 호출된다. -> 이거 플레이 버튼만 누르면 작동하게 구성할 수 있지 않을까.
	if ( m_bPressThankBtn )
	{
		DWORD dwTime = abs( (int)(timeGetTime() - m_dwThankBtnTime) );

		if ( dwTime < g_sprNoThankBtn.GetMScenePlayTime(0) )
		{	
			m_pPage->DrawMultiScene( 0, 0, &g_sprNoThankBtn, 0, dwTime );		
		}
		else
		{
			m_bPressThankBtn = FALSE;
			m_dwThankBtnTime = 0;
		}
	}
}


void CPlayScr::GetCardBtnClick()
{
	// ### [ 관전기능 ] ###
	if ( g_ObserverMan.IsMyObserver() ) 
		return;
	
	if ( m_bWind || m_bDrag )
		return;

	CCL_ASK_ONECARD	msg;
	msg.Set( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID );
	NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	
	SetNullButton();
	ResetLimitTime();
}
void CPlayScr::DumpCardBtnClick()
{
	// ### [ 관전기능 ] ###
	if ( g_ObserverMan.IsMyObserver() ) return;
	if ( m_bWind || m_bDrag )			return;

	if ( g_PLAYER(0)->GetCardCount() == 1 )
	{
		char cCardIndex = m_PlayerCard[0][0];
		if( ( (g_RI.FormKind == hoola::eFORM_KIND_JOKER) && cCardIndex >= 0 && cCardIndex <= CARD_JOKER2 ) || ( g_RI.FormKind == hoola::eFORM_KIND_NORMAL && cCardIndex >= 0 && cCardIndex <= 51 ) )
		{
			CCL_DISCARD msg;
			msg.Set( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, cCardIndex );
			NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			SetNullButton();
			ResetLimitTime();

			m_nDiscardTest = cCardIndex;
			return;
		}
	}

	if ( m_dqSelectedCard.empty() ) return;
	
	char cCardIndex = m_dqSelectedCard[m_dqSelectedCard.size() -1];

	if( ( (g_RI.FormKind == hoola::eFORM_KIND_JOKER) && cCardIndex >= 0 && cCardIndex <= CARD_JOKER2 ) || ( g_RI.FormKind == hoola::eFORM_KIND_NORMAL && cCardIndex >= 0 && cCardIndex <= 51 ) )
	{
		CCL_DISCARD msg;
		msg.Set( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, cCardIndex );
		NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		SetNullButton();
		ResetLimitTime();
		m_nDiscardTest = cCardIndex;
	}
}

void CPlayScr::DumpCardBtnClickAfter()
{
	// ### [ 관전기능 ] ###
	if ( g_ObserverMan.IsMyObserver() ) 
		return;

	char cCardIndex = -1;
	char cCardIndex1 = -1;
	for ( int i = 0; i < 8; i++ )
	{
		if ( m_PlayerCard[0][i] >= 0 )
		{
			if ( cCardIndex <= (m_PlayerCard[0][i] % 13) && m_PlayerCard[0][i] <= 51 )
			{
				cCardIndex = m_PlayerCard[0][i] % 13;
				cCardIndex1 = m_PlayerCard[0][i];
			}
		}
	}
	if( ( (g_RI.FormKind == hoola::eFORM_KIND_JOKER) && cCardIndex >= 0 && cCardIndex <= CARD_JOKER2 ) || ( g_RI.FormKind == hoola::eFORM_KIND_NORMAL && cCardIndex >= 0 && cCardIndex <= 51 ) )
	{
		CCL_DISCARD msg;
		msg.Set( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, cCardIndex1 );
		NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		SetNullButton();
		ResetLimitTime();

		m_nDiscardTest = cCardIndex1;
	}
}
BOOL CPlayScr::IsButtonClickEnableState()
{
	// ### [ 관전기능 ] ###
	if ( g_ObserverMan.IsMyObserver() )
	{
		return FALSE;
	}
	if ( m_bWind || m_bDrag )
	{
		return FALSE;
	}
	if ( g_RI.State == EGAME_WAITING )
	{
		return FALSE;
	}
	return TRUE;
}

void CPlayScr::SetSortBtn()
{
	if ( FALSE == IsButtonClickEnableState() )
		return;

	if( m_eSortBtnType == eSORTBTN_TOGETHER )
	{
		m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_SORTCARD_STRAIGHT, FALSE);
		m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_SORTCARD_TOGETHER, TRUE);
	}
	else if ( m_eSortBtnType == eSORTBTN_STRAIGHT )
	{
		m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_SORTCARD_STRAIGHT, TRUE);
		m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_SORTCARD_TOGETHER, FALSE);
	}
}
void CPlayScr::SortBtnClick()
{
	if ( FALSE == IsButtonClickEnableState() )
		return;

	if( m_pGameButtonHA->GetGraphicBtn(CGameButtonHA::BTN_SORTCARD_STRAIGHT)->IsShow() )
	{
		m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_SORTCARD_STRAIGHT, FALSE);
		m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_SORTCARD_TOGETHER, TRUE);
		SortMyCard( TRUE ); //스트레이트 타입으로 소트
		m_eSortBtnType = eSORTBTN_STRAIGHT;
	}
	else if ( m_pGameButtonHA->GetGraphicBtn(CGameButtonHA::BTN_SORTCARD_TOGETHER)->IsShow() )
	{
		m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_SORTCARD_STRAIGHT, TRUE);
		m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_SORTCARD_TOGETHER, FALSE);
		SortMyCard( FALSE ); //스트레이트 타입으로 소트
		m_eSortBtnType = eSORTBTN_TOGETHER;
	}
	NMBASE::SOUND::g_Sound.PlayWav( SND30 );
}
void CPlayScr::SetThankSelectButton()
{
	// ### [ 관전기능 ] ###
	if ( g_ObserverMan.IsMyObserver() )
		return;
	SetHelpMsg(eHELPMSG_THANKSELECT);
	NMBASE::SOUND::g_Sound.PlayWav( SND17 );

	m_pGameButtonHA->SetShowBtn(	CGameButtonHA::BTN_STOP,	FALSE);
	m_pGameButtonHA->SetEnableBtn(	CGameButtonHA::BTN_REGIST,	FALSE);	
	m_pGameButtonHA->SetShowBtn(	CGameButtonHA::BTN_PASS,	TRUE);
	m_pGameButtonHA->SetShowBtn(	CGameButtonHA::BTN_GETCARD, TRUE);
	m_pGameButtonHA->SetEnableBtn(	CGameButtonHA::BTN_GETCARD, FALSE);
	m_pGameButtonHA->SetShowBtn(	CGameButtonHA::BTN_DUMPCARD, FALSE);
	m_pGameButtonHA->SetEnableBtn(	CGameButtonHA::BTN_THANK,	FALSE);
	m_pGameButtonHA->SetShowBtn(	CGameButtonHA::BTN_THANKSELECT,	TRUE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_PASS, FALSE);
	SetLimitTime( eTIMER_THANK_SELECT );
	
}

void CPlayScr::SetThankButton(eTHANK_TYPE eType)
{
	// ### [ 관전기능 ] ###
	if ( g_ObserverMan.IsMyObserver() )
		return;

	SetHelpMsg( eHELPMSG_THANKENABLE );
	NMBASE::SOUND::g_Sound.PlayWav( SND17 );


	m_pGameButtonHA->SetShowBtn(	CGameButtonHA::BTN_STOP,	FALSE);
	m_pGameButtonHA->SetEnableBtn(	CGameButtonHA::BTN_REGIST,	FALSE);	
	m_pGameButtonHA->SetShowBtn(	CGameButtonHA::BTN_PASS,	TRUE);
	m_pGameButtonHA->SetShowBtn(	CGameButtonHA::BTN_GETCARD, TRUE);
	m_pGameButtonHA->SetEnableBtn(	CGameButtonHA::BTN_GETCARD, FALSE);
	m_pGameButtonHA->SetShowBtn(	CGameButtonHA::BTN_DUMPCARD, FALSE);

	switch(eType)
	{
	case eTHANK_NORMAL:
		{
			m_pGameButtonHA->SetEnableBtn(	CGameButtonHA::BTN_THANK,	TRUE);
		}
		break;
	case eTHANK_LOW:
		{
			m_pGameButtonHA->SetEnableBtn(	CGameButtonHA::BTN_THANK,	FALSE);
			m_pGameButtonHA->SetShowBtn(	CGameButtonHA::BTN_LOWTHANK,	TRUE);
		}
		break;
	case eTHANK_HIGH:
		{
			m_pGameButtonHA->SetEnableBtn(	CGameButtonHA::BTN_THANK,	FALSE);
			m_pGameButtonHA->SetShowBtn(	CGameButtonHA::BTN_HIGHTHANK,	TRUE);
		}
		break;
	case eTHANK_SEVEN4CARD:
		{
			m_pGameButtonHA->SetEnableBtn(	CGameButtonHA::BTN_THANK,	FALSE);
			m_pGameButtonHA->SetShowBtn(	CGameButtonHA::BTN_SEVENFOURTHANK,	TRUE);
		}
		break;
	default:
		break;
	}
}


void CPlayScr::SetNullButton()
{
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_STOP, TRUE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_STOP, FALSE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_REGIST, FALSE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_THANK, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_PASS, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_GETCARD, TRUE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_GETCARD, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_DUMPCARD, FALSE);

	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_LOWTHANK, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_HIGHTHANK, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_SEVENFOURTHANK, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_THANKSELECT, FALSE);
}

void CPlayScr::SetButtonGameOver()
{
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_STOP, TRUE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_STOP, FALSE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_REGIST, FALSE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_THANK, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_PASS, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_GETCARD, TRUE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_GETCARD, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_DUMPCARD, FALSE);

	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_SORTCARD_STRAIGHT, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_THANKSELECT, FALSE);
}

void CPlayScr::SetAllBtnClose()
{
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_STOP, TRUE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_STOP, FALSE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_REGIST, FALSE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_THANK, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_PASS, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_GETCARD, TRUE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_GETCARD, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_DUMPCARD, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_LOWTHANK, FALSE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_SORTCARD_STRAIGHT, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_THANKSELECT, FALSE);
}

void CPlayScr::SetGetCardButton()
{
	// ### [ 관전기능 ] ###
	if ( g_ObserverMan.IsMyObserver() )
		return;

	SetHelpMsg( eHELPMSG_GETCARD );
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_THANKSELECT, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_STOP, TRUE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_STOP, FALSE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_REGIST, FALSE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_THANK, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_PASS, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_DUMPCARD, FALSE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_DUMPCARD, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_GETCARD, TRUE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_GETCARD, TRUE);
	if( m_nTurnCnt == g_PLAYER(0)->ServPNum )
	{
		int nCardSum = 0;
		int nType = 0;

		if( (nType = CheckStopOK( nCardSum )) && m_nTurnIndex == 0 )
		{
			if ( nType == 1 ) // 7포카드
			{
				m_pGameButtonHA->GetGraphicBtn(CGameButtonHA::BTN_STOP)->SetSprite( &m_pGameButtonHA->m_btnSprAll, CARD_JOKER1/4 );
			}
			else if ( nType == 2 )// 대빵
			{
				m_pGameButtonHA->GetGraphicBtn(CGameButtonHA::BTN_STOP)->SetSprite( &m_pGameButtonHA->m_btnSprAll, 8/4 );
			}
			else if ( nType == 3 ) // 소빵
			{
				m_pGameButtonHA->GetGraphicBtn(CGameButtonHA::BTN_STOP)->SetSprite( &m_pGameButtonHA->m_btnSprAll, 44/4 );
			}
			else // 등록 후 스톱, 스톱룰
			{
				m_pGameButtonHA->GetGraphicBtn(CGameButtonHA::BTN_STOP)->SetSprite( &m_pGameButtonHA->m_btnSprAll, 40/4 );
			}

			m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_STOP, TRUE);
			SetHelpMsg( eHELPMSG_STOPENABLE );
		}
	}
}

void CPlayScr::SetDumpCardButton()
{
	if ( m_bGameOver )
	{
		SetNullButton();
		return;
	}
	// ### [ 관전기능 ] ###
	if ( g_ObserverMan.IsMyObserver() )
		return;

	SetHelpMsg( eHELPMSG_SELECTDUMPCARD );
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_THANKSELECT, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_STOP, TRUE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_STOP, FALSE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_REGIST, FALSE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_THANK, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_PASS, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_GETCARD, FALSE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_GETCARD, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_DUMPCARD, TRUE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_DUMPCARD, TRUE);

	if ( g_PLAYER(0)->GetCardCount() == 1 )
	{
		char cCardIndex = m_PlayerCard[0][0];
		if( ( (g_RI.FormKind == hoola::eFORM_KIND_JOKER) && cCardIndex >= 0 && cCardIndex <= CARD_JOKER2 ) 
			|| ( g_RI.FormKind == hoola::eFORM_KIND_NORMAL && cCardIndex >= 0 && cCardIndex <= 51 ) )
		{
			CCL_DISCARD msg;
			msg.Set( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, cCardIndex );
			NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			SetNullButton();
			ResetLimitTime();

			m_nDiscardTest = cCardIndex;
		}
	}
}

void CPlayScr::SetRegCardButton()
{
	// ### [ 관전기능 ] ###
	if ( g_ObserverMan.IsMyObserver() )
		return;

	if ( m_nUpCardType == 3 )
	{
		SetHelpMsg( eHELPMSG_SELECTREGPOS );
	}
	else
	{
		SetHelpMsg( eHELPMSG_REGISTENABLE );
	}
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_THANKSELECT, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_STOP, TRUE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_STOP, FALSE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_REGIST, TRUE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_THANK, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_PASS, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_GETCARD, FALSE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_GETCARD, FALSE);
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_DUMPCARD, TRUE);
	m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_DUMPCARD, TRUE);
}


DUALBYTEDQ IsSerial( BYTE *pData, int nMaxIndex )
{
	int nStart = -1;
	int nEnd = -1;
	BYTE *pData1 = new BYTE[nMaxIndex*2];
	CopyMemory( pData1, pData, sizeof(BYTE) * nMaxIndex );
	CopyMemory( pData1 + nMaxIndex, pData, sizeof(BYTE) * nMaxIndex );

	int nMax = 0;
	int nResStart = -1;
	int nResEnd = -1;
	int nCount = 0;

	DUALBYTEDQ dqRet;

	for ( int i = nMaxIndex; i < nMaxIndex * 2; i++ )
	{
		if ( pData1[i] )
		{
			nResStart = i;
			nResEnd = i;
			for ( int j = i - 1; j >= 0; j-- )
			{
				if ( pData1[j] )
				{
					nResStart = j;
				}
				else
				{
					break;
				}
			}

			for ( int j = i + 1; j < nMaxIndex * 2; j++ )
			{
				if ( pData1[j] )
				{
					nResEnd = j;
				}
				else
				{
					break;
				}
			}

			if ( (nResEnd - nResStart ) + 1 >= 3 )
			{
				BYTEDQ dqSer;
				dqSer.clear();
				for ( int k = nResStart; k <= nResEnd; k++ )
				{
					dqSer.push_back( k % 13 );
					pData1[k] = 0;
					if ( k < nMaxIndex )
						pData1[k+nMaxIndex] = 0;
				}
				dqRet.push_back( dqSer );
			}
		}
	}
	delete pData1;

	return dqRet;
}


DUALBYTEDQ IsSerialJoker( BYTE *pData, int nMaxIndex, int nJoker1, int nJoker2, int nCardNum )
{
	int nStart = -1;
	int nEnd = -1;
	BYTE *pData1 = new BYTE[nMaxIndex*2];
	BYTE *pData2 = new BYTE[nMaxIndex*2];
	CopyMemory( pData1, pData, sizeof(BYTE) * nMaxIndex );
	CopyMemory( pData1 + nMaxIndex, pData, sizeof(BYTE) * nMaxIndex );
	CopyMemory( pData2, pData1, nMaxIndex * 2 );

	int nMax = 0;
	int nResStart = -1;
	int nResEnd = -1;
	int nCount = 0;

	DUALBYTEDQ dqRet;

	if ( (nJoker1 && !nJoker2) || (!nJoker1 && nJoker2) )
	{

		for ( int nLoop = 0; nLoop < nMaxIndex; nLoop++ )
		{
			CopyMemory( pData1, pData2, nMaxIndex * 2 );
			if ( !pData1[nLoop] )
			{
				pData1[nLoop] = 1;
				pData1[nLoop + nMaxIndex] = 1;
			}
			else
			{
				continue;
			}

			for ( int i = nMaxIndex; i < nMaxIndex * 2; i++ )
			{
				if ( pData1[i] )
				{
					nResStart = i;
					nResEnd = i;
					for ( int j = i - 1; j >= 0; j-- )
					{
						if ( pData1[j] )
						{
							nResStart = j;
						}
						else
						{
							break;
						}
					}

					for ( int j = i + 1; j < nMaxIndex * 2; j++ )
					{
						if ( pData1[j] )
						{
							nResEnd = j;
						}
						else
						{
							break;
						}
					}

					if ( (nResEnd - nResStart ) + 1 >= 3 )
					{
						BYTEDQ dqSer;
						dqSer.clear();
						for ( int k = nResStart; k <= nResEnd; k++ )
						{
							if ( pData2[k] )
							{
								dqSer.push_back( k % 13 );
							}
							else
							{
								if ( nJoker1 )
									dqSer.push_back( 60 + (k%13) );
								else if ( nJoker2 )
									dqSer.push_back( 80 + (k%13) );
							}
							pData1[k] = 0;
							if ( k < nMaxIndex )
								pData1[k+nMaxIndex] = 0;
						}

						if ( !dqRet.empty() )
						{
							if ( dqRet[0].size() < dqSer.size() )
							{
								dqRet.clear();
								dqRet.push_back( dqSer );
							}
						}
						else
						{
							dqRet.push_back( dqSer );
						}
					}
				}
			}
		}
		
		delete pData1, pData2;

		return dqRet;
	}
	else if ( nJoker1 && nJoker2 )
	{
		for ( int nLoop = 0; nLoop < nMaxIndex; nLoop++ )
		{
			CopyMemory( pData1, pData2, nMaxIndex*2 );
			if ( !pData1[nLoop] )
			{
				pData1[nLoop] = 1;
				pData1[nLoop+nMaxIndex] = 1;
				for ( int nRepeat = nLoop + 1; nRepeat < nMaxIndex; nRepeat++ )
				{
					if ( !pData1[nRepeat] )
					{
						pData1[nRepeat] = 1;
						pData1[nRepeat+nMaxIndex] = 1;
						break;
					}
					else
					{
						continue;
					}
				}
			}
			else
			{
				continue;
			}

			for ( int i = nMaxIndex; i < nMaxIndex * 2; i++ )
			{
				if ( pData1[i] )
				{
					nResStart = i;
					nResEnd = i;
					for ( int j = i - 1; j >= 0; j-- )
					{
						if ( pData1[j] )
						{
							nResStart = j;
						}
						else
						{
							break;
						}
					}

					for ( int j = i + 1; j < nMaxIndex * 2; j++ )
					{
						if ( pData1[j] )
						{
							nResEnd = j;
						}
						else
						{
							break;
						}
					}

					if ( (nResEnd - nResStart ) + 1 >= 3 )
					{
						BYTEDQ dqSer;
						dqSer.clear();
						BOOL bFirstInsert = FALSE;
						for ( int k = nResStart; k <= nResEnd; k++ )
						{
							if ( pData2[k] )
							{
								dqSer.push_back( k % 13 );
							}
							else
							{
								if ( !bFirstInsert )
								{
									dqSer.push_back( 60 + (k%13) );
									bFirstInsert = TRUE;
								}
								else
								{
									dqSer.push_back( 80 + (k%13) );
								}
							}
							pData1[k] = 0;
							if ( k < nMaxIndex )
								pData1[k+nMaxIndex] = 0;
						}

						if ( !dqRet.empty() )
						{
							if ( nCardNum != -1 )
							{
								if ( std::count( dqSer.begin(), dqSer.end(), nCardNum ) == 1 )
								{
									int nCount = 0;
									for ( int i = 0; i < dqSer.size(); i++ )
									{
										if ( dqSer[i] < CARD_JOKER1 )
											nCount++;
									}

									if ( nCount >= 2 )
									{
										if ( dqRet[0].size() < dqSer.size() )
										{
											dqRet.clear();
											dqRet.push_back( dqSer );
										}
									}
								}
							}
							else
							{
								if ( dqRet[0].size() < dqSer.size() )
								{
									dqRet.clear();
									dqRet.push_back( dqSer );
								}
							}
						}
						else
						{
							if ( nCardNum != -1 )
							{
								int nCount = 0;
								for ( int i = 0; i < dqSer.size(); i++ )
								{
									if ( dqSer[i] < CARD_JOKER1 )
										nCount++;
								}

								if ( nCount >= 2 )
								{
									if ( std::count( dqSer.begin(), dqSer.end(), nCardNum ) == 1 )
									{
										dqRet.push_back( dqSer );
									}
								}
							}
							else
							{
								dqRet.push_back( dqSer );
							}
						}
					}
				}
			}
		}
	}
	delete pData1, pData2;

	return dqRet;
}


DUALBYTEDQ IsSerialJoker( int *pData, int nMaxIndex, int nJoker, int nCardIndex )
{
	int *pData1 = new int[nMaxIndex*2];
	CopyMemory( pData1, pData, sizeof(int) * nMaxIndex );
	CopyMemory( pData1+nMaxIndex, pData, sizeof(int) * nMaxIndex );

	int nIndex = nCardIndex % 13;

	DUALBYTEDQ dqRet;
	BYTEDQ dqSer;

	CString str;

	if ( nIndex < 2 )
	{
		if ( pData1[nIndex - 2 + 13] )
		{
			if ( nIndex-1 < 0 )
				nIndex+= 13;

			if ( nJoker == CARD_JOKER1 )
				dqSer.push_back( 60 + nIndex-1 );
			else if ( nJoker == CARD_JOKER2 )
				dqSer.push_back( 80 + nIndex-1 );

			dqSer.push_back( nCardIndex );
		}
		else if ( pData1[nIndex + 2] ) 
		{
			if ( nJoker == CARD_JOKER1 )
				dqSer.push_back( 60 + nIndex+1 );
			else if ( nJoker == CARD_JOKER2 )
				dqSer.push_back( 80 + nIndex+1 );

			dqSer.push_back( nCardIndex );
		}
	}
	else if ( nIndex >= 2 && nIndex <= 11 )
	{
		if ( pData1[nIndex - 2] )
		{
			if ( nJoker == CARD_JOKER1 )
				dqSer.push_back( 60 + nIndex-1 );
			else if ( nJoker == CARD_JOKER2 )
				dqSer.push_back( 80 + nIndex-1 );
			
			dqSer.push_back( nCardIndex );
		}
		else if ( pData1[nIndex + 2] ) 
		{
			if ( nJoker == CARD_JOKER1 )
				dqSer.push_back( 60 + nIndex+1 );
			else if ( nJoker == CARD_JOKER2 )
				dqSer.push_back( 80 + nIndex+1 );
			
			dqSer.push_back( nCardIndex );
		}
	}
	else
	{
		if ( pData1[nIndex - 2] )
		{
			if ( nJoker == CARD_JOKER1 )
				dqSer.push_back( 60 + nIndex-1 );
			else if ( nJoker == CARD_JOKER2 )
				dqSer.push_back( 80 + nIndex-1 );
			
			dqSer.push_back( nCardIndex );
		}
		else if ( pData1[nIndex + 2 - 13] ) 
		{
			if ( nIndex+1 >= 13 )
				nIndex -= 13;

			if ( nJoker == CARD_JOKER1 )
				dqSer.push_back( 60 + nIndex+1 );
			else if ( nJoker == CARD_JOKER2 )
				dqSer.push_back( 80 + nIndex+1 );
			
			dqSer.push_back( nCardIndex );
		}
	}

	delete pData1;

	dqRet.push_back(dqSer);

	return dqRet;
}

DUALBYTEDQ CPlayScr::FindSerialValue( BYTEDQ& dqSeq, BOOL bIgnoreJoker, int nCardNum )
{
	DUALBYTEDQ dqRet;
	if ( dqSeq.size() <= 2 )
	{
		dqRet.clear();
		return dqRet;
	}

	BYTEDQ dqTemp, dqRet1, dqTemp1;
	dqTemp = dqSeq;
	std::sort( dqTemp.begin(), dqTemp.end() );

	BYTE Data[MAX_CARD_FACE][MAX_CARD_VALUE];
	BYTE Data1[MAX_CARD_FACE][MAX_CARD_VALUE];
	ZeroMemory( Data, sizeof(BYTE) * MAX_CARD_VALUE * MAX_CARD_FACE);
	ZeroMemory( Data1, sizeof(BYTE) * MAX_CARD_VALUE * MAX_CARD_FACE);

	for ( int i = 0; i < dqTemp.size(); i++ )
	{
		//dqTemp는 현재 가지고 있는 카드덱이다.
		if ( dqTemp[i] < 13 )
		{
			Data[0][dqTemp[i]%13] = 1;
			Data1[0][dqTemp[i]%13] = dqTemp[i];
			//dqTemp1에 숫자값만 집어 넣는다.
			dqTemp1.push_back( dqTemp[i]%13 );
		}
		else if ( dqTemp[i] >= 13 && dqTemp[i] < 26)
		{
			Data[1][dqTemp[i]%13] = 1;
			Data1[1][dqTemp[i]%13] = dqTemp[i];
			dqTemp1.push_back( dqTemp[i]%13 );
		}
		else if ( dqTemp[i] >= 26 && dqTemp[i] < 39)
		{
			Data[2][dqTemp[i]%13] = 1;
			Data1[2][dqTemp[i]%13] = dqTemp[i];
			dqTemp1.push_back( dqTemp[i]%13 );
		}
		else if ( dqTemp[i] >= 39 && dqTemp[i] < CARD_JOKER1)
		{
			Data[3][dqTemp[i]%13] = 1;
			Data1[3][dqTemp[i]%13] = dqTemp[i];
			dqTemp1.push_back( dqTemp[i]%13 );
		}
	}

	DUALBYTEDQ dqResult;
	for ( int k = 0; k < 4; k++ )
	{
		BOOL bJoker2 = FALSE;
		if ( !bIgnoreJoker && (g_RI.FormKind == hoola::eFORM_KIND_JOKER) )
		{
			//FormKind 2는 조커 3은 매직
			//매직은 현재 사용중이 아니다
			int nJoker1 = 0;
			int nJoker2 = 0;

			if ( std::count( dqSeq.begin(), dqSeq.end(), CARD_JOKER1 ) == 1 )
			{
				//조커 카드를 찾아서 변수에 해당 값을 넣는다.
				nJoker1 = CARD_JOKER1;
			}
			else 
			{
				for ( int i = 0; i < dqSeq.size(); i++ )
				{
					//60~120 사이의 값은 조커로 인식한다.
					if ( dqSeq[i] >= 60 && dqSeq[i] < 120 )
					{						
						nJoker1 = CARD_JOKER1;
					}
				}
			}

			if ( std::count( dqSeq.begin(), dqSeq.end(), CARD_JOKER2 ) == 1 )
			{
				nJoker2 = CARD_JOKER2;
			}
			else
			{
				for ( int i = 0; i < dqSeq.size(); i++ )
				{
					//120~180 사이의 값은 조커로 인식한다.
					if ( dqSeq[i] >= 120 && dqSeq[i] < 180 )
					{						
						nJoker2 = CARD_JOKER2;
					}
				}
			}
			
			if ( nJoker1 || nJoker2 )
			{
				//조커 카드가 하나라도 있다면
				if ( nJoker1 && nJoker2 )
				{
					//조커 카드가 두개 있다면
					for ( int i = 0; i < 13; i++ )
					{
						if ( std::count( dqTemp1.begin(), dqTemp1.end(), i ) >= 2 )
						{
							return dqRet;
						}
					}
					dqResult = IsSerialJoker( Data[k], 13, nJoker1, nJoker2, nCardNum );
				}
				else
				{
					if ( dqSeq.size() == 2 )
					{
						int nJoker = nJoker1 ? nJoker1 : nJoker2;
						int nIndex = -1;
					
						BYTEDQ::iterator Iter;
						for ( Iter = dqSeq.begin(); Iter != dqSeq.end(); Iter++)
						{
							if ( *Iter < CARD_JOKER1 && (*Iter/13) == k )
							{
								dqResult = IsSerialJoker( m_CenterCard[k], 13, nJoker, *Iter );
								bJoker2 = TRUE;
								break;
							}
						}

						if ( dqResult.size() == 0 )
							dqResult = IsSerialJoker( Data[k], 13, nJoker1, nJoker2, nCardNum );
					}
					else
					{
						dqResult = IsSerialJoker( Data[k], 13, nJoker1, nJoker2, nCardNum );
					}
				}
			}
			else
			{
				dqResult = IsSerial( Data[k], 13 );
			}
		}
		else
		{
			dqResult = IsSerial( Data[k], 13 );
		}

		for ( int i = 0; i < dqResult.size(); i++ )
		{
			//일단 값 넣어두자..... 이뭐
//			dqFixedResult.push_back(dqResult[i]);

			//dqResult는 각 무늬 별 묶음 조합값을 가지고 있다.
			for ( int j = 0; j < dqResult[i].size(); j++ )
			{
				//각각의 값에 대하여
				int nPos = dqResult[i][j];
				
				if ( nPos >= 60 && nPos < 80 )
				{
					int nPos1 = nPos - 60;
					dqRet1.push_back( 60 + k * 13 + nPos1 ); 

					BYTEDQ::iterator Iter;
					Iter = std::find( dqSeq.begin(), dqSeq.end(), CARD_JOKER1 );
					if ( Iter != dqSeq.end() )
					{
						dqSeq.erase( Iter );
					}
				}
				else if ( nPos >= 80 && nPos < 100 )
				{
					int nPos1 = nPos - 80;
					dqRet1.push_back( 120 + k * 13 + nPos1 );

					BYTEDQ::iterator Iter;
					Iter = std::find( dqSeq.begin(), dqSeq.end(), CARD_JOKER2 );
					if ( Iter != dqSeq.end() )
					{
						dqSeq.erase( Iter );
					}
				}
				else
				{
					if ( bJoker2 )
					{
						dqRet1.push_back( nPos );
					}
					else
					{
						if ( Data[k][nPos] )
						{
							dqRet1.push_back( Data1[k][nPos] );
						}
					}					
				}
			}
			static int duplicateCheckCnt = 0;
			if ( duplicateCheckCnt != i)
			{
				if ( dqRet1.size() > 0 )
				{
					dqRet.push_back( dqRet1 );
					dqRet1.clear();
				}
				duplicateCheckCnt = i;
			}
		}

		if ( !dqRet1.empty() )
		{
			std::sort( dqRet1.begin(), dqRet1.end() );
			dqRet.push_back( dqRet1 );
		}
		dqRet1.clear();
	}

// 	if (dqFixedResult.size() > dqRet.size() )
// 	{
// 		return dqFixedResult;
// 	}

// 	//3보다 작은값에서 조커만 선택되는것
// 	if ( dqRet.size() < 3 )
// 	{
// 		BYTEDQ dq;
// 		dq.clear();
// 
// 		for( int i = 0 ; i < dqRet.size() ; i++ )
// 		{//조커카드만 골라보내기
// 			if (dqRet[i] > 53)
// 			{
// 				dq.push_back(dqRet[i]);
// 			}			
// 		}
// 		return dq;
// 	}

	return dqRet;
}

void CPlayScr::MakeTestCardDeck()
{
	for ( int i = 0 ; i < MAX_CARDCOUNT ; i++ )
	{
		m_PlayerCard[0][i] = -1;
	}
	ZeroMemory(m_CenterCard, sizeof(int)*13*4);
	ZeroMemory(m_SameNumCard, sizeof(int)*13*4);
//	RegisterFlag[0] = true;


// 	m_PlayerCard[0][0] = 1;
// 	m_PlayerCard[0][1] = 9;
// 	m_PlayerCard[0][2] = 15;
// 	m_PlayerCard[0][3] = 17;
// 	m_PlayerCard[0][4] = 40;
//  m_PlayerCard[0][5] = 46;
//  m_PlayerCard[0][6] = 24;
//  m_PlayerCard[0][7] = 37;
// 
// 
//  m_CenterCard[0][6] = 1;
// 	m_CenterCard[0][7] = 1;
// 	m_CenterCard[0][8] = 1;
// 
//  m_SameNumCard[9][1] = 1;
// 	m_SameNumCard[9][2] = 1;
// 	m_SameNumCard[9][3] = 1;



// 	m_PlayerCard[0][0] = 6;
// 	m_PlayerCard[0][1] = 19;
// 	m_PlayerCard[0][2] = 14;
// 	m_PlayerCard[0][3] = 23;
// 	m_PlayerCard[0][4] = 12;
// 	m_PlayerCard[0][5] = 40;
// 	m_PlayerCard[0][6] = 47;

// 	m_PlayerCard[0][0] = 13;
// 	m_PlayerCard[0][1] = 39;
// 	m_PlayerCard[0][2] = 2;
// 	m_PlayerCard[0][3] = 18;
// 	m_PlayerCard[0][4] = 19;
// 	m_PlayerCard[0][5] = 32;
// 	m_PlayerCard[0][6] = 20;
// 	m_PlayerCard[0][7] = 46;
//	g_PLAYER(0)->m_bRegist = TRUE;

// 	m_CenterCard[2][6] = 1;
// 	m_CenterCard[3][6] = 1;
// 
// 	m_SameNumCard[7][0] = 2;
// 	m_SameNumCard[7][1] = 1;
// 	m_SameNumCard[7][2] = 1;
// 	m_SameNumCard[7][3] = 3;
// 
// 	m_PlayerCard[0][0] = 39;
// 	m_PlayerCard[0][1] = 14;
// 	m_PlayerCard[0][2] = 42;
// 	m_PlayerCard[0][3] = 4;
// 	m_PlayerCard[0][4] = 18;
// 	m_PlayerCard[0][5] = 46;
// 	m_PlayerCard[0][6] = 8;

// 	m_PlayerCard[0][0] = 26;
// 	m_PlayerCard[0][1] = 27;
// 	m_PlayerCard[0][2] = 15;
// 	m_PlayerCard[0][3] = 41;
// 	m_PlayerCard[0][4] = 52;
// 	m_PlayerCard[0][5] = 53;
// 	m_PlayerCard[0][6] = 2;

// 	m_CenterCard[0][5] = 1;
// 	m_CenterCard[0][6] = 1;
// 	m_CenterCard[1][6] = 1;
// 
// 	m_SameNumCard[4][0] = 1;
// 	m_SameNumCard[4][2] = 1;
// 	m_SameNumCard[4][3] = 1;
// 
// 	m_PlayerCard[0][0] = 15;
// 	m_PlayerCard[0][1] = 3;
// 	m_PlayerCard[0][2] = 42;
// 	m_PlayerCard[0][3] = 20;
// 	m_PlayerCard[0][4] = 45;

// 	m_CenterCard[0][6] = 1;
// 	m_CenterCard[1][6] = 1;
// 	m_CenterCard[2][6] = 1;
// 	m_CenterCard[3][6] = 1;
// 
// 	m_SameNumCard[5][0] = 1;
// 	m_SameNumCard[5][2] = 1;
// 	m_SameNumCard[5][3] = 1;

// 	m_PlayerCard[0][0] = 15;
// 	m_PlayerCard[0][1] = 4;
// 	m_PlayerCard[0][2] = 17;
// 	m_PlayerCard[0][3] = 46;
// 	m_PlayerCard[0][4] = 47;
// 	m_PlayerCard[0][5] = 36;
// 	m_PlayerCard[0][6] = 52;
// 	m_PlayerCard[0][7] = 37;

// 	m_PlayerCard[0][0] = 1;
// 	m_PlayerCard[0][1] = 27;
// 	m_PlayerCard[0][2] = 40;
// 	m_PlayerCard[0][3] = 28;
// 	m_PlayerCard[0][4] = 29;
// 	m_PlayerCard[0][5] = 19;
// 	m_PlayerCard[0][6] = 23;


	m_CenterCard[1][6] = 1;
	m_CenterCard[1][7] = 1;

	m_CenterCard[2][6] = 1;
	m_CenterCard[2][7] = 1;
	m_CenterCard[2][8] = 1;
	m_CenterCard[2][9] = 1;

	m_CenterCard[3][6] = 1;

	g_PLAYER(0)->m_bRegist = TRUE;

	m_PlayerCard[0][0] = 39;
	m_PlayerCard[0][1] = 27;
	m_PlayerCard[0][2] = 17;
	m_PlayerCard[0][3] = 7;
	m_PlayerCard[0][4] = 53;
}

void CPlayScr::MakeFakeSet()
{
	MakeTestCardDeck();
//	CheckThank(32);

}

BYTEDQ CPlayScr::Find7CardValue( BYTEDQ& dqSeq )
{
	BYTEDQ dqResult;
	dqResult.clear();

	if (dqSeq.empty())
	{
		return dqResult;
	}	
	for ( int i = 0 ; i < dqSeq.size() ; i++ )
	{
		if ( IsSevenCard(dqSeq[i]) )
		{
			dqResult.push_back(dqSeq[i]);
		}
	}

	return dqResult;
}
// 같은 카드 찾기
DUALBYTEDQ CPlayScr::FindSameValue( BYTEDQ& dqSeq, BOOL bIgnoreJoker )
{
	DUALBYTEDQ dqRet;
	if ( dqSeq.size() < 3 )
	{
		dqRet.clear();
		return dqRet;
	}

	BYTEDQ dqTemp, dqTemp1, dqRet1;
	int nJoker1 = 0;
	int nJoker2 = 0;

	for ( int i = 0; i < dqSeq.size(); i++ )
	{
		if ( dqSeq[i] == CARD_JOKER1 )
		{
			nJoker1 = CARD_JOKER1;
		}
		else if ( dqSeq[i] == CARD_JOKER2 )
		{
			nJoker2 = CARD_JOKER2;
		}
		else
		{
			dqTemp.push_back( dqSeq[i] % 13 );
		}
	}

	if ( bIgnoreJoker || (nJoker1 == 0 && nJoker2 == 0) )
	{
		for ( int i = 0; i < 13; i++ )
		{
			if ( std::count( dqTemp.begin(), dqTemp.end(), i ) >= 3 )
			{
				dqTemp1.clear();
				for ( int j = 0; j < dqSeq.size(); j++ )
				{
					if ( (dqSeq[j] % 13) == i && ( dqSeq[j] != CARD_JOKER1 && dqSeq[j] != CARD_JOKER2) )
					{
						dqTemp1.push_back( dqSeq[j] );
					}
				}
				dqRet.push_back( dqTemp1 );
			}
		}
	}
	else
	{
		for ( int i = 0; i < 13; i++ )
		{
			if ( (nJoker1 && !nJoker2) || (!nJoker1 && nJoker2) )
			{
				if ( std::count( dqTemp.begin(), dqTemp.end(), i ) >= 2 )
				{
					dqTemp1.clear();
					for ( int j = 0; j < dqSeq.size(); j++ )
					{
						if ( dqSeq[j] == CARD_JOKER1 )
						{
							dqTemp1.push_back( 200 + i );
						}
						else if ( dqSeq[j] == CARD_JOKER2 )
						{
							dqTemp1.push_back( 220 + i );
						}
						else 
						{
							if ( (dqSeq[j] % 13) == i )
							{
								dqTemp1.push_back( dqSeq[j] );
							}
						}
					}
					if ( dqTemp1.size() >= 3 )
					{
						if ( dqRet.empty() )
						{
							std::sort( dqTemp1.begin(), dqTemp1.end() );
							dqRet.push_back( dqTemp1 );
						}
						else
						{
							if ( dqRet[0].size() < dqTemp1.size() )
							{
								std::sort( dqTemp1.begin(), dqTemp1.end() );
								dqRet.clear();
								dqRet.push_back( dqTemp1 );
							}
						}
					}
				}
			}
			else if ( nJoker1 && nJoker2 )
			{
				if ( std::count( dqTemp.begin(), dqTemp.end(), i ) >= 1 )
				{
					dqTemp1.clear();
					for ( int j = 0; j < dqSeq.size(); j++ )
					{
						if ( dqSeq[j] == CARD_JOKER1 )
						{
							dqTemp1.push_back( 200 + i );
						}
						else if ( dqSeq[j] == CARD_JOKER2 )
						{
							dqTemp1.push_back( 220 + i );
						}
						else if ( (dqSeq[j] % 13) == i )
						{
							dqTemp1.push_back( dqSeq[j] );
						}

					}
					if ( dqTemp1.size() >= 4 )
					{
						if ( dqRet.empty() )
						{
							std::sort( dqTemp1.begin(), dqTemp1.end() );
							dqRet.push_back( dqTemp1 );
						}
						else
						{
							if ( dqRet[0].size() < dqTemp1.size() )
							{
								std::sort( dqTemp1.begin(), dqTemp1.end() );
								dqRet.clear();
								dqRet.push_back( dqTemp1 );
							}
						}
					}
				}
			}
		}
	}

	return dqRet;
}


// 조건부 데이터 삭제
void CPlayScr::RemoveElement( BYTEDQ& dqData, BYTEDQ& dqWillBeRemoved )
{
	BYTEDQ::iterator Iter;
	for ( int i = 0; i < dqWillBeRemoved.size(); i++ )
	{
		BYTE byCardNum = dqWillBeRemoved[i];
		if ( (byCardNum >= 60 && byCardNum < 120) || (byCardNum >= 200 && byCardNum < 213) )
		{
			byCardNum = CARD_JOKER1;
		}
		else if ( (byCardNum >= 120 && byCardNum < 180) || (byCardNum >= 220 && byCardNum < 233) )
		{
			byCardNum = CARD_JOKER2;
		}

		Iter = std::find( dqData.begin(), dqData.end(), byCardNum );
		if ( Iter != dqData.end() )
		{
			dqData.erase( Iter );
		}
	}
}


// 붙이기 가능여부 
void CPlayScr::CheckAttach( BYTEDQ& dqData,					// 큐 데이터
						   DUALBYTEDQ& dqSerial,			// 묶음 큐
						   DUALBYTEDQ& dqSame,				// 낱장 큐
						   BYTEDQ& dqAttachA,				// 묶음쪽에 붙은 큐
						   BYTEDQ& dqAttachB,				// 낱장쪽에 붙은 큐
						   BYTEDQ& dqSeven,					// 7카트 큐
						   BOOL bIgnoreJoker )							// 조커를 무시할 것인지?
{
	//바닥에 깔린 카드에 붙이기가 가능한지 판별

	int CenterCard[MAX_CARD_FACE][MAX_CARD_VALUE];		// 바닥 낱장 조합
	int SameCard[MAX_CARD_VALUE][MAX_CARD_FACE];		// 바닥 묶음 조합

	CopyMemory( CenterCard, m_CenterCard, sizeof(int) * MAX_CARD_FACE * MAX_CARD_VALUE );
	CopyMemory( SameCard, m_SameNumCard, sizeof(int) * MAX_CARD_FACE * MAX_CARD_VALUE );

	BOOL bRegisted = FALSE;

	//////////////////////////////////////////////////////////////////////////
	//			낱장 큐와 묶음 큐에있는 정보를 토대로 바닥패를 세팅
	//////////////////////////////////////////////////////////////////////////
	if ( !dqSerial.empty() )	
	{
		//묶음조합 데이터가 있을경우
		for ( int i = 0; i < dqSerial.size(); i++ )
		{
			// 묶음조합 큐가 이중조합으로 되어있음
			// 각 조합에 해당하는 값을 꺼낸다.

			BYTEDQ dqTemp = dqSerial[i];		// i 번째 조합을 꺼내서 임시 저장한다
			for ( int j = 0; j < dqTemp.size(); j++ )
			{
				if ( dqTemp[j] < CARD_JOKER1 )
				{
					int nFace = dqTemp[j] / 13;				
					int nNum  = dqTemp[j] % 13;

					CenterCard[nFace][nNum] = 1;				// 묶음 카드정보를 세팅
				}
			}
			dqTemp.clear();
		}
		bRegisted = TRUE;									// 등록 상황을 참으로 설정
	}
	if ( !dqSame.empty() )
	{
		// 낱장 조합 카드 정보가 있을 경우
		for ( int i = 0; i < dqSame.size(); i++ )
		{
			//낱장조합 큐 역시 이중조합
			BYTEDQ dqTemp = dqSame[i];
			for ( int j = 0; j < dqTemp.size(); j++ )
			{
				if ( dqTemp[j] < CARD_JOKER1 )
				{
					int nFace = dqTemp[j] / 13;
					int nNum  = dqTemp[j] % 13;

					SameCard[nNum][nFace] = 1;				// 묶음 카드정보를 세팅
				}
			}
			dqTemp.clear();
		}
		bRegisted = TRUE;
	}
	//////////////////////////////////////////////////////////////////////////

	BYTEDQ dqRet;
	BYTEDQ::iterator IterErase;

	dqAttachA.clear();
	dqAttachB.clear();
	dqSeven.clear();

	int nJoker1 = 0;
	int nJoker2 = 0;

	int nJokerCardCnt = 0;

	for ( int i = 0; i < MAX_CARDCOUNT; i++ )
	{
		for ( int j = 0; j < dqData.size(); j++ )
		{
			if ( IsJokerCard(dqData[j]) )
			{
				// 조커 카드가 들어올 경우 값을 입력하고 다음 데이터를 받는다.

				if ( dqData[j] == CARD_JOKER1 )
				{
					nJoker1 = CARD_JOKER1;
				}
				if ( dqData[j] == CARD_JOKER2 )
				{
					nJoker2 = CARD_JOKER2;
				}
				nJokerCardCnt++;
				continue;
			}

			int face = dqData[j] / 13;
			int num  = dqData[j] % 13;
			int num1, num2;

			num1 = num - 1; //num1의 값은 카드값의 하나 앞선 번호를 지칭

			if ( num == 0 )
			{
				num1 = 12;
			}

			num2 = num + 1;	//num2의 값은 카드값의 하나 뒤의 번호를 지칭

			if ( num == 12 )
			{
				num2 = 0;
			}

			//BTS대응 코드 수정 조커 사이 한 카드 붙이기
			int nCardValBeforeBefore = 0;
			int nCardValNextNext = 0;

			switch(num)
			{
			case 0:
				{
					nCardValBeforeBefore = 11;
					nCardValNextNext = 2;
				}break;
			case 1:
				{
					nCardValBeforeBefore = 12;
					nCardValNextNext = 3;
				}break;
			case 11:
				{
					nCardValBeforeBefore = 9;
					nCardValNextNext = 0;
				}break;
			case 12:
				{
					nCardValBeforeBefore = 10;
					nCardValNextNext = 1;
				}break;
			default:
				{
					nCardValBeforeBefore = num - 2;
					nCardValNextNext = num + 2;
				}break;
			}



			if ( num == 6 )
			{
				// 7카드 일때
				CenterCard[face][num] = 1;			// 7카드이므로 등록
				dqSeven.push_back( dqData[j] );		// 7카드 큐에 집어 넣는다

				IterErase = std::remove( dqData.begin(), dqData.end(), dqData[j] );	//등록 처리 되었으므로 큐에서 삭제

				if ( IterErase != dqData.end() )
				{
					dqData.erase( IterErase, dqData.end() );
				}
			}
			else
			{
				// 7카드가 아닐때
				if ( (CenterCard[face][num1] >= 1 || CenterCard[face][num2] >= 1) )
				{
					CenterCard[face][num] = 1;

					dqAttachA.push_back( dqData[j] );
					IterErase = std::remove( dqData.begin(), dqData.end(), dqData[j] );
					if ( IterErase != dqData.end() )
						dqData.erase( IterErase, dqData.end() );
				}
				else if ( nJokerCardCnt > 0 )
				{
// 					if (CenterCard[face][nCardValBeforeBefore] >= 1 )
// 					{
// 						if ( nJoker1 )
// 						{
// 							dqAttachA.push_back( CARD_JOKER1 );
// 							IterErase = std::remove( dqData.begin(), dqData.end(), CARD_JOKER1 );
// 							if ( IterErase != dqData.end() )
// 								dqData.erase( IterErase, dqData.end() );
// 
// 							dqAttachA.push_back( dqData[j] );
// 							IterErase = std::remove( dqData.begin(), dqData.end(), dqData[j] );
// 							if ( IterErase != dqData.end() )
// 								dqData.erase( IterErase, dqData.end() );
// 							nJokerCardCnt--;
// 							nJoker1 = 0;
// 						}
// 						if ( nJoker2 )
// 						{
// 							dqAttachA.push_back( CARD_JOKER2 );
// 							IterErase = std::remove( dqData.begin(), dqData.end(), CARD_JOKER2 );
// 							if ( IterErase != dqData.end() )
// 								dqData.erase( IterErase, dqData.end() );
// 
// 							dqAttachA.push_back( dqData[j] );
// 							IterErase = std::remove( dqData.begin(), dqData.end(), dqData[j] );
// 							if ( IterErase != dqData.end() )
// 								dqData.erase( IterErase, dqData.end() );
// 							nJokerCardCnt--;
// 							nJoker2 = 0;
// 						}
// 					}
// 					else if ( CenterCard[face][nCardValNextNext] >= 1 )
// 					{
// 						if ( nJoker1 )
// 						{
// 							
// 							dqAttachA.push_back( dqData[j] );
// 							IterErase = std::remove( dqData.begin(), dqData.end(), dqData[j] );
// 							if ( IterErase != dqData.end() )
// 								dqData.erase( IterErase, dqData.end() );
// 
// 							dqAttachA.push_back( CARD_JOKER1 );
// 							IterErase = std::remove( dqData.begin(), dqData.end(), CARD_JOKER1 );
// 							if ( IterErase != dqData.end() )
// 								dqData.erase( IterErase, dqData.end() );
// 							nJokerCardCnt--;
// 							nJoker1 = 0;
// 						}
// 						if ( nJoker2 )
// 						{
// 							dqAttachA.push_back( dqData[j] );
// 							IterErase = std::remove( dqData.begin(), dqData.end(), dqData[j] );
// 							if ( IterErase != dqData.end() )
// 								dqData.erase( IterErase, dqData.end() );
// 
// 							dqAttachA.push_back( CARD_JOKER2 );
// 							IterErase = std::remove( dqData.begin(), dqData.end(), CARD_JOKER2 );
// 							if ( IterErase != dqData.end() )
// 								dqData.erase( IterErase, dqData.end() );
// 							
// 							nJokerCardCnt--;
// 							nJoker2 = 0;
// 						}
// 					}
					//조커 카운트가 1일때 처리
// 					if (CenterCard[face][nCardValBeforeBefore] >= 1 || CenterCard[face][nCardValNextNext] >= 1)
// 					{
// 						if ( nJoker1 )
// 						{
// 							dqAttachA.push_back( CARD_JOKER1 );
// 							dqAttachA.push_back( dqData[j] );
// 							IterErase = std::remove( dqData.begin(), dqData.end(), dqData[j] );
// 							if ( IterErase != dqData.end() )
// 								dqData.erase( IterErase, dqData.end() );
// 							nJokerCardCnt--;
// 							nJoker1 = 0;
// 						}
// 						if ( nJoker2 )
// 						{
// 							dqAttachA.push_back( CARD_JOKER2 );
// 							dqAttachA.push_back( dqData[j] );
// 							IterErase = std::remove( dqData.begin(), dqData.end(), dqData[j] );
// 							if ( IterErase != dqData.end() )
// 								dqData.erase( IterErase, dqData.end() );
// 							nJokerCardCnt--;
// 							nJoker2 = 0;
// 						}
// 						
// 					}
				}
				else 
				{
					// 매직카드 
					BOOL bMagic = FALSE;
					if ( !bMagic)
					{
						int k;
						for ( k  = 0; k < 4; k++ )
						{
							if ( SameCard[num][k] )
								break;
						}
						if ( k < 4 )
						{
							SameCard[num][face] = 1;

							dqAttachB.push_back( dqData[j] );
							IterErase = std::remove( dqData.begin(), dqData.end(), dqData[j] );
							if ( IterErase != dqData.end() )
								dqData.erase( IterErase, dqData.end() );
						}
					}
				}
			}
		}
	}

	if ( !bIgnoreJoker )
	{
		if ( !dqAttachA.empty() && !dqAttachB.empty() )
		{
			//두쪽 다 데이터가 있을 경우
			dqAttachA.clear();
			dqAttachB.clear();
		}
		else if ( !dqAttachA.empty() && dqAttachB.empty() )
		{
			//A Site만 데이터가 있을경우
			if ( nJoker1 )
			{
				dqAttachA.push_back( CARD_JOKER1 );
			}
			if ( nJoker2 )
			{
				dqAttachA.push_back( CARD_JOKER2 );
			}
		}
		else if ( dqAttachA.empty() && !dqAttachB.empty() )
		{
			//B Site만 데이터가 있을경우
			for ( int i = 0; i < dqAttachB.size(); i++ )
			{
				int num = dqAttachB[i];
				int count = 0;
				for ( int k  = 0 ; k < 4 ; k++ )
				{
					if ( SameCard[num][k] )
						count++;
				}
				if ( nJoker1 && nJoker2 )
				{
					if ( count <= 2 )
					{
						dqAttachB.push_back( CARD_JOKER1 );
						dqAttachB.push_back( CARD_JOKER2 );
					}
					else
					{
						dqAttachA.push_back( CARD_JOKER1 );
						dqAttachA.push_back( CARD_JOKER2 );
					}
				}
				else if ( (nJoker1 && !nJoker2) || (!nJoker1 && nJoker2) )
				{
					if ( count <= 3 )
					{
						if ( nJoker1 )
							dqAttachB.push_back( CARD_JOKER1 );
						else if ( nJoker2 )
							dqAttachB.push_back( CARD_JOKER2 );
					}
					else
					{
						if ( nJoker1 )
							dqAttachA.push_back( CARD_JOKER1 );
						else if ( nJoker2 )
							dqAttachA.push_back( CARD_JOKER2 );
					}
				}
			}
		}
		else
		{
			// 이도저도 아닌경우
			if ( nJoker1 )
			{
				dqAttachA.push_back( CARD_JOKER1 );
			}
			if ( nJoker2 )
			{
				dqAttachA.push_back( CARD_JOKER2 );
			}
		}
	}
}

BOOL CPlayScr::IsValidCardIndex_WITH_Joker(int nCardIndex)
{
	const int nLastCardIndex = CARD_JOKER2;
	if (nCardIndex >= 0 && nCardIndex <= nLastCardIndex)
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CPlayScr::IsValidCardIndex_WITHOUT_Joker(int nCardIndex)
{
	const int nLastCardIndex = 51;
	if (nCardIndex >= 0 && nCardIndex <= nLastCardIndex)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CPlayScr::CheckReg()
{
#ifdef _DEBUG
	CheckJokerCardProblem();
#endif
	//등록 가능한 상황인지 확인
	BYTEDQ dqSeq, dqSeqBak;
	for ( int i = 0; i < MAX_CARDCOUNT; i++ )
	{
		if ( IsValidCardIndex_WITHOUT_Joker(m_PlayerCard[0][i]) ) 
		{
			dqSeq.push_back( m_PlayerCard[0][i] ); 
		}
	}

// 	//◆A, ♥A, ♣A, ◆2, ◆3, ◆4, ◆5 보유 상태에서 자동 등록 선택 시(26, 13, 0, 27, 28, 29, 30)
// 	//◆A, ◆2, ◆3 낱장 조합과 ◆A, ♥A, ♣A 묶음 조합 중 선택 가능
// 	dqSeq.clear();
// 	dqSeq.push_back( 26 );
// 	dqSeq.push_back( 13 );
// 	dqSeq.push_back(  0 );
// 	dqSeq.push_back( 27 );
// 	dqSeq.push_back( 28 );
// 	dqSeq.push_back( 29 );
// 	dqSeq.push_back( 30 );
// 
// 	//◆7, ♥A, ♣A, ◆5, ◆8, ♣8, ♥8 보유 상태에서 자동 등록 선택 시(32, 13, 0, 30, 33, 7, 20)
// 	//◆7 낱장 등록 ◆8, ♣8, ♥8 묶음 등록
// 	dqSeq.clear();
// 	dqSeq.push_back( 32 );
// 	dqSeq.push_back( 13 );
// 	dqSeq.push_back(  0 );
// 	dqSeq.push_back( 30 );
// 	dqSeq.push_back( 33 );
// 	dqSeq.push_back(  7 );
// 	dqSeq.push_back( 20 );
// 
// 	//◆7, ♥A, ♣A, ◆9, ◆8, ♣8, ♥8 보유 상태에서 자동 등록 선택 시 (32, 13, 0, 34, 33, 7, 20)
// 	//◆7, ◆8, ◆9 낱장 조합과 ◆8, ♣8, ♥8 묶음 조합 중 선택 가능
// 	dqSeq.clear();
// 	dqSeq.push_back( 32 );
// 	dqSeq.push_back( 13 );
// 	dqSeq.push_back(  0 );
// 	dqSeq.push_back( 34 );
// 	dqSeq.push_back( 33 );
// 	dqSeq.push_back(  7 );
// 	dqSeq.push_back( 20 );
// 
// 	//♥7, ♣7, ♠7, ♥2, ♣2, ♠2, ◆8 보유 상태에서 7을 3장 선택한 후 등록 버튼 클릭 시(6, 19, 45, 1, 14, 40, 33)
// 	//♥7, ♣7, ♠7 낱장 조합과 ♥7, ♣7, ♠7 묶음 조합 중 선택 가능
// 	dqSeq.clear();
// 	dqSeq.push_back( 6 );
// 	dqSeq.push_back( 19 );
// 	dqSeq.push_back( 45 );
// 	dqSeq.push_back( 1 );
// 	dqSeq.push_back( 14 );
// 	dqSeq.push_back( 40 );
// 	dqSeq.push_back( 33 );


	//기획서 36페이지 3번째 경우(26 27, 28, 29, 3, 4, 5, 44)
// 	dqSeq.clear();
// 	dqSeq.push_back( 28 );
// 	dqSeq.push_back( 29 );
// 	dqSeq.push_back( 30 );
// 	dqSeq.push_back( 31 );
// 	dqSeq.push_back( 3 );
// 	dqSeq.push_back( 4 );
// 	dqSeq.push_back( 5 );
// 	dqSeq.push_back( 44 );

	//기획서 36페이지 4번째 경우
// 	dqSeq.clear();
// 	dqSeq.push_back( 0 );
// 	dqSeq.push_back( 13 );
// 	dqSeq.push_back( 26 );
// 	dqSeq.push_back( 27 );
// 	dqSeq.push_back( 2 );
// 	dqSeq.push_back( 28 );
// 	dqSeq.push_back( 41 );
// 	dqSeq.push_back( 1 );

//BTS대응
// 	dqSeq.clear();
// 	dqSeq.push_back( 26 );
// 	dqSeq.push_back( 29 );
// 	dqSeq.push_back( 30 );
// 	dqSeq.push_back( 31 );
// 	dqSeq.push_back( 45 );
// 	dqSeq.push_back( 37 );
// 	dqSeq.push_back( 38 );
// 	dqSeq.push_back( 17 );

	dqSeqBak = dqSeq;
	m_bPressThank = FALSE;
	m_bRegSelect = FALSE;

	m_cCardInfo.m_dqStraight.clear();	// 낱장 조합
	m_cCardInfo.m_dqTogether.clear();	// 묶음 조합
	m_cCardInfo.m_dqAttachStraight.clear();	// 낱장 쪽에 붙이기
	m_cCardInfo.m_dqAttachTogether.clear();	// 묶음 쪽에 붙이기
	m_cCardInfo.m_dqSeven.clear();		// 7카드
	m_cCardInfo.m_dqSelect.clear();		// 선택된 카드

	m_bClickRegCard = FALSE; // 원래 등록가능 카드가 변경

	m_cClickCardInfo.m_dqStraight.clear(); 
	m_cClickCardInfo.m_dqTogether.clear(); 
	m_cClickCardInfo.m_dqAttachStraight.clear();
	m_cClickCardInfo.m_dqAttachTogether.clear();
	m_cClickCardInfo.m_dqSeven.clear();

	m_nUpCardType = 0;

	//1장 붙이기상황에서 붙일수 있는 카드 선택용 dq
	m_dqAttachableFakeDq.clear();

	if ( dqSeq.empty() )
	{
		//큐에 카드정보가 하나도 없을 경우
		return FALSE;
	}

	m_cCardInfo.m_dqStraight = FindSerialValue( dqSeq );			// 묶음 조합 카드 데큐를 받아온다
	for ( int i = 0; i < m_cCardInfo.m_dqStraight.size(); i++ )
	{
		RemoveElement( dqSeq, m_cCardInfo.m_dqStraight[i] );		// dqSeq에 있는 묶음조합 카드 정보를 삭제 한다
	}
	m_cCardInfo.m_dqTogether  = FindSameValue( dqSeq );				// 낱장 조합 카드 데큐를 받아온다
	for ( int i = 0; i < m_cCardInfo.m_dqTogether.size(); i++ )
	{
		RemoveElement( dqSeq, m_cCardInfo.m_dqTogether[i] );		// dqSeq에 있는 낱장조합 카드 정보를 삭제 한다
	}
	
//	RemoveElement( dqSeq, m_dqSeven );		// dqSeq에 있는 7카드 정보를 삭제 한다

	//묶음조합에 들어갈 카드정보를 받고 자신의 패에 묶음조합에 해당하는 정보를 제거한다.
	//낱장조합을 한번 걸러진 패중 확인한다.
	//이중으로 프로세스가 실행된다. 정리가 필요

	if ( TRUE == g_PLAYER(0)->m_bRegist )
	{
		CheckAttach( dqSeq, m_cCardInfo.m_dqStraight, m_cCardInfo.m_dqTogether, m_cCardInfo.m_dqAttachStraight, m_cCardInfo.m_dqAttachTogether, m_cCardInfo.m_dqSeven );
	}
	m_cCardInfo.m_dqSeven = Find7CardValue( dqSeqBak );

	m_cCardInfo.m_dqUpCard.clear();

	//////////////////////////////////////////////////////////////////////////
	//					대략 이 부분이 카드 등록
	//////////////////////////////////////////////////////////////////////////
	if ( !m_cCardInfo.m_dqStraight.empty() )
	{
		//묶음조함 카드가 있을때
		if ( CheckDupValue( dqSeqBak, m_cCardInfo.m_dqStraight, m_cCardInfo.m_dqTogether, m_cCardInfo.m_dqSelect ) )
		{
			//CheckDupValue함수에서 m_dqSelect 큐에 등록 가능한 카드 조합을 세팅한다.
			
			//등록 상황 플레그 세팅
			m_bRegSelect = TRUE;

			//선택조합에서 어떤걸 정했는지 
			m_nSelectCurrent = 0;

			//선택조합의 사이즈를 세팅
			m_nSelectMax = m_cCardInfo.m_dqSelect.size();
		}
	}

	/*
	1. 먼저 바닥에 깔린 패를 확인하지 않고 자신이 보유한 패중 등록가능한 조합이 있는지 확인한다
	2. 만약 없을경우 바닥에 깔린 패를 확인하고 그중 자신이 보유한 패를 통해 붙일수 있는 조합이 있는지 확인한다.
	*/

	//////////////////////////////////////////////////////////////////////////
	//		선택카드/묶음조합/낱장조합/7카드를 m_dqupCard에 push_back
	//////////////////////////////////////////////////////////////////////////
	for ( int i = 0; i < m_cCardInfo.m_dqSelect.size(); i++ )
	{
		for ( int j = 0; j < m_cCardInfo.m_dqSelect[i].size(); j++ )
		{
			m_cCardInfo.m_dqUpCard.push_back( m_cCardInfo.m_dqSelect[i][j] );
		}
	}
	for ( int i = 0; i < m_cCardInfo.m_dqStraight.size(); i++ )
	{
		for ( int j = 0; j < m_cCardInfo.m_dqStraight[i].size(); j++ )
		{
			m_cCardInfo.m_dqUpCard.push_back( m_cCardInfo.m_dqStraight[i][j] );
		}
	}
	for ( int i = 0; i < m_cCardInfo.m_dqTogether.size(); i++ )
	{
		for ( int j = 0; j < m_cCardInfo.m_dqTogether[i].size(); j++ )
		{
			m_cCardInfo.m_dqUpCard.push_back( m_cCardInfo.m_dqTogether[i][j] );
		}
	}
	for ( int i = 0; i < m_cCardInfo.m_dqSeven.size(); i++ )
	{
		m_cCardInfo.m_dqUpCard.push_back( m_cCardInfo.m_dqSeven[i] );
	}


	for ( int i = 0; i < m_cCardInfo.m_dqAttachStraight.size(); i++ )
	{
		m_cCardInfo.m_dqUpCard.push_back( m_cCardInfo.m_dqAttachStraight[i] );
	}
	for ( int i = 0; i < m_cCardInfo.m_dqAttachTogether.size(); i++ )
	{
		m_cCardInfo.m_dqUpCard.push_back( m_cCardInfo.m_dqAttachTogether[i] );
	}

	m_cCardInfo.m_dqUpCard = MakeUniqueDQ(m_cCardInfo.m_dqUpCard);

	m_dqSelectedCard.clear();



	if ( m_cCardInfo.m_dqUpCard.empty() )
	{
		return FALSE;
	}

	if ( m_cCardInfo.m_dqUpCard.size() == 1)
	{
		//등록 가능한 카드가 하나 일 경우
		char cCardIndex = m_cCardInfo.m_dqUpCard[0];

		if ( std::count( m_cCardInfo.m_dqAttachStraight.begin(), m_cCardInfo.m_dqAttachStraight.end(), cCardIndex ) == 1 )
		{
			if ( CheckAvailableB( cCardIndex ) )
			{
				m_nUpCardType = 3;
				//낱장 묶음쪽에 붙일 카드 선택이 두가지 dq를 통해 진행
				//현재 Click dq를 사용하고 있기 때문에 임시로 데이터를 넣어줌
				m_cClickCardInfo.m_dqAttachStraight = m_cCardInfo.m_dqAttachStraight;

				MakeAttachableDq( cCardIndex );
			}
		}
	}
	else if( m_cCardInfo.m_dqSeven.size() >= 3 )
	{
		m_nUpCardType = 3;
	}


	return TRUE;	
}
BYTEDQ CPlayScr::MakeUniqueDQ(BYTEDQ dq)
{
	BYTEDQ dqResult;
	dqResult.clear();

	//240까지의 숫자를 가질수 있다. 근데 이거 정말 비효율적이니 다른 방법을 찾아보자
	for ( int i = 0 ; i < 240 ; i++ )
	{
		if ( std::count( dq.begin(), dq.end(), i ) > 0 )
		{
			dqResult.push_back((BYTE)i);
		}
	}
	return dqResult;
}

BOOL CheckTogetherJoker( BYTEDQ& dqData )
{
	BYTEDQ dqNonJoker;
	int nJoker1, nJoker2, nJokerCount;
	nJoker1 = nJoker2 = nJokerCount = 0; 
	
	for ( int i = 0; i < dqData.size(); i++ )
	{
		if ( dqData[i] == CARD_JOKER1 )
		{
			nJoker1 = CARD_JOKER1;
			nJokerCount++;
		}
		else if ( dqData[i] == CARD_JOKER2 )
		{
			nJoker2 = CARD_JOKER2;
			nJokerCount++;
		}
		else
		{
			dqNonJoker.push_back( dqData[i] % 13 );
		}
	}

	if ( dqData.size() <= 1 )
	{
		return FALSE;
	}
	if ( dqNonJoker.size() >= 2 )
	{
		for ( int i = 0; i < dqNonJoker.size()-1; i++ )
		{
			if ( dqNonJoker[i] != dqNonJoker[i+1] )
				return FALSE;
		}
	}
	if ( nJoker1 && nJoker2 )
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CPlayScr::CheckReg( BYTEDQ& dqSeq )
{
	// CheckReg() 함수는
	// 멤버변수로 선언된 여러 조합 묶음을 불러와 세팅을 다시함

	BYTEDQ dqSeqBak;
	dqSeqBak = dqSeq;

	m_cClickCardInfo.Reset();
	m_nUpCardType = 0;
	m_bRegSelect = FALSE;
	
	if ( dqSeq.empty() ) return FALSE;

 	BOOL bJoker2 = FALSE;
// 	if ( dqSeq.size() == 2 )
// 	{		
// 		if ( dqSeq[0] >= CARD_JOKER1 && dqSeq[1] < CARD_JOKER1 )
// 		{
// 			bJoker2 = TRUE;
// 		}
// 		else if ( dqSeq[0] < CARD_JOKER1 && dqSeq[1] >= CARD_JOKER1  )
// 		{
// 			bJoker2 = TRUE;
// 		}
// 	}

// 	int nJokerCount = 0;
// 	for ( int i = 0 ; i < dqSeq.size() ; i++ )
// 	{
// 		if( IsJokerCard( dqSeq[i] ) )
// 			nJokerCount++;
// 	}
// 	if ( (nJokerCount == 1) && (dqSeq.size() == 2) )
// 	{
// 		bJoker2 = TRUE;
// 	}
	

	m_cClickCardInfo.m_dqStraight = FindSerialValue( dqSeq, FALSE );
	for ( int i = 0 ; i < m_cClickCardInfo.m_dqStraight.size() ; i++ )
	{
		RemoveElement( dqSeq, m_cClickCardInfo.m_dqStraight[i] );
	}

	if ( bJoker2 == FALSE )
	{
		m_cClickCardInfo.m_dqTogether = FindSameValue( dqSeq, FALSE );
		for ( int i = 0 ; i < m_cClickCardInfo.m_dqTogether.size() ; i++ )
		{
			RemoveElement( dqSeq, m_cClickCardInfo.m_dqTogether[i] );
		}
		if ( TRUE == g_PLAYER(0)->m_bRegist )
		{
			CheckAttach( dqSeq, m_cClickCardInfo.m_dqStraight, m_cClickCardInfo.m_dqTogether, m_cClickCardInfo.m_dqAttachStraight, m_cClickCardInfo.m_dqAttachTogether, m_cClickCardInfo.m_dqSeven, FALSE );
		}		
	}
	m_cClickCardInfo.m_dqSeven = Find7CardValue(dqSeqBak);

	for ( int i = 0; i < m_cClickCardInfo.m_dqStraight.size(); i++ )
	{
		for ( int j = 0; j < m_cClickCardInfo.m_dqStraight[i].size(); j++ )
		{
			m_cClickCardInfo.m_dqUpCard.push_back( m_cClickCardInfo.m_dqStraight[i][j] );
		}
	}
	for ( int i = 0; i < m_cClickCardInfo.m_dqTogether.size(); i++ )
	{
		for ( int j = 0; j < m_cClickCardInfo.m_dqTogether[i].size(); j++ )
		{
			m_cClickCardInfo.m_dqUpCard.push_back( m_cClickCardInfo.m_dqTogether[i][j] );
		}
	}
	for ( int i = 0; i < m_cClickCardInfo.m_dqSeven.size(); i++ )
	{
		m_cClickCardInfo.m_dqUpCard.push_back( m_cClickCardInfo.m_dqSeven[i] );
	}
	if ( m_cClickCardInfo.m_dqUpCard.empty() )
	{
		if ( g_PLAYER(0)->m_bRegist )
		{
			for ( int i = 0; i < m_cClickCardInfo.m_dqAttachStraight.size(); i++ )
			{
				m_cClickCardInfo.m_dqUpCard.push_back( m_cClickCardInfo.m_dqAttachStraight[i] );
			}
			for ( int i = 0; i < m_cClickCardInfo.m_dqAttachTogether.size(); i++ )
			{
				m_cClickCardInfo.m_dqUpCard.push_back( m_cClickCardInfo.m_dqAttachTogether[i] );
			}
		}
	}
	else
	{
		for ( int i = 0; i < m_cClickCardInfo.m_dqAttachStraight.size(); i++ )
		{
			m_cClickCardInfo.m_dqUpCard.push_back( m_cClickCardInfo.m_dqAttachStraight[i] );
		}
		for ( int i = 0; i < m_cClickCardInfo.m_dqAttachTogether.size(); i++ )
		{
			m_cClickCardInfo.m_dqUpCard.push_back( m_cClickCardInfo.m_dqAttachTogether[i] );
		}
	}
	m_cClickCardInfo.m_dqUpCard = MakeUniqueDQ(m_cClickCardInfo.m_dqUpCard);

	if ( m_cClickCardInfo.m_dqUpCard.empty() )
	{
		return FALSE;
	}
	else
	{
		if ( m_cClickCardInfo.m_dqUpCard.size() == 1 )
		{
			char cCI = m_cClickCardInfo.m_dqUpCard[0];
			if ( cCI < CARD_JOKER1 && std::count( m_cClickCardInfo.m_dqAttachStraight.begin(), m_cClickCardInfo.m_dqAttachStraight.end(), cCI ) == 1 )
			{
				if ( CheckAvailableB( cCI ) )
				{
					m_nUpCardType = 3;
				}
			}
		}
		else if ( m_cClickCardInfo.m_dqUpCard.size() == 3 || m_cClickCardInfo.m_dqUpCard.size() == 4 )
		{
			for ( int i = 0; i < m_cClickCardInfo.m_dqUpCard.size(); i++ )
			{
				if ( m_cClickCardInfo.m_dqUpCard[i] % 13 != 6 )
				{
					return TRUE;
				}
			}
			m_nUpCardType = 3;
		}		
		return TRUE;
	}
}


BOOL CPlayScr::CheckDupValue( BYTEDQ& dqData, DUALBYTEDQ& dqSerial, DUALBYTEDQ& dqSame, DUALBYTEDQ& dqResult )
{
	//////////////////////////////////////////////////////////////////////////
	//		낱장 조합과 묶음 조합 중 중복되는 조합이 있는지 확인한다.
	//////////////////////////////////////////////////////////////////////////

	std::deque< int > dqSerialIndex;
	std::deque< int >::iterator IterErase;
	BYTEDQ dqTemp, dqTemp1;

	//dq에 받은 덱의 CardNum만 집어 넣는다
	for ( int i = 0; i < dqData.size(); i++ )
	{		
		dqTemp.push_back( dqData[i] % 13 );
	}

	for ( int i = 0; i < dqSerial.size(); i++ )
	{
		for ( int ii = 0; ii < dqSerial[i].size(); ii++ )
		{
			//묶음조합 카드
			char cCardIndex = dqSerial[i][ii];

			if ( std::count( dqTemp.begin(), dqTemp.end(), ( cCardIndex % 13 ) ) < 3 )
			{
				//같은 카드 인덱스를 받은것이 3개 이하면 루프에서 벗어남
				//묶음조합 카드에서 낱장조합이 될 가능성이 없으면 중복 조합 확인이 필요 없다
				continue;
			}

			BOOL bSame = FALSE;
			for ( int j = 0; j < dqSame.size(); j++ )
			{
				//dqStraight의 카드 데이터와 dqTogether의 카드 정보를 비교한다
				//내부에 있는 카드 정보중 같은것이 있으면 bSame플레그를 참으로 하고 다음으로 넘어간다
				if ( ( dqSame[j][0] % 13) == ( cCardIndex % 13 ) )
				{
					bSame = TRUE;
					break;
				}
			}
				
			if ( !bSame )
			{
				//위의 루프가 끝나고 참인 경우가 없으면
				//묶음조합과 낱장조합에서 중복되는 카드정보를 찾지 못했을 경우
				dqSerialIndex.push_back( i );	//묶음 조합의 인덱스 번호를 저장한다
				BOOL bExist = FALSE;
				for ( int k = 0; k < dqResult.size(); k++ )
				{
					//m_dqSelect에 있는 카드 정보중 같은 카드 인덱스를 가지고 있다면
					//for루프를 벗어나고 다음 조건을 실행시키지 않는다
					//왜?: 전체로직은 묶음조합의 인덱스 번호를 비교하고 있고 cCardIndex의 카드 인덱스와 
					//m_dqSelect의 정보 중 같은게 있을 경우 m_dqSelect에 묶음조합이 포함되어있다고 간주
					if ( ( dqResult[k][0] % 13 ) == ( cCardIndex % 13 ) )
					{
						bExist = TRUE;
						break;
					}
				}
				if ( !bExist )
				{
					//위 루프를 통과했다면 중복되는것이 없다.
					dqTemp1.clear();
					for ( int j = 0; j < dqData.size(); j++ )
					{
						if ( ( dqData[j] % 13 ) == ( cCardIndex % 13 ) )
						{
							dqTemp1.push_back( dqData[j] );
						}
					}
					dqResult.push_back( dqTemp1 );
				}
			}
		}
	}
	// unique는 중복되는 카드 조합을 지운다	
	IterErase = std::unique( dqSerialIndex.begin(), dqSerialIndex.end() );
	if ( IterErase != dqSerialIndex.end() )
		dqSerialIndex.erase( IterErase, dqSerialIndex.end() );

	for ( int i = 0 ; i < dqSerialIndex.size() ; i++ )
	{
		dqResult.push_front( dqSerial[dqSerialIndex[i]] );
	}

	if ( dqResult.size() >= 3 )
	{		
		DUALBYTEDQ dqStraightTemp;
		DUALBYTEDQ dqTogetherTemp;
		DUALBYTEDQ dqRegistTemp;
		
		dqStraightTemp.clear();
		dqTogetherTemp.clear();
		dqRegistTemp.clear();

		for (int i = 0 ; i < dqResult.size() ; i++)
		{
			if ( IsStraightDeque(dqResult[i]) )
			{
				dqStraightTemp.push_back(dqResult[i]);
			}
			else if( IsTogetherDeque(dqResult[i]) )
			{
				dqTogetherTemp.push_back(dqResult[i]);
			}
			else
			{
				TRACE("Wrong Message\n");
			}
		}
		if ( dqStraightTemp.size() > 1 )
		{
			if( Is_A_SmallThan_B(dqStraightTemp[0][0], dqStraightTemp[1][0]) )
			{
				dqRegistTemp.push_back(dqStraightTemp[0]);
			}
			else
			{
				dqRegistTemp.push_back(dqStraightTemp[1]);
			}

			if (dqTogetherTemp.size() > 0)
			{
				for (int i = 0 ; i < dqRegistTemp[0].size() ; i++)
				{
					for (int j = 0 ; j < dqTogetherTemp.size() ; j++)
					{
						if (hasDequeTheValue(dqTogetherTemp[j], dqRegistTemp[0][i]))
						{
							dqRegistTemp.push_back(dqTogetherTemp[j]);
							dqResult.clear();
							dqResult = dqRegistTemp;
							return TRUE;
						}// if (hasDequeTheValue(dqTogetherTemp[j], dqRegistTemp[0][i]))
					}// for (int j = 0 ; j < dqTogetherTemp.size() ; j++)
				}// for (int i = 0 ; i < dqRegistTemp[0].size() ; i++)				
			}// if (dqTogetherTemp.size() > 0)		
		}// if ( dqStraightTemp.size() > 1 )
	}// if ( dqResult.size() >= 3 )

	if ( dqResult.empty() ) return FALSE;
	return TRUE;
	
} 
BOOL CPlayScr::Is_A_SmallThan_B(BYTE a, BYTE b)
{
	if( (a % 13) < (b % 13) )
	{
		return TRUE;
	}
	else if ((a % 13) == (b % 13))
	{
		if ( a < b )
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL CPlayScr::IsStraightDeque(std::deque<BYTE> dq)
{
	//어설픈 코드 작성예- 임시로 만듬
	//받는 매개변수에 있어서 완벽한 낱장조합과 완벽한 묶은조합이 들어왔을 경우를 가정한다
	if ((dq[0] % 13) != (dq[1] % 13))
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CPlayScr::IsTogetherDeque(std::deque<BYTE> dq)
{
	//어설픈 코드 작성예- 임시로 만듬
	//받는 매개변수에 있어서 완벽한 낱장조합과 완벽한 묶은조합이 들어왔을 경우를 가정한다
	if ((dq[0] % 13) == (dq[1] % 13))
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CPlayScr::hasDequeTheValue(std::deque<BYTE> dq, BYTE b)
{
	for (int i = 0 ; i < dq.size() ; i++ )
	{
		if ( dq[i] == b )
		{
			return TRUE;
		}
	}
	return FALSE;
}
int CPlayScr::CanRegistedCard(char cCardIndex)
{
	int nJoker1 = 0;
	int nJoker2 = 0; 

	for ( int i = 0; i < m_dqSelectedCard.size(); i++ )
	{
		if ( (m_dqSelectedCard[i] >= 60 && m_dqSelectedCard[i] < 120) || (m_dqSelectedCard[i] >= 200 && m_dqSelectedCard[i] < 213) )
		{
			nJoker1 = 1;
		}
		else if ( (m_dqSelectedCard[i] >= 120 && m_dqSelectedCard[i] < 180) || (m_dqSelectedCard[i] >= 220 && m_dqSelectedCard[i] < 233) )
		{
			nJoker2 = 1;
		}
	}

	if ( m_bThankSelect )
	{
		if ( std::count( m_dqSelectedCard.begin(), m_dqSelectedCard.end(), cCardIndex ) >= 1 )
		{
			return 1;
		}
		else if ( std::count( m_dqThankSelect[m_nSelectCurrent].begin(), m_dqThankSelect[m_nSelectCurrent].end(), cCardIndex ) >= 1 )
		{
			return 2;
		}
		return 0;
	}

	if ( m_bThankReserved )
	{
		if ( m_bClickRegCard )
		{
			BOOL bSelected = FALSE;
			if ( std::count( m_dqSelectedCard.begin(), m_dqSelectedCard.end(), cCardIndex ) >= 1 )
			{
				bSelected = TRUE;
			}
			else if ( nJoker1 && cCardIndex == CARD_JOKER1 )
			{
				bSelected = TRUE;
			}
			else if ( nJoker2 && cCardIndex == CARD_JOKER2 )
			{
				bSelected = TRUE;
			}

			for ( int i = 0; i < m_dqThankSelectClick.size(); i++ )
			{
				if ( std::count( m_dqThankSelectClick[i].begin(), m_dqThankSelectClick[i].end(), cCardIndex ) >= 1 )
				{
					return 1;
				}
				else
				{
					if ( cCardIndex == CARD_JOKER1 )
					{
						for ( int loop = 0; loop < m_dqThankSelectClick[i].size(); loop++ )
						{
							if ( (m_dqThankSelectClick[i][loop] >= 60 && m_dqThankSelectClick[i][loop] < 120) || (m_dqThankSelectClick[i][loop] >= 200 && m_dqThankSelectClick[i][loop] < 213) )
							{
								return 1;
							}
						}
					}
					else if ( cCardIndex == CARD_JOKER2 )
					{
						for ( int loop = 0; loop < m_dqThankSelectClick[i].size(); loop++ )
						{
							if ( (m_dqThankSelectClick[i][loop] >= 120 && m_dqThankSelectClick[i][loop] < 180) || (m_dqThankSelectClick[i][loop] >= 220 && m_dqThankSelectClick[i][loop] < 233) )
							{
								return 1;
							}
						}
					}
				}
			}

			if ( bSelected )
			{
				return 0;
			}

			for ( int i = 0; i < m_dqThankSelect.size(); i++ )
			{
				if ( std::count( m_dqThankSelect[i].begin(), m_dqThankSelect[i].end(), cCardIndex ) >= 1 )
				{
					return 2;
				}
				else
				{
					if ( cCardIndex == CARD_JOKER1 )
					{
						for ( int loop = 0; loop < m_dqThankSelect[i].size(); loop++ )
						{
							if ( (m_dqThankSelect[i][loop] >= 60 && m_dqThankSelect[i][loop] < 120) || (m_dqThankSelect[i][loop] >= 200 && m_dqThankSelect[i][loop] < 213) )
							{
								return 1;
							}
						}
					}
					else if ( cCardIndex == CARD_JOKER2 )
					{
						for ( int loop = 0; loop < m_dqThankSelect[i].size(); loop++ )
						{
							if ( (m_dqThankSelect[i][loop] >= 120 && m_dqThankSelect[i][loop] < 180) || (m_dqThankSelect[i][loop] >= 220 && m_dqThankSelect[i][loop] < 233) )
							{
								return 1;
							}
						}
					}
				}
			}

			return 0;
		}
		else
		{
			for ( int i = 0; i < m_dqThankSelect.size(); i++ )
			{
				if ( std::count( m_dqThankSelect[i].begin(), m_dqThankSelect[i].end(), cCardIndex ) >= 1 )
				{
					return 1;
				}
				else
				{
					if ( cCardIndex == CARD_JOKER1 )
					{
						for ( int j = 0; j < m_dqThankSelect[i].size(); j++ )
						{
							if ( (m_dqThankSelect[i][j] >= 60 && m_dqThankSelect[i][j] < 120) || (m_dqThankSelect[i][j] >= 200 && m_dqThankSelect[i][j] < 213) )
							{
								return 1;
							}
						}
					}
					else if ( cCardIndex == CARD_JOKER2 )
					{
						for ( int j = 0; j < m_dqThankSelect[i].size(); j++ )
						{
							if ( (m_dqThankSelect[i][j] >= 120 && m_dqThankSelect[i][j] < 180) || (m_dqThankSelect[i][j] >= 220 && m_dqThankSelect[i][j] < 233 ) )
							{
								return 1;
							}
						}
					}
				}
			}

			return 0;
		}
	}	
	else
	{
		if ( m_bClickRegCard )
		{
			BOOL bSelected = FALSE;
			if ( std::count( m_dqSelectedCard.begin(), m_dqSelectedCard.end(), cCardIndex ) >= 1 )
			{
				bSelected = TRUE;
			}
			else if ( nJoker1 && cCardIndex == CARD_JOKER1 )
			{
				bSelected = TRUE;
			}
			else if ( nJoker2 && cCardIndex == CARD_JOKER2 )
			{
				bSelected = TRUE;
			}

			if ( std::count( m_cClickCardInfo.m_dqUpCard.begin(), m_cClickCardInfo.m_dqUpCard.end(), cCardIndex ) >= 1 )
			{
				return 1;
			}
			else
			{
				if ( cCardIndex == CARD_JOKER1 )
				{
					for ( int loop = 0; loop < m_cClickCardInfo.m_dqUpCard.size(); loop++ )
					{
						if ( (m_cClickCardInfo.m_dqUpCard[loop] >= 60 && m_cClickCardInfo.m_dqUpCard[loop] < 120) || (m_cClickCardInfo.m_dqUpCard[loop] >= 200 && m_cClickCardInfo.m_dqUpCard[loop] < 213 ) )
						{
							return 1;
						}
					}
				}
				else if ( cCardIndex == CARD_JOKER2 )
				{
					for ( int loop = 0; loop < m_cClickCardInfo.m_dqUpCard.size(); loop++ )
					{
						if ( (m_cClickCardInfo.m_dqUpCard[loop] >= 120 && m_cClickCardInfo.m_dqUpCard[loop] < 180) || (m_cClickCardInfo.m_dqUpCard[loop] >= 220 && m_cClickCardInfo.m_dqUpCard[loop] < 233 ) )
						{
							return 1;
						}
					}
				}
			}

			if ( bSelected )
			{
				return 0;
			}

			if ( std::count( m_cCardInfo.m_dqUpCard.begin(), m_cCardInfo.m_dqUpCard.end(), cCardIndex ) >= 1 )
			{
				return 2;
			}
			else
			{
				if ( cCardIndex == CARD_JOKER1 )
				{
					for ( int loop = 0; loop < m_cCardInfo.m_dqUpCard.size(); loop++ )
					{
						if ( (m_cCardInfo.m_dqUpCard[loop] >= 60 && m_cCardInfo.m_dqUpCard[loop] < 120)
							|| (m_cCardInfo.m_dqUpCard[loop] >= 200 && m_cCardInfo.m_dqUpCard[loop] < 213) )
						{
							return 1;
						}
					}
				}
				else if ( cCardIndex == CARD_JOKER2 )
				{
					for ( int loop = 0; loop < m_cCardInfo.m_dqUpCard.size(); loop++ )
					{
						if ( (m_cCardInfo.m_dqUpCard[loop] >= 120 && m_cCardInfo.m_dqUpCard[loop] < 180)
							|| (m_cCardInfo.m_dqUpCard[loop] >= 220 && m_cCardInfo.m_dqUpCard[loop] < 233) )
						{
							return 1;
						}
					}
				}
			}

			return 0;
		}
		else
		{
			if ( m_bRegSelect )
			{
				for ( int i = 0; i < m_cCardInfo.m_dqSelect.size(); i++ )
				{
					if ( std::count( m_cCardInfo.m_dqSelect[i].begin(), m_cCardInfo.m_dqSelect[i].end(), cCardIndex ) >= 1 )
					{
						return 1;
					}
					else
					{
						if ( cCardIndex == CARD_JOKER1 )
						{
							for ( int loop = 0; loop < m_cCardInfo.m_dqSelect[i].size(); loop++ )
							{
								if ( (m_cCardInfo.m_dqSelect[i][loop] >= 60 && m_cCardInfo.m_dqSelect[i][loop] < 120) 
									|| (m_cCardInfo.m_dqSelect[i][loop] >= 200 && m_cCardInfo.m_dqSelect[i][loop] < 213) )
								{
									return 1;
								}
							}
						}
						else if ( cCardIndex == CARD_JOKER2 )
						{
							for ( int loop = 0; loop < m_cCardInfo.m_dqSelect[i].size(); loop++ )
							{
								if ( (m_cCardInfo.m_dqSelect[i][loop] >= 120 && m_cCardInfo.m_dqSelect[i][loop] < 180)
									|| (m_cCardInfo.m_dqSelect[i][loop] >= 220 && m_cCardInfo.m_dqSelect[i][loop] < 233) )
								{
									return 1;
								}
							}
						}
					}
				}
				if ( std::count( m_cCardInfo.m_dqUpCard.begin(), m_cCardInfo.m_dqUpCard.end(), cCardIndex ) >= 1 )
				{
					return 1;
				}
			}
			else
			{
				if ( std::count( m_cCardInfo.m_dqUpCard.begin(), m_cCardInfo.m_dqUpCard.end(), cCardIndex ) >= 1 )
				{
					return 1;
				}
				else
				{
					if ( cCardIndex == CARD_JOKER1 )
					{
						for ( int loop = 0; loop < m_cCardInfo.m_dqUpCard.size(); loop++ )
						{
							if ( (m_cCardInfo.m_dqUpCard[loop] >= 60 && m_cCardInfo.m_dqUpCard[loop] < 120)
								|| (m_cCardInfo.m_dqUpCard[loop] >= 200 && m_cCardInfo.m_dqUpCard[loop] < 213) )
							{
								return 1;
							}
						}
					}
					else if ( cCardIndex == CARD_JOKER2 )
					{
						for ( int loop = 0; loop < m_cCardInfo.m_dqUpCard.size(); loop++ )
						{
							if ( (m_cCardInfo.m_dqUpCard[loop] >= 120 && m_cCardInfo.m_dqUpCard[loop] < 180)
								|| (m_cCardInfo.m_dqUpCard[loop] >= 220 && m_cCardInfo.m_dqUpCard[loop] < 233) )
							{
								return 1;
							}
						}
					}
				}
			}
			return 0;
		}
	}

	return 0;
}

BOOL CPlayScr::IsSelected(char cCardIndex)
{
	if ( m_dqSelectedCard.empty() )
	{
		return FALSE;
	}
	else
	{
		if ( std::count( m_dqSelectedCard.begin(), m_dqSelectedCard.end(), cCardIndex ) >= 1 )
		{
			return TRUE;
		}
		else
		{
			for ( int i = 0; i < m_dqSelectedCard.size(); i++ )
			{
				if ( m_dqSelectedCard[i] >= 60 && m_dqSelectedCard[i] < 120 )
				{
					if ( cCardIndex == CARD_JOKER1 ) 
						return TRUE;
				}
				else if ( m_dqSelectedCard[i] >= 120 && m_dqSelectedCard[i] < 180 )
				{
					if ( cCardIndex == CARD_JOKER2 )
						return TRUE;
				}
			}
		}
	}
	return FALSE;
}


void CPlayScr::CheckThank( BYTE cCardIndex, int nType )
{
	BYTEDQ dqSeq, dqSeqBak, dqTemp;
	DUALBYTEDQ dqStraight, dqTogether, dqThankSelect;
	char cThankFlag;

	for ( int i = 0; i < MAX_CARDCOUNT; i++ )
	{
		if ( IsValidCardIndex_WITHOUT_Joker(m_PlayerCard[0][i]) )
		{
			dqSeq.push_back( m_PlayerCard[0][i] ); 
		}
	}

	dqSeqBak = dqSeq;
	dqSeqBak.push_back( cCardIndex );

	m_nThankType = nType;

	if ( nType == 1 || nType == 2 ) // same
	{
		dqStraight = FindSerialValue( dqSeqBak );
		for ( int i = 0; i < dqStraight.size(); i++ )
		{
			if ( std::count( dqStraight[i].begin(), dqStraight[i].end(), cCardIndex ) >= 1 )
			{			
				dqThankSelect.push_back( dqStraight[i] );
				cThankFlag = 1;
				break;
			}
		}

		dqTogether = FindSameValue( dqSeqBak );
		for ( int i = 0; i < dqTogether.size(); i++ )
		{
			if ( std::count( dqTogether[i].begin(), dqTogether[i].end(), cCardIndex ) >= 1 )
			{
				dqThankSelect.push_back( dqTogether[i] );
				cThankFlag = 2;
				break;
			}
		}

		if ( !dqThankSelect.empty() )
		{
			m_cThankCard = cCardIndex;
			m_bThankReserved = TRUE;
			m_cThankFlag = cThankFlag;

			if ( dqThankSelect.size() >= 2 )
			{
				m_bThankSelect = TRUE;
				m_dqThankSelect = dqThankSelect;
				m_nSelectCurrent = 0;
				m_nSelectMax = 2;
			}
			else
			{
				m_bThankSelect = FALSE;
				m_dqThankSelect = dqThankSelect;
			}
		}
		else
		{
			PassThankyou();
		}
	}
}


void CPlayScr::CheckThank( BYTEDQ& dqSeq )
{
	BYTEDQ dqSeqBak, dqTemp;
	DUALBYTEDQ dqStraight, dqTogether, dqThankSelect;
	char cThankFlag;

	BYTE cCardIndex = m_cThankCard;

	dqSeqBak = dqSeq;
	dqSeqBak.push_back( cCardIndex );

	if ( m_nThankType == 1 || m_nThankType == 2 )
	{
		dqStraight = FindSerialValue( dqSeqBak );
		for ( int i = 0; i < dqStraight.size(); i++ )
		{
			if ( std::count( dqStraight[i].begin(), dqStraight[i].end(), cCardIndex ) >= 1 )
			{			
				dqThankSelect.push_back( dqStraight[i] );
				cThankFlag = 1;
				break;
			}
		}
		dqTogether = FindSameValue( dqSeqBak );
		for ( int i = 0; i < dqTogether.size(); i++ )
		{
			if ( std::count( dqTogether[i].begin(), dqTogether[i].end(), cCardIndex ) >= 1 )
			{
				dqThankSelect.push_back( dqTogether[i] );
				cThankFlag = 2;
				break;
			}
		}

		m_dqThankSelectClick.clear();

		if ( !dqThankSelect.empty() )
		{
			if ( dqThankSelect.size() == 1 )
			{
				m_cThankFlagClick = cThankFlag;
				m_dqThankSelectClick = dqThankSelect;
			}
		}
	}
}


void CPlayScr::ResetTurnFlag()
{
	m_cThankCard = -1;	
	m_bRegSelect = FALSE;
	m_bClickRegCard = FALSE;

	m_cCardInfo.Reset();
	m_cClickCardInfo.Reset();

	// 등록/붙이기 가능한 것 표시
	m_dqSelectedCard.clear();
	m_bPressThank = FALSE;
	m_bThankStep2 = FALSE;
	m_bThankReserved = FALSE;
	m_bThankSelect = FALSE;
	m_cThankFlag = 0;
	m_cThankFlagClick = 0;
	m_dqThankSelect.clear();
	m_dqThankSelectClick.clear();

	m_nSelectCurrent = -1;
	m_nSelectMax = -1;
}


BOOL CPlayScr::SendThank( int pnum )
{
	if ( g_RI.cRuleType == hoola::eRULE_TYPE_WAR )
	{
		m_nTurnCnt = pnum;
	}

	if ( g_PLAYER(0)->ServPNum != pnum )
	{
		ResetLimitTime();
		SetLimitTime( eTIMER_THANK_SELECT );
		return FALSE;
	}

	if ( g_PLAYER(0)->ServPNum == pnum && m_bThankReserved )
	{
		if ( !m_bThankSelect )
		{
			if ( m_dqThankSelect.size() == 0 ) return FALSE;
			if ( m_dqThankSelect[0].size() <= 3 )
			{
				char cThankCard[8];
				memset( cThankCard, -1, sizeof(char)*8 );
				for ( int i = 0; i < m_dqThankSelect[0].size(); i++ )
				{
					cThankCard[i] = m_dqThankSelect[0][i];
				}

				if ( m_cThankFlag > 0 )
				{
					CCL_ASK_THANK msg;
					CLTHANKCARD pCT;

					pCT.ThankCase = m_cThankFlag;
					pCT.nThankYouCnt = m_dqThankSelect[0].size();
					memcpy( pCT.ThankCard, cThankCard, sizeof( char ) * MAX_CARDCOUNT );
					msg.Set( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, &pCT );
					NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
				}

				ResetTurnFlag();
				ResetSelect();
				SetNullButton();
				CheckReg();
			} //if ( m_dqThankSelect[0].size() <= 3 )
			else
			{
				m_bThankStep2 = TRUE;
				m_bClickRegCard = TRUE;
				m_dqSelectedCard.clear();
				m_dqSelectedCard = m_dqThankSelect[0];
				BYTEDQ::iterator Iter;
				Iter = std::find( m_dqSelectedCard.begin(), m_dqSelectedCard.end(), m_cThankCard );
				if ( Iter !=  m_dqSelectedCard.end() )
					m_dqSelectedCard.erase( Iter );
				BYTEDQ dqTemp = m_dqSelectedCard;
				CheckThank( dqTemp );
				SetThankSelectButton();
			} //if ( m_dqThankSelect[0].size() > 3 )
			return TRUE;
		} // if ( !m_bThankSelect )
		else
		{	//이 부분이 카드 선택 뷰 그리는부분인듯 
			EnableSelect();
			SetLimitTime( eTIMER_THANK_SELECT_DUP );
		} //if ( m_bThankSelect )
	} //if ( g_PLAYER(0)->ServPNum == pnum && m_bThankReserved )
	return FALSE;
}


void CPlayScr::SetLimitTime( eTIMER_TYPE eType )
{
	if (m_bWind) 
		return;
	if ( !m_bGameOver )
	{
		if ( eType != m_nLTimeCase )
		{
			m_nLTimeCase = eType;
			m_dwLTime = timeGetTime();
			m_nSec = -1;
			m_bFirstCheckReg = FALSE;
			m_bFirstCheckingReg = FALSE;
		}
	} //if ( !m_bGameOver )
	else
	{
		m_nLTimeCase = 0;
		m_dwLTime = 0;
		m_nSec = -1;
	}
}

void CPlayScr::DrawProcessTimer()
{
	DWORD dwTime = abs( (int)(timeGetTime() - m_dwLTime) );
	int nSec = dwTime / 1000;

	switch ( m_nLTimeCase )
	{
	case eTIMER_THANK_NORMAL:
		{
			int nLimitTime = 4;
			if ( m_nSec != nSec && m_nSec <= nLimitTime )
			{		
				m_nSec = nSec;
				NMBASE::SOUND::g_Sound.PlayWav(SND18);
			}

			//시간이 초과했을 때
			if ( m_nSec >= nLimitTime )
			{
				if ( m_pGameButtonHA->IsEnableBtn(CGameButtonHA::BTN_THANK) && g_PLAYER(0)->JoinState == EGAME_JOININ 
					&& g_PLAYER(0)->PlayState == EPLAYER_GAMEPLAYING )
					PassBtnClick();
				ResetLimitTime();
				return;
			} 

			if ( m_pGameButtonHA->IsEnableBtn(CGameButtonHA::BTN_THANK) && g_PLAYER(0)->JoinState == EGAME_JOININ 
				&& g_PLAYER(0)->PlayState == EPLAYER_GAMEPLAYING )
			{
				int sceneIndex = 1;
				if ( dwTime < g_sprThankTimer.GetMScenePlayTime( sceneIndex ) )
				{
					m_pPage->DrawMultiScene( 0, 0, &g_sprThankTimer, sceneIndex, dwTime );
				}
			}
		}break;
	case eTIMER_GET:
		{
			int nLimitTime = 4;
			if ( m_nSec != nSec && m_nSec <= nLimitTime )
			{
				m_nSec = nSec;
				if ( m_nSec >= 3 && m_nSec <= 5 )
					NMBASE::SOUND::g_Sound.PlayWav(SND21);
			}

			//시간이 초과했을 때
			if ( m_nSec >= nLimitTime )
			{
				//받기관련 시간이 초과했을 때
				if ( m_nTurnCnt == g_PLAYER(0)->ServPNum && g_PLAYER(0)->JoinState == EGAME_JOININ && g_PLAYER(0)->PlayState == EPLAYER_GAMEPLAYING )
					GetCardBtnClick();
				ResetLimitTime();
				return;
			}

			int pnum = m_nTurnCnt - g_PLAYER(0)->ServPNum;
			if( pnum < 0 ) pnum += MAX_PLAYER;

			int nX, nY;
			switch ( pnum )
			{
				case 0:		nX = 407; nY = 565;		break;
				case 1:		nX = 144; nY = 386;		break;
				case 2:		nX = 143; nY = 195;		break;
				case 3:		nX = 823; nY = 195;		break;
				case 4:		nX = 824; nY = 386;		break;
				default:	nX = 1000; nY = 740;	break;
			}

			// 스프라이트 출력
			if ( g_PLAYER(pnum)->JoinState == EGAME_JOININ && g_PLAYER(pnum)->PlayState == EPLAYER_GAMEPLAYING )
			{
				int sceneIndex = 0;
				if ( dwTime < g_sprTimer.GetMScenePlayTime( sceneIndex ) )
				{
					m_pPage->DrawMultiScene( nX, nY, &g_sprTimer, sceneIndex, dwTime );
				}
			}
		}break;
	case eTIMER_REGIST:
	case eTIMER_THANK_SELECT:
	case eTIMER_THANK_SELECT_DUP:
		{
			int pnum = m_nTurnCnt - g_PLAYER(0)->ServPNum;
			int nCompareSec = 15;

			if ( m_bFirstCheckingReg )
				nCompareSec = 5;
			else
				nCompareSec = 12;

			if ( pnum == 0 && CheckAvailableReg() == FALSE && m_bFirstCheckReg == FALSE )
			{
				nCompareSec = 4;
				m_bFirstCheckingReg = TRUE;
			}
			m_bFirstCheckReg = TRUE;

			if ( m_nSec != nSec )
			{
				m_nSec = nSec;
				if ( nCompareSec != 5 && m_nSec == nCompareSec - 5 && pnum != 0 )
				{
					PlayVoice( g_PLAYER(0)->nSndFxKind, BORING, 40 );
				}
				if ( nCompareSec != 5 && m_nSec >= nCompareSec - 6 && m_nSec <= nCompareSec-3 )
				{
					NMBASE::SOUND::g_Sound.PlayWav(SND21);
				}
			}

			if ( m_nSec >= nCompareSec )
			{
				if ( m_nTurnCnt == g_PLAYER(0)->ServPNum && g_PLAYER(0)->JoinState == EGAME_JOININ && g_PLAYER(0)->PlayState == EPLAYER_GAMEPLAYING )
				{
					switch ( m_nLTimeCase )
					{
					case eTIMER_REGIST:
						DumpCardBtnClickAfter();
						break;
					case eTIMER_THANK_SELECT:
						SendThankStep2( 1 );						
						break;
					case eTIMER_THANK_SELECT_DUP:
						if ( SendThank() )
						{
							m_nSelectCurrent = -1;
							m_nSelectMax = -1;
						}
						break;
					}
				}
				ResetLimitTime();
				return;
			}

			//등록 시간 타이머가 돌아가고 있을때 우
			if ( m_nLTimeCase == eTIMER_REGIST )
			{
				if ( dwTime > ( nCompareSec * 1000 ) - 500 )
				{
					m_pGameButtonHA->SetEnableBtn(CGameButtonHA::BTN_REGIST, FALSE);
					return;
				}
			}


			// 스프라이트 출력
			if( pnum < 0 ) pnum += MAX_PLAYER;

			int nX, nY;
			switch ( pnum )
			{
				case 0:		nX = 407; nY = 565;		break;
				case 1:		nX = 144; nY = 386;		break;
				case 2:		nX = 143; nY = 195;		break;
				case 3:		nX = 823; nY = 195;		break;
				case 4:		nX = 824; nY = 386;		break;
				default:	nX = 1000; nY = 740;	break;
			}

			if ( g_PLAYER(pnum)->JoinState == EGAME_JOININ && g_PLAYER(pnum)->PlayState == EPLAYER_GAMEPLAYING )
			{
				int sceneIndex = 3;
				switch ( nCompareSec )
				{
					case 12:	sceneIndex = 2;		break;
					case 5:		sceneIndex = 1;		break;
					case 4:		sceneIndex = 0;		break;
				}
				if ( dwTime < g_sprTimer.GetMScenePlayTime( sceneIndex ) )
				{
					m_pPage->DrawMultiScene( nX, nY, &g_sprTimer, sceneIndex, dwTime );
				}
			}
		}break;
	case eTIMER_THANK_SPECIAL:
		{
			int nLimitTime = 5;
			if ( m_nSec != nSec && m_nSec <= nLimitTime )
			{
				m_nSec = nSec;
				if ( m_nSec >= 3 && m_nSec <= 5 )
					NMBASE::SOUND::g_Sound.PlayWav(SND21);
			}

			//시간이 초과했을 때
			if ( m_nSec >= nLimitTime )
			{
				if ( g_PLAYER(0)->JoinState == EGAME_JOININ && g_PLAYER(0)->PlayState == EPLAYER_GAMEPLAYING )
				{
					m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_LOWTHANK, FALSE);
					m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_HIGHTHANK, FALSE);
					m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_SEVENFOURTHANK, FALSE);
				}
				ResetLimitTime();
				return;
			}
			int nX = 407;
			int nY = 565;
			int sceneIndex = 1;
			if ( dwTime < g_sprTimer.GetMScenePlayTime( sceneIndex ) )
			{
				m_pPage->DrawMultiScene( nX, nY, &g_sprTimer, sceneIndex, dwTime );
			}
		}break;
	}
}

void CPlayScr::ResetLimitTime()
{
	m_nLTimeCase = 0;
	m_dwLTime = 0;
	m_nSec = -1;
	m_bFirstCheckReg = FALSE;
	m_bFirstCheckingReg = FALSE;
}

void CPlayScr::EnableSelect()
{
	m_bSelectDraw = TRUE;
	
	if ( TRUE == m_bRegSelect )
	{
		//	(678:777) (777:777) 조합 선택이 있을경우 우선순위는
		//	(678:777) 선택이 더 직관적이다
		m_nUpCardType = 0;
	}
}

void CPlayScr::ResetSelect()
{
	m_bSelectDraw = FALSE;
	m_nUpCardType = 0;
}

void CPlayScr::SelectOkBtnClick()
{
	if ( m_bThankStep2 )
	{
		SendThankStep2();
		return;
	}

	if ( m_bThankSelect )
	{
		if ( SendThank() )
		{
			m_nSelectCurrent = -1;
			m_nSelectMax = -1;
			ResetSelect();
		}
		return;
	}

	if ( m_bRegSelect )
	{
		if ( RegisBtnClick() )
		{
			m_nSelectCurrent = -1;
			m_nSelectMax = -1;
			ResetSelect();
		}
	}
}


BOOL CPlayScr::NowSelectedCard( char cCardIndex )
{
	if ( m_bThankSelect && !m_bClickRegCard )
	{
		//m_bThankSelect 땡큐 가능한 상황이거나
		//m_bClickRegCard 특정 카드를 선택한 상황이 아닐경우
		if ( m_nSelectCurrent == -1 || m_nSelectMax == -1 )
		{
			return FALSE;
		}

		if ( m_nSelectCurrent >= m_dqThankSelect.size() )
		{
			return FALSE;
		}

		if ( std::count( m_dqThankSelect[m_nSelectCurrent].begin(), m_dqThankSelect[m_nSelectCurrent].end(), cCardIndex ) >= 1 )
		{
			return TRUE;
		}

		return FALSE;
	}
	
	if ( m_bRegSelect && !m_bClickRegCard )
	{
		if ( m_nSelectCurrent == -1 || m_nSelectMax == -1 )
		{
			return FALSE;
		}
		if ( m_nSelectCurrent >= m_cCardInfo.m_dqSelect.size() )
		{
			return FALSE;
		}
		if ( std::count( m_cCardInfo.m_dqSelect[m_nSelectCurrent].begin(), m_cCardInfo.m_dqSelect[m_nSelectCurrent].end(), cCardIndex ) >= 1 )
		{
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}


BOOL CPlayScr::SendThank()
{
	if ( m_bClickRegCard )
	{
		char cThankCard[MAX_CARDCOUNT];
		memset( cThankCard, -1, sizeof(char) * MAX_CARDCOUNT );

		if ( m_dqThankSelectClick.empty() )
			return FALSE;

		if ( m_dqSelectedCard.size()+1 != m_dqThankSelectClick[0].size() )
			return FALSE;

		for ( int i = 0; i < m_dqThankSelectClick[0].size(); i++ )
		{
			cThankCard[i] = m_dqThankSelectClick[0][i];
		}

		if ( m_cThankFlagClick > 0 )
		{
			CCL_ASK_THANK msg;
			CLTHANKCARD pCT;

			pCT.ThankCase = m_cThankFlagClick;
			pCT.nThankYouCnt = m_dqThankSelectClick[0].size();

			memcpy( pCT.ThankCard, cThankCard, sizeof( char ) * 8 );

			msg.Set( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, &pCT );
			NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		ResetTurnFlag();
		ResetSelect();
		SetNullButton();
		CheckReg();
		return TRUE;
	}
	else
	{
		if ( m_nSelectCurrent == -1 || m_nSelectMax == -1 )
		{
			return FALSE;
		}

		if ( m_nSelectCurrent >= m_dqThankSelect.size() )
		{
			return FALSE;
		}

		char cThankCard[8];
		memset( cThankCard, -1, sizeof(char)*8 );
		for ( int i = 0; i < m_dqThankSelect[m_nSelectCurrent].size(); i++ )
		{
			cThankCard[i] = m_dqThankSelect[m_nSelectCurrent][i];
		}

		m_cThankFlag = m_nSelectCurrent > 0 ? 2 : 1;
		if ( m_cThankFlag > 0 )
		{
			CCL_ASK_THANK msg;
			CLTHANKCARD pCT;

			pCT.ThankCase = m_cThankFlag;
			pCT.nThankYouCnt = m_dqThankSelect[m_nSelectCurrent].size();

			memcpy( pCT.ThankCard, cThankCard, sizeof( char ) * MAX_CARDCOUNT );

			msg.Set( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, &pCT );
			NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}

		ResetTurnFlag();
		ResetSelect();
		SetNullButton();
		CheckReg();
		return TRUE;
	}
}



void CPlayScr::SendThankStep2( int nType )
{
	if ( m_bClickRegCard )
	{
		if ( nType == 1 )
		{
			if ( !m_dqThankSelectClick.empty() )
			{
				BYTE cThankCard[MAX_CARDCOUNT];
				memset( cThankCard, -1, sizeof(char) * MAX_CARDCOUNT );
				for ( int i = 0; i < m_dqThankSelect[0].size(); i++ )
				{
					cThankCard[i] = m_dqThankSelect[0][i];
				}

				if ( m_cThankFlag > 0 )
				{
					CCL_ASK_THANK msg;
					CLTHANKCARD pCT;

					pCT.ThankCase = m_cThankFlag;
					pCT.nThankYouCnt = m_dqThankSelect[0].size();

					memcpy( pCT.ThankCard, cThankCard, sizeof( char ) * MAX_CARDCOUNT );

					msg.Set( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, &pCT );
					NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
				}
				ResetTurnFlag();
				ResetSelect();
				SetNullButton();
				CheckReg();
				return ;
			}
		}
		else
		{
			if ( m_dqThankSelectClick.empty() )
			{
				return;
			}
		}

		BYTE cThankCard[8];
		memset( cThankCard, -1, sizeof(char)*8 );

		if ( m_dqThankSelectClick.empty() )
			return ;

		if ( m_dqSelectedCard.size()+1 != m_dqThankSelectClick[0].size() )
			return ;


		for ( int i = 0; i < m_dqThankSelectClick[0].size(); i++ )
		{
			cThankCard[i] = m_dqThankSelectClick[0][i];
		}

		if ( m_cThankFlagClick > 0 )
		{
			CCL_ASK_THANK msg;
			CLTHANKCARD pCT;

			pCT.ThankCase = m_cThankFlagClick;
			pCT.nThankYouCnt = m_dqThankSelectClick[0].size();

			memcpy( pCT.ThankCard, cThankCard, sizeof( char ) * MAX_CARDCOUNT );

			msg.Set( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, &pCT );
			NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		ResetTurnFlag();
		ResetSelect();
		SetNullButton();
		CheckReg();
		return;
	}
	else
	{
		if ( m_dqThankSelect.empty() )
			return;

		BYTE cThankCard[8];
		memset( cThankCard, -1, sizeof(char) * MAX_CARDCOUNT );
		for ( int i = 0; i < m_dqThankSelect[0].size(); i++ )
		{
			cThankCard[i] = m_dqThankSelect[0][i];
		}

		if ( m_cThankFlag > 0 )
		{
			CCL_ASK_THANK msg;
			CLTHANKCARD pCT;

			pCT.ThankCase = m_cThankFlag;
			pCT.nThankYouCnt = m_dqThankSelect[0].size();

			memcpy( pCT.ThankCard, cThankCard, sizeof( char ) * MAX_CARDCOUNT );

			msg.Set( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, &pCT );
			NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}

		ResetTurnFlag();
		ResetSelect();
		SetNullButton();
		CheckReg();
		return ;
	}
}

void CPlayScr::OnLButtonDblClick( int xp, int yp )
{
	if ( (m_nTurnCnt == g_PLAYER(0)->ServPNum && m_nTurnIndex == 1) && m_pGameButtonHA->IsEnableBtn(CGameButtonHA::BTN_DUMPCARD) )
	{
		int dist1 = 20;
		for ( int i = g_PLAYER(0)->GetCardCount() - 1; i >= 0; i-- )
		{
			char cCardIndex = m_PlayerCard[0][i];

			int nAddY = 0;
			if ( IsSelected( cCardIndex ) )
			{
				nAddY = 13;
			}

			int nAddX = 20;
			if ( i == g_PLAYER(0)->GetCardCount() - 1 )
				nAddX = 74;

			if ( (xp >= CardXY[0][0] + i * dist1) && (xp < CardXY[0][0] + i * dist1 + nAddX) )
			{
				if ( (yp >= CardXY[0][1] - nAddY) && (yp < CardXY[0][1] - nAddY + 99) )
				{
					if( ( (g_RI.FormKind == hoola::eFORM_KIND_JOKER) && cCardIndex >= 0 && cCardIndex <= CARD_JOKER2 ) 
						|| ( g_RI.FormKind == hoola::eFORM_KIND_NORMAL && cCardIndex >= 0 && cCardIndex <= 51 ) )
					{
						CCL_DISCARD msg;
						msg.Set( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, cCardIndex );
						NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
						SetNullButton();

						m_nDiscardTest = cCardIndex;
					}
				}
			}
		}
	}
}

void CPlayScr::ResultOkBtnClick()
{
	m_bShowGameResult = FALSE;
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_RESULTOK, FALSE);	
}

BOOL CPlayScr::IsNewCard( char cCardIndex )
{
	if ( cCardIndex >= 0 && cCardIndex < 54 )
	{
		if ( std::count( m_dqNewCard.begin(), m_dqNewCard.end(), cCardIndex ) >= 1 )
		{
			return TRUE;
		}
	}
	else if ( cCardIndex >= 0 && cCardIndex < CARD_JOKER1 )
	{
		if ( std::count( m_dqNewCard.begin(), m_dqNewCard.end(), cCardIndex ) >= 1 )
		{
			return TRUE;
		}
	}
	return FALSE;
}

void CPlayScr::ResetMoveCard()
{
	m_MoveCard.eType = eCARDEVENT_NULL;
	m_MoveCard.nPN = -1;
	m_MoveCard.dqCard.clear();
	m_MoveCard.dqStart.clear();
	m_MoveCard.dqDest.clear();
	m_MoveCard.dqAddx.clear();
	m_MoveCard.dqAddy.clear();
	m_MoveCard.dqDelay.clear();
	m_MoveCard.dqMoveFlag.clear();
	m_MoveCard.dqTime.clear();
	m_MoveCard.dqType.clear();
}

void CPlayScr::GetStraightDestPos( BYTEDQ& dqCard, std::deque< POINT >& dqDest )
{
	POINT pos;

	for ( int i = 0; i < dqCard.size(); i++ )
	{
		BYTE cCI = dqCard[i];
		if ( cCI >= 60 && cCI <  120 )
		{
			cCI -= 60;
		}
		else if ( cCI >= 120 && cCI < 180 )
		{
			cCI -= 120;
		}

		int nFace = cCI / 13;
		int nIndex = cCI % 13;

		pos.x = 176 + nIndex * 16;
		pos.y = 168 + nFace * 72;

		dqDest.push_back( pos );
	}
}
void CPlayScr::MakeAttachableDq( BYTE cardIndex )
{
	//////////////////////////////////////////////////////////////////////////
	//		여기부터 묶음조합 찾기
	//////////////////////////////////////////////////////////////////////////

	//조커 검색 없다 적용할것 조커카드는 숫자 2
	int nCardFace = (int)cardIndex / 13;
	int nCardNum = (int)cardIndex % 13;

	std::deque <BYTE> dqAttachableStr;
	std::deque <BYTE> dqAttachableTog;

	BOOL bNeedToFind1MoreNextCard = FALSE;
	BOOL bNeedToFind1MoreBeforeCard = FALSE;
	//한자리 앞 숫자 찾기
	if ( IsKCard(cardIndex) )
	{
		if( m_CenterCard[nCardFace][0] != 0 )
		{
			if ( m_CenterCard[nCardFace][0] == 2 )
			{
				dqAttachableStr.push_back( CARD_JOKER1 );
			}
			else if ( m_CenterCard[nCardFace][0] == 3 )
			{
				dqAttachableStr.push_back( CARD_JOKER2 );
			}
			else
			{
				dqAttachableStr.push_back( nCardFace * 13 );
			}			
		}
	}
	else if(m_CenterCard[nCardFace][nCardNum + 1] != 0)
	{
		if ( m_CenterCard[nCardFace][nCardNum + 1] == 2 )
		{
			dqAttachableStr.push_back( CARD_JOKER1 );
		}
		else if ( m_CenterCard[nCardFace][nCardNum + 1] == 3 )
		{
			dqAttachableStr.push_back( CARD_JOKER2 );
		}
		else
		{
			dqAttachableStr.push_back( nCardFace * 13 + nCardNum + 1 );
		}
	}
	else
	{
		bNeedToFind1MoreBeforeCard = TRUE;
	}

	//한자리 뒤 숫자 찾기
	if ( IsAceCard(cardIndex) )
	{
		if( m_CenterCard[nCardFace][12] != 0 )
		{
			if ( m_CenterCard[nCardFace][12] == 2 )
			{
				dqAttachableStr.push_back( CARD_JOKER1 );
			}
			else if ( m_CenterCard[nCardFace][12] == 3 )
			{
				dqAttachableStr.push_back( CARD_JOKER2 );
			}
			else
			{
				dqAttachableStr.push_back( nCardFace * 13 + 12);
			}			
		}
	}
	else if(m_CenterCard[nCardFace][nCardNum - 1] != 0)
	{
		if ( m_CenterCard[nCardFace][nCardNum - 1] == 2 )
		{
			dqAttachableStr.push_back( CARD_JOKER1 );
		}
		else if ( m_CenterCard[nCardFace][nCardNum - 1] == 3 )
		{
			dqAttachableStr.push_back( CARD_JOKER2 );
		}
		else
		{
			dqAttachableStr.push_back( nCardFace * 13 + nCardNum - 1 );
		}		
	}
	else
	{
		bNeedToFind1MoreNextCard = TRUE;
	}

	if ( TRUE == bNeedToFind1MoreBeforeCard)
	{
		int nBeforeCard = dqAttachableStr[0];
		int nBCFace = nBeforeCard / 13;
		int nBCNum = nBeforeCard % 13;
		if ( IsAceCard((BYTE)nBeforeCard) )
		{
			if( m_CenterCard[nBCFace][12] != 0 )
			{
				if (m_CenterCard[nBCFace][12] == 2 )
				{
					dqAttachableStr.push_back( CARD_JOKER1 );
				}
				else if (m_CenterCard[nBCFace][12] == 3 )
				{
					dqAttachableStr.push_back( CARD_JOKER2 );
				}
				else
				{
					dqAttachableStr.push_back( nBCFace * 13 + 12);
				}				
			}
		}
		else if(m_CenterCard[nBCFace][nBCNum - 1] != 0)
		{
			if (m_CenterCard[nBCFace][nBCNum - 1] == 2)
			{
				dqAttachableStr.push_back( CARD_JOKER1 );
			}
			else if (m_CenterCard[nBCFace][nBCNum - 1] == 3)
			{
				dqAttachableStr.push_back( CARD_JOKER2 );
			}
			else
			{
				dqAttachableStr.push_back( nBCFace * 13 + nBCNum - 1 );
			}			
		}
	}

	if ( TRUE == bNeedToFind1MoreNextCard)
	{
		int nNextCard = dqAttachableStr[0];
		int nNCFace = nNextCard / 13;
		int nNCNum = nNextCard % 13;
		if ( IsKCard((BYTE)nNextCard) )
		{
			if( m_CenterCard[nNCFace][0] != 0 )
			{
				if ( m_CenterCard[nNCFace][0] == 2 )
				{
					dqAttachableStr.push_back( CARD_JOKER1 );
				}
				else if ( m_CenterCard[nNCFace][0] == 3 )
				{
					dqAttachableStr.push_back( CARD_JOKER2 );
				}
				else
				{
					dqAttachableStr.push_back( nNCFace * 13 );
				}				
			}
		}
		else if(m_CenterCard[nNCFace][nNCNum + 1] != 0)
		{
			if (m_CenterCard[nNCFace][nNCNum + 1] == 2)
			{
				dqAttachableStr.push_back( CARD_JOKER1 );
			}
			if (m_CenterCard[nNCFace][nNCNum + 1] == 3)
			{
				dqAttachableStr.push_back( CARD_JOKER2 );
			}
			else
			{
				dqAttachableStr.push_back( nNCFace * 13 + nNCNum + 1 );
			}			
		}
	}

	dqAttachableStr.push_back(cardIndex);
	std::sort( dqAttachableStr.begin(), dqAttachableStr.end() );

	//////////////////////////////////////////////////////////////////////////
	//		여기부터 낱장조합 찾기
	//////////////////////////////////////////////////////////////////////////

	for ( int i = 0 ; i < 4 ; i++ )
	{
		if (m_SameNumCard[nCardNum][i] != 0)
		{
			if ( m_SameNumCard[nCardNum][i] == 2 )
			{
				dqAttachableTog.push_back(CARD_JOKER1);
			}
			else if ( m_SameNumCard[nCardNum][i] == 3 )
			{
				dqAttachableTog.push_back(CARD_JOKER2);
			}
			else
			{
				dqAttachableTog.push_back( i * 13 + nCardNum );
			}			
		}
	}
	if ( FALSE == dqAttachableTog.empty() )
	{
		dqAttachableTog.push_back(cardIndex);
	}

	//////////////////////////////////////////////////////////////////////////
	//		다 찾았으므로 큐에 집어넣는다
	//////////////////////////////////////////////////////////////////////////

	m_dqAttachableFakeDq.push_back(dqAttachableStr);
	m_dqAttachableFakeDq.push_back(dqAttachableTog);

}
void CPlayScr::GetStraightDestPos( BYTE cCard, POINT& ptDest )
{
	//카드 번호를 받아 묶음조합 위치를 dqDest에 저장
	if ( cCard >= 60 && cCard <  120 )
	{
		cCard -= 60;
	}
	else if ( cCard >= 120 && cCard < 180 )
	{
		cCard -= 120;
	}
	int nFace = ( cCard / 13 );
	int nIndex = ( cCard % 13 );

	ptDest.x = 176 + nIndex * 16;
	ptDest.y = 218 + nFace * 42;
}


void CPlayScr::GetTogetherDestPos( BYTEDQ& dqCard, std::deque< POINT >& dqDest )
{
	//카드 번호를 받아 낱장조합 위치를 dqDest에 저장
	POINT pos;
	int i = 0;
	int j = 0;
	
	for ( i = 0; i < dqCard.size(); i++ )
	{
		BYTE cCI = dqCard[i];
		int nJokerType = 0;

		if ( (cCI >= 60 && cCI < 120) || (cCI >= 200 && cCI < 213) )
		{
			cCI -= 60;
			nJokerType = 1;
		}
		else if ( (cCI >= 120 && cCI < 180) || (cCI >= 220 && cCI < 233) )
		{
			cCI -= 120;
			nJokerType = 2;
		}

		int nFace = cCI / 13;
		int nIndex = cCI % 13;

		m_SameCard[nIndex][nFace] = nJokerType + 1;
	}

	int nCount = 0;
	int cnt = 0;
	for ( i = 0; i < 13; ++i )
	{
		for( j = 0, cnt = 0; j < 4; ++j )
		{
			if( m_SameCard[i][j] )
			{
				++cnt;
			}
		}

		if ( cnt >= 3 )
		{
			++nCount;
		}
	}

	if ( nCount >= 1 )
	{
		int distx = 16;	
		int dist = 0;

		for ( int k = 0; k < dqCard.size(); k++ )
		{
			BYTE cCI = dqCard[k];

			if ( (cCI >= 60 && cCI < 120) || (cCI >= 200 && cCI < 213) )
			{
				cCI -= 60;
			}
			else if ( (cCI >= 120 && cCI < 180) || (cCI >= 220 && cCI < 233) )
			{
				cCI -= 120;
			}

			int nFace = cCI / 13;
			int nIndex = cCI % 13;

			for( i = 0, dist = 0; i < 13; i++ )
			{		
				int nX, nY;
												
				for( j = 0, cnt = 0; j < 4; j ++ )
				{
					if ( m_SameCard[ i ][ j ] )
					{
						nY = 218 + cnt * 42;
						cnt++;
					}

					if ( i == nIndex && j == nFace )
					{
						if ( nCount > 7 )
						{
							nX = 560 + dist * distx;
						}
						else
						{
							int nDist = m_dqSameCardPosIndex[ nCount - 1 ][dist];
							nX = 560 + (nDist-1) * distx;
						}
						
						pos.x = nX;
						pos.y = nY;
						dqDest.push_back( pos );
					}
				}

				if ( cnt >= 3 )
				{
					dist++;
				}
			}
		}
	}
}


void CPlayScr::GetTogetherDestPos( BYTE cCard, POINT& ptDest )
{
	int nFace = cCard / 13;
	int nIndex = cCard % 13;

	m_SameCard[nIndex][nFace] = 1;

	int nCount = 0;
	int cnt = 0;
	for ( int i = 0; i < 13; i++ )
	{
		cnt = 0;
		for( int j = 0 ; j < 4; j ++ )
		{
			if( m_SameCard[i][j] )
			{
				cnt++;
			}
		}

		if ( cnt >= 3 )
		{
			nCount++;
		}
	}

	if ( nCount >= 1 )
	{
		int distx = 16;	

		int nFace = cCard / 13;
		int nIndex = cCard % 13;
		int dist = 0;

		for( int i = 0 ; i < 13; i ++ )
		{		
			int nX, nY;
			
			
			cnt = 0;
			for( int j = 0; j < 4; j ++ )
			{
				if( m_SameCard[i][j] )
				{
					nY = 167 + cnt * 72;
					cnt++;
				}

				if ( i == nIndex && j == nFace )
				{
					if ( nCount > 7 )
					{
						nX = 560 + dist * distx;
					}
					else
					{
						int nDist = m_dqSameCardPosIndex[nCount-1][dist];
						nX = 560 + (nDist-1) * distx;
					}

					ptDest.x = nX;
					ptDest.y = nY;

					return;
				}
			}

			if ( cnt >= 3 )
			{
				dist++;
			}
		}
	}
}

void CPlayScr::SetMoveThankCard( int nCardNum, BYTE* pCardIndex, char nDumpCard, int pnum, int nAType )
{
	m_bWind = TRUE;
	SetNullButton();
	ResetLimitTime();

	PlayVoice(  g_PLAYER(pnum)->UI.Sex, g_PLAYER(pnum)->nSndFxKind, THANKU );

	BYTEDQ dqCard;
	for ( int i = 0; i < nCardNum; i++ )
	{
		dqCard.push_back( pCardIndex[i] );
	}
	std::sort( dqCard.begin(), dqCard.end() );
	std::deque< POINT > dqDest;

	CopyMemory( m_SameCard, m_SameNumCard, sizeof(int)*4*13 );

	if ( nAType == 1 ) // straight
	{
		GetStraightDestPos( dqCard, dqDest );
	}
	else if ( nAType == 2 )
	{
		GetTogetherDestPos( dqCard, dqDest );
	}

	MOVECARD *pMoveCard = new MOVECARD;

	pMoveCard->eMoveType = eCARDMOVE_STRAIGHT;
	pMoveCard->eType = (eCARDEVENT_TYPE)nAType;
	pMoveCard->nPN = pnum;
	pMoveCard->dqCard = dqCard;
	pMoveCard->dwTime = timeGetTime();
	pMoveCard->dqDest = dqDest;
	pMoveCard->cDumpCard = nDumpCard;

	if (  !g_ObserverMan.IsMyObserver() && pnum == 0 ) // 자기의 카드
	{
		for ( int i = 0; i < pMoveCard->dqCard.size(); i++ )
		{
			pMoveCard->dqMoveFlag.push_back( 0 );
			pMoveCard->dqDelay.push_back( i * 100 );
			pMoveCard->dqTime.push_back( 0 );

			POINT pos;
			if ( pMoveCard->dqCard[i] == nDumpCard )
			{
				pos.x = 464;
				pos.y = 344;
				pMoveCard->dqStart.push_back( pos );
			}
			else
			{
				for ( int j = 0; j < 8; j++ )
				{
					BYTE Card = pMoveCard->dqCard[i];
					if ( Card >= 60 && Card < 120 )
					{
						Card = CARD_JOKER1;
					}
					else if ( Card >= 120 && Card < 180 )
					{
						Card = CARD_JOKER2;
					}
					
					if ( Card == m_PlayerCard[0][j] )
					{
						pos.x = CardXY[pnum][0] + j * 20;
						pos.y = CardXY[pnum][1];
						pMoveCard->dqStart.push_back( pos );
						break;
					}
				}
			}
		}
	}
	else
	{
		int nCardNum = g_PLAYER(pnum)->GetCardCount();
		for ( int i = 0; i < pMoveCard->dqCard.size(); i++ )
		{
			pMoveCard->dqMoveFlag.push_back( 0 );
			pMoveCard->dqDelay.push_back( i * 100 );
			pMoveCard->dqTime.push_back( 0 );
	
			POINT pos;
			if ( pMoveCard->dqCard[i] == nDumpCard )
			{
				pos.x = 464;
				pos.y = 344;
				pMoveCard->dqStart.push_back( pos );
			}
			else
			{
				int dist2 = 11;
				if ( pnum == 0 )
					dist2 = 20;
				CRect rect;

				pos.x = CardXY[pnum][0] + (nCardNum - 1) * dist2;
				pos.y = CardXY[pnum][1];
				nCardNum--;

				pMoveCard->dqStart.push_back( pos );
			}
		}
	}

	int nXLength, nYLength;
	double dLength;
	double dTime;

	for ( int i = 0; i < pMoveCard->dqCard.size(); i++ )
	{
		if ( i >= pMoveCard->dqDest.size() || i >= pMoveCard->dqStart.size() )
			continue;

		nXLength = pMoveCard->dqDest[i].x - pMoveCard->dqStart[i].x;
		nYLength = pMoveCard->dqDest[i].y - pMoveCard->dqStart[i].y;
		dLength = sqrt( (double) (nXLength*nXLength + nYLength*nYLength) );
		dTime = ( dLength * 1000 ) / DOTPERSEC;
		if ( nXLength == 0 )
		{
			pMoveCard->dqAddx.push_back( 0.0 );
		}
		else
		{
			if ( dTime != 0 )
				pMoveCard->dqAddx.push_back( nXLength / dTime );
			else
				pMoveCard->dqAddx.push_back( 0.0 );
		}

		if ( nYLength == 0 )
		{
			pMoveCard->dqAddy.push_back( 0.0 );
		}
		else
		{
			if ( dTime != 0 )
				pMoveCard->dqAddy.push_back( nYLength / dTime );
			else
				pMoveCard->dqAddx.push_back( 0.0 );
		}
	}
	m_dqMoveCard.push_back( pMoveCard );
}

void CPlayScr::DrawThankMove()
{
	if ( m_dqMoveCard.empty() )
		return; 

	MOVECARD *pMoveCard = m_dqMoveCard[0];

	if ( ( pMoveCard->eMoveType == eCARDMOVE_STRAIGHT ) 
		&& ( pMoveCard->eType > 0 ) 
		&& ( pMoveCard->eType < 3 )
		&& ( !pMoveCard->dqCard.empty() ) )
	{
		int nSize = pMoveCard->dqCard.size();
		DWORD dwCurTime = timeGetTime();
		DWORD dwTime = (DWORD) abs( (int) (dwCurTime - pMoveCard->dwTime) );

		for ( int i = 0; i < nSize; i++ )
		{
			if ( pMoveCard->dqMoveFlag[i] == 0 )
			{
				if ( pMoveCard->dqDelay[i] <= dwTime )
				{
					pMoveCard->dqMoveFlag[i] = 1;
					pMoveCard->dqTime[i] = dwCurTime;

					if ( FALSE == IsMyPracticeGameObserver() )
						NMBASE::SOUND::g_Sound.PlayWav(SND09);
				}
			}
		}
		for ( int i = 0; i < nSize; i++ )
		{
			if ( pMoveCard->dqMoveFlag[i] == 1 )
			{
				BYTE cCI = pMoveCard->dqCard[i];
				int nX = pMoveCard->dqStart[i].x;
				int nY = pMoveCard->dqStart[i].y;
				m_pPage->PutSprAuto( nX, nY, &g_sprSmallCard, cCI );

				pMoveCard->dqMoveFlag[i] = 2;

				if ( pMoveCard->cDumpCard != cCI )
				{
					if ( !g_ObserverMan.IsMyObserver() && pMoveCard->nPN == 0 )
					{
						if ( cCI >= 60 && cCI < 120 )
						{
							cCI = CARD_JOKER1;
						}
						else if ( cCI >= 120 && cCI < 180 )
						{
							cCI = CARD_JOKER2;
						}

						for ( int j = 0; j < 8; j++ )
						{
							if ( m_PlayerCard[0][j] == cCI )
							{
								int nFace = cCI / 13;
								int nIndex = cCI % 13;
								if ( cCI != CARD_JOKER1 && cCI != CARD_JOKER2 )
								{
									m_MyCard[nIndex][nFace] = false;
									m_PlayerCard[0][j] = -1;
								}
								else
								{
									m_JokerCard[cCI-CARD_JOKER1] = 0;
									m_PlayerCard[0][j] = -1;
								}
								break;
							}
						}
					}
					else
					{
						int pnum = pMoveCard->nPN;
						//상대패 안보이는 문제 파악
						g_PLAYER(pnum)->SetCardCount( g_PLAYER(pnum)->GetCardCount() - 1 );
					}
				}
				else
				{
					m_nDisCardCnt --;
					if( m_nDisCardCnt < 0 ) m_nDisCardCnt = 0;
				}
			}
			else if ( pMoveCard->dqMoveFlag[i] == 2 )
			{
				DWORD dwTime1 = abs( (int) ( dwCurTime - pMoveCard->dqTime[i]) );
				BYTE cCI = pMoveCard->dqCard[i];
				if ( cCI >= 60 && cCI < 120 )
				{
					cCI = CARD_JOKER1;
				}
				else if ( cCI >= 120 && cCI < 180 )
				{
					cCI = CARD_JOKER2;
				}

				int nX = pMoveCard->dqStart[i].x + (int)(pMoveCard->dqAddx[i]*dwTime1);
				int nY = pMoveCard->dqStart[i].y + (int)(pMoveCard->dqAddy[i]*dwTime1);

				BOOL bFinish = FALSE;
				if ( pMoveCard->dqAddx[i] > 0 )
				{
					if ( nX >= pMoveCard->dqDest[i].x )
					{
						bFinish = TRUE;
					}
				}
				else 
				{
					if ( nX < pMoveCard->dqDest[i].x )
					{
						bFinish = TRUE;
					}
				}
				if ( !bFinish )
				{
					if ( pMoveCard->dqAddy[i] > 0 )
					{
						if ( nY >= pMoveCard->dqDest[i].y )
						{
							bFinish = TRUE;
						}
					}
					else
					{
						if ( nY < pMoveCard->dqDest[i].y )
						{
							bFinish = TRUE;
						}
					}
				}

				if ( !bFinish )
				{
					m_pPage->PutSprAuto( nX, nY, &g_sprBigCard, cCI );
				}
				else
				{
					m_pPage->PutSprAuto( pMoveCard->dqDest[i].x, pMoveCard->dqDest[i].y, &g_sprBigCard, cCI );

					pMoveCard->dqMoveFlag[i] = 3;

					BYTE cCI0 = pMoveCard->dqCard[i];

					int nFace;
					int nIndex;
					int nJokerType = 0; // 0:보통, 1:CARD_JOKER1, 2:CARD_JOKER2

					if ( cCI0 >= 60 && cCI0 < 120 )
					{
						nFace = (cCI0-60) / 13;
						nIndex = (cCI0-60) % 13;
						nJokerType = 1;
					}
					else if ( cCI0 >= 120 && cCI0 < 180 )
					{
						nFace = (cCI0-120) / 13;
						nIndex = (cCI0-120) % 13;
						nJokerType = 2;
					}
					else
					{
						nFace = cCI0 / 13;
						nIndex = cCI0 % 13;
					}					

					if ( pMoveCard->eType == eCARDEVENT_THANK )
					{
						m_CenterCard[nFace][nIndex] = nJokerType + 1;
						m_dqNewCard.push_back( cCI );
						for ( int m = 0; m < 8; m++ )
						{
// 							if ( m_MagicCard[m] == cCI )
// 							{
// 								m_MagicCard[m] = -1;
// 								m_CenterCard[nFace][nIndex] = 1;
// 								break;
//							}
						}
					}
					else if ( pMoveCard->eType == eCARDEVENT_REGIST )
					{
						m_SameNumCard[nIndex][nFace] = nJokerType + 1;
						m_dqNewCard.push_back( cCI );
					}
					if ( FALSE == IsMyPracticeGameObserver() )
						NMBASE::SOUND::g_Sound.PlayWav( SND10 );
				}
			}
		}

		BOOL bAllFinish = TRUE;
		for ( int i = 0; i < nSize; i++ )
		{
			if ( pMoveCard->dqMoveFlag[i] != 3 )
			{
				bAllFinish = FALSE;
			}
		}
		if ( bAllFinish )
		{
			m_bWind = FALSE;

			int pnum = pMoveCard->nPN;

			std::deque< MOVECARD * >::iterator Iter = std::find( m_dqMoveCard.begin(), m_dqMoveCard.end(), pMoveCard );
			if ( Iter != m_dqMoveCard.end() )
			{
				m_dqMoveCard.erase( Iter );
				delete pMoveCard;
			}

			// messasge 보냄
			if ( !g_ObserverMan.IsMyObserver() )
			{
				CCL_ENDCARDMOVE msg;
				msg.Set( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, 1 );
				NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			}

			if ( !g_ObserverMan.IsMyObserver() && pnum == 0 )
			{
				BYTEDQ dqSort;

				for ( int j = 0; j < 8; j++ )
				{
					if ( m_PlayerCard[0][j] >= 0 )
					{
						dqSort.push_back( m_PlayerCard[0][j] );
					}
				}

				memset( m_PlayerCard[0], -1, 8 );
				for ( int j = 0; j < dqSort.size(); j++ )
				{
					m_PlayerCard[0][j] = dqSort[j];
				}
				g_PLAYER(0)->SetCardCount( dqSort.size() );
			}

			m_nTurnIndex = 1;

			ResetLimitTime();
		}
	}
}

void CPlayScr::SetMoveRegCard( CSV_REGRESULT* pMsg )
{
	m_bWind = TRUE;
	SetNullButton();

	NMBASE::SOUND::g_Sound.PlayWav( SND31 );

	int pnum = m_nTurnCnt - g_PLAYER(0)->ServPNum;
	if( pnum < 0 ) pnum += MAX_PLAYER;

	MOVECARD *pMoveCard = new MOVECARD;
	
	pMoveCard->eMoveType = eCARDMOVE_TOGETHER;
	pMoveCard->eType = eCARDEVENT_GET;
	pMoveCard->nPN = pnum;
	pMoveCard->dwTime = timeGetTime();
	pMoveCard->cDumpCard = -1;

	BYTE card;
	POINT ptStart, ptDest;
	int nCardNum = g_PLAYER(pnum)->GetCardCount();
	int nTotalCount = 0;

	CopyMemory( m_SameCard, m_SameNumCard, sizeof(int) * MAX_CARD_FACE * MAX_CARD_VALUE );

	BOOL bSex = g_PLAYER(pnum)->UI.Sex;
	int nSndFxKind = g_PLAYER(pnum)->nSndFxKind;
	CONFIG cnfig = g_Config;

	if( !g_PLAYER(pnum)->m_bRegist ) 
	{
		PlayVoice( g_PLAYER(pnum)->UI.Sex, g_PLAYER(pnum)->nSndFxKind, REGIST );
		g_PLAYER(pnum)->m_dwRegist = timeGetTime();
	}

	g_PLAYER(pnum)->m_bRegist = TRUE;

	int nCount = pMsg->RC->nSevenCount;
	for ( int i = 0; i < nCount; i++ )
	{
		card = pMsg->RC->arrSevenCardValue[i];
		if ( !g_ObserverMan.IsMyObserver() && pnum == 0 )
		{
			GetMyStartPoint( card, ptStart );
		}
		else
		{
			GetOtherStartPoint( pnum, nCardNum, ptStart );
			nCardNum--;
		}
#ifdef _DEBUG
	if ( !((card >= 0 && card <= CARD_JOKER1) || (card >= 60 && card < 120) || (card >= 120 && card < 180)) )
	{
		CString str;
		str.Format( "잘못된 카드 값입니다 -%d", card );
		AfxMessageBox( str );
	}
#endif
		GetStraightDestPos( card, ptDest );

		pMoveCard->dqCard.push_back( card );
		pMoveCard->dqStart.push_back( ptStart );
		pMoveCard->dqDest.push_back( ptDest );
		pMoveCard->dqDelay.push_back( nTotalCount * 100 );
		pMoveCard->dqType.push_back( 1 );
		pMoveCard->dqMoveFlag.push_back( 0 );
		pMoveCard->dqTime.push_back( 0 );
		nTotalCount++;
	}

	nCount = pMsg->RC->nStCount[0];
	for ( int i = 0; i < nCount; i++ )
	{
		card = pMsg->RC->StraightCard[0][i];
		if ( !g_ObserverMan.IsMyObserver() && pnum == 0 )
		{
			GetMyStartPoint( card, ptStart );
		}
		else
		{
			GetOtherStartPoint( pnum, nCardNum, ptStart );
			nCardNum--;
		}
#ifdef _DEBUG
	if ( !((card >= 0 && card <= CARD_JOKER1) || (card >= 60 && card < 120) || (card >= 120 && card < 180)) )
	{
		CString str;
		str.Format( "잘못된 카드 값입니다 -%d", card );
		AfxMessageBox( str );
	}
#endif
		GetStraightDestPos( card, ptDest );

		pMoveCard->dqCard.push_back( card );
		pMoveCard->dqStart.push_back( ptStart );
		pMoveCard->dqDest.push_back( ptDest );
		pMoveCard->dqDelay.push_back( nTotalCount * 100 );
		pMoveCard->dqType.push_back( 1 );
		pMoveCard->dqMoveFlag.push_back( 0 );
		pMoveCard->dqTime.push_back( 0 );
		nTotalCount++;
	}
	nCount = pMsg->RC->nStCount[1];
	for ( int i = 0; i < nCount; i++ )
	{
		card = pMsg->RC->StraightCard[1][i];
		if ( !g_ObserverMan.IsMyObserver() && pnum == 0 )
		{
			GetMyStartPoint( card, ptStart );
		}
		else
		{
			GetOtherStartPoint( pnum, nCardNum, ptStart );
			nCardNum--;
		}
#ifdef _DEBUG
	if ( !((card >= 0 && card <= CARD_JOKER1) || (card >= 60 && card < 120) || (card >= 120 && card < 180)) )
	{
		CString str;
		str.Format( "잘못된 카드 값입니다 -%d", card );
		AfxMessageBox( str );
	}
#endif
		GetStraightDestPos( card, ptDest );

		pMoveCard->dqCard.push_back( card );
		pMoveCard->dqStart.push_back( ptStart );
		pMoveCard->dqDest.push_back( ptDest );
		pMoveCard->dqDelay.push_back( nTotalCount * 100 );
		pMoveCard->dqType.push_back( 1 );
		pMoveCard->dqMoveFlag.push_back( 0 );
		pMoveCard->dqTime.push_back( 0 );
		nTotalCount++;
	}

	BYTEDQ dqCard;
	std::deque< POINT > dqDest;

	for ( int i = 0; i < 2; i++ )
	{
		dqCard.clear();
		dqDest.clear();
		nCount = pMsg->RC->nToCount[i];
		if ( nCount > 0 )
		{
			for ( int j = 0; j < nCount; j++ )
			{
				card = pMsg->RC->TogetherCard[i][j];
				dqCard.push_back( card ); 
				if ( !g_ObserverMan.IsMyObserver() && pnum == 0 )
				{
					GetMyStartPoint( card, ptStart );
				}
				else
				{
					GetOtherStartPoint( pnum, nCardNum, ptStart );
					nCardNum--;
				}
				pMoveCard->dqStart.push_back( ptStart );
			}
#ifdef _DEBUG
	if ( !((card >= 0 && card < CARD_JOKER1) || (card >= 60 && card < 120) || (card >= 120 && card < 180)) )
	{
		CString str;
		str.Format( "잘못된 카드 값입니다 -%d", card );
		AfxMessageBox( str );
	}
#endif
			GetTogetherDestPos( dqCard, dqDest );

			for ( int j = 0; j < nCount; j++ )
			{
				pMoveCard->dqCard.push_back( dqCard[j] );
				pMoveCard->dqDest.push_back( dqDest[j] );
				pMoveCard->dqDelay.push_back( nTotalCount * 100 );
				pMoveCard->dqType.push_back( 2 );
				pMoveCard->dqMoveFlag.push_back( 0 );
				pMoveCard->dqTime.push_back( 0 );
				nTotalCount++;
			}
		}
	}
	
	nCount = pMsg->RC->nAtSCount;
	for ( int i = 0; i < nCount; i++ )
	{
		card = pMsg->RC->AttachStCard[i];
		if ( !g_ObserverMan.IsMyObserver() && pnum == 0 )
		{
			GetMyStartPoint( card, ptStart );
		}
		else
		{
			GetOtherStartPoint( pnum, nCardNum, ptStart );
			nCardNum--;
		}
#ifdef _DEBUG
	if ( !((card >= 0 && card <= CARD_JOKER1) || (card >= 60 && card < 120) || (card >= 120 && card < 180)) )
	{
		CString str;
		str.Format( "잘못된 카드 값입니다 -%d", card );
		AfxMessageBox( str );
	}
#endif
		GetStraightDestPos( card, ptDest );

		pMoveCard->dqCard.push_back( card );
		pMoveCard->dqStart.push_back( ptStart );
		pMoveCard->dqDest.push_back( ptDest );
		pMoveCard->dqDelay.push_back( nTotalCount * 100 );
		pMoveCard->dqType.push_back( 1 );
		pMoveCard->dqMoveFlag.push_back( 0 );
		pMoveCard->dqTime.push_back( 0 );

		nTotalCount++;
	}

	nCount = pMsg->RC->nAtTCount;
	for ( int i = 0; i < nCount; i++ )
	{
		card = pMsg->RC->AttachToCard[i];
		if ( !g_ObserverMan.IsMyObserver() && pnum == 0 )
		{
			GetMyStartPoint( card, ptStart );
		}
		else
		{
			GetOtherStartPoint( pnum, nCardNum, ptStart );
			nCardNum--;
		}
#ifdef _DEBUG
	if ( !((card >= 0 && card <= CARD_JOKER1) || (card >= 60 && card < 120) || (card >= 120 && card < 180)) )
	{
		CString str;
		str.Format( "잘못된 카드 값입니다 -%d", card );
		AfxMessageBox( str );
	}
#endif
		GetTogetherDestPos( card, ptDest );

		pMoveCard->dqCard.push_back( card );
		pMoveCard->dqStart.push_back( ptStart );
		pMoveCard->dqDest.push_back( ptDest );
		pMoveCard->dqDelay.push_back( nTotalCount * 100 );
		pMoveCard->dqType.push_back( 2 );
		pMoveCard->dqMoveFlag.push_back( 0 );
		pMoveCard->dqTime.push_back( 0 );

		nTotalCount++;
	}

	double dDist = 600 / 1000;
	int nXLength, nYLength;
	double dLength;
	double dTime;

	for ( int i = 0; i < pMoveCard->dqCard.size(); i++ )
	{
		nXLength = pMoveCard->dqDest[i].x - pMoveCard->dqStart[i].x;
		nYLength = pMoveCard->dqDest[i].y - pMoveCard->dqStart[i].y;
		dLength = sqrt( (double) (nXLength*nXLength + nYLength*nYLength) );
		dTime = ( dLength * 1000 ) / DOTPERSEC;
		if ( nXLength == 0 )
		{
			pMoveCard->dqAddx.push_back( 0.0 );
		}
		else
		{
			if ( dTime != 0 )
				pMoveCard->dqAddx.push_back( nXLength / dTime );
			else
				pMoveCard->dqAddx.push_back( 0.0 );
		}

		if ( nYLength == 0 )
		{
			pMoveCard->dqAddy.push_back( 0.0 );
		}
		else
		{
			if ( dTime != 0 )
				pMoveCard->dqAddy.push_back( nYLength / dTime );
			else
				pMoveCard->dqAddy.push_back( 0.0 );
		}
	}

	m_dqMoveCard.push_back( pMoveCard );
}

void CPlayScr::GetMyStartPoint( BYTE cCard, POINT& ptStart )
{
	for ( int j = 0; j < 8; j++ )
	{
		if ( (cCard >= 120 && cCard < 180) || (cCard >= 220 && cCard < 233) )
		{
			cCard = CARD_JOKER2;
		}
		else if ( (cCard >= 60 && cCard < 120) || (cCard >= 200 && cCard < 213) )
		{
			cCard = CARD_JOKER1;
		}

		if ( cCard == m_PlayerCard[0][j] )
		{
			ptStart.x = CardXY[0][0] + j * 20;
			ptStart.y = CardXY[0][1];
			return;
		}
	}
}


void CPlayScr::GetOtherStartPoint( int pnum, int nCardNum, POINT& ptStart )
{
	int dist2 = 11;

	if ( pnum == 0 )
		dist2 = 20;

	ptStart.x = CardXY[pnum][0] + (nCardNum - 1) * dist2;
	ptStart.y = CardXY[pnum][1];
}


void CPlayScr::DrawRegMove()
{
	if ( m_dqMoveCard.empty() )
		return; 

	MOVECARD *pMoveCard = m_dqMoveCard[0];

	if ( pMoveCard->eMoveType == eCARDMOVE_TOGETHER && pMoveCard->eType == 3 && !pMoveCard->dqCard.empty() )
	{
		int nSize = pMoveCard->dqCard.size();
		DWORD dwCurTime = timeGetTime();
		DWORD dwTime = abs( (int) ( dwCurTime - pMoveCard->dwTime ) );

		for ( int i = 0; i < nSize; i++ )
		{
			if ( pMoveCard->dqMoveFlag[i] == 0 )
			{
				if ( pMoveCard->dqDelay[i] <= dwTime )
				{
					pMoveCard->dqMoveFlag[i] = 1;
					pMoveCard->dqTime[i] = dwCurTime;

					if ( FALSE == IsMyPracticeGameObserver() )
						NMBASE::SOUND::g_Sound.PlayWav(SND09);
				}
			}
		}
		for ( int i = 0; i < nSize; i++ )
		{
			if ( pMoveCard->dqMoveFlag[i] == 1 )
			{
				BYTE cCI = pMoveCard->dqCard[i];
				int nX = pMoveCard->dqStart[i].x;
				int nY = pMoveCard->dqStart[i].y;
				m_pPage->PutSprAuto( nX, nY, &g_sprBigCard, cCI );

				pMoveCard->dqMoveFlag[i] = 2;

				if ( !g_ObserverMan.IsMyObserver() && pMoveCard->nPN == 0 )
				{
					if ( cCI >= 60 && cCI < 120 )
					{
						cCI = CARD_JOKER1;
					}
					else if ( cCI >= 120 && cCI < 180 )
					{
						cCI = CARD_JOKER2;
					}

					for ( int j = 0; j < 8; j++ )
					{
						if ( m_PlayerCard[0][j] == cCI )
						{
							int nFace = cCI / 13;
							int nIndex = cCI % 13;
							if ( cCI != CARD_JOKER1 && cCI != CARD_JOKER2 )
							{
								m_MyCard[nIndex][nFace] = false;
								m_PlayerCard[0][j] = -1;
							}
							else
							{
								m_JokerCard[cCI-CARD_JOKER1] = 0;
								m_PlayerCard[0][j] = -1;
							}
							break;
						}
					}
				}
				else
				{
					int pnum = pMoveCard->nPN;
					//상대패 안보이는 문제 파악(상대패)
					g_PLAYER(pnum)->SetCardCount( g_PLAYER(pnum)->GetCardCount() - 1 );
				}
			}
			else if ( pMoveCard->dqMoveFlag[i] == 2 )
			{
				DWORD dwTime1 = abs( (int) ( dwCurTime - pMoveCard->dqTime[i] ) );
				BYTE cCI = pMoveCard->dqCard[i];
				if ( cCI >= 60 && cCI < 120 )
				{
					cCI = CARD_JOKER1;
				}
				else if ( cCI >= 120 && cCI < 180 )
				{
					cCI = CARD_JOKER2;
				}

				int nX = pMoveCard->dqStart[i].x + (int)(pMoveCard->dqAddx[i]*dwTime1);
				int nY = pMoveCard->dqStart[i].y + (int)(pMoveCard->dqAddy[i]*dwTime1);

				BOOL bFinish = FALSE;
				if ( pMoveCard->dqAddx[i] > 0 )
				{
					if ( nX >= pMoveCard->dqDest[i].x )
					{
						bFinish = TRUE;
					}
				}
				else 
				{
					if ( nX < pMoveCard->dqDest[i].x )
					{
						bFinish = TRUE;
					}
				}
				if ( !bFinish )
				{
					if ( pMoveCard->dqAddy[i] > 0 )
					{
						if ( nY >= pMoveCard->dqDest[i].y )
						{
							bFinish = TRUE;
						}
					}
					else
					{
						if ( nY < pMoveCard->dqDest[i].y )
						{
							bFinish = TRUE;
						}
					}
				}

				if ( !bFinish )
				{
					m_pPage->PutSprAuto( nX, nY, &g_sprBigCard, cCI );
				}
				else
				{
					m_pPage->PutSprAuto( pMoveCard->dqDest[i].x, pMoveCard->dqDest[i].y, &g_sprBigCard, cCI );

					pMoveCard->dqMoveFlag[i] = 3;

					BYTE cCI0 = pMoveCard->dqCard[i];

					int nFace;
					int nIndex;
					int nJokerType = 0; // 0:보통, 1:CARD_JOKER1, 2:CARD_JOKER2

					if ( cCI0 >= 60 && cCI0 < 120 )
					{
						nFace = (cCI0-60) / 13;
						nIndex = (cCI0-60) % 13;
						nJokerType = 1;
					}
					else if ( cCI0 >= 120 && cCI0 < 180 )
					{
						nFace = (cCI0-120) / 13;
						nIndex = (cCI0-120) % 13;
						nJokerType = 2;
					}
					else
					{
						nFace = cCI0 / 13;
						nIndex = cCI0 % 13;
					}					

					if ( pMoveCard->dqType[i] == 1 )
					{
						m_CenterCard[nFace][nIndex] = nJokerType + 1;
						m_dqNewCard.push_back( cCI );

						for ( int m = 0; m < 8; m++ )
						{
// 							if ( m_MagicCard[m] == cCI )
// 							{
// 								m_MagicCard[m] = -1;
// 								m_CenterCard[nFace][nIndex] = 1;
// 								break;
// 							}
						}
					}
					else if ( pMoveCard->dqType[i] == 2 )
					{
						if ( m_SameNumCard[nIndex][nFace] != 1 )
							m_SameNumCard[nIndex][nFace] = nJokerType + 1;

						m_dqNewCard.push_back( cCI );
					}
				}
			}
		}

		BOOL bAllFinish = TRUE;
		for ( int i = 0; i < nSize; i++ )
		{
			if ( pMoveCard->dqMoveFlag[i] != 3 )
			{
				bAllFinish = FALSE;
			}
		}
		if ( bAllFinish )
		{
			m_bWind = FALSE;

			int pnum = pMoveCard->nPN;

			std::deque< MOVECARD * >::iterator Iter = std::find( m_dqMoveCard.begin(), m_dqMoveCard.end(), pMoveCard );
			if ( Iter != m_dqMoveCard.end() )
			{
				m_dqMoveCard.erase( Iter );
				delete pMoveCard;
			}

			// messasge 보냄
			if ( !g_ObserverMan.IsMyObserver() )
			{
				CCL_ENDCARDMOVE msg;
				msg.Set( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, 2 );
				NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			}

			if ( !g_ObserverMan.IsMyObserver() && pnum == 0 )
			{
				std::deque< char > dqSort;

				for ( int j = 0; j < 8; j++ )
				{
					if ( m_PlayerCard[0][j] >= 0 )
					{
						dqSort.push_back( m_PlayerCard[0][j] );
					}
				}

				memset( m_PlayerCard[0], -1, 8 );
				for ( int j = 0; j < dqSort.size(); j++ )
				{
					m_PlayerCard[0][j] = dqSort[j];
				}
				g_PLAYER(0)->SetCardCount( dqSort.size() );
			}

			m_nTurnIndex = 1;
		}
	}
}


void CPlayScr::SetMoveGetCard( char cCardIndex, int nPnum )
{
	ResetMoveCard();
	m_bWind = TRUE;

	if ( FALSE == IsMyPracticeGameObserver() )
		NMBASE::SOUND::g_Sound.PlayWav( SND08 );

//	int pnum = m_nTurnCnt - g_PLAYER(0)->ServPNum;
	int pnum = g_GAMEPROCESS()->GetPNum_ByServPN(nPnum);
	if( pnum < 0 ) pnum += MAX_PLAYER;

	POINT ptStart, ptDest;
	BYTEDQ dqCard;
	
	if ( g_RI.FormKind == hoola::eFORM_KIND_JOKER )
	{
		if ( cCardIndex < 0 || cCardIndex > CARD_JOKER2 )
		{
			dqCard.push_back( 54 );
		}
		else
		{
			dqCard.push_back( cCardIndex );
		}
	}
	else
	{
		if ( cCardIndex < 0 || cCardIndex > 51 )
		{
			dqCard.push_back( 54 );
		}
		else
		{
			dqCard.push_back( cCardIndex );
		}
	}

	int nCardNum = g_PLAYER(pnum)->GetCardCount();

	ptStart.x = 464;
	ptStart.y = 300;
	if ( pnum == 0 )
	{
		ptDest.x = CardXY[0][0] + nCardNum * 20;
		ptDest.y = CardXY[0][1];
	}
	else
	{
		ptDest.x = CardXY[pnum][0] + nCardNum * 11;
		ptDest.y = CardXY[pnum][1];
	}

	m_MoveCard.eType = eCARDEVENT_GET;
	m_MoveCard.nPN = pnum;
	m_MoveCard.dqCard = dqCard;
	m_MoveCard.dqStart.push_back( ptStart );
	m_MoveCard.dqDest.push_back( ptDest );
	m_MoveCard.dwTime = timeGetTime();
	m_MoveCard.dqTime.push_back( 0 );
	m_MoveCard.dqMoveFlag.push_back( 0 );
	m_MoveCard.dqDelay.push_back( 0 );
	
	int nXLength, nYLength;
	double dLength;
	double dTime;

	for ( int i = 0; i < m_MoveCard.dqCard.size(); i++ )
	{
		nXLength = m_MoveCard.dqDest[i].x - m_MoveCard.dqStart[i].x;
		nYLength = m_MoveCard.dqDest[i].y - m_MoveCard.dqStart[i].y;
		dLength = sqrt( (double) ( nXLength*nXLength + nYLength*nYLength ) );
		dTime = ( dLength * 1000 ) / DOTPERSEC;
		if ( nXLength == 0 )
		{
			m_MoveCard.dqAddx.push_back( 0.0 );
		}
		else
		{
			if ( dTime != 0 )
				m_MoveCard.dqAddx.push_back( nXLength / dTime );
			else
				m_MoveCard.dqAddx.push_back( 0.0 );
		}

		if ( nYLength == 0 )
		{
			m_MoveCard.dqAddy.push_back( 0.0 );
		}
		else
		{
			if ( dTime != 0 )
				m_MoveCard.dqAddy.push_back( nYLength / dTime );
			else
				m_MoveCard.dqAddy.push_back( 0.0 );
		}
	}
}


void CPlayScr::DrawGetMove()
{
	// 카드가 이동하면서 생기는 상황 그리기
	// dqMoveFlag값에 해당하는 형태로 카드가 이동

	if ( m_MoveCard.eType == eCARDEVENT_GET && !m_MoveCard.dqCard.empty() )
	{
		int nSize = m_MoveCard.dqCard.size();
		DWORD dwCurTime = timeGetTime();
		DWORD dwTime = (DWORD)abs( (int)(dwCurTime - m_MoveCard.dwTime) );

		for ( int i = 0; i < nSize; i++ )
		{
			if ( m_MoveCard.dqMoveFlag[i] == 0 )
			{
				if ( m_MoveCard.dqDelay[i] <= dwTime )
				{
					m_MoveCard.dqMoveFlag[i] = 1;
					m_MoveCard.dqTime[i] = dwCurTime;

					if ( FALSE == IsMyPracticeGameObserver() )
						NMBASE::SOUND::g_Sound.PlayWav(SND09);
				}
			}
		}
		for ( int i = 0; i < nSize; i++ )
		{
			if ( m_MoveCard.dqMoveFlag[i] == 1 )
			{
				m_nCurStockCnt--;
				m_MoveCard.dqMoveFlag[i] = 2;

				char cCI = m_MoveCard.dqCard[i];
				int nX = m_MoveCard.dqStart[i].x;
				int nY = m_MoveCard.dqStart[i].y;
				m_pPage->PutSprAuto( nX, nY, &g_sprBigCard, cCI );
			}
			else if ( m_MoveCard.dqMoveFlag[i] == 2 )
			{
				DWORD dwTime1 = abs((int)( dwCurTime - m_MoveCard.dqTime[i] ) );
				char cCI = m_MoveCard.dqCard[i];
				int nX = m_MoveCard.dqStart[i].x + (int)(m_MoveCard.dqAddx[i]*dwTime1);
				int nY = m_MoveCard.dqStart[i].y + (int)(m_MoveCard.dqAddy[i]*dwTime1);

				BOOL bFinish = FALSE;
				if ( m_MoveCard.dqAddx[i] > 0 )
				{
					if ( nX >= m_MoveCard.dqDest[i].x )
					{
						bFinish = TRUE;
					}
				}
				else 
				{
					if ( nX < m_MoveCard.dqDest[i].x )
					{
						bFinish = TRUE;
					}
				}
				if ( !bFinish )
				{
					if ( m_MoveCard.dqAddy[i] > 0 )
					{
						if ( nY >= m_MoveCard.dqDest[i].y )
						{
							bFinish = TRUE;
						}
					}
					else
					{
						if ( nY < m_MoveCard.dqDest[i].y )
						{
							bFinish = TRUE;
						}
					}
				}

				if ( !bFinish )
				{
					m_pPage->PutSprAuto( nX, nY, &g_sprBigCard, cCI );
				}
				else
				{
					m_pPage->PutSprAuto( m_MoveCard.dqDest[i].x, m_MoveCard.dqDest[i].y, &g_sprBigCard, cCI );

					m_MoveCard.dqMoveFlag[i] = 3;
					int pnum = m_MoveCard.nPN;

					if ( !g_ObserverMan.IsMyObserver() && pnum == 0 )
					{
						if ( (g_RI.FormKind == hoola::eFORM_KIND_JOKER) && ( cCI == CARD_JOKER1 || cCI == CARD_JOKER2 ) )
						{
							m_JokerCard[cCI - CARD_JOKER1] = 1;
							int cnt = g_PLAYER(0)->GetCardCount();
							m_PlayerCard[0][cnt] = cCI;
						}
						else
						{
							int cnt = g_PLAYER(0)->GetCardCount();
							m_MyCard[cCI%13][cCI/13] = true;
							m_PlayerCard[0][cnt] = cCI;
						}
					}
					//에러코드 - 카드합이 9가 될 수 없는데 간혹 이쪽에서 9를 만든다.
					g_PLAYER(pnum)->SetCardCount( g_PLAYER(pnum)->GetCardCount() + 1 );
				}
			}
		}

		BOOL bAllFinish = TRUE;
		for ( int i = 0; i < nSize; i++ )
		{
			if ( m_MoveCard.dqMoveFlag[i] != 3 )
			{
				bAllFinish = FALSE;
			}
		}
		if ( bAllFinish )
		{
			m_bWind = FALSE;

			int pnum = m_MoveCard.nPN;
			// messasge 보냄
			if ( !g_ObserverMan.IsMyObserver() )
			{
				CCL_ENDCARDMOVE msg;
				msg.Set( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, 3 );
				NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			}

			ResetMoveCard();
		}
	}
}


void CPlayScr::SetMoveDumpCard( char cCardIndex, char cTN, int nPnum)
{
#ifdef _DEBUG
	if (nPnum == GM().GetMyInfo()->ServPNum)
	{
		if (m_nDiscardTest != (int) cCardIndex )
		{			
			CString str;
			str.Format("버그)ID:%s 문제:카드 불일치 에러\n",GM().GetMyInfo()->UI.ID);
			AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
		}
	}	
#endif

	ResetMoveCard();
	m_bWind = TRUE;


	if ( FALSE == IsMyPracticeGameObserver() )
		NMBASE::SOUND::g_Sound.PlayWav( SND09 );

	int pnum = nPnum - g_PLAYER(0)->ServPNum;

	if ( pnum < 0 )
	{
		pnum += MAX_PLAYER;
	}

	POINT ptStart, ptDest;
	BYTEDQ dqCard;
	if ( (g_RI.FormKind == hoola::eFORM_KIND_JOKER) )
	{
		if ( cCardIndex < 0 || cCardIndex > CARD_JOKER2 )
		{
			dqCard.push_back( 54 );
		}
		else
		{
			dqCard.push_back( cCardIndex );
		}
	}
	else
	{
		if ( cCardIndex < 0 || cCardIndex > 51 )
		{
			dqCard.push_back( 54 );
		}
		else
		{
			dqCard.push_back( cCardIndex );
		}
	}

	int nCardNum = g_PLAYER(pnum)->GetCardCount();

	ptDest.x = 464;
	ptDest.y = 344;

	if ( !g_ObserverMan.IsMyObserver() && pnum == 0 )
	{
		// 관전자가 아니고 내 플레이어번호가 0일경우
		GetMyStartPoint( cCardIndex, ptStart );
	}
	else
	{
		GetOtherStartPoint( pnum, nCardNum, ptStart );
	}

	m_MoveCard.eType = eCARDEVENT_DUMP;
	m_MoveCard.nPN = pnum;
	m_MoveCard.dqCard = dqCard;
	m_MoveCard.dqStart.push_back( ptStart );
	m_MoveCard.dqDest.push_back( ptDest );
	m_MoveCard.dwTime = timeGetTime();
	m_MoveCard.dqTime.push_back( 0 );
	m_MoveCard.dqMoveFlag.push_back( 0 );
	m_MoveCard.dqDelay.push_back( 0 );
	m_MoveCard.ThankNum = cTN;
	
	int nXLength, nYLength;
	double dLength;
	double dTime;

	for ( int i = 0; i < m_MoveCard.dqCard.size(); i++ )
	{
		nXLength = m_MoveCard.dqDest[i].x - m_MoveCard.dqStart[i].x;
		nYLength = m_MoveCard.dqDest[i].y - m_MoveCard.dqStart[i].y;
		dLength = sqrt( (double) ( nXLength*nXLength + nYLength*nYLength ) );
		dTime = ( dLength * 1000 ) / DOTPERSEC;
		if ( nXLength == 0 )
		{
			m_MoveCard.dqAddx.push_back( 0.0 );
		}
		else
		{
			if ( dTime != 0 )
				m_MoveCard.dqAddx.push_back( nXLength / dTime );
			else
				m_MoveCard.dqAddx.push_back( 0.0 );
		}

		if ( nYLength == 0 )
		{
			m_MoveCard.dqAddy.push_back( 0.0 );
		}
		else
		{
			if ( dTime != 0 )
				m_MoveCard.dqAddy.push_back( nYLength / dTime );
			else
				m_MoveCard.dqAddy.push_back( 0.0 );
		}
	}
}


void CPlayScr::DrawDumpMove()
{
	if ( m_MoveCard.eType == eCARDEVENT_DUMP && !m_MoveCard.dqCard.empty() )
	{
		char cCI;
		int nSize = m_MoveCard.dqCard.size();
		DWORD dwCurTime = timeGetTime();
		DWORD dwTime = abs( (int) (dwCurTime - m_MoveCard.dwTime) );

		for ( int i = 0; i < nSize; i++ )
		{
			if ( m_MoveCard.dqMoveFlag[i] == 0 )
			{
				if ( m_MoveCard.dqDelay[i] <= dwTime )
				{
					m_MoveCard.dqMoveFlag[i] = 1;
					m_MoveCard.dqTime[i] = dwCurTime;

					if ( FALSE == IsMyPracticeGameObserver() )
						NMBASE::SOUND::g_Sound.PlayWav(SND09);
				}
			}
		}
		for ( int i = 0; i < nSize; i++ )
		{
			if ( m_MoveCard.dqMoveFlag[i] == 1 )
			{
				int pnum = m_MoveCard.nPN;
				cCI = m_MoveCard.dqCard[i];

				if(  !g_ObserverMan.IsMyObserver() && pnum == 0 )
				{
					if ( (g_RI.FormKind == hoola::eFORM_KIND_JOKER) && ( cCI == CARD_JOKER1 || cCI == CARD_JOKER2 ) )
					{
						m_JokerCard[cCI - CARD_JOKER1] = 0;
					}
					else
					{
						m_MyCard[cCI%13][cCI/13] = false;
					}

					std::deque< char > dqSort;
					std::deque< char >::iterator EraseIter;

					for ( int j = 0; j < 8; j++ )
					{
						if ( m_PlayerCard[0][j] >= 0 )
						{
							dqSort.push_back( m_PlayerCard[0][j] );
						}
					}

					EraseIter = std::find( dqSort.begin(), dqSort.end(), cCI );
					if ( EraseIter != dqSort.end() )
					{
						dqSort.erase( EraseIter );
					}

					memset( m_PlayerCard[0], -1, 8 );
					for ( int j = 0; j < dqSort.size(); j++ )
					{
						m_PlayerCard[0][j] = dqSort[j];
					}

					m_PlayerCard[0][7] = -1;
				}
// 				//상대패 안보이는 문제 파악
// 				if ( pnum == 0 )
// 				{
// 					g_PLAYER(pnum)->SetCardCount( g_PLAYER(pnum)->GetCardCount() - 1 );
// 				}
// 				else
// 				{
// 					g_PLAYER(pnum)->SetCardCount( g_PLAYER(pnum)->GetCardCount() - 1 );
// 				}
				//이부분은 카드를 버릴때 사용자 카드 갯수를 1감소시키는 코드인데 SetAddedCardData랑 중복되는 과정에서 오류가 있어서 주석 처리했다
				//하지만 연습게임 사용자가 버리기를 시도할때 SetAddedCardData가 호출되지 않아 카드가 쌓이고 있다 그래서 연습게임 사용자일경우 강제로 카드를 줄여준다

				if ( strcmp(g_PLAYER(pnum)->UI.ID, "_PRACTICSUSER_" ) == 0 )
				{
					g_PLAYER(pnum)->SetCardCount( g_PLAYER(pnum)->GetCardCount() - 1 );
				}


// 				nCardCountTestStep++;
// 				CString str;
// 				int nServerPnum = g_GAMEPROCESS()->GetServPNum_ByPN(pnum);
// 				str.Format("Discard -  ServPnum :%d StepCnt %d \n",nServerPnum,nCardCountTestStep);
// 				AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));
				

				BOOL bSex = g_PLAYER(pnum)->UI.Sex;
				int nSndFxKind = g_PLAYER(pnum)->nSndFxKind;
				CONFIG cnfig = g_Config;
				
				if ( m_MoveCard.ThankNum < 0 )
				{
					if ( g_PLAYER(pnum)->GetCardCount() == 1 )
						PlayVoice( g_PLAYER(pnum)->UI.Sex, g_PLAYER(pnum)->nSndFxKind, LEFTONE );
					else if ( g_PLAYER(pnum)->GetCardCount() > 1 )
						PlayVoice( g_PLAYER(pnum)->UI.Sex, g_PLAYER(pnum)->nSndFxKind, DUMP, 50 );
				}

				m_MoveCard.dqMoveFlag[i] = 2;

				int nX = m_MoveCard.dqStart[i].x;
				int nY = m_MoveCard.dqStart[i].y;
				m_pPage->PutSprAuto( nX, nY, &g_sprBigCard, cCI );
			}
			else if ( m_MoveCard.dqMoveFlag[i] == 2 )
			{
				DWORD dwTime1 = abs( (int) (dwCurTime - m_MoveCard.dqTime[i]) );
				cCI = m_MoveCard.dqCard[i];
				int nX = m_MoveCard.dqStart[i].x + (int)(m_MoveCard.dqAddx[i]*dwTime1);
				int nY = m_MoveCard.dqStart[i].y + (int)(m_MoveCard.dqAddy[i]*dwTime1);

				BOOL bFinish = FALSE;
				if ( m_MoveCard.dqAddx[i] > 0 )
				{
					if ( nX >= m_MoveCard.dqDest[i].x )
					{
						bFinish = TRUE;
					}
				}
				else 
				{
					if ( nX < m_MoveCard.dqDest[i].x )
					{
						bFinish = TRUE;
					}
				}
				if ( !bFinish )
				{
					if ( m_MoveCard.dqAddy[i] > 0 )
					{
						if ( nY >= m_MoveCard.dqDest[i].y )
						{
							bFinish = TRUE;
						}
					}
					else
					{
						if ( nY < m_MoveCard.dqDest[i].y )
						{
							bFinish = TRUE;
						}
					}
				}

				if ( !bFinish )
				{
					m_pPage->PutSprAuto( nX, nY, &g_sprBigCard, cCI );
				}
				else
				{
					m_pPage->PutSprAuto( m_MoveCard.dqDest[i].x, m_MoveCard.dqDest[i].y, &g_sprBigCard, cCI );

					m_MoveCard.dqMoveFlag[i] = 3;

					m_DisCard[m_nDisCardCnt++] = cCI;

					if ( FALSE == IsMyPracticeGameObserver() )
						NMBASE::SOUND::g_Sound.PlayWav( SND10 );
				}
			}
		}

		BOOL bAllFinish = TRUE;
		for ( int i = 0; i < nSize; i++ )
		{
			if ( m_MoveCard.dqMoveFlag[i] != 3 )
			{
				bAllFinish = FALSE;
			}
		}
		if ( bAllFinish )
		{
			m_bWind = FALSE;

			char cCN = m_MoveCard.ThankNum;
			ResetMoveCard();

			int pnum = m_MoveCard.nPN;
			// messasge 보냄
			if ( !g_ObserverMan.IsMyObserver() )
			{
				CCL_ENDCARDMOVE msg;
				msg.Set( g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID, 4 );
				NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			}

			ResetTurnFlag();
			ResetSelect();

			m_nTurnIndex = -1;			
			ResetLimitTime();
		}
	}
}


BOOL CPlayScr::CheckAvailableB( BYTE cCardIndex )
{
	int num = cCardIndex % 13;

	int k = 0;
	for ( ; k < 4; k++ )
	{
		if ( m_SameNumCard[num][k] )
			break;
	}
	if ( k < 4 )
		return TRUE;
	else
		return FALSE;
}

void CPlayScr::DrawSelectCardSpecialCase()
{
	if ( FALSE == m_bSelectDraw)
	{
		return;
	}
	if ( !g_ObserverMan.IsMyObserver() && m_nUpCardType == 3 && m_nTurnCnt == g_PLAYER(0)->ServPNum && m_nTurnIndex == 1 )
	{
		
		if ( m_cClickCardInfo.m_dqAttachStraight.size() == 1 )
		{
// 			m_pPage->DrawMultiScene( 174 + 135 - 30, 166 + 156 - 30, &g_sprGameBackPart, 0, timeGetTime() );
// 			m_pPage->DrawMultiScene( 557 + 135 - 30, 166 + 156 - 30, &g_sprGameBackPart, 0, timeGetTime() );
			if(m_dqAttachableFakeDq.size() == 0) return;
			CPoint ptCardSelectSprPos = CPoint(337, 201);

			//카드 선택 화면 바닥 그리기
			m_pPage->DrawScene( ptCardSelectSprPos.x, ptCardSelectSprPos.y, &g_sprCardSelect, 0, timeGetTime() );

			int nSize = 33;
			int nGap[4] = { 32, 22, 16, 12 };


			//낱장조합 카드 그리기
			CPoint ptStraightHorizontalSt = CPoint(337+16, 201+48);
			int nHorizontalDist = 33;

			
			nSize = m_dqAttachableFakeDq[0].size();
			if ( nSize < 3) nHorizontalDist = 33;
			else nHorizontalDist = nGap[nSize - 3];
			//m_dqUpCard
			for( int i = 0 ; i < m_dqAttachableFakeDq[0].size() ; i++ )
			{
				m_pPage->PutSprAuto( ptStraightHorizontalSt.x + nHorizontalDist * i, ptStraightHorizontalSt.y, &g_sprBigCard,m_dqAttachableFakeDq[0][i]);
			}

			//묶음조합 카드 그리기
			CPoint ptTogetherHorizonSt = CPoint(509, 248);
			
			nHorizontalDist = 33;//22
			nSize = m_dqAttachableFakeDq[1].size();
			if ( nSize < 3) nHorizontalDist = 33;
			else nHorizontalDist = nGap[nSize - 3];
// 			if (m_dqAttachableFakeDq[1].size() == 4)
// 			{
// 				nHorizontalDist = 22;
// 			}
// 			else
// 			{
// 				nHorizontalDist = 33;
// 			}

			for( int i = 0 ; i < m_dqAttachableFakeDq[1].size() ; i++ )
			{
				m_pPage->PutSprAuto( ptTogetherHorizonSt.x + nHorizontalDist * i, ptTogetherHorizonSt.y, &g_sprBigCard, m_dqAttachableFakeDq[1][i] );
			}

			int nXY[2][2] = { 337+16, 201+48, 337+172, 201+48 };

			if ( 0 == m_nSelectCurrent )
			{
				m_pPage->DrawScene( nXY[0][0] - 8 , nXY[0][1] - 8, &g_sprCardSelect, 4, timeGetTime() );
				m_pPage->DrawScene( nXY[1][0] - 8 , nXY[1][1] - 8, &g_sprCardSelect, 3, timeGetTime() );
			}
			else if ( 1 == m_nSelectCurrent )
			{
				m_pPage->DrawScene( nXY[1][0] - 8 , nXY[1][1] - 8, &g_sprCardSelect, 4, timeGetTime() );
				m_pPage->DrawScene( nXY[0][0] - 8 , nXY[0][1] - 8, &g_sprCardSelect, 3, timeGetTime() );
			}
		}
		else if ( FALSE == m_cCardInfo.m_dqSeven.empty() )
		{
			if( m_cCardInfo.m_dqSeven.size() >= 3 )
			{
				CPoint ptCardSelectSprPos = CPoint(337, 201);

				//카드 선택 화면 바닥 그리기
				m_pPage->DrawScene( ptCardSelectSprPos.x, ptCardSelectSprPos.y, &g_sprCardSelect, 0, timeGetTime() );

				//낱장조합 카드 그리기
				CPoint ptStraightVirticalSt = CPoint(383, 248);
				int nVerticalDist = 15;

				//Clip사이즈 입력
				m_pPage->SetTempClip(383,248,383 + 89,248 + 99);

				//m_dqUpCard
				for( int i = 0 ; i < m_cCardInfo.m_dqSeven.size() ; i++ )
				{
					m_pPage->PutSprAuto( ptStraightVirticalSt.x, ptStraightVirticalSt.y + nVerticalDist * i, &g_sprBigCard, m_cCardInfo.m_dqSeven[i], TEMPCLIP);
				}

				//묶음조합 카드 그리기
				CPoint ptTogetherHorizonSt = CPoint(509, 248);
				int nHorizontalDist = 35;//22
				if (m_cCardInfo.m_dqSeven.size() == 4)
				{
					nHorizontalDist = 22;
				}
				else
				{
					nHorizontalDist = 33;
				}
				for( int i = 0 ; i < m_cCardInfo.m_dqSeven.size() ; i++ )
				{
					m_pPage->PutSprAuto( ptTogetherHorizonSt.x + nHorizontalDist * i, ptTogetherHorizonSt.y, &g_sprBigCard, m_cCardInfo.m_dqSeven[i] );
				}

				int nXY[2][2] = { 337+16, 201+48, 337+172, 201+48 };

				if ( 0 == m_nSelectCurrent )
				{
					m_pPage->DrawScene( nXY[0][0] - 8 , nXY[0][1] - 8, &g_sprCardSelect, 4, timeGetTime() );
					m_pPage->DrawScene( nXY[1][0] - 8 , nXY[1][1] - 8, &g_sprCardSelect, 3, timeGetTime() );
				}
				else if ( 1 == m_nSelectCurrent )
				{
					m_pPage->DrawScene( nXY[1][0] - 8 , nXY[1][1] - 8, &g_sprCardSelect, 4, timeGetTime() );
					m_pPage->DrawScene( nXY[0][0] - 8 , nXY[0][1] - 8, &g_sprCardSelect, 3, timeGetTime() );
				}
			}			
		}		
	}
}

void CPlayScr::DrawJackpotAni( CDC *pDC )
{
	if ( m_bJackpot )
	{
		DWORD dwTime = abs( (int)(timeGetTime() - m_dwJackpot) );

		if ( m_bJackpotSound == FALSE )
		{
			m_bJackpotSound = TRUE;
			AutoPlayMSceneSound( &g_sprJackPotAni, 4 );
		}

		if ( m_nJackPotStep == 1 )
		{
			if ( dwTime <= g_sprJackPotAni.GetMScenePlayTime( 4 ) )
			{
				m_pPage->DrawMultiScene( 0, 0, &g_sprJackPotAni, 9, dwTime );

				JACKPOTMONEY data;

				if ( dwTime >= 3600 && dwTime <= 7800 )
				{
					CString str;
					str.Format( "%s님", m_sJackpotID );
					CRect rect;
					rect.SetRect( 357, 330, 648, 344 );
					COLORREF cr = pDC->SetTextColor( RGB(0,0,0) );
					pDC->DrawText( str, &rect, DT_CENTER );
					pDC->SetTextColor( cr );
				}

				if ( dwTime <= 7800 )
				{
					for ( int i = 0; i < (int)m_dqJackpotMoney.size(); i++ )
					{
						data = m_JackpotMoneyData[i];

						if ( dwTime >= data.dwStartTime )
						{
							if ( i == 3 || i == 7 || i == 11 || i == 15 )
							{
								m_pPage->PutSprAuto( data.nX, data.nY, &g_sprJackPotAni, 119 );
							}
							else
							{
								if ( (dwTime - data.dwStartTime) <= g_sprJackPotAni.GetMScenePlayTime( 6 ) )
								{
									m_pPage->DrawMultiScene( data.nX, data.nY, &g_sprJackPotAni, 6, dwTime - data.dwStartTime );
								}
								else
								{
									m_pPage->PutSprAuto( data.nX, data.nY, &g_sprJackPotAni, m_dqJackpotMoney[i] + 109 );
								}
							}
						}
					}
				}
			}
			else
			{
				m_nTurnIndex = -1;
				ResetLimitTime();

				m_bJackpot = FALSE;
				m_nJackPotStep = 0;
				m_dwJackpot = 0;
			}
		}
	}
}


void CPlayScr::SetMission( int nMNum, int nMBonus, int nMaxM )
{
	m_bMissionStart = TRUE;
	m_nMissionNum = nMNum;
	m_dwMission = timeGetTime();
}

void CPlayScr::DrawMissionStart()
{
	if ( m_bMissionStart )
	{
		DWORD dwTime = abs( (int)(timeGetTime() - m_dwMission ));

		if ( dwTime < g_sprMission.GetMScenePlayTime( m_nMissionNum ) )
		{
			m_pPage->DrawMultiScene( 0, 0, &g_sprMission, m_nMissionNum, dwTime );
		}
		else
		{
			m_bMissionStart = FALSE;
			m_bMission = TRUE;
			m_dwMission = 0;
		}
	}
}


void CPlayScr::DrawMission(CDC *pDC)
{
	if ( m_bMission )
	{
		m_pPage->DrawMultiScene( 0, 0, &g_sprMission, m_nMissionNum, g_sprMission.GetMScenePlayTime( m_nMissionNum ) - 100 );
	}
}


void CPlayScr::CompMission( char *pID, int& GetMoney )
{
	if ( pID == NULL )
		return;

	char NickName[20] = {0,};
	for ( int i = 0; i < MAX_PLAYER; i++ )
	{
		if ( strcmp( g_PLAYER(i)->UI.ID, pID ) == 0 )
		{
			strncpy( NickName, g_PLAYER(i)->UI.NickName, 19 );
			break;
		}
	}

	m_bMissionComp = TRUE;
	m_dwMissionComp = timeGetTime();
	m_sMissionCompID.Format( "%s", NickName );
	AutoPlayMSceneSound( &g_sprMissionComp, 0 );
}


void CPlayScr::DrawMissionComp(CDC *pDC)
{
	if ( m_bMissionComp )
	{
		DWORD dwTime = abs( (int)(timeGetTime() - m_dwMissionComp ));

		if ( dwTime < g_sprMissionComp.GetMScenePlayTime( 0 ) )
		{
			m_pPage->DrawMultiScene( 0, 0, &g_sprMissionComp, 0, dwTime );
			if ( dwTime > 1500 )
			{
				COLORREF OldColor = pDC->SetTextColor( RGB( 255, 255, 255 ) );
				CString str = m_sMissionCompID + "님 성공!";
				CRect rect;
				rect.SetRect( 489, 123, 631, 135 );
				pDC->DrawText( str, &rect, DT_CENTER );
				pDC->SetTextColor( OldColor );
			}
		}	
		else
		{
			m_pPage->DrawMultiScene( 0, 0, &g_sprMissionComp, 0, g_sprMissionComp.GetMScenePlayTime( 0 ) - 1 );
			COLORREF OldColor = pDC->SetTextColor( RGB( 255, 255, 255 ) );
			CString str = m_sMissionCompID + "님 성공!";
			CRect rect;
			rect.SetRect( 489, 123, 631, 135 );
			pDC->DrawText( str, &rect, DT_CENTER );
			pDC->SetTextColor( OldColor );
		}
	}
}

void CPlayScr::DrawStartFrame()
{
	if ( m_bStartFrame )
	{
		if ( m_bGoldState )
		{
			m_bDrawGold = FALSE;
			DWORD dwTime = abs( (int)(timeGetTime() - m_dwStartFrame) );
			
			int nIndex = 0;
			if ( m_nGoldStage == 4 )
			{
				nIndex  = 2;
			}
		
			if ( dwTime < g_sprGold.GetMScenePlayTime( nIndex ) )
			{	
				m_pPage->DrawMultiScene( 0, 0, &g_sprGold, nIndex, dwTime );
			}
			else
			{
				m_pPage->DrawMultiScene( 0, 0, &g_sprGold, nIndex, g_sprGold.GetMScenePlayTime( nIndex ) - 1 );

				m_bGoldState = FALSE;
				m_dwStartFrame = timeGetTime();
				NMBASE::SOUND::g_Sound.PlayWav( SND38 );
				m_bDrawGold = TRUE;
				if ( m_nGoldStage0 == 3 )
				{
					m_nGoldStage0 = 4;
				}
			}
		}
		else
		{
			DWORD dwTime = abs( (int) (timeGetTime() - m_dwStartFrame ));
			if ( m_bEndGoldStage )
			{
				m_bDrawGold = FALSE;

				int nIndex = 4;
				if ( m_nGoldStage0 == 6 )
				{
					nIndex  = 5;
				}
			
				if ( dwTime < g_sprGold.GetMScenePlayTime( nIndex ) )
				{	
					m_pPage->DrawMultiScene( 0, 0, &g_sprGold, nIndex, dwTime );
				}
				else
				{
					m_pPage->DrawMultiScene( 0, 0, &g_sprGold, nIndex, g_sprGold.GetMScenePlayTime( nIndex ) - 1 );

					m_dwStartFrame = timeGetTime();
					m_bEndGoldStage = FALSE;
					NMBASE::SOUND::g_Sound.PlayWav( SND38 );
				}
				return;
			}
			if ( m_nStartFrameStep == 1 )
			{
				int pnum;

				int nDrawCount = dwTime / 100;

				if ( nDrawCount >= 8 )
				{
					m_nStartFrameStep = 2;
					m_dwStartFrame = timeGetTime();
					nDrawCount = 7;
				}

				for( char ch = 0; ch < MAX_PLAYER; ch++)
				{
					int dist1 = 20; 
					int dist2 = 11;

					if( g_PLAYER(ch)->UI.UNum == 0 ) continue;
					if ( g_PLAYER(ch)->PlayState != 1 ) continue;

					pnum = g_PLAYER(ch)->ServPNum - g_PLAYER(0)->ServPNum;
					if( pnum < 0 ) pnum += MAX_PLAYER;



					int ServPNum = g_PLAYER(ch)->ServPNum;

					if( pnum == 0 )
					{
						for( int i = 0; i < nDrawCount; i ++ )
						{
							if ( m_bDrawGold && m_nGoldStage0 >= 2 )
							{
								m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist1, CardXY[pnum][1], &g_sprMyCard, 55 );
							}
							else
							{
								m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist1, CardXY[pnum][1], &g_sprMyCard, 54 );
							}
						}
					}
					else
					{
						for( int i = 0; i < nDrawCount; i ++ )
						{
							if ( m_bDrawGold && m_nGoldStage0 >= 2 )
							{
								m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist2 , CardXY[pnum][1], &g_sprSmallCard, 55 );
							}
							else
							{
								m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist2 , CardXY[pnum][1], &g_sprSmallCard, 54 );
							}
						}
					}
				}
			}
			else if ( m_nStartFrameStep == 2 )
			{
				int pnum;

				int nDrawCount = dwTime / 100;

				if ( nDrawCount >= 8 )
				{
					m_bStartFrame = FALSE;
					m_nStartFrameStep = 0;
					m_dwStartFrame = 0;

					if ( !g_ObserverMan.IsMyObserver() )
					{
						CCL_WINDCARDEND msg2;
						msg2.Set(g_RI.RoomNum, g_PLAYER(0)->ServPNum, g_PLAYER(0)->UI.UNum, g_PLAYER(0)->UI.ID);//int rnum, int pnum, int unum, char *id)
						NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg2.pData, msg2.GetTotalSize());
					}
					nDrawCount = 7;
				}

				for( int ch = 0; ch < MAX_PLAYER; ch++)
				{
					int dist1 = 20; 
					int dist2 = 11;

					if( g_PLAYER(ch)->UI.UNum == 0 ) continue;
					if ( g_PLAYER(ch)->PlayState != 1 ) continue;

					pnum = g_PLAYER(ch)->ServPNum - g_PLAYER(0)->ServPNum;
					if( pnum < 0 ) pnum += MAX_PLAYER;

					int ServPNum = g_PLAYER(ch)->ServPNum;

					if( pnum == 0 )
					{
						for( int i = 0; i < 7- nDrawCount; i ++ )
						{
							if ( m_bDrawGold && m_nGoldStage0 >= 2 )
							{
								m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist1, CardXY[pnum][1], &g_sprMyCard, 55 );
							}
							else
							{
								m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist1, CardXY[pnum][1], &g_sprMyCard, 54 );
							}
						}
						for( int i = 7 - nDrawCount ; i < 7; i ++ )
						{
							if ( !g_ObserverMan.IsMyObserver() )
								m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist1, CardXY[pnum][1], &g_sprMyCard, m_PlayerCard[pnum][i] );
							else
							{
								if ( m_bDrawGold && m_nGoldStage0 >= 2 )
								{
									m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist1, CardXY[pnum][1], &g_sprMyCard, 55 );
								}
								else
								{
									m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist1, CardXY[pnum][1], &g_sprMyCard, 54 );
								}
							}
						}
					}
					else
					{
						for( int i = 0; i < 7; i ++ )
						{	
							if ( m_bDrawGold && m_nGoldStage0 >= 2 )
							{
								m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist2 , CardXY[pnum][1], &g_sprSmallCard, 55 );
							}
							else
							{
								m_pPage->PutSprAuto( CardXY[pnum][0] + i * dist2 , CardXY[pnum][1], &g_sprSmallCard, 54 );
							}
						}
					}
				}
			}
		}
	}
}


void CPlayScr::SetHelpMsg( int nType )
{	
	switch ( nType )
	{
	case eHELPMSG_THANKENABLE:
		{
			m_dwHelpTime = timeGetTime();
			m_nHelpIndex = 0;
		}
		break;
	case eHELPMSG_SELECTDUMPCARD:
		{
			m_dwHelpTime = timeGetTime();
			m_nHelpIndex = 1;
		}
		break;
	case eHELPMSG_REGISTENABLE:
		{
			m_dwHelpTime = timeGetTime();
			m_nHelpIndex = 2;
		}
		break;
	case eHELPMSG_STOPENABLE:
		{
			m_dwHelpTime = timeGetTime();
			m_nHelpIndex = 4;
		}
		break;
	case eHELPMSG_GETCARD:
		{
			m_dwHelpTime = timeGetTime();
			m_nHelpIndex = 5;
		}
		break;
	case eHELPMSG_SELECTREGPOS:
		{
			m_dwHelpTime = timeGetTime();
			m_nHelpIndex = 6;
		}
		break;
	case eHELPMSG_THANKSELECT:
		{
			m_dwHelpTime = timeGetTime();
			m_nHelpIndex = 7;
		}
		break;
	default:
		break;
	}
}

void CPlayScr::SetAddedCardData( char *pStCards, char *pToCards, char *pID, int nDisCard, char *pPlayerCard )
{
	char TempSt[MAX_CARD_FACE][MAX_CARD_VALUE];
	char TempTo[MAX_CARD_VALUE][MAX_CARD_FACE];
	CopyMemory( TempSt, pStCards, sizeof(char) * MAX_CARD_VALUE * MAX_CARD_FACE );
	CopyMemory( TempTo, pToCards, sizeof(char) * MAX_CARD_VALUE * MAX_CARD_FACE );

	BOOL bFailed = TRUE;
	for ( int i = 0; i < 4; i++ )
	{
		for ( int j = 0; j < 13; j++ )
		{
			if ( TempSt[i][j] == 1 && !m_CenterCard[i][j] )
			{
				bFailed = FALSE;
			}
			else if ( TempSt[i][j] == 0 && m_CenterCard[i][j] )
			{
				bFailed = FALSE;
			}
			m_CenterCard[i][j] = TempSt[i][j];
		}
	}
	for ( int i = 0; i < 13; i++ )
	{
		for ( int j = 0; j < 4; j++ )
		{
			if ( TempTo[i][j] == 1 && !m_SameNumCard[i][j] )
			{
				bFailed = FALSE;
			}
			else if ( TempTo[i][j] == 0 && m_SameNumCard[i][j] )
			{
				bFailed = FALSE;
			}
			m_SameNumCard[i][j] = TempTo[i][j];
		}
	}

	//////////////////////////////////////////////////////////////////////////
	///				플레이어 카드 세팅
	//////////////////////////////////////////////////////////////////////////
	std::deque< char > dqServerCard;
	std::deque< char > dqClientCard;

	for ( int i = 0; i < 8; i++ )
	{
		if ( pPlayerCard[i] != -1 )
		{
			//-1은 카드가 없음을 의미
			dqServerCard.push_back( pPlayerCard[i] );
		}
	}

	for ( int i = 0; i < MAX_PLAYER; i++ )
	{
		//모든 사용자를 대상으로 값을 확인한다.
		if ( strcmp( pID, g_PLAYER(i)->UI.ID ) == 0 )
		{
			//서버는 사용자 ID별로 값을 보낸다.
			//값을 받지 못한 사용자는 어떻게 처리되는것이지?
			if ( !g_ObserverMan.IsMyObserver() && strcmp( pID, GM().GetMyInfo()->UI.ID ) == 0 )
			{
				//나 일때
				for ( int j = 0; j < MAX_CARDCOUNT; j++ )
				{
					if ( m_PlayerCard[i][j] != -1 && m_PlayerCard[i][j] != nDisCard )
					{
						dqClientCard.push_back( m_PlayerCard[i][j] );
					}
				}

				if ( dqClientCard.size() != dqServerCard.size() )
				{
					//카드 사이즈가 불일치 할 경우
					//자신의 카드 데이터를 초기화하고 서버 데이터로 치환한다.
					m_JokerCard[0] = FALSE;
					m_JokerCard[1] = FALSE;
					ZeroMemory( m_MyCard, sizeof(BYTE)*13*4 );

					for ( int j = 0; j < dqServerCard.size(); j++ )
					{
						if ( dqServerCard[j] == CARD_JOKER1 )
						{
							m_JokerCard[0] = TRUE;
						}
						else if ( dqServerCard[j] == CARD_JOKER2 )
						{
							m_JokerCard[1] = TRUE;
						}
						else
						{
							int nFace = dqServerCard[j] / 13;
							int nNum   = dqServerCard[j] % 13;
							m_MyCard[nNum][nFace] = 1;
						}
					}
					SortMyCard();
				}
				else
				{
					std::sort( dqClientCard.begin(), dqClientCard.end() );
					std::sort( dqServerCard.begin(), dqServerCard.end() );
					for ( int j = 0; j < dqClientCard.size(); j++ )
					{
						if ( dqClientCard[j] != dqServerCard[j] )
						{
							m_JokerCard[0] = FALSE;
							m_JokerCard[1] = FALSE;
							ZeroMemory( m_MyCard, sizeof(BYTE)*13*4 );

							for ( int j = 0; j < dqServerCard.size(); j++ )
							{
								if ( dqServerCard[j] == CARD_JOKER1 )
								{
									m_JokerCard[0] = TRUE;
								}
								else if ( dqServerCard[j] == CARD_JOKER2 )
								{
									m_JokerCard[1] = TRUE;
								}
								else
								{
									int nFace = dqServerCard[j] / 13;
									int nNum   = dqServerCard[j] % 13;

									m_MyCard[nNum][nFace] = 1;
								}
							}
							SortMyCard();
							break;
						}
					}
				}
			}
			g_PLAYER(i)->SetCardCount( dqServerCard.size() );
		}
	}
}

void CPlayScr::SetFixCardData( CSV_FIXREGIST FR )
{
	//char *pID, char *pPlayerCard
	char *pID			= FR.ID;
	char *pPlayerCard	= FR.PlayerCard;
	char arrStCards[MAX_CARD_FACE][MAX_CARD_VALUE];
	char arrToCards[MAX_CARD_VALUE][MAX_CARD_FACE];

	CopyMemory( arrStCards, FR.StCards, sizeof(char) * MAX_CARD_VALUE * MAX_CARD_FACE );
	CopyMemory( arrToCards, FR.ToCards, sizeof(char) * MAX_CARD_VALUE * MAX_CARD_FACE );

	BOOL bFailed = TRUE;
	std::deque< char > dqSPCard, dqCPCard;
	
	//사용자 카드 싱크 맞추기

	for ( int i = 0 ; i < MAX_CARDCOUNT ; i++ )
	{
		if ( pPlayerCard[i] != -1 )
		{
			dqSPCard.push_back( pPlayerCard[i] );
		}
	}

	for ( int i = 0 ; i < MAX_PLAYER ; i++ )
	{
		if ( strcmp( pID, g_PLAYER(i)->UI.ID ) == 0 )
		{
			if ( strcmp( pID, GM().GetMyInfo()->UI.ID ) == 0 )
			{
				for ( int j = 0; j < MAX_CARDCOUNT ; j++ )
				{
					if ( m_PlayerCard[i][j] != -1 )
					{
						dqCPCard.push_back( m_PlayerCard[i][j] );
					}
				}

				if ( dqCPCard.size() != dqSPCard.size() )
				{
					m_JokerCard[0] = FALSE;
					m_JokerCard[1] = FALSE;
					ZeroMemory( m_MyCard, sizeof(BYTE) * MAX_CARD_VALUE * MAX_CARD_FACE );

					for ( int j = 0; j < dqSPCard.size(); j++ )
					{
						if ( dqSPCard[j] == CARD_JOKER1 )
						{
							m_JokerCard[0] = TRUE;
						}
						else if ( dqSPCard[j] == CARD_JOKER2 )
						{
							m_JokerCard[1] = TRUE;
						}
						else
						{
							int nShape = dqSPCard[j] / 13;
							int nNum   = dqSPCard[j] % 13;							
							m_MyCard[nNum][nShape] = 1;
						}
					}
					SortMyCard();
					bFailed = FALSE;
				}
				else
				{
					std::sort( dqCPCard.begin(), dqCPCard.end() );
					std::sort( dqSPCard.begin(), dqSPCard.end() );

					for ( int j = 0; j < dqCPCard.size(); j++ )
					{
						if ( dqCPCard[j] != dqSPCard[j] )
						{
							m_JokerCard[0] = FALSE;
							m_JokerCard[1] = FALSE;
							ZeroMemory( m_MyCard, sizeof(BYTE) * MAX_CARD_VALUE * MAX_CARD_FACE );

							for ( int j = 0; j < dqSPCard.size(); j++ )
							{
								if ( dqSPCard[j] == CARD_JOKER1 )
								{
									m_JokerCard[0] = TRUE;
								}
								else if ( dqSPCard[j] == CARD_JOKER2 )
								{
									m_JokerCard[1] = TRUE;
								}
								else
								{
									int nShape = dqSPCard[j] / 13;
									int nNum   = dqSPCard[j] % 13;
									
									m_MyCard[nNum][nShape] = 1;
								}
							}
							SortMyCard();
							bFailed = FALSE;
							break;
						}
					}
				}

				for ( int j = 0; j < MAX_CARDCOUNT ; j++ )
				{
					m_PlayerCard[i][j] = pPlayerCard[j];
				}
			}
			else
			{
				if ( dqSPCard.size() != g_PLAYER(i)->GetCardCount() )
				{
					g_PLAYER(i)->SetCardCount( dqSPCard.size() );
				}
			}
			break;
		}
	}

	// 바닥패 싱크 맞추기
	for ( int i = 0 ; i < MAX_CARD_FACE ; i++ )
	{
		for ( int j = 0 ; j < MAX_CARD_VALUE ; j++ )
		{
			m_CenterCard[i][j] = (int) arrStCards[i][j];
		}
	}
	for ( int i = 0 ; i < MAX_CARD_VALUE ; i++ )
	{
		for ( int j = 0 ; j < MAX_CARD_FACE ; j++ )
		{
			m_SameNumCard[i][j] = (int) arrToCards[i][j];
		}
	}

	if ( !bFailed )
	{
		CString str;
		str.Format("버그)ID:%s 문제:카드 불일치 에러\n",GM().GetMyInfo()->UI.ID);
		AddGameInfoViewEdit(str.GetString(), GetChatColor(ECC2_NOTIFY));

		//로그 남기기
		FILE *fp = fopen( "CardLog.txt", "r" );
		if ( fp != NULL )
		{
			fprintf( fp, "\n\n***서버 데이터와 불일치***\nFixCardData\n" );

			for ( int i = 0; i < MAX_PLAYER; i++ )
			{
				if ( strcmp( pID, g_PLAYER(i)->UI.ID ) == 0 && strcmp( pID, GM().GetMyInfo()->UI.ID ) == 0 )
				{
					fprintf( fp, "\n\tServer Card: " );
					for ( int j = 0; j < 8; j++ )
					{
						fprintf( fp, "%d ", pPlayerCard[j] );
					}
					fprintf( fp, "\n\tClinet Card: " );
					for ( int j = 0; j < 8; j++ )
					{
						fprintf( fp, "%d ", m_PlayerCard[i][j] );
					}
					break;
				}
			}

			fclose( fp );
		}
	}
}


void CPlayScr::FixRegist( int nPN )
{
	if ( nPN == g_PLAYER(0)->ServPNum )
	{
		SortMyCard();

		if ( CheckReg() )
		{
			SetRegCardButton();
		}
		else
		{
			SetDumpCardButton();
		}
	}
}

void CPlayScr::DrawGold() 
{
	if ( m_bDrawGold )
	{
		if ( m_nGoldStage0 == 2 || m_nGoldStage0 == 3 || m_nGoldStage0 == 5 )
		{
			m_pPage->DrawMultiScene( 0, 0, &g_sprGold, 1, timeGetTime() );
		}
		else if ( m_nGoldStage0 == 4 || m_nGoldStage0 == 6 )
		{
			m_pPage->DrawMultiScene( 0, 0, &g_sprGold, 3, timeGetTime() );
		}
	}
}

void CPlayScr::EnableGameStartBtn()
{
	if ( !m_bShowGameResult )
	{
		m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_GAMESTART, TRUE);
		
		((CMyGraphButton*)m_pGameButtonHA->GetGraphicBtn(CGameButtonHA::BTN_GAMESTART))->SetStartTime();

		m_cSunPickup.Reset(); //선고르기

		DisableGamePracticsBtn();
	}
}

void CPlayScr::DisableGameStartBtn()
{
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_GAMESTART, FALSE);
	((CMyGraphButton*)m_pGameButtonHA->GetGraphicBtn(CGameButtonHA::BTN_GAMESTART))->SetStartTime();
}

void CPlayScr::EnableGamePracticsBtn()
{
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_PRACTICS, TRUE);
	m_cSunPickup.Reset(); //선고르기
}

void CPlayScr::DisableGamePracticsBtn()
{
	m_pGameButtonHA->SetShowBtn(CGameButtonHA::BTN_PRACTICS, FALSE);
}

void CPlayScr::ReqGameStart()
{
	if ( m_bResult == FALSE )
	{
		DisableGameStartBtn();
		
		CCL_REQGAMESTART msg;
		msg.Set( g_RI.RoomNum, GM().GetMyInfo()->UI.ID );
		NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	}
}

void CPlayScr::ReqPracticsGameStart()
{
	DisableGamePracticsBtn();
	
	CCL_REQGAMEPRACTICS msg;
	msg.Set( g_RI.RoomNum, GM().GetMyInfo()->UI.ID );
	TRACE("\n\n R: %d, %d",g_RI.RoomNum,GM().GetMyInfo()->UI.ID);
	NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
}

void CPlayScr::DrawHelpMsg()
{
	if ( m_nHelpIndex >= 0 )
	{
		DWORD dwTime = abs( (int)(timeGetTime() - m_dwHelpTime) );

		if ( dwTime < g_sprSignBoard.GetMScenePlayTime( m_nHelpIndex ) )
		{	
			m_pPage->DrawMultiScene( 0, 0, &g_sprSignBoard, m_nHelpIndex, dwTime );
		}
		else
		{
			m_nHelpIndex = -1;
			m_dwHelpTime = 0;
		}
	}
}

void CPlayScr::Set7cardEffect()
{
	ANIEFFECT AniEffect;
	ZeroMemory( &AniEffect, sizeof( ANIEFFECT ) );

	AniEffect.nX = 505 - 106;
	AniEffect.nY = 350 - 93;
	AniEffect.dwStartTime = timeGetTime();
	AniEffect.nType = 777;
	m_dqAniEffect.push_back( AniEffect );
}

void CPlayScr::SetRuleWinEffect( int ruleKind, char *pCardData )
{
	ANIEFFECT AniEffect;
	ZeroMemory( &AniEffect, sizeof( ANIEFFECT ) );
	memset( m_CardData, -1, MAX_CARDCOUNT );
	CopyMemory( m_CardData, pCardData, MAX_CARDCOUNT );
	m_RuleKind = ruleKind;
	
	AniEffect.nX = 505 - 154;
	AniEffect.nY = 350 - 93;
	AniEffect.dwStartTime = timeGetTime();
	AniEffect.nType = 1584;
	m_dqAniEffect.push_back( AniEffect );
}

void CPlayScr::DrawAniEffect()
{
	DWORD dwTime = timeGetTime();

	std::deque< ANIEFFECT >::iterator Iter;

	ANIEFFECT WillBeDeleted[8];
	ZeroMemory( WillBeDeleted, sizeof( ANIEFFECT ) * 8 );
	int nCount = 0;

	Iter = m_dqAniEffect.begin();
	while ( Iter != m_dqAniEffect.end() )
	{
		ANIEFFECT* pAniEffect = &(*Iter);

		DWORD dwTime1 = abs ((int) (dwTime - pAniEffect->dwStartTime) );
		
		if ( pAniEffect->nType <= 5 )
		{
			if ( dwTime1 < g_sprEffect.GetMScenePlayTime( pAniEffect->nType ) )
			{
				if ( (*Iter).bSound == FALSE )
				{
					(*Iter).bSound = TRUE;
					AutoPlayMSceneSound( &g_sprEffect, pAniEffect->nType );
				}
				m_pPage->DrawMultiScene( pAniEffect->nX, pAniEffect->nY, &g_sprEffect, pAniEffect->nType, dwTime1 );
			}
			else
			{
				WillBeDeleted[nCount++] = (*Iter);
			}
		}
		else if ( pAniEffect->nType == 777 )
		{
			if ( dwTime1 < 1900 )
			{
 				if ( (*Iter).bSound == FALSE )
 				{
 					(*Iter).bSound = TRUE;
					NMBASE::SOUND::g_Sound.PlayWav( SND59 );
 				}
				int nX, nY;
				nX = pAniEffect->nX;
				nY = pAniEffect->nY;
				m_pPage->DrawScene( pAniEffect->nX, pAniEffect->nY, &g_sprCardSelect, 5, dwTime1 );
				m_pPage->PutSprAuto( nX + 21, nY + 49, &g_sprBigCard, 6 );
				m_pPage->PutSprAuto( nX + CARD_JOKER2, nY + 49, &g_sprBigCard, 19 );
				m_pPage->PutSprAuto( nX + 85, nY + 49, &g_sprBigCard, 32 );
				m_pPage->PutSprAuto( nX + 117, nY + 49, &g_sprBigCard, 45 );
			}
			else
			{
				WillBeDeleted[nCount++] = (*Iter);
			}
		}
		else if ( pAniEffect->nType == 1584 )
		{
			if ( dwTime1 < 1900 )
			{
				if ( (*Iter).bSound == FALSE )
				{
					(*Iter).bSound = TRUE;
					NMBASE::SOUND::g_Sound.PlayWav( SND59 );
				}
				int nX, nY;
				nX = pAniEffect->nX;
				nY = pAniEffect->nY;

				int sprIndex = (m_RuleKind == 4) ? 7 : 6;
				m_pPage->DrawScene( pAniEffect->nX, pAniEffect->nY, &g_sprCardSelect, sprIndex, dwTime1 );

				int count = 0;
				for ( int i = 0; i < 8; i++ )
				{
					if ( m_CardData[i] >= 0 )
					{
						m_pPage->PutSprAuto( nX + 21 + count*32, nY + 49, &g_sprBigCard, m_CardData[i] );
						count++;
					}
				}
			}
			else
			{
				WillBeDeleted[nCount++] = (*Iter);
			}
		}
		else
		{
			if ( dwTime1 < g_sprJackPotAni.GetMScenePlayTime( pAniEffect->nType - 10 ) )
			{
				if ( (*Iter).bSound == FALSE )
				{
					(*Iter).bSound = TRUE;
					AutoPlayMSceneSound( &g_sprJackPotAni, pAniEffect->nType - 10 );
				}

				m_pPage->DrawMultiScene( pAniEffect->nX, pAniEffect->nY, &g_sprJackPotAni, pAniEffect->nType - 10, dwTime1 );
			}
			else
			{
				WillBeDeleted[nCount++] = (*Iter);
			}
		}

		Iter++;
	}

	if ( m_dqAniEffectResult.size() )
	{
		Iter = m_dqAniEffectResult.begin();
		while ( Iter != m_dqAniEffectResult.end() )
		{
			ANIEFFECT* pAniEffect = &(*Iter);
			if ( pAniEffect == NULL )
				continue;

			DWORD dwTime1 = abs ( (int) (dwTime - pAniEffect->dwStartTime ));

			int nKind = 0;
			int nType = pAniEffect->nType - 0x1000;
			if ( nType & 0x100 )
			{
				if ( nType & 0x010 )
				{
					if ( nType & 0x001 )
					{
						nKind = 6;
					}
					else
					{
						nKind = 3;
					}
				}
				else if ( nType & 0x001 )
				{
					nKind = 5;
				}
				else
				{
					nKind = 0;
				}
			}
			else if ( nType & 0x010 )
			{
				if ( nType & 0x001 )
				{
					nKind = 4;
				}
				else
				{
					nKind = 1;
				}
			}
			else if ( nType & 0x001 )
			{
				nKind = 2;
			}
		
			if ( dwTime1 < g_sprEffectBak.GetMScenePlayTime( nKind ) )
			{
				if ( (*Iter).bSound == FALSE )
				{
					(*Iter).bSound = TRUE;
					AutoPlayMSceneSound( &g_sprEffectBak, nKind );
				}

				m_pPage->DrawMultiScene( pAniEffect->nX, pAniEffect->nY, &g_sprEffectBak, nKind, dwTime1 );
			}
			else
			{
				WillBeDeleted[nCount++] = (*Iter);
			}

			Iter++;
		}
	}

	for ( int i = 0; i < nCount; i++ )
	{
		if ( WillBeDeleted[i].nType < 0x1000 )
		{
			Iter = m_dqAniEffect.begin();
			while ( Iter != m_dqAniEffect.end() )
			{
				ANIEFFECT* pAniEffect = &(*Iter);

				if ( WillBeDeleted[i].nX == pAniEffect->nX && 
					WillBeDeleted[i].nY == pAniEffect->nY &&
					WillBeDeleted[i].dwStartTime == pAniEffect->dwStartTime )
				{
					m_dqAniEffect.erase( Iter );
					break;
				}

				Iter++;
			}
		}
		else
		{
			Iter = m_dqAniEffectResult.begin();
			while ( Iter != m_dqAniEffectResult.end() )
			{
				ANIEFFECT* pAniEffect = &(*Iter);

				if ( WillBeDeleted[i].nX == pAniEffect->nX && 
					WillBeDeleted[i].nY == pAniEffect->nY )
				{
					m_dqAniEffectResult.erase( Iter );
					break;
				}

				Iter++;
			}
		}
	}
}

void CPlayScr::DrawSelectCard()
{
	if ( m_bThankSelect && m_bSelectDraw )
	{
		int nXY[2][2] = { 337+16, 201+48, 337+172, 201+48 };

		if ( m_dqThankSelect.size() == 2 )
		{
			m_pPage->DrawScene( 337, 201, &g_sprCardSelect, 0, timeGetTime() );
			
			int nSize = m_dqThankSelect.size();
			int nGap[4] = { 32, 24, 16, 12 };
			for ( int i = 0; i < nSize; i++ )
			{
				int nSize1 = m_dqThankSelect[i].size();
				if ( nSize1 < 3 ) continue;
	
				int nAdd = nGap[nSize1-3];
				for ( int j = 0; j < nSize1; j++ )
				{
					m_pPage->PutSprAuto( nXY[i][0] + nAdd*j, nXY[i][1], &g_sprBigCard, m_dqThankSelect[i][j] );
				}

				if ( i == m_nSelectCurrent )
				{
					m_pPage->DrawScene( nXY[i][0] - 8 , nXY[i][1] - 8, &g_sprCardSelect, 4, timeGetTime() );
				}
				else
				{
					m_pPage->DrawScene( nXY[i][0] - 8 , nXY[i][1] - 8, &g_sprCardSelect, 3, timeGetTime() );
				}
			}
		}
	}
	else if ( m_bRegSelect && m_bSelectDraw )
	{
		//낱장조합 묶음조합 내에서 선택 가능한 상황에서 그려짐
		if ( m_cCardInfo.m_dqSelect.size() >= 2 )
		{
			int nXY[2][2] = { 337+16, 201+48, 337+172, 201+48 };
			m_pPage->DrawScene( 337, 201, &g_sprCardSelect, 0, timeGetTime() );
			
			int nSize = m_cCardInfo.m_dqSelect.size();
			int nGap[4] = { 32, 24, 16, 12 };
			for ( int i = 0; i < nSize; i++ )
			{
				int nSize1 = m_cCardInfo.m_dqSelect[i].size();
				if ( nSize1 < 3 ) continue;
				int nAdd = nGap[nSize1-3];
				for ( int j = 0; j < nSize1; j++ )
				{
					m_pPage->PutSprAuto( nXY[i][0] + nAdd*j, nXY[i][1], &g_sprBigCard, m_cCardInfo.m_dqSelect[i][j] );
				}

				if ( i == m_nSelectCurrent )
				{
					m_pPage->DrawScene( nXY[i][0] - 8 , nXY[i][1] - 8, &g_sprCardSelect, 4, timeGetTime() );
				}
				else
				{
					m_pPage->DrawScene( nXY[i][0] - 8 , nXY[i][1] - 8, &g_sprCardSelect, 3, timeGetTime() );
				}
			}
		}
	}
}

void CPlayScr::DrawHoola()
{
	if ( (m_bHoolaWin && !m_bJackpot)==FALSE )
		return;

	DWORD dwTime = abs( (int)(timeGetTime() - m_dwHoolaTime ));

	if ( dwTime < g_sprHoola.GetMScenePlayTime( 0 ) && dwTime < 5000 )
	{
		if ( m_bHoolaSound == FALSE )
		{
			m_bHoolaSound = TRUE;
			AutoPlayMSceneSound( &g_sprHoola, 0 );
		}
		m_pPage->DrawMultiScene( 0, 0, &g_sprHoola, 0, dwTime );
	}
	else
	{
		m_bHoolaWin = FALSE;
		m_bHoolaSound = FALSE;

		if ( !m_dqAniEffectResult.empty()) 
		{
			std::deque< ANIEFFECT >::iterator Iter;
			Iter = m_dqAniEffectResult.begin();

			while ( Iter != m_dqAniEffectResult.end() )
			{
				(*Iter).dwStartTime = timeGetTime();

				Iter++;
			}
		}
	}
}

void CPlayScr::SetSelectRegCard(int nIndex)
{
	m_bClickRegCard = TRUE;
	if ( nIndex < m_cCardInfo.m_dqSelect.size() )
	{
		m_dqSelectedCard = m_cCardInfo.m_dqSelect[nIndex];
	}

	if ( m_dqSelectedCard.empty() ) 
		return;

	BYTEDQ dqTemp = m_dqSelectedCard;
	if ( CheckReg( dqTemp ) )
	{
		SetRegCardButton();
	}
	else
	{
		SetDumpCardButton();
	}
	ResetSelect();
}

void CPlayScr::SetJackpotData( INT64 i64Money )
{
	if( i64Money <= 0 )
		return;

	if ( !m_bJackpotData )
	{
		int nComma = 0;
		int nTime = 0;
		for ( int i = 0; i < 20; i++ )
		{
			if ( i == 3 || i == 7 || i == 11 || i == 15 || i == 19 )
			{
				nComma++;
			}

			m_JackpotMoneyData[i].nX = 635 - ((i-nComma)*18 + nComma*9);
			m_JackpotMoneyData[i].nY = 391;
			m_JackpotMoneyData[i].dwStartTime = 3800 + (i-nComma)*50;
		}
		for ( int i = 0; i < 20; i++ )
		{
			if ( i == 3 || i == 7 || i == 11 || i == 15 )
			{
				m_JackpotMoneyData[i].dwStartTime = m_JackpotMoneyData[i+1].dwStartTime;
			}
		}
		m_bJackpotData = TRUE;
	}

	m_dqJackpotMoney.clear();

	int nNum;
	INT64 TempMoney = i64Money;

	int nComma = 0;
	while ( 1 )
	{
		if ( nComma == 3 || nComma == 7 || nComma == 11 || nComma == 15 || nComma == 17 )
		{
			m_dqJackpotMoney.push_back( 11 );
		}
		else
		{
			nNum = TempMoney % 10;
			m_dqJackpotMoney.push_back( nNum );

			TempMoney = TempMoney / 10;
			if ( TempMoney == 0 )
				break;
		}
		nComma++;
	}
}

// 받은 유저의 아이디로 턴 카운트를 세팅한다
void CPlayScr::SetTurnID( char *pID )
{
	for ( int i = 0; i < MAX_PLAYER; i++ )
	{
		if ( strcmp( g_PLAYER(i)->UI.ID, pID ) == 0 )
		{
			g_pGameView->m_cPlayerDrawManager.SetTurn(i);
			m_nTurnCnt = g_PLAYER(i)->ServPNum;
			break;
		}
	}
}

BOOL CPlayScr::IsLowCardByDumpCard( char cDumpCard )
{
	int nCardSum = 0;
	int nHighCard = 0;
	int joker = 0;

	for( int i = 0; i < 8; i ++ )
	{
		if ( m_PlayerCard[0][i] >= 0 && m_PlayerCard[0][i] < CARD_JOKER1 ) 
		{
			int nCard = (m_PlayerCard[0][i]%13) + 1;
			if ( nCard == 7 )
			{
				nCard = 14;
				nHighCard = 14;
			}

			if ( nCard > nHighCard )
				nHighCard = nCard;

			nCardSum += nCard;
		}
		else if ( m_PlayerCard[0][i] >= CARD_JOKER1 )
		{
			joker++;
			nHighCard = 14;

			nCardSum += 14;
		}
	}

	int nDumpCard = 14;
	if ( cDumpCard >= 0 && cDumpCard < CARD_JOKER1 )
	{
		nDumpCard = cDumpCard%13 + 1;
		if ( nDumpCard == 7 )
			nDumpCard = 14;
	}
	
	int nCheckNum = (nCardSum - nHighCard) + nDumpCard;
	if ( nCheckNum <= 15 )
	{
		return TRUE;
	}

	return FALSE;
}

BOOL CPlayScr::IsHighCardByDumpCard( char cDumpCard )
{
	int nCardSum = 0;
	int nLowCard = 14;
	int joker = 0;

	for( int i = 0 ; i < MAX_CARDCOUNT ; i++ )
	{
		if ( m_PlayerCard[0][i] >= 0 && m_PlayerCard[0][i] < CARD_JOKER1 ) 
		{
			int nCard = (m_PlayerCard[0][i]%13) + 1;
			if ( nCard == 7 )
			{
				nCard = 14;
			}

			if ( nCard < nLowCard )
				nLowCard = nCard;

			nCardSum += nCard;
		}
		else if ( m_PlayerCard[0][i] >= CARD_JOKER1 )
		{
			joker++;
			nCardSum += 14;
		}
	}

	int nDumpCard = 14;
	if ( cDumpCard >= 0 && cDumpCard < CARD_JOKER1 )
	{
		nDumpCard = cDumpCard%13 + 1;
		if ( nDumpCard == 7 )
			nDumpCard = 14;
	}
// 	else
// 	{
// 		//조커 카드가 들어올 경우 땡큐가 불가능하다.
// 		return FALSE;
// 	}

	int nCheckNum = (nCardSum - nLowCard) + nDumpCard;
	if ( nCheckNum >= 83 )
	{
		return TRUE;
	}
	return FALSE;
}
BOOL CPlayScr::IsSevenFourCardByDumpCard( char cDumpCard )
{
	int nDumpCard = ( cDumpCard % 13 ) + 1;
	
	if ( nDumpCard != 7)
	{
		return FALSE;
	}

	int nSevenCardCnt = 0;
	for( int i = 0; i < 8; i ++ )
	{
		if ( m_PlayerCard[0][i] >= 0 && m_PlayerCard[0][i] < CARD_JOKER1 ) 
		{
			int nCard = (m_PlayerCard[0][i]%13) + 1;
			if ( nCard == 7 )
			{
				nSevenCardCnt++;
			}
		}
	}
	if ( nSevenCardCnt == 3 )
	{
		return TRUE;
	}
	return FALSE;
}

// 등록가능 유무 판별
BOOL CPlayScr::CheckAvailableReg()
{
	if ( m_cCardInfo.m_dqUpCard.size() > 0  )
	{
		return TRUE;
	}

	if ( m_JokerCard[0] > 0 || m_JokerCard[1] > 0 )
	{
		return TRUE;
	}

	return FALSE;
}

void CPlayScr::SetJackpotState( char* pID, int nStep )
{
	if ( nStep == 0 )
	{
		m_bJackpotConditionSucceed = FALSE;
		m_nJackpotConditionStep = 0;
		m_dwJackpotConditionTime = 0;
	}
	else
	{
		if ( strcmp( pID, GM().GetMyInfo()->UI.ID ) == 0 )
		{
			m_bJackpotConditionSucceed = TRUE;
			m_nJackpotConditionStep = nStep;
			m_dwJackpotConditionTime = timeGetTime();
		}
		else
		{
			m_bJackpotConditionSucceed = FALSE;
			m_nJackpotConditionStep = 0;
			m_dwJackpotConditionTime = 0;
		}
	}
}

BOOL CPlayScr::IsWinCaseHoola(int nWincase)
{
	return ( nWincase == 1 ) || ( nWincase == 2 );
}

void CPlayScr::PlayHoolaAnimation()
{
	m_bHoolaWin = TRUE;
	m_dwHoolaTime = timeGetTime();
}

void CPlayScr::SetResultDelay( BOOL bResultDelay )
{
	if (TRUE == bResultDelay)
	{
		m_bResultDelay = TRUE;
		m_dwResultDelay = timeGetTime();
	}
	else
	{
		//FALSE로 날라올 경우 코드 적용
	}
}

void CPlayScr::SetResultCard()
{
	for( int i = 0; i < MAX_PLAYER; i ++ )
	{
		int pnum = g_PLAYER(i)->ServPNum - g_PLAYER(0)->ServPNum;
		if( pnum < 0 ) pnum += MAX_PLAYER;

		for ( int j = 0; j < MAX_PLAYER; j++ )
		{
			if ( strlen(  g_PLAYER(pnum)->UI.ID ) > 0 && strlen( m_GameResult.Ui[j].ID ) > 0 ) 
			{
				if ( strcmp(  g_PLAYER(pnum)->UI.ID, m_GameResult.Ui[j].ID ) == 0 )
				{
					if ( (pnum == 0) && !g_ObserverMan.IsMyObserver() ) continue;
					CopyMemory( m_PlayerCard[pnum], m_GameResult.Hg.arrPlayerCard[j], sizeof( char ) * MAX_CARDCOUNT );
					break;
				}
			}
		}
	}

	for( int i = 0; i < MAX_PLAYER; i++ )
	{
		char card[MAX_CARD_VALUE][MAX_CARD_FACE];
		ZeroMemory( card, sizeof(char) * MAX_CARD_VALUE * MAX_CARD_FACE );

		int cnt = 0;
		int nJoker1, nJoker2;
		nJoker1 = nJoker2 = 0;

		for ( int j = 0; j < 8; j++ )
		{
			if( m_PlayerCard[i][j] >= 0 )
			{
				if ( m_PlayerCard[i][j] == CARD_JOKER1 )
				{
					nJoker1 = 1;
				}
				else if ( m_PlayerCard[i][j] == CARD_JOKER2 )
				{
					nJoker2 = 1;
				}
				else
				{
					char cCI = m_PlayerCard[i][j];
					int nIndex = cCI % 13;
					int nFace  = cCI / 13;
					card[nIndex][nFace] = 1;
					cnt++;
				}
			}
		}

		memset( m_PlayerCard[i], -1, sizeof(char) * 8 );
		int cnt1 = 0;
		for ( int k = 0; k < 13; k++ )
		{
			for ( int l = 0; l < 4; l++ )
			{
				if ( card[k][l] )
				{
					m_PlayerCard[i][cnt1++] = l*13 + k;
				}
			}
		}

		if ( nJoker1 )
		{
			m_PlayerCard[i][cnt1++] = CARD_JOKER1;
		}
		if ( nJoker2 )
		{
			m_PlayerCard[i][cnt1++] = CARD_JOKER2;
		}

		g_PLAYER(i)->SetCardCount( cnt1 );
	}

	int pnum = g_PLAYER(0)->ServPNum;
	char card;

	memset( m_MyCard, false, sizeof( char ) * 13 * 4 );
	m_JokerCard[0] = 0;
	m_JokerCard[1] = 0;

	for ( int i = 0; i < 8; i++ )
	{
		if ( (card = m_PlayerCard[0][i]) >= 0 )
		{
			if ( card == CARD_JOKER1 )
			{
				m_JokerCard[0] = 1;
			}
			else if ( card == CARD_JOKER2 )
			{
				m_JokerCard[1] = 1;
			}
			else
			{
				m_MyCard[card%13][card/13] = true;
			}
		}
	}

	GameResultFlag = true;
	m_bShowGameResult = TRUE;
}

void CPlayScr::SetCardData( char arrCenterCard[4][13], char arrSameCard[13][4] )
{
	ZeroMemory( &m_SameNumCard, sizeof(int) * MAX_CARD_FACE * MAX_CARD_VALUE );
	ZeroMemory( &m_CenterCard, sizeof(int) * MAX_CARD_FACE * MAX_CARD_VALUE );

	// Set Straight Card
	for ( int i = 0 ; i < MAX_CARD_FACE ; i++ )
	{
		for ( int j = 0 ; j < MAX_CARD_VALUE ; j++ )
		{
			m_CenterCard[i][j] = (int)arrCenterCard[i][j];			
		}
	}
	// Set Together Card
	for ( int i = 0 ; i < MAX_CARD_VALUE ; i++ )
	{
		for ( int j = 0 ; j < MAX_CARD_FACE ; j++ )
		{
			m_SameNumCard[i][j] = (int) arrSameCard[i][j];			
		}
	}
}
// BOOL CPlayScr::IsSevenCard(int nCardNum)
// {
// 	// 세븐카드에 해당하는 인덱스번호인지 확인하고 결과를 반환한다.
// 	switch ( nCardNum )
// 	{
// 	case 6 :
// 	case 19:
// 	case 32:
// 	case 45:
// 		return TRUE;
// 	default:
// 		break;
// 	}	
// 	return FALSE;
// }
// BOOL CPlayScr::IsJokerCard(int nCardNum)
// {
// 	// 세븐카드에 해당하는 인덱스번호인지 확인하고 결과를 반환한다.
// 	switch ( nCardNum )
// 	{
// 	case CARD_JOKER1:
// 	case CARD_JOKER2:
// 		return TRUE;
// 	default:
// 		break;
// 	}
// 	return FALSE;
// }