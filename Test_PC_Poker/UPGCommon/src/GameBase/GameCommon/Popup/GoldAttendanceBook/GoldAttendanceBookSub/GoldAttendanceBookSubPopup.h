#pragma once

class UPG_COMMON_EXPORT CGoldAttendanceBookSubPopup : public LSY::CLSYLibDialogBase
{
public:
	enum ENUM_TIMER_TYPE
	{
		TIMER_CLOSE_DLG,
	};
private:
	LSY::CImage *m_pImgBack; // 배경 이미지
	LSY::CButton *m_pBtnOk; // 계속하기 버튼
	LSY::CButton *m_pBtnClose; // 종료하기 버튼
	LSY::CButton *m_pBtnXClose; // X 버튼
	LSY::CText *m_pTxtGold1; // 획득 골드 텍스트
	LSY::CText *m_pTxtGold2; // 획득 골드 텍스트
	
	bool m_bCloseParent;
	int m_nPopupType;
	INT64 m_nGold;


	
public:
	CGoldAttendanceBookSubPopup(CWnd* pParent = NULL);
	~CGoldAttendanceBookSubPopup(void);
	
	DECLARE_MESSAGE_MAP()
public:
	// 가상 함수
	virtual INT_PTR DoModal(CWnd *pParent);
	virtual bool OnCreatedProject(void);
	// 메시지맵
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 이벤트
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	
	// 사용자 정의
	void InitLSYControl(); // LSY Control 연결
	void InitData(); // 데이터 세팅
	void CloseDlg(void);
	void HandleResponse(int Signal, int TotSize, char *lpData); // 서버 응답 핸들링
	
	void SetPopupType(int nType, INT64 nGold, bool bModaled); // 1 : 성공!, 2 : 실패!, 3 : 최종 성공, 4 : 종료
	bool GetCloseParent();
	void GetNumberWithComma(CString parm_data, CString &parm_string);
	void GetNumberWithComma2(INT64 nNum, char cBuffer[]);
};
