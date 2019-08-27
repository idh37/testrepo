#if !defined(AFX_MSGDLG_H__070F5001_1C5C_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_MSGDLG_H__070F5001_1C5C_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgDlg.h : header file
//

#include <mmsystem.h>
//#include <UPGPlatform.h>
#include "resource.h"

#define MSGBOX_WAIT_TIMER 334
#define MSGBOX_ANI_TIMER	335
#define IDM_BTN_LODINGCANCEL		(WM_USER+9601)	//취소버튼 

/////////////////////////////////////////////////////////////////////////////
// CMsgDlg dialog

class CMsgDlg : public CUPGDlgBase
{
// Construction
public:
	int				m_nMode;
	CString			m_strMsg;
	int				m_nTimerWait;
	int				m_nTimerAni;
	double			m_dbTick;

	NMBASE::GRAPHICGDI::CPage			m_cPage;
	NMBASE::GRAPHICGDI::xSprite			m_sprBack;	//스프라이트
	NMBASE::SKINGDI::CGraphButton	m_ctrlCancel;
	DWORD			m_dwNowTime;
	DWORD			m_dwStartTime;

	CMsgDlg(CWnd* pParent = NULL);   // standard constructor
	void MsgBox(int nMode, char *pMsg);
	void Draw(CDC *pDC);

	//{{AFX_DATA(CMsgDlg)
	enum { IDD = IDD_MSG_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(CMsgDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC); 

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGDLG_H__070F5001_1C5C_11D4_97A5_0050BF0FBE67__INCLUDED_)
