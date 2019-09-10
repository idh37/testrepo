
#pragma once

#include "ChuSeokSub/ChuSeokSubPopup.h"


class UPG_COMMON_EXPORT CChuSeokPopup : public LSY::CLSYLibDialogBase
{
private:
	// LSY Control
	LSY::CText* m_pTxtDay;
	LSY::CText* m_pTxtChip;
	LSY::CButton* m_pBtnOk;

	// �������� �޴� ��
	int m_RewardDay;		// ���� ����
	INT64 m_RewardMoney;	// �����

	// Ŭ����
	CChuSeokSubPopup m_popChuSeokSub;

	// ��Ÿ ����
	bool m_bTest; // ���� ��Ŷ ���� ����

public:
	CChuSeokPopup(CWnd* pParent = NULL);
	~CChuSeokPopup();

	virtual bool OnCreatedProject();
	virtual INT_PTR DoModal();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	LRESULT OnClickButton(LSY::CMessage* pMsg);
	
	void HandleResponse(int Signal, int TotSize, char *lpData);	

protected:
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

private:
	void InitLSYControl();
	void SetData();
};
