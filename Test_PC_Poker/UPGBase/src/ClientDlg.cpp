// ClientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ClientDlg.h"
#include "Global.h"

#include <Tlhelp32.h>
#include <assert.h>
// [넷마블 보안관]
#include <direct.h>
#include "../include/UPGBase.h"
#include "UserStruct.h"
#include "CommMsgString.h"

#ifdef _DEBUG // 테스트
#include "../src/LoginDlg.h"
#endif

#ifdef RELEASE_BUILD
#include "../src/LoginDlg.h"
#endif

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")

#include <imm.h>
#pragma comment(lib,"imm32.lib")

#include "CommonUserWndMsg.h"
#include "../src/VipService.h"

#include "NMContego/NMContegoManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

// CAboutDlg dialog used for App About


// [SSO 작업] - 기존 함수 교체
char* _StrTok(const char* str, const char sep)
{
	// strtok()와 같은 기능의 함수이지만 separator를 1개만 받a
	// 인자가 비어있는 경우도 인자로 인정함
	static const int TOK_BUFSIZE = 1024*40;
	static char* pnow = NULL;
	static char buf[TOK_BUFSIZE];
	static char token[TOK_BUFSIZE];
	if(str) {
		int slen = strlen(str);
		assert(slen < TOK_BUFSIZE);		// ASSERT
		strcpy(buf, str);
		pnow = buf;
	}
	if(!pnow || *pnow == 0)
		return NULL;

	int tlen = 0;
	char* porg = pnow;
	while(*pnow != 0)
	{
		if(*pnow == sep) {
			++pnow;
			break;
		}
		++tlen;
		++pnow;
	}
	if(tlen > 0) {
		strncpy(token, porg, tlen);
		token[tlen] = 0;
	}
	else {
		token[0] = 0;
	}
	return token;
}

class CAboutDlg : public CUPGDlgBase
{
public:
	CAboutDlg();
// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CUPGDlgBase(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CMainFrame dialog

CMainFrame::CMainFrame()
:	CFrameWnd(),
	m_bShowFirst(true),
	m_dlgMsgBox(this),
	m_bShowTournamentMergeDlg(false)
{	
	//AfxSetAllocStop(38915);

	GM().SetMainWnd(this);

	m_nTimerBadUser = NULL;
	m_bAskLogout = FALSE;
	ZeroMemory(&g_Config, sizeof(CONFIG));
	m_bExamServer = FALSE;
	m_nItemServer = 0;	
	m_pLocWebCtrl = NULL;

	//보유한도 보상
	m_bPostBrowserMove = FALSE;
	m_strURLPost = _T("");
	//{{AFX_DATA_INIT(CMainFrame)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = NULL;

	m_bLoginToLobby = FALSE;

	// GDK_SSOWEB_ITEMCODE 키를 가지는 영역에 set을 하지 않고 GetINT64함수를 이용해 GDK_SSOWEB_ITEMCODE 값을 가져오려는 경우에 에러가 생기므로
	// 메인프레임 생성자에서 초기화한다. 2012.02.17 최종학
	GDM().SetINT64(GDK_SSOWEB_ITEMCODE, 0);

	MM().AddMsgH(UPG_CMK_CHANGE_VIEW, Fnt(this, &CMainFrame::OnChangeView));
	MM().AddMsgH(UPG_CMK_SET_MAINWINDOW_TEXT, Fnt(this, &CMainFrame::OnSetMainWindowText));
	MM().AddMsgH(UPG_CMK_CLICK_LOBBY_EXIT_BUTTON, Fnt(this, &CMainFrame::OnClickLobbyExitButton));
	MM().AddMsgH(UPG_CMK_SHOW_ENDING_POPUP, Fnt(this, &CMainFrame::OnShowEndingPopup));

	MM().AddMsgH(UPG_CMK_MAIN_MESSAGE_SHOW, Fnt(this, &CMainFrame::OnShowMainMessage));
	MM().AddMsgH(UPG_CMK_MAIN_MESSAGE_CLOSE, Fnt(this, &CMainFrame::OnCloseMainMessage));
	MM().AddMsgH(UPG_CMK_MAIN_MESSAGE_CLEAR, Fnt(this, &CMainFrame::OnClearMainMessage));
	MM().AddMsgH(UPG_CMK_MAIN_MESSAGE_ISVISIBLE, Fnt(this, &CMainFrame::OnIsVisibleMainMessage));

	MM().AddMsgH(UPG_CMK_MSG_CHK_SHOW, Fnt(this, &CMainFrame::OnShowMsgChk));

	MM().AddMsgH(UPG_CMK_SAVE_CONFIG, Fnt(this, &CMainFrame::OnSaveConfig));
	MM().AddMsgH(UPG_CMK_LOAD_CONFIG, Fnt(this, &CMainFrame::OnLoadConfig));

	MM().AddMsgH(UPG_CMK_GET_OBSERVER_DLG, Fnt(this, &CMainFrame::OnGetObserverDlg));
	MM().AddMsgH(UPG_CMK_GET_INVITE_DLG, Fnt(this, &CMainFrame::OnGetInvateDlg));
	MM().AddMsgH(UPG_CMK_GET_ENDEVENT_DLG, Fnt(this, &CMainFrame::OnGetEndEventDlg));
	MM().AddMsgH(UPG_CMK_GET_RESTRICTION_DLG, Fnt(this, &CMainFrame::OnGetRestrictionDlg));	
	
	MM().AddMsgH(UPG_CMK_GET_PROFILE_DLG, Fnt(this, &CMainFrame::OnGetProfileDlg));
	MM().AddMsgH(UPG_CMK_GET_WHISPER_DLG, Fnt(this, &CMainFrame::OnGetWhisperDlg));
	MM().AddMsgH(UPG_CMK_GET_RECEIVEMESSSAGE_DLG, Fnt(this, &CMainFrame::OnGetRecevieMessageDlg));
	MM().AddMsgH(UPG_CMK_GET_RECEIVEADMINMESSSAGE_DLG, Fnt(this, &CMainFrame::OnGetRecevieAdminMessageDlg));
	MM().AddMsgH(UPG_CMK_GET_SENDMESSSAGE_DLG, Fnt(this, &CMainFrame::OnGetSendMessageDlg));
	MM().AddMsgH(UPG_CMK_GET_MESSSAGEBOX_DLG, Fnt(this, &CMainFrame::OnGetMessageBoxDlg));
	
	MM().AddMsgH(UPG_CMK_GET_GOLDCHANGE_DLG, Fnt(this, &CMainFrame::OnGetGoldChangeDlg));
	MM().AddMsgH(UPG_CMK_GET_GOLDBIGWHEEL_DLG, Fnt(this, &CMainFrame::OnGetGoldBigWheelDlg));
	MM().AddMsgH(UPG_CMK_GET_CLUB_DLG, Fnt(this, &CMainFrame::OnGetClubDlg));
	
	MM().AddMsgH(UPG_CMK_SHOW_UPP_DLG, Fnt(this, &CMainFrame::OnShowUPPDlg));
	MM().AddMsgH(UPG_CMK_SHOW_REWARD_DAILYLOSS_OVER, Fnt(this, &CMainFrame::OnShowRewardOver));
	
	MM().AddMsgH(UPG_CMK_RESET_BADUSERTIMER, Fnt(this, &CMainFrame::OnResetBadUserTimer));
	MM().AddMsgH(UPG_CMK_ADD_LOBBY_CHATVIEW_MESSAGE, Fnt(this, &CMainFrame::OnAddLobbyChatViewMessage));
	MM().AddMsgH(UPG_CMK_CHANGE_DISPLAY_MODE, Fnt(this, &CMainFrame::OnChangeDisplayMode));
	MM().AddMsgH(UPG_CMK_SCREEN_CAPTURE, Fnt(this, &CMainFrame::OnScreenCapture));
	
	MM().AddMsgH(UPG_CMK_SHOW_TIPMAN_DLG, Fnt(this, &CMainFrame::OnShowTipManDlg));
	MM().AddMsgH(UPG_CMK_SHOW_DIRECTGO_DLG, Fnt(this, &CMainFrame::OnShowDirectGoDlg));
	
	MM().AddMsgH(UPG_CMK_CHECK_BADUSER_REPORT, Fnt(this, &CMainFrame::OnCheckBadUserReport));
	MM().AddMsgH(UPG_CMK_SHOW_NEWASKCHARGEDLG, Fnt(this, &CMainFrame::OnShowNewAskChargeDlg));
	MM().AddMsgH(UPG_CMK_GAMEITEM_DLG_REFRESH_MYITEM, Fnt(this, &CMainFrame::OnGameItemDlgRefresh_MyItem));
	MM().AddMsgH(UPG_CMK_GAMEITEM_DLG_CLOSE, Fnt(this, &CMainFrame::OnCloseGameItemDlg));
	MM().AddMsgH(UPG_CMK_GAMEITEM_DLG_SHOW, Fnt(this, &CMainFrame::OnShowGameItemDlg));
	MM().AddMsgH(UPG_CMK_GET_USER_SMALLINFO, Fnt(this, &CMainFrame::OnGetUserSmallInfo));
	
	MM().AddMsgH(UPG_CMK_TIMELIMITITEM_DLG_SHOW, Fnt(this, &CMainFrame::OnShowTimeLimitItemDlg));
	MM().AddMsgH(UPG_CMK_TIMELIMITITEM_DLG_CLOSE, Fnt(this, &CMainFrame::OnCloseTimeLimitItemDlg));
	
	MM().AddMsgH(UPG_CMK_AVATAR_SHOP, Fnt(this, &CMainFrame::OnAvatarShop)); // [아이템 유료화 영역]
	MM().AddMsgH(UPG_CMK_CHANCEITEMSHOP, Fnt(this, &CMainFrame::OnChanceItemShop)); // [아이템 유료화 영역]
	MM().AddMsgH(UPG_CMK_ITEMSHOP, Fnt(this, &CMainFrame::OnItemShop)); // [아이템 유료화 영역]
	
	MM().AddMsgH(UPG_CMK_BADUSERCOMPLETE, Fnt(this, &CMainFrame::OnBadUserComplete)); // [넷마블 보안관]
	MM().AddMsgH(UPG_CMK_ONDESTROY_LOADINGDIALOG, Fnt(this, &CMainFrame::OnDestroyLoadingDialog)); // [넷마블 보안관]
	
	MM().AddMsgH(UPG_CMK_SHOW_MESSAGE_IMAGE_BOX, Fnt(this, &CMainFrame::OnShowMessageImageBox));
	MM().AddMsgH(UPG_CMK_SHOW_EXPLORER, Fnt(this, &CMainFrame::OnShowExplorer));
	MM().AddMsgH(UPG_CMK_SPRITE_LOADING_END, Fnt(this, &CMainFrame::OnLoadingEnd));
	
	MM().AddMsgH(UPG_CMK_LOAD_IMAGE_LIST_CLEAER, Fnt(this, &CMainFrame::OnClearLoadImageList));
	MM().AddMsgH(UPG_CMK_LOAD_IMAGE_LIST_ADD, Fnt(this, &CMainFrame::OnAddLoadImageList));
	MM().AddMsgH(UPG_CMK_LOADING_IMAGE_LIST, Fnt(this, &CMainFrame::OnLoadingImage));
	
	MM().AddMsgH(UPG_CMK_SHOW_CHANCE_BUTTON, Fnt(this, &CMainFrame::OnShowChanceBtn));
	MM().AddMsgH(UPG_CMK_OPEN_HIDDEN_URL, Fnt(this, &CMainFrame::OnOpenHiddenURL));	// 히든 URL(로그인시 친구찾기URL) 호출
	
	MM().AddMsgH(UPG_CMK_SETPOKERICON, Fnt(this, &CMainFrame::OnSetPokerIcon));	// 작업 표시줄 아이콘 변경
	MM().AddMsgH(UPG_CMK_CREATEDESKTOPICON, Fnt(this, &CMainFrame::OnCreateDesctopIcon)); // 
	MM().AddMsgH(UPG_CMK_UPDATE_ITEM_DATA, Fnt(this, &CMainFrame::OnUpdateItemData)); // 아이템 상태가 변경되었을때 호출
	
	MM().AddMsgH(UPG_CMK_OPTION_SETVOLUME, Fnt(this, &CMainFrame::OnSetVolume)); // 볼륨을 조정한다
	
	MM().AddMsgH(UPG_CMK_SHOW_DAILYPOPUP_DIALOG, Fnt(this, &CMainFrame::OnShowDailyDlg)); // 시작 팝업 창을 연다.
	MM().AddMsgH(UPG_CMK_SHOW_ENDEVENT_DIALOG, Fnt(this, &CMainFrame::OnShowEndEventDlg)); // 종료 팝업 창을 연다.
	
	MM().AddMsgH(UPG_CMK_SHOW_SELECTSTART_DLG, Fnt(this, &CMainFrame::OnShowSelectStartDlg));
	MM().AddMsgH(UPG_CMK_SHOW_RESETSELECTSTART_DLG, Fnt(this, &CMainFrame::OnShowResetSelectStartDlg));
	
	// 규제안 : 손실금액
	MM().AddMsgH(UPG_CMK_SHOW_RESTRICTION_DIALOG, Fnt(this, &CMainFrame::OnShowRestrictionDlg)); 
	MM().AddMsgH(UPG_CMK_HIDE_RESTRICTION_DIALOG, Fnt(this, &CMainFrame::OnHideRestrictionDlg)); 
	
	MM().AddMsgH(UPG_CMK_SHOW_PROFILE_DIALOG, Fnt(this, &CMainFrame::OnShowProfileDlg)); // 프로필 창을 연다.
	MM().AddMsgH(UPG_CMK_CLOSE_PROFILE_DIALOG, Fnt(this, &CMainFrame::OnCloseProfileDlg)); // 프로필 창을 닫는다.
	
	MM().AddMsgH(UPG_CMK_SHOW_WHISPER_DIALOG, Fnt(this, &CMainFrame::OnShowWhisperDlg)); // 귓말 보내기 창을 연다
	MM().AddMsgH(UPG_CMK_CLOSE_WHISPER_DIALOG, Fnt(this, &CMainFrame::OnCloseWhisperDlg)); // 귓말 보내기 창을 닫는다.
	
	MM().AddMsgH(UPG_CMK_SHOW_RECEIVEMESSAGE_DIALOG, Fnt(this, &CMainFrame::OnShowReceiveMessageDlg)); // 받은쪽지 창을 연다.
	MM().AddMsgH(UPG_CMK_CLOSE_RECEIVEMESSAGE_DIALOG, Fnt(this, &CMainFrame::OnCloseReceiveMessageDlg)); // 받은쪽지 창을 닫는다.
	
	// MM().AddMsgH(UPG_CMK_SHOW_SENDMESSAGE_DIALOG, Fnt(this, &CMainFrame::OnShowSendMessageDlg)); // 쪽지보내기 창을 연다.
	MM().AddMsgH(UPG_CMK_CLOSE_SENDMESSAGE_DIALOG, Fnt(this, &CMainFrame::OnCloseSendMessageDlg)); // 쪽지보내기 창을 닫는다.

	MM().AddMsgH(UPG_CMK_SHOW_MESSAGEBOX_DIALOG, Fnt(this, &CMainFrame::OnShowMessageBoxDlg)); // 쪽지함 창을 연다.
	MM().AddMsgH(UPG_CMK_CLOSE_MESSAGEBOX_DIALOG, Fnt(this, &CMainFrame::OnCloseMessageBoxDlg)); // 쪽지함 창을 닫는다.

	MM().AddMsgH(UPG_CMK_SHOW_GOLDCHANGE_DIALOG, Fnt(this, &CMainFrame::OnShowGoldChangeDlg)); // 골드교환 창을 연다.
	MM().AddMsgH(UPG_CMK_SHOW_GOLDBIGWHEEL_DIALOG, Fnt(this, &CMainFrame::OnShowGoldBigWheelDlg)); // 골드빅휠 창을 연다.
	MM().AddMsgH(UPG_CMK_SHOW_CLUB_DIALOG, Fnt(this, &CMainFrame::OnShowClubDlg)); // 클럽 창을 연다.

	MM().AddMsgH(UPG_CMK_CLOSE_LOADING_DLG, Fnt(this, &CMainFrame::OnCloseLoadingDlg));
	
	MM().AddMsgH(UPG_CMK_GET_GOLDLUCKYBAG_POPUP, Fnt(this, &CMainFrame::OnGetGoldLuckyBagPopup)); // 설 이벤트 황금 복주머니 팝업
	MM().AddMsgH(UPG_CMK_SHOW_GOLDLUCKYBAG_POPUP, Fnt(this, &CMainFrame::OnShowGoldLuckyBagPopup)); // 설 이벤트 황금 복주머니 팝업
	MM().AddMsgH(UPG_CMK_GET_NEWYEARATTENDANCE_POPUP, Fnt(this, &CMainFrame::OnGetNewYearAttendancePopup)); // 설 이벤트 출석 팝업
	MM().AddMsgH(UPG_CMK_CREATE_NEWYEARATTENDANCE_POPUP, Fnt(this, &CMainFrame::OnCreateNewYearAttendancePopup)); // 설 이벤트 출석 팝업
	MM().AddMsgH(UPG_CMK_SHOW_NEWYEARATTENDANCE_POPUP, Fnt(this, &CMainFrame::OnShowNewYearAttendancePopup)); // 설 이벤트 출석 팝업
	
	MM().AddMsgH(UPG_CMK_GET_GOLDMILEAGE_POPUP, Fnt(this, &CMainFrame::OnGetGoldMileagePopup)); // 골드 마일리지 팝업
	MM().AddMsgH(UPG_CMK_CREATE_GOLDMILEAGE_POPUP, Fnt(this, &CMainFrame::OnCreateGoldMileagePopup)); // 골드 마일리지 팝업
	MM().AddMsgH(UPG_CMK_SHOW_GOLDMILEAGE_POPUP, Fnt(this, &CMainFrame::OnShowGoldMileagePopup)); // 골드 마일리지 팝업
	//MM().AppendMsgH( )
	
	MM().AddMsgH(UPG_CMK_GET_GOLDSUPPORTFUND_POPUP, Fnt(this, &CMainFrame::OnGetGoldSupportFundPopup)); // 골드 지원금 팝업
	MM().AddMsgH(UPG_CMK_CREATE_GOLDSUPPORTFUND_POPUP, Fnt(this, &CMainFrame::OnCreateGoldSupportFundPopup)); // 골드 지원금 팝업
	MM().AddMsgH(UPG_CMK_SHOW_GOLDSUPPORTFUND_POPUP, Fnt(this, &CMainFrame::OnShowGoldSupportFundPopup)); // 골드 지원금 팝업

	MM().AddMsgH(UPG_CMK_GET_GOLDATTENDANCEBOOK_POPUP, Fnt(this, &CMainFrame::OnGetGoldAttendanceBookPopup)); // 골드 출석부 팝업
	MM().AddMsgH(UPG_CMK_CREATE_GOLDATTENDANCEBOOK_POPUP, Fnt(this, &CMainFrame::OnCreateGoldAttendanceBookPopup)); // 골드 출석부 팝업
	MM().AddMsgH(UPG_CMK_SHOW_GOLDATTENDANCEBOOK_POPUP, Fnt(this, &CMainFrame::OnShowGoldAttendanceBookPopup)); // 골드 출석부 팝업

	MM().AddMsgH(UPG_CMK_GET_GOLDWITHDRAW_POPUP, Fnt(this, &CMainFrame::OnGetGoldWithdrawPopup)); // 골드 적립 통장 팝업
	MM().AddMsgH(UPG_CMK_SHOW_GOLDWITHDRAW_POPUP, Fnt(this, &CMainFrame::OnShowGoldWithdrawPopup)); // 골드 적립 통장 팝업

	MM().AddMsgH(UPG_CMK_SHOW_CAPTCHA_DLG, Fnt(this, &CMainFrame::OnShowCaptchaDlg));
	MM().AddMsgH(UPG_CMK_CLOSE_CAPTCHA_DLG, Fnt(this, &CMainFrame::OnCloseCaptchaDlg));
	MM().AddMsgH(UPG_CMK_GET_CAPTCHA_DLG, Fnt(this, &CMainFrame::OnGetCaptchaDlg));
	MM().AddMsgH(UPG_CMK_GET_AUTHENTICODE_CAPTCHA, Fnt(this, &CMainFrame::OnGetAuthentiCodeOfCaptcha));
	MM().AddMsgH(UPG_CMK_SET_AUTHENTICODE_CAPTCHA, Fnt(this, &CMainFrame::OnSetAuthentiCodeOfCaptcha));
	MM().AddMsgH(UPG_CMK_GET_NEED_CAPTCHA, Fnt(this, &CMainFrame::OnGetNeedCaptcha));
	MM().AddMsgH(UPG_CMK_SET_NEED_CAPTCHA, Fnt(this, &CMainFrame::OnSetNeedCaptcha));
	MM().AddMsgH(UPG_CMK_GET_CHUSEOK_ATTENDANCE_DLG, Fnt(this, &CMainFrame::OnGetChuSeokAttendanceDlg));
	MM().AddMsgH(UPG_CMK_SHOW_CHUSEOK_ATTENDANCE_DLG, Fnt(this, &CMainFrame::OnShowChuSeokAttendanceDlg));
	MM().AddMsgH(UPG_CMK_GET_CHUSEOK_POPUP, Fnt(this, &CMainFrame::OnGetChuSeokPopup));
	MM().AddMsgH(UPG_CMK_SHOW_CHUSEOK_POPUP, Fnt(this, &CMainFrame::OnShowChuSeokPopup));
	
	MM().AddMsgH(UPG_CMK_GET_LOBBY_NOTIFY_DLG, Fnt(this, &CMainFrame::OnGetLobbyNotifyDlg));
	MM().AddMsgH(UPG_CMK_SHOW_LOBBY_NOTIFY_DLG, Fnt(this, &CMainFrame::OnShowLobbyNotifyDlg));

	MM().AddMsgH(UPG_CMK_SHOW_MESSAGE_MODELESS_DLG, Fnt(this, &CMainFrame::OnShowMsgModelessDlg));
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::DoDataExchange(CDataExchange* pDX)
{
	CFrameWnd::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMainFrame)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_MOVE()
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()	
	ON_MESSAGE(UM_SOCKET_NOTIFY, OnPacketNotify)
// 	ON_MESSAGE(UM_SOCKET_CLOSE, OnSocketClose)
// 	ON_MESSAGE(UM_SOCKET_CONNECT_FAIL, OnFailConnect)
// 	ON_MESSAGE(UM_SOCKET_CONNECT_SUCCESS, OnSuccessConnect)
	
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(UM_CLOSEALLPOPUP, OnClosePopup)
	ON_COMMAND(IDCANCEL, OnCancel)
	ON_COMMAND(IDOK, OnOK)
	ON_WM_CLOSE()
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
#ifdef _DEBUG
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS | CS_OWNDC );
#else
	cs.lpszClass = AfxRegisterWndClass( CS_DBLCLKS | CS_NOCLOSE | CS_OWNDC );
#endif

