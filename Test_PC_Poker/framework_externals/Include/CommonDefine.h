#pragma once

/**
 *	Socket Name(소켓을 사용하려면 32자 내외로 이름을 추가하세요.)
**/
// #define MASTERSOCK	"mastersock"		// 처음 로그인에만 사용한다.
// #define GAMESOCK	"gamesock"			// 로비/게임에서 사용한다.



/**
 *	Game Location
**/
enum EGAMELOCATION
{
	GAMELOCATION_UNKNOW = -1,
	GAMELOCATION_FIRST,			// 로그인 하기전
	GAMELOCATION_LOGIN,			// 로그인 중
	GAMELOCATION_LOADING,		// 로딩 중
	GAMELOCATION_LOBBY,			// 로비
	GAMELOCATION_GAME			// 게임
};


/**
 *	Loading State
**/
enum ELODINGSTATE
{
	ELODINGSTATE_FIRST= 0,
	ELODINGSTATE_CREATE,
	ELODINGSTATE_DIRECTENTER,
	ELODINGSTATE_DIRECTCREATE,
	ELODINGSTATE_ENTERROOM,
	ELODINGSTATE_ENTERDUMMYROOM,
	ELODINGSTATE_PREMQUICKJOIN
};



/**
 *	Data Manager에서 관리하는 Command
 *  현재 게임에 Dependency가 강하기 때문에 정리하는 작업을 한다.
**/

#define DMAN_GAMECODE			"g_gamecode"
#define DMAN_NOTICE_URL			"g_notice_url"
#define DMAN_LOCATION_URL		"g_location_url"
#define DMAN_AVATARITEM_URL		"g_avataritem_url"
#define DMAN_FUNCTIONITEM_URL	"g_functionitem_url"
#define DMAN_MYITEM_URL			"g_myitem_url"
#define DMAN_BODYGUARD_URL		"g_bodyguard_url"
#define DMAN_CASHCHARGE_URL		"g_cashcharge_url"
#define DMAN_LOBBYBANNER_URL	"g_lobbybanner_url"
#define DMAN_GAMEBANNER_URL		"g_gamebanner_url"
#define DMAN_LEADERS_NORMAL_URL	"g_leaders_normal_url"
#define DMAN_LEADERS_PLUS_URL	"g_leaders_plus_url"

#define DMAN_JACKPOT_MONEY		"g_jackpotmoney"
#define DMAN_JACKPOT_MONEY_TEMP	"g_jackpotmoney_temp"
#define DMAN_JACKPOT_RELEASE	"g_jackpotrelease"

// [아이템 유료화 영역]
#define DMAN_SSOWEB_URL			"g_ssoweb_url"
#define DMAN_SSOWEB_TYPE		"g_ssoweb_type"
#define DMAN_SSOWEB_ITEMCODE	"g_ssoweb_itemcode"
#define DMAN_SSOWEB_STATUS		"g_ssoweb_status"
#define DMAN_SSOWEB_ITEMNO		"g_ssoweb_itemno"

#define DMAN_OBSERVER_INFO_MSG	"g_observer_info_msg"
#define DMAN_OBSERVER_STEP		"g_observer_step"
#define DMAN_SHOW_ENDING_BANNER	"g_show_ending_banner"

#define DMAN_MAX_PREMIUMMONEY_WITH_OPTION "g_MaxPremiumMoneyWithOption"