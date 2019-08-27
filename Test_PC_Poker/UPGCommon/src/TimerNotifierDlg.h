#pragma once

#include "resource.h"



class UPG_COMMON_EXPORT CTimeNotifierDlg : public CUPGDlgBase
{
protected:
	enum ENUM_TYPE_EXIT_USE
	{
		ENUM_TYPE_EXIT_USE_NONE,
		ENUM_TYPE_EXIT_USE_ONCE,
		ENUM_TYPE_EXIT_USE_ALWAYS
	};
	enum ENUM_TYPE_TIME_NOTIFY
	{
		ENUM_TYPE_TIME_NOTIFY_NONE,
		ENUM_TYPE_TIME_NOTIFY_30MIN,
		ENUM_TYPE_TIME_NOTIFY_1HOUR
	};

	NMBASE::SKINGDI::CMyBitmap		m_imgBack;
	CString			m_strBackFile;
	CRect			m_rtUsingTime;

	NMBASE::SKINGDI::CBmpRadioBtn	m_rdExitNone;
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdExitOnce;
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdExitAlways;

	NMBASE::SKINGDI::CBmpRadioBtn	m_rd1Hour;
	NMBASE::SKINGDI::CBmpRadioBtn	m_rd2Hour;
	NMBASE::SKINGDI::CBmpRadioBtn	m_rd3Hour;
	NMBASE::SKINGDI::CBmpRadioBtn	m_rd4Hour;
	NMBASE::SKINGDI::CBmpRadioBtn	m_rd5Hour;
	NMBASE::SKINGDI::CBmpRadioBtn	m_rd6Hour;

	NMBASE::SKINGDI::CBmpRadioBtn	m_rdNotiNone;
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdNoti30Min;
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdNoti1Hour;

	NMBASE::SKINGDI::CMyButton		m_btnCancel;
	NMBASE::SKINGDI::CMyButton		m_btnOK;

	ENUM_TYPE_EXIT_USE		m_eExitUseType;
	short					m_sExitTime;
	ENUM_TYPE_TIME_NOTIFY	m_eTimeNotifyType;

	COleDateTimeSpan m_proceedTime;
	int m_nTotalRound;
	BOOL m_bEnableInitBtn;

	int m_nPlayTimeSec ;

public:
	CTimeNotifierDlg(CWnd* pParent = NULL);
	virtual ~CTimeNotifierDlg(){}

	enum { IDD = IDD_TIMENOTIFIER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);
protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

//	void RefreshBetRuleToolTip();		//º£ÆÃ·ê ¾È³» ÅøÆÁ °»½Å

	DECLARE_MESSAGE_MAP()

public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
 	afx_msg void OnDestroy();	
	afx_msg void OnTimer(UINT nIDEvent);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual INT_PTR DoModal(CSV_UPPINFO *pMsg );
};