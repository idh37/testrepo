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

#define MAX_CARDOPENGUIDE 5	//ī����¾˸� ����

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
	GAME_ROUND_READY = 0,			// �ƹ����µ� �ƴ�(�����)	0
		GAME_ROUND_START,				// 3���� ��						1
		GAME_ROUND_CARDCHOICE,			// 3���� ī�带 ������ 			2
		GAME_ROUND_4,					// 4���� ��						3
		GAME_ROUND_5,					// 5���� ��						4
		GAME_ROUND_6,					// 6���� ��						5
		GAME_ROUND_BOMB,				// 6�� ������ ��ź ī�� ����	6			
		GAME_ROUND_RESULT,				// ���� ���ڸ� ����			9	
};

class CGameProcess_NewPoker : public CGameProcess
{
public:
	CSevenRuleMan			m_cSvRule;		//���ο� �� 
	CBombCard				m_cBombCard;
	CResultFocus			m_cResultFocus;
	
	char					m_pJackpotWinnerID[16];

	CAllBetChip				m_cAllBetChip;
	// [ ī�� �̺�Ʈ ������ ]
	BOOL					m_bWindCardEnd;
	int						m_nWindCardEndCnt;
	
	// ���� ����
	BOOL					m_bGameStart;		// ������ ���۵Ǿ��°�?(�е����� ����)
	
	// [ ���� ������ ]
	int				m_nActiveBetBtnDelayTime;
	BOOL			m_bBettingDelayCheck ;  // 0�� �Է� �޾��� ���� �⺻���� �����̰� ���� ������ ��ƾ ó���� �ָ��ؼ� üũ������ ���� ����.

	// ������ ���� ���� ����, ��ź, ��Ŀ ī�尡 ����°�.. �� ���� ó���� �ʿ���.
	bool					m_bOpenBombcard;		// ��ź ī�� ���� �� ���� raise ���� 

	//�������
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


	// ��ź �����϶� �̻��� ���� ���ͼ� ���ӿ����� �Ǹ� ���ù�ư Ȱ��ȭ ���� �ʵ��� ����ó��.
	bool					m_bGameOver;

	//��Ŀ ī�� 
	DWORD					m_dwStartDrawJokerTime;
	DWORD					m_dwEndDrawJokerTime;
	
	CObjectList				m_cAniSprObjectMan;
	BOOL					m_bEventDrawFlg;	//��� Ÿ�� �Ǵ� ���Ĩ �̺�Ʈ 

	//2006.1.4 ����� �ȳ� �޼��� �׸��� �κ�
	int						m_nEndingInformationTextMsgCnt;		//���� �ȳ� �޼��� ����
	CString					m_listEndingInformationTextMsg[30];		//���� �ȳ� �޼��� ����

	CSelectCardNewPoker		m_cSelectCardMan;

	CPlayer_NP				*m_pPlayer;

protected:
	INT64 m_i64JackPotMoney;						// ���̱ݾ�
	INT64 m_i64OldJackPotMoney;						// ���� ���̱ݾ�

public:
	virtual void OnTimer();
	virtual void ResetGame();

	virtual BOOL OnMouseMove(int x, int y);
	virtual BOOL OnLButtonDblclk(int x, int y);
	virtual BOOL OnLButtonUp(int x, int y);
	virtual BOOL OnLButtonDown(int x, int y);

	virtual void OnGameKey(int nVKey, int nextended, int repeat = 0);

	//CSV_ACCEPT_CREATEROOM
	virtual bool Accept_CreateRoom(void *pMsgBase);	// �游��� �㰡
	virtual bool Accept_EnterRoom(void *pMsgBase);	// �濡 ���� �㰡 ����
	virtual void User_EnterRoom(void *pMsgBase);	// ������ �濡 ���� �˸�
	virtual void User_OutRoom(void *pMsgBase);		// ������ �濡�� ���� �˸�
	virtual void SetPlayInfo(int nowpnum, int newpnum, BOOL bParticipation );

