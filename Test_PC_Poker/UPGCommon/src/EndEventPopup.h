#pragma once

#include "afxwin.h"
//#include "GlobalBase.h"

class UPG_COMMON_EXPORT CEndEventBannerDlg : public LSY::CLSYLibDialogBase
{
public:
	CEndEventBannerDlg(CWnd* pParent = NULL);
	virtual ~CEndEventBannerDlg(){}

	LSY::CWindows *m_pWndBack;

public:
	virtual BOOL OnInitDialog();
	virtual bool OnCreatedProject(void);

	virtual INT_PTR Create(CWnd *pParent);
	BOOL PreTranslateMessage(MSG* pMsg);
	void Close(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);
protected:
	DECLARE_MESSAGE_MAP()
};

class UPG_COMMON_EXPORT CEndEventPopupDlg : public LSY::CLSYLibDialogBase
{

public:
	CEndEventPopupDlg(CWnd* pParent = NULL);
	virtual ~CEndEventPopupDlg(){}

	LSY::CWindows *m_pWndBack;
	CEndEventBannerDlg m_EndEventBannerDlg;

public:
	virtual BOOL OnInitDialog();
	virtual bool OnCreatedProject(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnClickButton2(LSY::CMessage *pMsg);
	void SetMissionStep(LSY::CImage *pImg, int step);

	virtual INT_PTR DoModal(CWnd *pParent);
	afx_msg void OnMove(int x, int y);

protected:
	DECLARE_MESSAGE_MAP()
};