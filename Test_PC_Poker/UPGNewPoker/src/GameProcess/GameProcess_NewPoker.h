// GameProcess_Seven.h: interface for the CGameProcess_Seven class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEPROCESS_GAMEPROCESS_NEWPOKER_H__87B4A196_AE81_4D0B_A34B_58AAD968CF9A__INCLUDED_)
#define AFX_GAMEPROCESS_GAMEPROCESS_NEWPOKER_H__87B4A196_AE81_4D0B_A34B_58AAD968CF9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <../src/GameProcess.h>
#include <SevenRuleMan.h>
#include "../SevenPoker/BombCard.h"
#include "../ResultFocus.h"
#include "../SelectCardNewPoker.h"
#include <UserStruct.h>
#include <CommMsgDef.h>
#include "../Player/Player_NP.h"
#include <../src/ObjectList.h>

#define MAX_CARDOPENGUIDE 5	//카드오픈알림 종류

typedef struct  {
	char					ID[16];
	char					PNum;
	INT64					nPMoney;	
}ENDINGUSERINFO;

typedef struct  {
	BOOL	bChanged;
	ENDINGUSERINFO	EndUserInfo[5];
}AFTERENDINGUSERINFO;

enum GAMEPLAY_ROUND_STATE
{
	GAME_ROUND_READY = 0,			// 아무상태도 아님(대기중)	0
		GAME_ROUND_START,				// 3장을 줌						1
		GAME_ROUND_CARDCHOICE,			// 3장중 카드를 선택함 			2
		GAME_ROUND_4,					// 4장을 줌						3
		GAME_ROUND_5,					// 5장을 줌						4
		GAME_ROUND_6,					// 6장을 줌						5
		GAME_ROUND_BOMB,				// 6구 배팅후 폭탄 카드 오픈	6			
		GAME_ROUND_RESULT,				// 승자 패자를 가림			9	
};

class CGameProcess_NewPoker : public CGameProcess
{
public:
	CSevenRuleMan			m_cSvRule;		//새로운 룰 
	CBombCard				m_cBombCard;
	CResultFocus			m_cResultFocus;
	
	char					m_pJackpotWinnerID[16];

	CAllBetChip				m_cAllBetChip;
	// [ 카드 이벤트 에러시 ]
	BOOL					m_bWindCardEnd;
	int						m_nWindCardEndCnt;
	
	// 게임 정보
	BOOL					m_bGameStart;		// 게임이 시작되었는가?(패돌리기 포함)
	
	// [ 베팅 딜레이 ]
	int				m_nActiveBetBtnDelayTime;
	BOOL			m_bBettingDelayCheck ;  // 0을 입력 받았을 때는 기본적인 딜레이가 없기 때문에 루틴 처리가 애매해서 체크용으로 넣은 변수.

	// 규제안 예약 베팅 관련, 폭탄, 조커 카드가 오픈됬는가.. 에 대한 처리가 필요함.
	bool					m_bOpenBombcard;		// 폭탄 카드 오픈 후 오는 raise 인지 

	//히든관련
	POINT	m_HiddenClickPos;
	int		m_nHinddenArrow_x;	
	int		m_nHinddenArrow_y;
	int		m_MouseHiddenState;
	BOOL	m_bMouseHiddenClick;
	BOOL	m_bkeyboardHiddenClick;
	DWORD	m_dwHiddenSceneDrawTime;
	DWORD   m_dwCardOpenGuideTime;

	BOOL	m_bDrawCardOpenGuide;
	int		m_NowCardOpenGuideIndex;
	vector<int>	m_CardOpenGuide;
// 	int						m_nHinddenArrow;
// 	BOOL					m_bHiddenClick;
// 	POINT					m_ptMouse;

	GAMEOVERRESULT			m_cGameOverResultData;
	int						m_nDebugCnt;


	// 폭탄 상태일때 이상한 값이 들어와서 게임오버가 되면 베팅버튼 활성화 되지 않도록 예외처리.
	bool					m_bGameOver;

