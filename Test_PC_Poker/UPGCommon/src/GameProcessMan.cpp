// GameProcessMan.cpp: implementation of the CGameProcessMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GameProcessMan.h"
#include "GlobalBase.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGameProcessMan::CGameProcessMan()
{
	m_pCurrentProces = NULL;
	m_CurGameMode = IDX_GAME_SP;
}

CGameProcessMan::~CGameProcessMan()
{
	if(m_pCurrentProces)
		delete m_pCurrentProces;

	m_pCurrentProces = NULL;
	//m_Seven.Clear();	
}

void CGameProcessMan::InitGameProcess(CGameProcess *pGameProces)
{
	m_pCurrentProces = pGameProces;
	m_pCurrentProces->Init(&g_pGameViewBase->Page);	
	m_CurGameMode = pGameProces->m_idxGame;	
}

CGameProcess* CGameProcessMan::GetCurGameProcess()
{	
	if( m_pCurrentProces == NULL )
	{
		g_pMainBaseDlg->SendMessage(WM_COMMAND, IDCANCEL, 0);
		return NULL;
	}
	return m_pCurrentProces;
}

CPlayer* CGameProcessMan::GetPlayer(int index )
{
	ASSERT(m_pCurrentProces);
	return m_pCurrentProces->GetPlayer(index);
}






