// Player.cpp: implementation of the CPlayer_NP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Player_NP.h"
#include "../AniListMan/AniFileListMan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CPlayer_NP::CPlayer_NP()
{
	Clear();
	ServPNum = 0;
	PNum = 0;
	m_i64ResultMoney = 0;
}

CPlayer_NP::~CPlayer_NP()
{
}

void CPlayer_NP::Clear()
{
	ZeroMemory(&UI, sizeof(UI));
	ZeroMemory(&m_cRoomChangeInfo, sizeof(CHANGE_USERINFO_ROOM));
	Reset();
	bFold = FALSE;
	m_bHiddenOpen = FALSE;

	// ### [ �ߺ��� IP�� ] ###
	MyIpCheck = -1;
	// ### [���� �߰� �۾�] ###
	nSndFxKind = 0;
	// +- ������ ǥ��
	m_nFoldRound = 0;
	m_i64MyJackpatMoney = 0;
	nRoundPrevMoney = 0;
	Avatar.ClearMember();

	m_bLastHalf = FALSE;

	//2006.3.20
	m_nCardChoiceLimtCnt = 0;
	m_i64ResultMoney = 0;
}

void CPlayer_NP::SetNewPlayer(USERINFO *pUI)
{
	Clear();
	CPlayer::SetNewPlayer(pUI);
	Avatar.SetAvatar(UI.AvatarInfo);
}

void CPlayer_NP::Reset()
{
	JoinState = 0;
	PlayState = 0;
	
	m_nCardTotal = 0;
	m_nOpenTotal = 0;
	m_bHiddenOpen = FALSE;
	bFold = FALSE;
	bFoldOpen = FALSE;

	m_nFoldRound = 0;
	m_i64MyJackpatMoney = 0;	
	nRoundPrevMoney = 0;
	// ī�� ����	

	for(int i =0; i < CARD_TOTAL; i++) m_listCard[i].Clear();
	m_bCardOPenMove = FALSE;

	m_nSDelay = -1;
	m_bChoiceCardMove = FALSE;			// ������� ī�� �̵� �÷��� 
	ZeroMemory(m_listCardRectEventEx,sizeof(CRect)*8);
	m_nTotalCardNum = 0;

	m_bLastHalf = FALSE;

	//����ī�� 
	m_nThrowAwayCardNo = -1;

	m_nFlagMoneyChipChanged = MONEY_CHIP_NONE;

	//����ī�����
	m_bHiddenScene = FALSE;
}


//�濡 �����Ҷ� �ѹ� �ʱ�ȭ �ϴ°͵� 
void CPlayer_NP::InitRoomData()
{
	Reset();

	CPoint pt;

	if(GAME()->GetMaxPlayer() > PNum)
	{

		m_cMyChip.Init(&g_pGameView->Page, &g_sprChip,PNum, &g_cUIData);
		m_ptPos = g_cUIData.m_listCardStartPos[PNum];
	}
}

void CPlayer_NP::OnTimer()
{	
	if(JoinState == 0)return ;
	m_cMyChip.OnMyChipTimer(timeGetTime());		
	
	int i=0;
	for(i = 0; i < GAME()->GetTotalCardNum(); i++) m_listCard[i].OnTimer(); //// ###[8��Ŀ]###	
	
	// ����� ī�� �̵� ó�� �κ� 
	if(m_bCardOPenMove == TRUE){

		if(m_nSDelay > 0){
			m_nSDelay--;
			if(m_nSDelay <= 0 ){
				m_nSDelay = 0;
				g_pGameView->m_cPlayerDrawManager.SetPlayerGameOver(0, true);
			}

			return;
		}

		for(int i = m_nTotalCardNum -1 ; i >=0  ; i --){		
			SetFaceUp(i);
		}

		m_bCardOPenMove = FALSE;
	}
	else if(m_bCardOPenMove == FALSE && m_nSDelay == 0)
	{
		// ����� ������ ������� �����ش�
		int tcnt = 0 ;
		for(i = 0 ; i < m_nTotalCardNum ; i++)
		{
			if(m_listCard[i].IsMoveStart() == FALSE && m_listCard[i].IsDirect() == FALSE && m_listCard[i].GetDelay() == 0 && m_listCard[i].IsFront() == TRUE){
				tcnt ++;
			}
		}

		if(tcnt == m_nTotalCardNum -1)
		{
			g_pGameView->m_cPlayerDrawManager.SetPlayerGameOver(0, true);
		}		
	}
}

