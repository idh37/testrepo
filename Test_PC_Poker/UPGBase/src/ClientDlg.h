// ClientDlg.h : header file
//

#if !defined(AFX_CLIENTDLG_H__02E2540E_E273_44EB_AB54_E23355729B9A__INCLUDED_)
#define AFX_CLIENTDLG_H__02E2540E_E273_44EB_AB54_E23355729B9A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CMainFrame dialog

#include "MsgDlg.h"
#include "../../UPGCommon/src/MsgBoxModelessDlg.h"
#include <../../UPGLobby/src/Lobby.h>
#include "../../UPGLobby/src/Object/Captcha/CaptchaDlg.h"

// [게임아이템창 리뉴얼] 2006.04.18
// 라이브러리 헤더 추가

// -> 아래 추가
#define MAX_BADUSERDELAYTIME 60000	// 신고기능 딜레이 시간(1분)
#define LOGIN_COOKIE_REFRESH_TIME (1000*60*30)
#define CAPTIONBAR_HEIGHT 28

class CMainFrame : public CFrameWnd
{
public:
	BOOL m_bExamServer; // 연습게임서버에 접속했다!!
	int m_nItemServer; // 아이템게임서버에 접속했다!! [게임 아이템 작업] [점프 아이템 작업]
	
	std::list<CWnd *> m_listModalDlg;
	
	UINT m_nTimerBadUser; // 112 신고 타이머
	NMBASE::SOCK::CSockCmnd m_SockCmnd; // 소켓 메세지 처리 클래스
	
	CLoadingDlg m_dlgLoading; // 로딩창
	CMsgDlg m_dlgLogoutMsg; // 로그아웃 표시 대화상자
	
	CInviteDlg m_dlgInvite; // 초대하기 다이얼로그
	CObserverDlg m_dlgObserver;
	CWhisperDlg m_dlgWhisper; // 귓말하기 다이얼로그
	
	CDailyPopup m_dlgDailyPopup;
	CEndEventPopupDlg m_dlgEndEvent; // 이벤트 종료 다이얼로그
	CRestrictionPopup m_dlgRestriction; // 이벤트 종료 다이얼로그
	
	CProfileDlg m_dlgProfile; // 프로필 다이얼로그
	CReceiveMessageDlg m_dlgReceiveMessage; // 받은쪽지 다이얼로그
	CReceiveAdminMessageDlg	m_dlgReceiveAdminMessage; //Admin 받은쪽지 다이알로그
	CSendMessageDlg m_dlgSendMessage; // 쪽지보내기 다이얼로그
	CMessageBox m_dlgMessageBox; // 쪽지함 다이얼로그
	CGoldChangeDlg m_dlgGoldChange; // 골드교환 다이얼로그
	CGoldBigWheelDlg m_dlgGoldBigWheel; // 골드빅휠 다이얼로그
	CClubDlg m_dlgClub; // 클럽 다이얼로그
	
	CUPPDlg m_dlgUPP; // UPP 2017.04
	
	CLobby m_cLobby;
	
	BOOL m_bAskLogout; // 게임 서버에게 로그아웃 요청한 상태인가?
	BOOL m_bLogoutbtnclick; // 욜라 빠른 클릭으로 아웃을 요청했냐.
	CMsgBoxDlg m_dlgMsgBox;
	CMsgBoxChkDlg m_dlgMsgBoxChk;
	CMsgBoxModelessDlg m_modelessMsgBoxDlg;
	
	BOOL m_bGroupLimit; //채널 입장 제한 여부
	
	// [게임아이템창 리뉴얼] 2006.04.18
	CNewGameItemDlg m_dlgNewGameItem; // 게임 아이템 대화상자	[게임 아이템 작업]
	CNewAskChargeDlg m_dlgGradeInsu; // 등급보험[올인보험개편]
	CBuyTimeLimitAvatarDlg m_dlgTimeLimitAvatar ;
	
