// CreateRoomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TimerNotifierDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTimeNotifierDlg dialog

//CTimeNotifierDlg::CTimeNotifierDlg(CWnd* pParent /*=NULL*/)
CTimeNotifierDlg::CTimeNotifierDlg(CWnd* pParent /*=NULL*/): CUPGDlgBase(CTimeNotifierDlg::IDD, pParent)
{
	m_eExitUseType = ENUM_TYPE_EXIT_USE_NONE;
	m_sExitTime = 0;
	m_eTimeNotifyType = ENUM_TYPE_TIME_NOTIFY_NONE;
	m_nTotalRound = 0;
	m_nPlayTimeSec = 0 ;
	m_strBackFile = "";
	m_bEnableInitBtn = FALSE;
}


void CTimeNotifierDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimeNotifierDlg)
	DDX_Control(pDX, IDC_BUTTON_NONE_EXIT, m_rdExitNone);
	DDX_Control(pDX, IDC_BUTTON_ONCE_EXIT, m_rdExitOnce);
	DDX_Control(pDX, IDC_BUTTON_ALWAYS_EXIT, m_rdExitAlways);
	DDX_Control(pDX, IDC_BUTTON_1HOUR, m_rd1Hour);
	DDX_Control(pDX, IDC_BUTTON_2HOUR, m_rd2Hour);
	DDX_Control(pDX, IDC_BUTTON_3HOUR, m_rd3Hour);
	DDX_Control(pDX, IDC_BUTTON_4HOUR, m_rd4Hour);
	DDX_Control(pDX, IDC_BUTTON_5HOUR, m_rd5Hour);
	DDX_Control(pDX, IDC_BUTTON_6HOUR, m_rd6Hour);
	DDX_Control(pDX, IDC_BUTTON_NONE_NOTIFY, m_rdNotiNone);
	DDX_Control(pDX, IDC_BUTTON_30MIN_NOTIFY, m_rdNoti30Min);
	DDX_Control(pDX, IDC_BUTTON_1HOUR_NOTIFY, m_rdNoti1Hour);

	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDOK, m_btnOK);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTimeNotifierDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CTimeNotifierDlg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimeNotifierDlg message handlers

