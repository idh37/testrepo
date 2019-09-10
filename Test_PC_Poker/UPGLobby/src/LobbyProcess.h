#pragma once


class CLobbyView;


enum REASON_MOVE_GAME
{
	REASON_MOVE_GAME_NULL,
	REASON_MOVE_GAME_GAMETAB,	//������ ����
	REASON_MOVE_GAME_JOINROOM,	//Ÿ ������ ���ӹ� ����
	REASON_MOVE_GAME_PREMJOIN,	//Ÿ ������ �̵� �� ������ιٷΰ��� �� �õ�.
	REASON_MOVE_GAME_QUICKJOIN,	//Ÿ ������ �̵� �� �ٷ����� ��õ�
	REASON_MOVE_GAME_PLAYTOGETHER, //���̰����ϱ�(������� ����ʴ�)
	REASON_MOVE_GAME_PLAYTOGETHERCOMMUNITY, //���̰����ϱ�(������� ����ʴ�) - Ŀ�´�Ƽ
};

class UPG_LOBBY_EXPORT CLobbyProcess //: public NMBASE::SKINGDI::CNMWidgetCompact
{
	INT64 m_llPrevMoney;
	INT64 m_llLoginMoney;
	INT64 m_llOldMoney;	//�Ӵ� ��ȭ�� �����ϱ� ���� ����Ӵ�	


	REASON_MOVE_GAME	m_eReasonMoveGame;	//���� �̵��� ����
	bool 				m_bMovingGame;	// �ٸ� ������ ���� �̵����ΰ�?
	CONNECT_MOVECHANINFO m_MoveGameInfo;// ���� �̵����� ������ ������ ���� ����
	int			m_nMoveGameCode;	//�̵����� ������ �����ڵ�
	SMALLROOMINFO	m_roominfo_Reserved;	// (����/ä�� �̵���) ������ ������ �����;
	bool			m_bObserve_Reserved;		//(����/ä�� �̵���) �������� �����;
	CString			m_strRoomPass_Reserved;		// (����/ä�� �̵���) �������� �����;
	CCL_ASK_PREMQUICKJOIN m_pqj_Reserved;	// (����/ä�� �̵���) ������ιٷΰ��� ���� �����;

	bool 			m_bUseFindRoomOption_Reserved;	// (����/ä�� �̵���) ������ �����;
	ROOMOPTION		m_FindRoomOption_Reserved;	// (����/ä�� �̵���) ������ �����;
	bool			m_bQuickStartInRoom;		// (����/ä�� �̵���) �ٸ��� �ٷΰ������� ����
	INT64			m_llRoomKeyPrev;			// (����/ä�� �̵���) �ٸ��� �ٷΰ��⿡�� ����ϴ� ���� IPS RoomKey


	bool			m_bInvitation_reserved;		//����� �� �ʴ��ؾ��� ���ΰ�?

	bool			m_bPartnerCommunity;
	USERINFO		m_PartnerInfo;				//���Ӱ����� ���� ���� �����
	CBuddyFriend	m_cPartnerBuddyFriend;		//���Ӱ����� ���� ���� ����� - Ŀ�´�Ƽ

	bool			m_bLogined;		//�α��εǾ��ִ� �����ΰ�?

	jackpot::JP_MONEYINFO m_JackpotMoneyInfo;

	CLobbyView *m_pLobbyView;



	bool			m_bShowSelectedRoom;		//���ù��� ���� ���������ϳ� ���ƾ��ϳ�? 
	ROOMOPTION		m_WebSelectedRoomOption;		//���ù�ɼ� (����ϴ°��� �õ�Ӵ�, ������, ���÷�)


	bool m_bShowRefuseDlg; // �ٷ� ���� �˾��� ��������, ������ ���� ���ɹ濡 �־�޶�� ��û���� üũ ������ �߰�.

	int	m_nWaitingPacket;	// ���� �̵��� SV_ALLROOMINFO, SV_ALLUSERINFO �� ��� �޾Ҵ��� üũ.(���� ���� �ذ��� ����)
	
public:
	CLobbyProcess(void);
	virtual ~CLobbyProcess(void);
	
	virtual BOOL Create();
	virtual void Release();


	bool IsShowSelectedRoom() { return m_bShowSelectedRoom; }
	ROOMOPTION* GetWebSelectedRoomOption() {return &m_WebSelectedRoomOption; }
	void AnalizeWebSelectedRoomOption();
	void SetLobbyView(CLobbyView *pLobbyView);


	void OnEnterGame();
	void OnEnterLobby(bool bRefresh);
	void OnLogin();
	BOOL OnMessage(UINT message, WPARAM wParam, LPARAM lParam);


	//������� �����
	void SetMovingGame(bool bMovingGame, int nMoveGameCode) { m_bMovingGame = bMovingGame; m_nMoveGameCode = nMoveGameCode; }
	int GetMovingGameCode() { return m_nMoveGameCode; }
	bool IsMovingGame() { return m_bMovingGame; }
	void SetMoveGameInfo(CONNECT_MOVECHANINFO *pMoveGameInfo) { m_MoveGameInfo = *pMoveGameInfo; }
	CONNECT_MOVECHANINFO *GetMoveGameInfo() { return &m_MoveGameInfo; }
	INT64 GetLoginMoney() { return m_llLoginMoney; }
	



