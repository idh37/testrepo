#pragma once

#include "..\\GraphComboBox.h"

// 게임 진행 관련
#define IDM_DIE				(WM_USER+6100)
#define IDM_CHECK			(WM_USER+6101)
#define IDM_CALL			(WM_USER+6102)
#define IDM_PING			(WM_USER+6103)
#define IDM_DADANG			(WM_USER+6104)
#define IDM_FULL			(WM_USER+6123)
#define IDM_HALF			(WM_USER+6105) // 이름변경(IDM_MAX -> IDM_HALF)
#define IDM_ALLIN			(WM_USER+6106)

#define IDM_TIPBTN			(WM_USER+6107) // tip
#define IDM_CAPTURE			(WM_USER+6108) // [화면캡쳐]
#define IDM_OPTION			(WM_USER+6109)
#define IDM_MINIMIZE		(WM_USER+6110)
#define IDM_CHANGEDISPLAY   (WM_USER+6111)
#define IDM_EXIT			(WM_USER+6112)
#define IDM_GRADE			(WM_USER+6113)
#define IDM_INVITE			(WM_USER+6114)	
// ### [ 관전기능 ] ###
#define IDM_OBSERVERVIEW	(WM_USER+6115)
#define IDM_PARTICIPATION 	(WM_USER+6116)
#define IDM_EMERGE 			(WM_USER+6117)

#define IDM_EXIT_BOTTOM		(WM_USER+6118)
#define IDM_POKER_SHOP		(WM_USER+6119)
#define IDM_BETBTNTYPE_COMBO	(WM_USER+6120)

// 규제안 경고 버튼.
#define IDM_LOSSMONEY_WARNING		(WM_USER+6121)
#define IDM_LOSSMONEY_WARNINGCLOSE	(WM_USER+6122)

//규제안 대기하기 버튼
#define IDM_WAIT_USER	(WM_USER+6124)

//규제안 게임시작 버튼
#define IDM_START_HL	(WM_USER+6125)
#define IDM_START_SD	(WM_USER+6126)
#define IDM_START_BD	(WM_USER+6127)
#define IDM_START_NP	(WM_USER+6128)
#define IDM_START_SP	(WM_USER+6129)

#define IDM_COMPLAIN	(WM_USER+6130)

#define IDM_GIVEUP		(WM_USER+6131) // 기권하기 버튼
#define IDM_CARD_OPEN	(WM_USER+6132) // 본인 카드 오픈 버튼
#define IDM_BDRULE_TIP	(WM_USER+6133) // 아침벳룰 툴팁

enum EGKEY_DEFINE
{
	KEY_CUT_1  = 0,
	KEY_CUT_2,
	KEY_CUT_3,
	KEY_CUT_4,

	KEY_BET_DIE,
	KEY_BET_CHK,
	KEY_BET_CAL,
	KEY_BET_PNG,
	KEY_BET_DAD,
	KEY_BET_HAF,

	KEY_HIDDEN_OPEN,
	KEY_HIDDEN_SLOW,

	KEY_CUT_CHANGE,
	KEY_CUT_PASS,

	// 버튼이 더 많다면 이후 추가
	KEY_CARD_SELECT_1,
	KEY_CARD_SELECT_2,
	KEY_CARD_SELECT_3,
	KEY_CARD_SELECT_4,

	// 하이로우 전용
	KEY_HILOWSEL_HIGH,	// 하이 선택
	KEY_HILOWSEL_LOW,	// 로우 선택
	KEY_HILOWSEL_SWING,	// 스윙 선택


	//-------------------------------
	KEY_DEF_ALL,
};


#define  MAX_GAME_KEY  5

UPG_GAMEBASE_EXPORT extern DWORD G_KEYDEF[ KEY_DEF_ALL ][ MAX_GAME_KEY ];

