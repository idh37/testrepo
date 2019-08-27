#include "StdAfx.h"
#include "CuttingStageMsgEffect.h"

namespace effect
{

CCuttingStageMsgEffect::CCuttingStageMsgEffect()
{	
	m_pScene = NULL;
	nDelayTime = 0;	
	Destroy();
}

CCuttingStageMsgEffect::~CCuttingStageMsgEffect()
{
	Destroy();
}

void CCuttingStageMsgEffect::Init(int changetype )
{
	Destroy();
	m_nObjID = ANF_BG_CUTINGINFO;	//종류		
	DrawFaceKind = DRAW_MIDDLE;

	int sindex = changetype -1;
	if( sindex < 0 )sindex = 0;

	nDelayTime = 10;

	CPoint ptPos = g_cUIData.GetPosBD( ui::EPOS_CUTSELECT );
	// 바꿀 카드를 선택하세요
	m_pScene = new NMBASE::SKINGDI::CNScene( ptPos.x, ptPos.y, &g_pGameView->Page, &GLOBAL_SPR[ spr::GSPR_GAME_CUT_INFO ], sindex );	


	return ;
}

void CCuttingStageMsgEffect::DrawObject(CDC *pDC)
{
	if ( pDC == NULL) 
	{ 
		nResult = PROCESS_KILLME;
	}

	if ( m_pScene != NULL )
	{
		m_pScene->DrawScene(NULL);
	}		
}

void CCuttingStageMsgEffect::Destroy()
{	
	if( m_pScene != NULL ){
		delete m_pScene;
	}
	m_pScene = NULL;
}

int  CCuttingStageMsgEffect::ProcessObject()
{	
	if( nDelayTime > 0 ){
		nDelayTime --;
		if( nDelayTime == 0 ){
			nDelayTime = 0;
			m_pScene->StartScene();
		}
		return PROCESS_OK;
	}

	if( m_pScene->m_stSceneState.bScenePlay == TRUE){
		m_pScene->OnTimer();
	}
	else{
		return PROCESS_KILLME;
	}


	return PROCESS_OK;
}

}


