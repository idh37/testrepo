#pragma once

#include "./GameProcess/GameProcess_NewPoker.h"
#include "./GameView.h"
#include "ImageDef.h"

#define GAME() (g_pGameProcess)
#define g_PLAYER(nPlayerPos)	((CPlayer_NP*)GM().GetCurrentProcess()->GetPlayer(nPlayerPos))	// �÷��̾ ã�� ���� �Լ�
#define g_GAMEPROCESS()			((CGameProcess_NewPoker*)GM().GetCurrentProcess())						// �������μ��� ������Ʈ ������

class CGameViewNewPoker;
class CChattingWindowMan;
class CPlayer_NP;

extern CGameViewNewPoker	*g_pGameView;
extern CChattingWindowMan	*g_pChatWinMan;
extern CPlayer_NP			*g_pMyInfo_NP;
extern CGameProcess_NewPoker *g_pGameProcess;

extern JOKERCARDDATA		g_JokerCardDeck;
extern int					g_MyIndex;

extern BOOL					g_bGetJokerCard;	// ��Ŀ ī�带 �޾Ҵ�

extern BOOL					g_bDeBugModeFlg;	// �����ڿ� ����� �÷��� 

extern CSeatKeeper			g_SeatKeeper;			//�ڸ� ��Ŵ��
extern CObserverMan			g_ObserverMan;			//����UI
extern COneShotCharge		g_cOneShotCharge;
extern CUIInfoData			g_cUIData;

extern const int			TOTAL_CARDNUM;  //�� ī���
extern const int			DUMY_CARD;      //ī��޸�
extern const CSize			g_szCard;

extern HINSTANCE		g_hInstGameDLL;

extern COLORREF GetChatColor(ENUM_CHAT_COLOR nColor);