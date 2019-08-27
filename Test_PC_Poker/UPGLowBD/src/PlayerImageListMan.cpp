// PlayerImageListMan.cpp: implementation of the CPlayerImageListMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "PlayerImageListMan.h"
#include "GlobalGame.h"
#include "UserStruct.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////





/////////////////////////////////////////////////////////////////////////////

//#include "FontBag.h"

CJekiChargeEffect::CJekiChargeEffect()
{
	m_pSpr = NULL ;	
	m_pMultiScene = NULL;
	nStartTime = 0;	
	nSaveCnt = 0;//남은 기회 인덱스	
	nChargeMoney = 0;
	nOptionKind = 0;
}

CJekiChargeEffect::~CJekiChargeEffect()
{
	
	Destroy();
}


void CJekiChargeEffect::Destroy()
{
	PNum = -1;
	m_pSpr = NULL ;
	
	if( m_pMultiScene != NULL ){
		delete m_pMultiScene;		
	}
	m_pMultiScene= NULL;
}

void CJekiChargeEffect::Init(INT64 ChargeMoney, int SaveCnt,int pnum , NMBASE::GRAPHICGDI::xSprite *pSpr,int optionkind)
{
	m_pSpr = pSpr;
	DrawFaceKind = DRAW_TOP;
	m_nObjID = ANF_PLAYER_JEKICHARGE;
	nStartTime = timeGetTime();	
	
	nOptionKind = optionkind;
	nChargeMoney = ChargeMoney;	
	
	PNum = pnum;
	
	int Xp = 0;
	int Yp = 0;	
	nSaveCnt = SaveCnt;
	
	switch(PNum) {
	case 0:
		Xp = 299 ;
		Yp = 573 ;	
		break;
	case 1:		
		Xp = 8 ;
		Yp = 366 ;
		break;
	case 2:
		Xp = 8 ;
		Yp = 184 ;	
		break;
	case 3:
		Xp = 835 ;
		Yp = 185 ;	
		break;
	case 4:		
		Xp = 835 ;
		Yp = 366 ;		
		break;	
	}
	


	if( m_pMultiScene != NULL){
		delete m_pMultiScene;
		m_pMultiScene = NULL;
	}
	
	m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(Xp,Yp,NULL,&g_pGameView->Page,m_pSpr,nOptionKind);
	m_pMultiScene->StartMScene();

}

void CJekiChargeEffect::ChargeDrawText(CDC *pDC)
{
	if (m_pMultiScene == NULL || pDC == NULL ){
		return;
	}	
	
	int Xp = m_pMultiScene->m_Xp;
	int Yp = m_pMultiScene->m_Yp;
	CString str;
	CFont *pFont = pDC->SelectObject( &NMBASE::UTIL::GetMyFont( NMBASE::UTIL::FONT_12_BOLD ) );;
	
	COLORREF color = pDC->GetTextColor();
	pDC->SetTextColor( RGB(220, 12, 12 ) );
	str.Format( "%s 지급완료", NumberToOrientalString(nChargeMoney) );
	CRect rc( Xp, Yp + 50, Xp + 158, Yp + 50 + 20 );
	//dc.TextOut( pt[i].x + 9, pt[i].y + 51, str );
	pDC->DrawText( str, rc, DT_CENTER );
	
	pDC->SelectObject( NMBASE::UTIL::GetMyFont( NMBASE::UTIL::FONT_12_NORMAL ) );
	pDC->SetTextColor( RGB(62, 62, 62 ) );
	
	if( nSaveCnt > 0 )
		str.Format( "%s x %d회 남음", NumberToOrientalString(nChargeMoney), nSaveCnt );
	else
		str.Format( "남은횟수 0회" );
	
	rc.SetRect( Xp, Yp + 74, Xp + 158, Yp + 74 + 20 );
	pDC->DrawText( str, rc, DT_CENTER );
	
	pDC->SelectObject( pFont );				
	pDC->SetTextColor(color);
}

int CJekiChargeEffect::ProcessObject()
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

