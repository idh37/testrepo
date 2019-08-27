// GameProcess_Base.h: interface for the GameProcess_BD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEPROCESS_BASE_H__460B5831_D4A4_4D68_8557_9BBC6348FF27__INCLUDED_)
#define AFX_GAMEPROCESS_BASE_H__460B5831_D4A4_4D68_8557_9BBC6348FF27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserStruct.h"
#include "CommMsgDef.h"
#include "NMovingCardMan.h"
#include "ChattingArea/ChatWnd_PlayResultInfo.h"


//종료후 들어오는 유저 정보를 담고 있다고 변경한다. 종료 딜레이 때문
typedef struct  
{
	char					ID[16];
	char					PNum;
	INT64					nPMoney;	
} NENDINGUSERINFO;

typedef struct  
{
	BOOL	bChanged;
	NENDINGUSERINFO	EndUserInfo[5];

	void Clear()
	{
		bChanged = FALSE;
		for( int i = 0 ; i < 5 ; i ++ )
		{
			ZeroMemory(&EndUserInfo[i],sizeof(NENDINGUSERINFO));
		}
	}
}NAFTERENDINGUSERINFO;

typedef struct  
{
	int				TextMsgCnt;		//종료 안내 메세지 개수
	CString			TextMsg[30];		//종료 안내 메세지 내용
	COLORREF		TextColor[30];							//컬러
	void Clear()
	{
		for ( int i = 0 ; i < 30 ; i ++ )
		{
			TextMsg[i] = "";
			TextColor[i] = NULL;
		}
		TextMsgCnt = 0;
	}
}GAMEENDING_MSG;


typedef struct  
{
	bool bOneWinnerFlag;		//TRUE 면 1명 FALSE 1명이상
	int  nWinnerNumCount;			//몇명인지
	char WinnerSPNum[MAX_PLAYER];		//피넘

	char GetWinnerSPNum_ONE()
	{
		for( int i = 0 ; i < MAX_PLAYER ; i ++ )
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
		if( spnum < 0 || spnum >= 5 )return FALSE;
		for( int i = 0 ; i < MAX_PLAYER ; i ++ )
		{
			if( WinnerSPNum[i] == spnum ){
				return TRUE;
			}
		}

		return FALSE;
	}

	void Clear()
	{
		bOneWinnerFlag = false;		//TRUE 면 1명 FALSE 1명이상
		nWinnerNumCount = 0;			//몇명인지
		memset(WinnerSPNum , -1, sizeof( WinnerSPNum ));
	}
}WINNER_PNUMDATA;

typedef struct  
{
	// 카드커팅타임시 모든 유저가 동시에 버튼 활성화 된다.
	bool bChangeStage;		// 커팅유무( 커팅 했는가 )
	bool bChangeCardFlyEnd;	// 실제 내턴에서 카드날리는 순간
	bool bMyTurn;			// 실제 내턴

	void Clear()
	{
		bChangeStage = false;	
		bChangeCardFlyEnd = false;
		bMyTurn = false;
	}

	void SetCuttingTimeFlag()
	{
		bChangeStage = true;	
		bChangeCardFlyEnd = true;
	}
} CUTTING_STAGE_INFO;

//커팅 조건 검사에 사용되는 배열데이터의 원소정의:2012.10.29 YS.Jeong
typedef struct _ARRAY_UPCARD_DATA
{
	int  nCardIndex;	// 카드 인덱스
	bool bSelectFlag;	// flag
	_ARRAY_UPCARD_DATA()
	{
		nCardIndex = NULL;
		bSelectFlag = false;
	}
} ARRAY_UPCARD_DATA;

class GameProcess_BD    : public CGameProcess //UGPCommon 작업 추가
{
public:
	GameProcess_BD(CGame *pGame);
	virtual ~GameProcess_BD();

public:

	// [ 카드 이벤트 에러시 ]
	bool			m_bWindCardEnd;
	int				m_nWindCardEndCnt;

	// [ 베팅 딜레이 ]
	int				m_nActiveBetBtnDelayTime;

	// 게임 정보
	BOOL			m_bGameStart;		// 게임이 시작되었는가?(패돌리기 포함)
	BOOL			m_bBtnClick;		//배팅 버튼을 클릭 했는지.	
	int				m_BetCount;			//베팅 정보 4장 받고 1번째 배팅 -> 2번째 배팅	

	BOOL			m_bBlind;

	GAMEENDING_MSG			m_GameEndMsgData;
	GAMEOVERRESULT			m_stGameOverResultData;
	WINNER_PNUMDATA			m_stWinnerPNumData;
	NAFTERENDINGUSERINFO	m_stAfterEndingUserInfo;
	CAllBetChip				m_cAllBetChip;

	int				m_nBetCallMoneyClearCnt;	//일정시간후 콜한금액들 지워주기

	//히든 카드 관련
	POINT	m_ptHiddenClickPos;
	bool	m_bHiddenClick;	

	INT64					m_i64JackPotActive;



	BOOL m_bObserverChatWarning;//관전자 채팅 경고 



