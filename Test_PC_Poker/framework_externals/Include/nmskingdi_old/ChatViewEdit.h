#if !defined(AFX_MYRICHEDIT_H__AAF09DEF_E3C3_11D3_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_MYRICHEDIT_H__AAF09DEF_E3C3_11D3_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyRichEdit.h : header file
//

#include "skingdidefine.h"

namespace NMBASE
{
	namespace SKINGDI
	{
		/////////////////////////////////////////////////////////////////////////////
		// CChatViewEdit window

		class NMSKINGDI_CLASS CChatViewEdit : public CRichEditCtrl
		{
		public:
			int oldline;
			CChatViewEdit();
			// Overrides
			// ClassWizard generated virtual function overrides
			//{{AFX_VIRTUAL(CChatViewEdit)
		protected:
			virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
			//}}AFX_VIRTUAL

			// Implementation
		public:

			void AddLinkText( CString str, DWORD nParam );
			BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

			void SetTextFormat(int kind);
			//bchangestate 텍스트의 상태가 변경될수도 있는것들 ( 대화내용 지우기 기능 추가로 사용한다)
			void AddText(CString *str, COLORREF crNewColor=RGB(0,0,0), BOOL bScroll=FALSE,BOOL bchangestate = FALSE);
			void AddText(char *str /* str2 */, COLORREF crNewColor=RGB(0,0,0), BOOL bScroll=FALSE,BOOL bchangestate = FALSE);
			virtual ~CChatViewEdit();

			// Generated message map functions
		protected:
			//{{AFX_MSG(CChatViewEdit)
			afx_msg void OnSetFocus(CWnd* pOldWnd);
			//}}AFX_MSG

			DECLARE_MESSAGE_MAP()
		};

		/////////////////////////////////////////////////////////////////////////////

		//{{AFX_INSERT_LOCATION}}
		// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

	}//namespace SKINGDI

}//namespace NMBASE

#endif // !defined(AFX_MYRICHEDIT_H__AAF09DEF_E3C3_11D3_97A2_0050BF0FBE67__INCLUDED_)