void CJekiChargeEffect::DrawObject(CDC *pDC)
{

	if( m_pMultiScene != NULL )
	{
		m_pMultiScene->DrawMScene(NULL);	

		DWORD drawt = timeGetTime() - nStartTime;
		if( drawt >= 500 ){		
			ChargeDrawText(pDC);
		}
	}
	/*
	if(nStartTime > 0)
	{
		NMBASE::GRAPHICGDI::CPage* pPage = &g_pGameView->Page;
		DWORD drawt = timeGetTime() - nStartTime;
		
		
		pPage->DrawMultiScene(Xp,Yp,m_pSpr,nOptionKind,drawt);
		
		if( drawt >= 500 ){
			CString str;
			CFont *pFont = pDC->SelectObject( &NMBASE::UTIL::GetMyFont( NMBASE::UTIL::FONT_12_BOLD ) );;
			
			COLORREF color = pDC->GetTextColor();
			pDC->SetTextColor( RGB(220, 12, 12 ) );
			str.Format( "%s원 지급완료 !!", NumberToOrientalString(nChargeMoney) );
			CRect rc( Xp, Yp + 50, Xp + 150, Yp + 50 + 20 );
			//dc.TextOut( pt[i].x + 9, pt[i].y + 51, str );
			pDC->DrawText( str, rc, DT_CENTER );
			
			pDC->SelectObject( NMBASE::UTIL::GetMyFont( NMBASE::UTIL::FONT_12_NORMAL ) );
			pDC->SetTextColor( RGB(62, 62, 62 ) );
			
			if( nSaveCnt > 0 )
				str.Format( "남은횟수 %s원 x %d회", NumberToOrientalString(nChargeMoney), nSaveCnt );
			else
				str.Format( "남은횟수 0회" );
			
			rc.SetRect( Xp, Yp + 74, Xp + 150, Yp + 74 + 20 );
			pDC->DrawText( str, rc, DT_CENTER );
			
			pDC->SelectObject( pFont );				
			pDC->SetTextColor(color);
			//dc.TextOut( pt[i].x + 9, pt[i].y + 69, str );
		}			
				
	}	
	*/
}






//ANF_PLAYER_CHANCEITEM_MSG 찬스 아이템 효과
//채팅창 찬스 메세지 지급 받을때 같이 보여준다.

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

	if( m_pMultiScene != NULL){
		delete m_pMultiScene;
		m_pMultiScene = NULL;
	}	

	/*
	ITEM_SCODE_REWARD_MADE=58,			// 메이드
	ITEM_SCODE_REWARD_RSF=54,			//로티플	//찬스 아이템
	ITEM_SCODE_REWARD_SF=53,			//스티플
	ITEM_SCODE_REWARD_FCARD=52,			//포카드
	ITEM_SCODE_REWARD_FH=51,			//풀하우스
	ITEM_SCODE_REWARD_LUCKAVATA=109,	//[행운아바타] 2006.07.24
	ITEM_SCODE_REWARD_LOWGRADE=123,		//승부사 입문 [입문아이템]
	ITEM_SCODE_REWARD_HIGHGRADE=124,	//승부사 등극
	ITEM_SCODE_REWARD_ALLINKING=125,	//올인왕찬스	
	ITEM_SCODE_REWARD_SPECIALLUCKITEM = 132, //[스행운]2007.10.17
	ITEM_SCODE_REWARD_ALLINCHANCE=134,	//[올인찬스아바타] 2008.07
	*/

	switch(itemcode) {
	case ITEM_SCODE_REWARD_HIGHGRADE:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&GLOBAL_SPR[ spr::GSPR_ET_CHANCE ],0);
		break;	
	case ITEM_SCODE_REWARD_GOLF:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&GLOBAL_SPR[ spr::GSPR_ET_CHANCE ],6);
		break;
	case ITEM_SCODE_REWARD_SECOND:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&GLOBAL_SPR[ spr::GSPR_ET_CHANCE ],7);
		break;
	case ITEM_SCODE_REWARD_THIRD:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&GLOBAL_SPR[ spr::GSPR_ET_CHANCE ],8);
		break;
	case ITEM_SCODE_REWARD_FH:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&GLOBAL_SPR[ spr::GSPR_ET_CHANCE ],5);
		break;	
