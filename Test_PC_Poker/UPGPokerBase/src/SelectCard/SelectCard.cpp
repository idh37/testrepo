// SelectCard.cpp: implementation of the CSelectCard class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SelectCard.h"
#include <CommMsgDef_Game.h>
#include "../UIInfoData.h"
#include "../GameButtonMan/GameViewButtonMan.h"
#include "../Sound.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSelectCard::CSelectCard()
{
	m_bShow = false;
	m_nMaxNum = 0;
	m_pPage = NULL;
	SetChoiceCard(false);
	m_pBigCardSpr = NULL;
	m_ptStart = CPoint(0,0);
	m_pBigDarkCardSpr = NULL;
}

CSelectCard::~CSelectCard()
{
}

void CSelectCard::LoadImage(void)
{
	AddLoadImageList(&m_sprSelectCard,".\\PokerCommon\\data\\selectcard.spr",true);
}

void CSelectCard::Clear()
{
	m_bShow = false;
	SetChoiceCard(false);
	m_cChoiceCardDeck.Clear();
}

void CSelectCard::SetChoiceCard(bool bChoiceCard)
{
	m_bChoiceCard = bChoiceCard;
}

void CSelectCard::Reset()
{
	Clear();
}

void CSelectCard::Init(CUIInfoData *pUIInfoData, CGame *pGame, NMBASE::GRAPHICGDI::xSprite *pBigCardSpr, NMBASE::GRAPHICGDI::xSprite *pBigDarkCardSpr)
{
	m_pUIInfoData = pUIInfoData;
	m_pGameView = pGame->GetGameView();
	m_pGameProcess = pGame->GetGameProcess();
	m_pBigCardSpr = pBigCardSpr;
	m_pBigDarkCardSpr = pBigDarkCardSpr;
	m_pPage = &m_pGameView->Page;
}

void CSelectCard::SetCard(int nGameType, char ncardnum[])
{
	if( g_RI.State == 0 ) return;

	m_nGameType = nGameType;
	m_nMaxNum = 4;
	CRect rtStart = m_pUIInfoData->m_rtSelectCardFirstCard_Choice;
	m_ptStart = m_pUIInfoData->m_ptSelectCardFirstCard_Choice;

	//깜깜이 모드일때는 3장중 버릴카드를 선택하게 된다.
	if(nGameType == NORMAL_GAME || g_RI.cRuleType == RULETYPE_5POKER)
	{
		m_nMaxNum = 3;
		rtStart = m_pUIInfoData->m_rtSelectCardFirstCard_Normal;
		m_ptStart = m_pUIInfoData->m_ptSelectCardFirstCard_Normal;
	}

	rtStart.OffsetRect(m_ptStart);

	Reset();
	for( int i = 0 ; i < m_nMaxNum ; i ++ )
	{
		if(i==0) m_listCardRect[i] = rtStart;
		else
		{
			m_listCardRect[i] = m_listCardRect[i-1];
			m_listCardRect[i].OffsetRect(CPoint(m_pUIInfoData->m_nSelectCardCardGab,0));
		}
		
		if( m_cChoiceCardDeck.nCard[i] == -1 ){
			m_cChoiceCardDeck.nCard[i] = ncardnum[i];
		}		
	}
}

void CSelectCard::OnDraw( CDC *pDC )
{
	if( m_bChoiceCard == false )return;
	if( m_bShow == false )return;	
	
	int nBackIndex = 0;
	
	if(m_nGameType == NORMAL_GAME) nBackIndex = 2;
	else if(g_RI.cRuleType == RULETYPE_5POKER) nBackIndex = 3;
	else nBackIndex = m_cChoiceCardDeck.nDiscardindex == -1 ? 0 : 1 ;	
	
	m_pPage->PutSprAuto(m_ptStart.x, m_ptStart.y, &m_sprSelectCard, nBackIndex);
	for(int i=0; i<m_nMaxNum;i++)
	{
		if(i == m_cChoiceCardDeck.nDiscardindex) m_pPage->PutSprAuto(m_listCardRect[i].left, m_listCardRect[i].top, m_pBigDarkCardSpr, m_cChoiceCardDeck.nCard[i] );
		else m_pPage->PutSprAuto(m_listCardRect[i].left, m_listCardRect[i].top, m_pBigCardSpr, m_cChoiceCardDeck.nCard[i] );
		CPoint ptPos(m_listCardRect[i].left, m_listCardRect[i].top);
		ptPos += m_pUIInfoData->m_ptSlectCardSelectMarkGab;
		
		if( m_cChoiceCardDeck.nDiscardindex == i )
		{
			if(m_cChoiceCardDeck.nMouseOverIndex == m_cChoiceCardDeck.nDiscardindex)
				m_pPage->PutSprAuto(ptPos.x, ptPos.y, &m_sprSelectCard, 6 );
			else
				m_pPage->PutSprAuto(ptPos.x, ptPos.y, &m_sprSelectCard, 4 );
		}
		else if(m_cChoiceCardDeck.nMouseOverIndex == i)
		{
			if(g_RI.cRuleType == RULETYPE_5POKER)	//깜깜이 모드일때는 버릴 카드를 선택한다.
				m_pPage->PutSprAuto(ptPos.x, ptPos.y, &m_sprSelectCard, 4 );
			else if(m_nGameType == NORMAL_GAME)
				m_pPage->PutSprAuto(ptPos.x, ptPos.y, &m_sprSelectCard, 5 );
			else if(m_cChoiceCardDeck.nDiscardindex == -1) 
				m_pPage->PutSprAuto(ptPos.x, ptPos.y, &m_sprSelectCard, 4 );
			else
				m_pPage->PutSprAuto(ptPos.x, ptPos.y, &m_sprSelectCard, 5 );
		}
	}						
}

