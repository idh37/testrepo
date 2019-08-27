#if !defined(AFX_QUICKJOINERDLG_H__88E12F72_2FFE_4BDD_938C_D365D994A84E__INCLUDED_)
#define AFX_QUICKJOINERDLG_H__88E12F72_2FFE_4BDD_938C_D365D994A84E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../resource.h"

typedef struct _tagGroupInfo
{
	bool	m_bUsed;			// 설정체크
	int		m_nRadioButPos;		// 라디오버튼 위치
	int		m_nComboValue;		// 콤보값
}SGroupInfo;

typedef struct 
{
	bool	m_bUseGameMode;			// 게임모드 사용 여부
	int		m_nGameMode;			// 게임모드

	bool	m_bUseBetRule;			// 베팅룰 사용여부
	bool	m_bAutoBet;				// 자동여부

	bool	m_bUseSeedMoney;		// 게임규모 사용 여부
	int		m_nSeedType;			// 게임규모(0: 큰방, 1: 일반방, 2:작은방)

	bool	m_bAutoEnterRoom;		// 자동으로 입장

	short	m_sBetRuleKind;			//리미트머니

	int		m_nCardSeting;			//카드세팅 4~6장

	float	m_nBetMulti;			//섯다 참가비

} SCONDITION;

// 설정 파일 저장 버전
#define QJ_VER		(3)

// Dialog Type  (IDX_GAME 와 순서가 동일)
enum QUICKJOINDLGTYPE
{
	EQJ_7POKER		= 0,	 
	EQJ_LOWBD		= 1,
	EQJ_NEWPOKER	= 2,
	EQJ_SUTDA		= 3,
	EQJ_HOOLA		= 4,
	//--------------------
	EQJ_MAXDLG	
};

enum PID_QUICKJOIN_ITEM
{
	//-----------------------
	PID_QUICKJOIN_ITEM_GAMEMODE,		// 게임모드
	PID_QUICKJOIN_ITEM_BETRULE,			// 베팅룰
	PID_QUICKJOIN_ITEM_SEEDMONEY,		// 시드머니	
	PID_QUICKJOIN_ITEM_CONDITION,		// 조건에 맞는 방이 없는경우
//	PID_QUICKJOIN_ITEM_STOPOPTION,		// 스톱옵션 설정(훌라 전용)
	//-----------------------
	PMAX_QUICKJOIN_ITEM,
};

// 기본으로 생성되는 라디오 버튼 최대 개수
#define MAX_RADIO	3

// 아것을 통해서 컨트롤의 X좌표는 해결이 된다.

