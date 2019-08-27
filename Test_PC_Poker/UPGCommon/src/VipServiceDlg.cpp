#include "stdafx.h"
#include "VipServiceDlg.h"
#include "VipCommMsg.h"
#include "GlobalBase.h"
#include "VipCloseDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define POKERVIP_VER	590		// VIP서버 버전

#define VIP_COLOR_GAME		RGB(250,200,255)
#define VIP_COLOR_LOBY		RGB(180,0,0)

CVipServiceDlg::CVipServiceDlg(CWnd* pParent /*=NULL*/)
: CUPGDlgBase(CVipServiceDlg::IDD, pParent)
{
	m_strServerName = "";
}


void CVipServiceDlg::DoDataExchange(CDataExchange* pDX)
{
	CUPGDlgBase::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_BTN_CANCEL, m_CancelBtn);
	DDX_Control(pDX, IDOK, m_CloseBtn);
	DDX_Control(pDX, IDC_BTN_SEND, m_SendBtn);
	DDX_Control(pDX, IDC_BTN_REQUEST_COUNSEL, m_RequestBtn);
	DDX_Control(pDX, IDC_CHAT_LIST, m_ChatViewEdit);
	DDX_Control(pDX, IDC_BTN_HIDE, m_HideBtn);
}


BEGIN_MESSAGE_MAP(CVipServiceDlg, CUPGDlgBase)
	//{{AFX_MSG_MAP(CVipServiceDlg)
	ON_BN_CLICKED(IDC_BTN_SEND, OnBtnSend)
	ON_BN_CLICKED(IDC_BTN_REQUEST_COUNSEL, OnBtnRequestCounsel)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBtnCancel)
	ON_BN_CLICKED(IDC_BTN_HIDE, OnBtnHide)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()	
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVipServiceDlg message handlers

BOOL CVipServiceDlg::OnInitDialog() 
{
	CUPGDlgBase::OnInitDialog();

	// TODO: Add extra initialization here

// 	int rtn = m_SockCmnd.Init(this, &NMBASE::SOCK::GSOCKMAN());
// 	if(rtn==FALSE)
// 	{
// 
// 		ShowMainMessageDlg("소켓을 생성 할 수 없습니다." );
// 		return FALSE;
// 	}
//	m_nServerId = -1;

	ZeroMemory( &m_ServiceCfg, sizeof(ST_SERVICE_ON_OFF) );

	m_nMode = MODE_REGUEST_DIALOG;

	m_CancelBtn.Init( 110, 215, ".\\common\\image\\vip\\vip_btn_cancel.bmp", 4 );
	m_CloseBtn.Init( 110, 260, ".\\common\\image\\vip\\vip_btn_close.bmp", 4 );
	m_SendBtn.Init( 236, 223, ".\\common\\image\\vip\\vip_btn_send.bmp", 4 );
	m_RequestBtn.Init( 28, 95, ".\\common\\image\\vip\\vip_btn_counsel.bmp", 4 );
	m_HideBtn.Init( 155, 181, ".\\common\\image\\vip\\vip_btn_close0.bmp", 4 );

	m_CancelBtn.ShowWindow( SW_HIDE );
	m_CloseBtn.ShowWindow( SW_HIDE );
	m_SendBtn.ShowWindow( SW_HIDE );
	m_RequestBtn.ShowWindow( SW_HIDE );
	m_HideBtn.ShowWindow( SW_HIDE );

	CRect rc( 0, 0, 217, 17 );
	rc.OffsetRect( 14, 228 );
	GetDlgItem(IDC_EDIT_SEND2)->MoveWindow( rc );
	m_nWaitingNum = 0;

	CRect rt;
	rt.SetRect( 0, 0, 244, 170 );
	rt.OffsetRect( 15, 43 );
	GetDlgItem(IDC_CHAT_LIST)->MoveWindow(&rt);

	// 채팅 뷰 리치 에디트 컨트롤 초기화
	m_ChatViewEdit.SetReadOnly(TRUE);
	m_ChatViewEdit.SetBackgroundColor(FALSE, RGB(53,60,70));
	m_ChatViewEdit.Init();

	m_ChatViewEdit.SetEventMask( m_ChatViewEdit.GetEventMask() | EN_LINK );
	((CRichEditCtrl*)GetDlgItem(IDC_CHAT_LIST))->SetEventMask(((CRichEditCtrl*)GetDlgItem(IDC_CHAT_LIST))->GetEventMask() | ENM_LINK | ENM_MOUSEEVENTS);

	m_ChatViewEdit.m_SkinVerticleScrollbar.MoveWindow( 267, 35, m_ChatViewEdit.m_SkinVerticleScrollbar.m_Thumb.x, 181, FALSE);

	m_bCounselFinished = FALSE;
	m_nLogin = 0;
	m_strServerName = "";

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CVipServiceDlg::ConnectServer()
{
	//	BOOL bResult = FALSE;

#ifdef _DEBUG
	//	m_nServerId = m_SockMan.ConnectSocket( "218.145.96.75", 27055 );
	m_strServerName = (CString)NMBASE::SOCK::GSOCKMAN().ConnectSocket("220.73.221.158", 27055 ); // 리얼에 vip상담 서버접속.
	//m_strServerName = (CString)NMBASE::SOCK::GSOCKMAN().ConnectSocket("192.168.62.230", 27055 ); // qa존에 vip 상담 서버 접속.
#else
	m_strServerName = (CString)NMBASE::SOCK::GSOCKMAN().ConnectSocket("220.73.221.158", 27055 );
#endif

	if( m_strServerName == CONNECT_FAIL )
	{
		ShowMainMessageDlg("상담서버에 접속 할 수 없습니다." );
		m_strServerName = "";
	}

	m_bCounselFinished = FALSE;
	m_nMode = MODE_REGUEST_DIALOG;
}

BOOL CVipServiceDlg::IsConnectServer()
{
	return !m_strServerName.IsEmpty();
}

BOOL CVipServiceDlg::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class

	return CUPGDlgBase::OnCommand(wParam, lParam);
}