	NMBASE::UTIL::CWebCtrl *m_pLocWebCtrl; // <- 추가
	
	//보유한도 보상
	BOOL m_bPostBrowserMove;
	CString m_strURLPost;
	bool m_bShowFirst;
	
	bool m_bShowTournamentMergeDlg;
		
	// 설 이벤트 팝업
	CGoldLuckyBagPopup m_popGoldLuckyBag;
	CNewYearAttendancePopup m_popNewYearAttendance;
	
	// 골드 적립 팝업
	CGoldMileagePopup m_popGoldMileage;
	
	// 골드 지원금 팝업
	CGoldSupportFundPopup m_popGoldSupportFund;

	// 골드 출석부 팝업
	CGoldAttendanceBookPopup m_popGoldAttendanceBook;

	// 칩 출석부 팝업
	CChipAttendPopup m_chipAttendPopup;

	// 골드 적립 통장 팝업
	CGoldWithdrawPopup m_popGoldWithdraw;

	CCaptchaDlg m_captchaDlg;
	CChuSeokDlg m_chuSeokDlg;
	CChuSeokPopup m_popChuSeok;
	CLobbyNotifyDlg m_lobbyNotifyDlg; // 로비에서 뜨는 공지 팝업

	BOOL m_bLoginToLobby;

public:	
	void CheckConfigDirectory();
	BOOL LoadConfig(LPCTSTR id);
	BOOL SaveConfig(LPCTSTR id);
	void SetupByConfig();
	void ChangeView(IDX_GAMEWHERE nWhere, bool bRefresh);
	
	void BrowserMove(LPCTSTR targeturl); // [게임 아이템 작업]
	void ShowItemState(); // [게임 아이템 작업]
	
	BOOL CheckGameMoney(INT64 Money);
	
	POINT GetWindowMousePos();
	void MoveToLobby();
	void OnDraw(void);

// Construction
public:
	CMainFrame(); // standard constructor
	virtual ~CMainFrame();

