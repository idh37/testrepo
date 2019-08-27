#pragma once

#include <vector>

class UPG_COMMON_EXPORT CNewYearAttendancePopup : public LSY::CLSYLibDialogBase
{
public:
	// 닫기 버튼
	LSY::CButton *m_pBtnClose;
	
	// 출석 완료
	LSY::CImage *(m_pImgCheck[28]);
	
	// 연속 출석 완료
	LSY::CImage *(m_pImgConCheck[4]);
	
	// 연속 출석 일 수
	LSY::CImage *m_pImgContinuous1;
	LSY::CImage *m_pImgContinuous2;
	
	// CSV_PROMOTION_COMMON_ATTENDANCE_USERINFO
	int m_Attendance_ID; //출석부 ID
	int m_NormalCount; //일반 누적횟수
	int m_ContiCount; //연속 누적횟수
	
public:
	// 생성자, 소멸자
	CNewYearAttendancePopup(CWnd* pParent = NULL);
	~CNewYearAttendancePopup(void);
	
	// 가상 함수
	virtual INT_PTR DoModal(CWnd *pParent);
	virtual bool OnCreatedProject(void);
	// 이벤트
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	
	// 사용자 정의
	void InitLSYControl(); // LSY Control 연결
	void InitData(int normalCount, int contiCount); // 데이터 세팅
	void CloseDlg(void);
	void HandleResponse(int Signal, int TotSize, char *lpData); // 서버 응답 핸들링
};
