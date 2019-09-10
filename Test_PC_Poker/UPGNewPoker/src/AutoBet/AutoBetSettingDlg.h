#if !defined(AFX_AUTOBETSETTINGDLG_H__CBFB06B0_9CC4_42CD_A644_D89C668AF014__INCLUDED_)
#define AFX_AUTOBETSETTINGDLG_H__CBFB06B0_9CC4_42CD_A644_D89C668AF014__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoBetSettingDlg.h : header file
//
#include "../resource.h"

/////////////////////////////////////////////////////////////////////////////
// CAutoBetSettingDlg dialog
#define BETLIST_W 7
#define BETLIST_H 5

#define BET_TIME			(WM_USER+50010)
#define MAX_BETNUM	45

#define IDM_OKBTN			(WM_USER+7500)
#define IDM_NOBTN			(WM_USER+7501)

class CAutoBetSettingDlg : public CUPGDlgBase
{
// Construction
public:
	CAutoBetSettingDlg(char *ID,int betkind = 0,BOOL nodie = FALSE,BOOL bnoedit = FALSE,BOOL bmy = FALSE,int gamemonde = 3,CWnd* pParent = NULL);   // standard constructor
public:
	int			m_listBetDect[MAX_BETNUM];	// Ŭ���ϱ� ���� �迭
	int			m_nBetRule;					// 591 ~ �̷� �ε��� ��
	int			m_nBetClickIndex;			// Ŭ�� �Ǿ��� �ε���
	BOOL		m_bNodie;					// ����� ���̹�ư ��Ȱ��ȭ
	BOOL		m_bNoEdit;					// �濡 ���� �Ǿ� ������ ������ �� �̻� ���� ����

	int			m_nRoomBetRule;				// �� ���� ���� ����
	BOOL		m_bRoomNodie;				// �� ����

	BOOL		m_bMy;						// ������ ���岨��
	BOOL		m_bNoEditClick;				// ���ٲٴ� ���� Ŭ�� ������

	int			m_nBetToolTipIndex;			// ���� �ε���

	int			m_nGameMode;				// ���� ���� 
	int			m_nMaxBetNum;				// Ƚ��

	CString		m_strSettingRule;			// �����ǰ� �ִ� ���÷� ��	

	NMBASE::GRAPHICGDI::CPage		m_cPage;					//�׷��� 
	CString		m_strMyID;
	
	NMBASE::SKINGDI::CGraphButton	m_ctrlOKBtn;
	NMBASE::SKINGDI::CGraphButton	m_ctrlNoBtn;	

	UINT		m_nTimeEvent;

	void Draw(CDC *dc);

	void SetComboIndex(int num);
	void SetBettingDeck(int num = 0);
	void SetBettingDeckPutCall(int num = 0,int cnum = 0,BOOL bguf = FALSE);
	int GetBettingTotalNum();

	int GetGuTotalNum(int num);
	void SetNoEditModeBettingDeck(int num);

	void OnInvalid();

// Dialog Data
	//{{AFX_DATA(CAutoBetSettingDlg)
	enum { IDD = IDD_BETSETTING_DLG };
	CComboBox	m_cmbBetList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoBetSettingDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAutoBetSettingDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void OnOK();
	afx_msg void OnSelchangeComboBetlist();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOBETSETTINGDLG_H__CBFB06B0_9CC4_42CD_A644_D89C668AF014__INCLUDED_)
