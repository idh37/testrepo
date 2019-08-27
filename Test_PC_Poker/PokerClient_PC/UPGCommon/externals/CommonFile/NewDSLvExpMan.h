// NewDSLvExpMan.h: interface for the CNewDSLvExpMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWDSLVEXPMAN_H__2CE073EC_431C_41A5_898E_8CA40EC398FE__INCLUDED_)
#define AFX_NEWDSLVEXPMAN_H__2CE073EC_431C_41A5_898E_8CA40EC398FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserStruct.h"

#define EXP_1		  (10000)				//		1만원
#define EXP_10	  (100000)				//		10만원
#define EXP_100	  (1000000)				//		100만원
#define EXP_1Y      (100000000)			//		1억원

//제한 경험치
#define EXP_1H	  (10000000000000000)	//		1경 (billion) 금액제한 고수
#define EXP_250H	  (2500000000000000000)	//		250경 (billion) 금액제한 고수

class CNewDSLvExpMan  
{
public:
	CNewDSLvExpMan();
	virtual ~CNewDSLvExpMan();	
	
	//레벨 리턴
	int	GetNewDSLv(USERINFO *pUI);
	//내 경험치 리턴
	UINT64 GetMyExp(USERINFO *pUI);
	//레벨에 해당되는 경험치 리턴
	UINT64 GetExp(int lv);
	//레벨
	int	GetLv(UINT64 exp);
	//승률
	double GetWinPercent(USERINFO *pUI);

	//게임머니 성적
	UINT64 GetNewDSLvExp_GameMoneyRecode(USERINFO *pUI);//INT64 maxmoney, INT64 maxbetmoney);
	//올인 시킨 횟수 성적
	UINT64 GetNewDSLvExp_GameAllinRecode(USERINFO *pUI);
	//게임 전적 성적
	UINT64 GetNewDSLvExp_GamePlayRecode(USERINFO *pUI);
	//족보 성적 
	UINT64 GetNewDSLvExp_GameJockboRecode(USERINFO *pUI);

	//경험치 바 그리기 위함 함수들
	double GetMyNewDSLvGaugeBarPercent(USERINFO *pUI);
};

#endif // !defined(AFX_NEWDSLVEXPMAN_H__2CE073EC_431C_41A5_898E_8CA40EC398FE__INCLUDED_)
