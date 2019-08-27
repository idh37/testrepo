#pragma once

#include "DataRoomList.h"
#include "DataRoomOptionList.h"

#define COLOR_GAMENAME_OFFLINE RGB(94, 94, 94)

class UPG_COMMON_EXPORT CGame
{
	
protected:
	//game base classes
	bool		m_bLoadSprite;
	bool		m_bLoadSound;
	int			m_nSkinKind;

	CGameViewBase *m_pGameViewBase;
	CGameProcess *m_pGameProcess;

public:
	CGame(void);
	virtual ~CGame(void);

protected:
	virtual BOOL Create(CWnd *pParentWnd) = 0;
	virtual void Release();

	virtual void LoadSprite();
	virtual bool ReLoadSprite(int nSkinKind, bool bFirstLoad = false);//스킨을 변경하는 게임만 적용
	virtual void LoadSound();

	virtual void ReleaseSprite();
	virtual void ReleaseSound();

	const int GetWidth(void);
	const int GetHeight(void);

	inline bool IsLoadSprite(void){return m_bLoadSprite;};
	inline bool IsLoadSound(void){return m_bLoadSound;};
public:
	CGameProcess*	GetGameProcess();
	CGameViewBase*	GetGameView();

	virtual LONG OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData) = 0;
	virtual LRESULT ShowCreateRooomDlg(ROOMINFO *pRoomInfo, ROOMOPTION *pWebRoomOption=NULL) = 0;
	virtual int GetDirectEnterRoomType(int nType) = 0;
	virtual void CheckDirectEnterRoomInfo(int nType, ROOMINFO *pRoomInfo) = 0;
	virtual LRESULT	ShowQuickJoinerDlg(void) = 0;
	virtual LRESULT	SendQuickJoiner(void) = 0;
	virtual CPlayer *CreateNewPlayer(void) = 0;
	virtual void AddGameChat(const char *str, COLORREF crNewColor, bool bUserChat) = 0;
	virtual void AddGameInfo(const char *str, COLORREF crNewColor, bool bUserChat) = 0;
	virtual void OnChangeFullScreen(bool bFullScreen) = 0;					//풀스크린/창모드 변경시 호출되는 함수
	virtual void OnCloseOptionDlg(LRESULT nRet) = 0;								//옵션창이 닫힐때 호술되는 함수
	virtual void OnLoadingEnd(void) = 0;									//로딩이 완료 되었을때 호출되는 함수
	virtual void OnFocusGame(bool bFocus)=0;
	virtual CString GetLobbyEnterSound(bool bMan)=0;
	virtual void SetBGMVolume(int nVolume)  {};		//배경음 볼륨을 조정한다. (배경음있는게임만)
	virtual void SetupByConfig(){};

	virtual void SetFormKindType(hoola::EFORM_KIND_TYPE form_KindType) { }

	virtual const IDX_GAME	GetGameType(void) = 0;
	virtual const int		GetMaxRoom(void) = 0;
	virtual const int		GetMaxPlayer(void) = 0;					//	
	virtual const int       GetMaxGamePlayer(void) = 0;				//	인덱스 관련 진희추가
	virtual const int		GetMaxObserver(void) = 0;
	virtual const int		GetJokboQuestCount(void) = 0;
	virtual const CString	GetGameString(void) = 0;			
	virtual const CString	GetGameName(void) = 0;					//각게임의 이름을 반환해주세요 "7포커", "로우바둑이", "뉴포커", "맞포커"
	virtual const int		GetGameCode(void) = 0;
				  int		GetMaxNewPlayer(void);
	virtual const CString	GetLoadingBackPath(void) = 0;
	virtual const COLORREF	GetGameColor(void) = 0;					//각 게임 고유의 색을 반환해주세요 7포커:RGB(66,79,113) 로우바둑이:RGB(72,95,66)  뉴포커:RGB(60,97,124) 맞포커:RGB(96,90,76);
	virtual const int		GetDefaultFormKind(void) = 0;			//게임 생성시 디폴트 FormKind 반환
	virtual const HICON		GetGameIcon(void) = 0;
	virtual const HICON		GetChatIcon(void) = 0;
	virtual const CString	GetModuleName(void) = 0;					//모듈 패스
	

	//순수게임진행이외의 기능 처리 (자리지킴이. 이벤트..등등)
	virtual void OnEnterRoom(BOOL bCreation);
	virtual void OnExitRoom();

	virtual void ShowGameWindow(bool bShow);

//	virtual CDataRoomList *GetDataRoomList() { return &m_DataRoomList; }
//	virtual CDataRoomOptionList *GetDataRoomOptionList() { return &m_DataRoomOptionList; }


	//패킷처리
	virtual LONG OnReceiveResGameContinue(int TotSize, char *lpData);
	//yoo [20150120]
	virtual BOOL OnReceiveResEndGame(void);

	friend class CGameManager;
	friend class CGameProcess;
};