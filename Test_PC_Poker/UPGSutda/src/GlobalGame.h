// 글로벌 변수, 클래스, 함수..

#pragma once

#include "GameViewSutda.h"
#include "Define_Sutda.h"
//#include "UserStruct.h"
#include "SoundRes.h"
#include "SpriteList.h"
#include "AniSprClassList.h"
#include "GFUtil.h"
#include "PlayerSutda.h"
#include "GameProcessSutda.h"
#include "UIInfoDataSutda.h"
#include "SutdaGameSound.h"

//--- 버튼 ---//

#define IDM_ACCEPT_REMATCH (WM_USER+6074)
#define IDM_REFUSAL_REMATCH (WM_USER+6075)

#define IDM_BANISHVOTE		(WM_USER+6094)	// 추방
#define IDM_GAMEITEM		(WM_USER+6095)	// 게임 아이템
#define IDM_RULE_ON			(WM_USER+6096) // 카드 족보 보이기
#define IDM_RULE_OFF		(WM_USER+6097) // 카드 족보 감추기
#define IDM_BTNDIRECT		(WM_USER+6098)
#define IDM_MENU_MAIN		(WM_USER+6099)

#define IDM_AUTO_BTN		(WM_USER+7124)
#define IDM_AUTO_OFF		(WM_USER+7126)
#define IDM_AUTO_M			(WM_USER+7129)
#define IDM_AUTO_N			(WM_USER+7127)

#define UM_WINDCARD_TIMER	(WM_USER+8001)

#define IDM_GAMEOVER_ANGBUY	(WM_USER+9003) // 수호천사 구매하기
#define IDM_GAMEOVER_ANGUSE	(WM_USER+9004) // 수호천사 사용하기

#define IDM_GAMEHELP		(WM_USER+9005) // 게임 도움말

#define IDM_CHANGE_BTN		(WM_USER+9007) // 오토베팅 보기 닫기
#define IDM_PASS_BTN		(WM_USER+9008) // 오토베팅 보기 닫기

#define IDM_USEREMPTY_BTN	(WM_USER+9012) //

#define IDM_AUTOCUTON_BTN	(WM_USER+9013) //
#define IDM_AUTOCUTOFF_BTN	(WM_USER+9014) //

#define IDM_JACKPOT			(WM_USER+9200)


#define UM_VIP_SERVICE		(WM_USER+9201)

//보유한도초과금
#define IDM_BTN_EXCESSMONEY (WM_USER+9205)






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


// UPGPokerBase의 CardDefine.h에 정의가 되어 있는데, 
// 7Poker쪽의 카드 출력하는 부분의 Y값 변경 요청에 의해서 새로 Define해 사용함.(기본은 9로 사용 중)
#define POKER7_CARD_GAB_Y 7


#define g_PLAYER(nPlayerPos)	((CPlayerSutda*)GM().GetCurrentProcess()->GetPlayer(nPlayerPos))	// 플레이어를 찾는 전역 함수
#define g_GAMEPROCESS()			((CGameProcessSutda*)GM().GetCurrentProcess())						// 게임프로세스 오브젝트 포인터

extern CGameViewSutda*	g_pGameView;		// GameView Pointer
extern CSeatKeeper		g_SeatKeeper;		// 자리 지킴이
extern CObserverMan		g_ObserverMan;		// 관전자
extern CChattingWindowMan* g_pChatWinMan;
extern COneShotCharge	g_cOneShotCharge;	// 원샷충전

extern const int		g_nDUMY_CARD;		// 카드뒷면

extern CUIInfoDataSutda g_cUIData;

extern HINSTANCE		g_hInstGameDLL;

extern COLORREF GetChatColor(ENUM_CHAT_COLOR nType);

int factorial( int input );