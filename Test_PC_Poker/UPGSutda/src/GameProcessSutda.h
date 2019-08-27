// GameProcessSutda.h: interface for the CGameProcessSutda class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEPROCESS7POKER_H__460B5831_D4A4_4D68_8557_9BBC6348FF27__INCLUDED_)
#define AFX_GAMEPROCESS7POKER_H__460B5831_D4A4_4D68_8557_9BBC6348FF27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 기능적인 처리를 담당하는 클래스

#include "UserStruct.h"
#include "CommMsgDef.h"
#include "PlayerSutda.h"


#define MAX_CARDOPENGUIDE 5	//카드오픈알림 종류

// 확장키 유무
enum EKEY_EXTEND
{
	KEY_NOTEXT = 0,
	KEY_EXT    = 1,
	KEY_EXTEND_ALL
};

//종료후 들어오는 유저 정보를 담고 있다고 변경한다. 종료 딜레이 때문
typedef struct _tagNENDINGUSERINFO
{
	char					ID[16];
	char					PNum;
	INT64					nPMoney;	
}NENDINGUSERINFO;


//
typedef struct _tagNAFTERENDINGUSERINFO
{
	BOOL	bChanged;
	NENDINGUSERINFO	EndUserInfo[MAX_PLAYER];		// 총 5명 유저

	void Clear()
	{
		bChanged = FALSE;
		for( int i = 0 ; i < MAX_PLAYER ; ++i )
		{
			ZeroMemory(&EndUserInfo[i],sizeof(NENDINGUSERINFO));
		}
	}
}NAFTERENDINGUSERINFO;


// 게임 종료 메시지
typedef struct _tagGAMEENDING_MSG
{
	int				TextMsgCnt;			// 종료 안내 메세지 개수
	CString			TextMsg[30];		// 종료 안내 메세지 내용
	COLORREF		TextColor[30];		// 컬러

	void Clear()
	{
		for( int i = 0 ; i < 30 ; i ++ )
		{
			TextMsg[i] = "";
			TextColor[i] = NULL;
		}
		TextMsgCnt = 0;
	}

}GAMEENDING_MSG;


// 플레이 데이터
typedef struct _tagWINNER_PNUMDATA
{
	BOOL bOneWinner;					// TRUE 면 1명 FALSE 1명이상
	int  nWinnerNum;					// 몇명인지
	char WinnerSPNum[MAX_PLAYER];		// 피넘

	char GetWinnerSPNum_ONE()
	{
		for( int i = 0 ; i < MAX_PLAYER ; ++i )
		{
			if( WinnerSPNum[i] > -1 )
			{
				return WinnerSPNum[i];
			}
		}

		return -1;
	}

	BOOL IsMyVictory(int spnum)
	{
		if( spnum < 0 || spnum >= MAX_PLAYER )
			return FALSE;

		for( int i = 0 ; i < MAX_PLAYER ; ++i )
		{
			if( WinnerSPNum[i] == spnum )
			{
				return TRUE;
			}
		}

		return FALSE;
	}

	void Clear()
	{
		bOneWinner = -1;		// TRUE 면 1명 FALSE 1명이상
		nWinnerNum = 0;			// 몇명인지
		memset(WinnerSPNum , -1, sizeof( WinnerSPNum ));
	}

}WINNER_PNUMDATA;


class CGameProcessSutda : public CGameProcess
{
public:
	CGameProcessSutda(CGame *pGame);
	virtual ~CGameProcessSutda();
		
	void chip_draw_func(int PNum, INT64 Money);				// 칩을 그리는 함수
	int GetTotalCardNum();									// 최대 카드 개수
	INT64 GetLastBetMoney(int nPlayerNum,int betCmd);		// 마지막 베팅 금액	

	INT64 GetRealBet();
	INT64 GetRaiseBat();
	INT64 GetTotalBet();
	void SetWindCardEnd(BOOL bFirst=FALSE);	

	void SoundFxKind(RAISE_LAST_CMD betkind, BOOL bSex, int nSndFxKind);		// ### [사운드 추가 작업] ###
	void SoundFxKindBig(RAISE_LAST_CMD betkind, BOOL bSex, int nSndFxKind);	// ### [사운드 추가 작업] ###	

	void Insert_EndingTextInformation(CString str,COLORREF tcolor);
	void Draw_TextInformationMsg();
	INT64 GetMaxMoney(INT64 nTotal,int betkind );