	cs.style	 = WS_POPUP | WS_SYSMENU | WS_OVERLAPPED | WS_CLIPCHILDREN | WS_MINIMIZEBOX;
	cs.dwExStyle = 0;
	cs.cx		 = 800;	
	cs.cy		 = 600;
	cs.hMenu	 = NULL;

	return TRUE;
}




int CMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct )
{
	if(CFrameWnd::OnCreate(lpCreateStruct) == -1)	 return -1;

//	_crtBreakAlloc = 28981;
	// Add "About..." menu item to system menu.
	// IDM_ABOUTBOX must be in the system command range.

	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);
	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}
	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	NMBASE::UTIL::CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);	
	NMBASE::UTIL::SetCurSiteNo_BySpareParam(g_SpareParam);
	NMBASE::UTIL::SetCurSiteNo_ByCPParam(g_CpCookie);
	NMBASE::SOCK::CSockMan::m_pFuncI_Ptr_Type=(pFuncI_Ptr)ToStringCommMsgIDHeader;

	ResistryGame();	

	ModifyStyle(NULL, WS_MINIMIZEBOX);

#ifdef _DEBUG 
	CString strCommand = GetCommandLine();
	int nTemp = strCommand.Find(":");
	nTemp = strCommand.Find(":", nTemp+1);
	if(nTemp == -1)
	{	
		//"poker_d.exe" 프로세스가 몇 개 실행되어있는지?
		CLoginDlg LoginDlg;
		/*
		int nProcCnt = 0;
		HANDLE         hProcessSnap = NULL; 		
		PROCESSENTRY32 pe32         = {0};
		hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
		if (hProcessSnap != INVALID_HANDLE_VALUE) 
		{
			pe32.dwSize = sizeof(PROCESSENTRY32);

			if (Process32First(hProcessSnap, &pe32))
			{ 
				DWORD Code = 0;
				do 
				{ 
					HANDLE hProcess = OpenProcess (PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID); 

					if (stricmp("poker_d.exe", pe32.szExeFile)==0)
					{
						nProcCnt++;
					}
					CloseHandle (hProcess);
				} 
				while (Process32Next(hProcessSnap, &pe32));
			} 
			CloseHandle (hProcessSnap);					
		}
		//로그인아이디 결정하기
		char PCName[200]={0,};
		DWORD BufSize = sizeof(PCName)-1;
		GetComputerName(PCName, &BufSize);		
		int nMyIDNO=0;		int i=0;
		while(PCName[i]!=0) nMyIDNO+= (PCName[i++]^0x55);	
		nMyIDNO = ( (nMyIDNO %100) *10 ) + nProcCnt; 		

		LoginDlg.m_ID.Format("t_test%05d", nMyIDNO);		
		LoginDlg.m_Pass = "a12a12";

		//qa용
		nMyIDNO = (nMyIDNO % 89) +10;	

		//게임개발팀 할당 받은 테스트 아이디 : qa_pk100~110 사용권장(qa_pk90~99까지는 사용금지)


		NMBASE::UTIL::CShellMan ShellMan;
		int nCnt=ShellMan.EnumProcessCnt("Poker_D.exe");
		LoginDlg.m_ID.Format("qa_pk%02d", nMyIDNO+4);	
		LoginDlg.m_Pass = "qa1234";

//		LoginDlg.m_ID.Format("clientdev_");		
//		LoginDlg.m_Pass = "a12a12";

//		LoginDlg.m_ID.Format("qwertyuiopas");		
//		LoginDlg.m_Pass = "a12a12a12";

		//QA존 서버
//		LoginDlg.m_ServerIP= "192.168.62.31";
//		LoginDlg.m_ServerIP= "192.168.62.78";
//		LoginDlg.m_ServerIP= "192.168.62.31";	//훌다 QA서버
//		LoginDlg.m_ServerPort= 24200;			//섯다 QA서버
//		LoginDlg.m_ServerIP= "192.168.62.31";	//하이로우 QA서버
		LoginDlg.m_ServerIP= "192.168.62.230";
		LoginDlg.m_ServerPort = 24200;

		//LoginDlg.m_ServerIP= "192.168.62.230"; // QA존 테스트 IP
		//LoginDlg.m_ServerIP= "192.168.62.232"; // 규제안 작업 서버
		//LoginDlg.m_ServerIP= "218.145.96.60"; // 재경님 로컬
		//LoginDlg.m_ServerIP= "218.145.96.201"; // 병인님 로컬

		//리얼서버 
		//LoginDlg.m_ServerIP= "220.73.221.235";
		//LoginDlg.m_ServerPort = 24200;

		//로컬서버
//		LoginDlg.m_ServerIP= "218.145.96.57";	//정중호님PC 접속
//		LoginDlg.m_ServerIP= "218.153.63.234";	//최연식님PC 접속		
//		LoginDlg.m_ServerIP= "218.145.96.103";	//박청룡님PC 접속
//		LoginDlg.m_ServerIP= "218.145.96.102";	//고영준님PC 접속
//		LoginDlg.m_ServerIP= "218.145.96.60";	//박재경님PC 접속
		
		LoginDlg.m_Group = 110;
		*/

	/*
		///////////////////////////////////////////////////////////////////////////////////////////
		//ini파일이있다면 설정되있는 아이피/포트를 읽어온다.
		DWORD dwSize = 0;
		char strTemp[1024];
		dwSize = GetPrivateProfileString("TEST", "IP", LoginDlg.m_ServerIP, strTemp, 1024,".\\test.ini");
		if(dwSize)	LoginDlg.m_ServerIP = strTemp;

		LoginDlg.m_ServerPort = GetPrivateProfileInt("TEST", "PORT", LoginDlg.m_ServerPort, ".\\test.ini");

		dwSize = GetPrivateProfileString("TEST", "ID", LoginDlg.m_ID, strTemp, 1024, ".\\test.ini");
		if(dwSize)	LoginDlg.m_ID = strTemp;		

		dwSize = GetPrivateProfileString("TEST", "PASS", LoginDlg.m_Pass, strTemp, 1024,".\\test.ini");
		if(dwSize)	LoginDlg.m_Pass = strTemp;

		LoginDlg.m_Group = GetPrivateProfileInt("TEST", "GROUP", LoginDlg.m_Group, ".\\test.ini");
		////////////////////////////////////////////////////////////////////////////////////////////////////////
	*/	

		if(LoginDlg.DoModal()==IDOK) 
		{
			g_LoginID = LoginDlg.m_ID;
			g_LoginPass = LoginDlg.m_Pass;
			g_ServIP = LoginDlg.m_ServerIP;
			g_ServPort = LoginDlg.m_ServerPort;
			g_LoginGroup = LoginDlg.m_Group;
		}
		else 
		{
			// 프로그램 종료
			ExitGameClient(false);
			return FALSE;
		}
	}
	else
	{
		//아이피를 얻어온다.
		int nStart = strCommand.Find("ip:");
		if(nStart == -1)
		{
			::MessageBox(NULL, "커멘드라인에 아이피 입력을 찾을수 없습니다. ip:xxx.xxx.xxx.xxx 형식으로 추가해 주세요", "경고", MB_OK);
			ExitGameClient(false);
			return FALSE;
		}
		nStart += 3;
		int nEnd = strCommand.Find(" ", nStart);
		if(nEnd == -1) nEnd = strCommand.GetLength();
		g_ServIP = strCommand.Mid(nStart, nEnd - nStart);

		//포트를 얻어온다.
		nStart = strCommand.Find("port:");
		if(nStart == -1)
		{
			::MessageBox(NULL, "커멘드라인에 포트 입력을 찾을수 없습니다. port:xxxxx 형식으로 추가해 주세요", "경고", MB_OK);
			ExitGameClient(false);
			return FALSE;
		}
		nStart += 5;
		nEnd = strCommand.Find(" ", nStart);
		if(nEnd == -1) nEnd = strCommand.GetLength();
		CString strTemp = strCommand.Mid(nStart, nEnd - nStart);
		g_ServPort = atoi(strTemp);

		//게임종류를 얻어온다.
		nStart = strCommand.Find("game:");
		if(nStart == -1)
		{
			::MessageBox(NULL, "커멘드라인에 게임 종류를 찾을수 없습니다. game:xxxxx 형식으로 추가해 주세요\n ex)101:세븐포커, 103:로우바둑이, 104:뉴포커, 105:맞포커", "경고", MB_OK);
			ExitGameClient(false);
			return FALSE;
		}
		nStart += 5;
		nEnd = strCommand.Find(" ", nStart);
		if(nEnd == -1) nEnd = strCommand.GetLength();
		strTemp = strCommand.Mid(nStart, nEnd - nStart);
		g_LoginGroup = atoi(strTemp);

		//아이디를 얻어온다.
		nStart = strCommand.Find("id:");
		if(nStart == -1)
		{
			::MessageBox(NULL, "커멘드라인에 아이디를 찾을수 없습니다. id:xxxxx 형식으로 추가해 주세요", "경고", MB_OK);
			ExitGameClient(false);
			return FALSE;
		}
		nStart += 3;
		nEnd = strCommand.Find(" ", nStart);
		if(nEnd == -1) nEnd = strCommand.GetLength();
		g_LoginID = strCommand.Mid(nStart, nEnd - nStart);

		//패스워드를 얻어온다.
		nStart = strCommand.Find("pass:");
		if(nStart == -1)
		{
			::MessageBox(NULL, "커멘드라인에 패스워드를 찾을수 없습니다. pass:xxxxx 형식으로 추가해 주세요", "경고", MB_OK);
			ExitGameClient(false);
			return FALSE;
		}
		nStart += 5;
		nEnd = strCommand.Find(" ", nStart);
		if(nEnd == -1) nEnd = strCommand.GetLength();
		g_LoginPass = strCommand.Mid(nStart, nEnd - nStart);
	}
