#ifndef __USERSTRUCT_H__
#define __USERSTRUCT_H__

//#include "PluginCommon.h"
#include "GameCommon.h"
#include "BetStruct.h"//¥��ġ��
#include <math.h>	//���Ĩ����Ʈ
#include "UserStructDefine.h"

#if defined(_ONLY_SERVER)
#include "LogFile.h"
#endif // defined(_ONLY_SERVER)

#pragma warning (disable:4996)

//����
#define VERSION 406		//	2011.01.27	// TODO ���Ӻ���

#define SIZE_DAT_KEY (33)

//G_TFT


const short SAME_MONEY_GAME_TYPE = 42; //�Ӵ� �����ڵ�   <----��Ŀ�Ӵ� �ڵ�

#define STRING_GAME_INITIAL ("pokerlobby")

#define MAX_CHANNEL_SETTING_COUNT 50

typedef struct 
{
	BOOL bRead[MAX_CHANNEL_SETTING_COUNT];	
	INT64 nLimitMoney[MAX_CHANNEL_SETTING_COUNT];	
	INT64 nSeedMoney[MAX_CHANNEL_SETTING_COUNT];
	INT64 nMinMoney[MAX_CHANNEL_SETTING_COUNT];
	INT64 nMaxMoney[MAX_CHANNEL_SETTING_COUNT];
	INT64 nCreateLimitM[MAX_CHANNEL_SETTING_COUNT];
	INT64 nQuckStartM[MAX_CHANNEL_SETTING_COUNT];
	INT64 nCreRoomSeedM[MAX_CHANNEL_SETTING_COUNT];
	INT64 nQuickSatrtSeedM[MAX_CHANNEL_SETTING_COUNT];
	int nBetRule[MAX_CHANNEL_SETTING_COUNT];
	int nFormKind[MAX_CHANNEL_SETTING_COUNT];
	int nBetCount[MAX_CHANNEL_SETTING_COUNT];
	int nDefaultCh[MAX_CHANNEL_SETTING_COUNT];
	int nDefaultCh_QS[MAX_CHANNEL_SETTING_COUNT];
} CHANNEL_SET_INFO;

typedef struct 
{
	BOOL bRead[MAX_CHANNEL_SETTING_COUNT];	
	int GroupType[MAX_CHANNEL_SETTING_COUNT];	
	INT64 nMinMoney[MAX_CHANNEL_SETTING_COUNT];
	INT64 nMaxMoney[MAX_CHANNEL_SETTING_COUNT];	
	INT64 nLimitMoney[MAX_CHANNEL_SETTING_COUNT];	
	INT nRoomMIdx[MAX_CHANNEL_SETTING_COUNT];
	INT nRoomLimitMIdx[MAX_CHANNEL_SETTING_COUNT];	
	INT64 nCreateLimitM[MAX_CHANNEL_SETTING_COUNT];
	INT64 nQuckStartM[MAX_CHANNEL_SETTING_COUNT];
	INT64 nCreRoomSeedM[MAX_CHANNEL_SETTING_COUNT];
	INT64 nQuickSatrtSeedM[MAX_CHANNEL_SETTING_COUNT];	
} ROOM_SET_INFO;

typedef struct 
{
	BOOL bRead;
	int GroupType;	
	INT64 nMaxBetPerRound;
	INT64 nWarningDailyLoss;
	INT64 nWarningDailyLossInRoom;
	INT64 nMaxDailyLossMoney;
	INT64 nBasicMaxDailyLossMoney;
	INT64 nNormalBetLimitMoney;
	INT64 nDiamondBetLimitMoney;

} LIMIT_LAW;


enum FORMKIND
{
	FORMKIND_NORMAL=0,
	FORMKIND_CHOICE=1,
	FORMKIND_BOMB=2,
	FORMKIND_JOKER=3,
	FORMKIND_DEFAULT=4,

	////////////////////////////�����ε����, Ÿ�پ˸� 2017.10.12/////////////////////////////////////
	FORMKIND_BLIND=5, //�����ε� ���ø���̴�. �ο�ٵ��̿��� ���..
	//////////////////////////
};

////////////////////////////���ý���,���ä�� 2017.11.07/////////////////////////////////////
enum ROOM_CHIPKIND //������ ���� ���� �Ҵ°� ������ ��ȭ Ÿ���̴�.
{
	ROOM_CHIPKIND_NORMAL = 0, //�׳� ������ ����ߴ� ���ӸӴ��̴�.
	ROOM_CHIPKIND_GOLD, //���Ӱ� �߰��� ����̴�.
	ROOM_CHIPKIND_GOLD_BANK, //����ũ�� �ڵ� �߰�
};
////////////////////////////////////////////////////////////////

// ��� ���� �и�. ���� ������ Ĩ Ÿ��. 2018-08-14
namespace PlayableChipType
{
	enum Kind
	{
		MIN = -1,
		NORMAL = 0,
		GOLD,
		MAX
	};
}

////////////////////////////Ŭ���ý��� 2017.12.21/////////////////////////////////////
enum CLUB_ACTION_CODE
{
	//Ŭ�� ������ ������ �����ڵ�
	CLUB_ACTION_ALLOW = 0, //�޾Ƶ���.. ������ �ʴ�, ������ Ŭ�� ���Ե��� �޾Ƶ��϶�..
	CLUB_ACTION_DENY, //�ź���.. ������ �ʴ�, ������ Ŭ�� ���Ե��� �ź��Ҷ�
};

enum CLUB_INVITED_TYPE
{
	//Ŭ�� �ʴ������ Ÿ���̴�.
	//�ʴ븦 �������̳�, �ڹ������� ���� ��û�� �Ѱ��̳�..
	CLUB_INVITED_BY_BOSS = 0, //������ �ʴ�
	CLUB_INVITED_BY_ME = 1, //���� �ڹ������� ���Կ�û
};

enum CLUB_MEMBER_GRADE
{
	CLUB_MEMBER_GRADE_BOSS = 0, //Ŭ���� �����̴�.
	CLUB_MEMBER_GRADE_NORMAL, //Ŭ���� �Ϲ�ȸ���̴�.

	CLUB_MEMBER_GRADE_NONE = 100, //Ŭ���� �������� ���� �����̴�.
};

enum CLUB_FIRE_REASON
{
	//Ŭ������ ����Ż��� �����̴�.
	CLUB_FIRE_REASON_BOSS_KICK = 0, //������ ������ Ż�� ���״�.(Ŭ���� ����)
	CLUB_FIRE_REASON_CLOSE, //Ŭ���� �ػ�Ǹ鼭 �ڿ����� Ż��
};

enum COMMON_ERR_CODE
{
	//�� Ŭ�������� ����ϴ°� �ƴϰ�, Ư�� Request�� ���� Response �Ҽ� ���� ��Ȳ���� ������ �ȴ�.
	COMMON_ERR_CODE_PERMISSION = 1000, //��� ������ ����.. ���� Ŭ���� ������ Ŭ������ �ƴ� ����� ��û�� ����
	COMMON_ERR_CODE_SYSTEM, //DB������ ���� �ý��ۿ��� ������ ������ ���� ���� ���
};

enum CLUB_MYINFO_TYPE
{
	//CLUB_MYINFO ��Ŷ�� ���÷� ���޵Ǵµ�, �̰� �� ���� �Ǵ��� Ÿ������ ������.
	CLUB_MYINFO_NORMAL = 0, //�α���, ä���̵��� �Ϲ����� ��Ȳ
	CLUB_MYINFO_MY_REQUEST, //������ �ʴ� ����, �ڹ��� Ż��� ���� ���ÿ� ���� ������ ����Ǿ� ���� ��Ȳ
	CLUB_MYINFO_OTHER_REQUEST, //�� Ŭ�� ���Կ� ���� ������ ����, ������ ���� ����, ������ Ŭ�� �ػ�� �ٸ� ������ ���ÿ� ���� ������ ����Ǿ� ���� ��Ȳ
};
/////////////////////////////////////////////////////

////////////////////////////2018�����̺�Ʈ 2018.01.22/////////////////////////////////////
enum PROMOTION_COMMON_GAMECOUNT_PMCODE
{
	//�����Ǽ��� �̿��Ͽ� ����Ǵ� ���θ���� ���� ID
	PROMOTION_COMMON_GAMECOUNT_PMCODE_2018_LUCKYBAG = 1, //2018�� ���ָӴ� �̺�Ʈ
	PROMOTION_COMMON_GAMECOUNT_PMCODE_2018_GOLDEN_LUCKYBAG, //2018�� Ȳ�� ���ָӴ� �̺�Ʈ
	PROMOTION_COMMON_GAMECOUNT_PMCODE_2018_NEWYEAR_CHICKEN, //2018�� ���� ġŲ �̺�Ʈ
};
///////////////////////////////////////////////////////////

enum PROMOTION_ODDEVEN_START_ERRCODE
{
	//Ȧ¦���θ�� ���ۿ�û�� ���� �����ڵ�
	//0.�������, 1.�����̹�������, 2.��Ÿ����, 10.ĸ�������ڵ� ����ġ
	PROMOTION_ODDEVEN_START_ERRCODE_OK = 0,
	PROMOTION_ODDEVEN_START_ERRCODE_TODAY_ALREADY_PLAY,
	PROMOTION_ODDEVEN_START_ERRCODE_ETC,

	////////////////////////////����⼮�� CAPTCHA 2018.07.31/////////////////////////////////////
	PROMOTION_ODDEVEN_START_ERRCODE_CAPTCHA_DISMATCH = 10,
	//////////////////////////////
};

enum PROMOTION_ODDEVEN_BET_SIDE
{
	//���û��̵� 0.¦��, 1.Ȧ��
	PROMOTION_ODDEVEN_BET_SIDE_EVEN = 0,
	PROMOTION_ODDEVEN_BET_SIDE_ODD
};

enum PROMOTION_WHEEL_START_ERRCODE
{
	//�� �̴ϰ��� ���θ�� ���ۿ�û�� ���� �����ڵ�
	//0.�������, 1.�����̹�������, 2.��Ÿ����, 10.ĸ�������ڵ� ����ġ
	PROMOTION_WHEEL_START_ERRCODE_OK = 0,
	PROMOTION_WHEEL_START_ERRCODE_TODAY_ALREADY_PLAY,
	PROMOTION_WHEEL_START_ERRCODE_ETC
};

//�������� ���� 2006.2.3
enum BENEFIT
{

	BENEFIT_SUPPLY_ALL = 0,	//��ü ���� ����
	BENEFIT_UNSUPPLY_CHANCE,	//���������� �� ���޿���
	BENEFIT_UNSUPPLY_JACKPOT,	//���� �� ���޿���,
	BENEFIT_UNSUPPLY_EVENT,		//�̺�Ʈ �� ���޿���
	BENEFIT_UNSUPPLY_FREECHARGE,    // ���� ���� �� ���޿���,

	BENEFIT_UNSUPPLY_ALL=10,		//��ü �� ����

	BENEFIT_MAXCOUNT			// �� ���� ���� ����
};

enum RULETYPE
{
	RULETYPE_7POKER=1,
	RULETYPE_5POKER
};

enum BET_CMD{
	BET_CMD_PING=0,			// ��	
	BET_CMD_DADANG,			// ����
	BET_CMD_ALLIN,			// ����
	BET_CMD_QUARTER,		// ���� 
	BET_CMD_HALF,			// ���� 
	BET_CMD_FULL,			// Ǯ
	BET_CMD_RACE,			// Race���� 	
	BET_CMD_SMALL_BLIND,	//���� �����ε� [4������]
	BET_CMD_BIG_BLIND,		//�� �����ε� [4������]
	// LIMIT����
	BET_CMD_LIMIT_1X,  // 1��
	BET_CMD_LIMIT_2X,  // 2��
	BET_CMD_LIMIT_3X,  // 3��
	BET_CMD_LIMIT_5X,  // 5��
	BET_CMD_LIMIT_10X,  // 10��
	BET_CMD_LIMIT_20X,  // 20��
	BET_CMD_LIMIT_30X,  // 30��
	BET_CMD_LIMIT_40X,  // 40��
	BET_CMD_LIMIT_60X,  // 60��
	BET_CMD_LIMIT_80X,  // 80��
	BET_CMD_LIMIT_100X,  // 100��
	BET_CMD_LIMIT_120X,  // 120��
	
	BET_CMD_MAX_BET,	// �ƽ� ����

	MAX_BET_CMD,
};

enum eLIMIT_X
{
	LIMIT_X_UNKNOWN = 0,
	LIMIT_1X=  1,
	LIMIT_2X=  2,
	LIMIT_3X=  3,
	LIMIT_5X=  5,
	LIMIT_10X=  10,
	LIMIT_20X=  20,
	LIMIT_30X=  30,
	LIMIT_40X=  40,
	LIMIT_60X=  60,
	LIMIT_80X=  80,
	LIMIT_100X=  100,
	LIMIT_120X=  120,
};

enum  RAISE_LAST_CMD		// nLastCmd ���� 
{
	RAISE_LAST_CMD_NULL=0,
	RAISE_LAST_CMD_FOLD,
	RAISE_LAST_CMD_CHECK,
	RAISE_LAST_CMD_CALL,
	RAISE_LAST_CMD_PING,
	RAISE_LAST_CMD_DADANG,
	RAISE_LAST_CMD_QUARTER,
	RAISE_LAST_CMD_HALF,
	RAISE_LAST_CMD_FULL,
	RAISE_LAST_CMD_RACE,	
	RAISE_LAST_CMD_ALLIN,
	RAISE_LAST_CMD_SMALL_BLIND,  //���� �����ε� [4������]
	RAISE_LAST_CMD_BIG_BLIND,	//�� �����ε� [4������]
	// LIMIT����
	RAISE_LAST_CMD_LIMIT_1X,  // 1��
	RAISE_LAST_CMD_LIMIT_2X,  // 2��
	RAISE_LAST_CMD_LIMIT_3X,  // 3��
	RAISE_LAST_CMD_LIMIT_5X,  // 5��
	RAISE_LAST_CMD_LIMIT_10X,  // 10��
	RAISE_LAST_CMD_LIMIT_20X,  // 20��
	RAISE_LAST_CMD_LIMIT_30X,  // 30��
	RAISE_LAST_CMD_LIMIT_40X,  // 40��
	RAISE_LAST_CMD_LIMIT_60X,  // 60��
	RAISE_LAST_CMD_LIMIT_80X,  // 80��
	RAISE_LAST_CMD_LIMIT_100X,  // 100��
	RAISE_LAST_CMD_LIMIT_120X,  // 120��

	RAISE_LAST_CMD_MAX_BET,		// �ƽ� ����

	RAISE_LAST_CMD_MAX
};


enum REASON_OUTROOM			// OnUserOutRoom ȣ��� �� ���� ���� 
{
	REASON_OUTROOM_NULL=0,
	REASON_OUTROOM_DISCONNECT,				// ������ ���� 
	REASON_OUTROOM_OUTOFMONEY,				// �Ӵ� �������� ����
	REASON_OUTROOM_USER_REQUEST,			// ������ ��û�� ���� ����
	REASON_OUTROOM_USER_AFK,				// �������, �ڸ�������� ���� ����
	REASON_OUTROOM_OUTOFPLAYTIME,			// �÷���Ÿ�� ���ѽð��� ���� 
	REASON_OUTROOM_DAILYLOSS_LIMIT,			// �÷���Ÿ�� ���ѽð��� ���� 
	REASON_OUTROOM_BANNED,					// �����߹�, �߹���ǥ ���� 
	REASON_OUTROOM_BANNED_BY_ADMIN,			// �����ڿ� ���� ����	
	REASON_OUTROOM_HACKER,					// ���ǵ��� ����� ����
	REASON_OUTROOM_ABNORMAL,				// ������ ���� (��������)
	REASON_OUTROOM_FLOODING,				// ���輺 ���� ����	
	REASON_OUTROOM_NOPLAYER,				// ���۹��� �������� �ڵ����� or �����ϴٰ� ����� ������ �ڵ�����.
	REASON_OUTROOM_GRADUATION,				// ����Ʈ ���� ���� 
	REASON_OUTROOM_CANT_OBSERVE,			// �����̵� �ʿ� ������ ���� �Ұ��� ���� ����
	REASON_OUTROOM_QUICKSTART_IN_ROOM,		// �ٸ� �� �ٷΰ���� ����
	REASON_OUTROOM_TOURNAMENT,
	REASON_OUTROOM_TOURNAMENT_EXCEPTION,	// ��ʸ�Ʈ ���� ��Ȳ�� ���� �� ����
	REASON_OUTROOM_TOURNAMENT_ALLIN,		// LEAVEROOMSTATE_ALLIN
	REASON_OUTROOM_TOURNAMENT_MERGE,		// LEAVEROOMSTATE_MERGE
	REASON_OUTROOM_TOURNAMENT_BREAK,		// LEAVEROOMSTATE_BREAK
	REASON_OUTROOM_TOURNAMENT_TIMEOVER,		// LEAVEROOMSTATE_TIMEOVER
	REASON_OUTROOM_TOURNAMENT_CANCEL,		// LEAVEROOMSTATE_CANCELGAME
	REASON_OUTROOM_TOURNAMENT_WINNER,		// LEAVEROOMSTATE_MERGE
	REASON_OUTROOM_ROUNDMODE_GAMEOVER,		// ���� ��� ��������� ����	
	REASON_OUTROOM_BD_MORNINGBET,			// �ο�ٵ��� ��ħ�� �� ����
};
enum GAMESTATE
{
	GAMESTATE_NONE=0,			//
	GAMESTATE_CARD_3,						// ī�� 3��
	GAMESTATE_CARD_SELECT,					// ī�� ����
	GAMESTATE_CARD_4,						// ī�� 4��
	GAMESTATE_CARD_5,						// ī�� 5��
	GAMESTATE_CARD_6,						// ī�� 6��
	GAMESTATE_CARD_HIDDEN,					// ���� 
	GAMESTATE_CARD_WINCASE,					// �� ���� (���̷ο�)
	GAMESTATE_CARD_GAMEOVER,				// ���ӿ��� 
};

enum DAILYLOSS_STATE
{
	DAILYLOSS_STATE_NORMAL = 0,
	DAILYLOSS_STATE_WARNING,		// 70%
	DAILYLOSS_STATE_WARNING_IN_ROOM,// 90%
	DAILYLOSS_STATE_BLOCKED,		// 10���� ���� (���Ӻ� ����)
	DAILYLOSS_STATE_BLOCKED_24H,	// 10���� �ʰ� (��� ���� ����)
	DAILYLOSS_STATE_LOGIN,
	DAILYLOSS_STATE_END
};

namespace auth
{

// 2�� ��й�ȣ
enum AUTH_RESPONSE_TYPE
{
	eAUTH_UNKNOWN = 0,		// ������
	eAUTH_SUCCESS,			// ����
	eAUTH_CONNETION_FAIL,	// �������( ���� ���� ���� )
	eAUTH_REENTRANT,		// �ߺ�����( ���� ���� �Ұ� )
	eAUTH_IP_MISMATCH,		// IP����ġ( ���� ���� �Ұ� )
	eAUTH_TIMEOUT,			// ����� Ȯ���� �̷��� �ð��� �����Ǿ���( ���� ���� �Ұ� )
	eAUTH_FAIL,				// ����( ���� ���� �Ұ� )
};

}

#if defined(_GAMETEST)
#define MAX_ROUNDMODE_PLAYER_SU 20		// ���� ��� ������ �ִ� ��
#else
#define MAX_ROUNDMODE_PLAYER_SU 300		// ���� ��� ������ �ִ� ��
#endif

//#define MAX_ROUNDMODE_WAIT_ROOM 100		// ���� ��� ��� �� ��
#define MAX_ROUNDMODE_WAIT_USER_TIME 20000	// ������ ���ð�.
#define MAX_ROUNDMODE_WAIT_GAMEROOM_PLAY_USER_TIME 120000	// �ٸ� �Ϲ� ���ӹ濡�� �÷������� ������ 2���� �� �ش�.


enum ROUND_MODE_MONEY_TYPE
{
	ROUNDMODE_MONEY_20B = 0,			// 200��
	ROUNDMODE_MONEY_3HB,			// 3000��
	ROUNDMODE_MONEY_4T,				// 4��
	ROUNDMODE_MONEY_10T,				// 10��
	ROUNDMODE_MONEY_END = ROUNDMODE_MONEY_10T
};

// �ɼ� �׽�Ʈ�� �ӽ� ������ 2015-06-19
//enum ROUND_MODE_MONEY_TYPE
//{
//	ROUNDMODE_MONEY_20B = 0,			// 200��
//	ROUNDMODE_MONEY_3HB,			// 3000��
//	ROUNDMODE_MONEY_4T,				// 4��
//	ROUNDMODE_MONEY_10T,				// 10��
//	ROUNDMODE_OPTION_1,				// 
//	ROUNDMODE_OPTION_2,				// 
//	ROUNDMODE_OPTION_3,				// 
//	ROUNDMODE_OPTION_4,				// 
//	ROUNDMODE_OPTION_5,				// 
//	ROUNDMODE_OPTION_6,				// 
//	ROUNDMODE_OPTION_7,				// 
//	ROUNDMODE_MONEY_END = ROUNDMODE_OPTION_7
//};
#define MAX_ROUNDMODE_TYPE		4		// TYPE�� �ִ� �� enum ROUND_MODE_MONEY_TYPE �� ���� �Ѿߵȴ�.
//#define MAX_ROUNDMODE_TYPE		10		// TYPE�� �ִ� �� enum ROUND_MODE_MONEY_TYPE �� ���� �Ѿߵȴ�.

//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////                               [���� ������ �۾�]                               //////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#define MAX_ITEM_NUM           (30) // �ִ������ ����

#define GAMEITEM_MES_ERROR     (-1) // ����
#define GAMEITEM_MES_SUCCESS   ( 0) // ����
#define GAMEITEM_MES_DBERROR   ( 1) // DB����
#define GAMEITEM_MES_NOITEM    ( 2) // �������� ���ų� ��밡���� ������ ����
#define GAMEITEM_MES_USEABLE   ( 3) // �������� �ְ� ������� �ʾҴ�
#define GAMEITEM_MES_EXPIRED   ( 4) // �̹� ������� �������̰�, �Ⱓ�� ����Ǿ���
#define GAMEITEM_MES_USEFAIL   ( 5) // �����ۻ�뿡 ����������, �ٽ� �õ� ��û

/*
   [ ���� ������ �ڵ�ǥ ]

	  1 : ���縶��
	  2 : ���帶��
	  3 : ��帶��
	 21 : õ�Ϲ��� ������
	 31 : ���� �ʱ�ȭ ������
	 41 : ����ġ ��й� ������
	 61 : ��ȣõ�� ������
	
	130 : �����౸ ü�¾�����(30�ϱ�)
	210 : �����౸ �ٷ¾�����(10�ϱ�)
	230 : �����౸ �ٷ¾�����(30�ϱ�)
	310 : �����౸ ���߷¾�����(10�ϱ�)
	330 : �����౸ ���߷¾�����(30�ϱ�)
	410 : �����౸ ��ȭ�¾�����(10�ϱ�)
	430 : �����౸ ��ȭ�¾�����(30�ϱ�)

	201 : ���� ����� ������
	301 : �����Ǽ� ������
	401 : ���۹��� ������
	501 : �÷��� ����Ʈ ������
	601 : ���� ������
*/

#define ITEM_SCODE_INVINCIBLE		21		// õ�Ϲ��� ������ ���� �ڵ� (���۹�, ����, �����) 

#define ITEM_SCODE_SECRET			100		// [�������] ������ ���� �ڵ�
#define ITEM_ECODE_SECRET			199		// [�������] ������ �� �ڵ�

#define ITEM_SCODE_SUPERMASTER		401		// ���� ���� ������ ���� �ڵ�
#define ITEM_ECODE_SUPERMASTER		401		// ���� ���� ������ �� �ڵ�

#define ITEM_SCODE_PLUSPOINT		501		// �÷��� ����Ʈ ������ ���� �ڵ�
#define ITEM_ECODE_PLUSPOINT		501		// �÷��� ����Ʈ ������ �� �ڵ�

#define ITEM_SCODE_POKER_HELP		1455		// ���� �����
#define ITEM_ECODE_POKER_HELP		1455		// ���� �����

#define ITEM_SCODE_QUICKJOINER	(1457)		// ������� �ٷ����� ������ ���� �ڵ�
#define ITEM_ECODE_QUICKJOINER	(1457)		// ������� �ٷ����� ������ �� �ڵ�

enum ITEM_SCODE_REWARD 
{
	ITEM_SCODE_REWARD_FH=51,				//Ǯ�Ͽ콺
	ITEM_SCODE_REWARD_FCARD=52,				//��ī��
	ITEM_SCODE_REWARD_SF=53,				//��Ƽ��
	ITEM_SCODE_REWARD_RSF=54,				//��Ƽ��	//���� ������
	ITEM_SCODE_REWARD_GOLF=58,				//����
	ITEM_SCODE_REWARD_SECOND=62,			//������
	ITEM_SCODE_REWARD_THIRD=63,				//����
	ITEM_SCODE_REWARD_LUCKAVATA=109,		//[���ƹ�Ÿ] 2006.07.24

// #if defined(__PROMOTION_CHANCEAVATA__) (2011.07.28 �����ƹ�Ÿ���θ��)
	ITEM_SCODE_REWARD_CHANCEAVATA = 509,	// �����ƹٳ� 2011.07.27
// #endif // (__PROMOTION_CHANCEAVATA__)
//#if defined(__SUTDA__)
	ITEM_SCODE_REWARD_38=59,				// 38���� ������
	ITEM_SCODE_REWARD_PARKMUNSU=60,			// ������ ������
	ITEM_SCODE_REWARD_TANGKILLER=61,		// ������
//#endif // (__SUTDA__)

//#if defined(__MODIFY_CHANCEITEM__)
	ITEM_SCODE_REWARD_VIP = 2231,			// VIP ���� ������
	ITEM_SCODE_REWARD_VVIP = 2232,			// VVIP ���� ������

//#endif // (__MODIFY_CHANCEITEM__)

