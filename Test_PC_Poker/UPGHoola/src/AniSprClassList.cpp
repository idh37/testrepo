// AniSprClassList.cpp: implementation of the AniSprClassList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "AniSprClassList.h"
#include "mmsystem.h"
#include "GlobalGame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
//���� Ŀ��Ʈ ǥ��

//=========================================================================//

//////////////////////////////////////////////////////////////////////////

//ANF_SDLG_GRADE
CGradeSpr::CGradeSpr()
{
	m_pMultiScene = NULL;	
	m_pPage = NULL;
}

CGradeSpr::~CGradeSpr()
{
	Destroy();
}

BOOL CGradeSpr::Init( int index , NMBASE::GRAPHICGDI::CPage *pPage)
{
	m_nObjID = ANF_SDLG_GRADE;	//����	
	DrawFaceKind = DRAW_TOP;
	Destroy();

	if( pPage == NULL )
	{
		nResult = PROCESS_KILLME;
		return FALSE;
	}
	m_pPage = pPage ;
	POINT pt;
	if( index == 0 )
	{
		pt.x = 361;
		pt.y = 200;
	}
	else if( index == 1 )
	{
		pt.x = 5;
		pt.y = 10;	
	}
	else if( index == 2 )
	{
		pt.x = 5;
		pt.y = 69;
	}

	m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(pt.x,pt.y,g_pGameView,m_pPage,&GradeSpr,index);
	m_pMultiScene->StartMScene(1);

	return TRUE;
}

void CGradeSpr::DrawObject(CDC *pDC)
{
	if(pDC == NULL)
	{
		nResult = PROCESS_KILLME;
	}

	if( m_pMultiScene != NULL)
	{
		m_pMultiScene->DrawMScene(pDC);
	}
}

void CGradeSpr::Destroy()
{	
	if( m_pMultiScene != NULL )
	{
		delete m_pMultiScene;
	}
	m_pMultiScene = NULL;
}

int  CGradeSpr::ProcessObject()
{	
	if( m_pMultiScene != NULL )
	{
		if( m_pMultiScene->m_stSceneState.bScenePlay == TRUE ){
			m_pMultiScene->OnTimer();		
		}
		else
		{
			Destroy();
			return PROCESS_KILLME;
		}
	}
	return PROCESS_OK;
}





//ANF_PLAYER_CHANCEITEM_MSG ���� ������ ȿ��
//ä��â ���� �޼��� ���� ������ ���� �����ش�.

CChance_Msg_EffectSpr::CChance_Msg_EffectSpr()
{		
	m_pMultiScene = NULL;	
	ZeroMemory(m_ChanceItemText,sizeof(m_ChanceItemText));
}

CChance_Msg_EffectSpr::~CChance_Msg_EffectSpr()
{	
	Destroy();
}

void CChance_Msg_EffectSpr::Init( int itemcode )
{
	m_nObjID = ANF_PLAYER_CHANCEITEM;	
	nDelTime = 5;
	DrawFaceKind = DRAW_TOP;

	if( m_pMultiScene != NULL)
	{
		delete m_pMultiScene;
		m_pMultiScene = NULL;
	}	

	/*
	ITEM_SCODE_REWARD_MADE=58,			// ���̵�
	ITEM_SCODE_REWARD_RSF=54,			//��Ƽ��	//���� ������
	ITEM_SCODE_REWARD_SF=53,			//��Ƽ��
	ITEM_SCODE_REWARD_FCARD=52,			//��ī��
	ITEM_SCODE_REWARD_FH=51,			//Ǯ�Ͽ콺
	ITEM_SCODE_REWARD_LUCKAVATA=109,	//[���ƹ�Ÿ] 2006.07.24
	ITEM_SCODE_REWARD_LOWGRADE=123,		//�ºλ� �Թ� [�Թ�������]
	ITEM_SCODE_REWARD_HIGHGRADE=124,	//�ºλ� ���
	ITEM_SCODE_REWARD_ALLINKING=125,	//���ο�����	
	ITEM_SCODE_REWARD_SPECIALLUCKITEM = 132, //[�����]2007.10.17
	ITEM_SCODE_REWARD_ALLINCHANCE=134,	//[���������ƹ�Ÿ] 2008.07
	*/

	switch(itemcode) 
	{
	case ITEM_SCODE_REWARD_HIGHGRADE:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,0);
		break;
		// 	case ITEM_SCODE_REWARD_MADE:
		// 		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,1);
		// 		break;
	case ITEM_SCODE_REWARD_RSF:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,2);
		break;
	case ITEM_SCODE_REWARD_SF:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,3);
		break;
	case ITEM_SCODE_REWARD_FCARD:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,4);
		break;
	case ITEM_SCODE_REWARD_FH:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,5);
		break;
	case ITEM_SCODE_REWARD_VIP :	//VIP ���� 11.08.28
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,7);
		break;	
	case ITEM_SCODE_REWARD_VVIP :	//VVIP ���� 11.08.28
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,8);
		break;
	case ITEM_SCORE_REWARDQUADRUPLE :
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,9);
		break;
	case ITEM_SCODE_REWARDTHANKHOOLA :
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,10);
		break;
	case ITEM_SCODE_REWARDJACKPOT :
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&ChanceEffectSpr,11);
		break;
	}

	//get_ChanceText(itemcode);
}

void CChance_Msg_EffectSpr::get_ChanceText(int itemCode)
{

// 	ZeroMemory(m_ChanceItemText,sizeof(m_ChanceItemText));
// 	CString str;
// 
// 	//UGP�۾� �Ʒ������� ��ü
// 	// 	str.Format( "[%s]�� %s \n�����ڰ� ȹ��.\n", 
// 	// 		GM().GetMyInfo()->UI.NickName, g_pMainDlg->m_RewardItemMan.GetRewardItemName( itemCode ));
// 
// 	str.Format( "��[%s]�� %s \n�����ڰ� ȹ��.\n", GM().GetMyInfo()->UI.NickName, RewardItemMan().GetRewardItemName( itemCode ).GetBuffer());
// 
// 	strncpy(m_ChanceItemText,(char*)(LPCTSTR)str,str.GetLength());	
}

void CChance_Msg_EffectSpr::Destroy()
{	
	if( m_pMultiScene != NULL )
	{
		delete m_pMultiScene;		
	}
	m_pMultiScene= NULL;
}

int CChance_Msg_EffectSpr::ProcessObject()
{	
	if(nDelTime>0)
	{
		nDelTime--;
		if( nDelTime == 0 )
		{
			nDelTime = 0;
			if( m_pMultiScene != NULL )
			{
				m_pMultiScene->StartMScene(1);
// 				if( strlen(m_ChanceItemText) > 0 )
// 				{
// 					AddGameInfoViewEdit(m_ChanceItemText, RGB(255, 249, 73));
// 				}				
			}
		}
		return PROCESS_OK;
	}

	if( m_pMultiScene != NULL )
	{
		if( m_pMultiScene->m_stSceneState.bScenePlay == TRUE )
		{
			m_pMultiScene->OnTimer();
		}
		else
		{
			Destroy();
			return PROCESS_KILLME;
		}		
	}
	return PROCESS_OK;
}

void CChance_Msg_EffectSpr::DrawObject(CDC *pDC)
{
	if( m_pMultiScene != NULL )
	{

		//		if( g_pChatWinMan->IsShow() && g_pChatWinMan->GetViewMode() == eCHATVIEW::eCHAT){
		//			m_pMultiScene->DrawScene(NULL);
		//		}

		m_pMultiScene->DrawMScene(NULL);		
	}
}



