#include "StdAfx.h"
#include "DrawNumber.h"

CDrawNumber::CDrawNumber(void)
{

	m_nDigitSpace=0;
	m_pSprite=NULL;
	m_nSprOffset=0;
	m_nPosX= m_nPosY=0;
	m_style =STYLE_DRAWNUMBER_NULL;
	m_llNumber=0;

}

CDrawNumber::~CDrawNumber(void)
{
}


void CDrawNumber::SetSprite(xSprite *pSprite, int nSprOffset)
{
	m_pSprite = pSprite;
	m_nSprOffset = nSprOffset;

}

void CDrawNumber::SetPosition(int nPosX, int nPosY)
{
	m_nPosX = nPosX;
	m_nPosY = nPosY;
}

void CDrawNumber::SetNumber(INT64 llNumber)
{
	m_llNumber = llNumber;
}

void CDrawNumber::SetStyle(STYLE_DRAWNUMBER style)
{
	m_style = style;
}

void CDrawNumber::SetDigitSpace(int nDigitSpace)
{
	//글자간격
	m_nDigitSpace = nDigitSpace;
}
void CDrawNumber::Draw(CPage &Page, CDC &DC)
{
	switch(m_style)
	{
		case STYLE_DRAWNUMBER_NULL: return;
		case STYLE_DRAWNUMBER_MONEY_STRING1:		Draw_Style_MoneyString1(Page, DC); return;			
		case STYLE_DRAWNUMBER_MONEY_COMMA_FROM_LEFT:			Draw_Style_MoneyComma_Left(Page, DC); return;			
		case STYLE_DRAWNUMBER_MONEY_COMMA_FROM_RIGHT:			Draw_Style_MoneyComma_Right(Page, DC); return;			
	}
}



CString CDrawNumber::MakeMoneyShort(INT64 nMoney, BOOL bSign)
{

	CString str; // 한글표시
	CString str1;
	INT64 t = nMoney; 
	INT64 Money = nMoney;	

	BOOL bMinus = FALSE;
	if( t < 0 ) {
		bMinus = TRUE;		
		Money = nMoney * -1;
	}

	if( nMoney == 0 )
	{
		str = "0";
	}
	else
	{

		int num[4] = {0,};	
		NumberToParse(Money, num[3], num[2], num[1], num[0]);

		if (num[3] > 0){
			str1.Format("%dJ",num[3]);
			str += str1;
			if (num[2] > 0){
				str1.Format("%dY",num[2]);
				str += str1;
			}
		}
		else if (num[2] > 0){
			str1.Format("%dY",num[2]);
			str += str1;
			if (num[1] > 0){
				str1.Format("%dM",num[1]);
				str += str1;
			}	
		}
		else if (num[1] > 0){
			str1.Format("%dM",num[1]);
			str += str1;
			if (num[0] > 0){
				str1.Format("%d",num[0]);
				str += str1;
			}	

		}
		else if (num[0] > 0)
		{
			str1.Format("%d",num[0]);
			str += str1;
		}	


	}

	if (bSign)
	{
		if( bMinus )
			str.Insert(0,"-");
		else
			str.Insert(0,"+");
	}


	return str;	


}

void CDrawNumber::Draw_Style_MoneyString1(CPage &Page, CDC &DC)
{
	if (m_pSprite==NULL)
		return;

	CString strMoney = MakeMoneyShort(m_llNumber, FALSE); 
	int len = strMoney.GetLength();
	int  i = 0;
	int  coord_x = m_nPosX;
	while(i<len)
	{
		int sprno = 0;

		char ch = strMoney.GetAt(i);
		if (ch == 'J') sprno = 12;			//조
		else if (ch == 'Y') sprno = 11;		//억
		else if (ch == 'M') sprno = 10;		//만
		else if (ch == '-') sprno = 14;		//-
		else if (ch == '+') sprno = 13;		//+
		else //숫자
		{				
			sprno = ch-'0';
		}
		sprno +=m_nSprOffset;
		Page.PutSprAuto(coord_x, m_nPosY, m_pSprite, sprno, AB);
		ASSERT(sprno<m_pSprite->GetTotalSpr());
		coord_x += m_pSprite->spr[sprno].xl;
		coord_x += m_nDigitSpace;	//글자간격
		i++;
	}

	//숫자가 그려진 영역 렉트
	m_rtNumber.SetRect(m_nPosX, m_nPosY, coord_x-m_nDigitSpace, m_nPosY+ m_pSprite->spr[0+m_nSprOffset].yl);

}

void CDrawNumber::Draw_Style_MoneyComma_Left(CPage &Page, CDC &DC)
{
	if (m_pSprite==NULL)
		return;

	CString strMoney = g_MakeCommaMoney(m_llNumber); 
	int len = strMoney.GetLength();
	int  i = 0;
	int  coord_x = m_nPosX;
	while(i<len)
	{
		int sprno = 0;

		char ch = strMoney.GetAt(i);
		if (ch == ',') sprno = 10;			//조
		else if (ch == '-') sprno = 11;		//-
		else if (ch == '+') sprno = 12;		//+
		else //숫자
		{				
			sprno = ch-'0';
		}
		sprno +=m_nSprOffset;
		Page.PutSprAuto(coord_x, m_nPosY, m_pSprite, sprno, AB);
		ASSERT(sprno<m_pSprite->GetTotalSpr());
		coord_x += m_pSprite->spr[sprno].xl;
		coord_x += m_nDigitSpace;	//글자간격
		i++;
	}

	//숫자가 그려진 영역 렉트
	m_rtNumber.SetRect(m_nPosX, m_nPosY, coord_x-m_nDigitSpace, m_nPosY+ m_pSprite->spr[0+m_nSprOffset].yl);

}


void CDrawNumber::Draw_Style_MoneyComma_Right(CPage &Page, CDC &DC)
{
	//오른쪽 정렬


	if (m_pSprite==NULL)
		return;

	CString strMoney = g_MakeCommaMoney(m_llNumber); 
	int len = strMoney.GetLength();
	int  i = len-1;
	int  coord_x = m_nPosX;
	while(i>=0)
	{
		int sprno = 0;

		char ch = strMoney.GetAt(i);
		if (ch == ',') sprno = 10;			//조
		else if (ch == '-') sprno = 11;		//-
		else if (ch == '+') sprno = 12;		//+
		else //숫자
		{				
			sprno = ch-'0';
		}
		sprno +=m_nSprOffset;

		ASSERT(sprno<m_pSprite->GetTotalSpr());
		coord_x -= m_pSprite->spr[sprno].xl;
		Page.PutSprAuto(coord_x, m_nPosY, m_pSprite, sprno, AB);		
		coord_x -= m_nDigitSpace;	//글자간격
		
		
		i--;
	}

	//숫자가 그려진 영역 렉트
	m_rtNumber.SetRect(m_nPosX, m_nPosY, coord_x-m_nDigitSpace, m_nPosY+ m_pSprite->spr[0+m_nSprOffset].yl);

}

CRect CDrawNumber::GetNumberRect()
{
	return m_rtNumber;
}