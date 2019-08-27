// SpriteList.cpp: implementation of the CSpriteList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SpriteList.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace spr
{

const struct SPRITE_AND_ENTRY
{
	_SPRITE eType;
	char* pName;
}	c_aSpriteAndEntry[] =
{
		// PokerBase 파일을 공유한다.
	{GSPR_GAME_CHIP,		(".\\PokerCommon\\data\\Chip.spr")},
	{GSPR_GAME_MONEY,		(".\\PokerCommon\\data\\TotalMoney.spr")},
	{GSPR_GAME_CALLMONEY,	(".\\PokerCommon\\data\\CallMoney.spr")},
	{GSPR_GAME_MEMONEY,		(".\\PokerCommon\\data\\me_money.spr")},
	{GSPR_GAME_USERMONEY,	(".\\PokerCommon\\data\\usermoney.spr")},
	{GSPR_GAME_JACKPOT,		(".\\PokerCommon\\data\\Jackpot.spr")},
		//game
	{GSPR_GAME_CUT_BACK,	(".\\Lowbd\\data\\Skin\\CuttingBack.spr")},
	{GSPR_GAME_CUT_INFO,	(".\\Lowbd\\data\\Skin\\CuttingInfo.spr")},
	{GSPR_GAME_CHATBOX,		(".\\Lowbd\\data\\chattingarea\\ChatBox.spr")},
	{GSPR_GAME_RESULTNUM,	(".\\Lowbd\\data\\Skin\\resultnumber.spr")},
	{GSPR_GAME_BIGCARD,		(".\\Lowbd\\data\\card\\BigCard_BD.spr")},
	{GSPR_GAME_DARKCARD,	(".\\Lowbd\\data\\card\\DarkCard_BD.spr")},
	{GSPR_GAME_HOSTDEALER,	(".\\Lowbd\\data\\Skin\\HostBossDealer.spr")},
	{GSPR_GAME_AUTOBETSETUP,(".\\Lowbd\\data\\autobet\\AutobetSetup.spr")},
	{GSPR_GAME_AUTOBETINFO,	(".\\Lowbd\\data\\autobet\\AutobetPanel.spr")},

	//Effect
		// PokerBase
	{GSPR_ET_BETCMD,		(".\\PokerCommon\\data\\BettingEffect.spr")},
	
		//game
	{GSPR_ET_MADE,			(".\\Lowbd\\data\\effect\\complete_made_eft.spr")},
	{GSPR_ET_WIN,			(".\\Lowbd\\data\\effect\\winlose.spr")},
	{GSPR_ET_ALLINANGEL,	(".\\Lowbd\\data\\AllinAngel.spr")},  
	{GSPR_ET_GRADE,			(".\\Lowbd\\data\\grade\\grade.spr")},
	{GSPR_ET_CHANCE,		(".\\Lowbd\\data\\Chance.spr")},
	{GSPR_ET_HIVALUE,		(".\\Lowbd\\data\\effect\\hivalueffect.spr")},
	{GSPR_ET_CARDDEALING,	(".\\Lowbd\\data\\effect\\CardDealing.spr")},
	{GSPR_ET_CUTTINGNO,		(".\\Lowbd\\data\\effect\\CuttingNum.spr")},
	{GSPR_ET_BLIND,			(".\\Lowbd\\data\\effect\\Blindbet.spr")},
	
	//BTN
		// PokerBase
	{GSPR_BTN_TOPMENU,		(".\\PokerCommon\\data\\TopMenuBtn.spr")},
	{GSPR_BTN_BOTTOM,		(".\\PokerCommon\\data\\UIBtn.spr")},
	{GSPR_BTN_CLOSE,		(".\\PokerCommon\\data\\sp_closeBtn.spr")},
		//game
	{GSPR_BTN_CHANGE_PASS,	(".\\Lowbd\\data\\button\\ChangePassBtn.spr")},
	{GSPR_V_H_OPEN,			(".\\Lowbd\\data\\button\\HiddenOpen.spr")},
	{GSPR_BTN_GAMESTART,	(".\\Lowbd\\data\\button\\GameStart.spr")},
	{GSPR_BTN_AUTOUP,		(".\\Lowbd\\data\\button\\AutoUp.spr")},
		//자동정렬 버튼추가:2012.09.27 YS.Jeong 
	{GSPR_BTN_AUTOSORT,		(".\\Lowbd\\data\\button\\AutoSort.spr")},

	/////////////////////////////////
	// Path 지정
	{GSPR_PATH_CHATTING,	(".\\Lowbd\\data\\chattingarea\\ChatBox.spr")},
	{GSPR_PATH_BET_N,		(".\\Lowbd\\data\\button\\BetBtn_N.spr")},
	{GSPR_PATH_BET_H,		(".\\Lowbd\\data\\button\\BetBtn_H.spr")},
	{GSPR_PATH_BET_P,		(".\\Lowbd\\data\\button\\BetBtn_P.spr")},
	{GSPR_PATH_HANDO,		(".\\Lowbd\\data\\effect\\HandoEffect.spr")},
	{GSPR_PATH_HELP_BTN,	(".\\Lowbd\\data\\gamehelp\\gh_btn.spr")},
	{GSPR_PATH_HELP_DOUMI,	(".\\Lowbd\\data\\gamehelp\\doumi.spr")},
	{GSPR_PATH_HELP_TBL,	(".\\Lowbd\\data\\Skin\\JbTable.spr")},
};

const char* Sprite_IDToName( _SPRITE eTargetType )
{
	for (int i = 0; i < dimof(c_aSpriteAndEntry); ++i)
	{
		if ( eTargetType == c_aSpriteAndEntry[ i ].eType )
		{
			return c_aSpriteAndEntry[ i ].pName;
		}
	}
	return ("GSPR_UNKNOWN");
}

}

