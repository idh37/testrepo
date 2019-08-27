// AllBetChip.cpp: implementation of the CAllBetChip class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <windows.h>
#include <math.h>
#include "AllBetChip.h"
#include "../UIInfoData.h"
#include "../SidePotMoney/SidePotMoney.h"

#define HALF_PI	1.570975f

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAllBetChip::CAllBetChip()
{
	m_pPage = NULL;
	m_nChipHeight = 0;
	m_pUIInfoData = NULL;
	m_pGameProcess = NULL;
	m_pChipSpr = NULL;		
	m_nStatus = EABCMS_UNKNOWN;
	m_dwMoveStart = 0;
	m_dwMoveTime = 1000;
	m_dwFadeOutTime = 1000;
	m_nAlpha = 31;
	m_nGoldIndex = 18;
}

CAllBetChip::~CAllBetChip()
{
}

void CAllBetChip::LoadImage(void)
{
	AddLoadImageList(&m_sprSideMoney,".\\PokerCommon\\data\\GetSideMoney.spr",true);
}

void CAllBetChip::Init(NMBASE::GRAPHICGDI::CPage* page, CUIInfoData* pUIInfoData, CGameProcess* pGameProcess, NMBASE::GRAPHICGDI::xSprite* pChipSpr)
{
	m_pPage	= page;	
	m_pUIInfoData = pUIInfoData;
	m_pGameProcess= pGameProcess;
	m_pChipSpr = pChipSpr;							// 칩 스프라이트

	ZeroMemory(m_listChipData, sizeof(CHIPDATA) * EndChip);

	m_listDrawOrder.push_back(0);
	m_listDrawOrder.push_back(1);
	m_listDrawOrder.push_back(2);
	m_listDrawOrder.push_back(3);

	m_listDrawOrder.push_back(4);
	m_listDrawOrder.push_back(5);
	m_listDrawOrder.push_back(6);
	m_listDrawOrder.push_back(7);
	m_listDrawOrder.push_back(8);

	m_listDrawOrder.push_back(9);
	m_listDrawOrder.push_back(10);
	m_listDrawOrder.push_back(11);
	m_listDrawOrder.push_back(12);

	m_listDrawOrder.push_back(13);
	m_listDrawOrder.push_back(14);
	m_listDrawOrder.push_back(15);
}

void CAllBetChip::Reset()
{		
	ZeroMemory(m_listChipData,sizeof(m_listChipData));// 그리기 위해 머니를 칩으로 계산해서 넣어둔다
	m_listUserGetMoneyData.clear();
	m_dwMoveStart = 0;

	m_nStatus = EABCMS_CENTERDRAW;
	m_i64Money = 0;
	m_nAlpha = 31;

	if(m_pChipSpr->spr) m_nChipHeight = m_pChipSpr->spr[0].yl;
}

// 총 배팅머니 셋팅
void CAllBetChip::SetMoney(INT64 tmoney)
{
	if(tmoney <= 0)
		return;

	if(!m_pGameProcess)
		return;

	m_i64Money = tmoney;

	ZeroMemory(m_listChipData,sizeof(m_listChipData));// 그리기 위해 머니를 칩으로 계산해서 넣어둔다

	CString money = _T("");
	money.Format("%I64d",m_i64Money);
	int nLen = money.GetLength();
	char temp[2] = {0,};

	if(nLen > (int)m_pUIInfoData->m_listBackChipGabPos.size())
	{
		return;
	}

	int index = 0;
	for(int i = 0; i < nLen; ++i)
	{
		temp[0] = money.GetAt(nLen - 1 - i);			
		int num = atoi(temp);
		if(num <= 0) continue;

		m_listChipData[index].chipnum = num;			

		if(i > _10J){
			m_listChipData[index].sprnum = _10J;			
		}
		else{
			if(g_RI.ChipKind==1)
				m_listChipData[index].sprnum = i+m_nGoldIndex;
			else
				m_listChipData[index].sprnum = i;
		}

		index++;
	}

	int nSize = m_pUIInfoData->m_listBackChipGabPos.size();
	for(int i=0, k=0; i< index;i++,k++)
	{
		if(k<nSize)	m_listChipData[i].ptPos = m_pUIInfoData->m_ptBackChipCenter + m_pUIInfoData->m_listBackChipGabPos[index-1][i];
	}
}

