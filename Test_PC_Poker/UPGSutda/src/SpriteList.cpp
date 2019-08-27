// SpriteList.cpp: implementation of the CSpriteList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SpriteList.h"
#include "GlobalGame.h"
#include "SelectCardSutda.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// 게임도우미(족보 확률)
string GH_BTN_PATH			= ".\\sutda\\data\\gamehelp\\gh_btn.spr";
string DOUMI_PATH			= ".\\sutda\\data\\gamehelp\\doumi.spr";
string JBTABLE_PATH			= ".\\sutda\\data\\Skin\\JbTable.spr";

string AUTOBETPANEL_PATH	= ".\\PokerCommon\\data\\AutobetPanel.spr";	
							  


string GRADE_PATH			= ".\\sutda\\data\\grade.spr";
string CHANCE_PATH			= ".\\sutda\\data\\Chance.spr";
string HIDDENOPEN_PATH		= ".\\sutda\\data\\HiddenOpen.spr";
string HANDOEFFECT_PATH		= ".\\sutda\\data\\HandoEffect.spr";
string MADEEFFECT_PATH		= ".\\PokerCommon\\data\\complete_made.spr";	//메이드 이펙트

// 카드
string BIGCARD_PATH			= ".\\sutda\\data\\card\\BigCard.spr";




// Skin 폴더
string CHATTINGWND_PATH		= ".\\sutda\\data\\Skin\\ChatBox.spr";
string CHATTIN_TAB_EFFECT_PATH		= ".\\sutda\\data\\Skin\\chattingtabeffect.spr";
string WINLOSER_PATH		= ".\\sutda\\data\\Skin\\winlose.spr";
string RESULTNUMBER_PATH	= ".\\sutda\\data\\Skin\\resultnumber.spr";
string BETBTN_N_PATH		= ".\\sutda\\data\\Skin\\BetBtn_N.spr";
string BETBTN_H_PATH		= ".\\sutda\\data\\Skin\\BetBtn_H.spr";
string BETBTN_P_PATH		= ".\\sutda\\data\\Skin\\BetBtn_P.spr";

// Common 폴더
string TOPMENUBTN_PATH		= ".\\PokerCommon\\data\\TopMenuBtn.spr";
string JACKPOT_PATH			= ".\\PokerCommon\\data\\Jackpot.spr";
string CHIP_PATH			= ".\\PokerCommon\\data\\Chip.spr";

string TIMER_PATH			= ".\\sutda\\data\\Anidata\\timer.spr";				//섯다용 타이머 이펙트는 따로있다.

string SP_CLOSEBTN_PATH		= ".\\PokerCommon\\data\\sp_closeBtn.spr";


string BETTINGEFFECT_PATH	= ".\\sutda\\data\\BettingEffect.spr";		//섯다용 베팅이펙트는 따로있다.

string PENATYMONEY_RED_PATH		= ".\\sutda\\data\\PenaltyRed.spr";
string PENATYMONEY_BLUE_PATH		= ".\\sutda\\data\\PenaltyBlue.spr";

string CALLMONEY_PATH			= ".\\sutda\\data\\CallMoney.spr";

string SUTDA_TURN_PATH		= ".\\sutda\\data\\turn.spr";

string SUTDA_HIDDENOPENTOOLTIP_PATH = ".\\sutda\\data\\HiddenOpen.spr";

NMBASE::GRAPHICGDI::xSprite g_sprGLOBAL[ spr::GSPR_ALL ];

NMBASE::GRAPHICGDI::xSprite g_sprWinLoser;				// 승자/패자 이펙트

NMBASE::GRAPHICGDI::xSprite g_sprChip;					// 칩 
NMBASE::GRAPHICGDI::xSprite g_sprBettingEffect;			// 배팅 이펙트

NMBASE::GRAPHICGDI::xSprite GradeSpr;					// 게임방 이용등급
NMBASE::GRAPHICGDI::xSprite ChanceEffectSpr;			// 찬스 아이템

NMBASE::GRAPHICGDI::xSprite g_sprBigCard;				// 큰 이미지 카드

NMBASE::GRAPHICGDI::xSprite g_sprJackpotNumber;			// 게임방 잭팟 숫자


NMBASE::GRAPHICGDI::xSprite g_sprCloseButton;			// 원샷충전 Close 버튼

NMBASE::GRAPHICGDI::xSprite g_SprAutoBettingInfoMsg;


NMBASE::GRAPHICGDI::xSprite g_sprPenaltyRed;				// 붉은 숫자 땡값 스프라이트
NMBASE::GRAPHICGDI::xSprite g_sprPenaltyBlue;			// 붉은 숫자 땡값 스프라이트
NMBASE::GRAPHICGDI::xSprite g_sprTurn;					//섯다 턴
NMBASE::GRAPHICGDI::xSprite g_HiddenOpenSpr;			//섯다 히든 오픈 단축키 툴팁


