// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.

#pragma once


//#define _BETAVER

enum IDX_GAMEURL
{
	IDX_GAMEURL_NULL = -1,
	IDX_GAMEURL_CHANCEAVATAR,
	IDX_GAMEURL_ROULETTE,
	IDX_GAMEURL_KISA,
	IDX_GAMEURL_FAMILYAVATARLIST,
	IDX_GAMEURL_GAMEAVATARLIST,
	IDX_GAMEURL_LOTTOAVATARLIST,
	IDX_GAMEURL_GAMEITEMLIST,
	IDX_GAMEURL_MYITEMLIST,
	IDX_GAMEURL_REFILL,
	IDX_GAMEURL_JACKPOT_RANK,
	IDX_GAMEURL_MYFAMILY,
	IDX_GAMEURL_FREECHARGEINSURANCE,
	IDX_GAMEURL_INSURANCE,
	IDX_GAMEURL_INSURANCE_GOLD,
	IDX_GAMEURL_INSURANCE_GOLD2,
	IDX_GAMEURL_INSURANCE_RESULT,	
	IDX_GAMEURL_FREECHARGE,
	IDX_GAMEURL_COLLECTOVERMONEY,
	IDX_GAMEURL_ACHIEVELIMITMONEY,
	IDX_GAMEURL_COLLECTREVISIONMONEY,
	IDX_GAMEURL_COLLECTBOTHMONEY,
	IDX_GAMEURL_COLLECTOVERGOLDBANK,
	IDX_GAMEURL_LOBBYITEMSHOP,
	IDX_GAMEURL_SEATKEEPER,
	IDX_GAMEURL_GOLDENCHIP_CHANGE,
	IDX_GAMEURL_GOLDENCHIP_LIST,
	IDX_GAMEURL_ENDING_BANNER,
	IDX_GAMEURL_ENDING_BANNER_NATE,
	IDX_GAMEURL_GAMEMANUAL,			//���ӹ��â 
	IDX_GAMEURL_LOBBYEVENTDIALOG,	//�κ��̺�Ʈ ��ư Ŭ���ϸ� ������ ���̾�α�
	IDX_GAMEURL_LOBBY_EVENT,		//�����ϴ� �̺�Ʈ
	IDX_GAMEURL_LOBBY_SHOP,			//�����ϴ� ��õ��ǰ
	IDX_GAMEURL_BADUSER,			//�Ű��ϱ� ������
	IDX_GAMEURL_PCROOM_ADVANTAGE,   //PC�� ���ú��� ������
	IDX_GAMEURL_GAMEMAINPAGE,		//����ȭ�� ������ ���� �� URL
	IDX_GAMEURL_GAMEMAINPAGE_NATE,		//����ȭ�� ������ ���� �� URL
	IDX_GAMEURL_ENDPOPUP,			//�����˾�
	IDX_GAMEURL_EVENT_PCPROMOTION_LOBBY_TOP,  //PC�� ��� ���θ�� ���� ���
	IDX_GAMEURL_GAMEMAINPAGE_TOCTOC,	//����ȭ�� ������ ���� �� URL
	IDX_GAMEURL_EVENTPAGE,			//���� �������� �̺�Ʈ ������
	IDX_GAMEURL_TIMELIMITAVATAR,	//2013�� 12���� Ư�� �ð��� �ǸŵǴ� ���� �ƹ�Ÿ ������
	IDX_GAMEURL_POSTBOX,
	IDX_GAMEURL_POSTBOX_SEND_MESSAGE,
	IDX_GAMEURL_POSTBOX_URL,
	IDX_GAMEURL_CHICKEN_URL,
	IDX_GAMEURL_CHICKEN_REWARD,
	IDX_GAMEURL_GOLD_BIGWHEEL,
	IDX_GAMEURL_CAPTCHA,
	IDX_GAMEURL_CAFE,
	IDX_GAMEURL_FREECHARGE_SHOP,
	MAX_GAMEURL,
};

enum IDX_GAMEWHERE
{
	IDX_GAMEWHERE_UNKNOW = -1,
	IDX_GAMEWHERE_FIRST,			//�α��� �ϱ���
	IDX_GAMEWHERE_LOGIN,			//�α��� ��
	IDX_GAMEWHERE_TITLE,			//
	IDX_GAMEWHERE_LOBY,				//�κ�
	IDX_GAMEWHERE_GAME				//����
};

enum GAMECHOICETYPE
{
	NORMAL_GAME=0,
	CHOICE_GAME
};

