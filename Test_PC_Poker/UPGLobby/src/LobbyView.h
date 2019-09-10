#pragma once
#include "PopupMemu.h"

class CLobbyProcess;

class CTitleButtonGroup;
class CTabGame;
class CUserInfoGroup;
class CUserInfoDetailDlg;
class CJackpotGroup;
class CJackpotCurrentInfoDlg;
class CUserListGroup;
class CUserListItem;
class CRoomListGroup;
class CRoomListItem;
class CRoomListItemLowBD;
class CRoomListItemHoola;
class CRoomListItemJoin;
class CRoomListItemLowBDJoin;
class CRoomListItemHoolaJoin;
class CRoomInfoDialog;
class CChatGroup;
class CShopTabControl;
class CBuddyListGroup;
class CBuddyListItem;
class CCafeListGroup;
class CCafeListItem;
class CBlackBuddyListGroup;
class CBlackBuddyListItem;
class CReverseBuddyListGroup;
class CReverseBuddyListItem;
class CVipJackpotDlg;
class CVipJackpotGroup;
class CTargetNotiGroup;
class CCongNotiGroup;

namespace LSY
{
	class CMessage;
	class CImage;
	class CWeb;
}
//////////////////////////////////////////////////////////////////////////////
// CLobbyView
class UPG_LOBBY_EXPORT CLobbyView : public CWnd
{
public:
	typedef std::map<std::string, CChattingRoomDlg *>	LISTCHATTINGROOM;
	typedef std::list<CChattingRoomInviteDlg *>			LISTINVITECHATTING;
	typedef std::list<CMessageNotificationPopup *>		LISTMEMONOTIFYPOPUP;
protected:
	DECLARE_DYNAMIC(CLobbyView)

	CLobbyProcess					*m_pLobbyProcess;
	CTitleButtonGroup				*m_pTitleButtonGroup;
	
	CUserInfoGroup					*m_pUserInfoGroup;
	CUserInfoDetailDlg				*m_pUserInfoDetailDlg;
	CJackpotGroup					*m_pJackpotGroup;
	CJackpotCurrentInfoDlg			*m_pJackpotCurrentInfoDlg;
	CUserListGroup					*m_pUserListGroup;
	CUserListItem					*m_pUserListItem;
	CRoomListGroup					*m_pRoomListGroup;
	CRoomListItem					*m_pRoomListItem;
	CRoomListItemLowBD				*m_pRoomListItemLowBD;
	CRoomListItemHoola				*m_pRoomListItemHoola;
	CRoomListItemJoin				*m_pRoomListItemJoin; 
	CRoomListItemLowBDJoin			*m_pRoomListItemLowBDJoin;
	CRoomListItemHoolaJoin			*m_pRoomListItemHoolaJoin;
	CRoomInfoDialog					*m_pRoomInfoDlg;
	CChatGroup						*m_pChatGroup;
	CShopTabControl					*m_pShopTab;
	LSY::CImage						*m_pPCRoomEventBack;
	LSY::CWeb						*m_pPCRoomWeb;
	CPopupMemu						*m_pPopupMenu;
	LISTCHATTINGROOM				m_mapChattingDlg;
	LISTINVITECHATTING				m_listInviteChatting;
	LISTMEMONOTIFYPOPUP				m_listMemoNotifyPopup;
	CBuddyListGroup					*m_pBuddyListGroup;
	CBuddyListItem					*m_pBuddyListItem;
	CCafeListGroup					*m_pCafeListGroup;
	CCafeListItem					*m_pCafeListItem;
	CBlackBuddyListGroup			*m_pBlackBuddyListGroup;
	CBlackBuddyListItem				*m_pBlackBuddyListItem;
	CReverseBuddyListGroup			*m_pReverseBuddyListGroup;
	CReverseBuddyListItem			*m_pReverseBuddyListItem;

	CVipJackpotDlg					*m_pVipJackpotDlg;
	CVipJackpotGroup				*m_pVipJackpotGroup;

	CTargetNotiGroup				*m_pTargetNotiGroup;
	CCongNotiGroup					*m_pCongNotiGroup;

	LSY::CSpriteWindows				*m_pEventInfoWnd;//����ȣ �׽�Ʈ

	LSY::CButton					*m_pBtnComebackPromotion;
	CUPGDlgBase						*m_pComeBackDlg;
	LSY::CButton					*m_pBtnChickenPromotion;

	LSY::CButton* m_chuSeokTest;

public:
	LSY::CImage						*m_pImgFundEventVIP;
	LSY::CImage						*m_pImgFundTooltipVIP;
	LSY::CImage						*m_pImgFundEventNormal;
	LSY::CImage						*m_pImgFundTooltipNormal;
	LSY::CImage						*m_pImgVIPGoldDisable;
	LSY::CImage						*m_pImgNormalGoldDisable;

public:
	CLobbyView();
	virtual ~CLobbyView();

