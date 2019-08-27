// SunPickUp.cpp: implementation of the CSunPickUp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SunPickUp.h"
#include "GlobalGame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSunPickUp::CSunPickUp()
{
	InitSelectCardPos();
	Reset();

}

CSunPickUp::~CSunPickUp()
{

}

void CSunPickUp::Reset()
{
	m_bPickUp = false;
	m_bCardSelected = false;

	m_nFirstPlayer = -1;

	for( int i = 0; i < MAX_FIRSTSELECTCARD_NUM; i++ )
	{
		m_SunPickup_Info[ i ].m_CardNo = 54;
		m_SunPickup_Info[ i ].m_SelectPnum = -1;
	}
}

void CSunPickUp::StartInfo( int cardnum )
{
	m_bPickUp = true;
	
	//나가기 예약 문제로 내상태를 일단 조인상태로 변경한다
	g_RI.State = EGAME_PLAYING;	
	
	GM().GetMyInfo()->JoinState = EGAME_JOININ; 
	GM().GetMyInfo()->PlayState = EPLAYER_GAMEPLAYING;
}

void CSunPickUp::PickupUser( int cardindex, int cardno, int pnum )
{
	if( cardno < 40 || cardno > 48 ) return;
	if( cardindex < 0 || cardindex > 8 ) return;
	if( pnum < 0 || pnum >= MAX_PLAYER ) return;

	m_SunPickup_Info[ cardindex ].m_CardNo = cardno;
	m_SunPickup_Info[ cardindex ].m_SelectPnum = g_GAMEPROCESS()->GetPNum_ByServPN( pnum );
}

void CSunPickUp::PickUpComplete( int winpunm )
{
	if( winpunm < 0 || winpunm >= MAX_PLAYER ) return;

	m_nFirstPlayer = g_GAMEPROCESS()->GetPNum_ByServPN( winpunm );
}

void CSunPickUp::Draw( NMBASE::GRAPHICGDI::CPage *pPage )
{
	if( m_bPickUp == false ) return;

	pPage->PutSprAuto( 392, 232, &g_sprSunPickup, 0 );

	//카드 및 선택한 유저 방향
	for( int i = 0; i < MAX_FIRSTSELECTCARD_NUM; i++ )
	{
		//유저가 선택한 카드표시 테두리	
		if( m_SunPickup_Info[ i ].m_SelectPnum == 0 )
		{
			pPage->PutSprAuto( m_SunPickup_Info[ i ].m_ptSelectCard.x - 4, m_SunPickup_Info[ i ].m_ptSelectCard.y - 4, &g_sprSunPickup, 4 );
		}
		else if( m_SunPickup_Info[ i ].m_SelectPnum == 1 || m_SunPickup_Info[ i ].m_SelectPnum == 2 )
		{
			pPage->PutSprAuto( m_SunPickup_Info[ i ].m_ptSelectCard.x - 26, m_SunPickup_Info[ i ].m_ptSelectCard.y - 4, &g_sprSunPickup, 3 );
		}
		else if( m_SunPickup_Info[ i ].m_SelectPnum == 3 || m_SunPickup_Info[ i ].m_SelectPnum == 4 )
		{
			pPage->PutSprAuto( m_SunPickup_Info[ i ].m_ptSelectCard.x - 4, m_SunPickup_Info[ i ].m_ptSelectCard.y - 4, &g_sprSunPickup, 2 );
		}

		pPage->PutSprAuto( m_SunPickup_Info[ i ].m_ptSelectCard.x, m_SunPickup_Info[ i ].m_ptSelectCard.y, &g_sprBigCard, m_SunPickup_Info[ i ].m_CardNo );
	}
	
// //	m_nFirstPlayer = 1;
// 	m_cGameViewBtnMananager.Init(g_pGameView, &g_cUIData, g_pChatWinMan);
	CUIInfoDataHoola *pHoolaUIData = dynamic_cast<CUIInfoDataHoola*>(&g_cUIData);
	
	//선이 결정됐을때
	if( m_nFirstPlayer >= 0 )
	{
		if( m_nFirstPlayer == 0 )
			pPage->PutSprAuto( 250, 449, &g_sprSunPickup, 5 );
		else if( m_nFirstPlayer >= 1 && m_nFirstPlayer <= 2 )
			pPage->PutSprAuto( pHoolaUIData->m_listAvatarPos[m_nFirstPlayer].x + 62, pHoolaUIData->m_listAvatarPos[m_nFirstPlayer].y - 19, &g_sprSunPickup, 5 );
		else if( m_nFirstPlayer >= 3 && m_nFirstPlayer <= 4 )
			pPage->PutSprAuto( pHoolaUIData->m_listAvatarPos[m_nFirstPlayer].x - 107, pHoolaUIData->m_listAvatarPos[m_nFirstPlayer].y - 19, &g_sprSunPickup, 5 );
	}
}

