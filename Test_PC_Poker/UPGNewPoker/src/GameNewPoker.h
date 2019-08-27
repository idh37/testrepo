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
	virtual bool ReLoadSprite(int nSkinKind, bool bFirstLoad = false);//스킨을 변경하는 게임만 적용
	virtual void LoadSound();

	virtual void ReleaseSprite();
	virtual void ReleaseSound();

	virtual CPlayer *CreateNewPlayer(void);

	CGameProcess_NewPoker*	GetGameProcess();
	CGameViewNewPoker*		GetGameView();		// 게임 뷰 포인터

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
	virtual void OnCloseOptionDlg(LRESULT nRet);								//옵션창이 닫힐때 호술되는 함수
	virtual	void OnLoadingEnd(void);
	virtual void OnFocusGame(bool bFocus);
	virtual CString GetLobbyEnterSound(bool bMan);

	//=========================================================================================
	//패킷 처리 관련 함수들
	virtual LONG OnReceiveResGameContinue(int TotSize, char *lpData);

	LONG OnReceiveUserInfo(int TotSize, char *lpData);					//유저 정보창은 게임방/대기실 동일하고 게임쪽에 존재하지 때문에 게임쪽에서 띄운다.
	LONG OnReceiveStartButtonEnable(int TotSize, char *lpData);			//시작버튼 활성화
	LONG OnReceiveObserverMessege(int TotSize, char *lpData);			//관전 ,참여 변경
	LONG OnReceiveReservationInfo(int TotSize, char *lpData);			//예약자 정보
	LONG OnReceiveAcceptEmerage(int TotSize, char *lpData);				//참여->관전
	LONG OnReceiveAcceptParticipation(int TotSize, char *lpData);		//관전->참여
	LONG OnReceiveEnterEmerage(int TotSize, char *lpData);				//겜참여 -> 관전
	LONG OnReceiveEnterParticipation(int TotSize, char *lpData);		//관전 -> 겜참여
	LONG OnReceiveGameOverResult(int TotSize, char *lpData);			//게임이 종료되었음
	LONG OnReceiveChatData(int TotSize, char *lpData);					//게임방 채팅데이타
	LONG OnReceiveExitSubscript(int TotSize, char *lpData);				//나가기예약
	LONG OnReceiveStartGame(int TotSize, char *lpData);					//게임 시작
	LONG OnReceiveSelectCard(int TotSize, char *lpData);				//카드 선택
	LONG OnReceiveWindCardEnd(int TotSize, char *lpData);				//유저 모두 카드날리기 종료
	LONG OnReceiveRaise(int TotSize, char *lpData);
	LONG OnReceiveFoldUser(int TotSize, char *lpData);					//유저가 폴드
	LONG OnReceiveHiddenCard(int TotSize, char *lpData);				//마지막 히든 카드
	LONG OnReceiveEndGame(int TotSize, char *lpData); 
	LONG OnReceiveSpecialCard(int TotSize, char *lpData);				//[ ###잭팟### ]게임방 & 대기실
	LONG OnReceiveSeatKeeper(int TotSize, char *lpData);				//자리 지킴이 패킷 처리
	LONG OnReceiveChangeMoney(int TotSize, char *lpData);
	LONG OnReceiveAcceptOutRoom(int TotSize, char *lpData);				//퇴실 요청 허가됨
	LONG OnReceiveUserEnterRoom(int TotSize, char *lpData);				//새로운 플레이어가 입실하였음
	LONG OnReceiveUserOutRoom(int TotSize, char *lpData);				//사용자가 방에서 퇴장했음
	LONG OnReceiveChangeRoomMaster(int TotSize, char *lpData);			//방장이 바뀌어야함
	LONG OnReceiveAbnormalOutRoom(int TotSize, char *lpData);			//비정상적으로 퇴장함
	LONG OnReceiveWhisperChat(int TotSize, char *lpData);				//귓속말
	LONG OnReceiveAdminWhisperChat(int TotSize, char *lpData);
	LONG OnReceiveChangeStyleRoomInfo(int TotSize, char *lpData);		//### [관리자 모드 작업] ###  방정보 변경
	LONG OnReceiveAddBombCard(int TotSize, char *lpData);
	LONG OnReceiveOpenBombCard(int TotSize, char *lpData);		
	LONG OnReceiveInsuranceSupplyResult(int TotSize, char *lpData);		//올인보험 지급 요청 결과
	LONG OnReceiveChangeGameUserInfo(int TotSize, char *lpData);
	LONG OnReceiveNotifyChangeOverMoney(int TotSize, char *lpData);
	LONG OnReceiveGiveJokerCard(int TotSize, char *lpData);
	LONG OnReceiveAskChangeSoundFX(int TotSize, char *lpData);			//### [사운드 추가 작업] ###
	LONG OnReceiveRefuseInvite(int TotSize, char *lpData);				//플레이어가 초청 메세지를 거부함
	LONG OnReceiveJACKPOTMONEYINFO(int TotSize, char *lpData);		//족보잭팟
	LONG OnReceiveSV_LOSSMONEYINFO(int nTotSize, char *lpData);		// 규제안 체크용.
	LONG OnReceiveSV_ASK_WAIT_INROOM(int TotSize, char *lpData);	//규제안 : 대기하기 패킷
	LONG OnReceiveSV_Reward_Allin_Angel(int TotSize, char *lpData);	// 올인천사
	LONG OnReceiveSV_Reward_Allin_Angel_Gold(int TotSize, char *lpData);
	LONG OnReceiveSV_Reward_CongNoti(int TotSize, char *lpData);
	LONG OnReceiveSV_ETCCHIP_PLAYER_INFO(int TotSize, char *lpData);	
	LONG OnReceiveSV_PM_ODDEVEN_WINNER_NOTI(int TotSize, char *lpData); //골드 출석부 노티	

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