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


//������ ������ ���� ������ ��� �ִٰ� �����Ѵ�. ���� ������ ����
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
	int				TextMsgCnt;		//���� �ȳ� �޼��� ����
	CString			TextMsg[30];		//���� �ȳ� �޼��� ����
	COLORREF		TextColor[30];							//�÷�
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
	bool bOneWinnerFlag;		//TRUE �� 1�� FALSE 1���̻�
	int  nWinnerNumCount;			//�������
	char WinnerSPNum[MAX_PLAYER];		//�ǳ�

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
		bOneWinnerFlag = false;		//TRUE �� 1�� FALSE 1���̻�
		nWinnerNumCount = 0;			//�������
		memset(WinnerSPNum , -1, sizeof( WinnerSPNum ));
	}
}WINNER_PNUMDATA;

typedef struct  
{
	// ī��Ŀ��Ÿ�ӽ� ��� ������ ���ÿ� ��ư Ȱ��ȭ �ȴ�.
	bool bChangeStage;		// Ŀ������( Ŀ�� �ߴ°� )
	bool bChangeCardFlyEnd;	// ���� ���Ͽ��� ī�峯���� ����
	bool bMyTurn;			// ���� ����

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

//Ŀ�� ���� �˻翡 ���Ǵ� �迭�������� ��������:2012.10.29 YS.Jeong
typedef struct _ARRAY_UPCARD_DATA
{
	int  nCardIndex;	// ī�� �ε���
	bool bSelectFlag;	// flag
	_ARRAY_UPCARD_DATA()
	{
		nCardIndex = NULL;
		bSelectFlag = false;
	}
} ARRAY_UPCARD_DATA;

class GameProcess_BD    : public CGameProcess //UGPCommon �۾� �߰�
{
public:
	GameProcess_BD(CGame *pGame);
	virtual ~GameProcess_BD();

public:

	// [ ī�� �̺�Ʈ ������ ]
	bool			m_bWindCardEnd;
	int				m_nWindCardEndCnt;

	// [ ���� ������ ]
	int				m_nActiveBetBtnDelayTime;

	// ���� ����
	BOOL			m_bGameStart;		// ������ ���۵Ǿ��°�?(�е����� ����)
	BOOL			m_bBtnClick;		//���� ��ư�� Ŭ�� �ߴ���.	
	int				m_BetCount;			//���� ���� 4�� �ް� 1��° ���� -> 2��° ����	

	BOOL			m_bBlind;

	GAMEENDING_MSG			m_GameEndMsgData;
	GAMEOVERRESULT			m_stGameOverResultData;
	WINNER_PNUMDATA			m_stWinnerPNumData;
	NAFTERENDINGUSERINFO	m_stAfterEndingUserInfo;
	CAllBetChip				m_cAllBetChip;

	int				m_nBetCallMoneyClearCnt;	//�����ð��� ���ѱݾ׵� �����ֱ�

	//���� ī�� ����
	POINT	m_ptHiddenClickPos;
	bool	m_bHiddenClick;	

	INT64					m_i64JackPotActive;



	BOOL m_bObserverChatWarning;//������ ä�� ��� 



	//	CMsgBoxDlg		m_MsgBoxDlg;
	//-----------------------(�ٵ���) (�߰��ڵ�)-----------------------//	
	CPlayer_BD				m_cPlayer[ MAX_NEWPLAYER ];




	int  m_nPosIndex[ MAX_PLAYER ][ 5 ];//�ٲ� �ε���
	int  m_nCardIndex[ MAX_PLAYER ][ 5 ];//�ٲ� ī�� ����
	int  m_nChangedNum[ MAX_PLAYER ];	

	//-----------------------------------------------------------------------------------//
	void	set_GameSpeed(); //UPG CClientDlg���ִ��� ������

	bool	IsValid();
	void	chip_draw_func(int PNum, INT64 Money);
	void	DoExitSubscript(int bexit);
	int		GetTotalCardNum();
	INT64	GetLastBetMoney(int nPlayerNum,int betCmd);	

	INT64	GetRealBet();
	INT64	GetRaiseBat();
	INT64	GetTotalBet();
	void	SetWindCardEnd(BOOL bFirst=FALSE);	

	//������ : �ƽ� �÷��� �߰� : �ƽ�����Ʈ
	void	SoundFxKind(int betkind, int sex, bool bMax = false);		// ### [���� �߰� �۾�] ###
	void	SoundFxKindBig(int betkind, int sex, bool bMax = false);	// ### [���� �߰� �۾�] ###	
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


	// ���� ���� �Լ���...	
	virtual bool Accept_CreateRoom(void *pMsg);	// �游��� �㰡
	virtual bool Accept_EnterRoom(void *pMsg);		// �濡 ���� �㰡 ����
	virtual void User_EnterRoom(void *pMsg);			// ������ �濡 ���� �˸�
	virtual void User_OutRoom(void *pMsg);				// ������ �濡�� ���� �˸�
	virtual void SetPlayInfo(int nowpnum, int newpnum, BOOL bParticipation );
	virtual BOOL IsPlayingNow();

	virtual void DoPrepareGame(STARTINFO *pSC = NULL);
	virtual void Raise(POKERGAME *pMsg);
	bool		 Raise_InfoAndSound(INT64 llLastPlayerBetMoney); // ������� �� ���� ����� ����
	virtual void OnGameOver(GAMEOVERRESULT *pGOR);
	virtual void OnResultGameOver();
	virtual void OnResultGameOverInfo(); 

	virtual void OnCheckTurnLimitTime();	
	virtual void EndWindCard();
	virtual void Card_Distribute();

	virtual BOOL OnCommand( WPARAM wParam, LPARAM  lParam ); //< CYS 100705 >	
	virtual void FoldUserProcess( int nPNum );
	virtual CPlayer *GetPlayer(int i);
	//UPG�۾� �߰�
	virtual void OnEnterRoom(BOOL bCreation);
	virtual void OnExitRoom();

	BOOL	PreTranslateMessage(MSG* pMsg);	

	//������ ������ �߰��� ���� �����Լ� �߰�
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
	void AutoUp_Recommand( BOOL bClicked = FALSE );	// ī�� �ڵ��ø�
	void AutoSort_Recommand( int nPNum = 0, BOOL bClicked = FALSE );  // ī�� ��������
	void AutoPos_Recommand();	// ī�� Ŀ�ý� ��ġ����
	void CuttingNumEffectDraw( int nPNum ); // Ŀ�� ���� ����Ʈ
	void BlindBetEffectDraw();	// ����ε� ���� ����Ʈ
	void DellBlindBetEffect();	//����ε� ���� ����Ʈ ����

	bool GameKey_betting(int nVKey, int nExtend);				  //betting 
	bool GameKey_cutting(int nVKey, int nExtend, int repeat);     //cutting
	bool GameKey_changepass(int nVKey, int nExtend, int repeat);  //�ٲٱ�
	bool GameKey_HiddenOpen( int nVKey, int nExtend, int repeat );//�������

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

	void OnNotifyCardExChange( void *pValue  );	//�ٲٱ� �˸� ��ħ ���� ���� 
	void OnReceiveUserCutting( void *pValue );   //�ٲ��� ��� ��( ī�� �̵�ó�� )
	bool ReceiveUserCutting_Pass( int nPNum, int nNextTurnNum );					//�ٲ��� ��� ��( ī�� �н� )
	bool ReceiveUserCutting_Change( void* pValue, int nPNum, int nNextTurnNum ); //�ٲ��� ��� ��( ī�� ü���� )


	INT64& GetJackPotMoney(){return m_i64JackPotMoney;}			// ���̱ݾ�
	void  SetJackPotMoney(INT64 i64Money);//{m_i64JackPotMoney = i64Money;}

	INT64& GetOldJackPotMoney(){return m_i64OldJackPotMoney;}	// ���� ���̱ݾ�
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

	int					m_nBackUpPosIndex[ 5 ];  //�ٲ� �ε���
	int					m_nBackUpCardIndex[ 5 ]; //�ٲ� ī�� ����
	int					m_nBackUpChangedNum;	

	eCHMODE				m_eCHMODE;		  // change, pass
	bool				m_bReserveChange; // �ٲٱ� ����
	bool				m_bChangeState;   // �ٲٱ� ���� ����
	bool				m_bPassState;	  // �н�   ���� ����
	bool				m_bHaveHiddenCard;// ����ī�带 ������ �ִ�

	CUTTING_STAGE_INFO* m_pCuttingStageInfo; // Ŀ������

	int		m_nResultDelayCount;
	bool	m_bResultDelayStart;

	// Ŀ�ý� call -> Ŀ�û��� ���� ȣ�� ����
	int		m_nCutInfoDelaySound;
	bool	m_bCutInfoDelaySound;
	bool    m_bSex;

	std::vector< DWORD > m_vtExceptionKey_NotMsg;
	std::vector< DWORD > m_vtExceptionKey_Msg;


	INT64 m_i64JackPotMoney;						// ���̱ݾ�
	INT64 m_i64OldJackPotMoney;						// ���� ���̱ݾ�

	PLAYRESULTINFO_DATA m_stPlayRTInfo;//���� ���� ����ü


	//����ī�� ���ÿ��ο� ��ȣ�� ������ ����ü : 2012.10.29 YS.Jeong
	ARRAY_UPCARD_DATA	m_aryCardData[ TOTAL_CARD_BD ];

};

#endif // !defined(AFX_GAMEPROCESS_BASE_H__460B5831_D4A4_4D68_8557_9BBC6348FF27__INCLUDED_)
