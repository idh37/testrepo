// PlayScr.h: interface for the CPlayScr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYSCR_H__4DF90567_2B2D_4E65_8A16_E24DFFE6B594__INCLUDED_)
#define AFX_PLAYSCR_H__4DF90567_2B2D_4E65_8A16_E24DFFE6B594__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserStruct.h"
#include "commmsg.h"
#include "PlayerHoola.h"
#include "SunPickUp.h"
#include "GameButtonHA.h"

#define MAX_CARDCOUNT		8
#define MAX_CARD_VALUE		13
#define MAX_CARD_FACE		4
#define CARD_JOKER1			52
#define CARD_JOKER2			53

typedef std::deque< std::deque< BYTE > > DUALBYTEDQ;
typedef std::deque< BYTE > BYTEDQ;
class CCardInfo
{
public:
	CCardInfo();
	virtual ~CCardInfo();
	void Reset();
public:
	DUALBYTEDQ			m_dqStraight;
	DUALBYTEDQ			m_dqTogether;
	DUALBYTEDQ			m_dqSelect;
	BYTEDQ				m_dqAttachStraight;
	BYTEDQ				m_dqAttachTogether;
	BYTEDQ				m_dqSeven;
	BYTEDQ				m_dqUpCard;
};

class CPlayScr  
{
public:
	enum eCARDEVENT_TYPE
	{
		eCARDEVENT_NULL = 0,
		eCARDEVENT_THANK,
		eCARDEVENT_REGIST,
		eCARDEVENT_GET,
		eCARDEVENT_DUMP,
		eCARDEVENT_UNKNOWN
	};
	enum eCARDMOVE_TYPE
	{
		eCARDMOVE_NULL = 0,
		eCARDMOVE_STRAIGHT,
		eCARDMOVE_TOGETHER,
		eCARDMOVE_MAX
	};
	enum eCARDMOVE_STATE
	{
		eCARDMOVE_START = 0,
		eCARDMOVE_ING,
		eCARDMOVE_END,
	};
	enum eHELPMSG_TYPE
	{		
		eHELPMSG_GETCARD = 3,
		eHELPMSG_SELECTDUMPCARD = 4,
		eHELPMSG_REGISTENABLE = 5,
		eHELPMSG_SELECTREGPOS = 6,
		eHELPMSG_THANKSELECT = 7,
		eHELPMSG_THANKENABLE = 8,
		eHELPMSG_STOPENABLE = 10,
	};
	enum eTHANK_TYPE
	{
		eTHANK_NORMAL = 0,
		eTHANK_LOW,
		eTHANK_HIGH,
		eTHANK_SEVEN4CARD,
	};
	enum eSORTBTN_TYPE
	{
		eSORTBTN_STRAIGHT,
		eSORTBTN_TOGETHER
	};
	enum eCARDSELECT_TYPE
	{
		eCARDSELECT_NONE = 0,
		eCARDSELECT_SELECT_777,
		eCARDSELECT_NONE_777,
		eCARDSELECT_SELECT_ATTACH,
		eCARDSELECT_NONE_ATTACH
	};
	enum eTIMER_TYPE
	{		
		eTIMER_THANK_NORMAL = 1,
		eTIMER_GET = 2,		
		eTIMER_REGIST = 3,
		eTIMER_THANK_SELECT = 4,
		eTIMER_THANK_SELECT_DUP = 5,		
		eTIMER_THANK_SPECIAL = 6
	};

	typedef struct 
	{
		BYTEDQ				dqCard;
		std::deque< double > dqAddx;
		std::deque< double > dqAddy;
		std::deque< POINT > dqStart;
		std::deque< POINT > dqDest;
		std::deque< DWORD > dqDelay;
		std::deque< DWORD > dqTime;
		std::deque< char >	dqMoveFlag;
		std::deque< char >	dqType;
		eCARDEVENT_TYPE		eType;
		eCARDMOVE_TYPE		eMoveType; // 1: Straight, 2: Together
		char				cDumpCard;
		DWORD				dwTime;
		int					nPN;
		char				ThankNum;
	}MOVECARD;