	virtual BOOL IsPlayingNow();


	//������ ������ �߰��� ���� �����Լ� �߰�
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

	// �� ������ �ӵ� ���� ���� �Լ���.
	void set_GameSpeed(); //UPG CClientDlg���ִ��� ������
	void OnInit();

	// ���� ���� �Լ���...
	void DoPrepareGame(STARTINFO *pSC = NULL);
	void OnGameOver(GAMEOVERRESULT *pGOR);

	virtual void OnExitRoom(void);
	virtual void OnEnterRoom(BOOL bCreation);

	// #### [ ���� ��Ŀ ] ####
	void SetWindCardEnd(BOOL bFirst=FALSE);	
	
	// [ ���� ���� ]
	void Raise(POKERGAME *pMsg);

	// [ Ĩ ���� ]	
	void chip_draw_func(int Pnum, INT64 Money, int sDelay=0, int Kind=0);


	// [ ī�� ������ ]
	void EndWindCard();
	void Card_Distribute(int flycardnum = 1);
	void OnCardEvent(CARDEVENT *pEvent);
	void InitObserverStep(void);

	// [ ���� ���� ]
	CString GetMyJokboName();
	CString GetJokboName(int nPNum);

	//������ : �ƽ� �÷��� �߰� : �ƽ�����Ʈ
	void SoundFxKind(int betkind, int sex, bool bMax = false);		// ### [���� �߰� �۾�] ###
	void SoundFxKindBig(int betkind, int sex, bool bMax = false);	// ### [���� �߰� �۾�] ###	


	int GetWinnerValue(int pnum,int &nVal);

	int GetBigGame();				// ���� ū�� �ƴ��� �˻�.

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
	
	// ���ο� �� ������ ���� ī�尪�� ��´�.
	void SetRuleCard();
	void GetResultCard(BOOL bopen,int winner);

	// �� ��ɰ� �ڱ� �ɼǿ��� �� ū���� ������ ��
	int GetMaxAutoBettingCount();

	void OnCheckTurnLimitTime();
	void DebugAutobet();

	BOOL GetHiddenEffect();
	void SetJokerCard();

	void Draw_TextInformationMsg(); //2006.1.4 ����� �ȳ� �޼��� �׸��� �κ�
	void Insert_EndingTextInformation(CString str);

	//��ī�� �̻��� �������� ��Ŀī�尡 ���ԵǾ� �ִ��� Ȯ���Ͽ� ������ ���Ե��� �ʴ����� �˷��ش�.
	BOOL CheckComplateHaveJokerCard(int wpnum);

	void Change_Money(CSV_CHANGE_MONEY *pMsg);

	virtual CPlayer *GetPlayer(int i);
	CPlayer_NP	*GetPlayerNP(int i);
	void		CheckMinimumUser(void);

	INT64& GetJackPotMoney(){return m_i64JackPotMoney;}			// ���̱ݾ�
	void  SetJackPotMoney(INT64 i64Money);

	INT64& GetOldJackPotMoney(){return m_i64OldJackPotMoney;}	// ���� ���̱ݾ�
	void  SetOldJackPotMoney(INT64 i64Money);

	void JackPotMoneyUpdate( jackpot::JP_MONEYINFO* pInfo);
	void ClearJackpotMoney(int nGameCode, INT64 i64LimitMoney);

	//////////////////////////////////////////////////////////////////////////
	//�������
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

	void	EndCardOpenGuide();////���¾ִϸ��̼��߰�
	int     GetCardOpneGuideIndex();
	BOOL    GetCardOpneGuideDraw() { return m_bDrawCardOpenGuide; }
	//////////////////////////////////////////////////////////////////////////

};

#endif // !defined(AFX_GAMEPROCESS_GAMEPROCESS_NEWPOKER_H__87B4A196_AE81_4D0B_A34B_58AAD968CF9A__INCLUDED_)