#endif

#ifdef RELEASE_BUILD
	CString strCommand = GetCommandLine();
	int nTemp = strCommand.Find(":");
	nTemp = strCommand.Find(":", nTemp+1);
	if(nTemp == -1)
	{	
		//"poker_d.exe" 프로세스가 몇 개 실행되어있는지?
		int nProcCnt = 0;
		HANDLE         hProcessSnap = NULL; 		
		PROCESSENTRY32 pe32         = {0};
		hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0); 
		if (hProcessSnap != INVALID_HANDLE_VALUE) 
		{
			pe32.dwSize = sizeof(PROCESSENTRY32);

			if (Process32First(hProcessSnap, &pe32))
			{ 
				DWORD Code = 0;
				do 
				{ 
					HANDLE hProcess = OpenProcess (PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID); 

					if (stricmp("poker_d.exe", pe32.szExeFile)==0)
					{
						nProcCnt++;
					}
					CloseHandle (hProcess);
				} 
				while (Process32Next(hProcessSnap, &pe32));
			} 
			CloseHandle (hProcessSnap);					
		}
		//로그인아이디 결정하기
		char PCName[200]={0,};
		DWORD BufSize = sizeof(PCName)-1;
		GetComputerName(PCName, &BufSize);		
		int nMyIDNO=0;		int i=0;
		while(PCName[i]!=0) nMyIDNO+= (PCName[i++]^0x55);	
		nMyIDNO = ( (nMyIDNO %100) *10 ) + nProcCnt; 


		

		CLoginDlg LoginDlg;
		LoginDlg.m_ID.Format("t_test%05d", nMyIDNO);		
		LoginDlg.m_Pass = "a12a12";

		//qa용
		nMyIDNO = (nMyIDNO % 89) +10;	

		//게임개발팀 할당 받은 테스트 아이디 : qa_pk100~110 사용권장(qa_pk90~99까지는 사용금지)
		LoginDlg.m_ID.Format("qa_pk%02d", nMyIDNO+4);	
		LoginDlg.m_Pass = "qa1234";

//		LoginDlg.m_ID.Format("clientdev_");		
//		LoginDlg.m_Pass = "a12a12";

//		LoginDlg.m_ID.Format("qwertyuiopas");		
//		LoginDlg.m_Pass = "a12a12a12";

		//QA존 서버
//		LoginDlg.m_ServerIP= "192.168.62.31";
//		LoginDlg.m_ServerIP= "192.168.62.78";
//		LoginDlg.m_ServerIP= "192.168.62.31";	//훌다 QA서버
//		LoginDlg.m_ServerPort= 24200;			//섯다 QA서버
//		LoginDlg.m_ServerIP= "192.168.62.31";	//하이로우 QA서버
		LoginDlg.m_ServerIP= "183.110.61.192";
		LoginDlg.m_ServerPort = 12000;

		//LoginDlg.m_ServerIP= "192.168.62.232"; // QA존 테스트 IP
		//LoginDlg.m_ServerPort = 24200;

		//LoginDlg.m_ServerIP= "218.145.96.60"; // 재경님 로컬
		//LoginDlg.m_ServerPort = 24200;

		//리얼서버 
		//LoginDlg.m_ServerIP= "220.73.221.235";
		//LoginDlg.m_ServerPort = 24200;

		//로컬서버
//		LoginDlg.m_ServerIP= "218.145.96.57";	//정중호님PC 접속
//		LoginDlg.m_ServerIP= "218.153.63.234";	//최연식님PC 접속		
//		LoginDlg.m_ServerIP= "218.145.96.103";	//박청룡님PC 접속
//		LoginDlg.m_ServerIP= "218.145.96.102";	//고영준님PC 접속
//		LoginDlg.m_ServerIP= "218.145.96.60";	//박재경님PC 접속
		
		LoginDlg.m_Group = 110;

	/*
		///////////////////////////////////////////////////////////////////////////////////////////
		//ini파일이있다면 설정되있는 아이피/포트를 읽어온다.
		DWORD dwSize = 0;
		char strTemp[1024];
		dwSize = GetPrivateProfileString("TEST", "IP", LoginDlg.m_ServerIP, strTemp, 1024,".\\test.ini");
		if(dwSize)	LoginDlg.m_ServerIP = strTemp;

		LoginDlg.m_ServerPort = GetPrivateProfileInt("TEST", "PORT", LoginDlg.m_ServerPort, ".\\test.ini");

		dwSize = GetPrivateProfileString("TEST", "ID", LoginDlg.m_ID, strTemp, 1024, ".\\test.ini");
		if(dwSize)	LoginDlg.m_ID = strTemp;		

		dwSize = GetPrivateProfileString("TEST", "PASS", LoginDlg.m_Pass, strTemp, 1024,".\\test.ini");
		if(dwSize)	LoginDlg.m_Pass = strTemp;

		LoginDlg.m_Group = GetPrivateProfileInt("TEST", "GROUP", LoginDlg.m_Group, ".\\test.ini");
		////////////////////////////////////////////////////////////////////////////////////////////////////////
	*/	

		if(LoginDlg.DoModal()==IDOK) 
		{
			g_LoginID = LoginDlg.m_ID;
			g_LoginPass = LoginDlg.m_Pass;
			g_ServIP = LoginDlg.m_ServerIP;
			g_ServPort = LoginDlg.m_ServerPort;
			g_LoginGroup = LoginDlg.m_Group;
		}
		else 
		{
			// 프로그램 종료
			ExitGameClient(false);
			return FALSE;
		}
	}
	else
	{
		//아이피를 얻어온다.
		int nStart = strCommand.Find("ip:");
		if(nStart == -1)
		{
			::MessageBox(NULL, "커멘드라인에 아이피 입력을 찾을수 없습니다. ip:xxx.xxx.xxx.xxx 형식으로 추가해 주세요", "경고", MB_OK);
			ExitGameClient(false);
			return FALSE;
		}
		nStart += 3;
		int nEnd = strCommand.Find(" ", nStart);
		if(nEnd == -1) nEnd = strCommand.GetLength();
		g_ServIP = strCommand.Mid(nStart, nEnd - nStart);

		//포트를 얻어온다.
		nStart = strCommand.Find("port:");
		if(nStart == -1)
		{
			::MessageBox(NULL, "커멘드라인에 포트 입력을 찾을수 없습니다. port:xxxxx 형식으로 추가해 주세요", "경고", MB_OK);
			ExitGameClient(false);
			return FALSE;
		}
		nStart += 5;
		nEnd = strCommand.Find(" ", nStart);
		if(nEnd == -1) nEnd = strCommand.GetLength();
		CString strTemp = strCommand.Mid(nStart, nEnd - nStart);
		g_ServPort = atoi(strTemp);

		//게임종류를 얻어온다.
		nStart = strCommand.Find("game:");
		if(nStart == -1)
		{
			::MessageBox(NULL, "커멘드라인에 게임 종류를 찾을수 없습니다. game:xxxxx 형식으로 추가해 주세요\n ex)101:세븐포커, 103:로우바둑이, 104:뉴포커, 105:맞포커", "경고", MB_OK);
			ExitGameClient(false);
			return FALSE;
		}
		nStart += 5;
		nEnd = strCommand.Find(" ", nStart);
		if(nEnd == -1) nEnd = strCommand.GetLength();
		strTemp = strCommand.Mid(nStart, nEnd - nStart);
		g_LoginGroup = atoi(strTemp);

		//아이디를 얻어온다.
		nStart = strCommand.Find("id:");
		if(nStart == -1)
		{
			::MessageBox(NULL, "커멘드라인에 아이디를 찾을수 없습니다. id:xxxxx 형식으로 추가해 주세요", "경고", MB_OK);
			ExitGameClient(false);
			return FALSE;
		}
		nStart += 3;
		nEnd = strCommand.Find(" ", nStart);
		if(nEnd == -1) nEnd = strCommand.GetLength();
		g_LoginID = strCommand.Mid(nStart, nEnd - nStart);

		//패스워드를 얻어온다.
		nStart = strCommand.Find("pass:");
		if(nStart == -1)
		{
			::MessageBox(NULL, "커멘드라인에 패스워드를 찾을수 없습니다. pass:xxxxx 형식으로 추가해 주세요", "경고", MB_OK);
			ExitGameClient(false);
			return FALSE;
		}
		nStart += 5;
		nEnd = strCommand.Find(" ", nStart);
		if(nEnd == -1) nEnd = strCommand.GetLength();
		g_LoginPass = strCommand.Mid(nStart, nEnd - nStart);
	}
#endif

	NMBASE::SOUND::g_Sound.Init(m_hWnd);// 다이렉트 사운드 초기화
#ifdef _DEBUG 
	// 디버그 모드인 경우 사운드 리소스 상태를 관찰하기 위한 콘솔로그 오픈
	NMBASE::SOUND::g_Sound.EnableConsoleLog(FALSE);
#endif

	IDX_GAME nGame = g_GetGameIdxByGameGroupCode((IDX_GAMEGROUP) g_LoginGroup);
	if (nGame==IDX_GAME_NULL)
	{
		ExitGameClient(false);
		return FALSE;

	}

	GM().SetCurrentGame(nGame);

	/************************************************************************/
	// 여기부터
	// 게임 바로가기 작업 - 초기화  (2007.11.27)
	// NMGoGameDirect_Init([뮤텍스핸들포인터], [현재게임이름], [Check URL, Option사항])
	// 게임이름은 웹에서 사용하는 영문숫자로된 이름 ( 예 : 섯다의  경우 sutda )
	
	InitGoGameDirect();
	
	// 여기까지
	/************************************************************************/

	// 아바타 디스플레이 모듈 초기화
#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
		NMBASE::AVATAR::PresetAvatarModuleSetSiteName(_T("TocToc"));
#endif //UPGRADE_20120308_TOCTOC_CHANNELING
	NMBASE::AVATAR::InitAvatarModule();
	// 로딩창 보여주기
	//m_dlgLoading.Create(3000, TRUE);  //최소 3초 이상을 시간베이스로 로딩되는 애니로 보여줌


	RegistCommonSoundFiles();
	RegistCommonImageFiles();

	//환경설정 폴더(Config) 없으면 폴더 만들기
	CheckConfigDirectory();

	CString str;
	float gamever = (float)VERSION/100;

#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	CString strChannelName;
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
		strChannelName = "톡톡";
	else
		strChannelName = "윈조이";
	str.Format("%s Ver%2.2f", strChannelName, gamever);
#else //UPGRADE_20120308_TOCTOC_CHANNELING
	str.Format("윈조이 Ver%2.2f", gamever);
#endif //UPGRADE_20120308_TOCTOC_CHANNELING

	SetMainWindowText(str);


	// SSO 웹브라우저 모듈을 초기화 한다 - [SSO 작업]
#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
		NMBASE::UTIL::NMPresetSSOWebBrowserSetSiteName("TocToc");
#endif //UPGRADE_20120308_TOCTOC_CHANNELING
	if(!NMBASE::UTIL::NMInitSSOWebBrowser(GetSafeHwnd(), g_AuthCookie, g_DataCookie, g_CpCookie)) 
	{
		// 릴리즈 모드에서 실패하는 경우 프로그램 종료
#ifndef _DEBUG 
#ifndef RELEASE_BUILD
		// 프로그램 종료
		CloseLoadingDlg();
		MessageBox("SSO WebBrowser 모듈을 초기화할 수 없습니다.", "Error", MB_OK|MB_ICONERROR);

		return FALSE;
#endif
#endif
	}

	if(!CCommunityManager::Create())
	{
		ShowMainMessageDlg("커뮤니티 서버 접속에 실패하였습니다.");
	}

	// 게임 윈도우와 방목록 윈도우를 만들어서 적당한 위치로 옮긴다

	CRect rect;	
	rect.SetRect(0,0,GM().GetWidth(),GM().GetHeight());
	MoveWindow(rect);

	if (m_cLobby.Create(this)==FALSE)	ShowMainMessageDlg("Failed to create Lobby");

	// 타이틀 바 제거후 아이콘 보이기
	this->ModifyStyle(0, WS_SYSMENU,0 );

	// 오리지날 사이즈를 구한다
	GetWindowRect(g_OrgMainDlgRect);

	LoadingImageList(3000, FALSE);
#ifndef _DEBUG 
	// 릴리즈 모드이면 윈도우 사이즈를 0으로 만들어 숨긴다
	//MoveWindow(0,0,0,0);
#endif
		
	//////////////////////
	// 통신 메세지 처리 클래스 초기화
	BOOL bResult = m_SockCmnd.Init(this, &NMBASE::SOCK::GSOCKMAN());
	if(bResult==FALSE)
	{
		// 프로그램 종료
		CloseLoadingDlg();
		ShowMainMessageDlg("소켓 초기화에 실패하였습니다");		
	}
	srand((unsigned)timeGetTime());  // 랜덤 초기화
//###버그를잡아라

	// 초대하기 대화상자 만들기
//	m_dlgInvite.SetInit(&InviteUserListBackSpr, &InviteAllUserListBackSpr, &InviteUserListBtnSpr);
	//m_dlgInvite.Create(IDD_INVITE_DIALOG, NULL);
	
	
	// ### [ 관전기능 ] ###