// �� ������ �߼�~ ���� ä�� ���� ���� ���� ������ ���� �Լ�
enum{
	en_ADD_MANUAL_POPCLOSE= 0,	//�˾� ���� ���� ����
	en_ADD_MANUAL,		//���� ����
	en_DEL_MANUAL,		//���� ����
	en_POPCLOSE,		//�˾� ����
};

typedef	struct {	
	char	Macrostr[10][40];	
}CHAT_MACRO;

typedef struct  {
	int nGameCode;			//������ ���� ���(���ӽ� ���� ä�� ��)
	int nGameMode;			//������ ���� ���(���ӽ� ���� ä�� ��)
	int MoneyLimitKind;		//������ ���� ���� ���� ���� �Ӵ�.
	int nGroupCode;

	void Clear(){
		nGroupCode = 0;		//�׷� �ڵ�
		nGameCode = 0;
		MoneyLimitKind = 0;
	}	
}MYGAMEINOF;

typedef struct  {
	float nCardSpeed;				// ī�� �̵� �ӵ� ��		
	int	  nCardFlyDelay;			// ī�� �����ٶ� ���徿 ������ �����̰� 
	int   nCardDistributeDelay;	// ī�� �й� �Ҷ� ������ ��	
	int   nBetCmdActiveDelay;		// ���� ��ư Ȱ��ȭ ������
	int   nBetCmdActiveDelayInAutoBet;		// �ڵ����ý� ���ù�ư Ȱ��ȭ ������
	int   nAutoBetActiveDelay;	// �ڵ� ����  ������

#define MIN_VALUE_CARDSPEED  (0.01f)
#define MAX_VALUE_CARDSPEED  (50.0f)

#define _MIN_VALUE	0
#define _MAX_VALUE	100

	void Clear(){
		nCardSpeed = 0.0f;				// ī�� �̵� �ӵ� ��		
		nCardFlyDelay = 0;			// ī�� �����ٶ� ���徿 ������ �����̰� 
		nCardDistributeDelay = 0;	// ī�� �й� �Ҷ� ������ ��	
		nBetCmdActiveDelay = 0;		// ���� ��ư Ȱ��ȭ ������
		nBetCmdActiveDelayInAutoBet = 0;
		nAutoBetActiveDelay = 0;	// �ڵ� ����  ������
	}

	float check_Limit_Value( float value ){
		float return_value = value;
		if( return_value >= MAX_VALUE_CARDSPEED ){
			return_value = MAX_VALUE_CARDSPEED;
		}
		else if( return_value <= MIN_VALUE_CARDSPEED ) {
			return_value = MIN_VALUE_CARDSPEED;
		}
		return return_value;
	}

	int   check_Limit_Value( int value ){
		int return_value = value;
		if( return_value >= _MAX_VALUE ){
			return_value = _MAX_VALUE;
		}
		else if( return_value <= _MIN_VALUE ) {
			return_value = _MIN_VALUE;
		}
		return return_value;
	}

	void set_CardSpeed( float value ){
		nCardSpeed += value;
		nCardSpeed = check_Limit_Value(nCardSpeed);
	}

	void set_CardFlyDelay( int value ){
		nCardFlyDelay += value;
		nCardFlyDelay = check_Limit_Value(nCardFlyDelay);
	}

	void set_CardDistributeDelay( int value ){
		nCardDistributeDelay += value;
		nCardDistributeDelay = check_Limit_Value(nCardDistributeDelay);
	}

	void set_BetCmdActiveDelay( int value ){
		nBetCmdActiveDelay = value;
		nBetCmdActiveDelay = check_Limit_Value(nBetCmdActiveDelay);
	}	

	void set_BetCmdActiveDelayInAutoBet( int value ){
		nBetCmdActiveDelayInAutoBet = value;
		nBetCmdActiveDelayInAutoBet = check_Limit_Value(nBetCmdActiveDelayInAutoBet); 
	}

	void set_AutoBetActiveDelay( int value ){
		nAutoBetActiveDelay += value;
		nAutoBetActiveDelay = check_Limit_Value(nAutoBetActiveDelay);
	}	
} CONTROL_GAMESPEED;