	// CRPARk 20120305 3�� �������˽� �Ƕ� �����ƹ�Ÿ ����
	ITEM_SCODE_REWARDALLINCLINIC = 101,	   // �Ƕ� ����Ŭ���� (2012��3�� ���� ���� ���̻� ���� ����)
	ITEM_SCODE_REWARDJACKPOT     = 127,    // �Ƕ� ����
    ITEM_SCODE_REWARD2MISSION    = 130,	   // �Ƕ� ���� �̼� (2012��03�� ���� ���� ���̻� ���� ����)
	ITEM_SCODE_REWARDTHANKHOOLA  = 131,    // �Ƕ� ��ť �Ƕ�
	ITEM_SCORE_REWARDQUADRUPLE	 = 2233,    // 4�������� (2012��03�� ���� ���� �ű� �߰�)

// ��� ȸ�� (Bad-Beat) ����
	ITEM_SCODE_REWARD_BADBEAT_FH     = 3000,			//Ǯ�Ͽ콺
	ITEM_SCODE_REWARD_BADBEAT_FCARD  = 3001,			//��ī��
	ITEM_SCODE_REWARD_BADBEAT_SF     = 3002,			//��Ƽ��
	ITEM_SCODE_REWARD_BADBEAT_RSF    = 3003,			//��Ƽ��
	ITEM_SCODE_REWARD_BADBEAT_GOLF   = 3004,			//����
	ITEM_SCODE_REWARD_BADBEAT_SECOND = 3005,			//������
	ITEM_SCODE_REWARD_BADBEAT_THIRD  = 3006,			//����
	ITEM_SCODE_REWARD_BADBEAT_5MADE  = 3007,			//���̵�
	ITEM_SCODE_REWARD_BADBEAT_6MADE  = 3008,			//6 ���̵������

// VIP ���ȸ�� ����
	ITEM_SCODE_REWARD_BADBEAT_FLUSH_VIP  = 3099,			//�÷���
	ITEM_SCODE_REWARD_BADBEAT_FH_VIP     = 3100,			//Ǯ�Ͽ콺
	ITEM_SCODE_REWARD_BADBEAT_FCARD_VIP  = 3101,			//��ī��
	ITEM_SCODE_REWARD_BADBEAT_SF_VIP     = 3102,			//��Ƽ��
	ITEM_SCODE_REWARD_BADBEAT_RSF_VIP    = 3103,			//��Ƽ��
	ITEM_SCODE_REWARD_BADBEAT_GOLF_VIP   = 3104,			//����
	ITEM_SCODE_REWARD_BADBEAT_SECOND_VIP = 3105,			//������
	ITEM_SCODE_REWARD_BADBEAT_THIRD_VIP  = 3106,			//����
	ITEM_SCODE_REWARD_BADBEAT_5MADE_VIP  = 3107,			//���̵�
	ITEM_SCODE_REWARD_BADBEAT_6MADE_VIP  = 3108,			//6 ���̵������

};

//���������� �ڵ�
namespace REWARDITEM_RESULT
{
	enum
	{
		ITEM_PURCHASED = 1,			// ���� �ߴ�.
		ITEM_NOT_PURCHASED = 2,		// �������� �ʾҴ�.
	};
}

#define DEFAULT_GAMEFEE (0.05)  //�⺻ ���Ӻ�  (�� ���Ӻ񺸴� ���� ���õǸ� ����������)

//�йи� ����
enum CODE_NEW_FAMILY
{
	CODE_NEW_FAMILY_NONE=0,
	CODE_NEW_FAMILY_MINI,
	CODE_NEW_FAMILY_SILVER,
	CODE_NEW_FAMILY_GOLD,
	CODE_NEW_FAMILY_DIAMOND,

	MAX_CODE_NEW_FAMILY
};

//PC�� ��ǰ �ڵ� 
const int CODE_PCROOM_NOT			= 0;	//	�����̾� PC���� �ƴ�
const int CODE_PCROOM_BASIC			= 19;	//	�����̾� PC��
const int CODE_PCROOM_PLUS			= 20;	//	�����̾� PC�� �÷���

//�йи� ����
struct PREMOPTION //�ɼ� ��ǰ ���� ����
{	
	enum CODE_OPTION
	{
		CODE_OPTION_KISA=0, //��� ȸ�� // ������
		CODE_OPTION_JEKI, //��� ����
		CODE_OPTION_LIMITUP, //�ѵ� ��
		CODE_OPTION_GOLD_EGG, // Ȳ�ݾ�
		//----------------------------
		MAX_PREMOPTION,
	};


	struct OPTIONUNIT
	{
		char bValid;
		char PremCode[6];
		char PremGoodsCode[4];
		DATE EndDateTime;	//�ɼ� ���� �ð�
		
		BYTE Count;
		INT64 Money;
		
	};

private:
	OPTIONUNIT Option[MAX_PREMOPTION];
public:

	PREMOPTION()
	{
		memset( Option, 0, sizeof(Option) );
	}

		//�ش� �ɼ� ��ǰ�� ������ �ִ��� �˻��Ѵ�.
		BOOL HaveOption(CODE_OPTION TypeOption)
		{
			if( TypeOption < 0 || TypeOption >= MAX_PREMOPTION ) return FALSE;

			COleDateTime curtime = COleDateTime::GetCurrentTime();
			if( curtime.m_dt > Option[TypeOption].EndDateTime ) return FALSE;

			return (Option[TypeOption].bValid);
		}
		//���� Ƚ���� �����´�. 
		int GetRemainCount( CODE_OPTION TypeOption )
		{
			if( TypeOption < 0 || TypeOption >= MAX_PREMOPTION ) return 0;
			if (HaveOption(TypeOption)==FALSE) return 0;

			return Option[TypeOption].Count;
		}

		INT64 GetMoney( CODE_OPTION TypeOption )
		{
			if( TypeOption < 0 || TypeOption >= MAX_PREMOPTION ) return 0;
			if (HaveOption(TypeOption)==FALSE) return 0;

			return Option[TypeOption].Money;
		}

		const char* GetOptionCode( CODE_OPTION TypeOption )
		{
			if (TypeOption<0 || TypeOption>=MAX_PREMOPTION) return NULL;
			return Option[TypeOption].PremCode;
		}

		const char* GetOptionGoodsCode( CODE_OPTION TypeOption )
		{
			if (TypeOption<0 || TypeOption>=MAX_PREMOPTION) return NULL;
			return Option[TypeOption].PremGoodsCode;
		}

		void SetCount( CODE_OPTION TypeOption, int Count )
		{
			if (TypeOption<0 || TypeOption>=MAX_PREMOPTION) return;
			Option[TypeOption].Count = Count;
		}

		void SetMoney( CODE_OPTION TypeOption, INT64 Money )
		{
			if (TypeOption<0 || TypeOption>=MAX_PREMOPTION) return;
			Option[TypeOption].Money = Money;
		}		

		// DB���� �о�� �ɼ� ��ǰ�� �����Ѵ�. 
		void SetOption(CODE_OPTION TypeOption, const char *pPremCode, const char *pPremGoodsCode, DATE EndDate)
		{
			if (TypeOption<0 || TypeOption>=MAX_PREMOPTION) return;
			
			ZeroMemory(&Option[TypeOption], sizeof(OPTIONUNIT));
			strncpy(Option[TypeOption].PremCode, pPremCode , sizeof(Option[TypeOption].PremCode)-1);
			strncpy(Option[TypeOption].PremGoodsCode, pPremGoodsCode , sizeof(Option[TypeOption].PremGoodsCode)-1);
			Option[TypeOption].bValid = (char) TRUE;
			Option[TypeOption].EndDateTime = EndDate;
		}
};


//�йи� ����
char *g_GetPremName(int PremMenuNo, BOOL bEnglish=TRUE);



//����� ���� �ڵ�
enum CODE_SPECIAL
{
	CODE_SPECIAL_NONE = -1,
	CODE_SPECIAL_RSF=3,
	CODE_SPECIAL_SF=2,
	CODE_SPECIAL_FCARD=1,
	CODE_SPECIAL_GOLF=CODE_SPECIAL_RSF,
	CODE_SPECIAL_SECOND=CODE_SPECIAL_SF,
	CODE_SPECIAL_THIRD=CODE_SPECIAL_FCARD,
	CODE_SPECIAL_FULLHOUSE = 4,
//#if defined(__SUTDA__)
	CODE_SPECIAL_38GWANG=CODE_SPECIAL_RSF,
	CODE_SPECIAL_GWANG=CODE_SPECIAL_SF,
	CODE_SPECIAL_DDANG=CODE_SPECIAL_FCARD,
//#endif // (__SUTDA__)
};

//[�����̾� ���� �������� �߰�2] [2007-05]
typedef struct
{
// 	STATUS_PREM	PremStatus;			// ������ ȸ�� �������±���
// 	int			ContentsNo;			// Contents_no
// 	int			RealMenuNo;				// Menu_no

	//�����̾� �������� ���� 2008.10
	BOOL		bPremChargeAvailable;		//���� ���� �ڰ��� �ִ°�?
	BOOL		bPremChargeSuccess;		//�������� Ƚ�� ��� ���� ����
	int		MChargeCnt;				//�������� �� ����Ƚ��
	int		DChargeCnt;				//�������� �� ����Ƚ��


}PREMINFO;

//����Ƚ�� ����
#define MAX_KICKOUT_COUNT_PREM_DIAMOND	(8)		//���̾Ƹ�� ȸ���� ����Ƚ�� 
#define MAX_KICKOUT_COUNT_SUPER			(4)		//���۱��� ������ ����Ƚ�� 
#define MAX_KICKOUT_COUNT_NORMAL		(2)		//�Ϲݱ��� ������ ����Ƚ�� 

//�����ð����� ����(��)
#ifdef _DEBUG
#define MAX_OBSERVE_TIME (60*3)	//3�� (�����)
#else
#define MAX_OBSERVE_TIME (60*15)	//15�� 
#endif // _DEBUG

/////////////////////////////////////////////////////////////////////////
enum STATE_JACKPOT		//�����̽ý���
{
		STATE_JACKPOT_NULL,				
		STATE_JACKPOT_IDLE,				//����  ���������� ����, ���� �ð��� �ƴ�
		STATE_JACKPOT_START,			//���� ���� ���� ����		
		STATE_JACKPOT_END,				//���� ���� ���� ����
		STATE_JACKPOT_ANNOUNCE,			//���̷� ��ǥ
};

enum STATE_JACKPOTROOM		//�����̽ý���
{
	STATE_JACKPOTROOM_NULL=0,			//���� ��ƴ� 
	STATE_JACKPOTROOM_HALFCOUNTACTIVE,	//���� ���� ī��Ʈ ���̴�. 
	STATE_JACKPOTROOM_NOMINATED,	//���� �ĺ���
	STATE_JACKPOTROOM_WIN,			//���� ��÷��		
};

//������� �ٷ�����
enum ERR_ENTERROOM
{
	ERR_ENTERROOM_OK,		 		//�� ���尡��
	ERR_ENTERROOM_INVALID, 				// �� ������ �߸�
	ERR_ENTERROOM_ALREADY_EXIST, 			//�̹� ��ȿ� �ִ�. 
	ERR_ENTERROOM_WRONG_PASSWORD, 		// ��� Ʋ��
	ERR_ENTERROOM_WRONG_USERID,			// ��û�� ���̵� �߸���
	ERR_ENTERROOM_MONEYLIMIT,  			//�Ӵ� ���� 
	ERR_ENTERROOM_INVALID_LOCATION, 		//������ �ƴ� 
	ERR_ENTERROOM_LACK_OF_MONEY,		 	//���� ���ڶ��. (�õ�Ӵ� ���� ����)		
	ERR_ENTERROOM_ROOM_DESTROY_RESERVATION, 		//�� �ı� ���� 
	ERR_ENTERROOM_BANNED,				// �Ϲݹ� �߹���ǥ ������ 
	ERR_ENTERROOM_SAME_IP,				//���� IP
	ERR_ENTERROOM_FULL,				// Ǯ��
	ERR_ENTERROOM_PLAYMONEYLIMIT, // ��޺���[���κ��谳��]
	ERR_ENTERROOM_FULL_OBSERVERLIMIT,	// �����ο��� ���������
	ERR_ENTERROOM_INVALID_MAXROOM_CONDITION,
	ERR_ENTERROOM_PLAYTIMEOVER,			// �� �÷��� ���ѿ� �ɸ�
	ERR_ENTERROOM_DAILYLOSS_LIMIT,
	ERR_ENTERROOM_TOURNAMENT_NOT_REGISTED,
	ERR_ENTERROOM_RD_MORNING_BET_BANNED, // �ο�ٵ��� ��ħ���÷������� ��û
};


////////////////////////////////////////////////////////////////////////
//������� �ٷ�����
// ������� �ٷ����� ���� �׸�

enum ID_QUICKJOIN_ITEM
{
	//-----------------------
	ID_QUICKJOIN_ITEM_MONEY,			// �����Ӵ�	
	ID_QUICKJOIN_ITEM_GAMEMODE,			// ���Ӹ��
	ID_QUICKJOIN_ITEM_GAMERULE,
	ID_QUICKJOIN_ITEM_BETRULE,			// ���÷�
	ID_QUICKJOIN_ITEM_SEEDMONEY,		// �õ�Ӵ�	
	//-----------------------
	MAX_QUICKJOIN_ITEM,
};

//�� �����׸� �ʿ��� ���� ���� 
struct QJData_Money
{
	INT64 MoneyMin, MoneyMax;
};
struct QJData_Game
{
	char idxGame;
};
struct QJData_GameMode
{
	char FormKind;
};
struct QJData_BetRule
{
	int nAutoBetKind;
};
struct QJData_SeedMoney
{
	int idxSeedMoney;
};
struct QJData_Sex
{
	char bMale;
};
struct QJData_Age
{
	int AgeMin, AgeMax;
};

struct QJData_GameRule
{
	int nGameRule;
};

//�� �����׸� ����ü
typedef union{
	QJData_Money		data_money;
	QJData_Game			data_game;
	QJData_GameMode		data_mode;
	QJData_BetRule		data_betrule;
	QJData_SeedMoney	data_seedmoney;
	QJData_Sex			data_sex;
	QJData_Age			data_age;
	QJData_GameRule     data_gamerule;
}UNION_QJDATA;

//data
struct QJData
{
	QJData() 
	{
		m_nPriority=0;
		m_IDQJ=-1;
		ZeroMemory(&union_data, sizeof(UNION_QJDATA));
	}
	char m_IDQJ;	//�˻����� �׸��� ����(ID_QUICKJOIN_ITEM)
	char m_nPriority; //���� �켱���� , 0�� ��� OFF
	UNION_QJDATA union_data;

	BOOL IsValidData()	//���� ������ ���Ἲ �˻� .
	{
		if (m_IDQJ<0 || m_IDQJ>=MAX_QUICKJOIN_ITEM) return FALSE;
		if (m_nPriority<=0 || m_nPriority>MAX_QUICKJOIN_ITEM) return FALSE;
		return TRUE;
	}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////                     ������ ������ ����ü
//////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
// ���Ӽ��� ���� ����
typedef struct
{
	char	WorkState;			// ���� ���� ����(0:���� 1:������ 2:������)
	char	Reserved[3];
} Ms_ServerStatus;

////////////////////////////////////////////////////////////////////////////////////////
// ���Ӽ��� ����
typedef struct
{
	int		SNum;				// ���� �ν� ��ȣ(�α��� ���� ��ȣ)

	short	GameCode;			// ���� �ڵ�
	short	ServerCode;			// ���� �ڵ�
	char	ServerName[20];		// ���� �̸�
	char	IP[20];				// ���� IP
	int		Port;				// ���� ��Ʈ ��ȣ
	int		TotChan;			// ������ ���� �� ä�� ����
	int		TotUser;			// ������ ������ ���� ��
	Ms_ServerStatus	Status;		// ���Ӽ��� ���� ����
} Ms_ServerInfo;

////////////////////////////////////////////////////////////////////////////////////////
// ���� ������ ���� ����
typedef struct
{
	char	ID[20];				// ����� ID
	char    NickName[NICK_LEN];		// �г��Ӽ��� 
	short	UNum;				// ���� ��ȣ
	short	GroupNo;			// ������ �׷� ��ȣ
	short	ChanNo;				// ������ ���� ä�� ��ȣ
	short	SNum;				// ������ ���� ��ȣ
	short	LocChanNo;			// ���� ä�� ��ȣ
	char	Reserved[2];
} Ms_UserInfo;

////////////////////////////////////////////////////////////////////////////////////////
// ����ä�� ����
typedef struct
{
	short	GameCode;			// ���� �ڵ� 
	short	GroupNo;			// �׷� ��ȣ
	short	ChanNo;				// ä�� ��ȣ(���յ� ä�� ��ȣ)
	short	LocChanNo;			// ���� ���� ä�� ��ȣ(���� ���� ��ü�� ä�� ��ȣ)
	short	MaxUser;			// �ִ� ���� �ο�
	short	NowUser;			// ���� �ο�
	short	ResUser;			// ���� �������� �ο�(������ �������� �Ǵ�)
	char	WorkState;			// ä�� ����(0:���� 1:������ 2:������)
	char	BanPrivate;			// ä�� �������������� (0:����������, 1:������ �Ұ�)   //[��Ŀ�� ������� ����ä���۾�] 2007.02.13
	char    bBanUseChan;		//[2009.07.31] ä�� ��� ����
	char	bVSMode;			// VSMode (0:�Ϲ� 1:VSMode)
	char	JackpotType;		// ���� Ÿ��  (0:���� 1:Half���� 2: Royal Half����)
	char	Reserved[3];
} Ms_ChannelInfo;

////////////////////////////////////////////////////////////////////////////////////////
// ������Ʈ�� ����ä�� ����
typedef struct
{
	short	GameCode;			// ���� �ڵ�
	short	GroupNo;			// �׷� ��ȣ
	short	ChanNo;				// ä�� ��ȣ(���յ� ä�� ��ȣ)
	short	NowUser;			// ���� �ο�
	short	ResUser;			// ���� �������� �ο�(������ �������� �Ǵ�)
	char	WorkState;			// ä�� ����(0:���� 1:������ 2:������)
	char	BanPrivate;			// ä�� �������������� (0:����������, 1:������ �Ұ�)   //[��Ŀ�� ������� ����ä���۾�] 2007.02.13
	char    bBanUseChan;		//[2009.07.31] ä�� ��� ����
	char	bVSMode;			// VSMode (0:�Ϲ� 1:VSMode)
	char	JackpotType;		// ���� Ÿ��  (0:���� 1:Half���� 2: Royal Half����)
	char	Reserved[3];
} Ms_SmallChanInfo;

////////////////////////////////////////////////////////////////////////////////////////
// ä�� ������Ʈ ����
typedef struct
{
	int		TotNewChan;			// ���ο� ä�� ���� ����
	int		TotModChan;			// ������ ä�� ���� ����
	int		TotDelChan;			// ������ ä�� ���� ����
} Ms_ChanUpdateInfo;

////////////////////////////////////////////////////////////////////////////////////////
// ���Ӽ��� �α��� ����
typedef struct
{
	char	Account[20];
	char	Password[20];
	Ms_ServerInfo  SvInfo;
} Ms_ServerLoginInfo;

// ���Ӽ��� �α��� ����
typedef struct
{
	char szAdminIP[24];			// �������� ���� IP����
	DWORD	dwIPMask;		// IP Mask����
} Ms_GroupInspectionAdminIPInfo;

////////////////////////////////////////////////////////////////////////////////////////
// ������ ���� ���� ����
typedef struct
{
	BOOL	bAllGroupShow;			// Ŭ���̾�Ʈ���� ��� �׷��� ä���� ������ ���ΰ�?
	BOOL	bAccessDeny;			// ���� �ź����ΰ�?
	BOOL	bUseSessionManagement;	// ���� ���� ����� ����ϰ� �ִ� �����ΰ�?
	int		TotGroup;				// ��ȿ�� �׷� ����
} Ms_MasterStatus;

////////////////////////////////////////////////////////////////////////////////////////
// �׷� ����
typedef struct
{
	short	GroupNo;				// �׷� ��ȣ
	short	TotLinkGroup;			// ��ũ �׷� ����
	BYTE	LinkGroup[20];			// ��ũ �׷� �迭
	char	GroupName[24];			// �׷� �̸�
	char	cGroupState;				// �׷� ����(0: ����, 1: ������)
} Ms_GroupInfo;

////////////////////////////////////////////////////////////////////////////////////////
// ����� ä�� ���� ����
typedef struct
{
	char		Kind;			// ������ ����(1:ä�� ���� ����, 2:ä�� ���� ����, 3:ä�� ���� ����)
	char		Reserved[3];
	UINT		SeqNo;			// �νĿ� ������ ��ȣ
	short		ServerTotUser;	// ���Ӽ��� ��ü�� ������
	short		ChanTotUser;	// ������ ä���� ������
	Ms_UserInfo Ui;				// ���� ����
} Ms_UserChangeInfo;

////////////////////////////////////////////////////////////////////////////////////////
// ����� ���� ���� ���� ����
typedef struct
{
	short	Code;			// �ڵ�
	short	UNum;			// ���� ��ȣ
	char	ID[20];			// ���� ID 
	BOOL	bDontSaveDB;	// ���� ����� DB����� ��ҽ�ų ���ΰ�?
} Ms_DisconnectUserInfo;

////////////////////////////////////////////////////////////////////////////////////////
// ����� ���� üũ ���� ����
typedef struct
{
	short	Code;			// �ڵ�
	short	UNum;			// ���� ��ȣ
	char	ID[20];			// ���� ID 
} Ms_CheckUserInfo;

////////////////////////////////////////////////////////////////////////////////////////
// �ٸ� ������ ä�� ���� ��û ����
typedef struct
{
	short	FromSNum;		// �̵��� ��û�� ���� ��ȣ
	short	TarGroupNo;		// �̵��ϰ����ϴ� �׷� ��ȣ
	short	TarChanNo;		// �̵��ϰ����ϴ� ä�� ��ȣ
	short	TotGameItem;	// ����ڰ� �����ϰ� �ִ� ������ ����
} Ms_AskMoveChan;

////////////////////////////////////////////////////////////////////////////////////////
// �ٸ� ������ ä�� ���� ��û ��� ����
typedef struct
{
	short				Code;			// ��� �ڵ�(0���� ������ ���� �Ұ�)
	char				Reserved[2];
	short				ToSNum;			// �̵� ��û�� �ߴ� ���� ��ȣ
	short				ToUNum;			// �̵� ��û�� �ߴ� ���� ��ȣ
	char				WaitIP[20];		// �̵��� ������ IP
	int					WaitPort;		// �̵��� ������ ��Ʈ ��ȣ
	DWORD				WaitPass;		// ���� ��� �н� ��ȣ
	Ms_UserChangeInfo	UCInfo;			// ������ ä�� ���� ���� 
} Ms_ResultMoveChan;

////////////////////////////////////////////////////////////////////////////////////////
// ������ ���� ���� ����
typedef struct
{
	char	IP[20];				// ���� IP
	int		Port;				// ���� ��Ʈ ��ȣ
	UINT	SeqNo;				// ���� ������ ��ȣ
	short	GroupNo;			// �׷� ��ȣ
	short	ChanNo;				// ���� ä�� ��ȣ
	short	SNum;				// ���� ��ȣ
	short	LocChanNo;			// ���� ä�� ��ȣ
} Ms_DestServerInfo;

////////////////////////////////////////////////////////////////////////////////////////
// �ٸ� ������ ���� ���� ��û ����
typedef struct
{
	short	FromSNum;		// �̵��� ��û�� ���� ��ȣ
	short	TarGameCode;		// �̵��ϰ����ϴ� �׷� ��ȣ
	short	TarGroupNo;		// �̵��ϰ����ϴ� �׷� ��ȣ
	short	TarChanNo;		// �̵��ϰ����ϴ� ä�� ��ȣ
	short	TotGameItem;	// ����ڰ� �����ϰ� �ִ� ������ ����
} Ms_AskMoveGame;

////////////////////////////////////////////////////////////////////////////////////////
// �ٸ� ������ ���� ���� ��û ��� ����
typedef struct
{
	short				Code;			// ��� �ڵ�(0���� ������ ���� �Ұ�)
	char				Reserved[2];
	int					nGameCode;		// �̵� ��û�� �ߴ� �����ڵ�
	short				ToSNum;			// �̵� ��û�� �ߴ� ���� ��ȣ
	short				ToUNum;			// �̵� ��û�� �ߴ� ���� ��ȣ
	char				WaitIP[20];		// �̵��� ������ IP
	int					WaitPort;		// �̵��� ������ ��Ʈ ��ȣ
	DWORD				WaitPass;		// ���� ��� �н� ��ȣ
	Ms_UserChangeInfo	UCInfo;			// ������ ä�� ���� ���� 
} Ms_ResultMoveGame;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////

typedef struct
{
	int        Index;		// ��������Ȯ���ϴ� �ε���
	int        NO;			// ����ũ �ѹ�
	int        Code;		// ������ �ڵ�
	char       PresID[ID_LEN];	// ������ ���
	short      UseDay;		// ��� ������ �Ⱓ(��)
	short      UseNum;		// ��� ������ ȸ��
	short	   FillNum;		// Ư�� ���� ��� Ƚ�� (õ�Ϲ��� ������)
	SYSTEMTIME StartDate;	// ��� ���� �ð�
	short      Use;			// ��� ����(�Ǵ� ����� ȸ��)
	char       Reserved[2];
} GAMEITEM;

typedef struct
{
	BOOL	  bLogable;  // �α��� ���ɿ���
	int       nResult;   // -1:����, 0:����, 1:DB���¿���, 2:�����۾��ų� ��밡���� �����۾���, 3:�������� �ְ� ������� �ʾҴ�
	int       nNum;      // ������ ������ ����
	GAMEITEM  Item[MAX_ITEM_NUM];

} GAMEITEM_LIST;


// Ư�� ���� �����ۿ� ����
struct REWARD_GAMEITEM_INFO
{
	// reward item db 08.02.27
	UINT		dbno;			// reward item db 
	char		bNeedUpdating;	// ����� DB ������Ʈ ����

