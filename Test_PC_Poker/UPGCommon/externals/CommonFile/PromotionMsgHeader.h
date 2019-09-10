#pragma once

/********************************************************************
	created:	2010/06/17
	created:	17:6:2010   15:01
	file Path:  \CommonFile
	file base:	PromotionMsgHeader
	file ext:	h
	author:		CHOI YEONSIK
	
	purpose:	��Ŷ�� ����ü
				GameServer <-> MasterServer
*********************************************************************/

#include <PromotionDefine.h>
#include <GameCommon.h>

namespace promotion
{

/////////////////////////////////
//ä���̵�( ���� �̵� )�� �̺�Ʈ ����
/////////////////////////////////

/*
��뿹
typedef struct  
{
	bool bPromotion;

	bool bTargetUser;
	
} PROMOTION_EXAMPLE;
*/

/*
PC�� Ȱ��ȭ
*/
typedef struct  
{
	bool	bPromotion;

	int		nPcRoomCode;	// PC�� �ڵ�
	int		nPlayRoundCnt;	// �÷���ī��Ʈ
	int		nApplyCnt;		// �����	
	int		nTodayApplyCnt; // ���� �� �����
	int		nReadDBlApplyCnt;// DB���� �о�� �����
	bool	bApplyLimit;	// 100�� �ʰ�
	bool	bDormantUser;	// �޸�����

} PROMOTION_PCROOMACT;

/*
PC�� ��Ŀ ��
*/
typedef struct  
{
	bool	bPromotion;

	int     nlimitTodaySS;
	int     nBackuplimitTodaySS;  //�α��ν� �ð�
	int     nUsinglimitTodaySS;   //����� �ð�
	char	szUniqDI[ 65 ];

} PROMOTION_PCZONE;


/*
���øӴ� ���θ��
*/
typedef struct  
{
	bool	bPromotion;

	SYSTEMTIME	LastUdtTime; // ������ ���� Ÿ��
	int			nPlayRoundCnt;	// �÷���ī��Ʈ
	int			nApplyCnt;		// �����	
	int			nTodayApplyCnt; // ���� �� �����
	int			nReadDBlApplyCnt;// DB���� �о�� �����
	bool		bApplyLimit;	// 100�� �ʰ�
	bool		bDormantUser;	// �޸�����
	int			nTodayLoseCnt;		// ���� �Ҹ꿹�� ����Ʈ
	int			nTomorrowLoseCnt;	// ���� �Ҹ꿹�� ����Ʈ
	// �Ϸ� �ִ� 5��
	// �ش��� �ű�/�޸� ������ ��� �ִ� 15��
	bool		bDormantLimit;  

} PROMOTION_BETTING;

/*
���ټ� ���θ��
*/



typedef struct 
{
	int nUserState;
	bool bPromotion;

	SYSTEMTIME LastUdtTime;	// ������ ���� Ÿ��
	bool bTrial;
	bool bToday;
	bool bTotal;
	int nTodayWinCnt;
	int nTodayWinCntLowRoom ; 
	int nTodayMissionCompleteCnt;
	int nTotalWinCnt;
	int nTotalWinCntLowRoom ; 
	int nTrialDayCnt;

	int nDbTodayWinCnt;	// DB���� �о�� ���� 10�� �¼�
	int nDbTodayWinCntLowRoom ; 
	int nDbTodayMissionCompleteCnt;	// DB���� �о�� 10�� �̼� ���� ī��Ʈ, �ִ� 10ȸ
	int nDbTotalWinCnt;	// DB���� �о�� ���� 100�� �¼�
	int nDbTotalWinCntLowRoom ; 
	int nDbTrialDayCnt;	// DB���� �о�� 1�� �޼� �� Retention �̼� ���� ��¥
	BYTE nMissionComplet[MAX_RETENTION_MISSION_DAY];	// �̼� �޼�  0:���� , 1:�̼� ������ , 2: �̼� �Ϸ�.
	BYTE nIsDay;		// ������ �̼� ���Թ�ȣ
} PROMOTION_RETENTION;

/*
��Ŀ �ų� ���ټ� ���θ��
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
	int nSilverPlayCnt;		// �ǹ� �÷��� �Ǽ�
	int nDbSilverPlayCnt;
	int nSilverTodayPoint;	// �ǹ� ���� ȹ�� ����
	int nDbSilvertodayPoint;
	int nSilverTotalPoint;	// �ǹ� ���� ȹ�� ����
	int nDbSilverTotalPoint;

	int nGoldPlayCnt;			// ��� �÷��� �Ǽ�
	int nDbGoldPlayCnt;
	int nGoldTodayPoint;		// ��� ���� ȹ�� ����
	int nDbGoldTodayPoint;
	int nGoldTotalPoint;		// ��� ���� ȹ�� ����
	int nDbGoldTotalPoint;

	int nNormalPlayCnt;		// ��� �÷��� �Ǽ�
	int nDbNormalPlayCnt;
	int nNormalTodayPoint;	// ��� ���� ȹ�� ����
	int nDbNormaltodayPoint;
	int nNormalTotalPoint;	// ��� ���� ȹ�� ����
	int nDbNormalTotalPoint;

	int nFirstPlayCnt;		// ù��° ���� ȹ�濡 �ʿ��� �Ǽ�
	int nSecondPlayCnt;		// �ι�°���� ���� ȹ�濡 �ʿ��� �Ǽ�

	int nPlayCnt;			// ���� nSilverPlayCnt,nGoldPlayCnt,nNormalPlayCnt  ��� �̰� �⺻���� ����Ѵ�.
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

	int nGoal[MISSION_STEP_MAX];		// �ܰ躰 �޼� ��ǥ
	int nState[MISSION_STEP_MAX];		// �ܰ躰 ���� ����
	int iCard[MISSION_STEP_MAX];		// �� �ܰ� �޼��� �߱޵� ī��
	int iRoomType[MISSION_STEP_MAX];	// 0:�ش����, 1:������, 2:������

	int iStep;				// �̼� �ܰ� (1 ~ 4)
	int iStepDb;			// ���������� �������� �ܰ�

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
	int iStep;				// �̼� �ܰ� (1 ~ 4)
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
	int nTodayRewardCount;		// ���� ȹ�� ��
	int nTotalRewardCount;		// ��ü ȹ�� ��
	INT64 nRewardMoney;			// ��÷ ��

	SYSTEMTIME LastUdttime;
} PROMOTION_VISITE;


// #if defined(__PROMOTION_CHANCEAVATA__) (2011.07.28 �����ƹ�Ÿ���θ��)
typedef struct 
{
	bool	bPromotion;

	SYSTEMTIME	LastUdtTime; // ������ ���� Ÿ��


	int nChanceAvata;
} PROMOTION_CHANCEAVATA;
// #endif // (__PROMOTION_CHANCEAVATA__)

typedef struct 
{
	bool bPromotion;
	SYSTEMTIME LastUdttime;
	int iStep;				// �̼� �ܰ� (1 ~ 6)
	int iStepDb;			// ���������� �������� �ܰ�
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

////////////////////////////�Ѱ���, ���뷮Ȯ�� 2017.08.25/////////////////////////////////////
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
	//�����Ǽ��� ���� �����ϴ� ���θ�ǿ� ���� ����ü
	bool bPromotion;
	
	char PM_Name[CHAR_BUFFER_PROMOTION_COMMON_PM_NAME];
	int CurrentGameCount; //���� ������ Ƚ��
	int TodayRewardCount; //���� ������� Ƚ��
	int TicketCount; //����ǵ� ����Ҷ����� �ʱ�ȭ�� �ȵǴ� ��ȭ ����

} PROMOTION_TOTAL_GAMECOUNT;

typedef struct
{
	//�⼮�η� ���� �����ϴ� ���θ�ǿ� ���� ����ü
	bool bPromotion;

	int Attendance_ID; //�⼮��ID, �⼮�� ������ �����ϹǷ�, �ܼ� �Ϸù�ȣ�� ��������

	int NormalCount; //�Ϲ� �⼮ Ƚ��
	int ContiCount; //���� �⼮ Ƚ��

	int LastReward_NormalCount; //���� �ֱٿ� ������� �Ϲ��⼮��
	int LastReward_ContiCount; //���� �ֱٿ� ������� �����⼮��

} PROMOTION_TOTAL_ATTENDANCE;

//typedef union _U_PROMOTION_INFO
typedef struct _U_PROMOTION_INFO
{
	//ä���̵��� �ش� ������ �����͸� DB�� �ٳ���� �ʰ� ������ ����ȭ �ϱ� ���� ����ü �����̴�.
	//���� ���θ���� �߰��Ǹ�, ���⿡�� ����ü�� ����� �ѱ� �����͸� �־� ��� ��

  /*PROMOTION_EXAMPLE   example;  ��뿹 */
	PROMOTION_PCROOMACT pcAct;
	PROMOTION_PCZONE    pcZone;
	PROMOTION_BETTING   betting;
// #if defined(__PROMOTION_CHANCEAVATA__) (2011.07.28 �����ƹ�Ÿ���θ��)
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

