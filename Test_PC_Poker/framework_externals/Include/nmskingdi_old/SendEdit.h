#if !defined(AFX_SENDEDIT_H__AAF09DFC_E3C3_11D3_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_SENDEDIT_H__AAF09DFC_E3C3_11D3_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SendEdit.h : header file
//

#include "skingdidefine.h"
#include <mmsystem.h>

/////////////////////////////////////////////////////////////////////////////
// CSendEdit window

namespace NMBASE
{
	namespace SKINGDI
	{
		typedef void (*FN_CALLBACK_PROCESSSENDCHAT)(const char*) ;

		class NMSKINGDI_CLASS CSendEdit : public CEdit
		{
		public:

			FN_CALLBACK_PROCESSSENDCHAT m_fnProcessChat;
			int		m_nUse;

			int		m_nNowHPos;
			CString m_listHistory[10];

			void SetProcSendChatFn(FN_CALLBACK_PROCESSSENDCHAT fnProcessChat);
			// Overrides
			// ClassWizard generated virtual function overrides
			//{{AFX_VIRTUAL(CSendEdit)
		protected:
			virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
			//}}AFX_VIRTUAL

			// Implementation
		public:
			CSendEdit();
			virtual ~CSendEdit();

			// Generated message map functions
		protected:
			//{{AFX_MSG(CSendEdit)
			afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
			afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
			//}}AFX_MSG

			DECLARE_MESSAGE_MAP()
		};

		/////////////////////////////////////////////////////////////////////////////

		//{{AFX_INSERT_LOCATION}}
		// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

	}//namespace SKINGDI

}//namespace NMBASE


#endif // !defined(AFX_SENDEDIT_H__AAF09DFC_E3C3_11D3_97A2_0050BF0FBE67__INCLUDED_)
