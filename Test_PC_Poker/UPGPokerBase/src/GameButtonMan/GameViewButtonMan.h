#pragma once

#include "..\\GraphComboBox.h"

// ���� ���� ����
#define IDM_DIE				(WM_USER+6100)
#define IDM_CHECK			(WM_USER+6101)
#define IDM_CALL			(WM_USER+6102)
#define IDM_PING			(WM_USER+6103)
#define IDM_DADANG			(WM_USER+6104)
#define IDM_FULL			(WM_USER+6123)
#define IDM_HALF			(WM_USER+6105) // �̸�����(IDM_MAX -> IDM_HALF)
#define IDM_ALLIN			(WM_USER+6106)

#define IDM_TIPBTN			(WM_USER+6107) // tip
#define IDM_CAPTURE			(WM_USER+6108) // [ȭ��ĸ��]
#define IDM_OPTION			(WM_USER+6109)
#define IDM_MINIMIZE		(WM_USER+6110)
#define IDM_CHANGEDISPLAY   (WM_USER+6111)
#define IDM_EXIT			(WM_USER+6112)
#define IDM_GRADE			(WM_USER+6113)
#define IDM_INVITE			(WM_USER+6114)	
// ### [ ������� ] ###
#define IDM_OBSERVERVIEW	(WM_USER+6115)
#define IDM_PARTICIPATION 	(WM_USER+6116)
#define IDM_EMERGE 			(WM_USER+6117)

#define IDM_EXIT_BOTTOM		(WM_USER+6118)
#define IDM_POKER_SHOP		(WM_USER+6119)
#define IDM_BETBTNTYPE_COMBO	(WM_USER+6120)

// ������ ��� ��ư.
#define IDM_LOSSMONEY_WARNING		(WM_USER+6121)
#define IDM_LOSSMONEY_WARNINGCLOSE	(WM_USER+6122)

//������ ����ϱ� ��ư
#define IDM_WAIT_USER	(WM_USER+6124)

//������ ���ӽ��� ��ư
#define IDM_START_HL	(WM_USER+6125)
#define IDM_START_SD	(WM_USER+6126)
#define IDM_START_BD	(WM_USER+6127)
#define IDM_START_NP	(WM_USER+6128)
#define IDM_START_SP	(WM_USER+6129)

#define IDM_COMPLAIN	(WM_USER+6130)

#define IDM_GIVEUP		(WM_USER+6131) // ����ϱ� ��ư
#define IDM_CARD_OPEN	(WM_USER+6132) // ���� ī�� ���� ��ư
#define IDM_BDRULE_TIP	(WM_USER+6133) // ��ħ���� ����

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

	// ��ư�� �� ���ٸ� ���� �߰�
	KEY_CARD_SELECT_1,
	KEY_CARD_SELECT_2,
	KEY_CARD_SELECT_3,
	KEY_CARD_SELECT_4,

	// ���̷ο� ����
	KEY_HILOWSEL_HIGH,	// ���� ����
	KEY_HILOWSEL_LOW,	// �ο� ����
	KEY_HILOWSEL_SWING,	// ���� ����


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
	LIMIT_BET_TYPE_NORMAL=0, // ����
	LIMIT_BET_TYPE_FINAL, // ��� (��¿����� �������ù�ư��� ���κ��ù�ư�� ����)
};

enum LIMIT_BET_STEP
{
	LIMIT_BET_STEP_1=0, // ��ȹ�� ����1	 ����
	LIMIT_BET_STEP_2, // ��ȹ�� ����2	 ����
	LIMIT_BET_STEP_3, // ��ȹ�� ���� ����

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
	//NMBASE::GRAPHICGDI::xSprite m_sprSeatKeeperBtn; // SeatKeeperŬ������ �̵�	
	NMBASE::GRAPHICGDI::xSprite	m_sprBetBtn_N;
	NMBASE::GRAPHICGDI::xSprite	m_sprBetBtn_H;
	NMBASE::GRAPHICGDI::xSprite	m_sprBetBtn_P;

	// �޺��ڽ� ��ư
	NMBASE::GRAPHICGDI::xSprite	m_sprBetBtnCombo;

