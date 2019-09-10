#pragma once
#include "afxwin.h"

class UPG_COMMON_EXPORT CSelectQuickstarterDlg : public LSY::CLSYLibDialogBase
{

public:
	CSelectQuickstarterDlg(CWnd* pParent = NULL);
	virtual ~CSelectQuickstarterDlg(){}

public:
	LSY::CWindows *m_pWndBack ;
	LSY::CCombo				*m_pComboLimit;
	LSY::CCombo				*m_pComboMode;
	LSY::CCombo				*m_pComboRule;
	LSY::CCombo				*m_pComboSeedMoney;
	LSY::CCombo				*m_pComboBlind;

	LSY::CImage*	m_pImgLimitBack ;
	LSY::CImage*	m_pImgModeBack ;
	LSY::CImage*	m_pImgRuleBack ;
	LSY::CImage*	m_pImgSeedMoneyBack ;
	LSY::CImage*	m_pImgBlindBack ;

	LSY::CCheckBox* m_pRandomCheckbox ;

	LSY::CCheckBox* m_pDiaCheckbox ;

	LSY::CText*	m_pTextRule ;

public:
	virtual BOOL OnInitDialog();
	virtual bool OnCreatedProject(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnSelChangeLimit(LSY::CMessage *msg);
	LRESULT OnSelChangeMode(LSY::CMessage *msg);
	LRESULT OnSelChangeRule(LSY::CMessage *msg);	

	void SetComboSeedMoney();
	bool IsGiveUpRoom(); // ��ǹ� ����
	void SetComboData() ;
	void SaveOption() ;

	virtual INT_PTR DoModal(CWnd *pParent);
protected:
	DECLARE_MESSAGE_MAP()

	struct RULE
	{
		INT64 i64Seed;
		INT64	llEnterLimitMoney;	// �������� �Ӵ�
		short sAutoBet;
		short sBetRuleKind;		//����Ʈ���÷�
		char	cCardSetting;		// ī����� (���۽� ī�� �� ��)=������ �߰�
		float fBetMoneyMulti;	//������ : ������ �߰�
	};

	struct BetRule
	{
		short sAutoBet;
	};

	struct BetRuleToolTipInfo
	{
		bool bBetRuleToolTip;					//���÷� �ȳ� ������ ���������ϴ°�?
		CString str1stLine;				//���÷� �ȳ� ���� �ؽ�Ʈ (1��)
		CString str2ndLine;				//���÷� �ȳ� ���� �ؽ�Ʈ (2��)			
		CRect rtToolTIpRect;						//���÷� �ȳ� ���� �ڽ� (ȭ�鰻�Ź� �ؽ�Ʈ ��ġ)
	};

	set<INT64>							m_listMinimumMoney;

	vector<INT64>						m_vecMinimumMoney;
	vector<INT64>						m_vecSeedMoney;

	typedef map<INT64, map< BetRule*, RULE > >		   MAP_LIST_RULE;
	typedef map<INT64, map< BetRule*, RULE > >::iterator MAP_LIST_RULE_ITER;
	MAP_LIST_RULE	m_listRule;

	INT64								m_i64DefaultSeed;
};