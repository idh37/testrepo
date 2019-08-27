#include "StdAfx.h"
#include "GradeEffect.h"

namespace effect
{

CGradeEffect::CGradeEffect()
{
	m_pMultiScene = NULL;	
	m_pPage = NULL;
}

CGradeEffect::~CGradeEffect()
{
	Destroy();
}

BOOL CGradeEffect::Init( int index , NMBASE::GRAPHICGDI::CPage *pPage)
{
	m_nObjID = ANF_SDLG_GRADE;	//Á¾·ù	
	DrawFaceKind = DRAW_TOP;
	Destroy();

	if( pPage == NULL ){
		nResult = PROCESS_KILLME;
		return FALSE;
	}
	m_pPage = pPage ;
	POINT pt;
	if( index == 0 ){
		pt.x = 361;
		pt.y = 200;
	}
	else if( index == 1 ){
		pt.x = 5;
		pt.y = 10;	
	}
	else if( index == 2 ){
		pt.x = 5;
		pt.y = 69;
	}

	m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(pt.x,pt.y,g_pGameView,m_pPage, &GLOBAL_SPR[ spr::GSPR_ET_GRADE ],index);
	m_pMultiScene->StartMScene(1);

	return TRUE;
}

void CGradeEffect::DrawObject(CDC *pDC)
{
	if(pDC == NULL){nResult = PROCESS_KILLME;}

	if( m_pMultiScene != NULL){
		m_pMultiScene->DrawMScene(pDC);
	}
}

void CGradeEffect::Destroy()
{	
	if( m_pMultiScene != NULL ){
		delete m_pMultiScene;
	}
	m_pMultiScene = NULL;
}

int  CGradeEffect::ProcessObject()
{	
	if( m_pMultiScene != NULL )
	{
		if( m_pMultiScene->m_stSceneState.bScenePlay == TRUE ){
			m_pMultiScene->OnTimer();		
		}
		else{
			Destroy();
			return PROCESS_KILLME;
		}
	}
	return PROCESS_OK;
}

}


