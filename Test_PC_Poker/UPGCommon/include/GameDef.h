// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.

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
	IDX_GAMEURL_GAMEMANUAL,			//게임방법창 
	IDX_GAMEURL_LOBBYEVENTDIALOG,	//로비이벤트 버튼 클릭하며 나오는 다이얼로그
	IDX_GAMEURL_LOBBY_EVENT,		//우측하단 이벤트
	IDX_GAMEURL_LOBBY_SHOP,			//우측하단 추천상품
	IDX_GAMEURL_BADUSER,			//신고하기 페이지
	IDX_GAMEURL_PCROOM_ADVANTAGE,   //PC방 혜택보기 페이지
	IDX_GAMEURL_GAMEMAINPAGE,		//바탕화면 아이콘 실행 시 URL
	IDX_GAMEURL_GAMEMAINPAGE_NATE,		//바탕화면 아이콘 실행 시 URL
	IDX_GAMEURL_ENDPOPUP,			//종료팝업
	IDX_GAMEURL_EVENT_PCPROMOTION_LOBBY_TOP,  //PC방 상생 프로모션 대기실 상단
	IDX_GAMEURL_GAMEMAINPAGE_TOCTOC,	//바탕화면 아이콘 실행 시 URL
	IDX_GAMEURL_EVENTPAGE,			//현재 진행중인 이벤트 페이지
	IDX_GAMEURL_TIMELIMITAVATAR,	//2013년 12월에 특정 시간에 판매되는 한정 아바타 페이지
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
	IDX_GAMEWHERE_FIRST,			//로그인 하기전
	IDX_GAMEWHERE_LOGIN,			//로그인 중
	IDX_GAMEWHERE_TITLE,			//
	IDX_GAMEWHERE_LOBY,				//로비
	IDX_GAMEWHERE_GAME				//게임
};

enum GAMECHOICETYPE
{
	NORMAL_GAME=0,
	CHOICE_GAME
};

// 방 생성시 중수~ 도신 채널 까지 수동 포함 미포함 결정 함수
enum{
	en_ADD_MANUAL_POPCLOSE= 0,	//팝업 종료 수동 포함
	en_ADD_MANUAL,		//수동 포함
	en_DEL_MANUAL,		//수동 삭제
	en_POPCLOSE,		//팝업 종료
};

typedef	struct {	
	char	Macrostr[10][40];	
}CHAT_MACRO;

typedef struct  {
	int nGameCode;			//선택한 게임 모드(접속시 값을 채워 옴)
	int nGameMode;			//선택한 게임 모드(접속시 값을 채워 옴)
	int MoneyLimitKind;		//선택한 게임 방의 입장 제한 머니.
	int nGroupCode;

	void Clear(){
		nGroupCode = 0;		//그룹 코드
		nGameCode = 0;
		MoneyLimitKind = 0;
	}	
}MYGAMEINOF;