	short		ItemCode;		// ������ �ڵ�
	short		GameCode;		// ���� �ڵ�
	SYSTEMTIME	ItemTime;		// ������ �ο����� �ð�
	SYSTEMTIME	TimeStamp;		// ������ �Ѱ� �ð�	( Invalid �ϴٸ� ( ��� 0�� ) ���� ��û���� �ʾ���. )
	SYSTEMTIME	TimeExpire;		// ������ ����ð�(Ŭ���̾�Ʈ ���Ῡ�ο� ������� ���øǿ� ������ �ð��̳��� �����Ҽ� �ֵ���)
};
#define MAX_REWARD_ITEM		30	// ����� ���� �ʿ��ϴٸ� ���� ���ڸ� �÷��� ����ϱ�...
typedef struct 
{
	int						nResult;
	int						nNum;
	REWARD_GAMEITEM_INFO	Item[MAX_REWARD_ITEM];

	// reward item db 08.02.27
	REWARD_GAMEITEM_INFO *GetRewardItem(int itemcode, SYSTEMTIME time)
	{
		for(int i=0;i < nNum; i++)
		{
			if (Item[i].ItemCode == itemcode &&	memcmp(&Item[i].ItemTime, &time, sizeof(SYSTEMTIME) ) == 0 )
				return &Item[i];
		}
		return NULL;
	}
}REWARDITEM_LIST;


// ���� ������ ����Ʈ ����(Ŭ���̾�Ʈ �۽ſ�)
typedef struct
{
	int			Kind;		// ����(0:������ ����Ʈ ���� 1:���� ������ ���� ����Ʈ ���� 100:������ ����Ʈ ���� ����)
	int			ErrorCode;	// ���� �ڵ�
	int			TotNum;		// �������� ����
	SYSTEMTIME	ServerTime;	// ������ ���� �ð�
	int			StrMsgLen1;	// ���ڿ� ���� 1 (������ ��� ������ ����� ���ڿ�)
	int			StrMsgLen2;	// ���ڿ� ���� 2 (������ ���� ���ڿ�)
} GAMEITEM_INFO;

// ���� ������ ���� ����
typedef struct
{
	int  Kind;			// ����
	char ItemName[256];	// ������ �̸�
	int  ItemCode;		// ������ �ڵ�
	int  StrMsgLen;		// ���� ���ڿ� ����
} BUYGAMEITEMINFO;

// ���Ӿ����� ��� ����(������ �ޱ� ��� ���)
typedef struct
{
	char		ID[ID_LEN];			// ������ ����� ���̵�
	GAMEITEM	Item;			// �����(Ȥ�� �����) �������� ����
	BOOL		bUseError;		// ������ ����� ������ �߻��Ͽ��°�?
	BOOL		bMessage;		// �޽����� �����ϴ°�?
	char		StrMsg[256];	// �޽��� ��Ʈ��
} USEGAMEITEMINFO;

// ������ ���� ��ȯ ����ü
struct ItemState
{
	int		  iState;		// 0: �߸��� ������ 1:��밡���� ������ 2:������� ������ 3:����� �� ���� ������(�پ� ������)
	BOOL	  bNowUsing;	// ���� ������ΰ�?
	COleDateTimeSpan LeftTime;		// ���� �ð�
	COleDateTime	  EndDate;		// �����Ͻ�
};


// �������� ���¸� ������ ���� �Լ�
ItemState GameItem_GetItemState(GAMEITEM &item, SYSTEMTIME* pCurTime=NULL);

// �������� ������ �˾Ƴ��� ���� �Լ�(�������� �����ڵ带 ���� �ڵ�� ���)
int GameItem_GetItemKind(int itemcode);

// Ư�� ������ �������� ��������� �˾Ƴ�(�������� �����ϴ� ��� pExist�� �Ѿ��, ����/Ŭ���̾�Ʈ ����)
BOOL GameItem_IsUsingItem(GAMEITEM_LIST &itemlist, int itemcode, BOOL *pExist, SYSTEMTIME* pCurTime=NULL);

// �������� �̸��� ������ ���� �Լ�
CString GameItem_GetItemName(GAMEITEM& item);
CString GameItem_GetItemName(int code);

// �������� ��� ������ ������ ���� �Լ�
CString GameItem_GetItemHelp(int code);

// ������ �̹����� �ε����� ������ ���� �Լ�(Ŭ���̾�Ʈ�� �̹��� ����Ʈ���� ���)
int GameItem_GetItemImageIndex(int code);

//////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////
int GetMoneyGrade(INT64 Money );
CString GetGradeName(int lv);
INT64 GetGradeToUserMoney(int grade);
int GetWinPro(int winnum, int loosenum , int drawnum);
int GetWinPro(int winnum, int loosenum);

//����->�ѱ� ��ȯ 
CString NumberToOrientalString( INT64 number );

//2008.12 ��Ŀ�� ����
/////////////////////////////////////////////////////////////////////
//���� ��Ŀ�Ӵ� ������ ���� 
struct RECORD
{	
	UINT WinNum, LoseNum, FoldNum, RSNum, SFNum, FCNum, AllinCnt, uHoola, uDaebbang, uSobbang; // �Ƕ�, �뻧, �һ� �߰�
	int TournamentRankedCount, TournamentTicketCount;
	
	RECORD operator -(const RECORD &gr)
	{
		RECORD ret;
		ret.WinNum = WinNum - gr.WinNum;
		ret.LoseNum = LoseNum - gr.LoseNum;
		ret.FoldNum = FoldNum - gr.FoldNum;
		ret.RSNum = RSNum - gr.RSNum;
		ret.SFNum = SFNum - gr.SFNum;
		ret.FCNum = FCNum - gr.FCNum;
		ret.AllinCnt = AllinCnt - gr.AllinCnt;
		// �Ƕ�����
		ret.uHoola    = uHoola - gr.uHoola;
		ret.uDaebbang = uDaebbang - gr.uDaebbang;
		ret.uSobbang  = uSobbang  - gr.uSobbang;

		ret.TournamentRankedCount = TournamentRankedCount;
		ret.TournamentTicketCount = TournamentTicketCount;

		return ret;
	}

	RECORD operator +(const RECORD &gr)
	{
		RECORD ret;
		ret.WinNum = WinNum + gr.WinNum;
		ret.LoseNum = LoseNum + gr.LoseNum;
		ret.FoldNum = FoldNum + gr.FoldNum;
		ret.RSNum = RSNum + gr.RSNum;
		ret.SFNum = SFNum + gr.SFNum;
		ret.FCNum = FCNum + gr.FCNum;
		ret.AllinCnt = AllinCnt + gr.AllinCnt;
		// �Ƕ�����
		ret.uHoola    = uHoola + gr.uHoola;
		ret.uDaebbang = uDaebbang + gr.uDaebbang;
		ret.uSobbang  = uSobbang  + gr.uSobbang;

		ret.TournamentRankedCount = TournamentRankedCount;
		ret.TournamentTicketCount = TournamentTicketCount;

		return ret;
	}

	bool operator !=(const RECORD &gr)
	{
		return ( (WinNum != gr.WinNum)	 || 
				(LoseNum != gr.LoseNum) || 
				(FoldNum != gr.FoldNum) /*||
				(RSNum != gr.RSNum) ||
				(SFNum != gr.SFNum) ||
				(FCNum != gr.FCNum) ||
				(AllinCnt != gr.AllinCnt) */
				);		
	}

	BOOL IsZero()
	{
		return ( WinNum==0 && LoseNum==0 && FoldNum==0 && RSNum==0 && SFNum==0 && FCNum==0 && AllinCnt==0 
				&& 0 == uHoola
				&& 0 == uDaebbang
				&& 0 == uSobbang );
	}
	
};



// 2008.12 ��Ŀ�� ���� 
struct ALLGAMERECORD
{	

	// ���� ����� ���� ���� 

	//�ִ� �����Ӵ�
	INT64 PMoneyRecord;	
	//�ִ� ȹ��Ӵ�
	INT64 WinPotRecord[MAX_GAMERECORD][MAX_GAME];

	//���� ī��Ʈ ��� ���� 
	RECORD Rec[MAX_GAMERECORD][MAX_GAME];
	

	// ------------------------�Լ�	----------------------------
	void ResetForGameDB()
	{
		//����DB�� �ִ� �����鸸 �����Ѵ�. (PMoneyRecord�� ������ ���� ����)
		ZeroMemory(WinPotRecord, sizeof(WinPotRecord));
		ZeroMemory(Rec, sizeof(Rec));
	}
	void Reset()
	{
		PMoneyRecord=0;
		ZeroMemory(WinPotRecord, sizeof(WinPotRecord));
		ZeroMemory(Rec, sizeof(Rec));

	}

	ALLGAMERECORD operator -(const ALLGAMERECORD &gr)
	{
		//�� �Լ���  DB�� �������� �α׾ƿ� �������� �α��� ������ ���� �� ���̸�ŭ ���� ���� ����Ѵ�.
		//(����) �ִ�ȹ��Ӵϴ� ���� ����  �ʰ� ����� ���� ���� 0�̾ƴ� ��ϰ��� ��ȯ�Ѵ�.
		ALLGAMERECORD ret = *this;
		for(int r=0; r<MAX_GAMERECORD; r++) 
		{			
			for(int g=0; g<MAX_GAME; g++) 
			{		
				ret.Rec[r][g] = Rec[r][g] - gr.Rec[r][g];

				//�ִ�ȹ��Ӵϴ� record��  break ���� �� �� ����Ѵ�. �ƴҶ��� 0���� ��ȯ
				if (WinPotRecord[r][g] > gr.WinPotRecord[r][g] )
				{
					ret.WinPotRecord[r][g] = WinPotRecord[r][g];
				}
				else
				{
					ret.WinPotRecord[r][g] =0;
				}
			}
		}		

		return ret;
	}

	ALLGAMERECORD operator +(const ALLGAMERECORD &gr)
	{
		//�� �Լ��� �Ϲ� ������ ��� ������� ������ ��� ���� ������ ��ȯ�ϱ� ���� ���.
		//(����) �ִ�ȹ��Ӵϴ� ���߿� �ִ밪�� ��ȯ. GOLD�� �Ϲ� �Ӵ��϶� ������ ��ȯ�ؾ� ���� �����ؾ� ��.
		ALLGAMERECORD ret = *this;
		
		ret.PMoneyRecord += gr.PMoneyRecord;
		for(int r=0; r<MAX_GAMERECORD; r++) 
		{			
			for(int g=0; g<MAX_GAME; g++) 
			{		
				ret.Rec[r][g] = Rec[r][g] + gr.Rec[r][g];
				ret.WinPotRecord[r][g] = max(WinPotRecord[r][g], gr.WinPotRecord[r][g]);
			}
		}		

		return ret;
	}

	bool operator !=(const ALLGAMERECORD &gr)
	{		
		for(int r=0; r<MAX_GAMERECORD; r++) 
		{			
			for(int g=0; g<MAX_GAME; g++) 
			{				
				if (Rec[r][g] != gr.Rec[r][g]) return true;
				//if (WinPotRecord[r][g] != gr.WinPotRecord[r][g]) return true;    ->(������ȭ���� �ִ�ȹ��Ӵϰ� ��ȭ�ɸ�����.����)
			}
		}
		
		return false;
	}
	//�ִ� �����Ӵ� ���
	INT64 GetPMoneyRecord() { return PMoneyRecord; }


	//�ִ� ȹ�� �Ӵ� ��� 
	INT64 RecordWinPot(IDX_GAMERECORD idxGR, IDX_GAME idxGame, INT64 WinPot) 
	{
		if (WinPotRecord[idxGR][idxGame] < WinPot)  
			WinPotRecord[idxGR][idxGame] = WinPot;
		return WinPotRecord[idxGR][idxGame];
	}
	void SetWinPotRecord(IDX_GAMERECORD idxGR, IDX_GAME idxGame, INT64 WinPot)	{		WinPotRecord[idxGR][idxGame] = WinPot;	}
	INT64 GetWinPotRecord(IDX_GAMERECORD idxGR, IDX_GAME idxGame)				{		return WinPotRecord[idxGR][idxGame];		}
	INT64 GetWinPotRecord(IDX_GAME idxGame)	///���Ӻ� �ְ����  ��
	{		
		INT64 MaxWinPot =0;
		for(int r=0; r<MAX_GAMERECORD; r++) 
		{
			if (MaxWinPot < WinPotRecord[r][idxGame])
			{
				MaxWinPot = WinPotRecord[r][idxGame];
			}			
		}

		return MaxWinPot;
	}


	//����ī��Ʈ 
	void SetAllinCnt(IDX_GAMERECORD idxGR, IDX_GAME idxGame, UINT num) { Rec[idxGR][idxGame].AllinCnt = num; }
	void IncAllinCnt(IDX_GAMERECORD idxGR, IDX_GAME idxGame) { Rec[idxGR][idxGame].AllinCnt++; }
	UINT GetAllinCnt(IDX_GAMERECORD idxGR, IDX_GAME idxGame) { return Rec[idxGR][idxGame].AllinCnt; }
	UINT GetAllinCnt(IDX_GAME idxGame)	///���Ӻ� ����ī��Ʈ ��
	{
		UINT ret=0;
		for(int r=0; r<MAX_GAMERECORD; r++) 
		{				
			ret+=Rec[r][idxGame].AllinCnt;
		}	
		return ret;
	}

	UINT GetAllinCnt()
	{
		UINT ret=0;
		for(int r=0; r<MAX_GAMERECORD; r++) 
		{			
			for(int g=0; g<MAX_GAME; g++) 
			{
				ret+=Rec[r][g].AllinCnt;
			}
		}
		return ret;
	}



	//��Ƽ��/��Ƽ��/��ī��
	void SetRSNum(IDX_GAMERECORD idxGR, IDX_GAME idxGame, UINT num)	{	Rec[idxGR][idxGame].RSNum = num;	}	
	void SetSFNum(IDX_GAMERECORD idxGR, IDX_GAME idxGame, UINT num)	{	Rec[idxGR][idxGame].SFNum = num;	}
	void SetFCNum(IDX_GAMERECORD idxGR, IDX_GAME idxGame, UINT num)	{	Rec[idxGR][idxGame].FCNum = num;	}
	
	void IncRS(IDX_GAMERECORD idxGR, IDX_GAME idxGame)	{		Rec[idxGR][idxGame].RSNum++;		}
	void IncSF(IDX_GAMERECORD idxGR, IDX_GAME idxGame)	{		Rec[idxGR][idxGame].SFNum++;		}
	void IncFC(IDX_GAMERECORD idxGR, IDX_GAME idxGame)	{		Rec[idxGR][idxGame].FCNum++;		}

	UINT GetRSNum(IDX_GAMERECORD idxGR, IDX_GAME idxGame)	{		return Rec[idxGR][idxGame].RSNum;		}
	UINT GetSFNum(IDX_GAMERECORD idxGR, IDX_GAME idxGame)	{		return Rec[idxGR][idxGame].SFNum;		}
	UINT GetFCNum(IDX_GAMERECORD idxGR, IDX_GAME idxGame)	{		return Rec[idxGR][idxGame].FCNum;		}

	// �Ƕ�����
	void SetHoolaNum( IDX_GAMERECORD idxGR, IDX_GAME idxGame, UINT uCnt )    { Rec[ idxGR ][ idxGame ].uHoola = uCnt; }	
	UINT GetHoolaNum( IDX_GAMERECORD idxGR, IDX_GAME idxGame )			     { return Rec[ idxGR ][ idxGame ].uHoola; }	
	void IncHoolaNum( IDX_GAMERECORD idxGR, IDX_GAME idxGame )				 { Rec[ idxGR ][ idxGame ].uHoola++;	  }

	void SetDaebbangNum( IDX_GAMERECORD idxGR, IDX_GAME idxGame, UINT uCnt ) { Rec[ idxGR ][ idxGame ].uDaebbang = uCnt; }	
	UINT GetDaebbangNum( IDX_GAMERECORD idxGR, IDX_GAME idxGame )			 { return Rec[ idxGR ][ idxGame ].uDaebbang; }	
	void IncDaebbangNum( IDX_GAMERECORD idxGR, IDX_GAME idxGame )				 { Rec[ idxGR ][ idxGame ].uDaebbang++;	 }

	void SetSobbangNum( IDX_GAMERECORD idxGR, IDX_GAME idxGame, UINT uCnt )  { Rec[ idxGR ][ idxGame ].uSobbang = uCnt;	}	
	UINT GetSobbangNum( IDX_GAMERECORD idxGR, IDX_GAME idxGame )			 { return Rec[ idxGR ][ idxGame ].uSobbang;	}
	void IncSobbangNum( IDX_GAMERECORD idxGR, IDX_GAME idxGame )			 { Rec[ idxGR ][ idxGame ].uSobbang++;	 }

	
	// ���Ӻ� �� ���� ��ϼ� 
	UINT GetRSNum(IDX_GAME idxGame)
	{
		UINT ret=0;
		for(int r=0; r<MAX_GAMERECORD; r++) 
		{				
			ret+=Rec[r][idxGame].RSNum;
		}	
		return ret;
	}	
	UINT GetSFNum(IDX_GAME idxGame)
	{
		UINT ret=0;
		for(int r=0; r<MAX_GAMERECORD; r++) 
		{				
			ret+=Rec[r][idxGame].SFNum;
		}	
		return ret;
	}
	UINT GetFCNum(IDX_GAME idxGame)
	{
		UINT ret=0;
		for(int r=0; r<MAX_GAMERECORD; r++) 
		{				
			ret+=Rec[r][idxGame].FCNum;
		}	
		return ret;
	}

	UINT GetRSNum() // �� rsf Ƚ�� 
	{
		UINT ret=0;
		for(int r=0; r<MAX_GAMERECORD; r++) 
		{			
			for(int g=0; g<MAX_GAME; g++) 
			{				
				ret+=Rec[r][g].RSNum;
			}
		}
		return ret;
	}	
	UINT GetSFNum()	// �� sf Ƚ�� 
	{
		UINT ret=0;
		for(int r=0; r<MAX_GAMERECORD; r++) 
		{			
			for(int g=0; g<MAX_GAME; g++) 
			{
				ret+=Rec[r][g].SFNum;
			}
		}
		return ret;
	}
	UINT GetFCNum()// �� fc Ƚ�� 
	{
		UINT ret=0;
		for(int r=0; r<MAX_GAMERECORD; r++) 
		{			
			for(int g=0; g<MAX_GAME; g++) 
			{
				ret+=Rec[r][g].FCNum;
			}
		}
		return ret;
	}

	//��, ��, ����
	UINT GetWinNum(IDX_GAMERECORD idxGR, IDX_GAME idxGame)				{	return Rec[idxGR][idxGame].WinNum;		}
	UINT GetLoseNum(IDX_GAMERECORD idxGR, IDX_GAME idxGame)				{	return Rec[idxGR][idxGame].LoseNum;	}
	UINT GetFoldNum(IDX_GAMERECORD idxGR, IDX_GAME idxGame)				{	return Rec[idxGR][idxGame].FoldNum;	}
	
	void SetWinNum(IDX_GAMERECORD idxGR, IDX_GAME idxGame, UINT num)		{	Rec[idxGR][idxGame].WinNum = num;		}
	void SetLoseNum(IDX_GAMERECORD idxGR, IDX_GAME idxGame, UINT num)	{	Rec[idxGR][idxGame].LoseNum = num;	}
	void SetFoldNum(IDX_GAMERECORD idxGR, IDX_GAME idxGame, UINT num)	{	Rec[idxGR][idxGame].FoldNum = num;	}	
	
	void IncWin(IDX_GAMERECORD idxGR, IDX_GAME idxGame)					{	Rec[idxGR][idxGame].WinNum++;			}
	void IncLose(IDX_GAMERECORD idxGR, IDX_GAME idxGame)					{	Rec[idxGR][idxGame].LoseNum++;		}
	void IncFold(IDX_GAMERECORD idxGR, IDX_GAME idxGame)					{	Rec[idxGR][idxGame].FoldNum++;		}	
	
	// ���Ӻ� �� ���� ��ϼ� 
	UINT GetWinNum(IDX_GAME idxGame)
	{
		UINT ret=0;
		for(int r=0; r<MAX_GAMERECORD; r++) 
		{				
			ret+=Rec[r][idxGame].WinNum;
		}	
		return ret;
	}	
	UINT GetLoseNum(IDX_GAME idxGame)
	{
		UINT ret=0;
		for(int r=0; r<MAX_GAMERECORD; r++) 
		{				
			ret+=Rec[r][idxGame].LoseNum;
		}	
		return ret;
	}
	UINT GetFoldNum(IDX_GAME idxGame)
	{
		UINT ret=0;
		for(int r=0; r<MAX_GAMERECORD; r++) 
		{				
			ret+=Rec[r][idxGame].FoldNum;
		}	
		return ret;
	}

	UINT GetWinNum() // �� �� Ƚ�� 
	{
		UINT ret=0;
		for(int r=0; r<MAX_GAMERECORD; r++) 
		{			
			for(int g=0; g<MAX_GAME; g++) 
			{
				ret+=Rec[r][g].WinNum;
			}
		}
		return ret;
	}	
	UINT GetLoseNum()	// �� �� Ƚ�� 
	{
		UINT ret=0;
		for(int r=0; r<MAX_GAMERECORD; r++) 
		{			
			for(int g=0; g<MAX_GAME; g++) 
			{
				ret+=Rec[r][g].LoseNum;
			}
		}
		return ret;
	}
	UINT GetFoldNum()// �� ���� Ƚ�� 
	{
		UINT ret=0;
		for(int r=0; r<MAX_GAMERECORD; r++) 
		{			
			for(int g=0; g<MAX_GAME; g++) 
			{
				ret+=Rec[r][g].FoldNum;
			}
		}
		return ret;
	}

	UINT GetTotalNum() const /// ��ü ��Ż Ƚ�� 
	{
		UINT ret=0;
		for(int r=0; r<MAX_GAMERECORD; r++) 
		{			
			for(int g=0; g<MAX_GAME; g++) 
			{
				ret+=GetTotalNum((IDX_GAMERECORD)r,(IDX_GAME)g);
			}
		}
		return ret;
	}

	UINT GetTotalNum(IDX_GAME idxGame) const // ��+��+����Ƚ��(���Ӻ�)
	{
		UINT ret=0;
		for(int r=0; r<MAX_GAMERECORD; r++) 
		{	
			ret+=GetTotalNum((IDX_GAMERECORD)r,idxGame);			
		}
		return ret;
	}

	UINT GetTotalNum(IDX_GAMERECORD idxGR, IDX_GAME idxGame) const // ��+��+���� Ƚ�� (���Ӻ�)
	{
		return (Rec[idxGR][idxGame].WinNum + Rec[idxGR][idxGame].LoseNum + Rec[idxGR][idxGame].FoldNum);
	}

	//�·� 
	int GetWinPro(IDX_GAMERECORD idxGR, IDX_GAME idxGame)
	{
		
		UINT winnum = GetWinNum(idxGR, idxGame);
		UINT totnum = winnum + GetLoseNum(idxGR, idxGame) + GetFoldNum(idxGR, idxGame);
		int winpro;
		if(totnum == 0) winpro = 0;
		else winpro = (int)(((double) winnum/ (double)totnum)*100);
		return winpro;
	}

	// ���Ӻ� �·� 
	int GetWinPro(IDX_GAME idxGame)
	{
		UINT winnum = GetWinNum(idxGame);
		UINT totnum = winnum + GetLoseNum(idxGame) + GetFoldNum( idxGame);
		int winpro;
		if(totnum == 0) winpro = 0;
		else winpro = (int)(((double) winnum/ (double)totnum)*100);
		return winpro;
	}

	int GetWinPro()
	{
		UINT winnum = GetWinNum();
		UINT totnum = winnum + GetLoseNum() + GetFoldNum();
		int winpro;
		if(totnum == 0) winpro = 0;
		else winpro = (int)(((double) winnum/ (double)totnum)*100);
		return winpro;		 
	}


	//SMALLUSERINFO�� �� �·� ��� 
	ARRAY_VALUE<float> GetSmallUIWinPer(GROUP_NO nGroupNum)
	{
		IDX_GAMERECORD idxGameRecord = g_GetGameRecordIndex(nGroupNum);
		ARRAY_VALUE<float> wp;

		for (int g = 0 ; g < MAX_GAME ; g++)
		{
			IDX_GAME idxGame = (IDX_GAME)g;
			wp.data[g] = (float)GetWinPro(idxGameRecord, idxGame);
		}
		return wp;
	}


	void SetTournametRankedCount(IDX_GAME idxGame, int ranked_count) { Rec[IDX_GAMERECORD_BASIC][idxGame].TournamentRankedCount = ranked_count; }
	int GetTournametRankedCount(IDX_GAME idxGame) const { return Rec[IDX_GAMERECORD_BASIC][idxGame].TournamentRankedCount; }
	int	IncTournamentRankedCount(IDX_GAME idxGame) { return ++Rec[IDX_GAMERECORD_BASIC][idxGame].TournamentRankedCount; }