//	m_dlgObserver.SetSprite(&InviteUserListBackSpr, &InviteUserListBtnSpr, &LevelSpr, &EtcSpr);
	//m_dlgObserver.Create(IDD_DLG_OBSERVER, NULL);
	

	// 욕설 필터링 데이터 읽기
	char strbuf[1024] = {0,};
	NMBASE::AVATAR::GetBaseAvatarDir(strbuf, 1023);

	//- lorddan ( 2008.10.02 ) - Bad Filter 초기화( Default Filter Level은 2 )
	int nRet = NMBASE::UTIL::NMInitFilter(strbuf);	
	if ( nRet != SUCCESS )
	{
		CloseLoadingDlg();
		AfxMessageBox( _T("프로그램 실행에 필요한 파일을 읽을 수 없습니다.\n프로그램을 재설치하시기 바랍니다.") );
		
		return FALSE;
	}

	//SetFilterLevel( 1 );	// 기존 검색 방식을 사용하는 경우, 활성화.
	//- lorddan ( 2008.10.02 ) - Bad Filter 

	g_strMasterServerName = (CString)NMBASE::SOCK::GSOCKMAN().ConnectSocket((char*)(LPCTSTR)g_ServIP, g_ServPort);
	g_strConnectecMasterServerName=g_strMasterServerName;
	g_strMainServerName = "";

	// 0 완전 종료 상태, 1 이벤트 기간중이지만 시간이 아닐때 , 2 준비 3 실행
	m_bGroupLimit = FALSE;	
	m_bLogoutbtnclick = FALSE;
	//웹 충전창에서 게임을 계속 한다고 서버에 보냈나
	g_bAskContinue = FALSE;


#ifndef _DEBUG 
	CRect rc(0,0,80,80);
	//rc.OffsetRect(0,0);
	if(!m_pLocWebCtrl) m_pLocWebCtrl = new NMBASE::UTIL::CWebCtrl();
	m_pLocWebCtrl->Create(NULL, "WebControl", WS_VISIBLE|WS_CHILD, rc, this, 1322);
	m_pLocWebCtrl->ShowWindow(SW_HIDE);
#endif

	SetTimer(TIMER_MAINFRAME_CEHCK_LONGTIMEPLAY, 60*1000, NULL);
	SetTimer(TIMER_MAINFRAME_REFRESH_LOGIN_COOKIE, LOGIN_COOKIE_REFRESH_TIME, NULL);
//	SetTimer(LONGTIMEPLAYCHECK_TIMER, 1000, NULL);

	// 비프음 끄기
	TOGGLEKEYS tk;
	SystemParametersInfo(SPI_SETBEEP, false, &tk, 0);

	// 시작할때

	SetTimer(TIMER_MAINFRAME_CEHCK_PROCESS, 5000, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CFrameWnd::OnSysCommand(nID, lParam);
	}
}
// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.
void CMainFrame::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CFrameWnd::OnPaint();
	}
}
// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMainFrame::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMainFrame::OnOK() 
{
	// TODO: Add extra validation here
	AfxPostQuitMessage(1);
}

void CMainFrame::OnCancel() 
{
	// TODO: Add extra cleanup here
	if(GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME)
	{
#ifdef _DEBUG
		ExitGameRequest();	// 겜방에서 ALT-F4 눌렀을 때 디버그 모드에서는 퇴장처리한다.
#endif
		return;
	}

	if(GM().GetCurrentWhere() == IDX_GAMEWHERE_FIRST || GM().GetCurrentWhere() == IDX_GAMEWHERE_LOGIN) GM().SetNowLogout(true);
	if(m_bAskLogout==FALSE && GM().IsNowLogout() == false)
	{
		m_bAskLogout = TRUE;				
		GM().SetWaitResponse(true);

		// ### [ 관전기능 ] ###
		CSV_ASK_LOGOUT alomsg;
		alomsg.Set(GM().GetMyInfo()->UI.UNum, GM().GetMyInfo()->UI.ID);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), alomsg.pData, alomsg.GetTotalSize());
	}
	// 로그아웃이 허용되었을 때만 프로그램을 종료한다
	if(GM().IsNowLogout() == false) 
		return;

	if(m_dlgMsgBox.GetSafeHwnd()) m_dlgMsgBox.OnOK();

	NMBASE::SOCK::GSOCKMAN().Destroy();
	GDM().Release();

	AfxPostQuitMessage(0);
}

void CMainFrame::CheckConfigDirectory()
{

	//config 폴더가 없으면 생성 
	char strDir[MAX_PATH]={0,};
	GetCurrentDirectory(MAX_PATH, strDir);

	//Config 폴더가 없을경우 생성시킨다.
	// 실행 파일명을 알아낸다
	char path[MAX_PATH];
	if(!NMBASE::UTIL::GetProfile_DefPath(path, MAX_PATH-1))
		return;

	CString strReportDir;
	strReportDir.Format("%s\\Config",path);
	if(_chdir(strReportDir.operator LPCTSTR())!=0){
		_mkdir(strReportDir.operator LPCTSTR());
	}

	SetCurrentDirectory(strDir);


}
BOOL CMainFrame::LoadConfig(LPCTSTR id)
{
	if(id == NULL || strlen(id) == 0) 
		return FALSE;

	memset(&g_Config, 0, sizeof(g_Config));

	g_Config.bSndFX				= NMBASE::UTIL::GetProfile_int(id, "SndFX",				1);		
	g_Config.eBetBtnType		= (CONFIG_BETBTN_TYPE) NMBASE::UTIL::GetProfile_int(id, "BetBtnType",		0); 
	g_Config.bCutStyle			= NMBASE::UTIL::GetProfile_int(id, "CutStyle",			1); 
	g_Config.nAutoUpType		= NMBASE::UTIL::GetProfile_int(id, "AutoUpType",		2 );	// 0 : 사용않함 1: 사용 2 : 선택 전
	g_Config.nAutoSortType		= NMBASE::UTIL::GetProfile_int(id, "AutoSortType",		2 );	// 0 : 사용않함 1: 사용 2 : 선택 전
	g_Config.bNoObserverChat	= NMBASE::UTIL::GetProfile_int(id, "NoObserverChat",	0); 
	g_Config.FxVolume			= NMBASE::UTIL::GetProfile_int(id, "FxVolume",			95 );	// 사운드 UI 수정	
	g_Config.nbShowHidden		= NMBASE::UTIL::GetProfile_int(id, "SHOWHIDDEN",		0 );	//히든학습
	g_Config.bNotificationConnectBuddy = NMBASE::UTIL::GetProfile_int(id, "NOTIFICATIONCONNECTBUDDY", 1);

	g_Config.BGMVolume = NMBASE::UTIL::GetProfile_int(id, "BGM_Volume", 75);
	g_Config.bMusicON = NMBASE::UTIL::GetProfile_int(id, "Music", 1);
	g_Config.bAutoHidden = NMBASE::UTIL::GetProfile_int(id, "AutoHidden", 1);
	g_Config.SndFxKind = NMBASE::UTIL::GetProfile_int(id, "SndFxKind", 0);	

	return TRUE;
}

BOOL CMainFrame::SaveConfig(LPCTSTR id)
{
	if(id == NULL || strlen(id) == 0) 
		return FALSE;

	NMBASE::UTIL::WriteProfile_int(id, "SndFX", g_Config.bSndFX);
	NMBASE::UTIL::WriteProfile_int(id, "BetBtnType", (int)g_Config.eBetBtnType);
	NMBASE::UTIL::WriteProfile_int(id, "CutStyle", g_Config.bCutStyle);
	NMBASE::UTIL::WriteProfile_int(id, "AutoUpType", g_Config.nAutoUpType);
	NMBASE::UTIL::WriteProfile_int(id, "AutoSortType", g_Config.nAutoSortType); //자동정렬:2012.09.27 YS.Jeong
	NMBASE::UTIL::WriteProfile_int(id, "NoObserverChat", g_Config.bNoObserverChat);
	NMBASE::UTIL::WriteProfile_int(id, "FxVolume", g_Config.FxVolume);		// 사운드 UI 수정	
	NMBASE::UTIL::WriteProfile_int(id, "SHOWHIDDEN", g_Config.nbShowHidden);	//히든학습
	NMBASE::UTIL::WriteProfile_int(id, "NOTIFICATIONCONNECTBUDDY", g_Config.bNotificationConnectBuddy);	//히든학습
	NMBASE::UTIL::WriteProfile_int(id, "BGM_Volume", g_Config.BGMVolume);
	NMBASE::UTIL::WriteProfile_int(id, "Music", g_Config.bMusicON);
	NMBASE::UTIL::WriteProfile_int(id, "AutoHidden", g_Config.bAutoHidden);
	NMBASE::UTIL::WriteProfile_int(id, "SndFxKind", g_Config.SndFxKind);
	

	return TRUE;
}


void CMainFrame::SetupByConfig()
{
	// 사운드 설정 적용
	NMBASE::SOUND::g_Sound.SetSndVolume( g_Config.FxVolume );
	NMBASE::SOUND::g_Sound.SetSndFx(g_Config.bSndFX);
	NMBASE::SOUND::g_Sound.SetMusic(g_Config.bMusicON);
	NMBASE::SKINGDI::NMRESAM.SetSndFx(g_Config.bSndFX); //대기실사운드용
	NMBASE::SKINGDI::NMRESAM.SetMusic(g_Config.bMusicON);
	NMBASE::SKINGDI::NMRESAM.SetVolumn( g_Config.FxVolume ) ;


	//귓말기능 삭제
	CSV_NOWHISPER nwmsg;
	nwmsg.Set((CCommunityManager::Instance()->GetOption(COM_ECOT_WHISPER) == COM_ECOPT_OFF)?true:false);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), nwmsg.pData, nwmsg.GetTotalSize());

// 
// 	CSV_ASK_CHANGESNDFX acsmsg;
// 	CPlayer *pPlayer = GM().GetMyInfo();
// 	if (pPlayer)
// 	{
// 		acsmsg.Set(GM().GetMyInfo()->UI.UNum, pPlayer->UI.ServPNum
// 		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), nwmsg.pData, nwmsg.GetTotalSize());

// 
// 	if(g_Config.bFailToFullScreen) {
// 		// 풀스크린이 성공하는지 여부를 체크하기 위함
// 		g_Config.nScreenMode = 0;
// 		g_Config.bFailToFullScreen = FALSE;
// 		// ### [ 관전기능 ] ###
// 		SaveConfig(GM().GetMyInfo()->UI.ID);
// 		MessageBox("지난번 실행시 풀스크린 모드로 화면 전환에 실패하였기때문에 창모드로 실행합니다.", "알림", MB_OK);
// 	} else {
// 		// 화면 모드 설정 적용
// 		if(g_Config.nScreenMode==1) {
// 			// 풀스크린이 성공하는지 여부를 체크하기 위함
// 			g_Config.bFailToFullScreen = TRUE;
// 			// ### [ 관전기능 ] ###
// 			SaveConfig(GM().GetMyInfo()->UI.ID);
// 
// 			ChangeDisplayMode();
// 
// 			// 풀스크린이 성공하였기때문에 원래값으로 초기화함
// 			g_Config.bFailToFullScreen = FALSE;
// 			// ### [ 관전기능 ] ###
// 			SaveConfig(GM().GetMyInfo()->UI.ID);
// 		}
// 	}
}

void CMainFrame::ChangeView(IDX_GAMEWHERE nWhere, bool bRefresh)
{
	(nWhere!=IDX_GAMEWHERE_GAME)? m_cLobby.ShowLobbyWindow(true) : m_cLobby.ShowLobbyWindow(false);
	(nWhere==IDX_GAMEWHERE_GAME)? GM().ShowGameWindow(true) : GM().ShowGameWindow(false);

	m_cLobby.OnChangeView(nWhere, bRefresh);
	
	GM().HideChatClear();

	switch(nWhere)
	{
	case IDX_GAMEWHERE_LOGIN:
		return;
	case IDX_GAMEWHERE_TITLE:
	case IDX_GAMEWHERE_LOBY: 
		{
			// 방 목록 및 사용자 목록 리스트를 요구			
		} 
		break;
	case IDX_GAMEWHERE_GAME:
		if( m_dlgNewGameItem.m_hWnd )
		{
			m_dlgNewGameItem.SafeClose();
		}
		
		//[행운아바타] 2006.07.24		
		RewardItemMan().CloseBuyDlg();

		// 한줄 추가
		VipService().OnEnterGame();

		GM().OnEnterRoom(bRefresh);
		CPlayer *pPlayer = GM().GetMyInfo();
		/*CCommunityManager::Instance()->SendBuddyInfoChange(GM().GetCurrentGameType(), pPlayer->UI.nRoomNum, pPlayer->UI.GetMoney());*/
		CCommunityManager::Instance()->SendBuddyInfoChange(GM().GetCurrentGameType(), pPlayer->UI.nRoomNum, pPlayer->UI.GetRealPMoney());
		break;
	}

	//***수정필요
	//GAME()->PlayMusic();
}



void CMainFrame::MoveToLobby()
{
	// 대기실로 이동
	SetChangeWhere(IDX_GAMEWHERE_LOBY, false);
	//m_LobyDlg.MoveToLobby();
}


LONG CMainFrame::OnSocketClose(UINT wParam, LONG lParam)
{
	//int sid = wParam;

	LPSTR pszSockName = (LPSTR)lParam;


	// 마스터 서버와의 접속이 끊어진 경우라면
	if(pszSockName == g_strMasterServerName)
	{
		//g_MasterSID = -1;
		g_strMasterServerName = "";

		if(GetCurrentWhere() == IDX_GAMEWHERE_FIRST) 
		{
			// 모든 소켓을 닫는다.
			NMBASE::SOCK::GSOCKMAN().CloseAllDataSocket();

// 			for(int i=0; i<MAX_DATA_SOCKET; i++)
// 			{
// 				NMBASE::SOCK::GSOCKMAN().CloseDataSocket(i);
// 			}

			g_strMainServerName = "";

			if(!GM().IsNowLogout()) 
			{
// 				CString str;
// 				str.Format("메인 서버와의 접속이 끊어졌습니다");
				ShowMainMessageDlg("메인 서버와의 접속이 끊어졌습니다");
			}

			SetChangeWhere(IDX_GAMEWHERE_FIRST,false);
			//ChangeView(0);
			GM().PostMainWndMessage(WM_COMMAND, IDCANCEL);
		}
	}
	// 메인서버와의 접속이 끊어진 경우라면
	else if(pszSockName == g_strMainServerName)
	{
		//// 메인서버와의 접속이 끊어지면 서버소켓과 모든 데이터 소켓도 접속을 끊는다
		//NMBASE::SOCK::GSOCKMAN().CloseServerSocket();
		//for(int i=0; i<MAX_DATA_SOCKET; i++)
		//{
		//	NMBASE::SOCK::GSOCKMAN().CloseDataSocket(i);
		//}

		// 모든 소켓을 닫는다.
		NMBASE::SOCK::GSOCKMAN().CloseAllDataSocket();

		g_strMainServerName = "";
		//g_MasterSID = -1;	

		// 로그인 중이 아니면 접속 끊김을 알린다
		if(GetCurrentWhere()!=IDX_GAMEWHERE_LOGIN && GetCurrentWhere()!=IDX_GAMEWHERE_FIRST)
		{		
			if(!m_bAskLogout)
			{
				//접속이 끊어진경우 커뮤니티 서버 접속도 끊어버린다.
				CCommunityManager::Instance()->Disconnect();
				
				// 로그아웃 요청한 상태가 아니면 접속 끊김을 알림
				ShowMainMessageDlg("게임 서버와의 접속이 끊어졌습니다");

				// 너무 많이 발생하여 강제 크래시 중단. 20190401
				/*
				// 로우바둑이에서 서버 접속 종료로 종료시 강제 크래시 발생 및 패킷 로그 생성. - 20181112
				if( GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME && GM().GetCurrentGameType() == IDX_GAME_BD)
				{
					NMBASE::UTIL::BugSplatWrapper::GetInstance().AddMemLog(NMBASE::SOCK::CSockMan::GetSocketLogTxt().c_str());

					CString strAllProcessList;
					strAllProcessList = "";

					CString strProcessName;    
					CString strProcessPid;
					DWORD pid = 0;
					HANDLE hProcess = NULL;
					PROCESSENTRY32 pe32 = { 0 };
					hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
					pe32.dwSize = sizeof(PROCESSENTRY32);
					if (Process32First(hProcess, &pe32))
					{
						do
						{
							strProcessName = pe32.szExeFile;
							pid = pe32.th32ProcessID;
							strProcessPid.Format(_T("%lu"), pid);
							//TRACE("%s\n", strProcessName + _T("/") + strProcessPid);
							strAllProcessList += strProcessName + _T("\r\n");
						}
						while (Process32Next(hProcess, &pe32));
					}
					NMBASE::UTIL::BugSplatWrapper::GetInstance().AddMemLog(strAllProcessList);
					CloseHandle(hProcess);
					*(volatile int *)0 = 0;
				}
				*/

				SetChangeWhere(IDX_GAMEWHERE_FIRST, false);
				//ChangeView(0);
			}

			// 프로그램 종료
			GM().SetNowLogout(true);
			GM().PostMainWndMessage(WM_COMMAND, IDCANCEL);
		}
		else
		{					
			//m_dlgLoading.OnCancel();
		}
	}
	else
	{
		// VIPService패킷
		VipService().m_ServiceDlg.OnSocketClose(wParam, lParam);
	}

	return 1;

}

