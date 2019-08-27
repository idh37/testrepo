#pragma once


enum INVITE_WIDGET_ID
{

	INVITE_WIDGET_ID_BEGIN = WM_USER+2005,
	//-------------------------------------------------------
	INVITE_WIDGET_ID_INVITETAB,
	INVITE_WIDGET_ID_INVITETAB_STATE,
	INVITE_BUTTON_ID_INVITETAB_ONEGAME,
	INVITE_BUTTON_ID_INVITETAB_ALLGAME,

	INVITE_WIDGET_ID_INVITELIST_ONEGAME,
	INVITE_WIDGET_ID_INVITELIST_ALLGAME,
	

	

	WIDGET_ID_INVITELIST_UNIT,
	WIDGET_ID_INVITELIST_UNIT_BUF = WIDGET_ID_INVITELIST_UNIT+100,


	INVITE_BUTTON_ID_INVITELIST_SORT_NICKNAME,
	INVITE_BUTTON_ID_INVITELIST_SORT_POSITION,
	INVITE_BUTTON_ID_INVITELIST_SORT_MONEY,
	INVITE_WIDGET_ID_INVITELIST_UPBUTTON,
	INVITE_WIDGET_ID_INVITELIST_DOWNBUTTON,
	INVITE_WIDGET_ID_INVITELIST_THUMBBUTTON,
		
};


class CInviteListWidget;
//////////////////////////////////////////////////////////////////////////////
// CInviteUnitWidget
class CInviteUnitWidget : public NMBASE::SKINGDI::CNMUnitWidget
{
public:
	CInviteUnitWidget();
	virtual ~CInviteUnitWidget();


	virtual void SetSound( char* szOver, char* szDown );	
	virtual void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );
	virtual BOOL OnMouseMoveWidget( UINT nFlags, CPoint point );
	virtual BOOL OnMouseLeaveWidget( UINT nFlags, CPoint point );
	virtual BOOL OnLButtonDownWidget( UINT nFlags, CPoint point );
	virtual BOOL OnLButtonDbClkWidget( UINT nFlags, CPoint point );
	virtual BOOL OnLButtonUpWidget( UINT nFlags, CPoint point );
	
	

	void SetUserInfo( SMALLUSERINFO* pUserInfo, BOOL bChecked=FALSE );
	void SetSetInTick( DWORD dwDelay );	
	void SetSprite(NMBASE::GRAPHICGDI::xSprite *pSprInvitaion, NMBASE::GRAPHICGDI::xSprite *pSprInviteUnit);

	BOOL GetCheck() { return m_bChecked; }
	SMALLUSERINFO *GetUserInfo() { return &m_UserInfo; }

protected:

	SMALLUSERINFO	m_UserInfo;
	BOOL			m_bChecked;

	enum WIDGET_STATE { STATE_NULL, STATE_ONLINE};
	int			m_nState;

	NMBASE::GRAPHICGDI::xSprite *m_pSprInvitaion;
	NMBASE::GRAPHICGDI::xSprite *m_pSprInviteUnit;


	
	
	

};



class CInviteListWidget :	public NMBASE::SKINGDI::CNMListWidget
{

protected:


	CDataUserList *m_pDataUserList;	

	NMBASE::SKINGDI::CNMButton m_BtnTab_JoinableRoom;
	NMBASE::SKINGDI::CNMButton m_BtnTab_AllRoom;


	map<string, UINT64> m_map_userinvite;	//초대유저 <아이디,유저키번호>


	CDataUserList::KEY_FILTER  m_FilterKey;
public:
	virtual BOOL CreateWidget( CWnd* pParent );
	virtual BOOL Initialize(int nControlID, CWnd *pParent, NMBASE::GRAPHICGDI::xSprite *pSprUnit, int nPosX, int nPosY, CDataUserList *pDataUserList);
	virtual void EnterWidget();
	virtual void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );
	virtual BOOL OnMouseMoveWidget( UINT nFlags, CPoint point );
	virtual BOOL OnLButtonDownWidget( UINT nFlags, CPoint point );
	virtual BOOL OnCommandWidget( int nCommand, WPARAM wParam, LPARAM lParam );	
	virtual NMBASE::SKINGDI::CNMUnitWidget *CreateNewUnitWidget(int nIndex);
	

	virtual int GetTotalDataCount();
	virtual BOOL RenewalList();


	void SetDataList(CDataUserList *pDataUserList);	
	void ClearDataList();
	void SetFilter(CDataUserList::KEY_FILTER FilterKey);

	// 체크된 유저 관리
	BOOL IsInviteCheckedUser(SMALLUSERINFO *pUI);
	void AddCheckedUser(SMALLUSERINFO *pUI);
	void DelCheckedUser(SMALLUSERINFO *pUI);
	void ClearCheckedUser();

	//체크된유저 초대하기	
	map<string, UINT64> *GetCheckedUser() { return &m_map_userinvite;}
	

	CInviteListWidget(void);
	virtual ~CInviteListWidget(void);
};


class CInviteTab : public NMBASE::SKINGDI::CNMTab
{
public:
	CInviteTab(void);
	~CInviteTab(void);

	CWnd *m_pParent;


	virtual BOOL CreateWidget( CWnd* pParent );	
	virtual void DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC );
	//virtual BOOL ChangeTab(int nViewWidgetID);
	BOOL RefreshInviteList();
	void RefreshButton();
	int GetCurSelIndex();


	map<string, UINT64> *GetCheckedUser();
	void ClearCheckedUser();


};
