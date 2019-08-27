#pragma once

#include "./GameProcess/GameProcess_NewPoker.h"
#include "./GameView.h"
#include "ImageDef.h"

#define GAME() (g_pGameProcess)
#define g_PLAYER(nPlayerPos)	((CPlayer_NP*)GM().GetCurrentProcess()->GetPlayer(nPlayerPos))	// 플레이어를 찾는 전역 함수
#define g_GAMEPROCESS()			((CGameProcess_NewPoker*)GM().GetCurrentProcess())						// 게임프로세스 오브젝트 포인터

class CGameViewNewPoker;
class CChattingWindowMan;
class CPlayer_NP;

extern CGameViewNewPoker	*g_pGameView;
extern CChattingWindowMan	*g_pChatWinMan;
extern CPlayer_NP			*g_pMyInfo_NP;
extern CGameProcess_NewPoker *g_pGameProcess;

extern JOKERCARDDATA		g_JokerCardDeck;
extern int					g_MyIndex;

extern BOOL					g_bGetJokerCard;	// 조커 카드를 받았다

extern BOOL					g_bDeBugModeFlg;	// 개발자용 디버그 플래그 

extern CSeatKeeper			g_SeatKeeper;			//자리 지킴이
extern CObserverMan			g_ObserverMan;			//관전UI
extern COneShotCharge		g_cOneShotCharge;
extern CUIInfoData			g_cUIData;

extern const int			TOTAL_CARDNUM;  //총 카드수
extern const int			DUMY_CARD;      //카드뒷면
extern const CSize			g_szCard;

extern HINSTANCE		g_hInstGameDLL;

extern COLORREF GetChatColor(ENUM_CHAT_COLOR nColor);