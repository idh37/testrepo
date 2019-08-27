// NMovement.cpp: implementation of the CNMovement class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "NMovement.h"
#include "GlobalGame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNMovement::CNMovement()
:m_bUseScene( false ),
m_dwStartTime( 0 ),
m_dwCurTime( 0 ),
m_dwDeltaTime( 0 ),
m_fPlayProportion( 0.f ),
m_pSpr( NULL )
{
	m_temp = 0.0f;	
	ReSet();
}

CNMovement::~CNMovement()
{
	int xxx = 0;
	m_pSpr = NULL;
}

void CNMovement::StraightMoveTo( POINT curpt, POINT destpt, float fSpeed , int delay, BOOL bChange )
{
	m_dest = destpt;		
	m_start = m_cur = curpt;
	m_delay = delay;
	m_bStop = FALSE;
	m_fPlayProportion = fSpeed; // 카드 이동속도 애니와 일반이동 같이 사용한다.

	m_pSpr = &GLOBAL_SPR[ spr::GSPR_ET_CARDDEALING ];
	TPASSERT( m_pSpr != NULL );

	m_dwPlayTime = m_pSpr->GetMScenePlayTime( m_nDestIndex );
}

void CNMovement::ReSet()
{
	m_delay = 0;		// 얼마나 기다린 후에 운동을 시작할 것인가.
	m_bStop = TRUE;
	m_cur.x = 0;
	m_cur.y = 0;
	m_dest = m_cur;	
	m_fPlayProportion = 0.0f;
	m_nmovinglen = 0;
	m_temp = 1.0f;
	m_SlowDownPos = 20.0f;

	m_dwStartTime = 0;
	m_dwCurTime = 0;
	m_nDestIndex = 0;
	m_bUseScene = false;

	m_dwDeltaTime = 0;
	m_dwPlayTime = 0;
}

void CNMovement::OnTimer( )
{
	m_dwCurTime = timeGetTime();

	if( m_dwCurTime < m_dwStartTime )
	{
		m_dwCurTime = 0;
		return;
	}
	// Delay가 있다면 그 수치가 0이 되었을 경우 이동..	
	if ( m_delay > 0 ) 
	{
		m_delay--;
		if( m_delay == 0  )
		{
			m_dwStartTime = m_dwCurTime;
			PBPlayEffectSound(SND_CARD_MOVE);
		}
	}
	else if( m_bStop == FALSE ) // 이동이 덜끝난 상태라면
	{
		if ( m_bUseScene )
		{
			// 애니
			m_dwDeltaTime = (DWORD) ( ( m_dwCurTime - m_dwStartTime ) * m_fPlayProportion );

			if ( m_dwDeltaTime > m_dwPlayTime )
			{
				m_cur = m_dest;
				Stop();
			}
		}
		else // 카드 이동
		{
			// 이동이 덜끝난 상태라면
			int w = m_dest.x - m_start.x;
			int h = m_dest.y - m_start.y;
			float TarLen = (float)sqrt((float) (abs(w*w) + abs(h*h)));

			if(m_nmovinglen >= TarLen) 
				m_nmovinglen = TarLen;	

			int halflen = (int) max(1,TarLen/2);
			int leftlen = (int)( TarLen - m_nmovinglen);		

			float halffps = (float)g_pGameView->m_ReFresh_FPS;		
			float framelen = max(1,TarLen / halffps);	

			static int cnt = 0;
			cnt ++;

			float movdot = (float)( framelen * (  halffps / m_fPlayProportion ));

			if( m_nmovinglen  >= (TarLen - ((TarLen / g_pGameView->m_ReFresh_FPS) * m_SlowDownPos )))
			{				
				//	float movdot1  = min( movdot ,movdot * (((1.0f - m_nmovinglen / (float)TarLen) * 2.1f) + 0.1f));
				float movdot1  = min( movdot ,movdot * (((1.0f - m_nmovinglen / (float)TarLen) * 1.5f) + 0.1f));
				m_nmovinglen += movdot1;		
			}
			else
			{			
				m_nmovinglen += movdot;
			}

			int dx=0, dy=0;
			if( w == 0 )
				w = 1;
			double al = (double)abs(h)/(double)abs(w);

			double rad = atan(al);
			dx = (int)(cos(rad) * (double)m_nmovinglen);
			dy = (int)(sin(rad) * (double)m_nmovinglen);

			if(w < 0 )
			{
				m_cur.x = m_start.x - dx; // 현재 그려지고 있는 x 좌표
			}
			else
			{
				m_cur.x = m_start.x + dx; // 현재 그려지고 있는 x 좌표
			}

			if(h < 0 )
			{
				m_cur.y = m_start.y - dy; // 현재 그려지고 있는 y 좌표
			}		
			else
			{
				m_cur.y = m_start.y + dy; // 현재 그려지고 있는 y 좌표
			}

			// 목표 거리 이상 이동하였다면 목표지점으로 강제 이동
			if(m_nmovinglen >= TarLen)
			{ 
				m_cur = m_dest;
				int xxx = cnt;
				cnt = 0;
			}

			if(m_cur == m_dest )
			{
				Stop();
			}	
		}
	}	
}

void CNMovement::AniDraw( NMBASE::GRAPHICGDI::CPage *pPage )
{
	if ( m_bStop ) 
	{
		return;
	}
 	
	if ( m_pSpr )
	{
		if ( m_dwDeltaTime > 0 )
		{
			pPage->DrawMultiScene( 0, 0, m_pSpr, m_nDestIndex, m_dwDeltaTime );
		}
	}
}
