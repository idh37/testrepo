#pragma once
#include "afxwin.h"
//#include "GlobalBase.h"

class UPG_COMMON_EXPORT CRestrictionOverPopup : public LSY::CLSYLibDialogBase
{

public:
	CRestrictionOverPopup(CWnd* pParent = NULL);
	virtual ~CRestrictionOverPopup(){}

	LSY::CWindows *m_pWndBack ;

	LSY::CImage*	m_pImgLimitMoneyover ;
	//LSY::CImage*	m_pImgLimitTimeover ;
	LSY::CImage*	m_pImgToday_Tomorrow;
	LSY::CImageNumber*	m_pImgNumMonth ;
	LSY::CImageNumber*	m_pImgNumDay ;
	LSY::CImageNumber*	m_pImgNumHour ;
	LSY::CImageNumber*	m_pImgNumMin ;
	LSY::CImageNumber*	m_pImgNumMin0 ;

public:
	virtual BOOL OnInitDialog();
	virtual bool OnCreatedProject(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);

	virtual INT_PTR DoModal(CWnd *pParent);

	afx_msg void OnCancel();
protected:
	DECLARE_MESSAGE_MAP()
};