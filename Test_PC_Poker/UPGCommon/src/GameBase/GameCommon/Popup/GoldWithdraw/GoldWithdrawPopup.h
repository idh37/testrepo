#pragma once

class UPG_COMMON_EXPORT CGoldWithdrawPopup : public LSY::CLSYLibDialogBase
{
public:
	// LSY 컨트롤 변수
	LSY::CButton *m_pBtnClose; // 닫기 버튼

	LSY::CText *m_pTxtSaveGold; // 적립 골드 텍스트
	LSY::CText *m_pTxtWithdrawGold; // 출금 골드 텍스트
	LSY::CText *m_pTxtMyGold; // 변경 후 골드 텍스트

	LSY::CButton *m_pBtn001; // 10만 버튼
	LSY::CButton *m_pBtn002; // 30만 버튼
	LSY::CButton *m_pBtn003; // 50만 버튼
	LSY::CButton *m_pBtn004; // 초기화 버튼
	LSY::CButton *m_pBtn005; // 100만 버튼
	LSY::CButton *m_pBtn006; // 200만 버튼
	LSY::CButton *m_pBtn007; // 500만 버튼
	LSY::CButton *m_pBtn008; // 최대 버튼

	LSY::CText *m_pTxtLimitNormal; // 골드 적립 한도 일반 텍스트
	LSY::CText *m_pTxtLimitGold; // 골드 적립 한도 골드 텍스트
	LSY::CText *m_pTxtLimitDia; // 골드 적립 한도 다이아 텍스트

	LSY::CButton *m_pBtnWithdraw; // 출금하기 버튼
	
	// 기타 변수
	INT64 m_i64SaveGold; // 적립 골드
	INT64 m_i64WithdrawGold; // 출금 골드
	INT64 m_i64MyGold; // 변경 후 골드

	INT64 m_nLimit; // 현재 유저 골드 적립 한도

	CString m_csLimitNormal; // 골드 적립 한도 일반
	CString m_csLimitGold; // 골드 적립 한도 골드
	CString m_csLimitDia; // 골드 적립 한도 다이아

	CMsgBoxDlg m_dlgMsgBox;
	
public:
	// 생성자, 소멸자 함수
	CGoldWithdrawPopup(CWnd* pParent = NULL);
	~CGoldWithdrawPopup(void);

	// 가상 함수
	virtual INT_PTR DoModal(CWnd *pParent);
	virtual bool OnCreatedProject(void);
	virtual void OnCancel();
	// 이벤트 함수
	LRESULT OnClickButton(LSY::CMessage *pMsg);

	// 팝업 기본 함수
	void InitLSYControl(); // LSY Control 연결
	void InitLSYControlToNULL(); // LSY Control NULL 값으로 초기화
	void InitData(); // 데이터 초기값 세팅
	void SetData(); // 데이터 세팅
	void CloseDlg(void); // 창 닫기
	void HandleResponse(int Signal, int TotSize, char *lpData); // 서버 응답 핸들링
	
	// 기타 함수
	void RaiseWithdrawGold(INT64 nAmount); // 출금 골드 추가
	void SetSumBtn();
};
