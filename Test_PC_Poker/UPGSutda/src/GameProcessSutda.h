// GameProcessSutda.h: interface for the CGameProcessSutda class.
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
#include "PlayerSutda.h"


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
	char					ID[16];
	char					PNum;
	INT64					nPMoney;	
}NENDINGUSERINFO;


//
typedef struct _tagNAFTERENDINGUSERINFO
{
	BOOL	bChanged;
	NENDINGUSERINFO	EndUserInfo[MAX_PLAYER];		// �� 5�� ����

	void Clear()
	{
		bChanged = FALSE;
		for( int i = 0 ; i < MAX_PLAYER ; ++i )
		{
			ZeroMemory(&EndUserInfo[i],sizeof(NENDINGUSERINFO));
		}
	}
}NAFTERENDINGUSERINFO;


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

}GAMEENDING_MSG;


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

}WINNER_PNUMDATA;


class CGameProcessSutda : public CGameProcess
{
public:
	CGameProcessSutda(CGame *pGame);
	virtual ~CGameProcessSutda();
		
	void chip_draw_func(int PNum, INT64 Money);				// Ĩ�� �׸��� �Լ�
	int GetTotalCardNum();									// �ִ� ī�� ����
	INT64 GetLastBetMoney(int nPlayerNum,int betCmd);		// ������ ���� �ݾ�	

	INT64 GetRealBet();
	INT64 GetRaiseBat();
	INT64 GetTotalBet();
	void SetWindCardEnd(BOOL bFirst=FALSE);	

	void SoundFxKind(RAISE_LAST_CMD betkind, BOOL bSex, int nSndFxKind);		// ### [���� �߰� �۾�] ###
	void SoundFxKindBig(RAISE_LAST_CMD betkind, BOOL bSex, int nSndFxKind);	// ### [���� �߰� �۾�] ###	

	void Insert_EndingTextInformation(CString str,COLORREF tcolor);
	void Draw_TextInformationMsg();
	INT64 GetMaxMoney(INT64 nTotal,int betkind );

	void Change_Money(CSV_CHANGE_MONEY *pMsg);  // G_TFT
public:
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

	//void GameStart(char HCardIndex, SD_STARTINFO *pSC, SD_OPENCARD *pOpenCard);				// ���ӽ��۽� ȣ��
	void GameStart(SD_STARTINFO *pSC, bool bRematch);				// ���ӽ��۽� ȣ��

	void RaiseBase(POKERGAME *pMsg);
	void Raise(POKERGAME *pMsg);
	void OnGameOver(GAMEOVERRESULT *pGOR);

	void OnCheckTurnLimitTimeBase();	
	void OnCheckTurnLimitTime();	
	void EndWindCard();
	void Card_Distribute();								// ī�� �й�(���ӽ��� �� Raise���� ȣ��)
	void OnCardEvent(CARDEVENT *pEvent);
	void OnResultGameOver();
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

	void CheckMinimumUser(void);				// �ּ��ο� üũ �Լ�(2�� �̻���� ���� ����)

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

	void	EndCardOpenGuide();////���¾ִϸ��̼��߰�
	int     GetCardOpneGuideIndex();
	BOOL    GetCardOpneGuideDraw() { return m_bDrawCardOpenGuide; }
	
	CSevenRuleMan& GetSevenRuleManager(){ return m_cSevenRuleManager;}


	INT64& GetJackPotMoney(){return m_i64JackPotMoney;}			// ���̱ݾ�
	void  SetJackPotMoney(INT64 i64Money){m_i64JackPotMoney = i64Money;}

	INT64& GetOldJackPotMoney(){return m_i64OldJackPotMoney;}	// ���� ���̱ݾ�
	void  SetOldJackPotMoney(INT64 i64Money){m_i64OldJackPotMoney = i64Money;}

	void JackPotMoneyUpdate(jackpot::JP_MONEYINFO* pInfo);
	void ClearJackpotMoney(int nGameCode, INT64 i64LimitMoney);

private:
	void ResultInfoDisplay(int nWinPlayerNum);	// ���Ӱ���� ä��â�� ����Ѵ�.

	void TestCode(MSG* pMsg);	// �׽�Ʈ �ڵ�

public:

	NMBASE::GRAPHICGDI::CPage*			m_pPage;
	
