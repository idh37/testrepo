#pragma once

#include <vector>

class UPG_COMMON_EXPORT CNewYearAttendancePopup : public LSY::CLSYLibDialogBase
{
public:
	// �ݱ� ��ư
	LSY::CButton *m_pBtnClose;
	
	// �⼮ �Ϸ�
	LSY::CImage *(m_pImgCheck[28]);
	
	// ���� �⼮ �Ϸ�
	LSY::CImage *(m_pImgConCheck[4]);
	
	// ���� �⼮ �� ��
	LSY::CImage *m_pImgContinuous1;
	LSY::CImage *m_pImgContinuous2;
	
	// CSV_PROMOTION_COMMON_ATTENDANCE_USERINFO
	int m_Attendance_ID; //�⼮�� ID
	int m_NormalCount; //�Ϲ� ����Ƚ��
	int m_ContiCount; //���� ����Ƚ��
	
public:
	// ������, �Ҹ���
	CNewYearAttendancePopup(CWnd* pParent = NULL);
	~CNewYearAttendancePopup(void);
	
	// ���� �Լ�
	virtual INT_PTR DoModal(CWnd *pParent);
	virtual bool OnCreatedProject(void);
	// �̺�Ʈ
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	
	// ����� ����
	void InitLSYControl(); // LSY Control ����
	void InitData(int normalCount, int contiCount); // ������ ����
	void CloseDlg(void);
	void HandleResponse(int Signal, int TotSize, char *lpData); // ���� ���� �ڵ鸵
};
