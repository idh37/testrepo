#pragma once

/********************************************************************
	created:	2010/06/17
	created:	17:6:2010   15:01
	file Path:  \CommonFile
	file base:	PromotionMsgHeader
	file ext:	h
	author:		CHOI YEONSIK
	
	purpose:	패킷용 구조체
				GameServer <-> MasterServer
*********************************************************************/

#include <PromotionDefine.h>
#include <GameCommon.h>

namespace promotion
{

/////////////////////////////////
//채널이동( 서버 이동 )시 이벤트 정보
/////////////////////////////////

/*
사용예
typedef struct  
{
	bool bPromotion;

	bool bTargetUser;
	
} PROMOTION_EXAMPLE;
*/

/*
PC방 활성화
*/
typedef struct  
{
	bool	bPromotion;

	int		nPcRoomCode;	// PC방 코드
	int		nPlayRoundCnt;	// 플레이카운트
	int		nApplyCnt;		// 응모권	
	int		nTodayApplyCnt; // 오늘 총 응모권
	int		nReadDBlApplyCnt;// DB에서 읽어온 응모권
	bool	bApplyLimit;	// 100개 초과
	bool	bDormantUser;	// 휴면유저

} PROMOTION_PCROOMACT;

/*
PC방 포커 존
*/
typedef struct  
{
	bool	bPromotion;

	int     nlimitTodaySS;
	int     nBackuplimitTodaySS;  //로그인시 시간
	int     nUsinglimitTodaySS;   //사용한 시간
	char	szUniqDI[ 65 ];

} PROMOTION_PCZONE;


/*
배팅머니 프로모션
*/
typedef struct  
{
	bool	bPromotion;

	SYSTEMTIME	LastUdtTime; // 마지막 리셋 타임
	int			nPlayRoundCnt;	// 플레이카운트
	int			nApplyCnt;		// 응모권	
	int			nTodayApplyCnt; // 오늘 총 응모권
	int			nReadDBlApplyCnt;// DB에서 읽어온 응모권
	bool		bApplyLimit;	// 100개 초과
	bool		bDormantUser;	// 휴면유저
	int			nTodayLoseCnt;		// 오늘 소멸예정 포인트
	int			nTomorrowLoseCnt;	// 명일 소멸예정 포인트
	// 하루 최대 5개
	// 해당일 신규/휴면 유저의 경우 최대 15개
	bool		bDormantLimit;  

} PROMOTION_BETTING;

/*
리텐션 프로모션
*/



typedef struct 
{
	int nUserState;
	bool bPromotion;

	SYSTEMTIME LastUdtTime;	// 마지막 리셋 타임
	bool bTrial;
	bool bToday;
	bool bTotal;
	int nTodayWinCnt;
	int nTodayWinCntLowRoom ; 
	int nTodayMissionCompleteCnt;
	int nTotalWinCnt;
	int nTotalWinCntLowRoom ; 
	int nTrialDayCnt;

	int nDbTodayWinCnt;	// DB에서 읽어온 매일 10승 승수
	int nDbTodayWinCntLowRoom ; 
	int nDbTodayMissionCompleteCnt;	// DB에서 읽어온 10승 미션 성공 카운트, 최대 10회
	int nDbTotalWinCnt;	// DB에서 읽어온 누적 100승 승수
	int nDbTotalWinCntLowRoom ; 
	int nDbTrialDayCnt;	// DB에서 읽어온 1승 달성 후 Retention 미션 진행 날짜
	BYTE nMissionComplet[MAX_RETENTION_MISSION_DAY];	// 미션 달성  0:없음 , 1:미션 진행중 , 2: 미션 완료.
	BYTE nIsDay;		// 오늘자 미션 슬롯번호
} PROMOTION_RETENTION;

/*
포커 신년 리텐션 프로모션
*/
typedef struct
{
	bool bPromotion;

	SYSTEMTIME LastUdttime;
	int nTodayCnt;
	int nTotalCnt;
	int nTodayWinCntLowRoom;
	int nTodayWinCntHiRoom;
	int nTodayAttendanceCnt;
	int nTotalAttendanceCnt;

	int nDbTodayCnt;
	int nDbTotalCnt;
	int nDbTodayWinCntLowRoom;
	int nDbTodayWinCntHiRoom;
	int nDbTodayAttendanceCnt;
	int nDbTotalAttendanceCnt;
	
} PROMOTION_NEWYEARRETENTION;

typedef struct  
{
	bool bPromotion;

	SYSTEMTIME LastUdttime;
	int nSilverPlayCnt;		// 실버 플래이 판수
	int nDbSilverPlayCnt;
	int nSilverTodayPoint;	// 실버 금일 획득 코인
	int nDbSilvertodayPoint;
	int nSilverTotalPoint;	// 실버 누적 획득 코인
	int nDbSilverTotalPoint;

	int nGoldPlayCnt;			// 골드 플래이 판수
	int nDbGoldPlayCnt;
	int nGoldTodayPoint;		// 골드 금일 획득 코인
	int nDbGoldTodayPoint;
	int nGoldTotalPoint;		// 골드 누적 획득 코인
	int nDbGoldTotalPoint;

	int nNormalPlayCnt;		// 노멀 플래이 판수
	int nDbNormalPlayCnt;
	int nNormalTodayPoint;	// 노멀 금일 획득 코인
	int nDbNormaltodayPoint;
	int nNormalTotalPoint;	// 노멀 누적 획득 코인
	int nDbNormalTotalPoint;

	int nFirstPlayCnt;		// 첫번째 코인 획득에 필요한 판수
	int nSecondPlayCnt;		// 두번째부터 코인 획득에 필요한 판수

	int nPlayCnt;			// 위의 nSilverPlayCnt,nGoldPlayCnt,nNormalPlayCnt  대신 이걸 기본으로 사용한다.
} PROMOTION_USERCARE;

typedef struct 
{
	bool bPromotion;

	SYSTEMTIME LastUdttime;

	int nLowPlayCount;
	int nDbLowPlayCount;
	int nGoldWinCount;
	int nDbGoldWinCount;

	int nHighPlayCount;
	int nDbHighPlayCount;
	int nHighWinCount;
	int nDbHighWinCount;
	bool bAcquireTicket;
} PROMOTION_GOLDCARD;

#define POKERGIRL_MAX_STEP	4

typedef struct 
{
	bool bPromotion;

	SYSTEMTIME LastUdttime;

	int nPlayCount;
	int nWinCount;
	int iJokbo;
	int nContWinCount;

	int iStep;
	int iStepDb;
	int iRoomType;

	int iCard[POKERGIRL_MAX_STEP];

	int iPlayMission;
	int iWinMission;
	int iJokboMission;
	int iContWinMission;

	bool bInitToday;

} PROMOTION_ADULTCARD;

typedef struct 
{
	bool bPromotion;

	SYSTEMTIME LastUdttime;

	int nPlayTimeSec;
	int nDbPlayTimeSec;

	bool bAllinToday;

	int nTotalAllinCount;

} PROMOTION_MISSION003;

typedef struct 
{
	bool bPromotion;

	SYSTEMTIME LastUdttime;

	int nWinCount;
	int nDbWinCount;

	bool bAllinToday;

	int nTotalAllinCount;

} PROMOTION_POKERADVENTURE;

typedef struct 
{
	bool bPromotion;

	SYSTEMTIME LastUdttime;

	int nGoal[MISSION_STEP_MAX];		// 단계별 달성 목표
	int nState[MISSION_STEP_MAX];		// 단계별 현재 상태
	int iCard[MISSION_STEP_MAX];		// 현 단계 달성시 발급될 카드
	int iRoomType[MISSION_STEP_MAX];	// 0:해당없음, 1:하위방, 2:상위방

	int iStep;				// 미션 단계 (1 ~ 4)
	int iStepDb;			// 접속했을때 진행중인 단계

	int nAttandanceCount;

	bool bInitToday;

} PROMOTION_MISSION201304;

typedef struct 
{
	bool bPromotion;

	SYSTEMTIME LastUdttime;

	SYSTEMTIME TodayCompleteTime;
	int nAttandanceCount;
	bool bComplete;

} PROMOTION_BORNIN1996;

typedef struct 
{
	bool bPromotion;

	SYSTEMTIME LastUdttime;

	int nPlayCount;
	int iStep;				// 미션 단계 (1 ~ 4)
	int nTicketCount;
	int nTicketCountDb;

} PROMOTION_LOTTERY201406;

typedef struct 
{
	bool bPromotion;

	int nTodayGoldCnt;
	int nTodaySilverCnt;
	int nGoldCnt;
	int nSilverCnt;

	SYSTEMTIME LastUdttime;
} PROMOTION_HARF;

typedef struct 
{
	bool bPromotion;

	int nWinCnt;
	int nVisiteCnt;
	int nTodayRewardCount;		// 오늘 획득 수
	int nTotalRewardCount;		// 전체 획득 수
	INT64 nRewardMoney;			// 당첨 금

	SYSTEMTIME LastUdttime;
} PROMOTION_VISITE;


// #if defined(__PROMOTION_CHANCEAVATA__) (2011.07.28 찬스아바타프로모션)
typedef struct 
{
	bool	bPromotion;

	SYSTEMTIME	LastUdtTime; // 마지막 리셋 타임


	int nChanceAvata;
} PROMOTION_CHANCEAVATA;
// #endif // (__PROMOTION_CHANCEAVATA__)

typedef struct 
{
	bool bPromotion;
	SYSTEMTIME LastUdttime;
	int iStep;				// 미션 단계 (1 ~ 6)
	int iStepDb;			// 접속했을때 진행중인 단계
	int nAttandanceCount;
	int nPlayCount;
	bool bInitToday;
} PROMOTION_MISSION201502;

typedef struct 
{
	bool bPromotion;
	int nPlayCount;
	int nTotalCoupon;
} PROMOTION_PLAYCOUPON;

////////////////////////////한가위, 유통량확대 2017.08.25/////////////////////////////////////
typedef struct
{
	bool bPromotion;
	int TodayGameCount;
	int CurrentLevel;

	bool RewardOpen[5];

} PROMOTION_SUPERMOON;
/////////////////////////////////////////////////////////////////////

typedef struct
{
	//게임판수로 무언가 진행하는 프로모션용 공통 구조체
	bool bPromotion;
	
	char PM_Name[CHAR_BUFFER_PROMOTION_COMMON_PM_NAME];
	int CurrentGameCount; //현재 게임한 횟수
	int TodayRewardCount; //오늘 보상받은 횟수
	int TicketCount; //응모권등 사용할때까지 초기화가 안되는 재화 갯수

} PROMOTION_TOTAL_GAMECOUNT;

typedef struct
{
	//출석부로 무언가 진행하는 프로모션용 공통 구조체
	bool bPromotion;

	int Attendance_ID; //출석부ID, 출석은 패턴이 동일하므로, 단순 일련번호로 관리하자

	int NormalCount; //일반 출석 횟수
	int ContiCount; //연속 출석 횟수

	int LastReward_NormalCount; //가장 최근에 보상받은 일반출석일
	int LastReward_ContiCount; //가장 최근에 보상받은 연속출석일

} PROMOTION_TOTAL_ATTENDANCE;

//typedef union _U_PROMOTION_INFO
typedef struct _U_PROMOTION_INFO
{
	//채널이동시 해당 유저의 데이터를 DB에 다녀오지 않고 서버간 동기화 하기 위한 구조체 집합이다.
	//따라서 프로모션이 추가되면, 여기에도 구조체를 만들어 넘길 데이터를 넣어 줘야 함

  /*PROMOTION_EXAMPLE   example;  사용예 */
	PROMOTION_PCROOMACT pcAct;
	PROMOTION_PCZONE    pcZone;
	PROMOTION_BETTING   betting;
// #if defined(__PROMOTION_CHANCEAVATA__) (2011.07.28 찬스아바타프로모션)
	PROMOTION_CHANCEAVATA chanceAvata;
// #endif // (__PROMOTION_CHANCEAVATA__)
	PROMOTION_RETENTION retention;
	PROMOTION_NEWYEARRETENTION newYearRetention;
	PROMOTION_USERCARE userCare;
	PROMOTION_GOLDCARD goldCard;
	PROMOTION_ADULTCARD adultCard;
	PROMOTION_MISSION003 mission003;
	PROMOTION_POKERADVENTURE pokerAdventure;
	PROMOTION_MISSION201304 mission201304;
	PROMOTION_BORNIN1996 bornIn1996;
	PROMOTION_LOTTERY201406 lottery201406;
	PROMOTION_HARF promotionHarf;
	PROMOTION_VISITE promotionVisite;
	PROMOTION_MISSION201502 mission201502;
	PROMOTION_PLAYCOUPON promotionPlayCoupon;

	PROMOTION_SUPERMOON promotionSuperMoon;////////////////////////////한가위, 유통량확대 2017.08.25/////////////////////////////////////

	////////////////////////////2018설날이벤트 2018.01.22/////////////////////////////////////
	PROMOTION_TOTAL_GAMECOUNT promotionLuckyBag_2018;
	PROMOTION_TOTAL_GAMECOUNT promotionGoldenLuckyBag_2018;
	PROMOTION_TOTAL_GAMECOUNT promotionChicken_2018;
	PROMOTION_TOTAL_ATTENDANCE promotionAttendance_2018NewYear;
	//이 구조체가 union 으로 선언되어 있어서, 타입이 같은 변수는 같은 메모리를 바라보게 된다....
	//처음엔 판수로 진행하는 프로모션 정보인 위 세개 변수들을 PROMOTION_TOTAL_GAMECOUNT 각각 선언했으나, 세 변수들은 수정할때
	//같은 값으로 동기화 되어버림~~(주의~!!)
	//이 구조체를 구지 UNION으로 가야할 필요가 있을까??? 
	/////////////////////////////////////////////////////////////////////////


} U_PROMOTION_INFO;

typedef struct
{
	int nEventType;			//이벤트 종류

	U_PROMOTION_INFO UPI;   //모든이벤트 구조체

} UNION_PROMOTION_INFO;

/////////////////////////////////
/////////////////////////////////


}

