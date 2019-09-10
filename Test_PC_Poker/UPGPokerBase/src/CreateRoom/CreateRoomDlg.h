#pragma once

#include "../resource.h"



class UPG_GAMEBASE_EXPORT CCreateRoomDlg : public CUPGDlgBase
{
protected:
	IDX_GAME m_nGame;
	struct RULE
	{
		INT64 i64Seed;
		INT64 llEnterLimitMoney; // �������� �Ӵ�
		short sAutoBet;
		short sBetRuleKind; // ����Ʈ���÷�
		char cCardSetting; // ī����� (���۽� ī�� �� ��)=������ �߰�
		float fBetMoneyMulti; // ������ : ������ �߰�
	};
	
	struct BetRule
	{
		short sAutoBet;
	};
	
	struct BetRuleToolTipInfo
	{
		bool bBetRuleToolTip; // ���÷� �ȳ� ������ ���������ϴ°�?
		CString str1stLine; // ���÷� �ȳ� ���� �ؽ�Ʈ (1��)
		CString str2ndLine; // ���÷� �ȳ� ���� �ؽ�Ʈ (2��)			
		CRect rtToolTIpRect; // ���÷� �ȳ� ���� �ڽ� (ȭ�鰻�Ź� �ؽ�Ʈ ��ġ)
	};
	
	set<INT64> m_listMinimumMoney;
	vector<INT64> m_listSeedMoney;
	
	typedef map<INT64, map< BetRule*, RULE > > MAP_LIST_RULE;
	typedef map<INT64, map< BetRule*, RULE > >::iterator MAP_LIST_RULE_ITER;
	MAP_LIST_RULE m_listRule;
	
	NMBASE::SKINGDI::CMyBitmap m_imgBack;
	NMBASE::SKINGDI::CMyBitmap m_imgHideMaxDescBack;
	NMBASE::SKINGDI::CMyBitmap m_imgHideMaxGoldDescBack;
	
	CString m_strBackFile;
	CEdit m_edRoomTitle;
	NMBASE::SKINGDI::CBmpRadioBtn m_rdNormal;	
	NMBASE::SKINGDI::CBmpRadioBtn m_rdGold;
	NMBASE::SKINGDI::CBmpRadioBtn m_rdFriend; // ģ�� ���
	NMBASE::SKINGDI::CMyBitmap m_imgRdTypeBG;	// �Ϲ�, ���, ģ�� ��� ���� BG
	NMBASE::SKINGDI::CBmpRadioBtn m_rdMode1; // ���� ��� - [�Ƕ� : ��Ŀ, �Ϲ�]
	NMBASE::SKINGDI::CBmpRadioBtn m_rdMode2;
	// �ο�ٵ��� ����ε� ����
	NMBASE::SKINGDI::CBmpRadioBtn m_rdUnion1;
	NMBASE::SKINGDI::CBmpRadioBtn m_rdUnion2;
	NMBASE::SKINGDI::CMyBitmap m_imgRdUnion;
	int	m_nBottomImgRdUnion;
	// �ο�ٵ��� ����ε� ����
	NMBASE::SKINGDI::CBmpRadioBtn m_rdBlind1;
	NMBASE::SKINGDI::CBmpRadioBtn m_rdBlind2;
	NMBASE::SKINGDI::CMyBitmap m_imgRdBlind;
	// �ο�ٵ��� ��ħ����
	NMBASE::SKINGDI::CBmpRadioBtn m_rdBetRule1;
	NMBASE::SKINGDI::CBmpRadioBtn m_rdBetRule2;
	NMBASE::SKINGDI::CMyBitmap m_imgRdBetRule;
	// �Ƕ�� ���� ��ư
	NMBASE::SKINGDI::CBmpRadioBtn m_rdStop1; // ����ON
	NMBASE::SKINGDI::CBmpRadioBtn m_rdStop2; // ����OFF
	// ��� ���� ���̾Ƹ�� ��� �ƽ��� ���� ���� ��ư
	NMBASE::SKINGDI::CBmpRadioBtn m_rdMaxRoom3;	// �ƽ���
	NMBASE::SKINGDI::CBmpRadioBtn m_rdMaxRoom4;
	NMBASE::SKINGDI::CMyButton m_btnOpenMaxDesc;
	NMBASE::SKINGDI::CMyButton m_btnCloseMaxDesc;
	
	CEdit m_edMRMoney_Input1;
	CEdit m_edMRMoney_Input2;
	
	CComboBox m_cbLimitUserNum;	// ������ ���� �޺��ڽ�
	CComboBox m_cbAutoBetSeed; // �õ�/���÷� �޺��ڽ�
	CComboBox m_cbMinimumMoney;	// �������� �޺��ڽ�
	
