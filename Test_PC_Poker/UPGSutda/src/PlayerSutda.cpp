// PlayerSutda.cpp: implementation of the PlayerSutda class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlayerSutda.h"
#include "GlobalGame.h"
#include "SelectCardSutda.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CPlayerSutda::CPlayerSutda()
{	
	ServPNum = 0;

	PNum = -1;

	Clear();
}

CPlayerSutda::~CPlayerSutda()
{
}

void CPlayerSutda::Clear()
{
	ZeroMemory(&UI, sizeof(UI));
	ZeroMemory(&m_cRoomChangeInfo, sizeof(CHANGE_USERINFO_ROOM));
	Reset();
	bFold = FALSE;

	MyIpCheck = -1;

	nSndFxKind = 0;
	
	nRoundPrevMoney = 0;
	m_bLastHalf = FALSE;		

	//2006.3.20
	m_nCardChoiceLimtCnt = 0;

	m_nTurnLimitCnt = 0;
}

void CPlayerSutda::SetPlayerNum(int pnum)
{
	PNum = pnum;
}

void CPlayerSutda::SetNewPlayer(USERINFO *pUI)
{	
	Clear();
	CPlayer::SetNewPlayer(pUI);

	InitRoomData();
	Avatar.SetAvatar(UI.AvatarInfo);
}

void CPlayerSutda::Reset()
{
	JoinState = EGAME_JOINOUT;
	PlayState = EPLAYER_GAMEEND;

	m_nCardTotalCount = 0;
	m_nOpenCardTotalCount = 0;
	bFold = FALSE;
	bFoldOpen = FALSE;

	m_n64ResultMoney = 0;
	nRoundPrevMoney  = 0;

	// ī�� ����
	for(int i = 0; i < CARD_TOTALCOUNT_BUF; ++i) 
	{
		m_cMyCard[i].Clear();
	}
	m_bResultCardOpenMove = FALSE;

	m_nSDelay = -1;	
	ZeroMemory(m_cCardRectEventEx, sizeof(m_cCardRectEventEx));
	m_nTotalCardCount = 0;

	m_bLastHalf = FALSE;

	//�̹��� ��ο� ���� ����Ʈ 
	m_cPlayerEffectList.Destroy();	

	m_nThrowCardNum = -1;

	m_bHiddenScene = FALSE;


	//����
	m_nJokboPoint = -1;
	m_nJokboAniPoint = -1;
	m_strJokboName.Empty();
	m_vecJokboIndex.clear();

	ZeroMemory(m_nOrignalCard, sizeof(m_nOrignalCard));	
	m_llPenaltyMoney = 0;
	m_ePenaltyState = PSTATE_NONE;


	SetResultCardOpenAni(FALSE);	

}


//�濡 �����Ҷ� �ѹ� �ʱ�ȭ �ϴ°͵� 
void CPlayerSutda::InitRoomData()
{
	Reset();

	ENUM_POSTYPE nPosType = EPT_UNKNOWN;
	switch(PNum)
	{
	case 0:	// �ڽ�
		{			
			nPosType = EPT_ME;
		} break;
	case 1:	//����
	case 2:
		{
			nPosType = EPT_LEFT;		
		} break;
	case 3:	// ����
	case 4:
		{
			nPosType = EPT_RIGHT;
		} break;
	default:
		return;
	}

	CPoint pt;
	pt.x = 12;
	pt.y = 0;

	m_ptCardStart   = g_cUIData.m_listCardStartPos[PNum];
	m_ptAvatarStart = g_cUIData.m_listAvatarPos[PNum];

	// ���� Ĩ ������ ��� �ʱ�ȭ
	m_cMyChip.Init(&g_pGameView->Page, &g_sprChip, PNum, &g_cUIData);
}

void CPlayerSutda::OnTimer()
{	
	// ���� Ĩó��
	m_cMyChip.OnMyChipTimer(timeGetTime());


	// ī�� ó��
	for(int i = 0; i < m_nTotalCardCount; ++i)
	{
		m_cMyCard[i].OnTimer();
	}

	// ����� ī�� �̵� ó�� �κ� 
	if(m_bResultCardOpenMove == TRUE)
	{
		if(m_nSDelay > 0)
		{
			m_nSDelay--;
			if(m_nSDelay <= 0 )
			{
				m_nSDelay = 0;				
				g_pGameView->m_cPlayerDrawManager.SetPlayerGameOver(0, true);
			}
		}

		int cnt = m_nTotalCardCount;

		for(int i = 0; i < m_nTotalCardCount; ++i)
		{
			if(m_cMyCard[i].GetMove().IsMoveStart() == FALSE /*&& m_cMyCard[i].IsCardOpen() == TRUE &&*/ /*m_cMyCard[i].GetDelay() == 0*/)
			{
				cnt--;
			}
		}

		if(cnt <= 0)
		{			
			m_bResultCardOpenMove = FALSE;
		}		
	}
	else if(m_bResultCardOpenMove == FALSE && m_nSDelay == 0)
	{
		// ����� ������ ������� �����ش�
		int nCount = 0 ;
		for(int i = 0 ; i < m_nTotalCardCount ; i++)
		{
			if(m_cMyCard[i].GetMove().IsMoveStart() == FALSE && /*m_cMyCard[i].GetDelay() == 0 && */m_cMyCard[i].IsCardFront() == TRUE)
			{
				++nCount;
			}
		}

		if(nCount == m_nTotalCardCount -1)
		{						
			g_pGameView->m_cPlayerDrawManager.SetPlayerGameOver(0, true);
		}
	}

	m_cPlayerEffectList.ProcessAll();	
}

void CPlayerSutda::OnSetChip(INT64 money)
{
	if(PlayState != EPLAYER_GAMEPLAYING && JoinState != EGAME_JOININ && bFold)
	{
		return;		
	}

	m_cMyChip.SetMoneyChip(money);
	m_cMyChip.SetMyMoveChip();	
}

// ī��� �������� �׸��°ͺ��� �Ʒ� �ִ�
void CPlayerSutda::OnDraw_Bottom(CDC *pDC)
{
	
	m_cPlayerEffectList.BottomDrawAll(pDC);	
}

void CPlayerSutda::OnDraw_Middle(CDC *pDC)
{
	m_cPlayerEffectList.MiddleDrawAll(pDC);
}

