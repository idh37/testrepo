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
	bool IsGiveUpRoom(); // 기권방 여부
	void SetComboData() ;
	void SaveOption() ;

	virtual INT_PTR DoModal(CWnd *pParent);
protected:
	DECLARE_MESSAGE_MAP()

	struct RULE
	{
		INT64 i64Seed;
		INT64	llEnterLimitMoney;	// 입장제한 머니
		short sAutoBet;
		short sBetRuleKind;		//리미트배팅룰
		char	cCardSetting;		// 카드셋팅 (시작시 카드 장 수)=규제안 추가
		float fBetMoneyMulti;	//규제안 : 참가비 추가
	};

	struct BetRule
	{
		short sAutoBet;
	};

	struct BetRuleToolTipInfo
	{
		bool bBetRuleToolTip;					//베팅룰 안내 툴팁이 보여져야하는가?
		CString str1stLine;				//베팅룰 안내 툴팁 텍스트 (1줄)
		CString str2ndLine;				//베팅룰 안내 툴팁 텍스트 (2줄)			
		CRect rtToolTIpRect;						//베팅룰 안내 툴팁 박스 (화면갱신및 텍스트 위치)
	};

	set<INT64>							m_listMinimumMoney;

	vector<INT64>						m_vecMinimumMoney;
	vector<INT64>						m_vecSeedMoney;

	typedef map<INT64, map< BetRule*, RULE > >		   MAP_LIST_RULE;
	typedef map<INT64, map< BetRule*, RULE > >::iterator MAP_LIST_RULE_ITER;
	MAP_LIST_RULE	m_listRule;

	INT64								m_i64DefaultSeed;
};