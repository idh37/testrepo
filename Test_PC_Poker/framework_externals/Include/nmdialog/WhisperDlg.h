#if !defined(AFX_WHISPERDLG_H__66883C61_413A_11D4_97B5_0050BF0FBE67__INCLUDED_)
#define AFX_WHISPERDLG_H__66883C61_413A_11D4_97B5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WhisperDlg.h : header file
//
#include "../resource.h"
#include "DialogDefine.h"
#include "MYButton.h"
#include "MyBitmap.h"
#include "GrpChatEdit.h"
#include "MyEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CWhisperDlg dialog

class NMDIALOG_CLASS CWhisperDlg : public CDialog
{
// Construction
public:
	CMyBitmap Back;
	BOOL bCreated;
	CString m_TarID;
	UINT64  m_llTargetUKey;
	CString TarNickName;	
// 여기까지
	CBrush		m_HollowBrush;
	CWhisperDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWhisperDlg)
	enum { IDD = IDD_WHISPER_DIALOG };
	CString	m_strChat;
	CString	m_strWho;
	CMyButton	m_CancelBtn;
	CMyButton	m_OkBtn;
	CMyButton	m_ReportBtn;
	CMyEdit		m_StrMsgEdit;
	CMyEdit		m_strIDEdit;
	CGrpChatEdit	m_ChatViewEdit;
	//}}AFX_DATA

public:
	void ShowPopup(const char *pNickname, const char *pID, UINT64 llUserKey);
	void ClearText();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWhisperDlg)
	public:
//함수 인자 수정
	//	virtual BOOL Create(char *id);
////	virtual BOOL Create(char *nickname);
// 여기까지
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWhisperDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedReport();
};


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WHISPERDLG_H__66883C61_413A_11D4_97B5_0050BF0FBE67__INCLUDED_)