//�� m_cPlayer �� ������ �� �̹����� 
void CPlayerSutda::OnDraw_Top(CDC *pDC)
{	
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) return;

	m_cPlayerEffectList.TopDrawAll(pDC);	

	// ��Ĩ �׸���
	if(g_RI.ChipKind == (int)ROOM_CHIPKIND_GOLD && m_cMyChip.IsMyChipShow())
	{
		m_cMyChip.OnMyChipDraw();
	}	
}


/////////////////ī�� ����///////////////////////////////

// ���̾� ������ ī��� ���η� ���� �׸���.
void CPlayerSutda::OnCardDraw(NMBASE::GRAPHICGDI::CPage *pPage) 
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
	{
		return;
	}

	if(pPage ==NULL)
	{
		return;
	}

	for(int i = 0; i < m_nTotalCardCount; ++i)
	{
		m_cMyCard[i].Draw( pPage, bFold, bFoldOpen );
	}	
 
	/*if(m_nTotalCardCount == 5)
	{
		for(int i = 0; i < m_nTotalCardCount; ++i) 
		{
			char szTemp1[256] = {0, };
			sprintf(szTemp1, "------ ����ī�� => Player:%d ī���ȣ:%d �յ�:%d \n", PNum, m_cMyCard[i].GetCardNum(), m_cMyCard[i].IsCardFront());
			OutputDebugString(szTemp1);
		}			
	}*/
}

BOOL CPlayerSutda::IsCardFront(int index)
{
	return m_cMyCard[index].IsCardFront();
}

// ī���� �ո��� ���̰� �Ѵ�.
void CPlayerSutda::SetCardFrontShow(int index)
{
	if(index < 0 || index >= m_nTotalCardCount)
		return;
	m_cMyCard[index].SetCardFront(TRUE);		
}

// ī���� �޸��� ���̰� �Ѵ�. 
void CPlayerSutda::SetCardBackShow(int index)
{
	if(index < 0 || index >= m_nTotalCardCount) 
		return;	
	m_cMyCard[index].SetCardFront(FALSE);
}

void CPlayerSutda::ShowCard(int index , BOOL bshow)
{
	if(index < 0 || index >= m_nTotalCardCount) 
		return;	
	m_cMyCard[index].SetCardShow(bshow);
}

void CPlayerSutda::SetResultFace(int index, BOOL bresult)
{
	if(index < 0 || index >= m_nTotalCardCount) 
		return;

	for(int i = 0 ; i < m_nTotalCardCount;i++)
	{
		m_cMyCard[i].SetResultDarkDraw(bresult);
	}
}

// 0 ~ 6 �� 7��
int CPlayerSutda::GetCardNum(int index)
{
	if(index < 0 || index >= m_nTotalCardCount) 
		return 0;
	return m_cMyCard[index].GetCardNum();
}

CPoint CPlayerSutda::GetCardPos(int index)
{
	if(index < 0 || index >= m_nTotalCardCount) 
		return 0;
	return m_cMyCard[index].GetPos();
}

void CPlayerSutda::SetCardPos(int index, CPoint ptPos)
{
	if(index < 0 || index >= m_nTotalCardCount) 
		return;
	return m_cMyCard[index].SetCardPos(ptPos.x, ptPos.y);
}


// ī�带 �� ���� ���·� ������.
void CPlayerSutda::SetGameResultCardOpen(int delay)	// ī�� ���� (�ִ� ó�� ����)
{
	m_bResultCardOpenMove = TRUE;
	m_nSDelay = delay;

	int ndelay = delay;	
}

void CPlayerSutda::SetChioceCardMove(int face)
{
	for(int i = m_nTotalCardCount -1 ; i >= 0; --i)
	{			
		if(face != 0)
		{
			if(face == 1)
			{
				m_cMyCard[i].SetCardFront(FALSE);
			}
			else
			{
				m_cMyCard[i].SetCardFront(TRUE);
			}			
		}
	}	
}

// ī�� ������ Ŭ�� �ߴ��� üũ
BOOL CPlayerSutda::IsGameOverPtInCardRect(int xp,int yp) 
{
	// ���� �϶� ��ī�� ����
	if(!g_ObserverMan.IsMyObserver() && JoinState != EGAME_JOINOUT && PlayState == EPLAYER_GAMEEND && bFold == TRUE && PNum == 0 )
	{	
		CPoint pt;
		pt.x = xp;
		pt.y = yp;

		CRect temp[7];
	
		for( int i = 0 ; i < m_nTotalCardCount ; ++i )
		{
			temp[i] = m_cCardRectEventEx[i];
		}

		temp[4].SetRect(m_cMyCard[4].GetPos().x, m_cMyCard[4].GetPos().y, m_cMyCard[4].GetPos().x + CARD_WIDTH, m_cMyCard[4].GetPos().y + CARD_HEIGHT);

		for( int i = 0 ; i < m_nTotalCardCount ; ++i )
		{
			if( temp[i].PtInRect (pt) == TRUE )
			{
				for(int x = 0 ; x < m_nTotalCardCount ; ++x )
				{
					if(IsCardFront(x) == FALSE)
					{
						SetCardFrontShow(x);
					}
				}
				return TRUE;
			}
		}
		
		if( bFoldOpen == FALSE )
		{
			for( int i = 0 ; i < m_nTotalCardCount ; ++i )
			{
				if(IsCardFront(i) == TRUE)
				{
					SetCardBackShow(i);
				}
			}
		}
	}

	return FALSE;
}

// ī�� ������ �ִ���
int CPlayerSutda::EventOverPtInCardRect(CPoint pt)
{
	for(int i = 0 ; i < m_nTotalCardCount ; ++i)
	{
		if(m_cCardRectEventEx[i].PtInRect (pt) == TRUE && i < m_nTotalCardCount)
		{
			return i;
		}
	}
	return -1;
}




