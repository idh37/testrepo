#pragma once

class UPG_COMMON_EXPORT CGoldLuckyBagPopup : public LSY::CLSYLibDialogBase
{
public:
	// �ݱ� ��ư
	LSY::CButton *m_pBtnClose;
	
	// �����ϱ� ��ư
	LSY::CButton *m_pBtnEnter1;
	LSY::CButton *m_pBtnEnter2;
	LSY::CButton *m_pBtnEnter3;
	LSY::CButton *m_pBtnEnter4;
	
	// ���� Ȳ�� ���ָӴ� ����
	LSY::CImage *m_pImgMyBagNum1;
	LSY::CImage *m_pImgMyBagNum2;
	LSY::CImage *m_pImgMyBagNum3;
	LSY::CImage *m_pImgMyBagNum4;
	LSY::CImage *m_pImgMyBagNum5;
	
	// ��� ���� Ƚ��
	LSY::CImage *m_pImgEnterNum_gold1;
	LSY::CImage *m_pImgEnterNum_gold2;
	LSY::CImage *m_pImgEnterNum_gold3;
	LSY::CImage *m_pImgEnterNum_gold4;
	LSY::CImage *m_pImgEnterNum_gold5;
	LSY::CImage *m_pImgEnterNum_gold6;
	
	// �Ӵ� ���� Ƚ��
	LSY::CImage *m_pImgEnterNum_money1;
	LSY::CImage *m_pImgEnterNum_money2;
	LSY::CImage *m_pImgEnterNum_money3;
	LSY::CImage *m_pImgEnterNum_money4;
	LSY::CImage *m_pImgEnterNum_money5;
	LSY::CImage *m_pImgEnterNum_money6;
	
	// ���ʽ� ���� 30% ���� Ƚ��
	LSY::CImage *m_pImgEnterNum_301;
	LSY::CImage *m_pImgEnterNum_302;
	LSY::CImage *m_pImgEnterNum_303;
	LSY::CImage *m_pImgEnterNum_304;
	LSY::CImage *m_pImgEnterNum_305;
	LSY::CImage *m_pImgEnterNum_306;
	
	// ���ʽ� ���� 50% ���� Ƚ��
	LSY::CImage *m_pImgEnterNum_501;
	LSY::CImage *m_pImgEnterNum_502;
	LSY::CImage *m_pImgEnterNum_503;
	LSY::CImage *m_pImgEnterNum_504;
	LSY::CImage *m_pImgEnterNum_505;
	LSY::CImage *m_pImgEnterNum_506;
	
	// �ʿ��� ���ָӴ� ����
	LSY::CImage *m_pImgBagGold1;
	LSY::CImage *m_pImgBagGold2;
	LSY::CImage *m_pImgBagGold3;
	LSY::CImage *m_pImgBagGold4;
	
public:
	CGoldLuckyBagPopup(CWnd* pParent = NULL);
	~CGoldLuckyBagPopup(void);
	
	// ���� �Լ�
	virtual INT_PTR DoModal(CWnd *pParent);
	virtual bool OnCreatedProject(void);
	// �̺�Ʈ
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	
	// ����� ����
	void InitLSYControl(); // LSY Control ����
	void InitData(); // ������ ����
	void CloseDlg(void);
	void HandleResponse(int Signal, int TotSize, char *lpData); // ���� ���� �ڵ鸵
};
