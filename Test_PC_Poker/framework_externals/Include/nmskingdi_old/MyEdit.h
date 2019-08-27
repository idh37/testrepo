#if !defined(AFX_MYEDIT_H__30091DD3_5086_4687_B744_6EADFA41DEB5__INCLUDED_)
#define AFX_MYEDIT_H__30091DD3_5086_4687_B744_6EADFA41DEB5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyEdit.h : header file
//

#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{
		/////////////////////////////////////////////////////////////////////////////
		// CMyEdit window

		class NMSKINGDI_CLASS CMyEdit : public CEdit
		{
			// Construction
		public:
			CMyEdit();

			CBrush		m_BgBrush;
			COLORREF	m_Textcolor;

			void		OnCreateBrush(COLORREF color);
			void		OnSetTextColor(COLORREF color);
			// Attributes
		public:

			// Operations
		public:

			// Overrides
			// ClassWizard generated virtual function overrides
			//{{AFX_VIRTUAL(CMyEdit)
			//}}AFX_VIRTUAL

			// Implementation
		public:
			virtual ~CMyEdit();

			// Generated message map functions
		protected:
			//{{AFX_MSG(CMyEdit)
			afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);	
			//}}AFX_MSG

			DECLARE_MESSAGE_MAP()
		};

		/////////////////////////////////////////////////////////////////////////////

		//{{AFX_INSERT_LOCATION}}
		// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

	}//namespace SKINGDI

}//namespace NMBASE


#endif // !defined(AFX_MYEDIT_H__30091DD3_5086_4687_B744_6EADFA41DEB5__INCLUDED_)
