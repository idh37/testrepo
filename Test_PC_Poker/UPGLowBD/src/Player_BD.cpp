// Player_Base.cpp: implementation of the CPlayer_BD class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Player_BD.h"

#include "./Effect/MadeEffect.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CPlayer_BD::CPlayer_BD()
:m_pMyLowBDRule( NULL ),	   // 바둑이 룰 구하기	
m_pEffectObjectList( NULL ),
m_nTurnLimitCnt( 0 )
{	
	SAFE_NEW( m_pMyLowBDRule, CBadugiRule );
	SAFE_NEW( m_pEffectObjectList, CObjectList );
	
	ServPNum = 0;
	PNum = -1;
	m_bFirst = true;
	
	ZeroMemory(&m_sUIPos ,sizeof(m_sUIPos));

	Clear();
}

CPlayer_BD::~CPlayer_BD()
{
	Clear();
	Destroy_Text();

	SAFE_DELETE( m_pMyLowBDRule );
	SAFE_DELETE( m_pEffectObjectList );
}

bool CPlayer_BD::IsValid()
{
	return true;
}

void CPlayer_BD::Destroy_Text()
{
}

void CPlayer_BD::Clear()
{
	ZeroMemory( &UI, sizeof( UI ) );
	ZeroMemory(&m_cRoomChangeInfo, sizeof(CHANGE_USERINFO_ROOM));
	Reset();
	bFold = FALSE;
	MyIpCheck = -1;
	nSndFxKind = 0;
	m_nTurnLimitCnt = 0;
}

void CPlayer_BD::SetPlayerNum(int pnum)
{
	PNum = pnum;
}

void CPlayer_BD::SetNewPlayer(USERINFO *pUI)
{	
	Clear();
	
	if ( m_bFirst )
	{
		//InitRoomData();
		m_bFirst = false;
	}

	memcpy(&UI, pUI, sizeof(USERINFO));
	
	Avatar.SetAvatar(UI.AvatarInfo);
	// 캐릭터 그리기 아바타		
	if (g_Config.bAvataAni)
	{
		Avatar.ResetAnimation();
		Avatar.SetAnimationTime(0);
	}	
}

void CPlayer_BD::Reset()
{
	JoinState = 0;
	PlayState = 0;
	
	m_nCardTotal = 0;
	m_nOpenTotal = 0;
	bFold = FALSE;
	bFoldOpen = FALSE;

	m_llResultMoney = 0;
	//bExit = FALSE;
	// 카드 관련
	for ( int i = 0; i < NCARD_TOTAL; i++ ) 
	{
		m_cMyCard[ i ].Clear();
	}

	m_bCardOPenMove = false;

	m_nSDelay = -1;	
	m_nTotalCardNum = 0;

	//이미지 드로우 관련 리스트 
	m_pEffectObjectList->Destroy();	
	
	m_bShowValue = false;	

	//컷팅 카운트 , 패스 저장
	m_stCutInfo.Clear();	
	m_pMyLowBDRule->Clear();
	
	m_bDontSelectCard = false;
	
	m_strValueName.clear();
}


void CPlayer_BD::SetNewCard( int cardNum )
{
	if ( !IsValid() ) return;

	if ( m_nTotalCardNum < 0 || m_nTotalCardNum > TOTAL_CARD_BD ) return;// ###[8포커]###

	m_cMyCard[ m_nTotalCardNum ].Init( cardNum, PNum , m_nTotalCardNum ); // PNum 소유자
	m_cMyCard[ m_nTotalCardNum ].m_bShow = false;  // 임시 ( FALSE -> 카드 날리기 받았을때 TRUE)	

	int nNewYp = m_sUIPos.PT_Card.y;		

	// 목적지 셋팅 
	int nGabX = 0;

	if ( PNum == 0 )
	{
		nGabX = g_cUIData.GetGabBD( ui::EGAB_CARD_POS_X );
	}
	else
	{
		nGabX = g_cUIData.GetGabBD( ui::EGAB_CARD_POS_OTHER_X );
	}

	m_cMyCard[ m_nTotalCardNum ].SetPos( m_sUIPos.PT_Card.x + ( m_nTotalCardNum * ( nGabX ) ), nNewYp );

	m_nTotalCardNum ++;
}

