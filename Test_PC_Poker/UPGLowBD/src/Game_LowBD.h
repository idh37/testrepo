#pragma once

class GameProcess_BD;
class CGameViewLowBD;

class UPG_GAME_EXPORT CGame_LowBD : public CGame
{
protected:
	int											m_nMaxUserCnt;
	int											m_nRegUserCnt;
	int											m_nReservedRegUserCnt;

public:
	bool										m_bFlagReceiveLeaveRoomReq;

public:
	CGame_LowBD();
	virtual ~CGame_LowBD(void);

protected:
	virtual BOOL Create(CWnd *pParentWnd);

	GameProcess_BD* GetGameProcess();
	CGameViewLowBD* GetGameView();

//	bool TMSUserInfoCompare(TMS::_TMS_UserInfo* first, TMS::_TMS_UserInfo* second);

	virtual void LoadSprite();	
	virtual void LoadSound();

	virtual void ReleaseSprite();
	virtual void ReleaseSound();

	virtual const IDX_GAME	GetGameType(void);
	virtual const int		GetMaxRoom(void);
	virtual const int		GetMaxPlayer(void);
	virtual const int		GetMaxGamePlayer(void);
	virtual const int		GetMaxObserver(void);
	virtual const int		GetJokboQuestCount(void);
	virtual const CString	GetGameString(void);
	virtual const CString	GetGameName(void);
	virtual const COLORREF	GetGameColor(void);
	virtual const int		GetGameCode(void);
	virtual const CString	GetLoadingBackPath(void);
	virtual const int		GetDefaultFormKind(void);
	virtual const HICON		GetGameIcon(void);
	virtual const HICON		GetChatIcon(void);
	virtual const CString	GetModuleName(void);
public:
	virtual LONG OnPacketNotify(char* pName, int nSignal, int nTotSize, char *lpData);

	virtual CPlayer *CreateNewPlayer(void);

	virtual LRESULT ShowCreateRooomDlg(ROOMINFO *pRoomInfo, ROOMOPTION *pWebRoomOption=NULL);
	virtual void CheckDirectEnterRoomInfo(int nType, ROOMINFO *pRoomInfo);
	virtual LRESULT	ShowQuickJoinerDlg(void);
	virtual LRESULT	SendQuickJoiner(void);
	virtual int GetDirectEnterRoomType(int nType);

	virtual void AddGameChat(const char *str, COLORREF crNewColor, bool bUserChat);
	virtual void AddGameInfo(const char *str, COLORREF crNewColor, bool bUserChat);
	virtual void OnChangeFullScreen(bool bFullScreen);					//풀스크린/창모드 변경시 호출되는 함수
	virtual void OnCloseOptionDlg(LRESULT nRet);						//옵션창이 닫힐때 호술되는 함수
	virtual void OnLoadingEnd(void);									//로딩이 완료 되었을때 호출되는 함수
	virtual void OnSetCurrentGame(void);
	virtual void OnFocusGame(bool bFocus);
	virtual CString GetLobbyEnterSound(bool bMan);

	//순수게임진행이외의 기능 처리 (자리지킴이. 이벤트..등등)
	// 	virtual void OnEnterRoom(BOOL bCreation);
	// 	virtual void OnExitRoom();

	// 네트웍 메시지를 처리하는 함수들...
private:
	LONG OnReceiveNotifyChangeOverMoney(int TotSize, char *lpData);
	LONG OnReceiveUserInfo(int TotSize, char *lpData);					//유저 정보창은 게임방/대기실 동일하고 게임쪽에 존재하지 때문에 게임쪽에서 띄운다.