	// ������ �սǱݾ� ���ӳ� ���ҽ�
	NMBASE::GRAPHICGDI::xSprite	m_sprLossMoney;	
	NMBASE::GRAPHICGDI::xSprite	m_sprLossMoneyBtn;
	NMBASE::GRAPHICGDI::xSprite	m_sprLossMoneyBtn2;

	// ������ : ����ϱ� ���̹���(spr���� �߰�)
	NMBASE::GRAPHICGDI::xSprite	m_sprWaitUserAlert;

	// ������ : �����ϱ� ���ӳ� ��ư
	NMBASE::GRAPHICGDI::xSprite	m_sprGamestartBack;

	NMBASE::GRAPHICGDI::xSprite	m_sprBtn_gamestart_HL;
	NMBASE::GRAPHICGDI::xSprite	m_sprBtn_gamestart_SD;
	NMBASE::GRAPHICGDI::xSprite	m_sprBtn_gamestart_BD;
	NMBASE::GRAPHICGDI::xSprite	m_sprBtn_gamestart_NP;
	NMBASE::GRAPHICGDI::xSprite	m_sprBtn_gamestart_SP;

	NMBASE::GRAPHICGDI::xSprite	m_sprShowFullExplain;
	NMBASE::GRAPHICGDI::xSprite	m_sprComplain;

	// ������ : ���� ���� ��ư
	NMBASE::SKINGDI::CGraphButton m_btnGameStart_HL;
	NMBASE::SKINGDI::CGraphButton m_btnGameStart_SD;
	NMBASE::SKINGDI::CGraphButton m_btnGameStart_BD;
	NMBASE::SKINGDI::CGraphButton m_btnGameStart_NP;
	NMBASE::SKINGDI::CGraphButton m_btnGameStart_SP;

	
	// �׷��� ��ư Ŭ����
	NMBASE::SKINGDI::CGraphButton m_btnExit;
	NMBASE::SKINGDI::CGraphButton m_btnGotoOtherRoom; //m_btnInvite; //������ �۾� :���ӹ濡�� �ٸ��� �ٷΰ���
	NMBASE::SKINGDI::CGraphButton m_btnChangeDisplay;
	NMBASE::SKINGDI::CGraphButton m_btnOption;
	NMBASE::SKINGDI::CGraphButton m_btnMinimize;
	NMBASE::SKINGDI::CGraphButton m_btnGameItem;
	NMBASE::SKINGDI::CGraphButton m_btnPokerShop;
	
	// ������ �սǰ�� ��ư
	NMBASE::SKINGDI::CGraphButton m_btnMoneyWarning;
	NMBASE::SKINGDI::CGraphButton m_btnMoneyWarningClose;

	// ������ ��� ��ư
	NMBASE::SKINGDI::CGraphButton m_btnWaitUser;
	
	// ������ : ��� �ȳ� �̹���
	NMBASE::SKINGDI::CGraphButton m_btnWaitUserAlert;

	NMBASE::SKINGDI::CGraphButton m_btnTip;

	NMBASE::SKINGDI::CGraphButton m_btnCardOpen; // ���� ī�� ���� ��ư
	NMBASE::SKINGDI::CGraphButton m_btnExitBottom; // ������ ��ư

	// �������� ��ư Ŭ����
	NMBASE::SKINGDI::CGraphButtonEx	m_btnDie;
	NMBASE::SKINGDI::CGraphButtonEx	m_btnCheck;
	NMBASE::SKINGDI::CGraphButtonEx	m_btnCall;
	NMBASE::SKINGDI::CGraphButtonEx	m_btnPing;
	NMBASE::SKINGDI::CGraphButtonEx	m_btnDadang;
	NMBASE::SKINGDI::CGraphButtonEx	m_btnFull;
	NMBASE::SKINGDI::CGraphButtonEx	m_btnHalf;	

	NMBASE::SKINGDI::CGraphButton m_btnCapture; // [ȭ��ĸ��]
	// ### [ ������� ] ###
	NMBASE::SKINGDI::CGraphButton m_btnObserverView; // ������Ȳ ��ư
	NMBASE::SKINGDI::CGraphButton m_btnSInOut; // ���� ��ư
	NMBASE::SKINGDI::CGraphButton m_btnGameStart; // 2004.06.29 ���۹�ư 
	
	NMBASE::SKINGDI::CGraphButton m_btnGiveUp; // ����ϱ� ��ư
	
