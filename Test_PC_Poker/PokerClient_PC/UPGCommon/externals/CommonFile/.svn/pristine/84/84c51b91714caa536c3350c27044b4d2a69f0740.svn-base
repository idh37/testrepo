#pragma once

const int SD_EMPTY_CARD = 0;
const int SD_MAX_USER_CARD_COUNT = 3;
const int SD_MAX_CARD_KIND = 10;
const int SD_MAX_CARD_COUNT = 20;
const int SD_TOTAL_USER = 5;

enum RANK_STATE
{
	RANKSTATE_OBSERVER = 0,			// 게임에 참가하지 않은 유저
	RANKSTATE_FOLDER,				// 다이
	RANKSTATE_LOSER,				// 결과에서 패자
	RANKSTATE_WINNER,				// 결과에서 이긴자
};
const int DEFAULT_RANKSTATE = RANKSTATE_LOSER;	// 게임 시작시 모두 LOSER, 게임 플레이중 다이 하면 FOLDER, 결과에서 승자는 WINNER 로 수정

enum CHOICE_STATE
{
	CSTATE_FIRST = 0,			// 패 조합 0,1 선택
	CSTATE_SECOND,				// 패 조합 1,2 선택
	CSTATE_THIRD,				// 패 조합 0,2 선택
	MAX_CSTATE,
};
const int DEFAULT_CHOICE = CSTATE_FIRST;

enum SD_GAMEMODE
{
	SD_2CARD = 0,		// 2장 섯다
	SD_3CARD,			// 3장 섯다
};
const int DEFAULT_GAMEMODE = SD_2CARD;

enum CODE_JOKBO
{
	POINT_DIE = -1,					//다이
	POINT_ZERO = 0,					//망통
	POINT_ONE,						//한끗
	POINT_TWO,						//두끗
	POINT_THREE,					//세끗
	POINT_FOUR,						//네끗
	POINT_FIVE,						//다섯끗
	POINT_SIX,						//여섯끗
	POINT_SEVEN,					//일곱끗
	POINT_EIGHT,					//여덟끗
	POINT_NINE,						//갑오

	POINT_SERYUK,					//세륙
	POINT_JANGSA,					//장사
	POINT_JANGPING,					//장삥
	POINT_GUPING,					//구삥
	POINT_DOKSA,					//독사
	POINT_ALI,						//알리

	POINT_ONEDDANG,					//삥땡
	POINT_TWODDANG,					//이땡
	POINT_THREEDDANG,				//삼땡
	POINT_FOURDDANG,				//사땡
	POINT_FIVEDDANG,				//오땡
	POINT_SIXDDANG,					//육땡
	POINT_SEVENDDANG,				//칠땡
	POINT_EIGHTDDANG,				//팔땡
	POINT_NINEDDANG,				//구땡 
	POINT_TENDDANG,					//장땡

	POINT_13_KWANGDDANG,			//일삼광땡
	POINT_18_KWANGDDANG,			//일팔광땡
	POINT_38_KWANGDDANG,			//삼팔광땡

	POINT_NONESPECIAL,				// 특수족보 없음
	POINT_DDANGKILLER,				//땡잡이
	POINT_PARKMOONSU,				//암행어사
	POINT_SAGU,						//사구 
	POINT_SAGU2,					//사구십자리

	MAX_CODE_JOKBO
};

enum ENTRYSTATE
{
	ESTATE_ALREADY = 0,				// 재경기 원래 참가자
	ESTATE_POSSIBLE,				// 재경기 참가 가능
	ESTATE_IMPOSSIBLE,				// 재경기 참가 불가능(처음부터 게임 참여 안함)
	ESTATE_SHORTPEE,				// 재경기 참가비 부족으로 참가 불가능
	ESTATE_ACCEPT,					// 참가 신청함
	ESTATE_REFUSE,					// 참가 거부
};

enum PENALTY_STATE
{
	PSTATE_NONE = 0,				// 패널티없음
	PSTATE_DDANG,					// 땡값
	PSTATE_GWANGDDANG,				// 광땡, 13, 18
	PSTATE_38GWANGDDANG,			// 38 광땡
	PSTATE_49,						// 49박
	MAX_PSTATE
};

enum REMATCH_STATE
{
	RSTATE_49 = 0,					// 49 재경기
	RSTATE_DRAW,					// 동률승 재경기
	MAX_RSTATE,
};


static int CHOICE_ARRAY[3][2] = {{0,1}, {1,2}, {0,2}};
	//       랭킹 포인트  :   0      1      2      3      4      5        6        7        8        9      10     11     12     13     14     15     16     17     18     19     20     21     22     23     24     25     26     27     28         29              30                31         32     33            34
static char* JokboString[35]={"망통","한끗","두끗","세끗","네끗","다섯끗","여섯끗","일곱끗","여덟끗","갑오","세륙","장사","장삥","구삥","독사","알리","삥땡","이땡","삼땡","사땡","오땡","육땡","칠땡","팔땡","구땡","장땡","광땡","광땡","삼팔광땡","특수족보없음", "삼칠망통-땡잡이","암행어사","사구","사구-십자리","망통"};

#include "UserStruct.h"

#include "SutdaRule.h"
#include "SutdaRuleMan.h"