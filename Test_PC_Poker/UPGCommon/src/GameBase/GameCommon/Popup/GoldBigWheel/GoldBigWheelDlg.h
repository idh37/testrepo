#pragma once

class UPG_COMMON_EXPORT CGoldBigWheelDlg : public LSY::CLSYLibDialogBase
{
protected:
	LSY::CWeb			*m_pWebBigWheel;
	LSY::CButton		*m_pBtnStart;
	LSY::CButton		*m_pBtnStop;
	LSY::CButton		*m_pBtnMinus;
	LSY::CButton		*m_pBtnPlus;
	LSY::CButton		*m_pBtnClose;

	LSY::CTextNumber	*m_pTxtMoney;
	LSY::CTextNumber	*m_pTxtGold;

	LSY::CImageNumber	*m_pImgEnterGold;
	LSY::CImageNumber	*m_pImgAutoCnt;
	LSY::CImageNumber	*m_pImgRemainCnt;

	LSY::CText			*m_pTxtRemainCnt;

	INT64		m_enterGold;
	INT64		m_remainCnt;
	int			m_autoCnt;
	int			m_remainAutoCnt;
	UINT		m_nTimeEvent;

public:
	// 생성자
	CGoldBigWheelDlg(CWnd* pParent = NULL);
	virtual ~CGoldBigWheelDlg(){}

	// 이벤트
	virtual bool OnCreatedProject(void);
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnInit(WPARAM &wParam, LPARAM &lParam);
	BOOL PreTranslateMessage( MSG* pMsg );

	virtual INT_PTR DoModal(int remainCnt, INT64 useGold, CWnd *pParent);
	void	Init();
	void	Start();
	void	WheelLoadCompleted();
	void	WheelCompleted();
	void	WheelNext();
	void	StartWheel(int remainCnt, INT64 useGold, INT64 getMoney);
	void	SetAutoCnt(int autoCnt);
	void	ClosePopup();
	bool	IsPossibleStart();

protected:

	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
};