typedef struct  {
	CONTROL_GAMESPEED		Ctr_GameSpeed;

	/*
	1 : ī�� �̵� ���ǵ�~
	2 : ī�� ���� �ִ� �� ( ���� ������ �� ���� ������ �� )
	3 : �ڵ� ���� �ӵ�	
	*/
	int   nDebug_ModeKind;			// 1 ī�� �̵�  ���ǵ�
	BOOL  bShowBetMoney;			// ���� �ݾ� ǥ��

	void Clear()
	{	
		Ctr_GameSpeed.Clear();
	}
		
	void set_CardSpeed( int gamerecode , int game , float value ){
		Ctr_GameSpeed.set_CardSpeed(value);
	}

	void set_CardFlyDelay( int gamerecode , int game , int value ){
		Ctr_GameSpeed.set_CardFlyDelay(value);
	}

	void set_CardDistributeDelay( int gamerecode , int game , int value ){
		Ctr_GameSpeed.set_CardDistributeDelay(value);
	}

	void set_BetCmdActiveDelay( int gamerecode , int game , int value ){
		Ctr_GameSpeed.set_BetCmdActiveDelay(value);
	}	

	void set_AutoBetActiveDelay( int gamerecode , int game , int value ){
		Ctr_GameSpeed.set_AutoBetActiveDelay(value);
	}	

	void set_BetCmdActiveDelayInAutoBet( int gamerecode , int game , int value ){
		Ctr_GameSpeed.set_BetCmdActiveDelayInAutoBet(value);
	}

	float get_CardSpeed( int gamerecode , int game  )
	{
		
		return Ctr_GameSpeed.nCardSpeed;
	}

	int get_CardFlyDelay( int gamerecode , int game  ){
		return Ctr_GameSpeed.nCardFlyDelay;
	}

	int get_CardDistributeDelay( int gamerecode , int game  ){
		return Ctr_GameSpeed.nCardDistributeDelay;
	}

	int get_BetCmdActiveDelay( int gamerecode , int game  ){
		return Ctr_GameSpeed.nBetCmdActiveDelay;
	}	

	int get_BetCmdActiveDelayInAutoBet( int gamerecode, int game ){
		return Ctr_GameSpeed.nBetCmdActiveDelayInAutoBet;
	}

	int get_AutoBetActiveDelay( int gamerecode , int game  ){
		return Ctr_GameSpeed.nAutoBetActiveDelay;
	}	

} GAMESPEED;

static const char rulename[20][50] = { "TT","ž", "�����", "�����", "Ʈ����", "��", "����", "����ƾ", "�÷���", "��", "��ī��", "��Ƽ��", "�齺Ƽ��","��Ƽ��", "��Ƽ��", "��Ƽ��","��Ƽ��" };

static const char strCard_Name[52][8] = {	"��2",	"��3",	"��4",	"��5",	"��6",	"��7",	"��8",	"��9",	"��10",	"��J",	"��Q",	"��K",	"��A",   	"��2",	"��3",	"��4",	"��5",	"��6",	"��7",	"��8",	"��9",	"��10",	"��J",	"��Q",	"��K",	"��A", 		"��2",	"��3",	"��4",	"��5",	"��6",	"��7",	"��8",	"��9",	"��10",	"��J",	"��Q",	"��K",	"��A",	"��2",	"��3",	"��4",	"��5",	"��6",	"��7",	"��8",	"��9",	"��10",	"��J",	"��Q",	"��K",	"��A",};

static const char strChip_Name[3][8] = { "����",  "��ĿĨ", "" };
static const char strChip_Count[3][8] = { "",  "Ĩ", "��Ʈ" };

#define GAMEVIEW_WIDTH		(1000)
#define GAMEVIEW_HEIGHT		(740)

#define UM_CLOSEALLPOPUP	(WM_USER+8891)			// ��� �˾��� �ݴ´�.

#define RADIAN 0.0174532888f

#define GAMEVIEW_TIMER (WM_USER+8000)

#define COLOR_565(dw)	((WORD)((dw&0xff)>>3) | (WORD)((dw&0xff00)>>10)<<5 | (WORD)((dw&0xff0000)>>19)<<11)
#define COLOR_555(dw)	(WORD) ( ((WORD)((dw&0xff)>>3) | (WORD)((dw&0xff00)>>11)<<5 | (WORD)((dw&0xff0000)>>19)<<10) )
#define COLOR555(r,g,b)   (WORD) (  (((WORD)(((r)/8)<<10))) | (((WORD)(((g)/8)<<5))) | ((WORD)(b)/8)  )
#define COLOR565(r,g,b)	(WORD) (  (((WORD)(((r)/8)<<11))) | (((WORD)(((g)/4)<<5))) | ((WORD)(b)/8)  )

#define Randomize() srand((unsigned)time(NULL))
#define Random(num) (int)(rand() % (num))
#define RandomA( s, e ) ( (s) + (Random( (e) - (s) + 1 )) ) 
#define MAX_MACRO_NUM (10)
#define LIMITGAMEPLAY_PLAYERNUM (2)


#define FONT_DEFAULT (&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL))
#define COLOR_DEFAULT_POPUP (RGB(102,103,104))

#define MINIMUM_MONEY_FOR_PLAY (100000000)						// �÷����ϱ����� �ּҸӴ� (1��)

