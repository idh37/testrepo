// SprMoneyDraw.cpp: implementation of the CSprMoneyDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SprMoneyDraw.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSprMoneyDraw::CSprMoneyDraw()
{
	m_pPage = NULL;
	m_pNumberSpr = NULL;
	m_pGoldSpr = NULL;
	m_nCutUnit = 4;
	m_bShow = true;
	m_uFormat = DT_LEFT;
	m_bShowZero = true;
	m_nSpaceBetweenLetters = 0;
}

CSprMoneyDraw::~CSprMoneyDraw()
{
	
}

void CSprMoneyDraw::Init(NMBASE::GRAPHICGDI::CPage *page,NMBASE::GRAPHICGDI::xSprite *spr,CRect rtRect, UINT uFormat, int nCutUnit, bool bShowZero, int nSpaceBetweenLetters/* = 0*/, NMBASE::GRAPHICGDI::xSprite *goldSpr)
{
	m_pPage = page;
	m_pNumberSpr = spr;
	m_pGoldSpr = goldSpr;
	
	ReSet();
	
	m_rtRect = rtRect;
	m_uFormat = uFormat;
	m_nCutUnit = nCutUnit;
	m_bShowZero = bShowZero;
	m_nSpaceBetweenLetters = nSpaceBetweenLetters;
}

void CSprMoneyDraw::SetRect(CRect rtRect)
{
	m_rtRect = rtRect;
}

void CSprMoneyDraw::ReSet()
{	
	m_bShow = false;
	m_i64Money = 0;
	m_nTotalMoneyIndex = 0;		
	ZeroMemory(m_listMoneyDeck,sizeof(m_listMoneyDeck));
}

void CSprMoneyDraw::Clear()
{
	m_pPage = NULL;
	m_pNumberSpr = NULL;
	ReSet();
}

void CSprMoneyDraw::SetMoney(INT64 money, bool bShowZeroUnit)
{
	ZeroMemory(m_listMoneyDeck,sizeof(int) * 30);
	m_nTotalMoneyIndex = 0;
	m_i64Money = money;	
	
	int kk = 0;
	int addx = 0; // 컴마

	int num[4] = {0,};
	INT64 tmoney = m_i64Money;
	
	if(tmoney < 0)
		tmoney = tmoney * -1;
	
	NMBASE::UTIL::g_NumberToParse(tmoney, num[3], num[2], num[1], num[0]);
	//1조 이상이면 만 이하는 보여주지 않는다. 2018.05.14 친구방에서 생기는 문제
	if(num[3] > 0)
		num[0] = 0;

	int i=0;
	int temp=0;
	CString str = _T("");
	char tnum[4] = {0,};

	if(m_i64Money != 0)
	{
		//0인 단위를 보여줄지의 여부
		if(bShowZeroUnit)
		{
			bool bFirst = true;
			//첫번째로 0이 아닌 숫자가 나오는 단위를 찾기위한 변수
			int nFirst = 3;

			//m_nCutUnit(표시 단위수) 만 표시한다.
			//표시 단위의 최하위 단위를 찾는다.
			for(i=3;i>=0;i--)
			{
				//첫번째로 0이 아닌 숫자가 나오는 단위를 찾는다.
				if(bFirst && (num[i] > 0))
				{
					temp++;
					nFirst = i;		//첫번째로 0이 아닌 숫자가 나오는 단위
					bFirst = false;
				}
				//0이 아닌 첫번째 단위가 아닐경우 자르는 단위수에 포함한다.
				else if(!bFirst && num[i] >= 0)
				{
					temp++;
				}

				if(temp == m_nCutUnit) break;
			}

			if(i < 0) i = 0;

			//0이 아닌 첫번째 단위까지 표시할 최하위 단위부터 표시한다.
			for(; i <= nFirst ; i ++){
				if(num[i] < 0) continue;

				//원단위는 표시하지 않는다.
				if( i != 0)
				{
					//만(-200),억(-300),조(-400) 단위의 이미지 인덱스를 삽입한다.
					m_listMoneyDeck[m_nTotalMoneyIndex] = (i + 1) * -100;
					m_nTotalMoneyIndex++;
				}

				//단위의 값이 0이면 '0000'의 이미지 인덱스를 삽입한다.
				if(num[i] == 0)
				{
					for(int j = 0; j < 4 ; j++){
						m_listMoneyDeck[m_nTotalMoneyIndex] = 0;
						m_nTotalMoneyIndex++;
					}
				}
				//단위의 값이 0이 아니면
				else
				{
					//문자열의 가장 마지막 숫자부터 출력할 문자의 이미지 인덱스를 만든다.
					str.Format("%d",num[i]);
					int len = str.GetLength() - 1;
					
					for(int j = len; j >= 0 ; j--)
					{
						tnum[0] = str.GetAt(j);
						m_listMoneyDeck[m_nTotalMoneyIndex] = atoi(tnum);
						m_nTotalMoneyIndex++;
					}
					
					//단위의 상위값중 출력할 문자의 이미지 인덱스를 넣지 않는 부분은 공백(-100)을 출력하도록 이미지 인덱스를 추가해준다.
					for(int j = len+1;j<4;j++)
					{
						m_listMoneyDeck[m_nTotalMoneyIndex] = -100;
						m_nTotalMoneyIndex++;
					}
				}
			}
		}
		else
		{
			//m_nCutUnit(표시 단위수) 만 표시한다.
			//표시 단위의 최하위 단위를 찾는다.
			for(i=3;i>=0;i--)
			{
				if(num[i] > 0) temp++;
				if(temp == m_nCutUnit)	break;
			}

			//표시 단위의 최하위 단위 부터 출력할 문자의 이미지 인덱스를 만든다.
			for(; i < 4 ; i ++){
				//0이 아닌 단위만 표시한다.
				if(num[i] <= 0) continue;

				//원단위는 단위 표시를 하지 않는다.
				if( i != 0)
				{
					//만(-200),억(-300),조(-400) 단위를 삽입한다. 
					m_listMoneyDeck[m_nTotalMoneyIndex] = (i + 1) * -100;
					m_nTotalMoneyIndex++;
				}
				
				//숫자를 문자열로 변환하여 문자열의 길이를 구한다.
				str.Format("%d",num[i]);
				int len = str.GetLength();
				
				//문자열의 가장 마지막 숫자부터 출력할 문자의 이미지 인덱스를 만든다.
				for(int j = len-1; j >= 0 ; j--){
					tnum[0] = str.GetAt(j);
					m_listMoneyDeck[m_nTotalMoneyIndex] = atoi(tnum);
					m_nTotalMoneyIndex++;
				}
			}
		}
	}
	//0원 일경우 0을 보여줄지의 여부
	else if(m_bShowZero){
		m_listMoneyDeck[m_nTotalMoneyIndex++] = 0;					
	}
	
	m_bShow = true;
}