	void Change_Money(CSV_CHANGE_MONEY *pMsg);  // G_TFT
public:
	virtual void Clear();
	virtual void Init(NMBASE::GRAPHICGDI::CPage *ppage);
	virtual BOOL OnLButtonDown(int x, int y);
	virtual void OnGameKey(int nVKey, int nextended, int repeat);

	// 게임 진행 함수들...	
	virtual bool Accept_CreateRoom(void *pMsg);			// 방만들기 허가
	virtual bool Accept_EnterRoom(void *pMsg);			// 방에 입장 허가 받음
	virtual void User_EnterRoom(void *pMsg);			// 유저가 방에 입장 알림
	virtual void User_OutRoom(void *pMsg);				// 유저가 방에서 나감 알림
	virtual void SetPlayInfo(int nowpnum, int newpnum, BOOL bParticipation );
	virtual BOOL IsPlayingNow();

	//void GameStart(char HCardIndex, SD_STARTINFO *pSC, SD_OPENCARD *pOpenCard);				// 게임시작시 호출
	void GameStart(SD_STARTINFO *pSC, bool bRematch);				// 게임시작시 호출

	void RaiseBase(POKERGAME *pMsg);
	void Raise(POKERGAME *pMsg);
	void OnGameOver(GAMEOVERRESULT *pGOR);

	void OnCheckTurnLimitTimeBase();	
	void OnCheckTurnLimitTime();	
	void EndWindCard();
	void Card_Distribute();								// 카드 분배(게임시작 및 Raise에서 호출)
	void OnCardEvent(CARDEVENT *pEvent);
	void OnResultGameOver();
	BOOL OnCommand(WPARAM wParam, LPARAM  lParam);	

	void DieUser(int pnum){};

	void set_GameSpeed(); //UPG CClientDlg에있던걸 가져옴

	void OnInit();

	//UPG작업 추가
	virtual void OnEnterRoom(BOOL bCreation);
	virtual void OnExitRoom();

	//UPG작업 추가
	BOOL PreTranslateMessage(MSG* pMsg);	
	BOOL IsSacnCode(int nScanCode, int &scankey, int &extended, int &previous);


public:
	virtual void SendCallButton( bool bUserAction = true );
	virtual void SendCheckButton( bool bUserAction = true );
	virtual void SendDieButton( bool bUserAction = true );
	virtual void SendHalfButton( bool bUserAction = true );
	virtual void SendDdaButton( bool bUserAction = true );
	virtual void SendPingButton( bool bUserAction = true );
	
	virtual void SendCallButton();
	virtual void SendCheckButton();
	virtual void SendDieButton();
	virtual void SendHalfButton();
	virtual void SendDdaButton();
	virtual void SendPingButton();

	void ResetFlg();
	virtual void ResetGame();


	virtual void OnTimer();
	virtual BOOL OnMouseMove(int x, int y);
	virtual BOOL OnLButtonDblclk(int x, int y);
	virtual BOOL OnLButtonUp(int x, int y);

	virtual CPlayer* GetPlayer(int i);
	CPlayerSutda* GetPlayerNP(int i);

	void CheckMinimumUser(void);				// 최소인원 체크 함수(2명 이상부터 게임 가능)

	void	SetMouseHiddenState( int nFlag ) { m_MouseHiddenState = nFlag; }
	int		GetMouseHiddenState() { return m_MouseHiddenState; }

	POINT	GetHiddenClickPos(){return m_HiddenClickPos;}
	void	SetHiddenClickPos(POINT pt){m_HiddenClickPos = pt;}

	int		GetHiddenArrowPosX(){return m_nHinddenArrow_x;}
	void	SetHiddenArrowPosX(int nX){m_nHinddenArrow_x = nX;}

	int		GetHiddenArrowPosY(){return m_nHinddenArrow_y;}
	void	SetHiddenArrowPosY(int nY){m_nHinddenArrow_y = nY;}

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
	
	CSevenRuleMan& GetSevenRuleManager(){ return m_cSevenRuleManager;}


	INT64& GetJackPotMoney(){return m_i64JackPotMoney;}			// 잭팟금액
	void  SetJackPotMoney(INT64 i64Money){m_i64JackPotMoney = i64Money;}

	INT64& GetOldJackPotMoney(){return m_i64OldJackPotMoney;}	// 이전 잭팟금액
	void  SetOldJackPotMoney(INT64 i64Money){m_i64OldJackPotMoney = i64Money;}

	void JackPotMoneyUpdate(jackpot::JP_MONEYINFO* pInfo);
	void ClearJackpotMoney(int nGameCode, INT64 i64LimitMoney);

private:
	void ResultInfoDisplay(int nWinPlayerNum);	// 게임결과를 채팅창에 출력한다.

