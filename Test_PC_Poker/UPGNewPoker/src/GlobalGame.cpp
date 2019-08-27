#include "stdafx.h"
#include "GlobalGame.h"

CGameViewNewPoker	*g_pGameView = NULL;
CChattingWindowMan	*g_pChatWinMan = NULL;
CPlayer_NP			*g_pMyInfo_NP = NULL;
CGameProcess_NewPoker *g_pGameProcess = NULL;

JOKERCARDDATA		g_JokerCardDeck;
int					g_MyIndex = 0;

BOOL				g_bGetJokerCard = FALSE;	// 조커 카드를 받았다

BOOL				g_bLoadSprSkinKind = -1;
BOOL				g_bDeBugModeFlg = FALSE;	// 개발자용 디버그 플래그 

CSeatKeeper			g_SeatKeeper;				//자리 지킴이
CObserverMan		g_ObserverMan;				//관전UI
COneShotCharge		g_cOneShotCharge;
CUIInfoData			g_cUIData;

const int			TOTAL_CARDNUM	= 52; //총 카드수
const int			DUMY_CARD		= 52; //카드 뒷면
const CSize			g_szCard(80, 117);
HINSTANCE		g_hInstGameDLL=NULL;

COLORREF GetChatColor(ENUM_CHAT_COLOR nColor)
{
	return g_cUIData.m_listChatColor[nColor];
}