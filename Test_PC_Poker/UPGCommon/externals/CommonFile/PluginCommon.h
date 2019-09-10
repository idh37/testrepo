#ifndef __PLUGINCOMMON_H__
#define __PLUGINCOMMON_H__

//�� ���� ���� ���Ǵ�� ���� ���� !!!!!!!!!
//�� ���� ���� ���Ǵ�� ���� ���� !!!!!!!!!


#ifdef NETMARBLEPLUGIN_EXPORTS

#define _DEFINE_NETMARBLEPLUGIN_API(RETTYPE, FUNC, ARG)  extern "C" __declspec(dllexport) RETTYPE FUNC##ARG;

#else

#define _API_TYPE(FUNC) TYPE##FUNC
#define _DEFINE_NETMARBLEPLUGIN_API(RETTYPE, FUNC, ARG)	\
		typedef RETTYPE (*_API_TYPE(FUNC))ARG; 
#define _API_INSTANCE(FUNC) _API_TYPE(FUNC) FUNC=NULL;

#endif


#pragma pack(push, 4)

////////////////////////////////////////////////////////////////////////////////
//																				
//		PLUGIN COMMON API 
//
////////////////////////////////////////////////////////////////////////////////

//BOOL _NMP_LoadConfig(void);
_DEFINE_NETMARBLEPLUGIN_API(BOOL, _NMP_LoadConfig, (void))


////////////////////////////////////////////////////////////////////////////////

//�� ���� ���� ���Ǵ�� ���� ���� !!!!!!!!!
//�� ���� ���� ���Ǵ�� ���� ���� !!!!!!!!!




////////////////////////////////////////////////////////////////////////////////
//																				
//		��Ŀ è�Ǿ� �÷����� ���� 
//
////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////// 
// STRUCTURES & CONSTANTS

//�� ���� ���� ���Ǵ�� ���� ���� !!!!!!!!!
//�� ���� ���� ���Ǵ�� ���� ���� !!!!!!!!!
struct CHAMPRECORD  //��Ŀè�Ǿ� 
{	
	enum { AMA=0, PRO, TOTAL };
	int ChampWin[TOTAL], ChampLose[TOTAL];	// ��Ʋ ����
	int nBattleCurSeq;				// ��Ʋ ������ �ش� ����
	
	int GetBattleIdx(int nType)
	{			
		float bt = (float) (ChampWin[nType]*100)  /  (float)(max(1,ChampWin[nType]+ChampLose[nType])); 
		bt+=0.5;		
		int idx = (int) bt;
		// ��Ʋ���� �ּ� 10 �ִ� 50
		if (idx<10) idx=10;
		if (idx>50) idx=50;
		return  idx;
	}
};

struct ATTENDANCESPPLYSTATEDATA{
	int nApplyState[8];
};

//////////////////////////////////////////////////////////////////////////////// 
// APIs

//�� ���� ���� ���Ǵ�� ���� ���� !!!!!!!!!
//�� ���� ���� ���Ǵ�� ���� ���� !!!!!!!!!

_DEFINE_NETMARBLEPLUGIN_API (BOOL, _NMP_GetChampionLosePoint,  
							 (int *pPoint, CHAMPRECORD* pRecord, INT64 Money,  int Group, int PremLeadersCode, int JoinPlayer, int MaxPlayer, BOOL bUpdateRecord, int reserve) )

_DEFINE_NETMARBLEPLUGIN_API (BOOL, _NMP_GetChampionWinPoint,  
							 (int *pPoint, CHAMPRECORD* pRecord, INT64 Money,  int Group, int PremLeadersCode, int JoinPlayer, int MaxPlayer, BOOL bUpdateRecord, int reserve) )

_DEFINE_NETMARBLEPLUGIN_API (BOOL, _NMP_GetInitialValue,  
							 (int* amaWin, int* amaLose, int* proWin, int *proLose, int *amaBPT, int *proBPT) ) 
	

_DEFINE_NETMARBLEPLUGIN_API (BOOL, _NMP_IsDoubleTimeZone, 
							 (BOOL* bDoubleTimeZone))

_DEFINE_NETMARBLEPLUGIN_API (BOOL, _NMP_IsEventRunning, 
							 (BOOL* bEventRun))

_DEFINE_NETMARBLEPLUGIN_API (BOOL, _NMP_GetLeagueKind, 
							 (int* LeagueKind, int Group))

_DEFINE_NETMARBLEPLUGIN_API (BOOL, _NMP_GetEventSequence, 
							 (int* pSeq))

// just for debug
_DEFINE_NETMARBLEPLUGIN_API (BOOL, _NMP_GetOldBetPoint, 
							 (double *pPoint, INT64 Money, int Group))
 
//�� ���� ���� ���Ǵ�� ���� ���� !!!!!!!!!
//�� ���� ���� ���Ǵ�� ���� ���� !!!!!!!!!

_DEFINE_NETMARBLEPLUGIN_API (BOOL , _NMP_AE_IsEventRunning,
								   (BOOL *bRunnig))

_DEFINE_NETMARBLEPLUGIN_API (BOOL , _NMP_AE_GetCurEventWeek,
								   (int *nWeekKind))

_DEFINE_NETMARBLEPLUGIN_API (BOOL , _NMP_AE_GetContinueAttendance,
								   (COleDateTime *lasttime,int *attendancecontinue ,ATTENDANCESPPLYSTATEDATA *data))

_DEFINE_NETMARBLEPLUGIN_API (BOOL , _NMP_AE_IsEventZoneTime,
								   (BOOL *bEventZoneTime ))

_DEFINE_NETMARBLEPLUGIN_API (BOOL , _NMP_AE_IsNotSupplyEvent,
								   (int *nDay ,BOOL *bIsNotSupply ))

_DEFINE_NETMARBLEPLUGIN_API (BOOL ,_NMP_AE_CheckNewEvnetPromotion,
							 (COleDateTime *date ,BOOL *bIsNotCleardata  ))


_DEFINE_NETMARBLEPLUGIN_API (BOOL ,_NMP_AE_CheckLimitChannel,
							 (int *ChanStyle,BOOL *bLimitChan))					 


								   
#pragma pack(pop)

#endif