// 0 : 
// 1 :
// 2 :
void CVipServiceDlg::ChangeMode(int nMode )
{
	//	if( nMode == m_nMode )
	m_nMode = nMode;

	m_CancelBtn.ShowWindow( SW_HIDE );
	m_CloseBtn.ShowWindow( SW_HIDE );
	m_SendBtn.ShowWindow( SW_HIDE );
	m_RequestBtn.ShowWindow( SW_HIDE );
	m_HideBtn.ShowWindow( SW_HIDE );

	switch( m_nMode )
	{
	case MODE_REGUEST_DIALOG:
		{
			GetDlgItem(IDC_BTN_REQUEST_COUNSEL)->ShowWindow( SW_SHOW );

			if( m_ServiceCfg.nType == 0 )		GetDlgItem(IDC_BTN_REQUEST_COUNSEL)->EnableWindow( TRUE );
			else								GetDlgItem(IDC_BTN_REQUEST_COUNSEL)->EnableWindow( FALSE );

			m_strBack = ".\\common\\image\\vip\\vip_back_counsel.bmp";
			NMBASE::SKINGDI::WindowResize2Bitmap(m_hWnd, m_strBack);

			m_CancelBtn.ShowWindow( SW_SHOW );

			if( m_ServiceCfg.nType == 0 )		m_RequestBtn.ShowWindow( SW_SHOW );
			else								m_RequestBtn.ShowWindow( SW_HIDE );

			GetDlgItem(IDC_EDIT_SEND2)->ShowWindow( SW_HIDE );
			GetDlgItem(IDC_CHAT_LIST)->ShowWindow( SW_HIDE );
			m_ChatViewEdit.m_SkinVerticleScrollbar.ShowWindow( SW_HIDE );

			// 			CRect rc(0, 0, 124, 20);
			// 			rc.OffsetRect(110, 215);
			// 			m_CancelBtn.MoveWindow( rc );
			m_CancelBtn.SetPos(110,215);
		}
		break;

	case MODE_WATING_DIALOG:
		{
			m_nWaitingNum = 0;
			m_strBack = ".\\common\\image\\vip\\vip_back_waiting.bmp";
			NMBASE::SKINGDI::WindowResize2Bitmap(m_hWnd, m_strBack);

			m_CancelBtn.ShowWindow( SW_SHOW );
			m_HideBtn.ShowWindow( SW_SHOW );

			GetDlgItem(IDC_EDIT_SEND2)->ShowWindow( SW_HIDE );
			GetDlgItem(IDC_CHAT_LIST)->ShowWindow( SW_HIDE );
			m_ChatViewEdit.m_SkinVerticleScrollbar.ShowWindow( SW_HIDE );

			// 			CRect rc(0, 0, 85, 30);
			// 			rc.OffsetRect(54, 206);
			// 			m_CancelBtn.MoveWindow( rc );

			m_CancelBtn.SetPos(65,181);
		}
		break;

	case MODE_COUNSEL_DIALOG:
		{
			GetDlgItem(IDC_BTN_SEND)->ShowWindow( SW_SHOW );
			//			GetDlgItem(IDC_CHAT_LIST)->ShowWindow( SW_SHOW );
			m_strBack = ".\\common\\image\\vip\\vip_back_chat.bmp";
			NMBASE::SKINGDI::WindowResize2Bitmap(m_hWnd, m_strBack);

			m_CloseBtn.ShowWindow( SW_SHOW );
			m_SendBtn.ShowWindow( SW_SHOW );

			GetDlgItem(IDC_EDIT_SEND2)->ShowWindow( SW_SHOW );
			GetDlgItem(IDC_CHAT_LIST)->ShowWindow( SW_SHOW );
			m_ChatViewEdit.m_SkinVerticleScrollbar.ShowWindow( SW_SHOW );

			m_ChatViewEdit.SetWindowText( "" );
			GetDlgItem(IDC_EDIT_SEND2)->SetWindowText( "" );

		} 
		break;
	}

	Invalidate( TRUE );
}


