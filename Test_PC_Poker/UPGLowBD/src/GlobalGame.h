// �۷ι� ����, Ŭ����, �Լ�..

#pragma once

#include "GameView.h"
#include "UserStruct.h"
#include "./sound/SoundRes.h"
#include "SpriteList.h"
#include "PlayerImageListMan.h"
#include "GFUtil.h"
#include "Player_BD.h"
#include "GameProcess_BD.h"
#include "./UIInfoData/UIInfoData_BD.h"

//--- ��ư ---//

#define IDM_BANISHVOTE		(WM_USER+6094)	// �߹�
#define IDM_GAMEITEM		(WM_USER+6095)	// ���� ������
#define IDM_RULE_ON			(WM_USER+6096) // ī�� ���� ���̱�
#define IDM_RULE_OFF		(WM_USER+6097) // ī�� ���� ���߱�
#define IDM_BTNDIRECT		(WM_USER+6098)
#define IDM_MENU_MAIN		(WM_USER+6099)
#define IDM_BTN_EXCESSMONEY		(WM_USER+9205)

#define IDM_AUTO_BTN		(WM_USER+7124)
#define IDM_AUTO_OFF		(WM_USER+7126)
#define IDM_AUTO_M			(WM_USER+7129)
#define IDM_AUTO_N			(WM_USER+7127)

//�ڸ� ��Ŵ��  (��ư Ŀ�ǵ� ���̵� �߰� ����ǥ�� �˾Ƽ� ����)
// #define IDM_BTN_USESEATKEEPER	(WM_USER+9203)
// #define IDM_BTN_BACKTOGAME		(WM_USER+9204)

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


#define g_PLAYER( nIndex ) static_cast< CPlayer_BD* >( GM().GetCurrentProcess()->GetPlayer( nIndex ) )
#define g_GAME_BD()		   static_cast< GameProcess_BD* >( GM().GetCurrentProcess() )

extern CGameViewLowBD *g_pGameView;

extern CSeatKeeper g_SeatKeeper;//�ڸ� ��Ŵ��
extern COneShotCharge		g_cOneShotCharge;
extern CObserverMan g_ObserverMan; //������
extern CChattingWindowMan *g_pChatWinMan;
extern ui::CUIInfoData_BD			g_cUIData;

extern const int TOTAL_CARDNUM;  //�� ī���
extern const int DUMY_CARD;      //ī��޸�
extern const int TOURNAMENT_DUMY_CARD; // ��ʸ�Ʈ ī�� �޸�

extern HINSTANCE		g_hInstGameDLL;

extern COLORREF GetChatColor(ENUM_CHAT_COLOR nColor);