//방에 입장할때 한번 초기화 하는것들 
void CPlayer_BD::InitRoomData()
{
	Reset();
	Destroy_Text();

	ENUM_POSTYPE nPosType = EPT_UNKNOWN;

	//아이디,아바타,레벨 좌표 설정
	switch(PNum)
	{
	case 0:
		{
			nPosType = EPT_ME;
		}break;
	case 1:
		{
			nPosType = EPT_LEFT;
		}break;
	case 2:
		{
			nPosType = EPT_LEFT;
		}break;
	case 3:
		{
			nPosType = EPT_RIGHT;
		}break;
	case 4:
		{
			nPosType = EPT_RIGHT;
		}break;
	}

	// 관전제외
	if ( PNum >= 0  && PNum < 5 )
	{
		//카드 시작 위치
		m_sUIPos.PT_Card = g_cUIData.GetPosBD( ( ui::E_POSITION_BD )( ui::EPOS_CARD_POS_0 + PNum ) );	
		
		m_sUIPos.PT_Avatar = g_cUIData.m_listAvatarPos[ PNum ];

		// 나의 칩 던지는 모듈 초기화
		m_cMyChip.Init(&g_pGameView->Page, &GLOBAL_SPR[ spr::GSPR_GAME_CHIP ], PNum, &g_cUIData);
	}
}

void CPlayer_BD::OnTimer()
{	
	m_cMyChip.OnMyChipTimer(timeGetTime());

	int i = 0 ;	

	for (i = 0; i < m_nTotalCardNum; i++ )
	{
		m_cMyCard[ i ].OnTimer();
	}

	// 결과때 카드 이동 처리 부분 
	if ( m_bCardOPenMove )
	{

		if(m_nSDelay > 0)
		{
			m_nSDelay--;
			if (m_nSDelay <= 0 )
			{
				m_nSDelay = 0;		
				g_pGameView->m_cPlayerDrawManager.SetPlayerGameOver(0, true);
			}
		}

		int cnt =m_nTotalCardNum;
		
		for(int i = 0; i < m_nTotalCardNum; i++){
			if(m_cMyCard[ i].bMoveStart == FALSE && m_cMyCard[ i].bOPen == TRUE && m_cMyCard[ i].nDelay == 0){
				cnt--;
			}
		}

		if ( cnt <= 0 )
		{			
			for(int i = 0 ; i < m_nTotalCardNum ; i ++)
			{
				m_cMyCard[ i].SetCardFace(FALSE,20,FALSE);				
			}
			m_bCardOPenMove = false;
		}		
	}
	else if ( !m_bCardOPenMove && m_nSDelay == 0)
	{
		// 결과시 족보를 순서대로 보여준다
		int tcnt = 0 ;
		for(i = 0 ; i < m_nTotalCardNum ; i++)
		{
			if (m_cMyCard[ i].bMoveStart == FALSE && m_cMyCard[ i].bDirect == FALSE && m_cMyCard[ i].nDelay == 0 && m_cMyCard[ i].m_bFront == TRUE)
			{
				tcnt ++;
			}
		}

		if(tcnt == m_nTotalCardNum -1)
		{						
			m_bShowValue = true;
			g_pGameView->m_cPlayerDrawManager.SetPlayerGameOver(0, true);
		}
	}
	
	m_pEffectObjectList->ProcessAll();
}

void CPlayer_BD::OnSetChip(INT64 money)
{
	if ( PlayState != 1 && JoinState != 1 && bFold )
	{
		return;		
	}

	m_cMyChip.SetMoneyChip(money);
	m_cMyChip.SetMyMoveChip();	
}

void CPlayer_BD::OnSetBetDrawMoney(INT64 money, BOOL bPlus,int betflg)
{
	
}


