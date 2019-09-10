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

// [���Ӿ�����â ������] 2006.04.18
// ���̺귯�� ��� �߰�

// -> �Ʒ� �߰�
#define MAX_BADUSERDELAYTIME 60000	// �Ű��� ������ �ð�(1��)
#define LOGIN_COOKIE_REFRESH_TIME (1000*60*30)
#define CAPTIONBAR_HEIGHT 28

class CMainFrame : public CFrameWnd
{
public:
	BOOL m_bExamServer; // �������Ӽ����� �����ߴ�!!
	int m_nItemServer; // �����۰��Ӽ����� �����ߴ�!! [���� ������ �۾�] [���� ������ �۾�]
	
	std::list<CWnd *> m_listModalDlg;
	
	UINT m_nTimerBadUser; // 112 �Ű� Ÿ�̸�
	NMBASE::SOCK::CSockCmnd m_SockCmnd; // ���� �޼��� ó�� Ŭ����
	
	CLoadingDlg m_dlgLoading; // �ε�â
	CMsgDlg m_dlgLogoutMsg; // �α׾ƿ� ǥ�� ��ȭ����
	
	CInviteDlg m_dlgInvite; // �ʴ��ϱ� ���̾�α�
	CObserverDlg m_dlgObserver;
	CWhisperDlg m_dlgWhisper; // �Ӹ��ϱ� ���̾�α�
	
	CDailyPopup m_dlgDailyPopup;
	CEndEventPopupDlg m_dlgEndEvent; // �̺�Ʈ ���� ���̾�α�
	CRestrictionPopup m_dlgRestriction; // �̺�Ʈ ���� ���̾�α�
	
	CProfileDlg m_dlgProfile; // ������ ���̾�α�
	CReceiveMessageDlg m_dlgReceiveMessage; // �������� ���̾�α�
	CReceiveAdminMessageDlg	m_dlgReceiveAdminMessage; //Admin �������� ���̾˷α�
	CSendMessageDlg m_dlgSendMessage; // ���������� ���̾�α�
	CMessageBox m_dlgMessageBox; // ������ ���̾�α�
	CGoldChangeDlg m_dlgGoldChange; // ��屳ȯ ���̾�α�
	CGoldBigWheelDlg m_dlgGoldBigWheel; // ������ ���̾�α�
	CClubDlg m_dlgClub; // Ŭ�� ���̾�α�
	
	CUPPDlg m_dlgUPP; // UPP 2017.04
	
	CLobby m_cLobby;
	
	BOOL m_bAskLogout; // ���� �������� �α׾ƿ� ��û�� �����ΰ�?
	BOOL m_bLogoutbtnclick; // ��� ���� Ŭ������ �ƿ��� ��û�߳�.
	CMsgBoxDlg m_dlgMsgBox;
	CMsgBoxChkDlg m_dlgMsgBoxChk;
	CMsgBoxModelessDlg m_modelessMsgBoxDlg;
	
	BOOL m_bGroupLimit; //ä�� ���� ���� ����
	
	// [���Ӿ�����â ������] 2006.04.18
	CNewGameItemDlg m_dlgNewGameItem; // ���� ������ ��ȭ����	[���� ������ �۾�]
	CNewAskChargeDlg m_dlgGradeInsu; // ��޺���[���κ��谳��]
	CBuyTimeLimitAvatarDlg m_dlgTimeLimitAvatar ;
	
	NMBASE::UTIL::CWebCtrl *m_pLocWebCtrl; // <- �߰�
	
	//�����ѵ� ����
	BOOL m_bPostBrowserMove;
	CString m_strURLPost;
	bool m_bShowFirst;
	
	bool m_bShowTournamentMergeDlg;
		
	// �� �̺�Ʈ �˾�
	CGoldLuckyBagPopup m_popGoldLuckyBag;
	CNewYearAttendancePopup m_popNewYearAttendance;
	
	// ��� ���� �˾�
	CGoldMileagePopup m_popGoldMileage;
	
	// ��� ������ �˾�
	CGoldSupportFundPopup m_popGoldSupportFund;

	// ��� �⼮�� �˾�
	CGoldAttendanceBookPopup m_popGoldAttendanceBook;

	// Ĩ �⼮�� �˾�
	CChipAttendPopup m_chipAttendPopup;

	// ��� ���� ���� �˾�
	CGoldWithdrawPopup m_popGoldWithdraw;

	CCaptchaDlg m_captchaDlg;
	CChuSeokDlg m_chuSeokDlg;
	CChuSeokPopup m_popChuSeok;
	CLobbyNotifyDlg m_lobbyNotifyDlg; // �κ񿡼� �ߴ� ���� �˾�

	BOOL m_bLoginToLobby;

public:	
	void CheckConfigDirectory();
	BOOL LoadConfig(LPCTSTR id);
	BOOL SaveConfig(LPCTSTR id);
	void SetupByConfig();
	void ChangeView(IDX_GAMEWHERE nWhere, bool bRefresh);
	
	void BrowserMove(LPCTSTR targeturl); // [���� ������ �۾�]
	void ShowItemState(); // [���� ������ �۾�]
	
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
	//���� �� �Լ���
	LRESULT OnAvatarShop(WPARAM &, LPARAM &); // [������ ����ȭ ����]
	LRESULT OnChanceItemShop(WPARAM &, LPARAM &); // [������ ����ȭ ����]
	LRESULT OnItemShop(WPARAM &, LPARAM &); // [������ ����ȭ ����]
	LRESULT OnShowChanceBtn(WPARAM &, LPARAM &); // [������ ����ȭ ����]
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
	LRESULT OnBadUserComplete(WPARAM &, LPARAM &); // [�ݸ��� ���Ȱ�]
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

	//������ : �սǱݾ� �˾�
	LRESULT OnShowRestrictionDlg(WPARAM &, LPARAM &);
	LRESULT OnHideRestrictionDlg(WPARAM &, LPARAM &);

	//������ : �������� �˾�
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
	
	// 2018 �� �⼮ �̺�Ʈ �˾�
	LRESULT OnGetNewYearAttendancePopup(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnCreateNewYearAttendancePopup(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnShowNewYearAttendancePopup(WPARAM &wParam, LPARAM &lParam);
	
	// ��� ���ϸ��� �˾�
	LRESULT OnGetGoldMileagePopup(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnCreateGoldMileagePopup(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnShowGoldMileagePopup(WPARAM &wParam, LPARAM &lParam);
	
	// ��� ������ �˾�
	LRESULT OnGetGoldSupportFundPopup(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnCreateGoldSupportFundPopup(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnShowGoldSupportFundPopup(WPARAM &wParam, LPARAM &lParam);

	// ��� �⼮�� �˾�
	LRESULT OnGetGoldAttendanceBookPopup(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnCreateGoldAttendanceBookPopup(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnShowGoldAttendanceBookPopup(WPARAM &wParam, LPARAM &lParam);

	// ��� ���� ���� �˾�
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