// ����Ű ó���� �Ѵ�.
void CPlayerSutda::GameKeyProcess(int nVKey, int nExtend)
{
	//������ ���� ���� : �������� �ȴ� ���¿��� Ű�� ������ �����ϵ��� ���� (������)
	if( g_RI.State == EGAME_PLAYING &&  /*g_Poker.CurPlayer == ServPNum &&*/ JoinState == EGAME_JOININ && PlayState == EPLAYER_GAMEPLAYING && /*PNum == 0 &&*/ !g_pGameView->m_cPlayerDrawManager.IsFlyWindCard())
	{
		for( int i = KEY_BET_DIE; i <= KEY_BET_HAF; ++i )
		{
			for( int j = 0; j < MAX_GAME_KEY; ++j )
			{				
				if( IS_VKEY( G_KEYDEF[i][j], nVKey, nExtend ) )
				{
					switch( i )
					{
					case KEY_BET_DIE:
						{
							if( g_Poker.nAutoBetNodie != 1 ) 
							{
// 								CString szTemp;
// 								szTemp.Format("GameKey(): gPPNum:%d, PNum:%d, ����ID:%s \n", g_Poker.CurPlayer, ServPNum, g_PLAYER(0)->UI.ID);
// 								AddGameChatViewEdit(szTemp.GetBuffer(), GetChatColor(ECC2_RESULT_JOKBO));
								g_GAMEPROCESS()->SendDieButton(true);
							}							
						}
						break;
					case KEY_BET_CHK:
						{
							if( g_Poker.nAutoBetCmd <= 0)
							{
								g_GAMEPROCESS()->SendCheckButton(true);
							}
						}
						break;
					case KEY_BET_CAL:
						{
							if( g_Poker.nAutoBetCmd <= 0)
							{
								g_GAMEPROCESS()->SendCallButton(true);
							}
						}
						break;
					case KEY_BET_PNG:
						{
							if( g_Poker.nAutoBetCmd <= 0)
							{
								g_GAMEPROCESS()->SendPingButton(true);
							}
						}
						break;
					case KEY_BET_DAD:
						{
							if( g_Poker.nAutoBetCmd <= 0)
							{
								g_GAMEPROCESS()->SendDdaButton(true);
							}							
						}
						break;
					case KEY_BET_HAF:
						{
							if( g_Poker.nAutoBetCmd <= 0)
							{	
								g_GAMEPROCESS()->SendHalfButton(true);								
							}							
						}
						break;

					default:
						break;
					}					
				}
			}
		}
	}	
}


void CPlayerSutda::OnLButtonDown(int xp, int yp)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) return;
	if( PNum != 0 || g_ObserverMan.IsMyObserver() || g_GAMEPROCESS() == NULL) return;

	POINT pt;
	pt.x = xp;
	pt.y = yp;


	if( JoinState != EGAME_JOINOUT && PlayState != EPLAYER_GAMEEND && m_nCardTotalCount == g_GAMEPROCESS()->GetRoundMaxCard() )
	{
		//���� ������
		if(g_pGameView->m_cPlayerDrawManager.IsFlyWindCard()==FALSE)
		{
			//���� ������
			for(int i=0; i<m_nTotalCardCount; i++)
			{
				if (m_cMyCard[i].OnLButtonDown(xp, yp))
				{
					//����ī��� ����ī�� ��ġ�� �ٲ۴�. (����ī�� ������ ����)
					SwapHiddenCard();
				}
			}

		}

// 		if( g_pGameView->m_cPlayerDrawManager.IsHiddenCardShow() == TRUE && g_pGameView->m_cPlayerDrawManager.IsFlyWindCard() == TRUE )
// 		{
// 			return;
// 		}
// 
// 		if( m_cMyCard[6].IsPtInCard(pt)  )
// 		{
// 			g_GAMEPROCESS()->SetMouseHiddenState( 1 );
// 			g_GAMEPROCESS()->SetMouseHiddenClick(TRUE);
// 			g_GAMEPROCESS()->SetHiddenClickPos(pt);
// 			SetCardFrontShow(6); // ���̱�
// 			return ;
// 		}
// 		g_GAMEPROCESS()->SetMouseHiddenClick(FALSE);
	}
}

void CPlayerSutda::OnLButtonUp(int xp, int yp)
{
	if( GetCurrentWhere() != IDX_GAMEWHERE_GAME ) return;

	if( PNum != 0 || g_ObserverMan.IsMyObserver() || g_GAMEPROCESS() == NULL) return;	

	


	if(JoinState != EGAME_JOINOUT && PlayState != EPLAYER_GAMEEND && m_nCardTotalCount == g_GAMEPROCESS()->GetRoundMaxCard() )
	{

		if(g_pGameView->m_cPlayerDrawManager.IsFlyWindCard()==FALSE) 
		{
			//���� ������
			for(int i=0; i<m_nTotalCardCount; i++)
			{

				if (m_cMyCard[i].OnLButtonUp(xp, yp))
				{
					//������ �Ϸ���
					CompleteJJogi(TRUE);	///�����⸦ �Ϸ��Ѵ�.				

				}				
			}
		}


// 		if( g_pGameView->m_cPlayerDrawManager.IsHiddenCardShow() == TRUE || g_pGameView->m_cPlayerDrawManager.IsFlyWindCard() == TRUE ) return;
// 
// 		if( g_GAMEPROCESS()->IsMouseHiddenClick() == TRUE)
// 		{
// 			HiddenOpne();
// 		}
	}
}

void CPlayerSutda::OnMouseMove(int xp, int yp)
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) 
		return;

	if( PNum == 0 && !g_ObserverMan.IsMyObserver() && bFold == TRUE )
	{
		IsGameOverPtInCardRect(xp, yp);
	}
	
	if( PNum != 0 || g_ObserverMan.IsMyObserver() || g_GAMEPROCESS() == NULL) 
		return;

	if( JoinState != EGAME_JOINOUT && PlayState != EPLAYER_GAMEEND && m_nCardTotalCount == g_GAMEPROCESS()->GetRoundMaxCard() )
	{	

		if(g_pGameView->m_cPlayerDrawManager.IsFlyWindCard()==FALSE) 
		{		
			//���� ������
			for(int i=0; i<m_nTotalCardCount; i++)
			{
				if (m_cMyCard[i].OnMouseMove(xp, yp))
				{
					//������ �Ϸ���
					CompleteJJogi(TRUE);	///�����⸦ �Ϸ��Ѵ�.				

				}
			}
		}

		


// 		if( g_GAMEPROCESS()->IsMouseHiddenClick() && g_pGameView->m_cPlayerDrawManager.IsHiddenCardShow() == FALSE )
// 		{			 		
// 			GetHiddenCardMoveWay( xp, yp );		
// 		}		
	}	
}