void CSprMoneyDraw::Draw(int chipKind)
{
	if(m_pNumberSpr == NULL)return;
	if(m_pPage == NULL)return;
	if(m_nTotalMoneyIndex==0)return;
	if(m_nTotalMoneyIndex > 30)return;
	
	int tmp = 0;
	int i=0;
	int ngap = 0;

	int nWidth = 0;
	NMBASE::GRAPHICGDI::xSprite *numberSpr = m_pNumberSpr;
	if(chipKind==1 && m_pGoldSpr != NULL)	numberSpr = m_pGoldSpr;

	for( i=0; i<m_nTotalMoneyIndex;i++){
		tmp = m_listMoneyDeck[i];

		if(tmp < 0){	
			tmp = 14 + (tmp/100);
		}

		if(tmp < 13)
		{
			nWidth += numberSpr->spr[tmp].xl;
		}
		else
		{
			nWidth += numberSpr->spr[0].xl;
		}

		nWidth += m_nSpaceBetweenLetters;
	}

	if(nWidth > 0) nWidth -= m_nSpaceBetweenLetters;

	int nStartX = m_rtRect.left;
	if(m_uFormat & DT_RIGHT)
	{
		nStartX = m_rtRect.right - nWidth;
		if(chipKind==1 && m_pGoldSpr != NULL)
		{
			nStartX -= numberSpr->spr[13].xl;
		}
	}
	else if(m_uFormat & DT_CENTER)
	{
		nStartX = m_rtRect.left + (m_rtRect.Width() - nWidth)/2;
	}

	m_rtDrawRect.SetRect(nStartX,m_rtRect.top,nStartX,m_rtRect.top+numberSpr->spr[0].yl);
	
	for( i=m_nTotalMoneyIndex-1; i>=0;i--){
		tmp = m_listMoneyDeck[i];		

		if(tmp < 0){	
			tmp = 14 + (tmp/100);
		}

		if(tmp < 13)
		{
			m_pPage->PutSprAuto( nStartX + ngap, m_rtRect.top, numberSpr, tmp, AB);
			ngap += numberSpr->spr[tmp].xl;
		}
		else
		{
			ngap += numberSpr->spr[0].xl;
		}
		ngap += m_nSpaceBetweenLetters;
	}

	if(chipKind==1 && m_pGoldSpr != NULL)
	{
		m_pPage->PutSprAuto( nStartX + ngap, m_rtRect.top, numberSpr, 13, AB);
		ngap += numberSpr->spr[13].xl;
	}

	m_rtDrawRect.right += ngap;
}

bool CSprMoneyDraw::IsIn(const CPoint &ptPos)
{
	return m_rtDrawRect.PtInRect(ptPos)?true:false;
}

CPoint CSprMoneyDraw::GetTopCenterPos(void)
{
	return CPoint(m_rtDrawRect.left + m_rtDrawRect.Width()/2, m_rtDrawRect.top);
}

CPoint CSprMoneyDraw::GetTopLeftPos(void)
{
	return CPoint(m_rtDrawRect.left, m_rtDrawRect.top);
}