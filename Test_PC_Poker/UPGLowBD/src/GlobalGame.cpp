// Global....
#include "StdAfx.h"
#include "GlobalGame.h"
#include "CommMsgDef.h"
#include "CommMsgDef_Game.h"

CGameViewLowBD *g_pGameView=NULL;		// ���� �� ���� ������(��������)

CSeatKeeper g_SeatKeeper;//�ڸ� ��Ŵ��
CObserverMan g_ObserverMan;			//����UI
COneShotCharge		g_cOneShotCharge;
CChattingWindowMan*	g_pChatWinMan = NULL;		// ä�� ������

ui::CUIInfoData_BD			g_cUIData;

const int TOTAL_CARDNUM  = 52; //�� ī���
const int DUMY_CARD      = 52; //ī�� �޸�
const int TOURNAMENT_DUMY_CARD = 55; // ��ʸ�Ʈ ī�� �޸�
HINSTANCE		g_hInstGameDLL=NULL;

COLORREF GetChatColor(ENUM_CHAT_COLOR nColor)
{
	return g_cUIData.m_listChatColor[nColor];
}


