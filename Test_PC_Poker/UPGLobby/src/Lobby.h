#pragma once


#include "LobbyView.h"
#include "LobbyProcess.h"

class UPG_LOBBY_EXPORT CLobby
{
	CLobbyView *m_pLobbyView;
	CLobbyProcess *m_pLobbyProcess;
public:
	CLobby(void);
	virtual ~CLobby(void);

	CLobbyView *GetLobbyView() { ASSERT(m_pLobbyView); return m_pLobbyView; }
	CLobbyProcess *GetLobbyProcess() { ASSERT(m_pLobbyProcess); return m_pLobbyProcess; }

	virtual BOOL Create(CWnd *pParentWnd);
	//virtual LONG OnPacketNotify(int Sid, int Signal, int TotSize, char *lpData);
	virtual LONG OnPacketNotify(char* pServerName, int Signal, int TotSize, char *lpData);
	virtual void Release();

	LRESULT OnLobbyWnd(WPARAM &wParam, LPARAM &lParam);

	void AddChatText(const char *pMsg, COLORREF color);

	void OnChangeFullScreen(BOOL bFullScr);
	void ShowLobbyWindow(bool bShow);

	void OnChangeView(IDX_GAMEWHERE nWhere,bool bRefresh=false);
	
	//void OnLogin();
	void URLEncode(char* output, char* input);

	bool IsMovingGame();

	void PlayEnterLobbySound(void);
	void PlayBGM(void);
	void StopBGM(void);
	void SetVolume();

	INT64 GetLoginMoney();		//로그인했을때 머니저장값을 가져온다.
	
	FOBIS_Common* GetFOBISLogData();
	CString GetVersionInfo();

	void ShowPopup();		// 한도 팝업 띄움
	void ShowLoginPopup();	// 로그인했을때 팝업 띄움.
};
