#pragma once

class UPG_COMMON_EXPORT CGoldMileagePopup : public LSY::CLSYLibDialogBase
{
public:
	// �ݱ� ��ư
	LSY::CButton *m_pBtnClose;
	// Ȯ�� ��ư
	LSY::CButton *m_pBtnOk;
	// ��� �ؽ�Ʈ
	LSY::CText *m_pTxtGoldAmount;
	
	INT64 m_TempGold; //������Ʈ�Ǵ� �ӽ�(������) ��� �ݾ��̴�.
	INT64 m_RealGold; //�ջ�Ǿ� ���� �����ϰԵ� ��� �ݾ��̴�.(���� �����Ӵ� ���� �Ѿ�)
	
public:
	// ������, �Ҹ���
	CGoldMileagePopup(CWnd* pParent = NULL);
	~CGoldMileagePopup(void);

	// ���� �Լ�
	virtual INT_PTR DoModal(CWnd *pParent);
	virtual bool OnCreatedProject(void);
	// �̺�Ʈ
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	
	// ����� ����
	void InitLSYControl(); // LSY Control ����
	void InitData(INT64 nTempGold); // ������ ����
	void CloseDlg(void);
	void HandleResponse(int Signal, int TotSize, char *lpData); // ���� ���� �ڵ鸵
};
