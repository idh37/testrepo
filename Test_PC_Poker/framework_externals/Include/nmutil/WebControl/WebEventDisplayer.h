#if !defined(AFX_WEBEVENTDISPLAYER_H__7F54F412_214D_4A07_9439_7059DB837F2C__INCLUDED_)
#define AFX_WEBEVENTDISPLAYER_H__7F54F412_214D_4A07_9439_7059DB837F2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WebEventDisplayer.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWebEventDisplayer dialog
#include"WebBro.h"

namespace NMBASE
{
	namespace UTIL
	{

		class CWebEventDisplayer : public CDialog
		{
			// Construction
		public:
			CWebEventDisplayer(CWnd* pParent = NULL);   // standard constructor

			// Dialog Data
			//{{AFX_DATA(CWebEventDisplayer)
			enum { IDD = IDD_WEBEVENT_DLG };
			// NOTE: the ClassWizard will add data members here
			//}}AFX_DATA


			// Overrides
			// ClassWizard generated virtual function overrides
			//{{AFX_VIRTUAL(CWebEventDisplayer)
		public:
			virtual BOOL PreTranslateMessage(MSG* pMsg);
		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
			//}}AFX_VIRTUAL

			// Implementation
		protected:
			DECLARE_EVENTSINK_MAP( )
			// Generated message map functions
			//{{AFX_MSG(CWebEventDisplayer)
			virtual BOOL OnInitDialog();
			afx_msg void OnDestroy();
			//}}AFX_MSG
			DECLARE_MESSAGE_MAP()

			void OnWindowSetHeight(long height);
			void OnWindowSetWidth(long width);
			void WbDocumentComplete(LPDISPATCH pDisp,VARIANT FAR* URL);
			void OnClosing(BOOL IsChildWindow,BOOL *pCancel);
		public:
			int m_WebW,m_WebH;
			CWebBrowser2 m_pLocWebCtrl;
		};

		//{{AFX_INSERT_LOCATION}}
		// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

	}//namespace UTIL

}//namespace NMBASE



#endif // !defined(AFX_WEBEVENTDISPLAYER_H__7F54F412_214D_4A07_9439_7059DB837F2C__INCLUDED_)
