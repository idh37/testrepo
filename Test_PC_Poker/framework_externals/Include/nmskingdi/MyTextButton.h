// MyTextButton.h: interface for the CMyTextButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYTEXTBUTTON_H__157B8DF0_B9D6_468B_A6B0_29271A579BE3__INCLUDED_)
#define AFX_MYTEXTBUTTON_H__157B8DF0_B9D6_468B_A6B0_29271A579BE3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "skingdidefine.h"
#include "MyButton.h"

namespace NMBASE
{
	namespace SKINGDI
	{


		struct BUTTON_TEXT_INFO
		{
			CRect		rc;
			CString		Text;

			COLORREF	Color;
			CFont*		pFont;

			BOOL		bLayered;
			COLORREF	BackColor;
			DWORD		format;
		};


		class NMSKINGDI_CLASS CMyTextButton : public CMyButton
		{
			// Construction
		public:
			CMyTextButton();
			virtual ~CMyTextButton();

			// Attributes
		public:

			void Draw(CDC& dc);
			void SetText( const char* pStr, CRect& rc, COLORREF color, CFont* pFont, BOOL bLayered = FALSE, COLORREF BackColor = RGB(0, 0, 0), DWORD format = DT_CENTER );

			void DrawLayeredStr( CString& str, CRect& rc, COLORREF MainColor, COLORREF ShellColor, CDC& dc, DWORD format );

		protected:

			std::vector< BUTTON_TEXT_INFO > m_TextInfo;


		};

	} //namespace SKINGDI

}//namespace NMBASE


#endif // !defined(AFX_MYTEXTBUTTON_H__157B8DF0_B9D6_468B_A6B0_29271A579BE3__INCLUDED_)