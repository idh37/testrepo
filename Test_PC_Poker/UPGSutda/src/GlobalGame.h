// �۷ι� ����, Ŭ����, �Լ�..

#pragma once

#include "GameViewSutda.h"
#include "Define_Sutda.h"
//#include "UserStruct.h"
#include "SoundRes.h"
#include "SpriteList.h"
#include "AniSprClassList.h"
#include "GFUtil.h"
#include "PlayerSutda.h"
#include "GameProcessSutda.h"
#include "UIInfoDataSutda.h"
#include "SutdaGameSound.h"

//--- ��ư ---//

#define IDM_ACCEPT_REMATCH (WM_USER+6074)
#define IDM_REFUSAL_REMATCH (WM_USER+6075)

#define IDM_BANISHVOTE		(WM_USER+6094)	// �߹�
#define IDM_GAMEITEM		(WM_USER+6095)	// ���� ������
#define IDM_RULE_ON			(WM_USER+6096) // ī�� ���� ���̱�
#define IDM_RULE_OFF		(WM_USER+6097) // ī�� ���� ���߱�
#define IDM_BTNDIRECT		(WM_USER+6098)
#define IDM_MENU_MAIN		(WM_USER+6099)

#define IDM_AUTO_BTN		(WM_USER+7124)
#define IDM_AUTO_OFF		(WM_USER+7126)
#define IDM_AUTO_M			(WM_USER+7129)
#define IDM_AUTO_N			(WM_USER+7127)

#define UM_WINDCARD_TIMER	(WM_USER+8001)

#define IDM_GAMEOVER_ANGBUY	(WM_USER+9003) // ��ȣõ�� �����ϱ�
#define IDM_GAMEOVER_ANGUSE	(WM_USER+9004) // ��ȣõ�� ����ϱ�

#define IDM_GAMEHELP		(WM_USER+9005) // ���� ����

#define IDM_CHANGE_BTN		(WM_USER+9007) // ���亣�� ���� �ݱ�
#define IDM_PASS_BTN		(WM_USER+9008) // ���亣�� ���� �ݱ�

#define IDM_USEREMPTY_BTN	(WM_USER+9012) //

#define IDM_AUTOCUTON_BTN	(WM_USER+9013) //
#define IDM_AUTOCUTOFF_BTN	(WM_USER+9014) //

#define IDM_JACKPOT			(WM_USER+9200)


#define UM_VIP_SERVICE		(WM_USER+9201)

//�����ѵ��ʰ���
#define IDM_BTN_EXCESSMONEY (WM_USER+9205)






// ���� ����
enum EGAMESTATE
{
	EGAME_WAITING		= 0,	// ����� 
	EGAME_PLAYING		= 1,	// ������ 
};

// �÷��̾� ���� ���� ����
enum EJOINSTATE
{	
	EGAME_JOINOUT		= 0,	// ���� ����
	EGAME_JOININ		= 1,	// ���� ���� 
};

// �÷��̾� ����
enum EPLAYERSTATE
{	
	EPLAYER_GAMEEND		= 0,	// �������� 
	EPLAYER_GAMEPLAYING	= 1,	// ������
};


// UPGPokerBase�� CardDefine.h�� ���ǰ� �Ǿ� �ִµ�, 
// 7Poker���� ī�� ����ϴ� �κ��� Y�� ���� ��û�� ���ؼ� ���� Define�� �����.(�⺻�� 9�� ��� ��)
#define POKER7_CARD_GAB_Y 7


#define g_PLAYER(nPlayerPos)	((CPlayerSutda*)GM().GetCurrentProcess()->GetPlayer(nPlayerPos))	// �÷��̾ ã�� ���� �Լ�
#define g_GAMEPROCESS()			((CGameProcessSutda*)GM().GetCurrentProcess())						// �������μ��� ������Ʈ ������

extern CGameViewSutda*	g_pGameView;		// GameView Pointer
extern CSeatKeeper		g_SeatKeeper;		// �ڸ� ��Ŵ��
extern CObserverMan		g_ObserverMan;		// ������
extern CChattingWindowMan* g_pChatWinMan;
extern COneShotCharge	g_cOneShotCharge;	// ��������

extern const int		g_nDUMY_CARD;		// ī��޸�

extern CUIInfoDataSutda g_cUIData;

extern HINSTANCE		g_hInstGameDLL;

extern COLORREF GetChatColor(ENUM_CHAT_COLOR nType);

int factorial( int input );