	//common data manipulation
	BOOL AddData_Room(SMALLROOMINFO *pSRI);
	BOOL DelData_Room(UINT64 llRoomKey);
	BOOL ChangeData_Room(SMALLROOMINFO *pSRI);	
	SMALLROOMINFO *GetData_Room(UINT64 llRoomKey);
	void DelAllData_Room();

	BOOL IsMetComboCondition(SMALLUSERINFO *pSUI);
	BOOL AddData_User(SMALLUSERINFO *pSUI);
	BOOL DelData_User(const char *pID);
	BOOL ChangeData_User(SMALLUSERINFO *pSUI);	
	SMALLUSERINFO *GetData_User(const char *pID);
	void DelAllData_User();

	void SetShowRefuseDlg(bool bShow) { m_bShowRefuseDlg = bShow; }
	bool GetShowRefuseDlg()			  { return m_bShowRefuseDlg ; }


	void ShowChanceBtn();	
	//������� �ٷ�����
	void RefreshQJButton();
	void OnRefusePremQuickJoin(CSV_REFUSE_PREMQUICKJOIN *pMsg);
	void InvaitEnterRoom(SMALLROOMINFO *pSRI = NULL);		
	void CreateRoom(bool bUseRoomOption, ROOMOPTION *pRoomOption);	
	void AskRefreshRoomList();
	void AskRefreshUserList( BOOL bFullList );//��ü ���Ӹ���Ʈ�� �⺻���� ó�� 2012.10.22 YS.Jeong

	void AskTournamentInfo();
	void AskTournamnetUserList();
	
	BOOL AskMoveGame(REASON_MOVE_GAME eReasonMoveGame, IDX_GAME idxGame, int nChanNo=-1);		//������ �ٲٰų� ä���̵��Ѵ�. (pSRI�� NULL�� �ƴ� ��� �̵��� �ٷ�����)

	void OnFailMoveGame();
	void OnAcceptMoveGame(IDX_GAME idxGame);
	void OnChangeMyMoney(INT64 llOldMoney);


	//���̹��߰�
	void MakeDummyRooms();
	void CheckEnableChat();				//���Ӵϰ� �ֻ��� ����̸� ä��â�� Disable��Ų��.

	void RefreshJoinableRoom();			/// �� �Ӵϰ� �������� ��ü �渮��Ʈ SMALLROOMINFO�� szClientSideVar[CS_JOINABLE](��������/�Ұ�����) ���� ������ �������ش�.
	BOOL CheckJoinableRoom(SMALLROOMINFO *pSRI);	//	//���� ������ ������ �ƴ��� üũ�Ͽ� FLAG�� �����Ѵ�.
	BOOL CheckPrevJoinedRoom(SMALLROOMINFO *pSRI);	//	//���� ���� ������ �ƴ��� üũ�Ͽ� FLAG�� �����Ѵ�.
	void RefreshTaskBar();

	BOOL AskEnterGlobalRoom(SMALLROOMINFO *pSRI, bool bObserve, const char *pRoomPass=NULL);	// ������ ��û
	BOOL OnQuickStartOtherServer(IDX_GAME nGameIndex, int nChanNo, bool bUseFindRoomOption, ROOMOPTION &FindRoomOption, bool bQuickStartInRoom = false, INT64 llRoomKeyPrev = 0);	//�ٷ�����(����ŸƮ)�� ���´µ�  �ٸ� ������ �̵� �� �ٽ� �õ��ؾ��Ҷ� 

	void ChangeProperRoomTab();		//�ӴϿ� ���� �´� �븮��Ʈ�� �����ش�.

	LRESULT OnGameMoneyChange(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnRefreshMyMoney(WPARAM &wParam, LPARAM &lParam);
	
	LRESULT OnPlayTogether(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnPlayTogetherCommunity(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnPlayTogetherCafe(WPARAM &wParam, LPARAM &lParam);	

	void OnAcceptCreateRoom();
	BOOL CreateRoomWithPartner(USERINFO *pPartnerInfo);
	BOOL CreateRoomWithPartnerCommunity(CBuddyFriend *pPartnerInfo);
	
	LRESULT OnLogic(LSY::CMessage *pMsg);
	void ChangeMyMoney();

	//���ù� üũ �Լ�
	BOOL CheckSelectedRoom(SMALLROOMINFO *pSRI);
	void RemoveCheckSelectedRoom();

	void ChangeRoomTabByEnterLimitMoney(IDX_GAME eGameIndex, INT64 llEnterLimitMoney);		//llEnterLimitMoney�� �´� �渮��Ʈ ������ �̵������ش�.

	void EnableWindowLobbyView(BOOL bState);
	int	GetWaitingPacketNum() { return m_nWaitingPacket;  }
	void ReceiveWaitingPacket() { m_nWaitingPacket--; }
};