bool CPlayer_BD::GetFace( int index )
{
	if ( !IsVaild_CardIndex( index ) )
	{
		return false;
	}

	return m_cMyCard[ index ].m_bFront;
}

// 카드의 앞면
void CPlayer_BD::SetFaceUp(int index)
{
	if ( !IsVaild_CardIndex( index ) )
	{
		return;
	}

	m_cMyCard[ index ].m_bFront = TRUE;		
}

// 카드의 뒷면 
void CPlayer_BD::SetFaceDown(int index)
{
	if ( !IsVaild_CardIndex( index ) )
	{
		return;
	}

	m_cMyCard[ index].m_bFront = FALSE;
}

void CPlayer_BD::ShowCard(int index , bool bshow)
{
	if ( !IsVaild_CardIndex( index ) )
	{
		return;
	}

	m_cMyCard[ index ].m_bShow = bshow;
}

void CPlayer_BD::SetResultFace(int index , bool bresult)
{
	if ( !IsVaild_CardIndex( index ) )
	{
		return;
	}

	for ( int i = 0 ; i < m_nTotalCardNum; i++ )
	{
		m_cMyCard[ i ].SetDarkCard( bresult );
	}
}

// 0 ~ 3 총 4장
int CPlayer_BD::GetCardNo(int index)
{
	if ( !IsVaild_CardIndex( index ) )
	{
		return 0;
	}

	return m_cMyCard[ index].GetCardNo();
}

// 0 ~ 3 총 4장
void CPlayer_BD::SetCardNo(int index, int nCardNo )
{
	if ( !IsVaild_CardIndex( index ) )
	{
		return;
	}

	m_cMyCard[ index ].SetCardNo( nCardNo );
}


// 카드를 다 덮은 상태로 모은다.
void CPlayer_BD::SetGameResultCardOpen(int delay)	// 카드 오픈 (애니 처리 설정)
{
	m_bCardOPenMove = true;
	int ndelay =delay;	
	m_nSDelay = ndelay;

	for(int i = m_nTotalCardNum -1 ; i >=0  ; i --)
	{		
		m_cMyCard[ i].SetCardFace(TRUE,ndelay,TRUE);
		ndelay += 1;		
	}	
}

void CPlayer_BD::SetChioceCardMove(BOOL bdirect,int face)
{
	for(int i = m_nTotalCardNum -1 ; i >=0  ; i --){			
		m_cMyCard[ i].SetCardFace(TRUE,1,bdirect);
		if(face != 0){
			if(face == 1){
				m_cMyCard[ i].m_bFront = FALSE;
			}
			else{
				m_cMyCard[ i].m_bFront = TRUE;
			}			
		}
	}	
}

bool CPlayer_BD::MouseOver_PtInCardRect(int xp,int yp)// 카드 영역을 클릭 했는지 
{
	// 다이 일때 내카드 열기
	if (!g_ObserverMan.IsMyObserver() && JoinState != 0 && PlayState == 0 && bFold == TRUE && PNum == 0 )
	{	
		for ( int i = m_nTotalCardNum - 1 ; i >= 0; i-- )
		{
			if ( m_cMyCard[ i ].PtInCard( xp, yp ) )
			{
				for (int x = 0 ; x < m_nTotalCardNum ; x ++)
				{
					if ( !GetFace( x ) )
					{
						SetFaceUp( x );
					}
				}
				return true;
			}
		}
		
		// 본인카드오픈 상태에서 카드를 뒤집어주는 연출을 하지 않도록 막는다.
		if( bFoldOpen == FALSE )
		{
			for( int  i = 0 ; i < m_nTotalCardNum ; i ++)
			{
				if ( GetFace( i ) )
				{
					SetFaceDown(i);
				}
			}
		}
	}
		
	return false;
}

int CPlayer_BD::Select_PtInCardRect(CPoint pt)// 카드 영역에 있는지
{	
	for ( int i = m_nTotalCardNum - 1; i >= 0 ; i-- )
	{
		if ( m_cMyCard[ i ].PtInCard( pt.x, pt.y ) )
		{
			return i;
		}
	}
	return -1;
}

