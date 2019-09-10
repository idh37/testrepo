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
	virtual bool ReLoadSprite(int nSkinKind, bool bFirstLoad = false);//��Ų�� �����ϴ� ���Ӹ� ����
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
	virtual void OnChangeFullScreen(bool bFullScreen) = 0;					//Ǯ��ũ��/â��� ����� ȣ��Ǵ� �Լ�
	virtual void OnCloseOptionDlg(LRESULT nRet) = 0;								//�ɼ�â�� ������ ȣ���Ǵ� �Լ�
	virtual void OnLoadingEnd(void) = 0;									//�ε��� �Ϸ� �Ǿ����� ȣ��Ǵ� �Լ�
	virtual void OnFocusGame(bool bFocus)=0;
	virtual CString GetLobbyEnterSound(bool bMan)=0;
	virtual void SetBGMVolume(int nVolume)  {};		//����� ������ �����Ѵ�. (������ִ°��Ӹ�)
	virtual void SetupByConfig(){};

	virtual void SetFormKindType(hoola::EFORM_KIND_TYPE form_KindType) { }

	virtual const IDX_GAME	GetGameType(void) = 0;
	virtual const int		GetMaxRoom(void) = 0;
	virtual const int		GetMaxPlayer(void) = 0;					//	
	virtual const int       GetMaxGamePlayer(void) = 0;				//	�ε��� ���� �����߰�
	virtual const int		GetMaxObserver(void) = 0;
	virtual const int		GetJokboQuestCount(void) = 0;
	virtual const CString	GetGameString(void) = 0;			
	virtual const CString	GetGameName(void) = 0;					//�������� �̸��� ��ȯ���ּ��� "7��Ŀ", "�ο�ٵ���", "����Ŀ", "����Ŀ"
	virtual const int		GetGameCode(void) = 0;
				  int		GetMaxNewPlayer(void);
	virtual const CString	GetLoadingBackPath(void) = 0;
	virtual const COLORREF	GetGameColor(void) = 0;					//�� ���� ������ ���� ��ȯ���ּ��� 7��Ŀ:RGB(66,79,113) �ο�ٵ���:RGB(72,95,66)  ����Ŀ:RGB(60,97,124) ����Ŀ:RGB(96,90,76);
	virtual const int		GetDefaultFormKind(void) = 0;			//���� ������ ����Ʈ FormKind ��ȯ
	virtual const HICON		GetGameIcon(void) = 0;
	virtual const HICON		GetChatIcon(void) = 0;
	virtual const CString	GetModuleName(void) = 0;					//��� �н�
	

	//�������������̿��� ��� ó�� (�ڸ���Ŵ��. �̺�Ʈ..���)
	virtual void OnEnterRoom(BOOL bCreation);
	virtual void OnExitRoom();

	virtual void ShowGameWindow(bool bShow);

//	virtual CDataRoomList *GetDataRoomList() { return &m_DataRoomList; }
//	virtual CDataRoomOptionList *GetDataRoomOptionList() { return &m_DataRoomOptionList; }


	//��Ŷó��
	virtual LONG OnReceiveResGameContinue(int TotSize, char *lpData);
	//yoo [20150120]
	virtual BOOL OnReceiveResEndGame(void);

	friend class CGameManager;
	friend class CGameProcess;
};