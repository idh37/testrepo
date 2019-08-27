// CurveControl.h: interface for the CCurveControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CURVECONTROL_H__924DA42D_B5A0_4923_B23A_F8DCC7E0A9B5__INCLUDED_)
#define AFX_CURVECONTROL_H__924DA42D_B5A0_4923_B23A_F8DCC7E0A9B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../SevenPoker/MovementEx.h"

#define MAX_MOVEDATA_NUM	100
#define MAX_CONTROL_POINT	3

typedef struct {
	POINT	spos;	//시작점 
	POINT	dpos;	//끝점
	POINT	movepos[MAX_CONTROL_POINT];	//이동할점들
}MOVEDATA;

class CCurveControl  
{
public:
	CCurveControl();
	virtual ~CCurveControl();

	MOVEDATA m_listMoveData[MAX_MOVEDATA_NUM];
	CRect	 m_rtMoveData[3];

	void Clear();
	void Init();

	void GetData_MoveData();
	void GetData_MoveToPoint();
	POINT GetData_MoveToPoint(int sxp,int syp,int dxp,int dyp);

};
#endif // !defined(AFX_CURVECONTROL_H__924DA42D_B5A0_4923_B23A_F8DCC7E0A9B5__INCLUDED_)
