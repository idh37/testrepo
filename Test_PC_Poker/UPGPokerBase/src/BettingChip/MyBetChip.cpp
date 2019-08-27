// MyBetChip.cpp: implementation of the CMyBetChip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyBetChip.h"
#include <math.h>
#include "../UIInfoData.h"
#include "../Sound.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define HALF_PI	1.570975f

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMyBetChip::CMyBetChip()
{
	m_pPage = NULL;
	m_ptChipStart = CPoint(0,0);
	m_nPNum = -1;
	m_pUIInfoData = NULL;
}

CMyBetChip::~CMyBetChip()
{

}
void CMyBetChip::Init(NMBASE::GRAPHICGDI::CPage* pPage, NMBASE::GRAPHICGDI::xSprite* pChipSpr, int nPNum, CUIInfoData *pUInfoData)
{	
	m_pPage		= pPage;
	m_pChipSpr	= pChipSpr;
	m_nPNum		= nPNum;
	m_pUIInfoData = pUInfoData;

	if(nPNum >=0 && nPNum < (int)pUInfoData->m_listCardStartPos.size())
		m_ptChipStart = m_pUIInfoData->m_listCardStartPos[nPNum] + m_pUIInfoData->m_ptChipStartGab;

	m_bMoveEnd = FALSE;
	ReSet();
}

void CMyBetChip::OnEnterRoom(void)
{
}

void CMyBetChip::ReSet()
{
	ZeroMemory(m_listMyChipData,sizeof(m_listMyChipData));

	m_bMoveStart = false;	
	m_ptStart = CPoint(0,0);
	m_bChipShow = false;
	m_bMoveEnd = false;
}

void CMyBetChip::OnMyChipTimer(DWORD dwCurTime)
{	
	OnMyMoveChipProcessEx(dwCurTime);
}

void CMyBetChip::OnMyChipDraw()
{
	if(m_pPage == NULL) return;	
	if(!m_bChipShow) return;
	if(!m_bMoveStart || m_bMoveEnd) return;

	for(int i = 0; i < EndChip; ++i)
	{	
		OnMyChipDraw(&m_listMyChipData[i]);
	}
}

// 칩 그리기
void CMyBetChip::OnMyChipDraw(MYCHIPDATA *pData)
{
	int hyp = 0;
	int vxp = 0;		
	int nChipTypeIndex = 0;

	if(g_RI.ChipKind==1)
		nChipTypeIndex = 18;

	for(int j = 0; j < pData->chipnum; ++j)
	{	
		if(m_pChipSpr)
		{
			m_pPage->PutSprAuto(pData->CurPos[j].x, pData->CurPos[j].y, m_pChipSpr, pData->sprnum + nChipTypeIndex, AB, pData->nAlpha);
		}
	}
}


void CMyBetChip::SetMyChipData(MYCHIPDATA &data, int index, DWORD dwStartTime, DWORD dwMoveTime)
{
	// 각각의 칩 모양과 간격을 정해줌 
	//int stx		= 498;			// 위치 
	//int sty		= 285;

	//int w		= 76;			// 지름
	//int radius	= w/2;
	//int erad	= radius - 30;	// 효과

	//int tx, ty;					

	//int cx = stx + radius;
	//int cy = sty + radius;

	//radius = radius * radius;
	//erad = erad * erad;

	//int xp, yp;

	float fK, fS; 
	float fX, fY;

	for(int i = 0; i < data.chipnum; i ++)
	{	
		if(index > _100J)
		{
			data.sprnum = _100J;			
		}
		else
		{
			data.sprnum = index;					
		}

		fK = float(rand()%1000)/1000.0f * 360.0f * 3.14195f / 180.0f; // 0~360 사이 임의의 각에 대한 라디안 값 
		fS = float(rand()%1000)/1000.0f; // 스케일 

		fX = 75 * cos(fK) * fS; 
		fY = 46 * sin(fK) * fS; 	

		data.TarPos[i].x	= (int)fX + 498;
		data.TarPos[i].y	= (int)fY + 255;
		data.StartPos[i].x	= m_ptChipStart.x;
		data.StartPos[i].y	= m_ptChipStart.y;
		data.CurPos[i].x	= m_ptChipStart.x;
		data.CurPos[i].y	= m_ptChipStart.y;
		data.bMoveEnd		= false;
		data.dwStartTime	= dwStartTime;
		data.dwMoveTime		= dwMoveTime;
	}		
}