	//조커 카드 
	DWORD					m_dwStartDrawJokerTime;
	DWORD					m_dwEndDrawJokerTime;
	
	CObjectList				m_cAniSprObjectMan;
	BOOL					m_bEventDrawFlg;	//골드 타임 또는 골든칩 이벤트 

	//2006.1.4 종료시 안내 메세지 그리는 부분
	int						m_nEndingInformationTextMsgCnt;		//종료 안내 메세지 개수
	CString					m_listEndingInformationTextMsg[30];		//종료 안내 메세지 내용

	CSelectCardNewPoker		m_cSelectCardMan;

	CPlayer_NP				*m_pPlayer;

protected:
	INT64 m_i64JackPotMoney;						// 잭팟금액
	INT64 m_i64OldJackPotMoney;						// 이전 잭팟금액

public:
	virtual void OnTimer();
	virtual void ResetGame();

	virtual BOOL OnMouseMove(int x, int y);
	virtual BOOL OnLButtonDblclk(int x, int y);
	virtual BOOL OnLButtonUp(int x, int y);
	virtual BOOL OnLButtonDown(int x, int y);

	virtual void OnGameKey(int nVKey, int nextended, int repeat = 0);

	//CSV_ACCEPT_CREATEROOM
	virtual bool Accept_CreateRoom(void *pMsgBase);	// 방만들기 허가
	virtual bool Accept_EnterRoom(void *pMsgBase);	// 방에 입장 허가 받음
	virtual void User_EnterRoom(void *pMsgBase);	// 유저가 방에 입장 알림
	virtual void User_OutRoom(void *pMsgBase);		// 유저가 방에서 나감 알림
	virtual void SetPlayInfo(int nowpnum, int newpnum, BOOL bParticipation );

	virtual BOOL IsPlayingNow();


	//규제안 예약기능 추가로 인한 가상함수 추가
	virtual void SendCallButton();
	virtual void SendCheckButton();
	virtual void SendDieButton();
	virtual void SendHalfButton();
	virtual void SendDdaButton();
	virtual void SendPingButton();

public:
	CGameProcess_NewPoker(CGame *pGame);
	virtual ~CGameProcess_NewPoker();

	void Init(NMBASE::GRAPHICGDI::CPage *ppage);

	// 패 돌리기 속도 조절 관련 함수들.
	void set_GameSpeed(); //UPG CClientDlg에있던걸 가져옴
	void OnInit();

	// 게임 진행 함수들...
	void DoPrepareGame(STARTINFO *pSC = NULL);
	void OnGameOver(GAMEOVERRESULT *pGOR);

	virtual void OnExitRoom(void);
	virtual void OnEnterRoom(BOOL bCreation);

	// #### [ 세븐 포커 ] ####
	void SetWindCardEnd(BOOL bFirst=FALSE);	
	
	// [ 베팅 관련 ]
	void Raise(POKERGAME *pMsg);

	// [ 칩 관련 ]	
	void chip_draw_func(int Pnum, INT64 Money, int sDelay=0, int Kind=0);


	// [ 카드 날리기 ]
	void EndWindCard();
	void Card_Distribute(int flycardnum = 1);
	void OnCardEvent(CARDEVENT *pEvent);
	void InitObserverStep(void);

	// [ 하이 족보 ]
	CString GetMyJokboName();
	CString GetJokboName(int nPNum);

	//규제안 : 맥스 플래그 추가 : 맥스이펙트
	void SoundFxKind(int betkind, int sex, bool bMax = false);		// ### [사운드 추가 작업] ###
	void SoundFxKindBig(int betkind, int sex, bool bMax = false);	// ### [사운드 추가 작업] ###	


	int GetWinnerValue(int pnum,int &nVal);

	int GetBigGame();				// 판이 큰지 아닌지 검사.

	int  GetTotalCardNum();

	void SetResultCardDeck(BOOL open,BOOL fold,BOOL jactpot , int pnum);
	void SetEnterRoomCardDeck(int nOpCnt , int pnum);


	// BOMB_GAME
	void SetADDBombCard();
	void SetOPenBombCard(char nCard);
	void OnOPenBombCard(int nCard);
	INT64 GetLastBetMoney(int PNum,int betCmd);