void LoadSpriteFile()
{
	/*-------------------------------------------------------------------------------*/
	/*	게임 방 공통 spr
	/*-------------------------------------------------------------------------------*/
	_LOADSPRITEDEFINE(g_sprChip, CHIP_PATH, false);
	_LOADSPRITEDEFINE(g_sprBettingEffect, BETTINGEFFECT_PATH, false);		


	//등급
	_LOADSPRITEDEFINE(GradeSpr, GRADE_PATH, false);

	//찬스아이템 효과
	_LOADSPRITEDEFINE(ChanceEffectSpr, CHANCE_PATH, false);
	// 새로 추가된 버튼및 히든, 체크	
	_LOADSPRITEDEFINE(g_sprGLOBAL[ spr::GSPR_V_H_OPEN ], HIDDENOPEN_PATH );
	_LOADSPRITEDEFINE(g_sprGLOBAL[ spr::GSPR_HANDO ]	, HANDOEFFECT_PATH, false); // 보유한도달성 이펙트
	_LOADSPRITEDEFINE(g_sprGLOBAL[ spr::GSPR_ET_MADE ]	, MADEEFFECT_PATH, false); //메이드이펙트

	/*-------------------------------------------------------------------------------*/
	/*	카드 spr
	/*-------------------------------------------------------------------------------*/	
	
	// 빅 카드
	_LOADSPRITEDEFINE(g_sprBigCard, BIGCARD_PATH, false);	
		


	/*-------------------------------------------------------------------------------*/
	/*	승리 spr
	/*-------------------------------------------------------------------------------*/	

	_LOADSPRITEDEFINE(g_sprWinLoser, WINLOSER_PATH, false);	
	
	// JackPot
	_LOADSPRITEDEFINE(g_sprJackpotNumber, JACKPOT_PATH, false);

	// 원샷충전 버튼
	_LOADSPRITEDEFINE(g_sprCloseButton, SP_CLOSEBTN_PATH, false);

	_LOADSPRITEDEFINE(g_SprAutoBettingInfoMsg, AUTOBETPANEL_PATH, false);
	

	// 떙값 숫자
	_LOADSPRITEDEFINE(g_sprPenaltyRed, PENATYMONEY_RED_PATH, false);
	_LOADSPRITEDEFINE(g_sprPenaltyBlue, PENATYMONEY_BLUE_PATH, false);


	_LOADSPRITEDEFINE(g_sprPenaltyBlue, PENATYMONEY_BLUE_PATH, false);

	_LOADSPRITEDEFINE(g_sprTurn, SUTDA_TURN_PATH, false);			//섯다 턴
	_LOADSPRITEDEFINE(g_HiddenOpenSpr, SUTDA_HIDDENOPENTOOLTIP_PATH, false);		//섯다 히든 오픈 단축키 툴팁
	


	

	// 채팅 스프라이트
	if(g_pChatWinMan)
	{
		g_pChatWinMan->LoadImage(CHATTINGWND_PATH.c_str(), CHATTIN_TAB_EFFECT_PATH.c_str() );
	}

	// 자리지킴이
	g_SeatKeeper.LoadImage();

	// 옵져버
	g_ObserverMan.LoadImage();

	// 원샷충전
	g_cOneShotCharge.LoadImage();
	g_SelectCard().LoadImage();

	// 캐릭터 정보
	if(g_pGameView)
	{
		// 버튼을 모두 로드한다.
		g_pGameView->m_cGameViewBtnMananager.LoadImage( BETBTN_N_PATH.c_str(), BETBTN_H_PATH.c_str(), BETBTN_P_PATH.c_str() );

		// 중앙 금액 표시
		g_pGameView->m_cTotalBetMoney.LoadImage(CALLMONEY_PATH.c_str(), CALLMONEY_PATH.c_str());

		// SidePot
		g_pGameView->m_cSidePotMoney.LoadImage(".\\NewPoker\\data\\side_betting_displayboard.spr");

		g_pGameView->m_cPlayerDrawManager.LoadImage(TIMER_PATH.c_str(), 
													&g_sprBettingEffect, 
													&g_sprWinLoser, 
													".\\sutda\\data\\AllinEffect.spr", 
													&g_sprTurn , 
													".\\sutda\\data\\bg.spr",
													".\\sutda\\data\\Level.spr");
//		g_pGameView->m_cGameHelper.LoadImage(GH_BTN_PATH.c_str(), DOUMI_PATH.c_str(), JBTABLE_PATH.c_str(), ".\\PokerCommon\\data\\jokboinfo.spr");

#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
		if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
			g_pGameView->m_cBackGround.LoadImage(".\\sutda\\data\\Skin\\Gameback_toctoc.spr");
		else
			g_pGameView->m_cBackGround.LoadImage(".\\sutda\\data\\Skin\\GameBack.spr");
#else //UPGRADE_20120308_TOCTOC_CHANNELING
		g_pGameView->m_cBackGround.LoadImage(".\\sutda\\data\\Skin\\GameBack.spr");
#endif //UPGRADE_20120308_TOCTOC_CHANNELING

		g_pGameView->m_cTitle.LoadImage(IDX_GAME_SD);
		//g_GAMEPROCESS()->m_AllBetChip.LoadImage();

		//섯다 축음기
		g_pGameView->m_Radiophonograph.LoadImage();

		//섯다 애니메이션
		g_pGameView->m_AniQue.LoadImage();

		g_pGameView->m_cAskRematchPopup.LoadImage();	

		g_pGameView->m_cChipBoard.LoadImage();

	}	

	// 한도머니
	g_cOwnLimit.LoadImage(HANDOEFFECT_PATH.c_str());
}