void CSelectCard::OnGameKey( int nVKey, int nextended, int repeat )
{
	if( m_bShow == false )return;
	if ( nVKey == 0 ) return;

	switch(nVKey) {
	case VK_LEFT:
		{		
			if( m_cChoiceCardDeck.nMouseOverIndex - 1 <= -1 ){
				m_cChoiceCardDeck.nMouseOverIndex = m_cChoiceCardDeck.nDiscardindex == m_nMaxNum - 1 ? m_nMaxNum - 2 : m_nMaxNum - 1;
			}
			else
			{
				m_cChoiceCardDeck.nMouseOverIndex = m_cChoiceCardDeck.nMouseOverIndex - 1;
			}
					
		}break;
	case VK_RIGHT:
		{			
			if( m_cChoiceCardDeck.nMouseOverIndex + 1 >= m_nMaxNum ){
				m_cChoiceCardDeck.nMouseOverIndex = m_cChoiceCardDeck.nDiscardindex == 0 ? 1 : 0;
			}
			else{
				m_cChoiceCardDeck.nMouseOverIndex = m_cChoiceCardDeck.nMouseOverIndex + 1 ;
			}					
		}break;
 	case VK_UP:
 	case VK_DOWN:		
 		{
 			OnSelectCard(m_cChoiceCardDeck.nMouseOverIndex);
 		}
		break;		
	}
	
	for ( int i = KEY_CARD_SELECT_1; i <= KEY_CARD_SELECT_4; i++ )
	{
		for ( int j = 0; j < MAX_GAME_KEY; j++ )
		{
			if ( IS_VKEY( G_KEYDEF[ i ][ j ], nVKey, nextended ) )
			{
				OnSelectCard( i - KEY_CARD_SELECT_1 );
			}
		}		
	}
}

void CSelectCard::OnMouseMove( int x, int y )
{
	if( m_bShow == false )return;

	CPoint pt;
	pt.x = x;
	pt.y = y;	
	
	for( int i = 0 ; i < m_nMaxNum ; i++ ) {
		if( m_listCardRect[i].PtInRect(pt) ) {
			m_cChoiceCardDeck.nMouseOverIndex = i;
			return ;
		}		
	}	

	m_cChoiceCardDeck.nMouseOverIndex = -1;
}

void CSelectCard::OnLButtonDown( int x, int y )
{
}

void CSelectCard::OnLButtonUp(int x, int y)
{
	if( m_bShow == false )return;
	
	POINT pt;
	pt.x = x;
	pt.y = y;

	//깜깜이 모드일때는 버릴 카드만 선택한다.
	if(m_nGameType == NORMAL_GAME || g_RI.cRuleType == RULETYPE_5POKER)
	{
		for( int i = 0 ; i < m_nMaxNum ; i++ ) {
			if( m_listCardRect[i].PtInRect(pt) ) {
				OnSelectCard(i);
				return ;
			}
		}
	}
	else
	{
		if( m_cChoiceCardDeck.nDiscardindex == -1 ){
			// 버릴카드 선택
			for( int i = 0 ; i < m_nMaxNum ; i++ ) {
				if( m_listCardRect[i].PtInRect(pt) ) {
					OnSelectCard(i);
					return ;
				}
			}
		}
		else 
		{
			// 오픈카드 선택
			for( int i = 0 ; i < m_nMaxNum ; i++ ) {
				if( m_cChoiceCardDeck.nDiscardindex != i && m_listCardRect[i].PtInRect(pt) ) {
					OnSelectCard(i);
					return ;
				}
				else if(m_cChoiceCardDeck.nDiscardindex == i && m_cChoiceCardDeck.nDiscardindex == m_cChoiceCardDeck.nMouseOverIndex ){
					// 버릴카드 선택			
					m_cChoiceCardDeck.nDiscardindex = -1;
					m_cChoiceCardDeck.nDiscardnum = -1;				
					
				}
			}
		}
	}
}