LONG CMainFrame::OnFailConnect(UINT wParam, LONG lParam)
{	
	//int sid = wParam;
	LPSTR pszSockName = (LPSTR)lParam;

	CString str;
	// 마스터 서버와 접속이 실패한 경우라면
	if(GetCurrentWhere()==IDX_GAMEWHERE_FIRST && g_strMasterServerName==pszSockName)
	{
		SetChangeWhere(IDX_GAMEWHERE_FIRST, false);		
		GM().SetWaitResponse(false);

		m_dlgLoading.OnCancel();
		
		ShowMainMessageDlg("메인 서버와 접속이 실패하였습니다");	
		GM().PostMainWndMessage(WM_COMMAND, IDCANCEL);

	}

	// 로그인하는중일때 메인서버와 접속이 실패한 경우라면
	if(GetCurrentWhere()==IDX_GAMEWHERE_LOGIN && g_strMainServerName==pszSockName)
	{
		SetChangeWhere(IDX_GAMEWHERE_FIRST, false);
		GM().SetWaitResponse(false);

		m_dlgLoading.OnCancel();

		ShowMainMessageDlg("게임 서버와 접속이 실패하였습니다");		
		GM().PostMainWndMessage(WM_COMMAND, IDCANCEL);
	}

	// 다른 게임 서버의 채널로 이동중 접속이 실패한 경우라면
	if(GetCurrentWhere()==IDX_GAMEWHERE_LOBY && g_strMasterServerName==pszSockName && m_cLobby.IsMovingGame())
	{
		SetChangeWhere(IDX_GAMEWHERE_FIRST, false);
		//g_bMovingChan = FALSE;
		GM().SetWaitResponse(false);		
		ShowMainMessageDlg("게임 이동에 실패하였습니다. 재접속해 주시기 바랍니다.");			

		// 프로그램 종료
		GM().SetNowLogout(true);
		GM().PostMainWndMessage(WM_COMMAND, IDCANCEL);
	}

	// VIPService패킷
	VipService().m_ServiceDlg.OnFailConnect(wParam, lParam);


	return 1;

}


LONG CMainFrame::OnSuccessConnect(UINT wParam, LONG lParam)
{	
	//int sid = wParam;
	LPSTR pszSockName = (LPSTR)lParam;

	CString str;
	// 마스터 서버와 접속이 실패한 경우라면
	if(g_strMasterServerName==pszSockName)
	{

	}
	else if(g_strMainServerName==pszSockName)
	{

	}
	else
	{
		// VIPService패킷
		VipService().m_ServiceDlg.OnSuccessConnect(wParam, lParam);
	}

	return 1;
}


LONG CMainFrame::OnPacketNotify(UINT wParam, LONG lParam)
{
	UINT nCommand = (UINT)wParam;

	switch(nCommand)
	{
	case UM_SOCKET_RECEIVE:
		{
			NMBASE::SOCK::SGAMENOTIFY *pNotifyData = (NMBASE::SOCK::SGAMENOTIFY*)lParam;
			if(!pNotifyData) return 0;

			CString strSockName = pNotifyData->szSockName;	// 소켓 이름
			int Signal   = pNotifyData->Signal;		// 메시지 시그널
			int TotSize  = pNotifyData->TotSize;	// 메시지 사이즈
			char *lpData = pNotifyData->lpData;		// 메시지 데이터 포인터

#ifdef TRACE_OUTPUT		
			char szTemp[256] = {0, };
			sprintf(szTemp, "CMainFrame::OnPacketNotify() - Command : %d", Signal);
			TTrace::Debug()->Send(szTemp, "UPGBase");   // single byte string	
#endif	// TRACE_OUTPUT

			m_cLobby.OnPacketNotify(strSockName.GetBuffer(), Signal, TotSize, lpData);
			GM().OnPacketNotify(strSockName.GetBuffer(), Signal, TotSize, lpData);


			// VIPService패킷
			if(VipService().m_ServiceDlg.IsConnectServer())
			{
				VipService().m_ServiceDlg.OnPacketNotify(strSockName.GetBuffer(), Signal, TotSize, lpData);
			}
		}
		break;

	case UM_SOCKET_CLOSE:
		{					
			OnSocketClose(wParam, lParam);
		}
		break;

	case UM_SOCKET_CONNECT_SUCCESS:
		{
			OnSuccessConnect(wParam, lParam);
		}
		break;

	case UM_SOCKET_CONNECT_FAIL:
		{
			OnFailConnect(wParam, lParam);
		}
		break;				

	default:
		break;
	}	

	return 1;
}

void CMainFrame::OnMove(int x, int y) 
{
	CFrameWnd::OnMove(x, y);
}



void CMainFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();

	GDM().Release();

	// TODO: Add your message handler code here
	NMBASE::SOUND::g_Sound.Destroy();

	KillTimer(TIMER_MAINFRAME_BADUSER);
	KillTimer(TIMER_MAINFRAME_CEHCK_LONGTIMEPLAY);
	KillTimer(TIMER_MAINFRAME_CEHCK_PROCESS);
	KillTimer(TIMER_MAINFRAME_REFRESH_LOGIN_COOKIE);
	KillTimer(TIMER_MAINFRAME_WEBCALL);
	KillTimer(TIMER_MAINFRAME_LOGIN_POPUP);

	// SSOWebBroswer를 파괴하기 전에 생성된 브라우저를 모두 파괴하지 않으면 프로그램 Crash가 일어남

	/************************************************************************/
	// 여기부터
	// 게임 바로가기 작업 - 게임실행  (2007.11.27)
	// 이전에 NMGoGameDirect_SetDestination 호출이 성공적으로 실행되었다면 자동으로 게임실행.
	// * 주의 * SSOWebBrowser 라이브러리를 사용하므로 UnInitSSOWebBrowser() 호출 이전에 반드시 실행!
	
	NMBASE::UTIL::NMGoGameDirect_OnExitGame();
	
	// 여기까지
	/************************************************************************/
	//g_pLobyDlg->m_BannerWnd.Destroy();	
	// [아이템 유료화 영역] 
	//g_pLobyDlg->DestroySSOWebItemZone();
	
	// ### [ 관전기능 ] ###
	if(m_dlgObserver.GetSafeHwnd() != NULL && m_dlgObserver.IsWindowVisible() == TRUE)
		m_dlgObserver.ShowWindow(SW_HIDE);

	//m_dlgObserver.User_DelAll(); //모두 제거
	
	// 초대 대화창 감춤
	if(m_dlgInvite.GetSafeHwnd() != NULL && m_dlgInvite.IsWindowVisible() == TRUE)
		m_dlgInvite.ShowWindow(SW_HIDE);

	// 광고 배너가 뜨기 전에 뮤텍스를 제거한다.
	GM().DestroyMutex();

	NMBASE::UTIL::CShellMan ShellMan;	
#ifndef _DEBUG
	if(m_pLocWebCtrl) { 
		//m_pLocWebCtrl->Navigate("about:blank");
		delete m_pLocWebCtrl; m_pLocWebCtrl = NULL; 
	}
	
	if(g_bNoGo == FALSE) {
		// 넷마블 홈페이지를 다시 원래 상태로 복원시킨다
#ifdef _TEST
		if( g_URLConverType == URL_CONVERT_DEV )
		{	
			ShellMan.ShowRefresh("http://game1-dev.netmarble.net/newpoker/");
		}
		else{
			ShellMan.ShowRefresh("http://game1.netmarble.net/newpoker/");
		}
		
#else
		ShellMan.ShowRefresh("http://game1.netmarble.net/newpoker/");
		
#endif		
		MM().Call(UPG_CMK_SHOW_EXPLORER, TRUE);
	}	
	
	
	//보유한도 보상
	if ( m_bPostBrowserMove  )
	{		
		//ShellExecute(NULL,  "open", m_strURLPost, NULL, NULL, SW_SHOWNORMAL);
		ShellMan.OpenNewBrowser(m_strURLPost);
	}
	
#endif	

	m_cLobby.Release();

	//NMBASE::SOCK::GSOCKMAN().CloseServerSocket();
	NMBASE::SOCK::GSOCKMAN().CloseAllDataSocket();
	NMBASE::SOCK::GSOCKMAN().Destroy();

	NMBASE::UTIL::NMUnInitSSOWebBrowser();	// [SSO 작업]

	CCommunityManager::Release();
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	switch(nIDEvent)
	{
	case TIMER_MAINFRAME_BADUSER:
		KillTimer(TIMER_MAINFRAME_BADUSER);
		break;
	case TIMER_MAINFRAME_CEHCK_LONGTIMEPLAY:
		GM().GetPlayTimeWarning().OnTimer(); // 장시간 게임 플레이 경고
		break;
	case TIMER_MAINFRAME_CEHCK_PROCESS:
		if( CheckAllProcessName() == 1 )
		{
			KillTimer( TIMER_MAINFRAME_CEHCK_PROCESS );
			
			NMBASE::SOCK::GSOCKMAN().Destroy();

			MessageBox( "허용되지 않은 외부 프로그램 사용으로 게임 접속이 종료 되었습니다." );

			AfxPostQuitMessage(0);
		}
		break;
	case TIMER_MAINFRAME_REFRESH_LOGIN_COOKIE:
		{
			NMBASE::UTIL::CShellMan ShellMan;
			ShellMan.OpenNewBrowser_Refresh();
		}
		break;
	case TIMER_UPGDLGBASE_REDRAW:
		GetObjectMan()->Draw();
		break;
	case TIMER_MAINFRAME_WEBCALL:
		{
			GM().OnTimer(nIDEvent);
		}
		break;
	case TIMER_MAINFRAME_LOGIN_POPUP:
		{
			KillTimer(TIMER_MAINFRAME_LOGIN_POPUP);
			m_cLobby.ShowLoginPopup();
		}
		break;
	}
	
	CFrameWnd::OnTimer(nIDEvent);
}

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( IsFullScreenState() ) return;

	SendMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));

	CFrameWnd::OnLButtonDown(nFlags, point);
}


// [게임 아이템 작업]

void CMainFrame::BrowserMove(LPCTSTR targeturl) 

{ 

	if(!targeturl) return;
	if(strlen(targeturl)==0) return;

   #if(0)	
		// 기존 방식(사용 안함)
		// 아바타 모듈 기본 폴더 얻기
		char strbuf[1024] = {0,}; 
		GetBaseAvatarDir(strbuf, 1023); 

		// 브라우저 제어 프로그램
		CString exeName = strbuf; 
		exeName += "\\NMIEMoveMFC.exe"; 

		// 넷마블 웹페이지 이동
		::ShellExecute( NULL, "open", exeName, targeturl, NULL, SW_HIDE );
	#else	
		// 새로운 방식
		NMBASE::UTIL::OpenNetmarbleWebpage(g_MyUniqNo, targeturl);
	#endif

} 

// 넷마블 홈페이지를 보이거나 안보이도록 한다(광고 배너로 인한 속도 저하 방지가 목적)
LRESULT CMainFrame::OnShowExplorer( WPARAM &wParam, LPARAM &lParam )
{
	BOOL bShow = wParam?TRUE:FALSE;
	NMBASE::UTIL::CShellMan ShellMan;					
	ShellMan.ShowExplorer( bShow, "netmarble", FALSE );
	return TRUE;
}

LRESULT CMainFrame::OnLoadingEnd( WPARAM &wParam, LPARAM &lParam )
{
	CloseLoadingDlg(false);
	GM().OnLoadingEnd();
	return TRUE;
}


void CMainFrame::WinHelp(DWORD dwData, UINT nCmd) 

{

	// TODO: Add your specialized code here and/or call the base class

//	CFrameWnd::WinHelp(dwData, nCmd);

}

POINT	CMainFrame::GetWindowMousePos()
{
	POINT pt;
	POINT pt1;
				
	GetCursorPos(&pt);	
	pt1 = pt;
	ScreenToClient(&pt1);
				
	pt.x = pt.x  - pt1.x;
	pt.y = pt.y  - pt1.y;
	return pt;
}


//////////////////////////////////////////////////////////////////////////

// [아이템 유료화 영역]

LRESULT CMainFrame::OnAvatarShop( WPARAM &wParam, LPARAM &lParam )
{
	CloseGameItemDlg();
	m_dlgNewGameItem.AvatarDoModal((const char*)lParam, wParam);

	return TRUE;
}

LRESULT CMainFrame::OnChanceItemShop( WPARAM &wParam, LPARAM &lParam )
{ 
	int groupcode   = atoi(GetPostedData( "groupcode=", GDM().GetString(GDK_SSOWEB_URL).c_str() ));
	for(int i=0; i<(int)RewardItemMan().m_RewardItem.size(); i++)
	{		
		if (RewardItemMan().m_RewardItem[i].ItemCode == groupcode)
		{
			CSV_ASK_BUYITEM buy_msg;
			buy_msg.Set( RewardItemMan().m_RewardItem[i].ItemCode, RewardItemMan().m_RewardItem[i].GameCode,
				RewardItemMan().m_RewardItem[i].ItemTime );
			NMBASE::SOCK::GSOCKMAN().SendData( g_strMainServerName.GetBuffer(), buy_msg.pData, buy_msg.GetTotalSize() );
			break;
		}
	}
	return TRUE;
}