	// Chip 
	void OnSetMoveChip();

	void ReSetGameOverFlg();
	enum SEVEN_CARD GetHighRule(int pnum);


	void ResultFourCardInPut(int pnum);
	CString GetShortname(int eName);
	
	// 새로운 룰 적용을 위해 카드값을 담는다.
	void SetRuleCard();
	void GetResultCard(BOOL bopen,int winner);

	// 방 기능과 자기 옵션에서 비교 큰값을 가지고 옴
	int GetMaxAutoBettingCount();

	void OnCheckTurnLimitTime();
	void DebugAutobet();

	BOOL GetHiddenEffect();
	void SetJokerCard();

	void Draw_TextInformationMsg(); //2006.1.4 종료시 안내 메세지 그리는 부분
	void Insert_EndingTextInformation(CString str);

	//포카드 이상인 족보에서 조커카드가 포함되어 있는지 확인하여 전적에 포함되지 않는지를 알려준다.
	BOOL CheckComplateHaveJokerCard(int wpnum);

	void Change_Money(CSV_CHANGE_MONEY *pMsg);

	virtual CPlayer *GetPlayer(int i);
	CPlayer_NP	*GetPlayerNP(int i);
	void		CheckMinimumUser(void);

	INT64& GetJackPotMoney(){return m_i64JackPotMoney;}			// 잭팟금액
	void  SetJackPotMoney(INT64 i64Money);

	INT64& GetOldJackPotMoney(){return m_i64OldJackPotMoney;}	// 이전 잭팟금액
	void  SetOldJackPotMoney(INT64 i64Money);

	void JackPotMoneyUpdate( jackpot::JP_MONEYINFO* pInfo);
	void ClearJackpotMoney(int nGameCode, INT64 i64LimitMoney);

	//////////////////////////////////////////////////////////////////////////
	//히든관련
	POINT	GetHiddenClickPos(){return m_HiddenClickPos;}
	void	SetHiddenClickPos(POINT pt){m_HiddenClickPos = pt;}

	int		GetHiddenArrowPosX(){return m_nHinddenArrow_x;}
	void	SetHiddenArrowPosX(int nX){m_nHinddenArrow_x = nX;}

	int		GetHiddenArrowPosY(){return m_nHinddenArrow_y;}
	void	SetHiddenArrowPosY(int nY){m_nHinddenArrow_y = nY;}

	// 	BOOL	IsHiddenClick(){return m_bHiddenClick;}
	// 	void	SetHiddenClick(BOOL bFlag){m_bHiddenClick = bFlag;}

	void	SetMouseHiddenState( int nFlag ) { m_MouseHiddenState = nFlag; }
	int		GetMouseHiddenState() { return m_MouseHiddenState; }

	BOOL	IsMouseHiddenClick(){return m_bMouseHiddenClick;}
	void	SetMouseHiddenClick(BOOL bFlag){m_bMouseHiddenClick = bFlag;}

	BOOL	IskeyboardHiddenClick(){return m_bkeyboardHiddenClick;}
	void	SetkeyboardHiddenClick(BOOL bFlag){m_bkeyboardHiddenClick = bFlag;}

	void    SetHiddenSceneDrawTime() { m_dwHiddenSceneDrawTime = timeGetTime(); }
	DWORD   GetHiddenSceneDrawTime() { return m_dwHiddenSceneDrawTime; }

	void    SetCardOpenGuideTime(); 
	DWORD   Getm_dwCardOpenGuideTime() { return m_dwCardOpenGuideTime; }

	void	EndCardOpenGuide();////오픈애니메이션추가
	int     GetCardOpneGuideIndex();
	BOOL    GetCardOpneGuideDraw() { return m_bDrawCardOpenGuide; }
	//////////////////////////////////////////////////////////////////////////

};

#endif // !defined(AFX_GAMEPROCESS_GAMEPROCESS_NEWPOKER_H__87B4A196_AE81_4D0B_A34B_58AAD968CF9A__INCLUDED_)
