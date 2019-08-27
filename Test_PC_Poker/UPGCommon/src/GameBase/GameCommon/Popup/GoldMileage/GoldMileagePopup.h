#pragma once

class UPG_COMMON_EXPORT CGoldMileagePopup : public LSY::CLSYLibDialogBase
{
public:
	// 닫기 버튼
	LSY::CButton *m_pBtnClose;
	// 확인 버튼
	LSY::CButton *m_pBtnOk;
	// 골드 텍스트
	LSY::CText *m_pTxtGoldAmount;
	
	INT64 m_TempGold; //업데이트되는 임시(적립된) 골드 금액이다.
	INT64 m_RealGold; //합산되어 실제 보유하게된 골드 금액이다.(이전 보유머니 포함 총액)
	
public:
	// 생성자, 소멸자
	CGoldMileagePopup(CWnd* pParent = NULL);
	~CGoldMileagePopup(void);

	// 가상 함수
	virtual INT_PTR DoModal(CWnd *pParent);
	virtual bool OnCreatedProject(void);
	// 이벤트
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	
	// 사용자 정의
	void InitLSYControl(); // LSY Control 연결
	void InitData(INT64 nTempGold); // 데이터 세팅
	void CloseDlg(void);
	void HandleResponse(int Signal, int TotSize, char *lpData); // 서버 응답 핸들링
};
