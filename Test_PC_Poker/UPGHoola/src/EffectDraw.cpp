#include "StdAfx.h"
#include "EffectDraw.h"
#include "SoundRes.h"


CEffectDraw::CEffectDraw(void)
{
	m_pSpr = NULL;
	m_pPage = NULL;
	m_pt = CPoint(0,0);
	m_eEffectType = eNUM_EFFECT_NULL;
	m_nPnum = -1;	
	m_nSceneNum = -1;

	m_bPlay = FALSE;
	m_dwStartTime = 0;
	m_dwEndTime = 0;
	m_dwTime = 0;	
	m_bPlaySceneSound = FALSE;
}

CEffectDraw::~CEffectDraw(void)
{
}
void CEffectDraw::PlayAnimation()
{
	m_bPlay = TRUE;
	m_dwStartTime = timeGetTime();
	m_dwEndTime = m_pSpr->GetMScenePlayTime(m_nSceneNum);

	if ( TRUE == m_bPlaySceneSound )
	{
		AutoPlayMSceneSound(m_pSpr, m_nSceneNum);
	}
}
void CEffectDraw::StopAnimation()
{
	m_bPlay = FALSE;
	m_dwStartTime = 0;
	m_dwEndTime = m_pSpr->GetMScenePlayTime(m_nSceneNum);
}

void CEffectDraw::SetAnimation( eNUM_EFFECT eType, int nPnum, NMBASE::GRAPHICGDI::CPage *pPage, CPoint pt )
{
	if ( NULL == pPage) return;

	m_nPnum = nPnum;
	m_pPage = pPage;
	m_eEffectType = eType;
	m_pt = pt;

	switch(eType)
	{
	case eNUM_EFFECT_THANK:
		{
			m_pSpr = &g_sprEffect;
			m_nSceneNum = 0;
			m_bPlaySceneSound = TRUE;
		}break;
	case eNUM_EFFECT_THANK_BAK:
		{
			m_pSpr = &g_sprEffect;
			m_nSceneNum = 5;
			m_bPlaySceneSound = TRUE;
		}break;
	case eNUM_EFFECT_STOP:
		{
			m_pSpr = &g_sprEffect;
			if (nPnum < 3)	//¿ÞÂÊ ºä
			{
				m_nSceneNum = 2;
			}
			else			//¿À¸¥ÂÊ ºä
			{
				m_nSceneNum = 1;
			}
			m_bPlaySceneSound = TRUE;
		}break;
	case eNUM_EFFECT_STOP_BAK:
		{
			m_pSpr = &g_sprEffect;
			if (nPnum < 3)	//¿ÞÂÊ ºä
			{
				m_nSceneNum = 4;		
			}
			else			//¿À¸¥ÂÊ ºä
			{
				m_nSceneNum = 3;
			}
			m_bPlaySceneSound = TRUE;
		}break;
	case eNUM_EFFECT_JOKER_BAK:
		{
			m_pSpr = &g_sprEffectBak;
			m_nSceneNum = 0;
			m_bPlaySceneSound = TRUE;
		}break;
	case eNUM_EFFECT_SEVEN_BAK:
		{
			m_pSpr = &g_sprEffectBak;
			m_nSceneNum = 1;
			m_bPlaySceneSound = TRUE;
		}break;
	case eNUM_EFFECT_REGIST_BAK:
		{
			m_pSpr = &g_sprEffectBak;
			m_nSceneNum = 2;
			m_bPlaySceneSound = TRUE;
		}break;
	case eNUM_EFFECT_JOKER_SEVEN_BAK:
		{
			m_pSpr = &g_sprEffectBak;
			m_nSceneNum = 3;
			m_bPlaySceneSound = TRUE;
		}break;
	case eNUM_EFFECT_SEVEN_REIGIST_BAK:
		{
			m_pSpr = &g_sprEffectBak;
			m_nSceneNum = 4;
			m_bPlaySceneSound = TRUE;
		}break;
	case eNUM_EFFECT_REIGIST_JOKER_BAK:
		{
			m_pSpr = &g_sprEffectBak;
			m_nSceneNum = 5;
			m_bPlaySceneSound = TRUE;
		}break;
	case eNUM_EFFECT_JOKER_SEVEN_REIGIST_BAK:
		{
			m_pSpr = &g_sprEffectBak;
			m_nSceneNum = 6;
			m_bPlaySceneSound = TRUE;
		}break;
	case eNUM_EFFECT_ALLIN:
		{
			m_pSpr = &g_sprAllin;
			m_nSceneNum = 0;
			m_bPlaySceneSound = TRUE;
		}break;
	default:
		return;
	}
}

void CEffectDraw::Draw(CDC *pDC)
{
	if ( FALSE == m_bPlay )
	{
		return;
	}
	m_dwTime = timeGetTime() - m_dwStartTime;
	m_pPage->DrawMultiScene(m_pt.x, m_pt.y, m_pSpr, m_nSceneNum, m_dwTime, FALSE );	

	if ( TRUE == IsAnimationEnd() )
	{
		m_bPlay = FALSE;
	}
}

BOOL CEffectDraw::IsAnimationEnd()
{
	if ( m_dwTime >= m_dwEndTime)
	{
		return TRUE;
	}
	return FALSE;
}