int CPlayerSutda::GetHiddenCardMoveWay( int xp, int yp )
{

	int moveway = 0;
 	int nMoveGapX = xp - g_GAMEPROCESS()->GetHiddenClickPos().x;
 	int nMoveGapY = yp - g_GAMEPROCESS()->GetHiddenClickPos().y;

	//�����ȼ� �̻� �̵������� ���� �����Ѵ�.
	if( abs( g_GAMEPROCESS()->GetHiddenArrowPosX() ) > 50 || abs( g_GAMEPROCESS()->GetHiddenArrowPosY() ) > 80 )	
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

	g_GAMEPROCESS()->SetHiddenArrowPosX( g_GAMEPROCESS()->GetHiddenArrowPosX() + nMoveGapX );
	g_GAMEPROCESS()->SetHiddenArrowPosY( g_GAMEPROCESS()->GetHiddenArrowPosY() + nMoveGapY );

	POINT pt;
	pt.x = xp;
	pt.y = yp;

	g_GAMEPROCESS()->SetHiddenClickPos( pt );

	return moveway;
}

void CPlayerSutda::HiddenOpne()
{
	PBPlayEffectSound(SND_HIDDEN_OPEN);

	g_pGameView->m_cPlayerDrawManager.SetFlyWindCard(false);
	SetCardFrontShow(6); // ���̱�

	g_pGameView->m_cPlayerDrawManager.SetHiddenCardShow(true);

	int nOldPoint = g_GAMEPROCESS()->GetSevenRuleManager().get_MyRule(0);
	SetCardResult();

	DrawEft_CompleteMadeValue( nOldPoint );
	//g_pGameView->m_cGameHelper.CalculationProbability();	

	g_GAMEPROCESS()->EndCardOpenGuide();		//���¾ִϸ��̼��߰�
}

//����Ʈ ȿ�� �����
void CPlayerSutda::ClearEffectSpr(ENUM_ANIFILEID kind, BOOL ballclear)
{
	if( ballclear == TRUE )
	{
		m_cPlayerEffectList.Destroy();
	}
	else
	{
		m_cPlayerEffectList.DelObject(kind);
	}
}

void CPlayerSutda::SetCardResult()
{	
	int nCardNum[10] = {0, };
	int nTotalCardCount = g_Poker.PS[ServPNum].nCardTotal;

	if(nTotalCardCount < 3)
	{
		return;
	}

	for(int i = 0; i < 2; ++i)
	{
		nCardNum[i] = GetCardNum(i);
	}


	int nMinNum = min(nTotalCardCount, CCardRule::TOTAL_CARD);
	for(int i = 2; i < nMinNum; ++i)
	{
		if(i == 6) 
		{
			nCardNum[i] = GetCardNum(6);
		}
		else  
		{
			nCardNum[i] = g_Poker.PS[ServPNum].nCard[i-2];
		}
	}
	//g_GAMEPROCESS()->GetSevenRuleManager().ClearAll();
	g_GAMEPROCESS()->GetSevenRuleManager().set_total_Card(PNum, nCardNum, nTotalCardCount );
	g_GAMEPROCESS()->GetSevenRuleManager().get_cal_rule_user(PNum);
	
	// �ڽ��� ī������ �������� ������ ����
	g_pGameView->m_cPlayerDrawManager.MyJokboNameUpdate();
}




// ȭ�鿡 ī�带 �߰� �ϰ� �����Ѵ�.L
void CPlayerSutda::SetAddCard( int nCardNum )
{
	//ī�带 ���� �߰��Ѵ�.

	int nRoundMaxCard = g_GAMEPROCESS()->GetRoundMaxCard();

	if(m_nTotalCardCount < 0 || m_nTotalCardCount >= nRoundMaxCard)
	{
		return;// ###[8��Ŀ]###
	}

	// ī�� �ʱ�ȭ
	m_cMyCard[m_nTotalCardCount].Init(nCardNum);
	
	// ī���� ���� Y��ǥ
	CPoint ptCardStart = GetStartCardPt();
	int nCardPosY = ptCardStart.y;	


	int nCardGab = (g_cUIData.m_nMyCardGab);


	int nOffsetLastCard = 0;
	
	///������ī��� �ٷ� �� ī��� ��ģ��
	if (m_nTotalCardCount == (nRoundMaxCard-1))
	{	
		nOffsetLastCard = - (CARD_WIDTH-4);		
	}
	

	// ������ ���� 
	int nCardPosX = ptCardStart.x + (m_nTotalCardCount * nCardGab) + nOffsetLastCard;
	m_cMyCard[m_nTotalCardCount].SetCardPos(nCardPosX , nCardPosY);


	//���⼭ CARD_TOTALCOUNT ������ �ʿ��ϴ�. �����ϰ�� 3�弸�����̶� CARD_TOTALCOUNT �� 2�� �ǵ��� �����ؾ��Ѵ�.


	// �� �ڽ�
// 	if( 0 == PNum)
// 	{
// 		if( m_nTotalCardCount == 4 )
// 		{
// 			m_cCardRectEventEx[3].SetRect(m_cMyCard[3].GetPos().x, 
// 										  m_cMyCard[3].GetPos().y, 
// 										  m_cMyCard[3].GetPos().x + nCardGab, 
// 										  m_cMyCard[3].GetPos().y + CARD_HEIGHT);
// 		}
// 		else if( m_nTotalCardCount == 3 )
// 		{
// 			m_cCardRectEventEx[m_nTotalCardCount].SetRect(m_cMyCard[m_nTotalCardCount].GetPos().x, 
// 														  m_cMyCard[m_nTotalCardCount].GetPos().y, 
// 														  m_cMyCard[m_nTotalCardCount].GetPos().x + CARD_WIDTH, 
// 														  m_cMyCard[m_nTotalCardCount].GetPos().y + CARD_HEIGHT);
// 		}
// 		else if( m_nTotalCardCount <= 2 )
// 		{
// 			m_cCardRectEventEx[m_nTotalCardCount].SetRect(m_cMyCard[m_nTotalCardCount].GetPos().x, 
// 														  m_cMyCard[m_nTotalCardCount].GetPos().y, 
// 														  m_cMyCard[m_nTotalCardCount].GetPos().x + nCardGab, 
// 														  m_cMyCard[m_nTotalCardCount].GetPos().y + CARD_HEIGHT);
// 		}
// 	}	

	// ���� ī�� ������ ���� ��Ų��.
	++m_nTotalCardCount;


}


void CPlayerSutda::Draw_DarkCard()
{
	if(JoinState == EGAME_JOINOUT || PlayState == EPLAYER_GAMEEND || bFold == TRUE )
	{
		return;
	}	

	for(int i = 0 ; i < m_nTotalCardCount ; ++i )
	{
		if( g_GAMEPROCESS()->GetSevenRuleManager().m_cRule[PNum].m_RemainCard[i] > -1 )
		{
			int num = g_GAMEPROCESS()->GetSevenRuleManager().m_cRule[PNum].m_RemainCard[i];
			
			for(int j = 0; j < m_nTotalCardCount ; ++j )
			{
				if( m_cMyCard[j].GetCardNum() == num )
				{
					m_cMyCard[j].SetResultDarkDraw(TRUE);
					break;
				}
			}
		}
	}
}


