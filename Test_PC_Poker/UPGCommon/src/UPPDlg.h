#pragma once

#include "afxwin.h"

class UPG_COMMON_EXPORT CUPPTooltipDlg : public LSY::CLSYLibDialogBase
{
public:
	CUPPTooltipDlg(CWnd* pParent = NULL);
	virtual ~CUPPTooltipDlg(){}

	LSY::CWindows *m_pWndBack ;

public:
	virtual BOOL OnInitDialog();
	virtual bool OnCreatedProject(void);

	virtual INT_PTR Create(CWnd *pParent);
	BOOL PreTranslateMessage(MSG* pMsg);
	void Close(void);
protected:
	DECLARE_MESSAGE_MAP()
};

// UPPDlg
class UPG_COMMON_EXPORT CUPPDlg : public LSY::CLSYLibDialogBase
{

public:
	CUPPDlg(CWnd* pParent = NULL);
	virtual ~CUPPDlg(){}

	LSY::CWindows *m_pWndBack ;

public:
	virtual BOOL OnInitDialog();
	virtual bool OnCreatedProject(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);
	void SetTimeMoney(int *time, int *money);
	void SetButton(LSY::CButton * pButton, bool bShow);

	virtual INT_PTR DoModal(CWnd *pParent, int *level, char* UserCN);
	BOOL PreTranslateMessage(MSG* pMsg);
	void ExitClient(void);
	void ShowTxt(void);
protected:
	DECLARE_MESSAGE_MAP()
	int nLevel;
	int nPlayTime;
	int nBillingMoney;
	char *m_strUserCN;
	CUPPTooltipDlg pDlgTooltip;
	BOOL bCreatedTooltip;
public:
	afx_msg void OnMove(int x, int y);
};