// 아이템별  라디오버튼의 X좌표
// { 다이얼로그 종류(5개 - 7Poker, LowBD, NewPoker, Sutda, Hoola),
//   최대 아이템 개수(4개 - 게임모드, 베팅룰, 시드머니, 조건에 맞는 방이 없는경우),
//   라디오버튼 개수(3개 - 각항목에 X 좌표(최대 3개))
// }
//const CPoint ptRadioButton[EQJ_MAXDLG][PMAX_QUICKJOIN_ITEM][MAX_RADIO]=
//{
//	// 7Poker
//	{{CPoint(36, 124),	CPoint(126, 124),	CPoint(216, 124)},	// 게임모드
//	 {CPoint(36, 179),	CPoint(126, 179),	CPoint(216, 179)},	// 베팅룰 
//	 {CPoint(36, 234),	CPoint(126, 234),	CPoint(0, 0)},		// 시드머니
//	{CPoint(36, 331),	CPoint(36, 351),	CPoint(0, 0)}},		// 조건에 맞는 방이 없는경우 
////	{CPoint(0, 0),		CPoint(0, 0),		CPoint(0, 0)}},		// 훌라 작업으로 추가내용(스톱옵션)
//
//    // LowBD
//	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},				// 게임모드
//	{CPoint(36, 124),	CPoint(126, 124),	CPoint(216, 124)},	// 베팅룰 
//	{CPoint(36, 179),	CPoint(126, 179),	CPoint(0, 0)},		// 시드머니
//	{CPoint(36, 276),	CPoint(36, 296),	CPoint(0, 0)}},		// 조건에 맞는 방이 없는경우 
////	{CPoint(0, 0),		CPoint(0, 0),		CPoint(0, 0)}},		// 훌라 작업으로 추가내용(스톱옵션)
//
//	// NewPoker
//	{{CPoint(36, 124),	CPoint(126, 124),	CPoint(216, 124)},	// 게임모드
//	 {CPoint(36, 179),	CPoint(126, 179),	CPoint(216, 179)},	// 베팅룰 
//	 {CPoint(36, 234),	CPoint(126, 234),	CPoint(0, 0)},		// 시드머니
//	 {CPoint(36, 331),	CPoint(36, 351),	CPoint(0, 0)}},		// 조건에 맞는 방이 없는경우
////	{CPoint(0, 0),		CPoint(0, 0),		CPoint(0, 0)}},		// 훌라 작업으로 추가내용(스톱옵션)
//
//	// Sutda
//	{{CPoint(36, 124),	CPoint(126, 124),	CPoint(216, 124)},	// 게임모드
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},	// 베팅룰 
//	{CPoint(36, 179),	CPoint(126, 179),	CPoint(0, 0)},		// 시드머니
//	{CPoint(36, 276),	CPoint(36, 296),	CPoint(0, 0)}},		// 조건에 맞는 방이 없는경우 
////	{CPoint(0, 0),		CPoint(0, 0),		CPoint(0, 0)}},		// 훌라 작업으로 추가내용(스톱옵션)
//
//	// Hoola
//	{{CPoint(36, 124),	CPoint(126, 124),	CPoint(216, 124)},	// 게임모드 //방 정원 (설정안함, 2인, 5인)
//	{CPoint(36, 179),	CPoint(126, 179),	CPoint(216, 179)},	// 베팅룰	//게임모드(설정안함, 조커, 일반)
//	{CPoint(36, 234),	CPoint(126, 234),	CPoint(0, 0)},		// 시드머니	//시드머니(설정안함, 설정함)
//	{CPoint(36, 331),	CPoint(36, 351),	CPoint(0, 0)}},		// 조건에 맞는 방이 없는경우 
////	{CPoint(125, 204),	CPoint(216, 204),	CPoint(0, 0)}}		// 스톱옵션	//스톱옵션(스톱on, 스톱off)
//};

//// 아이템별  라디오버튼의 X좌표
//// { 다이얼로그 종류(4개 - 7Poker, LowBD, NewPoker, Sutda),
////   최대 아이템 개수(4개 - 게임모드, 베팅룰, 시드머니, 조건에 맞는 방이 없는경우),
////   라디오버튼 개수(3개 - 각항목에 X 좌표(최대 3개))
//// }
//const CPoint ptComboBox[EQJ_MAXDLG][PMAX_QUICKJOIN_ITEM][MAX_RADIO]=
//{
//	// 7Poker
//	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// 게임모드
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// 베팅룰 
//	{CPoint(0, 0),	CPoint(185, 233),	CPoint(0, 0)},	// 시드머니
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// 조건에 맞는 방이 없는경우 
//
//	// LowBD
//	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// 게임모드
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// 베팅룰 
//	{CPoint(0, 0),	CPoint(185, 177),	CPoint(0, 0)},	// 시드머니
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// 조건에 맞는 방이 없는경우 
//
//	// NewPoker
//	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// 게임모드
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// 베팅룰 
//	{CPoint(0, 0),	CPoint(185, 233),	CPoint(0, 0)},	// 시드머니
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// 조건에 맞는 방이 없는경우
//
//	// Sutda
//	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// 게임모드
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// 베팅룰 
//	{CPoint(0, 0),	CPoint(185, 177),	CPoint(0, 0)},	// 시드머니
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// 조건에 맞는 방이 없는경우 
//
//	// Hoola
//	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// 게임모드
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// 베팅룰 
//	{CPoint(0, 0),	CPoint(185, 233),	CPoint(0, 0)},	// 시드머니
//	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// 조건에 맞는 방이 없는경우 
//};

