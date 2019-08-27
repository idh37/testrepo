
#pragma once

#include "ChuSeokSub/ChuSeokSubPopup.h"


class UPG_COMMON_EXPORT CChuSeokPopup : public LSY::CLSYLibDialogBase
{
private:
	// LSY Control
	LSY::CText* m_pTxtDay;
	LSY::CText* m_pTxtChip;
	LSY::CButton* m_pBtnOk;

	// 서버에서 받는 값
	int m_RewardDay;		// 접속 일차
	INT64 m_RewardMoney;	// 보상금

	// 클래스
	CChuSeokSubPopup m_popChuSeokSub;

	// 기타 변수
	bool m_bTest; // 서버 패킷 없이 진행

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