#define SET_VKEY( NOR, EXT)   ( NOR << 16 ) + ( EXT << 24 )
#define GET_VKEY_NOR( NOR )   ( ( NOR >> 16 ) & 0x000000FF )  
#define GET_VKEY_EXT( EXT )   ( ( EXT >> 24 ) & 0x00000001 )  
#define IS_VKEY( VKEY, NOR, EXT )  ( ( NOR == ( ( VKEY >> 16 )  & 0x000000FF ) ) && ( EXT == ( ( VKEY >> 24)  & 0x00000001) )  )

#define SET_VKEY_NOR( NOR )   SET_VKEY( NOR, 0 )
#define SET_VKEY_EXT( NOR )   SET_VKEY( NOR, 1 )

#define BTN_PARTICIPATION_INDEX			2
#define BTN_EMERGE_INDEX				2
#define BTN_PARTICIPATION_EXIT_INDEX	2

#define MAX_LIMIT_MONEY_INDEX			15

class CUIInfoData;
class CChattingWindowMan;


enum LIMIT_BET_TYPE
{
	LIMIT_BET_TYPE_NORMAL=0, // 예선
	LIMIT_BET_TYPE_FINAL, // 결승 (결승에서는 스몰베팅버튼대신 올인베팅버튼이 들어간다)
};

enum LIMIT_BET_STEP
{
	LIMIT_BET_STEP_1=0, // 기획서 베팅1	 베팅
	LIMIT_BET_STEP_2, // 기획서 베팅2	 베팅
	LIMIT_BET_STEP_3, // 기획서 히든 베팅

	MAX_LIMIT_BET_STEP
};


class UPG_GAMEBASE_EXPORT CGameViewButtonMan  
{
public:
	enum _NTMONEY
	{
		NT_DIE = 0,
		NT_PNG,
		NT_CHK,
		NT_CAL,
		NT_DAD,
		NT_HAF,
		NT_ALL
	};

	NMBASE::GRAPHICGDI::CPage *m_pPage;
	CGameViewBase *m_pParent;	
	CChattingWindowMan *m_pChattingWindowMan;

	NMBASE::GRAPHICGDI::xSprite	m_sprUITopBtn;
	NMBASE::GRAPHICGDI::xSprite	m_sprUIBottomBtn;
	NMBASE::GRAPHICGDI::xSprite	m_sprUICardOpenBtn;
	NMBASE::GRAPHICGDI::xSprite	m_sprOneShotChargeBtn;
	//NMBASE::GRAPHICGDI::xSprite m_sprSeatKeeperBtn; // SeatKeeper클래스로 이동	
	NMBASE::GRAPHICGDI::xSprite	m_sprBetBtn_N;
	NMBASE::GRAPHICGDI::xSprite	m_sprBetBtn_H;
	NMBASE::GRAPHICGDI::xSprite	m_sprBetBtn_P;

	// 콤보박스 버튼
	NMBASE::GRAPHICGDI::xSprite	m_sprBetBtnCombo;

	// 규제안 손실금액 게임내 리소스
	NMBASE::GRAPHICGDI::xSprite	m_sprLossMoney;	
	NMBASE::GRAPHICGDI::xSprite	m_sprLossMoneyBtn;
	NMBASE::GRAPHICGDI::xSprite	m_sprLossMoneyBtn2;

	// 규제안 : 대기하기 얼럿이미지(spr새로 추가)
	NMBASE::GRAPHICGDI::xSprite	m_sprWaitUserAlert;

	// 규제안 : 게임하기 게임내 버튼
	NMBASE::GRAPHICGDI::xSprite	m_sprGamestartBack;

	NMBASE::GRAPHICGDI::xSprite	m_sprBtn_gamestart_HL;
	NMBASE::GRAPHICGDI::xSprite	m_sprBtn_gamestart_SD;
	NMBASE::GRAPHICGDI::xSprite	m_sprBtn_gamestart_BD;
	NMBASE::GRAPHICGDI::xSprite	m_sprBtn_gamestart_NP;
	NMBASE::GRAPHICGDI::xSprite	m_sprBtn_gamestart_SP;

