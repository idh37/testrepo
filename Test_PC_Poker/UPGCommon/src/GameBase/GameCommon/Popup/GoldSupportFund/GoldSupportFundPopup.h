#pragma once

class UPG_COMMON_EXPORT CGoldSupportFundPopup :
	public LSY::CLSYLibDialogBase
{
private:
	// 팝업창
	LSY::CWindows *m_pWndPopup;
	// 확인 버튼
	LSY::CButton *m_pBtnOk;
	//지원금 이미지
	LSY::CImage *m_pImgFund;

private:
	int m_Grade;

public:
	CGoldSupportFundPopup(CWnd* pParent = NULL);
	~CGoldSupportFundPopup(void);
	
	// 가상 함수
	virtual INT_PTR DoModal(CWnd *pParent);
	virtual bool OnCreatedProject(void);
	// 이벤트
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	
	// 사용자 정의
	void InitLSYControl(); // LSY Control 연결
	void InitData(int nGrade); // 데이터 세팅
	void CloseDlg(void);
	void HandleResponse(int Signal, int TotSize, char *lpData); // 서버 응답 핸들링
};