// 자율규제안 수정 작업.. enum 값과 항목이 안맞아도 생성엔 문제없음.
const CPoint ptRadioButton[EQJ_MAXDLG][PMAX_QUICKJOIN_ITEM][MAX_RADIO]=
{
	// 7Poker
	{{CPoint(36, 124),	CPoint(126, 124),	CPoint(216, 124)},	// 게임모드
	{CPoint(-10, 0),		CPoint(-10, 0),	CPoint(-10, 0)},			// 베팅룰 
	{CPoint(36, 179),	CPoint(126, 179),	CPoint(216, 179)},	// 시드머니 // 게임규모 
	{CPoint(0, 0),		CPoint(0, 0),	CPoint(0, 0)}},			// 조건에 맞는 방이 없는경우 // 사용안함
	//	{CPoint(0, 0),		CPoint(0, 0),		CPoint(0, 0)}},	// 훌라 작업으로 추가내용(스톱옵션)

	// LowBD
	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},				// 게임모드
	{CPoint(36, 124),	CPoint(126, 124),	CPoint(216, 124)},	// 베팅룰 
	{CPoint(36, 179),	CPoint(126, 179),	CPoint(0, 0)},		// 시드머니
	{CPoint(36, 276),	CPoint(36, 296),	CPoint(0, 0)}},		// 조건에 맞는 방이 없는경우 
	//	{CPoint(0, 0),		CPoint(0, 0),		CPoint(0, 0)}},		// 훌라 작업으로 추가내용(스톱옵션)

	// NewPoker
	{{CPoint(36, 124),	CPoint(126, 124),	CPoint(216, 124)},	// 게임모드
	{CPoint(36, 179),	CPoint(126, 179),	CPoint(216, 179)},	// 베팅룰 
	{CPoint(36, 234),	CPoint(126, 234),	CPoint(0, 0)},		// 시드머니
	{CPoint(36, 331),	CPoint(36, 351),	CPoint(0, 0)}},		// 조건에 맞는 방이 없는경우
	//	{CPoint(0, 0),		CPoint(0, 0),		CPoint(0, 0)}},		// 훌라 작업으로 추가내용(스톱옵션)

	// Sutda
	{{CPoint(36, 124),	CPoint(126, 124),	CPoint(216, 124)},	// 게임모드
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},	// 베팅룰 
	{CPoint(36, 179),	CPoint(126, 179),	CPoint(0, 0)},		// 시드머니
	{CPoint(36, 276),	CPoint(36, 296),	CPoint(0, 0)}},		// 조건에 맞는 방이 없는경우 
	//	{CPoint(0, 0),		CPoint(0, 0),		CPoint(0, 0)}},		// 훌라 작업으로 추가내용(스톱옵션)

	// Hoola
	{{CPoint(36, 124),	CPoint(126, 124),	CPoint(216, 124)},	// 게임모드 //방 정원 (설정안함, 2인, 5인)
	{CPoint(36, 179),	CPoint(126, 179),	CPoint(216, 179)},	// 베팅룰	//게임모드(설정안함, 조커, 일반)
	{CPoint(36, 234),	CPoint(126, 234),	CPoint(0, 0)},		// 시드머니	//시드머니(설정안함, 설정함)
	{CPoint(36, 331),	CPoint(36, 351),	CPoint(0, 0)}},		// 조건에 맞는 방이 없는경우 
	//	{CPoint(125, 204),	CPoint(216, 204),	CPoint(0, 0)}}		// 스톱옵션	//스톱옵션(스톱on, 스톱off)
};

