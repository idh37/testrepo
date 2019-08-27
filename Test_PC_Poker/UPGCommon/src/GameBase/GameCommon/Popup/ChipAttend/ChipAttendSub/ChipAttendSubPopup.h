#pragma once

class UPG_COMMON_EXPORT CChipAttendSubPopup : public LSY::CLSYLibDialogBase
{
private:
	LSY::CImage *m_pImgBack; // 배경 이미지
	LSY::CText *m_pTxtChip; // 획득 골드 텍스트
	LSY::CButton *m_pBtnOk; // OK 버튼
	
	// 1 : 400 ~ 1,638,400 당첨 , 2 : 6,553,600 / 10,000,000 당첨
	int m_nPopupType;
	INT64 m_nRewardChip;
public:
	CChipAttendSubPopup(CWnd* pParent = NULL);
	~CChipAttendSubPopup(void);
	
	DECLARE_MESSAGE_MAP()
public:
	// 가상 함수
	virtual INT_PTR DoModal(CWnd *pParent);
	virtual bool OnCreatedProject(void);
	// 메시지
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	// 이벤트
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	
	// 사용자 정의
	void InitLSYControl(); // LSY Control 연결
	void InitData(); // 데이터 세팅
	void CloseDlg(void);
	
	void SetPopupType(int nType, INT64 nChip, bool bModaled); // 1 : 성공!, 2 : 실패!, 3 : 최종 성공, 4 : 종료
};
