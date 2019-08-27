#include "StdAfx.h"
#include "CuttingStageIndicateEffect.h"

namespace effect
{

//아침 점심 저녁 효과 드로우
CCuttingStageIndicateEffect::CCuttingStageIndicateEffect()
{
}

CCuttingStageIndicateEffect::~CCuttingStageIndicateEffect()
{		
}

void CCuttingStageIndicateEffect::Init()
{			
	m_nObjID = ANF_BG_STAGESTATE;
	DrawFaceKind = DRAW_BOTTOM;

	CPoint ptPos = g_cUIData.GetPosBD( ui::EPOS_CUTINFO );
	DXp = ptPos.x;		//그리는 시작점
	DYp = ptPos.y;
}

void CCuttingStageIndicateEffect::Destroy()
{	
}

int CCuttingStageIndicateEffect::ProcessObject()
{		
	if( m_bIsDestroy == TRUE ){
		return PROCESS_KILLME;
	}
	return PROCESS_OK;
}

void CCuttingStageIndicateEffect::DrawObject(CDC *pDC)
{
	NMBASE::GRAPHICGDI::CPage *pPage = &g_pGameView->Page;
	if( pPage == NULL ){
		m_bIsDestroy = TRUE;
		return;
	}
	if ( g_RI.nChangeType > 0 )
	{
		int sindex  = g_RI.nChangeType - 1;

		// 아침, 점심, 저녁
		if ( sindex >= 0 )
		{
			int nXGab = g_cUIData.GetGabBD( ui::EGAB_CUTINFO_X );
			pPage->PutSprAuto( DXp + ( ( g_RI.nChangeType - 1 ) * nXGab ) , DYp , &GLOBAL_SPR[ spr::GSPR_GAME_CUT_INFO ], sindex );
		}
	}	
}

}