// 	case ITEM_SCODE_REWARD_CHANCEAVATA:	//이벤트찬스아바타 11.08.01
// 		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&GLOBAL_SPR[ spr::GSPR_ET_CHANCE ],9);
// 		break;	

	case ITEM_SCODE_REWARD_VIP :	//VIP 찬스 11.08.28
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&GLOBAL_SPR[ spr::GSPR_ET_CHANCE ],10);
		break;	
	case ITEM_SCODE_REWARD_VVIP :	//VVIP 찬스 11.08.28
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&GLOBAL_SPR[ spr::GSPR_ET_CHANCE ],11);
		break;
	case ITEM_SCODE_REWARD_BADBEAT_SF_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_RSF_VIP:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&GLOBAL_SPR[ spr::GSPR_ET_CHANCE ],12);
		break;
	case ITEM_SCODE_REWARD_BADBEAT_FCARD_VIP:
		{
			if(IDX_GAME_SP != GM().GetCurrentGameType())
				m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&GLOBAL_SPR[ spr::GSPR_ET_CHANCE ],13);
			else										
				m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&GLOBAL_SPR[ spr::GSPR_ET_CHANCE ],12);
		}
		break;
	case ITEM_SCODE_REWARD_BADBEAT_GOLF_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_SECOND_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_THIRD_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_5MADE_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_6MADE_VIP:
	case ITEM_SCODE_REWARD_BADBEAT_FH_VIP:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&GLOBAL_SPR[ spr::GSPR_ET_CHANCE ],13);
		break;
	case ITEM_SCODE_REWARD_BADBEAT_FH:
	case ITEM_SCODE_REWARD_BADBEAT_FCARD:
	case ITEM_SCODE_REWARD_BADBEAT_SF:
	case ITEM_SCODE_REWARD_BADBEAT_RSF:
	case ITEM_SCODE_REWARD_BADBEAT_GOLF:
	case ITEM_SCODE_REWARD_BADBEAT_SECOND:
	case ITEM_SCODE_REWARD_BADBEAT_THIRD:
	case ITEM_SCODE_REWARD_BADBEAT_5MADE:
	case ITEM_SCODE_REWARD_BADBEAT_6MADE:
		m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene(313,521,NULL,&g_pGameView->Page,&GLOBAL_SPR[ spr::GSPR_ET_CHANCE ],14);
		break;
	}

	//get_ChanceText(itemcode);
}

void CChance_Msg_EffectSpr::get_ChanceText(int itemCode)
{

// 	ZeroMemory(m_ChanceItemText,sizeof(m_ChanceItemText));
// 	CString str;
// 
// 	str.Format( "◈[%s]님 %s \n구입자격 획득.\n", GM().GetMyInfo()->UI.NickName, RewardItemMan().GetRewardItemName( itemCode ));
// 
// 	strncpy(m_ChanceItemText,(char*)(LPCTSTR)str,str.GetLength());	


}

void CChance_Msg_EffectSpr::Destroy()
{	
	if( m_pMultiScene != NULL ){
		delete m_pMultiScene;		
	}
	m_pMultiScene= NULL;
}