	typedef struct{
		int					nX;
		int					nY;
		DWORD				dwStartTime;
	} JACKPOTMONEY;

	typedef struct {
		int					nX;
		int					nY;
		int					nType; // 1 : 레지 
		DWORD				dwStartTime;
		BOOL				bSound;		
	} ANIEFFECT;

	//////////////////////////////////////////////////////////////////////////
	//	함수
	//////////////////////////////////////////////////////////////////////////
public:	
	CPlayScr();
	virtual ~CPlayScr();
	void Init(CWnd *pWnd, NMBASE::GRAPHICGDI::CPage *ppage, int pnum, CGameButtonHA *pGameButtonHA);
	void Reset();
	void OnTimer();
	void OnGameKey( int nVKey, int nextended, int repeat );
	BOOL OnLButtonDown(int xp, int yp);
	BOOL OnLButtonUp(int xp, int yp);
	void OnLButtonDblClick( int xp, int yp );
	BOOL OnMouseMove(int, int);
	void Draw(CDC *pDC);
	


	//훌라로 이겼는가
	BOOL IsWinCaseHoola(int nWincase);
	//버튼 클릭이 가능한가
	BOOL IsButtonClickEnableState();
	//정상적인 카드 정보인가(조커 포함)
	BOOL IsValidCardIndex_WITH_Joker(int nCardIndex);
	//정상적인 카드 정보인가(조커 미포함)
	BOOL IsValidCardIndex_WITHOUT_Joker(int nCardIndex);
	//소빵으로 이겼는가
	BOOL IsLowCardByDumpCard( char cDumpCard );
	//대빵으로 이겼는가
	BOOL IsHighCardByDumpCard( char cDumpCard );
	//7포카드로 이겼는가
	BOOL IsSevenFourCardByDumpCard( char cDumpCard );	
	//낮장 조합인가
	BOOL IsStraightDeque(std::deque<BYTE> dq);
	//묶음 조합인가
	BOOL IsTogetherDeque(std::deque<BYTE> dq);
	//큐가 특정 값을 가지고 있는가
	BOOL hasDequeTheValue(std::deque<BYTE> dq, BYTE b);
	//카드 인덱스에서 A가 B보다 작은가 -> 이거 산술연산자 오버로딩하든지 해서 바꾸자
	BOOL Is_A_SmallThan_B(BYTE a, BYTE b);
	//K카드 인가
	BOOL IsKCard(BYTE n){ return ( n == 12 || n == 25 || n == 38 || n == 51 ); }
	//Ace카드 인가
	BOOL IsAceCard(BYTE n){ return ( n == 0 || n == 13 || n == 26 || n == 39 ); }
	//카드 인덱스가 7카드 인가
	BOOL IsSevenCard(int n){ return ( n == 6 || n == 19 || n == 32 || n == 45 ); };
	//카드 인덱스가 조커카드 인가
	BOOL IsJokerCard(int n){ return ( n == 52 || n == 53 ); };
	//연습게임 진행중이고 자신이 관전중인 사용자인가
	BOOL IsMyPracticeGameObserver();
	// 등록가능 유무 판별
	BOOL CheckAvailableReg();
	//큐에서 중복되는 데이터를 제거한뒤 반환
	BYTEDQ MakeUniqueDQ(BYTEDQ dq);	
	//선택된 카드인가
	BOOL IsSelected( char cCardIndex );
	//
	int  CheckStopOK( int &nCardSum );
	//
	int  CanRegistedCard( char cCardIndex );
	//게임 오버 상태를 세팅한다
	void SetGameOver( BOOL bGameOver ) { m_bGameOver = bGameOver; }	
	//게임오버 상태를 세팅한다
	void SetGameOverResult( GAMEOVERRESULT* pGameOverResult ) { m_GameResult = *pGameOverResult; }
	//게임 결과창 나오는 시기를 지연 세팅한다
	void SetResultDelay( BOOL bResultDelay );
	//게인의 게임 결과 카드를 세팅한다
	void SetResultCard();
	//카드를 정렬한다
	void SortMyCard( BOOL bTypeStraight );
	//m_SameNumCard, m_CenterCard를 세팅한다
	void SetCardData( char arrCenterCard[MAX_CARD_FACE][MAX_CARD_VALUE], char arrSameCard[MAX_CARD_VALUE][MAX_CARD_FACE] );	
	//특수 상황에서 카드선택시 묶음조합 카드를 등록한다
	void RegistStraight();
	//특수 상황에서 카드선택시 낱장조합 카드를 등록한다
	void RegistTogether();
	//
	void RegistByCardSelect(int nCardDeckIndex);
	//REGISTERCARD구조체를 초기화 한다
	void ResetREGISTERCARD(REGISTERCARD* rc);	
	//
	void MakeAttachableDq( BYTE cardIndex );	
	//훌라 애니메이션을 플레이한다
	void PlayHoolaAnimation();	
	//
	void CheckJokerCardProblem();
	//
	void SetThankSelectButton();
	//
	void SortMyCard();
	//
	void PassThankyou();	
	//
	void StopCard(char type=0);
	//
	void ResetPlayer( int pnum );
	//
	void SetSelectRegCard( int nIndex );
	//등록 상황 버튼 세팅
	void SetRegCardButton();
	//버리기 상황 버튼 세팅
	void SetDumpCardButton();
	//받기 상황 버튼 세팅
	void SetGetCardButton();
	//모든 버튼 비활성화
	void SetNullButton();
	//모든 버튼 비활성화
	void SetAllBtnClose();
	//땡큐 버튼 상태 세팅
	void SetThankButton(eTHANK_TYPE eType);
	//게임 시작 버튼 활성화
	void EnableGameStartBtn();
	//게임 시작 버튼 비활성화
	void DisableGameStartBtn();
	//연습게임 버튼 활성화
	void EnableGamePracticsBtn();
	//연습게임 버튼 비활성화
	void DisableGamePracticsBtn();
	//게임오버 상황의 버튼 세팅
	void SetButtonGameOver();
	//
	void SetSortBtn();
	//숫자키로 카드를 선택했을 때의 프로세스
	void KeyDownCardShortcut(int nVKey);
	//패스 버튼 클릭 대응 프로세스
	void PassBtnClick();
	//땡큐 버튼 클릭 대응 프로세스
	void ThankBtnClick();
	//등록 버튼 클릭 대응 프로세스
	BOOL RegisBtnClick( int nType = 0 );
	//스톱 버튼 클릭 대응 프로세스
	void StopBtnClick();
	//받기 버튼 클릭 대응 프로세스
	void GetCardBtnClick();
	//버리기 버튼 클릭 대응 프로세스
	void DumpCardBtnClick();
	//
	void DumpCardBtnClickAfter();
	//정렬 버튼 클릭 대응 프로세스
	void SortBtnClick();	
	//주어진 큐에서 묶음조합을 반환
	DUALBYTEDQ FindSerialValue( BYTEDQ& dqSeq, BOOL bIgnoreJoker = TRUE, int nCardNum = -1 );
	//주어진 큐에서 낱장조합을 반환
	DUALBYTEDQ FindSameValue( BYTEDQ& dqSeq, BOOL bIgnoreJoker = TRUE );
	//주어진 큐에서 7카드를 반환
	BYTEDQ Find7CardValue( BYTEDQ& dqSeq);
	//주어진 큐에서 특정 데이터를 삭제
	void RemoveElement( BYTEDQ& dqData, BYTEDQ& dqWillBeRemoved );
	//붙이기 가능 상황 확인
	void CheckAttach( BYTEDQ& dqData, DUALBYTEDQ& dqSerial, DUALBYTEDQ& dqSame, BYTEDQ& dqAttachA, BYTEDQ& dqAttachB, BYTEDQ& dqSeven, BOOL bIgnoreJoker = TRUE );
	//중복되는 데이터가 있는지 확인
	BOOL CheckDupValue( BYTEDQ& dqData, DUALBYTEDQ& dqSerial, DUALBYTEDQ& dqSame, DUALBYTEDQ& dqResult );
	//낱장조합쪽에 붙일 수 있는지 확인
	BOOL CheckAvailableB( BYTE cCardIndex );
	//등록 가능한지 확인
	BOOL CheckReg();
	//주어진 큐에서 등록 가능한지 확인
	BOOL CheckReg( BYTEDQ& dqSeq );
	//땡큐 가능한지 확인
	void CheckThank( BYTE cCardIndex, int nType = 1 );
	//주어진 큐에서 땡큐 가능한지 확인
	void CheckThank( BYTEDQ& dqSeq );
	//턴 정보 초기화
	void ResetTurnFlag();
	//
	BOOL SendThank( int pnum );
	//
	BOOL SendThank();
	//
	void SendThankStep2( int nType = 0 );
	//타이머 세팅
	void SetLimitTime( eTIMER_TYPE eType );
	//타이머 초기화
	void ResetLimitTime();
	//
	void EnableSelect();
	//
	void ResetSelect();
	//
	void SelectOkBtnClick();
	//
	BOOL NowSelectedCard( char cCardIndex );	
	//
	void ResultOkBtnClick();	
	//
	BOOL IsNewCard( char cCardIndex );	
	//
	void SetMoveThankCard( int nCardNum, BYTE* pCardIndex, char nDumpCard, int pnum, int nAType );
	//
	void SetMoveRegCard( CSV_REGRESULT* pMsg );
	//
	void SetMoveGetCard( char cCardIndex, int nPnum );
	//
	void SetMoveDumpCard( char cCardIndex, char nTN, int nPnum);
	//
	void ResetMoveCard();
	//
	void GetStraightDestPos( BYTEDQ& dqCard, std::deque< POINT >& dqDest );
	//
	void GetStraightDestPos( BYTE cCard, POINT& ptDest );
	//
	void GetTogetherDestPos( BYTEDQ& dqCard, std::deque< POINT >& dqDest );
	//
	void GetTogetherDestPos( BYTE cCard, POINT& ptDest );
	//
	void GetMyStartPoint( BYTE cCard, POINT& ptStart );
	//
	void GetOtherStartPoint( int pnum, int nCardNum, POINT& ptStart );
	//무늬 순서로 정렬한다
	void SortByFace();
	//잭팟 상태를 세팅한다
	void SetJackpotState( char* pID, int nStep );
	void SetJackpotData( INT64 i64Money );
	//미션 세팅
	void SetMission( int nMNum, int nMBonus, int nMaxM );
	//미션 종료
	void CompMission( char *pID, int& GetMoney );
	//안내 문구 세팅
	void SetHelpMsg( int nType );
	//
	void SetAddedCardData( char *pStCards, char *pToCards, char *pID, int nDisCard, char *pPlayerCard );
	//
	void SetFixCardData( CSV_FIXREGIST FR );
	//게임 시작 요청
	void ReqGameStart();
	//연습게임 시작 요청
	void ReqPracticsGameStart();
	//
	void FixRegist( int nPN );
	//
	void Set7cardEffect();
	//
	void SetRuleWinEffect( int ruleKind, char *pCardData );
	//테스트용 카드덱 세팅
	void MakeTestCardDeck();
	//가짜 환경 세팅
	void MakeFakeSet();