// 배팅 드로우		
void CAllBetChip::DrawBottom()
{
	if(m_pPage == NULL)
		return;

	switch(m_nStatus)
	{
	case EABCMS_CENTERDRAW:
	case EABCMS_WAIT:
		DrawCenterChip();
		break;
	}
}

void CAllBetChip::DrawTop()
{
	if(m_pPage == NULL)
		return;

	switch(m_nStatus)
	{
	case EABCMS_MOVE:
	case EABCMS_FADEOUT:
		DrawMoveAllChip();
		break;
	}
}

void CAllBetChip::DrawCenterChip(void)
{
	int hyp = 0;
	list<int>::iterator start = m_listDrawOrder.begin();
	list<int>::iterator end = m_listDrawOrder.end();

	int i=0;
	for(;start != end; ++start)
	{
		i = *start;
		if(m_listChipData[i].chipnum <= 0) continue;
		
		hyp = 0;
		for(int j = 0; j < m_listChipData[i].chipnum; ++j)
		{	
			m_pPage->PutSprAuto( m_listChipData[i].ptPos.x, m_listChipData[i].ptPos.y - hyp, m_pChipSpr, m_listChipData[i].sprnum, AB, 31);
			hyp+=4;
		}
	}
}

void CAllBetChip::DrawMoveAllChip(void)
{
	list<USER_GETMONEY_DATA>::iterator start = m_listUserGetMoneyData.begin();
	list<USER_GETMONEY_DATA>::iterator end = m_listUserGetMoneyData.end();

	for(;start != end;start++)
	{
		DrawMoveChip(*start);
	}
}

void CAllBetChip::DrawMoveChip(USER_GETMONEY_DATA &data)
{
	CString money = _T("");
	money.Format("%I64d",data.i64Money);
	int nLen = money.GetLength();
	char temp[2] = {0,};

	if(nLen > (int)m_pUIInfoData->m_listBackChipGabPos.size())
	{
		return;
	}

	int nNumber = 0;
	int sprnum = 0;
	int index = 0;
	int hyp = 0;
	CPoint ptGab = 0;

	int listChipNum[EndChip];
	int listChipIndex[EndChip];
	ZeroMemory(listChipNum, sizeof(int)*EndChip);
	ZeroMemory(listChipIndex, sizeof(int)*EndChip);

	for(int i = 0; i < nLen; ++i)
	{
		temp[0] = money.GetAt(nLen - 1 - i);
		nNumber = atoi(temp);
		if(nNumber <= 0) continue;

		if(i > _10J){
			sprnum = _10J;			
		}
		else{
			if(g_RI.ChipKind==1)
				sprnum = i+m_nGoldIndex;
			else
				sprnum = i;
		}

		if(g_RI.ChipKind!=1 && sprnum >= (int)m_pUIInfoData->m_listBackChipGabPos.size()) continue;

		listChipNum[index] = nNumber;
		listChipIndex[index] = sprnum;
		index++;
	}

	list<int>::iterator start = m_listDrawOrder.begin();
	list<int>::iterator end = m_listDrawOrder.end();
	int nChipCount = 0;
	int nSprIndex = 0;
	for(;start != end;start++)
	{
		nChipCount = listChipNum[*start];
		nSprIndex = listChipIndex[*start];
		if(nChipCount <= 0) continue;

		hyp = 0;
		for(int j=0;j<nChipCount;j++)
		{	
			ptGab = m_pUIInfoData->m_listBackChipGabPos[index-1][*start];
			m_pPage->PutSprAuto( data.ptCur.x + ptGab.x, data.ptCur.y + ptGab.y - hyp, m_pChipSpr, nSprIndex, AB, m_nAlpha);
			hyp+=4;
		}
	}

	if(data.i64ResultMoney < 0)
	{
		int nPlayIndex = 0;
		switch(index)
		{
		case 3:
		case 4:
		case 5:
		case 6:
			nPlayIndex = 1;
			break;
		case 7:
		case 8:
		case 9:
		case 10:
		case 11:
		case 12:
		case 13:
		case 14:
		case 15:
			nPlayIndex = 2;
			break;
		case 16:
			nPlayIndex = 3;
			break;
		}
		DWORD dwTotalPlayTime = m_sprSideMoney.GetMScenePlayTime(nPlayIndex);
		DWORD dwCurTime = timeGetTime() - m_dwMoveStartTime;
		if(dwCurTime >= dwTotalPlayTime) dwCurTime = dwTotalPlayTime - 1;
		m_pPage->DrawMultiScene(data.ptCur.x, data.ptCur.y, &m_sprSideMoney, nPlayIndex, dwCurTime);
	}
}

