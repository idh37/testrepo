// NewDSLvExpMan.cpp: implementation of the CNewDSLvExpMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "NewDSLvExpMan.h"
#include "LogFile.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
UINT64 g_NewDS_LvList[100] = {
	//	도신경험치	            도신레벨
		0  ,						//	 1		0						
		100000 	 , 					// 	 2		1
		200000 	 , 					// 	 3		2
		300000 	 , 					// 	 4		3
		400000 	 , 					// 	 5		4
		600000 	 , 					// 	 6		5
		800000 	 , 					// 	 7		6
		1000000 	 , 				// 	 8		7
		1500000 	 , 				// 	 9		8
		2000000 	 , 				// 	 10		9	
		3000000 	 , 				// 	 11		10		
		4779000 	 , 				// 	 12		11
		6773436 	 , 				// 	 13		12
		8714979 	 , 				// 	 14		13
		10651725 	 , 				// 	 15		14
		12657638 	 , 				// 	 16		15
		14773635 	 , 				// 	 17		16
		17024781 	 , 				// 	 18		17
		19431381 	 , 				// 	 19		18
		22424286 	 , 				// 	 20		19
		25723937 	 , 				// 	 21		20		
		29343223 	 , 				// 	 22		21
		33318924 	 , 				// 	 23		22
		37693129 	 , 				// 	 24		23
		42512731 	 , 				// 	 25		24
		47830092 	 , 				// 	 26		25
		53703820 	 , 				// 	 27		26
		60199634 	 , 				// 	 28		27
		67391328 	 , 				// 	 29		28
		77417195 	 , 				// 	 30		29
		88858108 	 , 				// 	 31		30		
		101886668 	 , 				// 	 32		31
		116736718 	 , 				// 	 33		32
		133678719 	 , 				// 	 34		33
		153025069 	 , 				// 	 35		34
		175136883 	 , 				// 	 36		35
		200431930 	 , 				// 	 37		36
		229393898 	 , 				// 	 38		37
		262583233 	 , 				// 	 39		38
		313836220 	 , 				// 	 40		39
		375202406 	 , 				// 	 41		40
		448666347 	 , 				// 	 42		41
		536685539 	 , 				// 	 43		42
		642230772 	 , 				// 	 44		43
		768895783 	 , 				// 	 45		44
		921031239 	 , 				// 	 46		45
		1103908234 	 , 				// 	 47		46
		1323917936 	 , 				// 	 48		47
		1588815567 	 , 				// 	 49		48
		2035220035 	 , 				// 	 50		49
		2608757784 	 , 				// 	 51		50	
		3346070755 	 , 				// 	 52		51
		4294654392 	 , 				// 	 53		52
		5515984243 	 , 				// 	 54		53
		7089690185 	 , 				// 	 55		54
		9118994473 	 , 				// 	 56		55
		11737801269 	 , 			// 	 57		56
		15119945924 	 , 			// 	 58		57
		19491270711 	 , 			// 	 59		58
		27484509347 	 , 			// 	 60		59
		38782632330 	 , 			// 	 61		60
		54763069284 	 , 			// 	 62		61
		77382212127 	 , 			// 	 63		62
		109420445982 	 , 			// 	 64		63
		154831931911 	 , 			// 	 65		64
		219244019251 	 , 			// 	 66		65
		310670781770 	 , 			// 	 67		66
		440533177868 	 , 			// 	 68		67
		625118591546 	 , 			// 	 69		68
		993940816776 	 , 			// 	 70		69
		1581362369186 	 , 			// 	 71		70
		2517531424620 	 , 			// 	 72		71
		4010430095477 	 , 			// 	 73		72
		6392628111435 	 , 			// 	 74		73
		10196244380169 	 , 			// 	 75		74
		16273208576371 	 , 			// 	 76		75
		25988316645277 	 , 			// 	 77		76
		41529332551159 	 , 			// 	 78		77
		66405405304505 	 , 			// 	 79		78
		110897029528853 	 , 		// 	 80		79	
		185308939133283 	 , 		// 	 81		80
		309836549024761 	 , 		// 	 82		81
		518356549315682 	 , 		// 	 83		82
		867728866355053 	 , 		// 	 84		83
		1453445853948660 	 , 		// 	 85		84
		2435975254025260 	 , 		// 	 86		85
		4085130503811020 	 , 		// 	 87		86
		6854848988208890 	 , 		// 	 88		87
		11509291454020100 	 , 		// 	 89		88
		19335609645574500 	 , 		// 	 90		89
		32503159817034800 	 , 		// 	 91		90
		54670314815079900 	 , 		// 	 92		91
		92010139836610400 	 , 		// 	 93		92
		154945075487686000 	 , 		// 	 94		93
		261082452199588000 	 , 		// 	 95		94
		440185014411347000 	 , 		// 	 96		95
		742592119314787000 	 , 		// 	 97		96
		1253495497406210000 	 , 	// 	 98		97
		2117153895121940000 	 , 	// 	 99		98
		3577990082758930000 	 , 	// 	 100	99
};