LONG CVipServiceDlg::OnSuccessConnect(WPARAM wParam, LPARAM lParam) 
{
	LPSTR pszSockName = (LPSTR)lParam;

	if(pszSockName == m_strServerName)
	{
		CVIPSV_HEARTBEAT msg;
		msg.Set();
		NMBASE::SOCK::GSOCKMAN().SendData( m_strServerName.GetBuffer(), msg.pData, msg.GetTotalSize() );
	}
	
	return 1;
}

LONG CVipServiceDlg::OnSocketClose(WPARAM wParam, LPARAM lParam) 
{
	LPSTR pszSockName = (LPSTR)lParam;

	// 서버와의 접속이 끊어진 경우라면
	if(pszSockName == m_strServerName)
	{
		m_nLogin = 0;
		m_strServerName = "";
		ShowWindow( SW_HIDE );	
		//	AfxMessageBox( "상담서버와 접속이 끊어졌습니다." );
	}	

	return 1;
}


LONG CVipServiceDlg::OnFailConnect(UINT wParam, LONG lParam)
{
	LPSTR pszSockName = (LPSTR)lParam;

	// 마스터 서버와 접속이 실패한 경우라면
	if(m_strServerName == pszSockName)
	{
		m_nLogin = 2;
		//AfxMessageBox( "상담서버 접속에 실패하였습니다." );

		//	memcpy( &m_ServiceCfg, msg.stSerceInfo, sizeof(ST_SERVICE_ON_OFF) );
		m_ServiceCfg.nType = 1;
		ShowWindow( SW_SHOW );
		ChangeMode( MODE_REGUEST_DIALOG );
	}

	//	ShowWindow( SW_HIDE );
	return 1;

}



void CVipServiceDlg::DisconnectServer()
{	
	NMBASE::SOCK::GSOCKMAN().CloseDataSocket(m_strServerName.GetBuffer());
	m_strServerName = "";

	m_nLogin = 0;
}


void CVipServiceDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	// TODO: Add your message handler code here
	CDC cdc;
	cdc.CreateCompatibleDC(&dc);

	NMBASE::SKINGDI::CMyBitmap Back;

	Back.LoadBitmapFile(m_strBack);	

	CBitmap *pOldBmp = cdc.SelectObject(&Back);

	if( m_nMode == MODE_REGUEST_DIALOG )
	{
		DrawRequest( &cdc );
	}
	else if( m_nMode == MODE_WATING_DIALOG )
	{
		DrawWaiting( &cdc );
	}
	else if( m_nMode == MODE_COUNSEL_DIALOG )
	{
		DrawCounsel( &cdc );
	}

	CRect rect;
	GetClientRect(&rect);
	dc.BitBlt(0, 0, rect.right, rect.bottom, &cdc, 0, 0, SRCCOPY);

	cdc.SelectObject(pOldBmp);
	Back.DeleteObject();


	// Do not call CUPGDlgBase::OnPaint() for painting messages
}

void CVipServiceDlg::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	PostMessage( WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM( point.x, point.y));

	CUPGDlgBase::OnLButtonDown(nFlags, point);
}

void CVipServiceDlg::OnBtnRequestCounsel() 
{
	// TODO: Add your control notification handler code here

	CVIPSV_REQUEST_COUNSEL msg;
	msg.Set();
	NMBASE::SOCK::GSOCKMAN().SendData( m_strServerName.GetBuffer(), msg.pData, msg.GetTotalSize() );
	//	ChangeMode( MODE_COUNSEL_DIALOG );
	//	Invalidate( TRUE );
}

void CVipServiceDlg::OnOK() 
{
	if( m_bCounselFinished == FALSE)
	{
		CVipCloseDlg dlg;
		if( dlg.DoModal() == IDCANCEL )
			return;
	}

	DisconnectServer();

	m_bCounselFinished = FALSE;

	CUPGDlgBase::OnOK();
}

void CVipServiceDlg::OnCancel()
{
	if( m_bCounselFinished == FALSE)
	{
		CVipCloseDlg dlg;
		if( dlg.DoModal() == IDCANCEL )
			return;
	}
	DisconnectServer();

	m_bCounselFinished = FALSE;
	CUPGDlgBase::OnCancel();
}

// 창 감추기
void CVipServiceDlg::OnBtnHide() 
{
	ShowWindow(SW_HIDE);
}


void CVipServiceDlg::OnBtnSend() 
{
	// TODO: Add your control notification handler code here

	CString strTmp;
	GetDlgItem(IDC_EDIT_SEND2)->GetWindowText( strTmp );
	GetDlgItem(IDC_EDIT_SEND2)->SetWindowText( "" );

	// 	CString strChat;
	// 	strChat.Format( "%s> %s\n", GM().GetMyInfo()->UI.ID, strTmp );
	// 	m_ChatViewEdit.AddText( &strChat );
	CVIPSV_CHATDATA msg;
	msg.Set( GM().GetMyInfo()->UI.ID, (char*)(LPCTSTR)strTmp, 0 );
	NMBASE::SOCK::GSOCKMAN().SendData( m_strServerName.GetBuffer(), msg.pData, msg.GetTotalSize() );
}

void CVipServiceDlg::OnBtnCancel() 
{
	if( m_nMode == MODE_WATING_DIALOG )
	{
		CVipCloseDlg dlg;
		if( dlg.DoModal() == IDCANCEL )
			return;

		CVIPSV_WAITING_CANCEL msg;
		msg.Set();
		NMBASE::SOCK::GSOCKMAN().SendData( m_strServerName.GetBuffer(), msg.pData, msg.GetTotalSize() );
	}

	DisconnectServer();

	CUPGDlgBase::OnOK();
}

void CVipServiceDlg::DrawRequest( CDC *pDC )
{

}

void CVipServiceDlg::DrawWaiting( CDC *pDC )
{
	CString strTmp;
	strTmp.Format( "%d명", m_nWaitingNum );

	pDC->SelectObject( NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_13_BOLD) );
	pDC->SetTextColor( RGB(255, 255, 255) );
	pDC->SetBkMode(TRANSPARENT);
	CRect rect;
	rect.SetRect( 0,0, 50, 21 );
	rect.OffsetRect( 183, 59 );
	pDC->DrawText( strTmp, rect, DT_CENTER |DT_VCENTER|DT_SINGLELINE);
}

