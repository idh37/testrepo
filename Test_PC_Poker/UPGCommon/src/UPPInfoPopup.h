#pragma once

#include "resource.h"
#include "UPGDlgBase.h"

enum TYPE_UPPPOPUP
{
	TYPE_UPPPOPUP_GRADE1_STEP1=0,
	TYPE_UPPPOPUP_GRADE1_STEP2,	
	TYPE_UPPPOPUP_GRADE2,	
	TYPE_UPPPOPUP_GRADE3_STEP1,	
	TYPE_UPPPOPUP_GRADE3_STEP2
};
class UPG_COMMON_EXPORT CUPPInfoPopup  : public CUPGDlgBase
{
public:
	CUPPInfoPopup (CWnd* pParent = NULL);   // standard constructor
	virtual ~CUPPInfoPopup ();

	enum { IDD = IDD_DIALOG_UPP_INFO };
	NMBASE::SKINGDI::CMyButton	m_btnOK;
	CRect	m_rtID;

	CRect	m_rtRemainTime;
	CRect	m_rtRemainRound;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
protected:
	NMBASE::SKINGDI::CMyBitmap			m_Back;
	NMBASE::SKINGDI::CHTransRegion		m_Rgn;
	CString m_strUserID;

	TYPE_UPPPOPUP m_ePopupType;

	CString m_strRemainTime;
	CString m_strRemainRound;

public:
	void SetRemainTimeRound(int nRemainTimeSec, int nRemainRound);

	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnClickEvent1();
	afx_msg void OnClickEvent2();
	afx_msg void OnClickEvent3();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);	

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	virtual INT_PTR DoModal(TYPE_UPPPOPUP eType);
};