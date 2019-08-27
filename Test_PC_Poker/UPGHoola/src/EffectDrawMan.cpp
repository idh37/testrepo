#include "StdAfx.h"
#include "EffectDrawMan.h"
#include "SpriteList.h"
#include "GlobalGame.h"

CEffectDrawMan::CEffectDrawMan(void)
{
	Reset();
	ptThank[0] = CPoint( 392, 346 );
	ptThank[1] = CPoint( 10, 307 );
	ptThank[2] = CPoint( 10, 115 );
	ptThank[3] = CPoint( 832, 115 );
	ptThank[4] = CPoint( 832, 307 );

	ptThankBak[0] = CPoint( 356, 433 );
	ptThankBak[1] = CPoint( -29, 264 );
	ptThankBak[2] = CPoint( -29, 72 );
	ptThankBak[3] = CPoint( 813, 72 );
	ptThankBak[4] = CPoint( 813, 264 );

	ptStop_StopBak[0] = CPoint( 305, 429 );
	ptStop_StopBak[1] = CPoint( 0, 264 );
	ptStop_StopBak[2] = CPoint( 0, 74 );
	ptStop_StopBak[3] = CPoint( 829, 74 );
	ptStop_StopBak[4] = CPoint( 829, 264 );

	ptResult[0] = CPoint( 375, 474 );
	ptResult[1] = CPoint( -3, 287 );
	ptResult[2] = CPoint( -3, 95 );
	ptResult[3] = CPoint( 835, 95 );
	ptResult[4] = CPoint( 835, 287 );

	ptAllIn[0] = CPoint( 222, 512 );
	ptAllIn[1] = CPoint( 22, 320 );
	ptAllIn[2] = CPoint( 22, 130 );
	ptAllIn[3] = CPoint( 863, 130 );
	ptAllIn[4] = CPoint( 863, 321 );
}

void CEffectDrawMan::Reset()
{
	for ( int i = 0 ; i < MAX_PLAYER ; i++)
	{
		m_mapEffectList[i].clear();
	}
}

CEffectDrawMan::~CEffectDrawMan(void)
{

}

void CEffectDrawMan::Init(NMBASE::GRAPHICGDI::CPage *pPage)
{
	m_pPage = pPage;

	for( int i = 0 ; i < MAX_PLAYER ; i++ )
	{
		for (int j = 0 ; j < eNUM_EFFECT_MAX ; j++)
		{
			SetEffect( i, (eNUM_EFFECT) j );
		}
	}
}
void CEffectDrawMan::PlayAnimation( eNUM_EFFECT eType, int nPnum )
{
	m_mapEffectList[nPnum][eType]->PlayAnimation();
}
void CEffectDrawMan::PlayAnimation( eNUM_EFFECT eType, char* pID )
{
	for ( int i = 0 ; i < MAX_PLAYER ; i++ )
	{
		if (strcmp( g_PLAYER(i)->UI.ID, pID) == 0)
		{
			PlayAnimation( eType, (int)i);
		}
	}
}
void CEffectDrawMan::StopAnimation( eNUM_EFFECT eType, int nPnum )
{
	m_mapEffectList[nPnum][eType]->StopAnimation();
}
void CEffectDrawMan::StopAllAnimation()
{
	for ( int i = 0 ; i < MAX_PLAYER ; i++ )
	{
		for ( int j = 0 ; j < eNUM_EFFECT_MAX ; j++ )
		{
			StopAnimation( (eNUM_EFFECT) j, i );
		}
	}
}

void CEffectDrawMan::SetEffect( int nPnum, eNUM_EFFECT eType )
{
	if (nPnum < 0) return;

	CPoint pt;
	switch(eType)
	{
	case eNUM_EFFECT_THANK:						pt = ptThank[nPnum];			break;
	case eNUM_EFFECT_THANK_BAK:					pt = ptThankBak[nPnum];			break;
	case eNUM_EFFECT_STOP:						pt = ptStop_StopBak[nPnum];		break;
	case eNUM_EFFECT_STOP_BAK:					pt = ptStop_StopBak[nPnum];		break;
	case eNUM_EFFECT_JOKER_BAK:					pt = ptResult[nPnum];			break;
	case eNUM_EFFECT_SEVEN_BAK:					pt = ptResult[nPnum];			break;
	case eNUM_EFFECT_REGIST_BAK:				pt = ptResult[nPnum];			break;
	case eNUM_EFFECT_JOKER_SEVEN_BAK:			pt = ptResult[nPnum];			break;
	case eNUM_EFFECT_SEVEN_REIGIST_BAK:			pt = ptResult[nPnum];			break;
	case eNUM_EFFECT_REIGIST_JOKER_BAK:			pt = ptResult[nPnum];			break;
	case eNUM_EFFECT_JOKER_SEVEN_REIGIST_BAK:	pt = ptResult[nPnum];			break;
	case eNUM_EFFECT_ALLIN:						pt = ptAllIn[nPnum];			break;
	default:
		return;
	}
	CEffectDraw* pEffectDraw;
	SAFE_NEW( pEffectDraw, CEffectDraw);
	pEffectDraw->SetAnimation(eType, nPnum, m_pPage, pt);
	m_mapEffectList[nPnum].insert(pair<int, CEffectDraw*> ((int)eType, pEffectDraw));
}

void CEffectDrawMan::Draw(CDC *pDC)
{
	for( int i = 0 ; i < MAX_PLAYER ; i++ )
	{
		if( m_mapEffectList[i].empty() )
			continue;

		for ( int j = 0 ; j < m_mapEffectList[i].size() ; j++ )
		{
			if ( NULL != m_mapEffectList[i][j] )
			{
				CEffectDraw* pED = m_mapEffectList[i][j];
				pED->Draw(pDC);
			}
		}		
	}
}