#pragma once

#include "ChipAttendSub/ChipAttendSubPopup.h"

class UPG_COMMON_EXPORT CChipAttendPopup : public LSY::CLSYLibDialogBase
{
private:
	// LSY Control
	LSY::CSprite *m_pSprWheel;
	LSY::CButton *m_pBtnStart;
	LSY::CButton *m_pBtnClose;
	LSY::CText *m_pTxtDate;

	// 서버에서 받은 값
	// CSV_PM_WHEEL_USER_BET_RESULT
	INT64 m_RoundReward; //도전한 획득하게되는 보상금
	int m_RewardIndex;
	// CSV_PM_WHEEL_WINNER_NOTI
	char *m_UserID; //아이디 혹은 닉네임
	INT64 m_RewardMoney; //당첨금
	int m_RoundWinner; //해당 라운드 당첨자 수
	
	// 기타 변수
	bool m_bInProgress; // 게임 진행 중인지 여부
	bool m_bTest; // 서버 패킷 없이 진행
	
	// 클래스
	CChipAttendSubPopup m_popChipAttendSub;
public:
	CChipAttendPopup(CWnd* pParent = NULL);
	~CChipAttendPopup();
	
protected:
	DECLARE_MESSAGE_MAP()
public:
	// 상속 이벤트
	virtual INT_PTR DoModal(CWnd *pParent);
	//virtual BOOL OnInitDialog(); // LSY Object 로드 전
	virtual bool OnCreatedProject(); // LSY Object 로드 후
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnCancel();
	// 메시지
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	// 이벤트
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	
	// 사용자 정의
	void InitLSYControl(); // LSY Control 연결
	void SetData(); // 데이터 세팅
	void HandleResponse(int Signal, int TotSize, char *lpData); // 서버 응답 핸들링
	int GetWheelAngleIdx();
};
