#pragma once

class UPG_LOBBY_EXPORT CUserInfoDetailDlg : public LSY::CWindows
{
protected:
	LSY::CButton		*m_pBtnDetailClose; // �ɹ��� �󼼺��� �ݱ� ��ư
	LSY::CText			*m_pLevel; // ��� �ؽ�Ʈ
	LSY::CText			*m_pInsurance; // ���� �ؽ�Ʈ
	LSY::CText			*m_pMembers; // �ɹ��� �ؽ�Ʈ
	LSY::CText			*m_pInsuranceGold1; // ��� ���� �ؽ�Ʈ1
	LSY::CText			*m_pInsuranceGold2; // ��� ���� �ؽ�Ʈ2
	LSY::CButton		*m_pBtnInsuranceReceive; // ���� ���޹ޱ� ��ư
	LSY::CButton		*m_pBtnMembersPreview; // �ɹ��� ���ú��� ��ư
	LSY::CButton		*m_pBtnInsuranceGoldReceive; // ��� ���� ���޹ޱ� ��ư
	USERINFO			*m_pUserInfo;
	
	int m_nLevel; // 0�̸� �̰��� ����
	char *m_chArrName; // �����
	INT64 m_i64SaveMoney; // ���� ������ �ݾ�

public:
	// ������, �Ҹ���
	CUserInfoDetailDlg();
	virtual ~CUserInfoDetailDlg();

	// ���� �Լ�
	virtual void OnCreateChild(LSY::CObjectBase *pObject);

	// �̺�Ʈ �Լ�
	LRESULT OnClickButton(LSY::CMessage *msg);

	// �Ϲ� �Լ�
	void SetMyUserInfo(USERINFO *pMyInfo, BOOL bLogin);
	void ShowRoomList(bool bShow);
	void HandleResponse(int Signal, int TotSize, char *lpData); // ���� ���� �ڵ鸵
};