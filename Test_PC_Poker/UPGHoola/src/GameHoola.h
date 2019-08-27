#pragma once


class CGameProcessHoola;
class CGameViewHoola;

// 게임 네트웍 Receive 패킷을 처리하는 클래스

class UPG_GAME_EXPORT CGameHoola : public CGame
{
public:
	CGameHoola(void);
	virtual ~CGameHoola(void);
	
	friend class CGameManager;
	
	void Wait(DWORD dwMillisecond);

protected:
	virtual BOOL Create(CWnd *pParentWnd);

	CGameProcessHoola* GetGameProcess();	// 게임 프로세스 포인터
	CGameViewHoola*	GetGameView();		// 게임 뷰 포인터
	
	hoola::EFORM_KIND_TYPE m_eForm_KindType;
	
	// @ 훌라코드
	BOOL	m_bResultDelay;
	DWORD	m_dwResultDelay;
	bool m_bIsGiveUpRoom;
	// @
	
	void LoadSprite();	
	void LoadSound();
	void ReleaseSprite();
	void ReleaseSound();

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
	LONG OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData);

	virtual LRESULT ShowCreateRooomDlg(ROOMINFO *pRoomInfo, ROOMOPTION *pWebRoomOption=NULL);
	virtual void CheckDirectEnterRoomInfo(int nType, ROOMINFO *pRoomInfo);
	virtual int GetDirectEnterRoomType(int nType);
	virtual LRESULT	ShowQuickJoinerDlg(void);
	virtual LRESULT	SendQuickJoiner(void);
	virtual CPlayer *CreateNewPlayer(void);
	virtual void AddGameChat(const char *str, COLORREF crNewColor, bool bUserChat);
	virtual void AddGameInfo(const char *str, COLORREF crNewColor, bool bUserChat);
	virtual void OnChangeFullScreen(bool bFullScreen);					// 풀스크린/창모드 변경시 호출되는 함수
	virtual void OnCloseOptionDlg(LRESULT nRet);						// 옵션창이 닫힐때 호술되는 함수
	virtual void OnLoadingEnd(void);
	virtual void OnFocusGame(bool bFocus);
	virtual CString GetLobbyEnterSound(bool bMan);
	virtual void SetBGMVolume(int nVolume);

	virtual void SetFormKindType(hoola::EFORM_KIND_TYPE form_KindType);

	// 훌라 추가 코드
	virtual void SetupByConfig();

	// 네트웍 메시지를 처리하는 함수들...