BOOL CTimeNotifierDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();

	// TODO: Add extra initialization here

	
	// 다이얼로그 컨트롤 포지션과 위치 재정렬
	NMBASE::UTIL::CDialogCtrlReposition CtrlRepos;
	CtrlRepos.Reposition(m_hWnd);	
	

	CPoint ptOk, ptCancel;
	
	//ptOk	 = CPoint(101, 318);
	//ptCancel = CPoint(191, 318);
	ptOk	 = CPoint(101, 374);
	ptCancel = CPoint(191, 374);	

	m_strBackFile = "PokerCommon\\image\\upp\\Lobby_TimeNotifier_Back.bmp";		
	
	m_btnOK.Init(ptOk.x, ptOk.y, ".\\PokerCommon\\image\\upp\\btn_init.bmp", 4);
	m_btnOK.SetClickSoundID(SND_BUTTON_CLICK_B);
	m_btnCancel.Init(ptCancel.x, ptCancel.y, ".\\PokerCommon\\image\\upp\\btn_save.bmp", 4);

	CRect rt;

	m_rdExitNone.ShowWindow(SW_SHOW);
	m_rdExitOnce.ShowWindow(SW_SHOW);
	m_rdExitAlways.ShowWindow(SW_SHOW);

	m_rd1Hour.ShowWindow(SW_SHOW);
	m_rd2Hour.ShowWindow(SW_SHOW);
	m_rd3Hour.ShowWindow(SW_SHOW);
	m_rd4Hour.ShowWindow(SW_SHOW);
	m_rd5Hour.ShowWindow(SW_HIDE);
	m_rd6Hour.ShowWindow(SW_HIDE);

	m_rdNotiNone.ShowWindow(SW_SHOW);
	m_rdNoti30Min.ShowWindow(SW_SHOW);
	m_rdNoti1Hour.ShowWindow(SW_SHOW);
	

	m_rdExitNone.Init(31,144,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
	m_rdExitOnce.Init(121,144,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
	m_rdExitAlways.Init(211,144,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);

	m_rdExitNone.AddToGroup(&m_rdExitOnce); 
	m_rdExitNone.AddToGroup(&m_rdExitAlways);

	//m_rd1Hour.Init(32,205,".\\PokerCommon\\image\\upp\\check.bmp",NULL,this);
	//m_rd2Hour.Init(85,205,".\\PokerCommon\\image\\upp\\check.bmp",NULL,this);
	//m_rd3Hour.Init(138,205,".\\PokerCommon\\image\\upp\\check.bmp",NULL,this);
	//m_rd4Hour.Init(191,205,".\\PokerCommon\\image\\upp\\check.bmp",NULL,this);
	//m_rd5Hour.Init(244,205,".\\PokerCommon\\image\\upp\\check.bmp",NULL,this);
	//m_rd6Hour.Init(297,205,".\\PokerCommon\\image\\upp\\check.bmp",NULL,this);

	m_rd1Hour.Init(72,205,".\\PokerCommon\\image\\upp\\check.bmp",NULL,this);
	m_rd2Hour.Init(135,205,".\\PokerCommon\\image\\upp\\check.bmp",NULL,this);
	m_rd3Hour.Init(198,205,".\\PokerCommon\\image\\upp\\check.bmp",NULL,this);
	m_rd4Hour.Init(261,205,".\\PokerCommon\\image\\upp\\check.bmp",NULL,this);
	m_rd5Hour.Init(600,205,".\\PokerCommon\\image\\upp\\check.bmp",NULL,this);
	m_rd6Hour.Init(600,205,".\\PokerCommon\\image\\upp\\check.bmp",NULL,this);

	m_rd1Hour.AddToGroup(&m_rd2Hour);
	m_rd1Hour.AddToGroup(&m_rd3Hour);
	m_rd1Hour.AddToGroup(&m_rd4Hour);
	m_rd1Hour.AddToGroup(&m_rd5Hour);
	m_rd1Hour.AddToGroup(&m_rd6Hour);

	m_rdNotiNone.Init(31, 275,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
	m_rdNoti30Min.Init(121, 275,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);
	m_rdNoti1Hour.Init(211, 275,".\\PokerCommon\\image\\CreateRoom\\rdo_sel.bmp",NULL,this);

	m_rdNotiNone.AddToGroup(&m_rdNoti30Min);
	m_rdNotiNone.AddToGroup(&m_rdNoti1Hour);		

	
// 	m_rd1Hour.SetCheck(TRUE);
// 	m_rdNotiNone.SetCheck(TRUE);

	switch (m_eExitUseType)
	{
	case ENUM_TYPE_EXIT_USE_NONE: m_rdExitNone.SetCheck(TRUE); break;
	case ENUM_TYPE_EXIT_USE_ONCE: m_rdExitOnce.SetCheck(TRUE); break;
	case ENUM_TYPE_EXIT_USE_ALWAYS: m_rdExitAlways.SetCheck(TRUE); break;
	}

	switch (m_sExitTime)
	{
	case 1: m_rd1Hour.SetCheck(TRUE); break;
	case 2: m_rd2Hour.SetCheck(TRUE); break;
	case 3: m_rd3Hour.SetCheck(TRUE); break;
	case 4: m_rd4Hour.SetCheck(TRUE); break;
	case 5: m_rd5Hour.SetCheck(TRUE); break;
	case 6: m_rd6Hour.SetCheck(TRUE); break;
	}

	switch (m_eTimeNotifyType)
	{
		case ENUM_TYPE_TIME_NOTIFY_NONE: m_rdNotiNone.SetCheck(TRUE); break;
		case ENUM_TYPE_TIME_NOTIFY_30MIN: m_rdNoti30Min.SetCheck(TRUE); break;
		case ENUM_TYPE_TIME_NOTIFY_1HOUR: m_rdNoti1Hour.SetCheck(TRUE); break;
	}

	rt.SetRect(0,0,178,13);
	rt.OffsetRect(98,44);
//	GetDlgItem(IDC_EDIT_ROOMTITLE)->MoveWindow(&rt);

	SetTimer(TIMER_TIMENOTIFIERDLG, 60000, NULL);

	m_rtUsingTime = CRect(106, 74, 106 + 161, 74 + 17);

	NMBASE::SKINGDI::WindowResize2Bitmap(m_hWnd, m_strBackFile);
	UpdateData(FALSE);	

	m_btnOK.EnableWindow(m_bEnableInitBtn);

	m_rdExitNone.EnableWindow(m_bEnableInitBtn);
	m_rdExitOnce.EnableWindow(m_bEnableInitBtn);
	m_rdExitAlways.EnableWindow(m_bEnableInitBtn);

	m_rd1Hour.EnableWindow(m_bEnableInitBtn);
	m_rd2Hour.EnableWindow(m_bEnableInitBtn);
	m_rd3Hour.EnableWindow(m_bEnableInitBtn);
	m_rd4Hour.EnableWindow(m_bEnableInitBtn);
	m_rd5Hour.EnableWindow(m_bEnableInitBtn);
	m_rd6Hour.EnableWindow(m_bEnableInitBtn);

	m_rdNotiNone.EnableWindow(m_bEnableInitBtn);
	m_rdNoti30Min.EnableWindow(m_bEnableInitBtn);
	m_rdNoti1Hour.EnableWindow(m_bEnableInitBtn);

	m_nPlayTimeSec = GM().GetCurrentPlayTime() ;

	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTimeNotifierDlg::OnOK()
{
	#ifdef UPGRADE_10210619_ADD_CLICKLOG
		SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_TIMEALARM_RESET);
	#endif //UPGRADE_10210619_ADD_CLICKLOG

	// TODO: Add extra validation here
//	EndDialog(IDOK);
	//초기화 코드를 입력한다.
	m_rdExitNone.SetCheck(TRUE);
	m_rdExitOnce.SetCheck(FALSE);
	m_rdExitAlways.SetCheck(FALSE);

	m_rd1Hour.SetCheck(FALSE);
	m_rd2Hour.SetCheck(FALSE);
	m_rd3Hour.SetCheck(FALSE);
	m_rd4Hour.SetCheck(FALSE);
	m_rd5Hour.SetCheck(FALSE);
	m_rd6Hour.SetCheck(FALSE);

	m_rdNotiNone.SetCheck(TRUE);
	m_rdNoti30Min.SetCheck(FALSE);
	m_rdNoti1Hour.SetCheck(FALSE);

	m_eExitUseType = ENUM_TYPE_EXIT_USE_NONE;
	m_sExitTime = 0;
	m_eTimeNotifyType = ENUM_TYPE_TIME_NOTIFY_NONE;

}

void CTimeNotifierDlg::OnCancel() 
{
	#ifdef UPGRADE_10210619_ADD_CLICKLOG
		SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_TIMEALARM_SET);
	#endif //UPGRADE_10210619_ADD_CLICKLOG

	// TODO: Add extra cleanup here
	int nWarning = 0;
	switch(m_eTimeNotifyType)
	{
	case ENUM_TYPE_TIME_NOTIFY_NONE: nWarning = 0 ; break;
	case ENUM_TYPE_TIME_NOTIFY_30MIN: nWarning = 1800 ; break;
	case ENUM_TYPE_TIME_NOTIFY_1HOUR: nWarning = 3600 ; break;
	}
	MM().Call(UPG_CMK_RESET_WARNINGTIMER, (WPARAM)nWarning);

	CCL_ASK_UPPMSG msg;
	msg.Set((int)CCL_ASK_UPPMSG::SETALARM, nWarning, (int)m_sExitTime * 3600, (int)m_eExitUseType);
//	msg.Set((int)CCL_ASK_UPPMSG::SETALARM, nWarning, (int)m_sExitTime * 60, (int)m_eExitUseType);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	EndDialog(IDCANCEL);
}

void CTimeNotifierDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);	
	if(cdc.m_hDC == NULL) return;

	if(m_imgBack.m_hObject) m_imgBack.DeleteObject();

	m_imgBack.LoadBitmapFile(m_strBackFile);
	
	cdc.SelectObject(&m_imgBack);
	
	//남은시간
	CString strText;
