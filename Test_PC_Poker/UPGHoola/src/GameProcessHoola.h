
// GameProcessHoola.h: interface for the CGameProcessHoola class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEPROCESS7POKER_H__460B5831_D4A4_4D68_8557_9BBC6348FF27__INCLUDED_)
#define AFX_GAMEPROCESS7POKER_H__460B5831_D4A4_4D68_8557_9BBC6348FF27__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ������� ó���� ����ϴ� Ŭ����

#include "UserStruct.h"
#include "CommMsgDef.h"
#include "PlayerHoola.h"
#include "PlayScr.h"


#define MAX_CARDOPENGUIDE 5	//ī����¾˸� ����

// Ȯ��Ű ����
enum EKEY_EXTEND
{
	KEY_NOTEXT = 0,
	KEY_EXT    = 1,
	KEY_EXTEND_ALL
};

//������ ������ ���� ������ ��� �ִٰ� �����Ѵ�. ���� ������ ����
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
	NENDINGUSERINFO	EndUserInfo[MAX_PLAYER];		// �� 5�� ����

	void Clear()
	{
		bChanged = FALSE;
		for( int i = 0 ; i < MAX_PLAYER ; ++i )
		{
			ZeroMemory(&EndUserInfo[i],sizeof(NENDINGUSERINFO));
		}
	}
} NAFTERENDINGUSERINFO;


