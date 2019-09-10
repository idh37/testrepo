// NewDSLvExpMan.h: interface for the CNewDSLvExpMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NEWDSLVEXPMAN_H__2CE073EC_431C_41A5_898E_8CA40EC398FE__INCLUDED_)
#define AFX_NEWDSLVEXPMAN_H__2CE073EC_431C_41A5_898E_8CA40EC398FE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserStruct.h"

#define EXP_1		  (10000)				//		1����
#define EXP_10	  (100000)				//		10����
#define EXP_100	  (1000000)				//		100����
#define EXP_1Y      (100000000)			//		1���

//���� ����ġ
#define EXP_1H	  (10000000000000000)	//		1�� (billion) �ݾ����� ���
#define EXP_250H	  (2500000000000000000)	//		250�� (billion) �ݾ����� ���

class CNewDSLvExpMan  
{
public:
	CNewDSLvExpMan();
	virtual ~CNewDSLvExpMan();	
	
	//���� ����
	int	GetNewDSLv(USERINFO *pUI);
	//�� ����ġ ����
	UINT64 GetMyExp(USERINFO *pUI);
	//������ �ش�Ǵ� ����ġ ����
	UINT64 GetExp(int lv);
	//����
	int	GetLv(UINT64 exp);
	//�·�
	double GetWinPercent(USERINFO *pUI);

	//���ӸӴ� ����
	UINT64 GetNewDSLvExp_GameMoneyRecode(USERINFO *pUI);//INT64 maxmoney, INT64 maxbetmoney);
	//���� ��Ų Ƚ�� ����
	UINT64 GetNewDSLvExp_GameAllinRecode(USERINFO *pUI);
	//���� ���� ����
	UINT64 GetNewDSLvExp_GamePlayRecode(USERINFO *pUI);
	//���� ���� 
	UINT64 GetNewDSLvExp_GameJockboRecode(USERINFO *pUI);

	//����ġ �� �׸��� ���� �Լ���
	double GetMyNewDSLvGaugeBarPercent(USERINFO *pUI);
};

#endif // !defined(AFX_NEWDSLVEXPMAN_H__2CE073EC_431C_41A5_898E_8CA40EC398FE__INCLUDED_)