void CPlayer_NP::OnSetChip(INT64 money)
{
	if(PlayState != 1 && JoinState != 1 && bFold)return;
	
	m_cMyChip.SetMoneyChip(money);
	m_cMyChip.SetMyMoveChip();	
}

// �� �ݾ� ǥ�� �ʱ�
void CPlayer_NP::OnReSetBetDraw()
{
	if(PlayState != 1)return;
}


// ī��� �������� �׸��°ͺ��� �Ʒ� �ִ�
void CPlayer_NP::OnDraw_Bottom(CDC *pDC)
{
	if( JoinState == 0 )return;

	//���� ī�� �׸���
	if( m_nThrowAwayCardNo > -1 && PNum == 0  )
	{
		CPoint ptPos = g_cUIData.m_listCardStartPos[0] + g_cUIData.m_ptDisCardGab;
		g_pGameView->Page.PutSprAuto(ptPos.x,ptPos.y,&g_sprDisCard,m_nThrowAwayCardNo);			
	}
}

void CPlayer_NP::OnDraw_Middle(CDC *pDC)
{

}

//�� m_cPlayer �� ������ �� �̹����� 
void CPlayer_NP::OnDraw_Top(CDC *pDC)
{	
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) return;
	if( JoinState == 0 )return;

	DrawDummyHiddenCard(pDC);

	// ��Ĩ �׸���
	if(m_cMyChip.IsMyChipShow())
	{
		m_cMyChip.OnMyChipDraw();
	}	
}


// void CPlayer_NP::OnDraw(CDC *pDC)
// {
// 	if( JoinState == 0 )return;
// 	//���� ī�� �׸���
// 	if( m_nThrowAwayCardNo > -1 && PNum == 0  )
// 	{
// 		CPoint ptPos = g_cUIData.m_listCardStartPos[0] + g_cUIData.m_ptDisCardGab;
// 		g_pGameView->Page.PutSprAuto(ptPos.x,ptPos.y,&g_sprDisCard,m_nThrowAwayCardNo);			
// 	}
// 
// 	if( PlayState == 0 )return;
// 
// 	// ��Ĩ �׸���
// 	if(m_cMyChip.IsMyChipShow())
// 	{
// 		m_cMyChip.OnMyChipDraw();
// 	}	
// }


