#if !defined(AFX_WEBCTRL4NEW_H__CBA5DFD8_6F1C_11D4_97A4_0050BF0FBE67__INCLUDED_)
#define AFX_WEBCTRL4NEW_H__CBA5DFD8_6F1C_11D4_97A4_0050BF0FBE67__INCLUDED_

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

		// [수호천사] 2004.07.08
#define 	WM_ALLINMESSAGE			(WM_USER+7510)

		enum 
		{
			ACD_GO_CHARGE = 1000,			// 충전 페이지 이동
			ACD_GO_LEADERS,				// 리더스 페이지 이동
			ACD_ALLIN_INSURANCE,		// 올인 보험 자세히 보기 
			ACD_ALLIN_INSURANCE_USE,	// 올인 보험 사용하기 
			ACD_ALLIN_INSURANCE_BUY,	// 올임 보험 구매하기 
			ACD_INVINCIBLE_BUY,			// 천하무적 구매하기 
			ACD_INVINCIBLE_DETAILS,		// 천하무적 자세히 보기 
			ACD_CLOSEWINDOW,			// 창닫기
			ACD_GO_MAINPAGE,			// 게임 페이지로 이동 

			ACD_GAME_CONTINUE,			// [DB기록 Process] 2004.11.29  게임계속하기
			ACD_GO_LEADERS_ZONE,			// [DB기록 Process] 2004.11.29  리더스 회원존
			ACD_GO_AVATA_ZONE,			// [DB기록 Process] 2004.11.29  아바타 상점
			ACD_GO_SEARCH_PCROOM,       // [올인창 수정] 2006.01.04 피시방 찾기
			// -> 아래 한줄 추가
			ACD_CLOSEPOPUP, // [아이템 유료화 영역]
		};

		class NMUTIL_CLASS CWebCtrl4New : public CHtmlView
		{
			//protected:
		public:
			CWebCtrl4New(HWND hParent = NULL);           // protected constructor used by dynamic creation
			DECLARE_DYNCREATE(CWebCtrl4New)

		protected:
			HWND	m_hParent;

			// html Data
		public:
			//{{AFX_DATA(CWebCtrl)
			// NOTE: the ClassWizard will add data members here
			//}}AFX_DATA

			// Attributes
		public:
			BOOL	m_bNaviate;
			BOOL m_bHideBorder;

			// Operations
		public:

			void SetOption(BOOL bHideBorder);
			//BOOL InsertHTML(LPCTSTR strHtml);
			//BOOL ExecJavascript(LPCTSTR strScript);
			//BOOL PutCharSet(LPCTSTR strCharset);

		public://protected:
			void SetBorderNone();

		public:

			virtual void OnBeforeNavigate2( LPCTSTR lpszURL, DWORD nFlags, LPCTSTR lpszTargetFrameName, CByteArray& baPostedData, LPCTSTR lpszHeaders, BOOL* pbCancel );
			virtual void OnNavigateComplete2( LPCTSTR strURL );
			// Overrides
			// ClassWizard generated virtual function overrides
			//{{AFX_VIRTUAL(CWebCtrl)
		protected:
			virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
			//}}AFX_VIRTUAL

			// Implementation
		public:
			//protected:
			virtual ~CWebCtrl4New();

#ifdef _DEBUG
			virtual void AssertValid() const;
			virtual void Dump(CDumpContext& dc) const;
#endif

			// Generated message map functions
			//{{AFX_MSG(CWebCtrl4New)
			// NOTE - the ClassWizard will add and remove member functions here.
			//}}AFX_MSG
			DECLARE_MESSAGE_MAP()
		};

		/////////////////////////////////////////////////////////////////////////////

		//{{AFX_INSERT_LOCATION}}
		// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

	}//namespace UTIL

}//namespace NMBASE



#endif // !defined(AFX_WEBCTRL4NEW_H__CBA5DFD8_6F1C_11D4_97A4_0050BF0FBE67__INCLUDED_)