BOOL CSunPickUp::OnLButtonUp( int xp, int yp )
{
	if( m_bPickUp == false ) return FALSE;
	if ( m_bCardSelected == true ) return FALSE;

	POINT pt;
	pt.x = xp;
	pt.y = yp;

	for( int i = 0; i < MAX_FIRSTSELECTCARD_NUM; i++ )
	{
		if( m_SunPickup_Info[ i ].m_SelectPnum != -1 ) continue;

		if( m_rtSelectCard[ i ].PtInRect( pt ) )
		{
			CCL_SUN_PICKUP	msg;
			msg.Set( i, g_RI.RoomNum, g_PLAYER(0)->ServPNum, 0 );
//			SockMan.SendData(g_MainSrvSID, msg.pData, msg.GetTotalSize());
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			m_bCardSelected = true;
			
			return TRUE;
		}
	}

	return FALSE;
}

//선고르기
void CSunPickUp::InitSelectCardPos()
{
	m_rtSelectCard[ 0 ].SetRect( 152, 186, 152 + 74, 186 + 99 );
	m_rtSelectCard[ 1 ].SetRect( 256, 186, 256 + 74, 186 + 99 );
	m_rtSelectCard[ 2 ].SetRect( 360, 186, 360 + 74, 186 + 99 );
	m_rtSelectCard[ 3 ].SetRect( 623, 186, 623 + 74, 186 + 99 );
	m_rtSelectCard[ 4 ].SetRect( 727, 186, 727 + 74, 186 + 99 );
	m_rtSelectCard[ 5 ].SetRect( 204, 320, 204 + 74, 320 + 99 );
	m_rtSelectCard[ 6 ].SetRect( 309, 320, 309 + 74, 320 + 99 );
	m_rtSelectCard[ 7 ].SetRect( 623, 320, 623 + 74, 320 + 99 );
	m_rtSelectCard[ 8 ].SetRect( 726, 320, 726 + 74, 320 + 99 );

	m_SunPickup_Info[ 0 ].m_ptSelectCard.x = 152;
	m_SunPickup_Info[ 0 ].m_ptSelectCard.y = 186;
	
	m_SunPickup_Info[ 1 ].m_ptSelectCard.x = 256;
	m_SunPickup_Info[ 1 ].m_ptSelectCard.y = 186;
	
	m_SunPickup_Info[ 2 ].m_ptSelectCard.x = 360;
	m_SunPickup_Info[ 2 ].m_ptSelectCard.y = 186;
	
	m_SunPickup_Info[ 3 ].m_ptSelectCard.x = 623;
	m_SunPickup_Info[ 3 ].m_ptSelectCard.y = 186;
	
	m_SunPickup_Info[ 4 ].m_ptSelectCard.x = 727;
	m_SunPickup_Info[ 4 ].m_ptSelectCard.y = 186;
	
	m_SunPickup_Info[ 5 ].m_ptSelectCard.x = 204;
	m_SunPickup_Info[ 5 ].m_ptSelectCard.y = 320;
	
	m_SunPickup_Info[ 6 ].m_ptSelectCard.x = 309;
	m_SunPickup_Info[ 6 ].m_ptSelectCard.y = 320;
	
	m_SunPickup_Info[ 7 ].m_ptSelectCard.x = 623;
	m_SunPickup_Info[ 7 ].m_ptSelectCard.y = 320;
	
	m_SunPickup_Info[ 8 ].m_ptSelectCard.x = 726;
	m_SunPickup_Info[ 8 ].m_ptSelectCard.y = 320;
}	

//선고르기
POINT CSunPickUp::GetSelectCardPos( int nindex )
{
	POINT pt;
	pt.x = -1;
	pt.y = -1;
	
	if( nindex < 0 || nindex >= MAX_FIRSTSELECTCARD_NUM ) return pt;
	
	return m_SunPickup_Info[ nindex ].m_ptSelectCard;
}