void CPlayer_NP::DrawDummyHiddenCard(CDC *pDC)
{
	if( GetCurrentWhere() != IDX_GAMEWHERE_GAME ) return;
	if( PNum != 0 ) return;

	if( g_ObserverMan.IsMyObserver() || PlayState == 0 
		|| m_nCardTotal != GAME()->GetTotalCardNum() || g_pGameView->m_cPlayerDrawManager.IsFlyWindCard() ) return;

	NMBASE::GRAPHICGDI::CPage *pPage = NULL;
	pPage = &g_pGameView->Page;
	if( pPage == NULL ) return;

	CPoint ptPos;
	int nCardIndex = 6;
	switch(g_RI.FormKind)
	{
	case 0:
	case 1:
		nCardIndex = 6;
		ptPos = GAME()->GetPlayerNP(0)->m_listCard[6].GetPos();
		break;
	case 2:
	case 3:
		nCardIndex = 7;
		ptPos = GAME()->GetPlayerNP(0)->m_listCard[7].GetPos();
		break;
	}

	CFont *oldf = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
		
	if( !g_pGameView->m_cPlayerDrawManager.IsHiddenCardShow())
	{

		int hX = GAME()->GetPlayerNP(0)->m_listCard[ nCardIndex ].GetPos().x + GAME()->GetHiddenArrowPosX();			
		int hY = GAME()->GetPlayerNP(0)->m_listCard[ nCardIndex ].GetPos().y + GAME()->GetHiddenArrowPosY();			

		if( m_bHiddenScene )
		{
			DWORD dwTemp = 0;

			int mxp = abs( GAME()->GetHiddenArrowPosX() );			
			int myp = abs( GAME()->GetHiddenArrowPosY() );	

			if( mxp > myp )
			{
				dwTemp = mxp * 28;
			}
			else
			{
				dwTemp = myp * 28;
			}

			//���� ���� �ִϸ��̼��� ������.
			if( dwTemp >= 1300 ) 
			{
				GAME()->SetMouseHiddenState( 2 );
				GAME()->SetHiddenSceneDrawTime();
			}
			else
			{
				pPage->DrawMultiScene( ptPos.x, ptPos.y, &g_sprLastCardOpen, 0, dwTemp, FALSE );
			}
		}
		else
		{
			// ���� ī���̹���
			pPage->PutSprAuto( hX, hY, &g_sprBigCard, 52);		
		}

		if( GAME()->IsMouseHiddenClick() == FALSE && GAME()->IskeyboardHiddenClick() == FALSE )
		{
			pDC->SetTextColor(RGB(255,225,255));
			pDC->TextOut(ptPos.x + 23,550,"���");

			// ����ī�� ���� ����Ű �̹����� �����ش�.
			pPage->PutSprAuto( ptPos.x + 9, ptPos.y + 62, &GLOBAL_SPR[ spr::GSPR_V_H_OPEN ], 0 ); //< CYS 100705 >
		}

		//���¾ִϸ��̼��߰�
		if( GAME()->GetCardOpneGuideIndex() != -1 && GAME()->GetCardOpneGuideDraw() )
		{
			pPage->DrawMultiScene( ptPos.x, ptPos.y, &g_sprLastCardOpen, GAME()->GetCardOpneGuideIndex(), timeGetTime() - GAME()->Getm_dwCardOpenGuideTime(), FALSE );

			if( g_sprLastCardOpen.IsMSceneTimeOver(  GAME()->GetCardOpneGuideIndex(), timeGetTime() - GAME()->Getm_dwCardOpenGuideTime() ) )
			{
				GAME()->EndCardOpenGuide();
			}
		}
	}



	//����ī�� ���� ������ �ִϸ��̼�  (������� Ȯ�� �� �̵� �ؾ� �ɵ� ...)
	if( GAME()->GetMouseHiddenState() == 2 )
	{
		pPage->DrawMultiScene( ptPos.x, ptPos.y, &g_sprLastCardOpen, 1, timeGetTime() - GAME()->GetHiddenSceneDrawTime(), FALSE );

		if( g_sprLastCardOpen.IsMSceneTimeOver( 1, timeGetTime() - GAME()->GetHiddenSceneDrawTime() ) )
		{
			GAME()->SetMouseHiddenState( 0 );
			GAME()->SetHiddenSceneDrawTime();
		}
	}
	
	pDC->SelectObject(oldf);
}



/////////////////ī�� ����///////////////////////////////

// ���ο� ī�� ����
void CPlayer_NP::SetNewCard(int cardNum , BOOL bbomb, BOOL bForceYP)
{
	if(m_nTotalCardNum < 0 || m_nTotalCardNum >= GAME()->GetTotalCardNum())
	{
		if(cardNum != 100){
			return;// ###[8��Ŀ]###
		}
	}
	
	m_listCard[m_nTotalCardNum].Init(cardNum, PNum ,bbomb,m_nTotalCardNum); // PNum ������
	m_listCard[m_nTotalCardNum].SetShow(TRUE);  // �ӽ� ( FALSE -> ī�� ������ �޾����� TRUE)
	
	int NewYp = m_ptPos.y;
	if(PNum == 0 ) // ù 3�� �޾��� ���� ���� ���� �׸���.
	{
		if( !g_ObserverMan.IsMyObserver() && GetFace(2) )// ### [ ������� ] ###
		{
			NewYp = m_ptPos.y - 7;
		}
		
		if ( bForceYP )
		{
			NewYp = m_ptPos.y - 7;
		}
	}

	// ������ ����
	if(PNum == 0) m_listCard[m_nTotalCardNum].SetPos(CPoint(m_ptPos.x+(m_nTotalCardNum*g_cUIData.m_nMyCardGab), NewYp));
	else m_listCard[m_nTotalCardNum].SetPos(CPoint(m_ptPos.x+(m_nTotalCardNum*g_cUIData.m_nCardGab), NewYp));

	m_nTotalCardNum ++;
}