private:
	void ReceiveSV_STARTBTNENABLE(int nTotSize, char *lpData);
	void ReceiveSV_OBSERVER_MESSAGE(int nTotSize, char *lpData);
	void ReceiveSV_RESERVATIONINFO(int nTotSize, char *lpData);
	void ReceiveSV_ACCEPT_EMERAGE(int nTotSize, char *lpData);
	void ReceiveSV_ACCEPT_PARTICIPATION(int nTotSize, char *lpData);
	void ReceiveSV_ENTER_EMERAGE(int nTotSize, char *lpData);
	void ReceiveSV_ENTER_PARTICIPATION(int nTotSize, char *lpData);
	void ReceiveSV_GAMEOVERRESULT(int nTotSize, char *lpData);

	void ReceiveCL_CHATDATA(int nTotSize, char *lpData);
	void ReceiveCL_EXITSUBSCRIPT(int nTotSize, char *lpData);

	void ReceiveSV_STARTCARD(int nTotSize, char *lpData);	
	void ReceiveSV_ENDGAME(int nTotSize, char *lpData);
	void ReceiveSV_SPECIALCARD(int nTotSize, char *lpData);
	void ReceiveSV_SEATKEEPER(int nTotSize, char *lpData);
	void ReceiveSV_CHANGE_MONEY(int nTotSize, char *lpData);
	void ReceiveSV_ACCEPT_OUTROOM(int nTotSize, char *lpData);	
	void ReceiveSV_CHANGEMYINFO(int nTotSize, char *lpData);
	void ReceiveSV_USERENTERROOM(int nTotSize, char *lpData);
	void ReceiveSV_USEROUTROOM(int nTotSize, char *lpData);
	void ReceiveSV_CHANGEROOMMASTER(int nTotSize, char *lpData);	

	BOOL ReceiveSV_WhisperChat(int nTotSize, char *lpData);
	BOOL ReceiveSV_AdminWhisperChat(int nTotSize, char *lpData);

	void ReceiveSV_JACKPOTMONEYINFO(int nTotSize, char *lpData);
	void ReceiveSV_LOSSMONEYINFO(int nTotSize, char *lpData); // 규제안 체크용.

	LRESULT OnSetLimitMoney(WPARAM &, LPARAM &);
	LRESULT OnSetLimitGold(WPARAM &, LPARAM &);
	LRESULT OnGetMoneyGrade(WPARAM &, LPARAM &);
	LRESULT OnGetJokboName(WPARAM &, LPARAM &);
	LRESULT OnGetMyJokboName(WPARAM &, LPARAM &);	
	LRESULT OnGetResultMoney(WPARAM &, LPARAM &);	
	LRESULT OnClearJackPotMoney(WPARAM &, LPARAM &);	
	LRESULT OnGetResultOpen(WPARAM &, LPARAM &);
	LRESULT OnSetPlayRakeBackGold(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnSetPlayRakeBackGoldEvent(WPARAM &wParam, LPARAM &lParam);

	LONG OnReceiveChangeStyleRoomInfo(int TotSize, char *lpData);		//### [관리자 모드 작업] ###  방정보 변경

//훌라 함수
	void ReceiveSV_DISCARD(int nTotSize, char *lpData);	
	void ReceiveSV_REGRESULT(int nTotSize, char *lpData);	
	void ReceiveSV_STOPGAMERESULT(int nTotSize, char *lpData);	
	void ReceiveSV_THANKRESULT(int nTotSize, char *lpData);	
	void ReceiveSV_SPECIALRULERESULT(int nTotSize, char *lpData);	
	void ReceiveSV_TURNTHANK(int nTotSize, char *lpData);
	void ReceiveSV_TURNGET(int nTotSize, char *lpData);
	void ReceiveSV_TURNREG(int nTotSize, char *lpData);
	void ReceiveSV_CANTHANK(int nTotSize, char *lpData);
	void ReceiveSV_ADDEDCARDDATA(int nTotSize, char *lpData);
	void ReceiveSV_FIXREGIST(int nTotSize, char *lpData);
	void ReceiveSV_MISSION(int nTotSize, char *lpData);
	void ReceiveSV_MISSIONCOMPLETE(int nTotSize, char *lpData);
	void ReceiveSV_MAGICCARD(int nTotSize, char *lpData);
	void ReceiveCL_BRINGINCARD(int nTotSize, char *lpData);
	void ReceiveCL_SUN_PICKUP_START(int nTotSize, char *lpData);
	void ReceiveCL_SUN_PICKUP(int nTotSize, char *lpData);
	void ReceiveCL_SUN_PICKUP_COMPLETE(int nTotSize, char *lpData);
	void ReceiveSV_JACKPOTCOUNT(int nTotSize, char *lpData);
	void ReceiveCL_GOLDSTAGE_INFO(int nTotSize, char *lpData);

	//잭팟 정보 처리
	void ReceiveOPCODE_SV_JP_WINNER_ALRAM(int nTotSize, char *lpData);
	void ReceiveOPCODE_SV_JP_EVENTTIME_ROOM(int nTotSize, char *lpData);
	void ReceiveSV_ASK_CHANGESNDFX(int nTotSize, char *lpData);
	
	void ReceiveSV_CongNoti(int TotSize, char *lpData);
	void ReceiveSV_PM_ODDEVEN_WINNER_NOTI(int TotSize, char *lpData);
	void ReceiveSV_GIVEUP_RULE_NOTI(int TotSize, char *lpData);
	void ReceiveSV_GIVEUP_LOSE_MONEY_NOTI(int TotSize, char *lpData);
	
public: //훌라 추가 함수
	void OnGameOver();
};