void CVipServiceDlg::DrawCounsel( CDC *pDC )
{
}

BOOL CVipServiceDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message == WM_KEYDOWN )
	{
		if( GetFocus()->GetDlgCtrlID() == IDC_EDIT_SEND2 )
		{
			if( pMsg->wParam == VK_RETURN )
			{
				CString strTmp;
				GetDlgItem(IDC_EDIT_SEND2)->GetWindowText( strTmp );
				GetDlgItem(IDC_EDIT_SEND2)->SetWindowText( "" );

				// 				CString strChat;
				// 				strChat.Format( "%s> %s\n", GM().GetMyInfo()->UI.ID, strTmp );
				// 				m_ChatViewEdit.AddText( &strChat );
				CVIPSV_CHATDATA msg;
				msg.Set( GM().GetMyInfo()->UI.ID, (char*)(LPCTSTR)strTmp, 0 );
				NMBASE::SOCK::GSOCKMAN().SendData( m_strServerName.GetBuffer(), msg.pData, msg.GetTotalSize() );
				//m_SendEdit.SendMessage( pMsg->message, pMsg->wParam, pMsg->lParam );
				return 1;
			}
		}
	}
	return CUPGDlgBase::PreTranslateMessage( pMsg );
}

HBRUSH CVipServiceDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CUPGDlgBase::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO: Change any attributes of the DC here

	if( pWnd->GetDlgCtrlID() == IDC_EDIT_SEND2 )
	{
		pDC->SetTextColor(RGB(255, 255, 255));
		pDC->SetBkColor(RGB(53,60,70));
		return CreateSolidBrush(RGB(53,60,70));
	}

	// TODO: Return a different brush if the default is not desired
	return hbr;
}