CNewDSLvExpMan::CNewDSLvExpMan()
{
}

CNewDSLvExpMan::~CNewDSLvExpMan()
{	
}

double CNewDSLvExpMan::GetWinPercent(USERINFO *pUI)
{
	double winpro = 0.0000;
	ALLGAMERECORD allGameRec = pUI->GetMergedAllGameRecords();
	
	winpro = allGameRec.GetWinNum() / (allGameRec.GetWinNum() + allGameRec.GetLoseNum() + 0.0001);
	return winpro;
}

//레벨에 해당되는 경험치 리턴
UINT64 CNewDSLvExpMan::GetExp(int lv)
{
	if(lv < 0 || lv >= 100)return 0;	
	return g_NewDS_LvList[lv];
}

int	CNewDSLvExpMan::GetLv(UINT64 exp)
{	
	if(exp >= g_NewDS_LvList[99])return 99;	
	
	int i = 0;
	for( ; i < 100 ; i ++ )
	{
		if( exp < g_NewDS_LvList[i] ) break;
	}
	if(i >= 99)return 99;
	
	return i;
}

//내 경험치 리턴
UINT64 CNewDSLvExpMan::GetMyExp(USERINFO *pUI)
{
	if( pUI == NULL )return 0;
	int len = strlen( pUI->ID ); 
	if( len <= 0 || len >= 16 )return 0;		
	
	UINT64 gmr = GetNewDSLvExp_GameMoneyRecode(pUI);
	UINT64 gar = GetNewDSLvExp_GameAllinRecode(pUI);
	UINT64 gpr = GetNewDSLvExp_GamePlayRecode(pUI);
	UINT64 gjr = GetNewDSLvExp_GameJockboRecode(pUI);
	
	UINT64 nExp = 0;
	nExp = gmr + gar + gpr + gjr;	

	return nExp;
}

//레벨 리턴
int	CNewDSLvExpMan::GetNewDSLv(USERINFO *pUI)
{
	if( pUI == NULL )return 0;
	int len = strlen( pUI->ID ); 
	if( len <= 0 || len >= 16 )return 0;

	return GetLv( GetMyExp(pUI));	
}