void CMyBetChip::SetMoneyChip(INT64 Betmoney)
{
	if(Betmoney <= 0 )
		return;

	CString money=_T("");
	money.Format("%I64d",Betmoney);

	ZeroMemory(m_listMyChipData,sizeof(m_listMyChipData));

	m_bChipShow = true;
	DWORD dwStartTime = timeGetTime();
	DWORD dwMoveTime = 400;

	int gap = 4;
	char temp[4] = {0,};

	int len = money.GetLength();

	if(len == 0 )
		return;

	if(len < 2)
	{				
		temp[0] = money.GetAt(0);		
		m_listMyChipData[0].chipnum = atoi(temp);	
		SetMyChipData(m_listMyChipData[0],0,dwStartTime,dwMoveTime);		
	}	
	else if(len == 2)
	{	
		temp[0] = money.GetAt(0);		

		m_listMyChipData[1].chipnum = atoi(temp);	

		// 각각의 칩 모양과 간격을 정해줌 
		SetMyChipData(m_listMyChipData[1],1,dwStartTime,dwMoveTime);

		temp[0] = money.GetAt(1);	
		m_listMyChipData[0].chipnum = atoi(temp);			
		// 각각의 칩 모양과 간격을 정해줌 
		SetMyChipData(m_listMyChipData[0],0,dwStartTime,dwMoveTime);
	}
	else 
	{		
		temp[0] = money.GetAt(0);		
		m_listMyChipData[len-1].chipnum = atoi(temp);
		SetMyChipData(m_listMyChipData[len-1],len-1,dwStartTime,dwMoveTime);

		temp[0] = money.GetAt(1);		
		m_listMyChipData[len-2].chipnum = atoi(temp);
		SetMyChipData(m_listMyChipData[len-2],len-2,dwStartTime,dwMoveTime);

		temp[0] = money.GetAt(2);		
		m_listMyChipData[len-3].chipnum = atoi(temp);
		SetMyChipData(m_listMyChipData[len-3],len-3,dwStartTime,dwMoveTime);		

		if(len > 3)
		{
			temp[0] = money.GetAt(3);		
			m_listMyChipData[len-4].chipnum = atoi(temp);
			SetMyChipData(m_listMyChipData[len-4],len-4,dwStartTime,dwMoveTime);		
		}
	}

	m_ptPos = m_ptChipStart;
}

void CMyBetChip::SetMyMoveChip()
{
	if(!m_bChipShow) return;
	m_ptStart = m_ptPos;
	m_bMoveStart = true;				// 이동시작	
	m_bMoveEnd = false;
	PBPlayEffectSound(SND_CHIP);
}

// 칩이동 처리 
void CMyBetChip::OnMyMoveChipProcessEx(DWORD dwCurTime)					
{
	if(m_bMoveStart == FALSE)
		return;		

	CPoint ptTarget, ptStart, ptCur;
	DWORD dwDelta = 0;
	float fDelta = 0.0f;

	int i=0;
	for(i= 0; i < EndChip; ++i)
	{	
		if(m_listMyChipData[i].bMoveEnd) continue;
		if(dwCurTime < m_listMyChipData[i].dwStartTime) continue;
		dwDelta = dwCurTime - m_listMyChipData[i].dwStartTime;
		if(dwDelta >= m_listMyChipData[i].dwMoveTime)
		{
			m_listMyChipData[i].bMoveEnd = true;
			if(m_listMyChipData[i].chipnum > 0)
			{
				for(int j = 0; j < m_listMyChipData[i].chipnum; ++j)
				{
					m_listMyChipData[i].CurPos[j] = m_listMyChipData[i].TarPos[j];
				}
			}
			continue;
		}

		float fDelta2 = (float)dwDelta / (float)m_listMyChipData[i].dwMoveTime;
		fDelta = sinf(fDelta2 * HALF_PI);	//감속 운동
		fDelta2 = cosf(fDelta2 *HALF_PI);
		if(fDelta > 1.0f) fDelta = 1.0f;
		if(fDelta2 > 1.0f) fDelta2 = 1.0f;

		for(int j = 0; j < m_listMyChipData[i].chipnum; ++j)
		{
			// 이동이 덜끝난 상태라면
			ptTarget = m_listMyChipData[i].TarPos[j];
			ptStart	= m_listMyChipData[i].StartPos[j];
			ptCur.x = ptStart.x + (int)(float(ptTarget.x - ptStart.x) * fDelta);
			ptCur.y = ptStart.y + (int)(float(ptTarget.y - ptStart.y) * fDelta);
			m_listMyChipData[i].CurPos[j] = ptCur;
		}

		m_listMyChipData[i].nAlpha = (int)(float(31) * fDelta2);
	}	

	bool bMoveEnd = true;
	for(i = 0; i < EndChip; ++i)
	{
		if(!m_listMyChipData[i].bMoveEnd) bMoveEnd = false;
	}

	if(bMoveEnd)
	{
		ReSet();		
		m_bMoveStart = false;
		m_bMoveEnd   = true;
	}
	else
	{
		m_bMoveStart = true;	
	}
}