void CAllBetChip::OnMove(DWORD dwCurTime)
{
	DWORD dwTimeGab = dwCurTime - m_dwMoveStart;
	if(dwTimeGab >= m_dwMoveTime)
	{
		m_dwMoveStart += m_dwMoveTime;
		m_nStatus = EABCMS_FADEOUT;
		return;
	}

	float fDelta = sinf(HALF_PI * (float(dwTimeGab) / float(m_dwMoveTime)));

	list<USER_GETMONEY_DATA>::iterator start = m_listUserGetMoneyData.begin();
	list<USER_GETMONEY_DATA>::iterator end = m_listUserGetMoneyData.end();

	for(;start != end;start++)
	{
		USER_GETMONEY_DATA &data = *start;
		data.ptCur.x = data.ptStart.x + (int)((data.ptTarget.x - data.ptStart.x) * fDelta);
		data.ptCur.y = data.ptStart.y + (int)((data.ptTarget.y - data.ptStart.y) * fDelta);
	}
}

void CAllBetChip::OnFadeOut(DWORD dwCurTime)
{
	DWORD dwTimeGab = dwCurTime - m_dwFadeOutTime;
	if(dwTimeGab >= m_dwFadeOutTime)
	{
		m_dwMoveStart += m_dwFadeOutTime;
		m_nStatus = EABCMS_END;
		return;
	}

	float fDelta = cosf(HALF_PI * (float(dwTimeGab) / float(m_dwFadeOutTime)));
	m_nAlpha = (int)(float(31) * fDelta);
}


void CAllBetChip::OnTimer()
{	
	DWORD dwCurTime = timeGetTime();

	switch(m_nStatus)
	{
	case EABCMS_WAIT:
		if(m_dwMoveStart > dwCurTime)
		{
			return;
		}
		m_nStatus = EABCMS_MOVE;
		m_dwMoveStartTime = timeGetTime();
	case EABCMS_MOVE:
		OnMove(dwCurTime);
		break;
	case EABCMS_FADEOUT:
		OnFadeOut(dwCurTime);
		break;
	}
}

void CAllBetChip::SetWinner(CSidePotMoney *pSidePotResult)
{
	ASSERT(pSidePotResult != NULL);

	if(NULL == pSidePotResult)
		return;

	m_dwMoveStart = timeGetTime() + m_pUIInfoData->m_dwChipMoveDelayTime;
	m_nStatus = EABCMS_WAIT;

	MAP_RANKING::iterator start = pSidePotResult->GetRanking().begin();
	MAP_RANKING::iterator end = pSidePotResult->GetRanking().end();
	USER_GETMONEY_DATA stGetMoneyData;

	LIST_SAME_PLAYER::iterator start_pnum, end_pnum;

	int nAdd = 0;
	if(g_RI.cRuleType == RULETYPE_5POKER)
	{
		nAdd += 2;
	}

	for(;start != end;start++)
	{
		start_pnum = start->second.begin();
		end_pnum = start->second.end();

		for(;start_pnum != end_pnum;start_pnum++)
		{
			stGetMoneyData.i64Money = pSidePotResult->GetUserSidePot(start_pnum->first).i64ResultMoney;
			if(stGetMoneyData.i64Money <= 0) continue;
			stGetMoneyData.nPNum = start_pnum->first;
			stGetMoneyData.i64ResultMoney = start_pnum->second;
			stGetMoneyData.ptStart = m_pUIInfoData->m_ptBackChipCenter;
			stGetMoneyData.ptCur = stGetMoneyData.ptStart;
			stGetMoneyData.ptTarget = m_pUIInfoData->m_listCardStartPos[stGetMoneyData.nPNum+nAdd] + m_pUIInfoData->m_ptChipStartGab;
			m_listUserGetMoneyData.push_back(stGetMoneyData);
		}
	}
}