	void MessageLooping();

	// Overrides
	//{{AFX_VIRTUAL(CLobbyView)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetLobyProcess(CLobbyProcess *pLobbyProcess);
	CLobbyProcess *GetLobbyProcess(void){return m_pLobbyProcess;}

	//virtual BOOL Create(CWnd *pParentWnd);
	
	CTabGame						*m_pTabGame;

protected:
	virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam );
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_VIRTUAL

public:
	//External Function
	void PopupShopDialog( int nTab, LPCTSTR url );
	void PopupEventDialog( int nTab );
	void PopupEventURLDialog ( LPCSTR url);
	DECLARE_DISPATCH_MAP()

	//{{AFX_MSG(CLobbyView)
protected:
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	//}}AFX_MSG

protected:
	DECLARE_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
public:
	void StartTimer(void);
	void StopTimer(void);

protected:
	UINT				m_hEventTimer;	// loop timer

	CRect				m_rectMoveAble;	// Window Moveable Rect

public:
	
	void OnChangeDisplayMode(BOOL bFullScr);
	// test ///////////////////////////////////////////////////////////////////////
public:
	int m_nMPosX,		m_nMPosY;
	
	// ������ ����ȭ ���� 
	DWORD				m_delta;
	DWORD				m_curtime;
	DWORD				m_prevtime;
	void				ResetTime()	{	m_delta = m_curtime = m_prevtime = 0;	}

	virtual const int GetWidth();
	virtual const int GetHeight();

	
	//void SetMyAvatar(short *pAvatarInfo);
	void SetMyUserInfo(USERINFO *pMyInfo, BOOL bLogin);						//�������� �����Ѵ�.(�α��μ����� �ѹ��� ȣ��)
	void AddChatText(const char *pMsg, COLORREF color);			// ä�ø޼��� 
	void ShowGameRoomInfo(GAMEINFO *pGI);						// ���ӷ� ������ �����ش�.
	void CloseGameRoomInfo();									// ���ӷ� ����â�� �ݴ´�.
	MONEYRANGE *GetCurrentUserListMoneyRange();					// ��������Ʈ�� �ӴϹ��� �޺��ڽ��� ���õ� �ӴϹ��� ������ �����´�.
	IDX_GAME GetCurrentUserListGame();							// ��������Ʈ�� �������� �޺��ڽ��� ���õ� ���� ������ �����´�.
	void ShowLoadingRoom(BOOL bLoading);						// �븮��Ʈ�� �ε����̶�� �ִϸ��̼��� ON/OFF�Ѵ�.
	void ShowLoadingUser(BOOL bLoading);						// ��������Ʈ�� �ε����̶�� �ִϸ��̼��� ON/OFF�Ѵ�.

	void SetChanceInfo(CRewardItemMan::REWARD_TYPE eRewardType,int nRewardItemCode, COleDateTime &timeExpire);		//���������� ��ư�� ���� �������� ����
	void SetGameTab(IDX_GAME idxGame);

	void SetGoldDisable();										// ��� ����� ���� ������ �� ��Ȱ�� �̹��� Ȱ��
	void SetFundEventEnable();									// ��� ������ �̺�Ʈ �̹��� Ȱ��

	void ChangeRoomTab(ROOMLIST_TYPE eRoomListType);			//������ ���� (VIP������ �븻������)
	void ShowRoomList(bool bShow);								//�Ϲ� �� ����Ʈ�� �������� ��ʸ�Ʈ �븮��Ʈ�� �������� ���θ� ����

	void OnEnterLobby(bool bRefresh);
	void OnLeaveLobby();

	SMALLUSERINFO *GetSelectedUserInfo();						//��������Ʈ�� ���ؽ�Ʈ �޴��� ���õ� ������ ��ȯ
	void ShowQJButton(IDX_GAME idxGame, BOOL bShow);								// ������� �ٷ����� ��ư Ȱ��ȭ

	void EnableChat(BOOL bEnable);								//ä��â Ȱ��/��Ȱ��ȭ
	void SetFocusChat();										//ä��â�� ��Ŀ���� �ش�.
	void ClearChatHistory();									//ä��â ��ϳ����� Ŭ�����Ѵ�.
	void SetFocusSearchUser();										//�����˻�â�� ��Ŀ���� �ش�.
	void SetFocusSearchRoom();										//��˻�â�� ��Ŀ���� �ش�.

	void RaiseChatView();

	BOOL ShowJackpotDisplay(BOOL bShow);									//���̱ݾ� ����
	void SetJackpotDisplyInfo(jackpot::JP_MONEYINFO *pJackpotMoneyInfo);		//���� ��Ȳ�� ���� ����
	void SetJackpotRelease();

	// �������� ���� �Լ�
	void SetHalfJackpotInit();
	void SetHalfJackpotReady(UINT32 nTime);
	void SetHalfJackpotTimeStart(UINT32 nTime);
	void SetHalfJackpotTimeBeforeEnd();
	void SetHalfJackpotTimeEnd();
	void SetHalfJackpotAlarm(promotion::_HALFJACKPOT_ALARM* pAlarm );
	void SetHalfJackpotMoney(promotion::HALFJACKPOT_MONEY* pMoney) ;
	void SetHalfJackpotRoom(promotion::HALFJACKPOT_ROOM* pRoom);
	void SetHalfJackpotRoomEnd(promotion::HALFJACKPOT_PRIZE* pPrize);

	void RefreshLobbyWeb();										//������ ��������

	void RoomListSortDefault(void);

	void SetMaintenanceGame(IDX_GAME idxGame, BOOL bMaintenance, BOOL bAdmin);

	void AllCloseChattingDlg(void);
	void AllDisconnectDisableChattingDlg(void);
	void AllChangeCaptionChattingDlg(void);
	void CloseChattingDlg(CChattingRoomDlg *pDlg);
	bool IsShowChattingDlg(CChattingRoomDlg *pDlg);

	bool IsFullCreateRoom(void);
	bool CreateCattingRoom(const std::string &strRoomID, const std::list<std::string> &listInviteID, const bool &bMaster);
	CChattingRoomDlg *GetChattingRoom(std::string strRoomID);

	void AllCloseInviteChatting(void);
	void CloseInviteChatting(CChattingRoomInviteDlg *pDlg, UINT nResult);
	bool CreateInviteCatting(std::string strRoomID, std::string strSenderID);
	bool IsExistInviteChatting(const std::string &strRoomID, const std::string &strSenderID);

	void AllCloseMemoNotifyPopup(void);
	void CloseMemoNotifyPopup(CMessageNotificationPopup *pWnd);
	bool CreateMemoNotifyPopup(MEMO *pMemo);
	bool CreateMemoNotifyPopup(std::string strConnectBuddyNickName);

	void OnClickPopupMenu(UINT nID);
	void SetSelectID(const std::string &strID);
	void SetSelectNickName(const std::string &strNickName);
	void SetSelectRoomNum(int nRoomNum); // �� ��ȣ
	void RemoveSelectedRoom();		//�渮��Ʈ���� ���ù��� �����Ѵ�.

	void SetTargetNotiPos(int x, int y);		//TargetNoti Set position


	//������ : �̿����� �ȳ� ��ư
	void SetShowUseLimitBtn(bool bShow);
	//yoo
	LRESULT OnShowRestrictionButton(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnHideRestrictionButton(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnChangeRestrictionButton(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnShowRestriction(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnShowRestrictionOver(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnShowRestrictionPlayOver(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnShowPromotionComeBack(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnShowPromotionComeBackDlg(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnShowPromotionComeBackDlgUpdate(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnClickButton(LSY::CMessage *msg);
	
	//������ : ���Աݾ� �ȳ� �˾�
	void ShowUseLimitDlg();

	afx_msg void OnTimer(UINT nIDEvent);
	void HandleResponse(int Signal, int TotSize, char *lpData); // ���� ���� �ڵ鸵
	
protected:
	bool IsMemoNotifyUsePos(const CPoint &ptPos);
	CPoint GetFirstHideMemoNotifyPopupPos(const CSize &szSize, const CPoint &ptStart);
	CMessageNotificationPopup *GetPosToLastHideMemoNotifyPopup(const CPoint &ptPos);

public:
	//void SetRoomListJoinable( bool bset );
	//bool GetEnableJoinable();

	LRESULT OnActivePCEvent(WPARAM &wParam, LPARAM &lParam);

	LRESULT OnCallCreateObject(LSY::CMessage *msg);
	LRESULT OnReturn(LSY::CMessage *msg);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnEnable(BOOL bEnable);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	void SetEventBannerURL(const char *szURL);	//���� �̺�Ʈ ��� ���� URL ����

	LRESULT OnSetEventBannerURL(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnSendCreateChatting(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnCheckShowCommunityWindow(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnAddWhisper(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnSetPokerIcon(WPARAM &wParam, LPARAM &lParam);

	LRESULT OnSetUserListTabType(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnGetUserListTabType(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnSetUserListTabActive(WPARAM &wParam, LPARAM &lParam);

	LRESULT OnChangeMyUserInfo(WPARAM &wParam, LPARAM &lParam);

	LRESULT OnChangePromotionLobby(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnShowChickenPromotionBtn(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnShowGoldBackPromotionBtn(WPARAM &wParam, LPARAM &lParam);	
	LRESULT OnChangeGoldTabState(WPARAM &wParam, LPARAM &lParam);

	LRESULT OnSetFundEventEnable(WPARAM &wParam, LPARAM &lParam);

	friend class CMainFrame;
};