	NMBASE::SKINGDI::CMyButton m_btnCancel;
	NMBASE::SKINGDI::CMyButton m_btnOK;
	
	CBrush m_cHollowBrush;
	bool m_bUseWebSelectedRoom; // ������ ���õ� ��ɼ��� ����Ұ��ΰ�?
	ROOMOPTION m_WebSelectedRoomOption; // ������ ���õ� ��ɼ�����
	BetRuleToolTipInfo m_stBetRuleToolTip; // ���÷� �ȳ� ����
	
public:
	INT64 m_i64MinimumMoney; // ���� ���� �ݾ�
	INT64 m_i64SeedMoney;
	short m_sAutoBet;
	short m_sBetRuleKind;
	int m_nChipKind;
	bool m_bFriend;
	// ### ���� ��� ###
	// @ �Ƕ� : ��Ŀ(hoola::eFORM_KIND_JOKER), �Ϲ�(hoola::eFORM_KIND_NORMAL)
	int m_nGameMode;
	int m_nFormKind;
	// ### ���� Ÿ�� ###
	// @ �Ƕ� : ����on(hoola::eRULE_TYPE_STOP), ����off(hoola::eRULE_TYPE_WAR)
	int m_nGameType;
	int m_nMaxUserNum;
	bool m_bVIPEnterLimitYN;
	int m_nLimitRoomUser; // ������ �۾� : ������ �ɼ��� index����
	char cCardSetting; // ī����� (���۽� ī�� �� ��)=������ �߰�
	float fBetMoneyMulti; // ������ ��� (���ٸ� ���)
	CString m_strRoomTitle;
	CPoint m_ptMaxDescPos;
	bool m_bUnionMode;	// ���չ� ���
	bool m_bBetRule;	// ��ħ����
	
	// �ƽ���(����Ŀ��, �ƽ��Ӵ� ���� ������ �õ�Ӵ� ������ �߰��� �Ǿ�����)
	BOOL m_bIsMaxRoom;
	INT64 m_i64InMoney;
	BOOL m_bShowMaxRoomDesc; 
	BOOL m_bShowRdBlind;
	BOOL m_bShowRdUnion;

public:
	CCreateRoomDlg(IDX_GAME nGame, CWnd* pParent = NULL);
	virtual ~CCreateRoomDlg(){}
	
	enum { IDD = IDD_CREATEROOM_DIALOG };
	
	void SetWebSelectedRoomOption(ROOMOPTION* pRoomOption);	// ������ ���õǾ��� ��ɼ��� �����ؼ� ����� ���̾�α� �˾��� ���õǾ��� ��ɼ��� ����Ʈ�� �������ش�.
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	
protected:
	// ���� �Լ�
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	
	// ����� ���� �Լ�
	void InitCBLimitUser();
	// �������� �Ӵ� ���� �õ�Ӵ�/�ڵ� ������ �����Ѵ�.
	BOOL SetComboAutoBetSeed(INT64 llEnterLimitMoney, INT64 llDefaultSeedMoney = 0, short sDefaultAutobet = -1);
	
	BOOL IsCreateDiaRoom();	// �ƽ��游��Ⱑ���Ѱ�?
	BOOL IsCreateMaxRoom();	// �ƽ��游��Ⱑ���Ѱ�?
	BOOL IsCreateMaxGoldRoom();	// �ƽ��游��Ⱑ���Ѱ�?
	BOOL IsCreateUnionRoom();	//  ���չ游��Ⱑ�����Ѱ�?
	BOOL GetCheckRdGold(); // ���� ����� �����ΰ�
	void RefreshBetRuleToolTip(); // ���÷� �ȳ� ���� ����
	void SetGiveUpRoom(); // ��ǹ� ����
	bool IsGiveUpRoom(); // ��ǹ� ����
	void HideOptionBtns(); // ����ε�, ���չ� ������ư Hide
	void InitPosBtns();	// ��ư ����.
	void SetPosBtns();	// ��ư ��ġ ����.
	CString GetGameStr(int nGame); // ���� ���ϸ� ��������
	
	DECLARE_MESSAGE_MAP()
	
public:
	// �̺�Ʈ
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeMinimumMony();
	afx_msg void OnCbnSelchangeAutobetSeed();	
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnEnChangeEditMaxroom();
	afx_msg void OnEnChangeEditMrmoneyInput1();
	afx_msg void OnEnChangeEditMrmoneyInput2();
	
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	
public:
	UINT m_MaxRoomMoney1;
	UINT m_MaxRoomMoney2;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeComboLimitUser();
};