NMBASE::GRAPHICGDI::xSprite GLOBAL_SPR[ spr::GSPR_LOADING_ALL ];

void ReleaseSpriteFile()
{
	for( int i = 0; i < spr::GSPR_LOADING_ALL; i++ )
	{
		GLOBAL_SPR[ i ].Remove();
	}
}

void LoadSpriteFile()
{
	ReleaseSpriteFile();

	// 모든 스프라이트 읽어들인다.
	for( int i = 0; i < spr::GSPR_LOADING_ALL; i++ )
	{
		std::string strFullName = spr::Sprite_IDToName( ( spr::_SPRITE ) i );
		_LOADSPRITEDEFINE( GLOBAL_SPR[ i ], strFullName, false  );
	}
	
	
	// 채팅 스프라이트
	if ( g_pChatWinMan)
	{
		std::string strPath = spr::Sprite_IDToName( spr::GSPR_PATH_CHATTING ); // 경로
		g_pChatWinMan->LoadImage( strPath.c_str() );
	}

	// 버튼을 모두 로드한다.
	if ( g_pGameView )
	{
		std::string strPathBtnN = spr::Sprite_IDToName( spr::GSPR_PATH_BET_N );
		std::string strPathBtnH = spr::Sprite_IDToName( spr::GSPR_PATH_BET_H );
		std::string strPathBtnP = spr::Sprite_IDToName( spr::GSPR_PATH_BET_P );	
		g_pGameView->m_cGameViewBtnMan.LoadImage(strPathBtnN.c_str(), strPathBtnH.c_str(), strPathBtnP.c_str());
	}

	// 자리지킴이
	g_SeatKeeper.LoadImage();

	// 옵져버
	g_ObserverMan.LoadImage();

	g_cOneShotCharge.LoadImage();

	std::string strHando = spr::Sprite_IDToName( spr::GSPR_PATH_HANDO );
	g_cOwnLimit.LoadImage( strHando.c_str() );

	
	if ( g_pGameView )
	{
		g_pGameView->m_cSidePotMoney.LoadImage(".\\Lowbd\\data\\side_betting_displayboard.spr");
		g_pGameView->m_cGameHelper.LoadImage( Sprite_IDToName( spr::GSPR_PATH_HELP_BTN ), Sprite_IDToName( spr::GSPR_PATH_HELP_DOUMI ), Sprite_IDToName( spr::GSPR_PATH_HELP_TBL ), ".\\PokerCommon\\data\\jokboinfobd.spr" );

		g_pGameView->m_cPlayerDrawManager.LoadImage( NULL, &GLOBAL_SPR[ spr::GSPR_ET_BETCMD ], &GLOBAL_SPR[ spr::GSPR_ET_WIN ], ".\\Lowbd\\data\\AllinEffect.spr");
		g_pGameView->m_cTotalBetMoney.LoadImage();

#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
		if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
			g_pGameView->m_cBackGround.LoadImage(".\\Lowbd\\data\\Skin\\Gameback_toctoc.spr");
		else
			g_pGameView->m_cBackGround.LoadImage(".\\Lowbd\\data\\Skin\\Gameback.spr");
#else //UPGRADE_20120308_TOCTOC_CHANNELING
		g_pGameView->m_cBackGround.LoadImage(".\\Lowbd\\data\\Skin\\Gameback.spr");
#endif //UPGRADE_20120308_TOCTOC_CHANNELING
		g_pGameView->m_cTitle.LoadImage(IDX_GAME_BD);
		g_GAME_BD()->m_cAllBetChip.LoadImage();
	}

}
