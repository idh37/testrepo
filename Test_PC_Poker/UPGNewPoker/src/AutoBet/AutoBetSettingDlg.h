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
	int			m_listBetDect[MAX_BETNUM];	// 클릭하기 위한 배열
	int			m_nBetRule;					// 591 ~ 이런 인덱스 값
	int			m_nBetClickIndex;			// 클릭 되어진 인덱스
	BOOL		m_bNodie;					// 몇구까지 다이버튼 비활성화
	BOOL		m_bNoEdit;					// 방에 설정 되어 있으면 설정된 값 이상만 변경 가능

	int			m_nRoomBetRule;				// 방 배팅 설정 정보
	BOOL		m_bRoomNodie;				// 방 설정

	BOOL		m_bMy;						// 내꺼냐 방장꺼냐
	BOOL		m_bNoEditClick;				// 못바꾸는 영역 클릭 했을때

	int			m_nBetToolTipIndex;			// 툴팁 인덱스

	int			m_nGameMode;				// 게임 종류 
	int			m_nMaxBetNum;				// 횟수

	CString		m_strSettingRule;			// 설정되고 있는 베팅룰 값	

	NMBASE::GRAPHICGDI::CPage		m_cPage;					//그래픽 
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