	void SetTournametTicketCount(IDX_GAME idxGame, int Ticket_count) { Rec[IDX_GAMERECORD_BASIC][idxGame].TournamentTicketCount = Ticket_count; }
	int GetTournametTicketCount(IDX_GAME idxGame) const { return Rec[IDX_GAMERECORD_BASIC][idxGame].TournamentTicketCount; }
	int	IncTournamentTicketCount(IDX_GAME idxGame) { return ++Rec[IDX_GAMERECORD_BASIC][idxGame].TournamentTicketCount; }
	int	DecTournamentTicketCount(IDX_GAME idxGame) { return --Rec[IDX_GAMERECORD_BASIC][idxGame].TournamentTicketCount; }

	double GetTournamentRankedRatio(IDX_GAME idxGame) const
	{
		if ( 0 == Rec[IDX_GAMERECORD_BASIC][idxGame].TournamentTicketCount ) return 0;
		return 100 * ((double)Rec[IDX_GAMERECORD_BASIC][idxGame].TournamentRankedCount / (double)Rec[IDX_GAMERECORD_BASIC][idxGame].TournamentTicketCount);
	}


	BOOL IsAllZero(IDX_GAMERECORD idxGR, IDX_GAME idxGame)
	{
		return (Rec[idxGR][idxGame].IsZero() && GetWinPotRecord(idxGR, idxGame)==0);
	}
	BOOL IsAllZero(IDX_GAME idxGame)
	{
		for(int r=0; r<MAX_GAMERECORD; r++)
		{
			if (!IsAllZero((IDX_GAMERECORD)r,idxGame)) 
				return FALSE;		
		}
		return TRUE;
	}


	IDX_GAME GetMostPlayed_GameIndex(IDX_GAMERECORD idxGR)
	{
		//���� ������ ���� ���� ����
		IDX_GAME MostPlayedGame=(IDX_GAME)0;
		for(int i=1; i<MAX_GAME;i++)
		{
			if (GetTotalNum(idxGR, MostPlayedGame) < GetTotalNum(idxGR, (IDX_GAME)i))
				MostPlayedGame = (IDX_GAME)i;			
		}
		
		return MostPlayedGame;		
	}

	CString Log()
	{
		CString strLog;
		for(int r=0; r<MAX_GAMERECORD; r++)
		{
			for(int g=0; g<MAX_GAME; g++)
			{
				IDX_GAMERECORD ir=(IDX_GAMERECORD)r;
				IDX_GAME ig=(IDX_GAME)g;
				CString strTemp;
				strTemp.Format("%d_%s (w=%d, l=%d, f=%d, rs=%d, sf=%d, fc=%d, ac=%d, wp=%I64d)\n", 
					r,GetGameABB(g),
					GetWinNum(ir,ig),
					GetLoseNum(ir,ig),
					GetFoldNum(ir,ig),
					GetRSNum(ir,ig),
					GetSFNum(ir,ig),
					GetFCNum(ir,ig),
					GetAllinCnt(ir,ig),
					GetWinPotRecord(ir,ig));

				strLog+=strTemp;
			}
			
		}		
		return strLog;
	}

};


/////////////////////////////////////////////////////////////////////////////////////////
// �α��� ��û ����	[SSO �۾�] - �߰�
typedef struct
{
	char ID[ID_LEN];			// ID
	char Pass[16];			// ��ȣȭ�� �н������ 4��� Ŀ��
	char SockNameIP[20];
	char HostNameIP[20];
	int	 Port;
	int  AuthCookieLen;		// ���� ��Ű ����
	int  DataCookieLen;		// ������ ��Ű ����
	int  CPCookieLen;		// ������ ��Ű ����
	int  bForceLogin;		//2004.05.07 �������Ѿ� ����
	//gmarket�۾�
	int  SiteCode;			// ����Ʈ ��ȣ
	// ������ ���� ����
	UINT  SeqNo;			// ������ �νĿ� ������ ��ȣ
	short GroupNo;			// ������ �׷� ��ȣ
	short ChanNo;			// ������ ���� ä�� ��ȣ
	short SNum;				// ������ ���� ��ȣ
	short LocChanNo;		// ������ ���� ä�� ��ȣ
	
	int	  FamilyCompensation;	//�����ѵ� ����
	int GameCode;			// ������ ������ �ڵ� // 2008.12 ��Ŀ�� ����
	int  nEtcParam;			//< CYS 100525 > ���� bit ����
} ASK_LOGININFO;

typedef struct
{
	int GameCode;
	char IP[IP_LEN];		//--> Ŭ���̾�Ʈ ���ÿ��� ���� IP
	char MAC[MAC_LEN];		//--> MAC Address
	char GameVersion[10];	//--> Ŭ���̾�Ʈ �������
	char CountryCD[4];		//--> KR, US....
	char TimeZone[6];		//--> 00:09(KR)
	char OS[30];			//--> OS ����
	char OSCurrentBuild[8];	//--> OS CurrentBuild ����
	char Platform[6];		//--> 32, 64 bit ����
} FOBIS_Common;

/////////////////////////////////////////////////////
////////////////    ����� ����    //////////////////
/////////////////////////////////////////////////////

// �α��ν� ���� ����
typedef struct
{
	ALLGAMERECORD AllGameRecord[PlayableChipType::MAX];		// ��� ���� �и�. ���� ������ Ĩ Ÿ�� ���� ���� ���� ����. 2018-08-14
	INT64 pBackup;

} ORGINFO;


//////////////////////////////////////////////////////////////////////////
// ī�� �ý���. 2018.12.20
namespace CafeSystem
{
	namespace CafeKind
	{
		enum Type
		{
			NONE = 0,
			CAFE,				// ī��
			UNITED_CAFE,		// ����
		};
	}

	namespace JoinResult
	{
		enum Type
		{
			SUCCESS = 0,			// ���� ��û �Ϸ�
			ALREADY_MEMBER,			// �̹� �����. (���Ե� ����)
			EXCESS_DAY_LIMIT_COUNT,		// 1�� �ʴ� �Ǽ� ���� �ʰ�.			"1�� �ʴ� 10�� ������ �ʰ��Ͽ����ϴ�. ���� 00�� ���� �ʴ밡 �����մϴ�"
			ALREADY_ASKED,			// �̹� ���� ��û�� �� ����.
			FULL_MEMBER_COUNT,			// �̹� ����� �� á��.
			YOURE_NOT_CAFE_BOSS,		// ī������ �ƴ�.
		};
	}

	namespace InviteResult
	{
		enum Type
		{
			SUCCESS = 0,				// �ʴ� �Ϸ�
			NOT_EXIST_INVITE_MEMBER,	// �ʴ��� ����� ����.
			EXCESS_DAY_LIMIT_COUNT,		// 1�� �ʴ� �Ǽ� ���� �ʰ�.			"1�� �ʴ� 10�� ������ �ʰ��Ͽ����ϴ�. ���� 00�� ���� �ʴ밡 �����մϴ�"
			YOURE_NOT_CAFE_BOSS,		// ī������ �ƴ�.
			ALREADY_MEMBER,				// �̹� �����. (���Ե� ����)
			ALREADY_ASKED,			// �̹� ���� ��û�� �� ����.
			FULL_MEMBER_COUNT,			// �̹� ����� �� á��.				"ī����� �� �ִ� �����Դϴ�. �ʴ븦 �ϱ� ���ؼ��� ī����� Ż���ϰų� VIP����� ����Ǿ�� �մϴ�"
			NOW_UPDATING_VIP_GRADE,		// ���� VIP ��� �������� ���.		"VIP ��� ���� �߿��� ī����� �ʴ��� �� �����ϴ�.  XX�� ���� �ٽ� �õ����ֽñ� �ٶ��ϴ�"
		};
	}

	namespace Notice
	{
		enum Type
		{
			NONE,
			VIPGRADE_UP_CAFE_BOSS_MORETHAN_4_GRADE,					// "VIP ����� ����Ͽ� ī���� ������ ���� �Ͽ����ϴ�."
																	// "VIP����� �������� ���� ���� ī����� ������ �� �ֽ��ϴ�."
			VIPGRADE_UP_UNITEDCAFE_BOSS_MORETHAN_6_GRADE,			// "VIP ����� ����Ͽ� ���տ� �������� ���� �Ͽ����ϴ�."
																	// "VIP����� �������� ���� ���� ���տ��� ������ �� �ֽ��ϴ�."
			VIPGRADE_DOWN_CAFE_BOSS,								// "VIP ����� �϶��Ͽ� ī���� ������ ���� �Ͽ����ϴ�"
																	// "������ �ʰ��� ī����� �������ڰ� ������ �������� Ż��ó�� �˴ϴ�."
																	// "VIP ����� �������� ���� ���� ī����� ������ �� �ֽ��ϴ�"
			VIPGRADE_DOWN_UNITEDCAFE_BOSS,							// "VIP ����� �϶��Ͽ� ������ ������ ���� �Ͽ����ϴ�"
																	// "������ �ʰ��� ���տ��� �������ڰ� ������ �������� Ż��ó�� �˴ϴ�."
																	// "VIP ����� �������� ���� ���� ī������ ������ �� �ֽ��ϴ�"
			FIRED_CAFE_MEMBER,										// "ī���� VIP ����� �϶��Ͽ� �ڵ�Ż�� �Ǿ����ϴ�."
																	// "������ �ʰ��� ī����� �������ڰ� ������ �������� Ż��ó�� �˴ϴ�"
			FIRED_UNITEDCAFE_MEMBER,								// "������ VIP ����� �϶��Ͽ� �ڵ�Ż�� �Ǿ����ϴ�."
																	// "������ �ʰ��� ���տ��� �������ڰ� ������ �������� Ż��ó�� �˴ϴ�"
			MAX,
		};
	}

	// 0: ��ȸ�� 1: ��ȸ�� 2: ��ȸ�� 3: ī����, 4: ���տ�(ī���� ����) 5: ������ 
	namespace MemberGrade
	{
		enum Type
		{
			NONE = 0,
			CAFE_ASSOCIATE_MEMBER = 1,
			CAFE_MEMBER = 2,
			CAFE_BOSS = 3,
			UNITED_CAFE_MEMBER = 4,
			UNITED_CAFE_BOSS = 5,
		};
	}

	// ī�� ����
	struct CafeInfo
	{
		int cafeID_;				// ī�� ID. 0�� ��� invalid
		char cafeName_[NICK_LEN];	// ī�� �̸�. (���� ����� boss nickname)

		void Clear()
		{
			cafeID_ = 0;
			::ZeroMemory(cafeName_, sizeof(cafeName_));
		}
	};

	// ī��/���� ���� ����
	struct MemberInfo
	{
		char memberID_[ID_LEN];		// ��� ID
		MemberGrade::Type memberGrade_;			// 0: ��ȸ�� 1: ��ȸ�� 2: ��ȸ�� 3: ī����, 4: ���տ�(ī���� ����) 5: ������ 
		int globalRoomNum_;			// �������� ��� �� ��ȣ. �������� �ƴϸ� 0. ���չ��� ��쿡�� ���.

		void Clear()
		{
			::ZeroMemory(memberID_, sizeof(memberID_));
			memberGrade_ = MemberGrade::NONE;
			globalRoomNum_ = 0;
		}
	};
}	// namespace CafeSystem
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////////
// ����� ����
typedef struct
{
	int		nGameIndex;		//���Ӱ����ε���( 7��Ŀ:0, �ο�ٵ���: 1, ����Ŀ: 2, ����Ŀ: 3
	int		SiteCode;			//����Ʈ �ڵ� (�ݸ���(nmb):0 ������(gmt):10 ����(auc):20 )
	UINT64	llUserKey;			// IPS�� ��ϵ� UserKey;
	short	UNum;			// ���� ��ȣ - DB�� ����Ǵ� ������ �ƴ�	
	char	ID[ ID_LEN ];		 // ����� ���̵�
	char	UniqNo[ CN_LEN ];	 // ����� ����ũ ���̵� 
	char	NickName[ NICK_LEN ];// �г��Ӽ��� 
	char	Sex;				// ����(0:����, 1:����)
	int		nAge;				// ����(-1: �����, 0�̻�: ������ ����)
	char	cPosition;			// ���� ��ġ(WH_CHAN:ä�μ��� WH_LOBY: ���� WH_SHOP:���� WH_GAME:���ӹ�)
	int		nRoomNum;		// ���ȣ(���� ��ġ�� ���ӹ��϶��� ��ȿ)
	int		nLocalRoomNum;	// ���ü��� ���ȣ

	SYSTEMTIME	PremDate;			// �����̾� ���� �Ⱓ(������) [�����̾� �۾�]
	char			nPCRoomIPType;	// �����̾� PC�� IP ���� (������:1 �÷���:2)[�ű� PC�� �۾�]
	char			PremMenuNo;		//2004.05.07 �������Ѿ� ����

	char cPrevPremMenuNo;		// ���� �����̾�(31�� ���� ����� ������ �ִٸ� PremMenuNo�� ������.
	SYSTEMTIME	stPrevPremDate;		// ���� �����̾� ���� �Ⱓ(������)					[�����̾� �۾�]
	
	enum PREMIUM_CANCEL_CAUSE	
	{
		// ȸ���� ���� ����
		PREMIUM_CANCEL_CAUSE_MODIFY = 1,			// ����/����
		PREMIUM_CANCEL_CAUSE_NOTRECOGNITION = 2,	// ���� �̹ݿ� 
	};
	PREMIUM_CANCEL_CAUSE ePremCancelCause;

	INT64 llOverMoney;	// �ʰ��� 2010.08.24
	SYSTEMTIME tmOverMoneyUpdate;	// �ʰ��� ������ 2010.08.24
	INT64 llRevisionMoneyNotRecognition;	// �����Ӵ� - ���� �̹ݿ� 2010.08.24
	SYSTEMTIME tmRevisionMoneyUpdateNotRecognition;	// �����Ӵ� - ���� �̹ݿ� ������ 2010.08.24
	INT64 llRevisionMoneyModify;	// �����Ӵ� - ����/���� 2010.08.24
	SYSTEMTIME tmRevisionMoneyUpdateModify;	// �����Ӵ� - ����/���� ������ 2010.08.24

	INT64 llEarnedMoney;	// EMoney

	char		Reserved;					//����Ʈ ���� 
	char nIcon;             // ����������
	char cObserver;			// ### [ ������� ] ### 
	char AdminGrade;		//### [������ ��� �۾�] ###

	//���ӱ�� 
	ALLGAMERECORD AllGameRecord[PlayableChipType::MAX];		// // ��� ���� �и�. ���� ������ Ĩ Ÿ�� ���� ���� ���� ����. 2018-08-14

	short AvatarInfo[MAX_AVATARLAYER];	// �ƹ�Ÿ ����
	char Desc[62];								// �ϰ����� ��	

	//���κ���
	int			nAIN_InsuranceKind;			// ���� ���� ����
	INT64     nAIN_Insurance_Money;		// ���� �����

	char	nBadUser;	//�������� ���� 2006.2.3
	char	nBadIPUser;	//�ҷ� ������ ��������0 ~ 4 0�� ���� �������� ���� 2006.2.3

	char		DS_IDColor;					// ���� �Ƶ� Į��
	char		DS_Level;					// ���� ���� 
	// ���������� 2006.3.2
	short		DS_SupplyBenefit;			// ���� ���� ���������� ���� ����
	INT64		DS_GetMaxMoney;				// �ְ� ���� �Ӵ�
	INT64		DS_GetBetMaxMoney;			// �ְ� ȹ�� �Ӵ�	


	// ��޺���[���κ��谳��]
	POKERINFO	PI;							// ��Ŀ ����
	UINT		AllinCnt;					// ����ī��Ʈ 2006.1.9


	//���������� 2006.3.2
	int			nAllinChargeCnt;			// �������� ���� Ƚ��
	SYSTEMTIME	AllinChargeDate;			// �������� ������ ������ �ð�

	double		DS_Exp;						//���������� ����ġ	

	bool		bTournament;	
	bool		bIsLeagueIcon;	// ������ Group ������ Ȱ��ȭ ����
	USHORT		nLeagueLevel;	// ���� ���

	////////////////////////////Ŭ���ý��� 2017.12.21/////////////////////////////////////
	int ClubCode; //������ Ŭ��������ȣ
	int ClubMemberGrade; //Ŭ�������, ������, �Ϲ��̳�...
	int ClubBossLevel; //������ VIP ���
	char ClubName[CHAR_BUFFER_CLUB_NAME]; //������ Ŭ���̸�
	int ClubMyVIPLevel; //���� VIP ����̴�.
	////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// ī�� �ý���. 2018.12.20
	CafeSystem::CafeInfo cafeInfo_;						// ī�� ����
	CafeSystem::CafeInfo unitedCafeInfo_;				// ���� ����
	CafeSystem::MemberGrade::Type cafeMemberGrade_;		// 0: ��ȸ�� 1: ��ȸ�� 2: ��ȸ�� 3: ī����, 4: ���տ�(ī���� ����) 5: ������ 
	//////////////////////////////////////////////////////////////////////////

	FOBIS_Common FOBISCommon;

	void SetFOBIS(const FOBIS_Common* pFOBISCommon)
	{
		if(NULL == pFOBISCommon) { return; }

		memcpy_s(&FOBISCommon, sizeof(FOBIS_Common), pFOBISCommon, sizeof(FOBIS_Common));

		FOBISCommon.CountryCD[3] = '\0';
		FOBISCommon.GameVersion[9] = '\0';
		FOBISCommon.IP[23] = '\0';
		FOBISCommon.MAC[17] = '\0';
		FOBISCommon.OS[29] = '\0';
		FOBISCommon.OSCurrentBuild[7] = '\0';
		FOBISCommon.Platform[5] = '\0';
		FOBISCommon.TimeZone[5] = '\0'; 
	}

	/////////////////////////////////////////////////////////////////////////////
	//G_TFT
	INT64 ChangeMoney(INT64 ChangeMoney)
	{
		//ChnageMoney��ŭ ��ȭ ��Ű�� ������ ������ ���� ��ȯ�Ѵ�.
		const INT64 MAX_POKER_MONEY=10000000000000000; ///1���� ���� ���Ѵ�.
	
		if( MAX_POKER_MONEY-PMoney < ChangeMoney )				
		{
			PMoney = MAX_POKER_MONEY;
			nIcon = GetMoneyGrade(PMoney);
			return (MAX_POKER_MONEY-PMoney);	//���� ���� �Ӵ�
		}
		else if (PMoney+ChangeMoney <0)		//��ȭ �Ӵϸ� �������� �������Ǵ� ���
		{
#if defined(_ONLY_SERVER)
			GLOBALLOG_ERROR("UserInfo::ChangeMoney invalid ChangeMoney: PMoney(%I64d) + ChangeMoney(%I64d) < 0 (ID=%s, CN=%s)", 
				PMoney, ChangeMoney, ID, UniqNo);
#endif

			INT64 llActualChange = -PMoney;
			PMoney = 0;
			nIcon = GetMoneyGrade(PMoney);
			return llActualChange;
		}	
		
		PMoney += ChangeMoney;			

		nIcon = GetMoneyGrade(PMoney);

		if ( false == g_IsTournamentGameIndex((IDX_GAME)nGameIndex) )
			llRealPMoney = PMoney;

		ASSERT(PMoney>=0);
		return ChangeMoney;
	}
	INT64 SetMoney(INT64 Money)
	{
		if ( false == g_IsTournamentGameIndex((IDX_GAME)nGameIndex) )
		{
			llRealPMoney = 0;
			return ChangeRealPMoney(Money);
		}

		PMoney=0;
		return ChangeMoney(Money);
	}

	INT64  GetMoney() const	//(����Ӵ�)
	{
		return PMoney;
	}

	INT64  GetTotalMoney() const	//���� ���� �� �����Ӵ�
	{ 
		return PMoney;
	}

	INT64 GetInMoney() const 
	{
		return llInMoney;
	}

	INT64 SetInMoney(INT64 llMoney) 
	{
		llInMoney = 0;
		return AddInMoney(llMoney);
	}

	INT64 AddInMoney(INT64 llMoney) 
	{
		static const INT64 MAX_POKER_MONEY = 10000000000000000; ///1���� ���� ���Ѵ�.
		if (llInMoney + llMoney > MAX_POKER_MONEY)
		{
			llInMoney = MAX_POKER_MONEY;
			return (MAX_POKER_MONEY - llInMoney);	//���� ���� �Ӵ�
		}
		else if (llInMoney + llMoney < 0)		//��ȭ �Ӵϸ� �������� �������Ǵ� ���
		{
#if defined(_ONLY_SERVER)
			GLOBALLOG_ERROR("UserInfo::AddInMoney invalid AddInMoney: llInMoney(%I64d) + llMoney(%I64d) < 0 (ID=%s, CN=%s)", 
				llInMoney, llMoney, ID, UniqNo);
#endif

			INT64 llActualChange = -llInMoney;
			llInMoney = 0;
			return llActualChange;
		}

		llInMoney += llMoney;
		ASSERT(llInMoney >= 0);
		return llMoney;
	}

	INT64 GetSafeMoney() const 
	{
		return llSafeMoney;
	}

	INT64 SetSafeMoney(INT64 llMoney) 
	{
		llSafeMoney = 0;
		return AddSafeMoney(llMoney);
	}

	void SetRegDate(char* _szRegDate)
	{
		strncpy( szRegDate, _szRegDate, 20);
	}

	char* GetRegDate()
	{
		return szRegDate;
	}

	INT64 AddSafeMoney(INT64 llMoney) 
	{
		static const INT64 MAX_POKER_MONEY = 10000000000000000; ///1���� ���� ���Ѵ�.
		if (llSafeMoney + llMoney > MAX_POKER_MONEY)
		{
			llSafeMoney = MAX_POKER_MONEY;
			return (MAX_POKER_MONEY - llSafeMoney);	//���� ���� �Ӵ�
		}
		else if (llSafeMoney + llMoney < 0)		//��ȭ �Ӵϸ� �������� �������Ǵ� ���
		{
#if defined(_ONLY_SERVER)
			GLOBALLOG_ERROR("UserInfo::AddSafeMoney invalid AddInMoney: llSafeMoney(%I64d) + llMoney(%I64d) < 0 (ID=%s, CN=%s)", 
				llSafeMoney, llMoney, ID, UniqNo);
#endif

			INT64 llActualChange = -llSafeMoney;
			llSafeMoney = 0;
			return llActualChange;
		}	

		llSafeMoney += llMoney;
		ASSERT(llSafeMoney >= 0);
		return llMoney;
	}


	////////////////////////////////////////////////////
	// ��ʸ�Ʈ�� Ĩ ���
	////////////////////////////////////////////////////
	INT64 ChangeRealPMoney(INT64 change_money)
	{
		//change_money��ŭ ��ȭ ��Ű�� ������ ������ ���� ��ȯ�Ѵ�.
		const INT64 MAX_POKER_MONEY=10000000000000000; ///1���� ���� ���Ѵ�.

		if( MAX_POKER_MONEY-llRealPMoney < change_money )				
		{
			llRealPMoney = MAX_POKER_MONEY;
			nIcon = GetMoneyGrade(llRealPMoney);
			return (MAX_POKER_MONEY - llRealPMoney);	//���� ���� �Ӵ�
		}
		else if (llRealPMoney + change_money <0)		//��ȭ �Ӵϸ� �������� �������Ǵ� ���
		{
#if defined(_ONLY_SERVER)
			GLOBALLOG_ERROR("UserInfo::ChangeMoney invalid ChangeMoney: llRealPMoney(%I64d) + change_money(%I64d) < 0 (ID=%s, CN=%s)", 
				llRealPMoney, change_money, ID, UniqNo);
#endif

			INT64 llActualChange = -llRealPMoney;
			llRealPMoney = 0;
			nIcon = GetMoneyGrade(llRealPMoney);
			return llActualChange;
		}	

		llRealPMoney += change_money;			

		nIcon = GetMoneyGrade(llRealPMoney);

		if ( false == g_IsTournamentGameIndex((IDX_GAME)nGameIndex) )
			PMoney = llRealPMoney;

		ASSERT(llRealPMoney>=0);
		return change_money;
	}
	INT64 SetRealPMoney(INT64 Money)
	{
		if ( true == g_IsTournamentGameIndex((IDX_GAME)nGameIndex) )
		{
			llRealPMoney = 0;
			return ChangeRealPMoney(Money);
		}

		// ��ʸ�Ʈ�� �ƴϸ� llRealPMoney & PMoney �Ѵ� ������
		INT64 ret = SetMoney(Money);
		llRealPMoney = PMoney;
		return ret;
	}

	INT64  GetRealPMoney() const	//(����Ӵ�)
	{
		return llRealPMoney;
	}

	INT64  GetTotalRealPMoney() const	//���� ���� �� �����Ӵ�
	{ 
		return llRealPMoney;
	}

    // 2018�� 7�� 2��° ������Ʈ ���� - ���� ī�� ���� ��� �߰� 
	void SetCardOpen(bool bOpen)
	{
		bCardOpen = bOpen;
	}

	bool GetCardOpen()
	{
		return bCardOpen;
	}

	bool BTournamentGame() const { return bTournamentGame; }
	void BTournamentGame(bool val) { bTournamentGame = val; }
	////////////////////////////////////////////////////
	// ��ʸ�Ʈ�� Ĩ ���
	////////////////////////////////////////////////////

//#if defined (__ROUND_MODE__)
	INT64 GetRoundModeGameChip() const { return llRoundModeChip;}
	void  SetRoundModeGameChip(INT64 nChip)  { llRoundModeChip = nChip; SetInMoney(nChip);}
	void  AddRoundModeGameChip(INT64 nChip)  { llRoundModeChip =+ nChip;}
//#endif

	//////////////////////////////////////////////////////////////////////////
	// ��� ���� �и�. ���� ������ Ĩ Ÿ��. 2018-08-14
	int GetAllGameTotalNum() const
	{
		int nTotalNum = 0;

		size_t nRecordCount = _countof(AllGameRecord);
		for (size_t i = 0; i < nRecordCount; ++i)
		{
			nTotalNum += AllGameRecord[i].GetTotalNum();
		}

		return nTotalNum;
	}

	ALLGAMERECORD GetMergedAllGameRecords() const
	{
		ALLGAMERECORD allRecords;
		allRecords.Reset();

		size_t nRecordCount = _countof(AllGameRecord);
		for (size_t i = 0; i < nRecordCount; ++i)
		{
			allRecords = allRecords + AllGameRecord[i];
		}

		return allRecords;
	}

	template<size_t _Size>
	void CopyAllGameRecords(OUT ALLGAMERECORD (&destGameRec)[_Size])
	{
		size_t nCopySize = min(_Size, _countof(AllGameRecord));

		for (size_t i = 0; i < nCopySize; ++i)
		{
			destGameRec[i] = AllGameRecord[i];
		}
	}
	//////////////////////////////////////////////////////////////////////////

private:
	// MaxRoom. 2011.01.27
	INT64 PMoney;		// ��Ŀ �Ӵ� (��ʸ�Ʈ������ ��ʸ�Ʈ �Ӵ�)
	INT64 llInMoney;	// In-Money
	INT64 llSafeMoney;	// Safe-Money (PMoney - InMoney)

	INT64 llRealPMoney;	// ��Ŀ �Ӵ� (��ʸ�Ʈ���� ���� ��Ŀ �Ӵ� ������ ���)

	INT64 llTournamentChip;	// ��ʸ�Ʈ ���� �Ӵ�, ��ʸ�Ʈ���������� PMoney �� ������� �ʰ� llTournamentMoney �� ���
	bool bTournamentGame;	// ��ʸ�Ʈ �������� �ƴ��� üũ�ϴ� �÷���, true �� ��ʸ�Ʈ, false �� ��ʸ�Ʈ �ƴ�
//#if defined (__ROUND_MODE__)
	INT64 llRoundModeChip;	// ���� ���� Ĩ
	bool  bRoundMode;		//  ���� ��� üũ�� �濡 �����ϸ� true ��.
//#endif

	bool bCardOpen; // 2018�� 7�� 2��° ������Ʈ ���� - ���� ī�� ���� ��� �߰�
	char szRegDate[20]; // ���Գ�¥ // 2019�� 7�� 30�� �߰�
} USERINFO;


////////////////////////////���ε� �� ����ȭ���� ���� 2018.04.18/////////////////////////////////////	
typedef struct
{
	//���� �� ä���̵��� �����ͼ����� ���� �ѱ� ��Ÿ �����͸� ���⿡ �ִ´�.
	//�� ����ü�� �����ͼ����� ����ȭ �Ǿ�� ��
	INT64 Gold_RealChip;
	int Gold_RemainChip;
	INT64 Gold_OverChip;
	int Gold_OverChip_SavedMemberGrade;
	SYSTEMTIME Gold_OverChip_LastUpdateDate;

	////////////////////////////2018��06�� ���� 2018.06.01/////////////////////////////////////
	INT64 GoldBank_RealChip; //������� �����ݾ�
	int GoldBank_RemainChip; //������� ����Ĩ
	INT64 GoldBank_OverChip; //������� �����ȱݾ�
	///////////////////////////////////////////

	////////////////////////////2018��06�� ���� 2018.06.01/////////////////////////////////////
	int Gold_Insurance_Level; //��� ���� ���
	INT64 Gold_Insurance_Money; //��� ���� ������
	///////////////////////////////////////////

} ETC_MOVE_INFO;
///////////////////////////////////////


//�ڸ� ��Ŵ��
#define MAX_SEATKEEPER_TURN (5) //1ȸ ���� �ִ� �� �� ���°�?

#ifdef __TOURNAMENT__
  #define	IsAbleToUseSeatKeeper(UI) (false)
#else
  #define	IsAbleToUseSeatKeeper(UI) (CheckPremiumState(&UI) >= CODE_NEW_FAMILY_GOLD || UI.nPCRoomIPType > 0) //�йи� ����̻� Ȥ�� �ǽù游 ��밡���ϴ�.
#endif






//���� �ƹ�Ÿ ����Ƚ�� ��� ����ü 
enum COUNT_BASE
{ 
	UNCOUNTABLE = 0, 
	DAILY_COUNT, 
	MONTHLY_COUNT, 
	ANNUAL_COUNT, 
	PERMANENT_COUNT 
};

COUNT_BASE g_GetChanceItemCountBase(int ChaneItemCode);
typedef struct
{	
	int		ChanceItemCode;		//���� ������ �ڵ�
	int		BuyCnt;				//���� ������ ����Ƚ��
	SYSTEMTIME  LastBuyDate;	//���� ������ ������ ���Žð�
	
	int	GetBuyCount(COUNT_BASE CountBase) //�ð����ؿ� ���� ���� ��ȯ 
	{
		if (UNCOUNTABLE == CountBase ) return 0;
		if (PERMANENT_COUNT == CountBase) return BuyCnt;
		COleDateTime now = COleDateTime::GetCurrentTime();
				
		if (now.GetYear() != LastBuyDate.wYear) 		return 0;
		if (ANNUAL_COUNT == CountBase)	return  BuyCnt;				
		if (now.GetMonth() != LastBuyDate.wMonth)		return 0;
		if (MONTHLY_COUNT == CountBase)	return  BuyCnt;
		if (now.GetDay() != LastBuyDate.wDay)		return 0;
		
		return  BuyCnt;			
	}
} CHANCECNT;

// ������� �ƹ�Ÿ, �ºλ� 
const int MAX_CHANCE_AVATAR_COUNT=15;//<== ���� �ƹ�Ÿ �����۵� �� ���� Ƚ���� ���� �������� ���� ���� ���� ���ų� ���ƾ� �Ѵ�.

typedef struct 
{	
	CHANCECNT chancecnt[MAX_CHANCE_AVATAR_COUNT];

	void SetChanceCount(int ChanceItemCode, int BuyCount, COleDateTime *pLastBuyDate)
	{		
		if (g_GetChanceItemCountBase(ChanceItemCode)==UNCOUNTABLE) return;
				
		//������ �߰�
		CHANCECNT *pChanceCnt = GetChanceCount(0);
		if (pChanceCnt)
		{
			pChanceCnt->ChanceItemCode = ChanceItemCode;
			pChanceCnt->BuyCnt = BuyCount;
			pLastBuyDate->GetAsSystemTime(pChanceCnt->LastBuyDate);
		}
	}

	//���������� ����Ƚ���� ��ȯ�Ѵ�. 
	int GetChanceBuyCount(int ChanceItemCode)
	{			
		CHANCECNT *pChanceCnt = GetChanceCount(ChanceItemCode);
		if (pChanceCnt==NULL) return 0;
		return pChanceCnt->GetBuyCount(g_GetChanceItemCountBase(ChanceItemCode));		
	}

	CHANCECNT *GetChanceCount(int ChanceItemCode)
	{
		//�ش� �����������ڵ忡 ���� ����ī��Ʈ ����ü�� ã�� ��ȯ 
		for(int i=0; i<MAX_CHANCE_AVATAR_COUNT; i++)
		{
			if (chancecnt[i].ChanceItemCode == ChanceItemCode)
			{
				return &chancecnt[i];
			}
		}
		return NULL;
	}
	CHANCECNT *IncChanceCount(int ChanceItemCode)
	{
		COUNT_BASE countbase = g_GetChanceItemCountBase(ChanceItemCode);
		if (UNCOUNTABLE == countbase) return NULL;	

		CHANCECNT *pChanceCnt = GetChanceCount(ChanceItemCode);
		
		if (pChanceCnt==NULL)
		{
			//������ �߰�
			CHANCECNT *pChanceCnt2 = GetChanceCount(0);
			
			ASSERT(pChanceCnt2);//> ���� �ɸ��� MAX_CHANCE_AVATAR_COUNT �� ����� �������� �ʾҴ�..

			if (pChanceCnt2==NULL) return NULL;

			pChanceCnt2->ChanceItemCode = ChanceItemCode;
			pChanceCnt2->BuyCnt = 1;
			GetLocalTime(&pChanceCnt2->LastBuyDate);

			return pChanceCnt2;

		}
		else
		{
			pChanceCnt->BuyCnt = pChanceCnt->GetBuyCount(countbase)+1;
			GetLocalTime(&pChanceCnt->LastBuyDate);	
			return pChanceCnt;	
		}	
	}

	//���� 
	int nSCTodayBuyItemCnt; //��������� ������ ���� ���� ī��Ʈ 

} CHANCEITEMCNT;


////////////////////////////////////////////////////////////////////////////////////
//����ī����
//Ƚ�� �ð�����
struct GAMECOUNTER
{