typedef struct  {
	float nCardSpeed;				// 카드 이동 속도 값		
	int	  nCardFlyDelay;			// 카드 나눠줄때 한장씩 나오는 딜레이값 
	int   nCardDistributeDelay;	// 카드 분배 할때 딜레이 값	
	int   nBetCmdActiveDelay;		// 베팅 버튼 활성화 딜레이
	int   nBetCmdActiveDelayInAutoBet;		// 자동베팅시 베팅버튼 활성화 딜레이
	int   nAutoBetActiveDelay;	// 자동 베팅  딜레이

#define MIN_VALUE_CARDSPEED  (0.01f)
#define MAX_VALUE_CARDSPEED  (50.0f)

#define _MIN_VALUE	0
#define _MAX_VALUE	100

	void Clear(){
		nCardSpeed = 0.0f;				// 카드 이동 속도 값		
		nCardFlyDelay = 0;			// 카드 나눠줄때 한장씩 나오는 딜레이값 
		nCardDistributeDelay = 0;	// 카드 분배 할때 딜레이 값	
		nBetCmdActiveDelay = 0;		// 베팅 버튼 활성화 딜레이
		nBetCmdActiveDelayInAutoBet = 0;
		nAutoBetActiveDelay = 0;	// 자동 베팅  딜레이
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
	1 : 카드 이동 스피두~
	2 : 카드 나눠 주는 텀 ( 한장 나가고 또 한장 나갈때 텀 )
	3 : 자동 배팅 속도	
	*/
	int   nDebug_ModeKind;			// 1 카드 이동  스피드
	BOOL  bShowBetMoney;			// 배팅 금액 표시

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

static const char rulename[20][50] = { "TT","탑", "원페어", "투페어", "트리플", "줄", "백줄", "마운틴", "플러시", "집", "포카드", "스티플", "백스티플","로티플", "로티플", "로티플","로티플" };

static const char strCard_Name[52][8] = {	"♣2",	"♣3",	"♣4",	"♣5",	"♣6",	"♣7",	"♣8",	"♣9",	"♣10",	"♣J",	"♣Q",	"♣K",	"♣A",   	"♥2",	"♥3",	"♥4",	"♥5",	"♥6",	"♥7",	"♥8",	"♥9",	"♥10",	"♥J",	"♥Q",	"♥K",	"♥A", 		"◆2",	"◆3",	"◆4",	"◆5",	"◆6",	"◆7",	"◆8",	"◆9",	"◆10",	"◆J",	"◆Q",	"◆K",	"◆A",	"♠2",	"♠3",	"♠4",	"♠5",	"♠6",	"♠7",	"♠8",	"♠9",	"♠10",	"♠J",	"♠Q",	"♠K",	"♠A",};

static const char strChip_Name[3][8] = { "코인",  "포커칩", "" };
static const char strChip_Count[3][8] = { "",  "칩", "비트" };

#define GAMEVIEW_WIDTH		(1000)
#define GAMEVIEW_HEIGHT		(740)

#define UM_CLOSEALLPOPUP	(WM_USER+8891)			// 모든 팝업을 닫는다.

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

#define MINIMUM_MONEY_FOR_PLAY (100000000)						// 플레이하기위한 최소머니 (1억)

#define MONEY_SEPERATOR_FOR_VIP_5000UK (500000000000)					//VIP나누는 기준머니 (1조)
#define MONEY_SEPERATOR_FOR_VIP (1000000000000)					//VIP나누는 기준머니 (1조)
#define MONEY_SEPERATOR_FOR_VIP_DIA (3000000000000)					//VIP나누는 기준머니 (1조)
#define MONEY_DISABLE_CAHT (300000000000000)					//채팅 활성/비활성 시키는 기준
#define GOLD_SEPERATOR_FOR_VIP_SEED (10000)						//골드 일반은 시드머니 (1만) 미만
#define GOLD_SEPERATOR_FOR_VIP_ENTER (500000)					//골드 VIP는 입장제한 (50만) 이상 (50만 입장제한 5000시드방은 양쪽에 노출 2018.9)

//VIP 방 체크 매크로

//#if defined(_GAMECHANNEL_CHANGE)
#define IsVIPRoom(pRoomInfo)  ( ( ( (pRoomInfo)->nGameIndex==IDX_GAME_SP \
							   || (pRoomInfo)->nGameIndex==IDX_GAME_BD || (pRoomInfo)->nGameIndex==IDX_GAME_NP || (pRoomInfo)->nGameIndex==IDX_GAME_HL ) ) \
							   ? ((pRoomInfo)->llEnterLimitMoney>=MONEY_SEPERATOR_FOR_VIP_5000UK):((pRoomInfo)->llEnterLimitMoney>=MONEY_SEPERATOR_FOR_VIP) )
#define VIPRoom_Money(gameindex)  (long long)( ( ( (gameindex)==IDX_GAME_SP \
							   || (gameindex)==IDX_GAME_BD || (gameindex)==IDX_GAME_NP || (gameindex)==IDX_GAME_HL ) ) \
							   ? MONEY_SEPERATOR_FOR_VIP_5000UK:MONEY_SEPERATOR_FOR_VIP )

#define MAX_DUMMYROOMKEY			(1000)						//더미방이쓰는 룸키 영역 0~1000 
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

// [아이템 유료화 영역]
#define GDK_SSOWEB_URL			"g_ssoweb_url"
#define GDK_SSOWEB_TYPE			"g_ssoweb_type"
#define GDK_SSOWEB_ITEMCODE		"g_ssoweb_itemcode"
#define GDK_SSOWEB_STATUS		"g_ssoweb_status"
#define GDK_SSOWEB_ITEMNO		"g_ssoweb_itemno"

#define GDK_OBSERVER_INFO_MSG	"g_observer_info_msg"
#define GDK_OBSERVER_STEP		"g_observer_step"

#define GDK_MAX_PREMIUMMONEY_WITH_OPTION "g_MaxPremiumMoneyWithOption"


/**
 *	Socket Name(소켓을 사용하려면 32자 내외로 이름을 추가하세요.)
**/
// #define MASTERSOCK	"mastersock"		// 처음 로그인에만 사용한다.
//#define GAMESOCK	"gamesock"			// 로비/게임에서 사용한다.
//#define VIPSOCK		"vipsock"			// Vip에서 사용한다.