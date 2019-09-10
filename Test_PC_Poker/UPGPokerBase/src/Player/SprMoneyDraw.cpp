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
	int addx = 0; // �ĸ�

	int num[4] = {0,};
	INT64 tmoney = m_i64Money;
	
	if(tmoney < 0)
		tmoney = tmoney * -1;
	
	NMBASE::UTIL::g_NumberToParse(tmoney, num[3], num[2], num[1], num[0]);
	//1�� �̻��̸� �� ���ϴ� �������� �ʴ´�. 2018.05.14 ģ���濡�� ����� ����
	if(num[3] > 0)
		num[0] = 0;

	int i=0;
	int temp=0;
	CString str = _T("");
	char tnum[4] = {0,};

	if(m_i64Money != 0)
	{
		//0�� ������ ���������� ����
		if(bShowZeroUnit)
		{
			bool bFirst = true;
			//ù��°�� 0�� �ƴ� ���ڰ� ������ ������ ã������ ����
			int nFirst = 3;

			//m_nCutUnit(ǥ�� ������) �� ǥ���Ѵ�.
			//ǥ�� ������ ������ ������ ã�´�.
			for(i=3;i>=0;i--)
			{
				//ù��°�� 0�� �ƴ� ���ڰ� ������ ������ ã�´�.
				if(bFirst && (num[i] > 0))
				{
					temp++;
					nFirst = i;		//ù��°�� 0�� �ƴ� ���ڰ� ������ ����
					bFirst = false;
				}
				//0�� �ƴ� ù��° ������ �ƴҰ�� �ڸ��� �������� �����Ѵ�.
				else if(!bFirst && num[i] >= 0)
				{
					temp++;
				}

				if(temp == m_nCutUnit) break;
			}

			if(i < 0) i = 0;

			//0�� �ƴ� ù��° �������� ǥ���� ������ �������� ǥ���Ѵ�.
			for(; i <= nFirst ; i ++){
				if(num[i] < 0) continue;

				//�������� ǥ������ �ʴ´�.
				if( i != 0)
				{
					//��(-200),��(-300),��(-400) ������ �̹��� �ε����� �����Ѵ�.
					m_listMoneyDeck[m_nTotalMoneyIndex] = (i + 1) * -100;
					m_nTotalMoneyIndex++;
				}

				//������ ���� 0�̸� '0000'�� �̹��� �ε����� �����Ѵ�.
				if(num[i] == 0)
				{
					for(int j = 0; j < 4 ; j++){
						m_listMoneyDeck[m_nTotalMoneyIndex] = 0;
						m_nTotalMoneyIndex++;
					}
				}
				//������ ���� 0�� �ƴϸ�
				else
				{
					//���ڿ��� ���� ������ ���ں��� ����� ������ �̹��� �ε����� �����.
					str.Format("%d",num[i]);
					int len = str.GetLength() - 1;
					
					for(int j = len; j >= 0 ; j--)
					{
						tnum[0] = str.GetAt(j);
						m_listMoneyDeck[m_nTotalMoneyIndex] = atoi(tnum);
						m_nTotalMoneyIndex++;
					}
					
					//������ �������� ����� ������ �̹��� �ε����� ���� �ʴ� �κ��� ����(-100)�� ����ϵ��� �̹��� �ε����� �߰����ش�.
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
			//m_nCutUnit(ǥ�� ������) �� ǥ���Ѵ�.
			//ǥ�� ������ ������ ������ ã�´�.
			for(i=3;i>=0;i--)
			{
				if(num[i] > 0) temp++;
				if(temp == m_nCutUnit)	break;
			}

			//ǥ�� ������ ������ ���� ���� ����� ������ �̹��� �ε����� �����.
			for(; i < 4 ; i ++){
				//0�� �ƴ� ������ ǥ���Ѵ�.
				if(num[i] <= 0) continue;

				//�������� ���� ǥ�ø� ���� �ʴ´�.
				if( i != 0)
				{
					//��(-200),��(-300),��(-400) ������ �����Ѵ�. 
					m_listMoneyDeck[m_nTotalMoneyIndex] = (i + 1) * -100;
					m_nTotalMoneyIndex++;
				}
				
				//���ڸ� ���ڿ��� ��ȯ�Ͽ� ���ڿ��� ���̸� ���Ѵ�.
				str.Format("%d",num[i]);
				int len = str.GetLength();
				
				//���ڿ��� ���� ������ ���ں��� ����� ������ �̹��� �ε����� �����.
				for(int j = len-1; j >= 0 ; j--){
					tnum[0] = str.GetAt(j);
					m_listMoneyDeck[m_nTotalMoneyIndex] = atoi(tnum);
					m_nTotalMoneyIndex++;
				}
			}
		}
	}
	//0�� �ϰ�� 0�� ���������� ����
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