// Global....

#include "StdAfx.h"
#include "GlobalGame.h"
#include "CommMsgDef.h"
#include "CommMsgDef_Game.h"

CGameViewHoola*	g_pGameView = NULL;	// ���� �� ���� ������(��������)
CSeatKeeper			g_SeatKeeper;				// �ڸ� ��Ŵ��
CObserverMan		g_ObserverMan;				// ����UI
CChattingWindowMan*	g_pChatWinMan = NULL;		// ä�� ������
COneShotCharge		g_cOneShotCharge;			// ��������

CUIInfoDataHoola   g_cUIData;
HINSTANCE		g_hInstGameDLL=NULL;

const int			g_nDUMY_CARD = 52;			// ī�� �޸�

COLORREF GetChatColor(ENUM_CHAT_COLOR nType)
{
	return g_cUIData.m_listChatColor[nType];
}