	PROMOTION_SUPERMOON promotionSuperMoon;////////////////////////////�Ѱ���, ���뷮Ȯ�� 2017.08.25/////////////////////////////////////

	////////////////////////////2018�����̺�Ʈ 2018.01.22/////////////////////////////////////
	PROMOTION_TOTAL_GAMECOUNT promotionLuckyBag_2018;
	PROMOTION_TOTAL_GAMECOUNT promotionGoldenLuckyBag_2018;
	PROMOTION_TOTAL_GAMECOUNT promotionChicken_2018;
	PROMOTION_TOTAL_ATTENDANCE promotionAttendance_2018NewYear;
	//�� ����ü�� union ���� ����Ǿ� �־, Ÿ���� ���� ������ ���� �޸𸮸� �ٶ󺸰� �ȴ�....
	//ó���� �Ǽ��� �����ϴ� ���θ�� ������ �� ���� �������� PROMOTION_TOTAL_GAMECOUNT ���� ����������, �� �������� �����Ҷ�
	//���� ������ ����ȭ �Ǿ����~~(����~!!)
	//�� ����ü�� ���� UNION���� ������ �ʿ䰡 ������??? 
	/////////////////////////////////////////////////////////////////////////


} U_PROMOTION_INFO;

typedef struct
{
	int nEventType;			//�̺�Ʈ ����

	U_PROMOTION_INFO UPI;   //����̺�Ʈ ����ü

} UNION_PROMOTION_INFO;

/////////////////////////////////
/////////////////////////////////


}

