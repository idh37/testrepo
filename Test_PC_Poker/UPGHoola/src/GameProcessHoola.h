
// GameProcessHoola.h: interface for the CGameProcessHoola class.
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
#include "PlayerHoola.h"
#include "PlayScr.h"


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
	char ID[16];
	char PNum;
	INT64 nPMoney;	
} NENDINGUSERINFO;

//
typedef struct _tagNAFTERENDINGUSERINFO
{
	BOOL bChanged;
	NENDINGUSERINFO	EndUserInfo[MAX_PLAYER];		// 총 5명 유저

	void Clear()
	{
		bChanged = FALSE;
		for( int i = 0 ; i < MAX_PLAYER ; ++i )
		{
			ZeroMemory(&EndUserInfo[i],sizeof(NENDINGUSERINFO));
		}
	}
} NAFTERENDINGUSERINFO;


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

} GAMEENDING_MSG;


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

} WINNER_PNUMDATA;

struct STRUCT_BACKUP_SND_DATA
{
	int nSndFxKind;
	BOOL bSex;
};

enum eMUSIC_TYPE
{
	eMUSIC_NULL,
	eMUSIC_NORMAL,
	eMUSIC_GOLD
};

class CGameProcessHoola : public CGameProcess
{
public:
	// @ 훌라 변수
	CPlayScr m_cPlayScr;							//PlayScr클래스 사용
	STRUCT_BACKUP_SND_DATA m_nArrSndData[ MAX_PLAYER ];			//유저가 중간에 나갔을때 사용하기위해 사운드 데이터를 백업한다.
	// @
	
	// 게임 정보
	BOOL m_bGameStart;		// 게임이 시작되었는가?(패돌리기 포함)

	CGameProcessHoola(CGame *pGame);
	virtual ~CGameProcessHoola();
	
	eMUSIC_TYPE m_eMusicType;
// 	void SoundFxKind(int betkind, int sex);		// ### [사운드 추가 작업] ###
// 	void SoundFxKindBig(int betkind, int sex);	// ### [사운드 추가 작업] ###	

	void Insert_EndingTextInformation(CString str,COLORREF tcolor);
	void Draw_TextInformationMsg();
	INT64 GetMaxMoney(INT64 nTotal,int betkind );

	void Change_Money(CSV_CHANGE_MONEY *pMsg);  // G_TFT

public:
	// 가상 함수
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

	// 사용자 카드 정보 전체 초기화
	void ResetAllPlayerCard( int nResetData = 0 );
	
	// 이벤트
	void OnGameOver(GAMEOVERRESULT *pGOR);
	void OnCheckTurnLimitTimeBase();	
	void OnCheckTurnLimitTime();
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
	
	//훌라 playScr받는 코드
	CPlayScr* GetPlayScr() {return &m_cPlayScr;}
	void SetPlayerSndKindArray(); //게임오버 결과시 나오는 사용자 사운드 데이터가 날라가기 때문에 미리 백업해둔다.
	void GameOverMoneyProcess( GAMEOVERRESULT *pGOR );
	void SetStartMsgTitle(); //게임 시작시 전광판에 나오는 메시지를 세팅한다.
	void SetMissionCompleteMsgTitle(char* pID); //미션 성공시 나오는 메시지를 세팅한다
	void SetStopJackpotMsgTitle(jackpot::JP_WIN* pAlram); //스톱 잭팟 성공시 나오는 메시지를 세팅한다.
	void SetJacpotAnimationState(jackpot::JP_WIN* pAlram); //잭팟 애니 표현 부분
	void SetGoldStage(bool bGoldState, int nGoldStage);
	void SetResultDelay(DWORD dwTime = 1880);
	void StopResultDelay();
	BOOL GetResultDelay() { return m_bResultDelay; }
	void SetStartBtnDelay(int nType) {m_nStartBtnDelayType = nType;}
	int m_nStartBtnDelayType;
	
	//규제안 예약 기능으로 인하여 추가된 가상함수
	virtual void SendCallButton();
	virtual void SendCheckButton();
	virtual void SendDieButton();
	virtual void SendHalfButton();
	virtual void SendDdaButton();
	virtual void SendPingButton();
	
public:
	void StopMusic();
	
