// SprMoneyDraw.cpp: implementation of the CSprPublicMomeyDraw class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SprPublicMomeyDraw.h"
#include "GlobalGame.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define GAB	67	

CSprPublicMomeyDraw::CSprPublicMomeyDraw()
{
	pPage = NULL;
	pNumberSpr = NULL;
}

CSprPublicMomeyDraw::~CSprPublicMomeyDraw()
{

}

void CSprPublicMomeyDraw::Init(NMBASE::GRAPHICGDI::CPage *page,xSprite *spr,int xp, int yp , BOOL bbox,int pnum)
{
	if(spr == NULL || page  == NULL)
		return;

	pPage = page;
	pNumberSpr = spr;	

	ReSet();

	Xp = xp;
	Yp = yp;

	PNum = pnum;
}

void CSprPublicMomeyDraw::ReSet()
{	
	bShow = FALSE;	
	nMoney = 0;
	nTotalMoneyIndex = 0;	
	ZeroMemory(nMoneyDeck,sizeof(nMoneyDeck));
}

void CSprPublicMomeyDraw::Clear()
{
	pPage = NULL;
	pNumberSpr = NULL;
	ReSet();
	Xp = 0;
	Yp = 0;
}

void CSprPublicMomeyDraw::SetMoney(INT64 money,BOOL bmoneymark)
{
	ZeroMemory(nMoneyDeck,sizeof(nMoneyDeck));
	nTotalMoneyIndex = 0;
	nMoney = money;	

	bMoneyMark = bmoneymark; // FALSE =  숫자 TRUE = 한글 
	int kk = 0;
	int addx = 0; // 컴마
	/*
	if(bMoneyMark == FALSE){ // 숫자

	INT64 tmoney = nMoney;

	if(tmoney < 0)
	tmoney = tmoney * -1;

	CString str = _T("");		
	str.Format("%I64d",tmoney);
	char num[4] = {0,};
	int len = str.GetLength();

	if(len-1 > 30)return;

	if(tmoney != 0 ){
	for( int j = len - 1; j >= 0 ; j-- ) {						
	num[0] = str.GetAt(j);

	if(kk >= 3)	{
	kk = 0;
	nMoneyDeck[nTotalMoneyIndex] = -1;
	nTotalMoneyIndex++;
	}
	nMoneyDeck[nTotalMoneyIndex] = atoi(num);
	nTotalMoneyIndex++;
	kk++;			
	}
	}

	if(nMoney == 0){
	nMoneyDeck[nTotalMoneyIndex++] = 0;					
	}
	}
	*/
	//else // 한글
	{		
		int num[4];
		memset(num,-1,sizeof(num));

		INT64 tmoney = nMoney;

		if(tmoney < 0)
			tmoney = tmoney * -1;

		SNumberToParse(tmoney, num[3], num[2], num[1], num[0]);		

		for(int i = 0 ; i < 4 ; i ++)
		{
			if(num[i] < 0) 
				continue;
			if( i != 0)
			{
				nMoneyDeck[nTotalMoneyIndex] = -100;
				nTotalMoneyIndex++;
			}		

			CString str = _T("");		
			str.Format("%d",num[i]);
			int len = 0;
			len = str.GetLength();	

			if(len-1 > 30)
				return;

			if( num[i] == 0)
			{
				for(int j = 0; j < 4 ; j++)
				{					
					nMoneyDeck[nTotalMoneyIndex] = 0;
					nTotalMoneyIndex++;
				}
			}
			else
			{
				for(int j = len-1; j >= 0 ; j--)
				{
					char tnum[4] = {0,};
					tnum[0] = str.GetAt(j);
					nMoneyDeck[nTotalMoneyIndex] = atoi(tnum);
					nTotalMoneyIndex++;
				}			
			}			
		}

		if(nMoney == 0)
		{
			nMoneyDeck[nTotalMoneyIndex++] = 0;					
		}		
	}

	bShow = TRUE;
}

void CSprPublicMomeyDraw::Draw(int gap)
{
	if(pNumberSpr == NULL)
		return;
	if(nTotalMoneyIndex==0 || nTotalMoneyIndex > 30 )
		return;

	if(nMoney == 0)
		return;

	int w = pNumberSpr->spr[0].xl;
	int sx = Xp;
	int sy = Yp;
	int tmp = 0;
	int i=0;
	int ngap = 0;	

	/*

	if(bMoneyMark == FALSE)
	{
	for( i=0; i<nTotalMoneyIndex;i++){
	if( nMoneyDeck[i] >= -1 && nMoneyDeck[i] < 500 )
	{
	tmp = nMoneyDeck[i];				

	if(tmp == -1){
	ngap -= 4;
	pPage->PutSprAuto( sx  - i * w - ngap, sy, pNumberSpr, 10 );

	}					
	else
	pPage->PutSprAuto( sx  - i * w - ngap, sy, pNumberSpr, tmp );
	}
	}
	}*/

	//	else 
	//	{
	int cnt = 0 ;
	int s = 0;
	ngap = 0;
	for( i=0; i<nTotalMoneyIndex;i++)
	{

		if( nMoneyDeck[i] >= -100 && nMoneyDeck[i] < 500 )
		{
			tmp = nMoneyDeck[i];

			if(tmp == -100)
			{
				switch(s) 
				{
					case 0:
						ngap += GAB - 1;//만 
						break;
					case 1:
						ngap += GAB - 3;//억
						break;
					case 2:
						ngap += GAB - 3;//조
						break;
				}
				cnt = 0;
				s ++;

			}
			else
			{				
				if( tmp != -1)
				{
					pPage->PutSprAuto( sx  - (cnt * w) - ngap, sy, pNumberSpr, tmp );
					cnt++;
				}
			}
		}
	}
	//	}
	//	*/
}

int CSprPublicMomeyDraw::SNumberToParse(INT64 money, int &Num1, int &Num2, int &Num3, int &Num4)
{

	INT64 t = money; 
	/*	
	INT64 v = t / (INT64)10000000000000000;	// 경 단위
	if(v > 0) {
	t = t - v*(INT64)10000000000000000;
	Num1 = v;
	}
	*/
	INT64 v = t / (INT64)1000000000000;	// 조 단위
	if(v > 0) 
	{		
		t = t - v*(INT64)1000000000000;
		Num1 = (int)v;		
	}
	else
	{
		if(money >=1000000000000)
		{
			Num1 = 0;
		}
		else
		{
			Num1 = -1;
		}
	}

	v = t / (INT64)100000000;	// 억 단위
	if(v > 0) 
	{
		t = t - v*(INT64)100000000;
		Num2 = (int) v;		
	}
	else
	{
		if(money >=100000000)
		{
			Num2 = 0;
		}
		else
		{
			Num2 = -1;
		}
	}

	v = t / (INT64)10000;	// 만 단위
	if(v > 0) 
	{		
		t = t - v*(INT64)10000;
		Num3 = (int) v;		
	}
	else
	{
		if(money >=10000)
		{
			Num3 = 0;
		}
		else
		{
			Num3 = -1;
		}
	}

	if( money >= 0 ) 
	{
		Num4 = (int)  t;
	}


	return 0;
}