void CPlayer_BD::OnLButtonDown(int xp, int yp)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) return;
	if( PNum != 0 || g_ObserverMan.IsMyObserver() )return;
	
	POINT pt;
	pt.x = xp;
	pt.y = yp;

	if ( !m_bDontSelectCard )
	{
		OnChangeSelect( pt );
	}

	//히든 카드를 가지고 있는지 검사
	if ( !g_GAME_BD()->IsHiddenCardFlag() )
	{
		return;
	}

	if (JoinState != 0 && PlayState != 0 && m_nCardTotal == TOTAL_CARD_BD )
	{		
		if ( g_pGameView->m_cPlayerDrawManager.IsHiddenCardShow() )
		{
			return;
		}

		bool bHiddenClick = false;
		for ( int i = 0; i < TOTAL_CARD_BD; i++ )
		{
			if ( m_cMyCard[ i ].IsHiddenCard() )
			{
				POINT ptCur;
				ptCur.x = m_cMyCard[ i ].m_Xp;
				ptCur.y = m_cMyCard[ i ].m_Yp;
				
				CRect rtRect;
				rtRect.SetRect( ptCur.x, ptCur.y, ptCur.x + m_cMyCard[ i ].m_size.x, ptCur.y + m_cMyCard[ i ].m_size.y );

				POINT ptPos;
				ptPos.x = xp;
				ptPos.y = yp;

				if ( rtRect.PtInRect( ptPos ) )
				{
					g_GAME_BD()->m_bHiddenClick = true;
					g_GAME_BD()->m_ptHiddenClickPos.x = xp;
					g_GAME_BD()->m_ptHiddenClickPos.y = yp;
					PBPlayEffectSound(SND_HIDDEN_OPEN);
					
					bHiddenClick = true;
					break;
				}
			}
		}

		if ( bHiddenClick )
		{
			for ( int i = 0; i < TOTAL_CARD_BD; i++ )
			{
				SetFaceUp( i ); // 보이기
			}
		}
		else
		{
			g_GAME_BD()->m_bHiddenClick = false;
		}
	}
}

void CPlayer_BD::OnLButtonUp(int xp, int yp)
{
	if ( GetCurrentWhere() != IDX_GAMEWHERE_GAME) return;
	
	if( PNum != 0 || g_ObserverMan.IsMyObserver() )return;	

	//히든 카드를 가지고 있는지 검사
	if ( !g_GAME_BD()->IsHiddenCardFlag() )
	{
		return;
	}

	if ( JoinState != 0 && PlayState != 0 && m_nCardTotal == TOTAL_CARD_BD )
	{		
		if ( g_pGameView->m_cPlayerDrawManager.IsHiddenCardShow() )
		{
			return;
		}

		if ( g_GAME_BD()->m_bHiddenClick )
		{			 	
			g_pGameView->m_cPlayerDrawManager.SetFlyWindCard( false );
			
			for ( int i = 0; i < TOTAL_CARD_BD; i++ )
			{
				SetFaceUp( i );
				m_cMyCard[ i ].SetHiddenCard( false );
			}
			
			g_pGameView->m_cPlayerDrawManager.SetHiddenCardShow( true );

			g_GAME_BD()->AutoSort_Recommand(); //< CYS 101201 >

			SetCardResult();
			m_bShowValue = true;
			DrawEft_CompleteMadeValue( 0, true );
		}		
	}
}

void CPlayer_BD::DrawEft_CompleteMadeValue(int oldpoint, bool bForce )
{
	if ( g_ObserverMan.IsMyObserver() || PNum != 0  || bFold == TRUE || PlayState != 1 )
	{
		return;
	}

	//보다 좋은 족보(메이드) 완성시 효과 
	if ( m_pMyLowBDRule->m_MyValue > 0 && m_pMyLowBDRule->m_MyValue <= 4 )
	{
		int nCurPoint = m_pMyLowBDRule->m_RuleCompleteCard.numberpoint;

		if ( nCurPoint > oldpoint || bForce ) // 처음 4장, 히든오픈시 메이드인 경우 이펙트 보여준다.
		{
			effect::CMadeEffect *pObj = NULL;
			SAFE_NEW( pObj, effect::CMadeEffect );
			pObj->Init();
			g_pGameView->m_cAniSprListManager.ADDObject(pObj);
		}
	}
}