// ù�κп��� ī�� ��ġ��ǥ �� !! ����� ��Ӱ� �׸���.
void CPlayer_NP::OnCardDraw(NMBASE::GRAPHICGDI::CPage *pPage, BOOL bFold, BOOL bFoldOpen)
{
	if(pPage ==NULL)return;
	for(int i = 0; i < m_nTotalCardNum; i++){
		m_listCard[i].Draw( pPage, bFold, bFoldOpen );
	}
}

BOOL CPlayer_NP::GetFace(int index)
{
	return m_listCard[index].IsFront();
}

// ī���� �ո�
void CPlayer_NP::SetFaceUp(int index)
{
	if(index < 0 || index >= GAME()->GetTotalCardNum()) return;
	m_listCard[index].SetFront(TRUE);
}

// ī���� �޸� 
void CPlayer_NP::SetFaceDown(int index)
{
	if(index < 0 || index >= GAME()->GetTotalCardNum()) return;
	m_listCard[index].SetFront(FALSE);
}

void CPlayer_NP::SetResultFace(int index ,BOOL bresult)
{
	if(index < 0 || index >= GAME()->GetTotalCardNum()) return;

	for(int i = 0 ; i < m_nTotalCardNum;i++){
		m_listCard[i].SetDark(bresult);
	}
}

// 0 ~ 6 �� 7��
int CPlayer_NP::GetCardNo(int index)
{
	if(index < 0 || index >= GAME()->GetTotalCardNum()) return 0;
	return m_listCard[index].GetCardNum();
}

// ī�带 �� ���� ���·� ������.
void CPlayer_NP::SetGameResultCardOpen(int delay)	// ī�� ���� (�ִ� ó�� ����)
{
	m_bCardOPenMove = TRUE;
	m_nSDelay = delay;
}

void CPlayer_NP::SetChioceCardMove(BOOL bdirect,int face)
{
	m_bChoiceCardMove = TRUE;
	for(int i = 2 ; i >=0  ; i --){			
		m_listCard[i].SetCardFace(TRUE,1,bdirect);
		if(face != 0){
			m_listCard[i].SetFront((face == 1)?FALSE:TRUE);
		}
	}	
}

BOOL CPlayer_NP::OverPtInCardRect(int xp,int yp)// ī�� ������ Ŭ�� �ߴ��� 
{
	for(int i = 0 ; i < m_nTotalCardNum ; i ++){
		if(m_listCard[i].PtInCard(xp,yp) == TRUE){
			return TRUE;
		}
	}

	return FALSE;
}

int CPlayer_NP::EventOverPtInCardRect(CPoint pt)// ī�� ������ �ִ���
{
	int t = m_nTotalCardNum;
	for(int i = 0 ; i < t ; i ++){
		if(m_listCardRectEventEx[i].PtInRect (pt) == TRUE && i < t-1){
			return i;
		}
	}
	return -1;
}

void CPlayer_NP::SetFlag_Money_Chip( int nFlag )
{
	m_nFlagMoneyChipChanged |= nFlag;
}

BOOL CPlayer_NP::IsFlag_Money_Chip( int nFlag )
{
	return 	m_nFlagMoneyChipChanged & nFlag;
}

void CPlayer_NP::DrawEft_CompleteMadeValue(int oldpoint, bool bForce )
{
	if ( g_ObserverMan.IsMyObserver() || PNum != 0  || bFold == TRUE || PlayState != 1 )
	{
		return;
	}

	int curpoint = GAME()->m_cSvRule.get_MyRule(PNum);
	//���� ���� ����(���̵�) �ϼ��� ȿ�� 
	if ( curpoint >= CCardRule::N7C_TITLE)
	{	

		if ( curpoint > oldpoint || bForce ) // ó�� 4��, ������½� ���̵��� ��� ����Ʈ �����ش�.
		{
			CMadeEffect *pObj = NULL;
			SAFE_NEW( pObj, CMadeEffect );
			pObj->Init(m_nTotalCardNum);
			GAME()->m_cAniSprObjectMan.AddObject(pObj);
		}
	}
}