//	COleDateTimeSpan timeProceed = g_GetSyncTime();
	strText.Format("%02d시간 %02d분 (%d판)", m_proceedTime.GetHours(), m_proceedTime.GetMinutes(), m_nTotalRound );
	cdc.SetBkMode(TRANSPARENT);
	cdc.SetTextColor(RGB(20,90,160));
	cdc.SelectObject(NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_14D_BOLD));

	cdc.DrawText(strText, &m_rtUsingTime, DT_VCENTER|DT_SINGLELINE|DT_CENTER);

	cdc.SetTextColor(RGB(192,27,27));
	cdc.SetTextAlign(TA_RIGHT) ;
	cdc.SelectObject(NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_12D_NORMAL));

	int hour = m_nPlayTimeSec / 3600 ;
	int min  = (m_nPlayTimeSec % 3600) / 60 ;

	CString str ;
	str.Format("%d", hour) ;
	cdc.TextOut(207,305, str);

	str.Format("%d", min) ;
	cdc.TextOut(251,305, str);

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	m_imgBack.DeleteObject();

	cdc.DeleteDC();	
}

void CTimeNotifierDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));
	CUPGDlgBase::OnLButtonDown(nFlags, point);
}

BOOL CTimeNotifierDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	return TRUE;
}

HBRUSH CTimeNotifierDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CUPGDlgBase::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here
	switch(nCtlColor) {

	case CTLCOLOR_STATIC:
		// The Slider Control has CTLCOLOR_STATIC, but doesn't let
		// the background shine through,
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(255,255,255));
		TCHAR lpszClassName[255];
		GetClassName(pWnd->m_hWnd, lpszClassName, 255);
		if(_tcscmp(lpszClassName, TRACKBAR_CLASS) == 0)
		return CUPGDlgBase::OnCtlColor(pDC, pWnd, nCtlColor);