	HICON m_hIcon;

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual void WinHelp(DWORD dwData, UINT nCmd = HELP_CONTEXT);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX); // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	//펑터 콜 함수들
	LRESULT OnAvatarShop(WPARAM &, LPARAM &); // [아이템 유료화 영역]
	LRESULT OnChanceItemShop(WPARAM &, LPARAM &); // [아이템 유료화 영역]
	LRESULT OnItemShop(WPARAM &, LPARAM &); // [아이템 유료화 영역]
	LRESULT OnShowChanceBtn(WPARAM &, LPARAM &); // [아이템 유료화 영역]
	LRESULT OnChangeView(WPARAM &, LPARAM &);
	LRESULT OnClickLobbyExitButton(WPARAM &, LPARAM &);
	LRESULT OnShowEndingPopup(WPARAM &, LPARAM &);
	LRESULT OnSetMainWindowText(WPARAM &, LPARAM &);
	LRESULT OnShowMainMessage(WPARAM &, LPARAM &);
	LRESULT OnCloseMainMessage(WPARAM &, LPARAM &);
	LRESULT OnClearMainMessage(WPARAM &, LPARAM &);
	LRESULT OnIsVisibleMainMessage(WPARAM &, LPARAM &);
	LRESULT OnShowMsgChk(WPARAM &, LPARAM &);
	LRESULT OnSaveConfig(WPARAM &, LPARAM &);
	LRESULT OnLoadConfig(WPARAM &, LPARAM &);
	LRESULT OnGetObserverDlg(WPARAM &, LPARAM &);
	LRESULT OnGetInvateDlg(WPARAM &, LPARAM &);
	LRESULT OnGetEndEventDlg(WPARAM &, LPARAM &);
	LRESULT OnGetRestrictionDlg(WPARAM &, LPARAM &);
	LRESULT OnGetProfileDlg(WPARAM &, LPARAM &);
	LRESULT OnGetWhisperDlg(WPARAM &, LPARAM &);
	LRESULT OnGetRecevieMessageDlg(WPARAM &, LPARAM &);
	LRESULT OnGetRecevieAdminMessageDlg(WPARAM &, LPARAM &);
	LRESULT OnGetSendMessageDlg(WPARAM &, LPARAM &);
	LRESULT OnGetMessageBoxDlg(WPARAM &, LPARAM &);
	LRESULT OnGetGoldChangeDlg(WPARAM &, LPARAM &);
	LRESULT OnGetGoldBigWheelDlg(WPARAM &, LPARAM &);
	LRESULT OnGetClubDlg(WPARAM &, LPARAM &);
	LRESULT OnAddLobbyChatViewMessage(WPARAM &, LPARAM &);
	LRESULT OnResetBadUserTimer(WPARAM &, LPARAM &);
	LRESULT OnChangeDisplayMode(WPARAM &, LPARAM &);
	LRESULT OnScreenCapture(WPARAM &, LPARAM &);
	LRESULT OnCheckBadUserReport(WPARAM &, LPARAM &);
	LRESULT OnShowNewAskChargeDlg(WPARAM &, LPARAM &);
	LRESULT OnShowTipManDlg(WPARAM &, LPARAM &);
	LRESULT OnShowDirectGoDlg(WPARAM &, LPARAM &);
	LRESULT OnCloseLoadingDlg(WPARAM &, LPARAM &);
	LRESULT OnGameItemDlgRefresh_MyItem(WPARAM &, LPARAM &);
	LRESULT OnCloseGameItemDlg(WPARAM &, LPARAM &);
	LRESULT OnShowGameItemDlg(WPARAM &, LPARAM &);
	LRESULT OnShowTimeLimitItemDlg(WPARAM &, LPARAM &);
	LRESULT OnCloseTimeLimitItemDlg(WPARAM &, LPARAM &);
	LRESULT OnGetUserSmallInfo(WPARAM &, LPARAM &);
	LRESULT OnBadUserComplete(WPARAM &, LPARAM &); // [넷마블 보안관]
	LRESULT OnDestroyLoadingDialog(WPARAM &, LPARAM &);
	LRESULT OnShowMessageImageBox(WPARAM &, LPARAM &);
	LRESULT OnShowExplorer(WPARAM &, LPARAM &);
	LRESULT OnLoadingEnd(WPARAM &, LPARAM &);
	LRESULT OnClearLoadImageList(WPARAM &, LPARAM &);
	LRESULT OnAddLoadImageList(WPARAM &, LPARAM &);
	LRESULT OnLoadingImage(WPARAM &, LPARAM &);
	LRESULT OnOpenHiddenURL(WPARAM &, LPARAM &);
	LRESULT OnSetPokerIcon(WPARAM &, LPARAM &);
	LRESULT OnCreateDesctopIcon(WPARAM &, LPARAM &);
	LRESULT OnUpdateItemData(WPARAM &, LPARAM &);
	LRESULT OnSetVolume(WPARAM &, LPARAM &);
	LRESULT OnShowDailyDlg(WPARAM &, LPARAM &);
	LRESULT OnShowEndEventDlg(WPARAM &, LPARAM &);

	//규제안 : 손실금액 팝업
	LRESULT OnShowRestrictionDlg(WPARAM &, LPARAM &);
	LRESULT OnHideRestrictionDlg(WPARAM &, LPARAM &);

	//규제안 : 선택입장 팝업
	LRESULT OnShowSelectStartDlg(WPARAM &, LPARAM &);
	LRESULT OnShowResetSelectStartDlg(WPARAM &, LPARAM &);

	LRESULT OnShowUPPDlg(WPARAM &, LPARAM &);
	LRESULT OnShowRewardOver(WPARAM &, LPARAM &);

	LRESULT OnShowProfileDlg(WPARAM &, LPARAM &);
	LRESULT OnCloseProfileDlg(WPARAM &, LPARAM &);
	LRESULT OnShowWhisperDlg(WPARAM &, LPARAM &);
	LRESULT OnCloseWhisperDlg(WPARAM &, LPARAM &);
	LRESULT OnShowReceiveMessageDlg(WPARAM &, LPARAM &);
	LRESULT OnCloseReceiveMessageDlg(WPARAM &, LPARAM &);
	LRESULT OnShowSendMessageDlg(WPARAM &, LPARAM &);
	LRESULT OnCloseSendMessageDlg(WPARAM &, LPARAM &);
	LRESULT OnShowMessageBoxDlg(WPARAM &, LPARAM &);
	LRESULT OnCloseMessageBoxDlg(WPARAM &, LPARAM &);
	LRESULT OnShowGoldChangeDlg(WPARAM &, LPARAM &);
	LRESULT OnShowGoldBigWheelDlg(WPARAM &, LPARAM &);
	LRESULT OnShowClubDlg(WPARAM &, LPARAM &);
	LRESULT OnGetGoldLuckyBagPopup(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnShowGoldLuckyBagPopup(WPARAM &wParam, LPARAM &lParam);
	
	// 2018 설 출석 이벤트 팝업
	LRESULT OnGetNewYearAttendancePopup(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnCreateNewYearAttendancePopup(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnShowNewYearAttendancePopup(WPARAM &wParam, LPARAM &lParam);
	
	// 골드 마일리지 팝업
	LRESULT OnGetGoldMileagePopup(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnCreateGoldMileagePopup(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnShowGoldMileagePopup(WPARAM &wParam, LPARAM &lParam);
	
	// 골드 지원금 팝업
	LRESULT OnGetGoldSupportFundPopup(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnCreateGoldSupportFundPopup(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnShowGoldSupportFundPopup(WPARAM &wParam, LPARAM &lParam);

	// 골드 출석부 팝업
	LRESULT OnGetGoldAttendanceBookPopup(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnCreateGoldAttendanceBookPopup(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnShowGoldAttendanceBookPopup(WPARAM &wParam, LPARAM &lParam);

	// 골드 적립 통장 팝업
	LRESULT OnGetGoldWithdrawPopup(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnShowGoldWithdrawPopup(WPARAM &wParam, LPARAM &lParam);

	LRESULT OnShowCaptchaDlg(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnCloseCaptchaDlg(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnGetCaptchaDlg(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnGetAuthentiCodeOfCaptcha(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnSetAuthentiCodeOfCaptcha(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnGetNeedCaptcha(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnSetNeedCaptcha(WPARAM &wParam, LPARAM &lParam);

	LRESULT OnGetChuSeokAttendanceDlg(WPARAM& wParam, LPARAM& lParam);
	LRESULT OnShowChuSeokAttendanceDlg(WPARAM& wParam, LPARAM& lParam);
	LRESULT OnGetChuSeokPopup(WPARAM& wParam, LPARAM& lParam);
	LRESULT OnShowChuSeokPopup(WPARAM& wParam, LPARAM& lParam);

	LRESULT OnGetLobbyNotifyDlg(WPARAM& wParam, LPARAM& lParam);
	LRESULT OnShowLobbyNotifyDlg(WPARAM& wParam, LPARAM& lParam);

	LRESULT OnShowMsgModelessDlg(WPARAM& wParam, LPARAM& lParam);
	
public:
	virtual BOOL	PreCreateWindow(CREATESTRUCT& cs);

	// Generated message map functions
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnOK();
	afx_msg void OnCancel();
	afx_msg LONG OnPacketNotify(UINT, LONG);
	afx_msg LONG OnSocketClose(UINT, LONG);
	afx_msg LONG OnFailConnect(UINT, LONG);	
	afx_msg LONG OnSuccessConnect(UINT, LONG);	
	afx_msg LONG OnClosePopup(UINT, LONG);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	int CheckAllProcessName();
	afx_msg void OnClose();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTDLG_H__02E2540E_E273_44EB_AB54_E23355729B9A__INCLUDED_)