BOOL CPlayerSutda::OnGameKey(int nVKey, int nExtend, int repeat)
{	
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
	{
		return FALSE;
	}

	if( g_GAMEPROCESS() == NULL) 
	{
		return FALSE;
	}
	
	if( PNum != 0 || g_ObserverMan.IsMyObserver() || JoinState == EGAME_JOINOUT || bFold == TRUE ) 
	{
		return FALSE;	
	}

	
	// ī�� ���̱� ó��
	//if( g_pGameView->m_cPlayerDrawManager.IsFlyWindCard() == FALSE && m_nCardTotalCount == g_GAMEPROCESS()->GetTotalCardNum() && !g_pGameView->m_cPlayerDrawManager.IsHiddenCardShow() )
	
	int nHiddenCardIndex = g_GAMEPROCESS()->GetRoundMaxCard() - 1;
	if (g_pGameView->m_cPlayerDrawManager.IsFlyWindCard() == FALSE && g_GAMEPROCESS()->IsHiddenRound() && m_cMyCard[nHiddenCardIndex].IsReadyToJJogi() )
	{
		bool bHave = false;

		for( int i = KEY_HIDDEN_OPEN; i <= KEY_HIDDEN_SLOW; ++i )
		{
			for( int j = 0; j < MAX_GAME_KEY; ++j )
			{
				if( IS_VKEY( G_KEYDEF[ i ][ j ], nVKey, nExtend ) )
				{
					switch ( i )
					{
					case KEY_HIDDEN_OPEN: 
						{
// 							if( j == 1 && g_Poker.CurPlayer == ServPNum) //< CYS 100705 > // ���Ͽ�delete Ű�º��ý�����
// 							{
// 								continue;
// 							}

							CompleteJJogi(TRUE);

							bHave = true;
						} break;

					case KEY_HIDDEN_SLOW:
						{
							CompleteJJogi(TRUE);



							bHave = true;							
						} break;
					}
				}
			}

			if( bHave )
			{
				// �Ǽ��� ����Ű ���ý� 2�� ȣ�����
				return TRUE;
			}
		}
	}


	// ���ӿ��� ����ϴ� ����Ű ó��
	GameKeyProcess(nVKey, nExtend);


	return TRUE;	
}




void CPlayerSutda::DrawEft_CompleteMadeValue(int oldpoint, bool bForce )
{
	if ( g_ObserverMan.IsMyObserver() || PNum != 0  || bFold == TRUE || PlayState != 1 )
	{
		return;
	}

	int curpoint = g_GAMEPROCESS()->GetSevenRuleManager().get_MyRule(PNum);
	//���� ���� ����(���̵�) �ϼ��� ȿ�� 
	if ( curpoint >= CCardRule::N7C_TITLE)
	{	

		if ( curpoint > oldpoint || bForce ) // ó�� 4��, ������½� ���̵��� ��� ����Ʈ �����ش�.
		{
			CMadeEffect *pObj = NULL;
			SAFE_NEW( pObj, CMadeEffect );
			pObj->Init(m_nTotalCardCount);
			g_pGameView->m_cAniSprListManager.ADDObject(pObj);
		}
	}
}




void CPlayerSutda::SwapHiddenCard()		//����ī�尡���� �ٷ� ��ī��� ��ġ�� �ٲ۴�(�����⸦ ����)
{
	int nIndexHiddenCard = m_nTotalCardCount-1;
	int nIndexPrevCard = nIndexHiddenCard-1;

	int nRoundMaxCard = g_GAMEPROCESS()->GetRoundMaxCard();
	ASSERT(nIndexHiddenCard==nRoundMaxCard-1);

	int nCardBackup  = m_cMyCard[nIndexPrevCard].GetCardNum();
	m_cMyCard[nIndexPrevCard].SetCardNum( m_cMyCard[nIndexHiddenCard].GetCardNum() );

	m_cMyCard[nIndexHiddenCard].SetCardNum(nCardBackup);	
	m_cMyCard[nIndexHiddenCard].SetCardFront(TRUE);


}



void CPlayerSutda::CalcJokbo()
{

	///### ������ �κ��� �Լ� ������ ���� üũ ���ش�(����� ī����� ȿ���� ������) ###//
	//[�����߰�]�������ϰ�� ������ ������ ������. ���������� ����.
//	if( accept_watch == FALSE && g_bWatch ) return;	//�����ڰ� �����ð�� 
	//	if(m_bWatch && g_RI.Rule != 2) return;
	//	if(g_RI.Rule!=2) return;

	//### ���� �������� ī���� ������ ���Ѵ�. ###//
	int Nowcardnum = 0;

	for( int i = 0; i < m_nTotalCardCount; i++ ) 
	{
		if( m_cMyCard[ i ].IsCardFront() == TRUE ) Nowcardnum++;
	}
	
	if( Nowcardnum >= CARD_TOTALCOUNT_BUF|| Nowcardnum < 0 ) return;
 
	//ī������ ����� �� ��ŭ ������ �� �����ε��� ���ϱ�
	int jokbo_index = 0;
	m_strJokboName.Empty();
	m_vecJokboIndex.clear();	
	m_nJokboPoint = -1;	//�ʱ�ȭ	

	//ī�带 �����ؼ� ������ ����
	int count = factorial( Nowcardnum )/2;

	int i;
	int point[3]={-1, -1, -1};
	for( i = 0 ; i < count ; i++ )
	{	
		point[i] = g_GAMEPROCESS()->GetPairPoint(GetCardNum(i%2), GetCardNum((i+1)/2+1)); //��������Ʈ ���	

		if( m_nJokboPoint < point[i] ) 
		{
			//�籸, ������, ������� ����
			if( point[i] != 10 && point[i] != 11 && point[i] != 30 && point[i] != 31 )
				m_nJokboPoint = point[i];

			//�ƹ��͵� ���� ������ ������ �־��ش�.
			if(m_nJokboPoint == -1 ) m_nJokboPoint = point[i];
		}
	}

	for( i = 0 ; i< count ; i++ )
	{
		//���� ���� ��ġ�� ����ش�.
		jokbo_index = g_GAMEPROCESS()->GetJokboIndex(GetCardNum(i%2), GetCardNum((i+1)/2+1)); 			
		m_vecJokboIndex.push_back(jokbo_index);
		
		if( point[i%2] != point[(i+1)/2+1] )
		{
			CString strTemp;
			strTemp = g_GAMEPROCESS()->GetPairString(point[i]);			

			if( i > 0 && m_strJokboName.GetLength() > 0 ) 
				m_strJokboName += ", ";		

			m_strJokboName += strTemp;
		}
	}
}