	NMBASE::GRAPHICGDI::xSprite	m_sprShowFullExplain;
	NMBASE::GRAPHICGDI::xSprite	m_sprComplain;

	// 규제안 : 게임 시작 버튼
	NMBASE::SKINGDI::CGraphButton m_btnGameStart_HL;
	NMBASE::SKINGDI::CGraphButton m_btnGameStart_SD;
	NMBASE::SKINGDI::CGraphButton m_btnGameStart_BD;
	NMBASE::SKINGDI::CGraphButton m_btnGameStart_NP;
	NMBASE::SKINGDI::CGraphButton m_btnGameStart_SP;

	
	// 그래픽 버튼 클래스
	NMBASE::SKINGDI::CGraphButton m_btnExit;
	NMBASE::SKINGDI::CGraphButton m_btnGotoOtherRoom; //m_btnInvite; //규제안 작업 :게임방에서 다른방 바로가기
	NMBASE::SKINGDI::CGraphButton m_btnChangeDisplay;
	NMBASE::SKINGDI::CGraphButton m_btnOption;
	NMBASE::SKINGDI::CGraphButton m_btnMinimize;
	NMBASE::SKINGDI::CGraphButton m_btnGameItem;
	NMBASE::SKINGDI::CGraphButton m_btnPokerShop;
	
	// 규제안 손실경고 버튼
	NMBASE::SKINGDI::CGraphButton m_btnMoneyWarning;
	NMBASE::SKINGDI::CGraphButton m_btnMoneyWarningClose;

	// 규제안 대기 버튼
	NMBASE::SKINGDI::CGraphButton m_btnWaitUser;
	
	// 규제안 : 대기 안내 이미지
	NMBASE::SKINGDI::CGraphButton m_btnWaitUserAlert;

	NMBASE::SKINGDI::CGraphButton m_btnTip;

	NMBASE::SKINGDI::CGraphButton m_btnCardOpen; // 본인 카드 오픈 버튼
	NMBASE::SKINGDI::CGraphButton m_btnExitBottom; // 나가기 버튼

	// 게임진행 버튼 클래스
	NMBASE::SKINGDI::CGraphButtonEx	m_btnDie;
	NMBASE::SKINGDI::CGraphButtonEx	m_btnCheck;
	NMBASE::SKINGDI::CGraphButtonEx	m_btnCall;
	NMBASE::SKINGDI::CGraphButtonEx	m_btnPing;
	NMBASE::SKINGDI::CGraphButtonEx	m_btnDadang;
	NMBASE::SKINGDI::CGraphButtonEx	m_btnFull;
	NMBASE::SKINGDI::CGraphButtonEx	m_btnHalf;	

	NMBASE::SKINGDI::CGraphButton m_btnCapture; // [화면캡쳐]
	// ### [ 관전기능 ] ###
	NMBASE::SKINGDI::CGraphButton m_btnObserverView; // 관전현황 버튼
	NMBASE::SKINGDI::CGraphButton m_btnSInOut; // 관전 버튼
	NMBASE::SKINGDI::CGraphButton m_btnGameStart; // 2004.06.29 시작버튼 
	
	NMBASE::SKINGDI::CGraphButton m_btnGiveUp; // 기권하기 버튼
	
	NMBASE::SKINGDI::CGraphButton m_btnAutoBetClose;
	NMBASE::SKINGDI::CGraphButton m_btnUseGrade;
	//NMBASE::SKINGDI::CGraphButton	m_btnVip;
	NMBASE::SKINGDI::CGraphButton m_btnInGameBadUserComplain; // 신고

	NMBASE::SKINGDI::CGraphButtonListEx	m_ctrlBetBtnList;

	NMBASE::SKINGDI::CGraphButtonList m_ctrlBtnList;

	NMBASE::SKINGDI::CGraphButton m_btnBDRuleToolTip;

	CGraphComboBox m_cmbBetBtn; //버튼타입 고르는 콤보박스
	
	CUIInfoData *m_pUIInfoData;
	