	//////////////////////////////////////////////////////////////////////////
	// 사용자 컨트롤 관련 함수
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// Process 관련 함수
	//////////////////////////////////////////////////////////////////////////
	
	// 받은 유저의 아이디로 턴 카운트를 세팅한다
	void SetTurnID( char *pID );
	
	//////////////////////////////////////////////////////////////////////////
	// View 관련 함수
	//////////////////////////////////////////////////////////////////////////
	//타이머 그리기
	void DrawProcessTimer();
	//게임결과(땡큐박, 독박 등) 애니메이션 이펙트를 그린다
	void DrawAniEffect();
	//카드 선택 상황 그리기
	void DrawSelectCard();
	//훌라에 성공했을 때, 훌라를 그린다
	void DrawHoola();
	//중앙 하단 도움말 그리기
	void DrawHelpMsg();
	//카드 단축키 그리기
	void DrawMyCardShortcut();
	//버튼들 위에 그리기 위해서 따로 빼는 노땡큐버튼
	void DrawNoThankBtnSpr();		
	//중앙 카드덱에 표시되는 남은 카드 갯수를 그린다
	void DrawRemainCardCount( CDC *pDC );
	//골드 상황에서 애니메이션 그리기
	void DrawGold();
	//잭팟 상황 애니메이션 그리기
	void DrawJackpotAni( CDC *pDC );
	//미션 시작 그리기
	void DrawMissionStart();
	//미션 상황 그리기
	void DrawMission(CDC* pDC);
	//미션 결과 그리기
	void DrawMissionComp(CDC *pDC);
	//시작 프레임 그리기
	void DrawStartFrame();
	//땡큐 이동 상황 그리기
	void DrawThankMove();
	//등록 이동 상황 그리기
	void DrawRegMove();
	//받기 이동 상황 그리기
	void DrawGetMove();
	//버리기 이동 상황 그리기
	void DrawDumpMove();
	//카드 선택 창 그리기
	void DrawSelectCardSpecialCase();
	//잭팟 그리기
	void DrawJackPot();
	//사용자의 카드를 그린다
	void DrawPlayerCard( CDC *pDC );
	//바닥패를 그린다
	void DrawCommonCard();



