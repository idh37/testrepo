#pragma once

#define IDM_TABVIEWCHAT		(WM_USER+6200) // 족보등급	보이기
#define IDM_TABVIEWJOKBO	(WM_USER+6201)
#define IDM_TABVIEWEVENT	(WM_USER+6202)
#define IDM_BADUSER			(WM_USER+6203)
#define IDM_WHISPERLIST		(WM_USER+6204) //귓말사용자보기
#define IDM_SETWHISPER		(WM_USER+6205)
#define IDM_TABVIEWLEAGSON	(WM_USER+6206)


#define MAX_WHISPERCNT		6	//전체 + 5명 = 6
#define FIRST_WHISPERCNT	1	

enum eCHATVIEW
{
	eCHAT	= 0,
	eEVENT	= 1
};

class CUIInfoData;
class CGameHelperMan;

class UPG_GAMEBASE_EXPORT CChattingWindowMan  
{
public:
	enum ENUM_CHATTING_TAB_TYPE
	{
		ECTT_CHAT = 0,
		ECTT_JOKBO,
		ECTT_EVENT,
		ECTT_MAX
	};
protected:
	CUIInfoData				*m_pUIInfoData;
	CGameViewBase			*m_pGameView;
	CGame					*m_pGame;

	NMBASE::GRAPHICGDI::xSprite					m_sprChatBox;
	NMBASE::GRAPHICGDI::xSprite					m_sprTabEffect;

	int						m_nXp;
	int						m_nYp;

	int						m_nWhisperXp;		
	int						m_nWhisperYp;

	CWnd					*m_pParent;
	NMBASE::GRAPHICGDI::CPage					*m_pPage;

	NMBASE::SKINGDI::CGraphButton			m_btnChat;
	NMBASE::SKINGDI::CGraphButton			m_btnJokbo;
	NMBASE::SKINGDI::CGraphButton			m_btnEvent;
	NMBASE::SKINGDI::CGraphButton			m_btnBadUser;
	NMBASE::SKINGDI::CGraphButton			m_btnLift;
	CGameHelperMan							*m_pGameHelperMan;

	NMBASE::SKINGDI::CNChatView				*m_pChatting;
	NMBASE::SKINGDI::CNChatView				*m_pInfoBox;
	NMBASE::SKINGDI::CNEditBox				*m_pSendEdit;
	ENUM_CHATTING_TAB_TYPE	m_nViewTab;

	NMBASE::SKINGDI::CNText*				m_parrTextGrade[10];
	NMBASE::SKINGDI::CNText*				m_parrTextID[10];
	NMBASE::SKINGDI::CNText*				m_parrTextMoney[10];

	NMBASE::SKINGDI::CNText*				m_parrTextRoundGrade[5];
	NMBASE::SKINGDI::CNText*				m_parrTextRoundID[5];
	NMBASE::SKINGDI::CNText*				m_parrTextRoundMoney[5];

	NMBASE::SKINGDI::CNText*				m_pTextRoundInfo;

	bool					m_bAddChat;
	bool					m_bAddEvent;

	CString					m_strChatInfo;
	CRect					m_rcChatInfo;
public:
	CChattingWindowMan();
	virtual ~CChattingWindowMan();

	virtual void LoadImage(LPCTSTR lpChatSprPath, LPCTSTR lpTabEffectPath=NULL);

	virtual void Init(CUIInfoData *pUIInfoData, CGameViewBase *pGameView, CGameHelperMan *pGameHelperMan);
	virtual void InitButton(NMBASE::GRAPHICGDI::xSprite *pBadButton);
	virtual BOOL OnLButtonDown( int x, int y);
	virtual BOOL OnLButtonUp( int x, int y );
	virtual BOOL OnMouseMove( int x, int y );
	virtual BOOL OnMouseWheel( int x, int y );
	virtual void Draw( CDC *pDC );
	virtual void DrawWhisperList( CDC *pDC );
	virtual COLORREF GetChatColor(int nColor);
	virtual void SetCurrnetChat(void);

	virtual void ReSetWindowPos(int x , int y);
	virtual void SelectNextTab(void);

	virtual void OnTimer();
	virtual void ResetChatting_STR();
	virtual bool CheckWhisperSendChat(const char *str);
	
	virtual void SetViewPage(const ENUM_CHATTING_TAB_TYPE &nViewTab = ECTT_CHAT);
	virtual const ENUM_CHATTING_TAB_TYPE &GetViewMode()
	{
		return m_nViewTab;
	};	

	virtual void OnCommand(WPARAM wParam);

	virtual void OnDestroy();

	virtual void AddChat(LPCTSTR lpStr, COLORREF crNewColor, bool bUserChat);
	virtual void AddInfo(LPCTSTR lpStr, COLORREF crNewColor, bool bUserChat);
	//virtual void AddWhipserUser( LPCTSTR lpStr );
	virtual void SystemKeyReturn(LPCTSTR str);
	virtual void SetEditFocus(void);
	virtual bool IsEditFocus(void);
	virtual CString GetEditText(void);

	virtual void SetEvent(void);
	void OnSetWhisper(void);

	LRESULT OnChangeView(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnIsComboPopupClose(WPARAM &wParam, LPARAM &lParam);

	virtual NMBASE::SKINGDI::CNChatView* GetChatView(){return m_pChatting;}
	virtual NMBASE::SKINGDI::CNEditBox*  GetSendEdit(){return m_pSendEdit;}
	
	virtual CGameViewBase* GetGameViewBase(){return m_pGameView;}

	void SetLiftBtnEnable (bool bEnable)
	{
		m_btnLift.Enable(bEnable);
	}
};

UPG_GAMEBASE_EXPORT extern CChattingWindowMan *GetCurrentChttingWindowMan(void);