	BOOL m_bMouseOver;
	int m_nMousOverIndex;	
	BOOL m_bBPing,m_bBCheck,m_bBCall,m_bBMax,m_bBDDa,m_bFold;
	
	bool m_bBtnClick;
	CONFIG_BETBTN_TYPE m_eBtnDirectionType;	// 0:N 1:H 2:P 
	
	bool m_bMaxState; //규제안 : 맥스버튼 활성화 여부
	
	BOOL m_bAllin; //규제안 : 올인버튼 활성화 여부
	
	//규제안 : 대기하기 값
	bool m_bIsWaitUser;	
	int m_nExitSubScript;
	
protected:
	NMBASE::SKINGDI::CNText* m_pNTMoney[ NT_ALL ];
	bool m_bShowNTM[ NT_ALL ]; // 그리고 안그리고
	
	int m_nLastCmdBackup; //리미트배팅추가
	bool m_bInculdeBetttingBtn; //베팅버튼을 포함하는지여부(훌라에서는 베팅버튼을 사용하지않는다)
	
	//리미트 베팅관련
	LIMIT_BET_TYPE m_eLimitBetType;
	LIMIT_BET_STEP m_eSmallBetStep;
	LIMIT_BET_STEP m_eBigBetStep;
	int m_nLastRound; //베팅버튼이 세팅(SetBettinBtnDiable호출될때) 마지막 라운드를 기억 (라운드가 바뀔때마다 m_eSmallBetStep, m_eBigBetStep을 STEP_0 으로 리셋시킴)
	
	void OnChangeRound(); //라운드가 바뀌는지 검사한다.
	
	bool m_bIsShowLossMoneyText; //베팅버튼을 포함하는지여부(훌라에서는 베팅버튼을 사용하지않는다)
	bool m_bIsShowOnceLossMoneyText; //처음 규제안 상태가 변경되면 호출하지만, 다음 부터는 경고 버튼을 눌러야 보인다.
	
	DAILYLOSS_STATE	m_PreState;

public:
	CGameViewButtonMan();
	virtual ~CGameViewButtonMan();

protected:
	virtual void ReSetPostionNTMoney();
	virtual void SetKeyDirect( CONFIG_BETBTN_TYPE eBtnDirectionType);
	virtual void InitializeKeyValue( CONFIG_BETBTN_TYPE eBtnDirectionType );
	virtual CString GetMoneyString( INT64 nMoney );
	
	//규제안 관련하여 맥스 제한 금액 체크를 위한 함수
	INT64 GetMaxMoney( INT64 nMoney , int spnum);
	
public:

	virtual void Reset(void); //올인버튼을 하프버튼으로만들어준다. 나중에 ResetGame 함수와 헷갈리므로 ResetBtn()으로 바꾼다.
	virtual void ResetGame(); //ResetGame 될때 호출

	
	///바둑이 토너먼트 리미트 베팅////////////////////////////////////////
	//int GetBetMoneyIndex( INT64 betmoney );	
	LIMIT_BET_STEP GetLimitBetBtnStep( int nRound , eLIMIT_X eLimitMultiflier );
	eLIMIT_X GetLimitMultiflierWithBetStep(int nRound, LIMIT_BET_STEP eLimitBetStep);

	//(RAISE_LAST_CMD ↔ BET_CMD ↔ eLIMIT_X 변환 함수들)
	BET_CMD GetBetCmdWithLimitMultiflier(eLIMIT_X eLimitMultiflier);
	eLIMIT_X GetLimitMultiflierWithLastCmd(RAISE_LAST_CMD eLastCmd);
	eLIMIT_X GetLimitMultiflierWithBetCmd(BET_CMD eBetCmd);

	void SetSmallBetBtnStep(LIMIT_BET_STEP eLimitBetStep);
	void SetBigBetBtnStep(LIMIT_BET_STEP eLimitBetStep);
	void IncLimitBetBtnStep(); //스몰버튼, 빅버튼의 단계를 한단계씩 올린다. (베팅1->베팅2, 베팅2->히든)
	