bool CPlayer_BD::OnMouseMove(int xp, int yp)
{
	if ( GetCurrentWhere() != IDX_GAMEWHERE_GAME ) return false;

	if ( PNum == 0 && !g_ObserverMan.IsMyObserver() && bFold == TRUE )
	{
		if ( MouseOver_PtInCardRect(xp,yp) )
		{
			return true;
		}
	}

	if ( strlen( UI.ID ) > 0 )
	{
		CRect rt;
		rt.SetRect(m_sUIPos.PT_Avatar.x, m_sUIPos.PT_Avatar.y ,m_sUIPos.PT_Avatar.x+75,m_sUIPos.PT_Avatar.y+125 );
		POINT pt;
		pt.x = xp ; 
		pt.y = yp ; 
		if ( rt.PtInRect( pt ) )
		{
			SetCursor( GM().GetCurHandCursor() );
			return true;
		}
		else
		{
			SetCursor( GM().GetCurSelCursor() );
		}
	}

	return false;
}

void CPlayer_BD::OnChangeSelect( CPoint pt , int index )		//바꿀때 마우스 이벤트
{	
	if ( !IsValid() ) 
	{
		return;
	}

	if ( g_ObserverMan.IsMyObserver() )
	{
		return;
	}

	if ( PNum != 0 || g_RI.State == 0 || JoinState== 0 || PlayState == 0 || bFold ) 
	{
		return;
	}
		
	if ( g_GAME_BD()->m_bWindCardEnd ) return;
	if ( g_GAME_BD()->GetMovingCardMgr()->IsCardMoving( 0 ) ) return;
	if ( g_GAME_BD()->m_nGameState == RSTATE_RESULT ) return;
	
	if ( m_bDontSelectCard )
	{
		if ( IsCutCardSelectedNum() > 0 && g_GAME_BD()->GetCuttingStageInfo()->bChangeCardFlyEnd == false )
		{
			ReSetCardPostion();
		}
		return;
	} 

	int nindex = -1;
	if ( index == -1 ) // 마우스 선택
	{
		nindex = Select_PtInCardRect(pt);
	}
	else  // Key 선택
	{
		nindex = index;
	}

	if( nindex > -1 )
	{
		if( m_cMyCard[ nindex].IsCardMoving() )return;
		POINT ptStart, ptEnd;

		bool bSelect = m_cMyCard[ nindex ].IsSelected();

		m_cMyCard[ nindex ].SetSelected( !bSelect );

		int gab = CARD_GAB_Y_SELECT;
		//선택 한걸 올림
		if ( m_cMyCard[ nindex].IsSelected() )
		{
			ptStart = CPoint(m_cMyCard[ nindex].m_Xp,m_cMyCard[ nindex ].m_Yp );
			ptEnd.x = m_cMyCard[ nindex ].m_Xp;
			ptEnd.y = m_cMyCard[ nindex ].m_Yp - gab;
		}
		else
		{
			//선택한걸 내림
			ptStart = CPoint(m_cMyCard[ nindex ].m_Xp, m_cMyCard[ nindex ].m_Yp - gab);
			ptEnd.x = m_cMyCard[ nindex ].m_Xp;
			ptEnd.y = m_cMyCard[ nindex ].m_Yp;
		}

		if ( g_Poker.nState >= RSTATE_GAME_ROUND_CHANGECARD_M && g_Poker.nState <= RSTATE_GAME_ROUND_CHANGECARD_N )
		{
			// 이미 진행했다.
			if ( g_GAME_BD()->GetCuttingStageInfo()->bChangeStage  )
			{
				g_GAME_BD()->SetEnableCuttingBtn( true );
			}
		}

		m_cMyCard[ nindex].StraightMoveTo( ptStart, ptEnd, 2 );
	}
}