	LONG OnReceiveStartButtonEnable(int nTotSize, char *lpData);
	LONG ReceiveSV_OBSERVER_MESSAGE(int nTotSize, char *lpData);
	LONG ReceiveSV_RESERVATIONINFO(int nTotSize, char *lpData);
	LONG ReceiveSV_ACCEPT_EMERAGE(int nTotSize, char *lpData);
	LONG ReceiveSV_ACCEPT_PARTICIPATION(int nTotSize, char *lpData);
	LONG ReceiveSV_ENTER_EMERAGE(int nTotSize, char *lpData);
	LONG ReceiveSV_ENTER_PARTICIPATION(int nTotSize, char *lpData);
	LONG ReceiveSV_ASK_BANISHVOTE(int nTotSize, char *lpData);
	LONG OnReceiveWhisperChat(int TotSize, char *lpData);				//귓속말
	LONG OnRecevieAdminWhisperChat(int TotSize, char *lpData);			//관리자 귓속말
	LONG OnReceiveExitSubscript(int TotSize, char *lpData);				//나가기예약	
	LONG OnReceiveInsuranceSupplyResult(int TotSize, char *lpData);		//올인보험 지급 요청 결과
	LONG OnReceiveSV_Reward_Allin_Angel(int TotSize, char *lpData);		// 올인천사
	LONG OnReceiveSV_Reward_Allin_Angel_Gold(int TotSize, char *lpData);	
	
	void ReceiveSV_NOTIFY_BANISHCOUNT(int nTotSize, char *lpData);
	void ReceiveSV_GAMEOVERRESULT(int nTotSize, char *lpData);

	void ReceiveCL_CHATDATA(int nTotSize, char *lpData);
	void ReceiveCL_EXITSUBSCRIPT(int nTotSize, char *lpData);

	void ReceiveSV_STARTCARD(int nTotSize, char *lpData);
	void ReceiveSV_SELECTCARD(int nTotSize, char *lpData);
	void ReceiveSV_WINDCARDEND(int nTotSize, char *lpData);
	void ReceiveSV_RAISE(int nTotSize, char *lpData);
	void ReceiveSV_DIEUSER(int nTotSize, char *lpData);
	void ReceiveSV_HIDDENCARD(int nTotSize, char *lpData);
	void ReceiveSV_ENDGAME(int nTotSize, char *lpData);
	void ReceiveSV_SPECIALCARD(int nTotSize, char *lpData);
	void ReceiveSV_SEATKEEPER(int nTotSize, char *lpData);
	void ReceiveSV_CHANGE_MONEY(int nTotSize, char *lpData);
	void ReceiveSV_ACCEPT_OUTROOM(int nTotSize, char *lpData);	
	void ReceiveSV_CHANGEMYINFO(int nTotSize, char *lpData);
	void ReceiveSV_USERENTERROOM(int nTotSize, char *lpData);
	void ReceiveSV_USEROUTROOM(int nTotSize, char *lpData);
	void ReceiveSV_CHANGEROOMMASTER(int nTotSize, char *lpData);
	void ReceiveSV_CongNoti(int TotSize, char *lpData);
	void ReceiveSV_ETCCHIP_PLAYER_INFO(int TotSize, char *lpData);
	void ReceiveSV_PM_ODDEVEN_WINNER_NOTI(int TotSize, char *lpData);

	LRESULT OnSetLimitMoney(WPARAM &, LPARAM &);
	LRESULT OnSetLimitGold(WPARAM &, LPARAM &);
	LRESULT OnGetMoneyGrade(WPARAM &, LPARAM &);
	LRESULT OnGetJokboName(WPARAM &, LPARAM &);
	LRESULT OnGetMyJokboName(WPARAM &, LPARAM &);
	LRESULT OnGetResultMoney(WPARAM &, LPARAM &);
	LRESULT OnSetJackPotMoney(WPARAM &, LPARAM &);
	LRESULT OnClearJackPotMoney(WPARAM &, LPARAM &);	
	LRESULT OnGetResultOpen(WPARAM &, LPARAM &);
	LRESULT OnSetPlayRakeBackGold(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnSetPlayRakeBackGoldEvent(WPARAM &wParam, LPARAM &lParam);

	LRESULT OnSetExitReserve(WPARAM &wParam, LPARAM &lParam);
	
	LONG OnReceiveChangeStyleRoomInfo(int TotSize, char *lpData);		//### [관리자 모드 작업] ###  방정보 변경

	//규제안 : 대기하기 패킷
	void ReceiveSV_ASK_WAIT_INROOM(int TotSize, char *lpData);

	friend class CGameManager;
};