	BET_CMD GetSmallBetBtnCmd(INT64 llMyMoney);
	BET_CMD GetBigBetBtnCmd(INT64 llMyMoney);
	LIMIT_BET_STEP GetSmallBetBtnStep();
	LIMIT_BET_STEP GetBigBetBtnStep();
	eLIMIT_X GetSmallBtnLimitMultiplier();
	eLIMIT_X GetBigBtnLimitMultiplier();
	void SetLimitBetType(LIMIT_BET_TYPE eLimitBetType);
	/////////////////////////////////////////////////////////////////////

	//규제안 : 손실금액 경고
	void SetLossMoneyWarning(bool enterroom = false) ;

	//규제안 : 대기기능 추가(다이아몬드 회원)
	void SetWaitFlag(bool isWait);

	//규제안 : 자기 턴이면 예약실행
	void RunReservedBet();
	void ShowReserveBtn(bool bShow = true);
	bool CheckMaxFlag();
	bool CheckReservedBetting(INT64 bettingMoney);

	INT64 nBetPingMoney;
	INT64 nBetDDaMoney;
	INT64 nBetChkMoney;
	INT64 nBetHalfMoney;
	INT64 nBetCallMoney;

	bool m_bChangePassState;

	/*===========================(공통)=============================*/
	virtual void LoadImage( LPCTSTR strBetBtn_N, LPCTSTR strBetBtn_H, LPCTSTR strBetBtn_P );
	virtual void Init(CGameViewBase *pView, CUIInfoData *pUIInfoData, CChattingWindowMan *pChattingWindowMan);	
	virtual void DrawBtn(CDC *pDC);
	virtual BOOL OnLButtonDown(int x , int y);
	virtual BOOL OnLButtonUp(int x , int y);
	virtual BOOL OnMouseMove(int x , int y);
	//베팅 버튼 관련
	virtual void ClearFlag();
	virtual void SetBettinBtnDiable(bool bDisable = true);
	virtual void FirstBetCheck(); //2017.8 경기장개편
	virtual BOOL IsFirstBet();	//첫베팅 금액 부족시 버그 수정. 2018.05
	virtual void SetChangeDisplayButton(bool bFull);
	virtual void SetObserverBtn(int nNum);
	virtual void InitObserverBtn(void);
	virtual	void SetLastCmdBackup( RAISE_LAST_CMD eLastCmd, bool bReset = false );
//	virtual void SetGameRoomInfo( int bBetRuleKind );
	virtual	int  GetLastCmdBackup();
	virtual void SetOverImageIndex( int Cmd, int nButtonNumber );

	/*===========================(하이로우)=============================*/		
	virtual void EnterRoom_Button_Init( int bBetRuleKind );

	virtual void SetBetBtnType(CONFIG_BETBTN_TYPE eBtnDirectionType, bool bForceSet = false);
	virtual void SetButtonClick(bool bClick) { m_bBtnClick = bClick; }
	virtual bool GetButtonClick() { return m_bBtnClick;}

	virtual BOOL OnCommand( WPARAM wParam, LPARAM lParam );
	virtual void SetShowBetMony();
	
	virtual void MakeHalfBtnAllinBtn(BOOL bAllin); //하프버튼을 올인버튼으로 만든다. (
	virtual void MakeDadangBtnAllinBtn(BOOL bAllin);
	virtual void MakeFullBtnAllinBtn(BOOL bAllin);
	virtual void MakeMaxBtn(int BUTTON_IDM , BOOL isMax, INT64 maxMoney);
	virtual void ResetMaxBtn(); //맥스버튼 초기화

	BOOL GetShowBDTooltip();	
};

#define MakeBigBtnAllinBtn MakeHalfBtnAllinBtn			//리미트베팅 빅버튼을 올인버튼으로 만든다. 
#define MakeSmallBtnAllinBtn MakeDadangBtnAllinBtn		//리미트베팅 스몰버튼을 올인버튼으로 만든다. 