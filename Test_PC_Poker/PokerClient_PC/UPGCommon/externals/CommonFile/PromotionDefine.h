#pragma once

/********************************************************************
	created:	2010/06/01
	created:	1:6:2010   18:45
	file Path:  \CommonFile
	file base:	PromotionDefine
	file ext:	h
	author:		CHOI YEONSIK
	
	purpose:	�̺�Ʈ ����, Ŭ���̾�Ʈ ����( ���ο� �̺�Ʈ ��Ͻ� �ݵ�� �߰� )
				
				���ο� �̺�Ʈ �߰���
				1) CUser �߰�( �������� ���������� �ʿ��ϴٸ� )
				2) CServerPromotionManager �߰�

�����θ�� �̷¡�

	 �� PC�� Ȱ��ȭ  
		 PMCODE : PM000220
		 PROMOTION_EVENT_PCROOMACT
		 �Ⱓ : 20110315 - 0419
		 ���SP��
			USP_Event_GetIsEntryType2
			USP_Event_PokerCommon_GetPoint
			USP_Event_PokerCommon_SetPoint
			USP_Event_PokerCommon_SetPointLog
	
	 �� ��Ŀ�� 
		 PMCODE : PM000224
		 PROMOTION_EVENT_PCEXPZONE
		 �Ⱓ : 20110419 - 0519
		 ���SP��
			USP_Event_PokerCommon_GetIsAllowIP ��� ������
			USP_Event_PokerCommon_GetTime      ���� �ð�
			USP_Event_PokerCommon_SetTime	   ���� �ð� ����
			USP_Event_PokerCommon_SetTimeLog   �α�

	 �� ��Ŀ�Ӵ� ���� ���θ�� 
		 PMCODE : PM000223
		 PROMOTION_EVENT_BETTING
		 ���� : 2011.04.19(ȭ)~2011.05.16(��)
		 ���SP��
			USP_Event_GetIsEntryType2         �ű�/�޸� -1, 0, 1
			USP_Event_GetIsEntryType2Set	  �ű�/�޸� Ư������ 
			USP_Event_PokerCommon_GetPointBetting
			USP_Event_PokerCommon_SetPointBetting
			USP_Event_PokerCommon_SetPointLog

	

*********************************************************************/

namespace promotion
{

#define PMCODE_LEN		( 9 )
#define UMCODE_LEN		( 9 )

enum EPROMOTION_EVENT_SEQ
{
	PROMOTION_EVENT_UNKNOWN		= -1,
	PROMOTION_EVENT_JACKPOT,		  //����			���
	PROMOTION_EVENT_GTIME,			  //���Ÿ��		���
	PROMOTION_EVENT_CHAMP,			  //�ٵ��� �����	���
	PROMOTION_EVENT_PCROOMACT,		  //PC�� Ȱ��ȭ		20110315 - 0418
	PROMOTION_EVENT_PCEXPZONE,		  //PC�� ��Ŀ��		20110419 - 0516
	PROMOTION_EVENT_BETTING,		  //��Ŀ�Ӵ� ����	20110419 - 0516

// #if defined(__PROMOTION_CHANCEAVATA__) (2011.07.28 �����ƹ�Ÿ���θ��)
	PROMOTION_EVENT_CHANCEAVATA,	  // �����ƹ�Ÿ ���θ�� 20110804 - 0818
// #endif // (__PROMOTION_CHANCEAVATA__)

	PROMOTION_EVENT_RETENTION,		  // �ο�ٵ��� ���ټ����θ�� 201117 - 1229
	PROMOTION_EVENT_NEWYEARRETENTION,	// ��Ŀ �ų� ���ټ� ���θ�� 20120117 - 
	PROMOTION_EVENT_USERCARE,
	PROMOTION_EVENT_GOLDCARD,
	PROMOTION_EVENT_ADULTCARD,
	PROMOTION_EVENT_MISSION003,
	PROMOTION_EVENT_POKERADVENTURE,
	PROMOTION_EVENT_HALFJACKPOT,
	PROMOTION_EVENT_MISSION201304,
	PROMOTION_EVENT_BORNIN1996,
	PROMOTION_EVENT_LOTTERY201406,
	PROMOTION_EVENT_HALF,	//Harf
	PROMOTION_EVENT_VISITE,	//Visite
	PROMOTION_EVENT_MISSION201502,
	PROMOTION_EVENT_PLAY_COUPON,
	PROMOTION_EVENT_VIPJACKPOT,
	PROMOTION_EVENT_SUPERMOON,////////////////////////////�Ѱ���, ���뷮Ȯ�� 2017.08.25/////////////////////////////////////

	////////////////////////////2018�����̺�Ʈ 2018.01.22/////////////////////////////////////
	PROMOTION_EVENT_TOTAL_GAMECOUNT, //�����Ǽ��� ����Ǵ� ���� ���θ�� �ڵ��̴�.
	PROMOTION_EVENT_TOTAL_ATTENDANCE, //�⼮���� ����Ǵ� ���� ���θ�� �ڵ��̴�.
	///////////////////////////////////////////////////////////////////////////

	////////////////////////////���� �������, ��������� 2018.02.27/////////////////////////////////////
	PROMOTION_EVENT_GOLD_HELP, //��� ������ �̺�Ʈ �ڵ��̴�.
	/////////////////////////////////////////////////////////////////////////

	////////////////////////////Ȧ¦�̴ϰ��� �̺�Ʈ 2018.03.12/////////////////////////////////////
	PROMOTION_EVENT_ODD_EVEN_GAME, //Ȧ¦ �̴ϰ��� �̺�Ʈ �ڵ��̴�.
	/////////////////////////////////////////////////////////////////////////

	////////////////////////////�� �̴ϰ��� �̺�Ʈ 2019.07.30/////////////////////////////////////
	PROMOTION_EVENT_WHEEL_GAME, //�� �̴ϰ��� �̺� �ڵ��̴�.
	/////////////////////////////////////////////////////////////////////////

	////////////////////////////�̴ϰ��� ���� 2018.06.04/////////////////////////////////////
	PROMOTION_EVENT_BIG_WHEEL, // �̴ϰ��� ���� �̺�Ʈ �ڵ�.
	/////////////////////////////////////////////////////////////////////////

	////////////////////////////2018 �߼� �̺�Ʈ. ���� �⼮ & �Ǽ� ���� 2018.09.20/////////////////////////////////////
	PROMOTION_EVENT_THANKSGIVING, 
	/////////////////////////////////////////////////////////////////////////

	PROMOTION_EVENT_2019HAVEST, // 2019 �߼� �̺�Ʈ

	PROMOTION_EVENT_ALL				
};

enum PROMOTION_MISSION_STEP
{
	MISSION_STEP_NONE = 0,
	MISSION_PLAYROUND_STEP,
	MISSION_WIN_STEP,
	MISSION_JOKBO_STEP,
	MISSION_ALLIN_STEP,

	MISSION_STEP_LAST,

	// ���� ���Ե��� �ʴ�
	MISSION_CONT_WIN_STEP,
	MISSION_PLAYTIME_STEP,
	MISSION_STEP_MAX
};

#define LOTTERY201406_MISSION_STEP_MAX	6

class IPromotion
{
public:
	virtual bool IsPromotion() = 0;
	virtual void SetPromotion( bool bPromotion ) = 0;

protected:
	WORD m_wdEventType;
};

#define MISSION_201502_STEP_MAX	6
#define MISSION_201502_REWARD_STEP_MAX	11
#define MAX_RETENTION_MISSION_DAY 10
}


