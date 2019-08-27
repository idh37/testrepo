// SpriteList.cpp: implementation of the CSpriteList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SpriteList.h"
#include "GlobalGame.h"
#include "SelectCardHighlow.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// 게임도우미(족보 확률)
string GH_BTN_PATH			= ".\\highlow\\data\\gamehelp\\gh_btn.spr";
string DOUMI_PATH			= ".\\highlow\\data\\gamehelp\\doumi.spr";
string JBTABLE_PATH			= ".\\highlow\\data\\Skin\\JbTable.spr";

string AUTOBETPANEL_PATH	= ".\\PokerCommon\\data\\AutobetPanel.spr";	
							  


string GRADE_PATH			= ".\\highlow\\data\\grade.spr";
string CHANCE_PATH			= ".\\highlow\\data\\Chance.spr";
string HIDDENOPEN_PATH		= ".\\highlow\\data\\HiddenOpen.spr";
string HANDOEFFECT_PATH		= ".\\highlow\\data\\HandoEffect.spr";
string MADEEFFECT_PATH		= ".\\PokerCommon\\data\\complete_made.spr";	//메이드 이펙트

// 카드
string BIGCARD_PATH			= ".\\highlow\\data\\card\\BigCard.spr";
string DARKCARD_PATH		= ".\\highlow\\data\\card\\DarkBigCard.spr";
string DISCARD_PATH			= ".\\highlow\\data\\card\\discard.spr";
string CARDDEALING_PATH		= ".\\highlow\\data\\card\\CardDealing.spr";

string LASTCARDOPEN_PATH = ".\\highlow\\data\\card\\LastCardOpen.spr";

// 결과
string HIVALUEFFECT_PATH	= ".\\highlow\\data\\jokboEffect\\Effect_HighLow.spr";

// Skin 폴더
string CHATTINGWND_PATH		= ".\\highlow\\data\\Skin\\ChatBox.spr";
string WINLOSER_PATH		= ".\\highlow\\data\\Skin\\winlose.spr";
string RESULTNUMBER_PATH	= ".\\highlow\\data\\Skin\\resultnumber.spr";
string BETBTN_N_PATH		= ".\\highlow\\data\\Skin\\BetBtn_N.spr";
string BETBTN_H_PATH		= ".\\highlow\\data\\Skin\\BetBtn_H.spr";
string BETBTN_P_PATH		= ".\\highlow\\data\\Skin\\BetBtn_P.spr";

// Common 폴더
string TOPMENUBTN_PATH		= ".\\PokerCommon\\data\\TopMenuBtn.spr";
string JACKPOT_PATH			= ".\\PokerCommon\\data\\Jackpot.spr";
string CHIP_PATH			= ".\\PokerCommon\\data\\Chip.spr";

string TIMER_PATH			= ".\\PokerCommon\\data\\timer.spr";

string SP_CLOSEBTN_PATH		= ".\\PokerCommon\\data\\sp_closeBtn.spr";


string BETTINGEFFECT_PATH	= ".\\PokerCommon\\data\\BettingEffect.spr";

string HILOWSELEFFECT_PATH	= ".\\highlow\\data\\choice.spr";
string HILOWSELBUTTON_PATH	= ".\\highlow\\data\\HighLowSwingBtn.spr";
string HILOWSELRESULT_PATH	= ".\\highlow\\data\\result_highlowswing.spr";



NMBASE::GRAPHICGDI::xSprite g_sprGLOBAL[ spr::GSPR_ALL ];

NMBASE::GRAPHICGDI::xSprite g_sprUITopBtn;				// 위쪽 버튼
NMBASE::GRAPHICGDI::xSprite g_sprWinLoser;				// 승자/패자 이펙트

NMBASE::GRAPHICGDI::xSprite g_sprChip;					// 칩 
NMBASE::GRAPHICGDI::xSprite g_sprBettingEffect;			// 배팅 이펙트

NMBASE::GRAPHICGDI::xSprite g_sprJokboEffect;			// 하이 족보 효과 골프,세컨,서드

NMBASE::GRAPHICGDI::xSprite GradeSpr;					// 게임방 이용등급
NMBASE::GRAPHICGDI::xSprite ChanceEffectSpr;			// 찬스 아이템

NMBASE::GRAPHICGDI::xSprite g_sprBigCard;				// 큰 이미지 카드
NMBASE::GRAPHICGDI::xSprite g_sprDarkCard;				// 어두운 큰 이미지 카드
NMBASE::GRAPHICGDI::xSprite g_sprDisCard;				// 버린 카드
NMBASE::GRAPHICGDI::xSprite g_sprJackpotNumber;			// 게임방 잭팟 숫자

