#pragma once
#include "resource.h"
#include "UPGDlgBase.h"

class UPG_COMMON_EXPORT CRestrictionInfoPopup  : public CUPGDlgBase
{
public:
	CRestrictionInfoPopup (CWnd* pParent = NULL);   // standard constructor
	virtual ~CRestrictionInfoPopup ();

	enum { IDD = IDD_DIALOG_RESTRICTION };
	
	NMBASE::SKINGDI::CMyButton	m_btnOK;
	NMBASE::SKINGDI::CMyButton	m_btnExplain;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
protected:
	NMBASE::SKINGDI::CMyBitmap			m_Back;
	NMBASE::SKINGDI::CHTransRegion		m_Rgn;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);	

	afx_msg void OnBnClickedExplain();

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	virtual INT_PTR DoModal();
};