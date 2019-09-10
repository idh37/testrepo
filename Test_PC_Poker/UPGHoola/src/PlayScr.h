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
		int					nType; // 1 : ���� 
		DWORD				dwStartTime;
		BOOL				bSound;		
	} ANIEFFECT;

	//////////////////////////////////////////////////////////////////////////
	//	�Լ�
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
	


	//�Ƕ�� �̰�°�
	BOOL IsWinCaseHoola(int nWincase);
	//��ư Ŭ���� �����Ѱ�
	BOOL IsButtonClickEnableState();
	//�������� ī�� �����ΰ�(��Ŀ ����)
	BOOL IsValidCardIndex_WITH_Joker(int nCardIndex);
	//�������� ī�� �����ΰ�(��Ŀ ������)
	BOOL IsValidCardIndex_WITHOUT_Joker(int nCardIndex);
	//�һ����� �̰�°�
	BOOL IsLowCardByDumpCard( char cDumpCard );
	//�뻧���� �̰�°�
	BOOL IsHighCardByDumpCard( char cDumpCard );
	//7��ī��� �̰�°�
	BOOL IsSevenFourCardByDumpCard( char cDumpCard );	
	//���� �����ΰ�
	BOOL IsStraightDeque(std::deque<BYTE> dq);
	//���� �����ΰ�
	BOOL IsTogetherDeque(std::deque<BYTE> dq);
	//ť�� Ư�� ���� ������ �ִ°�
	BOOL hasDequeTheValue(std::deque<BYTE> dq, BYTE b);
	//ī�� �ε������� A�� B���� ������ -> �̰� ��������� �����ε��ϵ��� �ؼ� �ٲ���
	BOOL Is_A_SmallThan_B(BYTE a, BYTE b);
	//Kī�� �ΰ�
	BOOL IsKCard(BYTE n){ return ( n == 12 || n == 25 || n == 38 || n == 51 ); }
	//Aceī�� �ΰ�
	BOOL IsAceCard(BYTE n){ return ( n == 0 || n == 13 || n == 26 || n == 39 ); }
	//ī�� �ε����� 7ī�� �ΰ�
	BOOL IsSevenCard(int n){ return ( n == 6 || n == 19 || n == 32 || n == 45 ); };
	//ī�� �ε����� ��Ŀī�� �ΰ�
	BOOL IsJokerCard(int n){ return ( n == 52 || n == 53 ); };
	//�������� �������̰� �ڽ��� �������� ������ΰ�
	BOOL IsMyPracticeGameObserver();
	// ��ϰ��� ���� �Ǻ�
	BOOL CheckAvailableReg();
	//ť���� �ߺ��Ǵ� �����͸� �����ѵ� ��ȯ
	BYTEDQ MakeUniqueDQ(BYTEDQ dq);	
	//���õ� ī���ΰ�
	BOOL IsSelected( char cCardIndex );
	//
	int  CheckStopOK( int &nCardSum );
	//
	int  CanRegistedCard( char cCardIndex );
	//���� ���� ���¸� �����Ѵ�
	void SetGameOver( BOOL bGameOver ) { m_bGameOver = bGameOver; }	
	//���ӿ��� ���¸� �����Ѵ�
	void SetGameOverResult( GAMEOVERRESULT* pGameOverResult ) { m_GameResult = *pGameOverResult; }
	//���� ���â ������ �ñ⸦ ���� �����Ѵ�
	void SetResultDelay( BOOL bResultDelay );
	//������ ���� ��� ī�带 �����Ѵ�
	void SetResultCard();
	//ī�带 �����Ѵ�
	void SortMyCard( BOOL bTypeStraight );
	//m_SameNumCard, m_CenterCard�� �����Ѵ�
	void SetCardData( char arrCenterCard[MAX_CARD_FACE][MAX_CARD_VALUE], char arrSameCard[MAX_CARD_VALUE][MAX_CARD_FACE] );	
	//Ư�� ��Ȳ���� ī�弱�ý� �������� ī�带 ����Ѵ�
	void RegistStraight();
	//Ư�� ��Ȳ���� ī�弱�ý� �������� ī�带 ����Ѵ�
	void RegistTogether();
	//
	void RegistByCardSelect(int nCardDeckIndex);
	//REGISTERCARD����ü�� �ʱ�ȭ �Ѵ�
	void ResetREGISTERCARD(REGISTERCARD* rc);	
	//
	void MakeAttachableDq( BYTE cardIndex );	
	//�Ƕ� �ִϸ��̼��� �÷����Ѵ�
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
	//��� ��Ȳ ��ư ����
	void SetRegCardButton();
	//������ ��Ȳ ��ư ����
	void SetDumpCardButton();
	//�ޱ� ��Ȳ ��ư ����
	void SetGetCardButton();
	//��� ��ư ��Ȱ��ȭ
	void SetNullButton();
	//��� ��ư ��Ȱ��ȭ
	void SetAllBtnClose();
	//��ť ��ư ���� ����
	void SetThankButton(eTHANK_TYPE eType);
	//���� ���� ��ư Ȱ��ȭ
	void EnableGameStartBtn();
	//���� ���� ��ư ��Ȱ��ȭ
	void DisableGameStartBtn();
	//�������� ��ư Ȱ��ȭ
	void EnableGamePracticsBtn();
	//�������� ��ư ��Ȱ��ȭ
	void DisableGamePracticsBtn();
	//���ӿ��� ��Ȳ�� ��ư ����
	void SetButtonGameOver();
	//
	void SetSortBtn();
	//����Ű�� ī�带 �������� ���� ���μ���
	void KeyDownCardShortcut(int nVKey);
	//�н� ��ư Ŭ�� ���� ���μ���
	void PassBtnClick();
	//��ť ��ư Ŭ�� ���� ���μ���
	void ThankBtnClick();
	//��� ��ư Ŭ�� ���� ���μ���
	BOOL RegisBtnClick( int nType = 0 );
	//���� ��ư Ŭ�� ���� ���μ���
	void StopBtnClick();
	//�ޱ� ��ư Ŭ�� ���� ���μ���
	void GetCardBtnClick();
	//������ ��ư Ŭ�� ���� ���μ���
	void DumpCardBtnClick();
	//
	void DumpCardBtnClickAfter();
	//���� ��ư Ŭ�� ���� ���μ���
	void SortBtnClick();	
	//�־��� ť���� ���������� ��ȯ
	DUALBYTEDQ FindSerialValue( BYTEDQ& dqSeq, BOOL bIgnoreJoker = TRUE, int nCardNum = -1 );
	//�־��� ť���� ���������� ��ȯ
	DUALBYTEDQ FindSameValue( BYTEDQ& dqSeq, BOOL bIgnoreJoker = TRUE );
	//�־��� ť���� 7ī�带 ��ȯ
	BYTEDQ Find7CardValue( BYTEDQ& dqSeq);
	//�־��� ť���� Ư�� �����͸� ����
	void RemoveElement( BYTEDQ& dqData, BYTEDQ& dqWillBeRemoved );
	//���̱� ���� ��Ȳ Ȯ��
	void CheckAttach( BYTEDQ& dqData, DUALBYTEDQ& dqSerial, DUALBYTEDQ& dqSame, BYTEDQ& dqAttachA, BYTEDQ& dqAttachB, BYTEDQ& dqSeven, BOOL bIgnoreJoker = TRUE );
	//�ߺ��Ǵ� �����Ͱ� �ִ��� Ȯ��
	BOOL CheckDupValue( BYTEDQ& dqData, DUALBYTEDQ& dqSerial, DUALBYTEDQ& dqSame, DUALBYTEDQ& dqResult );
	//���������ʿ� ���� �� �ִ��� Ȯ��
	BOOL CheckAvailableB( BYTE cCardIndex );
	//��� �������� Ȯ��
	BOOL CheckReg();
	//�־��� ť���� ��� �������� Ȯ��
	BOOL CheckReg( BYTEDQ& dqSeq );
	//��ť �������� Ȯ��
	void CheckThank( BYTE cCardIndex, int nType = 1 );
	//�־��� ť���� ��ť �������� Ȯ��
	void CheckThank( BYTEDQ& dqSeq );
	//�� ���� �ʱ�ȭ
	void ResetTurnFlag();
	//
	BOOL SendThank( int pnum );
	//
	BOOL SendThank();
	//
	void SendThankStep2( int nType = 0 );
	//Ÿ�̸� ����
	void SetLimitTime( eTIMER_TYPE eType );
	//Ÿ�̸� �ʱ�ȭ
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
	//���� ������ �����Ѵ�
	void SortByFace();
	//���� ���¸� �����Ѵ�
	void SetJackpotState( char* pID, int nStep );
	void SetJackpotData( INT64 i64Money );
	//�̼� ����
	void SetMission( int nMNum, int nMBonus, int nMaxM );
	//�̼� ����
	void CompMission( char *pID, int& GetMoney );
	//�ȳ� ���� ����
	void SetHelpMsg( int nType );
	//
	void SetAddedCardData( char *pStCards, char *pToCards, char *pID, int nDisCard, char *pPlayerCard );
	//
	void SetFixCardData( CSV_FIXREGIST FR );
	//���� ���� ��û
	void ReqGameStart();
	//�������� ���� ��û
	void ReqPracticsGameStart();
	//
	void FixRegist( int nPN );
	//
	void Set7cardEffect();
	//
	void SetRuleWinEffect( int ruleKind, char *pCardData );
	//�׽�Ʈ�� ī�嵦 ����
	void MakeTestCardDeck();
	//��¥ ȯ�� ����
	void MakeFakeSet();

	//////////////////////////////////////////////////////////////////////////
	// ����� ��Ʈ�� ���� �Լ�
	//////////////////////////////////////////////////////////////////////////


	//////////////////////////////////////////////////////////////////////////
	// Process ���� �Լ�
	//////////////////////////////////////////////////////////////////////////
	
	// ���� ������ ���̵�� �� ī��Ʈ�� �����Ѵ�
	void SetTurnID( char *pID );
	
	//////////////////////////////////////////////////////////////////////////
	// View ���� �Լ�
	//////////////////////////////////////////////////////////////////////////
	//Ÿ�̸� �׸���
	void DrawProcessTimer();
	//���Ӱ��(��ť��, ���� ��) �ִϸ��̼� ����Ʈ�� �׸���
	void DrawAniEffect();
	//ī�� ���� ��Ȳ �׸���
	void DrawSelectCard();
	//�Ƕ� �������� ��, �Ƕ� �׸���
	void DrawHoola();
	//�߾� �ϴ� ���� �׸���
	void DrawHelpMsg();
	//ī�� ����Ű �׸���
	void DrawMyCardShortcut();
	//��ư�� ���� �׸��� ���ؼ� ���� ���� �붯ť��ư
	void DrawNoThankBtnSpr();		
	//�߾� ī�嵦�� ǥ�õǴ� ���� ī�� ������ �׸���
	void DrawRemainCardCount( CDC *pDC );
	//��� ��Ȳ���� �ִϸ��̼� �׸���
	void DrawGold();
	//���� ��Ȳ �ִϸ��̼� �׸���
	void DrawJackpotAni( CDC *pDC );
	//�̼� ���� �׸���
	void DrawMissionStart();
	//�̼� ��Ȳ �׸���
	void DrawMission(CDC* pDC);
	//�̼� ��� �׸���
	void DrawMissionComp(CDC *pDC);
	//���� ������ �׸���
	void DrawStartFrame();
	//��ť �̵� ��Ȳ �׸���
	void DrawThankMove();
	//��� �̵� ��Ȳ �׸���
	void DrawRegMove();
	//�ޱ� �̵� ��Ȳ �׸���
	void DrawGetMove();
	//������ �̵� ��Ȳ �׸���
	void DrawDumpMove();
	//ī�� ���� â �׸���
	void DrawSelectCardSpecialCase();
	//���� �׸���
	void DrawJackPot();
	//������� ī�带 �׸���
	void DrawPlayerCard( CDC *pDC );
	//�ٴ��и� �׸���
	void DrawCommonCard();



	//////////////////////////////////////////////////////////////////////////
	//	����
	//////////////////////////////////////////////////////////////////////////