int CPlayerSutda::GetHiddenCardIndex()
{
	//����ī�� �ε��� ��ȯ
	return g_GAMEPROCESS()->GetRoundMaxCard() - 1;
}


void CPlayerSutda::BackupOriginalCard()
{
	for(int i=0; i<g_GAMEPROCESS()->GetRoundMaxCard(); i++)
	{
		m_nOrignalCard[i] = m_cMyCard[i].GetCardNum();		
	}
}


void CPlayerSutda::ResetOrginalCard()	//ó�� �޾Ҵ� ī��� �ٽ� �����Ѵ�.
{
	for(int i=0; i<g_GAMEPROCESS()->GetRoundMaxCard(); i++)
	{
		m_cMyCard[i].SetCardNum(m_nOrignalCard[i]);
		m_cMyCard[i].SetCardFront(TRUE);
	}
}

void CPlayerSutda::SetOrginalCard(char nCard[], int nTotCard)
{
	
	for(int i=0; i<CARD_TOTALCOUNT_BUF; i++)
	{
		if (i < nTotCard)
			m_nOrignalCard[i] = nCard[i];
		else
			m_nOrignalCard[i] = 0;
	}
	
}

void CPlayerSutda::SetOrginalCard( int nIndex, int nCard )
{
	if( nIndex < CARD_TOTALCOUNT_BUF )
	{
		m_nOrignalCard[ nIndex ] = nCard; // ���õǴ� ī�� ���� �ϰ� ����
	}
	else
	{
		ASSERT( 0 ); // �߸��� ī�� �ε��� ����
	}
}


int CPlayerSutda::GetOrginalCard(int nCardIndex)
{
	if (nCardIndex<0 || nCardIndex>= CARD_TOTALCOUNT_BUF)
		return 0;


	return m_nOrignalCard[nCardIndex];

}

void CPlayerSutda::CardResultGather( BOOL bLastPlayer, int nDelay )
{
	//���������� ī�带 �캸�� �������� üũ�Ѵ�.
	//m_LastOpen = r_lastplayer;

	CPoint ptCardPos = m_ptCardStart;
	for( int i = 0; i < m_nTotalCardCount; i++)
	{	
//		if(MyCard[i].CardNo < 0) continue;

		const int DURAITION_CARDGATHER = 200;
		//�������忡 �̺�Ʈ�� ���带 �־��ش�.
		
		CPoint ptCardDestination(ptCardPos.x  + (i * 20), ptCardPos.y);

		m_cMyCard[i].MoveCard(	ptCardDestination, 											
								m_cMyCard[i].GetCardNum(),
								DURAITION_CARDGATHER,
								nDelay+5									
								);

		if( i == m_nTotalCardCount-1 )
		{
			m_cMyCard[i].SetEvent(EVENT_CARD_GATHEREND, PNum, bLastPlayer);			
		}
		
	}

	NMBASE::SOUND::g_Sound.PlayWav(SND04);	
}


void CPlayerSutda::CardResultOpen(BOOL bLastPlayer)
{
	//ī�� ������
	
	if (g_GAMEPROCESS()->GetRoundMaxCard()==3)
	{
		m_cMyCard[2].Clear();	// 3��°�� Ŭ����

		//ī�����
		SetCardTotal(2);
		m_nTotalCardCount = 2;

	}
	
	

	//CPoint ptCardPos = m_cMyCard[0].GetPos();
	for( int i = 0; i <2 ; i++ )
	{	
		const int DURAITION_CARDOPEN = 200;
		//�������忡 �̺�Ʈ�� ���带 �־��ش�.

		m_cMyCard[i].SetCardNum( m_nOrignalCard[i] );
		m_cMyCard[i].SetCardFront(TRUE);

		// ī���� ���� Y��ǥ
// 		int nCardPosY = m_ptCardStart.y;
// 		int nCardGab = (g_cUIData.m_nMyCardGab);		
// 		int nCardPosX = (nCardGab/2) + m_ptCardStart.x + (i * nCardGab);	//ī��迭�� �߾�����		

		//CPoint ptCardDestination(nCardPosX, nCardPosY);
		CPoint ptCardDestination  = GetFinalCardPt(2, i);

		m_cMyCard[i].MoveCard(			
			ptCardDestination, 											
			m_cMyCard[i].GetCardNum(),
			DURAITION_CARDOPEN,
			i*5									
			);	

	}

	CalcJokbo();	//�������

// 	if (PNum==0 )
// 	{
// 		g_pGameView->m_cPlayerDrawManager.MyJokboNameUpdate(); ///$$$$$�����϶�?		
// 	}

	g_pGameView->m_cPlayerDrawManager.PlayerJokboNameUpdate(PNum);


	BOOL bEffetExist  = g_GAMEPROCESS()->ResultJokboExamine(PNum);		///bEffetExist �� ����Ȯ������Ʈ(ī��Ŀ���°�)�� �ִ��� ����


	m_cMyCard[1].SetEvent2(EVENT_CARD_END_RESULT_CARD, PNum, bEffetExist);	//ī�� ������ ������ ������ �˷��ش�.
}

void CPlayerSutda::CardResultOpen_FoldOpen()
{
	int nMaxCard = g_GAMEPROCESS()->GetRoundMaxCard();
	if( nMaxCard == 3 )
	{
		m_cMyCard[nMaxCard].Clear();	// 3��°�� Ŭ����
		SetCardTotal(nMaxCard);			//ī�����
		m_nTotalCardCount = nMaxCard;
	}

	for( int i = 0; i < nMaxCard ; i++ )
	{	
		const int DURAITION_CARDOPEN = 200;
		//�������忡 �̺�Ʈ�� ���带 �־��ش�.

		m_cMyCard[i].SetCardNum( m_nOrignalCard[i] );
		m_cMyCard[i].SetCardFront(TRUE);
		//m_cMyCard[i].SetResultDarkDraw( FALSE ); // ���ٴ� �ش� �÷��׷� �������� �ʴ´�. CMyCard::Draw ���ο��� ó��

		m_cMyCard[i].MoveCard(			
								GetFinalCardPt( nMaxCard, i ), 											
								m_cMyCard[i].GetCardNum(),
								DURAITION_CARDOPEN,
								i * 5									
							);	

	}

	CalcJokbo();	//�������

	g_pGameView->m_cPlayerDrawManager.PlayerJokboNameUpdate(PNum);
}

