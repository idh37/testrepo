// Global....

#include "StdAfx.h"
#include "GlobalGame.h"
#include "CommMsgDef.h"
#include "CommMsgDef_Game.h"

CGameViewSutda*	g_pGameView = NULL;	// ���� �� ���� ������(��������)
CSeatKeeper			g_SeatKeeper;				// �ڸ� ��Ŵ��
CObserverMan		g_ObserverMan;				// ����UI
CChattingWindowMan*	g_pChatWinMan = NULL;		// ä�� ������
COneShotCharge		g_cOneShotCharge;			// ��������

CUIInfoDataSutda   g_cUIData;
HINSTANCE		g_hInstGameDLL=NULL;

const int			g_nDUMY_CARD = 0;			// ī�� �޸�

COLORREF GetChatColor(ENUM_CHAT_COLOR nType)
{
	return g_cUIData.m_listChatColor[nType];
}

int factorial( int input )
{
	if( input == 0 ) return 1;

	return input* factorial( input -1 );
}
