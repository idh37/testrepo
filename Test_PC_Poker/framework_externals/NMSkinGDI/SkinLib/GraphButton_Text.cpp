// GraphButton_Text.cpp: implementation of the CGraphButton_Text class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GraphButton_Text.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


namespace NMBASE
{
	namespace SKINGDI
	{
		//////////////////////////////////////////////////////////////////////
		// Construction/Destruction
		//////////////////////////////////////////////////////////////////////

		CGraphButton_Text::CGraphButton_Text()
		{
			m_TextRect = NULL;
			bTextDraw = FALSE;
			m_DrawString = "";
			m_pFont = NULL;
			m_Color = RGB(255,255,255);
		}

		CGraphButton_Text::~CGraphButton_Text()
		{

		}

		void CGraphButton_Text::set_Text(CRect rt, CString str )
		{
			m_TextRect = rt;
			//	m_TextRect.OffsetRect(rt.left,rt.top);	
			m_DrawString = str;
			bTextDraw = TRUE;
		}

		void CGraphButton_Text::set_Font( CFont *pFont)
		{
			m_pFont = pFont;
		}

		void CGraphButton_Text::set_TextColor(  COLORREF color )
		{
			m_Color = color;
		}

		void CGraphButton_Text::Draw(CDC *pDC, int opt, int grade )
		{
			if(m_pPage == NULL) return;
			if(m_pSpr == NULL) return;
			if(m_bShow == FALSE) return;

			// 버튼 그리기
			if( m_SceneObject )
			{
				DWORD time = GetTickCount();
				if(m_bEnable==FALSE)
				{
					if(m_ButtonCnt <4) m_pPage->DrawScene(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 2, time );
					else m_pPage->DrawScene(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 3, time);		//	다운된그림
				}
				else if(m_bOver==TRUE && m_bClick==FALSE)	m_pPage->DrawScene(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 1, time);
				else if(m_bClick==FALSE) m_pPage->DrawScene(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 0, time);
				else if(m_bClick==TRUE) m_pPage->DrawScene(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 2, time);
			}
			else
			{
				if(m_bEnable==FALSE)
				{
					if(m_ButtonCnt <4) m_pPage->PutSprAuto(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 2, opt, grade );
					else m_pPage->PutSprAuto(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 3, opt, grade);		//	다운된그림
				}
				else if(m_bOver==TRUE && m_bClick==FALSE)	m_pPage->PutSprAuto(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 1, opt, grade);
				else if(m_bClick==FALSE) m_pPage->PutSprAuto(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 0, opt, grade);
				else if(m_bClick==TRUE) m_pPage->PutSprAuto(m_Xp, m_Yp, m_pSpr, ( m_iButtonNumber * m_ButtonCnt ) + 2, opt, grade);
			}

			if( pDC == NULL )return;
			if( bTextDraw == TRUE ){		
				//	pDC->DrawText("sadfasdf",&m_TextRect,DT_CENTER | DT_WORDBREAK);
				CFont *pOldFont = NULL;
				if( m_pFont != NULL ){
					pOldFont = pDC->SelectObject(m_pFont);
				}

				COLORREF color = pDC->GetTextColor();
				pDC->SetTextColor(m_Color);
				pDC->DrawText(m_DrawString,&m_TextRect,DT_CENTER | DT_WORDBREAK);

				if( pOldFont != NULL ){
					pDC->SelectObject(pOldFont);
				}
				pDC->SetTextColor(color);
			}	
		}


	} //namespace SKINGDI

}//namespace NMBASE