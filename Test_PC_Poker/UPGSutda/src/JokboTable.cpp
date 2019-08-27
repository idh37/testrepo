// JokboTable.cpp: implementation of the CJokboTable class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "JokboTable.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJokboTable::CJokboTable()
{
	m_JokboTip_Num = -1;
	m_bMouseron = FALSE;
	m_pJokboTableSpr = NULL;
	m_dwJokboTipOn = 0;

	for( int i = 0; i < MAX_JOKBO_TABLE; i++)
	{
		m_TableIndex[i] = FALSE;
		
		m_Xp[i] = 304; m_Yp[i] = 518 + ( i * 18 );	//±◊∑¡¡Ÿ ¿ßƒ° 
		//√º≈©«“ ∑∫∆Æ
		m_JokboRect[i].SetRect( 304, 518 + ( i * 18 ), 304 + 60, ( 518 + 18) + ( i * 18 ) );
	}
}

CJokboTable::~CJokboTable()
{

}



void CJokboTable::Init(NMBASE::GRAPHICGDI::xSprite *pJokboTableSpr)
{
	m_pJokboTableSpr = pJokboTableSpr;
	
	ReSetJokboTable();
}


void CJokboTable::OnMouseMove(int nX, int nY)
{
	POINT pt;
	m_bMouseron = FALSE;

	//¡∑∫∏∞° ∆¡¿ª ∫∏ø©¡‡æﬂ «œ≥™ ∞ÀªÁ«—¥Ÿ.
	for( int i = 0; i < MAX_JOKBO_TABLE; i++)
	{
		pt.x = nX;
		pt.y = nY;
		
		if( m_JokboRect[i].PtInRect(pt) )
		{
			m_bMouseron = TRUE;

			if( m_JokboTip_Num != i )
				m_dwJokboTipOn = timeGetTime();
		
			m_JokboTip_Num = i;

			break;
		}

	}

	if( m_bMouseron == FALSE ) 
	{
		m_dwJokboTipOn = 0;
		m_JokboTip_Num = -1;
	}
}

void CJokboTable::DrawBottom( NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC )
{
	CFont *pOldFont = pDC->SelectObject(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_NORMAL));
	COLORREF oldcolor = pDC->GetTextColor();
	

	static const char *szJokboName[MAX_JOKBO_TABLE]= { "ªÔ∆»±§∂Ø", "∂Ø", "æÀ∏Æ", "µ∂ªÁ", "±∏ªÊ", "¿ÂªÊ", "¿ÂªÁ", "ºº∑˙", "∞©ø¿", "≤˝.∏¡≈Î", "ªÁ±∏.∂Ø¿‚¿Ã", "æœ«‡æÓªÁ"};
	
	for( int i = 0; i < MAX_JOKBO_TABLE; i++)
	{
		
		if( m_TableIndex[i] == TRUE )
		{
			pDC->SetTextColor(RGB(255,255,255));
			pPage->PutSprAuto(m_Xp[i], m_Yp[i], m_pJokboTableSpr, 18);
		}
		else
		{
			pDC->SetTextColor(RGB(122,115,88));
		}

		int nOldExtra = SetTextCharacterExtra(pDC->m_hDC, -1);	
		//¡∑∫∏∏Ì ¬ÔæÓ¡÷±‚
		pDC->DrawText(szJokboName[i], strlen(szJokboName[i]), m_JokboRect[i], DT_SINGLELINE|DT_VCENTER|DT_CENTER);
		SetTextCharacterExtra(pDC->m_hDC, nOldExtra);		
	}	

	pDC->SetTextColor(oldcolor);
	pDC->SelectObject(pOldFont);
}


void CJokboTable::DrawTop( NMBASE::GRAPHICGDI::CPage *pPage, CDC *pDC )
{


	if( m_JokboTip_Num >= 0 )
	{
		DWORD dwTime = timeGetTime();
		pPage->DrawScene( 4, 445,m_pJokboTableSpr, m_JokboTip_Num, abs((int) (dwTime - m_dwJokboTipOn)) );
	}


}

void CJokboTable::SetJokboTableIndex(int nIndex)
{
	if (nIndex<0 || nIndex>=MAX_JOKBO_TABLE)
		return;

	m_TableIndex[nIndex]= TRUE;

}
void CJokboTable::ReSetJokboTable()
{

	for(int i=0; i<MAX_JOKBO_TABLE; i++)
	{
		m_TableIndex[i]= FALSE;
	}
}