	NMBASE::SKINGDI::CGraphButton m_btnAutoBetClose;
	NMBASE::SKINGDI::CGraphButton m_btnUseGrade;
	//NMBASE::SKINGDI::CGraphButton	m_btnVip;
	NMBASE::SKINGDI::CGraphButton m_btnInGameBadUserComplain; // �Ű�

	NMBASE::SKINGDI::CGraphButtonListEx	m_ctrlBetBtnList;

	NMBASE::SKINGDI::CGraphButtonList m_ctrlBtnList;

	NMBASE::SKINGDI::CGraphButton m_btnBDRuleToolTip;

	CGraphComboBox m_cmbBetBtn; //��ưŸ�� ���� �޺��ڽ�
	
	CUIInfoData *m_pUIInfoData;
	
	BOOL m_bMouseOver;
	int m_nMousOverIndex;	
	BOOL m_bBPing,m_bBCheck,m_bBCall,m_bBMax,m_bBDDa,m_bFold;
	
	bool m_bBtnClick;
	CONFIG_BETBTN_TYPE m_eBtnDirectionType;	// 0:N 1:H 2:P 
	
	bool m_bMaxState; //������ : �ƽ���ư Ȱ��ȭ ����
	
	BOOL m_bAllin; //������ : ���ι�ư Ȱ��ȭ ����
	
	//������ : ����ϱ� ��
	bool m_bIsWaitUser;	
	int m_nExitSubScript;
	
protected:
	NMBASE::SKINGDI::CNText* m_pNTMoney[ NT_ALL ];
	bool m_bShowNTM[ NT_ALL ]; // �׸��� �ȱ׸���
	
	int m_nLastCmdBackup; //����Ʈ�����߰�
	bool m_bInculdeBetttingBtn; //���ù�ư�� �����ϴ�������(�Ƕ󿡼��� ���ù�ư�� ��������ʴ´�)
	
	//����Ʈ ���ð���
	LIMIT_BET_TYPE m_eLimitBetType;
	LIMIT_BET_STEP m_eSmallBetStep;
	LIMIT_BET_STEP m_eBigBetStep;
	int m_nLastRound; //���ù�ư�� ����(SetBettinBtnDiableȣ��ɶ�) ������ ���带 ��� (���尡 �ٲ𶧸��� m_eSmallBetStep, m_eBigBetStep�� STEP_0 ���� ���½�Ŵ)
	
	void OnChangeRound(); //���尡 �ٲ���� �˻��Ѵ�.
	
	bool m_bIsShowLossMoneyText; //���ù�ư�� �����ϴ�������(�Ƕ󿡼��� ���ù�ư�� ��������ʴ´�)
	bool m_bIsShowOnceLossMoneyText; //ó�� ������ ���°� ����Ǹ� ȣ��������, ���� ���ʹ� ��� ��ư�� ������ ���δ�.
	
	DAILYLOSS_STATE	m_PreState;

public:
	CGameViewButtonMan();
	virtual ~CGameViewButtonMan();

protected:
	virtual void ReSetPostionNTMoney();
	virtual void SetKeyDirect( CONFIG_BETBTN_TYPE eBtnDirectionType);
	virtual void InitializeKeyValue( CONFIG_BETBTN_TYPE eBtnDirectionType );
	virtual CString GetMoneyString( INT64 nMoney );
	
	//������ �����Ͽ� �ƽ� ���� �ݾ� üũ�� ���� �Լ�
	INT64 GetMaxMoney( INT64 nMoney , int spnum);
	
public:

	virtual void Reset(void); //���ι�ư�� ������ư���θ�����ش�. ���߿� ResetGame �Լ��� �򰥸��Ƿ� ResetBtn()���� �ٲ۴�.
	virtual void ResetGame(); //ResetGame �ɶ� ȣ��

	
	///�ٵ��� ��ʸ�Ʈ ����Ʈ ����////////////////////////////////////////
	//int GetBetMoneyIndex( INT64 betmoney );	
	LIMIT_BET_STEP GetLimitBetBtnStep( int nRound , eLIMIT_X eLimitMultiflier );
	eLIMIT_X GetLimitMultiflierWithBetStep(int nRound, LIMIT_BET_STEP eLimitBetStep);

