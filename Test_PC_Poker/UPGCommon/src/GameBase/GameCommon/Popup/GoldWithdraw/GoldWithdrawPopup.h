#pragma once

class UPG_COMMON_EXPORT CGoldWithdrawPopup : public LSY::CLSYLibDialogBase
{
public:
	// LSY ��Ʈ�� ����
	LSY::CButton *m_pBtnClose; // �ݱ� ��ư

	LSY::CText *m_pTxtSaveGold; // ���� ��� �ؽ�Ʈ
	LSY::CText *m_pTxtWithdrawGold; // ��� ��� �ؽ�Ʈ
	LSY::CText *m_pTxtMyGold; // ���� �� ��� �ؽ�Ʈ

	LSY::CButton *m_pBtn001; // 10�� ��ư
	LSY::CButton *m_pBtn002; // 30�� ��ư
	LSY::CButton *m_pBtn003; // 50�� ��ư
	LSY::CButton *m_pBtn004; // �ʱ�ȭ ��ư
	LSY::CButton *m_pBtn005; // 100�� ��ư
	LSY::CButton *m_pBtn006; // 200�� ��ư
	LSY::CButton *m_pBtn007; // 500�� ��ư
	LSY::CButton *m_pBtn008; // �ִ� ��ư

	LSY::CText *m_pTxtLimitNormal; // ��� ���� �ѵ� �Ϲ� �ؽ�Ʈ
	LSY::CText *m_pTxtLimitGold; // ��� ���� �ѵ� ��� �ؽ�Ʈ
	LSY::CText *m_pTxtLimitDia; // ��� ���� �ѵ� ���̾� �ؽ�Ʈ

	LSY::CButton *m_pBtnWithdraw; // ����ϱ� ��ư
	
	// ��Ÿ ����
	INT64 m_i64SaveGold; // ���� ���
	INT64 m_i64WithdrawGold; // ��� ���
	INT64 m_i64MyGold; // ���� �� ���

	INT64 m_nLimit; // ���� ���� ��� ���� �ѵ�

	CString m_csLimitNormal; // ��� ���� �ѵ� �Ϲ�
	CString m_csLimitGold; // ��� ���� �ѵ� ���
	CString m_csLimitDia; // ��� ���� �ѵ� ���̾�

	CMsgBoxDlg m_dlgMsgBox;
	
public:
	// ������, �Ҹ��� �Լ�
	CGoldWithdrawPopup(CWnd* pParent = NULL);
	~CGoldWithdrawPopup(void);

	// ���� �Լ�
	virtual INT_PTR DoModal(CWnd *pParent);
	virtual bool OnCreatedProject(void);
	virtual void OnCancel();
	// �̺�Ʈ �Լ�
	LRESULT OnClickButton(LSY::CMessage *pMsg);

	// �˾� �⺻ �Լ�
	void InitLSYControl(); // LSY Control ����
	void InitLSYControlToNULL(); // LSY Control NULL ������ �ʱ�ȭ
	void InitData(); // ������ �ʱⰪ ����
	void SetData(); // ������ ����
	void CloseDlg(void); // â �ݱ�
	void HandleResponse(int Signal, int TotSize, char *lpData); // ���� ���� �ڵ鸵
	
	// ��Ÿ �Լ�
	void RaiseWithdrawGold(INT64 nAmount); // ��� ��� �߰�
	void SetSumBtn();
};