void CPlayerSutda::GenealogyEffect()
{
	POINT ps;
	ps.x = 0;
	ps.y = 0;

	//������ ���� ī�� ���� �ִϸ��̼��� �����ش�.
	int point = g_GAMEPROCESS()->GetAniPoint( m_nOrignalCard[0], m_nOrignalCard[1] );

// #ifdef _DEBUG
// //	g_pGameView->m_strDebug.Format(" %d , %d, %d, %d, %d", Play[pnum].MyCard[0].CardNo, Play[pnum].MyCard[1].CardNo, \
// //	Play[pnum].MyCard[2].CardNo, Play[pnum].MyCard[3].CardNo, pnum);
// #endif

	if( point > 19 ) return;	//���� ���Ͽ� ��Ȯ��ȿ���� ����.

	int cx = 0;
	int cy = 0;

	int totdelay = 0;

	//��Ȯ��ȿ��. [��/��/��]�� �����Ѵ�.
	CPoint ptFirstCard = GetCardPos(0);	
	CPoint ptSecondCard = GetCardPos(1);

	//38���� �� ���� 
	if( point == 0 || point == 1 || point == 2 )
	{	
		int card1 = 0;
		int card2 = 0;

		//ps.x = Play[pnum].MyCard[0].Xp; 		ps.y = Play[pnum].MyCard[0].Yp;
			
		g_pGameView->m_AniQue.Ani_Insert( ANIMATION_CARDGROWTH, ptFirstCard, PNum , 0, GetCardNum(0)-1, FALSE, NULL, NULL, totdelay );//ī�� Ŀ���°�
		g_pGameView->m_AniQue.Ani_Insert(ANIMATION_CARDGROWTH_EFT, ptFirstCard, 0, 0, 11, FALSE, SND13,  NULL, 30 + totdelay );				//��½(����)�̴°�

		//ps.x = Play[pnum].MyCard[1].Xp; ps.y = Play[pnum].MyCard[1].Yp;
	
		g_pGameView->m_AniQue.Ani_Insert( ANIMATION_CARDGROWTH, ptSecondCard, PNum, 1, GetCardNum(1)-1, FALSE, NULL,  NULL, 30 + totdelay );
		g_pGameView->m_AniQue.Ani_Insert(ANIMATION_CARDGROWTH_EFT, ptSecondCard, 0, 0, 11, FALSE, SND13,  NULL,  50 + totdelay );
		
		//ī���ȣ�� ���� ������ ����Ʈȿ���� ���� ī�� ������ ���ش�.
		if( point == 0 )
		{
			//3,8�� ī�带 �������ش�.
			if(GetCardNum(0) == 5) 
			{
				card1 = 10; card2 = 13;
			}
			else { card1 = 13; card2 = 10; }
		}
		else if( point == 1 )
		{
			//1,3�� ī�带 �������ش�.
			if(GetCardNum(0) == 1) 
			{
				card1 = 12; card2 = 10;
			}
			else { card1 = 10; card2 = 12; }
		}
		else if( point == 2 )
		{
			//1,8�� ī�带 �������ش�.
			if(GetCardNum(0) == 1) 
			{
				card1 = 12; card2 = 13;
			}
			else { card1 = 13; card2 = 12; }
		}
		//ps.x = Play[pnum].MyCard[0].Xp; ps.y = Play[pnum].MyCard[0].Yp;
		g_pGameView->m_AniQue.Ani_Insert(ANIMATION_CARDGROWTH_EFT, ptFirstCard, 0, 0, card1, FALSE, NULL, NULL, 30 + totdelay);	//�ɳ����°�
		//ps.x = Play[pnum].MyCard[1].Xp; ps.y = Play[pnum].MyCard[1].Yp;
		g_pGameView->m_AniQue.Ani_Insert(ANIMATION_CARDGROWTH_EFT, ptSecondCard, 0, 0, card2, FALSE, NULL, NULL, 55 + totdelay);	

	}
	//�嶯~�涯
	else if( point >= 3 && point <= 12)
	{
		//ps.x = Play[pnum].MyCard[0].Xp; ps.y = Play[pnum].MyCard[0].Yp;
		g_pGameView->m_AniQue.Ani_Insert( ANIMATION_CARDGROWTH, ptFirstCard, PNum, 0, GetCardNum(0)-1, FALSE, NULL, NULL, totdelay );	//ī�� Ŀ���°�
		g_pGameView->m_AniQue.Ani_Insert(ANIMATION_CARDGROWTH_EFT, ptFirstCard, 0, 0, 15, FALSE, SND13, NULL, 30  + totdelay);				//��½(����)�̴°�

		
		//ps.x = Play[pnum].MyCard[1].Xp; ps.y = Play[pnum].MyCard[1].Yp;
		g_pGameView->m_AniQue.Ani_Insert( ANIMATION_CARDGROWTH, ptSecondCard, PNum, 1, GetCardNum(1)-1, FALSE, NULL, NULL, 30  + totdelay);	//ī�� Ŀ���°�
		g_pGameView->m_AniQue.Ani_Insert(ANIMATION_CARDGROWTH_EFT, ptSecondCard, 0, 0, 15, FALSE, SND13, NULL, 50  + totdelay);				//��½�̴°�
		
		//ps.x = Play[pnum].MyCard[0].Xp; ps.y = Play[pnum].MyCard[0].Yp;
		g_pGameView->m_AniQue.Ani_Insert(ANIMATION_CARDGROWTH_EFT, ptFirstCard, 0, 0, GetCardMonthValue(0)-1, FALSE, NULL, NULL, 30  + totdelay);	//�����ε��� ī��ȿ�� 
		//ps.x = Play[pnum].MyCard[1].Xp; ps.y = Play[pnum].MyCard[1].Yp;
		g_pGameView->m_AniQue.Ani_Insert(ANIMATION_CARDGROWTH_EFT, ptSecondCard, 0, 0, GetCardMonthValue(1)-1, FALSE, NULL, NULL, 55  + totdelay);	//�����ε��� ī��ȿ��

	}
	//�˸�~����
	else if( point >= 13 && point <= 19 )
	//else if( point == 13 || point == 14)	//�˸�~����
	{
		int card1 = 0;
		int card2 = 0;

		//ps.x = Play[pnum].MyCard[0].Xp; ps.y = Play[pnum].MyCard[0].Yp;
		g_pGameView->m_AniQue.Ani_Insert( ANIMATION_CARDGROWTH, ptFirstCard, PNum, 0, GetCardNum(0)-1, FALSE, NULL, NULL, totdelay );//ī�� Ŀ���°�
		g_pGameView->m_AniQue.Ani_Insert(ANIMATION_CARDGROWTH_EFT, ptFirstCard, 0, 0, 14, FALSE, SND12, NULL, 30  + totdelay);				//��½(����)�̴°�

		//ps.x = Play[pnum].MyCard[1].Xp; ps.y = Play[pnum].MyCard[1].Yp;
		g_pGameView->m_AniQue.Ani_Insert( ANIMATION_CARDGROWTH, ptSecondCard, PNum, 1, GetCardNum(1)-1, FALSE, NULL, NULL, 30  + totdelay);
		g_pGameView->m_AniQue.Ani_Insert(ANIMATION_CARDGROWTH_EFT, ptSecondCard, 0, 0, 14, FALSE, SND12, NULL, 50 + totdelay);
		
	}
}

