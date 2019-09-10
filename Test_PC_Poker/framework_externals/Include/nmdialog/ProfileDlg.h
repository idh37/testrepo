#if !defined(AFX_PROFILEDLG_H__2C402B81_30AF_11D4_97A5_0050BF0FBE67__INCLUDED_)
#define AFX_PROFILEDLG_H__2C402B81_30AF_11D4_97A5_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProfileDlg.h : header file
//

#include "resource.h"
#include "UserStruct.h"
#include "CharView.h"

enum PROFILE_OPTION_BUTTON_TYPE
{
	PROFILE_OPTION_BUTTON_TYPE_NONE=0,				//�ݱ� ��ư�� ����
	PROFILE_OPTION_BUTTON_TYPE_FORCEBANISH,			//�߹��ư	
	PROFILE_OPTION_BUTTON_TYPE_WHISPER,				//�Ӹ���ư

};


/////////////////////////////////////////////////////////////////////////////
// CProfileDlg dialog

class NMDIALOG_CLASS CProfileDlg : public CDialog
{
	// Construction
public:

	CProfileDlg(CWnd* pParent = NULL);   // standard constructor
	void Init(USERINFO *pUI, PROFILE_OPTION_BUTTON_TYPE eOptionButtonType=PROFILE_OPTION_BUTTON_TYPE_NONE);	// [���� ������ �۾�] <-- ����




	void ChangeGameTab(IDX_GAME idxGame);


	IDX_GAME		m_idxSelectedGame;


	void ShowGameInfo( IDX_GAME showkind );
	void ShowSubGameInfo( int showkind );
	void OnDrawGameInfo(CDC *pDC);



	void RefreshButton();

	CString GetOptionButtionPath();

private:

	// Dialog Data
	//{{AFX_DATA(CProfileDlg)
	enum { IDD = IDD_PROFILE_DIALOG };
	CMyButton	m_OptionBtn;	
	CMyButton	m_ProfileCloseBtn;
	CMyButton	m_GameTabBtn[MAX_GAME];
	//}}AFX_DATA


	CMyBitmap m_cBack;	
	CCharView m_cCharView;
	CFont m_cFont1, m_cFont2;
	USERINFO m_sUI;
	
	BOOL m_bIsMyInfo;			//���������� 1 ������ , 0 �ٸ��� ����	
	CHTransRegion m_cRgn;
	CString m_strBackPath;


	CRect	 m_RectGameRecord;		//���ӱ�� ����

	PROFILE_OPTION_BUTTON_TYPE m_eOptionButtonType;
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProfileDlg)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProfileDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonDetailprofile();
	afx_msg void OnButtonOption();		
	afx_msg void OnBanishvote();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROFILEDLG_H__2C402B81_30AF_11D4_97A5_0050BF0FBE67__INCLUDED_)