void CPlayer_BD::ReSetCardPostionOnly()
{
	if ( !IsValid() ) return;
	if ( PNum != 0 ) return;

	for ( int i = 0 ; i < TOTAL_CARD_BD ; i++ )
	{
		if ( m_cMyCard[ i ].GetCardIndex() > -1 )
		{
			POINT pt = m_cMyCard[ i].GetMovingCurPos();
			if ( pt.y != m_sUIPos.PT_Card.y )
			{
				POINT pt1;				
				pt1.x = m_cMyCard[ i ].m_Xp;
				pt1.y = m_cMyCard[ i ].m_Yp;
				m_cMyCard[ i ].StraightMoveTo( pt, pt1, 2 );		
			}
		}		
	}
}


void CPlayer_BD::ReSetCardPostion()
{
	if ( !IsValid() ) return;
	if ( PNum != 0 ) return;
	
	for ( int i = 0 ; i < TOTAL_CARD_BD ; i++ )
	{
		if ( m_cMyCard[ i ].GetCardIndex() > -1 )
		{
			POINT pt = m_cMyCard[ i].GetMovingCurPos();
			if ( pt.y != m_sUIPos.PT_Card.y )
			{
				POINT pt1;				
				pt1.x = m_cMyCard[ i ].m_Xp;
				pt1.y = m_cMyCard[ i ].m_Yp;
				m_cMyCard[ i ].StraightMoveTo( pt, pt1, 2 );		
				m_cMyCard[ i ].SetSelected( false );
			}
		}		
	}
}


void CPlayer_BD::Draw_DarkCard()
{
	if( JoinState == 0 )
	{
		return;
	}	

	for( int i = 0 ; i < m_nTotalCardNum ; i++ )
	{
		m_cMyCard[ i ].SetDarkCard( true );
	}

	for( int i = 0 ; i < m_nTotalCardNum ; i++ )
	{
		int nCardValue = m_cMyCard[ i ].GetCardNo();
		
		for( int j = 0; j < m_pMyLowBDRule->m_RuleCompleteCard.m_RuleCompleteCnt; j++ )
		{
			if( m_pMyLowBDRule->m_RuleCompleteCard.RealCard[ j ] ==  nCardValue ) // 족보이루는 카드는 밝게
			{
				m_cMyCard[ i ].SetDarkCard( false );
				break;
			}
		}
	}
}

/* 0 하이(세븐,바둑포함) 1 로우 3 스윙*/
const char* CPlayer_BD::GetValueString( VALUEDATA *data)
{
	m_strValueName = m_pMyLowBDRule->GetFullValueName();

	return m_strValueName.c_str();
}

//이팩트 효과 지우기
void CPlayer_BD::ClearObjectEffect(OBJECT_TYPE kind, bool ballclear)
{
	if ( true == ballclear )
	{
		m_pEffectObjectList->Destroy();
	}
	else
	{
		m_pEffectObjectList->DelObject( kind );
	}
}

//미리 바꿀 카드를 선택 해 두었는지 검사 후 바꾸기 버튼 , 패스 버튼 활성화 한다.
int CPlayer_BD::IsCutCardSelectedNum()
{
	int cnt = 0;
	
	for ( int i = 0 ; i < TOTAL_CARD_BD ; i ++ )
	{
		if ( m_cMyCard[ i ].IsSelected() )
		{
			cnt++;
		}
	}
	return cnt;	
}