// 아이템별  라디오버튼의 X좌표
// { 다이얼로그 종류(4개 - 7Poker, LowBD, NewPoker, Sutda),
//   최대 아이템 개수(4개 - 게임모드, 베팅룰, 시드머니, 조건에 맞는 방이 없는경우),
//   라디오버튼 개수(3개 - 각항목에 X 좌표(최대 3개))
// }
const CPoint ptComboBox[EQJ_MAXDLG][PMAX_QUICKJOIN_ITEM][MAX_RADIO]=
{
	//// 7Poker
	//{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// 게임모드
	//{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},	// 게임규모 
	//{CPoint(0, 0),	CPoint(0, 0),	CPoint(185, 177)},		// 시드머니
	//{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// 조건에 맞는 방이 없는경우 

	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// 게임모드
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// 베팅룰 
	{CPoint(0, 0),	CPoint(185, 233),	CPoint(0, 0)},	// 시드머니
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// 조건에 맞는 방이 없는경우 

	// LowBD
	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// 게임모드
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// 베팅룰 
	{CPoint(0, 0),	CPoint(185, 177),	CPoint(0, 0)},	// 시드머니
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// 조건에 맞는 방이 없는경우 

	// NewPoker
	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// 게임모드
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// 베팅룰 
	{CPoint(0, 0),	CPoint(185, 233),	CPoint(0, 0)},	// 시드머니
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// 조건에 맞는 방이 없는경우

	// Sutda
	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// 게임모드
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// 베팅룰 
	{CPoint(0, 0),	CPoint(185, 177),	CPoint(0, 0)},	// 시드머니
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// 조건에 맞는 방이 없는경우 

	// Hoola
	{{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// 게임모드
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)},		// 베팅룰 
	{CPoint(0, 0),	CPoint(185, 233),	CPoint(0, 0)},	// 시드머니
	{CPoint(0, 0),	CPoint(0, 0),	CPoint(0, 0)}},		// 조건에 맞는 방이 없는경우 
};

const CPoint ptGameScale[EQJ_MAXDLG]=
{
	CPoint(25, 198),	// 7Poker
	CPoint(25, 143),	// LowBD
	CPoint(25, 198),	// NewPoker
	CPoint(25, 143),	// Studa
	CPoint(25, 198)		// Hoola
};

const CRect	rtGameScaleText = CRect(11, 59, 11+253, 59+31);


// 라디오 버튼의 크기
#define WIDTH_RADIOBUTTON			(77)	// 라디오 버튼의 너비(원래 넓이는 11이지만 클릭이 편하도록 영역 넓이를 버튼이 겹치지 않는 만큼을 세팅)
#define HEIGHT_RADIOBUTTON			(11)	// 라디오 버튼의 높이

// 콤보 박스의 크기
#define WIDTH_COMBOBOX				(106)	// 콤보 박스 너비
#define HEIGHT_COMBOBOX				(100)	// 콤보 박스 높이


// 하나의 아이템 그룹에 최대 아래와 같이 3종류의 Type이 있다.
enum TYPE_CONTROL
{
	TYPE_CONTROL_RADIO			 = 0,	// 라디오 버튼
	TYPE_CONTROL_COMBO			 = 1,	// 콤보 박스
};


#define IDC_QUICKJOIN_PRIORITYBUTTON (1000000)
#define IDC_QUICKJOIN_RADIO			 (1001000)
 
// 기본적인 조건 Base
class CQJConditionBase
{
public:
	typedef struct _tagCONTROL
	{
		_tagCONTROL()
		{
			pContol = pChildControl = NULL;				
		}
		CWnd* pContol;		
		CWnd* pChildControl;
		TYPE_CONTROL TypeControl;
	}SCONTROL;

	CWnd* m_pParentWnd;						// 부모 윈도우
	QUICKJOINDLGTYPE	m_eDialogType;		// 다이얼로그 타입	
	PID_QUICKJOIN_ITEM	m_eQuickJoinItemID;	// 바로입장 세팅 항목 ID
	int m_nItemNumber;						// 몇번째 항목인가?(보유머니, 게임종류, 게임모드, 베팅룰, 시드머니, 스톱옵션적용여부)

	vector<SCONTROL> m_vecControls;			// 컨트롤 백터
	
	QJData m_sQuickJoinData;				// Data

public:	
	CQJConditionBase();
	virtual ~CQJConditionBase();

