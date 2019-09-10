#pragma once


class CGameProcessHighLow;
class CGameViewHighLow;

// ���� ��Ʈ�� Receive ��Ŷ�� ó���ϴ� Ŭ����

class UPG_GAME_EXPORT CGameHighLow : public CGame
{
public:
	CGameHighLow(void);
	virtual ~CGameHighLow(void);

	friend class CGameManager;

protected:
	virtual BOOL Create(CWnd *pParentWnd);

	CGameProcessHighLow* GetGameProcess();	// ���� ���μ��� ������
	CGameViewHighLow*	GetGameView();		// ���� �� ������

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
	virtual void OnChangeFullScreen(bool bFullScreen);					// Ǯ��ũ��/â��� ����� ȣ��Ǵ� �Լ�
	virtual void OnCloseOptionDlg(LRESULT nRet);						// �ɼ�â�� ������ ȣ���Ǵ� �Լ�
	virtual void OnLoadingEnd(void);
	virtual void OnFocusGame(bool bFocus);
	virtual CString GetLobbyEnterSound(bool bMan);

	// ��Ʈ�� �޽����� ó���ϴ� �Լ���...
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

	BOOL ReceiveSV_WhisperChat(int nTotSize, char *lpData);
	BOOL ReceiveSV_AdminWhisperChat(int nTotSize, char *lpData);

	void ReceiveSV_JACKPOTMONEYINFO(int nTotSize, char *lpData);

	void ReceiveSV_LOSSMONEYINFO(int nTotSize, char *lpData);		// ������ üũ��.

	void ReceiveSV_Reward_Allin_Angel(int nTotSize, char *lpData);	// ����õ��

	void ReceiveSV_HL_EndRaise(int nTotSize, char *lpData);
	void ReceiveSV_HL_HighLowSelect(int nTotSize, char *lpData);

	void ReceiveSV_CongNoti(int TotSize, char *lpData);
	void ReceiveSV_PM_ODDEVEN_WINNER_NOTI(int TotSize, char *lpData);

	LRESULT OnSetLimitMoney(WPARAM &, LPARAM &);
	LRESULT OnSetLimitGold(WPARAM &, LPARAM &);
	LRESULT OnGetMoneyGrade(WPARAM &, LPARAM &);
	LRESULT OnGetJokboName(WPARAM &, LPARAM &);
	LRESULT OnGetLowJokboName(WPARAM &, LPARAM &);
	LRESULT OnGetMyJokboName(WPARAM &, LPARAM &);	
	LRESULT OnGetMyLowJokboName(WPARAM &, LPARAM &);		
	LRESULT OnGetResultMoney(WPARAM &, LPARAM &);	
	LRESULT OnClearJackPotMoney(WPARAM &, LPARAM &);	
	LRESULT OnGetResultOpen(WPARAM &, LPARAM &);
	LRESULT OnSetPlayRakeBackGold(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnSetPlayRakeBackGoldEvent(WPARAM &wParam, LPARAM &lParam);

	LONG OnReceiveChangeStyleRoomInfo(int TotSize, char *lpData);		//### [������ ��� �۾�] ###  ������ ����

	//������ : ����ϱ� ��Ŷ
	void ReceiveSV_ASK_WAIT_INROOM(int TotSize, char *lpData);
};