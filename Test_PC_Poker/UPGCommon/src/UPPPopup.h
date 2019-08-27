#pragma once

#include "resource.h"
#include "UPGDlgBase.h"

class UPG_COMMON_EXPORT CUPPPopup  : public CUPGDlgBase
{
public:
	CUPPPopup (CWnd* pParent = NULL);   // standard constructor
	virtual ~CUPPPopup ();

	enum { IDD = IDD_DIALOG_UPP_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	NMBASE::SKINGDI::CMyBitmap			m_Back;
	NMBASE::UTIL::CNMSSOWebBrowser	*m_pBrowser;
	CString m_strURL;
	CWnd* m_pParent;
	int m_nUPPPopupType;

	NMBASE::SKINGDI::CMyButton	m_btnOK;

	NMBASE::SKINGDI::CCustomHtml			*m_pCustomHtml;

public:
	void SetURL(const char * pURL);	

	BOOL CreateCustomHtml();
	void DestoryCustomHtml();
	BOOL CreateBrowser();
	void DestroyBrowser();
	 
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnDestroy();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

	void ClosePopup();
	void Cancel();
	DECLARE_DISPATCH_MAP()

public:
	virtual INT_PTR DoModal(int nPopupType = 0 );
};