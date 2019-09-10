#pragma once

class CGameProcess_NewPoker;
class CGameViewNewPoker;

class UPG_GAME_EXPORT CGameNewPoker : public CGame
{
public:
	CGameNewPoker();
	virtual ~CGameNewPoker(void);

protected:
	virtual BOOL Create(CWnd *pParentWnd);

	virtual void LoadSprite();
	virtual bool ReLoadSprite(int nSkinKind, bool bFirstLoad = false);//��Ų�� �����ϴ� ���Ӹ� ����
	virtual void LoadSound();

	virtual void ReleaseSprite();
	virtual void ReleaseSound();

	virtual CPlayer *CreateNewPlayer(void);

	CGameProcess_NewPoker*	GetGameProcess();
	CGameViewNewPoker*		GetGameView();		// ���� �� ������

	virtual const IDX_GAME	GetGameType(void);
	virtual const int		GetMaxRoom(void);
	virtual const int		GetMaxPlayer(void);
	virtual const int		GetMaxGamePlayer(void);
	virtual const int		GetMaxObserver(void);
	virtual const int		GetJokboQuestCount(void);
	virtual const CString	GetGameString(void);
	virtual const int		GetGameCode(void);
	virtual const CString	GetGameName(void);
	virtual const COLORREF	GetGameColor(void);
	virtual const CString	GetLoadingBackPath(void);
	virtual const int		GetDefaultFormKind(void);
	virtual const HICON		GetGameIcon(void);
	virtual const HICON		GetChatIcon(void);
	virtual const CString	GetModuleName(void);
public:
	virtual LONG OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData);
	virtual LRESULT ShowCreateRooomDlg(ROOMINFO *pRoomInfo, ROOMOPTION *pWebRoomOption=NULL);
	virtual LRESULT	ShowQuickJoinerDlg(void);
	virtual LRESULT	SendQuickJoiner(void);
	virtual void CheckDirectEnterRoomInfo(int nType, ROOMINFO *pRoomInfo);
	virtual int GetDirectEnterRoomType(int nType);
	virtual void AddGameChat(const char *str, COLORREF crNewColor, bool bUserChat);
	virtual void AddGameInfo(const char *str, COLORREF crNewColor, bool bUserChat);
	virtual void OnChangeFullScreen(bool bFullScreen) ;
	virtual void OnCloseOptionDlg(LRESULT nRet);								//�ɼ�â�� ������ ȣ���Ǵ� �Լ�
	virtual	void OnLoadingEnd(void);
	virtual void OnFocusGame(bool bFocus);
	virtual CString GetLobbyEnterSound(bool bMan);

	//=========================================================================================
	//��Ŷ ó�� ���� �Լ���
	virtual LONG OnReceiveResGameContinue(int TotSize, char *lpData);

	LONG OnReceiveUserInfo(int TotSize, char *lpData);					//���� ����â�� ���ӹ�/���� �����ϰ� �����ʿ� �������� ������ �����ʿ��� ����.
	LONG OnReceiveStartButtonEnable(int TotSize, char *lpData);			//���۹�ư Ȱ��ȭ
	LONG OnReceiveObserverMessege(int TotSize, char *lpData);			//���� ,���� ����
	LONG OnReceiveReservationInfo(int TotSize, char *lpData);			//������ ����
	LONG OnReceiveAcceptEmerage(int TotSize, char *lpData);				//����->����
	LONG OnReceiveAcceptParticipation(int TotSize, char *lpData);		//����->����
	LONG OnReceiveEnterEmerage(int TotSize, char *lpData);				//������ -> ����
	LONG OnReceiveEnterParticipation(int TotSize, char *lpData);		//���� -> ������
	LONG OnReceiveGameOverResult(int TotSize, char *lpData);			//������ ����Ǿ���
	LONG OnReceiveChatData(int TotSize, char *lpData);					//���ӹ� ä�õ���Ÿ
	LONG OnReceiveExitSubscript(int TotSize, char *lpData);				//�����⿹��
	LONG OnReceiveStartGame(int TotSize, char *lpData);					//���� ����
	LONG OnReceiveSelectCard(int TotSize, char *lpData);				//ī�� ����
	LONG OnReceiveWindCardEnd(int TotSize, char *lpData);				//���� ��� ī�峯���� ����
	LONG OnReceiveRaise(int TotSize, char *lpData);
	LONG OnReceiveFoldUser(int TotSize, char *lpData);					//������ ����
	LONG OnReceiveHiddenCard(int TotSize, char *lpData);				//������ ���� ī��
	LONG OnReceiveEndGame(int TotSize, char *lpData); 
	LONG OnReceiveSpecialCard(int TotSize, char *lpData);				//[ ###����### ]���ӹ� & ����
	LONG OnReceiveSeatKeeper(int TotSize, char *lpData);				//�ڸ� ��Ŵ�� ��Ŷ ó��
	LONG OnReceiveChangeMoney(int TotSize, char *lpData);
	LONG OnReceiveAcceptOutRoom(int TotSize, char *lpData);				//��� ��û �㰡��
	LONG OnReceiveUserEnterRoom(int TotSize, char *lpData);				//���ο� �÷��̾ �Խ��Ͽ���
	LONG OnReceiveUserOutRoom(int TotSize, char *lpData);				//����ڰ� �濡�� ��������
	LONG OnReceiveChangeRoomMaster(int TotSize, char *lpData);			//������ �ٲ�����
	LONG OnReceiveAbnormalOutRoom(int TotSize, char *lpData);			//������������ ������
	LONG OnReceiveWhisperChat(int TotSize, char *lpData);				//�ӼӸ�
	LONG OnReceiveAdminWhisperChat(int TotSize, char *lpData);
	LONG OnReceiveChangeStyleRoomInfo(int TotSize, char *lpData);		//### [������ ��� �۾�] ###  ������ ����
	LONG OnReceiveAddBombCard(int TotSize, char *lpData);
	LONG OnReceiveOpenBombCard(int TotSize, char *lpData);		
	LONG OnReceiveInsuranceSupplyResult(int TotSize, char *lpData);		//���κ��� ���� ��û ���
	LONG OnReceiveChangeGameUserInfo(int TotSize, char *lpData);
	LONG OnReceiveNotifyChangeOverMoney(int TotSize, char *lpData);
	LONG OnReceiveGiveJokerCard(int TotSize, char *lpData);
	LONG OnReceiveAskChangeSoundFX(int TotSize, char *lpData);			//### [���� �߰� �۾�] ###
	LONG OnReceiveRefuseInvite(int TotSize, char *lpData);				//�÷��̾ ��û �޼����� �ź���
	LONG OnReceiveJACKPOTMONEYINFO(int TotSize, char *lpData);		//��������
	LONG OnReceiveSV_LOSSMONEYINFO(int nTotSize, char *lpData);		// ������ üũ��.
	LONG OnReceiveSV_ASK_WAIT_INROOM(int TotSize, char *lpData);	//������ : ����ϱ� ��Ŷ
	LONG OnReceiveSV_Reward_Allin_Angel(int TotSize, char *lpData);	// ����õ��
	LONG OnReceiveSV_Reward_Allin_Angel_Gold(int TotSize, char *lpData);
	LONG OnReceiveSV_Reward_CongNoti(int TotSize, char *lpData);
	LONG OnReceiveSV_ETCCHIP_PLAYER_INFO(int TotSize, char *lpData);	
	LONG OnReceiveSV_PM_ODDEVEN_WINNER_NOTI(int TotSize, char *lpData); //��� �⼮�� ��Ƽ	

	//=========================================================================================

	LRESULT OnSetLimitMoney(WPARAM &, LPARAM &);
	LRESULT OnSetLimitGold(WPARAM &, LPARAM &);
	LRESULT OnGetMoneyGrade(WPARAM &, LPARAM &);
	LRESULT OnGetMyJokboName(WPARAM &, LPARAM &);	
	LRESULT OnGetJokboName(WPARAM &, LPARAM &);
	LRESULT OnGetResultMoney(WPARAM &, LPARAM &);	
	LRESULT OnClearJackPotMoney(WPARAM &, LPARAM &);	
	LRESULT OnGetResultOpen(WPARAM &, LPARAM &);
	LRESULT OnSetPlayRakeBackGold(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnSetPlayRakeBackGoldEvent(WPARAM &wParam, LPARAM &lParam);

	friend class CGameManager;
	
};