	void TestCode(MSG* pMsg);	// 테스트 코드

public:

	NMBASE::GRAPHICGDI::CPage*			m_pPage;
	
	// [ 카드 이벤트 에러시 ]
	BOOL			m_bWindCardEnd;
	int				m_nWindCardEndCnt;

	// [ 베팅 딜레이 ]
	int				m_nActiveBetBtnDelayTime;

	// 게임 정보
	BOOL			m_bGameStart;				// 게임이 시작되었는가?(패돌리기 포함)
	
	BOOL			m_bPlayEnding;				// 진 엔딩 (딜레이 때문에 )
	DWORD			m_nPlayEndingDelayTime;		// 진 

	int				m_BetCount;					// 베팅 정보 4장 받고 1번째 배팅 -> 2번째 배팅	
	INT64			m_nDouble;					// 따당값 미리 구함

	int				m_nBetCallMoneyClearCnt;	// 콜한금액들 지워주기

	BOOL			m_bObserverChatWarning;				// 관전자 채팅 경고 

	GAMEENDING_MSG			m_GameEndMsgData;
	GAMEOVERRESULT			m_GameOverResultData;
	WINNER_PNUMDATA			m_WinnerPNumData;
	NAFTERENDINGUSERINFO	m_AfterEndingUserInfo;

	//bts 수정. 승자가 결과 노출전에 나가면 족보가 비노출됨. 그래서 족보를 저장하기 위한 변수
	//2012.7.26 류승민
	CString m_strwinnerJokbo ;

	CAllBetChip				m_AllBetChip;

protected:
	CPlayerSutda			m_cPlayer[MAX_NEWPLAYER];		// 최대 플레이어를 미리 생성한다.(플레이어:5, 관전자:10)

private:
	bool m_bOnceInit;

	CSevenRuleMan			m_cSevenRuleManager;

	// 히든 카드 관련
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


	int		m_nResultDelayCount;
	BOOL	m_bResultDelayStart;

	INT64 m_i64JackPotMoney;						// 잭팟금액
	INT64 m_i64OldJackPotMoney;						// 이전 잭팟금액


////////////////////섯다용 //////////////////////

public:


	bool m_bRematch;
	int m_HighJokboPoint;	//높은 족보를 구한다
	int m_nOrignalCard[CARD_TOTALCOUNT_BUF];

	int m_NowWinpnum;	//이긴사람
	DWORD m_nNotifyResultCardOpenAnimationEndDelay;	//일정 딜레이 후  카드 오픈 연출이 끝났다고 알려준다.(딜레이값)

	bool IsRematchGame(){ return m_bRematch; }	//재경기인가?
	void SetRematchGame(bool bRematch ) { m_bRematch = bRematch; }
	
	//카드 선택(3장섯다)
	void OnAskChoiceCard(char JokboCard[]);
	

	void GetGenealogyPoint();
	int GetPairPoint(int card1, int card2);
	int GetAniPoint( int card1, int card2);	//애니메이션별 족보
	char* GetPairString(int point);
	int GetJokboIndex(int card1, int card2);
	int GetJokboGoodAndBad();
	
	void OnCompleteJJogiAnimation();

	void OnResultCard(SD_RESULTCARD *pResultCard);		//게임결과 받고 카드 오픈한다.
	int GetFirstPlayer();			//보스 사용자 
	int GetLastPlayer();			//보스 이전 사용자

	BOOL ResultJokboExamine( int nPnum );		//게임결과시 높은족보가 있는지 검사한다.
	void CheckSortVoice(int nWinnerPNum);						//한끗차이 승부인지 검사한다. 

	void NotifyResultCardOpenAnimationEnd(int nDelay);		//일정 딜레이 후  카드 오픈 연출이 끝났다고 알려준다.

	void OnSamePair();

	void OnRematch_by_Draw(bool bWinnerJoin);		//동률패에 의한 재경기
	void OnRematch_by_49(bool bWinnerJoin, bool bReEntry);			//49에 의한 재경기

	int GetRoundMaxCard();						//현재 판에서 받아야할 최대 카드장수(재대결판:2장, 2장섯다:2장, 3장섯다:3장);

	void OnJoinRematchGame(const char* szID);		//유저가 재경기에 참가했다고 알려줌

	
	BOOL IsHiddenRound();		///마지막 히든 라운드인가?
	

};

#endif // !defined(AFX_GAMEPROCESS7POKER_H__460B5831_D4A4_4D68_8557_9BBC6348FF27__INCLUDED_)