LRESULT CMainFrame::OnItemShop( WPARAM &wParam, LPARAM &lParam )
{	
	CloseGameItemDlg();

	int nTab = (int) wParam;
	m_dlgNewGameItem.DoModal(nTab, TRUE, (int)GDM().GetINT64(GDK_SSOWEB_ITEMCODE));
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
// [넷마블 보안관]
LRESULT CMainFrame::OnBadUserComplete( WPARAM &wParam, LPARAM &lParam )
{
	CBadUserComplete dlg1( (CWnd *)this );
	dlg1.SetID( g_TargetID );
	if ( dlg1.DoModal() == IDCANCEL )
	{		
		NMBASE::UTIL::CShellMan sm;
		sm.OpenNewBrowser(getGameURL(IDX_GAMEURL_BADUSER));
	}
	return TRUE;
}

LRESULT CMainFrame::OnDestroyLoadingDialog( WPARAM &wParam, LPARAM &lParam )
{
	ShowWindow(SW_SHOW);
	SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
	SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SetActiveWindow();
	if (m_bLoginToLobby == TRUE)
	{
		SetTimer(TIMER_MAINFRAME_LOGIN_POPUP, 0, NULL);
		m_bLoginToLobby = FALSE;
	}
	return TRUE;
}

LRESULT CMainFrame::OnShowMessageImageBox( WPARAM &wParam, LPARAM &lParam )
{
	CMsgImgBox dlg((CWnd *)lParam);
	return dlg.DoModal((char *)wParam);
}

LRESULT CMainFrame::OnChangeView(WPARAM &wParam, LPARAM &lParam)
{
	IDX_GAMEWHERE nView = (IDX_GAMEWHERE)wParam;
	if (nView == IDX_GAMEWHERE_LOBY && GM().GetCurrentWhere() == IDX_GAMEWHERE_LOGIN)
	{
		m_bLoginToLobby = TRUE;
	}
	GM().SetCurrentWhere(nView);

	if(nView == IDX_GAMEWHERE_GAME) m_cLobby.GetLobbyView()->StopTimer();
	else m_cLobby.GetLobbyView()->StartTimer();

	bool bRefresh = lParam?true:false;
	ChangeView(nView, bRefresh);
	return TRUE;
}

LRESULT CMainFrame::OnClickLobbyExitButton(WPARAM &wParam, LPARAM &lParam)
{
	bool bShowEndPopup = wParam?true:false;

	if(bShowEndPopup)
	{
		if( m_bLogoutbtnclick == TRUE ) return FALSE;
		if(GM().IsWaitResponse()) return FALSE;
		GM().SetWaitResponse(true);			
		
		m_bLogoutbtnclick = TRUE;
			
		CPlayer *pPlayer = GM().GetMyInfo();
		if (pPlayer)
		{
			INT64 llEaredMoney = pPlayer->UI.GetMoney() - m_cLobby.GetLoginMoney();
			MM().Call(UPG_CMK_SHOW_POPUP_ENDDING_UPP_LV3);
			//패밀리 엔딩 팝업
			if (MM().Call(UPG_CMK_SHOW_ENDING_POPUP, (WPARAM)&llEaredMoney) ==IDCANCEL)
			{
				GM().SetWaitResponse(false);
				m_bLogoutbtnclick = FALSE;
				return FALSE;
			}
			
		}
		
	}

	gNMContegoMan().NMCtgRelease();
	GM().PostMainWndMessage(WM_COMMAND, IDCANCEL);

	return TRUE;
}

LRESULT CMainFrame::OnShowEndingPopup(WPARAM &wParam, LPARAM &lParam)
{


#ifdef _BETAVER
	return ShowMainMessageDlg(true, "지금 종료 하시겠습니까?");

#endif

	INT64 llEarnedMoney = *(INT64*)wParam;	//접속 후 획득/감소 머니

	//패밀리 엔딩 팝업
	CEndPopup EndPopupDlg(this);
	return EndPopupDlg.DoModal(llEarnedMoney);
}

LRESULT CMainFrame::OnSetMainWindowText(WPARAM &wParam, LPARAM &lParam)
{
	LPCTSTR lpText = (LPCTSTR)lParam;
	SetWindowText(lpText);
	return TRUE;
}

LRESULT CMainFrame::OnShowMainMessage(WPARAM &wParam, LPARAM &lParam)
{
	return m_dlgMsgBox.DoModal(CMsgBoxDlg::EMBT_NOTICE, (char *)lParam);
}

LRESULT CMainFrame::OnCloseMainMessage(WPARAM &wParam, LPARAM &lParam)
{
	bool bCancle = false;
	if(wParam) bCancle = true;

	if(m_dlgMsgBox.GetSafeHwnd() == NULL) return FALSE;
	if(m_dlgMsgBox.IsWindowVisible() == FALSE) return FALSE;

	m_dlgMsgBox.SendMessage(WM_COMMAND, IDOK);

	return TRUE;
}

LRESULT CMainFrame::OnClearMainMessage(WPARAM &wParam, LPARAM &lParam)
{
	bool bCancle = false;
	if(wParam) bCancle = true;

	if(m_dlgMsgBox.GetSafeHwnd() == NULL) return FALSE;
	if(m_dlgMsgBox.IsWindowVisible() == FALSE) return FALSE;

	m_dlgMsgBox.ClearMessage();

	return TRUE;
}

LRESULT CMainFrame::OnIsVisibleMainMessage(WPARAM &wParam, LPARAM &lParam)
{
	if(m_dlgMsgBox.GetSafeHwnd() && m_dlgMsgBox.IsWindowVisible()) return TRUE;
	return FALSE;
}

LRESULT CMainFrame::OnSaveConfig(WPARAM &wParam, LPARAM &lParam)
{
	if (SaveConfig((LPCTSTR)lParam))
	{
		SetupByConfig();
		return TRUE;
	}
	return FALSE;
}

LRESULT CMainFrame::OnShowMsgChk(WPARAM &wParam, LPARAM &lParam)
{
	return m_dlgMsgBoxChk.DoModal((char *)wParam, (int)lParam);
}

LRESULT CMainFrame::OnLoadConfig(WPARAM &wParam, LPARAM &lParam)
{
	if(LoadConfig((LPCTSTR)lParam))
	{
		SetupByConfig();
		return TRUE;
	}

	return FALSE;
}

LRESULT CMainFrame::OnGetObserverDlg(WPARAM &wParam, LPARAM &lParam)
{
	CObserverDlg **ppDlg = (CObserverDlg **)lParam;
	*ppDlg = &m_dlgObserver;
	return TRUE;
}

LRESULT CMainFrame::OnGetProfileDlg(WPARAM &wParam, LPARAM &lParam)
{
	CProfileDlg **ppDlg = (CProfileDlg **)lParam;
	*ppDlg = &m_dlgProfile;
	return TRUE;
}

LRESULT CMainFrame::OnGetRecevieMessageDlg(WPARAM &wParam, LPARAM &lParam)
{
	CReceiveMessageDlg **ppDlg = (CReceiveMessageDlg **)lParam;
	*ppDlg = &m_dlgReceiveMessage;
	return TRUE;
}

LRESULT CMainFrame::OnGetRecevieAdminMessageDlg(WPARAM &wParam, LPARAM &lParam)
{
	CReceiveAdminMessageDlg **ppDlg = (CReceiveAdminMessageDlg **)lParam;
	*ppDlg = &m_dlgReceiveAdminMessage;
	return TRUE;
}

LRESULT CMainFrame::OnGetSendMessageDlg(WPARAM &wParam, LPARAM &lParam)
{
	CSendMessageDlg **ppDlg = (CSendMessageDlg **)lParam;
	*ppDlg = &m_dlgSendMessage;
	return TRUE;
}

LRESULT CMainFrame::OnGetMessageBoxDlg(WPARAM &wParam, LPARAM &lParam)
{
	CMessageBox **ppDlg = (CMessageBox **)lParam;
	*ppDlg = &m_dlgMessageBox;
	return TRUE;
}

LRESULT CMainFrame::OnGetGoldChangeDlg(WPARAM &wParam, LPARAM &lParam)
{
	CGoldChangeDlg **ppDlg = (CGoldChangeDlg **)lParam;
	*ppDlg = &m_dlgGoldChange;
	return TRUE;
}

LRESULT CMainFrame::OnGetGoldBigWheelDlg(WPARAM &wParam, LPARAM &lParam)
{
	CGoldBigWheelDlg **ppDlg = (CGoldBigWheelDlg **)lParam;
	*ppDlg = &m_dlgGoldBigWheel;
	return TRUE;
}

LRESULT CMainFrame::OnGetClubDlg(WPARAM &wParam, LPARAM &lParam)
{
	CClubDlg **ppDlg = (CClubDlg **)lParam;
	*ppDlg = &m_dlgClub;
	return TRUE;
}

LRESULT CMainFrame::OnGetInvateDlg(WPARAM &wParam, LPARAM &lParam)
{
	CInviteDlg **ppDlg = (CInviteDlg **)lParam;
	*ppDlg = &m_dlgInvite;
	return TRUE;
}

LRESULT CMainFrame::OnGetEndEventDlg(WPARAM &wParam, LPARAM &lParam)
{
	CEndEventPopupDlg **ppDlg = (CEndEventPopupDlg **)lParam;
	*ppDlg = &m_dlgEndEvent;
	return TRUE;
}

LRESULT CMainFrame::OnGetRestrictionDlg(WPARAM &wParam, LPARAM &lParam)
{
	CRestrictionPopup **ppDlg = (CRestrictionPopup **)lParam;
	*ppDlg = &m_dlgRestriction;
	return TRUE;
}

LRESULT CMainFrame::OnGetWhisperDlg(WPARAM &wParam, LPARAM &lParam)
{
	CWhisperDlg **ppDlg = (CWhisperDlg **)lParam;
	*ppDlg = &m_dlgWhisper;
	return TRUE;
}

LRESULT CMainFrame::OnAddLobbyChatViewMessage(WPARAM &wParam, LPARAM &lParam)
{
	char *pMessage = (char *)lParam;
	COLORREF color = (COLORREF)wParam;
	
	m_cLobby.AddChatText(pMessage, color);
	return TRUE;
}

LRESULT CMainFrame::OnResetBadUserTimer(WPARAM &wParam, LPARAM &lParam)
{
	KillTimer(TIMER_MAINFRAME_BADUSER);
	m_nTimerBadUser = SetTimer(TIMER_MAINFRAME_BADUSER, MAX_BADUSERDELAYTIME, NULL);

	return TRUE;
}

LRESULT CMainFrame::OnChangeDisplayMode(WPARAM &wParam, LPARAM &lParam)
{
	CObserverDlg *pObserverDlg = GetObserverDlg();
	
	static CRect OrgRect;
	static LONG orgstyle, orgExstyle;
	// TODO: Add your control notification handler code here
	if(Display.DDRAWINIT==TRUE)
	{
		// 변경되기 전의 윈도우 속성으로 복구하고 원래의 위치로 이동
		SetWindowLong(GetSafeHwnd(), GWL_STYLE, orgstyle);
		SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, orgExstyle);

		//TopMost 속성을 제거하고 가장 위에 나타나도록 한다.
		SetWindowPos(&wndNoTopMost,OrgRect.left, OrgRect.top, OrgRect.right, OrgRect.bottom, SWP_HIDEWINDOW);
		SetWindowPos(&wndTop,OrgRect.left, OrgRect.top, OrgRect.right, OrgRect.bottom, SWP_SHOWWINDOW);
		MoveWindow(OrgRect);
		Display.CloseDirectDraw();

		CLobbyView *pLobbyView = m_cLobby.GetLobbyView();
		pLobbyView->MoveWindow(CRect(0,0,pLobbyView->GetWidth(),pLobbyView->GetHeight()));

		// 버튼 초기화
		CInviteDlg *pInviteDlg = GetInviteDlg();
		if (pInviteDlg && IsWindow(pInviteDlg->GetSafeHwnd()))
		{
			pInviteDlg->ShowWindow(SW_HIDE);		// 초대 대화창 감춤
		}
		
		if (pObserverDlg && IsWindow(pObserverDlg->GetSafeHwnd()))
		{
			pObserverDlg->ShowWindow(SW_HIDE);		// 관전창 감춤
		}

		
		m_cLobby.OnChangeFullScreen(false);
		GM().OnChangeFullScreen(false);
		SetFullScreenState(false);
	}
	else 
	{
		// 변경되기 전의 원래 사이즈와 위치를 저장
		GetWindowRect(OrgRect);

		// 타이틀바와 다이얼로그 경계속성을 제거
		orgstyle = ::GetWindowLong(GetSafeHwnd(), GWL_STYLE);
		orgExstyle = ::GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);
		
		int expand_width = 1024;
		int expand_height = 768;

		int color_depth[2] = { 16, 32 };
		OSVERSIONINFO osi = { sizeof(OSVERSIONINFO), 0 };
		GetVersionEx(&osi);
		if ( osi.dwMajorVersion > 6 || (osi.dwMajorVersion == 6 && osi.dwMinorVersion >= 2) )
		{
			color_depth[0] = 32;
			color_depth[1] = 16;
		}
		
		Display.CloseDirectDraw();
		bool bFull = false;
		// 기본 1024 x 768 시도...		
		bFull = (Display.InitDirectDraw(GetSafeHwnd(), expand_width, expand_height, color_depth[0])==TRUE);

		if( false == bFull )
		{
			Display.CloseDirectDraw();
			bFull = (Display.InitDirectDraw(GetSafeHwnd(), expand_width, expand_height, color_depth[1])==TRUE);
			SetFullScreenState( bFull );
		}

		if( bFull )
		{
 			LONG newstyle =  orgstyle & (~WS_CAPTION);
 			::SetWindowLong(GetSafeHwnd(), GWL_STYLE, newstyle);
 			LONG newExstyle =  orgExstyle & (~WS_EX_OVERLAPPEDWINDOW);
 			::SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, newExstyle);
 
			SetWindowPos(&CWnd::wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);
 			pObserverDlg->ShowWindow(SW_HIDE);
			// 초대 대화창 감춤
			CInviteDlg *pInviteDlg = GetInviteDlg();
			if (pInviteDlg && IsWindow(pInviteDlg->GetSafeHwnd()))
			{
				pInviteDlg->ShowWindow(SW_HIDE);
			}

			CRect MovRect;
			CLobbyView *pLobbyView = m_cLobby.GetLobbyView();
			MovRect.SetRect(0,0,pLobbyView->GetWidth(),pLobbyView->GetHeight());
			MovRect.OffsetRect(CPoint((expand_width - pLobbyView->GetWidth())/2, (expand_height - pLobbyView->GetHeight())/2));
			pLobbyView->MoveWindow(MovRect);

			m_cLobby.OnChangeFullScreen(true);
			GM().OnChangeFullScreen(true);
			SetFullScreenState(true);
		}
		else
		{
			Display.CloseDirectDraw();
			// 변경되기 전의 윈도우 속성으로 복구하고 원래의 위치로 이동
			SetWindowLong(GetSafeHwnd(), GWL_STYLE, orgstyle);
			SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, orgExstyle);
			MoveWindow(OrgRect);
		}
 		SetWindowPos(&CWnd::wndNoTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOSIZE|SWP_SHOWWINDOW);
	}

	return TRUE;
}

LRESULT CMainFrame::OnScreenCapture(WPARAM &wParam, LPARAM &lParam)
{
	CTime time = CTime::GetCurrentTime();
	CString fname;

	fname.Format("%s_%d_%d_%d_%d_%d.jpg", GM().GetCurrentGameString().GetString(), time.GetMonth(), time.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
	
	gNMContegoMan().UnProtectScreenCaptureLocal();

	if(NMBASE::GRAPHICGDI::ScreenCapture(GetSafeHwnd(), fname)) 
	{
		ShowMainMessageDlg("스크린 샷을 \'내 문서\'폴더에 저장하였습니다.");
	}

	gNMContegoMan().ProtectScreenCaptureLocal();

	return TRUE;
}

LRESULT CMainFrame::OnCheckBadUserReport(WPARAM &wParam, LPARAM &lParam)
{
	if(m_nTimerBadUser != NULL)
	{
		MM().Call(UPG_CMK_SHOW_MESSAGE_IMAGE_BOX, (WPARAM)".\\common\\image\\112\\SkillLimitation.bmp");
		PostQuitMessage(0);
		return true;
	}

	return false;
}

LRESULT CMainFrame::OnShowNewAskChargeDlg(WPARAM &wParam, LPARAM &lParam)
{
#ifdef _BETAVER
	//베타서비스 무료충전
	
	CCL_REQUEST_BETA_ALLIN_MONEY msg;
	msg.Set();
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());	

	return TRUE;
#endif
	
	CString str = (LPCTSTR)lParam;
	NEW_ASK_CHARGE_DLG_TYPE type = (NEW_ASK_CHARGE_DLG_TYPE)wParam;

	if ( m_dlgGradeInsu.GetSafeHwnd() )
	{
		if (!str.IsEmpty())
			m_dlgGradeInsu.Navigate( str );
	}
	else
	{
		m_dlgGradeInsu.DoModal( type, str );
	}

	return TRUE;
}

