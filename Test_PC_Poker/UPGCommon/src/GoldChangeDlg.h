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
	// ������
	CGoldChangeDlg(CWnd* pParent = NULL);
	virtual ~CGoldChangeDlg(){}

	// �̺�Ʈ
	virtual bool OnCreatedProject(void);
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnInit(WPARAM &wParam, LPARAM &lParam);

	virtual INT_PTR DoModal(CWnd *pParent);
	void	Init();

protected:
	// �̺�Ʈ
	LRESULT OnKeyUpEdit(LSY::CMessage *msg);
	
	void SetChangeGold(INT64 nChangeGold);
	bool ChangeGold(INT64 addValue);
	bool ValidationCheckChange();		// ��� �ؽ�Ʈ �ڽ� ��ȿ�� �˻�(��� ��ȯ ��ư Ŭ�� �� ȣ��)

	DECLARE_MESSAGE_MAP()
};