void CPlayer_BD::SetCardResult()
{	
	int arrTcard[ BD_TOTAL_CARD ] = { -1,};	

	int cnt = 0 ;
	for ( int i = 0 ; i < m_nTotalCardNum ; i ++ )
	{
		if ( m_cMyCard[ i ].IsVaildCardNo() )
		{
			arrTcard[ i ] =	m_cMyCard[ i ].GetCardNo();
			cnt++;
		}
	}

	m_pMyLowBDRule->m_nGameMode = 0;
	m_pMyLowBDRule->SetCard( PNum, arrTcard, cnt );
	m_pMyLowBDRule->GetMyRule();
	m_pMyLowBDRule->GetFullValueName();

	// 자신의 카드위에 족보명을 보위기 위함
	g_pGameView->m_cPlayerDrawManager.MyJokboNameUpdate();
	g_pGameView->m_cGameHelper.CalculationProbability();
}

//카드 배열 정렬 기본형,일자형
void CPlayer_BD::set_CardArray()
{	
	if( GetCurrentWhere() != IDX_GAMEWHERE_GAME || g_ObserverMan.IsMyObserver() || g_RI.State == 0 || PNum != 0 ) return;
	
	for ( int i = 2 ; i < m_nTotalCardNum ; i ++ )
	{
		if( g_Config.bCardArray == FALSE )
		{
			// 기본형으로 바꿈
			POINT pt = m_cMyCard[ i].GetMovingCurPos();
			if( pt.y == m_sUIPos.PT_Card.y )
			{
				pt.y -= CARD_GAB_Y;
				m_cMyCard[ i].SetMovingCurPos( pt );
			}
		}
		else{
			//일자형으로 바꿈
			POINT pt = m_cMyCard[ i ].GetMovingCurPos();
			if ( pt.y == ( m_sUIPos.PT_Card.y - CARD_GAB_Y ) )
			{
				pt.y += CARD_GAB_Y;
				m_cMyCard[ i ].SetMovingCurPos( pt );
			}
		}
	}
}

//다이 유저 카드 닫음 (본인 제외)
void CPlayer_BD::FoldUser_CardClose()
{
	if( (!g_ObserverMan.IsMyObserver() && PNum == 0)  || bFold != TRUE  )return;

	for( int i = 1 ; i < TOTAL_CARD_BD ; i ++ )
	{
		POINT startpos ,destpos;
		startpos.x = m_cMyCard[ i ].m_Xp;
		startpos.y = m_cMyCard[ i ].m_Yp;
		destpos.x = m_cMyCard[ 0 ].m_Xp + ( i * 10 );
		destpos.y = m_cMyCard[ 0 ].m_Yp;

		float spd = g_GameSpeed.get_CardSpeed( 0,IDX_GAME_BD);
		m_cMyCard[ i ].StraightMoveTo(startpos, destpos,spd ,0);		
	}
}

void CPlayer_BD::FoldUser_CardOpen()
{
	if( !g_ObserverMan.IsMyObserver() && PNum == 0 )return;

	for( int i = 1 ; i < TOTAL_CARD_BD ; i ++ )
	{
		POINT startpos ,destpos;
		startpos.x = m_cMyCard[ i ].m_Xp;
		startpos.y = m_cMyCard[ i ].m_Yp;
		destpos.x = m_cMyCard[ 0 ].m_Xp + ( i * 29 );
		destpos.y = m_cMyCard[ 0 ].m_Yp;

		float spd = g_GameSpeed.get_CardSpeed( 0, IDX_GAME_BD );
		m_cMyCard[ i ].StraightMoveTo(startpos, destpos,spd ,0);		
	}
}

//max방에서 보우금액 업데이트를 위한 함수 추가
INT64 CPlayer_BD::GetNowMoney()
{
	if( g_RI.bIsMaxRoom )
	{
		return UI.GetInMoney();
	}
	else if( g_RI.ChipKind==1 )
	{
		return nPlayGold;
	}

	return UI.GetMoney();
}

void CPlayer_BD::SetNowMoney( INT64 nMoney )
{
	if( g_RI.bIsMaxRoom )
	{
		UI.SetInMoney( nMoney );
	}
	else if( g_RI.ChipKind==1 )
	{
		nPlayGold = nMoney;
	}
	else
	{
		UI.SetMoney( nMoney );
	}
}