	////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////
//	user definition - ī���� �߰��� ���� �ؾ� �� ���� 

typedef unsigned char COUNTTYPE;			// Ƚ�� ���� �� ���� (���⼭�� 0~254������ �����ִ�.)
#define ERR_COUNT (COUNTTYPE)(-1)			// ������ ��ȯ Ƚ�� (�Ʒ� ���� �����ø� ����)

	//DB�� ��ϵǴ� ī���� ���� KIND �� (�ߺ�����, ��������, �ѹ� ������ KIND ���� ����, �߰��� �� ��)
	//(����) KIND���� TinyInt �̹Ƿ� 255�� ���� �ʵ��� �����Ѵ�. 0�� ������� �ʴ´�. (1~254)
	enum KIND_COUNTER
	{
		KIND_COUNTER_NULL = 0,

		//-------DB KIND�� �߰� ����-----------
		KIND_COUNTER_SEATKEEPER=11,	// �ڸ���Ŵ�� DB KIND ��
		KIND_COUNTER_CHANCEITEM=12, // ���������� ����Ƚ��
#if defined(_BETAVER)
		KIND_COUNTER_BETA_ALLINMONEY=13, // ��Ÿ���� �Ӵ� ����Ƚ��
#endif // defined(_BETAVER)		
		//-------------------------------------	

#if defined(_BETAVER)
		MAX_COUNTER = 3 ///���� ������ ī���� �� ������ ���� 
#else
		MAX_COUNTER = 2 ///���� ������ ī���� �� ������ ���� 
#endif // defined(_BETAVER)		
		//-------------------------------------	
	};

	//���⼭ �� COUNTER_KIND�� ���� Ƚ�� ���� �ð� ������ �������ش�.
	static COUNT_BASE GetCountBase(KIND_COUNTER CounterKind)
	{		
		switch(CounterKind)
		{
			//-------Ƚ�� ���� ���� ��������-----------
			case KIND_COUNTER_SEATKEEPER: return DAILY_COUNT;  //�ڸ���Ŵ�� ��Ƚ��
			case KIND_COUNTER_CHANCEITEM: return DAILY_COUNT;  //���������� ��Ƚ��
#if defined(_BETAVER)
			case KIND_COUNTER_BETA_ALLINMONEY: return DAILY_COUNT;  //���������� ��Ƚ��
#endif // defined(_BETAVER)
			//-----------------------------------------
		}		
		return UNCOUNTABLE;
	}

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
	struct COUNTERUNIT
	{	
		BYTE	kind;				// ����
		COUNTTYPE	count;			// Ƚ��
		SYSTEMTIME    lasttime;			// ������ ��� �ð� 
		char    bNeedDBUpdate;		// �α� �ƿ� �� DB�� ����ؾ� �� �ʿ䰡 �ֳ�?

		COUNTERUNIT ()
		{	
			kind=0;
			count=0;
			ZeroMemory(&lasttime, sizeof(lasttime));
			bNeedDBUpdate=0;
		}
		
		private:
		COUNTTYPE _GetCount(COUNT_BASE CountBase) //�ð����ؿ� ���� ���� ��ȯ 
		{
			if (UNCOUNTABLE == CountBase ) return ERR_COUNT;
			if (PERMANENT_COUNT == CountBase) return count;
			COleDateTime now = COleDateTime::GetCurrentTime();

			COleDateTime LastDateTime(lasttime);
			if (LastDateTime.GetStatus() == COleDateTime::invalid) 
				return count;
					
			if (now.GetYear() != LastDateTime.GetYear()) 		return 0;
			if (ANNUAL_COUNT == CountBase)	return  count;				
			if (now.GetMonth() != LastDateTime.GetMonth())		return 0;
			if (MONTHLY_COUNT == CountBase)	return  count;
			if (now.GetDay() != LastDateTime.GetDay())		return 0;
			
			return count;			
		}
		public:
		COUNTTYPE GetCount()
		{			
			return _GetCount(GetCountBase((KIND_COUNTER)kind));
		}
	};
private:

	COUNTERUNIT Counter[MAX_COUNTER];	
	
	// ����ִ� ī���Ϳ� ���ο� ī���� ����
	COUNTTYPE NewCount(KIND_COUNTER CounterKind, int Count, COleDateTime &LastDate)
	{		
		COUNTERUNIT *pEmptyCounter = GetCounterPtr(KIND_COUNTER_NULL);
		if (pEmptyCounter==NULL) return ERR_COUNT;

		pEmptyCounter->kind = CounterKind;
		pEmptyCounter->count = (COUNTTYPE)Count;
		LastDate.GetAsSystemTime(pEmptyCounter->lasttime);
		return pEmptyCounter->count = Count;
	}

public:
	GAMECOUNTER () 
	{
		ZeroMemory(this, sizeof(GAMECOUNTER));
	}

	COUNTERUNIT *GetCounterIndexPtr(int i)
	{
		if (i<0 || i>=MAX_COUNTER) return NULL;
		return &Counter[i];
	}

	// ī���� ������ ��������
	COUNTERUNIT *GetCounterPtr(KIND_COUNTER CounterKind)
	{
		if (CounterKind<KIND_COUNTER_NULL) return NULL;

		for(int i=0; i<MAX_COUNTER; i++)
		{
			if (Counter[i].kind == CounterKind)
			{
				return &Counter[i];
			}
		}
		return NULL;
	}


	///Ƚ���� �����´�.
	COUNTTYPE GetCount(KIND_COUNTER CounterKind)
	{
		COUNTERUNIT *pCounter = GetCounterPtr(CounterKind);
		if ( NULL == pCounter ) return 0;
		return pCounter->GetCount();
	}

	/// Ƚ���� nIncValue��ŭ ������Ų��. (������ Ƚ�� ��ȯ)
	COUNTTYPE IncCount(KIND_COUNTER CounterKind, int nIncValue=1)
	{
		COUNTERUNIT *pCounter = GetCounterPtr(CounterKind);
		if ( NULL == pCounter )
		{
			SetCount(CounterKind, nIncValue, COleDateTime::GetCurrentTime());

			pCounter = GetCounterPtr(CounterKind);
			ASSERT(pCounter);
			if (pCounter==NULL)				
				return 0; //Ƚ�� ���� ���� 			

			pCounter->bNeedDBUpdate = TRUE; //�α׾ƿ��� DB������Ʈ �Ѵ�.
			return pCounter->count;
		}
		
		pCounter->count = GetCount(CounterKind) + nIncValue;		
		GetLocalTime(&pCounter->lasttime);
		pCounter->bNeedDBUpdate = TRUE; //�α׾ƿ��� DB������Ʈ �Ѵ�.

		return pCounter->count;
	}

	// ī���͸� �����Ѵ�. (DB���� ���� ���� �� �� )
	COUNTTYPE SetCount(KIND_COUNTER CounterKind, int Count, COleDateTime &LastDate)
	{
		// UNCOUNTABLE�� ���������ʴ´�.
		if (GetCountBase(CounterKind)==UNCOUNTABLE) return ERR_COUNT; 

		COUNTERUNIT *pCounter = GetCounterPtr(CounterKind);
		if ( NULL == pCounter )
		{
			return NewCount(CounterKind, Count, LastDate);
		}

		pCounter->count = (COUNTTYPE) Count;		
		LastDate.GetAsSystemTime(pCounter->lasttime);

		return pCounter->count;
	}

	BOOL NeedDBUpdate()		//DB���� �ؾ��ϳ�?
	{
		for(int i=0; i<MAX_COUNTER; i++)
		{
			if (Counter[i].bNeedDBUpdate)
				return TRUE;
		}

		return FALSE;
	}
};

//10�ð� ����
// upp �߰� 
#define PLAYTIME_SECTION "PLAYTIMELIMIT"
#define UPP_SECTION "UPPLIMIT"
typedef struct
{
	// ���θ�ǿ��� ���̴� �÷��� �ð� ����� UPP ���� ����ϴ� �÷��� �ð�
	// ������ ����� �ٸ��Ƿ� ���� ������ 
	DWORD dwLastPlayedTime;
	DWORD dwCurrPlayTime;
	BOOL  bSetZero;
	
	// UPP ���� ���Ǵ� �÷��� �ð� ���� 
	DWORD dwUPPLastPlayedTime ;
	DWORD dwUPPCurrPlayTime ;
	DWORD dwAdjustTime ; 
	int   nLeftCount;
	int	  nLeftCount2;
	int   nUPPWarningPolicy; // �ð� �˸��̰� �����Ǿ����� ��å 0: ������, 1:�ѹ�,2:�׻�
	int   nUPPWarningCounter; // �ð� �˸��� ������ ���� ī���� 
	int   nUPPExitCounter   ; // �ð� �˸��� �������Ḧ ���� ī���� 
	int   nUPPCurRound;     // UPP ��å�� ���� ���ǵ����� ���Ӷ���� 
	int   nUPPCurPlayTime ; // UPP ��å�� ���� ���ǵ����� ���� �÷��� �ð� 
	int   nUPPAgree;		// UPP ��å�� �����ߴ��� ���� 
	int   nUPPWarningReserveTime ; // 3��� UPP ������ ��� �ش� �ð��� ���������� warning
	int   nUPPExitReserveTime ; // 3��� UPP ������ ��� �ش� �ð��� �Ǹ� ������ ����
	int   nUPPElapsedTime ; // �α� �� �ĺ��� ������ �ð� OnTimer ���� ȣ�� 
	int   nUPPState ; // ���� ���߽ÿ��� ��������,����,normal ���п��̾����� ������������ �����ð����ιٲ� 
	int   nUPPRound ; // �α��� �Ǽ�  
	int   nUPPLevel ; // upp �� ���� ���� ����� ǥ��
	int   nUPPDay;    // ������ �ѱ�� �ð� ���� �ʱ�ȭ�� ���ؼ� ��¥ ������ ����
					  // ���� Playtime ������ DB ���� �о�ö� ��¥�� ����ؾ� �� 


	DWORD GetCurrPlayTime()		{ return dwCurrPlayTime; };
	DWORD GetTotalPlayTime()	{ return dwLastPlayedTime + dwCurrPlayTime; }

	BOOL  IsValid();
	BOOL  GetExpired();
	DWORD CheckNotice();

	BOOL  UPPExpired() ; 
	
	static int   GetLimitTime();

}PLAYTIEMINFO;


/////////////////////////////////////////////////////////////////////////////////////////
// ����� ����2 (�ΰ� ������: �ٸ� ������ ä�� �̵��� ������ �ʿ��� ������)
typedef struct
{
	BOOL			bNoWhisper;			// �Ӹ� �ź����ΰ�?
	int				ConnectSec;			// ���� �ð�(��)
	char			RegID[ID_LEN];			// �ֹε�� ��ȣ
	char			UniqNo[ CN_LEN ];			// ����ũ �ѹ�
	in_addr			PeerIP;				// ������ IP�ּ� 
	DWORD			ServerTick;			// ������ Ŭ���̾�Ʈ���� �ð� ��2006.1.9 ���ǵ���
	BOOL			bAdminUser;        //### [������ ��� �۾�] ###
	// [ ����� ]
	int				UserLevel;		// �α��ν� ����
	SYSTEMTIME	StartTime;		// ���� �ð�
	REWARDITEM_LIST	RewardGameItem;
	//����õ��
	int AllinAngelCnt;				//���� õ�� ���� ���� Ƚ��
	SYSTEMTIME	AllinAngelDate;		//���� õ�� ������ ���� �Ͻ�
	///////////////////////////////////////////////////////////////////////////////////////////		
	
	///////////////////////////////////////////////////////////////////////////////////////////
	PREMINFO		PremInfo;			//[�����̾� ���� �������� �߰�2] [2007-05]
	BOOL			bWriteUnionMoneyDB;
	int				GamePlaySec;// �÷��̽ð�
	CHANCEITEMCNT	ChanceCount;

	// todo: array�� ����
	ALLGAMERECORD RecordOnLogin;  //�α������� ������ �������	//2008.03.04 ������� ����� ��ȭ ������ ���� �Ǽ�

	ST_CHEAT_INFO		CheatInfo;
	int			nLobbySec;	//���� ü�� �ð� (���ǿ��� �����ð� ���ӹ濡 �������� ������ �ٸ� ä�η� �̵�) //���ӹ� ����� 0���� �� ����

	IDX_GAME		MyGame;		// ���� �����ε���  (���� �� ����� �� ������ ���� �ε����� �ٲ��)
	//�йи� ����
	PREMOPTION PremOption;
	PREMOPTION stPrevPremOption;		// ���� �ɼ�

	// �ѵ� �˸� ���� üũ ����
	// [�ѵ� ���� �۾�] [2009-02-19]
	char			bCheckMaxMoney;

	//����ī����
	GAMECOUNTER GameCounter[MAX_GAME];
	//10�ð� ����
	char            DI[DI_LEN + 1];
	PLAYTIEMINFO    PlayTimeInfo;

	// �� �ִ� �ս� ����
	INT64			llDailyLoss;	// ���̳ʽ��� �սǻ���
	time_t			DailyLossBlockTime;
	DAILYLOSS_STATE eDailyLossState;

	//�ƾ�巹��
	char			MAC[13];			//DAT�α�

	//PC�� ��ȣ
	int PcRoomNumber;

	ORGINFO	LoginRecord;		//�α��ν� �������
	DATE LastMoneyUpdateTime;	//������ �Ӵ� ���� ����

	//G_TFT
	int Last_RoomNo;		//������ �����ߴ� ���ȣ
	INT64 Last_RKey;		//������ �����ߴ� �� Ű 
	INT64 Last_PKey;		//������ �����ߴ� �� Ű
	INT64 llRoomKeyPrev;	//������ �����ߴ� IPS�� Ű 
//////////// NEWPOKER Add ToDo: �Ⱦ��� ���� �����ϱ�
	int nPlayCount;

	SYSTEMTIME  	EventStartTime;
	char			UDPIP[32];			// ������ UDP ������ ������ IP�ּ�(���� IP�ּҿ� ����)
	int				UDPort;				// UDP ��Ʈ ��ȣ
	BOOL			IsNotReal;			// ���� IP�� �ƴѰ�?	

	INT64			nStartPMoney;				// ���� ���۽� ������ �ִ� �ݾ�


	int				nLuckAvataCnt;		//[���ƹ�Ÿ] 2006.07.24 ������ Ƚ�� 
	BOOL			bWriteGameDB;

	INT64			nAbusePoint[4];			// 0:����IP�뿪�� �Ƹ�����Ʈ , 1: ����IP�뿪�� ��������Ʈ , 2:��� �Ƹ�����Ʈ, 3:��� ������
	char			bHaveNightEvent;		//���Ÿ�� ���� ����
	char			bHaveGoldenChipEvent;	//���Ĩ ���� ����	
	char			temp[2];				// ����� �ϼս� ���� �����ȿ� ���

	//2008.03.04 ������� ����� ��ȭ
	BLOCKDATA		BlockInfo;

	//�Ƕ�
	int				nMissionNum;

	// �Ǽ��̺�Ʈ ���¡ üũ. ���� ���¡ ī��Ʈ�� ���� �̵��ÿ��� ��������.
	int nCheckedAbusingCount;
	
	char cMorningBetBanishCount;			// ��ħ���� ������ ī��Ʈ( 0 ~ 7 )
	SYSTEMTIME tmMorningBetBanishEndDate;	// ��ħ���� ������ �� ����ð�

	BOOL bGameAbuser; //���� ����� ���� üũ ///////////////////////////2019.04.29 ��Ű�� ���θ�� �۾�/////////////////////////////// �ϸ鼭 �߰�

//////////// NEWPOKER Add ToDo: �Ⱦ��� ���� �����ϱ�
} USERINFO2;

typedef struct  // ���� ����� ���� ���ſ�
{
	int		nGameIndex;		// ���Ӱ����ε���
	UINT64	llUserKey;		// IPS User Key
	char	ID[ID_LEN];			// ����� ���̵�
	char	NickName[NICK_LEN]; // �г��Ӽ��� 	
	char	Sex;				// ����
	
	INT64  PMoney;		    // ��Ŀ �Ӵ�
	char	Position;			// ���� ��ġ(WH_CHAN:ä�μ��� WH_LOBY: ���� WH_SHOP:���� WH_GAME:���ӹ�)
	int		RoomNum;			// ���ȣ(���� ��ġ�� ���ӹ��϶��� ��ȿ)
	char	AdminGrade;		//�����ڵ���� ��Ÿ����. 0�ϰ�� ������ �ƴ�  ### [������ ��� �۾�] ###

	////////////////////////////Ŭ���ý��� 2017.12.21/////////////////////////////////////
	int ClubMemberGrade; //Ŭ�������, ������, �Ϲ��̳�... �߰�
	int ClubCode;
	char ClubName[CHAR_BUFFER_CLUB_NAME]; //������ Ŭ���̸�

	//////////////////////////////////////////////////////////////////////////
	// ī�� �ý���. 2018.12.20
	CafeSystem::CafeInfo cafeInfo_;						// ī�� ����
	CafeSystem::CafeInfo unitedCafeInfo_;				// ���� ����
	CafeSystem::MemberGrade::Type cafeMemberGrade_;		// 0: ��ȸ�� 1: ��ȸ�� 2: ��ȸ�� 3: ī����, 4: ���տ�(ī���� ����) 5: ������ 
	//////////////////////////////////////////////////////////////////////////

} SMALLUSERINFO;

/////////////////////////////////////////////////////////////////////////////////////////
// ���� ����	[���� ������ �۾�]
typedef struct
{
	int  GameCode;				// ���� �ڵ�
	char NoticeURL[128];			// �������� URL
	char BannerURL1[128];		// �κ� ���� URL
	char BannerURL2[128];		// ���� ���� URL
	char LocationURL[128];		// ��ġ�˸�(ģ��ã��)�� URL	
	char SrvInfoMsg[128];		// ä�ηκ� �޼���	

	// [���Ӿ�����â ������] 2006.04.20
	char AvatarItemURL[128];	// �ƹ�Ÿ ������ URL
	char FuncItemURL[128];	  // ��� ������ URL
	char MyItemURL[128];     // �� ������ URL  
	char CashURL[128];		// ĳ������ URL
	char BDURL[128];		// ĳ������ URL
	
	char LeadersJoinURL[128];	// [��ȣõ��]
	char LeadersJoinURL2[128];	//
	INT64 JackPot;				// [ ###����### ]�α�� ���� ���� ���� �Ӵ�
	DATE ServerTime;		//���� �ð� (����ȭ)
} SERVERINFO;

/////////////////////////////////////////////////////////////////////////////////////////
// ������ ���� ����
typedef struct
{
	INT64 BettingLimit;				// ���� �ѵ�(�Ϲ� ȸ�� ����)
	INT64 LossMoneyLimit;			// �սǸӴ� �ѵ�
	INT64 WarningLossMoney;			// �սǸӴ� �˸���
	INT64 WarningLossMoney_Room;	// �սǸӴ� �� �˸���
	INT64 BasicLossMoneyLimit;		// ��޿� ���� �սǱݾ�
	INT64 nDiamondLimit;			// ���̾� ȸ�� ����
	INT64 MaxBetPerFriendRoomRound;	// ����ɿ��� ���� ���� �ִ� ���� �ݾ� (��������� ����Ǵ� ���� �ݾ�) - 2018�� 5�� 10�� �۾�
} RESTRAINTINFO;	

/////////////////////////////////////////////////////////////////////////////////////////
// �α��� ����
typedef struct
{
	USERINFO UI;			// ����� ����	
	char UniqNo[16];		// ����ũ �ѹ�
	short GroupNo;			// ������ ���� ä�� ��ȣ
	short ChanNo;			// ������ ���� ä�� ��ȣ
	short LocChanNo;		// ������ ���� ä�� ��ȣ	
	char  ChanName[32];		// ������ ä�� �̸�	

	char bVSMode;			//���� ä���� VS ����ΰ�?
	char bMovableToVS;		//���� �׷쿡 VSä���� �ֳ�?

	char JackpotType;		// ���� Ÿ��  (0:���� 1:Half���� 2: Royal Half����)
	///�ѵ��۾�
	INT64  MaxServerMoney;					//���� ������ �ִ� �ִ�Ӵ�
	INT64  DisplayMaxMoney[MAX_CODE_NEW_FAMILY];	// ���÷��̿� ���񽺺� ���� �ִ� �ѵ�

	////////////////////////////���ý���,���ä�� 2017.11.07/////////////////////////////////////
	INT64 MaxServerMoney_Gold;					//���� ������ �ִ� �ִ�Ӵ�(���)
	INT64 DisplayMaxMoney_Gold[MAX_CODE_NEW_FAMILY];	// ���÷��̿� ���񽺺� ���� �ִ� �ѵ�(���)
	INT64 llMaxPremiumMoneyWithOption_Gold;
	//////////////////////////////////////////////

	// ȸ���� + �ɼ� ��� ������ ������ �ִ� �ִ� �Ӵ�		// // 2011.01.27. �йи� ����. ���� 1500��
	INT64 llMaxPremiumMoneyWithOption;
	char bNewlySignedup;	 //���� �Ӵ� ���ڵ� ���� �����̴�..
//////////// NEWPOKER Add ToDo: �Ⱦ��� ���� �����ϱ�
	char RegID[ID_LEN];			// �ֹε�� ��ȣ [���� ������ �۾�]
	char bNightEvent;			// 0 �̸� �̺�Ʈ ����  1 �̺�Ʈ ������ 2 ������	
	char temp[3];
	char  Reserved[2];
//////////// NEWPOKER Add ToDo: �Ⱦ��� ���� �����ϱ�

	RESTRAINTINFO RestraintInfo;
} LOGININFO;

/////////////////////////////////////////////////////////////////////////////////////////
// ä�� �̵��� ���� ����(�ٸ� �������� ä�� �̵�)
typedef struct
{
	char	ID[NICK_LEN];			// �������� ID
	short	UNum;			// �������� ������ȣ
	char	Reserved[2];
	DWORD	WaitPass;		// ���� ��� �н� ��ȣ
} CONNECT_MOVECHANINFO;

/////////////////////////////////////////////////////////////////////////////////////////
// ä�� ���� ����
typedef struct
{
	int		Code;				// ó�� �ڵ�(0���� ������ ���� �Ұ� �ڵ�)
	short	GroupNo;			// ������ ä���� �׷� ��ȣ
	short	ChanNo;				// ���� ä�� ��ȣ
	short	LocChanNo;			// ���� ä�� ��ȣ
	char	ChanName[32];		// ä�� �̸�
	INT64	nJackPotMoney;		//�� ���� �Ӵϸ� ��//2005.12.29 -jackpot-
	char JackpotType;		// ���� Ÿ��  (0:���� 1:Half���� 2: Royal Half����)
	char bVSMode;			//���� ä���� VS ����ΰ�?
	char bMovableToVS;		//���� �׷쿡 VSä���� �ֳ�?
} ENTERCHANINFO;


////////////////////////////////////////////////////////////////////////////////////////
// Ŭ���̾�Ʈ�� �׷� ����
typedef struct
{
	short	GroupNo;				// �׷� ��ȣ
	char	Reserved[2];
	char	GroupName[24];			// �׷� �̸�

	INT64	GroupLimitMoney;		// �׷� �������� �Ӵ�
} GROUPINFO;

/////////////////////////////////////////////////////////////////////////////////////////
// ä�� ����
typedef struct
{
	short GroupNo;			// �׷� ��ȣ
	short ChanNo;			// ���� ä�� ��ȣ
	short NowUser;			// ���� �����ο�
	short MaxUser;			// �ִ� �����ο�
	char  WorkState;		// ä�� ����(0:���� 1:������ 2:������)
	char  bVSMode;			// VsMode
	char  JackpotType;			// ���̽ý��� 
	char  Reserved[3];
} CHANNELINFO;

/////////////////////////////////////////////////////////////////////////////////////////
// ����� ã�� ���
typedef struct
{
	char	ID[ID_LEN];			// ���̵�
	char	NickName[NICK_LEN];		// �г��Ӽ��� 	
	short	GroupNo;			// �׷� ��ȣ
	short	ChanNo;			// ���� ä�� ��ȣ
	char	ChanName[32];		// ä�� �̸�
	short	Position;			// ��ġ
	short	RoomNum;			// ���ȣ
} RESULTFINDID;


/////////////////////////////////////////////////////////////////////////////////////////
//�Ӵ� ���� ����
struct MONEYRANGE
{
	INT64 MinMoney; //(MinMoney % 10) ==> 1 �� ������ "MinMoney-1 �ʰ�", ==> 0�� ������ "MinMoney �̻�"
	INT64 MaxMoney; //(MaxMoney % 10) ==> 9 �� ������ "MaxMoney+1 �̸�", ==> 0�� ������ "MaxMoney ����"