protected:
	DUALBYTEDQ			m_dqAttachableFakeDq;
	BOOL				m_bResultDelay;
	DWORD				m_dwResultDelay;
	BOOL				m_bGameOver;													// ���� ���� �����ΰ�?
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
	int					CardXY[MAX_PLAYER][2];											// ī�� ��ũ���� ������ǥ
	bool				GameResultFlag;
	GAMEOVERRESULT		m_GameResult;
	int					m_nTurnIndex;
	int					m_nTurnCnt;	
	BYTE				m_JokerCard[2];
	char				m_PlayerCard[MAX_PLAYER][MAX_CARDCOUNT];						// �� ������ ���� 7���� ī��
	BYTE				m_DisCard[54];													// ���� ī��
	int					m_nDisCardCnt;
	int					m_nCurStockCnt;													// �׿� �ִ� ī�� ��
	eCARDSELECT_TYPE	m_eCardSelectType;
	int					m_nDiscardTest;
	CGameButtonHA*		m_pGameButtonHA;												// ���ӿ� ���Ǵ� ��ư��
	BOOL				m_bResult;														// ���� ����
	BYTE				m_MyCard[MAX_CARD_VALUE][MAX_CARD_FACE];
	int  				m_CenterCard[MAX_CARD_FACE][MAX_CARD_VALUE];					// �߾ӿ� ��� ī��� ���̵�ī��
	int  				m_SameNumCard[MAX_CARD_VALUE][MAX_CARD_FACE];					// ���� ���� ī��
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
