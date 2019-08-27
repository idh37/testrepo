// 글로벌 변수, 클래스, 함수..

#pragma once

#include "GameView.h"
#include "UserStruct.h"
#include "./sound/SoundRes.h"
#include "SpriteList.h"
#include "PlayerImageListMan.h"
#include "GFUtil.h"
#include "Player_BD.h"
#include "GameProcess_BD.h"
#include "./UIInfoData/UIInfoData_BD.h"

//--- 버튼 ---//

#define IDM_BANISHVOTE		(WM_USER+6094)	// 추방
#define IDM_GAMEITEM		(WM_USER+6095)	// 게임 아이템
#define IDM_RULE_ON			(WM_USER+6096) // 카드 족보 보이기
#define IDM_RULE_OFF		(WM_USER+6097) // 카드 족보 감추기
#define IDM_BTNDIRECT		(WM_USER+6098)
#define IDM_MENU_MAIN		(WM_USER+6099)
#define IDM_BTN_EXCESSMONEY		(WM_USER+9205)

#define IDM_AUTO_BTN		(WM_USER+7124)
#define IDM_AUTO_OFF		(WM_USER+7126)
#define IDM_AUTO_M			(WM_USER+7129)
#define IDM_AUTO_N			(WM_USER+7127)

//자리 지킴이  (버튼 커맨드 아이디 추가 물음표는 알아서 수정)
// #define IDM_BTN_USESEATKEEPER	(WM_USER+9203)
// #define IDM_BTN_BACKTOGAME		(WM_USER+9204)

// 게임 상태
enum EGAMESTATE
{
	EGAME_WAITING		= 0,	// 대기중 
	EGAME_PLAYING		= 1,	// 게임중 
};

// 플레이어 게임 참여 상태
enum EJOINSTATE
{	
	EGAME_JOINOUT		= 0,	// 게임 불참
	EGAME_JOININ		= 1,	// 게임 참여 
};

// 플레이어 상태
enum EPLAYERSTATE
{	
	EPLAYER_GAMEEND		= 0,	// 게임종료 
	EPLAYER_GAMEPLAYING	= 1,	// 게임중
};


#define g_PLAYER( nIndex ) static_cast< CPlayer_BD* >( GM().GetCurrentProcess()->GetPlayer( nIndex ) )
#define g_GAME_BD()		   static_cast< GameProcess_BD* >( GM().GetCurrentProcess() )

extern CGameViewLowBD *g_pGameView;

extern CSeatKeeper g_SeatKeeper;//자리 지킴이
extern COneShotCharge		g_cOneShotCharge;
extern CObserverMan g_ObserverMan; //관전자
extern CChattingWindowMan *g_pChatWinMan;
extern ui::CUIInfoData_BD			g_cUIData;

extern const int TOTAL_CARDNUM;  //총 카드수
extern const int DUMY_CARD;      //카드뒷면
extern const int TOURNAMENT_DUMY_CARD; // 토너먼트 카드 뒷면

extern HINSTANCE		g_hInstGameDLL;

extern COLORREF GetChatColor(ENUM_CHAT_COLOR nColor);

