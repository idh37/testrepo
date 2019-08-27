// MovementEx.cpp: implementation of the CMovementEx class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MovementEx.h"
#include <math.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMovementEx::CMovementEx()
{
	ReSet();
	m_strSoundID = "";
	m_fPlayProportion = 1.0f;
}

CMovementEx::~CMovementEx()
{

}

void CMovementEx::ReSet()
{
	m_dwEndTime = 0;
	m_dwStartTime = 0;
	m_dwMoveTime = 0;
	m_dwCurTime = 0;

	m_ptStart = CPoint(0,0);
	m_ptEnd = CPoint(0,0);
	m_ptCurrent = CPoint(0,0);
	m_ptGab = CPoint(0,0);

	m_bStart = false;
	m_bEnd = false;
	m_bMoveStart = false;
	m_nIndex = -1;
	m_dwGab = 0;

	m_nType = MT_NORMAL;
	m_pSprite = NULL;
	m_bPlaySound = false;
}

void CMovementEx::StartMove(NMBASE::GRAPHICGDI::xSprite *pSprite, CPoint ptStart, CPoint ptEnd, int nIndex, DWORD dwMoveTime, DWORD dwDelay, int opt, float fPlayProportion)
{
	ReSet();

	m_ptGab = ptEnd - ptStart;

	m_ptStart = ptStart;
	m_ptEnd = ptEnd;
	m_ptCurrent = ptStart;

	m_dwCurTime  = timeGetTime();
	m_dwStartTime = m_dwCurTime + dwDelay;
	m_dwEndTime = m_dwStartTime + dwMoveTime;
	m_dwMoveTime = dwMoveTime;
	m_dwGab = 0;

	m_fPlayProportion = fPlayProportion;
	
	m_bEnd = false;
	m_bStart = true;
	m_bMoveStart = false;
	m_nIndex = nIndex;

	m_pSprite = pSprite;
	m_nOpt = opt;
	m_nType = MT_NORMAL;
}

void CMovementEx::StartMoveMSprite(NMBASE::GRAPHICGDI::xSprite *pSprite, CPoint ptStart, CPoint ptEnd, int nIndex, DWORD dwDelay, int opt, float fPlayProportion)
{
	ReSet();

	m_dwCurTime  = timeGetTime();
	m_dwStartTime = m_dwCurTime + dwDelay;
	m_dwMoveTime = pSprite->GetMScenePlayTime(nIndex);
	m_dwEndTime = m_dwStartTime + m_dwMoveTime;
	m_dwGab = 0;
	m_nOpt = opt;

	m_ptStart = ptStart;
	m_ptEnd = ptEnd;
	m_ptCurrent = ptStart;

	m_fPlayProportion = fPlayProportion;

	m_bEnd = false;
	m_bStart = true;
	m_bMoveStart = false;
	m_nIndex = nIndex;

	m_pSprite = pSprite;
	m_nType = MT_MSPRITE;
}

void CMovementEx::StartMoveSSprite(NMBASE::GRAPHICGDI::xSprite *pSprite, CPoint ptStart, CPoint ptEnd, int nIndex, DWORD dwDelay, int opt, float fPlayProportion)
{
	ReSet();

	m_dwCurTime  = timeGetTime();
	m_dwStartTime = m_dwCurTime + dwDelay;
	m_dwMoveTime = pSprite->GetScenePlayTime(nIndex);
	m_dwEndTime = m_dwStartTime + m_dwMoveTime;
	m_dwGab = 0;
	m_nOpt = opt;

	m_ptStart = ptStart;
	m_ptEnd = ptEnd;
	m_ptCurrent = ptStart;

	m_fPlayProportion = fPlayProportion;

	m_bEnd = false;
	m_bStart = true;
	m_bMoveStart = false;
	m_nIndex = nIndex;

	m_pSprite = pSprite;
	m_nType = MT_SSPRITE;
}

void CMovementEx::StartMoveImage(NMBASE::GRAPHICGDI::xSprite *pSprite, CPoint ptStart, CPoint ptEnd, DWORD dwGab, DWORD dwDelay, int opt, float fPlayProportion)
{
	ReSet();

	m_dwCurTime  = timeGetTime();
	m_dwStartTime = m_dwCurTime + dwDelay;
	m_dwMoveTime = pSprite->GetTotalSpr() * dwGab;
	m_dwEndTime = m_dwStartTime + m_dwMoveTime;
	m_dwGab = dwGab;
	m_nOpt = opt;

	m_ptStart = ptStart;
	m_ptEnd = ptEnd;
	m_ptCurrent = ptStart;

	m_bEnd = false;
	m_bStart = true;
	m_bMoveStart = false;
	m_nIndex = 0;

	m_pSprite = pSprite;
	m_nType = MT_IMAGE;
}

void CMovementEx::OnTimer()
{
	if(!m_bStart || m_bEnd) return;	

	m_dwCurTime = timeGetTime();
	if( m_dwCurTime < m_dwStartTime )
	{
		m_dwCurTime = 0;
		return;
	}

	if(!m_bPlaySound)
	{
		if(m_strSoundID != "") PBPlayEffectSound(m_strSoundID.c_str());
		m_bPlaySound = true;
	}

	if(m_fPlayProportion != 1.0f)
	{
		m_dwCurTime = m_dwStartTime + (DWORD)(float(m_dwCurTime - m_dwStartTime) * m_fPlayProportion);
	}

	if( m_dwCurTime >= m_dwEndTime)
	{
		m_dwCurTime = m_dwEndTime - m_dwStartTime;
		m_bEnd = true;
		m_bMoveStart = false;
		m_bStart = false;
		m_ptCurrent = m_ptEnd;
		return;
	}

	m_dwCurTime = m_dwCurTime - m_dwStartTime;

	if(!m_bMoveStart)
	{
		if(m_strSoundID != "") 
			NMBASE::SOUND::g_Sound.PlayWav( m_strSoundID.c_str() );

		m_bMoveStart = true;
	}

	switch(m_nType)
	{
	case MT_NORMAL:
		{
			float fRate = float(m_dwCurTime) / float(m_dwMoveTime);
			fRate = fRate * fRate;
			m_ptCurrent.x = m_ptStart.x + (int)((float)m_ptGab.x * fRate);
			m_ptCurrent.y = m_ptStart.y + (int)((float)m_ptGab.y * fRate);
		}
		break;
	case MT_IMAGE:
		{
			m_nIndex = m_dwCurTime / m_dwGab;
		}
		break;
	case MT_SSPRITE:
	case MT_MSPRITE:
		break;
	}
}

void CMovementEx::DrawSprite(NMBASE::GRAPHICGDI::CPage *pPage)
{
// 	if(!IsMoveStart() || IsMoveEnd()) 
// 		return;
	switch(m_nType)
	{
	case MT_NORMAL:
		pPage->PutSprAuto(m_ptCurrent.x, m_ptCurrent.y, m_pSprite, m_nIndex, m_nOpt);
		break;
	case MT_IMAGE:
		pPage->PutSprAuto(m_ptCurrent.x, m_ptCurrent.y, m_pSprite, m_nIndex, m_nOpt);
		break;
	case MT_MSPRITE:
		pPage->DrawMultiScene(m_ptCurrent.x, m_ptCurrent.y, m_pSprite, m_nIndex, m_dwCurTime, FALSE, m_nOpt);
		break;
	case MT_SSPRITE:
		pPage->DrawScene(m_ptCurrent.x, m_ptCurrent.y, m_pSprite, m_nIndex, m_dwCurTime, FALSE, m_nOpt);
		break;
	}
}