LRESULT CMainFrame::OnShowTipManDlg(WPARAM &wParam, LPARAM &lParam)
{
	
	return TRUE;
}

LRESULT CMainFrame::OnShowDirectGoDlg(WPARAM &wParam, LPARAM &lParam)
{
	return GM().ShowQuickJoinerDlg();
}

LRESULT CMainFrame::OnCloseLoadingDlg(WPARAM &wParam, LPARAM &lParam)
{
	if(m_dlgLoading.GetSafeHwnd() == NULL || m_dlgLoading.IsWindowVisible() == FALSE) return FALSE;
	bool bCancle = wParam?true:false;
	if(bCancle) m_dlgLoading.OnCancel();
	else m_dlgLoading.OnOK();
	return TRUE;
}

LRESULT CMainFrame::OnShowSelectStartDlg(WPARAM &wParam, LPARAM &lParam)
{
	return GM().ShowSelectStarterDlg();
}

LRESULT CMainFrame::OnShowResetSelectStartDlg(WPARAM &wParam, LPARAM &lParam)
{
	return GM().ShowResetSelectStarterDlg();
}

LRESULT CMainFrame::OnGameItemDlgRefresh_MyItem(WPARAM &wParam, LPARAM &lParam)
{
	// [아이템구매창 쓰레드] 2005-12-27
	// [게임아이템창 리뉴얼] 2006.04.28
	if ( m_dlgNewGameItem.GetSafeHwnd() != NULL )
	{
		// [게임아이템창 리뉴얼] 2006.04.18
		m_dlgNewGameItem.RefreshMyItem();
	}
	return TRUE;
}

LRESULT CMainFrame::OnCloseGameItemDlg(WPARAM &wParam, LPARAM &lParam)
{
	if(m_dlgNewGameItem.GetSafeHwnd() != NULL)
	{		
		m_dlgNewGameItem.SafeClose();
	}
	return TRUE;
}

LRESULT CMainFrame::OnShowGameItemDlg(WPARAM &wParam, LPARAM &lParam)
{
	bool bForceClose = wParam?true:false;
	if(m_dlgNewGameItem.GetSafeHwnd() && m_dlgNewGameItem.IsWindowVisible())
	{
		if( bForceClose )
		{
			CloseGameItemDlg();
		}
		else return FALSE;
	}

	m_dlgNewGameItem.DoModal(0, FALSE, 0);
//	m_dlgNewGameItem.DoModaless( 0, FALSE, 0, TRUE );
	return TRUE;
}

LRESULT CMainFrame::OnShowTimeLimitItemDlg(WPARAM &wParam, LPARAM &lParam)
{
	m_dlgTimeLimitAvatar.DoModal();
	//	m_dlgNewGameItem.DoModaless( 0, FALSE, 0, TRUE );
	return TRUE;
}

LRESULT CMainFrame::OnCloseTimeLimitItemDlg(WPARAM &wParam, LPARAM &lParam)
{
	if(m_dlgTimeLimitAvatar.GetSafeHwnd() != NULL)
	{		
		m_dlgTimeLimitAvatar.SafeClose();
	}
	return TRUE;
}

LRESULT CMainFrame::OnGetUserSmallInfo(WPARAM &wParam, LPARAM &lParam)
{
	const char *pID = (const char *)lParam;
	return (LRESULT) m_cLobby.GetLobbyProcess()->GetData_User(pID);
}

BOOL CMainFrame::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(IsFullScreenState())
	{
		CRect rtRect(0,0,1024,768);
		CBrush blackbrush;
		blackbrush.CreateSolidBrush(RGB(0,0,0));
		pDC->FillRect(&rtRect, &blackbrush);
	}

	return TRUE;
}

void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFrameWnd::OnShowWindow(bShow, nStatus);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

LRESULT CMainFrame::OnClearLoadImageList(WPARAM &wParam, LPARAM &lParam)
{
	m_dlgLoading.m_loadman.Clear();
	return TRUE;
}

LRESULT CMainFrame::OnAddLoadImageList(WPARAM &wParam, LPARAM &lParam)
{
	NMBASE::GRAPHICGDI::xSprite *pSprite = (NMBASE::GRAPHICGDI::xSprite *)wParam;
	char *pPath = (char *)lParam;

	m_dlgLoading.m_loadman.AddSprite(pSprite, pPath);
	return TRUE;
}

LRESULT CMainFrame::OnLoadingImage(WPARAM &wParam, LPARAM &lParam)
{
	m_dlgLoading.m_dwMinShowTime	=	(DWORD)wParam;
	m_dlgLoading.m_bBasedOnTime		=	(BOOL)lParam;

	m_dlgLoading.Create(IDD_DIALOG_LOADING, NULL);
	return TRUE;
}

LRESULT CMainFrame::OnShowChanceBtn(WPARAM &wParam, LPARAM &lParam)
{
	m_cLobby.GetLobbyProcess()->ShowChanceBtn();
	return TRUE;
}

LRESULT CMainFrame::OnOpenHiddenURL(WPARAM &wParam, LPARAM &lParam)
{
	const char *pURL =	(const char *)wParam;	

	if(m_pLocWebCtrl) 
	{
		m_pLocWebCtrl->Navigate(pURL);
		return TRUE;
	}
	else
	{
		return FALSE;
	}	
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return CFrameWnd::PreTranslateMessage(pMsg);
}




LRESULT CMainFrame::OnCreateDesctopIcon(WPARAM &wParam, LPARAM &lParam)
{

#ifdef _BETAVER
	return TRUE;
#endif 

	char szFileName[MAX_PATH]={0,};
	GetModuleFileName(g_hInstBaseDLL, szFileName, MAX_PATH-1);	//UPGBase.dll 안에 있는 아이콘으로 만든다.

	NMBASE::UTIL::CDesktopIconMan icon;
	//GM().GetCurrentGame()
	//버전체크하지않고 게임실행할때 마다 만든다 (아래 한줄 삭제)
//	if (icon.CheckSetVersion( "NetmarblePoker", GM().GetMyInfo()->UI.SiteCode))	
	{

	
		//for(int idxGame=0; idxGame<MAX_GAME; idxGame++)
		{
			//CGame *pGame = GM().GetGame((IDX_GAME)idxGame);
			CGame *pGame = GM().GetCurrentGame();//포커류 아이콘 생성 정책 변경
			IDX_GAME idxGame = pGame->GetGameType();
			if (pGame ==NULL)
				return FALSE;
				//continue;
			
			CString strModName = pGame->GetModuleName();	

			if ( GM().GetMyInfo()->UI.SiteCode == NMBASE::UTIL::SNO_NETMARBLE )
				icon.MakeIcon(getGameURL(IDX_GAMEURL_GAMEMAINPAGE, (LPARAM) idxGame), pGame->GetGameName(), pGame->GetGameName(), strModName, 0);
			else if ( GM().GetMyInfo()->UI.SiteCode == NMBASE::UTIL::SNO_NATE )
				icon.MakeIcon(getGameURL(IDX_GAMEURL_GAMEMAINPAGE_NATE, (LPARAM) idxGame), "네이트 " + pGame->GetGameName(), "네이트 " + pGame->GetGameName(), strModName, 0);
			else if ( GM().GetMyInfo()->UI.SiteCode == NMBASE::UTIL::SNO_TOCTOC )
				icon.MakeIcon(getGameURL(IDX_GAMEURL_GAMEMAINPAGE_TOCTOC, (LPARAM) idxGame), "톡톡 " + pGame->GetGameName(), "톡톡 " + pGame->GetGameName(), strModName, 0);

		}
		return TRUE;		//바탕화면 아이콘 생성
	}

	return FALSE;	
}

//아이템 상태가 변경 되었을때 호출된다.
LRESULT CMainFrame::OnUpdateItemData(WPARAM &wParam, LPARAM &lParam)
{
	return FALSE;
}

LRESULT CMainFrame::OnSetPokerIcon(WPARAM &wParam, LPARAM &lParam)
{
	//아이콘 설정
	m_hIcon = GM().GetCurrentGame()->GetGameIcon();
	SetIcon(m_hIcon, FALSE);
	return TRUE;
	
}
LONG CMainFrame::OnClosePopup(UINT, LONG)
{
	//포커상점닫기
	MM().Call(UPG_CMK_GAMEITEM_DLG_CLOSE);

	MM().Call(UPG_CMK_TIMELIMITITEM_DLG_CLOSE);

	//찬스아바타 구매창 닫기
	RewardItemMan().CloseBuyDlg();

	return 0;
}

LRESULT CMainFrame::OnSetVolume(WPARAM &wParam, LPARAM &lParam)
{
	m_cLobby.SetVolume();
	return 1;
}


LRESULT CMainFrame::OnCloseProfileDlg(WPARAM &wParam, LPARAM &lParam)
{
	if (m_dlgProfile.GetSafeHwnd())
	{
		m_dlgProfile.SendMessage(WM_COMMAND, IDCANCEL);
	}

	return 1;
}

LRESULT CMainFrame::OnShowProfileDlg(WPARAM &wParam, LPARAM &lParam)
{
	USERINFO *pInfo  = (USERINFO*) (wParam);
	if (pInfo==NULL)
		return 0;

	CPlayer *pMyPlayer = GM().GetMyInfo();
	if (pMyPlayer==NULL)
		return 0;

	m_dlgProfile.Init(pInfo);

	if (m_dlgProfile.GetSafeHwnd())
	{		
		return 0;
	}

	CWnd *pParent = (CWnd *)lParam;

	return (LRESULT) m_dlgProfile.DoModal(pParent?pParent:this);
}

LRESULT CMainFrame::OnShowDailyDlg(WPARAM &wParam, LPARAM &lParam)
{
	if (m_dlgDailyPopup.GetSafeHwnd())
	{		
		return 0;
	}

	CWnd *pParent = (CWnd *)lParam;

	return (LRESULT) m_dlgDailyPopup.DoModal(pParent?pParent:this);
}

LRESULT CMainFrame::OnShowEndEventDlg(WPARAM &wParam, LPARAM &lParam)
{
	if (m_dlgEndEvent.GetSafeHwnd())
	{		
		return 0;
	}

	CWnd *pParent = (CWnd *)lParam;

	return (LRESULT) m_dlgEndEvent.DoModal(pParent?pParent:this);
}

//규제안 : 손실금액 경고창을 연다
LRESULT CMainFrame::OnShowRestrictionDlg(WPARAM &wParam, LPARAM &lParam)
{
	if (m_dlgRestriction.GetSafeHwnd())
	{		
		return 0;
	}

	CWnd *pParent = (CWnd *)lParam;
	m_dlgRestriction.DoModal(GM().GetMainWnd(), GM().GetCurrentGameType());

	//return (LRESULT) m_dlgRestriction.DoModal(pParent?pParent:this);
	return 1;
}

LRESULT CMainFrame::OnHideRestrictionDlg(WPARAM &wParam, LPARAM &lParam)
{
	CWnd *pParent = (CWnd *)lParam;

	if (m_dlgRestriction.GetSafeHwnd() != NULL && m_dlgRestriction.IsWindowVisible())
		m_dlgRestriction.SendMessage(WM_COMMAND, IDCANCEL, 0);

	return 1;
}

LRESULT CMainFrame::OnCloseReceiveMessageDlg(WPARAM &wParam, LPARAM &lParam)
{
	if(m_dlgReceiveAdminMessage.GetSafeHwnd())
	{
		m_dlgReceiveAdminMessage.SendMessage(WM_COMMAND, IDCANCEL);
	}

	if (m_dlgReceiveMessage.GetSafeHwnd())
	{
		m_dlgReceiveMessage.SendMessage(WM_COMMAND, IDCANCEL);
	}

	return 1;
}

LRESULT CMainFrame::OnShowReceiveMessageDlg(WPARAM &wParam, LPARAM &lParam)
{
	Camel::Dar2::SS::Memo *pMemo  = (Camel::Dar2::SS::Memo *) (wParam);
	if (pMemo==NULL)
		return 0;

	if(m_dlgReceiveAdminMessage.GetSafeHwnd())
	{
		return 0;
	}

	if (m_dlgReceiveMessage.GetSafeHwnd())
	{		
		return 0;
	}

	CWnd *pParent = (CWnd *)lParam;
	LRESULT nRet = IDCANCEL;

	if(CCommunityManager::IsAdminMemo(pMemo))
	{
		if(m_dlgReceiveMessage.GetSafeHwnd())
		{
			m_dlgReceiveMessage.SendMessage(WM_COMMAND, IDCANCEL);
		}

		if(m_dlgReceiveAdminMessage.GetSafeHwnd())
		{
			m_dlgReceiveAdminMessage.ChangeMemo(pMemo);
		}
		else
		{
			nRet = (LRESULT) m_dlgReceiveAdminMessage.DoModal(pMemo, pParent?pParent:this);
		}
	}
	else
	{
		if(m_dlgReceiveAdminMessage.GetSafeHwnd())
		{
			m_dlgReceiveAdminMessage.SendMessage(WM_COMMAND, IDCANCEL);
		}

		if(m_dlgReceiveMessage.GetSafeHwnd())
		{
			m_dlgReceiveAdminMessage.ChangeMemo(pMemo);
		}
		else
		{
			nRet = (LRESULT) m_dlgReceiveMessage.DoModal(pMemo, pParent?pParent:this);
		}
	}
	
	return nRet;
}

LRESULT CMainFrame::OnCloseSendMessageDlg(WPARAM &wParam, LPARAM &lParam)
{
	if (m_dlgSendMessage.GetSafeHwnd())
	{
		m_dlgSendMessage.SendMessage(WM_COMMAND, IDCANCEL);
	}

	return 1;
}

// LRESULT CMainFrame::OnShowSendMessageDlg(WPARAM &wParam, LPARAM &lParam)
// {
// 	std::pair<std::string, std::string> *pTempPair = (std::pair<std::string, std::string> *)wParam;
// 
// 	if (pTempPair==NULL)
// 		return 0;
// 
// 	if (m_dlgSendMessage.GetSafeHwnd())
// 	{		
// 		return 0;
// 	}
// 
// 	CWnd *pParent = (CWnd *)lParam;
// 
// 	return (LRESULT) m_dlgSendMessage.DoModal(pTempPair->first, pTempPair->second, pParent?pParent:this);
// }

LRESULT CMainFrame::OnCloseWhisperDlg(WPARAM &wParam, LPARAM &lParam)
{
	if (m_dlgWhisper.GetSafeHwnd())
	{
		m_dlgWhisper.SendMessage(WM_COMMAND, IDCANCEL);
	}

	return 1;
}

LRESULT CMainFrame::OnShowWhisperDlg(WPARAM &wParam, LPARAM &lParam)
{
	SMALLUSERINFO *pUserInfo = (SMALLUSERINFO *)(wParam);
	if (pUserInfo==NULL)
		return 0;

	if (m_dlgWhisper.GetSafeHwnd())
	{		
		return 0;
	}

	CWnd *pParent = (CWnd *)lParam;

	return (LRESULT) m_dlgWhisper.DoModal(pUserInfo->NickName, pUserInfo->ID, pUserInfo->llUserKey, pParent?pParent:this);
}

