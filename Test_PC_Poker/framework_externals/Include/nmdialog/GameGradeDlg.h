#if !defined(AFX_DECISIONGAMBLEDLG_H__89DE355A_6DEA_4DA6_965F_4661CAE8465F__INCLUDED_)
#define AFX_DECISIONGAMBLEDLG_H__89DE355A_6DEA_4DA6_965F_4661CAE8465F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DecisionGambleDlg.h : header file
//

#include "../resource.h"
#include "DialogDefine.h"
#include "MyBitmap.h"
/////////////////////////////////////////////////////////////////////////////
// CGameGradeDlg dialog

class NMDIALOG_CLASS CGameGradeDlg : public CDialog
{
// Construction
public:
	CGameGradeDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL Create(CWnd* pParentWnd);
	BOOL Create(CWnd *pParentWnd, int x, int y, int UseMode);


	void MoveDlg(int x, int y);

	void DoModal(CWnd* pParent, int Mode);

	CWnd*		m_pParent;
	UINT		m_hTimer;
	CMyBitmap	m_Back;
	int			m_UseMode;

	int m_PosX;
	int m_PosY;

// Dialog Data
	//{{AFX_DATA(CGameGradeDlg)
	enum { IDD = IDD_DIALOG_GAME_GRADE };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGameGradeDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGameGradeDlg)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DECISIONGAMBLEDLG_H__89DE355A_6DEA_4DA6_965F_4661CAE8465F__INCLUDED_)