	// [ ī�� �̺�Ʈ ������ ]
	BOOL			m_bWindCardEnd;
	int				m_nWindCardEndCnt;

	// [ ���� ������ ]
	int				m_nActiveBetBtnDelayTime;

	// ���� ����
	BOOL			m_bGameStart;				// ������ ���۵Ǿ��°�?(�е����� ����)
	
	BOOL			m_bPlayEnding;				// �� ���� (������ ������ )
	DWORD			m_nPlayEndingDelayTime;		// �� 

	int				m_BetCount;					// ���� ���� 4�� �ް� 1��° ���� -> 2��° ����	
	INT64			m_nDouble;					// ���簪 �̸� ����

	int				m_nBetCallMoneyClearCnt;	// ���ѱݾ׵� �����ֱ�

	BOOL			m_bObserverChatWarning;				// ������ ä�� ��� 

	GAMEENDING_MSG			m_GameEndMsgData;
	GAMEOVERRESULT			m_GameOverResultData;
	WINNER_PNUMDATA			m_WinnerPNumData;
	NAFTERENDINGUSERINFO	m_AfterEndingUserInfo;

	//bts ����. ���ڰ� ��� �������� ������ ������ ������. �׷��� ������ �����ϱ� ���� ����
	//2012.7.26 ���¹�
	CString m_strwinnerJokbo ;

	CAllBetChip				m_AllBetChip;

protected:
	CPlayerSutda			m_cPlayer[MAX_NEWPLAYER];		// �ִ� �÷��̾ �̸� �����Ѵ�.(�÷��̾�:5, ������:10)

private:
	bool m_bOnceInit;

	CSevenRuleMan			m_cSevenRuleManager;

	// ���� ī�� ����
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

	INT64 m_i64JackPotMoney;						// ���̱ݾ�
	INT64 m_i64OldJackPotMoney;						// ���� ���̱ݾ�


////////////////////���ٿ� //////////////////////

public:


	bool m_bRematch;
	int m_HighJokboPoint;	//���� ������ ���Ѵ�
	int m_nOrignalCard[CARD_TOTALCOUNT_BUF];

	int m_NowWinpnum;	//�̱���
	DWORD m_nNotifyResultCardOpenAnimationEndDelay;	//���� ������ ��  ī�� ���� ������ �����ٰ� �˷��ش�.(�����̰�)

	bool IsRematchGame(){ return m_bRematch; }	//�����ΰ�?
	void SetRematchGame(bool bRematch ) { m_bRematch = bRematch; }
	
	//ī�� ����(3�弸��)
	void OnAskChoiceCard(char JokboCard[]);
	

	void GetGenealogyPoint();
	int GetPairPoint(int card1, int card2);
	int GetAniPoint( int card1, int card2);	//�ִϸ��̼Ǻ� ����
	char* GetPairString(int point);
	int GetJokboIndex(int card1, int card2);
	int GetJokboGoodAndBad();
	
	void OnCompleteJJogiAnimation();

	void OnResultCard(SD_RESULTCARD *pResultCard);		//���Ӱ�� �ް� ī�� �����Ѵ�.
	int GetFirstPlayer();			//���� ����� 
	int GetLastPlayer();			//���� ���� �����

	BOOL ResultJokboExamine( int nPnum );		//���Ӱ���� ���������� �ִ��� �˻��Ѵ�.
	void CheckSortVoice(int nWinnerPNum);						//�Ѳ����� �º����� �˻��Ѵ�. 

	void NotifyResultCardOpenAnimationEnd(int nDelay);		//���� ������ ��  ī�� ���� ������ �����ٰ� �˷��ش�.

	void OnSamePair();

	void OnRematch_by_Draw(bool bWinnerJoin);		//�����п� ���� ����
	void OnRematch_by_49(bool bWinnerJoin, bool bReEntry);			//49�� ���� ����

	int GetRoundMaxCard();						//���� �ǿ��� �޾ƾ��� �ִ� ī�����(������:2��, 2�弸��:2��, 3�弸��:3��);

	void OnJoinRematchGame(const char* szID);		//������ ���⿡ �����ߴٰ� �˷���

	
	BOOL IsHiddenRound();		///������ ���� �����ΰ�?
	

};

#endif // !defined(AFX_GAMEPROCESS7POKER_H__460B5831_D4A4_4D68_8557_9BBC6348FF27__INCLUDED_)