NMBASE::GRAPHICGDI::xSprite g_sprResultNumber;			// 결과 숫자

NMBASE::GRAPHICGDI::xSprite g_sprCardDealing;			// 카드 딜링 Animation

NMBASE::GRAPHICGDI::xSprite g_sprCloseButton;			// 원샷충전 Close 버튼

NMBASE::GRAPHICGDI::xSprite g_SprAutoBettingInfoMsg;

NMBASE::GRAPHICGDI::xSprite g_sprLastCardOpen;			//히든카드오픈


NMBASE::GRAPHICGDI::xSprite g_sprHighLowSwingEfc;		//하이로우 이펙트
NMBASE::GRAPHICGDI::xSprite g_sprHighLowSwingBtn;		//하이로우스윙 선택 버튼
NMBASE::GRAPHICGDI::xSprite g_sprHighLowSwing_ResultSelectCase;		//하이로우 선택 결과


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
	// 버린카드
	_LOADSPRITEDEFINE(g_sprDisCard, DISCARD_PATH, false);		
	// 족보에 안들어간 카드
	_LOADSPRITEDEFINE(g_sprDarkCard, DARKCARD_PATH, false);
	//히든카드오픈
	_LOADSPRITEDEFINE( g_sprLastCardOpen, LASTCARDOPEN_PATH, false );		

	/*-------------------------------------------------------------------------------*/
	/*	승리 spr
	/*-------------------------------------------------------------------------------*/	
	//하이 족보 효과 골프,세컨,서드
	_LOADSPRITEDEFINE(g_sprJokboEffect, HIVALUEFFECT_PATH, false);		

	_LOADSPRITEDEFINE(g_sprWinLoser, WINLOSER_PATH, false);	

	// 위쪽 버튼
	_LOADSPRITEDEFINE(g_sprUITopBtn, TOPMENUBTN_PATH, false);	

	// JackPot
	_LOADSPRITEDEFINE(g_sprJackpotNumber, JACKPOT_PATH, false);

	// 결과 숫자
	_LOADSPRITEDEFINE(g_sprResultNumber, RESULTNUMBER_PATH, false);

	// 원샷충전 버튼
	_LOADSPRITEDEFINE(g_sprCloseButton, SP_CLOSEBTN_PATH, false);

	_LOADSPRITEDEFINE(g_SprAutoBettingInfoMsg, AUTOBETPANEL_PATH, false);
	// 카드 딜링
	_LOADSPRITEDEFINE(g_sprCardDealing, CARDDEALING_PATH, false);

	// 채팅 스프라이트
	if(g_pChatWinMan)
	{
		g_pChatWinMan->LoadImage(CHATTINGWND_PATH.c_str());
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
		g_pGameView->m_cTotalBetMoney.LoadImage();

		// SidePot
		g_pGameView->m_cSidePotMoney.LoadImage(".\\highlow\\data\\side_betting_displayboard.spr");

		g_pGameView->m_cPlayerDrawManager.LoadImage(TIMER_PATH.c_str(), &g_sprBettingEffect, &g_sprWinLoser, ".\\highlow\\data\\AllinEffect.spr");
		g_pGameView->m_cGameHelper.LoadImage(GH_BTN_PATH.c_str(), DOUMI_PATH.c_str(), JBTABLE_PATH.c_str(), ".\\highlow\\data\\jokboinfo.spr");
		
#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
		if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
			g_pGameView->m_cBackGround.LoadImage(".\\highlow\\data\\Skin\\Gameback_toctoc.spr");
		else
			g_pGameView->m_cBackGround.LoadImage(".\\highlow\\data\\Skin\\GameBack.spr");
#else //UPGRADE_20120308_TOCTOC_CHANNELING
		g_pGameView->m_cBackGround.LoadImage(".\\highlow\\data\\Skin\\GameBack.spr");
#endif //UPGRADE_20120308_TOCTOC_CHANNELING

		g_pGameView->m_cTitle.LoadImage(IDX_GAME_HL);
		g_GAMEPROCESS()->m_AllBetChip.LoadImage();
	}	

	// 한도머니
	g_cOwnLimit.LoadImage(HANDOEFFECT_PATH.c_str());

	

	//하이로우 전용
	_LOADSPRITEDEFINE(g_sprHighLowSwingEfc, HILOWSELEFFECT_PATH, false);
	_LOADSPRITEDEFINE(g_sprHighLowSwingBtn, HILOWSELBUTTON_PATH, false);
	_LOADSPRITEDEFINE(g_sprHighLowSwing_ResultSelectCase, HILOWSELRESULT_PATH, false);
}