	//	CMsgBoxDlg		m_MsgBoxDlg;
	//-----------------------(바둑이) (추가코드)-----------------------//	
	CPlayer_BD				m_cPlayer[ MAX_NEWPLAYER ];




	int  m_nPosIndex[ MAX_PLAYER ][ 5 ];//바뀔 인덱스
	int  m_nCardIndex[ MAX_PLAYER ][ 5 ];//바뀔 카드 숫자
	int  m_nChangedNum[ MAX_PLAYER ];	

	//-----------------------------------------------------------------------------------//
	void	set_GameSpeed(); //UPG CClientDlg에있던걸 가져옴

	bool	IsValid();
	void	chip_draw_func(int PNum, INT64 Money);
	void	DoExitSubscript(int bexit);
	int		GetTotalCardNum();
	INT64	GetLastBetMoney(int nPlayerNum,int betCmd);	

	INT64	GetRealBet();
	INT64	GetRaiseBat();
	INT64	GetTotalBet();
	void	SetWindCardEnd(BOOL bFirst=FALSE);	

	//규제안 : 맥스 플래그 추가 : 맥스이펙트
	void	SoundFxKind(int betkind, int sex, bool bMax = false);		// ### [사운드 추가 작업] ###
	void	SoundFxKindBig(int betkind, int sex, bool bMax = false);	// ### [사운드 추가 작업] ###	
	void	SoundFXCutting( int nType, int nSex );

	void	Insert_EndingTextInformation(CString str,COLORREF tcolor);
	void	Draw_TextInformationMsg();


	void	OnBtnCall( bool bUserAction = true );
	void	OnBtnCheck( bool bUserAction = true );
	void	OnBtnFold( bool bUserAction = true );
	void	OnBtnMax( bool bUserAction = true );
	void	OnBtnDda( bool bUserAction = true );
	void	OnBtnPing( bool bUserAction = true );

	void	OnBtnChange();
	void	OnBtnPass();

	void	Change_Money(CSV_CHANGE_MONEY *pMsg); //G_TFT
	bool    IsSacnCode(int nScanCode, int &scankey, int &extended, int &previous);

	void	ShowCard(char* nCard);

	virtual void Clear();
	virtual void Init(NMBASE::GRAPHICGDI::CPage *ppage);
	virtual void OnTimer();
	virtual void ResetFlg();
	virtual void ResetGame();
	virtual BOOL OnMouseMove(int x, int y);
	virtual BOOL OnLButtonDblclk(int x, int y);
	virtual BOOL OnLButtonUp(int x, int y);
	virtual BOOL OnLButtonDown(int x, int y);
	virtual void OnGameKey(int nVKey, int nextended, int repeat = 0);


	// 게임 진행 함수들...	
	virtual bool Accept_CreateRoom(void *pMsg);	// 방만들기 허가
	virtual bool Accept_EnterRoom(void *pMsg);		// 방에 입장 허가 받음
	virtual void User_EnterRoom(void *pMsg);			// 유저가 방에 입장 알림
	virtual void User_OutRoom(void *pMsg);				// 유저가 방에서 나감 알림
	virtual void SetPlayInfo(int nowpnum, int newpnum, BOOL bParticipation );
	virtual BOOL IsPlayingNow();

	virtual void DoPrepareGame(STARTINFO *pSC = NULL);
	virtual void Raise(POKERGAME *pMsg);
	bool		 Raise_InfoAndSound(INT64 llLastPlayerBetMoney); // 레이즈시 전 유저 사운드및 정보
	virtual void OnGameOver(GAMEOVERRESULT *pGOR);
	virtual void OnResultGameOver();
	virtual void OnResultGameOverInfo(); 

	virtual void OnCheckTurnLimitTime();	
	virtual void EndWindCard();
	virtual void Card_Distribute();

	virtual BOOL OnCommand( WPARAM wParam, LPARAM  lParam ); //< CYS 100705 >	
	virtual void FoldUserProcess( int nPNum );
	virtual CPlayer *GetPlayer(int i);
	//UPG작업 추가
	virtual void OnEnterRoom(BOOL bCreation);
	virtual void OnExitRoom();

	BOOL	PreTranslateMessage(MSG* pMsg);	

	//규제안 예약기능 추가로 인한 가상함수 추가
	virtual void SendCallButton();
	virtual void SendCheckButton();
	virtual void SendDieButton();
	virtual void SendHalfButton();
	virtual void SendDdaButton();
	virtual void SendPingButton();

	void SetCardExchangeLog(CSV_BD_NOTIFY_CARDEXCHANGE msg);
	void SetCardExchangeLog(CSV_BD_GIVETOUSER_NEWCHANGECARD msg);
	void SetCardExchangeLog(CCL_BD_CARD_EXCHANGE msg);
	void SetCardExchangeLog(CCL_BD_CARD_EXCHANGEEND msg);
	void SetCardExchangeLog(CSV_RAISE msg);
	void SetCardExchangeLog(CCL_ASK_RAISE msg);

public:
	enum eCHMODE 
	{
		E_PASS = 0,
		E_CHANGE,
	};