// ���� ���� �޽���
typedef struct _tagGAMEENDING_MSG
{
	int				TextMsgCnt;			// ���� �ȳ� �޼��� ����
	CString			TextMsg[30];		// ���� �ȳ� �޼��� ����
	COLORREF		TextColor[30];		// �÷�

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


// �÷��� ������
typedef struct _tagWINNER_PNUMDATA
{
	BOOL bOneWinner;					// TRUE �� 1�� FALSE 1���̻�
	int  nWinnerNum;					// �������
	char WinnerSPNum[MAX_PLAYER];		// �ǳ�

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
		bOneWinner = -1;		// TRUE �� 1�� FALSE 1���̻�
		nWinnerNum = 0;			// �������
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
	// @ �Ƕ� ����
	CPlayScr m_cPlayScr;							//PlayScrŬ���� ���
	STRUCT_BACKUP_SND_DATA m_nArrSndData[ MAX_PLAYER ];			//������ �߰��� �������� ����ϱ����� ���� �����͸� ����Ѵ�.
	// @
	
	// ���� ����
	BOOL m_bGameStart;		// ������ ���۵Ǿ��°�?(�е����� ����)

	CGameProcessHoola(CGame *pGame);
	virtual ~CGameProcessHoola();
	
	eMUSIC_TYPE m_eMusicType;
// 	void SoundFxKind(int betkind, int sex);		// ### [���� �߰� �۾�] ###
// 	void SoundFxKindBig(int betkind, int sex);	// ### [���� �߰� �۾�] ###	

	void Insert_EndingTextInformation(CString str,COLORREF tcolor);
	void Draw_TextInformationMsg();
	INT64 GetMaxMoney(INT64 nTotal,int betkind );

	void Change_Money(CSV_CHANGE_MONEY *pMsg);  // G_TFT

public:
	// ���� �Լ�
	virtual void Clear();
	virtual void Init(NMBASE::GRAPHICGDI::CPage *ppage);
	virtual BOOL OnLButtonDown(int x, int y);
	virtual void OnGameKey(int nVKey, int nextended, int repeat);

	// ���� ���� �Լ���...	
	virtual bool Accept_CreateRoom(void *pMsg);			// �游��� �㰡
	virtual bool Accept_EnterRoom(void *pMsg);			// �濡 ���� �㰡 ����
	virtual void User_EnterRoom(void *pMsg);			// ������ �濡 ���� �˸�
	virtual void User_OutRoom(void *pMsg);				// ������ �濡�� ���� �˸�
	virtual void SetPlayInfo(int nowpnum, int newpnum, BOOL bParticipation );
	virtual BOOL IsPlayingNow();

	// ����� ī�� ���� ��ü �ʱ�ȭ
	void ResetAllPlayerCard( int nResetData = 0 );
	
	// �̺�Ʈ
	void OnGameOver(GAMEOVERRESULT *pGOR);
	void OnCheckTurnLimitTimeBase();	
	void OnCheckTurnLimitTime();
	BOOL OnCommand(WPARAM wParam, LPARAM  lParam);	

	void DieUser(int pnum){};
	void set_GameSpeed(); //UPG CClientDlg���ִ��� ������
	void OnInit();

	//UPG�۾� �߰�
	virtual void OnEnterRoom(BOOL bCreation);
	virtual void OnExitRoom();

	//UPG�۾� �߰�
	BOOL PreTranslateMessage(MSG* pMsg);	
	BOOL IsSacnCode(int nScanCode, int &scankey, int &extended, int &previous);
	
	//�Ƕ� playScr�޴� �ڵ�
	CPlayScr* GetPlayScr() {return &m_cPlayScr;}
	void SetPlayerSndKindArray(); //���ӿ��� ����� ������ ����� ���� �����Ͱ� ���󰡱� ������ �̸� ����صд�.
	void GameOverMoneyProcess( GAMEOVERRESULT *pGOR );
	void SetStartMsgTitle(); //���� ���۽� �����ǿ� ������ �޽����� �����Ѵ�.
	void SetMissionCompleteMsgTitle(char* pID); //�̼� ������ ������ �޽����� �����Ѵ�
	void SetStopJackpotMsgTitle(jackpot::JP_WIN* pAlram); //���� ���� ������ ������ �޽����� �����Ѵ�.
	void SetJacpotAnimationState(jackpot::JP_WIN* pAlram); //���� �ִ� ǥ�� �κ�
	void SetGoldStage(bool bGoldState, int nGoldStage);
	void SetResultDelay(DWORD dwTime = 1880);
	void StopResultDelay();
	BOOL GetResultDelay() { return m_bResultDelay; }
	void SetStartBtnDelay(int nType) {m_nStartBtnDelayType = nType;}
	int m_nStartBtnDelayType;
	
	//������ ���� ������� ���Ͽ� �߰��� �����Լ�
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

	void CheckMinimumUser(void);				// �ּ��ο� üũ �Լ�(2�� �̻���� ���� ����)
	
	INT64& GetJackPotMoney(){return m_i64JackPotMoney;}			// ���̱ݾ�
	void  SetJackPotMoney(INT64 i64Money){m_i64JackPotMoney = i64Money;}

	INT64& GetOldJackPotMoney(){return m_i64OldJackPotMoney;}	// ���� ���̱ݾ�
	void  SetOldJackPotMoney(INT64 i64Money){m_i64OldJackPotMoney = i64Money;}

	void JackPotMoneyUpdate(jackpot::JP_MONEYINFO* pInfo);
	void ClearJackpotMoney(int nGameCode, INT64 i64LimitMoney);


	//�Ƕ� �Լ�
	void DoPrepareGame(STARTINFO *pSI);
	void SetupByConfig();
	void PlayMusic();
	void MusicTest();

public:
	NMBASE::GRAPHICGDI::CPage*			m_pPage;
	

	// [ ���� ������ ]
	int				m_nActiveBetBtnDelayTime;

	// ���� ����	
	BOOL			m_bPlayEnding;				// �� ���� (������ ������ )
	DWORD			m_nPlayEndingDelayTime;		// �� 

	BOOL			m_bObserverChatWarning;				// ������ ä�� ��� 

	GAMEENDING_MSG			m_GameEndMsgData;
	GAMEOVERRESULT			m_GameOverResultData;
	WINNER_PNUMDATA			m_WinnerPNumData;
	NAFTERENDINGUSERINFO	m_AfterEndingUserInfo;

	//�Ƕ� ����
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
	CPlayerHoola			m_cPlayer[MAX_NEWPLAYER];		// �ִ� �÷��̾ �̸� �����Ѵ�.(�÷��̾�:5, ������:10)

private:
	bool m_bOnceInit;
	
	int		m_nResultDelayCount;
	BOOL	m_bResultDelayStart;

	INT64 m_i64JackPotMoney;						// ���̱ݾ�
	INT64 m_i64OldJackPotMoney;						// ���� ���̱ݾ�

	DWORD	m_bResultDelay;
	DWORD	m_dwResultDelaySt;
	DWORD	m_dwResultDelayEnd;
};

#endif // !defined(AFX_GAMEPROCESS7POKER_H__460B5831_D4A4_4D68_8557_9BBC6348FF27__INCLUDED_)
