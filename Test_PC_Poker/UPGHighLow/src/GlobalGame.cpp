// Global....

#include "StdAfx.h"
#include "GlobalGame.h"
#include "CommMsgDef.h"
#include "CommMsgDef_Game.h"

CGameViewHighLow*	g_pGameView = NULL;	// ���� �� ���� ������(��������)
CSeatKeeper			g_SeatKeeper;				// �ڸ� ��Ŵ��
CObserverMan		g_ObserverMan;				// ����UI
CChattingWindowMan*	g_pChatWinMan = NULL;		// ä�� ������
COneShotCharge		g_cOneShotCharge;			// ��������

CUIInfoDataHighLow   g_cUIData;

const int			g_nDUMY_CARD = 52;			// ī�� �޸�
HINSTANCE		g_hInstGameDLL=NULL;
COLORREF GetChatColor(ENUM_CHAT_COLOR nType)
{
	return g_cUIData.m_listChatColor[nType];
}