	CNMovingCardMan* GetMovingCardMgr()
	{
		TPASSERT( NULL != m_pMovingCardMgr );
		return m_pMovingCardMgr;
	}

	bool OnPrePress();
	void ClearCuttingBackUpCard();
	void AutoUp_Recommand( BOOL bClicked = FALSE );	// 카드 자동올림
	void AutoSort_Recommand( int nPNum = 0, BOOL bClicked = FALSE );  // 카드 숫자정렬
	void AutoPos_Recommand();	// 카드 커팅시 위치정렬
	void CuttingNumEffectDraw( int nPNum ); // 커팅 숫자 이펙트
	void BlindBetEffectDraw();	// 블라인드 베팅 이펙트
	void DellBlindBetEffect();	//블라인드 베팅 이펙트 제거

	bool GameKey_betting(int nVKey, int nExtend);				  //betting 
	bool GameKey_cutting(int nVKey, int nExtend, int repeat);     //cutting
	bool GameKey_changepass(int nVKey, int nExtend, int repeat);  //바꾸기
	bool GameKey_HiddenOpen( int nVKey, int nExtend, int repeat );//히든오픈

	CObserverDlg* GetObserverDlgProcess()
	{
		TPASSERT( NULL != m_pObserverDlg );
		return m_pObserverDlg;
	}

	void SetChangeState( bool bChange )
	{
		m_bChangeState = bChange;
	}

	void SetPassState( bool bPass )
	{
		m_bPassState = bPass;
	}

	void CheckMinimumUser(void);

	CUTTING_STAGE_INFO* GetCuttingStageInfo() const
	{
		TPASSERT( m_pCuttingStageInfo != NULL );
		return m_pCuttingStageInfo;
	}

	void Initialize_ExceptionKey();

	void SetEnableCuttingBtn( bool bEnable );

	void OnNotifyCardExChange( void *pValue  );	//바꾸기 알림 아침 점심 저녁 
	void OnReceiveUserCutting( void *pValue );   //바꾼후 결과 값( 카드 이동처리 )
	bool ReceiveUserCutting_Pass( int nPNum, int nNextTurnNum );					//바꾼후 결과 값( 카드 패스 )
	bool ReceiveUserCutting_Change( void* pValue, int nPNum, int nNextTurnNum ); //바꾼후 결과 값( 카드 체인지 )


	INT64& GetJackPotMoney(){return m_i64JackPotMoney;}			// 잭팟금액
	void  SetJackPotMoney(INT64 i64Money);//{m_i64JackPotMoney = i64Money;}

	INT64& GetOldJackPotMoney(){return m_i64OldJackPotMoney;}	// 이전 잭팟금액
	void  SetOldJackPotMoney(INT64 i64Money);//{m_i64OldJackPotMoney = i64Money;}

	void JackPotMoneyUpdate( jackpot::JP_MONEYINFO* pInfo );
	void ClearJackpotMoney(int nGameCode, INT64 i64LimitMoney);

	void SetHiddenCardFlag( bool bHidden )
	{
		m_bHaveHiddenCard = bHidden;
	}

	bool IsHiddenCardFlag()
	{
		return m_bHaveHiddenCard;
	}

protected:
	CObserverDlg*		m_pObserverDlg;
	CNMovingCardMan*	m_pMovingCardMgr;
	int					m_nTotalCardNum;	

	int					m_nBackUpPosIndex[ 5 ];  //바뀔 인덱스
	int					m_nBackUpCardIndex[ 5 ]; //바뀔 카드 숫자
	int					m_nBackUpChangedNum;	

	eCHMODE				m_eCHMODE;		  // change, pass
	bool				m_bReserveChange; // 바꾸기 예약
	bool				m_bChangeState;   // 바꾸기 가능 상태
	bool				m_bPassState;	  // 패스   가능 상태
	bool				m_bHaveHiddenCard;// 히든카드를 가지고 있다

	CUTTING_STAGE_INFO* m_pCuttingStageInfo; // 커팅정보

	int		m_nResultDelayCount;
	bool	m_bResultDelayStart;

	// 커팅시 call -> 커팅사운드 연속 호출 지연
	int		m_nCutInfoDelaySound;
	bool	m_bCutInfoDelaySound;
	bool    m_bSex;

	std::vector< DWORD > m_vtExceptionKey_NotMsg;
	std::vector< DWORD > m_vtExceptionKey_Msg;


	INT64 m_i64JackPotMoney;						// 잭팟금액
	INT64 m_i64OldJackPotMoney;						// 이전 잭팟금액

	PLAYRESULTINFO_DATA m_stPlayRTInfo;//게임 정보 구조체


	//버릴카드 선택여부와 번호를 저장할 구조체 : 2012.10.29 YS.Jeong
	ARRAY_UPCARD_DATA	m_aryCardData[ TOTAL_CARD_BD ];

};

#endif // !defined(AFX_GAMEPROCESS_BASE_H__460B5831_D4A4_4D68_8557_9BBC6348FF27__INCLUDED_)
