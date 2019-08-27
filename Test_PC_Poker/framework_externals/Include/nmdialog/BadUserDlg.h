#if !defined(AFX_BADUSERDLG_H__F991EC81_C025_11D4_97A4_0050BF0FBE67__INCLUDED_)
#define AFX_BADUSERDLG_H__F991EC81_C025_11D4_97A4_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BadUserDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CBadUserDlg dialog

#include "../resource.h"
#include "HideChatList.h"
#include "MYButton.h"
#include "DialogDefine.h"
#include "MyBitmap.h"
#include "BmpCheckButton.h"
#include "BmpRadioBtn.h"


typedef	struct
{
	char	szID[16];		// 사용자 아이디
	char	szNick[20]; // 닉네임수정
}IDLIST_ITEM;


enum REPORT_KIND 
{
	REPORT_KIND_NULL=0,
	REPORT_KIND_SWEAR,		// 욕설 및 불건전대화
	REPORT_KIND_MONEYDEAL,	//게임머니 거래 및 상업성 광고신고
	REPORT_KIND_SCAM,		//짜고치기

};
class NMDIALOG_CLASS CBadUserDlg : public CDialog
{
// Construction
public:
	CBadUserDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CBadUserDlg();

	static int m_SubmitCnt;
	CMyBitmap Back;
	CHideChatList *m_pHideChat;

	CRect	m_rectTargetList;

	CString		m_strTargetID;
	CString		m_TargetNickName;
	BOOL m_bAnnounce;
	REPORT_KIND m_Kind;

	list< IDLIST_ITEM > m_vIDList;
	
	void	ADD_BadUserNickName( char* szNickName );
	void	AddUser( char *szID, char *szNick );
	CString	FindID( CString szNick );
	void	ClearIDList();

// Dialog Data
	//{{AFX_DATA(CBadUserDlg)
	enum { IDD = IDD_BADUSER };	
	CBmpCheckButton	m_btnCheck;
	CListBox	  m_listTarget;
	CBmpRadioBtn  m_radioReport1, m_radioReport2, m_radioReport3;
	CMyButton	m_CancelBtn;
	CMyButton	m_OkBtn;
	CEdit		m_edtReason;
	CString	m_SubmitReason;	
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBadUserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CBadUserDlg)
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeListTargetid();
	afx_msg void OnEnChangeEditReason();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BADUSERDLG_H__F991EC81_C025_11D4_97A4_0050BF0FBE67__INCLUDED_)