//게임머니 성적
UINT64 CNewDSLvExpMan::GetNewDSLvExp_GameMoneyRecode(USERINFO *pUI)//INT64 maxmoney, INT64 maxbetmoney);
{
	if( pUI == NULL )return 0;
	int len = strlen( pUI->ID ); 
	if( len <= 0 || len >= 16 )return 0;
	/*
	INT64 aRecode = 0;
	aRecode = (((pUI->DS_GetMaxMoney / EXP_1Y) * ( pUI->DS_GetBetMaxMoney / EXP_1Y )) * 100) + EXP_10;
	
	aRecode = (INT64)(aRecode * 0.55);
	if( aRecode >= EXP_250H) aRecode = EXP_250H;
	*/
	
	UINT64 aRecode = 0;
	double total = 0.0f;
	//total = (((double)(pUI->DS_GetMaxMoney / EXP_1Y) * (double)( pUI->DS_GetBetMaxMoney / EXP_1Y )) * 100.0f) + EXP_10;	
	total = (((double)(pUI->DS_GetMaxMoney / EXP_1Y) * (double)( pUI->DS_GetBetMaxMoney / EXP_1Y )) * 100.0f) + EXP_10;	
	total = total * 0.55;
	double temp = total;
	aRecode = (UINT64)total;	

	if( aRecode >= EXP_250H){
		aRecode = EXP_250H;

	//	CLogFile log("dslvexpman.txt");		
	//	CString str;
	//	str.Format("GetNewDSLvExp_GameMoneyRecode Er: Max=%I64d , Bet=%I64d ,Total=%e",pUI->DS_GetMaxMoney,pUI->DS_GetBetMaxMoney,temp);
	//	log.Write(str);
	}

	return aRecode;
}
//올인 시킨 횟수 성적
UINT64 CNewDSLvExpMan::GetNewDSLvExp_GameAllinRecode(USERINFO *pUI)
{
	if( pUI == NULL )return 0;
	int len = strlen( pUI->ID ); 
	if( len <= 0 || len >= 16 )return 0;
	
	// 승률 계산
	double winpro = GetWinPercent(pUI);

	UINT64 aRecode = 0;
	
	double gmm = (double)pUI->DS_GetMaxMoney / EXP_1Y;
	double gbm = (double)pUI->DS_GetBetMaxMoney / EXP_1Y;
	double ain = (double)pUI->AllinCnt;

	double total = 0.0f;
	total = (gmm * gbm ) * ain *  winpro +  (gmm * ain ) + 1.0f;
	total = total * 0.15;
	double temp = total;
	aRecode = (UINT64)total;	
	if( aRecode >= EXP_1H){
		aRecode = EXP_1H;
	//	CLogFile log("dslvexpman.txt");		
	//	CString str;
	//	str.Format("GetNewDSLvExp_GameAllinRecode Er: Max=%I64d , Bet=%I64d ,allcnt=%d,Total=%e",pUI->DS_GetMaxMoney,pUI->DS_GetBetMaxMoney,pUI->AllinCnt,temp);
	//	log.Write(str);
	}
	
	return aRecode;
}
//게임 전적 성적
UINT64 CNewDSLvExpMan::GetNewDSLvExp_GamePlayRecode(USERINFO *pUI)
{
	if( pUI == NULL )return 0;
	int len = strlen( pUI->ID ); 
	if( len <= 0 || len >= 16 )return 0;
	
	UINT64 aRecode = 0;
	// 승률 계산
	double winpro = GetWinPercent(pUI);
	//총 전적
	ALLGAMERECORD allGameRec = pUI->GetMergedAllGameRecords();
	int totalplaycnt = allGameRec.GetTotalNum();
	int nWinNum = allGameRec.GetWinNum();
	
	double a1 = (double)(nWinNum / 100.0f) *  ((double)( (double)totalplaycnt * (double)nWinNum ) / 100.0f );
	a1 = (a1 + ((double)nWinNum*2.0f));
	a1 = a1 * EXP_10;
	a1 = (double)(a1 * ( 2.0f + winpro ));
	a1 = (a1 * (( (double)( (double)pUI->PI.nRSF + (double)pUI->PI.nSF ) * 0.5)+1.0f));
	a1 = a1 + 1.0f;	
	a1 = a1 * 0.15;

	double temp = a1;
	aRecode = (UINT64)a1;
	if( aRecode >= EXP_1H) {
		aRecode = EXP_1H;
	///	CLogFile log("dslvexpman.txt");		
	///	CString str;
	//	str.Format("GetNewDSLvExp_GamePlayRecode Er:\n win=%d , lo=%d ,fold=%d, RSF=%d, SF=%d, Total=%e",pUI->WinNum,pUI->LooseNum,pUI->DrawNum,pUI->PI.nRSF,pUI->PI.nSF,temp);
	//	log.Write(str);
	}

	/*
	INT64 aRecode = 0;
	// 승률 계산
	float winpro = GetWinPercent(pUI);
	//총 전적
	int totalplaycnt = pUI->DrawNum + pUI->LooseNum + pUI->WinNum;
	
	UINT64 a1 = (double)((pUI->WinNum / 100) * (double)(( totalplaycnt * pUI->WinNum ) / 100 ));
	a1 = (a1 + (pUI->WinNum*2));
	a1 = a1 * EXP_10;
	a1 = (UINT64)(a1 * ( 2 + winpro ));
	a1 = (a1 * ((( pUI->PI.nRSF + pUI->PI.nSF ) * 0.5)+1));
	aRecode = a1 +1;

	aRecode = (INT64)(aRecode * 0.15);
	if( aRecode >= EXP_1H) aRecode = EXP_1H;
	*/
	return aRecode;
}
//족보 성적 
UINT64 CNewDSLvExpMan::GetNewDSLvExp_GameJockboRecode(USERINFO *pUI)
{
	if( pUI == NULL )return 0;
	int len = strlen( pUI->ID ); 
	if( len <= 0 || len >= 16 )return 0;
	
	/*
	//총 전적
	int totalplaycnt = pUI->DrawNum + pUI->LooseNum + pUI->WinNum;
	INT64 aRecode = 0;
	
	float a1 = (pUI->PI.nRSF * EXP_100) + ( pUI->PI.nSF * (EXP_10 * 5))  + (pUI->PI.nFCARD * EXP_10);		
	float a3 = ((totalplaycnt + pUI->WinNum) * ( pUI->PI.nRSF * pUI->PI.nSF));
	float a4 = ( pUI->PI.nSF + pUI->PI.nFCARD );
	aRecode = a1 * (a3 + a4) + 1;
	
	aRecode = (INT64)(aRecode * 0.15);
	if( aRecode >= EXP_1H) aRecode = EXP_1H;
	*/

	//총 전적
	ALLGAMERECORD allGameRec = pUI->GetMergedAllGameRecords();
	int totalplaycnt = allGameRec.GetTotalNum();
	int nWinNum = allGameRec.GetWinNum();
	UINT64 aRecode = 0;
	
	double a1 = ((double)pUI->PI.nRSF * EXP_100) + ( (double)pUI->PI.nSF * (EXP_10 * 5))  + ((double)pUI->PI.nFCARD * EXP_10);		
	double a3 = (((double)totalplaycnt + (double)nWinNum) * ( (double)pUI->PI.nRSF * (double)pUI->PI.nSF));
	double a4 = ( (double)pUI->PI.nSF + (double)pUI->PI.nFCARD );
	double total = (a1 * (a3 + a4) + 1.0f);
	total = total * 0.15;

	double temp = total;
	
	aRecode = (UINT64)total;	
	if( aRecode >= EXP_1H){
		aRecode = EXP_1H;
	//	CLogFile log("dslvexpman.txt");		
	//	CString str;
	//	str.Format("GetNewDSLvExp_GameJockboRecode Er:\n win=%d , lo=%d ,fold=%d, RSF=%d, SF=%d, F=%d ,Total=%e",pUI->WinNum,pUI->LooseNum,pUI->DrawNum,pUI->PI.nRSF,pUI->PI.nSF,pUI->PI.nFCARD,temp);
	//	log.Write(str);
	}
	
	return aRecode;
}




//경험치 바 그리기 위함 함수들
double CNewDSLvExpMan::GetMyNewDSLvGaugeBarPercent(USERINFO *pUI)
{
	if( pUI == NULL )return 0;
	int len = strlen( pUI->ID );
	if( len <= 0 || len >= 16 )return 0;

	double precent = 0.0000;

	UINT64 myexp = GetMyExp(pUI);
	UINT64 mylvexp = GetExp(pUI->DS_Level-1);
	UINT64 nextlvexp = GetExp(pUI->DS_Level);
	//1렙당의 경험치
	INT64 destexp = nextlvexp - mylvexp;

	double re = (double)destexp * 0.01;
	//현재 내가 얻은 경험치
	INT64 curexp   = myexp - mylvexp;		
	
	precent = (double)(curexp / re);

	if( precent < 0.0f ) precent = 0.0f;

	return precent;
}
