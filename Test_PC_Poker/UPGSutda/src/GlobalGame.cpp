// Global....

#include "StdAfx.h"
#include "GlobalGame.h"
#include "CommMsgDef.h"
#include "CommMsgDef_Game.h"

CGameViewSutda*	g_pGameView = NULL;	// 게임 뷰 윈도 포인터(전역변수)
CSeatKeeper			g_SeatKeeper;				// 자리 지킴이
CObserverMan		g_ObserverMan;				// 관전UI
CChattingWindowMan*	g_pChatWinMan = NULL;		// 채팅 윈도우
COneShotCharge		g_cOneShotCharge;			// 원샷충전

CUIInfoDataSutda   g_cUIData;
HINSTANCE		g_hInstGameDLL=NULL;

const int			g_nDUMY_CARD = 0;			// 카드 뒷면

COLORREF GetChatColor(ENUM_CHAT_COLOR nType)
{
	return g_cUIData.m_listChatColor[nType];
}

int factorial( int input )
{
	if( input == 0 ) return 1;

	return input* factorial( input -1 );
}
