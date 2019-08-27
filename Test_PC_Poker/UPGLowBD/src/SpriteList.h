// SpriteList.h: interface for the CSpriteList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPRITELIST_H__11E4A548_361F_4AD1_A8F6_F6950C28C8D0__INCLUDED_)
#define AFX_SPRITELIST_H__11E4A548_361F_4AD1_A8F6_F6950C28C8D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


namespace spr
{

	enum _SPRITE
	{
		GSPR_UNKNOWN = -1,

		//BEGIN
		// PokerBase 공통파일사용
		GSPR_GAME_CHIP,
		GSPR_GAME_MONEY,
		GSPR_GAME_CALLMONEY,
		GSPR_GAME_MEMONEY,
		GSPR_GAME_USERMONEY,
		GSPR_GAME_JACKPOT,

		//game
		GSPR_GAME_CUT_BACK,
		GSPR_GAME_CUT_INFO,
		GSPR_GAME_CHATBOX,
		GSPR_GAME_RESULTNUM,
		GSPR_GAME_BIGCARD,
		GSPR_GAME_DARKCARD,
		GSPR_GAME_HOSTDEALER,
		GSPR_GAME_AUTOBETSETUP,
		GSPR_GAME_AUTOBETINFO,

		//EFFECT
		//PokerBase 공통파일사용
		GSPR_ET_BETCMD,

		//game
		GSPR_ET_MADE,	//메이드이펙트
		GSPR_ET_WIN,
		GSPR_ET_ALLINANGEL,
		GSPR_ET_GRADE,
		GSPR_ET_CHANCE,
		GSPR_ET_HIVALUE,
		GSPR_ET_CARDDEALING,
		GSPR_ET_CUTTINGNO,
		GSPR_ET_BLIND,	//블라인드

		//BTN
		//PokerBase 공통파일사용
		GSPR_BTN_TOPMENU, // Top 메뉴버튼
		GSPR_BTN_BOTTOM,
		GSPR_BTN_CLOSE,   // 닫기버튼
		//game
		GSPR_BTN_CHANGE_PASS,
		GSPR_V_H_OPEN,	    // 히든 오픈
		GSPR_BTN_GAMESTART, // 게임시작
		GSPR_BTN_AUTOUP,	// 자동올림
		GSPR_BTN_AUTOSORT,	// 자동정렬:2012.09.27 YS.Jeong 

		GSPR_LOADING_ALL,  // 읽어드릴 SPR 파일
		//END
		////////////////////////////////////////////////////////////////////////////////////


		/////////////////////////////////
		// Path 지정
		GSPR_PATH_CHATTING,
		GSPR_PATH_BET_N,
		GSPR_PATH_BET_H,
		GSPR_PATH_BET_P,
		GSPR_PATH_HANDO,
		GSPR_PATH_HELP_BTN,
		GSPR_PATH_HELP_DOUMI,
		GSPR_PATH_HELP_TBL,
		GSPR_ALL
	};

	extern const char* Sprite_IDToName( _SPRITE eTargetType );
}

extern NMBASE::GRAPHICGDI::xSprite GLOBAL_SPR[ spr::GSPR_LOADING_ALL ];

void LoadSpriteFile();

#endif // !defined(AFX_SPRITELIST_H__11E4A548_361F_4AD1_A8F6_F6950C28C8D0__INCLUDED_)