void CSelectCard::OnSelectCard( int index )
{	
	if ( index <0 || index>=m_nMaxNum) return;

	PBPlayEffectSound(SND_CARD_SELECT);

	//깜깜이 모드일때는 버릴 카드만 선택한다.
	if(m_nGameType == NORMAL_GAME || g_RI.cRuleType == RULETYPE_5POKER)
	{
		m_cChoiceCardDeck.nOPenCardIndex = index;
		m_cChoiceCardDeck.nOPenCardNum = m_cChoiceCardDeck.nCard[index];
		EndSelectCard();
	}
	else
	{
		if( m_cChoiceCardDeck.nDiscardindex == -1 ){
			// 버릴카드 선택			
			m_cChoiceCardDeck.nDiscardindex = index;
			m_cChoiceCardDeck.nDiscardnum = m_cChoiceCardDeck.nCard[index];
		}
		else {

			if( m_cChoiceCardDeck.nDiscardindex != index ){
				// 오픈카드 선택
				m_cChoiceCardDeck.nOPenCardIndex = index;
				m_cChoiceCardDeck.nOPenCardNum = m_cChoiceCardDeck.nCard[index];
				EndSelectCard();
			}
			else if( m_cChoiceCardDeck.nDiscardindex == index ) {
				m_cChoiceCardDeck.nDiscardindex = -1;
				m_cChoiceCardDeck.nDiscardnum = -1;
			}	
		}
	}
}

void CSelectCard::EndSelectCard(BOOL bautoselect)
{
	if( m_bChoiceCard == false )return;

	m_bChoiceCard = false;
	//오픈한 카드가 없다면
	if( m_cChoiceCardDeck.nOPenCardIndex == -1 && m_cChoiceCardDeck.nOPenCardNum == -1 )
	{
		//깜깜이 모드일때는 버릴 카드만 선택한다.
		if(m_nGameType == NORMAL_GAME || g_RI.cRuleType == RULETYPE_5POKER)
		{
			//일반 모드일때는 가장 첫번째 카드를 선택한다.
			m_cChoiceCardDeck.nOPenCardIndex = 0;
			m_cChoiceCardDeck.nOPenCardNum = m_cChoiceCardDeck.nCard[0];
		}
		else
		{
			//버릴카드를 선택했다면
			if( m_cChoiceCardDeck.nDiscardindex != -1 && m_cChoiceCardDeck.nDiscardnum != -1 )
			{
				for(int j=0;j<4;j++)
				{
					//벌리카드 이외의 첫번째 카드를 선택한다.
					if( m_cChoiceCardDeck.nDiscardindex != j )
					{						
						m_cChoiceCardDeck.nOPenCardNum = m_cChoiceCardDeck.nCard[j];
						m_cChoiceCardDeck.nOPenCardIndex = j;
						break;
					}
				}
			}
			//버릴카를 선택하지 않았다면 첫번째 카드를 선택하고 가장 마지막 카드를 버린다.
			else
			{
				m_cChoiceCardDeck.nDiscardindex = m_nMaxNum - 1;
				m_cChoiceCardDeck.nDiscardnum = m_cChoiceCardDeck.nCard[m_nMaxNum - 1];
				m_cChoiceCardDeck.nOPenCardIndex = 0;
				m_cChoiceCardDeck.nOPenCardNum = m_cChoiceCardDeck.nCard[0];
			}
		}
	}	
	
	OnEndSelectCard(bautoselect);

	//서버로 내가 선택한 그림 보내준다.
	POKERCLNT pc={0,}; 
	pc.nStyle = 3; // 배팅
	pc.nBtn   = m_cChoiceCardDeck.nOPenCardIndex+1; // 선택카드 (인덱스 1 , 2, 3)

	CPlayer *pPlayer = GM().GetMyInfo();
	CCL_ASK_SELECTCARD scard;
	scard.Set(g_RI.RoomNum,  pPlayer->UI.UNum, pPlayer->ServPNum, pPlayer->UI.ID, &pc, m_cChoiceCardDeck.nDiscardindex == - 1 ? 4 : (m_cChoiceCardDeck.nDiscardindex + 1));
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), scard.pData, scard.GetTotalSize());	

	m_bShow = false;
}

void CSelectCard::DisableSelectCard()
{
	m_bShow = false;
	m_bChoiceCard = false;
}