void CPlayerSutda::SetCardTornOut(BOOL bFlag)
{

	//������ ī�� ȿ��
	for(int i=0; i<m_nTotalCardCount; i++)
	{
		m_cMyCard[i].SetCardTornOut(bFlag);
	}

}

// �� ���� ��������
int CPlayerSutda::GetCardMonthValue(int index)
{
	if(index < 0 || index >= m_nTotalCardCount) 
		return 0;
	return m_cMyCard[index].GetCardMonthValue();
}

void CPlayerSutda::SetCardBlinking(int nTime) //ī�� �����Ÿ��� ȿ��
{

	//������ ī�� ȿ��
	for(int i=0; i<m_nTotalCardCount; i++)
	{
		m_cMyCard[i].SetCardBlinking(nTime);
	}

}

CPoint CPlayerSutda::GetStartCardPt()
{
	return GetFinalCardPt(g_GAMEPROCESS()->GetRoundMaxCard(), 0);

}



void CPlayerSutda::CompleteJJogi(BOOL bNotifyServer)
{
	//������ �Ϸ� 
	if (JoinState == EGAME_JOINOUT || PlayState == EPLAYER_GAMEEND)
	{
		return;
	}

	int nHiddenCardIndex = g_GAMEPROCESS()->GetRoundMaxCard() - 1;
	
	m_cMyCard[nHiddenCardIndex].CompleteJJogi((PNum==0 && bNotifyServer), GetHiddenCardFinalPos());

	
	NMBASE::SOUND::g_Sound.PlayWav( SND01 );	//�ɱ� ȿ����(����)

	if (PNum==0 && !g_ObserverMan.IsMyObserver())
		g_pGameView->m_cPlayerDrawManager.MyJokboNameUpdate();
}


CPoint CPlayerSutda::GetHiddenCardFinalPos()
{
	//���� ���� ī���� ������ġ 


	int nRoundMaxCard = g_GAMEPROCESS()->GetRoundMaxCard();
	int nPrevCardIndex = nRoundMaxCard-2;
	
	return GetFinalCardPt(nRoundMaxCard, nRoundMaxCard-1);
// 	CPoint ptCardPos = GetCardPos(nPrevCardIndex);
// 
// 	if (PNum==0 || nRoundMaxCard==2)
// 		ptCardPos.x += (g_cUIData.m_nMyCardGab);
// 	else
// 		ptCardPos.x += (g_cUIData.m_nCardGab);
// 
// 	
// 	return ptCardPos;
}

CPoint CPlayerSutda::GetFinalCardPt(int nMaxCard, int nCardIndex)			//ī������� ���� ī�� ��ġ 
{
	//ī�� ����� 3���̸� ó������ ...
	//ī�� ����� 2���̸� ó������ 1/3 ������ ��ġ���� 

	//nMaxCard : �� ǥ���� ī�� ���
	//nCardIndex : ī�� �ε���
	//��ȯ�� ī�尡 ��ġ�� ��ġ


	CPoint pt = m_ptCardStart;
	if (nMaxCard==2)
	{
		if (PNum==0)
			pt.Offset( (CARD_WIDTH / 2) +1, 0);
		else
			pt.Offset( (CARD_WIDTH / 3) +1, 0);
	}

	if (PNum ==0)
	{
		pt.Offset((g_cUIData.m_nMyCardGab) * nCardIndex, 0);
		return pt;
	}
	else
	{
		if (nCardIndex==0)
			return pt;

		pt.Offset(g_cUIData.m_nMyCardGab, 0);
		
		if (nCardIndex==1)
			return pt;

		pt.Offset(g_cUIData.m_nCardGab, 0);

		if (nCardIndex==2)
			return pt;

	}

	return pt;
}

int CPlayerSutda::GetSndFxKind()
{	
	int nServPNum = g_GAMEPROCESS()->GetServPNum_ByPN(PNum);
	if (nServPNum<0 || nServPNum>=MAX_PLAYER)
		return 0;

	return g_Poker.PS[nServPNum].nSndFxKind;
}


BOOL CPlayerSutda::DoesHaveGoodCard()
{
	//ù��° ���忡�� ���� ī�尡 ���Դ���?(��, ��, ��)

	for(int i=0; i<m_nTotalCardCount; i++)
	{
		int nCardNum = m_cMyCard[i].GetCardNum();
		if ( nCardNum==1 || nCardNum==5 || nCardNum==15 )
		{
			return TRUE;
		}
	}

	return FALSE;
}

//max�濡�� ����ݾ� ������Ʈ�� ���� �Լ� �߰�
INT64 CPlayerSutda::GetNowMoney()
{
	if( g_RI.bIsMaxRoom )
	{
		return UI.GetInMoney();
	}
	else if( g_RI.ChipKind==(int)ROOM_CHIPKIND_GOLD )
	{
		return nPlayGold;
	}

	return UI.GetMoney();
}

void CPlayerSutda::SetNowMoney( INT64 nMoney )
{
	if( g_RI.bIsMaxRoom )
	{
		UI.SetInMoney( nMoney );
	}
	else if( g_RI.ChipKind==(int)ROOM_CHIPKIND_GOLD )
	{
		nPlayGold = nMoney;
	}
	else
	{
		UI.SetMoney( nMoney );
	}
}
