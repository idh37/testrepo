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

	LSY::CSpriteWindows				*m_pEventInfoWnd;//신평호 테스트

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
	
	// 프레임 동기화 관련 
	DWORD				m_delta;
	DWORD				m_curtime;
	DWORD				m_prevtime;
	void				ResetTime()	{	m_delta = m_curtime = m_prevtime = 0;	}

	virtual const int GetWidth();
	virtual const int GetHeight();

	
	//void SetMyAvatar(short *pAvatarInfo);
	void SetMyUserInfo(USERINFO *pMyInfo, BOOL bLogin);						//내정보를 세팅한다.(로그인성공시 한번만 호출)
	void AddChatText(const char *pMsg, COLORREF color);			// 채팅메세지 
	void ShowGameRoomInfo(GAMEINFO *pGI);						// 게임룸 정보를 보여준다.
	void CloseGameRoomInfo();									// 게임룸 정보창을 닫는다.
	MONEYRANGE *GetCurrentUserListMoneyRange();					// 유저리스트의 머니범위 콤보박스에 선택된 머니범위 정보를 가져온다.
	IDX_GAME GetCurrentUserListGame();							// 유저리스트의 게임필터 콤보박스에 선택된 게임 정보를 가져온다.
	void ShowLoadingRoom(BOOL bLoading);						// 룸리스트가 로딩중이라는 애니메이션을 ON/OFF한다.
	void ShowLoadingUser(BOOL bLoading);						// 유저리스트가 로딩중이라는 애니메이션을 ON/OFF한다.

	void SetChanceInfo(CRewardItemMan::REWARD_TYPE eRewardType,int nRewardItemCode, COleDateTime &timeExpire);		//찬스아이템 버튼을 위한 찬스정보 설정
	void SetGameTab(IDX_GAME idxGame);

	void SetGoldDisable();										// 골드 경기장 없는 게임은 탭 비활성 이미지 활성
	void SetFundEventEnable();									// 골드 적립금 이벤트 이미지 활성

	void ChangeRoomTab(ROOMLIST_TYPE eRoomListType);			//룸탭을 변경 (VIP방인지 노말방인지)
	void ShowRoomList(bool bShow);								//일반 룸 리스트를 보여줄지 토너먼트 룸리스트를 보여줄지 여부를 지정

	void OnEnterLobby(bool bRefresh);
	void OnLeaveLobby();

	SMALLUSERINFO *GetSelectedUserInfo();						//유저리스트에 컨텍스트 메뉴로 선택된 유저를 반환
	void ShowQJButton(IDX_GAME idxGame, BOOL bShow);								// 내맘대로 바로입장 버튼 활성화

	void EnableChat(BOOL bEnable);								//채팅창 활성/비활성화
	void SetFocusChat();										//채팅창에 포커스를 준다.
	void ClearChatHistory();									//채팅창 기록내용을 클리어한다.
	void SetFocusSearchUser();										//유저검색창에 포커스를 준다.
	void SetFocusSearchRoom();										//방검색창에 포커스를 준다.

	void RaiseChatView();

	BOOL ShowJackpotDisplay(BOOL bShow);									//잭팟금액 보기
	void SetJackpotDisplyInfo(jackpot::JP_MONEYINFO *pJackpotMoneyInfo);		//잭팟 현황판 정보 세팅
	void SetJackpotRelease();

	// 하프잭팟 관련 함수
	void SetHalfJackpotInit();
	void SetHalfJackpotReady(UINT32 nTime);
	void SetHalfJackpotTimeStart(UINT32 nTime);
	void SetHalfJackpotTimeBeforeEnd();
	void SetHalfJackpotTimeEnd();
	void SetHalfJackpotAlarm(promotion::_HALFJACKPOT_ALARM* pAlarm );
	void SetHalfJackpotMoney(promotion::HALFJACKPOT_MONEY* pMoney) ;
	void SetHalfJackpotRoom(promotion::HALFJACKPOT_ROOM* pRoom);
	void SetHalfJackpotRoomEnd(promotion::HALFJACKPOT_PRIZE* pPrize);

	void RefreshLobbyWeb();										//웹영역 리프레쉬

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
	void SetSelectRoomNum(int nRoomNum); // 방 번호
	void RemoveSelectedRoom();		//방리스트에서 선택방을 제거한다.

	void SetTargetNotiPos(int x, int y);		//TargetNoti Set position


	//규제안 : 이용제한 안내 버튼
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
	
	//규제안 : 제함금액 안내 팝업
	void ShowUseLimitDlg();

	afx_msg void OnTimer(UINT nIDEvent);
	void HandleResponse(int Signal, int TotSize, char *lpData); // 서버 응답 핸들링
	
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

	void SetEventBannerURL(const char *szURL);	//대기실 이벤트 배너 영역 URL 세팅

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