int CPlayer_NP::GetHiddenCardMoveWay( int xp, int yp )
{
	int moveway = 0;

	// 
	int nMoveGapX = xp - GAME()->GetHiddenClickPos().x;
	int nMoveGapY = yp - GAME()->GetHiddenClickPos().y;

	//�����ȼ� �̻� �̵������� ���� �����Ѵ�.
	if( abs( GAME()->GetHiddenArrowPosX() ) > 50 || abs( GAME()->GetHiddenArrowPosY() ) > 80 )	
	{
		HiddenOpne();
		return 0;
	}

	int nxp = abs( nMoveGapX );
	int nyp = abs( nMoveGapY );

	//	4	|	1
	//------------------------
	//	3	|	2

	if( ( nMoveGapX > 0 && nMoveGapY > 0 ) || ( nMoveGapX > 0 && nMoveGapY == 0 ) ) 
	{
		if( xp > yp )
		{
			moveway = 1;
		}
		else if( xp < yp )
		{
			moveway = 2;
		}
		else
		{
			moveway = 1;
		}
	}
	else if( ( nMoveGapX > 0 && nMoveGapY < 0 ) || ( nMoveGapX == 0 && nMoveGapY < 0 ) )
	{
		if( nxp > nyp )
		{
			moveway = 1;
		}
		else if( nxp < nyp )
		{
			moveway = 4;
		}
		else
		{
			moveway = 4;
		}
	}
	else if( ( nMoveGapX < 0 && nMoveGapY > 0 )  || ( nMoveGapX == 0 && nMoveGapY > 0 )  )
	{
		if( nxp > nyp )
		{
			moveway = 3;
		}
		else if( nxp < nyp )
		{
			moveway = 2;
		}
		else
		{
			moveway = 3;
		}
	}
	else if( ( nMoveGapX < 0 && nMoveGapY < 0 )  || ( nMoveGapX == 0 && nMoveGapY < 0 ) )
	{
		m_bHiddenScene = TRUE;

		//�밢�� ���� ���� üũ
		if( nxp > nyp )
		{

		}
		else if( nxp < nyp )
		{

		}
		else
		{

		}
	}


	GAME()->SetHiddenArrowPosX( GAME()->GetHiddenArrowPosX() + nMoveGapX );
	GAME()->SetHiddenArrowPosY( GAME()->GetHiddenArrowPosY() + nMoveGapY );

	POINT pt;
	pt.x = xp;
	pt.y = yp;

	GAME()->SetHiddenClickPos( pt );

	return moveway;
}

void CPlayer_NP::HiddenOpne()
{
	PBPlayEffectSound(SND_HIDDEN_OPEN);

	m_bHiddenOpen = TRUE; // ī�� Ŭ�������� ���̱� �Ѵ�.

	g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(false);

	if(g_RI.FormKind == BOMB_GAME || g_RI.FormKind == JOKER_GAME)
	{
		SetFaceUp(7); // ���̱�
	}
	else
	{
		SetFaceUp(6); // ���̱�
	}

	g_pGameView->m_cPlayerDrawManager.SetHiddenCardShow(true);

	int nOldPoint = GAME()->m_cSvRule.get_MyRule(0);

	GAME()->SetJokerCard();
	g_pGameView->m_cGameHelper.CalculationProbability();

//	GAME()->m_bHiddenClick = FALSE;
	g_pGameView->m_cPlayerDrawManager.MyJokboNameUpdate();

	GAME()->GetPlayerNP(0)->DrawEft_CompleteMadeValue( nOldPoint );

	GAME()->EndCardOpenGuide();		//���¾ִϸ��̼��߰�
}

//max�濡�� ����ݾ� ������Ʈ�� ���� �Լ� �߰�
INT64 CPlayer_NP::GetNowMoney()
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

void CPlayer_NP::SetNowMoney( INT64 nMoney )
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