	virtual BOOL Create(QUICKJOINDLGTYPE eQJDlgType, PID_QUICKJOIN_ITEM eQJItemID, CWnd *pParentWnd, int nItemNumber);
	virtual void OnDestory();
	virtual void EnableControl(BOOL bEnable);
	virtual void ShowControl(BOOL bEnable);
	virtual int GetCurSelRadioButton(void){return 0;}
};


class CQJCondition : public CQJConditionBase
{
public:

	NMBASE::SKINGDI::CBmpRadioBtn*   m_pFirstRadioBtn;
	BOOL			m_bPriorityCheck;

	RULETYPE		m_eRuleType;			// 맞포커같은 경우 5/7룰이 있다.	

public:
	CQJCondition();
	virtual ~CQJCondition();

	BOOL DoesExist();

	// 실제 컨트롤을생성하는 함수( 하나의 아이템 그룹에 해당하는 컨트롤만 생성을 한다. )
	virtual BOOL Create(QUICKJOINDLGTYPE eQJDlgType,  PID_QUICKJOIN_ITEM eQJItemID, CWnd *pParentWnd, int nItemNumber);

	int GetRadioComboIndex(int &idxCombo);
	virtual int GetCurSelRadioButton(void);

	CComboBox *GetComboBox(int idxCombo);

	void RefreshConditonButton();

	virtual BOOL SaveData(const char *pStrSection);

	virtual BOOL LoadData(const char *pSection, BOOL bUseFlag);

	virtual BOOL _LoadData(const char *pSection);
};

// 하나의 아이템에 해당하는 컨트롤 그룹
class CMyCQJCondition : public CQJCondition
{
public:

	virtual BOOL Create(QUICKJOINDLGTYPE eQJDlgType, PID_QUICKJOIN_ITEM eQJItemID, CWnd *pParentWnd, int nItemNumber, INT64&	i64LImitMoney);	

	// 컨트롤에 이벤트가 발생을 하면 호출되는 함수
	BOOL ReloadCondition(IDX_GAME idxGame);	

	void SetCheckOff();	
	
	void SetCombo_AutobetSeedMoney(IDX_GAME idxGame, CComboBox* pComboBox, INT64 i64LImitMoney);
	BOOL SetAutobetSeedMoneyValue( void );	

	PID_QUICKJOIN_ITEM GetQuickjoinItemType(){return m_eItemType;}

	virtual BOOL SaveData(const char *pStrSection);

	virtual BOOL LoadData(const char *pSection, BOOL bUseFlag);

	virtual BOOL _LoadData(const char *pSection);
	
protected:
	CComboBox* m_pComboAutobetSeedMoney;		// 배팅룰/시드머니 콤보박스

	IDX_GAME m_eGameType;

	PID_QUICKJOIN_ITEM m_eItemType;				// 아이템 타입
};

// 게임에 해당하는 모든 컨트롤 그룹 생성
class CQuickJoinControls 
{	

public:

	QUICKJOINDLGTYPE m_eDialogType;								// 다이얼로그 타입

	CMyCQJCondition m_QuickJoinItemGroup[PMAX_QUICKJOIN_ITEM];	// 컨트롤 그룹을 생성한다.(보유머니/게임모드/게임룰/베팅룰/시드머니)

	int				m_nQuickJoinItemCreateCount;				// Item이 생성된 개수
	IDX_GAME		m_eSelectGameID;							// 선택된 게임 ID


	INT64			m_i64SelectLImitMoney;						// 제한머니

public:

	BOOL Create(CWnd *pParentWnd, QUICKJOINDLGTYPE eQJDlgType);

	void SetCheckOff();

	BOOL OnCommand(UINT CmdID);

	void ResetComboBox(IDX_GAME idxGame);

	BOOL SaveData();

	BOOL LoadData(BOOL bUseFlag);

	BOOL EnableControl(BOOL bEnable);

	int GetQjDataSize();

	int GetQjDataBinary(BYTE *pBuffer);

	static CString BinaryToString(const BYTE *pBuffer, int nSize);

	static BOOL SendQJData();

	CMyCQJCondition *GetQJCondition(PID_QUICKJOIN_ITEM eIDQJ);
private:
	void RefreshCondition();
};

