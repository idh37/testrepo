#pragma once

class UPG_COMMON_EXPORT CGoldChangeDlg : public LSY::CLSYLibDialogBase
{
public:
	LSY::CButton		*m_pBtn10;
	LSY::CButton		*m_pBtn100;
	LSY::CButton		*m_pBtn1000;
	LSY::CButton		*m_pBtn10000;
	LSY::CButton		*m_pBtn100000;
	LSY::CButton		*m_pBtnRemove;
	LSY::CButton		*m_pBtnMax;
	LSY::CButton		*m_pBtnChange;
	LSY::CButton		*m_pBtnClose;

	LSY::CTextNumber	*m_pTxtCurMoney;
	LSY::CTextNumber	*m_pTxtCurGold;
	LSY::CEdit			*m_pTxtChangeGold;
	LSY::CTextNumber	*m_pTxtResultMoney;

	INT64				m_nChangeGold;
	INT64				m_nMaxGold;
	
public:
	// 생성자
	CGoldChangeDlg(CWnd* pParent = NULL);
	virtual ~CGoldChangeDlg(){}

	// 이벤트
	virtual bool OnCreatedProject(void);
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnInit(WPARAM &wParam, LPARAM &lParam);

	virtual INT_PTR DoModal(CWnd *pParent);
	void	Init();

protected:
	// 이벤트
	LRESULT OnKeyUpEdit(LSY::CMessage *msg);
	
	void SetChangeGold(INT64 nChangeGold);
	bool ChangeGold(INT64 addValue);
	bool ValidationCheckChange();		// 골드 텍스트 박스 유효성 검사(골드 교환 버튼 클릭 시 호출)

	DECLARE_MESSAGE_MAP()
};
