#pragma once

class UPG_COMMON_EXPORT CGoldAttendanceBookSubPopup : public LSY::CLSYLibDialogBase
{
public:
	enum ENUM_TIMER_TYPE
	{
		TIMER_CLOSE_DLG,
	};
private:
	LSY::CImage *m_pImgBack; // ��� �̹���
	LSY::CButton *m_pBtnOk; // ����ϱ� ��ư
	LSY::CButton *m_pBtnClose; // �����ϱ� ��ư
	LSY::CButton *m_pBtnXClose; // X ��ư
	LSY::CText *m_pTxtGold1; // ȹ�� ��� �ؽ�Ʈ
	LSY::CText *m_pTxtGold2; // ȹ�� ��� �ؽ�Ʈ
	
	bool m_bCloseParent;
	int m_nPopupType;
	INT64 m_nGold;


	
public:
	CGoldAttendanceBookSubPopup(CWnd* pParent = NULL);
	~CGoldAttendanceBookSubPopup(void);
	
	DECLARE_MESSAGE_MAP()
public:
	// ���� �Լ�
	virtual INT_PTR DoModal(CWnd *pParent);
	virtual bool OnCreatedProject(void);
	// �޽�����
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// �̺�Ʈ
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	
	// ����� ����
	void InitLSYControl(); // LSY Control ����
	void InitData(); // ������ ����
	void CloseDlg(void);
	void HandleResponse(int Signal, int TotSize, char *lpData); // ���� ���� �ڵ鸵
	
	void SetPopupType(int nType, INT64 nGold, bool bModaled); // 1 : ����!, 2 : ����!, 3 : ���� ����, 4 : ����
	bool GetCloseParent();
	void GetNumberWithComma(CString parm_data, CString &parm_string);
	void GetNumberWithComma2(INT64 nNum, char cBuffer[]);
};