	CString GetStrMoneyRange()  //�Ӵ� ������ �ؽ�Ʈ�� �ٲ۴�. 
	{
		CString strMin, strMax;
		if ( MinMoney > 0 )
		{
			if ( MinMoney%10 == 1 )
			{
				strMin.Format("%s �ʰ� ", NumberToOrientalString(MinMoney-1));
			}
			else
			{
				strMin.Format("%s �̻� ", NumberToOrientalString(MinMoney));
			}
		}

		if ( MaxMoney > 0 )
		{
			if ( MaxMoney%10 == 9 )
			{
				strMax.Format("%s �̸�", NumberToOrientalString(MaxMoney+1));
			}
			else
			{
				strMax.Format("%s ����", NumberToOrientalString(MaxMoney));
			}
		}
		CString strRet = strMin + strMax;
		strRet.TrimRight();
		return strRet;
	}

	CString GetStrMoneyRange_for_Combobox()  //�Ӵ� ������ �ؽ�Ʈ�� �ٲ۴�. 
	{
		CString strMin, strMax;
		strMin = strMax = "";
		if ( MinMoney > 0 )
		{
			if ( MinMoney%10 == 1 )
			{
				strMin.Format("%s �ʰ� ", NumberToOrientalString(MinMoney-1));
			}
			else
			{
				strMin.Format("%s �̻� ", NumberToOrientalString(MinMoney));
			}
		}
		
		if ( MaxMoney > 0 )
		{
			if ( MaxMoney%10 == 9 )
			{
				strMax.Format("%s �̸�", NumberToOrientalString(MaxMoney+1));
			}
			else
			{
				strMax.Format("%s ����", NumberToOrientalString(MaxMoney));
			}
		}
		if( MinMoney == 0 && MaxMoney == 0 )
		{
			strMax.Format("���� ����");
			return strMax;
		}
		if(strMin.GetLength() > 0 ){
			return strMin;
		}
		return strMax;
	}
	
	BOOL IsInRange(INT64 PMoney) const // PMoney�� �Ӵ� ���� (MinMoney~ MaxMoney) ������ ���ΰ�?
	{		
		//MinMoney =0 �ϰ�� MaxMoney=0�ϰ�� �˻� ���� 
		if (MinMoney>0 && MinMoney > PMoney) return FALSE;
		if (MaxMoney>0 && MaxMoney < PMoney) return FALSE;		
		return TRUE;
	}

	INT64 GetDispMaxMoney()  //�泻�� ���÷��� �Ǵ� Max �Ӵ� 
	{
		if ( MaxMoney%10 == 9 )
		{
			return (MaxMoney+1);
		}
		return MaxMoney;
	}
	INT64 GetDispMinMoney() //�泻�� ���÷��� �Ǵ� Min �Ӵ� 
	{
		if ( MinMoney%10 == 1 )
		{
			return (MinMoney-1);
		}
		
		return MinMoney;

	}
	CString GetStrMoneyRange_for_Combobox2(bool bNoSpace=false)  //�ӴϹ������ؽ�Ʈ�ιٲ۴�. 
	{		
		//ex) "OO���̻�", "OO��~ OO��", "OO��̸�"

		CString strMoneyRange;
		if (GetDispMinMoney()!=0 && GetDispMaxMoney()!=0)
		{
			strMoneyRange.Format("%s ~ %s", NumberToOrientalString(GetDispMinMoney()), NumberToOrientalString(GetDispMaxMoney()), strMoneyRange.GetBuffer());
		}
		else if (GetDispMaxMoney()==0)
		{
			if ( MinMoney%10 == 1 )
			{
				strMoneyRange.Format("%s �ʰ�", NumberToOrientalString(GetDispMinMoney()), strMoneyRange.GetBuffer());
			}
			else
			{
				strMoneyRange.Format("%s �̻�", NumberToOrientalString(MinMoney), strMoneyRange.GetBuffer());
			}			
		}
		else if (GetDispMinMoney()==0)
		{
			if ( MaxMoney%10 == 9 )
			{
				strMoneyRange.Format("%s �̸�", NumberToOrientalString(GetDispMaxMoney()), strMoneyRange.GetBuffer());
			}
			else
			{
				strMoneyRange.Format("%s ����", NumberToOrientalString(MaxMoney), strMoneyRange.GetBuffer());
			}		
		}

		if (bNoSpace)		
			strMoneyRange.Replace(" ", "");

		return strMoneyRange;
	}

	bool operator==(const MONEYRANGE &rhs) const
	{
		return (MinMoney == rhs.MinMoney && MaxMoney == rhs.MaxMoney);
	}
};

////////////////////////////////////////////////////////////////////////////////////////
// �� ����
typedef struct
{	
	int		nGameCode;			// �����ڵ� (42;������Ŀ, 54:�ٵ���, 61: ����Ŀ, 218: ����Ŀ)
	int		nGameIndex;			// �����ε���
	UINT64	llRoomKey;			// IPS Room Key
	int		nLocalRoomNum;		// ���� ���ȣ
	short	RoomNum;			// �۷ι� ���ȣ
	short	UNum;				// ������ ���� ��ȣ
	char	Title[32];			// ������
	char	ID[ID_LEN];			// ���� ID
	char	NickName[NICK_LEN];		// �г��Ӽ���

	char	NowUserNum;			// ���� �����ο�
	char	MaxUserNum;			// �ִ� �����ο�
	char	NowObserverNum;		// ### [ ������� ] ### ������ �ο�

	// 7��Ŀ:			0: ���̽�,	 1: �Ϲ�
	// �ο�ٵ���:		0: ����
	// ����Ŀ:			0: ��Ŀ,	1: ��ź
	// ����Ŀ:			0: 7��Ŀ ���̽�, 1: 7��Ŀ �Ϲ�	//-> RuleType , 1: 7Poker, 2: 5Poker
	// �Ƕ�				
	char	FormKind;			// Normal, Choice 4�� , 62,bomb
	char	Secret;				// ���� ����(0=���� 1=�����)
	char	State;				// ����(0:����� 1:������)
	char	nChangeType;		// �ٵ���
	char	cRuleType;			// ����Ŀ, �Ƕ�
	char	bSuperMaster;		// ���� ���ΰ�?	[���� ������ �۾�]
	char	RoomMasterType;		// 0 : normal 1: SuperMaster
	char	bWaitInRoom;		// 1 �̸� ���̾Ƹ�� ������ �����
	char	Pass[5];			// �н�����(������� �ϰ��)

	MONEYRANGE	MoneyRange;			// �Ӵ� ���� ���� (�������� �Ӵϱ���)
	INT64		llEnterLimitMoney;	// �������� �Ӵ�

	char	arrRaiseStyleDesc[ 10 ];// ���ý�Ÿ�� ����	1-2-2-3 ����, 1-1-2-2 ���� �� ���� ����. �迭�ε��� 1 = 1���� ��������. 0���ε����� ������
	INT64	BetMoney;				// �õ�Ӵ� (������� SeedKind���� ���� ������ ���� �־��ش�.)
	INT64	CurSeedMoney;			// ����õ�Ӵ�(���ӽ����ο�����)
	int		nRaiseStyle;			// ���ý�Ÿ�� ����

	float	fBetMoneyMulti;			// ������ ��� (���ٸ� ���)
	int		nAutoBetKind;			// ������� ���� 0�̸� ������� ������� �ʴ´�	
	short	sBetRuleKind;			// �������� (�ظ���, ����, ����Ʈ) //< CYS 110513 >
	char	cCardSetting;			// ī�� ���� (���)
	char	bNodie;
	bool	bEventJackpotTime;		// 1000�� && 22:00 ~ 02:00
	bool	bEventJackpotGiftTime;	// 1000�� && 22:00 ~ 02:00 && 3�� �̻� ���� ���ް��ɹ�
	
	//������ �ɼ�
	enum OBSERVER_OPTION {OBSERVER_OPTION_OK=0, OBSERVER_OPTION_NOCHAT, OBSERVER_OPTION_UNALLOWABLE};
	OBSERVER_OPTION ObserverOption;		// ���� �Ұ�Ÿ�� =>  0 : ��������(Default) 1 : ���� ä�� ���� 2 : ���� �Ұ� 

	bool bIsMaxRoom;	// MaxRoom �ΰ�?
	INT64 llInMoney;	// MAXRoom ���� �ݾ�

	time_t tCreatedTime;

	//���°� 1: ���̾Ƹ�� 2: ������
	char	cMemberRoom;		// ���̾� ȸ���� ���� ����� �� 2015-02-02 Add // ���� ��忡�� ��� �� 

	////////////////////////////���ý���,���ä�� 2017.11.07/////////////////////////////////////
	int ChipKind;
	/////////////////////////////////////////
	bool bIsFriendRoom; // ģ���� �ΰ�? // 2018�� 5�� 2�� ������Ī ����� ���� ���� �߰�

	////////////////////////////VIP�׷� �и� 2018.07.16/////////////////////////////////////
	int VIPGroup; //VIPä���� �� �׷��ȣ.. Default 0�̰� �׷��� �ʿ��� ��� �ش� ���� ���

	//////////////////////////////////////////////////////////////////////////
	// ī�� �ý���. 2018.12.20
	CafeSystem::CafeKind::Type cafeKind_;		// ī����� ��� ����.
	int cafeID_;		// Ŭ�󿡼� ����. ���չ��� ��쿡�� 0�� �ƴѰ�.
	//////////////////////////////////////////////////////////////////////////
	
	bool bMorningBet; // �ο�ٵ��� ��ħ���� ( ��ħ�� �߿��� ����,�ݸ� ����ǰ� ���� �� ���Ƽ ���� )

} ROOMINFO;


////////////////////////////////////////////////////////////////////////////////////////
// �÷��̾� ���� ����
typedef struct
{
	char JoinState;		// ���� ���� ����(0:����, 1:����)
	char PlayState;		// ���� ����(0:�������� 1:������)
	char nState;     // 0: �ƹ����¾ƴ�, 1: ī��3��, 2: ī��2��,����1, 3: ī��4~5��, 4: ����, 5: ���
	char nCardTotal; // ��ī�� ��
	char nOpenTotal; // ���µ� ī���	
	char nCard[4];   // ���� ī�� �������� ����ī�� ###[8��Ŀ]### 5	
	char bCheck;

	char bOnGame; // ���� ���� ����
	char bFold;   // ���� ���� ����
	char nCheckIp;	// ### [ �ߺ��� IP�� ] ###
	char nSndFxKind; // ### [���� �߰� �۾�] ###
	char arrCuttingNum[3];	// �ٵ��� Ŀ������	index 0:��ħ, 1:����, 2: ����

//#if defined(__HOOLA__)
	char cRanking;		// ���� ����
	char cCurCardCnt;  // [ �Ƕ� ] ī�� ���
	bool bRegister;	   // ��� ����(0:���X 1: ���)
	char TurnCnt;      // ���° ���ΰ� ������ + 1
	bool bAutoPlay;     // �ڵ� �÷��� ��
	char JackpotCount; // ������ ������ ���� ��ī��Ʈ ����(������ٰ� ����)
//#endif

} PLAYSTATE;

////////////////////////////////////////////////////////////////////////////////////////
// �÷��̾� ���� ���� 2
typedef struct
{
	BOOL bJoinBanishVote;	// �߹� ��ǥ�� �������ΰ�?
	BOOL bHaveBanishVote;	// �߹� ��ǥ�� ���ƴ°�?
	// [ ���� ��Ŀ ]
	BOOL bActive;			// ��Ƽ�� ����(���� ����)
	int  nActive;			// ���� �ð�
	BOOL bWindOn;			// ������ ���ѽð� �÷���
	int  nWindOn;			// ������ ���ѽð�
	int  nFoldNum;			// �ڵ�����Ƚ��(3ȸ�̻��̸� ������) ������ ������ �÷��� �ʱ�ȭ
	int  In_OutCount;		// ### [ ������� ] ### ����, ���� �ݺ�����
	int  Totalmoneylimit;	//2004.05.07 �������Ѿ� ����	

	char bWinner;			//���ӿ��� �����ΰ�?
	
	// �������� 
	int	 SevenValue;		// �������� 
	int	 LowName;			//�ο����� ����
	int  FullHouseNo;		// Ǯ�Ͽ콺�϶��� ���� ����


	//�ڵ����� ���� 2006.5.3//////////////////////////////////////////////////
	BOOL bAutoRuleDisconnect; // �ڵ� ������ ������ ��, �ڵ������� ������ ���� ���¿��� ����ų� �ð����� TRUE
	//////////////////////////////////////////////////////////////////////////
	
	/////////////�ٵ���///////////////////
	BOOL bChanged;			//TRUE �ٲٱ��û�Ҷ� FALSE �ٲ� ����� ������.
	int	 nChangedCnt;		// �ٲٱ� ���� �ð�
	BOOL bChangeCard;		//�ٲٱ� ��û�� �Դ�.
	BOOL bOrderChanged;		//TRUE �ٲٱ��û�Ҷ� FALSE �ٲ� ����� ������.(���� �Ƶ� �϶� ���)
	BOOL bChangedEnd;		//TRUE Ŭ���̾�Ʈ �ٲٱ� ���� ���� ��Ŷ�� �޾ƾ� ��, FALSE �ٲٱ� ���� ���� ��Ŷ ���� 
	int	 nChangedEndCnt;	//�ٲٱ� ���� ���� ���ð�

	///////////////���̷ο�/////////////////
	BOOL bWinCaseOn; // �� ������ ���ѽð� �÷���
	int  nWinCaseOn; // ���� �ð�
	int  WinnerCase; // 1 : High, 2 : Low , 3: Swing
	char nTurn;			//�� ���� (4�� ������)

	// ����������� �Ǽ�/�Ӵ�
	ALLGAMERECORD m_stInitialRecord;
	INT64 m_llInitialMoney;

	// ����
#if defined(__SUTDA__)
	bool bChoice;
	int nChoice;
	bool bRematch;
	int nRematch;
	bool bResultCard;
	int nResultCard;
#endif // (__SUTDA__)

#if defined(__HOOLA__)
	// [�Ƕ�]
	char cCheckThank;		// ���� �߳� ���߳� ���������� ���
	bool bThank;			// ��ũ ������	

	char cThankYou;			//  1: ��������, 2: ����, 3: ���� ,4: ����̵�
	char nThankYouCnt;	    // ���

	bool bAbleToHoola;		// �Ƕ� ���� ( �Ƕ� �ڱ� �Ͽ� ��� ī�带 �� ���� )
	bool bAbleToSeven4Card; // 7��ī�� ����

	int  nThankNum;		    // ��ť����?( �̼ǿ��� ��� )
	short sSevenCardRegNum; // 7ī�� ��ϼ���

	bool bStop;				// ���� �� ����( �� ���� )
	bool bThankBak;			// ��ť��
#endif

} PLAYSTATE2;


typedef struct
{
	char nCard[4];   // ����ī��+����ī��+ ���̽�ī��
} BACKCARD; // ############## ī�� ������ �����÷��׷ε� ����Ѵ�

/////////////////////////////////////////////////////
///////////      �Թ�� ������ ����     /////////////
/////////////////////////////////////////////////////
typedef struct{
	int      nUNum;  // ����� ���̵�
	char    bInUse;  // �������(���������÷����̸� ������ ����)
	char       nUse; // ������� ������Ƚ��
	char     nState; // 0: �ƹ����¾ƴ�, 1:ī��3��, 2:ī��2�����1, 3: ī��4��~6��, 4: ������, 5: ���¸��
	char     nStyle; // 0: üũ, 1: ����, 2: ��Ʈ, 3: ����
	char       nBtn; // 0: ��(�ּ�), 1: ����(����), 2: �ƽ�(�ִ�)
	char     bFold;  // ���� ����
	char     bCall;  // �ݻ��� ����
	char   nLastCmd; // ������ ����
	INT64  nNowBat;  // �������������� �Ӵ�
	INT64  nTotBat;  // �ѹ��� �Ӵ�
	INT64 nRealBat;  // �ǹ��� �Ӵ�
	INT64  nPMoney;  // �������� ��Ŀ�Ӵ�
	INT64 nReCall;   // �ް� 
	INT64 nCalcBat;  // ��
	char	bMaxBet; // �ִ� ���� �ѵ� ���� ����
	char	nRemainRaiseCount; // ���� ���� ���忡�� ���� ���� ���� Ƚ��
	INT64	nRoundChip;	// ���� ��忡�� ����ϴ� Ĩ

	/////////////////////////////�ս��ѵ� �ʰ� ��� �߰� 2017.05.15/////////////////////////////////////////////
	char bRealOverBet; //������ ���ÿ� ���� ��¥�� �Ϸ� �ս��ѵ��� �Ѿ�� �ִ� ��Ȳ�̴�..0�̸� �ȳѾư�, 1�̸� �Ѿ
	///////////////////////////////////////////////
}RAISEUSER;

/////////////////////////////////////////////////////
///////////      �Թ�� �÷��� ����     /////////////
/////////////////////////////////////////////////////
typedef struct{
	char  bDistCard;	
	char  nDistUser;
	char  nRound;	 // ���� ����
	char  nState;

	char bRaiseOn;  // ������ ���ӿ���
	char bBtnBet;
	char bBtnCheck;
	char CurPlayer;
	char LastPlayer;
	char LastStyle;	
	char nBossPNum;		//����
	char nImageBoss;	//���̸� ����	

	char nReMainTotalCardNum;	// ī�� ���� ���(�����ְ� ���� ���) 

	//�ڵ����� ���� 2006.5.3//////////////////////////////////////////////////
	char nAutoBetCmd;
	char nAutoBetNodie;	
	//////////////////////////////////////////////////////////////////////////

	PLAYSTATE PS[MAX_PLAYER];
	RAISEUSER RU[MAX_PLAYER]; 	
	BACKCARD  BC[MAX_PLAYER]; // ���� ī��	

	short JPHalfCount;			//���̽ý��� (����ī��Ʈ)
//////////// NEWPOKER Add ToDo: �Ⱦ��� ���� �����ϱ�
	char  nPreCmd;
	char nBombCard;		// ��ź ī�� 
	char bDSEvent;		//�������� �̺�Ʈ �÷��� //���Ĩ �̺�Ʈ 2006.2.28
	char temp[3];
//////////// NEWPOKER Add ToDo: �Ⱦ��� ���� �����ϱ�
}POKERGAME;

typedef struct{
	int  nStyle;    // 0: ����, 1.üũ , 2: ��, 3: ��Ʈ, 4: ����
	int  nBtn;      // 0: ��(�ּ�), 1: ����(����), 2: ����, 3: �ƽ�(�ִ�)
	DWORD  PlayTick;			// ������ Ŭ���̾�Ʈ���� �ð� ��
}POKERCLNT; //  ##############3 ī�峯���� �������ε� ����Ѵ�

typedef struct{
	char ID[16];
	char NickName[20];
	char  nCard;             // ��Ƽ��, ��Ƽ��, ��ī��
	short  nRoom;
	char ChanName[25];		// ä�� �̸�
	INT64 JackPotPlus;	    // ���� ���ʽ� ����
	INT64 NowJackPot;	    // ���� �ܾ�
}SPECIALCARD;

typedef struct{
	char     ID[ID_LEN];
	int      nUNum;
	USERINFO UI;
}USERRESULT;

typedef struct{
	char     ID[ID_LEN];
	int      nUNum;
	int      nCard;
	INT64    nMoney;
}BUYCARD;

typedef struct{
	char     ID[ID_LEN];
	int      nUNum;
	int      nCard;
}CHANGECARD;

typedef struct  
{
	ALLGAMERECORD m_stChangeRecord;	// �ش� �濡���� ��������
	INT64	m_llChangeMoney;					// �ش� �濡���� ����Ӵ�
	INT64	llMaxLossMoney;						// �ִ� �ս� ���� �Ӵ�
}CHANGE_USERINFO_ROOM;

////////////////////////////////////////////////////////////////////////////////////////
// �� ���� ����

//#if defined(__HOOLA__)
//	[ �Ƕ� ]
typedef struct
{
	PLAYSTATE Ps[ MAX_PLAYER ]; // �������� �������, ī�����
	bool  bGoldState;
	int   nGoldStage;
	bool  bPractics;
	short sRoomState;              // �Ϲ� : 0, �������� : 1
	
	char	arrCenterCard[ 4 ][ 13 ];		   // �߾ӿ� ��� ��Ʈ����Ʈ ī��
	char	arrSameNumCard[ 13 ][ 4 ];	       // ���� ���� ī��
	short	sBackCardCnt;				   // �׿� �ִ� ī���
	short	sDisCardCnt;				   // ���� ī�� ��
	short	sDisCardNum;				   // ���� ī���� �� ���� �ѹ�, ������ -1
	short	sCurMission;
	char    arrMagicCard[ 8 ];			   // ����ī��
} HOOLAGAME;
//#endif

typedef struct
{
	ROOMINFO Ri;
	USERINFO Ui[MAX_NEWPLAYER];// ### [ ������� ] ###
	CHANGE_USERINFO_ROOM Cui[MAX_NEWPLAYER];	// �ش� �濡���� �����Ӵ�/�����Ǽ��� ���� ����

	union
	{
		POKERGAME Pg;
		HOOLAGAME Hg;
	};

} ENTERROOMINFO;

////////////////////////////////////////////////////////////////////////////////////////
// �� ����� ���� ���� ����
typedef struct
{
	ALLGAMERECORD AllGameRecord[PlayableChipType::MAX];
//	ARRAY_VALUE<UINT> AllinCnt;  //����ī��Ʈ		//2008.11 ��Ŀ�� ���� �������� ����	
	INT64 PMoney;
	UINT  AllinCnt;			// ����ī��Ʈ 2006.1.9
} CHANGEMYINFO;

////////////////////////////////////////////////////////////////////////////////////////
// �߹� ��ǥ ��û ����
typedef struct
{
	short RoomNum;		// �� ��ȣ
	short MyServPNum;	// ��û�� ���� �÷��� ��ȣ
	char MyID[ID_LEN];		// ��û�� ���̵�
	short TarServPNum;	// �߹� ��� ���� �÷��� ��ȣ
	char TarID[ID_LEN];		// �߹� ��� ���̵�
	char Reson[42];		// �߹� ����
} BANISHVOTEINFO;

////////////////////////////////////////////////////////////////////////////////////////
// ���� ���� ����

typedef struct
{
	char JoinID[ MAX_PLAYER ][ ID_LEN ];		// ������ ���̵�
	PLAYSTATE Ps[ MAX_PLAYER ];			// �÷��̾� ���� ����ü
	char  nCard[ 4 ];
	int   nDistUser;					// ī�� ������
	INT64   nSeedMoney;					// �õ�Ӵ� 
	float	fBetMoneyMulti;				// ����� ������ ���
	double	fRaceBetRatio;				// ���̽� ���� ���� 
	char	nImageBoss;					// ���̸� ����
	char RoomMasterType;				// ���� ���� üũ 

//#if defined(__HOOLA__)
	char	PlayerCard[ 8 ];			// ���� �÷��̾� ī������
	bool	bPracticsGame;
//#endif
	char	nRoundCount;

} STARTINFO;
////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////�����ε����, Ÿ�پ˸� 2017.10.12/////////////////////////////////////
typedef struct
{
	char nCard[4];

} BLINDSHOWINFO;
///////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
// ���� ���� ���
typedef struct 
{
	char  ID[ID_LEN];
	char   NickName[NICK_LEN];

	ALLGAMERECORD AllGameRecord[PlayableChipType::MAX];		//���� ���� ���		// 2008.12 ��Ŀ�� ����
	INT64	PMoney;
	int		nRank;			//���(�ٵ���)
	INT64   nResultMoney;	// ȹ��� �Ǵ� ���� �Ӵ� .
	char	WinCase;		// ���̷ο� 1:���� 2:�ο� 3:����
	char	CaseSel;		// ������ ���� 1:���� 2:�ο� 3:����
	float   fGoryRatio;		// ���� ���Ӻ���
	INT64	GameFeeDP;		// DP�� ���Ӻ� (2009.05.19)

	INT64 llInMoney;
	INT64 llSafeMoney;

//#if defined(__SUTDA__)
	INT64 nPenaltyMoney;	// ������ ����
	int nPenaltyState;		// ���� ����(0:����, 1:����, 2:������, 3:38������, 4:49��, 5:����)
//#endif // (__SUTDA__)

	//////////// NEWPOKER Add ToDo: �Ⱦ��� ���� �����ϱ�
	POKERINFO PI;
	//////////// NEWPOKER Add ToDo: �Ⱦ��� ���� �����ϱ�

#if defined(__HOOLA__)
	// [ �Ƕ� ] (1 �Ƕ�(ù��), 2 �Ƕ�, 3 ī���ι��� , 4 �뻧, 5 �һ�, 6 stop , 7 ������� ī�� ����)
	char	cRanking;		// ���
	char	cJokerBak;			// ��Ŀ ī�� ���
	char	cSevenBakCnt;		// 7 �� ������ �ִٸ� �׼� (���)^2
	char	arrSevenCardCnt[ 4 ];	// �� �ѹ�
	bool	bStopBak;		// ������ڿ��� ���� �ٵ�, ���ϰ�� �ھ׼�
	bool	bRegisterBak;	// ��Ϲ� �׼�
	bool	bThankYouBak;	// ��ũ��	
	int		CardSum;		// �� ���� ī�� �հ�
	char	cGold;

	bool	bHoola;
	char	cRemainCardCnt;
	bool	bRegister;
	bool	bDaebbangWin;
	bool	bSobbangWin;
	bool	bSeven4Win;
	int		nMission;

// 	INT64	i64GetMaxMoney;
// 	INT64	i64GetBetMaxMoney;
	bool	bVIPUser;
	char	cDosinLimit;
#endif

} GAMEOVERINFO;

struct GAMEOVER_HOOLA
{
	short	  sWinnerCase; // �Ƕ�(1 �Ƕ�(ù��), 2 �Ƕ�, 3 ī���ι��� , 4 �뻧, 5 �һ�, 6 stop , 7 ������� ī�� ����) 
	short	  sGoldStage;
	bool	  bGoldHoola;
	bool      bClientDelayTime;
	char	  arrPlayerCard[ MAX_PLAYER ][ 8 ]; // �������� ī��

	////////////////////////////�Ƕ� ��ǹ� 2018.03.29/////////////////////////////////////
	bool	bGiveUpGame; //�̹��ǿ� ���⸦ �ߴ��� �����̴�.
	//////////////////////////////////////////
};

struct GAMEOVERRESULT
{
	// ��ü ���� 
	char      cGame;        // ��������  (0:����, 1:�ٵ���, 2:����Ŀ, 3:����Ŀ)
	char      WinCase;		// (����:1 �ο�:2 ����:3  ���� 1�� ) ���ڰ� 2��: 4 (���̷�  �̰�°� �ο�� �̰�°� ���ڰ� 2���ΰ�) 
	char      bOpen;		// ��� �̰�°�?(��� �׾ Ȧ�� �̰�°� FALSE, ������ �����ߴ°� TRUE)
//	char      bJackPot;     // ������ �ִ°�   		
	GAMEOVERINFO  Ui[MAX_PLAYER];
	CHANGE_USERINFO_ROOM Cui[MAX_PLAYER];		// �ش� �濡���� �����Ӵ�/�����Ǽ��� ���� ����

	union
	{
		POKERGAME Pg;
		GAMEOVER_HOOLA Hg;
	};
	
	//< CYS 111002 > ����
	//INT64 JackPotMoney;
	
	// ���� ������  
	char	  nWinnerNum;						//���ڰ� �������  (�Ʒ� �迭 ����� ������..)		
	char      WinID[MAX_PLAYER][ ID_LEN ];			// ���� �Ƶ�		
	char      nNowWinner[MAX_PLAYER];			// ���� �÷��̾� �ѹ�		
	
	#define EMPTY_CHAR ""	
	#define IDX_SP_WINNER 0			//������Ŀ ���� �迭 �ε���
	#define IDX_HL_1ST_WINNER 0		//���̷ο� ù��° ���� �迭 �ε���
	#define IDX_HL_2ND_WINNER 1		//���̷ο� �ι�° ���� �迭 �ε���

	GAMEOVERRESULT(){}
	void Init(int GameIdx) 
	{
		ZeroMemory(this, sizeof(GAMEOVERRESULT));
		memset(nNowWinner,-1,sizeof(nNowWinner));
		cGame=GameIdx; 
	}
//////////// NEWPOKER Add ToDo: �Ⱦ��� ���� �����ϱ�
 	char      ID[ID_LEN];		// ���� �Ƶ�	
 	char	  NickName[NICK_LEN]; // ���� �г���
	INT64     nWinMoney;	// ���� ������ ����(�� ���þ��� ������ ����)
	INT64	  nRealBat;  // ���� ������ �������� �Ӵ� X
	INT64     nGory;    // �����ݾ�
	char      JokerCardDeck[5];

	////////////////////////////���ý���,���ä�� 2017.11.07/////////////////////////////////////
	int ChipKind; //�ش� �濡�� � ��ȭ�� ������ �ߴ��� �˷��ش�.
	bool CommitionDiscount; //������ ������ �ϰ� �ִ��� �����̴�.true�̸� ������, false�� ���ξ���
	///////////////////////////////////////////