LRESULT CMainFrame::OnCloseMessageBoxDlg(WPARAM &wParam, LPARAM &lParam)
{
	if (m_dlgMessageBox.GetSafeHwnd())
	{
		m_dlgMessageBox.SendMessage(WM_COMMAND, IDCANCEL);
	}

	return 1;
}

LRESULT CMainFrame::OnShowMessageBoxDlg(WPARAM &wParam, LPARAM &lParam)
{
	if (m_dlgMessageBox.GetSafeHwnd())
	{		
		return 0;
	}

	return (LRESULT) m_dlgMessageBox.DoModal(this);
}

LRESULT CMainFrame::OnShowGoldChangeDlg(WPARAM &wParam, LPARAM &lParam)
{
	if (m_dlgGoldChange.GetSafeHwnd())
	{		
		return 0;
	}

	return (LRESULT) m_dlgGoldChange.DoModal(this);
}

LRESULT CMainFrame::OnShowGoldBigWheelDlg(WPARAM &wParam, LPARAM &lParam)
{
	int remainCount = (int)wParam;	
	INT64 usingGold = (INT64)lParam;

	if(usingGold <= 0)
	{
		CCL_PM_BIGWHEEL_GET_REMAIN_GAMECOUNT msg;
		msg.Set();
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());

		return 0;
	}

	if (m_dlgGoldBigWheel.GetSafeHwnd())
	{		
		return 0;
	}

	return (LRESULT) m_dlgGoldBigWheel.DoModal(remainCount, usingGold, this);
}


LRESULT CMainFrame::OnShowClubDlg(WPARAM &wParam, LPARAM &lParam)
{
	if (m_dlgClub.GetSafeHwnd())
	{
		return 0;
	}

	GM().SetIsShowingClubDlg(true);

	{
		CCL_CLUB_REQUEST_DETAIL_INFO msg;
		msg.Set();
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	}

	if(GM().GetClubMemberGrade()==CLUB_MEMBER_GRADE_NONE)
	{
		CCL_CLUB_REQUEST_MYSELF_INVITED_LIST msg;
		msg.Set(-1);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	}

	if(GM().GetClubMemberGrade()==CLUB_MEMBER_GRADE_NONE)
	{
		CCL_CLUB_REQUEST_INVITED_LIST msg;
		msg.Set(-1);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	}

	if(GM().GetClubMemberGrade()==CLUB_MEMBER_GRADE_BOSS)
	{
		CCL_CLUB_REQUEST_MYSELF_JOIN_LIST msg;
		msg.Set(-1);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	}

	if(GM().GetClubMemberGrade()==CLUB_MEMBER_GRADE_BOSS)
	{
		CCL_CLUB_REQUEST_JOIN_LIST msg;
		msg.Set(-1);
		NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	}
	
	return (LRESULT) m_dlgClub.DoModal(this);
}

LRESULT CMainFrame::OnShowUPPDlg(WPARAM &wParam, LPARAM &lParam)
{
	if (m_dlgUPP.GetSafeHwnd())
	{		
		return 0;
	}
	
	return (LRESULT) m_dlgUPP.DoModal(this, (int *)wParam, (char*)lParam);
}

LRESULT CMainFrame::OnShowRewardOver(WPARAM &wParam, LPARAM &lParam)
{
	CRestrictionOverPopup dlg;
	if (dlg.GetSafeHwnd())
	{		
		return 0;
	}

	return (LRESULT) dlg.DoModal(this);
}

LRESULT CMainFrame::OnGetGoldLuckyBagPopup(WPARAM &wParam, LPARAM &lParam)
{
	CGoldLuckyBagPopup **ppDlg = (CGoldLuckyBagPopup **)lParam;
	*ppDlg = &m_popGoldLuckyBag;
	return TRUE;
}

LRESULT CMainFrame::OnShowGoldLuckyBagPopup(WPARAM &wParam, LPARAM &lParam)
{
	if (m_popGoldLuckyBag.GetSafeHwnd())
	{
		return 0;
	}
	
	return (LRESULT)m_popGoldLuckyBag.DoModal(this);
}

LRESULT CMainFrame::OnGetNewYearAttendancePopup(WPARAM &wParam, LPARAM &lParam)
{
	CNewYearAttendancePopup **ppDlg = (CNewYearAttendancePopup **)lParam;
	*ppDlg = &m_popNewYearAttendance;
	return TRUE;
}

LRESULT CMainFrame::OnCreateNewYearAttendancePopup(WPARAM &wParam, LPARAM &lParam)
{
	if (m_popNewYearAttendance.GetSafeHwnd())
	{
		return 0;
	}
	
	return -1;
}

LRESULT CMainFrame::OnShowNewYearAttendancePopup(WPARAM &wParam, LPARAM &lParam)
{
	return (LRESULT)m_popNewYearAttendance.DoModal(this);
}

// 골드 적립금
LRESULT CMainFrame::OnGetGoldMileagePopup(WPARAM &wParam, LPARAM &lParam)
{
	CGoldMileagePopup **ppDlg = (CGoldMileagePopup **)lParam;
	*ppDlg = &m_popGoldMileage;
	return TRUE;
}

LRESULT CMainFrame::OnCreateGoldMileagePopup(WPARAM &wParam, LPARAM &lParam)
{
	if (m_popGoldMileage.GetSafeHwnd())
	{
		return 0;
	}
	
	return -1;
}

LRESULT CMainFrame::OnShowGoldMileagePopup(WPARAM &wParam, LPARAM &lParam)
{
	return (LRESULT)m_popGoldMileage.DoModal(this);
}

// 골드 지원금
LRESULT CMainFrame::OnGetGoldSupportFundPopup(WPARAM &wParam, LPARAM &lParam)
{
	CGoldSupportFundPopup **ppDlg = (CGoldSupportFundPopup **)lParam;
	*ppDlg = &m_popGoldSupportFund;

	return TRUE;
}

LRESULT CMainFrame::OnCreateGoldSupportFundPopup(WPARAM &wParam, LPARAM &lParam)
{
	if (m_popGoldSupportFund.GetSafeHwnd())
	{
		return 0;
	}
	
	return -1;
}

LRESULT CMainFrame::OnShowGoldSupportFundPopup(WPARAM &wParam, LPARAM &lParam)
{
	return (LRESULT)m_popGoldSupportFund.DoModal(this);
}

// 골드 출석부
LRESULT CMainFrame::OnGetGoldAttendanceBookPopup(WPARAM &wParam, LPARAM &lParam)
{
	//CGoldAttendanceBookPopup **ppDlg = (CGoldAttendanceBookPopup **)lParam;
	//*ppDlg = &m_popGoldAttendanceBook;
	CChipAttendPopup **ppDlg = (CChipAttendPopup **)lParam;
	*ppDlg = &m_chipAttendPopup;

	return TRUE;
}

LRESULT CMainFrame::OnCreateGoldAttendanceBookPopup(WPARAM &wParam, LPARAM &lParam)
{
	//if (m_popGoldAttendanceBook.GetSafeHwnd())
	if (m_chipAttendPopup.GetSafeHwnd())
	{
		return 0;
	}
	
	return -1;
}

LRESULT CMainFrame::OnShowGoldAttendanceBookPopup(WPARAM &wParam, LPARAM &lParam)
{
	//return (LRESULT)m_popGoldAttendanceBook.DoModal(this);

	LRESULT result = (LRESULT)m_chipAttendPopup.DoModal(this);

	// 한도 초과 팝업을 딜레이 시켜야 할 경우
	if (GM().GetWaitOwnLimitPopup() == true)
	{
		//MM().Call(UPG_CMK_SHOW_GOLD_OWNLIMIT_POPUP);
		GM().SetWaitOwnLimitPopup(false);
		m_cLobby.ShowPopup();
	}

	// 골드 보험 팝업을 딜레이 시켜야 할 경우
	if (GM().GetWaitGoldInsuPopup() == true)
	{
		GM().SetWaitGoldInsuPopup(false);
		ShowNewAskChargeDlg(NEW_ASK_CHARGE_DLG_TYPE_INSU_GOLD2, NULL);
	}

	return result;
}

// 골드 적립 통장 출금
LRESULT CMainFrame::OnGetGoldWithdrawPopup(WPARAM &wParam, LPARAM &lParam)
{
	CGoldWithdrawPopup **ppDlg = (CGoldWithdrawPopup **)lParam;
	*ppDlg = &m_popGoldWithdraw;

	return TRUE;
}

LRESULT CMainFrame::OnShowGoldWithdrawPopup(WPARAM &wParam, LPARAM &lParam)
{
	// 다이얼로그가 이미 존재할 경우
	if (m_popGoldWithdraw.GetSafeHwnd())
	{
		return 0;
	}

	return (LRESULT)m_popGoldWithdraw.DoModal(this);
}

LRESULT CMainFrame::OnShowCaptchaDlg(WPARAM &wParam, LPARAM &lParam)
{
	if (m_captchaDlg.GetSafeHwnd() != NULL)
		return FALSE;

	return (LRESULT)m_captchaDlg.DoModal();
}

LRESULT CMainFrame::OnCloseCaptchaDlg(WPARAM &wParam, LPARAM &lParam)
{
	if (!m_captchaDlg.SafeClose())
	{
		return FALSE;
	}

	return TRUE;
}

LRESULT CMainFrame::OnGetCaptchaDlg(WPARAM &wParam, LPARAM &lParam)
{
	CCaptchaDlg **ppDlg = (CCaptchaDlg **)lParam;
	*ppDlg = &m_captchaDlg;

	return TRUE;
}

LRESULT CMainFrame::OnGetAuthentiCodeOfCaptcha(WPARAM &wParam, LPARAM &lParam)
{	
	CString authentiCode = m_captchaDlg.GetAuthentiCode();
	*((CString*)wParam) = authentiCode;

	return TRUE;
}

LRESULT CMainFrame::OnSetAuthentiCodeOfCaptcha(WPARAM &wParam, LPARAM &lParam)
{
	CString authentiCode = *((CString*)wParam);
	m_captchaDlg.SetAuthentiCode(authentiCode);

	return TRUE;
}

LRESULT CMainFrame::OnGetNeedCaptcha(WPARAM &wParam, LPARAM &lParam)
{
	wParam = (WPARAM)m_captchaDlg.GetNeedCaptcha();

	return TRUE;
}

LRESULT CMainFrame::OnSetNeedCaptcha(WPARAM &wParam, LPARAM &lParam)
{
	bool needCaptcha = false;

	if(wParam)
	{
		needCaptcha = true;
	}

	m_captchaDlg.SetNeedCaptcha(needCaptcha);
	
	return TRUE;
}

LRESULT CMainFrame::OnGetChuSeokAttendanceDlg(WPARAM& wParam, LPARAM& lParam)
{
	CChuSeokDlg** ppChuseokDlg = (CChuSeokDlg**)lParam;
	*ppChuseokDlg = &m_chuSeokDlg;

	return TRUE;
}

LRESULT CMainFrame::OnShowChuSeokAttendanceDlg(WPARAM& wParam, LPARAM& lParam)
{
	if(m_chuSeokDlg.GetSafeHwnd())
	{
		return FALSE;
	}

	return (LRESULT)m_chuSeokDlg.DoModal();
}

LRESULT CMainFrame::OnGetChuSeokPopup(WPARAM& wParam, LPARAM& lParam)
{
	CChuSeokPopup** ppPopChuSeok = (CChuSeokPopup**)lParam;
	*ppPopChuSeok = &m_popChuSeok;

	return TRUE;
}

LRESULT CMainFrame::OnShowChuSeokPopup(WPARAM& wParam, LPARAM& lParam)
{
	if (m_popChuSeok.GetSafeHwnd())
	{
		return FALSE;
	}

	LRESULT result = (LRESULT)m_popChuSeok.DoModal();

	// 한도 초과 팝업을 딜레이 시켜야 할 경우
	if (GM().GetWaitOwnLimitPopup() == true)
	{
		//MM().Call(UPG_CMK_SHOW_GOLD_OWNLIMIT_POPUP);
		GM().SetWaitOwnLimitPopup(false);
		m_cLobby.ShowPopup();
	}

	// 골드 보험 팝업을 딜레이 시켜야 할 경우
	if (GM().GetWaitGoldInsuPopup() == true)
	{
		GM().SetWaitGoldInsuPopup(false);
		ShowNewAskChargeDlg(NEW_ASK_CHARGE_DLG_TYPE_INSU_GOLD2, NULL);
	}

	return result;
}

LRESULT CMainFrame::OnGetLobbyNotifyDlg(WPARAM& wParam, LPARAM& lParam)
{
	CLobbyNotifyDlg** ppLobbyNotifyDlg = (CLobbyNotifyDlg**)lParam;
	*ppLobbyNotifyDlg = &m_lobbyNotifyDlg;

	return TRUE;
}

LRESULT CMainFrame::OnShowLobbyNotifyDlg(WPARAM& wParam, LPARAM& lParam)
{
	if (m_lobbyNotifyDlg.GetSafeHwnd())
	{
		return FALSE;
	}

	return (LRESULT)m_lobbyNotifyDlg.DoModal();
}

LRESULT CMainFrame::OnShowMsgModelessDlg(WPARAM& wParam, LPARAM& lParam)
{
	if(m_modelessMsgBoxDlg.GetSafeHwnd())
	{
		m_modelessMsgBoxDlg.ShowWindow(true, (LPCTSTR)lParam);
		return FALSE;
	}	

	return (LRESULT)m_modelessMsgBoxDlg.ShowMsg((HWND)wParam, (LPCTSTR)lParam);
}

void CMainFrame::OnDraw(void)
{
	AllDrawObjectMan();
}

LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	switch(message)
	{
	case WM_TIMER:
		if (wParam == TIMER_UPGDLGBASE_REDRAW)
		{
			OnDraw();

			return TRUE;
		}
		break;
	}

	if(message >= CM_MESSAGE_FIRST && message <= CM_MESSAGE_LAST)
	{
		return m_cLobby.GetLobbyView()->WindowProc(message, wParam, lParam);
	}

	return CFrameWnd::WindowProc(message, wParam, lParam);
}

void CMainFrame::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	if (GetCurrentWhere() == IDX_GAMEWHERE_GAME)
	{
#ifdef _DEBUG
		//ExitGameRequest();
		GM().ExitSubscript();
		
#else	
		GM().ExitSubscript();		

#endif

		return;
	}



	CFrameWnd::OnClose();
}

TCHAR* blockProcessName[] = 
{
	"AFCrashReport.exe",
	"afreecastudio.exe",
	"AIManager.exe",
	"ChatStudio.exe",
	"AFStudio2.exe",
	"obs64.exe", 
	"bdcam.exe", 
	"oCam.exe", 
	"GOMCam.exe"
};

int CMainFrame::CheckAllProcessName()
{
	return 0;

	CString strProcessName;    
	CString strProcessPid;

	DWORD pid = 0;

	HANDLE hProcess = NULL;

	PROCESSENTRY32 pe32 = { 0 };
	hProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hProcess, &pe32))
	{
		do
		{
			strProcessName = pe32.szExeFile;
			pid = pe32.th32ProcessID;
			strProcessPid.Format(_T("%lu"), pid);

			//TRACE("%s\n", strProcessName + _T("/") + strProcessPid);
			
			for(int j = 0; j < sizeof(blockProcessName) / sizeof(TCHAR*); ++j)
			{
				if(strcmp(strProcessName, blockProcessName[j]) == 0)
				{
					return 1;
				}
			}
		}

		while (Process32Next(hProcess, &pe32));
	}

	CloseHandle(hProcess);

	return 0;
}

