// CurveControl.cpp: implementation of the CCurveControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CurveControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define MAX_HIGHT	300

CCurveControl::CCurveControl()
{
	Clear();
}

CCurveControl::~CCurveControl()
{
	Clear();
}

void CCurveControl::Clear()
{
	ZeroMemory(m_listMoveData,sizeof(m_listMoveData));
	m_rtMoveData[0] = NULL;	//시작점 영역
	m_rtMoveData[1] = NULL;	//왼쪽 끝점 영역
	m_rtMoveData[2] = NULL;	//오른쪽 끝점 영역
}
void CCurveControl::Init()
{
	Clear();
	m_rtMoveData[0].SetRect(443,252,443+149,292+114);		//시작점 영역
	m_rtMoveData[1].SetRect(-310,700,500,800);				//왼쪽 끝점 영역
	m_rtMoveData[2].SetRect(500,700,1350,800);			//오른쪽 끝점 영역

	GetData_MoveData();
}
void CCurveControl::GetData_MoveData()
{
	//시작점과 끝점을 가지고 온다
	for(int i = 0 ; i < MAX_MOVEDATA_NUM ; i ++)
	{
		m_listMoveData[i].spos.x = RandomA(m_rtMoveData[0].left,m_rtMoveData[0].right);
		m_listMoveData[i].spos.y = RandomA(m_rtMoveData[0].top,m_rtMoveData[0].bottom);
		m_listMoveData[i].movepos[0].x = m_listMoveData[i].spos.x;
		m_listMoveData[i].movepos[0].y = m_listMoveData[i].spos.y;
		if(i < MAX_MOVEDATA_NUM/2)
		{
			m_listMoveData[i].dpos.x = RandomA(m_rtMoveData[1].left,m_rtMoveData[1].right);
			m_listMoveData[i].dpos.y = RandomA(m_rtMoveData[1].top,m_rtMoveData[1].bottom);			
		}
		else
		{
			m_listMoveData[i].dpos.x = RandomA(m_rtMoveData[2].left,m_rtMoveData[2].right);
			m_listMoveData[i].dpos.y = RandomA(m_rtMoveData[2].top,m_rtMoveData[2].bottom);
		}

		m_listMoveData[i].movepos[2].x = m_listMoveData[i].dpos.x;
		m_listMoveData[i].movepos[2].y = m_listMoveData[i].dpos.y;
	}

	GetData_MoveToPoint();
}

void CCurveControl::GetData_MoveToPoint()
{
	int cnt = 0 ;
	while(cnt < MAX_MOVEDATA_NUM)
	{		
		POINT pt =GetData_MoveToPoint(m_listMoveData[cnt].movepos[0].x,m_listMoveData[cnt].movepos[0].y,
			m_listMoveData[cnt].movepos[2].x,m_listMoveData[cnt].movepos[2].y);
		
		m_listMoveData[cnt].movepos[1].x = pt.x;
		m_listMoveData[cnt].movepos[1].y = pt.y;
		
		cnt++;
	}
}

POINT CCurveControl::GetData_MoveToPoint(int sxp,int syp,int dxp,int dyp)
{
	// 총 이동 거리 
	int   tarlen = (int)GetDistance(sxp,syp,dxp,dyp);	
	// 목적지 방향 
	float tarang = GetAngle(sxp,syp,dxp,dyp);

	int halflen = tarlen/2;	//절반
	int r = halflen/2;
	
	int movelen = 0;
//	movelen = RandomA(r,halflen-50);
	movelen = RandomA(10,50);
	halflen -= movelen;
	
	POINT vp = GetMovePoint( sxp, syp, tarang, (float)halflen);

	vp.y -= RandomA(200,MAX_HIGHT+200);	

	return vp;
}