// 	case CTLCOLOR_BTN:
// 		// let static controls shine through
// 		pDC->SetBkMode(TRANSPARENT);
// 		return HBRUSH(m_cHollowBrush);
	default:

		break;
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}

void CTimeNotifierDlg::OnDestroy() 
{
	CUPGDlgBase::OnDestroy();

	// TODO: Add your message handler code here
	KillTimer(TIMER_TIMENOTIFIERDLG);
}

void CTimeNotifierDlg::OnTimer(UINT nIDEvent)
{
	if (nIDEvent == TIMER_TIMENOTIFIERDLG)
	{
	}

	CUPGDlgBase::OnTimer(nIDEvent);
}


BOOL CTimeNotifierDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	switch(wParam)
	{
	case IDC_BUTTON_NONE_EXIT: 
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_TIMEALARM_GAMEEND_RESERVE_NOTUSE);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			m_eExitUseType = ENUM_TYPE_EXIT_USE_NONE; 
			m_sExitTime = 0;
			m_rd1Hour.SetCheck(FALSE);
			m_rd2Hour.SetCheck(FALSE);
			m_rd3Hour.SetCheck(FALSE);
			m_rd4Hour.SetCheck(FALSE);
			m_rd5Hour.SetCheck(FALSE);
			m_rd6Hour.SetCheck(FALSE);
		}break;
	case IDC_BUTTON_ONCE_EXIT:
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_TIMEALARM_GAMEEND_RESERVE_ONETIME);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			m_eExitUseType = ENUM_TYPE_EXIT_USE_ONCE; 
		}break;
	case IDC_BUTTON_ALWAYS_EXIT: 
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_TIMEALARM_GAMEEND_RESERVE_ALWAYS);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			m_eExitUseType = ENUM_TYPE_EXIT_USE_ALWAYS; 
		}break;
	case IDC_BUTTON_1HOUR: 
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_TIMEALARM_ENDTIME_CHECK_1HOUR);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			m_sExitTime = 1; 
		}break;
	case IDC_BUTTON_2HOUR: 
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_TIMEALARM_ENDTIME_CHECK_2HOUR);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			m_sExitTime = 2; 
		}break;
	case IDC_BUTTON_3HOUR: 
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_TIMEALARM_ENDTIME_CHECK_3HOUR);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			m_sExitTime = 3; 
		}break;
	case IDC_BUTTON_4HOUR: 
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_TIMEALARM_ENDTIME_CHECK_4HOUR);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			m_sExitTime = 4; 
		}break;
	case IDC_BUTTON_5HOUR: 
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_TIMEALARM_ENDTIME_CHECK_5HOUR);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			m_sExitTime = 5; 
		}break;
	case IDC_BUTTON_6HOUR: 
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_TIMEALARM_ENDTIME_CHECK_6HOUR);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			m_sExitTime = 6; 
		}break;
	case IDC_BUTTON_NONE_NOTIFY : 
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_TIMEALARM_PLAYTIME_CHECK_NOTUSE);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			m_eTimeNotifyType = ENUM_TYPE_TIME_NOTIFY_NONE; 
		}break;
	case IDC_BUTTON_30MIN_NOTIFY: 
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_TIMEALARM_PLAYTIME_CHECK_30M);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			m_eTimeNotifyType = ENUM_TYPE_TIME_NOTIFY_30MIN; 
		}break;
	case IDC_BUTTON_1HOUR_NOTIFY: 
		{
			#ifdef UPGRADE_10210619_ADD_CLICKLOG
				SendUILog_Basic(CSSTATUS_UINO_POKER_LOBBY_TITLEBAR_OPTION_TIMEALARM_PLAYTIME_CHECK_1H);
			#endif //UPGRADE_10210619_ADD_CLICKLOG
			m_eTimeNotifyType = ENUM_TYPE_TIME_NOTIFY_1HOUR; 
		}break;
	}

	switch(wParam)
	{
		case IDC_BUTTON_1HOUR:
		case IDC_BUTTON_2HOUR:
		case IDC_BUTTON_3HOUR:
		case IDC_BUTTON_4HOUR:
		case IDC_BUTTON_5HOUR:
		case IDC_BUTTON_6HOUR:
			{
				if (m_eExitUseType == ENUM_TYPE_EXIT_USE_NONE)
				{
					m_eExitUseType = ENUM_TYPE_EXIT_USE_ONCE;
					m_rdExitNone.SetCheck(FALSE);
					m_rdExitOnce.SetCheck(TRUE);
					m_rdExitAlways.SetCheck(FALSE);
				}
			}break;
		case IDC_BUTTON_ONCE_EXIT: 
		case IDC_BUTTON_ALWAYS_EXIT:
			{
				if ( 0 == m_sExitTime )
				{
					m_sExitTime = 1;
					m_rd1Hour.SetCheck(TRUE);
					m_rd2Hour.SetCheck(FALSE);
					m_rd3Hour.SetCheck(FALSE);
					m_rd4Hour.SetCheck(FALSE);
					m_rd5Hour.SetCheck(FALSE);
					m_rd6Hour.SetCheck(FALSE);
				}
			}break;
	}
	return CUPGDlgBase::OnCommand(wParam, lParam);
}
BOOL CTimeNotifierDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	if ( pMsg->message == WM_KEYDOWN  )
	{
		if (pMsg->wParam==VK_TAB)
		{
			BOOL bRet = CUPGDlgBase::PreTranslateMessage(pMsg);
			Invalidate(FALSE);			
			return bRet;
		}
	}


	return CUPGDlgBase::PreTranslateMessage(pMsg);

}
INT_PTR CTimeNotifierDlg::DoModal(CSV_UPPINFO *pMsg )
{
	m_bEnableInitBtn = FALSE;
	if(pMsg != NULL)
	{
		//시간과 판수정보를 받아 저장한다.
		m_proceedTime.SetDateTimeSpan(0,0,0,*pMsg->m_play);
		m_nTotalRound = *pMsg->m_round;

		//등급이 지정되어있을때 강제 지정한다.
		if ((*pMsg->m_level == 1)||(*pMsg->m_level == 2))
		{
			m_eExitUseType = ENUM_TYPE_EXIT_USE_ALWAYS;
			m_sExitTime = 1;
			m_eTimeNotifyType = ENUM_TYPE_TIME_NOTIFY_30MIN;

			m_bEnableInitBtn = FALSE;
		}
		else
		{
		//아닐경우 유저 세팅을 호출한다
			m_sExitTime = (short)*pMsg->m_exit / 3600;
//			m_sExitTime = (short)*pMsg->m_exit / 60;
// 			if (m_sExitTime > 0)
// 			{
// 				m_eExitUseType = ENUM_TYPE_EXIT_USE_ONCE;
// 			}
			switch (*pMsg->m_policy)
			{
				case 0: m_eExitUseType = ENUM_TYPE_EXIT_USE_NONE; break;
				case 1: m_eExitUseType = ENUM_TYPE_EXIT_USE_ONCE; break;
				case 2: m_eExitUseType = ENUM_TYPE_EXIT_USE_ALWAYS; break;				
			}

			int nTimeNotify = *pMsg->m_warning / 1800;
			switch(nTimeNotify)
			{
			case 0: m_eTimeNotifyType = ENUM_TYPE_TIME_NOTIFY_NONE; break;
			case 1: m_eTimeNotifyType = ENUM_TYPE_TIME_NOTIFY_30MIN; break;
			case 2: m_eTimeNotifyType = ENUM_TYPE_TIME_NOTIFY_1HOUR; break;
			}

			m_bEnableInitBtn = TRUE;
		}
		
	}

	
	return CUPGDlgBase::DoModal();
}