#define MONEY_SEPERATOR_FOR_VIP_5000UK (500000000000)					//VIP������ ���ظӴ� (1��)
#define MONEY_SEPERATOR_FOR_VIP (1000000000000)					//VIP������ ���ظӴ� (1��)
#define MONEY_SEPERATOR_FOR_VIP_DIA (3000000000000)					//VIP������ ���ظӴ� (1��)
#define MONEY_DISABLE_CAHT (300000000000000)					//ä�� Ȱ��/��Ȱ�� ��Ű�� ����
#define GOLD_SEPERATOR_FOR_VIP_SEED (10000)						//��� �Ϲ��� �õ�Ӵ� (1��) �̸�
#define GOLD_SEPERATOR_FOR_VIP_ENTER (500000)					//��� VIP�� �������� (50��) �̻� (50�� �������� 5000�õ���� ���ʿ� ���� 2018.9)

//VIP �� üũ ��ũ��

//#if defined(_GAMECHANNEL_CHANGE)
#define IsVIPRoom(pRoomInfo)  ( ( ( (pRoomInfo)->nGameIndex==IDX_GAME_SP \
							   || (pRoomInfo)->nGameIndex==IDX_GAME_BD || (pRoomInfo)->nGameIndex==IDX_GAME_NP || (pRoomInfo)->nGameIndex==IDX_GAME_HL ) ) \
							   ? ((pRoomInfo)->llEnterLimitMoney>=MONEY_SEPERATOR_FOR_VIP_5000UK):((pRoomInfo)->llEnterLimitMoney>=MONEY_SEPERATOR_FOR_VIP) )
#define VIPRoom_Money(gameindex)  (long long)( ( ( (gameindex)==IDX_GAME_SP \
							   || (gameindex)==IDX_GAME_BD || (gameindex)==IDX_GAME_NP || (gameindex)==IDX_GAME_HL ) ) \
							   ? MONEY_SEPERATOR_FOR_VIP_5000UK:MONEY_SEPERATOR_FOR_VIP )

#define MAX_DUMMYROOMKEY			(1000)						//���̹��̾��� ��Ű ���� 0~1000 
#define IsDummyRoomKey(a) ((a >= 0) &&  (a <= MAX_DUMMYROOMKEY))

#define IsAbleToVIPCounsel(nPremCode) (nPremCode==CODE_NEW_FAMILY_DIAMOND)

#define GDK_GAMECODE			"g_gamecode"

#define GDK_NOTICE_URL			"g_notice_url"
#define GDK_LOCATION_URL		"g_location_url"
#define GDK_AVATARITEM_URL		"g_avataritem_url"
#define GDK_FUNCTIONITEM_URL	"g_functionitem_url"
#define GDK_MYITEM_URL			"g_myitem_url"
#define GDK_BODYGUARD_URL		"g_bodyguard_url"
#define GDK_CASHCHARGE_URL		"g_cashcharge_url"
#define GDK_LOBBYBANNER_URL		"g_lobbybanner_url"
#define GDK_GAMEBANNER_URL		"g_gamebanner_url"
#define GDK_LEADERS_NORMAL_URL	"g_leaders_normal_url"
#define GDK_LEADERS_PLUS_URL	"g_leaders_plus_url"

#define GDK_JACKPOT_MONEY		"g_jackpotmoney"
#define GDK_JACKPOT_MONEY_TEMP	"g_jackpotmoney_temp"
#define GDK_JACKPOT_RELEASE		"g_jackpotrelease"

// [������ ����ȭ ����]
#define GDK_SSOWEB_URL			"g_ssoweb_url"
#define GDK_SSOWEB_TYPE			"g_ssoweb_type"
#define GDK_SSOWEB_ITEMCODE		"g_ssoweb_itemcode"
#define GDK_SSOWEB_STATUS		"g_ssoweb_status"
#define GDK_SSOWEB_ITEMNO		"g_ssoweb_itemno"

#define GDK_OBSERVER_INFO_MSG	"g_observer_info_msg"
#define GDK_OBSERVER_STEP		"g_observer_step"

#define GDK_MAX_PREMIUMMONEY_WITH_OPTION "g_MaxPremiumMoneyWithOption"


/**
 *	Socket Name(������ ����Ϸ��� 32�� ���ܷ� �̸��� �߰��ϼ���.)
**/
// #define MASTERSOCK	"mastersock"		// ó�� �α��ο��� ����Ѵ�.
//#define GAMESOCK	"gamesock"			// �κ�/���ӿ��� ����Ѵ�.
//#define VIPSOCK		"vipsock"			// Vip���� ����Ѵ�.