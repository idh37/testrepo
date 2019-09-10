#include "stdafx.h"
#include "BackGround_BD.h"


CBackGround_BD::CBackGround_BD()
{
		
}

CBackGround_BD::~CBackGround_BD()
{
}

void CBackGround_BD::DrawBottom(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC)
{
	DWORD dwBackTime = timeGetTime() - m_dwStartTime;
	if(dwBackTime >= m_dwBackPlayTotalTime) dwBackTime = m_dwBackPlayTotalTime - 1;

	pPage->DrawMultiScene(0, 0, &m_sprBack, m_nPlayIndex, dwBackTime, FALSE);
}

// Ŀ�ý� ī�� �̹��� ������ ������ ������ ��´�6
void CBackGround_BD::DrawMiddle(NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC)
{
	CPoint ptTitle = g_cUIData.GetPosBD( ui::EPOS_DEALING_TITEL );
	int nSceneIndex = m_bVipRoom ? 7 : 5;
	pPage->DrawScene( ptTitle.x, ptTitle.y, &m_sprBack, nSceneIndex, timeGetTime() );

	// vip�� ����Ʈ
	if ( m_bVipRoom )
	{
		DWORD dwBackTime = timeGetTime() - m_dwStartTime;
		if(dwBackTime >= m_dwBackPlayTotalTime) dwBackTime = m_dwBackPlayTotalTime - 1;
		pPage->DrawMultiScene(0, 0, &m_sprBack, 2, dwBackTime, FALSE);
	}
	// ����ε� ǥ��
	if(g_RI.FormKind == 5)
	{
		pPage->PutSprAuto(461, 190, &GLOBAL_SPR[ spr::GSPR_ET_BLIND ], 2 );
	}
	// ��ħ���� ǥ��
	if(g_RI.bMorningBet && g_RI.State == 1 && g_RI.nChangeType == 0)
	{
		pPage->PutSprAuto(355, 114, &m_sprBack, 16);
	}
}