	void ResetFlg();
	virtual void ResetGame();

	virtual void OnTimer();
	virtual BOOL OnMouseMove(int x, int y);
	virtual BOOL OnLButtonDblclk(int x, int y);
	virtual BOOL OnLButtonUp(int x, int y);
	
	virtual CPlayer* GetPlayer(int i);
	CPlayerHoola* GetPlayerNP(int i);

	void CheckMinimumUser(void);				// 최소인원 체크 함수(2명 이상부터 게임 가능)
	
	INT64& GetJackPotMoney(){return m_i64JackPotMoney;}			// 잭팟금액
	void  SetJackPotMoney(INT64 i64Money){m_i64JackPotMoney = i64Money;}

	INT64& GetOldJackPotMoney(){return m_i64OldJackPotMoney;}	// 이전 잭팟금액
	void  SetOldJackPotMoney(INT64 i64Money){m_i64OldJackPotMoney = i64Money;}

	void JackPotMoneyUpdate(jackpot::JP_MONEYINFO* pInfo);
	void ClearJackpotMoney(int nGameCode, INT64 i64LimitMoney);


	//훌라 함수
	void DoPrepareGame(STARTINFO *pSI);
	void SetupByConfig();
	void PlayMusic();
	void MusicTest();

public:
	NMBASE::GRAPHICGDI::CPage*			m_pPage;
	

	// [ 베팅 딜레이 ]
	int				m_nActiveBetBtnDelayTime;

	// 게임 정보	
	BOOL			m_bPlayEnding;				// 진 엔딩 (딜레이 때문에 )
	DWORD			m_nPlayEndingDelayTime;		// 진 

	BOOL			m_bObserverChatWarning;				// 관전자 채팅 경고 

	GAMEENDING_MSG			m_GameEndMsgData;
	GAMEOVERRESULT			m_GameOverResultData;
	WINNER_PNUMDATA			m_WinnerPNumData;
	NAFTERENDINGUSERINFO	m_AfterEndingUserInfo;

	//훌라 변수
	BOOL					m_bForceReserveExitGame;

	void SendCL_ASK_CONTINUE( int rnum, int pnum, int unum, char* id );
	void SendCL_STOPGAME( int rnum, int pnum, int unum, char* id, int ntype );
	void SendCL_REGIST( int rnum, int pnum, int unum, char* id, REGISTERCARD* pRC );
	void SendCL_ASK_ONECARD( int rnum, int pnum, int unum, char* id );
	void SendCL_PRESS_THANK( int rnum, int pnum, int unum, char* id, char *pMyCard );
	void SendCL_DISCARD( int rnum, int pnum, int unum, char* id, int cardnum );
	void SendCL_ASK_THANK( int rnum, int pnum, int unum, char* id, CLTHANKCARD *pct );
	void SendCL_ENDCARDMOVE( int rnum, int pnum, int unum, char* id, char cEndFlag );
	void SendCL_WINDCARDEND( int rnum, int pnum, int unum, char* id );
	void SendCL_REQGAMESTART( int nRN, char *pID );
	void SendCL_REQGAMEPRACTICS( int nRN, char *pID );
	void SendCL_PRESS_GIVEUP();
	
	void ReviewRCDATA(REGISTERCARD* pRC); 

protected:
	CPlayerHoola			m_cPlayer[MAX_NEWPLAYER];		// 최대 플레이어를 미리 생성한다.(플레이어:5, 관전자:10)

private:
	bool m_bOnceInit;
	
	int		m_nResultDelayCount;
	BOOL	m_bResultDelayStart;

	INT64 m_i64JackPotMoney;						// 잭팟금액
	INT64 m_i64OldJackPotMoney;						// 이전 잭팟금액

	DWORD	m_bResultDelay;
	DWORD	m_dwResultDelaySt;
	DWORD	m_dwResultDelayEnd;
};

#endif // !defined(AFX_GAMEPROCESS7POKER_H__460B5831_D4A4_4D68_8557_9BBC6348FF27__INCLUDED_)
