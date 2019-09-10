#pragma once

class UPG_COMMON_EXPORT CGoldSupportFundPopup :
	public LSY::CLSYLibDialogBase
{
private:
	// �˾�â
	LSY::CWindows *m_pWndPopup;
	// Ȯ�� ��ư
	LSY::CButton *m_pBtnOk;
	//������ �̹���
	LSY::CImage *m_pImgFund;

private:
	int m_Grade;

public:
	CGoldSupportFundPopup(CWnd* pParent = NULL);
	~CGoldSupportFundPopup(void);
	
	// ���� �Լ�
	virtual INT_PTR DoModal(CWnd *pParent);
	virtual bool OnCreatedProject(void);
	// �̺�Ʈ
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	
	// ����� ����
	void InitLSYControl(); // LSY Control ����
	void InitData(int nGrade); // ������ ����
	void CloseDlg(void);
	void HandleResponse(int Signal, int TotSize, char *lpData); // ���� ���� �ڵ鸵
};
