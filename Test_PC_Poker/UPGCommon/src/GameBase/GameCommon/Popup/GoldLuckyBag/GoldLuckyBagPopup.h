#pragma once

class UPG_COMMON_EXPORT CGoldLuckyBagPopup : public LSY::CLSYLibDialogBase
{
public:
	// 닫기 버튼
	LSY::CButton *m_pBtnClose;
	
	// 응모하기 버튼
	LSY::CButton *m_pBtnEnter1;
	LSY::CButton *m_pBtnEnter2;
	LSY::CButton *m_pBtnEnter3;
	LSY::CButton *m_pBtnEnter4;
	
	// 나의 황금 복주머니 갯수
	LSY::CImage *m_pImgMyBagNum1;
	LSY::CImage *m_pImgMyBagNum2;
	LSY::CImage *m_pImgMyBagNum3;
	LSY::CImage *m_pImgMyBagNum4;
	LSY::CImage *m_pImgMyBagNum5;
	
	// 골드 응모 횟수
	LSY::CImage *m_pImgEnterNum_gold1;
	LSY::CImage *m_pImgEnterNum_gold2;
	LSY::CImage *m_pImgEnterNum_gold3;
	LSY::CImage *m_pImgEnterNum_gold4;
	LSY::CImage *m_pImgEnterNum_gold5;
	LSY::CImage *m_pImgEnterNum_gold6;
	
	// 머니 응모 횟수
	LSY::CImage *m_pImgEnterNum_money1;
	LSY::CImage *m_pImgEnterNum_money2;
	LSY::CImage *m_pImgEnterNum_money3;
	LSY::CImage *m_pImgEnterNum_money4;
	LSY::CImage *m_pImgEnterNum_money5;
	LSY::CImage *m_pImgEnterNum_money6;
	
	// 보너스 쿠폰 30% 응모 횟수
	LSY::CImage *m_pImgEnterNum_301;
	LSY::CImage *m_pImgEnterNum_302;
	LSY::CImage *m_pImgEnterNum_303;
	LSY::CImage *m_pImgEnterNum_304;
	LSY::CImage *m_pImgEnterNum_305;
	LSY::CImage *m_pImgEnterNum_306;
	
	// 보너스 쿠폰 50% 응모 횟수
	LSY::CImage *m_pImgEnterNum_501;
	LSY::CImage *m_pImgEnterNum_502;
	LSY::CImage *m_pImgEnterNum_503;
	LSY::CImage *m_pImgEnterNum_504;
	LSY::CImage *m_pImgEnterNum_505;
	LSY::CImage *m_pImgEnterNum_506;
	
	// 필요한 복주머니 갯수
	LSY::CImage *m_pImgBagGold1;
	LSY::CImage *m_pImgBagGold2;
	LSY::CImage *m_pImgBagGold3;
	LSY::CImage *m_pImgBagGold4;
	
public:
	CGoldLuckyBagPopup(CWnd* pParent = NULL);
	~CGoldLuckyBagPopup(void);
	
	// 가상 함수
	virtual INT_PTR DoModal(CWnd *pParent);
	virtual bool OnCreatedProject(void);
	// 이벤트
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	
	// 사용자 정의
	void InitLSYControl(); // LSY Control 연결
	void InitData(); // 데이터 세팅
	void CloseDlg(void);
	void HandleResponse(int Signal, int TotSize, char *lpData); // 서버 응답 핸들링
};