	//< CYS 111002 > ����
//	char	  temp1;	

// 	float       nPercent; // �ۼ�Ƽ�� : �ܼ��� �����ִ� ��ɸ� �ִ� 1%,5%,10% ���
// 	INT64     AddEventMoney;	// �߰� �̺�Ʈ �Ӵ� ���� �Ӵϰ��� ������ �߰� �̺�Ʈ ��÷�̴� �������׸� �ش�
// 	int		  AddEventPrizeCnt; // �̺�Ʈ ���� Ƚ�� �ϵ �˰� ������ȴ�. �޼����� ���� ���׸� �����ش�. 	
// 	//���Ĩ �̺�Ʈ 2006.2.28
// 	int 	  nGoldSupplyCnt;	//�������� �̺�Ʈ �޴� ����

// 	char	  nCurRoomGoldenGaugeStep;//������ �ܰ躰 ǥ�� �� 0 ,1,2,3,4,5
// 	char	  temp;
	
};

/////////////////////////////////////////////////////
////////////    [���� ������Ʈ �۾�]   //////////////
/////////////////////////////////////////////////////

typedef struct
{
	short  nUNum;   // ����� ��ȣ
	char ID[ID_LEN];  // ���̵�
	INT64 PMoney; // ��Ŀ�Ӵ� ����ġ
} AGENT_USER;


//### [������ ��� �۾�] ###
typedef struct
{
	short  AdminUNum;
	char AdminID[ID_LEN];
	short  TarUNum;
	char TarID[ID_LEN];
} ADMINMESSAGE;

//  �� �ɼ� ���� ���� 
typedef struct
{
	short   RoomNum;			// ���ȣ
	short   UNum;				// ������ ���� ��ȣ
	char  ID[ID_LEN];			// ���� ID
	char  Title[32];        // ������

	//�ڵ����÷� �߰� [2005.05.24]
	char  nBetKind;			// 0~8���� 591 592 59F......
	char  bNoDie;			// �����ϸ� ���̹�ư�� nBetKind ���������� ��Ȱ��ȭ ��Ų��			

	char  Reserved[2];
	char  nMaxUser;			// ���� �� ����
	char  nPass[4];			// �н� ���� ����
	int  bSecret;			// ��й� ����		
} STYLEROOMINFO;

//���� ���� �˸� �޼���
typedef struct //���
{
	short   RoomNum;		// ���ȣ
	short   UNum;			// ������ ���� ��ȣ
	char  ID[ID_LEN];			// ���� ID
	char  Title[32];        // ������
	char  bReseveApply;    // ��������ٷνǽ����� ��������.. (0:�ǽ� , 1:����) 

	//�ڵ����÷� �߰� [2005.05.24]
	char  nBetKind;			// 0~8���� 591 592 59F......
	char  bNoDie;			// �����ϸ� ���̹�ư�� nBetKind ���������� ��Ȱ��ȭ ��Ų��		

	char  bSecret;			// ��й� ����	
	char  nMaxUserNum;
} CHANGESTYLEROOMINFO;

//////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////                               [�����̾� �۾�]	                                //////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

int CheckPremiumState(USERINFO* pUI, SYSTEMTIME* pCurTime=NULL);
int IsPremiumMoreMini(USERINFO* pUI, SYSTEMTIME* pCurTime=NULL);

//������ ī������
typedef struct {

	BOOL bOpenCard;
	char ID[MAX_PLAYER][16];
	int SelCase[MAX_PLAYER];	//����/�ο�/��������		
	BACKCARD  BC[MAX_PLAYER]; // ���� ī��
} HL_CARDINFO;


// ### [ �ߺ��� IP�� ] ###
typedef struct 
{
	char IpIndex[MAX_PLAYER];
} IPARRINDEX;
////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////
////////////    ### [ ������� ] ###   //////////////
/////////////////////////////////////////////////////
//����˸� �޼���
typedef struct
{
	short Seq; //�������
	short UNum;
	char ID[ID_LEN];
	char NickName[NICK_LEN];
	////
} RESERVATIONINFO;

//////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//���� ���� ���� 
enum CODE_INSURANCE
{
	CODE_INSURANCE_GOSU= 4,
	CODE_INSURANCE_SUNSU,
	CODE_INSURANCE_YOUNGWOONG,
	CODE_INSURANCE_JIZON=6,
	CODE_INSURANCE_DOSIN, // ������Ŀ ��޺��� ����
};


/////////////////////////////////////////////////////////////////////////////
//  ���� ���� ���� 
#define TOP_INSURANCE		(CODE_INSURANCE_JIZON)	//�������� ����
#define BOTTOM_INSURANCE	(CODE_INSURANCE_DOSIN)   //���ź��� ����

//���κ��� ���� ��û
typedef struct  {
	char		ID[20];

	int			UNum;	
	int			nAIN_InsuranceRequest;	//��û�� ���� ����
}AIN_INSURANCE_JOIN_REQUEST;

typedef struct  {
	char		ID[20];

	int			UNum;	
	int			nAIN_Insurance_Result;	//��û�� �����	0 : ���� 1 : ����
	int			nAIN_Insurance_Kind;	//����� ���� ����	

	INT64		nAIN_Insurance_Money;		//������ �����
	INT64		nApplyCurPMoney;		//������ ����(PMoney)��
	INT64		nAIN_Join_Total_Insu_Money;		//�̹��� ���Ե� ���������
}AIN_INSURANCE_JOIN_RESULT;

//���κ��� ���� ��û
typedef struct  {
	char		ID[20];

	int		UNum;	
	int		nAIN_InsuranceRequest;	//��û�� ���� ����
}AIN_INSURANCE_SUPPLY_REQUEST;

typedef struct {
	char		ID[20];

	short		UNum;
	BOOL		nAIN_Insurance_Result;		// ��� 0 ���� 1 ����
	int			nCurAIN_InsuranceKind;		//��û�� ���޵� ���谡�� ����

	INT64		nCurAIN_InsuranceMoney;		//���� �� ����� ���谡�Աݾ�	
	INT64		nSupply_InsuranceMoney;		//��û�� ���޵� ���谡�� �ݾ�
	
	INT64		nApplyCurPMoney;		//������ ����(PMoney)��	
	
}AIN_INSURANCE_SUPPLY_RESULT;




typedef struct {	
	char	TargetIP[24];		//����� ������
	char	AdminID[16];		//������ ���̵�
	char	AdminIP[24];		//������ ������
	char	TargetID[16];		//����� ���̵� 

	char	Memo[48];			//����

	int		Status;				//���°�type 1= BENEFIT_UNSUPPLY_CHANCE 2=BENEFIT_UNSUPPLY_JACKPOT 3=BENEFIT_UNSUPPLY_EVENT
	int		nType;				//���� 1 ���̵� 2 ������ �� ����
	char	ItemCodeList[128];	//���� ������ ����Ʈ

}BLOCKIPLIST_DATA;//�������� ���� 2006.2.3



typedef struct  {
	char			AdminID[ID_LEN];	//����� ������ ���̵�
	char			AdminIP[24];	//����� ������ ���̵�	
	char			TargetID[ID_LEN];	//�������� ���̵�
	char			TargetIP[24];	//�������� ������
	char			Memo[48];		// ���� ����
	char			nID_or_IPFlg;	//���̵� ������� ������ �������
	char			nBadUserKind;	//���� ���� 0~4
	short			UNum;			// UNUm
	char			StayServerIP[24];		//������ �׷� ��ȣ ��� �޼�������
	int				nStayPort;		//������ ä�� ��ȣ ��� �޼�������
	int				nSaveType;		// ���� ���� 0: �ű� 1: ����  2 : ����
	char			ItemCodeList[48];//���� �����۷� ����Ʈ
}ADMIN_ADDBADUSER_DATA;





//[���ƹ�Ÿ] 2006.07.24
typedef struct  {
//	char		nSameGroupCode;				//0 ��ü ä�� �׷��ڵ�
//	short		nSameChannelSupplyUserCnt;	//0 ��ü ä�� �ش� �������� ���� �޴� ���ƹ�Ÿ ����
	char		nSupplyKind;				//���� ���� 0 ��ü ä�� 1 ������ ä�θ�
	char		GroupCode[MAX_GROUP_GRADE_INDEX];				//�׷� ��ȣ	
	short		nSupplyUserCnt[MAX_GROUP_GRADE_INDEX];			//�ش� �������� ���� �޴� ���ƹ�Ÿ ����
	BYTE		nHaveMaxLuckAvataCnt;		//�����Ҽ� �ִ� �ִ� ��	
	char		LevelLimit[GRADE_MAX+1];				//��������
	char		Membership[MAX_CODE_NEW_FAMILY+1];				//��������//&&&&&&&
	int	PlayCnt;	//2008.03.04 ������� ����� ��ȭ ������ ���� �Ǽ�
	
}LUCKAVATA_DATA; 


#define SEND_MAXCHANNEL 20
typedef struct {
	int			nUser[SEND_MAXCHANNEL];
} CHANNEL_USERNUMBER;


CString Log_USERINFO(USERINFO *pUI);
CString GetAutoBettingRuleName(IDX_GAME idxGame,  int nAutoBet, BETRULE_KIND eBetRuleKind/* ����, ����,Ʈ */, RULETYPE ruleType=RULETYPE_7POKER );
CString GetCardSettingName(IDX_GAME idxGame, char cCardSetting );
CString GetBetMoneyMultiName(IDX_GAME idxGame, float fBetMoneyMulti );


//////////////////////////////////////////////////////////////////
//��Ŀ ���� �߰�

// ��Ŀ �� ���� (������)
enum RSTATE
{
	RSTATE_NONE = 0,			// �ƹ����µ� �ƴ�(�����)
	RSTATE_DISTRIBUTECARD,		// �и� ������ ��
	RSTATE_PREPARESTART,		// ���� ������ �غ���
	RSTATE_EXTRAFIRSTBET,		//4�� ���� 
	RSTATE_STARTGAME,			// ������ ���۵Ǿ���
	RSTATE_GAME_ROUND_CHANGECARD_M,	//��ħ ����
	RSTATE_GAME_ROUND_CHANGECARD_A,	//���� ����	
	RSTATE_GAME_ROUND_CHANGECARD_N,	//���� ����	
	RSTATE_HIGHLOW_SELECT,			//����, �ο�, ���� ���� 
	RSTATE_BOMB,				// ��źī��		
	RSTATE_RESULT,  // ���� ���
#if defined(__SUTDA__)
	RSTATE_SD_RESULTCARD,			// �� ���� ���� ��û, ���� ����
#endif // (__SUTDA__)
#if defined(__HOOLA__)
	RSTATE_SUNSELECT,			// ��������
#endif
//#if defined( __ROUND_MODE__ )
	RSTATE_ROUND_MODE_GAME_END,	// 10���� ����Ǹ� �� �̻� ���� ������ �ȵȴ�.
//#endif


};


//��ɼ�(�õ�Ӵ�)
typedef struct 
{
	char idxGame;
	char idxNo;
	char bDefault;
	char rule;
	INT64 i64SeedMoney;
	//INT64 i64DefaultSeedMoney;
} ROOMOPTION_SEEDMONEY;

//��ɼ�(�ڵ�����)
typedef struct 
{	
	char idxGame;
	char idxNo;
	char rule;
	char bDefault;
	short nAutoBet;
}ROOMOPTION_AUTOBET;

//��ɼ�(�Ӵ�����)
typedef struct 
{	
	char idxGame;
	char idxNo;
	char bDefault;	
	
	MONEYRANGE MoneyRange;
} ROOMOPTION_MONEYLIMIT;

/*
//< CYS 110321 > ����
enum TYPE_JACKPOT_SYSTEM
{
	TYPE_JACKPOT_SYSTEM_NULL=0,		
	TYPE_JACKPOT_SYSTEM_NORMAL_1,		//����1 (
	TYPE_JACKPOT_SYSTEM_NORMAL_2,		//����2
		
	//		TYPE_JACKPOT_SYSTEM_ROYAL,			//�ξ�����	
	TYPE_JACKPOT_SYSTEM_ROYAL_1,	//�ξ�����1
	TYPE_JACKPOT_SYSTEM_ROYAL_2,	//�ξ�����2	
		
	MAX_TYPE_JACKPOT_SYSTEM,
	MIN_TYPE_JACKPOT_SYSTEM = TYPE_JACKPOT_SYSTEM_NULL+1
};

static char *g_JackpotTypeName[]={"N/A", "Half I", "Half II","Royal Half","Reserve","Reserve"}; //���� ������
#ifdef _GAMETEST
#define JACKPOT_DURATION_ANNOUNCE (2) //���� ��ǥ �ð� (��)
#else
#define JACKPOT_DURATION_ANNOUNCE (5) //���� ��ǥ �ð� (��)
#endif // _GAMETEST

typedef struct  
{	
	char WinRoomName[60];
	short RoomNo;
} JACKPOT_WINROOM_FOR_LOSER;

typedef struct  
{
	INT64 RoomKey;		//G_TFT ���� ����
	INT64 JackpotM;
	JACKPOT_WINROOM_FOR_LOSER WinRoomInfo;	//���� ���� ����

} JACKPOTROOM;

*/
//���÷꿡 ���� �ùٸ� �õ��ΰ� �Ǵ� ?
inline BOOL g_IsCorrectSeed(GROUP_NO Group, USERINFO *pUI, int nAutoRule, int SeedKindIdx, INT64 BaseSeedMoney)
{	
	return TRUE;
	/*
	if (pUI==NULL) return FALSE;

	static int m_SEPERATOR_IDX[MAX_GROUP_GRADE_INDEX]={0,};	
	m_SEPERATOR_IDX[GROUP_GRADE_PLATINUM2] = 2;

	GROUP_GRADE gg=g_GetGroupGrade(Group);		

	if( gg == GROUP_GRADE_PLATINUM2)
	{
		if( nAutoRule == 0)
		{
			return (SeedKindIdx < m_SEPERATOR_IDX[gg]);
		}
		else if( nAutoRule == 501)
		{
			return (SeedKindIdx >= m_SEPERATOR_IDX[gg]);
		}
		return FALSE;
	}
	return TRUE;
	*/
}

#define MAX_ROOMOPTION_CNT 100	// ���� �ɼ� ���� �þ�� �� üũ�ؾ� ��
typedef struct  
{
	MONEYRANGE	stMoneyRange;	// �������� �Ӵϱ���
	INT64	llEnterLimitMoney;	// �������� �Ӵ�
	INT64	llSeedMoney;		// �õ� �Ӵ�
	short	sAutoBet;			// ���÷�
	char	cDefault;			// ����Ʈ �ɼ� - Ŭ�󿡼��� ���
	char	cDefaultQS;			// QuickStart�� ����Ʈ �ɼ�(�ڵ����÷�) - Ŭ�󿡼��� ���
	int		nRaiseStyle;		// 1-1-2-3 ���� ���������� �������
	short   sBetRuleKind;		// ���� ���( �ظ��� ����, ���� ����(�⺻), ����Ʈ ���� ) //< CYS 110512 >
	char	cRuleType;			// �������� 1: 7��Ŀ(RULETYPE_7POKER),  2: 5��Ŀ(RULETYPE_5POKER)
	char	cFormKind;			// ���Ӹ��: 0: ���(FORMKIND_NORMAL), 1: ���̽�(FORMKIND_CHOICE), 2: ��ź(FORMKIND_BOMB), 3: ��Ŀ(FORMKIND_JOKER) 
	char    cMaxUserNum;		// �Ƕ� 2��, 5��
	char	cCardSetting;		// ī����� (���۽� ī�� �� ��)
	float	fBetMoneyMulti;		// ������ ��� (���ٸ� ���)
	char	cMemberRoom;		// ���̾� ȸ���� ���� ����� �� 2015-02-02 Add //���°� 1: ���̾Ƹ�� 2: ������
	INT64	llDefaultQuickStartLimitMoney;		// ���� ����� �⺻ ��
	INT64	llDefaultCreateRoomLimitMoney;		// �� ������ �⺻ ��
	INT64   llDefaultCreateRoom_SeedMoney;		// �� ������ �⺻ �õ�Ӵ�
	INT64   llDefaultQuickStart_SeedMoney;		// ���� �� ����� �⺻ �õ�Ӵ�

	////////////////////////////���ý���,���ä�� 2017.11.07/////////////////////////////////////
	int ChipKind; //������ �����ϴ� ��ȭ Ÿ���̴�. �� CHIPKIND �� ���ǵǾ� ����.. ���Ӱ� ��尡 �߰� �Ǹ鼭 �ʵ嵵 ���� �������.
	//////////////////////////////////////////////
	bool bIsFriendRoom; // ģ���� �ΰ�? // 2018�� 5�� 2�� ������Ī ����� ���� ���� �߰�

	////////////////////////////VIP�׷� �и� 2018.07.16/////////////////////////////////////
	int VIPGroup; //VIPä���� �� �׷��ȣ.. Default 0�̰� �׷��� �ʿ��� ��� �ش� ���� ���
	
	bool bMorningBet; // �ο�ٵ��� ��ħ���� ( ��ħ�� �߿��� ����,�ݸ� ����ǰ� ���� �� ���Ƽ ���� )

} ROOMOPTION;

typedef struct  
{
	MONEYRANGE stUserMoneyRange;		// ���� �����Ӵ� ����
	int	nArrayCnt;
	ROOMOPTION arrRoomOption[MAX_ROOMOPTION_CNT];
	ROOMOPTION stDefaultCreateRoom;		// �ٷν��ۿ� ����Ʈ ��ɼ�
} ROOM_CREATEOPTION;

typedef struct
{
	PLAYSTATE Ps;			// �÷��̾� ���� ����ü
	char  nCard[4];			// ���� ���� ī��� ������ ���� ī�尡 ���� ������ ���������� ���浵 �޴� ȿ���� ������
	//��Ŀ ��ź�� ���� ���--------
	// TRUE ��Ŀ ī�� ����, FALSE ������ ��ź ī�� 
	BOOL  bBonusCard;
	// FALSE �϶� 0��°�� ��ź ī���, TRUE �϶� 0��°�� ù��° ���� ��Ŀ 1��° ����ް� ��Ŀ
	char  nBonusCard[4];
	//------------------
	int   nDistUser;					// ī�� ������	
} VS_REMAINCARDINFO;// VSMODE���� ������ ����� ������ ���� ī�带 ���⿡ �־ �ѹ��� �����ش�

enum EnumClientSideVar
{
	CS_JOINABLE = 0,
	CS_PREV_ROOM,
	CS_SELECTED_ROOM,
	CS_MAX,
};

typedef struct	// ���� �� ���� ���ſ�
{
	int		nGameIndex;		// ���Ӱ����ε���
	short	sGroupNo;		// �׷��ȣ
	short	sChanNo;		// ä�ι�ȣ
	short	sRoomNum;			// ���ȣ
	UINT64	llRoomKey;			// IPS ��Ű
	char	szTitle[32];			// ������
	char	szMasterId[16];			// ������̵�
	
	MONEYRANGE stMoneyRange;	// �������� �Ӵϱ���
	INT64 llEnterLimitMoney;	// �������� �Ӵ�
	INT64 llSeedMoney;			// �õ�Ӵ�
	INT64 llTotalPMoneySum;	// �����Ӵ� ��
	char	cRuleType;			// �������� 1: 7��Ŀ(RULETYPE_7POKER),  2: 5��Ŀ(RULETYPE_5POKER)
	char	cFormKind;			// ���Ӹ��: 0: ���(FORMKIND_NORMAL), 1: ���̽�(FORMKIND_CHOICE), 2: ��ź(FORMKIND_BOMB), 3: ��Ŀ(FORMKIND_JOKER) 
	short	sAutoBetKind;		// 0: ��������, ...
	char	cCardSetting;		// ī�����
	float	fBetMoneyMulti;		// ������ ��� (only ����)
	char   cNowUserNum;		// ���� �����ο�
	char   cMaxUserNum;		// �ִ� �����ο�
	char   cNowObserverNum;    // ### [ ������� ] ###������ �ο�
	char   cState;				// ����(0:����� 1:������)
	char	arrUserInfo[MAX_NEWPLAYER];		// ������������, -1: ����, 0: ��, 1: ��
	bool	bSecretRoom;					// ����Ŀ ���� ����  true: ����, false: �Ϲݹ�
	
	// MaxRoom. 2011.01.27
	bool bIsMaxRoom;	// MaxRoom �ΰ�?
	INT64 llInMoney;	// MAXRoom ���� �ݾ�

	time_t tCreatedTime;			// ����� �ð�
	char szClientSideVar[CS_MAX];	// Ŭ���̾�Ʈ���� ����ϴ� ����. 0: ����(0:����� 1:������), 1: ������ ǥ��
	short	sBetRuleKind;			// �������� (�ظ���, ����, ����Ʈ) //< CYS 110513 >

	char	cMemberRoom;		// ���̾� ȸ���� ���� ����� �� 2015-02-02 Add //���°� 1: ���̾Ƹ�� 2: ������

	////////////////////////////���ý���,���ä�� 2017.11.07/////////////////////////////////////
	int ChipKind; //������ �����ϴ� ��ȭ Ÿ���̴�. 
	//////////////////////////////////////////////////////////
	bool bIsFriendRoom; // ģ���� �ΰ�? // 2018�� 5�� 2�� ������Ī ����� ���� ���� �߰�

	////////////////////////////VIP�׷� �и� 2018.07.16/////////////////////////////////////
	int VIPGroup; //VIPä���� �� �׷��ȣ.. Default 0�̰� �׷��� �ʿ��� ��� �ش� ���� ���

	//////////////////////////////////////////////////////////////////////////
	// ī�� �ý���. 2018.12.20
	CafeSystem::CafeKind::Type cafeKind_;		// ī����� ��� ����.
	int cafeID_;
	//////////////////////////////////////////////////////////////////////////
	
	bool bMorningBet; // �ο�ٵ��� ��ħ���� ( ��ħ�� �߿��� ����,�ݸ� ����ǰ� ���� �� ���Ƽ ���� )

} SMALLROOMINFO;

////////////////////////////////////////////////////////////////////////////////////////
// ���� ����
typedef struct
{
	SMALLROOMINFO	stSmallRoomInfo;
	SMALLUSERINFO	stSmallUserInfo[MAX_NEWPLAYER];
} GAMEINFO;

typedef struct 
{
	bool	m_bUseMoneySearch;		// �����Ӵ� �˻� ��뿩��
	int		m_nMoneyIndex;			// 0: 90% �̸�, 1: 90 ~ 110%, 2: 110% �̻�
	int		m_nPriorityMoneySearch;	// �����Ӵ� �˻� �켱����

	bool	m_bUseRuleTypeSearch;	// �������� �˻� ��뿩��
	int		m_nRuleTypeIndex;			// 0: 7��Ŀ, 1: 5��Ŀ
	int		m_nPriorityRuleType;		// �������� �˻� �켱����

	bool	m_bUseFormKindSearch;	// ���Ӹ�� �˻� ��뿩��
	int		m_nFormKindIndex;			// 0:���, 1:���̽�, 2:��ź, 3: ��Ŀ
	int		m_nPriorityFormKind;		// ���Ӹ�� �˻� �켱����

	bool	m_bUseLimitSearch;			// �������� �˻� ��뿩��
	INT64	m_llEnterLimitMoney;		// �������� �ݾ�
	int		m_nPriorityEnterLimit;		// �������� �˻� �켱����

	bool	m_bUseSeedBetRuleSearch;	// �õ�/���÷� �˻� ��뿩��
	INT64	m_llSeedMoney;					// �õ�/���÷� ��� �õ�
	int		m_nBetRule;						// �õ�/���÷� ��� ���÷� 490, 591...
	int		m_nPrioritySeedBetRule;		// �õ�/���÷� ��� �켱����

	int m_ChipKind;////////////////////////////���ý���,���ä�� 2017.11.07/////////////////////////////////////

} SEARCHCONDITION;

#if defined(__SUTDA__)

enum SUTDA_GAMESTATE
{
	SD_GAMESTATE_NONE = 0,					// ���� ���� ����
	SD_GAMESTATE_CARD_FIRST,				// ù��° �� ����
	SD_GAMESTATE_CARD_HIDDEN,				// ���� ī�� ����
	SD_GAMESTAE_CARE_GAMEOVER,				// ���� ����
	SD_GAMESTATE_CHECK_REMATCH,				// ���� ���� Ȯ����
	SD_GAMESTATE_CARD_REMATCH,				// ���� �� ����
};

// ���� ���� ����
typedef struct  
{
	char JoinID[MAX_PLAYER][16];			// ������ ���̵�
	PLAYSTATE Ps[MAX_PLAYER];				// �÷��̾� ���� ����ü
	int   nDistUser;						// ī�� ������
	INT64   nSeedMoney;						// �õ�Ӵ� 
	float	fBetMoneyMulti;					// ������ ���
	double	fRaceBetRatio;					// ���̽� ���� ���� 
	char	nImageBoss;						// ���̸� ����
	char RoomMasterType;					// ���� ���� üũ 
	int nCard[2];
} SD_STARTINFO;

// 3�� ���ٿ��� ���� �� ���� ��
typedef struct 
{
	char szID[MAX_PLAYER][16];
	char cCardIndex[MAX_PLAYER];
} SD_OPENCARD;

typedef struct 
{
	char szID[MAX_PLAYER][16];
	BACKCARD Bc[MAX_PLAYER];
} SD_RESULTCARD;
#endif	// defined(__SUTDA__)


#if defined(__HOOLA__)

#define MAX_SUN_SELECT_PLAYER		5
#define CARD_DECK_COLORS_CLUBS		0
#define CARD_DECK_COLORS_HEARTS		1
#define CARD_DECK_COLORS_DIAMONDS	2
#define CARD_DECK_COLORS_SPADES		3

#define SUN_CARD_COLORS							CARD_DECK_COLORS_SPADES
#define SUN_CARD_START_INDEX					(2)
#define MAX_SUN_CARD_INDEX						(10)
#define MAX_SUN_CARD_NUM						(MAX_SUN_CARD_INDEX - SUN_CARD_START_INDEX + 1)
#define MAX_SUN_SELECT_WAITTIME					(10)
#define MAX_WAIT_GAME_START_BY_SUN_SELECT		(3)

enum _eRoomState
{
	_eRoomState_Standby = 0,
	_eRoomState_Playing = 1,
};


// �Ƕ� ���� �۾�
typedef struct {
	int  nSevenCount;			// ���/���̱� �� 7ī�� ����
	char arrSevenCardValue[ 4 ];// 7ī���� ī�� �ε���
	int  nStCount[ 2 ];			// �������� ī�� ����
	BYTE StraightCard[ 2 ][ 8 ];// �������� ī�� �ε���
	int  nToCount[ 2 ];			// �������� ī�� ����
	BYTE TogetherCard[ 2 ][ 8 ];// �������� ī�� �ε���
	int  nAtSCount;				// �������� ������ ���̱� ������ ī�� ����
	BYTE AttachStCard[ 8 ];		// �������� ������ ���̱� ������ ī�� �ε���
	int  nAtTCount;				// �������� ������ ���̱� ������ ī�� ����
	BYTE AttachToCard[ 8 ];		// �������� ������ ���̱� ������ ī�� �ε���
	BYTE flag;				    // ��Ŀī�带 ���̱� �Ҷ� 1 �ƴҶ��� ���� �������� ����
}REGISTERCARD;

/*
////////////////////////////////////////////////////////////////////////////////////////
// ���� ���� ����
typedef struct
{
	char JoinID[MAX_PLAYER][16];	// ������ ���̵�
	PLAYSTATE Ps[MAX_PLAYER];		// �÷��̾� ���� ����ü
	INT64 PlayerMarble[MAX_PLAYER];	// �÷��̾��� ���� ����
	char PlayerCard[8];				// ���� �÷��̾� ī������
	int  SoundType[MAX_PLAYER];     // ���� ����
	char PracticsGame;
} STARTINFO;
*/

typedef struct 
{
	char PlayerCard[MAX_SUN_CARD_NUM];				// �� ���ÿ� ����� ī�� ����
} SUNCARDINFO;


////////////////////////////////////////////////////////////////////////////////////////
// �÷��̾��� ���ӿ���
typedef struct
{
	char ID[16];		// ���̵�
	char Ranking;
	char Reserved[3];
} PLAYERGAMEOVERINFO;

////////////////////////////////////////////////////////////////////////////////////////
// �÷��̾� thank you ���� ī�� �������� ��� �����ش�
typedef struct
{
	char CardNum;  // ���� ī�� �ε���
	char ThankNum; // thank �ֳ� ����
} THANKYOUCARD;

////////////////////////////////////////////////////////////////////////////////////////
// �÷��̾� thank you ���� = > ����
typedef struct
{
	char  ThankCase;    // 1: ��������, 2: ��Ʈ����Ʈ   
	char  nThankYouCnt; // ���
	BYTE  ThankCard[8];
} CLTHANKCARD;

////////////////////////////////////////////////////////////////////////////////////////
// �÷��̾� thank you ���� ���
typedef struct
{
	char  BackCardNum;        // �ռ� ����ī�� ( ������ -1 )
	char  ThankCase;    // 1: ��������, 2: ��Ʈ����Ʈ   
	char  nThankYouCnt;		  // ī�� ��
	BYTE  ThankCard[8];       // ī�� �ִ� 8��
//	char  CenterCard[4][13];  // �߾ӿ� ��ġ�� ���� ���� �迭	
//	char  SameNumCard[13][4]; // ���� ���� 
} THANKYOURESULT;
 

////////////////////////////////////////////////////////////////////////////////////////
// ������ ī���� ��û
typedef struct
{
	char  RegisterCase;		  // 1: ��������, 2: ����, 3: ���� ,4: ����̵�	
	char RegCardCnt;		  // ����� ī�� ��
	char  RegisterCard[8];    // ����� ī�� �ִ� 4��
}  USERREGISTERCARD;

////////////////////////////////////////////////////////////////////////////////////////
// ī�� ��� ���
typedef struct
{
	char  RegCardCnt;		  // ����� ī�� ��
	char  RegisterCase;		  // 1: ��������, 2: ����, 3: ���� ,4: ����̵�	
	char  RegisterCard[8];    // ����� ī�� �ִ� 3��
//	char  CenterCard[4][13];  // �߾ӿ� ��ġ�� ���� ���� �迭	
//	char  SameNumCard[13][4]; // ���� ���� 
} REGISTERRESULT;

////////////////////////////////////////////////////////////////////////////////////////
// ī�� ���̱� ��û
typedef struct
{
	char  AttachCase;		  //  1 , 2
	char AttachCardCnt;		  // ����� ī�� ��
	char  AttachCard[8];    // ����� ī�� �ִ� 4��
}  USERATTACHCARD;

////////////////////////////////////////////////////////////////////////////////////////
// ī�� ���̱� ���
typedef struct
{
	char AttachCnt;			 // �ѹ�
	char  AttachCase;		 //    1 , 2 
	char  AttachCard[8];		 // ���� ī�� �ѹ�		
//	char  CenterCard[4][13];  // �߾ӿ� ��ġ�� ���� ���� �迭	
//	char  SameNumCard[13][4]; // ���� ���� 
} ATTACHRESULT;

////////////////////////////////////////////////////////////////////////////////////////
// �Ƕ�� �̰������ ������������ �˸�
/*
typedef struct{
	char ID[20];
	char  nCard;             // ��Ƽ��, ��Ƽ��, ��ī��
	char  nRoom;
	char ChanName[25];		// ä�� �̸�
	INT64 JackPotPlus;	    // ���� ���ʽ� ����
	INT64 NowJackPot;	    // ���� �ܾ�
	INT64 PMoney;
}SPECIALCARD;
*/
#endif // (__HOOLA__)

//#if defined (__ROUND_MODE__)

struct ROUND_MODE_RESULT
{
	BYTE	  nRank;				// ����
	char	  NickName[NICK_LEN];   // ���� �г���
	INT64     nWinMoney;			// ���� ������ ����(�� ���þ��� ������ ����)
};

struct ROUND_MODE_GAMEOVERRESULT
{// ���� ��� ���� �� ȹ�� �ݾ�	
	ROUND_MODE_RESULT RoundResult[MAX_PLAYER];
};

struct ROUND_MODE_WAIT_ROOM
{// ������ ��� ��
	BOOL bUse;							// ���� Ȱ��ȭ
	int unum[MAX_PLAYER];				// ����� Unum
	DWORD WaitCount[MAX_PLAYER];			// 2�� �ȿ� �� ������ �ٸ� ������ �鿩������.
	BOOL bWaitUser[MAX_PLAYER];			// �����ؾ� �� ����	
	BOOL bNowCreateRoom;				// ��� ����� ������ ���� �����Ǿ��ٰ� unum ���������� �����鿡�� ���� �޽����� ������. TRUE �� Send�ؾߵ�.
	int nUserCount;						// ���� ���� ��
	char cMemberRoom;					// 2 �̻��� ���� ���
};

struct ROUND_MODE_CHIPS
{// �����忡�� ����ϴ� Ĩ ����	
	INT64	nChips[MAX_PLAYER];
};

struct ROUND_MODE_RESERVER_ENTER_ROOM
{// ���� �� ���� ��忡 �����ؾ� �ɶ� ��� 
	BOOL bRoundModeReserver;		// ���� ���� ���� ���� ��忡 ���� ������ �ϸ� ���� �����Ŀ� üũ�Ͽ� �濡�� ������ ���� ������ ������ ����.
	int  nRoomNum;					// ���� ����ÿ� �����ؾ� �Ǵ� �� ��ȣ
	int  llRoomKey;				// ����� ���� ����
};

enum E_ROUND_MODE_MATCHING_LOG
{
	E_RESERVE = 1,
	E_RESERVE_CANCEL,
	E_MATCHING,
	E_MATCHING_OK,
	E_MATCHING_CANCEL,
	E_MATCHING_AUTO_CANCEL,
	E_GAME_START,
	E_OUT_ROOM,
};

typedef struct
{
	BOOL bValid;			// ��ȿ�� �����ΰ�?
	int WorkKind;			//  1:���ӷα� 2: ���α� 3: ������ �Ѿ׸Ӵ�����
	UINT StartTick;			// DB ó�� ���� �ð�

	int			GameCode;		//���� �ڵ�
	INT64		RoundModeMoney;	
	SYSTEMTIME	stRoundModeReserveTime;		//���� ���� ��û�� �̿���
	SYSTEMTIME	stRoundModeCancelTime;		//��Ī ����� ���¿��� ��Ī ����� �̿���	(db���� �� �ʱ�ȭ)
	SYSTEMTIME	stMatchingTime;				//���� ��Ī �˸��� ���� �̿���
	SYSTEMTIME	stMatchingOKTime;			//��Ī �˸��� �ް� ������ �̿���				
	SYSTEMTIME	stMatchingCancelTime;		//��Ī �˸��� �ް� ������ �̿���				(db���� �� �ʱ�ȭ)
	SYSTEMTIME	stMatchingAutoCancelTime;	//��Ī �˸��� �ް� 10�ʰ� ������ ��� �ڵ� ��ҵ� �̿���	(db���� �� �ʱ�ȭ)
	SYSTEMTIME	stRoomGameStartTime;		//����濡 ���� �� ���ӽ������� ���� ���� �̿���				(db���� �� �ʱ�ȭ)
	SYSTEMTIME	stRoomOutTime;				//����� ������� ������� �濡�� ����� ��� �ð�			(db���� �� �ʱ�ȭ)
}ROUNDMODE_WAIT_TIME_LOG;


////////////////////////////Ŭ���ý��� 2017.12.21/////////////////////////////////////
typedef struct 
{
	//�ʴ� �� ���Խ�û���� ����ü�̴�.
	//���� ���Կ�û�� �ߴ�, �ʴ븦 �޾Ҵ� ������ �����ؼ� ���� �ᵵ ����
	//�ʴ�seq, Ŭ��ID, Ŭ���̸�,ȸ����,�ִ�ȸ����,�����ڵ�,��û�ڵ�
	INT64 InviteSeq; //�ʴ��� Sequence��
	int ClubID; //Ŭ��ID
	char ClubName[CHAR_BUFFER_CLUB_NAME];
	int MemberMax; //�����Ҽ� �ִ� �� ȸ����
	int MemberCount; //���� ���Ե� ȸ����
	int StatusCode; //���°�.. 0.�������, 1.����
	int RequestCode; //��û�ڵ�... 0.�ʴ������ 1.�������Կ�û�Ѱ�

} CLUB_INVITED_INFO;

typedef struct
{
	//Ŭ���� ���� ����ü �̴�.
	char MemberID[NICK_LEN];
	INT64 PMoney; //�����Ӵ�
	INT64 LastLoginTime; //�ֱ� ���ӽð�(�д����� ����Ͽ� ���۵�) - ���� �� ���� 55 �� ���� 55������ �����ߴٴ°�.. 
} CLUB_MEMBER_INFO;

typedef struct
{
	//�Ϲ� �������� ���� Ŭ�� ���� ��û ����ü �̴�.
	char UserID[NICK_LEN];
	INT64 JoinSeq; //���Կ�û Sequence��
	INT64 UserPMoney; //���� ������ �����Ӵ�
	int StatusCode; //���°�.. 0.�������, 1.����
} CLUB_JOIN_INFO;

typedef struct
{
	//Ŭ�� ���� ����ü �̴�.
	char ClubName[CHAR_BUFFER_CLUB_NAME];
	char BossID[NICK_LEN];   // �ʴ��ѻ�� ID, �ᱹ ������ �ʴ븦 �������̹Ƿ�..
	int ClubID; //Ŭ�� ������ȣ
	int BossLevel; //���� VIP ����
	INT64 BossPMoney; //������ �������� ���ӸӴ�
	int MemberMax; //�����Ҽ� �ִ� �� ȸ����
	int MemberCount; //���� ���Ե� ȸ����
} CLUB_INFO;
////////////////////////////////////////

////////////////////////////2018�����̺�Ʈ 2018.01.22/////////////////////////////////////
typedef struct
{
	//��������� ������ �� �ִ� ��ǰ �����̴�.
	char PM_Name[CHAR_BUFFER_PROMOTION_COMMON_PM_NAME];
	char GiftName[CHAR_BUFFER_PROMOTION_COMMON_TICKET_GIFT_NAME];
	int GiftIndex; //���� ������ȣ(����� �� ��ȣ�� �ָ� ��)
	int NeedTicketCount; //�ʿ��� ����� ����
} TICKET_GIFT_INFO;

typedef struct
{
	//Ƽ������ ������ ����
	int GiftIndex; //���� ������ȣ
	int SB_Count; //������ Ƚ��
} TICKET_SUBSCRIPTION_INFO;
///////////////////////////////////////


////////////////////////////���� �������, ��������� 2018.02.27/////////////////////////////////////
typedef struct
{
	//��� ������ ������� ���� ����
	int Grade; //����
	char DayCode[CHAR_BUFFER_DAYCODE_LEN];
	char User_ID[ID_LEN];
	char User_CN[CN_LEN];
	INT64 LossMoney;
} GOLD_HELP_REWARD_USER_INFO;
///////////////////////////////////////////

////////////////////////////2018��06�� ���� 2018.06.01/////////////////////////////////////
typedef struct
{
	//��Ÿ��ȭ ���� ���� ����
	int Level;
	char ChipKind[CHAR_BUFFER_ETC_CHIP_CODE];
	char LevelName[CHAR_BUFFER_GOLD_INSURANCE_NAME];
	INT64 JoinLimitMoney; //������ �ϱ� ���� �����ݾ�
	INT64 SaveMoney; //���� ���Խ� �����ϴ� �ݾ�
	
} ETCCHIP_INSURANCE_LEVEL_INFO;
///////////////////////////////////////////

//#endif //__ROUND_MODE__


#endif // __USERSTRUCT_H__