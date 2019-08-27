// MyEdit.cpp : implementation file
//

#include "stdafx.h"
#include "MyEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

namespace NMBASE
{
	namespace SKINGDI
	{
		/////////////////////////////////////////////////////////////////////////////
		// CMyEdit

		CMyEdit::CMyEdit()
		{
			m_BgBrush.CreateSolidBrush(RGB(135,175,210));	
			m_Textcolor = RGB(62,103,103);
		}

		CMyEdit::~CMyEdit()
		{
		}


		BEGIN_MESSAGE_MAP(CMyEdit, CEdit)
			//{{AFX_MSG_MAP(CMyEdit)
			ON_WM_CTLCOLOR_REFLECT()	
			//}}AFX_MSG_MAP
		END_MESSAGE_MAP()

		/////////////////////////////////////////////////////////////////////////////
		// CMyEdit message handlers

		void	CMyEdit::OnCreateBrush(COLORREF color)
		{
			m_BgBrush.DeleteObject();
			m_BgBrush.CreateSolidBrush(color);
		}

		void	CMyEdit::OnSetTextColor(COLORREF color)
		{
			m_Textcolor = color;
		}

		HBRUSH CMyEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
		{
			// TODO: Change any attributes of the DC here	

			// TODO: Return a non-NULL brush if the parent's handler should not be called
			// For the Text color.
			pDC->SetBkMode(TRANSPARENT);
			pDC->SetTextColor(m_Textcolor);
			// For the Background color.
			return m_BgBrush;
		}

	}//namespace SKINGDI

}//namespace NMBASE