int CChance_Msg_EffectSpr::ProcessObject()
{	
	if(nDelTime>0){
		nDelTime--;
		if( nDelTime == 0 ){
			nDelTime = 0;
			if( m_pMultiScene != NULL ){
				m_pMultiScene->StartMScene(1);
// 				if( strlen(m_ChanceItemText) > 0 )
// 				{
// 
// 					AddGameInfoViewEdit( m_ChanceItemText, GetChatColor(ECC2_NOTIFY) );
// 				}				
			}
		}
		return PROCESS_OK;
	}

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

void CChance_Msg_EffectSpr::DrawObject(CDC *pDC)
{
	if( m_pMultiScene != NULL )
	{
		m_pMultiScene->DrawMScene(NULL);		
	}
}


CCuttingInfo_BD::CCuttingInfo_BD()
{	
	m_pMultiScene = NULL;
	nDelayTime = 0;	
	Destroy();
}

CCuttingInfo_BD::~CCuttingInfo_BD()
{
	Destroy();
}

void CCuttingInfo_BD::Init( int nPNum, int nCutCnt )
{
	Destroy();
	m_nObjID = ANF_CUTINFO_NUM;	//종류		
	DrawFaceKind = DRAW_BOTTOM;
	nDelayTime = 1/*( cutcnt * 1) + 20*/;
	
	int nIndex = ( 4 - nCutCnt ) + ( ( 4 - nCutCnt ) * 4 ) +  nPNum;
	m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene( 0, 0, NULL, &g_pGameView->Page, &GLOBAL_SPR[ spr::GSPR_ET_CUTTINGNO ] , nIndex );	

	return ;
}

void CCuttingInfo_BD::DrawObject(CDC *pDC)
{
	if(pDC == NULL){nResult = PROCESS_KILLME;}

	if( m_pMultiScene != NULL ){
		m_pMultiScene->DrawMScene(NULL);
	}		
}

void CCuttingInfo_BD::Destroy()
{	
	if( m_pMultiScene != NULL ){
		delete m_pMultiScene;
	}
	m_pMultiScene = NULL;
}

int  CCuttingInfo_BD::ProcessObject()
{	
	if( nDelayTime > 0 ){
		nDelayTime --;
		if( nDelayTime == 0 ){
			nDelayTime = 0;
			if( m_pMultiScene != NULL ){
				m_pMultiScene->StartMScene( 1 );	
			}
		}
		return PROCESS_OK;
	}

	if( m_pMultiScene != NULL ){
		if( m_pMultiScene->m_stSceneState.bScenePlay == TRUE ){
			m_pMultiScene->OnTimer();
		}
		else{
			return PROCESS_KILLME;
		}
	}


	return PROCESS_OK;
}

CBlindBet_BD::CBlindBet_BD()
{
	m_pMultiScene = NULL;
	Destroy();
}

CBlindBet_BD::~CBlindBet_BD()
{
	Destroy();
}

void CBlindBet_BD::Init(int nPNum)
{
	Destroy();
	m_nObjID = ANF_PLAYER_BLIND;	//종류		
	DrawFaceKind = DRAW_TOP;	
	
	int x = 0;
	int y = 0;

	switch (nPNum)
	{
	case 0:
		x = 494;
		y = 582;
		break;
	case 1:
		x = 129;
		y = 366;
		break;
	case 2:
		x = 129;
		y = 172;
		break;
	case 3:
		x = 718;
		y = 172;
		break;
	case 4:
		x = 718;
		y = 366;
		break;
	default:
		break;
	}

	m_pMultiScene = new NMBASE::SKINGDI::CNMultiScene( x, y, NULL, &g_pGameView->Page, &GLOBAL_SPR[ spr::GSPR_ET_BLIND ] , 0 );
	m_pMultiScene->StartMSceneLoop();	
	return ;
}

void CBlindBet_BD::DrawObject(CDC *pDC)
{
	if(pDC == NULL){nResult = PROCESS_KILLME;}	

	if( m_pMultiScene != NULL ){
		m_pMultiScene->DrawMScene(NULL);
	}	
}

void CBlindBet_BD::Destroy()
{
	if( m_pMultiScene != NULL ){
		delete m_pMultiScene;
	}
	m_pMultiScene = NULL;
}

int  CBlindBet_BD::ProcessObject()
{
	if( m_pMultiScene != NULL ){
		if( m_pMultiScene->m_stSceneState.bScenePlay == TRUE ){
			m_pMultiScene->OnTimer();
		}
		else{
			return PROCESS_KILLME;
		}
	}
	return PROCESS_OK;
}