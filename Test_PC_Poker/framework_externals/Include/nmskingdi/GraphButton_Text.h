// GraphButton_Text.h: interface for the CGraphButton_Text class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHBUTTON_TEXT_H__BA2FD268_3104_4BEC_8656_2BA7AF83E389__INCLUDED_)
#define AFX_GRAPHBUTTON_TEXT_H__BA2FD268_3104_4BEC_8656_2BA7AF83E389__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GraphButtonEx.h"
#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{

		class NMSKINGDI_CLASS CGraphButton_Text  : public CGraphButtonEx
		{
		public:
			CGraphButton_Text();
			virtual ~CGraphButton_Text();

			BOOL	bTextDraw;
			CRect   m_TextRect;	
			CString m_DrawString;
			CFont	*m_pFont;
			COLORREF m_Color;
			void set_Text(CRect rt, CString str);
			void set_Font( CFont *pFont);
			void set_TextColor( COLORREF color);

			virtual void Draw( CDC *pDC, int opt = 0, int grade = 0 );
		};

	} //namespace SKINGDI

}//namespace NMBASE

#endif // !defined(AFX_GRAPHBUTTON_TEXT_H__BA2FD268_3104_4BEC_8656_2BA7AF83E389__INCLUDED_)