/////////////////////////////////////////////////////////////////////////////
// CQuickJoinDlg dialog

class CUIInfoData;

class UPG_GAMEBASE_EXPORT CQuickJoinDlg : public CUPGDlgBase
{
	// Construction
public:
	CQuickJoinDlg(QUICKJOINDLGTYPE eDlgType, CUIInfoData *pUIInfoData, CWnd* pParent = NULL);

	NMBASE::SKINGDI::CMyBitmap m_cBackImg;
	NMBASE::SKINGDI::CMyBitmap m_cGameScaleImage;

	NMBASE::SKINGDI::CHTransRegion m_cRgn;

	IDX_GAME GetSelectedGame();

	static BOOL StringToBinary(CString &str, BYTE *pBuffer, int nSize);
	static bool IsEnableQJ(QUICKJOINDLGTYPE eType);
	static BOOL GetSearchCondition(QUICKJOINDLGTYPE eType, CString StrSection, SCONDITION* pInfo);

	static void SetInfo(QUICKJOINDLGTYPE eType, bool bUseGameMode, int nGameMode, int nRoomType, int nMaxUser, int nComboSel) ;

	//static IDX_GAME GetSelectGame();
	
	// Dialog Data
	//{{AFX_DATA(CQuickJoinDlg)
	enum { IDD = IDD_DIALOG_QUICKJOIN };
	//}}AFX_DATA

	/*--------- 라디오 버튼 ---------*/

	// 검색기능 사용(ON/OFF)(공통)
	NMBASE::SKINGDI::CBmpRadioBtn	m_RadioSearchOn;
	NMBASE::SKINGDI::CBmpRadioBtn	m_RadioSearchOff;

	NMBASE::SKINGDI::CBmpRadioBtn	m_rdMode1;	
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdMode2;
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdMode3;
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdMode4;
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdMode5;	
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdMode6;
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdMode7;
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdMode8;

	//규제안에 추가된 카드세팅 라디오 버튼
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdMode9;
	NMBASE::SKINGDI::CBmpRadioBtn	m_rdMode10;

	CComboBox		m_cbRoomType;	//게임규모 콤보박스
	CComboBox		m_cbCardSeting;	//규제안 카드세팅 추가 2014.01

	int				m_nMaxUser ;
	int				m_nRoomType ;
	int				m_nGameMode ;
	int				m_nComboSel ;	//게임규모의 선택값

	float			m_nBetMoneyMulti ; //규제안 관련해서 섯다 참가비의 저장된 선택값
	int				m_nCardSet ; //규제안 관련해서 카드세팅의 저장된 선택값
	bool			m_bUseCardSeting ;		//규제안 관련해서 카드세팅 사용여부

	bool			m_bUseGameMode;			// 게임모드 사용 여부

	/*--------- 버튼(공통) ---------*/

	NMBASE::SKINGDI::CMyButton		m_BtnApply;					// 설정
	NMBASE::SKINGDI::CMyButton		m_BtnCancel;				// 취소	

	QUICKJOINDLGTYPE m_eDialogType;				// 다이얼로그 종류

	CQuickJoinControls m_cQuickJoinControls;	// Quick Join에 필요한 컨트롤 그룹

	CUIInfoData* m_pUIInfoData;					// UI좌표 정보

	CPoint m_ptRoomType ;
	
	IDX_GAME m_nGame ;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQuickJoinDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

	static BOOL _LoadData(const char *pSection, int nItemID, SGroupInfo& sGroupInfo);

	static void LoadData(const char *pSection, bool* bUsed, int* nMode, int* nRoomType, int* nMaxUser, int* nCursel, bool* bUseCardOrBetMulti, int* nCardSet ,  float *nBetMoneyMulti );
	BOOL SaveData(const char *pStrSection) ;

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CQuickJoinDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();	
	afx_msg void OnButtonOn();
	afx_msg void OnButtonOff();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnButtonApply();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnCbnSelchangeComboCardseting();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUICKJOINERDLG_H__88E12F72_2FFE_4BDD_938C_D365D994A84E__INCLUDED_)
