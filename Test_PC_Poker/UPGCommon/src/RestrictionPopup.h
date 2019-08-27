#pragma once
#include "afxwin.h"
//#include "GlobalBase.h"

class UPG_COMMON_EXPORT CRestrictionPopup : public LSY::CLSYLibDialogBase
{

public:
	CRestrictionPopup(CWnd* pParent = NULL);
	virtual ~CRestrictionPopup(){}

	LSY::CWindows *m_pWndBack ;

	LSY::CImage*	m_pImgGameName ;
	LSY::CImage*	m_pImgExplainlossmoney ;

	LSY::CText*		m_pTxtLossMoney;

	//yoo 2015.01 ±‘¡¶æ»
	LSY::CText*		m_pTxt_Game[5];
	LSY::CText*		m_pTxt_Money[5];
	LSY::CImage*	m_pImg_State[5];
	LSY::CButton*		m_pBtnOK;
	LSY::CButton*		m_pBtnPlay;
	IDX_GAME m_nCurrentGame;




public:
	virtual BOOL OnInitDialog();
	virtual bool OnCreatedProject(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);

	LRESULT OnMouseOver(LSY::CMessage *pMsg);
	LRESULT OnMouseLeave(LSY::CMessage *pMsg);

	virtual INT_PTR DoModal(CWnd *pParent, IDX_GAME nCurrentTab);

	afx_msg void OnCancel();
protected:
	DECLARE_MESSAGE_MAP()
};