#pragma once

#include "../resource.h"



class UPG_GAMEBASE_EXPORT CCreateRoomDlg : public CUPGDlgBase
{
protected:
	IDX_GAME m_nGame;
	struct RULE
	{
		INT64 i64Seed;
		INT64 llEnterLimitMoney; // 입장제한 머니
		short sAutoBet;
		short sBetRuleKind; // 리미트배팅룰
		char cCardSetting; // 카드셋팅 (시작시 카드 장 수)=규제안 추가
		float fBetMoneyMulti; // 규제안 : 참가비 추가
	};
	
	struct BetRule
	{
		short sAutoBet;
	};
	
	struct BetRuleToolTipInfo
	{
		bool bBetRuleToolTip; // 베팅룰 안내 툴팁이 보여져야하는가?
		CString str1stLine; // 베팅룰 안내 툴팁 텍스트 (1줄)
		CString str2ndLine; // 베팅룰 안내 툴팁 텍스트 (2줄)			
		CRect rtToolTIpRect; // 베팅룰 안내 툴팁 박스 (화면갱신및 텍스트 위치)
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
	NMBASE::SKINGDI::CBmpRadioBtn m_rdFriend; // 친구 모드
	NMBASE::SKINGDI::CMyBitmap m_imgRdTypeBG;	// 일반, 골드, 친구 모드 선택 BG
	NMBASE::SKINGDI::CBmpRadioBtn m_rdMode1; // 게임 모드 - [훌라 : 조커, 일반]
	NMBASE::SKINGDI::CBmpRadioBtn m_rdMode2;
	// 로우바둑이 블라인드 베팅
	NMBASE::SKINGDI::CBmpRadioBtn m_rdUnion1;
	NMBASE::SKINGDI::CBmpRadioBtn m_rdUnion2;
	NMBASE::SKINGDI::CMyBitmap m_imgRdUnion;
	int	m_nBottomImgRdUnion;
	// 로우바둑이 블라인드 베팅
	NMBASE::SKINGDI::CBmpRadioBtn m_rdBlind1;
	NMBASE::SKINGDI::CBmpRadioBtn m_rdBlind2;
	NMBASE::SKINGDI::CMyBitmap m_imgRdBlind;
	// 로우바둑이 아침벳룰
	NMBASE::SKINGDI::CBmpRadioBtn m_rdBetRule1;
	NMBASE::SKINGDI::CBmpRadioBtn m_rdBetRule2;
	NMBASE::SKINGDI::CMyBitmap m_imgRdBetRule;
	// 훌라용 라디오 버튼
	NMBASE::SKINGDI::CBmpRadioBtn m_rdStop1; // 스톱ON
	NMBASE::SKINGDI::CBmpRadioBtn m_rdStop2; // 스톱OFF
	// 모든 게임 다이아몬드 멤버 맥스방 선택 라디오 버튼
	NMBASE::SKINGDI::CBmpRadioBtn m_rdMaxRoom3;	// 맥스방
	NMBASE::SKINGDI::CBmpRadioBtn m_rdMaxRoom4;
	NMBASE::SKINGDI::CMyButton m_btnOpenMaxDesc;
	NMBASE::SKINGDI::CMyButton m_btnCloseMaxDesc;
	
	CEdit m_edMRMoney_Input1;
	CEdit m_edMRMoney_Input2;
	
	CComboBox m_cbLimitUserNum;	// 방입장 정원 콤보박스
	CComboBox m_cbAutoBetSeed; // 시드/베팅룰 콤보박스
	CComboBox m_cbMinimumMoney;	// 입장제한 콤보박스
	
	NMBASE::SKINGDI::CMyButton m_btnCancel;
	NMBASE::SKINGDI::CMyButton m_btnOK;
	
	CBrush m_cHollowBrush;
	bool m_bUseWebSelectedRoom; // 웹에서 선택된 방옵션을 사용할것인가?
	ROOMOPTION m_WebSelectedRoomOption; // 웹에서 선택된 방옵션정보
	BetRuleToolTipInfo m_stBetRuleToolTip; // 베팅룰 안내 툴팁
	
public:
	INT64 m_i64MinimumMoney; // 입장 제한 금액
	INT64 m_i64SeedMoney;
	short m_sAutoBet;
	short m_sBetRuleKind;
	int m_nChipKind;
	bool m_bFriend;
	// ### 게임 모드 ###
	// @ 훌라 : 조커(hoola::eFORM_KIND_JOKER), 일반(hoola::eFORM_KIND_NORMAL)
	int m_nGameMode;
	int m_nFormKind;
	// ### 게임 타입 ###
	// @ 훌라 : 스톱on(hoola::eRULE_TYPE_STOP), 스톱off(hoola::eRULE_TYPE_WAR)
	int m_nGameType;
	int m_nMaxUserNum;
	bool m_bVIPEnterLimitYN;
	int m_nLimitRoomUser; // 규제안 작업 : 방정원 옵션의 index저장
	char cCardSetting; // 카드셋팅 (시작시 카드 장 수)=규제안 추가
	float fBetMoneyMulti; // 참가비 배수 (섯다만 사용)
	CString m_strRoomTitle;
	CPoint m_ptMaxDescPos;
	bool m_bUnionMode;	// 연합방 모드
	bool m_bBetRule;	// 아침벳룰
	
	// 맥스방(맞포커용, 맥스머니 구간 설정과 시드머니 설정이 추가로 되어있음)
	BOOL m_bIsMaxRoom;
	INT64 m_i64InMoney;
	BOOL m_bShowMaxRoomDesc; 
	BOOL m_bShowRdBlind;
	BOOL m_bShowRdUnion;

public:
	CCreateRoomDlg(IDX_GAME nGame, CWnd* pParent = NULL);
	virtual ~CCreateRoomDlg(){}
	
	enum { IDD = IDD_CREATEROOM_DIALOG };
	
	void SetWebSelectedRoomOption(ROOMOPTION* pRoomOption);	// 웹에서 선택되어진 방옵션을 세팅해서 방생성 다이얼로그 팝업시 선택되어진 방옵션을 디폴트로 선택해준다.
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	
protected:
	// 가상 함수
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	
	// 사용자 정의 함수
	void InitCBLimitUser();
	// 입장제한 머니 따라서 시드머니/자동 베팅을 셋팅한다.
	BOOL SetComboAutoBetSeed(INT64 llEnterLimitMoney, INT64 llDefaultSeedMoney = 0, short sDefaultAutobet = -1);
	
	BOOL IsCreateDiaRoom();	// 맥스방만들기가능한가?
	BOOL IsCreateMaxRoom();	// 맥스방만들기가능한가?
	BOOL IsCreateMaxGoldRoom();	// 맥스방만들기가능한가?
	BOOL IsCreateUnionRoom();	//  연합방만들기가가능한가?
	BOOL GetCheckRdGold(); // 골드방 만들기 상태인가
	void RefreshBetRuleToolTip(); // 베팅룰 안내 툴팁 갱신
	void SetGiveUpRoom(); // 기권방 세팅
	bool IsGiveUpRoom(); // 기권방 여부
	void HideOptionBtns(); // 블라인드, 연합방 라디오버튼 Hide
	void InitPosBtns();	// 버튼 생성.
	void SetPosBtns();	// 버튼 위치 조정.
	CString GetGameStr(int nGame); // 게임 파일명 가져오기
	
	DECLARE_MESSAGE_MAP()
	
public:
	// 이벤트
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