#include "stdafx.h"
#include "GlobalGame.h"

CGameViewNewPoker	*g_pGameView = NULL;
CChattingWindowMan	*g_pChatWinMan = NULL;
CPlayer_NP			*g_pMyInfo_NP = NULL;
CGameProcess_NewPoker *g_pGameProcess = NULL;

JOKERCARDDATA		g_JokerCardDeck;
int					g_MyIndex = 0;

BOOL				g_bGetJokerCard = FALSE;	// ��Ŀ ī�带 �޾Ҵ�

BOOL				g_bLoadSprSkinKind = -1;
BOOL				g_bDeBugModeFlg = FALSE;	// �����ڿ� ����� �÷��� 

CSeatKeeper			g_SeatKeeper;				//�ڸ� ��Ŵ��
CObserverMan		g_ObserverMan;				//����UI
COneShotCharge		g_cOneShotCharge;
CUIInfoData			g_cUIData;

const int			TOTAL_CARDNUM	= 52; //�� ī���
const int			DUMY_CARD		= 52; //ī�� �޸�
const CSize			g_szCard(80, 117);
HINSTANCE		g_hInstGameDLL=NULL;

COLORREF GetChatColor(ENUM_CHAT_COLOR nColor)
{
	return g_cUIData.m_listChatColor[nColor];
}