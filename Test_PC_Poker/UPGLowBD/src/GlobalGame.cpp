// Global....
#include "StdAfx.h"
#include "GlobalGame.h"
#include "CommMsgDef.h"
#include "CommMsgDef_Game.h"

CGameViewLowBD *g_pGameView=NULL;		// 게임 뷰 윈도 포인터(전역변수)

CSeatKeeper g_SeatKeeper;//자리 지킴이
CObserverMan g_ObserverMan;			//관전UI
COneShotCharge		g_cOneShotCharge;
CChattingWindowMan*	g_pChatWinMan = NULL;		// 채팅 윈도우

ui::CUIInfoData_BD			g_cUIData;

const int TOTAL_CARDNUM  = 52; //총 카드수
const int DUMY_CARD      = 52; //카드 뒷면
const int TOURNAMENT_DUMY_CARD = 55; // 토너먼트 카드 뒷면
HINSTANCE		g_hInstGameDLL=NULL;

COLORREF GetChatColor(ENUM_CHAT_COLOR nColor)
{
	return g_cUIData.m_listChatColor[nColor];
}