	//////////////////////////////////////////////////////////////////////////
	//	변수
	//////////////////////////////////////////////////////////////////////////
protected:
	DUALBYTEDQ			m_dqAttachableFakeDq;
	BOOL				m_bResultDelay;
	DWORD				m_dwResultDelay;
	BOOL				m_bGameOver;													// 게임 오버 상태인가?
	CCardInfo			m_cCardInfo;
	CCardInfo			m_cClickCardInfo;
public:	
	CWnd						*m_pParent;
	NMBASE::GRAPHICGDI::CPage	*m_pPage;
	CSunPickUp			m_cSunPickup;

	std::deque< MOVECARD* > m_dqMoveCard;
	std::deque< char >	m_dqNewCard;
	MOVECARD			m_MoveCard;
	DUALBYTEDQ			m_dqSameCardPosIndex;
	BYTEDQ				m_dqSelectedCard;
	int					m_nLTimeCase;
	DWORD				m_dwLTime;
	int					m_nSec;
	BOOL				m_bRegSelect;	
	BOOL				m_bClickRegCard;
	BOOL				m_bSelectDraw;
	BOOL				m_bPressThank;
	BOOL				m_bThankReserved;
	BOOL				m_bThankSelect;
	int					m_nUpCardType;
	char				m_cThankCard;
	char				m_cThankFlag;
	char				m_cThankFlagClick;
	BOOL				m_bThankStep2;
	DUALBYTEDQ			m_dqThankSelect;
	DUALBYTEDQ			m_dqThankSelectClick;
	BOOL				m_bShowGameResult;
	int					m_SameCard[MAX_CARD_VALUE][MAX_CARD_FACE];
	int					m_nSelectCurrent;
	int					m_nSelectMax;		
	int					CardXY[MAX_PLAYER][2];											// 카드 스크린의 기준좌표
	bool				GameResultFlag;
	GAMEOVERRESULT		m_GameResult;
	int					m_nTurnIndex;
	int					m_nTurnCnt;	
	BYTE				m_JokerCard[2];
	char				m_PlayerCard[MAX_PLAYER][MAX_CARDCOUNT];						// 각 개인이 가진 7장의 카드
	BYTE				m_DisCard[54];													// 버린 카드
	int					m_nDisCardCnt;
	int					m_nCurStockCnt;													// 쌓여 있는 카드 수
	eCARDSELECT_TYPE	m_eCardSelectType;
	int					m_nDiscardTest;
	CGameButtonHA*		m_pGameButtonHA;												// 게임에 사용되는 버튼들
	BOOL				m_bResult;														// 게임 종료
	BYTE				m_MyCard[MAX_CARD_VALUE][MAX_CARD_FACE];
	int  				m_CenterCard[MAX_CARD_FACE][MAX_CARD_VALUE];					// 중앙에 등록 카드및 사이드카드
	int  				m_SameNumCard[MAX_CARD_VALUE][MAX_CARD_FACE];					// 같은 숫자 카드
	eSORTBTN_TYPE		m_eSortBtnType;
	BOOL				m_bWind;
	BOOL				m_bJackpot;
	DWORD				m_dwJackpot;
	BOOL				m_bJackpotConditionSucceed;
	int					m_nJackpotConditionStep;
	DWORD				m_dwJackpotConditionTime;
	BOOL				m_bJackpotMouseOver;
	BOOL				m_bJackpotMouseOverOff;
	DWORD				m_dwJackpotMouseTime;
	JACKPOTMONEY		m_JackpotMoneyData[20];
	std::deque< int >	m_dqJackpotMoney;
	BOOL				m_bJackpotData;
	char				m_sJackpotID[16];
	int					m_nJackPotStep;
	BOOL				m_bLClick;
	int					m_nLClickType;
	BOOL				m_bDrag;
	POINT				m_ptDrag;
	POINT				m_ptDragCard;
	BOOL				m_bMissionStart;
	BOOL				m_bMission;
	int					m_nMissionNum;
	DWORD				m_dwMission;
	BOOL				m_bMissionComp;
	BOOL				m_dwMissionComp;
	CString				m_sMissionCompID;
	BOOL				m_bStartFrame;
	int					m_nStartFrameStep;
	DWORD				m_dwStartFrame;
	CPoint				m_ptAttachACardPos[MAX_CARD_VALUE][MAX_CARD_FACE];
	CPoint				m_ptAttachBCardPos[MAX_CARD_FACE][MAX_CARD_VALUE];
	BOOL				m_bGoldState;
	int					m_nGoldStage;
	int					m_nGoldStage0;
	BOOL				m_bDrawGold;
	BOOL				m_bPressThankBtn;
	DWORD				m_dwThankBtnTime;
	int					m_nThankType;
	DWORD				m_dwHelpTime;
	int					m_nHelpIndex;
	BOOL				m_bEndGoldStage;
	char				m_CardData[MAX_CARDCOUNT];
	char				m_RuleKind;
	std::deque< ANIEFFECT > m_dqAniEffect;
	std::deque< ANIEFFECT > m_dqAniEffectResult;
	BOOL				m_bHoolaWin;
	DWORD				m_dwHoolaTime;
	BOOL				m_bHoolaSound;
	BOOL				m_bJackpotSound;
	int					m_nOldCardCnt[MAX_PLAYER];
	DWORD				m_nOldCardCntTime[MAX_PLAYER];
	BOOL				m_bPracticsGame;
	BOOL				m_bPlayMusicAfter;
	BOOL				m_bFirstCheckReg;
	BOOL				m_bFirstCheckingReg;
};

#endif // !defined(AFX_PLAYSCR_H__4DF90567_2B2D_4E65_8A16_E24DFFE6B594__INCLUDED_)
