#pragma once

class UPG_LOBBY_EXPORT CUserInfoDetailDlg : public LSY::CWindows
{
protected:
	LSY::CButton		*m_pBtnDetailClose; // 맴버스 상세보기 닫기 버튼
	LSY::CText			*m_pLevel; // 등급 텍스트
	LSY::CText			*m_pInsurance; // 보험 텍스트
	LSY::CText			*m_pMembers; // 맴버스 텍스트
	LSY::CText			*m_pInsuranceGold1; // 골드 보험 텍스트1
	LSY::CText			*m_pInsuranceGold2; // 골드 보험 텍스트2
	LSY::CButton		*m_pBtnInsuranceReceive; // 보험 지급받기 버튼
	LSY::CButton		*m_pBtnMembersPreview; // 맴버스 혜택보기 버튼
	LSY::CButton		*m_pBtnInsuranceGoldReceive; // 골드 보험 지급받기 버튼
	USERINFO			*m_pUserInfo;
	
	int m_nLevel; // 0이면 미가입 상태
	char *m_chArrName; // 보험명
	INT64 m_i64SaveMoney; // 누적 적립된 금액

public:
	// 생성자, 소멸자
	CUserInfoDetailDlg();
	virtual ~CUserInfoDetailDlg();

	// 가상 함수
	virtual void OnCreateChild(LSY::CObjectBase *pObject);

	// 이벤트 함수
	LRESULT OnClickButton(LSY::CMessage *msg);

	// 일반 함수
	void SetMyUserInfo(USERINFO *pMyInfo, BOOL bLogin);
	void ShowRoomList(bool bShow);
	void HandleResponse(int Signal, int TotSize, char *lpData); // 서버 응답 핸들링
};