
#pragma once

#include "../../resource.h"


class UPG_LOBBY_EXPORT CCaptchaDlg : public CUPGDlgBase
{
public:

	NMBASE::SKINGDI::CMyBitmap m_backGround;
	NMBASE::SKINGDI::CMyButton m_closeBtn;

	NMBASE::UTIL::CNMSSOWebBrowser* m_captchaWeb;
	
	enum { IDD = IDD_DIALOG_CAPTCHA };	

private:

	CString m_authentiCode;

	bool m_needCaptcha;

public:

	CCaptchaDlg(CWnd* pParent = NULL);
	virtual ~CCaptchaDlg();

	BOOL SafeClose();

	CString GetAuthentiCode() const;
	void SetAuthentiCode(CString authenticode);
	bool GetNeedCaptcha() const;
	void SetNeedCaptcha(bool needCaptcha);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
protected:

	virtual void DoDataExchange(CDataExchange* pDX);
	virtual BOOL OnInitDialog();	

	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);	
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
};