	//(RAISE_LAST_CMD �� BET_CMD �� eLIMIT_X ��ȯ �Լ���)
	BET_CMD GetBetCmdWithLimitMultiflier(eLIMIT_X eLimitMultiflier);
	eLIMIT_X GetLimitMultiflierWithLastCmd(RAISE_LAST_CMD eLastCmd);
	eLIMIT_X GetLimitMultiflierWithBetCmd(BET_CMD eBetCmd);

	void SetSmallBetBtnStep(LIMIT_BET_STEP eLimitBetStep);
	void SetBigBetBtnStep(LIMIT_BET_STEP eLimitBetStep);
	void IncLimitBetBtnStep(); //������ư, ���ư�� �ܰ踦 �Ѵܰ辿 �ø���. (����1->����2, ����2->����)
	
	BET_CMD GetSmallBetBtnCmd(INT64 llMyMoney);
	BET_CMD GetBigBetBtnCmd(INT64 llMyMoney);
	LIMIT_BET_STEP GetSmallBetBtnStep();
	LIMIT_BET_STEP GetBigBetBtnStep();
	eLIMIT_X GetSmallBtnLimitMultiplier();
	eLIMIT_X GetBigBtnLimitMultiplier();
	void SetLimitBetType(LIMIT_BET_TYPE eLimitBetType);
	/////////////////////////////////////////////////////////////////////

	//������ : �սǱݾ� ���
	void SetLossMoneyWarning(bool enterroom = false) ;

	//������ : ����� �߰�(���̾Ƹ�� ȸ��)
	void SetWaitFlag(bool isWait);

	//������ : �ڱ� ���̸� �������
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

	/*===========================(����)=============================*/
	virtual void LoadImage( LPCTSTR strBetBtn_N, LPCTSTR strBetBtn_H, LPCTSTR strBetBtn_P );
	virtual void Init(CGameViewBase *pView, CUIInfoData *pUIInfoData, CChattingWindowMan *pChattingWindowMan);	
	virtual void DrawBtn(CDC *pDC);
	virtual BOOL OnLButtonDown(int x , int y);
	virtual BOOL OnLButtonUp(int x , int y);
	virtual BOOL OnMouseMove(int x , int y);
	//���� ��ư ����
	virtual void ClearFlag();
	virtual void SetBettinBtnDiable(bool bDisable = true);
	virtual void FirstBetCheck(); //2017.8 ����尳��
	virtual BOOL IsFirstBet();	//ù���� �ݾ� ������ ���� ����. 2018.05
	virtual void SetChangeDisplayButton(bool bFull);
	virtual void SetObserverBtn(int nNum);
	virtual void InitObserverBtn(void);
	virtual	void SetLastCmdBackup( RAISE_LAST_CMD eLastCmd, bool bReset = false );
//	virtual void SetGameRoomInfo( int bBetRuleKind );
	virtual	int  GetLastCmdBackup();
	virtual void SetOverImageIndex( int Cmd, int nButtonNumber );

	/*===========================(���̷ο�)=============================*/		
	virtual void EnterRoom_Button_Init( int bBetRuleKind );

	virtual void SetBetBtnType(CONFIG_BETBTN_TYPE eBtnDirectionType, bool bForceSet = false);
	virtual void SetButtonClick(bool bClick) { m_bBtnClick = bClick; }
	virtual bool GetButtonClick() { return m_bBtnClick;}

	virtual BOOL OnCommand( WPARAM wParam, LPARAM lParam );
	virtual void SetShowBetMony();
	
	virtual void MakeHalfBtnAllinBtn(BOOL bAllin); //������ư�� ���ι�ư���� �����. (
	virtual void MakeDadangBtnAllinBtn(BOOL bAllin);
	virtual void MakeFullBtnAllinBtn(BOOL bAllin);
	virtual void MakeMaxBtn(int BUTTON_IDM , BOOL isMax, INT64 maxMoney);
	virtual void ResetMaxBtn(); //�ƽ���ư �ʱ�ȭ

	BOOL GetShowBDTooltip();	
};

#define MakeBigBtnAllinBtn MakeHalfBtnAllinBtn			//����Ʈ���� ���ư�� ���ι�ư���� �����. 
#define MakeSmallBtnAllinBtn MakeDadangBtnAllinBtn		//����Ʈ���� ������ư�� ���ι�ư���� �����. 