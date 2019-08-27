#pragma once


class CLobbyView;


enum REASON_MOVE_GAME
{
	REASON_MOVE_GAME_NULL,
	REASON_MOVE_GAME_GAMETAB,	//게임탭 변경
	REASON_MOVE_GAME_JOINROOM,	//타 서버의 게임방 입장
	REASON_MOVE_GAME_PREMJOIN,	//타 서버로 이동 후 내맘대로바로가기 재 시도.
	REASON_MOVE_GAME_QUICKJOIN,	//타 서버로 이동 후 바로입장 재시도
	REASON_MOVE_GAME_PLAYTOGETHER, //같이게임하기(방생성후 상대초대)
	REASON_MOVE_GAME_PLAYTOGETHERCOMMUNITY, //같이게임하기(방생성후 상대초대) - 커뮤니티
};

class UPG_LOBBY_EXPORT CLobbyProcess //: public NMBASE::SKINGDI::CNMWidgetCompact
{
	INT64 m_llPrevMoney;
	INT64 m_llLoginMoney;
	INT64 m_llOldMoney;	//머니 변화를 감지하기 위한 백업머니	


	REASON_MOVE_GAME	m_eReasonMoveGame;	//서버 이동의 이유
	bool 				m_bMovingGame;	// 다른 서버로 게임 이동중인가?
	CONNECT_MOVECHANINFO m_MoveGameInfo;// 게임 이동으로 접속할 목적지 서버 정보
	int			m_nMoveGameCode;	//이동중인 게임의 게임코드
	SMALLROOMINFO	m_roominfo_Reserved;	// (게임/채널 이동시) 참가할 방정보 저장용;
	bool			m_bObserve_Reserved;		//(게임/채널 이동시) 참가정보 저장용;
	CString			m_strRoomPass_Reserved;		// (게임/채널 이동시) 참가정보 저장용;
	CCL_ASK_PREMQUICKJOIN m_pqj_Reserved;	// (게임/채널 이동시) 내맘대로바로가기 정보 저장용;

	bool 			m_bUseFindRoomOption_Reserved;	// (게임/채널 이동시) 퀵조인 저장용;
	ROOMOPTION		m_FindRoomOption_Reserved;	// (게임/채널 이동시) 퀵조인 저장용;
	bool			m_bQuickStartInRoom;		// (게임/채널 이동시) 다른방 바로가기인지 여부
	INT64			m_llRoomKeyPrev;			// (게임/채널 이동시) 다른방 바로가기에서 사용하는 이전 IPS RoomKey


	bool			m_bInvitation_reserved;		//방생성 후 초대해야할 것인가?

	bool			m_bPartnerCommunity;
	USERINFO		m_PartnerInfo;				//게임같이할 유저 정보 저장용
	CBuddyFriend	m_cPartnerBuddyFriend;		//게임같이할 유저 정보 저장용 - 커뮤니티

	bool			m_bLogined;		//로그인되어있는 상태인가?

	jackpot::JP_MONEYINFO m_JackpotMoneyInfo;

	CLobbyView *m_pLobbyView;



	bool			m_bShowSelectedRoom;		//선택방이 현재 보여져야하나 말아야하나? 
	ROOMOPTION		m_WebSelectedRoomOption;		//선택방옵션 (사용하는것은 시드머니, 방종류, 베팅룰)


	bool m_bShowRefuseDlg; // 바로 입장 팝업을 보여줄지, 서버에 입장 가능방에 넣어달라고 요청할지 체크 때문에 추가.

	int	m_nWaitingPacket;	// 게임 이동시 SV_ALLROOMINFO, SV_ALLUSERINFO 을 모두 받았는지 체크.(멈춤 현상 해결을 위함)
	
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


	//멤버변수 입출력
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
	//내맘대로 바로입장
	void RefreshQJButton();
	void OnRefusePremQuickJoin(CSV_REFUSE_PREMQUICKJOIN *pMsg);
	void InvaitEnterRoom(SMALLROOMINFO *pSRI = NULL);		
	void CreateRoom(bool bUseRoomOption, ROOMOPTION *pRoomOption);	
	void AskRefreshRoomList();
	void AskRefreshUserList( BOOL bFullList );//전체 게임리스트를 기본으로 처리 2012.10.22 YS.Jeong

	void AskTournamentInfo();
	void AskTournamnetUserList();
	
	BOOL AskMoveGame(REASON_MOVE_GAME eReasonMoveGame, IDX_GAME idxGame, int nChanNo=-1);		//게임을 바꾸거나 채널이동한다. (pSRI가 NULL이 아닐 경우 이동후 바로입장)

	void OnFailMoveGame();
	void OnAcceptMoveGame(IDX_GAME idxGame);
	void OnChangeMyMoney(INT64 llOldMoney);


	//더미방추가
	void MakeDummyRooms();
	void CheckEnableChat();				//내머니가 최상위 등급이면 채팅창을 Disable시킨다.

	void RefreshJoinableRoom();			/// 내 머니가 변했을때 전체 방리스트 SMALLROOMINFO의 szClientSideVar[CS_JOINABLE](참여가능/불가여부) 상태 변수를 갱신해준다.
	BOOL CheckJoinableRoom(SMALLROOMINFO *pSRI);	//	//접속 가능한 방인지 아닌지 체크하여 FLAG에 저장한다.
	BOOL CheckPrevJoinedRoom(SMALLROOMINFO *pSRI);	//	//이전 접속 방인지 아닌지 체크하여 FLAG에 저장한다.
	void RefreshTaskBar();

	BOOL AskEnterGlobalRoom(SMALLROOMINFO *pSRI, bool bObserve, const char *pRoomPass=NULL);	// 방입장 요청
	BOOL OnQuickStartOtherServer(IDX_GAME nGameIndex, int nChanNo, bool bUseFindRoomOption, ROOMOPTION &FindRoomOption, bool bQuickStartInRoom = false, INT64 llRoomKeyPrev = 0);	//바로입장(퀵스타트)를 보냈는데  다른 서버로 이동 후 다시 시도해야할때 

	void ChangeProperRoomTab();		//머니에 따라 맞는 룸리스트를 보여준다.

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

	//선택방 체크 함수
	BOOL CheckSelectedRoom(SMALLROOMINFO *pSRI);
	void RemoveCheckSelectedRoom();

	void ChangeRoomTabByEnterLimitMoney(IDX_GAME eGameIndex, INT64 llEnterLimitMoney);		//llEnterLimitMoney에 맞는 방리스트 탭으로 이동시켜준다.

	void EnableWindowLobbyView(BOOL bState);
	int	GetWaitingPacketNum() { return m_nWaitingPacket;  }
	void ReceiveWaitingPacket() { m_nWaitingPacket--; }
};
