#if !defined(AFX_WEBCTRL_H__CBA5DFD8_6F1C_11D4_97A4_0050BF0FBE67__INCLUDED_)
#define AFX_WEBCTRL_H__CBA5DFD8_6F1C_11D4_97A4_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WebCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWebCtrl html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

#include "../UtilDefine.h"

namespace NMBASE
{
	namespace UTIL
	{

		typedef void (CALLBACK_ONBN2)(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, LPCTSTR lpszPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
		typedef void (CALLBACK_ONNC2)(LPCTSTR strURL);
		typedef void (CALLBACK_ONQUIT)();

		class NMUTIL_CLASS CWebCtrl : public CHtmlView
		{
			//protected:
		public:
			CWebCtrl();           // protected constructor used by dynamic creation
			DECLARE_DYNCREATE(CWebCtrl)

			// html Data
		public:
			//{{AFX_DATA(CWebCtrl)
			// NOTE: the ClassWizard will add data members here
			//}}AFX_DATA

			// Attributes
		public:

			BOOL m_bHideBorder;

			// Operations
		public:

			void SetOption(BOOL bHideBorder);
			int  GetUserNavigatedCount() { return m_nNavigatedCount; }
			void SetUserNavigatedCount(int count) { m_nNavigatedCount = count; }
			void ResetUserNavigatedCount() { m_nNavigatedCount = 0; }

			void SetCallback_OnBeforeNavigate2(CALLBACK_ONBN2* pCallback) {
				m_pfnWebCallBack_ONBN2 = pCallback; }
			void SetCallback_OnNavigateComplete2(CALLBACK_ONNC2* pCallback) {
				m_pfnWebCallBack_ONNC2 = pCallback; }
			void SetCallback_OnQuit(CALLBACK_ONQUIT* pCallback) {
				m_pfnWebCallBack_OnQUIT = pCallback; }

			void Navigate2(LPCTSTR lpszURL, DWORD dwFlags = 0, LPCTSTR lpszTargetFrameName = NULL);

		protected:
			void SetBorderNone();

		public:
			// Overrides
			// ClassWizard generated virtual function overrides
			//{{AFX_VIRTUAL(CWebCtrl)
		public:
			virtual void PostNcDestroy();
			virtual void OnBeforeNavigate2(LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel);
			virtual void OnNavigateComplete2( LPCTSTR strURL );
			virtual void OnDocumentComplete(LPCTSTR lpszURL);
			virtual void OnNewWindow2(LPDISPATCH* ppDisp, BOOL* Cancel);
		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
			//}}AFX_VIRTUAL

			// Implementation
		protected:
			CALLBACK_ONBN2 *m_pfnWebCallBack_ONBN2;
			CALLBACK_ONNC2 *m_pfnWebCallBack_ONNC2;
			CALLBACK_ONQUIT *m_pfnWebCallBack_OnQUIT;
			int m_nNavigatedCount;

		public:
			virtual ~CWebCtrl();

#ifdef _DEBUG
			virtual void AssertValid() const;
			virtual void Dump(CDumpContext& dc) const;
#endif

			// Generated message map functions
			//{{AFX_MSG(CWebCtrl)
			afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT nMsg);
			//}}AFX_MSG
			DECLARE_MESSAGE_MAP()

			virtual BOOL OnWebClose(BOOL IsChildWindow, BOOL *pCancel);
			DECLARE_EVENTSINK_MAP()
		};

		/////////////////////////////////////////////////////////////////////////////

		//{{AFX_INSERT_LOCATION}}
		// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

	}//namespace UTIL

}//namespace NMBASE


#endif // !defined(AFX_WEBCTRL_H__CBA5DFD8_6F1C_11D4_97A4_0050BF0FBE67__INCLUDED_)
