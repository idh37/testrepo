#pragma once

#include "GoldAttendanceBookSub/GoldAttendanceBookSubPopup.h"

class UPG_COMMON_EXPORT CGoldAttendanceBookPopup : public LSY::CLSYLibDialogBase
{
public:
	enum ENUM_TIMER_TYPE
	{
		TIMER_CARD_OUT,
		TIMER_SUCCESS_ANI1,
		TIMER_SUCCESS_ANI2,
		TIMER_SUCCESS_DLG,
		TIMER_FAIL_ANI1,
		TIMER_FAIL_ANI2,
		TIMER_FAIL_DLG,
		TIMER_GAME_RESULT,
		TIMER_WINNER_NOTI,
		TIMER_WINNER_NOTI_END,
	};
private:
	LSY::CSprite *m_pSprBack;
	LSY::CSprite *m_pSprOdd;
	LSY::CSprite *m_pSprEven;
	LSY::CSprite *m_pSprCard;
	LSY::CSprite *m_pSprSuccess;
	LSY::CSprite *m_pSprNotice;
	LSY::CText *m_pTxtWinNum;
	LSY::CText *m_pTxtGold;
	LSY::CText *m_pTxtWinGold;
	LSY::CImage *m_pImgCard;
	LSY::CButton *m_pBtnOdd;
	LSY::CButton *m_pBtnEven;
	LSY::CButton *m_pBtnClose;
	
	// 서버에서 받아오는 값
	int m_Round_Cur; //단계 1 ~ 8까지 있음
	INT64 m_CurrentReward; //지금까지 확보된 보상금액(배팅실패, 혹은 게임포기를 하면 받게 되는 보상금)
	INT64 m_WinReward; //이번 라운드에서 배팅성공시 받게되는 보상금액이다.
	int m_BetTime; //배팅제한시간이다(초), 만일 -1로 가면 제한이 없다는 의미
	
	bool m_BetSuccess; //성공여부
	int m_Round; //도전한 라운드
	int m_CardNum; //이번판 결과로 부여된 카드번호이다.(세븐포커 카드 인덱스와 동일함)
	INT64 m_RoundReward; //도전한 라운드에 배팅성공시 획득하게되는 보상금
	
	//char *m_UserID; //아이디 혹은 닉네임 -> 지금 필요 없으므로 일단 패스
	int m_WinRound; //당첨 라운드
	INT64 m_RewardMoney; //당첨금
	int m_RoundWinner; //해당 라운드 당첨자 수
	
	// 
	bool m_bContinue; // 게임 진행 가능 여부
	bool m_bInProgress; // 게임 진행 중인지 여부
	
	// 클래스
	CGoldAttendanceBookSubPopup m_popGoldAttendanceBookSub;
	
public:
	CGoldAttendanceBookPopup(CWnd* pParent = NULL);
	~CGoldAttendanceBookPopup();
	
	DECLARE_MESSAGE_MAP()
public:
	// 가상 함수
	virtual INT_PTR DoModal(CWnd *pParent);
	//virtual BOOL OnInitDialog(); // LSY Object 로드 전
	virtual bool OnCreatedProject(); // LSY Object 로드 후
	virtual void OnCancel();
	// 메시지맵
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 이벤트
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnMouseOver(LSY::CMessage *pMsg);
	LRESULT OnMouseLeave(LSY::CMessage *pMsg);
	LRESULT OnMouseMove(LSY::CMessage *pMsg);
	LRESULT OnLButtonDown(LSY::CMessage *pMsg);
	LRESULT OnLButtonUp(LSY::CMessage *pMsg);
	
	// 사용자 정의
	void InitLSYControl(); // LSY Control 연결
	void InitLSYControlToNULL(); // LSY Control NULL 값으로 초기화
	void SetData(int nRound, bool bShowNoti); // 데이터 세팅
	void CloseDlg(bool bContinue);
	void HandleResponse(int Signal, int TotSize, char *lpData); // 서버 응답 핸들링
	void UserBetResult(); // 유저 배팅 결과
	void Wait(DWORD dwMillisecond);
	int GetPokerSprIndex(int nCardNum);
	void GetNumberWithComma(CString parm_data, CString &parm_string);
	void GetNumberWithComma2(INT64 nNum, char cBuffer[]);
	void CardOut(); // 카드 사출
};