LONG CVipServiceDlg::OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData)
{
	CString strSockName = pName;

	if(m_strServerName != strSockName)
		return 1;

	//	PutConsoleSignalString(Signal);

	switch(Signal)
	{
	case VIPSV_CHECK_VERSION:
		{
			CVIPSV_CHECK_VERSION msg;
			msg.Get(lpData, TotSize);
			
			int ver = *msg.pnVersion;

			// 버전이 틀리면 연결종료
			if(ver != POKERVIP_VER)
			{
				ShowMainMessageDlg("상담 서버와의 버전이 일치하지 않아 접속을 종료합니다.");
				DisconnectServer();
				ShowWindow( SW_HIDE );
				break;
			}

			// 로그인 요청
			CVIPSV_ASK_LOGIN askMsg;
			int nRoomNumber = 0;

			if( GM().GetCurrentWhere() == IDX_GAMEWHERE_GAME )
				nRoomNumber = g_RI.RoomNum;

			ST_VIPSERVER_LOGIN_INFO stInfo;
			ZeroMemory( &stInfo, sizeof(ST_VIPSERVER_LOGIN_INFO) );
			stInfo.nType = 0;					// 0 : 서버, 1 : 어드민
			stInfo.nGameCode = (int)GM().GetCurrentGameCode();
			stInfo.nPremCode = GM().GetMyInfo()->UI.PremMenuNo;				// PREM_LEADERSVIP 인경우만 로그인 허용
			stInfo.nGroupNum = g_NowChanNo;
			stInfo.nChannelNum = 0;
			stInfo.nRoomNum = nRoomNumber;
			strcpy( stInfo.szID, GM().GetMyInfo()->UI.ID );
			//stInfo.szPass[20];

			askMsg.Set( &stInfo );

			NMBASE::SOCK::GSOCKMAN().SendData( m_strServerName.GetBuffer(), askMsg.pData, askMsg.GetTotalSize() );

		} 
		break;

		// 로그인 허용
	case VIPSV_ACCEPT_LOGIN:
		{
			CVIPSV_ACCEPT_LOGIN msg;
			msg.Get(lpData, TotSize);
		}
		break;

	case VIPSV_REFUSE_LOGIN:
		{
			CVIPSV_REFUSE_LOGIN msg;
			msg.Get(lpData, TotSize);			

			int nType = *msg.nType;

			switch( nType )
			{
			case 1:
				ShowMainMessageDlg("VIP 회원만 접속할 수 있습니다." );
				DisconnectServer();
				ShowWindow( SW_HIDE );
				break;

			default:
				//				AfxMessageBox( "NONE" );
				DisconnectServer();
				ShowWindow( SW_HIDE );
				break;
			}
		}
		break;

	case VIPSV_SERVICE_CFG:
		{
			CVIPSV_SERVICE_CFG msg;
			msg.Get( lpData, TotSize );			

			memcpy( &m_ServiceCfg, msg.stSerceInfo, sizeof(ST_SERVICE_ON_OFF) );

			m_nLogin = 1;

			ShowWindow( SW_SHOW );
			ChangeMode( MODE_REGUEST_DIALOG );

		}
		break;

	case VIPSV_WAITING_COUNSEL:
		{
			CVIPSV_WAITING_COUNSEL msg;
			msg.Get( lpData, TotSize );			

			if( MODE_COUNSEL_DIALOG == m_nMode )
				break;

			ChangeMode( MODE_WATING_DIALOG );
			m_nWaitingNum = *msg.nCount;

			if( IsWindowVisible() == FALSE )
			{

				CString strTmp;
				strTmp.Format( "VIP상담 : 현재 대기인원은 %d명입니다.\n", m_nWaitingNum );

				//%%% 추후 수정
				/*				if( g_Where == WH_GAME )
				{
				// 게임방 채팅창에 뿌려줌
				g_ChatWinMan->m_pChatting->AddText( &strTmp, VIP_COLOR_GAME );

				}
				else if( g_Where == WH_LOBY )
				{
				// 대기실 채팅창에 부려줌
				AddLobbyChatViewEdit( strTmp.GetString(), VIP_COLOR_LOBY );
				}
				*/
			}

		}
		break;

	case VIPSV_CHATDATA:
		{
			CVIPSV_CHATDATA msg;
			msg.Get(lpData, TotSize);			

			CString chat;
			chat.Append((LPCTSTR)msg.StrChat, *msg.l_StrChat);

			//- Bad Filter (상담채팅에 필터 적용)
			const int SIZE_CHAT_BUF = 5000;
			char szTChat[SIZE_CHAT_BUF] = {0,};
			NMBASE::UTIL::NMIsBadToRep( (char*)chat.GetString(), szTChat, SIZE_CHAT_BUF-1 );


			CString strChat;
			strChat.Format( "%s> %s\n", msg.ID, szTChat );
			m_ChatViewEdit.AddText( &strChat , RGB(197, 199, 198) );
		} 
		break;

	case VIPSV_CONNECT_COUNSEL_FAIL:
		{
			CVIPSV_CONNECT_COUNSEL_FAIL msg;
			msg.Get(lpData, TotSize );
			
			if( msg.pInfo->bCounseling == TRUE )
			{
				DisconnectServer();
				ShowWindow( SW_HIDE );
				ShowMainMessageDlg("상담은 하루 세번까지 가능합니다." );
			}
		}
		break;

	case VIPSV_CONNECT_COUNSEL:
		{
			CVIPSV_CONNECT_COUNSEL msg;
			msg.Get(lpData, TotSize );
			
			if( strcmp(msg.pInfo->szID, GM().GetMyInfo()->UI.ID) != 0 )
				break;

			if( IsWindowVisible() == FALSE )
				ShowWindow( SW_SHOW );

			ChangeMode( MODE_COUNSEL_DIALOG );
		}
		break;

	case VIPSV_EXIT_COUNSEL:
		{
			CVIPSV_EXIT_COUNSEL msg;
			msg.Get(lpData, TotSize );
			

			CString chat;
			chat.Append((LPCTSTR)msg.StrChat, *msg.l_StrChat);

			m_ChatViewEdit.AddText( &chat, RGB(99, 122, 146) );

			m_bCounselFinished = TRUE;

		} 
		break;

	default:
		break;	
	}

	return 1;
}
