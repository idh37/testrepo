// Global....

#include "StdAfx.h"
#include "GlobalGame.h"
#include "CommMsgDef.h"
#include "CommMsgDef_Game.h"

CGameViewHighLow*	g_pGameView = NULL;	// 게임 뷰 윈도 포인터(전역변수)
CSeatKeeper			g_SeatKeeper;				// 자리 지킴이
CObserverMan		g_ObserverMan;				// 관전UI
CChattingWindowMan*	g_pChatWinMan = NULL;		// 채팅 윈도우
COneShotCharge		g_cOneShotCharge;			// 원샷충전

CUIInfoDataHighLow   g_cUIData;

const int			g_nDUMY_CARD = 52;			// 카드 뒷면
HINSTANCE		g_hInstGameDLL=NULL;
COLORREF GetChatColor(ENUM_CHAT_COLOR nType)
{
	return g_cUIData.m_listChatColor[nType];
}